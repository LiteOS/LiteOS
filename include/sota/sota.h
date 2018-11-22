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
#ifndef __SOTA_H__
#define __SOTA_H__

#include<stdint.h>
#include "los_memory.h"
#include"ota/ota_api.h"

typedef enum
{
    IDLE = 0,
    DOWNLOADING,
    DOWNLOADED,
    UPDATING,
    UPDATED,
}at_fota_state;

typedef enum
{
    APP_MODE = 0,
    BOOTLOADER_MODE,
}run_mode_e;

typedef struct
{
    int (*get_ver)(char* buf, uint32_t len);
    int (*set_ver)(const char* buf, uint32_t len);
    int (*sota_send)(const char* buf, int len);
    void* (*sota_malloc)(size_t size);
    void (*sota_free)(void *ptr);
    uint32_t frame_buf_len;
    uint8_t  run_mode;
    uint8_t  rsv[3];
    ota_opt_s ota_info;
} sota_opt_t;

typedef struct
{
    int (*read_flash)(ota_flash_type_e type, void *buf, int32_t len, uint32_t location);
    int (*write_flash)(ota_flash_type_e type, const void *buf, int32_t len, uint32_t location);
}sota_flag_opt_s;

int sota_init(sota_opt_t* flash_op);
int32_t sota_process_main(void *arg, const int8_t *buf, int32_t buflen);
void sota_tmr(void);

#define SOTA_DEBUG
#ifdef SOTA_DEBUG
#define SOTA_LOG(fmt, arg...)  printf("[%s:%d][I]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define SOTA_LOG(fmt, arg...)
#endif

typedef enum
{
SOTA_OK = 0,
SOTA_DOWNLOADING = 1,
SOTA_NEEDREBOOT = 2,
SOTA_BOOTLOADER_DOWNLOADING = 3,
SOTA_MEM_FAILED = 4,
SOTA_FAILED = 101,
SOTA_TIMEOUT = 102,
}SOTA_RET;
#endif
