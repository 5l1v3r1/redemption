/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  Product name: redemption, a FLOSS RDP proxy
  Copyright (C) Wallix 2010
  Author(s): Christophe Grosjean, Javier Caverni, Xavier Dunat,
             Raphael Zhou, Meng Tan

  Manage Modules Life cycle : creation, destruction and chaining
  find out the next module to run from context reading
*/

#include "capture/fdx_capture.hpp"
#include "acl/connect_to_target_host.hpp"
#include "mod/file_validator_service.hpp"
#include "utils/sugar/scope_exit.hpp"
#include "utils/sugar/unique_fd.hpp"
#include "utils/netutils.hpp"
#include "utils/parse_primary_drawing_orders.hpp"
#include "mod/rdp/params/rdp_session_probe_params.hpp"
#include "mod/rdp/params/rdp_application_params.hpp"
#include "mod/metrics_hmac.hpp"
#include "mod/rdp/rdp.hpp"
#include "mod/rdp/rdp_params.hpp"
#include "mod/rdp/rdp_verbose.hpp"
#include "acl/module_manager/create_module_rdp.hpp"
#include "utils/sugar/bytes_view.hpp"
#include "utils/genfstat.hpp"
#include "acl/mod_pack.hpp"
#include "transport/failure_simulation_socket_transport.hpp"

namespace
{
    void file_verification_error(
        FrontAPI& front,
        timeval now,
        AuthApi & sesman,
        chars_view up_target_name,
        chars_view down_target_name,
        chars_view msg)
    {
        for (auto&& service : {up_target_name, down_target_name}) {
            if (not service.empty()) {
                const KVList data = {
                        KVLog("icap_service"_av, service),
                        KVLog("status"_av, msg),
                };
                sesman.log6(LogId::FILE_VERIFICATION_ERROR, data);
                front.session_update(now, LogId::FILE_VERIFICATION_ERROR, data);
            }
        }
    }
}

struct RdpData
{
    struct ModMetrics : Metrics
    {
        using Metrics::Metrics;

        RDPMetrics protocol_metrics{*this};
    };

    struct FileValidator
    {
        struct FileValidatorTransport : FileTransport
        {
            using FileTransport::FileTransport;

            size_t do_partial_read(uint8_t * buffer, size_t len) override
            {
                size_t r = FileTransport::do_partial_read(buffer, len);
                if (r == 0) {
                    LOG(LOG_ERR, "ModuleManager::create_mod_rdp: ModRDPWithMetrics::FileValidator::do_partial_read: No data read!");
                    Error error(ERR_TRANSPORT_NO_MORE_DATA, errno);
                    this->notify_error(error);
                    throw error;
                }
                return r;
            }
        };

        struct CtxError
        {
            AuthApi & sesman;
            std::string up_target_name;
            std::string down_target_name;
            FrontAPI& front;
        };

        CtxError ctx_error;
        FileValidatorTransport trans;
        // TODO wait result (add delay)
        FileValidatorService service;
        TimeBase & time_base;
        EventContainer & events;
        mod_rdp * mod = nullptr;

        FileValidator(unique_fd&& fd, CtxError&& ctx_error, TimeBase & time_base, EventContainer & events)
        : ctx_error(std::move(ctx_error))
        , trans(std::move(fd), [this](const Error & err){
            file_verification_error(
                this->ctx_error.front,
                this->time_base.get_current_time(),
                this->ctx_error.sesman,
                this->ctx_error.up_target_name,
                this->ctx_error.down_target_name,
                err.errmsg()
            );
            return err;
        })
        , service(this->trans)
        , time_base(time_base)
        , events(events)
        {
            this->events.create_event_fd_timeout(
                "File Validator Event", this,
                this->trans.get_fd(),std::chrono::seconds(3600),
                this->time_base.get_current_time()+std::chrono::seconds(3600),
                [this](Event&/*event*/)
                {
                    if (this->mod){
                        this->mod->DLP_antivirus_check_channels_files();
                    }
                },
                [](Event&event)
                {
                    event.alarm.set_timeout(event.alarm.now+std::chrono::seconds(3600));
                });
        }

        ~FileValidator()
        {
            try {
                this->service.send_close_session();
            }
            catch (...) {
            }
            this->events.end_of_lifespan(this);
        }
    };

    std::unique_ptr<ModMetrics> metrics;
    int metrics_timer = 0;

    void set_metrics_timer(std::chrono::seconds log_interval)
    {
        this->metrics_timer = this->events.create_event_timeout(
            "RDP Metrics Timer", this,
            this->time_base.get_current_time() + log_interval,
            [this,log_interval](Event&event)
            {
                event.alarm.reset_timeout(event.alarm.now + log_interval);
                this->metrics->log(event.alarm.now);
            });
    }

    TimeBase & time_base;
    EventContainer & events;

    std::unique_ptr<FileValidator> file_validator;
    std::unique_ptr<FdxCapture> fdx_capture;
    Fstat fstat;

    RdpData(TimeBase & time_base, EventContainer & events)
     : time_base(time_base)
     , events(events)
    {}

    ~RdpData()
    {
        this->events.end_of_lifespan(this);
    }
};



class ModRDPWithSocketAndMetrics final : public mod_api
{
public:
    std::unique_ptr<SocketTransport> socket_transport_ptr;
    ModRdpFactory rdp_factory;
    AuthApi & sesman;
    mod_rdp mod;

    FdxCapture* get_fdx_capture(Random & gen, Inifile & ini, CryptoContext & cctx)
    {
        if (!this->rdp_data.fdx_capture) {
            LOG(LOG_INFO, "Enable clipboard file storage");
            int  const groupid = ini.get<cfg::video::capture_groupid>();
            auto const& session_id = ini.get<cfg::context::session_id>();
            auto const& subdir = ini.get<cfg::capture::record_subdirectory>();
            auto const& record_dir = ini.get<cfg::video::record_path>();
            auto const& hash_dir = ini.get<cfg::video::hash_path>();
            auto const& filebase = ini.get<cfg::capture::record_filebase>();

            this->rdp_data.fdx_capture = std::make_unique<FdxCapture>(
                str_concat(record_dir.as_string(), subdir),
                str_concat(hash_dir.as_string(), subdir),
                filebase,
                session_id, groupid, cctx, gen, this->rdp_data.fstat,
                /* TODO should be a log (siem?)*/
                [](const Error & /*error*/){});

            ini.set_acl<cfg::capture::fdx_path>(this->rdp_data.fdx_capture->get_fdx_path());
        }

        return this->rdp_data.fdx_capture.get();
    }

