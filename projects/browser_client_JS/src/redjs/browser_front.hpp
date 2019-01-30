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
Copyright (C) Wallix 2010-2019
Author(s): Jonathan Poelen
*/

#pragma once

#include "core/channel_list.hpp"
#include "core/front_api.hpp"
#include "gdi/screen_info.hpp"

namespace redjs
{

class BrowserFront : public FrontAPI
{
public:
    BrowserFront(ScreenInfo& screen_info, bool verbose);

    bool can_be_start_capture() override;
    bool must_be_stop_capture() override;

    void draw(RDPOpaqueRect const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;

    void draw(const RDPScrBlt & cmd, Rect clip) override;
    void draw(const RDPDestBlt & cmd, Rect clip) override;
    void draw(const RDPMultiDstBlt & cmd, Rect clip) override;
    void draw(RDPMultiOpaqueRect const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDP::RDPMultiPatBlt const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(const RDP::RDPMultiScrBlt & cmd, Rect clip) override;
    void draw(RDPPatBlt const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;

    void draw(const RDPMemBlt & cmd, Rect clip, const Bitmap & bmp) override;

    void draw(RDPMem3Blt const & cmd, Rect clip, gdi::ColorCtx color_ctx, const Bitmap & bmp) override;
    void draw(RDPLineTo const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDPGlyphIndex const & cmd, Rect clip, gdi::ColorCtx color_ctx, const GlyphCache & gly_cache) override;
    void draw(RDPPolygonSC const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDPPolygonCB const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDPPolyline const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDPEllipseSC const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(RDPEllipseCB const & cmd, Rect clip, gdi::ColorCtx color_ctx) override;
    void draw(const RDPColCache   & /*unused*/) override;
    void draw(const RDPBrushCache & /*unused*/) override;
    void draw(const RDP::FrameMarker & cmd) override;
    void draw(const RDP::RAIL::NewOrExistingWindow & /*unused*/) override;
    void draw(const RDP::RAIL::WindowIcon & /*unused*/) override;
    void draw(const RDP::RAIL::CachedIcon & /*unused*/) override;
    void draw(const RDP::RAIL::DeletedWindow & /*unused*/) override;
    void draw(const RDP::RAIL::NewOrExistingNotificationIcons & /*unused*/) override;
    void draw(const RDP::RAIL::DeletedNotificationIcons & /*unused*/) override;
    void draw(const RDP::RAIL::ActivelyMonitoredDesktop & /*unused*/) override;
    void draw(const RDP::RAIL::NonMonitoredDesktop & /*unused*/) override;

    void draw(const RDPBitmapData & cmd, const Bitmap & bmp) override;

    void set_palette(const BGRPalette& /*unused*/) override;
    void draw(RDPNineGrid const &  /*unused*/, Rect  /*unused*/, gdi::ColorCtx  /*unused*/, Bitmap const & /*unused*/) override;
    void draw(RDPSetSurfaceCommand const & cmd, RDPSurfaceContent const & /*content*/) override;

    ResizeResult server_resize(int width, int height, BitsPerPixel bpp) override;

    void set_pointer(const Pointer & /*unused*/) override;

    void begin_update() override;
    void end_update() override;

    const CHANNELS::ChannelDefArray & get_channel_list() const override
    {
        return cl;
    }

    void send_to_channel(
        const CHANNELS::ChannelDef & /*channel*/, const uint8_t * /*data*/,
        std::size_t /*length*/, std::size_t /*chunk_size*/, int /*flags*/) override;

    void update_pointer_position(uint16_t /*unused*/, uint16_t /*unused*/) override;

private:
    uint16_t width;
    uint16_t height;
    bool verbose;
    ScreenInfo& screen_info;
    CHANNELS::ChannelDefArray cl;
};

}
