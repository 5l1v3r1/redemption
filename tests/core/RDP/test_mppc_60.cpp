/*
    This program is free software; you can redistribute it and/or modify it
     under the terms of the GNU General Public License as published by the
     Free Software Foundation; either version 2 of the License, or (at your
     option) any later version.

    This program is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
     Public License for more details.

    You should have received a copy of the GNU General Public License along
     with this program; if not, write to the Free Software Foundation, Inc.,
     675 Mass Ave, Cambridge, MA 02139, USA.

    Product name: redemption, a FLOSS RDP proxy
    Copyright (C) Wallix 2013
    Author(s): Christophe Grosjean, Raphael Zhou
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestMPPC60
#include "system/redemption_unit_tests.hpp"

#define LOGNULL
//#define LOGPRINT

#include "core/RDP/mppc_60.hpp"

RED_AUTO_TEST_CASE(TestRDP60BlukCompression)
{
    rdp_mppc_60_enc mppc_enc;

    uint8_t uncompressed_data[] = {
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00,
        0x20, 0x00, 0x20, 0x00, 0x80, 0x00, 0x80, 0x00
    };

    uint8_t  compressionFlags;
    uint16_t datalen;
    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    RED_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));

//     LOG(LOG_INFO, "bytes_in_opb=%d", mppc_enc.bytes_in_opb);
//     hexdump_d(mppc_enc.outputBuffer, mppc_enc.bytes_in_opb);

/*
    int bits_left = 8;
    int opb_index = 0;

    char outputBuffer[1024];
    memset(outputBuffer, 0, sizeof(outputBuffer));
    rdp_mppc_60_enc::insert_n_bits(6, 0x24, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(6, 0x04, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(7, 0x39, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(4, 0x01, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(7, 0x69, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(7, 0x79, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(2, 0x00, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(8, 0x4d, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(5, 0x18, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(2, 0x00, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(7, 0x19, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(5, 0x18, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(2, 0x00, outputBuffer, bits_left, opb_index);
    rdp_mppc_60_enc::insert_n_bits(13, 0x17ff, outputBuffer, bits_left, opb_index);

    hexdump_d(outputBuffer, opb_index + (bits_left != 8));
*/
}

RED_AUTO_TEST_CASE(TestRDP60BlukCompression2)
{
    rdp_mppc_60_enc mppc_enc;

    uint8_t uncompressed_data[] = {
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00,
        0x20, 0x00, 0x00, 0x20, 0x00, 0x80, 0x00, 0x80, 0x00
    };

    uint8_t  compressionFlags;
    uint16_t datalen;
    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    RED_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));

    //LOG(LOG_INFO, "bytes_in_opb=%d", mppc_enc.bytes_in_opb);
    //hexdump_d(mppc_enc.outputBuffer, mppc_enc.bytes_in_opb);
}

