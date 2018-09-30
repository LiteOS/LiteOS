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

#include "test_fota_package_sha256.h"
#include "fota_package_checksum.h"
#include "fota_package_sha256.h"
#include <string.h>
#include "fota_package_head.h"/*lint !e451*/

extern "C"
{
//    extern typedef struct fota_pack_checksum_alg_tag_s;
}

extern int fota_pack_sha256_init(fota_pack_sha256_s *thi);

static fota_pack_sha256_s thi;

void TestFotaPackageSha256::test_fota_pack_sha256_init()
{
    int ret = fota_pack_sha256_init(&thi);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_init() is failed");
}

void TestFotaPackageSha256::test_fota_pack_sha256_update()
{
    uint8_t buff[] = {0,1,2,3,4};
    int ret = thi.base.update(&thi.base, buff, (uint16_t)5);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_pack_sha256_update() is failed");
}

void TestFotaPackageSha256::test_fota_pack_sha256_check()
{
    int ret = 0;
    ret = thi.base.check(NULL, NULL, 0);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_check() is failed");

    uint8_t checksum[32] = {1};
    ret = thi.base.check(&thi.base, checksum, 0); 
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_check() is failed");

    ret = thi.base.check(&thi.base, checksum, (uint16_t)32); 
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_pack_sha256_check() is failed");
}

void TestFotaPackageSha256::test_fota_pack_sha256_destroy()
{
    thi.base.destroy(&thi.base);
}

TestFotaPackageSha256::TestFotaPackageSha256()
{
    TEST_ADD(TestFotaPackageSha256::test_fota_pack_sha256_init);
    TEST_ADD(TestFotaPackageSha256::test_fota_pack_sha256_update);
    TEST_ADD(TestFotaPackageSha256::test_fota_pack_sha256_check);
    TEST_ADD(TestFotaPackageSha256::test_fota_pack_sha256_destroy);
}

void TestFotaPackageSha256::setup()
{
    std::cout<<"in steup\n";
}

void TestFotaPackageSha256::tear_down()
{
    std::cout<<"in teardown\n";
}


