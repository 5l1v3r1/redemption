/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2013
 *   Author(s): Christophe Grosjean, Xiaopeng Zhou, Jonathan Poelen, Meng Tan
 *
 */

#include "configs/config.hpp"
#include "mod/internal/close_mod.hpp"
#include "core/front_api.hpp"
#include "gdi/graphic_api.hpp"
#include "core/RDP/slowpath.hpp"
#include "RAIL/client_execute.hpp"

static FlatWabClose build_close_widget(gdi::GraphicApi & drawable,
                                Rect const widget_rect,
                                CloseMod & mod,
                                WidgetScreen & screen,
                                std::string auth_error_message,
                                CloseModVariables vars,
                                Font const& font, Theme const& theme, bool back_selector);

static FlatWabClose build_close_widget(gdi::GraphicApi & drawable,
                                Rect const widget_rect,
                                CloseMod & mod,
                                WidgetScreen & screen,
                                std::string auth_error_message,
                                CloseModVariables vars,
                                Font const& font, Theme const& theme, bool back_selector)
{

    struct temporary_text
    {
        char text[255];

        explicit temporary_text(CloseModVariables vars)
        {
            if (vars.get<cfg::context::module>() == "selector") {
                snprintf(text, sizeof(text), "%s", TR(trkeys::selector, language(vars)).c_str());
            }
            else {
                // TODO target_application only used for user message,
                // the two branches of alternative should be unified et message prepared by sesman
                if (!vars.get<cfg::globals::target_application>().empty()) {
                    snprintf(
                        text, sizeof(text), "%s",
                        vars.get<cfg::globals::target_application>().c_str());
                }
                else {
                    snprintf(
                        text, sizeof(text), "%s@%s",
                        vars.get<cfg::globals::target_user>().c_str(),
                        vars.get<cfg::globals::target_device>().c_str());
                }
            }
        }
    };


    FlatWabClose widget(drawable, widget_rect.x, widget_rect.y, widget_rect.cx, widget_rect.cy,
                        screen, &mod, auth_error_message.c_str(),
                        (vars.is_asked<cfg::globals::auth_user>() || vars.is_asked<cfg::globals::target_device>())
                            ? nullptr
                            : vars.get<cfg::globals::auth_user>().c_str(),
                        (vars.is_asked<cfg::globals::auth_user>() || vars.is_asked<cfg::globals::target_device>())
                            ? nullptr
                            : temporary_text(vars).text,
                        true,
                        vars.get<cfg::context::close_box_extra_message>().c_str(),
                        font, theme, language(vars), back_selector);

    return widget;
}



CloseMod::CloseMod(
    std::string auth_error_message,
    CloseModVariables vars,
    TimeBase& time_base,
    EventContainer& events,
    GdProvider & gd_provider,
    FrontAPI & front, uint16_t width, uint16_t height,
    Rect const widget_rect, ClientExecute & rail_client_execute,
    Font const& font, Theme const& theme, bool back_selector)
    : close_widget(
        build_close_widget(gd_provider.get_graphics(), widget_rect, *this, this->screen, auth_error_message,
                           vars, font, theme, back_selector))
    , vars(vars)
    , front_width(width)
    , front_height(height)
    , front(front)
    , screen(gd_provider.get_graphics(), front_width, front_height, font, nullptr, theme)
    , rail_client_execute(rail_client_execute)
    , dvc_manager(false)
    , dc_state(DCState::Wait)
    , rail_enabled(rail_client_execute.is_rail_enabled())
    , current_mouse_owner(MouseOwner::WidgetModule)
    , time_base(time_base)
    , events(events)
{
    this->screen.set_wh(this->front_width, this->front_height);

    if (vars.get<cfg::globals::close_timeout>().count()) {
        LOG(LOG_INFO, "WabCloseMod: Ending session in %u seconds",
            static_cast<unsigned>(vars.get<cfg::globals::close_timeout>().count()));
    }
    gd_provider.get_graphics().set_palette(BGRPalette::classic_332());

    this->screen.add_widget(&this->close_widget);
    this->close_widget.set_widget_focus(&this->close_widget.cancel, Widget::focus_reason_tabkey);
    this->screen.set_widget_focus(&this->close_widget, Widget::focus_reason_tabkey);

    this->screen.rdp_input_invalidate(this->screen.get_rect());

    Event close_event("Close:Close Event", this);
    close_event.alarm.set_timeout(
                        time_base.get_current_time()
                        +std::chrono::seconds{this->vars.get<cfg::globals::close_timeout>()});
    close_event.actions.on_timeout = [this](Event&e)
    {
        this->set_mod_signal(BACK_EVENT_STOP);
        e.garbage = true;
    };
    this->events.push_back(std::move(close_event));

    Event refresh_event("Close:Refresh Event", this);
    refresh_event.alarm.set_timeout(time_base.get_current_time());
    refresh_event.alarm.set_period(std::chrono::seconds{1});
    refresh_event.actions.on_timeout = [this](Event& event)
    {
        auto elapsed = event.alarm.now.tv_sec-event.alarm.start_time.tv_sec;
        auto remaining = std::chrono::seconds{this->vars.get<cfg::globals::close_timeout>()}
                        - std::chrono::seconds{elapsed};
        this->close_widget.refresh_timeleft(remaining.count());
    };
    events.push_back(std::move(refresh_event));
}

