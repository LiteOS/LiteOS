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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#if defined (__GNUC__) || defined (__CC_ARM)
#include "fs/sys/fcntl.h"
#include <los_printf.h>
#endif

#include "fs/los_vfs.h"
#include "fs/los_jffs2.h"

#include <hal_spi_flash.h>

/* Defines ------------------------------------------------------------------*/
#define SPI_FLASH_ID            0xEF4018

#define SPI_FLASH_TOTAL_SIZE    (1024 * 1024)
#define SPI_FLASH_SECTOR_SIZE   (4 * 1024)
#define SPI_FLASH_PAGE_SIZE     256

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
static int stm32f4xx_jffs2_read(struct mtd_info *mtd, loff_t from, size_t len,
                                        size_t *retlen, u_char *buf)
{
    int ret = hal_spi_flash_read((void *)buf, len, from);
    if(ret < 0)
    {
        *retlen = 0;
        return ret;
    }
    *retlen = len;
    return 0;
}

static int stm32f4xx_jffs2_write(struct mtd_info *mtd, loff_t to, size_t len,
                                        size_t *retlen, const u_char *buf)
{
    int ret = hal_spi_flash_write((void *)buf, len, (uint32_t *)&to);
    if(ret < 0)
    {
        *retlen = 0;
        return ret;
    }
    *retlen = len;
    return 0;
}

static int stm32f4xx_jffs2_erase(struct mtd_info *mtd, loff_t from, size_t len)
{
    return hal_spi_flash_erase(from, len);
}


static struct mtd_info mtd_spi_flash =
{
    .type = MTD_TYPE_SPI_FLASH,
    .size = SPI_FLASH_TOTAL_SIZE,
    .erasesize = SPI_FLASH_SECTOR_SIZE,

    .read = stm32f4xx_jffs2_read,
    .write = stm32f4xx_jffs2_write,
    .erase = stm32f4xx_jffs2_erase
};


int stm32f4xx_jffs2_init(int need_erase)
{
    int ret;

    (void)hal_spi_flash_config();
    if (need_erase)
    {
        (void)hal_spi_flash_erase(0, SPI_FLASH_TOTAL_SIZE);
    }

    (void)jffs2_init();

    ret = jffs2_mount("/jffs2/", &mtd_spi_flash);
    if(ret < 0)
    {
        PRINT_ERR ("failed to mount jffs2!\n");
    }

    return ret;
}

/* Private functions --------------------------------------------------------*/
