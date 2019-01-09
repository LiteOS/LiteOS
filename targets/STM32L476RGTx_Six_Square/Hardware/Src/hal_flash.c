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
#include "stm32l4xx.h"
#include "stm32l4xx_hal_flash.h"
#include "hal_flash.h"


#ifdef HAL_FLASH_MODULE_ENABLED

int hal_flash_unlock(void)
{
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();

    if (status != HAL_OK)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void hal_flash_lock(void)
{
    (void)HAL_FLASH_Lock();
}

/**
  * @brief  Get the page of the given address
  * @param  addr: Address of the FLASH Memory
  * @retval The page of the given address
  */
static uint32_t get_page(uint32_t addr)
{
    uint32_t page = 0;

    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        /* Bank 1 */
        page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
        /* Bank 2 */
        page = (addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    }

    return page;
}

/**
  * @brief  Get the bank of the given address
  * @param  addr: Address of the FLASH Memory
  * @retval The bank of the given address
  */
static uint32_t get_bank(uint32_t addr)
{
    uint32_t bank = 0;

    if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
    {
        /* No Bank swap */
        if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
        {
            bank = FLASH_BANK_1;
        }
        else
        {
            bank = FLASH_BANK_2;
        }
    }
    else
    {
        /* Bank swap */
        if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
        {
            bank = FLASH_BANK_2;
        }
        else
        {
            bank = FLASH_BANK_1;
        }
    }

    return bank;
}


int hal_flash_erase(uint32_t addr, int32_t len)
{
    static uint8_t page_buf[FLASH_PAGE_SIZE];

    FLASH_EraseInitTypeDef erase = {FLASH_TYPEERASE_PAGES};
    uint32_t page_err = 0;
    HAL_StatusTypeDef status = HAL_OK;
    int ret = 0;

    if (addr < FLASH_BASE || addr >= FLASH_BASE + FLASH_SIZE
        || len <= 0 || addr + len >= FLASH_BASE + FLASH_SIZE)
    {
        return -1;
    }

    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        return -1;
    }

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    uint32_t start_bank = get_bank(addr);
    uint32_t end_bank = get_bank(addr + len - 1);
    uint32_t start_page = get_page(addr);
    uint32_t end_page = get_page(addr + len - 1);

    uint32_t off = addr % FLASH_PAGE_SIZE;
    uint32_t loc = addr - off;
    uint32_t off_end = (addr + len) % FLASH_PAGE_SIZE;
    uint32_t loc_end = addr + len;
    int on_one_page = 0;

    if (start_bank == end_bank && start_page == end_page)
    {
        on_one_page = 1;
    }

    if (off != 0) // start address is not on page boundary
    {
        ret = hal_flash_read(page_buf, off, loc);
        if (ret != 0)
        {
            return -1;
        }

        if (on_one_page && off_end != 0)
        {
            ret = hal_flash_read(page_buf + off_end, FLASH_PAGE_SIZE - off_end, loc_end);
            if (ret != 0)
            {
                return -1;
            }
        }

        erase.Banks = start_bank;
        erase.Page = start_page;
        erase.NbPages = 1;
        status = HAL_FLASHEx_Erase(&erase, &page_err);
        if (status != HAL_OK)
        {
            return -1;
        }

        ret = hal_flash_write(page_buf, off, &loc);
        if (ret != 0)
        {
            return -1;
        }

        if (on_one_page)
        {
            if (off_end != 0)
            {
                ret = hal_flash_write(page_buf + off_end, FLASH_PAGE_SIZE - off_end, &loc_end);
            }
            return ret;
        }

        if (start_page == 255)
        {
            start_page = 0;
            start_bank = end_bank;
        }
        else
        {
            start_page++;
        }
    }

    if (off_end != 0) // end address is not on page boundary
    {
        ret = hal_flash_read(page_buf + off_end, FLASH_PAGE_SIZE - off_end, loc_end);
        if (ret != 0)
        {
            return -1;
        }
    }

    if (start_bank == end_bank)
    {
        erase.Banks = start_bank;
        erase.Page = start_page;
        erase.NbPages = end_page - start_page + 1;
        status = HAL_FLASHEx_Erase(&erase, &page_err);
    }
    else
    {
        erase.Banks = start_bank;
        erase.Page = start_page;
        erase.NbPages = 255 - start_page + 1;

        status = HAL_FLASHEx_Erase(&erase, &page_err);
        if (status != HAL_OK)
        {
            return -1;
        }

        erase.Banks = end_bank;
        erase.Page = 0;
        erase.NbPages = end_page + 1;
        status = HAL_FLASHEx_Erase(&erase, &page_err);
    }

    if (status != HAL_OK)
    {
        return -1;
    }

    if (off_end != 0)
    {
        ret = hal_flash_write(page_buf + off_end, FLASH_PAGE_SIZE - off_end, &loc_end);
    }

    return ret;
}

int hal_flash_write(const uint8_t *buf, int32_t len, uint32_t *location)
{
    uint32_t loc_ori = 0;
    int32_t i = 0;

    if (NULL == buf || len <= 0 || NULL == location
        || *location < FLASH_BASE
        || *location >= FLASH_BASE + FLASH_SIZE
        || *location + len >= FLASH_BASE + FLASH_SIZE)
    {
        return -1;
    }

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

    loc_ori = *location;

#if 0
    for (i = 0; i < len; ++i)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
                              location_cur + i, pbuf[i]) != HAL_OK)
        {
            return -1;
        }
    }
    *location += len;

#else
    for (i = 0; i < len; i += 8)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, loc_ori + i, *(uint64_t *)(buf + i)) == HAL_OK)
        {
            /* Check the written value */
            if (*(uint64_t *)(buf + i) != *(uint64_t *)(loc_ori + i))
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
#endif

    *location += len;

    return 0;
}

int hal_flash_erase_write(const uint8_t *buf, int32_t len, uint32_t location)
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

int hal_flash_read(uint8_t *buf, int32_t len, uint32_t location)
{
    if (NULL == buf || len <= 0
        || location < FLASH_BASE
        || location >= FLASH_BASE + FLASH_SIZE
        || location + len >= FLASH_BASE + FLASH_SIZE)
    {
        return -1;
    }

#if 0
    for (int32_t i = 0; i < len; i++)
    {
        buf[i] = *(__IO uint8_t *)(location + i);
    }
#endif

    memcpy(buf, (uint8_t *)location, len);

    return 0;
}

#endif /* HAL_FLASH_MODULE_ENABLED */