CloseMod::~CloseMod()
{
    end_of_lifespan(this->events, this);
    this->vars.set<cfg::context::close_box_extra_message>("");
    this->screen.clear();
    this->rail_client_execute.reset(true);
}

void CloseMod::init()
{
    if (this->rail_enabled && !this->rail_client_execute) {
        this->rail_client_execute.ready(
            *this, this->front_width, this->front_height, this->font(),
            this->is_resizing_hosted_desktop_allowed());

        this->dvc_manager.ready(this->front);
    }
}

void CloseMod::notify(Widget* sender, notify_event_t event)
{
    (void)sender;
    if (NOTIFY_CANCEL == event) {
        LOG(LOG_INFO, "CloseMod::notify Click on Close Button");
        this->set_mod_signal(BACK_EVENT_STOP);
    }
    else if (NOTIFY_SUBMIT == event) {
        LOG(LOG_INFO, "CloseMod::notify Click on Back to Selector Button");
        this->vars.ask<cfg::context::selector>();
        this->vars.ask<cfg::globals::target_user>();
        this->vars.ask<cfg::globals::target_device>();
        this->vars.ask<cfg::context::target_protocol>();
        this->set_mod_signal(BACK_EVENT_NEXT);
    }
}



void CloseMod::rdp_input_invalidate(Rect r)
{
    this->screen.rdp_input_invalidate(r);

    if (this->rail_enabled) {
        this->rail_client_execute.input_invalidate(r);
    }
}

