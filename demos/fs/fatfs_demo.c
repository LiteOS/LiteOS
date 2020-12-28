/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Fat Fs Demo
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

/* Includes ----------------------------------------------------------------- */
#include "fs_common.h"
#include "fatfs_hal.h"

#define FATFS_PATH "/fatfs"

char fatfs_file_name[100] = {0};
char fatfs_dir_name[100] = {0};

extern int fatfs_init(int need_erase);
extern int fatfs_unmount(const char *path, uint8_t drive);

void fatfs_demo(void)
{
    int8_t drive;
    int ret;

    drive = hal_fatfs_init(0);
    if (drive < 0) {
        FS_LOG_ERR("fatfs_init failed.");
        return;
    }

    ret = sprintf_s(fatfs_file_name, sizeof(fatfs_file_name), "%s/%d:/%s", FATFS_PATH, (uint8_t)drive, LOS_FILE);
    if (ret <= 0) {
        FS_LOG_ERR("sprintf_s file name failed.");
    }

    ret = sprintf_s(fatfs_dir_name, sizeof(fatfs_dir_name), "%s/%d:/%s", FATFS_PATH, (uint8_t)drive, LOS_DIR);
    if (ret <= 0) {
        FS_LOG_ERR("sprintf_s dir name failed.");
    }

    los_vfs_io(fatfs_file_name, fatfs_dir_name);

    fatfs_unmount("/fatfs/", drive);
}
