/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "fota_firmware_writer.h"
#include "fota/fota_package_head.h"
#include "upgrade_flag.h"


void fota_fmw_wr_init(fota_firmware_writer_s *writer)
{
    (void)memset(writer, 0, sizeof(*writer));
}
void fota_fmw_wr_free_save_buffer(fota_firmware_writer_s *writer)
{
    if(writer->buffer)
    {
        atiny_free(writer->buffer);
        writer->buffer = NULL;
    }
    writer->buffer_stored_len = 0;
    writer->block_size = 0;
}

void fota_fmw_wr_destroy(fota_firmware_writer_s *writer)
{
    writer->offset_flag = false;
    fota_fmw_wr_free_save_buffer(writer);

}


void fota_fmw_wr_set_device(fota_firmware_writer_s *writer, fota_hardware_s *hardware)
{
    writer->hardware = hardware;
}

int fota_fmw_wr_check(fota_firmware_writer_s *writer)
{
    if(NULL == writer->hardware)
    {
        FOTA_LOG("null poiter");
        return FOTA_ERR;
    }
    return FOTA_OK;
}

static int fota_fmw_write_data(fota_firmware_writer_s *writer, uint32_t offset, const uint8_t *buffer, uint32_t len)
{

    int ret = writer->hardware->write_software(writer->hardware,  offset, buffer, len);

    if (0 == offset)
    {
        (void)flag_enable_hwpatch(buffer, len);
    }
    return ret;
}
int fota_fmw_wr_write_stored_data(fota_firmware_writer_s *writer)
{
    int ret;

#if (OTA_COMBINE_TO_WRITE_LAST_BLOCK == OTA_YES)
    if (writer->buffer_stored_len < writer->block_size)
    {
        ret = writer->hardware->read_software(writer->hardware,  writer->offset,
                    writer->buffer + writer->buffer_stored_len, writer->block_size - writer->buffer_stored_len);
        if (ret != FOTA_OK)
        {
            FOTA_LOG("read_software fail offset %ld, size %ld ret %d", writer->offset,
                                writer->block_size - writer->buffer_stored_len, ret);
            return ret;
        }

        writer->buffer_stored_len = writer->block_size;
    }
#endif

    ret = fota_fmw_write_data(writer,  writer->offset, writer->buffer, writer->buffer_stored_len);
    if(ret != FOTA_OK)
    {
        FOTA_LOG("write_software err ret %d, offset %u, len %u", ret, writer->offset, writer->buffer_stored_len);
        return FOTA_ERR;
    }

    writer->offset_flag = false;
    return FOTA_OK;
}



static int fota_fmw_wr_begin_not_aligned(fota_firmware_writer_s *writer, uint32_t block_begin_offset, uint32_t block_size,
                                uint32_t offset, const uint8_t *buff, uint16_t len , uint16_t *out_len)
{
       uint32_t block_left_len = (block_begin_offset + block_size) - offset;
       uint32_t write_len = MIN(len, block_left_len);
       int ret;

        if (writer->offset_flag && writer->buffer)
        {
            memcpy(writer->buffer + writer->buffer_stored_len, buff, write_len);
            writer->buffer_stored_len += write_len;
        }
        else
        {
            if (writer->buffer == NULL)
            {
                writer->buffer = atiny_malloc(block_size);
                if (writer->buffer == NULL)
                {
                    FOTA_LOG("malloc null %d", block_size);
                    return FOTA_ERR;
                }
                writer->block_size = block_size;
            }

            writer->buffer_stored_len = (offset - block_begin_offset);
            ret = writer->hardware->read_software(writer->hardware, block_begin_offset, writer->buffer,
                    writer->buffer_stored_len);
            if (ret != FOTA_OK)
            {
                FOTA_LOG("read_software fail offset %ld, size %ld ret %d", block_begin_offset, writer->buffer_stored_len, ret);
                return ret;
            }
            memcpy(writer->buffer + writer->buffer_stored_len, buff, write_len);
            writer->offset_flag = true;
            writer->offset = block_begin_offset;
            writer->buffer_stored_len += write_len;
        }


        if (writer->buffer_stored_len >= block_size)
        {
            ret = fota_fmw_write_data(writer, writer->offset, writer->buffer, block_size);
            if (ret != FOTA_OK)
            {
                FOTA_LOG("fota_fmw_write_data fail,offset %ld, size %ld, ret %ld",
                        writer->offset, block_size, ret);
                return ret;
            }
            writer->offset_flag = false;
        }

        *out_len = (uint16_t)write_len;

        return FOTA_OK;
}

