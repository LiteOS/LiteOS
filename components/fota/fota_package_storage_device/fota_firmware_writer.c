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
#include "fota_package_head.h"


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
    writer->buffer_len = 0;
}

void fota_fmw_wr_destroy(fota_firmware_writer_s *writer)
{
    writer->offset_flag = false;
    fota_fmw_wr_free_save_buffer(writer);

}


void fota_fmw_wr_set_device(fota_firmware_writer_s *writer, atiny_fota_storage_device_s *storage_device, fota_hardware_s *hardware)
{
    writer->storage_device = storage_device;
    writer->hardware = hardware;
}

int fota_fmw_wr_check(fota_firmware_writer_s *writer)
{
    if((NULL == writer->hardware)
        || ((NULL == writer->hardware->get_block_size))
        || (NULL == writer->storage_device)
        || (NULL == writer->storage_device->write_software))
    {
        FOTA_LOG("null poiter");
        return FOTA_ERR;
    }
    return FOTA_OK;
}

int fota_fmw_wr_create_buffer(fota_firmware_writer_s *writer, uint32_t offset)
{
    uint32_t block_size = writer->hardware->get_block_size(writer->hardware, offset);
    if(0 == block_size)
    {
        FOTA_LOG("get_block_size 0 size");
        return FOTA_ERR;
    }

    if((block_size == writer->buffer_len)
        && (writer->buffer))
    {
        writer->buffer_stored_len = 0;
        return FOTA_OK;
    }

    fota_fmw_wr_free_save_buffer(writer);

    writer->buffer = atiny_malloc(block_size);
    if(NULL == writer->buffer)
    {
        FOTA_LOG("atiny_malloc fail");
        return FOTA_ERR;
    }
    writer->buffer_len = block_size;
    writer->buffer_stored_len = 0;
    return FOTA_OK;
}

static inline uint32_t fota_fmw_wr_get_offset_info(fota_firmware_writer_s *writer, uint32_t offset, uint32_t *block_size, uint32_t *block_end)
{
    *block_size = writer->hardware->get_block_size(writer->hardware, offset);
    if(*block_size <= 0)
    {
        FOTA_LOG("block_size 0");
        return FOTA_ERR;
    }

    *block_end = (offset + *block_size) - ((offset + *block_size) % *block_size);
    return FOTA_OK;
}

int fota_fmw_wr_write_stored_data(fota_firmware_writer_s *writer)
{
    int ret;

    ret = writer->storage_device->write_software(writer->storage_device,  writer->offset, writer->buffer, writer->buffer_stored_len);
    if(ret != FOTA_OK)
    {
        FOTA_LOG("write_software err ret %d, offset %u, len %u", ret, writer->offset, writer->buffer_stored_len);
        return FOTA_ERR;
    }

    writer->offset += writer->buffer_stored_len;
    writer->buffer_stored_len = 0;
    return FOTA_OK;
}



int fota_fmw_wr_write_combined_data(fota_firmware_writer_s *writer,
                                            uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *write_len)
{
    uint32_t block_size;
    uint32_t block_end;
    uint16_t copy_len;

    *write_len = 0;
    if((NULL == writer->buffer) ||  (0 == writer->buffer_stored_len))
    {
        return FOTA_OK;
    }

    if(writer->offset_flag && (writer->offset + writer->buffer_stored_len != offset))
    {
        return fota_fmw_wr_write_stored_data(writer);
    }

    if(fota_fmw_wr_get_offset_info(writer, offset, &block_size, &block_end) != FOTA_OK)
    {
        return FOTA_ERR;
    }

    if(block_size != writer->buffer_len)
    {
        FOTA_LOG("err space size buf size %u, block size %u, offset %u", writer->buffer_len, block_size, offset);
        return FOTA_ERR;
    }


    copy_len = MIN(block_end - offset, len);
    copy_len = MIN(writer->buffer_len - writer->buffer_stored_len, copy_len);

    memcpy(writer->buffer + writer->buffer_stored_len, buff, copy_len);
    writer->buffer_stored_len += copy_len;
    *write_len = copy_len;
    if(((offset + copy_len) < block_end) && ((writer->buffer_stored_len) < block_size))
    {
        return FOTA_OK;
    }

    return fota_fmw_wr_write_stored_data(writer);

}

int fota_fmw_wr_write_buffer_data(fota_firmware_writer_s *writer, uint32_t offset,
                                              const uint8_t *buff, uint16_t len)
{
    uint32_t block_size;
    int ret;
    uint32_t block_end;
//    uint32_t offset_end;

    if(fota_fmw_wr_get_offset_info(writer, offset + len - 1, &block_size, &block_end) != FOTA_OK)
    {
        return FOTA_ERR;
    }

#if 0
    offset_end = offset + len;
    while(block_end  <= offset_end)
    {
        block_size = writer->hardware->get_block_size(writer->hardware, block_end);
        block_end += block_size;
    };
#endif
    block_end -= block_size;

    if(offset < block_end)
    {
        uint16_t write_len = block_end - offset;

        ret = writer->storage_device->write_software(writer->storage_device, offset, buff, write_len);
        if(ret != FOTA_OK)
        {
            FOTA_LOG("write_software err ret %d, offset %u, len %u", ret, offset, block_end - offset);
        }
        buff += write_len;
        len -= write_len;
        offset = block_end;
        writer->offset = offset;
        writer->offset_flag = true;
    }

    if(len > 0)
    {
        if(fota_fmw_wr_create_buffer(writer, offset) != FOTA_OK)
        {
            return FOTA_ERR;
        }
        if(writer->buffer_len < len)
        {
            FOTA_LOG("len err buffer len %u, len %u", writer->buffer_len, len);
            return FOTA_ERR;
        }
        memcpy(writer->buffer, buff, len);
        writer->offset = offset;
        writer->buffer_stored_len = len;
        writer->offset_flag = true;
    }

    return FOTA_OK;
}


int fota_fmw_wr_write(fota_firmware_writer_s *writer, uint32_t offset, const uint8_t *buff, uint16_t len)
{
    uint16_t write_len = 0;

    if(fota_fmw_wr_check(writer) != FOTA_OK)
    {
        return FOTA_ERR;
    }

    if(fota_fmw_wr_write_combined_data(writer, offset,
                  buff, len, &write_len) != FOTA_OK)
    {
        return FOTA_ERR;
    }

    buff += write_len;
    len -= write_len;
    offset += write_len;

    if(len <= 0)
    {
        return FOTA_OK;
    }
    return fota_fmw_wr_write_buffer_data(writer, offset, buff, len);
}

int fota_fmw_wr_write_end(fota_firmware_writer_s *writer)
{
    if(0 == writer->buffer_stored_len)
    {
        return FOTA_OK;
    }

    return fota_fmw_wr_write_stored_data(writer);
}