RED_AUTO_TEST_CASE(TestRDP60BlukCompression3)
{
    rdp_mppc_60_enc mppc_enc;

    uint8_t uncompressed_data[] = {
/* 0000 */ 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00,  // ................
/* 0010 */ 0x01, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,  // .....U..........
/* 0020 */ 0x00, 0x00, 0x24, 0x00, 0x00, 0x55, 0x24, 0x00, 0x00, 0xaa, 0x24, 0x00, 0x00, 0xff, 0x24, 0x00,  // ..$..U$...$...$.
/* 0030 */ 0x00, 0x00, 0x49, 0x00, 0x00, 0x55, 0x49, 0x00, 0x00, 0xaa, 0x49, 0x00, 0x00, 0xff, 0x49, 0x00,  // ..I..UI...I...I.
/* 0040 */ 0x00, 0x00, 0x6d, 0x00, 0x00, 0x55, 0x6d, 0x00, 0x00, 0xaa, 0x6d, 0x00, 0x00, 0xff, 0x6d, 0x00,  // ..m..Um...m...m.
/* 0050 */ 0x00, 0x00, 0x92, 0x00, 0x00, 0x55, 0x92, 0x00, 0x00, 0xaa, 0x92, 0x00, 0x00, 0xff, 0x92, 0x00,  // .....U..........
/* 0060 */ 0x00, 0x00, 0xb6, 0x00, 0x00, 0x55, 0xb6, 0x00, 0x00, 0xaa, 0xb6, 0x00, 0x00, 0xff, 0xb6, 0x00,  // .....U..........
/* 0070 */ 0x00, 0x00, 0xdb, 0x00, 0x00, 0x55, 0xdb, 0x00, 0x00, 0xaa, 0xdb, 0x00, 0x00, 0xff, 0xdb, 0x00,  // .....U..........
/* 0080 */ 0x00, 0x00, 0xff, 0x00, 0x00, 0x55, 0xff, 0x00, 0x00, 0xaa, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,  // .....U..........
/* 0090 */ 0x00, 0x00, 0x00, 0x24, 0x00, 0x55, 0x00, 0x24, 0x00, 0xaa, 0x00, 0x24, 0x00, 0xff, 0x00, 0x24,  // ...$.U.$...$...$
/* 00a0 */ 0x00, 0x00, 0x24, 0x24, 0x00, 0x55, 0x24, 0x24, 0x00, 0xaa, 0x24, 0x24, 0x00, 0xff, 0x24, 0x24,  // ..$$.U$$..$$..$$
/* 00b0 */ 0x00, 0x00, 0x49, 0x24, 0x00, 0x55, 0x49, 0x24, 0x00, 0xaa, 0x49, 0x24, 0x00, 0xff, 0x49, 0x24,  // ..I$.UI$..I$..I$
/* 00c0 */ 0x00, 0x00, 0x6d, 0x24, 0x00, 0x55, 0x6d, 0x24, 0x00, 0xaa, 0x6d, 0x24, 0x00, 0xff, 0x6d, 0x24,  // ..m$.Um$..m$..m$
/* 00d0 */ 0x00, 0x00, 0x92, 0x24, 0x00, 0x55, 0x92, 0x24, 0x00, 0xaa, 0x92, 0x24, 0x00, 0xff, 0x92, 0x24,  // ...$.U.$...$...$
/* 00e0 */ 0x00, 0x00, 0xb6, 0x24, 0x00, 0x55, 0xb6, 0x24, 0x00, 0xaa, 0xb6, 0x24, 0x00, 0xff, 0xb6, 0x24,  // ...$.U.$...$...$
/* 00f0 */ 0x00, 0x00, 0xdb, 0x24, 0x00, 0x55, 0xdb, 0x24, 0x00, 0xaa, 0xdb, 0x24, 0x00, 0xff, 0xdb, 0x24,  // ...$.U.$...$...$
/* 0100 */ 0x00, 0x00, 0xff, 0x24, 0x00, 0x55, 0xff, 0x24, 0x00, 0xaa, 0xff, 0x24, 0x00, 0xff, 0xff, 0x24,  // ...$.U.$...$...$
/* 0110 */ 0x00, 0x00, 0x00, 0x49, 0x00, 0x55, 0x00, 0x49, 0x00, 0xaa, 0x00, 0x49, 0x00, 0xff, 0x00, 0x49,  // ...I.U.I...I...I
/* 0120 */ 0x00, 0x00, 0x24, 0x49, 0x00, 0x55, 0x24, 0x49, 0x00, 0xaa, 0x24, 0x49, 0x00, 0xff, 0x24, 0x49,  // ..$I.U$I..$I..$I
/* 0130 */ 0x00, 0x00, 0x49, 0x49, 0x00, 0x55, 0x49, 0x49, 0x00, 0xaa, 0x49, 0x49, 0x00, 0xff, 0x49, 0x49,  // ..II.UII..II..II
/* 0140 */ 0x00, 0x00, 0x6d, 0x49, 0x00, 0x55, 0x6d, 0x49, 0x00, 0xaa, 0x6d, 0x49, 0x00, 0xff, 0x6d, 0x49,  // ..mI.UmI..mI..mI
/* 0150 */ 0x00, 0x00, 0x92, 0x49, 0x00, 0x55, 0x92, 0x49, 0x00, 0xaa, 0x92, 0x49, 0x00, 0xff, 0x92, 0x49,  // ...I.U.I...I...I
/* 0160 */ 0x00, 0x00, 0xb6, 0x49, 0x00, 0x55, 0xb6, 0x49, 0x00, 0xaa, 0xb6, 0x49, 0x00, 0xff, 0xb6, 0x49,  // ...I.U.I...I...I
/* 0170 */ 0x00, 0x00, 0xdb, 0x49, 0x00, 0x55, 0xdb, 0x49, 0x00, 0xaa, 0xdb, 0x49, 0x00, 0xff, 0xdb, 0x49,  // ...I.U.I...I...I
/* 0180 */ 0x00, 0x00, 0xff, 0x49, 0x00, 0x55, 0xff, 0x49, 0x00, 0xaa, 0xff, 0x49, 0x00, 0xff, 0xff, 0x49,  // ...I.U.I...I...I
/* 0190 */ 0x00, 0x00, 0x00, 0x6d, 0x00, 0x55, 0x00, 0x6d, 0x00, 0xaa, 0x00, 0x6d, 0x00, 0xff, 0x00, 0x6d,  // ...m.U.m...m...m
/* 01a0 */ 0x00, 0x00, 0x24, 0x6d, 0x00, 0x55, 0x24, 0x6d, 0x00, 0xaa, 0x24, 0x6d, 0x00, 0xff, 0x24, 0x6d,  // ..$m.U$m..$m..$m
/* 01b0 */ 0x00, 0x00, 0x49, 0x6d, 0x00, 0x55, 0x49, 0x6d, 0x00, 0xaa, 0x49, 0x6d, 0x00, 0xff, 0x49, 0x6d,  // ..Im.UIm..Im..Im
/* 01c0 */ 0x00, 0x00, 0x6d, 0x6d, 0x00, 0x55, 0x6d, 0x6d, 0x00, 0xaa, 0x6d, 0x6d, 0x00, 0xff, 0x6d, 0x6d,  // ..mm.Umm..mm..mm
/* 01d0 */ 0x00, 0x00, 0x92, 0x6d, 0x00, 0x55, 0x92, 0x6d, 0x00, 0xaa, 0x92, 0x6d, 0x00, 0xff, 0x92, 0x6d,  // ...m.U.m...m...m
/* 01e0 */ 0x00, 0x00, 0xb6, 0x6d, 0x00, 0x55, 0xb6, 0x6d, 0x00, 0xaa, 0xb6, 0x6d, 0x00, 0xff, 0xb6, 0x6d,  // ...m.U.m...m...m
/* 01f0 */ 0x00, 0x00, 0xdb, 0x6d, 0x00, 0x55, 0xdb, 0x6d, 0x00, 0xaa, 0xdb, 0x6d, 0x00, 0xff, 0xdb, 0x6d,  // ...m.U.m...m...m
/* 0200 */ 0x00, 0x00, 0xff, 0x6d, 0x00, 0x55, 0xff, 0x6d, 0x00, 0xaa, 0xff, 0x6d, 0x00, 0xff, 0xff, 0x6d,  // ...m.U.m...m...m
/* 0210 */ 0x00, 0x00, 0x00, 0x92, 0x00, 0x55, 0x00, 0x92, 0x00, 0xaa, 0x00, 0x92, 0x00, 0xff, 0x00, 0x92,  // .....U..........
/* 0220 */ 0x00, 0x00, 0x24, 0x92, 0x00, 0x55, 0x24, 0x92, 0x00, 0xaa, 0x24, 0x92, 0x00, 0xff, 0x24, 0x92,  // ..$..U$...$...$.
/* 0230 */ 0x00, 0x00, 0x49, 0x92, 0x00, 0x55, 0x49, 0x92, 0x00, 0xaa, 0x49, 0x92, 0x00, 0xff, 0x49, 0x92,  // ..I..UI...I...I.
/* 0240 */ 0x00, 0x00, 0x6d, 0x92, 0x00, 0x55, 0x6d, 0x92, 0x00, 0xaa, 0x6d, 0x92, 0x00, 0xff, 0x6d, 0x92,  // ..m..Um...m...m.
/* 0250 */ 0x00, 0x00, 0x92, 0x92, 0x00, 0x55, 0x92, 0x92, 0x00, 0xaa, 0x92, 0x92, 0x00, 0xff, 0x92, 0x92,  // .....U..........
/* 0260 */ 0x00, 0x00, 0xb6, 0x92, 0x00, 0x55, 0xb6, 0x92, 0x00, 0xaa, 0xb6, 0x92, 0x00, 0xff, 0xb6, 0x92,  // .....U..........
/* 0270 */ 0x00, 0x00, 0xdb, 0x92, 0x00, 0x55, 0xdb, 0x92, 0x00, 0xaa, 0xdb, 0x92, 0x00, 0xff, 0xdb, 0x92,  // .....U..........
/* 0280 */ 0x00, 0x00, 0xff, 0x92, 0x00, 0x55, 0xff, 0x92, 0x00, 0xaa, 0xff, 0x92, 0x00, 0xff, 0xff, 0x92,  // .....U..........
/* 0290 */ 0x00, 0x00, 0x00, 0xb6, 0x00, 0x55, 0x00, 0xb6, 0x00, 0xaa, 0x00, 0xb6, 0x00, 0xff, 0x00, 0xb6,  // .....U..........
/* 02a0 */ 0x00, 0x00, 0x24, 0xb6, 0x00, 0x55, 0x24, 0xb6, 0x00, 0xaa, 0x24, 0xb6, 0x00, 0xff, 0x24, 0xb6,  // ..$..U$...$...$.
/* 02b0 */ 0x00, 0x00, 0x49, 0xb6, 0x00, 0x55, 0x49, 0xb6, 0x00, 0xaa, 0x49, 0xb6, 0x00, 0xff, 0x49, 0xb6,  // ..I..UI...I...I.
/* 02c0 */ 0x00, 0x00, 0x6d, 0xb6, 0x00, 0x55, 0x6d, 0xb6, 0x00, 0xaa, 0x6d, 0xb6, 0x00, 0xff, 0x6d, 0xb6,  // ..m..Um...m...m.
/* 02d0 */ 0x00, 0x00, 0x92, 0xb6, 0x00, 0x55, 0x92, 0xb6, 0x00, 0xaa, 0x92, 0xb6, 0x00, 0xff, 0x92, 0xb6,  // .....U..........
/* 02e0 */ 0x00, 0x00, 0xb6, 0xb6, 0x00, 0x55, 0xb6, 0xb6, 0x00, 0xaa, 0xb6, 0xb6, 0x00, 0xff, 0xb6, 0xb6,  // .....U..........
/* 02f0 */ 0x00, 0x00, 0xdb, 0xb6, 0x00, 0x55, 0xdb, 0xb6, 0x00, 0xaa, 0xdb, 0xb6, 0x00, 0xff, 0xdb, 0xb6,  // .....U..........
/* 0300 */ 0x00, 0x00, 0xff, 0xb6, 0x00, 0x55, 0xff, 0xb6, 0x00, 0xaa, 0xff, 0xb6, 0x00, 0xff, 0xff, 0xb6,  // .....U..........
/* 0310 */ 0x00, 0x00, 0x00, 0xdb, 0x00, 0x55, 0x00, 0xdb, 0x00, 0xaa, 0x00, 0xdb, 0x00, 0xff, 0x00, 0xdb,  // .....U..........
/* 0320 */ 0x00, 0x00, 0x24, 0xdb, 0x00, 0x55, 0x24, 0xdb, 0x00, 0xaa, 0x24, 0xdb, 0x00, 0xff, 0x24, 0xdb,  // ..$..U$...$...$.
/* 0330 */ 0x00, 0x00, 0x49, 0xdb, 0x00, 0x55, 0x49, 0xdb, 0x00, 0xaa, 0x49, 0xdb, 0x00, 0xff, 0x49, 0xdb,  // ..I..UI...I...I.
/* 0340 */ 0x00, 0x00, 0x6d, 0xdb, 0x00, 0x55, 0x6d, 0xdb, 0x00, 0xaa, 0x6d, 0xdb, 0x00, 0xff, 0x6d, 0xdb,  // ..m..Um...m...m.
/* 0350 */ 0x00, 0x00, 0x92, 0xdb, 0x00, 0x55, 0x92, 0xdb, 0x00, 0xaa, 0x92, 0xdb, 0x00, 0xff, 0x92, 0xdb,  // .....U..........
/* 0360 */ 0x00, 0x00, 0xb6, 0xdb, 0x00, 0x55, 0xb6, 0xdb, 0x00, 0xaa, 0xb6, 0xdb, 0x00, 0xff, 0xb6, 0xdb,  // .....U..........
/* 0370 */ 0x00, 0x00, 0xdb, 0xdb, 0x00, 0x55, 0xdb, 0xdb, 0x00, 0xaa, 0xdb, 0xdb, 0x00, 0xff, 0xdb, 0xdb,  // .....U..........
/* 0380 */ 0x00, 0x00, 0xff, 0xdb, 0x00, 0x55, 0xff, 0xdb, 0x00, 0xaa, 0xff, 0xdb, 0x00, 0xff, 0xff, 0xdb,  // .....U..........
/* 0390 */ 0x00, 0x00, 0x00, 0xff, 0x00, 0x55, 0x00, 0xff, 0x00, 0xaa, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,  // .....U..........
/* 03a0 */ 0x00, 0x00, 0x24, 0xff, 0x00, 0x55, 0x24, 0xff, 0x00, 0xaa, 0x24, 0xff, 0x00, 0xff, 0x24, 0xff,  // ..$..U$...$...$.
/* 03b0 */ 0x00, 0x00, 0x49, 0xff, 0x00, 0x55, 0x49, 0xff, 0x00, 0xaa, 0x49, 0xff, 0x00, 0xff, 0x49, 0xff,  // ..I..UI...I...I.
/* 03c0 */ 0x00, 0x00, 0x6d, 0xff, 0x00, 0x55, 0x6d, 0xff, 0x00, 0xaa, 0x6d, 0xff, 0x00, 0xff, 0x6d, 0xff,  // ..m..Um...m...m.
/* 03d0 */ 0x00, 0x00, 0x92, 0xff, 0x00, 0x55, 0x92, 0xff, 0x00, 0xaa, 0x92, 0xff, 0x00, 0xff, 0x92, 0xff,  // .....U..........
/* 03e0 */ 0x00, 0x00, 0xb6, 0xff, 0x00, 0x55, 0xb6, 0xff, 0x00, 0xaa, 0xb6, 0xff, 0x00, 0xff, 0xb6, 0xff,  // .....U..........
/* 03f0 */ 0x00, 0x00, 0xdb, 0xff, 0x00, 0x55, 0xdb, 0xff, 0x00, 0xaa, 0xdb, 0xff, 0x00, 0xff, 0xdb, 0xff,  // .....U..........
/* 0400 */ 0x00, 0x00, 0xff, 0xff, 0x00, 0x55, 0xff, 0xff, 0x00, 0xaa, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,  // .....U..........
/* 0410 */ 0x00,                                               // .
    };

    uint8_t  compressionFlags;
    uint16_t datalen;
    mppc_enc.compress(uncompressed_data, sizeof(uncompressed_data), compressionFlags, datalen,
        rdp_mppc_enc::MAX_COMPRESSED_DATA_SIZE_UNUSED);

    int flags = PACKET_COMPRESSED;

    RED_CHECK_EQUAL(flags, (compressionFlags & PACKET_COMPRESSED));

//     LOG(LOG_INFO, "bytes_in_opb=%d", mppc_enc.bytes_in_opb);
//     hexdump_d(mppc_enc.outputBuffer, mppc_enc.bytes_in_opb);
}