static int fota_fmw_wr_entire_blocks(fota_firmware_writer_s *writer, uint32_t block_begin_offset,
                                    uint32_t block_size, const uint8_t *buff, uint16_t len, uint16_t *out_len)
{
    int ret;

    *out_len = 0;

    for (; len >= block_size; len -= block_size, block_begin_offset += block_size,
                                buff += block_size, *out_len += block_size)
    {
        ret = fota_fmw_write_data(writer, block_begin_offset, buff, block_size);
        if (ret != FOTA_OK)
        {
            FOTA_LOG("fota_fmw_write_data fail,offset %ld, size %ld, ret %ld",
                    writer->offset, block_size, ret);
            return ret;
        }
    }

    return FOTA_OK;
}


static int fota_fmw_wr_end_not_aligned_block(fota_firmware_writer_s *writer, uint32_t block_begin_offset,
                                            uint32_t block_size, const uint8_t *buff, uint16_t len)
{
    if (writer->buffer == NULL)
    {
        writer->buffer = atiny_malloc(block_size);
        if (writer->buffer == NULL)
        {
            FOTA_LOG("malloc null %d", block_size);
            return FOTA_ERR;
        }
        writer->block_size = block_size;
    }

    memcpy(writer->buffer, buff, len);
    writer->buffer_stored_len = len;
    writer->offset_flag = true;
    writer->offset = block_begin_offset;
    return FOTA_OK;
}

int fota_fmw_wr_write(fota_firmware_writer_s *writer, uint32_t offset, const uint8_t *buff, uint16_t len)
{
    uint16_t write_len = 0;
    int ret;
    uint32_t block_size;

    if(fota_fmw_wr_check(writer) != FOTA_OK)
    {
        return FOTA_ERR;
    }


    if(writer->offset_flag && (writer->offset + writer->buffer_stored_len != offset)
        && (writer->buffer_stored_len > 0))
    {

        ret = fota_fmw_wr_write_stored_data(writer);
        if (ret != FOTA_OK)
        {
            return ret;
        }
    }

    block_size = writer->hardware->get_block_size(writer->hardware);
    if (block_size == 0)
    {
        FOTA_LOG("err block_size is 0");
        return FOTA_ERR;
    }

    if (offset % block_size)
    {
        uint32_t block_begin;

        block_begin = ((offset / block_size) * block_size);
        ret = fota_fmw_wr_begin_not_aligned(writer, block_begin, block_size, offset, buff, len, &write_len);
        if (ret != FOTA_OK)
        {
            return ret;
        }

        buff += write_len;
        len -= write_len;
        offset += write_len;
    }

    if (len <= 0)
    {
        return FOTA_OK;
    }

    ret = fota_fmw_wr_entire_blocks(writer, offset, block_size, buff, len, &write_len);
    if (ret != FOTA_OK)
    {
        return ret;
    }

    buff += write_len;
    len -= write_len;
    offset += write_len;

    if (len <= 0)
    {
        return FOTA_OK;
    }

    return fota_fmw_wr_end_not_aligned_block(writer, offset, block_size, buff, len);

}

int fota_fmw_wr_write_end(fota_firmware_writer_s *writer)
{
    if(0 == writer->buffer_stored_len)
    {
        return FOTA_OK;
    }

    return fota_fmw_wr_write_stored_data(writer);
}




