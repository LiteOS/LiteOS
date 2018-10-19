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

#ifdef WITH_SOTA
#include "ota_pack_adaptor.h"
#include "fota/fota_package_head.h"
#include "stddef.h"
#include "upgrade_flag.h"



typedef struct
{
    atiny_fota_storage_device_s device;
    fota_hardware_s hardware;
    ota_opt_s *(*get_ota_opt)(void);
    ota_flash_type_e type;
    uint32_t total_len;
} ota_adaptor_device_s;


static ota_adaptor_device_s *ota_pack_get_adaptor_device();

static ota_opt_s * ota_pack_adaptor_get_ota_opt(ota_adaptor_device_s *device)
{
     if (device->get_ota_opt == NULL)
    {
        FOTA_LOG("ota_opt null");
        return NULL;
    }

    return device->get_ota_opt();
}

static int ota_pack_adaptor_write_software(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();
    ota_opt_s *ota_opt = ota_pack_adaptor_get_ota_opt(device);

    (void)thi;
    if (ota_opt == NULL)
    {
        FOTA_LOG("get_ota_opt fail");
        return FOTA_ERR;
    }
    return ota_opt->write_flash(device->type, buffer, len, offset);
}

static int ota_adaptor_read_software(struct fota_hardware_api_tag_s *this, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();
    ota_opt_s *ota_opt = ota_pack_adaptor_get_ota_opt(device);

    if (ota_opt == NULL)
    {
        FOTA_LOG("get_ota_opt fail");
        return FOTA_ERR;
    }
    return ota_opt->read_flash(device->type, buffer, len, offset);
}


static int  ota_adaptor_write_software_end(atiny_fota_storage_device_s *thi, atiny_download_result_e result, uint32_t total_len)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();

    (void)thi;
    (void)result;
    device->total_len = total_len;
    return FOTA_OK;
}

static uint32_t ota_pack_adator_get_block_size(struct fota_hardware_api_tag_s *this, uint32_t offset)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();
    ota_opt_s *ota_opt = ota_pack_adaptor_get_ota_opt(device);
    if (ota_opt == NULL)
    {
        FOTA_LOG("get_ota_opt fail");
        return FOTA_ERR;
    }

    return ota_opt->flash_block_size;
}

static int ota_pack_active_software(atiny_fota_storage_device_s *thi)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();
    return flag_set_info(device->type, device->total_len);
}



static ota_adaptor_device_s g_ota_storage_device = {.device = {.write_software = ota_pack_adaptor_write_software,
                                                    .write_software_end = ota_adaptor_write_software_end,
                                                    .active_software = ota_pack_active_software},

                                         .hardware = {.get_block_size = ota_pack_adator_get_block_size,
                                                      .read_software = ota_adaptor_read_software}};

static ota_adaptor_device_s *ota_pack_get_adaptor_device()
{
    return &g_ota_storage_device;
}

int ota_init_pack_adaptor(ota_opt_s *(*get_ota_opt)(void), fota_pack_device_info_s *device_info)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();

    ota_opt_s *ota_opt;

    if (get_ota_opt == NULL)
    {
        FOTA_LOG("get_ota_opt fail");
        return FOTA_ERR;
    }

    device->get_ota_opt = get_ota_opt;
    ota_opt = ota_pack_adaptor_get_ota_opt(device);
    memset(device_info, 0, sizeof(*device_info));

    device_info->storage_device = &device->device;
    device_info->hardware = &device->hardware;
    device_info->storage_device = &device->device;
    device_info->hardware = &device->hardware;
    device_info->key.rsa_N = ota_opt->key.rsa_N;
    device_info->key.rsa_E = ota_opt->key.rsa_E;

    return FOTA_OK;
}

int ota_set_adaptor_flash_type(atiny_fota_storage_device_s *thi, ota_flash_type_e type)
{
    ota_adaptor_device_s *device = ota_pack_get_adaptor_device();

    device->type = type;
    return FOTA_OK;
}

#endif