RED_AUTO_TEST_CASE(TestRDP60BlukDecompression)
{
    rdp_mppc_60_dec mppc_dec;

    uint8_t compressed_data[] = {
        0x24, 0x91, 0x8b, 0x74, 0x9e, 0x26, 0x4c, 0x06,
        0xf3, 0x7f, 0x01
    };
    uint8_t uncompressed_data[] = {
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00,
        0x20, 0x00, 0x20, 0x00, 0x80, 0x00, 0x80, 0x00
    };

    uint8_t  compressionFlags = 0x22;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(compressed_data, sizeof(compressed_data),
        compressionFlags, rdata, rlen);

    RED_CHECK_EQUAL(sizeof(uncompressed_data), rlen);
    RED_CHECK_EQUAL(0,                         memcmp(uncompressed_data,
        rdata, rlen));

    //LOG(LOG_INFO, "rlen=%d", rlen);
    //hexdump_d(rdata, rlen);
}

RED_AUTO_TEST_CASE(TestRDP60BlukDecompression1)
{
    rdp_mppc_60_dec mppc_dec;

    uint8_t compressed_data[] = {
        0x24, 0x41, 0x10, 0xe9, 0x3c, 0x4d, 0x98, 0x0c,
        0xe6, 0xff, 0x02
    };
    uint8_t uncompressed_data[] = {
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00,
        0x20, 0x00, 0x20, 0x00, 0x80, 0x00, 0x80, 0x00
    };

    uint8_t  compressionFlags = 0x22;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(compressed_data, sizeof(compressed_data),
        compressionFlags, rdata, rlen);

    RED_CHECK_EQUAL(sizeof(uncompressed_data), rlen);
    RED_CHECK_EQUAL(0,                         memcmp(uncompressed_data,
        rdata, rlen));

    //LOG(LOG_INFO, "rlen=%d", rlen);
    //hexdump_d(rdata, rlen);
}

