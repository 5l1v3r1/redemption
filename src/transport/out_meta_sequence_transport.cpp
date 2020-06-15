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
   Author(s): Christophe Grosjean, Jonatan Poelen
*/

#include "utils/log.hpp"

#include "utils/genfstat.hpp"
#include "utils/strutils.hpp"
#include "core/error.hpp"

#include "transport/out_meta_sequence_transport.hpp"
#include "transport/mwrm_reader.hpp"

#include <cstdio>


OutMetaSequenceTransport::WrmFGen::WrmFGen(
    const char * const prefix,
    const char * const hash_prefix,
    const char * const filename,
    const char * const extension)
{
    assert(0 != strcmp(prefix, hash_prefix));

    if (!utils::strbcpy(this->path, prefix)
        || !utils::strbcpy(this->hash_path, hash_prefix)
        || !utils::strbcpy(this->filename, filename)
        || !utils::strbcpy(this->extension, extension)) {
        LOG(LOG_ERR, "Filename too long");
        throw Error(ERR_TRANSPORT);
    }

    this->filename_gen[0] = 0;
}

const char * OutMetaSequenceTransport::WrmFGen::get_filename(unsigned count) const noexcept
{
    std::snprintf(
        this->filename_gen, sizeof(this->filename_gen), "%s%s-%06u%s",
        this->path, this->filename, count, this->extension);
    return this->filename_gen;
}

const char * OutMetaSequenceTransport::WrmFGen::get_hash_filename(unsigned count) const noexcept
{
    std::snprintf(
        this->hash_filename_gen, sizeof(this->hash_filename_gen), "%s%s-%06u%s",
        this->hash_path, this->filename, count, this->extension);
    return this->hash_filename_gen;
}


OutMetaSequenceTransport::MetaFilename::MetaFilename(const char * path, const char * basename)
{
    int res = snprintf(this->filename, sizeof(this->filename)-1, "%s%s.mwrm", path, basename);
    if (res > int(sizeof(this->filename) - 6) || res < 0) {
        throw Error(ERR_TRANSPORT_OPEN_FAILED);
    }
}


OutMetaSequenceTransport::OutMetaSequenceTransport(
    CryptoContext & cctx,
    Random & rnd,
    Fstat & fstat,
    const char * path,
    const char * hash_path,
    const char * basename,
    timeval now,
    uint16_t width,
    uint16_t height,
    const int groupid,
    ReportMessageApi * report_message,
    uint32_t file_permissions)
: meta_buf_encrypt_transport(cctx, rnd, fstat, report_error_from_reporter(report_message))
, wrm_filter_encrypt_transport(cctx, rnd, fstat, report_error_from_reporter(report_message))
, fstat(fstat)
, filegen_(path, hash_path, basename, ".wrm")
, groupid_(groupid)
, mf_(path, basename)
, hf_(hash_path, basename)
, start_sec_(now.tv_sec)
, stop_sec_(now.tv_sec)
, cctx(cctx)
, file_permissions_(file_permissions)
{
    this->meta_buf_encrypt_transport.open(
        this->mf_.filename,
        this->hf_.filename,
        S_IRUSR | S_IRGRP | S_IWUSR,
        file_permissions_);

    MwrmWriterBuf mwrm_file_buf;
    mwrm_file_buf.write_header(width, height, this->cctx.get_with_checksum());
    this->meta_buf_encrypt_transport.send(mwrm_file_buf.buffer());
}

OutMetaSequenceTransport::~OutMetaSequenceTransport()
{
    if (this->meta_buf_encrypt_transport.is_open()) {
        this->do_close();
    }
}

void OutMetaSequenceTransport::timestamp(timeval now)
{
    this->stop_sec_ = now.tv_sec;
}

bool OutMetaSequenceTransport::next()
{
    if (!this->wrm_filter_encrypt_transport.is_open()) {
        LOG(LOG_ERR, "OutMetaSequenceTransport::next: Create next file failed!");
        throw Error(ERR_TRANSPORT_NO_MORE_DATA);
    }

    this->next_meta_file();
    ++this->seqno;
    return true;
}

bool OutMetaSequenceTransport::disconnect()
{
    do_close();
    return true;
}

void OutMetaSequenceTransport::do_send(const uint8_t * data, size_t len)
{
    if (!this->wrm_filter_encrypt_transport.is_open()) {
        const char * filename = this->filegen_.get_filename(this->num_file_);
        const char * hash_filename = this->filegen_.get_hash_filename(this->num_file_);
        this->wrm_filter_encrypt_transport.open(filename, hash_filename, this->groupid_, file_permissions_);
    }
    this->wrm_filter_encrypt_transport.send(data, len);
}

void OutMetaSequenceTransport::do_close()
{
    if (this->wrm_filter_encrypt_transport.is_open()) {
        this->next_meta_file();
    }

    uint8_t qhash[MD_HASH::DIGEST_LENGTH];
    uint8_t fhash[MD_HASH::DIGEST_LENGTH];

    this->meta_buf_encrypt_transport.close(qhash, fhash);
}

void OutMetaSequenceTransport::next_meta_file()
{
    uint8_t qhash[MD_HASH::DIGEST_LENGTH];
    uint8_t fhash[MD_HASH::DIGEST_LENGTH];

    this->wrm_filter_encrypt_transport.close(qhash, fhash);

    const char * filename = this->filegen_.get_filename(this->num_file_);
    this->current_filename_[0] = 0;

    this->num_file_ ++;

    struct stat stat;
    if (fstat.stat(filename, stat)){
        throw Error(ERR_TRANSPORT_WRITE_FAILED);
    }

    MwrmWriterBuf mwrm_file_buf;
    mwrm_file_buf.write_line(
        filename, stat,
        this->start_sec_, this->stop_sec_ + 1,
        this->cctx.get_with_checksum(), qhash, fhash);
    this->meta_buf_encrypt_transport.send(mwrm_file_buf.buffer());

    this->start_sec_ = this->stop_sec_+1;
}
