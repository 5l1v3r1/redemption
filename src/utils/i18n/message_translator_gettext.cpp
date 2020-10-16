#include <clocale>
#include <libintl.h>

#include "core/app_path.hpp"
#include "utils/fileutils.hpp"
#include "utils/log.hpp"
#include "utils/sugar/algostring.hpp"

#include "message_translator_gettext.hpp"

namespace
{    
    constexpr zstring_view GETTEXTINSTALL_DIRNAME = "locale"_zv;
    constexpr zstring_view CATEGORY_DIRNAME = "LC_MESSAGES"_zv;
    constexpr zstring_view DOMAIN_NAME = "redemption"_zv;
    constexpr zstring_view DOMAIN_NAME_EXT = ".mo"_zv;
    constexpr const char *CODESET = "UTF-8";
    constexpr const char *LOCALE = "C.UTF-8";
    constexpr const char *LANGUAGE_ENV_VAR = "LANGUAGE";
    
    [[nodiscard]]
    inline bool
    check_directory(const std::string& directory_path,
                    const char *log_msg_part,
                    zstring_view locale_name)
    {
        if (!dir_exist(directory_path))
        {
            LOG(LOG_WARNING,
                "need %s directory for messages with \"%s\" locale",
                log_msg_part,
                locale_name);
            return false;
        }
        return true;
    }
    
    [[nodiscard]]
    inline bool
    check_domain_name_file_path(const std::string& domain_name_file_path,
                                zstring_view locale_name)
    {
        if (!file_exist(domain_name_file_path))
        {
            LOG(LOG_WARNING,
                "need domain name file for messages with \"%s\" locale",
                locale_name);
            return false;
        }
        
        if (dir_exist(domain_name_file_path))
        {
            LOG(LOG_WARNING,
                "domain name must be a file for messages with \"%s\" locale",
                locale_name);
            return false;    
        }
        return true;
    }
}

namespace i18n
{
    MessageTranslatorGettext::MessageTranslatorGettext() :
        _clearable_context(false)
    { }

    MessageTranslatorGettext::~MessageTranslatorGettext()
    {
        clear_context();
    }
    
    void MessageTranslatorGettext::clear_context()
    {
        if (!_clearable_context)
        {
            return;
        }
        
        if (!std::setlocale(LC_MESSAGES, ""))
        {
            LOG(LOG_ERR, "cannot reset LC_MESSAGES environment variable");
        }
        
        if (unsetenv(LANGUAGE_ENV_VAR) == -1)
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "unsetenv() error (errno : %d)", errno);
                throw std::bad_alloc();  
            }
            LOG(LOG_WARNING,
                "clear \"%s\" environment variable not successful",
                LANGUAGE_ENV_VAR);
        }
        
        if (!bindtextdomain("", ""))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "bindtextdomain() error (errno : %d)", errno);
                throw std::bad_alloc();   
            }
        }
        
        if (!bind_textdomain_codeset("", ""))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR,
                    "bind_textdomain_codeset() error (errno : %d)",
                    errno);
                throw std::bad_alloc();    
            }
        }
        
        if (!textdomain(""))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "textdomain() error (errno : %d)", errno);
                throw std::bad_alloc();     
            }
        }
        LOG(LOG_INFO,
            "i18n context is cleared. Default system locale is used");
        _clearable_context = false;
    }
    
    void MessageTranslatorGettext::set_context(zstring_view locale_name)
    {        
        std::string gettextinstall_path = str_concat(app_path(AppPath::Share),
                                                     '/',
                                                     GETTEXTINSTALL_DIRNAME,
                                                     '/');
        
        if (!check_directory(gettextinstall_path,
                             "gettextinstall",
                             locale_name))
        {
            return;
        }
        
        std::string target_locale_path = str_concat(gettextinstall_path,
                                                    '/',
                                                    locale_name,
                                                    '/');

        if (!check_directory(target_locale_path,
                             "target locale",
                             locale_name))
        {
            return;
        }

        std::string category_path = str_concat(target_locale_path,
                                               '/',
                                               CATEGORY_DIRNAME,
                                               '/');

        if (!check_directory(category_path,
                             "category",
                             locale_name))
        {
            return;
        }
        
        if (auto domain_name_file_path = str_concat(category_path,
                                                    '/',
                                                    DOMAIN_NAME,
                                                    DOMAIN_NAME_EXT);
            !check_domain_name_file_path(domain_name_file_path, locale_name))
        {
            return;    
        }
        
        if (!std::setlocale(LC_MESSAGES, LOCALE))
        {            
            LOG(LOG_ERR,
                "cannot set LC_MESSAGES environment variable "
                "with locale for use GNU gettext framework");
            return;
        }
        
        if (setenv(LANGUAGE_ENV_VAR, locale_name.c_str(), 1) == -1)
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "setenv() error (errno : %d)", errno);
                throw std::bad_alloc();
            }
            LOG(LOG_ERR,
                "cannot set LANGUAGE environment variable "
                "with locale \"%s\" for use GNU gettext framework",
                locale_name);
            return;
        }
        
        if (!bindtextdomain(DOMAIN_NAME.c_str(), gettextinstall_path.c_str()))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "bindtextdomain() error (errno : %d)", errno);
                throw std::bad_alloc();        
            }
            LOG(LOG_ERR,
                "cannot set directory for domain name "
                "with locale \"%s\" for use GNU gettext framework",
                locale_name);
            return;
        }
        
        if (!bind_textdomain_codeset(DOMAIN_NAME.c_str(), CODESET))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR,
                    "bind_textdomain_codeset() error (errno : %d)",
                    errno);
                throw std::bad_alloc();        
            }
            LOG(LOG_ERR,
                "cannot set codeset for domain name "
                "with locale \"%s\" for use GNU gettext framework",
                locale_name);
            return;    
        }
        
        if (!textdomain(DOMAIN_NAME.c_str()))
        {
            if (errno == ENOMEM)
            {
                LOG(LOG_ERR, "textdomain() error (errno : %d)", errno);
                throw std::bad_alloc();
            }
            LOG(LOG_ERR,
                "cannot set domain name with locale \"%s\" "
                "for use GNU gettext framework",
                locale_name);
            return;
        }
        LOG(LOG_INFO,
            "i18n context is set for \"%s\" locale",
            locale_name);
        _clearable_context = true;
        return;
    }

    zstring_view
    MessageTranslatorGettext::get_translated_text(const char *text)
        const noexcept
    {
        const char *translated_text = gettext(text);

        return zstring_view(zstring_view::is_zero_terminated { },
                            translated_text,
                            std::strlen(translated_text));
    }
}
