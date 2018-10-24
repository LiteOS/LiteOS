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

#include "fota/fota_package_storage_device.h"
#include "fota/fota_package_head.h"
#include "fota_firmware_writer.h"
#if (FOTA_PACK_CHECKSUM != FOTA_PACK_NO_CHECKSUM)
#include "dtls_interface.h"
#endif

typedef struct
{
    atiny_fota_storage_device_s interface;
    fota_hardware_s hardware;
    ota_opt_s ota_opt;
    fota_pack_head_s head;
    fota_firmware_writer_s writer;
    uint32_t total_len;
    ota_flash_type_e type;
    int init_flag;
} fota_pack_storage_device_s;

static inline fota_pack_storage_device_s *fota_pack_storage_get_storage_device(atiny_fota_storage_device_s *this)
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
            FOTA_LOG("check err,len %u", total_len);
            break;
        }

    }
    while(0);

    device->total_len = total_len;

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
    if(0 == len || buffer == NULL)
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

    if (fota_pack_head_get_checksum(&device->head))
    {
        ret = fota_pack_checksum_update_data(fota_pack_head_get_checksum(&device->head), offset,
                                             buffer, len, &device->hardware);
        if(ret != FOTA_OK)
        {
            FOTA_LOG("fota_pack_checksum_update_data fail %d", ret);
        }
    }

    return FOTA_OK;

}

int fota_pack_storage_active_software(atiny_fota_storage_device_s *thi)
{
    fota_pack_storage_device_s *device = fota_pack_storage_get_storage_device(thi);

    if(NULL == thi)
    {
        FOTA_LOG("null err");
        return FOTA_ERR;
    }

    return flag_set_info((OTA_FULL_SOFTWARE == device->type) ? UPGRADE_FULL : UPGRADE_DIFF,
                        device->total_len);
    }

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
    device->init_flag = true;
}


static fota_pack_storage_device_s g_fota_pack_storage_device;

atiny_fota_storage_device_s *fota_get_pack_device(void)
{

    fota_pack_storage_device_s *device  = &g_fota_pack_storage_device;
    fota_init_pack_device(device);
    return &device->interface;
}


static int ota_pack_read_software(struct fota_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)fota_get_pack_device();

    (void)thi;
    if (device->ota_opt.read_flash)
    {
        return device->ota_opt.read_flash(device->type, buffer, len, offset);
    }
    FOTA_LOG("read_flash null");

    return FOTA_ERR;
}

static int ota_pack_write_software(struct fota_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)fota_get_pack_device();

    (void)thi;
    if (device->ota_opt.write_flash)
    {
        return device->ota_opt.write_flash(device->type, buffer, len, offset);
    }
    FOTA_LOG("write_flash null");

    return FOTA_ERR;
}

static void ota_pack_set_flash_type(struct fota_hardware_tag_s *thi, ota_flash_type_e type)
{
    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)fota_get_pack_device();

    device->type = type;
    FOTA_LOG("ota_pack_set_flash_type type %d", type);
}

static uint32_t ota_pack_get_block_size(struct fota_hardware_tag_s *thi, uint32_t offset)
{
    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)fota_get_pack_device();
    return device->ota_opt.flash_block_size;
}

int ota_init_pack_device(const ota_opt_s *ota_opt)
{
    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)fota_get_pack_device();
    fota_pack_device_info_s device_info;

    if (ota_opt == NULL)
    {
        FOTA_LOG("ota_opt null");
        return FOTA_ERR;
    }

    memcpy(&device->ota_opt, ota_opt, sizeof(device->ota_opt));
#if (FOTA_PACK_CHECKSUM != FOTA_PACK_NO_CHECKSUM)
    dtls_int();
#endif
    device->hardware.read_software = ota_pack_read_software;
    device->hardware.write_software = ota_pack_write_software;
    device->hardware.set_flash_type = ota_pack_set_flash_type;
    device->hardware.get_block_size = ota_pack_get_block_size;

    device_info.hardware = &device->hardware;
    memcpy(&device_info.key, &device->ota_opt.key, sizeof(device_info.key));

    if(fota_pack_head_set_head_info(&device->head,  &device_info) != FOTA_OK)
    {
        return FOTA_ERR;
    }

    fota_fmw_wr_set_device(&device->writer, device_info.hardware);

    return FOTA_OK;
}


