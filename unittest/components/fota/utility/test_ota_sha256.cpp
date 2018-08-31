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
#include "test_ota_sha256.h"
#include "ota_sha256.h"
#include "ota.h"
#include "ota_default.h"
#include "adapter_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern void ota_sha256_init( ota_sha256_context *ctx );
extern void ota_sha256_free( ota_sha256_context *ctx );
extern void ota_sha256_starts( ota_sha256_context *ctx, int is224 );

extern int ota_default_set_reboot(int32_t image_len);
void TestOtaSha256::test_ota_sha256_init()
{
    ota_sha256_context *ctx = NULL;
    ota_sha256_init(ctx);
    TEST_ASSERT_MSG(ctx == NULL, "ota_sha256_init() is failed");

    ctx = (ota_sha256_context *)malloc(sizeof(ota_sha256_context));
    ota_sha256_init(ctx);
    TEST_ASSERT_MSG(ctx != NULL, "ota_sha256_init() is failed");

    free(ctx);
}

void TestOtaSha256::test_ota_sha256_free()
{
    ota_sha256_context *ctx = NULL;
    ota_sha256_free(ctx);
    TEST_ASSERT_MSG(ctx == NULL, "ota_sha256_free() is failed");

    ctx = (ota_sha256_context *)malloc(sizeof(ota_sha256_context));
    ota_sha256_init(ctx);
    ota_sha256_free(ctx);
    TEST_ASSERT_MSG(ctx != NULL, "ota_sha256_free() is failed");

    free(ctx);    
}

void TestOtaSha256::test_ota_sha256_starts()
{
    ota_sha256_context *ctx = NULL;
    ctx = (ota_sha256_context *)malloc(sizeof(ota_sha256_context));
    ota_sha256_init(ctx);
    ota_sha256_starts(ctx, 0);
    TEST_ASSERT_MSG(ctx->is224 == 0, "ota_sha256_starts() is failed");

    ota_sha256_starts(ctx, 1);
    TEST_ASSERT_MSG(ctx->is224 == 1, "ota_sha256_starts() is failed");

    free(ctx);   
}
static void init_assist()
{
    static ota_assist assist;
    assist.func_printf = printf;
    assist.func_ota_read = hal_spi_flash_read;
    assist.func_ota_write = hal_spi_flash_erase_write;
    ota_register_assist(&assist);
}

void TestOtaSha256::test_ota_sha256_update()
{init_assist();
    int32_t test_image_len = -1;
        int ret = ota_default_set_reboot(test_image_len);
        TEST_ASSERT_MSG((OTA_ERRNO_ILEGAL_PARAM == ret),"ota_default_set_reboot() is failed");
    
        test_image_len = 0x1000;
        ret = ota_default_set_reboot(test_image_len);
        TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_set_reboot() is failed");
//    test_image_len = 0x10000;
//    ret = ota_default_set_reboot(test_image_len);
}


TestOtaSha256::TestOtaSha256()
{
    TEST_ADD(TestOtaSha256::test_ota_sha256_init);
    TEST_ADD(TestOtaSha256::test_ota_sha256_free);
    TEST_ADD(TestOtaSha256::test_ota_sha256_starts);
    TEST_ADD(TestOtaSha256::test_ota_sha256_update);
}


