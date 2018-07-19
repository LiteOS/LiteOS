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

#include "fota_port.h"
#include "ota.h"
#include <string.h>
#include <stdlib.h>
#include <atiny_adapter.h>
#include <board.h>




#define FLASH_BLOCK_SIZE 0x1000
#define FLASH_BLOCK_MASK 0xfff
#define HAL_FOTA_LOG(fmt, ...) \
(void)printf("[%s:%d][%lu]" fmt "\r\n",  __FUNCTION__, __LINE__, (uint32_t) atiny_gettime_ms(),  ##__VA_ARGS__)

#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif /* MIN */

#define OK 0
#define ERR -1


typedef struct
{
    atiny_fota_storage_device_s device;
    fota_hardware_s hardware;
    uint32_t total_len;
}ota_hal_s;

ota_hal_s g_hal_storage_device ;


int hal_get_fota_device(atiny_fota_storage_device_s **storage_device, fota_hardware_s **hardware)
{
    if((NULL == storage_device)
        || (NULL == hardware))
    {
        HAL_FOTA_LOG("null pointer");
        return ERR;
    }
    *storage_device = &g_hal_storage_device.device;
    *hardware = &g_hal_storage_device.hardware;
    return OK;
}

int hal_init_fota(void)
{
    ota_assist assist;
    int ret;

    assist.func_printf = printf;
    assist.func_ota_read = NULL;
    assist.func_ota_write = NULL;
    ota_register_assist(&assist);

    ret = ota_init();
    if (ret != OK)
    {
        OTA_LOG("read/write boot information failed");
    }

    return ret;
}


