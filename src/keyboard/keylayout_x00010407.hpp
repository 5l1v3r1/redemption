/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean, Dominique Lafages
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   header file. Keylayout object, used by keymap managers
*/


#pragma once

#include "keylayout.hpp"

namespace x00010407 {    // German (Germany) // German (IBM)

const static int LCID = 0x00010407;

const static char * const locale_name = "de-DE.ibm";

const Keylayout::KeyLayout_t noMod = {
    /* x00 - x07 */    0x0000, 0x001B,    '1',    '2',    '3',    '4',    '5',    '6',
    /* x08 - x0F */       '7',    '8',    '9',    '0', 0x00DF, 0x00B4, 0x0008, 0x0009,
    /* x10 - x17 */       'q',    'w',    'e',    'r',    't',    'z',    'u',    'i',
    /* x18 - x1F */       'o',    'p', 0x00FC, 0x002B, 0x000D, 0x0000,    'a',    's',
    /* x20 - x27 */       'd',    'f',    'g',    'h',    'j',    'k',    'l', 0x00F6,
    /* x28 - x2F */    0x00E4, 0x005E, 0x0000, 0x0023,    'y',    'x',    'c',    'v',
    /* x30 - x37 */       'b',    'n',    'm', 0x002C, 0x002E, 0x002D, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '7',
    /* x48 - x4F */       '8',    '9',    '-',    '4',    '5',    '6',    '+',    '1',
    /* x50 - x57 */       '2',    '3',    '0', 0x002C, 0x0000, 0x0000, 0x003C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t shift = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0021, 0x0022, 0x00A7, 0x0024, 0x0025, 0x0026,
    /* x08 - x0F */    0x002F, 0x0028, 0x0029, 0x003D, 0x003F, 0x0060, 0x0008, 0x0000,
    /* x10 - x17 */       'Q',    'W',    'E',    'R',    'T',    'Z',    'U',    'I',
    /* x18 - x1F */       'O',    'P', 0x00DC, 0x002A, 0x000D, 0x0000,    'A',    'S',
    /* x20 - x27 */       'D',    'F',    'G',    'H',    'J',    'K',    'L', 0x00D6,
    /* x28 - x2F */    0x00C4, 0x00B0, 0x0000, 0x0027,    'Y',    'X',    'C',    'V',
    /* x30 - x37 */       'B',    'N',    'M', 0x003B, 0x003A, 0x005F, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x003E, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000, 0x0000, 0x007F, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */       '/', 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t altGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x00B2, 0x00B3, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x005C, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0040, 0x0000, 0x20AC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x0000, 0x007E, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t shiftAltGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x00B2, 0x00B3, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x005C, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x0000, 0x007E, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t ctrl = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x001B, 0x001D, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x001C, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t capslock_noMod = {
    /* x00 - x07 */    0x0000, 0x001B,    '1',    '2',    '3',    '4',    '5',    '6',
    /* x08 - x0F */       '7',    '8',    '9',    '0', 0x00DF, 0x00B4, 0x0008, 0x0009,
    /* x10 - x17 */       'Q',    'W',    'E',    'R',    'T',    'Z',    'U',    'I',
    /* x18 - x1F */       'O',    'P', 0x00DC, 0x002B, 0x000D, 0x0000,    'A',    'S',
    /* x20 - x27 */       'D',    'F',    'G',    'H',    'J',    'K',    'L', 0x00D6,
    /* x28 - x2F */    0x00C4, 0x005E, 0x0000, 0x0023,    'Y',    'X',    'C',    'V',
    /* x30 - x37 */       'B',    'N',    'M', 0x002C, 0x002E, 0x002D, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x003C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t capslock_shift = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0021, 0x0022, 0x00A7, 0x0024, 0x0025, 0x0026,
    /* x08 - x0F */    0x002F, 0x0028, 0x0029, 0x003D, 0x003F, 0x0060, 0x0008, 0x0009,
    /* x10 - x17 */       'q',    'w',    'e',    'r',    't',    'z',    'u',    'i',
    /* x18 - x1F */       'o',    'p', 0x00FC, 0x002A, 0x000D, 0x0000,    'a',    's',
    /* x20 - x27 */       'd',    'f',    'g',    'h',    'j',    'k',    'l', 0x00F6,
    /* x28 - x2F */    0x00E4, 0x00B0, 0x0000, 0x0027,    'y',    'x',    'c',    'v',
    /* x30 - x37 */       'b',    'n',    'm', 0x003B, 0x003A, 0x005F, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x003E, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t capslock_altGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x00B2, 0x00B3, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x005C, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0040, 0x0000, 0x20AC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x0000, 0x007E, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::KeyLayout_t capslock_shiftAltGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x00B2, 0x00B3, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x005C, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x0000, 0x007E, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x007C, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const Keylayout::dkey_t deadkeys[] = {
    { 0x00b4, 0x0d, 13, { {0x006f, 0x00f3}  // 'o' = 'ó'
                        , {0x0049, 0x00cd}  // 'I' = 'Í'
                        , {0x0059, 0x00dd}  // 'Y' = 'Ý'
                        , {0x0041, 0x00c1}  // 'A' = 'Á'
                        , {0x0045, 0x00c9}  // 'E' = 'É'
                        , {0x0055, 0x00da}  // 'U' = 'Ú'
                        , {0x0020, 0x00b4}  // ' ' = '´'
                        , {0x0065, 0x00e9}  // 'e' = 'é'
                        , {0x0075, 0x00fa}  // 'u' = 'ú'
                        , {0x0061, 0x00e1}  // 'a' = 'á'
                        , {0x0079, 0x00fd}  // 'y' = 'ý'
                        , {0x004f, 0x00d3}  // 'O' = 'Ó'
                        , {0x0069, 0x00ed}  // 'i' = 'í'
                        }
    },
    { 0x0060, 0x0d, 11, { {0x006f, 0x00f2}  // 'o' = 'ò'
                        , {0x0049, 0x00cc}  // 'I' = 'Ì'
                        , {0x0020, 0x0060}  // ' ' = '`'
                        , {0x0041, 0x00c0}  // 'A' = 'À'
                        , {0x0045, 0x00c8}  // 'E' = 'È'
                        , {0x0055, 0x00d9}  // 'U' = 'Ù'
                        , {0x0065, 0x00e8}  // 'e' = 'è'
                        , {0x0075, 0x00f9}  // 'u' = 'ù'
                        , {0x0061, 0x00e0}  // 'a' = 'à'
                        , {0x004f, 0x00d2}  // 'O' = 'Ò'
                        , {0x0069, 0x00ec}  // 'i' = 'ì'
                        }
    },
    { 0x005e, 0x29, 11, { {0x006f, 0x00f4}  // 'o' = 'ô'
                        , {0x0049, 0x00ce}  // 'I' = 'Î'
                        , {0x0020, 0x005e}  // ' ' = '^'
                        , {0x0041, 0x00c2}  // 'A' = 'Â'
                        , {0x0045, 0x00ca}  // 'E' = 'Ê'
                        , {0x0055, 0x00db}  // 'U' = 'Û'
                        , {0x0065, 0x00ea}  // 'e' = 'ê'
                        , {0x0075, 0x00fb}  // 'u' = 'û'
                        , {0x0061, 0x00e2}  // 'a' = 'â'
                        , {0x004f, 0x00d4}  // 'O' = 'Ô'
                        , {0x0069, 0x00ee}  // 'i' = 'î'
                        }
    },
};

const static uint8_t nbDeadkeys = 3;

} // END NAMESPACE - x00010407

static const Keylayout keylayout_x00010407( x00010407::LCID
                                          , x00010407::locale_name
                                          , x00010407::noMod
                                          , x00010407::shift
                                          , x00010407::altGr
                                          , x00010407::shiftAltGr
                                          , x00010407::ctrl
                                          , x00010407::capslock_noMod
                                          , x00010407::capslock_shift
                                          , x00010407::capslock_altGr
                                          , x00010407::capslock_shiftAltGr
                                          , x00010407::deadkeys
                                          , x00010407::nbDeadkeys
);

