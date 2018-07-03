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

#include "internals.h"
#include "agenttiny.h"
#include "atiny_update_info.h"

#define OFFSET_BASE_TOCKEN_INFO (0U)
#define OFFSET_BASE_FW_DOWNLOAD_INFO (32U)

struct atiny_update_info_tag_s
{
    atiny_fota_storage_device_s *device;
};

static atiny_update_info_s g_update_info = {0};

int atiny_update_info_set(atiny_update_info_s *thi, atiny_fota_storage_device_s *device)
{
    if(NULL == thi || NULL == device)
        return -1;

    thi->device = device;

    return 0;
}

int atiny_update_info_write(atiny_update_info_s *thi, atiny_update_info_e type, const uint8_t *info, uint32_t len)
{
    uint32_t offset = 0;

    if(NULL == thi || type >= ATINY_UPDATE_INFO_MAX || NULL == info)
        return -1;

    switch ( type )
    {
        case TOCKEN_INFO:
            offset = OFFSET_BASE_TOCKEN_INFO;
            break;
        case FW_DOWNLOAD_INFO:
            offset = OFFSET_BASE_FW_DOWNLOAD_INFO;
            break;
        default:
            return -1;
    }
    return thi->device->write_update_info(thi->device, offset, info, len);
}

int atiny_update_info_read(atiny_update_info_s *thi, atiny_update_info_e type, uint8_t *info, uint32_t len)
{
    uint32_t offset = 0;

    if(NULL == thi || type >= ATINY_UPDATE_INFO_MAX || NULL == info)
        return -1;

    switch ( type )
    {
        case TOCKEN_INFO:
            offset = OFFSET_BASE_TOCKEN_INFO;
            break;
        case FW_DOWNLOAD_INFO:
            offset = OFFSET_BASE_FW_DOWNLOAD_INFO;
            break;
        default:
            return -1;
    }
    return thi->device->read_update_info(thi->device, offset, info, len);
}

atiny_update_info_s *atiny_update_info_get_instance(void)
{
    return &g_update_info;
}