RED_AUTO_TEST_CASE(TestRDP60BlukDecompression2)
{
    rdp_mppc_60_dec mppc_dec;

    uint8_t compressed_data[] = {
        0x24, 0x41, 0x10, 0xe9, 0x3c, 0x4d, 0xc4, 0x7f,
        0x64, 0x42, 0xfe, 0x2f
    };
    uint8_t uncompressed_data[] = {
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00,
        0x20, 0x00, 0x00, 0x20, 0x00, 0x80, 0x00, 0x80, 0x00
    };

    uint8_t  compressionFlags = 0x22;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(compressed_data, sizeof(compressed_data),
        compressionFlags, rdata, rlen);

    RED_CHECK_EQUAL(sizeof(uncompressed_data), rlen);
    RED_CHECK_EQUAL(0,                         memcmp(uncompressed_data,
        rdata, rlen));

    //LOG(LOG_INFO, "rlen=%d", rlen);
    //hexdump_d(rdata, rlen);
}

RED_AUTO_TEST_CASE(TestRDP60BlukDecompression3)
{
    rdp_mppc_60_dec mppc_dec;

    uint8_t compressed_data[] = {
/* 0000 */ 0x04, 0x41, 0x10, 0x64, 0x21, 0x91, 0x55, 0x11, 0xba, 0xfc, 0x0f, 0x82, 0xd8, 0xbe, 0x80, 0x3b,  // .A.d!.U........;
/* 0010 */ 0x31, 0x59, 0x0c, 0xd1, 0x97, 0x05, 0x21, 0x77, 0x62, 0xb2, 0x18, 0x62, 0x4a, 0x48, 0xc8, 0x9d,  // 1Y....!wb..bJH..
/* 0020 */ 0x98, 0x2c, 0x86, 0x38, 0x34, 0x24, 0xe4, 0x4e, 0x4c, 0x16, 0x43, 0x5c, 0x18, 0x12, 0x72, 0x27,  // .,.84$.NL.C...r'
/* 0030 */ 0x26, 0x8b, 0x21, 0x1e, 0x0e, 0x09, 0xb9, 0x13, 0x93, 0xc5, 0x10, 0x9f, 0x84, 0x84, 0xdc, 0x89,  // &.!.............
/* 0040 */ 0xc9, 0x62, 0xc4, 0xa2, 0xed, 0x43, 0xee, 0xc4, 0x64, 0x31, 0x62, 0xd4, 0xf6, 0x21, 0x77, 0x62,  // .b...C..d1b..!wb
/* 0050 */ 0xb2, 0x18, 0xa2, 0x6f, 0x99, 0x32, 0x77, 0x62, 0xb2, 0x18, 0x62, 0x4a, 0x99, 0x32, 0x77, 0x62,  // ...o.2wb..bJ.2wb
/* 0060 */ 0xb2, 0x18, 0xe2, 0xd0, 0x32, 0x65, 0xee, 0xc4, 0x64, 0x31, 0xc4, 0x85, 0x65, 0xca, 0xdc, 0x89,  // ....2e..d1..e...
/* 0070 */ 0xc9, 0x62, 0x88, 0x87, 0xcb, 0x94, 0xb9, 0x13, 0x93, 0xc5, 0x10, 0x9f, 0x94, 0x29, 0x73, 0x27,  // .b...........)s'
/* 0080 */ 0x26, 0x8b, 0x11, 0x2a, 0x10, 0x80, 0x05, 0x65, 0xb2, 0x18, 0x31, 0x50, 0x62, 0x0a, 0x66, 0x0a,  // &..*...e..1Pb.f.
/* 0090 */ 0x46, 0x0a, 0xe9, 0xcb, 0x82, 0x32, 0x77, 0x62, 0xb2, 0x18, 0x62, 0x4a, 0x99, 0x32, 0x77, 0x62,  // F....2wb..bJ.2wb
/* 00a0 */ 0xb2, 0x18, 0xe2, 0xd0, 0x32, 0x65, 0xee, 0xc4, 0x64, 0x31, 0xc4, 0x85, 0x65, 0xca, 0xdc, 0x89,  // ....2e..d1..e...
/* 00b0 */ 0xc9, 0x62, 0x88, 0x87, 0xcb, 0x94, 0xb9, 0x13, 0x93, 0xc5, 0x10, 0x9f, 0x94, 0x29, 0x73, 0x27,  // .b...........)s'
/* 00c0 */ 0x26, 0x8b, 0x11, 0x0a, 0x65, 0x58, 0x50, 0x22, 0x8b, 0x11, 0x22, 0x25, 0x0e, 0xc5, 0x1c, 0x8a,  // &...eXP".."%....
/* 00d0 */ 0x41, 0x51, 0xfa, 0xb2, 0xa0, 0xcc, 0x9d, 0x98, 0x2c, 0x86, 0x98, 0x52, 0xa6, 0xcc, 0x9d, 0x98,  // AQ......,..R....
/* 00e0 */ 0x2c, 0x86, 0x38, 0xb4, 0x4c, 0x99, 0x3b, 0x31, 0x59, 0x0c, 0x71, 0x61, 0x99, 0x32, 0x77, 0x62,  // ,.8.L.;1Y.qa.2wb
/* 00f0 */ 0xb2, 0x18, 0xe2, 0xe1, 0x32, 0x65, 0xee, 0xc4, 0x64, 0x31, 0xc4, 0x27, 0x65, 0xca, 0xdc, 0x89,  // ....2e..d1.'e...
/* 0100 */ 0xc9, 0x62, 0xf8, 0xa2, 0x0c, 0x0b, 0x4a, 0x64, 0x31, 0x42, 0xa0, 0xc4, 0x85, 0x98, 0x0b, 0x31,  // .b....Jd1B.....1
/* 0110 */ 0x72, 0x90, 0xbe, 0x2c, 0x28, 0x73, 0x27, 0x26, 0x8b, 0x21, 0xa6, 0x94, 0x29, 0x73, 0x27, 0x26,  // r..,(s'&.!..)s'&
/* 0120 */ 0x8b, 0x21, 0x0e, 0x2d, 0x53, 0xe6, 0x4e, 0x4c, 0x16, 0x43, 0x5c, 0x58, 0xa6, 0xcc, 0x9d, 0x98,  // .!.-S.NL.C.X....
/* 0130 */ 0x2c, 0x86, 0x78, 0xb8, 0x4c, 0x99, 0x3b, 0x31, 0x59, 0x0c, 0xf1, 0x49, 0x99, 0x32, 0x77, 0x62,  // ,.x.L.;1Y..I.2wb
/* 0140 */ 0xb2, 0x18, 0x50, 0x94, 0x61, 0x41, 0x89, 0x2c, 0x86, 0x0f, 0x4a, 0x3c, 0x8c, 0x79, 0x18, 0xa3,  // ..P.aA.,..J<.y..
/* 0150 */ 0xc4, 0xd0, 0x97, 0x05, 0x65, 0xee, 0xc4, 0x64, 0x31, 0xc4, 0x94, 0x32, 0x65, 0xee, 0xc4, 0x64,  // ....e..d1..2e..d
/* 0160 */ 0x31, 0xc4, 0xa1, 0x65, 0xca, 0xdc, 0x89, 0xc9, 0x62, 0x88, 0x0b, 0xcb, 0x94, 0xb9, 0x13, 0x93,  // 1..e....b.......
/* 0170 */ 0xc5, 0x10, 0x0f, 0x97, 0x29, 0x73, 0x27, 0x26, 0x8b, 0x21, 0x3e, 0x29, 0x53, 0xe6, 0x4e, 0x4c,  // ....)s'&.!>)S.NL
/* 0180 */ 0x16, 0xc3, 0x96, 0x32, 0x2c, 0x28, 0x91, 0xc5, 0x80, 0xa0, 0xc4, 0x27, 0x98, 0x4f, 0x30, 0x4a,  // ...2,(.....'.O0J
/* 0190 */ 0x28, 0x7d, 0x59, 0x50, 0xe6, 0x4e, 0x4c, 0x16, 0x43, 0x4c, 0x29, 0x53, 0xe6, 0x4e, 0x4c, 0x16,  // (}YP.NL.CL)S.NL.
/* 01a0 */ 0x43, 0x1c, 0x5a, 0xa6, 0xcc, 0x9d, 0x98, 0x2c, 0x86, 0xb8, 0xb0, 0x4c, 0x99, 0x3b, 0x31, 0x59,  // C.Z....,...L.;1Y
/* 01b0 */ 0x0c, 0xf1, 0x70, 0x99, 0x32, 0x77, 0x62, 0xb2, 0x18, 0xe2, 0x93, 0x32, 0x65, 0xee, 0xc4, 0x64,  // ..p.2wb....2e..d
/* 01c0 */ 0x31, 0x85, 0x94, 0x61, 0x41, 0x89, 0x2c, 0x86, 0x0d, 0x25, 0xb2, 0x98, 0x6c, 0x3e, 0x89, 0xbe,  // 1..aA.,..%..l>..
/* 01d0 */ 0x2c, 0x28, 0x73, 0x27, 0x26, 0x8b, 0x21, 0xa6, 0x94, 0x29, 0x73, 0x27, 0x26, 0x8b, 0x21, 0x0e,  // ,(s'&.!..)s'&.!.
/* 01e0 */ 0x2d, 0x53, 0xe6, 0x4e, 0x4c, 0x16, 0x43, 0x5c, 0x58, 0xa6, 0xcc, 0x9d, 0x98, 0x2c, 0x86, 0x78,  // -S.NL.C.X....,.x
/* 01f0 */ 0xb8, 0x4c, 0x99, 0x3b, 0x31, 0x59, 0x0c, 0xf1, 0x49, 0x99, 0x32, 0x77, 0x62, 0xb2, 0x18, 0xb1,  // .L.;1Y..I.2wb...
/* 0200 */ 0x52, 0x86, 0x05, 0x25, 0xb2, 0x98, 0xff, 0x0b,                          // R..%....
    };
    uint8_t uncompressed_data[] = {
/* 0000 */ 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00,  // ................
/* 0010 */ 0x01, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,  // .....U..........
/* 0020 */ 0x00, 0x00, 0x24, 0x00, 0x00, 0x55, 0x24, 0x00, 0x00, 0xaa, 0x24, 0x00, 0x00, 0xff, 0x24, 0x00,  // ..$..U$...$...$.
/* 0030 */ 0x00, 0x00, 0x49, 0x00, 0x00, 0x55, 0x49, 0x00, 0x00, 0xaa, 0x49, 0x00, 0x00, 0xff, 0x49, 0x00,  // ..I..UI...I...I.
/* 0040 */ 0x00, 0x00, 0x6d, 0x00, 0x00, 0x55, 0x6d, 0x00, 0x00, 0xaa, 0x6d, 0x00, 0x00, 0xff, 0x6d, 0x00,  // ..m..Um...m...m.
/* 0050 */ 0x00, 0x00, 0x92, 0x00, 0x00, 0x55, 0x92, 0x00, 0x00, 0xaa, 0x92, 0x00, 0x00, 0xff, 0x92, 0x00,  // .....U..........
/* 0060 */ 0x00, 0x00, 0xb6, 0x00, 0x00, 0x55, 0xb6, 0x00, 0x00, 0xaa, 0xb6, 0x00, 0x00, 0xff, 0xb6, 0x00,  // .....U..........
/* 0070 */ 0x00, 0x00, 0xdb, 0x00, 0x00, 0x55, 0xdb, 0x00, 0x00, 0xaa, 0xdb, 0x00, 0x00, 0xff, 0xdb, 0x00,  // .....U..........
/* 0080 */ 0x00, 0x00, 0xff, 0x00, 0x00, 0x55, 0xff, 0x00, 0x00, 0xaa, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00,  // .....U..........
/* 0090 */ 0x00, 0x00, 0x00, 0x24, 0x00, 0x55, 0x00, 0x24, 0x00, 0xaa, 0x00, 0x24, 0x00, 0xff, 0x00, 0x24,  // ...$.U.$...$...$
/* 00a0 */ 0x00, 0x00, 0x24, 0x24, 0x00, 0x55, 0x24, 0x24, 0x00, 0xaa, 0x24, 0x24, 0x00, 0xff, 0x24, 0x24,  // ..$$.U$$..$$..$$
/* 00b0 */ 0x00, 0x00, 0x49, 0x24, 0x00, 0x55, 0x49, 0x24, 0x00, 0xaa, 0x49, 0x24, 0x00, 0xff, 0x49, 0x24,  // ..I$.UI$..I$..I$
/* 00c0 */ 0x00, 0x00, 0x6d, 0x24, 0x00, 0x55, 0x6d, 0x24, 0x00, 0xaa, 0x6d, 0x24, 0x00, 0xff, 0x6d, 0x24,  // ..m$.Um$..m$..m$
/* 00d0 */ 0x00, 0x00, 0x92, 0x24, 0x00, 0x55, 0x92, 0x24, 0x00, 0xaa, 0x92, 0x24, 0x00, 0xff, 0x92, 0x24,  // ...$.U.$...$...$
/* 00e0 */ 0x00, 0x00, 0xb6, 0x24, 0x00, 0x55, 0xb6, 0x24, 0x00, 0xaa, 0xb6, 0x24, 0x00, 0xff, 0xb6, 0x24,  // ...$.U.$...$...$
/* 00f0 */ 0x00, 0x00, 0xdb, 0x24, 0x00, 0x55, 0xdb, 0x24, 0x00, 0xaa, 0xdb, 0x24, 0x00, 0xff, 0xdb, 0x24,  // ...$.U.$...$...$
/* 0100 */ 0x00, 0x00, 0xff, 0x24, 0x00, 0x55, 0xff, 0x24, 0x00, 0xaa, 0xff, 0x24, 0x00, 0xff, 0xff, 0x24,  // ...$.U.$...$...$
/* 0110 */ 0x00, 0x00, 0x00, 0x49, 0x00, 0x55, 0x00, 0x49, 0x00, 0xaa, 0x00, 0x49, 0x00, 0xff, 0x00, 0x49,  // ...I.U.I...I...I
/* 0120 */ 0x00, 0x00, 0x24, 0x49, 0x00, 0x55, 0x24, 0x49, 0x00, 0xaa, 0x24, 0x49, 0x00, 0xff, 0x24, 0x49,  // ..$I.U$I..$I..$I
/* 0130 */ 0x00, 0x00, 0x49, 0x49, 0x00, 0x55, 0x49, 0x49, 0x00, 0xaa, 0x49, 0x49, 0x00, 0xff, 0x49, 0x49,  // ..II.UII..II..II
/* 0140 */ 0x00, 0x00, 0x6d, 0x49, 0x00, 0x55, 0x6d, 0x49, 0x00, 0xaa, 0x6d, 0x49, 0x00, 0xff, 0x6d, 0x49,  // ..mI.UmI..mI..mI
/* 0150 */ 0x00, 0x00, 0x92, 0x49, 0x00, 0x55, 0x92, 0x49, 0x00, 0xaa, 0x92, 0x49, 0x00, 0xff, 0x92, 0x49,  // ...I.U.I...I...I
/* 0160 */ 0x00, 0x00, 0xb6, 0x49, 0x00, 0x55, 0xb6, 0x49, 0x00, 0xaa, 0xb6, 0x49, 0x00, 0xff, 0xb6, 0x49,  // ...I.U.I...I...I
/* 0170 */ 0x00, 0x00, 0xdb, 0x49, 0x00, 0x55, 0xdb, 0x49, 0x00, 0xaa, 0xdb, 0x49, 0x00, 0xff, 0xdb, 0x49,  // ...I.U.I...I...I
/* 0180 */ 0x00, 0x00, 0xff, 0x49, 0x00, 0x55, 0xff, 0x49, 0x00, 0xaa, 0xff, 0x49, 0x00, 0xff, 0xff, 0x49,  // ...I.U.I...I...I
/* 0190 */ 0x00, 0x00, 0x00, 0x6d, 0x00, 0x55, 0x00, 0x6d, 0x00, 0xaa, 0x00, 0x6d, 0x00, 0xff, 0x00, 0x6d,  // ...m.U.m...m...m
/* 01a0 */ 0x00, 0x00, 0x24, 0x6d, 0x00, 0x55, 0x24, 0x6d, 0x00, 0xaa, 0x24, 0x6d, 0x00, 0xff, 0x24, 0x6d,  // ..$m.U$m..$m..$m
/* 01b0 */ 0x00, 0x00, 0x49, 0x6d, 0x00, 0x55, 0x49, 0x6d, 0x00, 0xaa, 0x49, 0x6d, 0x00, 0xff, 0x49, 0x6d,  // ..Im.UIm..Im..Im
/* 01c0 */ 0x00, 0x00, 0x6d, 0x6d, 0x00, 0x55, 0x6d, 0x6d, 0x00, 0xaa, 0x6d, 0x6d, 0x00, 0xff, 0x6d, 0x6d,  // ..mm.Umm..mm..mm
/* 01d0 */ 0x00, 0x00, 0x92, 0x6d, 0x00, 0x55, 0x92, 0x6d, 0x00, 0xaa, 0x92, 0x6d, 0x00, 0xff, 0x92, 0x6d,  // ...m.U.m...m...m
/* 01e0 */ 0x00, 0x00, 0xb6, 0x6d, 0x00, 0x55, 0xb6, 0x6d, 0x00, 0xaa, 0xb6, 0x6d, 0x00, 0xff, 0xb6, 0x6d,  // ...m.U.m...m...m
/* 01f0 */ 0x00, 0x00, 0xdb, 0x6d, 0x00, 0x55, 0xdb, 0x6d, 0x00, 0xaa, 0xdb, 0x6d, 0x00, 0xff, 0xdb, 0x6d,  // ...m.U.m...m...m
/* 0200 */ 0x00, 0x00, 0xff, 0x6d, 0x00, 0x55, 0xff, 0x6d, 0x00, 0xaa, 0xff, 0x6d, 0x00, 0xff, 0xff, 0x6d,  // ...m.U.m...m...m
/* 0210 */ 0x00, 0x00, 0x00, 0x92, 0x00, 0x55, 0x00, 0x92, 0x00, 0xaa, 0x00, 0x92, 0x00, 0xff, 0x00, 0x92,  // .....U..........
/* 0220 */ 0x00, 0x00, 0x24, 0x92, 0x00, 0x55, 0x24, 0x92, 0x00, 0xaa, 0x24, 0x92, 0x00, 0xff, 0x24, 0x92,  // ..$..U$...$...$.
/* 0230 */ 0x00, 0x00, 0x49, 0x92, 0x00, 0x55, 0x49, 0x92, 0x00, 0xaa, 0x49, 0x92, 0x00, 0xff, 0x49, 0x92,  // ..I..UI...I...I.
/* 0240 */ 0x00, 0x00, 0x6d, 0x92, 0x00, 0x55, 0x6d, 0x92, 0x00, 0xaa, 0x6d, 0x92, 0x00, 0xff, 0x6d, 0x92,  // ..m..Um...m...m.
/* 0250 */ 0x00, 0x00, 0x92, 0x92, 0x00, 0x55, 0x92, 0x92, 0x00, 0xaa, 0x92, 0x92, 0x00, 0xff, 0x92, 0x92,  // .....U..........
/* 0260 */ 0x00, 0x00, 0xb6, 0x92, 0x00, 0x55, 0xb6, 0x92, 0x00, 0xaa, 0xb6, 0x92, 0x00, 0xff, 0xb6, 0x92,  // .....U..........
/* 0270 */ 0x00, 0x00, 0xdb, 0x92, 0x00, 0x55, 0xdb, 0x92, 0x00, 0xaa, 0xdb, 0x92, 0x00, 0xff, 0xdb, 0x92,  // .....U..........
/* 0280 */ 0x00, 0x00, 0xff, 0x92, 0x00, 0x55, 0xff, 0x92, 0x00, 0xaa, 0xff, 0x92, 0x00, 0xff, 0xff, 0x92,  // .....U..........
/* 0290 */ 0x00, 0x00, 0x00, 0xb6, 0x00, 0x55, 0x00, 0xb6, 0x00, 0xaa, 0x00, 0xb6, 0x00, 0xff, 0x00, 0xb6,  // .....U..........
/* 02a0 */ 0x00, 0x00, 0x24, 0xb6, 0x00, 0x55, 0x24, 0xb6, 0x00, 0xaa, 0x24, 0xb6, 0x00, 0xff, 0x24, 0xb6,  // ..$..U$...$...$.
/* 02b0 */ 0x00, 0x00, 0x49, 0xb6, 0x00, 0x55, 0x49, 0xb6, 0x00, 0xaa, 0x49, 0xb6, 0x00, 0xff, 0x49, 0xb6,  // ..I..UI...I...I.
/* 02c0 */ 0x00, 0x00, 0x6d, 0xb6, 0x00, 0x55, 0x6d, 0xb6, 0x00, 0xaa, 0x6d, 0xb6, 0x00, 0xff, 0x6d, 0xb6,  // ..m..Um...m...m.
/* 02d0 */ 0x00, 0x00, 0x92, 0xb6, 0x00, 0x55, 0x92, 0xb6, 0x00, 0xaa, 0x92, 0xb6, 0x00, 0xff, 0x92, 0xb6,  // .....U..........
/* 02e0 */ 0x00, 0x00, 0xb6, 0xb6, 0x00, 0x55, 0xb6, 0xb6, 0x00, 0xaa, 0xb6, 0xb6, 0x00, 0xff, 0xb6, 0xb6,  // .....U..........
/* 02f0 */ 0x00, 0x00, 0xdb, 0xb6, 0x00, 0x55, 0xdb, 0xb6, 0x00, 0xaa, 0xdb, 0xb6, 0x00, 0xff, 0xdb, 0xb6,  // .....U..........
/* 0300 */ 0x00, 0x00, 0xff, 0xb6, 0x00, 0x55, 0xff, 0xb6, 0x00, 0xaa, 0xff, 0xb6, 0x00, 0xff, 0xff, 0xb6,  // .....U..........
/* 0310 */ 0x00, 0x00, 0x00, 0xdb, 0x00, 0x55, 0x00, 0xdb, 0x00, 0xaa, 0x00, 0xdb, 0x00, 0xff, 0x00, 0xdb,  // .....U..........
/* 0320 */ 0x00, 0x00, 0x24, 0xdb, 0x00, 0x55, 0x24, 0xdb, 0x00, 0xaa, 0x24, 0xdb, 0x00, 0xff, 0x24, 0xdb,  // ..$..U$...$...$.
/* 0330 */ 0x00, 0x00, 0x49, 0xdb, 0x00, 0x55, 0x49, 0xdb, 0x00, 0xaa, 0x49, 0xdb, 0x00, 0xff, 0x49, 0xdb,  // ..I..UI...I...I.
/* 0340 */ 0x00, 0x00, 0x6d, 0xdb, 0x00, 0x55, 0x6d, 0xdb, 0x00, 0xaa, 0x6d, 0xdb, 0x00, 0xff, 0x6d, 0xdb,  // ..m..Um...m...m.
/* 0350 */ 0x00, 0x00, 0x92, 0xdb, 0x00, 0x55, 0x92, 0xdb, 0x00, 0xaa, 0x92, 0xdb, 0x00, 0xff, 0x92, 0xdb,  // .....U..........
/* 0360 */ 0x00, 0x00, 0xb6, 0xdb, 0x00, 0x55, 0xb6, 0xdb, 0x00, 0xaa, 0xb6, 0xdb, 0x00, 0xff, 0xb6, 0xdb,  // .....U..........
/* 0370 */ 0x00, 0x00, 0xdb, 0xdb, 0x00, 0x55, 0xdb, 0xdb, 0x00, 0xaa, 0xdb, 0xdb, 0x00, 0xff, 0xdb, 0xdb,  // .....U..........
/* 0380 */ 0x00, 0x00, 0xff, 0xdb, 0x00, 0x55, 0xff, 0xdb, 0x00, 0xaa, 0xff, 0xdb, 0x00, 0xff, 0xff, 0xdb,  // .....U..........
/* 0390 */ 0x00, 0x00, 0x00, 0xff, 0x00, 0x55, 0x00, 0xff, 0x00, 0xaa, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,  // .....U..........
/* 03a0 */ 0x00, 0x00, 0x24, 0xff, 0x00, 0x55, 0x24, 0xff, 0x00, 0xaa, 0x24, 0xff, 0x00, 0xff, 0x24, 0xff,  // ..$..U$...$...$.
/* 03b0 */ 0x00, 0x00, 0x49, 0xff, 0x00, 0x55, 0x49, 0xff, 0x00, 0xaa, 0x49, 0xff, 0x00, 0xff, 0x49, 0xff,  // ..I..UI...I...I.
/* 03c0 */ 0x00, 0x00, 0x6d, 0xff, 0x00, 0x55, 0x6d, 0xff, 0x00, 0xaa, 0x6d, 0xff, 0x00, 0xff, 0x6d, 0xff,  // ..m..Um...m...m.
/* 03d0 */ 0x00, 0x00, 0x92, 0xff, 0x00, 0x55, 0x92, 0xff, 0x00, 0xaa, 0x92, 0xff, 0x00, 0xff, 0x92, 0xff,  // .....U..........
/* 03e0 */ 0x00, 0x00, 0xb6, 0xff, 0x00, 0x55, 0xb6, 0xff, 0x00, 0xaa, 0xb6, 0xff, 0x00, 0xff, 0xb6, 0xff,  // .....U..........
/* 03f0 */ 0x00, 0x00, 0xdb, 0xff, 0x00, 0x55, 0xdb, 0xff, 0x00, 0xaa, 0xdb, 0xff, 0x00, 0xff, 0xdb, 0xff,  // .....U..........
/* 0400 */ 0x00, 0x00, 0xff, 0xff, 0x00, 0x55, 0xff, 0xff, 0x00, 0xaa, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,  // .....U..........
/* 0410 */ 0x00,                                               // .
    };

    uint8_t  compressionFlags = 0x22;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(compressed_data, sizeof(compressed_data),
        compressionFlags, rdata, rlen);

    RED_CHECK_EQUAL(sizeof(uncompressed_data), rlen);
    RED_CHECK_EQUAL(0,                         memcmp(uncompressed_data,
        rdata, rlen));

    //LOG(LOG_INFO, "rlen=%d", rlen);
    //hexdump_d(rdata, rlen);
}

