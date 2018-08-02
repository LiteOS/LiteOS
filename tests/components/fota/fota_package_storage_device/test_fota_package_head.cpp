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

#include "test_fota_package_head.h"
#include "fota_package_head.h"
#include "fota_firmware_writer.h"
#include "stddef.h"
#include "ota_sha256.h"


struct fota_pack_checksum_tag_s
{
    ota_sha256_context sha256_context;
    uint32_t offset;
    bool offset_flag;
    fota_pack_head_s *head;
};


extern "C"
{
    extern int fota_pack_head_parse_head_len(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *used_len);
    extern int fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *used_len);
    extern int fota_pack_head_check(const fota_pack_head_s *head, uint32_t len);
    //extern int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_hardware_s *hardware, head_update_check update_check, void *param);
    extern int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_pack_device_info_s *device_info);
    extern fota_pack_checksum_s *fota_pack_head_get_checksum(fota_pack_head_s *head);
    extern void fota_pack_head_init(fota_pack_head_s *head);
}

static int flag_test_get_block_size;
static uint32_t test_get_block_size(struct fota_hardware_api_tag_s *thi, uint32_t offset)
{
    if (flag_test_get_block_size == 0)
        return 0;
    if (flag_test_get_block_size == 1)
        return 10;
}


static int flag_head_len;
int si_fota_pack_head_parse_head_len(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{
    if(flag_head_len == 0)
        return 0;    
}

static int test_update_check(const uint8_t *head_buff , uint16_t len, void *param)
{
    return FOTA_ERR;
}

void TestFotaPackageHead::test_fota_pack_head_init()
{
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    fota_pack_head_init(head);
    TEST_ASSERT_MSG((NULL != head), "fota_pack_head_init() is failed");

    free(head);
}

void TestFotaPackageHead::test_fota_pack_head_parse_head_len()
{
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    uint32_t offset = 0;
    uint8_t buff[12] = {0};
    uint16_t len = 0;
    uint16_t used_len = 0;

    offset = 10;
    head->stored_len = 5;
    int ret = fota_pack_head_parse_head_len(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");

    offset = 0;
    head->buff = NULL;
    ret = fota_pack_head_parse_head_len(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse_head_len() is failed"); // create head->buff;

    head->stored_len = 10;
    offset = 0;
    len = 12;
    buff[3] = 1;
    ret = fota_pack_head_parse_head_len(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");

    buff[3] = 0;
    ret = fota_pack_head_parse_head_len(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");

    buff[4] = buff[5] = 1;
    buff[8] = buff[9] = buff[10] = buff[11] = 1;
    ret = fota_pack_head_parse_head_len(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse_head_len() is failed");
    
    atiny_free(head->buff);
    free(head);
}

void TestFotaPackageHead::test_fota_pack_head_destroy()
{
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    head->buff = (uint8_t *)malloc(sizeof(uint8_t));
    memset(head->buff, 0, sizeof(uint8_t));
    head->checksum_pos = (uint8_t *)malloc(sizeof(uint8_t));
    memset(head->checksum_pos, 0, sizeof(uint8_t));
    head->checksum = (fota_pack_checksum_s *)malloc(sizeof(fota_pack_checksum_s));
    memset(head->checksum, 0, sizeof(fota_pack_checksum_s));

    fota_pack_head_destroy(head);
    TEST_ASSERT_MSG((NULL == head->buff), "fota_pack_head_destroy() is failed");

    free(head);
}

void TestFotaPackageHead::test_fota_pack_parse_checksum()
{
    stubInfo si_head_len;
    setStub((void *)fota_pack_head_parse_head_len, (void *)si_fota_pack_head_parse_head_len, &si_head_len);

    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    uint32_t offset = 0;
//        uint8_t buff[20] = {0};
    uint8_t buff[12] = {0};
    uint16_t len = 0;
    uint16_t used_len = 0;
    int ret = 0;

    flag_head_len = 0;
    offset = 5;
    len = 12;
    head->head_len = 14;
    head->stored_len = 15;
    head->buff = (uint8_t *)malloc(15 * sizeof(uint8_t));
    memset(head->buff, 0, 15 * sizeof(uint8_t));
    buff[11 - offset] = 15;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);  
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_parse_checksum() is failed");

    head->head_len = 17;
    buff[11 - offset] = 18;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_parse_checksum() is failed");


    buff[8] = 1;
    head->checksum_pos = (uint8_t *)malloc(sizeof(uint8_t));  
    memset(head->checksum_pos, 0, sizeof(uint8_t));
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_parse_checksum() is failed");

    buff[10] = 1;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_parse_checksum() is failed");

    free(head->checksum);
    head->checksum = (fota_pack_checksum_s *)malloc(sizeof(fota_pack_checksum_s));
    memset(head->checksum, 0, sizeof(fota_pack_checksum_s));
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_parse_checksum() is failed");

    free(head->checksum);
    free(head->checksum_pos);
    free(head->buff);
    free(head);
    cleanStub(&si_head_len);
}

void TestFotaPackageHead::test_fota_pack_head_parse()
{
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    uint32_t offset = 0;
    uint8_t buff[12] = {0};
    uint16_t len = 0;
    uint16_t used_len = 0;
    int ret = 0;

    offset = 10;
    head->stored_len = 5;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

    offset = 0;
    head->buff = NULL;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
//    free(head->buff);

    len = 12;
    head->stored_len = 10;
    buff[4] = buff[5] = 1;
    buff[8] = buff[9] = buff[10] = buff[11] = 1;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");
    free(head->buff);

    memset(head, 0, sizeof(fota_pack_head_s));
    head->head_len = 12;
    head->stored_len = 15;
    len = 12;
    head->buff = (uint8_t *)malloc(15 * sizeof(uint8_t));
    memset(head->buff, 0, 15 * sizeof(uint8_t));
    stubInfo si_head_len;
    setStub((void *)fota_pack_head_parse_head_len, (void *)si_fota_pack_head_parse_head_len, &si_head_len);
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);     //stored_len = 12;
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
    
#if 0
    memset(buff, 0, sizeof(buff));
    buff[11] = 10;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");    //â†
#endif

    head->update_check = test_update_check;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

    memset(buff, 0, sizeof(buff));
    buff[11] = 10;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");     //â†
    
#if 0
    buff[11] = 15;
    head->hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    memset(head->hardware, 0, sizeof(fota_hardware_s));
    head->hardware->get_block_size = test_get_block_size;
    ret = fota_pack_head_parse(head, offset, buff, len, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");
#endif 

    cleanStub(&si_head_len);
    free(head->buff);
    free(head);
}

void TestFotaPackageHead::test_fota_pack_head_check()
{
    uint32_t len = 0;
    fota_pack_head_s * head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));    
    memset(head, 0, sizeof(fota_pack_head_s));
    uint8_t buff[12] = {0};
    head->buff = buff;

    int ret = fota_pack_head_check(head,len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

    len = 1;
    head->stored_len = head->head_len = 12;
    ret = fota_pack_head_check(head,len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

    len = 0;
    ret = fota_pack_head_check(head,len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");

    free(head);
}

void TestFotaPackageHead::test_fota_pack_head_set_head_info()
{
    fota_pack_head_s head;
    fota_pack_device_info_s device_info;
    memset(&head, 0, sizeof(fota_pack_head_s));
    memset(&device_info, 0, sizeof(fota_pack_device_info_s));
    int ret = fota_pack_head_set_head_info(&head, &device_info);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_set_head_info() is failed");

}

void TestFotaPackageHead::test_fota_pack_head_get_checksum()
{
    fota_pack_head_s *head = (fota_pack_head_s *)malloc(sizeof(fota_pack_head_s));
    memset(head, 0, sizeof(fota_pack_head_s));
    head->checksum = NULL;
    fota_pack_checksum_s * cheksum = fota_pack_head_get_checksum(head);
    TEST_ASSERT_MSG((NULL == cheksum), "fota_pack_head_get_checksum() is failed");

    free(head);
}

TestFotaPackageHead::TestFotaPackageHead()
{
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_parse_head_len);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_destroy);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_parse_checksum);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_parse);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_check);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_set_head_info);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_get_checksum);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_init)
}



