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
#include "test_fatfs.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "fs/sys/fcntl.h"
#include "fs/los_vfs.h"
#include "fs/los_fatfs.h"


static DSTATUS stm32f4xx_fatfs_status(BYTE lun)
{
    return 0;
}

static DSTATUS stm32f4xx_fatfs_initialize(BYTE lun)
{
    return 0;
}

static DRESULT stm32f4xx_fatfs_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    return RES_OK;
}

static DRESULT stm32f4xx_fatfs_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    return RES_OK;
}

static DRESULT stm32f4xx_fatfs_ioctl(BYTE lun, BYTE cmd, void *buff)
{
    return RES_OK;
}

static struct diskio_drv spi_drv =
{
    stm32f4xx_fatfs_initialize,
    stm32f4xx_fatfs_status,
    stm32f4xx_fatfs_read,
    stm32f4xx_fatfs_write,
    stm32f4xx_fatfs_ioctl
};

}


/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestFatfs::TestFatfs()
{
    TEST_ADD(TestFatfs::test_errno);
    TEST_ADD(TestFatfs::test_exception);
}

TestFatfs::~TestFatfs()
{
}


void TestFatfs::test_errno(void)
{
    extern int g_fatfs_errno;

    for (int i = FR_OK; i <= FR_INVALID_PARAMETER + 1; i++)
    {
        g_fatfs_errno = i;
        unlink("/fatfs/f.txt");
    }

    g_fatfs_errno = FR_OK;
}

void TestFatfs::test_exception(void)
{
    extern FRESULT f_read_ret;
    int fd;
    int ret;
    uint8_t drive = -1;
    char buf[16] = {0};

    ret = fatfs_mount("/fatfs/", &spi_drv, (uint8_t *)&drive);
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_RDONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    f_read_ret = FR_INVALID_PARAMETER;
    ret = read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);
    f_read_ret = FR_OK;

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}


/* Private functions --------------------------------------------------------*/
void TestFatfs::setup()
{
    int ret;
    uint8_t drive = -1;

    static int is_first = 1;
    if (is_first)
    {
        extern UINT32 g_mux_pend_ret;

        is_first = 0;
        g_mux_pend_ret = LOS_NOK;
        ret = fatfs_init();
        if (ret == 0)
        {
            FS_PRINTF ("fatfs_init success\n");
        }
        g_mux_pend_ret = LOS_OK;
    }

    fatfs_init();

    if(fatfs_mount("/fatfs/", &spi_drv, (uint8_t *)&drive) < 0)
    {
        FS_PRINTF ("failed to mount fatfs!\n");
    }

    if(drive < 0)
    {
        FS_PRINTF("stm32f4xx_fatfs_init failed, drive: %d", drive);
    }

    snprintf(file_name, sizeof(file_name), "%s/%d:/%s", FATFS_PATH, drive, LOS_FILE);
    snprintf(file_rename, sizeof(file_rename), "%s/%d:/%s", FATFS_PATH, drive, LOS_FILE_RN);
    snprintf(dir_name, sizeof(dir_name), "%s/%d:/%s", FATFS_PATH, drive, LOS_DIR);
    snprintf(path_name, sizeof(path_name), "%s/%s", dir_name, LOS_FILE);
    fs_type = TEST_FS_FATFS;
}

void TestFatfs::tear_down()
{
    int ret;
    uint8_t drive = -1;

    struct file_system fs = {0};
    fatfs_unmount("/fatfs/", 0);
    los_fs_unregister(&fs);

    static int is_first = 1;
    if (is_first)
    {
        extern FRESULT f_mount_ret;

        is_first = 0;
        f_mount_ret = FR_INVALID_PARAMETER;
        if(fatfs_mount("/fatfs/", &spi_drv, (uint8_t *)&drive) == 0)
        {
            FS_PRINTF ("fatfs_mount success\n");
        }
        f_mount_ret = FR_OK;
    }
}
