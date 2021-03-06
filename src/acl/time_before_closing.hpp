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
  Copyright (C) Wallix 2019
  Author(s): Christophe Grosjean

*/

#pragma once
#include "utils/sugar/algostring.hpp"

inline std::string time_before_closing(uint32_t elapsed_time, Translator tr)
{
    std::string msg;

    const auto hours = elapsed_time / (60*60);
    const auto minutes = elapsed_time / 60 - hours * 60;
    const auto seconds = elapsed_time - hours * (60*60) - minutes * 60;

    if (hours) {
        str_append(
            msg,
            std::to_string(hours),
            ' ',
            tr(trkeys::hour),
            (hours > 1) ? "s, " : ", "
        );
    }

    if (minutes || hours) {
        str_append(
            msg,
            std::to_string(minutes),
            ' ',
            tr(trkeys::minute),
            (minutes > 1) ? "s, " : ", "
        );
    }

    str_append(
        msg,
        std::to_string(seconds),
        ' ',
        tr(trkeys::second),
        (seconds > 1) ? "s " : " ",
        tr(trkeys::before_closing)
    );
    return msg;
}

