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
Copyright (C) Wallix 2017
Author(s): Jonathan Poelen
*/

#pragma once

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "core/session_reactor.hpp"
#include "mod/mod_api.hpp"
#include "mod/mod_api.hpp"

inline void execute_graphics_event(
    SessionReactor& session_reactor, GraphicFdContainer & graphic_fd_events_, GraphicEventContainer & graphic_events_, gdi::GraphicApi& gd)
{
    session_reactor.execute_graphics(graphic_fd_events_, graphic_events_, [](int /*fd*/, auto& /*e*/){ return true; }, gd);
}

inline void execute_negociate_mod(
    SessionReactor& session_reactor, TopFdContainer & fd_events_, GraphicFdContainer & graphic_fd_events_, TimerContainer& timer_events_, GraphicEventContainer & graphic_events_, GraphicTimerContainer & graphic_timer_events_, mod_api& mod, gdi::GraphicApi& gd)
{
    session_reactor.execute_timers(
        fd_events_,
        graphic_fd_events_,
        timer_events_,
        graphic_timer_events_,
        EnableGraphics{true},
        [&]()->gdi::GraphicApi&{ return gd; });
    int n = 0;
    int const limit = 1000;
    while (!mod.is_up_and_running()
        && (!graphic_events_.is_empty() || !graphic_fd_events_.is_empty())
        && (++n < limit)
    ) {
        execute_graphics_event(session_reactor, graphic_fd_events_, graphic_events_, gd);
    }
    RED_REQUIRE_LT(n, limit);
}

inline void execute_mod(SessionReactor& session_reactor, TopFdContainer & fd_events_, GraphicFdContainer & graphic_fd_events_, TimerContainer& timer_events_, GraphicEventContainer & graphic_events_, GraphicTimerContainer & graphic_timer_events_, mod_api& mod, gdi::GraphicApi& gd, int n)
{
    execute_negociate_mod(session_reactor, fd_events_, graphic_fd_events_, timer_events_, graphic_events_, graphic_timer_events_, mod, gd);
    int count = 0;
    for (; count < n && (!graphic_events_.is_empty() || !graphic_fd_events_.is_empty()); ++count) {
        // LOG(LOG_INFO, "===================> count = %u", count);
        execute_graphics_event(session_reactor, graphic_fd_events_, graphic_events_, gd);
    }
    RED_CHECK_EQ(count, n);
}
