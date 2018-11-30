/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     example_eflash.c
 * @brief    the main function for the Eflash driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "soc.h"
#include "drv_eflash.h"
#include "csky_All_Demo.h"
static eflash_handle_t eflash = NULL;

static void eflash_erase(int operate_addr, int operate_len)
{
    eflash_info_t *info = NULL;
    uint32_t index;
    uint32_t erase_fail = 0;
    uint32_t data[128];
    uint32_t ret;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    uint32_t sector_num;
    eflash_status_t status;
    index = 1;

    for (i = 0; i < index; i++) {
        eflash = csi_eflash_initialize(i, NULL);

        info = csi_eflash_get_info(eflash);
        sector_num = operate_len / info->sector_size;

        for (j = 0; j < sector_num; j++) {
            ret = csi_eflash_erase_sector(eflash, operate_addr);

            if (ret != 0) {
                printf("[error]the ret is %x the addr is %x \r\n", ret, operate_addr);
            }

            while (1) {
                status = csi_eflash_get_status(eflash);

                if (status.busy == 0) {
                    break;
                }
            }

            ret = csi_eflash_read(eflash, operate_addr & ~(info->sector_size), data, info->sector_size);

            for (k = 1; k < (info->sector_size >> 2); k++) {
                if (data[k] != data[k - 1]) {
                    break;
                }
            }

            if (k != (info->sector_size >> 2)) {
                erase_fail = 1;
                printf("[error]the index %d eflash %x sector is erased failed\r\n", i, j);
            }

            operate_addr = operate_addr + info->sector_size;
        }

        if (erase_fail) {
            printf("[error]the index %d eflash erase failed\r\n", i);
            return;
        } else {
            printf("[success]the index %d eflash erase pass\r\n", i);
        }

        ret = csi_eflash_uninitialize(eflash);
    }
}


static void eflash_program(int operate_addr, int operate_len)
{
    eflash_info_t *info = NULL;
    uint32_t index;
    uint32_t write_fail = 0;
    uint8_t source_data[512];
    uint8_t dest_data[512] = {0};
    uint32_t i;
    uint32_t j;
    uint32_t k;
    uint32_t sector_num;

    memset(source_data, 0x5a, sizeof(source_data));
    index = CONFIG_EFLASH_NUM;

    for (i = 0; i < index; i++) {
        eflash = csi_eflash_initialize(i, NULL);
        info = csi_eflash_get_info(eflash);

        sector_num = operate_len / info->sector_size;

        for (j = 0; j < sector_num; j++) {
            csi_eflash_program(eflash, operate_addr, source_data, info->sector_size);

            csi_eflash_read(eflash, operate_addr, dest_data, info->sector_size);

            for (k = 0; k < info->sector_size; k++) {
                if (dest_data[k] != source_data[k]) {
                    break;
                }
            }

            if (k != info->sector_size) {
                write_fail = 1;
                printf("[error]the index %d eflash %x sector is write failed\r\n", i, j);
            }

            operate_addr = operate_addr + info->sector_size;
            operate_len = operate_len - info->sector_size;
        }

        if (operate_len) {
            csi_eflash_program(eflash, operate_addr, source_data, operate_len);

            csi_eflash_read(eflash, operate_addr, dest_data, operate_len);

            for (k = 0; k < operate_len; k++) {
                if (dest_data[k] != source_data[k]) {
                    break;
                }
            }

            if (k != operate_len) {
                write_fail = 1;
            }
        }

        if (write_fail) {
            printf("[error]the index %d eflash write failed\r\n", i);
            return;
        } else {
            printf("[success]the index %d eflash write pass\r\n", i);
        }

        csi_eflash_uninitialize(eflash);
    }

    printf("[success]test eflash successfully\n");
}
/*****************************************************************************
test_eflash: main function of the eflash test

INPUT: NULL

RETURN: NULL

*****************************************************************************/
int example_eflash(int operate_addr, int operate_len)
{
    eflash_erase(operate_addr, operate_len);
    eflash_program(operate_addr, operate_len);
    return 0;
}

int task_EFLASH(void)
{
#if SHOW_PRECOMPILE_INFO
#pragma Message("非接管中断模式 --> EFLASH")
#endif

	HopeRun_printf("开始EFLASH测试............\n");
    return example_eflash((0x10000000 + 128 * 1024), 0x200);
}
