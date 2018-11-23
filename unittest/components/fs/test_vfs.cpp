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
#include "test_vfs.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "fs/sys/fcntl.h"
#include "fs/los_vfs.h"
}

/* Global variables ---------------------------------------------------------*/
char file_name[MAX_NAME_LEN];
char file_rename[MAX_NAME_LEN];
char dir_name[MAX_NAME_LEN];
char path_name[MAX_NAME_LEN];

int fs_type;

/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestVfs::TestVfs()
{
    TEST_ADD(TestVfs::test_file_open);
    TEST_ADD(TestVfs::test_file_read);
    TEST_ADD(TestVfs::test_file_write);
    TEST_ADD(TestVfs::test_file_seek);
    TEST_ADD(TestVfs::test_file_sync);
    TEST_ADD(TestVfs::test_file_stat);
    TEST_ADD(TestVfs::test_file_rename);
    TEST_ADD(TestVfs::test_file_unlink);
    TEST_ADD(TestVfs::test_file_ioctl);
    TEST_ADD(TestVfs::test_dir_make);
    TEST_ADD(TestVfs::test_dir_read);
    TEST_ADD(TestVfs::test_vfs_exception);
    TEST_ADD(TestVfs::test_vfs_nullfs);
}

TestVfs::~TestVfs()
{
}

void TestVfs::test_setup(void)
{
}

void TestVfs::test_tear_down(void)
{
}

