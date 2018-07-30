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

#include "test_fota_package_checksum.h"
#include "fota_package_checksum.h"
#include <string.h>
#include "fota_package_head.h"
#include "ota_sha256.h"

struct fota_pack_checksum_tag_s
{
    ota_sha256_context sha256_context;
    uint32_t offset;
    bool offset_flag;
    fota_pack_head_s *head;
};

static int read_software(struct fota_hardware_api_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    return 0;
}

extern "C"
{
    
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_create()
{
    fota_pack_checksum_s * thi = fota_pack_checksum_create(FOTA_PACK_SHA_S56);
    TEST_ASSERT_MSG((thi != NULL), "fota_pack_checksum_create() is failed");
    atiny_free(thi);

    thi = fota_pack_checksum_create(FOTA_PACK_MAX_CHECKSUM_TYPE);
    TEST_ASSERT_MSG((thi == NULL), "fota_pack_checksum_create() is failed");
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_delete()
{
    fota_pack_checksum_s * thi = NULL;
    fota_pack_checksum_delete(thi);
    TEST_ASSERT_MSG((thi == NULL), "fota_pack_checksum_delete() is failed");

    thi = fota_pack_checksum_create(FOTA_PACK_SHA_S56);
    fota_pack_checksum_delete(thi);
    TEST_ASSERT_MSG((thi != NULL), "fota_pack_checksum_delete() is failed");
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_update_head()
{
    int ret = fota_pack_checksum_update_head(NULL,NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_head() is failed");

    fota_pack_checksum_s * thi = fota_pack_checksum_create(FOTA_PACK_SHA_S56);
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    ret = fota_pack_checksum_update_head(thi,head);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_head() is failed");

    head->head_len = head->stored_len = 12;
    ret = fota_pack_checksum_update_head(thi,head);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_head() is failed");

    head->buff = (uint8_t *)malloc(12 * sizeof(uint8_t));
    ret = fota_pack_checksum_update_head(thi,head);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_head() is failed");

    free(head->buff);
    free(head);
    atiny_free(thi);
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_update_data()
{
    int ret = fota_pack_checksum_update_data(NULL, 0, NULL, 0, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");

    fota_pack_checksum_s * thi = fota_pack_checksum_create(FOTA_PACK_SHA_S56);
    ret = fota_pack_checksum_update_data(thi, 0, NULL, 0, NULL);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");

    ret = fota_pack_checksum_update_data(thi, 0, NULL, 12, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");

    thi->head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(thi->head, 0, sizeof(fota_pack_head_s));
    uint8_t * buff = (uint8_t *)malloc(12 * sizeof(uint8_t));
    memset(buff, 0, sizeof(buff));
    thi->offset_flag = true;
    thi->offset = 12;
    ret = fota_pack_checksum_update_data(thi, 12, buff, 12, NULL);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");

    thi->offset_flag = false;
    ret = fota_pack_checksum_update_data(thi, 12, buff, 12, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");

    fota_hardware_s * hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    hardware->read_software = read_software;
    ret = fota_pack_checksum_update_data(thi, 12, buff, 12, hardware);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");
    
    free(hardware);
    free(buff);
    free(thi->head);
    atiny_free(thi);    
}    

void TestFotaPackageCheckSum::test_fota_pack_checksum_check()
{
    fota_pack_checksum_s * thi = NULL;
    int ret = fota_pack_checksum_check(thi, NULL, 0);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_check() is failed");

    thi = fota_pack_checksum_create(FOTA_PACK_SHA_S56);
    ret = fota_pack_checksum_check(thi, NULL, 0);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_check() is failed");

    uint8_t expected_value[32] = {0};
    ret = fota_pack_checksum_check(thi, expected_value, 32);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_check() is failed");

    atiny_free(thi);
}

TestFotaPackageCheckSum::TestFotaPackageCheckSum()
{
    TEST_ADD(TestFotaPackageCheckSum::test_fota_pack_checksum_create);
    TEST_ADD(TestFotaPackageCheckSum::test_fota_pack_checksum_delete);
    TEST_ADD(TestFotaPackageCheckSum::test_fota_pack_checksum_update_head);
    TEST_ADD(TestFotaPackageCheckSum::test_fota_pack_checksum_update_data);
    TEST_ADD(TestFotaPackageCheckSum::test_fota_pack_checksum_check);
}


