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
#include "hal_spi_flash.h"
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

static int hal_fota_write_begin_not_aligned_buffer(uint32_t offset, const uint8_t *buffer, uint32_t len, uint32_t *used_len, uint8_t **block_buff)
{
    uint32_t not_aligned = (offset & FLASH_BLOCK_MASK);
    int ret;

    *used_len = 0;
    if(not_aligned > 0)
    {
        uint32_t not_aligned_size;
        uint32_t aligned_offset = (offset & (~FLASH_BLOCK_MASK));
        uint32_t copy_len;

        if(NULL == *block_buff)
        {
             *block_buff = atiny_malloc(FLASH_BLOCK_SIZE);
            if(NULL == *block_buff)
            {
                 HAL_FOTA_LOG("atiny_malloc fail");
                 return ERR;
            }
        }

        ret = hal_spi_flash_read(*block_buff, FLASH_BLOCK_SIZE, aligned_offset);
        if(ret != OK)
        {
            HAL_FOTA_LOG("hal_spi_flash_read fail offset %lu, len %u", aligned_offset, FLASH_BLOCK_SIZE);
            return ERR;
        }
        not_aligned_size =  FLASH_BLOCK_SIZE - not_aligned;
        copy_len = MIN(len, not_aligned_size);
        (void)memcpy(*block_buff + not_aligned, buffer, copy_len);
        ret = hal_spi_flash_erase_write(*block_buff, FLASH_BLOCK_SIZE, aligned_offset);
        if(ret != OK)
        {
            HAL_FOTA_LOG("hal_fota_write_flash fail offset %lu, len %u", aligned_offset, FLASH_BLOCK_SIZE);
            return ERR;
        }

        *used_len = copy_len;

    }

    return OK;
}


static int hal_fota_write_end_not_aligned_buffer(uint32_t offset, const uint8_t *buffer, uint32_t len, uint8_t **block_buff)
{
    int ret;

    if(NULL == *block_buff)
    {
        *block_buff = atiny_malloc(FLASH_BLOCK_SIZE);
        if(NULL == *block_buff)
        {
            HAL_FOTA_LOG("atiny_malloc fail");
            return ERR;
        }
    }

    ret = hal_spi_flash_read(*block_buff, FLASH_BLOCK_SIZE, offset);
    if(ret != OK)
    {
        HAL_FOTA_LOG("hal_spi_flash_read fail offset %lu, len %lu", offset, len);
        return ERR;
    }
    (void)memcpy(*block_buff, buffer, len);
    ret = hal_spi_flash_erase_write(*block_buff, FLASH_BLOCK_SIZE, offset);
    if(ret != OK)
    {
        HAL_FOTA_LOG("hal_fota_write_flash fail offset %lu, len %u", offset, FLASH_BLOCK_SIZE);
        return ERR;
    }

    return OK;
}




static int hal_fota_write_flash(uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    int ret = ERR;
    uint32_t used_len = 0;
    uint8_t *block_buff = NULL;

    if(NULL == buffer)
    {
        HAL_FOTA_LOG("null pointer");
        return ERR;
    }
    if(0 == len)
    {
        HAL_FOTA_LOG("len 0");
        return ERR;
    }

    ret  = hal_fota_write_begin_not_aligned_buffer(offset, buffer, len, &used_len, &block_buff);
    if(ret != OK)
    {
        goto EXIT;
    }

    len -= used_len;
    if(len == 0)
    {
       goto EXIT;
    }
    buffer += used_len;
    offset += used_len;


    while(len >= FLASH_BLOCK_SIZE)
    {
       ret = hal_spi_flash_erase_write(buffer, FLASH_BLOCK_SIZE, offset);
       if(ret != OK)
       {
           HAL_FOTA_LOG("hal_fota_write_flash fail offset %lu, len %u", offset, FLASH_BLOCK_SIZE);
           goto EXIT;
       }
       len -= FLASH_BLOCK_SIZE;
       buffer += FLASH_BLOCK_SIZE;
       offset += FLASH_BLOCK_SIZE;
    }

    if(len > 0)
    {
        ret = hal_fota_write_end_not_aligned_buffer(offset, buffer, len, &block_buff);
    }

EXIT:
    if(block_buff != NULL)
    {
        atiny_free(block_buff);
    }

    return ret;

}