void TestVfs::test_file_open(void)
{
    int fd;
    int ret;

    fd = open(NULL, O_CREAT);
    TEST_ASSERT(fd < 0);

    fd = open("/test/", O_CREAT);
    TEST_ASSERT(fd < 0);

    fd = open(file_name, O_CREAT | O_WRONLY | O_RDWR | O_EXCL);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_read(void)
{
    int fd = -1;
    int ret = -1;
    char buf[16] = {0};

    ret = read(fd, 0, 0);
    TEST_ASSERT(ret < 0);

    ret = read(LOS_MAX_FILES, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_RDONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret >= 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);

    fd = open(file_name, O_WRONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_write(void)
{
    int fd;
    int ret;
    char buf[16] = {0};

    ret = write(fd, 0, 0);
    TEST_ASSERT(ret < 0);

    ret = write(LOS_MAX_FILES, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_WRONLY | O_APPEND | O_EXCL);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret >= 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);

    fd = open(file_name, O_RDONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_seek(void)
{
    int fd;
    int ret;

    lseek(LOS_MAX_FILES, 0, 0);

    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = lseek(fd, 0, 0);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = lseek(fd, 0, 1);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = lseek(fd, 0, 2);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = lseek(fd, 0, 3);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_sync(void)
{
    int fd;
    int ret;

    ret = fsync(LOS_MAX_FILES);
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = fsync(fd);
    TEST_ASSERT(ret >= 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_stat(void)
{
    int fd;
    int ret;
    struct stat s = {0};

    ret = stat(NULL, NULL);
    TEST_ASSERT(ret < 0);

    ret = stat("error", &s);
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = stat(file_name, &s);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_rename(void)
{
    int ret;

    ret = rename(NULL, NULL);
    TEST_ASSERT(ret < 0);

    ret = rename("old", file_rename);
    TEST_ASSERT(ret < 0);

    ret = rename(file_name, "new");
    TEST_ASSERT(ret < 0);

    ret = rename(file_name, file_rename);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_unlink(void)
{
    int ret;

    ret = unlink(NULL);
    TEST_ASSERT(ret < 0);

    ret = unlink("error");
    TEST_ASSERT(ret < 0);

    ret = unlink(file_name);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_ioctl(void)
{
    int fd;
    int ret;

    ret = ioctl(LOS_MAX_FILES, 0);
    TEST_ASSERT(ret < 0);

    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = ioctl(fd, 0);
    TEST_ASSERT(ret < 0);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_dir_make(void)
{
    int ret;

    if (fs_type != TEST_FS_SPIFFS)
    {
        ret = mkdir(NULL, 0);
        TEST_ASSERT(ret < 0);

        ret = mkdir("error", 0);
        TEST_ASSERT(ret < 0);

        ret = mkdir(dir_name, 0);
        TEST_ASSERT_EQUALS(ret, 0);
    }
}

void TestVfs::test_dir_read(void)
{
    int ret;
    struct dir *dir;
    struct dirent *dirent;

    dir = opendir(NULL);
    TEST_ASSERT(dir == NULL);

    dir = opendir("error");
    TEST_ASSERT(dir == NULL);

    dir = opendir(dir_name);
    TEST_ASSERT(dir != NULL);

    dirent = readdir(NULL);
    TEST_ASSERT(dirent == NULL);

    dirent = readdir(dir);
    TEST_ASSERT(dirent != NULL);

    ret = closedir(dir);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = closedir(NULL);
    TEST_ASSERT(ret < 0);
}

void TestVfs::test_vfs_exception(void)
{
    extern UINT32 g_mux_pend_ret;
    int fd;
    int ret;

    ret = close(LOS_MAX_FILES);
    TEST_ASSERT(ret < 0);

    ret = los_fs_register(NULL);
    TEST_ASSERT_EQUALS(ret, LOS_NOK);

    ret = los_fs_unregister(NULL);
    TEST_ASSERT_EQUALS(ret, LOS_NOK);

    ret = los_fs_mount(NULL, NULL, NULL);
    TEST_ASSERT_EQUALS(ret, LOS_NOK);

    ret = los_fs_unmount(NULL);
    TEST_ASSERT_EQUALS(ret, LOS_NOK);

    if (fs_type == TEST_FS_FATFS)
    {
        struct dir *dir;
        struct dirent *dirent;

        g_mux_pend_ret = LOS_NOK;
        fd = open(file_name, O_CREAT);
        TEST_ASSERT(fd < 0);

        dir = opendir(dir_name);
        TEST_ASSERT(dir == NULL);

        g_mux_pend_ret = LOS_OK;
        dir = opendir(dir_name);
        TEST_ASSERT(dir != NULL);

        g_mux_pend_ret = LOS_NOK;
        dirent = readdir(dir);
        TEST_ASSERT(dirent == NULL);

        g_mux_pend_ret = LOS_NOK;
        ret = closedir(dir);
        TEST_ASSERT(ret < 0);

        g_mux_pend_ret = LOS_OK;
        ret = closedir(dir);
        TEST_ASSERT_EQUALS(ret, 0);

        g_mux_pend_ret = LOS_NOK;
        ret = mkdir(dir_name, 0);
        TEST_ASSERT(ret < 0);

        g_mux_pend_ret = LOS_OK;
    }
}

static int nullfs_open(struct file *, const char *, int)
{
    return 0;
}

static int nullfs_close(struct file *)
{
    return 0;
}

static int nullfs_unlink(struct mount_point *, const char *)
{
    return 0;
}

static int nullfs_ioctl(struct file *, int, unsigned long)
{
    return 0;
}

static struct file_ops nullfs_ops =
{
    nullfs_open,
    nullfs_close,
    NULL,
    NULL,
    NULL,
    NULL,
    nullfs_unlink,
    NULL,
    nullfs_ioctl,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static struct file_system nullfs_fs =
{
    "nullfs",
    &nullfs_ops,
    NULL,
    0
};

static struct file_system nullfs2_fs =
{
    "nullfs_-+",
    &nullfs_ops,
    NULL,
    0
};

void TestVfs::test_vfs_nullfs(void)
{
    extern UINT32 g_mux_create_ret;
    extern UINT32 g_mux_pend_ret;
    int fd;
    int ret;
    char buf[16] = {0};
    struct stat s = {0};
    struct dir *dir;
    struct dirent *dirent;

    los_vfs_init();

    ret = los_fs_register (&nullfs_fs);
    TEST_ASSERT(ret == 0);

    ret = los_fs_register (&nullfs_fs);
    TEST_ASSERT(ret != 0);

    ret = los_fs_register (&nullfs2_fs);
    TEST_ASSERT(ret != 0);

    ret = los_fs_mount ("error", "/nullfs/", NULL);
    TEST_ASSERT(ret != 0);

    ret = los_fs_mount ("nullfs", "/nullfs/", NULL);
    TEST_ASSERT(ret == 0);

    ret = los_fs_mount ("nullfs", "/nullfs/", NULL);
    TEST_ASSERT(ret != 0);

    g_mux_create_ret = LOS_NOK;
    ret = los_fs_mount ("nullfs", "/nullfs/again/", NULL);
    TEST_ASSERT(ret != 0);
    g_mux_create_ret = LOS_OK;

    ret = los_fs_mount ("nullfs", "/nullfs/again/", NULL);
    TEST_ASSERT(ret == 0);

    fd = open("/nullfs/f.txt", O_CREAT);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = lseek(fd, 0, 0);
    TEST_ASSERT(ret == 0);

    ret = stat("/nullfs/f.txt", &s);
    TEST_ASSERT(ret < 0);

    ret = ioctl(fd, 0);
    TEST_ASSERT(ret == 0);

    ret = fsync(fd);
    TEST_ASSERT(ret < 0);

    ret = rename("/nullfs/f.txt", "/nullfs/f2.txt");
    TEST_ASSERT(ret < 0);

    ret = rename(file_name, "/nullfs/f.txt");
    TEST_ASSERT(ret < 0);

    ret = mkdir("/nullfs/d", 0);
    TEST_ASSERT(ret < 0);

    dir = opendir("/nullfs/d");
    TEST_ASSERT(dir == NULL);

    ret = close(fd);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_fs_unmount("/nullfs/");
    TEST_ASSERT(ret == 0);

    ret = los_fs_unmount("/nullfs/again/");
    TEST_ASSERT(ret == 0);

    ret = los_fs_unregister(&nullfs_fs);
    TEST_ASSERT(ret == 0);

    g_mux_pend_ret = LOS_NOK;
    ret = los_fs_unregister(&nullfs_fs);
    TEST_ASSERT(ret != 0);
    g_mux_pend_ret = LOS_OK;
}
