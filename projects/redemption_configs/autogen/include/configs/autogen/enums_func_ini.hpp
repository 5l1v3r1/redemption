//
// DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN
//

#pragma once

#include "configs/io.hpp"
#include "configs/autogen/enums.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>


namespace configs {

template<> struct zstr_buffer_traits<CaptureFlags> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<CaptureFlags> & buf,
    cfg_s_type<CaptureFlags> /*type*/,
    CaptureFlags x
);

parse_error parse(CaptureFlags & x, spec_type<CaptureFlags> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<Level> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<Level> & buf,
    cfg_s_type<Level> /*type*/,
    Level x
);

parse_error parse(Level & x, spec_type<Level> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<Language> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<Language> & buf,
    cfg_s_type<Language> /*type*/,
    Language x
);

parse_error parse(Language & x, spec_type<Language> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ClipboardEncodingType> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ClipboardEncodingType> & buf,
    cfg_s_type<ClipboardEncodingType> /*type*/,
    ClipboardEncodingType x
);

parse_error parse(ClipboardEncodingType & x, spec_type<ClipboardEncodingType> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<KeyboardLogFlagsCP> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<KeyboardLogFlagsCP> & buf,
    cfg_s_type<KeyboardLogFlagsCP> /*type*/,
    KeyboardLogFlagsCP x
);

parse_error parse(KeyboardLogFlagsCP & x, spec_type<KeyboardLogFlagsCP> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<KeyboardLogFlags> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<KeyboardLogFlags> & buf,
    cfg_s_type<KeyboardLogFlags> /*type*/,
    KeyboardLogFlags x
);

parse_error parse(KeyboardLogFlags & x, spec_type<KeyboardLogFlags> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ClipboardLogFlags> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ClipboardLogFlags> & buf,
    cfg_s_type<ClipboardLogFlags> /*type*/,
    ClipboardLogFlags x
);

parse_error parse(ClipboardLogFlags & x, spec_type<ClipboardLogFlags> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<FileSystemLogFlags> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<FileSystemLogFlags> & buf,
    cfg_s_type<FileSystemLogFlags> /*type*/,
    FileSystemLogFlags x
);

parse_error parse(FileSystemLogFlags & x, spec_type<FileSystemLogFlags> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ServerNotification> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ServerNotification> & buf,
    cfg_s_type<ServerNotification> /*type*/,
    ServerNotification x
);

parse_error parse(ServerNotification & x, spec_type<ServerNotification> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ServerCertCheck> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ServerCertCheck> & buf,
    cfg_s_type<ServerCertCheck> /*type*/,
    ServerCertCheck x
);

parse_error parse(ServerCertCheck & x, spec_type<ServerCertCheck> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<TraceType> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<TraceType> & buf,
    cfg_s_type<TraceType> /*type*/,
    TraceType x
);

parse_error parse(TraceType & x, spec_type<TraceType> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<KeyboardInputMaskingLevel> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<KeyboardInputMaskingLevel> & buf,
    cfg_s_type<KeyboardInputMaskingLevel> /*type*/,
    KeyboardInputMaskingLevel x
);

parse_error parse(KeyboardInputMaskingLevel & x, spec_type<KeyboardInputMaskingLevel> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<SessionProbeOnLaunchFailure> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<SessionProbeOnLaunchFailure> & buf,
    cfg_s_type<SessionProbeOnLaunchFailure> /*type*/,
    SessionProbeOnLaunchFailure x
);

parse_error parse(SessionProbeOnLaunchFailure & x, spec_type<SessionProbeOnLaunchFailure> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<VncBogusClipboardInfiniteLoop> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<VncBogusClipboardInfiniteLoop> & buf,
    cfg_s_type<VncBogusClipboardInfiniteLoop> /*type*/,
    VncBogusClipboardInfiniteLoop x
);

