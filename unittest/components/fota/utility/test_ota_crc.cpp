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

#include <stdio.h>
#include "ota_crc.h"
#include "test_ota_crc.h"
#include "ota.h"

#define OTA_IMAGE_INTEGRITY_LENGTH 32

typedef struct
{
    ota_state state;
    uint8_t restart_cnt;
    uint8_t cur_state;
    uint16_t rsv;
    int32_t cur_offset;
    uint32_t old_image_length;
    uint32_t image_length;
    uint8_t image_integrity[OTA_IMAGE_INTEGRITY_LENGTH];
    uint32_t crc;
} ota_default_flag;

static void init_assist()
{
    static ota_assist assist;
    assist.func_printf = printf;
    ota_register_assist(&assist);
}

static ota_default_flag test_g_ota_flag;


extern uint32_t calc_crc32(uint32_t origin, const void* buf, int32_t len);

void TestOtaCrc::test_calc_crc32()
{
    uint32_t ret = 0;

    calc_crc32(0, NULL, 0);
    TEST_ASSERT_MSG(0 == ret, "calc_crc32() is failed");
    
    init_assist();
    test_g_ota_flag.state = OTA_S_IDLE;
    test_g_ota_flag.restart_cnt = 0;
    test_g_ota_flag.cur_state = 0;
    test_g_ota_flag.cur_offset = 0;
    test_g_ota_flag.image_length = 0;
    calc_crc32(0, &test_g_ota_flag, sizeof(ota_default_flag) - sizeof(uint32_t));
}

TestOtaCrc::TestOtaCrc()
{
    TEST_ADD(TestOtaCrc::test_calc_crc32);
}


