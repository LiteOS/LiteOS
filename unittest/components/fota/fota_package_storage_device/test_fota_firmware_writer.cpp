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
#include "test_fota_firmware_writer.h"
#include "fota_firmware_writer.h"
#include "fota_package_head.h"

static int funcno;

extern "C"
{
    extern void fota_fmw_wr_free_save_buffer(fota_firmware_writer_s *writer);
    extern void fota_fmw_wr_destroy(fota_firmware_writer_s *writer); 
    extern int fota_fmw_wr_check(fota_firmware_writer_s *writer);
    extern int fota_fmw_wr_create_buffer(fota_firmware_writer_s *writer, uint32_t offset);
    extern int fota_fmw_wr_write_combined_data(fota_firmware_writer_s *writer,
                                            uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *write_len);
    extern int fota_fmw_wr_write_buffer_data(fota_firmware_writer_s *writer, uint32_t offset,
                                              const uint8_t *buff, uint16_t len);
    extern int fota_fmw_wr_write_stored_data(fota_firmware_writer_s *writer);
    extern int fota_fmw_wr_write_end(fota_firmware_writer_s *writer);
}

static int test_fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{
    *used_len = len;
    return 0;
}

static int flag;
static uint32_t get_block_size(struct fota_hardware_api_tag_s *thi, uint32_t offset)
{
    if (flag == 0)
        return 0;
    if (flag == 1)
        return 10;
}

