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
#include "test_spiffs.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "fs/sys/fcntl.h"
#include "fs/los_vfs.h"
#include "fs/los_spiffs.h"


#define SPIFFS_PHYS_SIZE    1024 * 1024
#define PHYS_ERASE_SIZE     64 * 1024
#define LOG_BLOCK_SIZE      64 * 1024
#define LOG_PAGE_SIZE       256

static s32_t stm32f4xx_spiffs_read (struct spiffs_t *fs, u32_t addr, u32_t size, u8_t *buff)
{
    return SPIFFS_OK;
}

static s32_t stm32f4xx_spiffs_write (struct spiffs_t *fs, u32_t addr, u32_t size, u8_t *buff)
{
    return SPIFFS_OK;
}

static s32_t stm32f4xx_spiffs_erase (struct spiffs_t *fs, u32_t addr, u32_t size)
{
    return SPIFFS_OK;
}

}


/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestSpiffs::TestSpiffs()
{
    TEST_ADD(TestSpiffs::test_errno);
    TEST_ADD(TestSpiffs::test_exception);
}

TestSpiffs::~TestSpiffs()
{
}


void TestSpiffs::test_errno(void)
{
    extern int g_spiffs_errno;

    for (int i = SPIFFS_ERR_NOT_MOUNTED; i >= SPIFFS_ERR_SEEK_BOUNDS; i--)
    {
        g_spiffs_errno = i;
        unlink("/spiffs/f.txt");
    }

    g_spiffs_errno = SPIFFS_OK;
}

void TestSpiffs::test_exception(void)
{
    extern s32_t spiffs_mount_ret;
    int ret;

    spiffs_mount_ret = SPIFFS_ERR_INTERNAL;
    ret = spiffs_mount ("/spiffs/", 0, SPIFFS_PHYS_SIZE, PHYS_ERASE_SIZE,
            LOG_BLOCK_SIZE, LOG_PAGE_SIZE, stm32f4xx_spiffs_read,
            stm32f4xx_spiffs_write, stm32f4xx_spiffs_erase);
    TEST_ASSERT(ret < 0);
    spiffs_mount_ret = SPIFFS_OK;
}


/* Private functions --------------------------------------------------------*/
void TestSpiffs::setup()
{
    spiffs_init();

    if(spiffs_mount ("/spiffs/", 0, SPIFFS_PHYS_SIZE, PHYS_ERASE_SIZE,
            LOG_BLOCK_SIZE, LOG_PAGE_SIZE, stm32f4xx_spiffs_read,
            stm32f4xx_spiffs_write, stm32f4xx_spiffs_erase) != LOS_OK)
    {
        FS_PRINTF ("failed to mount spiffs!\n");
    }

    snprintf(file_name, sizeof(file_name), "%s/%s", SPIFFS_PATH, LOS_FILE);
    snprintf(file_rename, sizeof(file_rename), "%s/%s", SPIFFS_PATH, LOS_FILE_RN);
    snprintf(dir_name, sizeof(dir_name), "%s/%s", SPIFFS_PATH, LOS_DIR);
    snprintf(path_name, sizeof(path_name), "%s/%s", dir_name, LOS_FILE);
    fs_type = TEST_FS_SPIFFS;
}

void TestSpiffs::tear_down()
{
    struct file_system fs = {0};
    spiffs_unmount("/spiffs/");
    los_fs_unregister(&fs);
}