    RdpData rdp_data;

    ModRdpFactory& get_rdp_factory() noexcept
    {
        return this->rdp_factory;
    }

private:
    ModWrapper & mod_wrapper;
    Inifile & ini;

public:
    ModRDPWithSocketAndMetrics(ModWrapper & mod_wrapper, Inifile & ini,
        const char * name, unique_fd sck, uint32_t verbose
      , std::string * error_message
      , TimeBase& time_base
      , EventContainer & events
      , AuthApi & sesman
      , gdi::GraphicApi & gd
      , FrontAPI & front
      , const ClientInfo & info
      , RedirectionInfo & redir_info
      , Random & gen
      , TimeObj & timeobj
      , const ChannelsAuthorizations channels_authorizations
      , const ModRDPParams & mod_rdp_params
      , const TLSClientParams & tls_client_params
      , LicenseApi & license_store
      , ModRdpVariables vars
      , [[maybe_unused]] RDPMetrics * metrics
      , [[maybe_unused]] FileValidatorService * file_validator_service
      , ModRdpUseFailureSimulationSocketTransport use_failure_simulation_socket_transport
        )
    : socket_transport_ptr([](  ModRdpUseFailureSimulationSocketTransport use_failure_simulation_socket_transport
                              , const char * name, unique_fd sck
                              , const char *ip_address, int port, std::chrono::milliseconds recv_timeout
                              , uint32_t verbose, std::string * error_message) -> SocketTransport*
        {
            if (ModRdpUseFailureSimulationSocketTransport::Off == use_failure_simulation_socket_transport)
            {
                return new SocketTransport(  name, std::move(sck)
                                           , ip_address
                                           , port
                                           , recv_timeout
                                           , to_verbose_flags(verbose)
                                           , error_message);
            }

            LOG(LOG_WARNING, "ModRDPWithSocketAndMetrics::ModRDPWithSocketAndMetrics: Mod_rdp use Failure Simulation Socket Transport (mode=%s)",
                (  ModRdpUseFailureSimulationSocketTransport::SimulateErrorRead == use_failure_simulation_socket_transport
                 ? "SimulateErrorRead" : "SimulateErrorWrite"));

            return new FailureSimulationSocketTransport(
                  ModRdpUseFailureSimulationSocketTransport::SimulateErrorRead == use_failure_simulation_socket_transport
                , name, std::move(sck)
                , ip_address
                , port
                , recv_timeout
                , to_verbose_flags(verbose)
                , error_message);
        }(  use_failure_simulation_socket_transport, name, std::move(sck)
          , ini.get<cfg::context::target_host>().c_str()
          , ini.get<cfg::context::target_port>()
          , std::chrono::milliseconds(ini.get<cfg::globals::mod_recv_timeout>())
          , verbose, error_message))
    , sesman(sesman)
    , mod(*this->socket_transport_ptr, time_base, mod_wrapper, events
        , sesman,gd, front, info, redir_info, gen, timeobj
        , channels_authorizations, mod_rdp_params, tls_client_params
        , license_store
        , vars, metrics, file_validator_service, this->get_rdp_factory())
    , rdp_data(time_base, events)
    , mod_wrapper(mod_wrapper)
    , ini(ini)
    {
        this->mod_wrapper.target_info_is_shown = false;
        this->sesman.begin_dispatch_to_capture();
//        this->mod_wrapper.set_mod_transport(&this->socket_transport);
    }

    std::string module_name() override {return "RDP Mod With Socket And Metrics";}

    ~ModRDPWithSocketAndMetrics()
    {
//        log_proxy::target_disconnection(this->sesman.get_auth_error_message())
//        this->mod_wrapper.set_mod_transport(nullptr);
        log_proxy::target_disconnection(
            this->ini.template get<cfg::context::auth_error_message>().c_str());
        this->sesman.end_dispatch_to_capture();
    }

    // from RdpInput

    void rdp_gdi_up_and_running() override {}

    void rdp_gdi_down() override {}

    void rdp_input_scancode(long param1, long param2, long param3, long param4, Keymap2 * keymap) override
    {
        this->mod.rdp_input_scancode(param1, param2, param3, param4, keymap);
    }

    // from RdpInput
    void rdp_input_mouse(int device_flags, int x, int y, Keymap2 * keymap) override
    {
        this->mod.rdp_input_mouse(device_flags, x, y, keymap);
    }

    // from RdpInput
    void rdp_input_unicode(uint16_t unicode, uint16_t flag) override {
        this->mod.rdp_input_unicode(unicode, flag);
    }

    // from RdpInput
    void rdp_input_invalidate(const Rect r) override
    {
        this->mod.rdp_input_invalidate(r);
    }

    // from RdpInput
    void rdp_input_synchronize(uint32_t time, uint16_t device_flags, int16_t param1, int16_t param2) override
    {
        return this->mod.rdp_input_synchronize(time, device_flags, param1, param2);
    }

    void refresh(Rect clip) override
    {
        return this->mod.refresh(clip);
    }

    // from mod_api
    [[nodiscard]] bool is_up_and_running() const override { return false; }

    // from mod_api
    // support auto-reconnection
    bool is_auto_reconnectable() const override {
        return this->mod.is_auto_reconnectable();
    }

    bool server_error_encountered() const override {
        return this->mod.server_error_encountered();
    }

    // from mod_api
    void disconnect() override
    {
        return this->mod.disconnect();
    }

    // from mod_api
    void display_osd_message(std::string const & message) override
    {
        this->mod_wrapper.display_osd_message(message);
    }

    // from mod_api
    void move_size_widget(int16_t left, int16_t top, uint16_t width, uint16_t height) override
    {
        return this->mod.move_size_widget(left, top, width, height);
    }

    // from mod_api
    void send_input(int time, int message_type, int device_flags, int param1, int param2) override
    {
        return this->mod.send_input(time, message_type, device_flags, param1, param2);
    }

