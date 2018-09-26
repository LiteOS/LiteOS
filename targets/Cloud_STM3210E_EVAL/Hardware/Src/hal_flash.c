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

#include <string.h>
#include "hal_flash.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_flash.h"

#ifdef HAL_FLASH_MODULE_ENABLED

#define FLASH_SECTOR_ILEGAL      0xFFFFFFFF
#define ADDR_FLASH_SECTOR_0      ((uint32_t)0x08000000) /* Base address of Sector 0, 2 Kbytes   */
#define ADDR_FLASH_SECTOR_END    ((uint32_t)0x08080000) /* End address of Sector 256 */

extern void    FLASH_PageErase(uint32_t PageAddress);

static uint32_t prv_flash_get_sector(uint32_t addr)
{
    uint32_t sector = 0;

    if ((addr < ADDR_FLASH_SECTOR_END) && (addr >= ADDR_FLASH_SECTOR_0))
    {
        sector = ~(FLASH_PAGE_SIZE - 1) & (addr - ADDR_FLASH_SECTOR_0);
    }
    else
    {
        sector = FLASH_SECTOR_ILEGAL;
    }

    return sector;
}

int hal_flash_read(void* buf, int32_t len, uint32_t location)
{
    if (NULL == buf
        || len < 0
        || len >= ADDR_FLASH_SECTOR_END - ADDR_FLASH_SECTOR_0)
    {
        return -1;
    }

    if (FLASH_SECTOR_ILEGAL != prv_flash_get_sector(location)
        && FLASH_SECTOR_ILEGAL != prv_flash_get_sector(location + len))
    {
        memcpy(buf, (uint8_t*)location, len);
        return 0;
    }
    else
    {
        return -1;
    }

}

int hal_flash_erase(uint32_t addr, int32_t len)
{
    uint32_t begin_sector;
    uint32_t end_sector;
    uint32_t i;

    if (len < 0 || len >= ADDR_FLASH_SECTOR_END - ADDR_FLASH_SECTOR_0)
    {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        return -1;
    }

    begin_sector = prv_flash_get_sector(addr);
    end_sector = prv_flash_get_sector(addr + len);

    if (FLASH_SECTOR_ILEGAL == begin_sector
        || FLASH_SECTOR_ILEGAL == end_sector)
    {
        return -1;
    }

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

    for (i = begin_sector; i <= end_sector; ++i)
    {
        FLASH_PageErase(i * FLASH_PAGE_SIZE + ADDR_FLASH_SECTOR_0);
    }

    return 0;
}

int hal_flash_write(const void* buf, int32_t len, uint32_t* location)
{
    int i;
    uint8_t* pbuf;
    uint32_t location_cur;

    if (NULL == buf
        || NULL == location
        || len < 0
        || len >= ADDR_FLASH_SECTOR_END - ADDR_FLASH_SECTOR_0)
    {
        return -1;
    }

    location_cur = *location;
    pbuf = (uint8_t*)buf;

    if (FLASH_SECTOR_ILEGAL == prv_flash_get_sector(location_cur)
        || FLASH_SECTOR_ILEGAL == prv_flash_get_sector(location_cur + len))
    {
        return -1;
    }

    len /= 2;
    for (i = 0; i < len; ++i)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
                            location_cur, *(uint16_t *)(pbuf + i)) == HAL_OK)
        {
            location_cur += 2;
        }
        else
        {
            return -1;
        }
    }
    *location += len;

    return 0;
}

int hal_flash_erase_write(const void* buf, int32_t len, uint32_t location)
{
    if (NULL == buf)
    {
        return -1;
    }

    if (hal_flash_erase(location, len) != 0)
    {
        (void)HAL_FLASH_Lock();
        return -1;
    }

    if (hal_flash_write(buf, len, &location) != 0)
    {
        (void)HAL_FLASH_Lock();
        return -1;
    }

    return 0;
}

void hal_flash_lock(void)
{
    (void)HAL_FLASH_Lock();
}

#endif /* HAL_FLASH_MODULE_ENABLED */