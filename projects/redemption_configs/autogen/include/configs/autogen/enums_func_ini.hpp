//
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN
//

#pragma once

#include "configs/zbuffer.hpp"
#include "configs/autogen/enums.hpp"


namespace configs
{

template<>
struct str_buffer_size<Level>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<Language>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<ClipboardEncodingType>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<OcrLocale>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<RdpModeConsole>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<RdpStoreFile>
{
    static const std::size_t value = 0;
};

template<>
struct str_buffer_size<LoginLanguage>
{
    static const std::size_t value = 0;
};

} // namespace config
