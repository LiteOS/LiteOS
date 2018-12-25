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
#include <malloc.h>
#include "test_vfs.h"
#include "stub.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "fs/sys/fcntl.h"
#include "fs/los_vfs.h"

int fatfs_init(void);
int spiffs_init(void);
struct mount_point *los_mp_find (const char *path, const char **path_in_mp);
}

/* Global variables ---------------------------------------------------------*/
char file_name[MAX_NAME_LEN];
char file_rename[MAX_NAME_LEN];
char dir_name[MAX_NAME_LEN];
char path_name[MAX_NAME_LEN];

int fs_type;

/* Private function prototypes ----------------------------------------------*/
static void* stub_malloc(size_t size)
{
    return NULL;
}


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

    fd = los_open(NULL, O_CREAT);
    TEST_ASSERT(fd < 0);

    fd = los_open("/test/", O_CREAT);
    TEST_ASSERT(fd < 0);

    fd = los_open(file_name, O_CREAT | O_WRONLY | O_RDWR | O_EXCL);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_read(void)
{
    int fd = -1;
    int ret = -1;
    char buf[16] = {0};

    ret = los_read(fd, 0, 0);
    TEST_ASSERT(ret < 0);

    ret = los_read(LOS_MAX_FILES, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    fd = los_open(file_name, O_RDONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret >= 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);

    fd = los_open(file_name, O_WRONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_write(void)
{
    int fd;
    int ret;
    char buf[16] = {0};

    ret = los_write(fd, 0, 0);
    TEST_ASSERT(ret < 0);

    ret = los_write(LOS_MAX_FILES, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    fd = los_open(file_name, O_WRONLY | O_APPEND | O_EXCL);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret >= 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);

    fd = los_open(file_name, O_RDONLY);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_seek(void)
{
    int fd;
    int ret;

    los_lseek(LOS_MAX_FILES, 0, 0);

    fd = los_open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_lseek(fd, 0, 0);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_lseek(fd, 0, 1);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_lseek(fd, 0, 2);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_lseek(fd, 0, 3);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_sync(void)
{
    int fd;
    int ret;

    ret = los_sync(LOS_MAX_FILES);
    TEST_ASSERT(ret < 0);

    fd = los_open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_sync(fd);
    TEST_ASSERT(ret >= 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_stat(void)
{
    int fd;
    int ret;
    struct stat s = {0};

    ret = los_stat(NULL, NULL);
    TEST_ASSERT(ret < 0);

    ret = los_stat("error", &s);
    TEST_ASSERT(ret < 0);

    fd = los_open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_stat(file_name, &s);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_rename(void)
{
    int ret;

    ret = los_rename(NULL, NULL);
    TEST_ASSERT(ret < 0);

    ret = los_rename("old", file_rename);
    TEST_ASSERT(ret < 0);

    ret = los_rename(file_name, "new");
    TEST_ASSERT(ret < 0);

    ret = los_rename(file_name, file_rename);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_unlink(void)
{
    int ret;

    ret = los_unlink(NULL);
    TEST_ASSERT(ret < 0);

    ret = los_unlink("error");
    TEST_ASSERT(ret < 0);

    ret = los_unlink(file_name);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_file_ioctl(void)
{
    int fd;
    int ret;

    ret = los_ioctl(LOS_MAX_FILES, 0);
    TEST_ASSERT(ret < 0);

    fd = los_open(file_name, O_CREAT | O_RDWR | O_TRUNC);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_ioctl(fd, 0);
    TEST_ASSERT(ret < 0);

    ret = los_close(fd);
    TEST_ASSERT_EQUALS(ret, 0);
}

void TestVfs::test_dir_make(void)
{
    int ret;

    if (fs_type != TEST_FS_SPIFFS)
    {
        ret = los_mkdir(NULL, 0);
        TEST_ASSERT(ret < 0);

        ret = los_mkdir("error", 0);
        TEST_ASSERT(ret < 0);

        ret = los_mkdir(dir_name, 0);
        TEST_ASSERT_EQUALS(ret, 0);
    }
}

void TestVfs::test_dir_read(void)
{
    int ret;
    struct dir *dir;
    struct dirent *dirent;

    dir = los_opendir(NULL);
    TEST_ASSERT(dir == NULL);

    dir = los_opendir("error");
    TEST_ASSERT(dir == NULL);

    dir = los_opendir(dir_name);
    TEST_ASSERT(dir != NULL);

    dirent = los_readdir(NULL);
    TEST_ASSERT(dirent == NULL);

    dirent = los_readdir(dir);
    TEST_ASSERT(dirent != NULL);

    ret = los_closedir(dir);
    TEST_ASSERT_EQUALS(ret, 0);

    ret = los_closedir(NULL);
    TEST_ASSERT(ret < 0);
}

void TestVfs::test_vfs_exception(void)
{
    extern UINT32 g_mux_pend_ret;
    int fd;
    int ret;

    struct mount_point *mp = los_mp_find(NULL, NULL);
    TEST_ASSERT(mp == NULL);

    fd = los_open("/xxx/xxx", O_CREAT);
    TEST_ASSERT(fd < 0);

    ret = los_close(LOS_MAX_FILES);
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
        struct stat s = {0};

        g_mux_pend_ret = LOS_NOK;
        ret = los_stat(file_name, &s);
        TEST_ASSERT(ret < 0);

        fd = los_open(file_name, O_CREAT);
        TEST_ASSERT(fd < 0);

        dir = los_opendir(dir_name);
        TEST_ASSERT(dir == NULL);

        g_mux_pend_ret = LOS_OK;
        dir = los_opendir(dir_name);
        TEST_ASSERT(dir != NULL);

        g_mux_pend_ret = LOS_NOK;
        dirent = los_readdir(dir);
        TEST_ASSERT(dirent == NULL);

        g_mux_pend_ret = LOS_NOK;
        ret = los_closedir(dir);
        TEST_ASSERT(ret < 0);

        g_mux_pend_ret = LOS_OK;
        ret = los_closedir(dir);
        TEST_ASSERT_EQUALS(ret, 0);

        g_mux_pend_ret = LOS_NOK;
        ret = los_mkdir(dir_name, 0);
        TEST_ASSERT(ret < 0);

        g_mux_pend_ret = -1;
        ret = los_mkdir(dir_name, 0);
        TEST_ASSERT(ret < 0);

        g_mux_pend_ret = -1;
        dir = los_opendir(dir_name);
        TEST_ASSERT(dir == NULL);

        g_mux_pend_ret = -1;
        fd = los_open(file_name, O_CREAT);
        TEST_ASSERT(fd < 0);

        g_mux_pend_ret = LOS_OK;

        stubInfo si;
        setStub((void *)malloc, (void *)stub_malloc, &si);

        dir = los_opendir("xxx");
        TEST_ASSERT(dir == NULL);

        ret = los_fs_mount("fatfs", "/fatxx/", NULL);
        TEST_ASSERT_EQUALS(ret, LOS_NOK);

        cleanStub(&si);
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

    fd = los_open("/nullfs/f.txt", O_CREAT);
    TEST_ASSERT(fd >= 0 && fd < LOS_MAX_FILES);

    ret = los_read(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = los_write(fd, buf, sizeof(buf));
    TEST_ASSERT(ret < 0);

    ret = los_lseek(fd, 0, 0);
    TEST_ASSERT(ret == 0);

    ret = los_stat("/nullfs/f.txt", &s);
    TEST_ASSERT(ret < 0);

    ret = los_ioctl(fd, 0);
    TEST_ASSERT(ret == 0);

    ret = los_sync(fd);
    TEST_ASSERT(ret < 0);

    ret = los_rename("/nullfs/f.txt", "/nullfs/f2.txt");
    TEST_ASSERT(ret < 0);

    ret = los_rename(file_name, "/nullfs/f.txt");
    TEST_ASSERT(ret < 0);

    ret = los_mkdir("/nullfs/d", 0);
    TEST_ASSERT(ret < 0);

    dir = los_opendir("/nullfs/d");
    TEST_ASSERT(dir == NULL);

    ret = los_close(fd);
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
