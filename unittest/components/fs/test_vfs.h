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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __TEST_VFS_H__
#define __TEST_VFS_H__

/* Includes -----------------------------------------------------------------*/
#include <cpptest.h>
#include "stub.h"

/* Defines ------------------------------------------------------------------*/
#ifndef FS_PRINTF
#define FS_PRINTF(fmt, arg...)  printf("[%s:%d]" fmt "\n", __func__, __LINE__, ##arg)
#endif

#define TEST_FS_SPIFFS      0
#define TEST_FS_FATFS       1
#define TEST_FS_JFFS2       2

#define SPIFFS_PATH         "/spiffs"
#define FATFS_PATH          "/fatfs"
#define JFFS2_PATH          "/jffs2"

#define LOS_FILE            "f.txt"
#define LOS_FILE_RN         "file.txt"
#define LOS_DIR             "d"

#define MAX_NAME_LEN 64

extern char file_name[MAX_NAME_LEN];
extern char file_rename[MAX_NAME_LEN];
extern char dir_name[MAX_NAME_LEN];
extern char path_name[MAX_NAME_LEN];

extern int fs_type;

/* Macros -------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Functions API ------------------------------------------------------------*/
class TestVfs : public Test::Suite
{
public:
    TestVfs();
    ~TestVfs();

    virtual void test_setup(void);
    virtual void test_tear_down(void);

    void test_file_open(void);
    void test_file_read(void);
    void test_file_write(void);
    void test_file_seek(void);
    void test_file_sync(void);
    void test_file_stat(void);
    void test_file_rename(void);
    void test_file_unlink(void);
    void test_file_ioctl(void);
    void test_dir_make(void);
    void test_dir_read(void);
    void test_vfs_exception(void);
    void test_vfs_nullfs(void);
};

#endif /* __TEST_VFS_H__ */
