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
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean

   Unit test to conversion of RDP drawing orders to PNG images
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestXXXXXXXXXX
#include "system/redemption_unit_tests.hpp"


// #define LOGNULL
#define LOGPRINT
#include "utils/apps/recording_progress.hpp"
#include "get_file_contents.hpp"

//#include "path/to/file.hpp"

BOOST_AUTO_TEST_CASE(TestXXXXXXXXX)
{
    std::string contents;

    time_t const start_time = 123456789;
    time_t const end_time = start_time + 100;
    char const * filename = "/tmp/test_progress_123.test";

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");
//         {"percentage":0,"eta":-1,"videos":0}
//         {"percentage":10,"eta":0,"videos":0}

        p(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":0})");

        p(start_time + 100);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":99,"eta":0,"videos":0})");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, R"({"percentage":100,"eta":0,"videos":1})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");

        p.next_video(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1})");

        p(start_time + 100);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":99,"eta":0,"videos":1})");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, R"({"percentage":100,"eta":0,"videos":2})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");

        p.next_video(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1})");

        p.raise_error(2, "plouf");
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents,
            R"({"percentage":90,"eta":0,"videos":1,"error":{"code":2,"message":"plouf"}})");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1,"error":{"code":2,"message":"plouf"}})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "10 0");

        p(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "90 0");

        p(start_time + 100);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "99 0");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, "100 0");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "10 0");

        p.next_video(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "90 0");

        p(start_time + 100);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "99 0");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, "100 0");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        BOOST_REQUIRE(p.is_valid());

        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "10 0");

        p.next_video(start_time + 90);
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "90 0");

        p.raise_error(2, "plouf");
        BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
        BOOST_CHECK_EQUAL(contents, "-1 plouf (2)");
    }
    BOOST_CHECK_EQUAL(0, get_file_contents(contents, filename));
    BOOST_CHECK_EQUAL(contents, "-1 plouf (2)");

    unlink(filename);
}