    // from mod_api
    [[nodiscard]] Dimension get_dim() const override
    {
        return this->mod.get_dim();
    }

    // from mod_api
    void log_metrics() override
    {
        return this->mod.log_metrics();
    }

    // from mod_api
    void DLP_antivirus_check_channels_files() override
    {
        return this->mod.DLP_antivirus_check_channels_files();
    }

    void send_to_mod_channel(CHANNELS::ChannelNameId front_channel_name, InStream & chunk, std::size_t length, uint32_t flags) override
    {
        this->mod.send_to_mod_channel(front_channel_name, chunk, length, flags);
    }

    void create_shadow_session(const char * userdata, const char * type) override
    {
        this->mod.create_shadow_session(userdata, type);
    }

    void send_auth_channel_data(const char * data) override
    {
        this->mod.send_auth_channel_data(data);
    }

    void send_checkout_channel_data(const char * data) override
    {
        this->mod.send_checkout_channel_data(data);
    }
};

inline static ModRdpSessionProbeParams get_session_probe_params(Inifile & ini)
{
    ModRdpSessionProbeParams spp;
    spp.enable_session_probe = ini.get<cfg::mod_rdp::enable_session_probe>();
    spp.enable_launch_mask = ini.get<cfg::mod_rdp::session_probe_enable_launch_mask>();
    spp.used_clipboard_based_launcher = ini.get<cfg::mod_rdp::session_probe_use_clipboard_based_launcher>();
    spp.start_launch_timeout_timer_only_after_logon = ini.get<cfg::mod_rdp::session_probe_start_launch_timeout_timer_only_after_logon>();
    spp.vc_params.effective_launch_timeout
        = ini.get<cfg::mod_rdp::session_probe_start_launch_timeout_timer_only_after_logon>()
        ? ((ini.get<cfg::mod_rdp::session_probe_on_launch_failure>()
                == SessionProbeOnLaunchFailure::disconnect_user)
            ? ini.get<cfg::mod_rdp::session_probe_launch_timeout>()
            : ini.get<cfg::mod_rdp::session_probe_launch_fallback_timeout>())
        : std::chrono::milliseconds::zero();
    spp.vc_params.on_launch_failure = ini.get<cfg::mod_rdp::session_probe_on_launch_failure>();
    spp.vc_params.keepalive_timeout = ini.get<cfg::mod_rdp::session_probe_keepalive_timeout>();
    spp.vc_params.on_keepalive_timeout  =
        ini.get<cfg::mod_rdp::session_probe_on_keepalive_timeout>();
    spp.vc_params.end_disconnected_session =
        ini.get<cfg::mod_rdp::session_probe_end_disconnected_session>();
    spp.customize_executable_name =
        ini.get<cfg::mod_rdp::session_probe_customize_executable_name>();
    spp.vc_params.disconnected_application_limit =
        ini.get<cfg::mod_rdp::session_probe_disconnected_application_limit>();
    spp.vc_params.disconnected_session_limit =
        ini.get<cfg::mod_rdp::session_probe_disconnected_session_limit>();
    spp.vc_params.idle_session_limit =
        ini.get<cfg::mod_rdp::session_probe_idle_session_limit>();
    spp.exe_or_file = ini.get<cfg::mod_rdp::session_probe_exe_or_file>();
    spp.arguments = ini.get<cfg::mod_rdp::session_probe_arguments>();
    spp.vc_params.launcher_abort_delay =
        ini.get<cfg::mod_rdp::session_probe_launcher_abort_delay>();
    spp.clipboard_based_launcher.clipboard_initialization_delay_ms =
        ini.get<cfg::mod_rdp::session_probe_clipboard_based_launcher_clipboard_initialization_delay>();
    spp.clipboard_based_launcher.start_delay_ms =
        ini.get<cfg::mod_rdp::session_probe_clipboard_based_launcher_start_delay>();
    spp.clipboard_based_launcher.long_delay_ms =
         ini.get<cfg::mod_rdp::session_probe_clipboard_based_launcher_long_delay>();
    spp.clipboard_based_launcher.short_delay_ms =
        ini.get<cfg::mod_rdp::session_probe_clipboard_based_launcher_short_delay>();
    spp.vc_params.end_of_session_check_delay_time = ini.get<cfg::mod_rdp::session_probe_end_of_session_check_delay_time>();
    spp.vc_params.ignore_ui_less_processes_during_end_of_session_check =
        ini.get<cfg::mod_rdp::session_probe_ignore_ui_less_processes_during_end_of_session_check>();
    spp.vc_params.update_disabled_features                             = ini.get<cfg::mod_rdp::session_probe_update_disabled_features>();
    spp.vc_params.childless_window_as_unidentified_input_field =
        ini.get<cfg::mod_rdp::session_probe_childless_window_as_unidentified_input_field>();
    spp.is_public_session = ini.get<cfg::mod_rdp::session_probe_public_session>();
    spp.vc_params.session_shadowing_support = ini.get<cfg::mod_rdp::session_shadowing_support>();
    spp.vc_params.on_account_manipulation =
        ini.get<cfg::mod_rdp::session_probe_on_account_manipulation>();
    spp.vc_params.extra_system_processes =
        ExtraSystemProcesses(
            ini.get<cfg::context::session_probe_extra_system_processes>().c_str());
    spp.vc_params.outbound_connection_monitor_rules =
        OutboundConnectionMonitorRules(
            ini.get<cfg::context::session_probe_outbound_connection_monitoring_rules>().c_str());
    spp.vc_params.process_monitor_rules =
        ProcessMonitorRules(ini.get<cfg::context::session_probe_process_monitoring_rules>().c_str());
    spp.vc_params.windows_of_these_applications_as_unidentified_input_field = ExtraSystemProcesses(
        ini.get<cfg::context::session_probe_windows_of_these_applications_as_unidentified_input_field>().c_str());
    spp.vc_params.enable_log = ini.get<cfg::mod_rdp::session_probe_enable_log>();
    spp.vc_params.enable_log_rotation = ini.get<cfg::mod_rdp::session_probe_enable_log_rotation>();
    spp.vc_params.log_level = ini.get<cfg::mod_rdp::session_probe_log_level>();
    spp.vc_params.allow_multiple_handshake = ini.get<cfg::mod_rdp::session_probe_allow_multiple_handshake>();
    spp.vc_params.enable_crash_dump = ini.get<cfg::mod_rdp::session_probe_enable_crash_dump>();
    spp.vc_params.handle_usage_limit = ini.get<cfg::mod_rdp::session_probe_handle_usage_limit>();
    spp.vc_params.memory_usage_limit = ini.get<cfg::mod_rdp::session_probe_memory_usage_limit>();
    spp.vc_params.disabled_features = ini.get<cfg::mod_rdp::session_probe_disabled_features>();
    spp.vc_params.bestsafe_integration = ini.get<cfg::mod_rdp::session_probe_bestsafe_integration>();
    spp.used_to_launch_remote_program = ini.get<cfg::context::use_session_probe_to_launch_remote_program>();
    spp.fix_too_long_cookie = ini.get<cfg::mod_rdp::experimental_fix_too_long_cookie>();

    spp.vc_params.at_end_of_session_freeze_connection_and_wait =
        ini.get<cfg::mod_rdp::session_probe_at_end_of_session_freeze_connection_and_wait>();
    return spp;
}

