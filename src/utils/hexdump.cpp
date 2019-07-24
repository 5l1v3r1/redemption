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
Author(s): Christophe Grosjean, Jonathan Poelen
*/

#include "utils/hexdump.hpp"
#include "utils/log.hpp"

#include <cstdio> // std::sprintf
#include <cstring> // strlen

namespace {

// $line_prefix "%.4x" $sep_page_values ($value_prefix "%.2x" $value_suffix) $sep_value_chars "%*c" $prefix_chars ("%c")
void hexdump_impl(
    const unsigned char * data, size_t size,
    char const * line_prefix, char const * sep_page_values,
    char const * value_prefix, char const * value_suffix,
    char const * sep_value_chars, char const * prefix_chars)
{
    constexpr unsigned line_length = 16;
    char buffer[2048];
    size_t const sep_len = strlen(value_prefix) + strlen(value_suffix) + 2;
    for (size_t j = 0; j < size; j += line_length){
        char * line = buffer;
        line += std::sprintf(line, "%s%.4x%s",
            line_prefix, static_cast<unsigned>(j), sep_page_values);

        size_t i;

        for (i = 0; i < line_length && j+i < size; i++){
            line += std::sprintf(line, "%s%.2x%s",
                value_prefix, static_cast<unsigned>(data[j+i]), value_suffix);
        }

        line += std::sprintf(line, "%s", sep_value_chars);
        if (i < line_length){
            line += std::sprintf(line, "%*c", static_cast<int>((line_length-i)*sep_len), ' ');
        }
        line += std::sprintf(line, "%s", prefix_chars);

        for (i = 0; i < line_length && j+i < size; i++){
            unsigned char tmp = data[j+i];
            if (tmp < ' ' || tmp > '~' || tmp == '\\'){
                tmp = '.';
            }
            line += std::sprintf(line, "%c", tmp);
        }

        if (line != buffer){
            line[0] = 0;
            LOG(LOG_INFO, "%s", buffer);
        }
    }
}

} // namespace


void hexdump(const_byte_ptr data, size_t size)
{
    // %.4x %x %x ... %c%c..
    hexdump_impl(data.as_u8p(), size, "", " ", "", " ", "", "");
}

void hexdump_av(const_bytes_view data)
{
    hexdump(data.as_u8p(), data.size());
}


void hexdump_d(const_byte_ptr data, size_t size)
{
    // /* %.4x */ 0x%x 0x%x ... // %c%c..
    hexdump_impl(data.as_u8p(), size, "/* ", " */ ", "0x", ", ", "", " // ");
}

void hexdump_av_d(const_bytes_view data)
{
    hexdump_d(data.as_u8p(), data.size());
}


void hexdump_c(const_byte_ptr data, size_t size)
{
    // /* %.4x */ "\x%x\x%x ..." // %c%c..
    hexdump_impl(data.as_u8p(), size, "/* ", " */ \"", "\\x", "", "\"", " // ");
}

void hexdump_av_c(const_bytes_view data)
{
    hexdump_c(data.as_u8p(), data.size());
}