static int hal_fota_write_software(atiny_fota_storage_device_s *this, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    if((offset + len) > OTA_IMAGE_DOWNLOAD_SIZE)
    {
        HAL_FOTA_LOG("err offset %lu, len %lu", offset, len);
        return ERR;
    }
    return hal_fota_write_flash(OTA_IMAGE_DOWNLOAD_ADDR + offset, buffer, len);
}

static int  hal_fota_write_software_end(atiny_fota_storage_device_s *this, atiny_download_result_e result, uint32_t total_len)
{
    ota_hal_s *hal = (ota_hal_s *)this;

    if(NULL == this)
    {
        HAL_FOTA_LOG("null pointer");
        return ERR;
    }

    hal->total_len = (ATINY_FOTA_DOWNLOAD_OK == result) ? total_len : 0;

    return OK;
}

static int hal_fota_active_software(atiny_fota_storage_device_s *this)
{
    ota_hal_s *hal = (ota_hal_s *)this;

    if(NULL == this)
    {
        HAL_FOTA_LOG("null pointer");
        return ERR;
    }

    if(0 == hal->total_len)
    {
        HAL_FOTA_LOG("0 len");
        return ERR;
    }

    return ota_set_reboot(hal->total_len);
}

static int hal_fota_get_software_result(atiny_fota_storage_device_s *this)
{
    ota_state state;
    if(ota_check_update_state(&state) != OK)
    {
        HAL_FOTA_LOG("ota_check_update_state fail");
    }

    return (OTA_S_SUCCEED == state) ? OK : ERR;
}

static int hal_fota_write_update_info(atiny_fota_storage_device_s *this, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    if(offset + len >  UPDATE_INFO_SIZE)
    {
        HAL_FOTA_LOG("err offset %lu, len %lu", offset, len);
        return ERR;
    }
    return hal_fota_write_flash(UPDATE_INFO_ADDR + offset, buffer, len);
}
static int hal_fota_read_update_info(atiny_fota_storage_device_s *this, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    if((offset + len) > UPDATE_INFO_SIZE)
    {
        HAL_FOTA_LOG("err offset %lu, len %lu", offset, len);
        return ERR;
    }
    return hal_spi_flash_read(buffer, len, UPDATE_INFO_ADDR + offset);
}


static uint32_t hal_fota_get_block_size(struct fota_hardware_api_tag_s *this, uint32_t offset)
{
    return FLASH_BLOCK_SIZE;
}
static uint32_t hal_fota_get_max_size(struct fota_hardware_api_tag_s *this)
{
    return OTA_IMAGE_DOWNLOAD_SIZE;
}

static int hal_fota_read_software(struct fota_hardware_api_tag_s *this, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    if((offset + len) > OTA_IMAGE_DOWNLOAD_SIZE)
    {
        HAL_FOTA_LOG("err offset %lu, len %lu", offset, len);
        return ERR;
    }

    return hal_spi_flash_read(buffer, len, OTA_IMAGE_DOWNLOAD_ADDR + offset);
}



ota_hal_s g_hal_storage_device = {{hal_fota_write_software, hal_fota_write_software_end, hal_fota_active_software,
                                   hal_fota_get_software_result, hal_fota_write_update_info, hal_fota_read_update_info},
                                  {hal_fota_get_block_size, hal_fota_get_max_size, hal_fota_read_software},
                                  0};

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

    hal_spi_flash_config();
    assist.func_printf = printf;
    assist.func_ota_read = hal_spi_flash_read;
    assist.func_ota_write = hal_spi_flash_erase_write;
    ota_register_assist(&assist);

    ret = ota_init();
    if (ret != OK)
    {
        OTA_LOG("read/write boot information failed");
    }

    return ret;

}


