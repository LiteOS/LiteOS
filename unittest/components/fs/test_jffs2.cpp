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
#include "test_jffs2.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "fs/sys/fcntl.h"
#include "fs/los_vfs.h"
#include "fs/los_jffs2.h"


int jffs2_unmount(const char *path);

#define SPI_FLASH_TOTAL_SIZE    (1024 * 1024)
#define SPI_FLASH_SECTOR_SIZE   (4 * 1024)

static int stm32f4xx_jffs2_read(struct mtd_info *mtd, loff_t from, size_t len,
                                        size_t *retlen, u_char *buf)
{
    return 0;
}

static int stm32f4xx_jffs2_write(struct mtd_info *mtd, loff_t to, size_t len,
                                        size_t *retlen, const u_char *buf)
{
    return 0;
}

static int stm32f4xx_jffs2_erase(struct mtd_info *mtd, loff_t from, size_t len)
{
    return 0;
}

static struct mtd_info mtd_spi_flash =
{
    MTD_TYPE_SPI_FLASH, //.type
    0,
    SPI_FLASH_TOTAL_SIZE, //.size
    SPI_FLASH_SECTOR_SIZE, //.erasesize
    0,

    stm32f4xx_jffs2_read, //.read
    stm32f4xx_jffs2_write, //.write
    stm32f4xx_jffs2_erase //.erase
};

}


/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestJffs2::TestJffs2()
{
    TEST_ADD(TestJffs2::test_errno);
    TEST_ADD(TestJffs2::test_exception);
}

TestJffs2::~TestJffs2()
{
}


void TestJffs2::test_errno(void)
{
}

void TestJffs2::test_exception(void)
{
    extern int jffs2_mount_ret;
    extern int jffs2_stat_flag;
    extern int jffs2_opendir_ret;
    extern int jffs2_open_ret;
    extern int jffs2_close_ret;
    extern int jffs2_rw_ret;
    int ret;
    int fd;
    struct dir *dir;
    struct dirent *dirent;
    char buf[] = "Hello World";

    ret = jffs2_mount(NULL, NULL);
    TEST_ASSERT(ret < 0);

    jffs2_mount_ret = EIO;
    ret = jffs2_mount("/jffs2/", &mtd_spi_flash);
    TEST_ASSERT(ret != 0);
    jffs2_mount_ret = ENOERR;

    jffs2_open_ret = EIO;
    fd = open(file_name, O_RDWR);
    TEST_ASSERT(fd < 0);
    jffs2_open_ret = ENOERR;

    fd = open(file_name, O_RDWR);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    jffs2_rw_ret = EIO;
    ret = read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);
    jffs2_rw_ret = ENOERR;

//    jffs2_close_ret = EIO;
//    ret = close(fd);
//    TEST_ASSERT(ret != 0);
//    jffs2_close_ret = ENOERR;

    ret = close(fd);
    TEST_ASSERT(ret == 0);

    jffs2_stat_flag = 1;
    ret = unlink(file_name);
    TEST_ASSERT(ret == 0);
    jffs2_stat_flag = 2;
    ret = unlink(file_name);
    TEST_ASSERT(ret < 0);
    jffs2_stat_flag = -1;
    ret = unlink(file_name);
    TEST_ASSERT(ret < 0);
    jffs2_stat_flag = 0;

    jffs2_opendir_ret = EIO;
    dir = opendir(dir_name);
    TEST_ASSERT(dir == NULL);
    jffs2_opendir_ret = ENOERR;

    dir = opendir(dir_name);
    TEST_ASSERT(dir != NULL);

    jffs2_stat_flag = 1;
    dirent = readdir(dir);
    TEST_ASSERT(dirent != NULL);
    jffs2_stat_flag = -1;
    dirent = readdir(dir);
    TEST_ASSERT(dirent == NULL);
    jffs2_stat_flag = 0;

    jffs2_close_ret = EIO;
    ret = closedir(dir);
    TEST_ASSERT(ret != 0);
    jffs2_close_ret = ENOERR;

    ret = closedir(dir);
    TEST_ASSERT(ret == 0);
}


/* Private functions --------------------------------------------------------*/
void TestJffs2::setup()
{
    static int is_first = 1;
    if (is_first)
    {
        extern UINT32 g_mux_pend_ret;
        int ret;

        is_first = 0;
        g_mux_pend_ret = LOS_NOK;
        ret = jffs2_init();
        if (ret == 0)
        {
            FS_PRINTF ("jffs2_init success\n");
        }
        g_mux_pend_ret = LOS_OK;
    }

    jffs2_init();

    if(jffs2_mount("/jffs2/", &mtd_spi_flash) < 0)
    {
        FS_PRINTF ("failed to mount jffs2!\n");
    }

    snprintf(file_name, sizeof(file_name), "%s/%s", JFFS2_PATH, LOS_FILE);
    snprintf(file_rename, sizeof(file_rename), "%s/%s", JFFS2_PATH, LOS_FILE_RN);
    snprintf(dir_name, sizeof(dir_name), "%s/%s", JFFS2_PATH, LOS_DIR);
    snprintf(path_name, sizeof(path_name), "%s/%s", dir_name, LOS_FILE);
    fs_type = TEST_FS_JFFS2;
}

void TestJffs2::tear_down()
{
    struct file_system fs = {0};
    jffs2_unmount("/jffs2/");
    los_fs_unregister(&fs);
}
