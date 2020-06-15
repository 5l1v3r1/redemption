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
   Copyright (C) Wallix 2013
   Author(s): Christophe Grosjean

*/

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "test_only/test_framework/working_directory.hpp"


#include "transport/in_meta_sequence_transport.hpp"
#include "transport/out_meta_sequence_transport.hpp"
#include "core/error.hpp"
#include "utils/genfstat.hpp"

#include "test_only/lcg_random.hpp"

#include <string_view>

using namespace std::string_view_literals;


constexpr auto is_encrypted = InCryptoTransport::EncryptionMode::Encrypted;
constexpr auto is_not_encrypted = InCryptoTransport::EncryptionMode::NotEncrypted;

inline void read_sample_files(char const * filename)
{
    size_t const sizes[]{1471394, 444578, 290245};

    constexpr size_t buf_sz = 10000;
    unsigned char buffer[buf_sz];

    using Read = Transport::Read;
    Read status;
    Fstat fstat;
    CryptoContext cctx;
    InMetaSequenceTransport wrm_trans(cctx, filename, ".mwrm", is_not_encrypted, fstat);
    for (size_t const file_total : sizes) {
        for (size_t i = 0; i < file_total / buf_sz; ++i) {
            RED_CHECK_NO_THROW(status = wrm_trans.atomic_read(buffer, buf_sz));
            RED_CHECK_EQUAL(Read::Ok, status);
        }
        RED_CHECK_NO_THROW(status = wrm_trans.atomic_read(buffer, file_total % buf_sz));
        RED_CHECK_EQUAL(Read::Ok, status);
    }

    RED_CHECK_NO_THROW(status = wrm_trans.atomic_read(buffer, 1));
    RED_CHECK_EQUAL(Read::Eof, status);
}

RED_AUTO_TEST_CASE(TestSequenceFollowedTransportWRM1)
{
    read_sample_files(FIXTURES_PATH "/sample");
}

RED_AUTO_TEST_CASE(TestSequenceFollowedTransportWRM1_v2)
{
    read_sample_files(FIXTURES_PATH "/sample_v2");
}

RED_AUTO_TEST_CASE(TestSequenceFollowedTransportWRM2)
{
//        "800 600\n",
//        "0\n",
//        "\n",
//        FIXTURES_PATH "/sample0.wrm 1352304810 1352304870\n",
//        FIXTURES_PATH "/sample1.wrm 1352304870 1352304930\n",
//        FIXTURES_PATH "/sample2.wrm 1352304930 1352304990\n",

    // This is what we are actually testing, chaining of several files content
    {
        Fstat fstat;
        CryptoContext cctx;
        InMetaSequenceTransport mwrm_trans(cctx, FIXTURES_PATH "/sample", ".mwrm", is_not_encrypted, fstat);
        RED_CHECK_EQUAL(0, mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample0.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304810 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304870 == mwrm_trans.end_chunk_time());
        RED_CHECK(1 == mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample1.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304870 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304930 == mwrm_trans.end_chunk_time());
        RED_CHECK(2 == mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample2.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304930 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304990 == mwrm_trans.end_chunk_time());
        RED_CHECK(3 == mwrm_trans.get_seqno());

        RED_CHECK_EXCEPTION_ERROR_ID(mwrm_trans.next(), ERR_TRANSPORT_NO_MORE_DATA);
    }

    // check we can do it two times
    Fstat fstat;
    CryptoContext cctx;
    InMetaSequenceTransport mwrm_trans(cctx, FIXTURES_PATH "/sample", ".mwrm", is_not_encrypted, fstat);

    RED_CHECK(0 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample0.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304810 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304870 == mwrm_trans.end_chunk_time());
    RED_CHECK(1 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample1.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304870 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304930 == mwrm_trans.end_chunk_time());
    RED_CHECK(2 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample2.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304930 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304990 == mwrm_trans.end_chunk_time());
    RED_CHECK(3 == mwrm_trans.get_seqno());
}

RED_AUTO_TEST_CASE(TestSequenceFollowedTransportWRM2_RIO)
{
//        "800 600\n",
//        "0\n",
//        "\n",
//        FIXTURES_PATH "/sample0.wrm 1352304810 1352304870\n",
//        FIXTURES_PATH "/sample1.wrm 1352304870 1352304930\n",
//        FIXTURES_PATH "/sample2.wrm 1352304930 1352304990\n",

    // This is what we are actually testing, chaining of several files content
    Fstat fstat;
    CryptoContext cctx;
    InMetaSequenceTransport mwrm_trans(cctx, FIXTURES_PATH "/sample", ".mwrm", is_not_encrypted, fstat);
    RED_CHECK(0 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample0.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304810 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304870 == mwrm_trans.end_chunk_time());
    RED_CHECK(1 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample1.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304870 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304930 == mwrm_trans.end_chunk_time());
    RED_CHECK(2 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample2.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304930 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304990 == mwrm_trans.end_chunk_time());
    RED_CHECK(3 == mwrm_trans.get_seqno());

    RED_CHECK_EXCEPTION_ERROR_ID(mwrm_trans.next(), ERR_TRANSPORT_NO_MORE_DATA);
}

