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

#include "fota_package_storage_device.h"
#include "fota_package_head.h"
#include "fota_firmware_writer.h"



typedef struct
{
    atiny_fota_storage_device_s interface;
    atiny_fota_storage_device_s *storage_device;
    fota_hardware_s *hardware;

    fota_pack_head_s head;
    fota_firmware_writer_s writer;
    fota_pack_checksum_s *checksum;
    uint32_t total_len;
    int init_flag;
}fota_pack_storage_device_s;

static inline fota_pack_storage_device_s * fota_pack_storage_get_storage_device(atiny_fota_storage_device_s *this)
{
    return (fota_pack_storage_device_s *)this;
}

static int fota_pack_storage_write_software_end(atiny_fota_storage_device_s *this, atiny_download_result_e result, uint32_t total_len)
{
    fota_pack_storage_device_s *device;
    int ret = FOTA_OK;

    if(NULL == this)
    {
        FOTA_LOG("null pointer");
        return FOTA_ERR;
    }

    device = fota_pack_storage_get_storage_device(this);

    do
    {

        if(fota_fmw_wr_write_end(&device->writer) != FOTA_OK)
        {
            ret = FOTA_ERR;
            break;
        }

        if(result != ATINY_FOTA_DOWNLOAD_OK)
        {
            break;
        }

        if(fota_pack_head_check(&device->head, total_len) != FOTA_OK)
        {
            ret = FOTA_ERR;
            FOTA_LOG("check len err,len %u", total_len);
            break;
        }

    }while(0);


    if(device->storage_device && device->storage_device->write_software_end)
    {
        uint32_t head_len = fota_pack_head_get_head_len(&device->head);
        uint32_t notify_len = 0;
        if(ATINY_FOTA_DOWNLOAD_OK == result)
        {
            notify_len = ((total_len >= head_len) ? (total_len - head_len) : 0);
        }
        ret |= device->storage_device->write_software_end(device->storage_device, result, notify_len);
    }

    //current download finish, clear the save data
    if(ATINY_FOTA_DOWNLOAD_OK == result)
    {
        fota_pack_head_destroy(&device->head);
        fota_fmw_wr_destroy(&device->writer);
    }

    return ret;
}


static int fota_pack_storage_write_software(atiny_fota_storage_device_s *this,
                                           uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    fota_pack_storage_device_s *device;
    uint16_t used_len = 0;
    int ret;

    if(NULL == this)
    {
        FOTA_LOG("null pointer");
        return FOTA_ERR;
    }
    if(0 == len)
    {
        FOTA_LOG("write 0 len");
        return FOTA_ERR;
    }


    device = fota_pack_storage_get_storage_device(this);

    ret = fota_pack_head_parse(&device->head, offset, buffer, (uint16_t)len, &used_len);
    if(ret != FOTA_OK)
    {
        FOTA_LOG("fota_pack_head_parse fail %d", ret);
        return ret;
    }

    if(used_len == len)
    {
        return FOTA_OK;
    }

    len -= used_len;
    offset += used_len;
    buffer += used_len;

    ret = fota_fmw_wr_write(&device->writer, offset - fota_pack_head_get_head_len(&device->head), buffer, len);
    if(ret != FOTA_OK)
    {
        FOTA_LOG("fota_fmw_wr_write fail %d", ret);
        return ret;
    }

    ret = fota_pack_checksum_update_data(fota_pack_head_get_checksum(&device->head), offset,
                buffer, len, device->hardware);
    if(ret != FOTA_OK)
    {
        FOTA_LOG("fota_pack_checksum_update_data fail %d", ret);
    }

    return ret;

}


#define DEFINE_PROXY_FUNC(name, function, types_args, args)\
int name types_args\
{\
    fota_pack_storage_device_s *device = fota_pack_storage_get_storage_device(this);\
    int ret;\
     if((NULL == this)\
        || (NULL == device->storage_device)\
        || (NULL == device->storage_device->function))\
    {\
        FOTA_LOG("null err");\
        return FOTA_ERR;\
    }\
    ret = device->storage_device->function args;\
    if(ret != FOTA_OK)\
    {\
        FOTA_LOG("call " #function " fail");\
    }\
    return ret;\
}

DEFINE_PROXY_FUNC(fota_pack_storage_active_software, active_software,
                    (atiny_fota_storage_device_s *this), (((fota_pack_storage_device_s *)this)->storage_device))


DEFINE_PROXY_FUNC(fota_pack_storage_get_software_result, get_software_result,
                    (atiny_fota_storage_device_s *this), (((fota_pack_storage_device_s *)this)->storage_device))



DEFINE_PROXY_FUNC(fota_pack_storage_write_update_info, write_update_info,
                    (atiny_fota_storage_device_s *this, uint32_t offset, const uint8_t *buffer, uint32_t len),
                    (((fota_pack_storage_device_s *)this)->storage_device, offset, buffer, len))


DEFINE_PROXY_FUNC(fota_pack_storage_read_update_info, read_update_info,
                    (atiny_fota_storage_device_s *this, uint32_t offset, uint8_t *buffer, uint32_t len),
                    (((fota_pack_storage_device_s *)this)->storage_device, offset, buffer, len))




static void fota_init_pack_device(fota_pack_storage_device_s *device)
{
    if(device->init_flag)
    {
        return;
    }

    (void)memset(device, 0, sizeof(*device));
    fota_fmw_wr_init(&device->writer);
    fota_pack_head_init(&device->head);

    device->interface.write_software = fota_pack_storage_write_software;
    device->interface.write_software_end = fota_pack_storage_write_software_end;
    device->interface.active_software = fota_pack_storage_active_software;
    device->interface.get_software_result = fota_pack_storage_get_software_result;
    device->interface.write_update_info = fota_pack_storage_write_update_info;
    device->interface.read_update_info = fota_pack_storage_read_update_info;
    device->init_flag = true;
}


static fota_pack_storage_device_s g_fota_pack_storage_device;

atiny_fota_storage_device_s *fota_get_pack_device(void)
{

    fota_pack_storage_device_s *device  = &g_fota_pack_storage_device;
    fota_init_pack_device(device);
    return &device->interface;
}


int fota_set_pack_device(atiny_fota_storage_device_s *device,  fota_pack_device_info_s *device_info)
{
    fota_pack_storage_device_s *pack_device;

    if((NULL == device) || (NULL == device_info)
        || (NULL == device_info->storage_device) || (NULL == device_info->hardware))
    {
        FOTA_LOG("null err");
        return FOTA_ERR;
    }

    pack_device = fota_pack_storage_get_storage_device(device);

    if(fota_pack_head_set_head_info(&pack_device->head,  device_info->hardware, NULL, pack_device) != FOTA_OK)
    {
        return FOTA_ERR;
    }

    fota_fmw_wr_set_device(&pack_device->writer, device_info->storage_device, device_info->hardware);
    pack_device->storage_device = device_info->storage_device;
    pack_device->hardware = device_info->hardware;

    return FOTA_OK;
}

