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

#include "ota_port.h"
#include "hal_spi_flash.h"
#include "flag_manager.h"
#include "upgrade_flag.h"
#include <string.h>
#include <stdlib.h>
#include "osdepends/atiny_osdep.h"
#include <board.h>

#define FLASH_BLOCK_SIZE 0x1000

#define FLASH_BLOCK_MASK 0xfff
#define HAL_OTA_LOG(fmt, ...) \
(void)printf("[%s:%d][%lu]" fmt "\r\n",  __FUNCTION__, __LINE__, (uint32_t) atiny_gettime_ms(),  ##__VA_ARGS__)


#define OK 0
#define ERR -1


static int hal_ota_write_flash(uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    int ret;
    uint8_t *block_buff;

    if((NULL == buffer) || (0 == len) || (len > FLASH_BLOCK_SIZE)
        || ((offset & FLASH_BLOCK_MASK)))
    {
        HAL_OTA_LOG("invalid param len %ld, offset %ld", len, offset);
        return ERR;
    }

    if (len == FLASH_BLOCK_SIZE)
    {
        ret = hal_spi_flash_erase_write(buffer, FLASH_BLOCK_SIZE, offset);
        if(ret != OK)
        {
           HAL_OTA_LOG("hal_ota_write_flash fail offset %lu, len %u", offset, FLASH_BLOCK_SIZE);
        }
        return ret;
    }

    block_buff = atiny_malloc(FLASH_BLOCK_SIZE);
    if(NULL == block_buff)
    {
        HAL_OTA_LOG("atiny_malloc fail");
        return ERR;
    }


    ret = hal_spi_flash_read(block_buff + len, FLASH_BLOCK_SIZE - len, offset + len);
    if(ret != OK)
    {
        HAL_OTA_LOG("hal_spi_flash_read fail offset %lu, len %lu", offset + len, FLASH_BLOCK_SIZE - len);
        return ret;
    }
    (void)memcpy(block_buff, buffer, len);
    ret = hal_spi_flash_erase_write(block_buff, FLASH_BLOCK_SIZE, offset);
    if(ret != OK)
    {
        HAL_OTA_LOG("hal_ota_write_flash fail offset %lu, len %u", offset, FLASH_BLOCK_SIZE);
    }
    return ret;
}



void hal_init_ota(void)
{
    hal_spi_flash_config();
}

static const uint32_t g_flash_base_addrs[] = {OTA_IMAGE_DOWNLOAD_ADDR, OTA_IMAGE_DOWNLOAD_ADDR, OTA_FLAG_ADDR1};
static const uint32_t g_flash_max_size[] = {OTA_IMAGE_DOWNLOAD_SIZE, OTA_IMAGE_DOWNLOAD_SIZE, FLASH_BLOCK_SIZE};

static int hal_check_flash_param(ota_flash_type_e type, int32_t len, uint32_t location)
{
    if (type > OTA_UPDATE_INFO)
    {
        HAL_OTA_LOG("err type %d", type);
        return ERR;
    }

    if(len > g_flash_max_size[type])
    {
        HAL_OTA_LOG("err offset %lu, len %lu", location, len);
        return ERR;
    }

    return OK;
}

static int hal_read_flash(ota_flash_type_e type, void *buf, int32_t len, uint32_t location)
{
    if (hal_check_flash_param(type, len, location) != OK)
    {
        return ERR;
    }

    return hal_spi_flash_read(buf, len, g_flash_base_addrs[type] + location);
}

static int hal_write_flash(ota_flash_type_e type, const void *buf, int32_t len, uint32_t location)
{
    if (hal_check_flash_param(type, len, location) != OK)
    {
        return ERR;
    }

    return hal_ota_write_flash(g_flash_base_addrs[type] + location, (const uint8_t *)buf, len);
}


void hal_get_ota_opt(ota_opt_s *opt)
{
    if (opt == NULL)
    {
        HAL_OTA_LOG("opt NULL");
        return;
    }

    memset(opt, 0, sizeof(*opt));
    opt->read_flash = hal_read_flash;
    opt->write_flash = hal_write_flash;
    opt->flash_block_size = FLASH_BLOCK_SIZE;
}