RED_AUTO_TEST_CASE(TestRDP60BlukDecompression4)
{
    #include "../../fixtures/test_mppc_7.hpp"

    rdp_mppc_60_dec mppc_dec;

    memcpy(mppc_dec.history_buf, __historyBuffer, sizeof(__historyBuffer));
    memcpy(mppc_dec.offset_cache, __offsetCache, sizeof(__offsetCache));
    mppc_dec.history_ptr = mppc_dec.history_buf + __historyOffset;

    uint8_t  compressionFlags = 0x22;

    const uint8_t * rdata;
    uint32_t        rlen;

    mppc_dec.decompress(__outputBuffer, sizeof(__outputBuffer),
        compressionFlags, rdata, rlen);

    RED_CHECK_EQUAL(sizeof(__srcData), rlen);
    RED_CHECK_EQUAL(0,                 memcmp(__srcData, rdata, rlen));

    LOG(LOG_INFO, "rlen=%d", rlen);
    hexdump_d(rdata, rlen);
}

RED_AUTO_TEST_CASE(TestCacheAdd) {
    uint16_t offset_cache[4] = { 0, 0, 0, 0 };

    cache_add(offset_cache, 15);
    RED_CHECK_EQUAL(offset_cache[0], 15);
    RED_CHECK_EQUAL(offset_cache[1], 0);
    RED_CHECK_EQUAL(offset_cache[2], 0);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 7);
    RED_CHECK_EQUAL(offset_cache[0], 7);
    RED_CHECK_EQUAL(offset_cache[1], 15);
    RED_CHECK_EQUAL(offset_cache[2], 0);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 23);
    RED_CHECK_EQUAL(offset_cache[0], 23);
    RED_CHECK_EQUAL(offset_cache[1], 7);
    RED_CHECK_EQUAL(offset_cache[2], 15);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 152);
    RED_CHECK_EQUAL(offset_cache[0], 152);
    RED_CHECK_EQUAL(offset_cache[1], 23);
    RED_CHECK_EQUAL(offset_cache[2], 7);
    RED_CHECK_EQUAL(offset_cache[3], 15);

    cache_add(offset_cache, 65000);
    RED_CHECK_EQUAL(offset_cache[0], 65000);
    RED_CHECK_EQUAL(offset_cache[1], 152);
    RED_CHECK_EQUAL(offset_cache[2], 23);
    RED_CHECK_EQUAL(offset_cache[3], 7);
}