inline static ApplicationParams get_rdp_application_params(Inifile & ini)
{
    ApplicationParams ap;
    ap.alternate_shell = ini.get<cfg::mod_rdp::alternate_shell>().c_str();
    ap.shell_arguments = ini.get<cfg::mod_rdp::shell_arguments>().c_str();
    ap.shell_working_dir = ini.get<cfg::mod_rdp::shell_working_directory>().c_str();
    ap.use_client_provided_alternate_shell = ini.get<cfg::mod_rdp::use_client_provided_alternate_shell>();
    ap.target_application_account = ini.get<cfg::globals::target_application_account>().c_str();
    ap.target_application_password = ini.get<cfg::globals::target_application_password>().c_str();
    ap.primary_user_id = ini.get<cfg::globals::primary_user_id>().c_str();
    ap.target_application = ini.get<cfg::globals::target_application>().c_str();
    return ap;
}


ModPack create_mod_rdp(ModWrapper & mod_wrapper,
    Inifile& ini, gdi::GraphicApi & drawable, FrontAPI& front, ClientInfo client_info /* /!\ modified */,
    ClientExecute& rail_client_execute, Keymap2::KeyFlags key_flags,
    Font & glyphs,
    Theme & theme,
    TimeBase & time_base,
    EventContainer& events,
    AuthApi & sesman,
    LicenseApi & file_system_license_store,
    Random & gen,
    TimeObj & timeobj,
    CryptoContext & cctx,
    std::array<uint8_t, 28>& server_auto_reconnect_packet)
{
    switch (ini.get<cfg::mod_rdp::mode_console>()) {
        case RdpModeConsole::force:
            client_info.console_session = true;
            LOG(LOG_INFO, "Session::mode console : force");
            break;
        case RdpModeConsole::forbid:
            client_info.console_session = false;
            LOG(LOG_INFO, "Session::mode console : forbid");
            break;
        case RdpModeConsole::allow:
            break;
    }

    // BEGIN READ PROXY_OPT
    std::string allow = ini.get<cfg::mod_rdp::allow_channels>();
    std::string deny = ini.get<cfg::mod_rdp::deny_channels>();

    if (ini.get<cfg::globals::enable_wab_integration>()) {
        auto result = update_authorized_channels(allow, deny, ini.get<cfg::context::proxy_opt>());
        allow = result.first;
        deny = result.second;
    }
    ChannelsAuthorizations channels_authorizations(allow, deny);
    // END READ PROXY_OPT

    const bool smartcard_passthrough = ini.get<cfg::mod_rdp::force_smartcard_authentication>();

    ini.set<cfg::context::close_box_extra_message>("");
    ModRDPParams mod_rdp_params(
        (smartcard_passthrough ? "" : ini.get<cfg::globals::target_user>().c_str())
      , (smartcard_passthrough ? "" : ini.get<cfg::context::target_password>().c_str())
      , ini.get<cfg::context::target_host>().c_str()
      , "0.0.0.0"   // client ip is silenced
      , key_flags
      , glyphs
      , theme
      , server_auto_reconnect_packet
      , ini.get_mutable_ref<cfg::context::close_box_extra_message>()
      , to_verbose_flags(ini.get<cfg::debug::mod_rdp>())
      //, RDPVerbose::basic_trace4 | RDPVerbose::basic_trace3 | RDPVerbose::basic_trace7 | RDPVerbose::basic_trace
    );

    SCOPE_EXIT(ini.set<cfg::context::perform_automatic_reconnection>(false));
    mod_rdp_params.perform_automatic_reconnection = ini.get<cfg::context::perform_automatic_reconnection>();
    mod_rdp_params.device_id = ini.get<cfg::globals::device_id>().c_str();

    //mod_rdp_params.enable_tls                          = true;
    TLSClientParams tls_client_params;
    tls_client_params.tls_min_level                       = ini.get<cfg::mod_rdp::tls_min_level>();
    tls_client_params.tls_max_level                       = ini.get<cfg::mod_rdp::tls_max_level>();
    tls_client_params.show_common_cipher_list             = ini.get<cfg::mod_rdp::show_common_cipher_list>();
    tls_client_params.cipher_string                       = ini.get<cfg::mod_rdp::cipher_string>();

    if (!mod_rdp_params.target_password[0]) {
        mod_rdp_params.enable_nla                      = false;
    }
    else {
        mod_rdp_params.enable_nla                      = ini.get<cfg::mod_rdp::enable_nla>();
    }
    mod_rdp_params.enable_krb                          = ini.get<cfg::mod_rdp::enable_kerberos>();
    mod_rdp_params.enable_fastpath                     = ini.get<cfg::mod_rdp::fast_path>();
    //mod_rdp_params.enable_new_pointer                  = true;
    mod_rdp_params.enable_glyph_cache                  = ini.get<cfg::globals::glyph_cache>();

    mod_rdp_params.clipboard_params.disable_log_syslog        = bool(ini.get<cfg::video::disable_clipboard_log>() & ClipboardLogFlags::syslog);
    mod_rdp_params.file_system_params.disable_log_syslog      = bool(ini.get<cfg::video::disable_file_system_log>() & FileSystemLogFlags::syslog);

    mod_rdp_params.session_probe_params = get_session_probe_params(ini);

    mod_rdp_params.ignore_auth_channel                 = ini.get<cfg::mod_rdp::ignore_auth_channel>();
    mod_rdp_params.auth_channel                        = CHANNELS::ChannelNameId(ini.get<cfg::mod_rdp::auth_channel>());
    mod_rdp_params.checkout_channel                    = CHANNELS::ChannelNameId(ini.get<cfg::mod_rdp::checkout_channel>());
    mod_rdp_params.application_params.alternate_shell                     = ini.get<cfg::mod_rdp::alternate_shell>().c_str();
    mod_rdp_params.application_params.shell_arguments                     = ini.get<cfg::mod_rdp::shell_arguments>().c_str();
    mod_rdp_params.application_params.shell_working_dir                   = ini.get<cfg::mod_rdp::shell_working_directory>().c_str();
    mod_rdp_params.application_params.use_client_provided_alternate_shell = ini.get<cfg::mod_rdp::use_client_provided_alternate_shell>();
    mod_rdp_params.application_params.target_application_account          = ini.get<cfg::globals::target_application_account>().c_str();
    mod_rdp_params.application_params.target_application_password         = ini.get<cfg::globals::target_application_password>().c_str();
    mod_rdp_params.rdp_compression                     = ini.get<cfg::mod_rdp::rdp_compression>();
    mod_rdp_params.error_message                       = &ini.get_mutable_ref<cfg::context::auth_error_message>();
    mod_rdp_params.disconnect_on_logon_user_change     = ini.get<cfg::mod_rdp::disconnect_on_logon_user_change>();
    mod_rdp_params.open_session_timeout                = ini.get<cfg::mod_rdp::open_session_timeout>();

    mod_rdp_params.server_cert_store                   = ini.get<cfg::mod_rdp::server_cert_store>();
    mod_rdp_params.server_cert_check                   = ini.get<cfg::mod_rdp::server_cert_check>();
    mod_rdp_params.server_access_allowed_message       = ini.get<cfg::mod_rdp::server_access_allowed_message>();
    mod_rdp_params.server_cert_create_message          = ini.get<cfg::mod_rdp::server_cert_create_message>();
    mod_rdp_params.server_cert_success_message         = ini.get<cfg::mod_rdp::server_cert_success_message>();
    mod_rdp_params.server_cert_failure_message         = ini.get<cfg::mod_rdp::server_cert_failure_message>();
    mod_rdp_params.server_cert_error_message           = ini.get<cfg::mod_rdp::server_cert_error_message>();

    mod_rdp_params.ignore_auth_channel = ini.get<cfg::mod_rdp::ignore_auth_channel>();
    mod_rdp_params.auth_channel = CHANNELS::ChannelNameId(ini.get<cfg::mod_rdp::auth_channel>());
    mod_rdp_params.checkout_channel = CHANNELS::ChannelNameId(ini.get<cfg::mod_rdp::checkout_channel>());

    mod_rdp_params.application_params = get_rdp_application_params(ini);

    mod_rdp_params.rdp_compression = ini.get<cfg::mod_rdp::rdp_compression>();
    mod_rdp_params.error_message = &ini.get_mutable_ref<cfg::context::auth_error_message>();
    mod_rdp_params.disconnect_on_logon_user_change = ini.get<cfg::mod_rdp::disconnect_on_logon_user_change>();
    mod_rdp_params.open_session_timeout = ini.get<cfg::mod_rdp::open_session_timeout>();
    mod_rdp_params.server_cert_store = ini.get<cfg::mod_rdp::server_cert_store>();
    mod_rdp_params.server_cert_check = ini.get<cfg::mod_rdp::server_cert_check>();
    mod_rdp_params.server_access_allowed_message = ini.get<cfg::mod_rdp::server_access_allowed_message>();
    mod_rdp_params.server_cert_create_message = ini.get<cfg::mod_rdp::server_cert_create_message>();
    mod_rdp_params.server_cert_success_message = ini.get<cfg::mod_rdp::server_cert_success_message>();
    mod_rdp_params.server_cert_failure_message = ini.get<cfg::mod_rdp::server_cert_failure_message>();
    mod_rdp_params.server_cert_error_message = ini.get<cfg::mod_rdp::server_cert_error_message>();
    mod_rdp_params.enable_server_cert_external_validation = ini.get<cfg::mod_rdp::enable_server_cert_external_validation>();
    mod_rdp_params.hide_client_name = ini.get<cfg::mod_rdp::hide_client_name>();
    mod_rdp_params.enable_persistent_disk_bitmap_cache = ini.get<cfg::mod_rdp::persistent_disk_bitmap_cache>();
    mod_rdp_params.enable_cache_waiting_list = ini.get<cfg::mod_rdp::cache_waiting_list>();
    mod_rdp_params.persist_bitmap_cache_on_disk = ini.get<cfg::mod_rdp::persist_bitmap_cache_on_disk>();
    mod_rdp_params.password_printing_mode = ini.get<cfg::debug::password>();
    mod_rdp_params.cache_verbose = to_verbose_flags(ini.get<cfg::debug::cache>());

    mod_rdp_params.disabled_orders                     += parse_primary_drawing_orders(
        ini.get<cfg::mod_rdp::disabled_orders>().c_str(),
        bool(to_verbose_flags(ini.get<cfg::debug::mod_rdp>()) & (RDPVerbose::basic_trace | RDPVerbose::capabilities)));

    mod_rdp_params.bogus_sc_net_size                   = ini.get<cfg::mod_rdp::bogus_sc_net_size>();
    mod_rdp_params.bogus_refresh_rect                  = ini.get<cfg::globals::bogus_refresh_rect>();

    mod_rdp_params.drive_params.proxy_managed_drives   = ini.get<cfg::mod_rdp::proxy_managed_drives>().c_str();
    mod_rdp_params.drive_params.proxy_managed_prefix   = app_path(AppPath::DriveRedirection);

    mod_rdp_params.lang                                = language(ini);

    mod_rdp_params.allow_using_multiple_monitors       = ini.get<cfg::globals::allow_using_multiple_monitors>();

    mod_rdp_params.adjust_performance_flags_for_recording
            = (ini.get<cfg::globals::is_rec>()
            && ini.get<cfg::client::auto_adjust_performance_flags>()
            && ((ini.get<cfg::video::capture_flags>()
                & (CaptureFlags::wrm | CaptureFlags::ocr)) != CaptureFlags::none));
    {
        auto & rap = mod_rdp_params.remote_app_params;
        rap.rail_client_execute = &rail_client_execute;
        rap.windows_execute_shell_params = rail_client_execute.get_windows_execute_shell_params();

        bool const rail_is_required = (ini.get<cfg::mod_rdp::use_native_remoteapp_capability>()
            && ((mod_rdp_params.application_params.target_application
                && *mod_rdp_params.application_params.target_application)
             || (ini.get<cfg::mod_rdp::use_client_provided_remoteapp>()
                && not rap.windows_execute_shell_params.exe_or_file.empty())));

        rap.should_ignore_first_client_execute = rail_client_execute.should_ignore_first_client_execute_;
        rap.enable_remote_program = ((client_info.remote_program
            || (ini.get<cfg::mod_rdp::wabam_uses_translated_remoteapp>()
                && ini.get<cfg::context::is_wabam>()))
                && rail_is_required);
        rap.remote_program_enhanced = client_info.remote_program_enhanced;
        rap.convert_remoteapp_to_desktop = (!client_info.remote_program
            && ini.get<cfg::mod_rdp::wabam_uses_translated_remoteapp>()
            && ini.get<cfg::context::is_wabam>()
            && rail_is_required);
        rap.use_client_provided_remoteapp = ini.get<cfg::mod_rdp::use_client_provided_remoteapp>();
        rap.rail_disconnect_message_delay = ini.get<cfg::context::rail_disconnect_message_delay>();
        rap.bypass_legal_notice_delay = ini.get<cfg::mod_rdp::remoteapp_bypass_legal_notice_delay>();
        rap.bypass_legal_notice_timeout = ini.get<cfg::mod_rdp::remoteapp_bypass_legal_notice_timeout>();
    }

    mod_rdp_params.clean_up_32_bpp_cursor              = ini.get<cfg::mod_rdp::clean_up_32_bpp_cursor>();
    mod_rdp_params.large_pointer_support               = ini.get<cfg::globals::large_pointer_support>();
    mod_rdp_params.load_balance_info                   = ini.get<cfg::mod_rdp::load_balance_info>().c_str();

    // ======================= File System Params ===================
    {
        auto & fsp = mod_rdp_params.file_system_params;
        fsp.disable_log_syslog = bool(ini.get<cfg::video::disable_file_system_log>()
                                      & FileSystemLogFlags::syslog);
        fsp.bogus_ios_rdpdr_virtual_channel     = ini.get<cfg::mod_rdp::bogus_ios_rdpdr_virtual_channel>();
        fsp.enable_rdpdr_data_analysis          =  ini.get<cfg::mod_rdp::enable_rdpdr_data_analysis>();
    }
    // ======================= End File System Params ===================


    // ======================= Dynamic Channel Params ===================

    mod_rdp_params.dynamic_channels_params.allowed_channels = ini.get<cfg::mod_rdp::allowed_dynamic_channels>().c_str();
    mod_rdp_params.dynamic_channels_params.denied_channels  = ini.get<cfg::mod_rdp::denied_dynamic_channels>().c_str();

    // ======================= End Dynamic Channel Params ===================

    mod_rdp_params.experimental_fix_input_event_sync =
        ini.get<cfg::mod_rdp::experimental_fix_input_event_sync>();
    mod_rdp_params.support_connection_redirection_during_recording =
        ini.get<cfg::globals::support_connection_redirection_during_recording>();
    mod_rdp_params.clipboard_params.log_only_relevant_activities =
        ini.get<cfg::mod_rdp::log_only_relevant_clipboard_activities>();
    mod_rdp_params.split_domain = ini.get<cfg::mod_rdp::split_domain>();

    mod_rdp_params.enable_remotefx = ini.get<cfg::mod_rdp::enable_remotefx>();
    mod_rdp_params.use_license_store = ini.get<cfg::mod_rdp::use_license_store>();
    mod_rdp_params.accept_monitor_layout_change_if_capture_is_not_started
        = ini.get<cfg::mod_rdp::accept_monitor_layout_change_if_capture_is_not_started>();

    mod_rdp_params.enable_restricted_admin_mode = ini.get<cfg::mod_rdp::enable_restricted_admin_mode>();
    mod_rdp_params.file_system_params.smartcard_passthrough        = smartcard_passthrough;

    mod_rdp_params.session_probe_params.alternate_directory_environment_variable = ini.get<cfg::mod_rdp::session_probe_alternate_directory_environment_variable>();
    enum {
        SESSION_PROBE_ALTERNATE_DIRECTORY_ENVIRONMENT_VARIABLE_NAME_MAX_LENGTH = 3
    };
    size_t const alternate_directory_environment_variable_length = mod_rdp_params.session_probe_params.alternate_directory_environment_variable.length();
    if (alternate_directory_environment_variable_length) {
        if (alternate_directory_environment_variable_length > SESSION_PROBE_ALTERNATE_DIRECTORY_ENVIRONMENT_VARIABLE_NAME_MAX_LENGTH) {
            mod_rdp_params.session_probe_params.alternate_directory_environment_variable.resize(SESSION_PROBE_ALTERNATE_DIRECTORY_ENVIRONMENT_VARIABLE_NAME_MAX_LENGTH);
        }

        mod_rdp_params.session_probe_params.customize_executable_name = true;

        mod_rdp_params.session_probe_params.vc_params.enable_self_cleaner = ini.get<cfg::mod_rdp::session_probe_enable_cleaner>();
    }

    const char * const name = "RDP Target";

    Rect const adjusted_client_execute_rect = rail_client_execute.adjust_rect(client_info.get_widget_rect());

    const bool host_mod_in_widget = (client_info.remote_program
        && !mod_rdp_params.remote_app_params.enable_remote_program);

    if (host_mod_in_widget) {
        client_info.screen_info.width  = adjusted_client_execute_rect.cx / 4 * 4;
        client_info.screen_info.height = adjusted_client_execute_rect.cy;
        client_info.cs_monitor = GCC::UserData::CSMonitor{};
    }
    else {
        rail_client_execute.reset(false);
    }

    // ================== FileValidator ============================
    auto & vp = mod_rdp_params.validator_params;
    vp.log_if_accepted = ini.get<cfg::file_verification::log_if_accepted>();
    vp.block_invalid_file_up = ini.get<cfg::file_verification::block_invalid_file_up>();
    vp.block_invalid_file_down = ini.get<cfg::file_verification::block_invalid_file_down>();
    vp.max_file_size_rejected = std::min<uint64_t>(
        (1ull << (64 - 20 /* mebibyte */)) - 1u,
        ini.get<cfg::file_verification::max_file_size_rejected>()
    ) * 1024u * 1024u;
    vp.enable_clipboard_text_up = ini.get<cfg::file_verification::clipboard_text_up>();
    vp.enable_clipboard_text_down = ini.get<cfg::file_verification::clipboard_text_down>();
    vp.up_target_name = ini.get<cfg::file_verification::enable_up>() ? "up" : "";
    vp.down_target_name = ini.get<cfg::file_verification::enable_down>() ? "down" : "";
    vp.lang = language(ini);

    bool enable_validator = ini.get<cfg::file_verification::enable_up>()
        || ini.get<cfg::file_verification::enable_down>();

    std::unique_ptr<RdpData::FileValidator> file_validator;
    int validator_fd = -1;

    if (enable_validator) {
        auto const& socket_path = ini.get<cfg::file_verification::socket_path>();
        bool const no_log_for_unix_socket = false;
        unique_fd ufd = addr_connect(socket_path.c_str(), no_log_for_unix_socket);
        if (ufd) {
            validator_fd = ufd.fd();
            fcntl(validator_fd, F_SETFL, fcntl(validator_fd, F_GETFL) & ~O_NONBLOCK);
            file_validator = std::make_unique<RdpData::FileValidator>(
                std::move(ufd),
                RdpData::FileValidator::CtxError{
                    sesman,
                    mod_rdp_params.validator_params.up_target_name,
                    mod_rdp_params.validator_params.down_target_name,
                    front
                },
                time_base, events);
            file_validator->service.send_infos({
                "server_ip"_av, ini.get<cfg::context::target_host>(),
                "client_ip"_av, ini.get<cfg::globals::host>(),
                "auth_user"_av, ini.get<cfg::globals::auth_user>()
            });
        }
        else {
            LOG(LOG_ERR, "Error, can't connect to validator, file validation disable");
            file_verification_error(
                front,
                time_base.get_current_time(),
                sesman,
                mod_rdp_params.validator_params.up_target_name,
                mod_rdp_params.validator_params.down_target_name,
                "Unable to connect to FileValidator service"_av
            );
            // enable_validator = false;
            throw Error(ERR_SOCKET_CONNECT_FAILED);
        }
    }
    // ================== End FileValidator =========================

    // ================== Metrics =========================
    bool const enable_metrics = (ini.get<cfg::metrics::enable_rdp_metrics>()
        && create_metrics_directory(ini.get<cfg::metrics::log_dir_path>().as_string()));

    std::unique_ptr<RdpData::ModMetrics> metrics;

    if (enable_metrics) {
        metrics = std::make_unique<RdpData::ModMetrics>(
            ini.get<cfg::metrics::log_dir_path>().as_string(),
            ini.get<cfg::context::session_id>(),
            hmac_user(
                ini.get<cfg::globals::auth_user>(),
                ini.get<cfg::metrics::sign_key>()),
            hmac_account(
                ini.get<cfg::globals::target_user>(),
                ini.get<cfg::metrics::sign_key>()),
            hmac_device_service(
                ini.get<cfg::globals::target_device>(),
                ini.get<cfg::context::target_service>(),
                ini.get<cfg::metrics::sign_key>()),
            hmac_client_info(
                ini.get<cfg::globals::host>(),
                client_info.screen_info,
                ini.get<cfg::metrics::sign_key>()),
            timeobj.get_time(),
            ini.get<cfg::metrics::log_file_turnover_interval>(),
            ini.get<cfg::metrics::log_interval>());
    }
    // ================== End Metrics ======================


    // ================== Application Driver =========================
    char const * application_driver_exe_or_file            = nullptr;
    char const * application_driver_script                 = nullptr;
    if (!strcasecmp(mod_rdp_params.application_params.alternate_shell, "__APP_DRIVER_IE__")) {
        application_driver_exe_or_file = ini.get<cfg::mod_rdp::application_driver_exe_or_file>();
        application_driver_script      = ini.get<cfg::mod_rdp::application_driver_ie_script>();
    }
    else if (!strcasecmp(mod_rdp_params.application_params.alternate_shell, "__APP_DRIVER_CHROME_UIA__")) {
        application_driver_exe_or_file = ini.get<cfg::mod_rdp::application_driver_exe_or_file>();
        application_driver_script      = ini.get<cfg::mod_rdp::application_driver_chrome_uia_script>();
    }
    if (application_driver_exe_or_file) {
        char const * session_probe_dir_remote = "\\\\tsclient\\SESPRO\\";

        struct FILE_INFO {
            std::string size;
            std::string hash;
        };

        auto get_file_info = [](const char * directory, const char * filename) -> FILE_INFO {
            FILE_INFO file_info;

            std::string file_path(directory);
            file_path += filename;
            file_path += ".hash";

            std::string file_contents;
            (void)append_file_contents(file_path.c_str(), file_contents, 8192);

            size_t pos = file_contents.find(' ');

            if (std::string::npos != pos) {
                file_info.size.assign(file_contents.c_str(), pos);
                file_info.hash.assign(file_contents, pos + 1, std::string::npos);
            }

            return file_info;
        };

        std::string & application_driver_alternate_shell = ini.get_mutable_ref<cfg::context::application_driver_alternate_shell>();
        application_driver_alternate_shell  = "\x02";
        application_driver_alternate_shell += session_probe_dir_remote;
        application_driver_alternate_shell += application_driver_exe_or_file;

        std::string session_probe_dir_local { app_path(AppPath::DriveRedirection) };
        session_probe_dir_local += "/sespro/";

        {
            FILE_INFO file_info = get_file_info(session_probe_dir_local.c_str(), application_driver_exe_or_file);

            application_driver_alternate_shell += "\x02";
            application_driver_alternate_shell += file_info.size;

            application_driver_alternate_shell += "\x02";
            application_driver_alternate_shell += file_info.hash;
        }

        application_driver_alternate_shell += "\x02";
        application_driver_alternate_shell += session_probe_dir_remote;
        application_driver_alternate_shell += application_driver_script;

        {
            FILE_INFO file_info = get_file_info(session_probe_dir_local.c_str(), application_driver_script);

            application_driver_alternate_shell += "\x02";
            application_driver_alternate_shell += file_info.size;

            application_driver_alternate_shell += "\x02";
            application_driver_alternate_shell += file_info.hash;
        }

        application_driver_alternate_shell += "\x02";

        mod_rdp_params.application_params.alternate_shell = application_driver_alternate_shell.c_str();


        std::string& application_driver_shell_arguments = ini.get_mutable_ref<cfg::context::application_driver_shell_arguments>();

        application_driver_shell_arguments  = ini.get<cfg::mod_rdp::application_driver_script_argument>();
        application_driver_shell_arguments += " ";
        application_driver_shell_arguments += ini.get<cfg::mod_rdp::shell_arguments>();

        mod_rdp_params.application_params.shell_arguments = application_driver_shell_arguments.c_str();

        mod_rdp_params.session_probe_params.enable_session_probe                               = true;
        mod_rdp_params.session_probe_params.vc_params.launch_application_driver                = true;
        mod_rdp_params.session_probe_params.vc_params.launch_application_driver_then_terminate = !(ini.get<cfg::mod_rdp::enable_session_probe>());
        mod_rdp_params.session_probe_params.vc_params.enable_application_driver_cleaner        = ini.get<cfg::mod_rdp::application_driver_enable_cleaner>();
    }
    // ================== End Application Driver ======================


    unique_fd client_sck =
        connect_to_target_host(ini, sesman, trkeys::authentification_rdp_fail, ini.get<cfg::mod_rdp::enable_ipv6>());
    IpAddress local_ip_address;

    switch (ini.get<cfg::mod_rdp::client_address_sent>())
    {
        case ClientAddressSent::no_address :
            break;
        case ClientAddressSent::front :
            mod_rdp_params.client_address =
                ini.get<cfg::globals::host>().c_str();
            break;
        case ClientAddressSent::proxy :
            if (!get_local_ip_address(local_ip_address, client_sck.fd()))
            {
                throw Error(ERR_SOCKET_CONNECT_FAILED);
            }
            mod_rdp_params.client_address = local_ip_address.ip_addr;
            break;
    }

    auto new_mod = std::make_unique<ModRDPWithSocketAndMetrics>(
        mod_wrapper,
        ini,
        name,
        std::move(client_sck),
        ini.get<cfg::debug::mod_rdp>(),
        &ini.get_mutable_ref<cfg::context::auth_error_message>(),
        time_base,
        events,
        sesman,
        drawable,
        front,
        client_info,
        ini.get_mutable_ref<cfg::mod_rdp::redir_info>(),
        gen,
        timeobj,
        channels_authorizations,
        mod_rdp_params,
        tls_client_params,
        file_system_license_store,
        ini,
        enable_metrics ? &metrics->protocol_metrics : nullptr,
        enable_validator ? &file_validator->service : nullptr,
        ini.get<cfg::debug::mod_rdp_use_failure_simulation_socket_transport>()
    );

    if (enable_validator) {
        new_mod->rdp_data.file_validator = std::move(file_validator);
        new_mod->rdp_data.file_validator->mod = &new_mod->mod;
    }

    if (enable_metrics) {
        new_mod->rdp_data.metrics = std::move(metrics);
        new_mod->rdp_data.set_metrics_timer(std::chrono::seconds(ini.get<cfg::metrics::log_interval>()));
    }

    if (new_mod) {
        assert(&ini == &ini);
        new_mod->get_rdp_factory().always_file_storage  = (ini.get<cfg::file_storage::store_file>() == RdpStoreFile::always);
        switch (ini.get<cfg::file_storage::store_file>())
        {
            case RdpStoreFile::never:
                break;
            case RdpStoreFile::on_invalid_verification:
                if (!enable_validator) {
                    break;
                }
                [[fallthrough]];
            case RdpStoreFile::always:
                new_mod->get_rdp_factory().get_fdx_capture = [mod = new_mod.get(), &gen, &ini, &cctx]{ return mod->get_fdx_capture(gen, ini, cctx);};
        }
    }

    auto tmp_psocket_transport = &(*new_mod->socket_transport_ptr);

    if (!host_mod_in_widget) {
        auto mod = new_mod.release();
        ModPack mod_pack{mod, &(mod->mod), mod->mod.get_windowing_api(), nullptr, false, false, tmp_psocket_transport};
        return mod_pack;
    }

    // TODO: this should be external, not in mod_factory
    // Host Mod In Widget
    LOG(LOG_INFO, "Creation of internal module 'RailModuleHostMod'");

    std::string target_info = str_concat(
        ini.get<cfg::context::target_str>(),
        ':',
        ini.get<cfg::globals::primary_user_id>());

    rail_client_execute.set_target_info(target_info);

    auto* host_mod = new RailModuleHostMod(
        ini,
        time_base,
        events,
        drawable,
        front,
        client_info.screen_info.width,
        client_info.screen_info.height,
        adjusted_client_execute_rect,
        std::move(new_mod),
        rail_client_execute,
        glyphs,
        theme,
        client_info.cs_monitor,
        !ini.get<cfg::globals::is_rec>()
    );
    host_mod->init();

    ModPack mod_pack{host_mod, nullptr, &rail_client_execute, host_mod, false, false, tmp_psocket_transport};
    return mod_pack;
}
