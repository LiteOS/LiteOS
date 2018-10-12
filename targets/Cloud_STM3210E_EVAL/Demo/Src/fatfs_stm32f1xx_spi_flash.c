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

#ifdef __GNUC__
#include <sys/unistd.h>
#include <sys/stat.h>
#endif

#if defined (__GNUC__) || defined (__CC_ARM)
#include <sys/fcntl.h>
#include <los_printf.h>
#endif

#include <los_vfs.h>
#include <los_fatfs.h>

/*------------------------------
 * notice:
 * =================
 * M25P80 SPI FLASH : 
 *      The memory is organized as 16 sectors, each containing 256 pages. Each page is 256
 * bytes wide. Thus, the whole memory can be viewed as consisting of 4096 pages, or 1,048,576 bytes.
 *
 *      The whole memory can be erased using the Bulk Erase instruction, or a sector at a time,
 * using the Sector Erase instruction.
 *
 * So it doesn't fit fatfs ( M25P80: 1 sector = 64 kB )
 *------------------------------*/
#include "hal_spi_flash.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
static DSTATUS stm32f1xx_fatfs_status(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;
    
    if(SPI_FLASH_ID == hal_spi_flash_get_id())
    {
        stat &= ~STA_NOINIT;
    }
    return stat;
}

static DSTATUS stm32f1xx_fatfs_initialize(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    hal_spi_flash_config();
    hal_spi_flash_wake_up();
    stat = stm32f1xx_fatfs_status(lun);
    return stat;
}

static DRESULT stm32f1xx_fatfs_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    hal_spi_flash_read((void *)buff, count * FF_MAX_SS, sector * FF_MAX_SS);
    return RES_OK;
}

static DRESULT stm32f1xx_fatfs_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    hal_spi_flash_erase_write((void *)buff, count * FF_MAX_SS, sector * FF_MAX_SS);
    return RES_OK;
}

static DRESULT stm32f1xx_fatfs_ioctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;

    switch (cmd)
    {
    case GET_SECTOR_COUNT:
        *(DWORD *)buff = SPI_FLASH_TOTAL_SIZE/FF_MAX_SS;
        break;
    case GET_SECTOR_SIZE:
        *(DWORD *)buff = FF_MAX_SS;
        break;
    case GET_BLOCK_SIZE:
        *(DWORD *)buff = 1;
        break;
    }
    res = RES_OK;
    return res;
}

static struct diskio_drv spi_drv =
{
    stm32f1xx_fatfs_initialize,
    stm32f1xx_fatfs_status,
    stm32f1xx_fatfs_read,
    stm32f1xx_fatfs_write,
    stm32f1xx_fatfs_ioctl
};

int stm32f1xx_fatfs_init(void)
{
    int8_t drive = -1;
    
    (void)fatfs_init();

    if(fatfs_mount("/fatfs/", &spi_drv, (uint8_t *)&drive) < 0)
    {
        PRINT_ERR ("failed to mount fatfs!\n");
    }
    
    return drive;
}

DWORD get_fattime (void)
{
    return 0;
}

/* Private functions --------------------------------------------------------*/