RED_AUTO_TEST_CASE(TestCacheFindSwap) {
    uint16_t offset_cache[4] = { 0, 0, 0, 0 };

    cache_add(offset_cache, 15);
    RED_CHECK_EQUAL(offset_cache[0], 15);
    RED_CHECK_EQUAL(offset_cache[1], 0);
    RED_CHECK_EQUAL(offset_cache[2], 0);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 7);
    RED_CHECK_EQUAL(offset_cache[0], 7);
    RED_CHECK_EQUAL(offset_cache[1], 15);
    RED_CHECK_EQUAL(offset_cache[2], 0);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 23);
    RED_CHECK_EQUAL(offset_cache[0], 23);
    RED_CHECK_EQUAL(offset_cache[1], 7);
    RED_CHECK_EQUAL(offset_cache[2], 15);
    RED_CHECK_EQUAL(offset_cache[3], 0);

    cache_add(offset_cache, 152);
    RED_CHECK_EQUAL(offset_cache[0], 152);
    RED_CHECK_EQUAL(offset_cache[1], 23);
    RED_CHECK_EQUAL(offset_cache[2], 7);
    RED_CHECK_EQUAL(offset_cache[3], 15);

    cache_add(offset_cache, 65000);
    RED_CHECK_EQUAL(offset_cache[0], 65000);
    RED_CHECK_EQUAL(offset_cache[1], 152);
    RED_CHECK_EQUAL(offset_cache[2], 23);
    RED_CHECK_EQUAL(offset_cache[3], 7);

    RED_CHECK_EQUAL(-1, rdp_mppc_60_enc::cache_find(offset_cache, 736));

    RED_CHECK_EQUAL(2, rdp_mppc_60_enc::cache_find(offset_cache, 23));
    cache_swap(offset_cache, 2);
    RED_CHECK_EQUAL(offset_cache[0], 23);
    RED_CHECK_EQUAL(offset_cache[1], 152);
    RED_CHECK_EQUAL(offset_cache[2], 65000);
    RED_CHECK_EQUAL(offset_cache[3], 7);

    RED_CHECK_EQUAL(3, rdp_mppc_60_enc::cache_find(offset_cache, 7));
    cache_swap(offset_cache, 3);
    RED_CHECK_EQUAL(offset_cache[0], 7);
    RED_CHECK_EQUAL(offset_cache[1], 152);
    RED_CHECK_EQUAL(offset_cache[2], 65000);
    RED_CHECK_EQUAL(offset_cache[3], 23);
}
