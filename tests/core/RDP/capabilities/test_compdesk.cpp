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
   Author(s): Christophe Grosjean

   Unit test to RDP CompDesk object
   Using lib boost functions for testing
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCapabilityCompDesk
#include <boost/test/auto_unit_test.hpp>

#define LOGNULL
#include "RDP/capabilities/compdesk.hpp"

BOOST_AUTO_TEST_CASE(TestCapabilityCompDeskEmit)
{
    CompDeskCaps compdesk_caps;
    compdesk_caps.CompDeskSupportLevel = COMPDESK_SUPPORTED;

    BOOST_CHECK_EQUAL(compdesk_caps.capabilityType, static_cast<uint16_t>(CAPSETTYPE_COMPDESK));
    BOOST_CHECK_EQUAL(compdesk_caps.len, static_cast<uint16_t>(CAPLEN_COMPDESK));
    BOOST_CHECK_EQUAL(compdesk_caps.CompDeskSupportLevel, static_cast<uint16_t>(COMPDESK_SUPPORTED));

    StaticOutStream<1024> out_stream;
    compdesk_caps.emit(out_stream);
    InStream stream(out_stream.get_data(), out_stream.get_offset());

    CompDeskCaps compdesk_caps2;

    BOOST_CHECK_EQUAL(compdesk_caps2.capabilityType, static_cast<uint16_t>(CAPSETTYPE_COMPDESK));
    BOOST_CHECK_EQUAL(compdesk_caps2.len, static_cast<uint16_t>(CAPLEN_COMPDESK));

    BOOST_CHECK_EQUAL((uint16_t)CAPSETTYPE_COMPDESK, stream.in_uint16_le());
    BOOST_CHECK_EQUAL((uint16_t)CAPLEN_COMPDESK, stream.in_uint16_le());
    compdesk_caps2.recv(stream, CAPLEN_COMPDESK);

    BOOST_CHECK_EQUAL(compdesk_caps2.CompDeskSupportLevel, static_cast<uint16_t>(COMPDESK_SUPPORTED));
}