void CloseMod::rdp_input_mouse(int device_flags, int x, int y, Keymap2 * keymap)
{
    if (device_flags & (MOUSE_FLAG_WHEEL | MOUSE_FLAG_HWHEEL)) {
        x = this->old_mouse_x;
        y = this->old_mouse_y;
    }
    else {
        this->old_mouse_x = x;
        this->old_mouse_y = y;
    }

    if (!this->rail_enabled) {
        this->screen.rdp_input_mouse(device_flags, x, y, keymap);
    }
    else {
        bool out_mouse_captured = false;
        if (!this->rail_client_execute.input_mouse(device_flags, x, y, out_mouse_captured)) {
            switch (this->dc_state) {
                case DCState::Wait:
                    if (device_flags == (SlowPath::PTRFLAGS_DOWN | SlowPath::PTRFLAGS_BUTTON1)) {
                        this->dc_state = DCState::FirstClickDown;

                        if (this->first_click_down_timer) {
                            for(auto & event: this->events){
                                if (event.id == this->first_click_down_timer){
                                    event.alarm.set_timeout(
                                                this->time_base.get_current_time()
                                                +std::chrono::seconds{1});
                                }
                            }
                        }
                        else {
                            Event dc_event("Close::DC Event", this);
                            this->first_click_down_timer = dc_event.id;
                            dc_event.alarm.set_timeout(
                                                this->time_base.get_current_time()
                                                +std::chrono::seconds{1});
                            dc_event.actions.on_timeout = [this](Event&)
                            {
                                this->dc_state = DCState::Wait;
                            };
                            this->events.push_back(std::move(dc_event));
                        }
                    }
                break;

                case DCState::FirstClickDown:
                    if (device_flags == SlowPath::PTRFLAGS_BUTTON1) {
                        this->dc_state = DCState::FirstClickRelease;
                    }
                    else if (device_flags == (SlowPath::PTRFLAGS_DOWN | SlowPath::PTRFLAGS_BUTTON1)) {
                    }
                    else {
                        this->cancel_double_click_detection();
                    }
                break;

                case DCState::FirstClickRelease:
                    if (device_flags == (SlowPath::PTRFLAGS_DOWN | SlowPath::PTRFLAGS_BUTTON1)) {
                        this->dc_state = DCState::SecondClickDown;
                    }
                    else {
                        this->cancel_double_click_detection();
                    }
                break;

                case DCState::SecondClickDown:
                    if (device_flags == SlowPath::PTRFLAGS_BUTTON1) {
                        this->dc_state = DCState::Wait;

                        bool out_mouse_captured_2 = false;

                        this->rail_client_execute.input_mouse(PTRFLAGS_EX_DOUBLE_CLICK, x, y, out_mouse_captured_2);

                        this->cancel_double_click_detection();
                    }
                    else if (device_flags == (SlowPath::PTRFLAGS_DOWN | SlowPath::PTRFLAGS_BUTTON1)) {
                    }
                    else {
                        this->cancel_double_click_detection();
                    }
                break;

                default:
                    assert(false);

                    this->cancel_double_click_detection();
                break;
            }

            if (out_mouse_captured) {
                this->allow_mouse_pointer_change(false);

                this->current_mouse_owner = MouseOwner::ClientExecute;
            }
            else {
                if (MouseOwner::WidgetModule != this->current_mouse_owner) {
                    this->redo_mouse_pointer_change(x, y);
                }

                this->current_mouse_owner = MouseOwner::WidgetModule;
            }
        }

        this->screen.rdp_input_mouse(device_flags, x, y, keymap);

        if (out_mouse_captured) {
            this->allow_mouse_pointer_change(true);
        }
    }
}

void CloseMod::rdp_input_scancode(
    long param1, long param2, long param3, long param4, Keymap2 * keymap)
{
    this->screen.rdp_input_scancode(param1, param2, param3, param4, keymap);

    if (this->rail_enabled) {
        if (!this->alt_key_pressed) {
            if ((param1 == 56) && !(param3 & SlowPath::KBDFLAGS_RELEASE)) {
                this->alt_key_pressed = true;
            }
        }
        else {
//            if ((param1 == 56) && (param3 == (SlowPath::KBDFLAGS_DOWN | SlowPath::KBDFLAGS_RELEASE))) {
            if ((param1 == 56) && (param3 & SlowPath::KBDFLAGS_RELEASE)) {
                this->alt_key_pressed = false;
            }
            else if ((param1 == 62) && !param3) {
                LOG(LOG_INFO, "CloseMod::rdp_input_scancode: Close by user (Alt+F4)");
                throw Error(ERR_WIDGET);    // F4 key pressed
            }
        }
    }
}

void CloseMod::refresh(Rect r)
{
    this->screen.refresh(r);

    if (this->rail_enabled) {
        this->rail_client_execute.input_invalidate(r);
    }
}

void CloseMod::send_to_mod_channel( CHANNELS::ChannelNameId front_channel_name, InStream& chunk, size_t length, uint32_t flags)
{
    if (this->rail_enabled && this->rail_client_execute) {
        if (front_channel_name == CHANNELS::channel_names::rail) {
            this->rail_client_execute.send_to_mod_rail_channel(length, chunk, flags);
        }
        else if (front_channel_name == CHANNELS::channel_names::drdynvc) {
            this->dvc_manager.send_to_mod_drdynvc_channel(length, chunk, flags);
        }
    }
}

void CloseMod::cancel_double_click_detection()
{
    assert(this->rail_enabled);

    if (this->first_click_down_timer) {
        for(auto & event: this->events){
            if (event.id == this->first_click_down_timer){
                event.garbage = true;
                event.id = 0;
            }
        }
    }
    this->first_click_down_timer = 0;
    this->dc_state = DCState::Wait;
}

bool CloseMod::is_resizing_hosted_desktop_allowed() const
{
    assert(this->rail_enabled);

    return false;
}
