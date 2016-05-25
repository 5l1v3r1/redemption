#pragma once

#include "configs/c_str_buf.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>


namespace configs {

template<> struct CStrBuf<CaptureFlags> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<CaptureFlags> & buf, CaptureFlags x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(CaptureFlags & x, spec_type<CaptureFlags>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (5 - 1)) - 1)) {
        x = ~~static_cast<CaptureFlags>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<Level> : CStrBuf<void> {};

inline char const * to_cstr(CStrBuf<Level> &, Level x)
{
    constexpr char const * arr[]{
        "low", 
        "medium", 
        "high", 
    };
    assert(static_cast<unsigned long>(x) < 3);
    return arr[static_cast<unsigned long>(x))];
}
bool parse(Level & x, spec_type<Level>, char const * str)
{
    if (0 == strcasecmp(str, "low")) {
        x = Level::low;
        return true;
    }

    if (0 == strcasecmp(str, "medium")) {
        x = Level::medium;
        return true;
    }

    if (0 == strcasecmp(str, "high")) {
        x = Level::high;
        return true;
    }

    return false;
}


template<> struct CStrBuf<Language> : CStrBuf<void> {};

inline char const * to_cstr(CStrBuf<Language> &, Language x)
{
    constexpr char const * arr[]{
        "en", 
        "fr", 
    };
    assert(static_cast<unsigned long>(x) < 2);
    return arr[static_cast<unsigned long>(x))];
}
bool parse(Language & x, spec_type<Language>, char const * str)
{
    if (0 == strcasecmp(str, "en")) {
        x = Language::en;
        return true;
    }

    if (0 == strcasecmp(str, "fr")) {
        x = Language::fr;
        return true;
    }

    return false;
}


template<> struct CStrBuf<ClipboardEncodingType> : CStrBuf<void> {};

inline char const * to_cstr(CStrBuf<ClipboardEncodingType> &, ClipboardEncodingType x)
{
    constexpr char const * arr[]{
        "utf8", 
        "latin1", 
    };
    assert(static_cast<unsigned long>(x) < 2);
    return arr[static_cast<unsigned long>(x))];
}
bool parse(ClipboardEncodingType & x, spec_type<ClipboardEncodingType>, char const * str)
{
    if (0 == strcasecmp(str, "utf-8")) {
        x = ClipboardEncodingType::utf8;
        return true;
    }

    if (0 == strcasecmp(str, "latin1")) {
        x = ClipboardEncodingType::latin1;
        return true;
    }

    return false;
}


template<> struct CStrBuf<KeyboardLogFlags> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<KeyboardLogFlags> & buf, KeyboardLogFlags x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(KeyboardLogFlags & x, spec_type<KeyboardLogFlags>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<KeyboardLogFlags>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<ClipboardLogFlags> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<ClipboardLogFlags> & buf, ClipboardLogFlags x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(ClipboardLogFlags & x, spec_type<ClipboardLogFlags>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<ClipboardLogFlags>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<FileSystemLogFlags> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<FileSystemLogFlags> & buf, FileSystemLogFlags x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(FileSystemLogFlags & x, spec_type<FileSystemLogFlags>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<FileSystemLogFlags>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<ServerNotification> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<ServerNotification> & buf, ServerNotification x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(ServerNotification & x, spec_type<ServerNotification>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (4 - 1)) - 1)) {
        x = ~~static_cast<ServerNotification>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<ServerCertCheck> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<ServerCertCheck> & buf, ServerCertCheck x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(ServerCertCheck & x, spec_type<ServerCertCheck>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (4 - 1)) - 1)) {
        x = ~~static_cast<ServerCertCheck>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<TraceType> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<TraceType> & buf, TraceType x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(TraceType & x, spec_type<TraceType>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<TraceType>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<KeyboardInputMaskingLevel> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<KeyboardInputMaskingLevel> & buf, KeyboardInputMaskingLevel x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(KeyboardInputMaskingLevel & x, spec_type<KeyboardInputMaskingLevel>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<KeyboardInputMaskingLevel>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<SessionProbeOnLaunchFailure> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<SessionProbeOnLaunchFailure> & buf, SessionProbeOnLaunchFailure x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(SessionProbeOnLaunchFailure & x, spec_type<SessionProbeOnLaunchFailure>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<SessionProbeOnLaunchFailure>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<VncBogusClipboardInfiniteLoop> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<VncBogusClipboardInfiniteLoop> & buf, VncBogusClipboardInfiniteLoop x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(VncBogusClipboardInfiniteLoop & x, spec_type<VncBogusClipboardInfiniteLoop>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<VncBogusClipboardInfiniteLoop>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<ColorDepthSelectionStrategy> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<ColorDepthSelectionStrategy> & buf, ColorDepthSelectionStrategy x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(ColorDepthSelectionStrategy & x, spec_type<ColorDepthSelectionStrategy>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (2 - 1)) - 1)) {
        x = ~~static_cast<ColorDepthSelectionStrategy>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<WrmCompressionAlgorithm> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<WrmCompressionAlgorithm> & buf, WrmCompressionAlgorithm x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(WrmCompressionAlgorithm & x, spec_type<WrmCompressionAlgorithm>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (3 - 1)) - 1)) {
        x = ~~static_cast<WrmCompressionAlgorithm>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<RdpCompression> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<RdpCompression> & buf, RdpCompression x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(RdpCompression & x, spec_type<RdpCompression>, char const * str)
{
    char * end = nullptr;
    errno = 0;
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    auto n = std::strtoul(str, &end, is_hex ? 16 : 10);
    if (!errno && end && !*end && n <= static_cast<unsigned long>((1 << (5 - 1)) - 1)) {
        x = ~~static_cast<RdpCompression>(n);
        return true;
    }
    return false;
}


template<> struct CStrBuf<ColorDepth> : CStrBuf<unsigned long> {};

inline char const * to_cstr(CStrBuf<ColorDepth> & buf, ColorDepth x)
{
    std::snprintf(buf.get(), buf.size(), "lu", static_cast<unsigned long>(x));
    return buf.get();
}

bool parse(ColorDepth & x, spec_type<ColorDepth>, char const * str)
{
    bool const is_hex = str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
    unsigned long const val = std::strtoul(str, &end, is_hex ? 16 : 10)

    if (val == static_cast<unsigned long>(ColorDepth::depth8)) {
        x = ColorDepth::depth8
        return true;
    }

    if (val == static_cast<unsigned long>(ColorDepth::depth15)) {
        x = ColorDepth::depth15
        return true;
    }

    if (val == static_cast<unsigned long>(ColorDepth::depth16)) {
        x = ColorDepth::depth16
        return true;
    }

    if (val == static_cast<unsigned long>(ColorDepth::depth24)) {
        x = ColorDepth::depth24
        return true;
    }

    return false;
}


}