RED_AUTO_TEST_CASE(TestSequenceFollowedTransportWRM3)
{
//        "800 600\n",
//        "0\n",
//        "\n",
//        "/var/rdpproxy/recorded/sample0.wrm 1352304810 1352304870\n",
//        "/var/rdpproxy/recorded/sample1.wrm 1352304870 1352304930\n",
//        "/var/rdpproxy/recorded/sample2.wrm 1352304930 1352304990\n",

    // This is what we are actually testing, chaining of several files content

    {
        Fstat fstat;
        CryptoContext cctx;
        InMetaSequenceTransport mwrm_trans(cctx, FIXTURES_PATH "/moved_sample", ".mwrm", is_not_encrypted, fstat);
        RED_CHECK(0 == mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample0.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304810 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304870 == mwrm_trans.end_chunk_time());
        RED_CHECK(1 == mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample1.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304870 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304930 == mwrm_trans.end_chunk_time());
        RED_CHECK(2 == mwrm_trans.get_seqno());

        mwrm_trans.next();
        RED_CHECK(FIXTURES_PATH "/sample2.wrm"sv == mwrm_trans.path());
        RED_CHECK(1352304930 == mwrm_trans.begin_chunk_time());
        RED_CHECK(1352304990 == mwrm_trans.end_chunk_time());
        RED_CHECK(3 == mwrm_trans.get_seqno());

        RED_CHECK_EXCEPTION_ERROR_ID(mwrm_trans.next(), ERR_TRANSPORT_NO_MORE_DATA);
    }

    // check we can do it two times
    Fstat fstat;
    CryptoContext cctx;
    InMetaSequenceTransport mwrm_trans(cctx, FIXTURES_PATH "/moved_sample", ".mwrm", is_not_encrypted, fstat);

    RED_CHECK(0 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample0.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304810 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304870 == mwrm_trans.end_chunk_time());
    RED_CHECK(1 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample1.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304870 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304930 == mwrm_trans.end_chunk_time());
    RED_CHECK(2 == mwrm_trans.get_seqno());

    mwrm_trans.next();
    RED_CHECK(FIXTURES_PATH "/sample2.wrm"sv == mwrm_trans.path());
    RED_CHECK(1352304930 == mwrm_trans.begin_chunk_time());
    RED_CHECK(1352304990 == mwrm_trans.end_chunk_time());
    RED_CHECK(3 == mwrm_trans.get_seqno());
}

RED_AUTO_TEST_CASE(TestCryptoInmetaSequenceTransport)
{
    WorkingDirectory hash_wd("hash");
    WorkingDirectory recorded_wd("recorded");

    CryptoContext cctx;
    cctx.set_master_key(cstr_array_view(
        "\x00\x01\x02\x03\x04\x05\x06\x07"
        "\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
        "\x10\x11\x12\x13\x14\x15\x16\x17"
        "\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
    ));
    cctx.set_hmac_key(cstr_array_view("12345678901234567890123456789012"));

    // two files: 5 and 10 bytes
    {
        LCGRandom rnd;
        Fstat fstat;
        timeval tv;
        tv.tv_usec = 0;
        tv.tv_sec = 1352304810;
        const int groupid = 0;

        cctx.set_trace_type(TraceType::cryptofile);

        OutMetaSequenceTransport crypto_trans(cctx, rnd, fstat, recorded_wd.dirname(), hash_wd.dirname(), "TESTOFS", tv, 800, 600, groupid, nullptr, -1);
        crypto_trans.send("AAAAX", 5);
        tv.tv_sec += 100;
        crypto_trans.timestamp(tv);
        crypto_trans.next();
        crypto_trans.send("BBBBXCCCCX", 10);
        tv.tv_sec += 100;
        crypto_trans.timestamp(tv);
        RED_TEST_PASSPOINT();
    }

    hash_wd.add_files({
        "TESTOFS.mwrm",
        "TESTOFS-000000.wrm",
        "TESTOFS-000001.wrm"
    });
    recorded_wd.add_files({
        "TESTOFS.mwrm",
        "TESTOFS-000000.wrm",
        "TESTOFS-000001.wrm"
    });

    RED_CHECK_WORKSPACE(hash_wd);
    RED_CHECK_WORKSPACE(recorded_wd);

    auto basepath = recorded_wd.dirname().string() + "TESTOFS";

    {
        Fstat fstat;
        InMetaSequenceTransport crypto_trans(cctx, basepath.c_str(), ".mwrm", is_encrypted, fstat);
        char buffer[15];
        // 5 + 10
        RED_CHECK_EXCEPTION_ERROR_ID(crypto_trans.recv_boom(buffer, 15), ERR_TRANSPORT_NO_MORE_DATA);
    }
    {
        Fstat fstat;
        InMetaSequenceTransport crypto_trans(cctx, basepath.c_str(), ".mwrm", is_encrypted, fstat);

        char buffer[15];

        RED_CHECK_NO_THROW(crypto_trans.recv_boom(buffer, 5));
        // internal: next()
        RED_CHECK_NO_THROW(crypto_trans.recv_boom(buffer + 5, 10));

        RED_CHECK(make_array_view(buffer) == "AAAAXBBBBXCCCCX"_av);
    }
}

RED_AUTO_TEST_CASE(CryptoTestInMetaSequenceTransport2)
{
    Fstat fstat;
    CryptoContext cctx;
    cctx.set_master_key(cstr_array_view(
        "\x00\x01\x02\x03\x04\x05\x06\x07"
        "\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
        "\x10\x11\x12\x13\x14\x15\x16\x17"
        "\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
    ));
    cctx.set_hmac_key(cstr_array_view("12345678901234567890123456789012"));

    RED_CHECK_EXCEPTION_ERROR_ID(InMetaSequenceTransport(cctx, "TESTOFSXXX", ".mwrm", is_encrypted, fstat), ERR_TRANSPORT_OPEN_FAILED);
}