parse_error parse(VncBogusClipboardInfiniteLoop & x, spec_type<VncBogusClipboardInfiniteLoop> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ColorDepthSelectionStrategy> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ColorDepthSelectionStrategy> & buf,
    cfg_s_type<ColorDepthSelectionStrategy> /*type*/,
    ColorDepthSelectionStrategy x
);

parse_error parse(ColorDepthSelectionStrategy & x, spec_type<ColorDepthSelectionStrategy> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<WrmCompressionAlgorithm> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<WrmCompressionAlgorithm> & buf,
    cfg_s_type<WrmCompressionAlgorithm> /*type*/,
    WrmCompressionAlgorithm x
);

parse_error parse(WrmCompressionAlgorithm & x, spec_type<WrmCompressionAlgorithm> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<RdpCompression> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<RdpCompression> & buf,
    cfg_s_type<RdpCompression> /*type*/,
    RdpCompression x
);

parse_error parse(RdpCompression & x, spec_type<RdpCompression> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<OcrLocale> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<OcrLocale> & buf,
    cfg_s_type<OcrLocale> /*type*/,
    OcrLocale x
);

parse_error parse(OcrLocale & x, spec_type<OcrLocale> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<BogusNumberOfFastpathInputEvent> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<BogusNumberOfFastpathInputEvent> & buf,
    cfg_s_type<BogusNumberOfFastpathInputEvent> /*type*/,
    BogusNumberOfFastpathInputEvent x
);

parse_error parse(BogusNumberOfFastpathInputEvent & x, spec_type<BogusNumberOfFastpathInputEvent> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<SessionProbeOnKeepaliveTimeout> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<SessionProbeOnKeepaliveTimeout> & buf,
    cfg_s_type<SessionProbeOnKeepaliveTimeout> /*type*/,
    SessionProbeOnKeepaliveTimeout x
);

parse_error parse(SessionProbeOnKeepaliveTimeout & x, spec_type<SessionProbeOnKeepaliveTimeout> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<SmartVideoCropping> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<SmartVideoCropping> & buf,
    cfg_s_type<SmartVideoCropping> /*type*/,
    SmartVideoCropping x
);

parse_error parse(SmartVideoCropping & x, spec_type<SmartVideoCropping> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<RdpModeConsole> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<RdpModeConsole> & buf,
    cfg_s_type<RdpModeConsole> /*type*/,
    RdpModeConsole x
);

parse_error parse(RdpModeConsole & x, spec_type<RdpModeConsole> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<SessionProbeDisabledFeature> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<SessionProbeDisabledFeature> & buf,
    cfg_s_type<SessionProbeDisabledFeature> /*type*/,
    SessionProbeDisabledFeature x
);

parse_error parse(SessionProbeDisabledFeature & x, spec_type<SessionProbeDisabledFeature> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<RdpFileRecord> : zstr_buffer_traits<void> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<RdpFileRecord> & buf,
    cfg_s_type<RdpFileRecord> /*type*/,
    RdpFileRecord x
);

parse_error parse(RdpFileRecord & x, spec_type<RdpFileRecord> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<SessionProbeOnAccountManipulation> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<SessionProbeOnAccountManipulation> & buf,
    cfg_s_type<SessionProbeOnAccountManipulation> /*type*/,
    SessionProbeOnAccountManipulation x
);

parse_error parse(SessionProbeOnAccountManipulation & x, spec_type<SessionProbeOnAccountManipulation> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<ColorDepth> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<ColorDepth> & buf,
    cfg_s_type<ColorDepth> /*type*/,
    ColorDepth x
);

parse_error parse(ColorDepth & x, spec_type<ColorDepth> /*type*/, array_view_const_char value)
;

template<> struct zstr_buffer_traits<OcrVersion> : zstr_buffer_traits<unsigned long> {};

array_view_const_char assign_zbuf_from_cfg(
    zstr_buffer_from<OcrVersion> & buf,
    cfg_s_type<OcrVersion> /*type*/,
    OcrVersion x
);

parse_error parse(OcrVersion & x, spec_type<OcrVersion> /*type*/, array_view_const_char value)
;

} // namespace configs
