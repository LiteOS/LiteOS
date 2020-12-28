/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Ram Fs Demo
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "fs/los_ramfs.h"
#include "fs_common.h"

#define RAMFS_PATH        "/ramfs"
#define RAMFS_SIEZ        (2 * 1024)
#define DEMO_BUF_LEN      128

void ramfs_demo(void)
{
    int ret;
    char bufWrite[] = "this is a demo write to file";
    char bufRead[DEMO_BUF_LEN];
    struct dir *testdir = NULL;
    int bufLen;

    ret = ramfs_init();
    if (ret == LOS_NOK) {
        FS_LOG_ERR("ramfs_init failed.");
        return;
    }
    ret = ramfs_mount(RAMFS_PATH, RAMFS_SIEZ);
    if (ret == LOS_NOK) {
        FS_LOG_ERR("ramfs_mount failed.");
        return;
    }

    bufLen = strlen(bufWrite);
    write_file("/ramfs/test.txt", bufWrite, bufLen);

    memset_s(bufRead, DEMO_BUF_LEN, 0, DEMO_BUF_LEN);
    read_file("/ramfs/test.txt", bufRead, 1);
    printf("%s\n", bufRead);

    memset_s(bufRead, DEMO_BUF_LEN, 0, DEMO_BUF_LEN);
    read_file("/ramfs/test.txt", bufRead, 10);
    printf("%s\n", bufRead);

    memset_s(bufRead, DEMO_BUF_LEN, 0, DEMO_BUF_LEN);
    read_file("/ramfs/test.txt", bufRead, bufLen);
    printf("%s\n", bufRead);

    ret = open_dir("/ramfs/dir", &testdir);
    if (ret != LOS_OK) {
        FS_LOG_ERR("failed to open dir");
        return;
    }

    write_file("/ramfs/dir/test01.txt", bufWrite, bufLen);
    write_file("/ramfs/dir/test02.txt", bufWrite, bufLen);
    write_file("/ramfs/dir/test03.txt", bufWrite, bufLen);

    ret = read_dir("/ramfs/dir", testdir);
    if (ret != LOS_OK) {
        FS_LOG_ERR("failed to read dir");
        return;
    }

    ret = los_closedir(testdir);
    if (ret != LOS_OK) {
        FS_LOG_ERR("failed to close dir");
        return;
    }
    los_fs_unmount("/ramfs");
}
