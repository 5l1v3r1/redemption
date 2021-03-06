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

#include "test_only/test_framework/redemption_unit_tests.hpp"
#include "test_only/test_framework/working_directory.hpp"


#include "utils/recording_progress.hpp"
#include "utils/fileutils.hpp"


RED_AUTO_TEST_CASE_WF(TestRecordingProgress, wf)
{
    std::string contents;

    time_t const start_time = 123456789;
    time_t const end_time = start_time + 100;
    char const * filename = wf.c_str();

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");
//         {"percentage":0,"eta":-1,"videos":0}
//         {"percentage":10,"eta":0,"videos":0}

        p(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":0})");

        p(start_time + 100);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":99,"eta":0,"videos":0})");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, R"({"percentage":100,"eta":0,"videos":1})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");

        p.next_video(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1})");

        p(start_time + 100);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":99,"eta":0,"videos":1})");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, R"({"percentage":100,"eta":0,"videos":2})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::JSON_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":0,"eta":-1,"videos":0})");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":10,"eta":0,"videos":0})");

        p.next_video(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1})");

        p.raise_error(2, "plouf");
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents,
            R"({"percentage":90,"eta":0,"videos":1,"error":{"code":2,"message":"plouf"}})");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, R"({"percentage":90,"eta":0,"videos":1,"error":{"code":2,"message":"plouf"}})");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "10 0");

        p(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "90 0");

        p(start_time + 100);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "99 0");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, "100 0");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "10 0");

        p.next_video(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "90 0");

        p(start_time + 100);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "99 0");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, "100 0");

    {
        unlink(filename);
        UpdateProgressData p(
            UpdateProgressData::OLD_FORMAT,
            filename,
            start_time,
            end_time,
            0, 0
        );

        RED_REQUIRE(p.is_valid());

        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "0 -1");

        p(start_time + 10);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "10 0");

        p.next_video(start_time + 90);
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "90 0");

        p.raise_error(2, "plouf");
        contents.clear(); (void)append_file_contents(filename, contents);
        RED_CHECK_EQUAL(contents, "-1 plouf (2)");
    }
    contents.clear(); (void)append_file_contents(filename, contents);
    RED_CHECK_EQUAL(contents, "-1 plouf (2)");
}