static int flag_write_software;
static int write_software(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{   
    if (flag_write_software == 1);
    {
        len = 0;printf("**********  %d  *********\n",__LINE__);
    }
    return FOTA_OK;
}

static fota_firmware_writer_s test_writer;

void TestFotaFirmwareWrite::test_fota_fmw_wr_init()
{
    //fota_firmware_writer_s test_writer;
    fota_fmw_wr_init(&test_writer);
    TEST_ASSERT_MSG((test_writer.offset == 0), "fota_fmw_wr_init() is failed");
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_free_save_buffer()
{
    //fota_firmware_writer_s test_writer;
    test_writer.buffer = (uint8_t *)atiny_malloc(sizeof(uint8_t));
    fota_fmw_wr_free_save_buffer(&test_writer);
    TEST_ASSERT_MSG((test_writer.buffer == NULL), "fota_fmw_wr_free_save_buffer() is failed");
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_destroy()
{
    fota_fmw_wr_destroy(&test_writer);
    fota_fmw_wr_free_save_buffer(&test_writer);
    TEST_ASSERT_MSG((test_writer.buffer == NULL), "fota_fmw_wr_destroy() is failed");
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_set_device()
{
    fota_fmw_wr_set_device(&test_writer, NULL, NULL);
    TEST_ASSERT_MSG((test_writer.hardware == NULL), "fota_fmw_wr_set_device() is failed");
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_check()
{
    fota_fmw_wr_init(&test_writer);
    int ret = fota_fmw_wr_check(&test_writer);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_check() is failed");
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_create_buffer()
{
    fota_fmw_wr_init(&test_writer);
    test_writer.hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    test_writer.hardware->get_block_size = get_block_size;
    test_writer.buffer_len = 10;
    int ret = fota_fmw_wr_create_buffer(&test_writer, 0);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_check() is failed");

    flag = 1;
    ret = fota_fmw_wr_create_buffer(&test_writer, 0);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");
    atiny_free(test_writer.buffer);

    test_writer.buffer = (uint8_t *)malloc(sizeof(uint8_t));
    ret = fota_fmw_wr_create_buffer(&test_writer, 0);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");
    
    free(test_writer.buffer);
    free(test_writer.hardware);
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_write_combined_data()
{
    fota_fmw_wr_init(&test_writer);
    uint8_t *buff = NULL;
    uint16_t len = 0;
    uint16_t write_len = 0;
    int ret = fota_fmw_wr_write_combined_data(&test_writer, 0, buff, len, &write_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");

    test_writer.offset_flag = 0;
    test_writer.buffer = (uint8_t *)malloc(sizeof(uint8_t));
    test_writer.buffer_stored_len = 1;
    flag = 0;
    test_writer.hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    test_writer.hardware->get_block_size = get_block_size;
    ret = fota_fmw_wr_write_combined_data(&test_writer, 0, buff, len, &write_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_check() is failed");

    flag = 1;
    test_writer.buffer_len = 1;
    ret = fota_fmw_wr_write_combined_data(&test_writer, 0, buff, len, &write_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_check() is failed");

    test_writer.buffer_len = 10;
    ret = fota_fmw_wr_write_combined_data(&test_writer, 0, buff, len, &write_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");

    free(test_writer.buffer);
    free(test_writer.hardware);        
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_write_buffer_data()
{
    int ret = 0;
    fota_fmw_wr_init(&test_writer);
    test_writer.hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    test_writer.hardware->get_block_size = get_block_size;
    flag = 0;
    ret = fota_fmw_wr_write_buffer_data(&test_writer, 0, NULL, 1);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_check() is failed");

    flag = 1;
    uint8_t buff[] = {0,1,2,3,4,5,6,7,8,9};
    test_writer.buffer_len = 10;
    test_writer.buffer = (uint8_t *)malloc(10 * sizeof(uint8_t));
    ret = fota_fmw_wr_write_buffer_data(&test_writer, 10, buff, 1);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");

    flag = 1;
    flag_write_software = 1;
    test_writer.storage_device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    test_writer.storage_device->write_software = write_software;
    uint8_t buf[] = {1,2};printf("**********  %d  *********\n",__LINE__);
    ret = fota_fmw_wr_write_buffer_data(&test_writer, 2, buf, 10);printf("**********  %d  *********\n",__LINE__);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_check() is failed");

    free(test_writer.storage_device);
    free(test_writer.buffer);
    free(test_writer.hardware);
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_write()
{
    fota_fmw_wr_init(&test_writer);
    test_writer.hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    test_writer.hardware->get_block_size = get_block_size;
    test_writer.storage_device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    test_writer.storage_device->write_software = NULL;
    int ret = fota_fmw_wr_write(&test_writer, 0, NULL, 0);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_write() is failed");

    test_writer.storage_device->write_software = write_software;
    ret = fota_fmw_wr_write(&test_writer, 0, NULL, 0);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_write() is failed");

    free(test_writer.hardware);
    free(test_writer.storage_device);
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_write_stored_data()
{   
    fota_fmw_wr_init(&test_writer);
    test_writer.storage_device = fota_get_pack_device();
    int ret = fota_fmw_wr_write_stored_data(&test_writer);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_fmw_wr_write_stored_data() is failed");

//    stubInfo si_fota_pack_head_parse;
//    setStub((void *)fota_pack_head_parse, (void *)test_fota_pack_head_parse, &si_fota_pack_head_parse);
//    test_writer.buffer_stored_len = 12;
//    ret = fota_fmw_wr_write_stored_data(&test_writer);
//    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_write_stored_data() is failed");

//    cleanStub(&si_fota_pack_head_parse);
}

void TestFotaFirmwareWrite::test_fota_fmw_wr_write_end()
{
    fota_fmw_wr_init(&test_writer);
    int ret = fota_fmw_wr_write_end(&test_writer);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_write_end() is failed");

    test_writer.buffer_stored_len = 12; 
    test_writer.storage_device = (atiny_fota_storage_device_s*)malloc(sizeof(atiny_fota_storage_device_s));
    test_writer.storage_device->write_software = write_software;
    ret = fota_fmw_wr_write_end(&test_writer);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_fmw_wr_write_end() is failed");

    free(test_writer.storage_device);
}

TestFotaFirmwareWrite::TestFotaFirmwareWrite()
{
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_init);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_free_save_buffer);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_destroy);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_set_device);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_check);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_create_buffer);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_write_combined_data);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_write_buffer_data);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_write);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_write_stored_data);
    TEST_ADD(TestFotaFirmwareWrite::test_fota_fmw_wr_write_end);
}

TestFotaFirmwareWrite::~TestFotaFirmwareWrite()
{  
}

void TestFotaFirmwareWrite::setup()
{
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestFotaFirmwareWrite::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}


