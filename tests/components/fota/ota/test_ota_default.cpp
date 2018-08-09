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

#include "test_ota_default.h"
#include "ota.h"
#include "ota_default.h"
#include "adapter_layer.h"
#include "ota_sha256.h"


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


extern "C"
{
    uint32_t calc_crc32(uint32_t origin, const void* buf, int32_t len);
}

#define OTA_IMAGE_DOWNLOAD_SIZE       0x00040000

static ota_default_flag test_g_ota_flag;
static int flag;

static int test_board_rollback_copy(int32_t image_len,
                        void (*func_get_update_record)(uint8_t* state, uint32_t* offset),
                        int (*func_set_update_record)(uint8_t state, uint32_t offset))
{
    return -1;
}

static int test_board_update_copy(int32_t old_image_len, int32_t new_image_len,
                      void (*func_get_update_record)(uint8_t* state, uint32_t* offset),
                      int (*func_set_update_record)(uint8_t state, uint32_t offset))
{
    if(flag == -1)
        return -1;
    uint8_t* state = (uint8_t *)malloc(sizeof(uint8_t));
    uint32_t* offset = (uint32_t *)malloc(sizeof(uint32_t));
    func_get_update_record(state, offset);
    func_set_update_record(*state, *offset);

    free(state);
    free(offset);
    return 0;
}

static uint32_t test_calc_crc32(uint32_t origin, const void* buf, int32_t len)
{
    return 0;
}

int test_hal_spi_flash_read(void* buf, int32_t len, uint32_t location)
{  
    switch (flag)
    {
    case 0:     break;
    case 1:
        memcpy(buf, &test_g_ota_flag, len);
//        buf = (void *)(&test_g_ota_flag);
    
        break;
    case -1:
        return -1;
    default:    break;
    }
    return 0;
}

int test_hal_spi_flash_erase_write(const void* buf, int32_t len, uint32_t location)
{
    switch (flag)
    {
    case 0:    
        break;
    case -1:   
        return -1;
    default:   
        break;
    }
    return 0;
}

static void init_assist()
{
    static ota_assist assist;
    assist.func_printf = printf;
    assist.func_ota_read = test_hal_spi_flash_read;
    assist.func_ota_write = test_hal_spi_flash_erase_write;
    ota_register_assist(&assist);
}

//  Global Val 
//init assist
//static ota_assist assist;
//assist.func_printf = printf;
//assist.func_ota_read = test_hal_spi_flash_read;
//assist.func_ota_write = test_hal_spi_flash_erase_write;
//ota_register_assist(&assist);


void TestOtaDefault::test_ota_default_init()
{
    init_assist();
    int ret = ota_default_init();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_init() is failed");

    flag = -1;
    ret = ota_default_init();
    TEST_ASSERT_MSG((OTA_ERRNO_SPI_FLASH_WRITE == ret),"ota_default_init() is failed");
    flag = 0;
}

void TestOtaDefault::test_ota_default_set_reboot()
{
    int32_t test_image_len = -1;
    int ret = ota_default_set_reboot(test_image_len);
    TEST_ASSERT_MSG((OTA_ERRNO_ILEGAL_PARAM == ret),"ota_default_set_reboot() is failed");

    test_image_len = 0x1000;
    ret = ota_default_set_reboot(test_image_len);
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_set_reboot() is failed");

//    test_image_len = 0x10000;
//    ret = ota_default_set_reboot(test_image_len);
}

void TestOtaDefault::test_ota_default_check_update_state()
{
    init_assist();
    stubInfo si_calc_crc32;
    calc_crc32(0, NULL, 0);
    setStub((void *)calc_crc32, (void *)test_calc_crc32, &si_calc_crc32);
    test_g_ota_flag.state = OTA_S_SUCCEED;
    test_g_ota_flag.restart_cnt = 0;
    test_g_ota_flag.cur_state = 0;
    test_g_ota_flag.cur_offset = 0;
    test_g_ota_flag.image_length = 0;
    test_g_ota_flag.crc = calc_crc32(0, &test_g_ota_flag, sizeof(ota_default_flag) - sizeof(uint32_t));
    test_g_ota_flag.old_image_length = OTA_IMAGE_DOWNLOAD_SIZE;
    memset(test_g_ota_flag.image_integrity, 0, OTA_IMAGE_INTEGRITY_LENGTH);
    flag = 1;
    ota_default_init();
    ota_state* test_st = (ota_state *)malloc(sizeof(ota_state));
    int ret = ota_default_check_update_state(test_st);
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_check_update_state() is failed");

//    flag = -1;
//    ret = ota_default_check_update_state(test_st);
//    TEST_ASSERT_MSG((OTA_ERRNO_SPI_FLASH_WRITE == ret),"ota_default_check_update_state() is failed");
    
    free(test_st);
    cleanStub(&si_calc_crc32);
}

// USE_BOOTLOADER
void TestOtaDefault::test_ota_default_update_process()
{   
    stubInfo si_calc_crc32;
    setStub((void *)calc_crc32, (void *)test_calc_crc32, &si_calc_crc32);
    init_assist();
    test_g_ota_flag.state = OTA_S_IDLE;
    test_g_ota_flag.restart_cnt = 0;
    test_g_ota_flag.cur_state = 0;
    test_g_ota_flag.cur_offset = 0;
    test_g_ota_flag.image_length = 0;
    test_g_ota_flag.crc = calc_crc32(0, &test_g_ota_flag, sizeof(ota_default_flag) - sizeof(uint32_t));
    test_g_ota_flag.old_image_length = OTA_IMAGE_DOWNLOAD_SIZE;
    memset(test_g_ota_flag.image_integrity, 0, OTA_IMAGE_INTEGRITY_LENGTH);
    flag = 1;
    ota_default_init();
    int ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");

    test_g_ota_flag.state = OTA_S_FAILED;
    ota_default_init();
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");

    test_g_ota_flag.state = OTA_S_NEEDUPDATE;
    test_g_ota_flag.image_integrity[0] = 0;
    ota_default_init();
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_INTEGRITY_CHECK == ret),"ota_default_roll_back_image() is failed");

    test_g_ota_flag.image_length = 1;
    flag = 1;
    ota_default_init();
    flag = -1;
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_INTEGRITY_CHECK == ret),"ota_default_roll_back_image() is failed");

    test_g_ota_flag.state = OTA_S_UPDATING;
    flag = 1;
    ota_default_init();
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");

    stubInfo si_board_update_copy;
    setStub((void *)board_update_copy, (void *)test_board_update_copy, &si_board_update_copy);
    flag = -1;
    ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");
    cleanStub(&si_board_update_copy);

    flag = 1;
    test_g_ota_flag.state = OTA_S_SUCCEED;
    test_g_ota_flag.restart_cnt = 5 + 1;
    ota_default_init();
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");

    test_g_ota_flag.restart_cnt = 0;
    ota_default_init();
    ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_roll_back_image() is failed");

//    test_g_ota_flag.state = (ota_state)8848;
//    ota_default_init();
//    ret = ota_default_update_process();
//    TEST_ASSERT_MSG((OTA_ERRNO_INTERNEL == ret),"ota_default_roll_back_image() is failed");
      
    cleanStub(&si_calc_crc32);
}

void TestOtaDefault::test_ota_default_jump_to_application()
{
    int ret = ota_default_jump_to_application();
    TEST_ASSERT_MSG((0 == ret),"ota_default_jump_to_application() is failed");
}

void TestOtaDefault::test_ota_default_roll_back_image()
{   
    
}

void TestOtaDefault::test_prv_get_update_record()
{
    stubInfo si_calc_crc32, si_board_update_copy;
    setStub((void *)calc_crc32, (void *)test_calc_crc32, &si_calc_crc32);
    setStub((void *)board_update_copy, (void *)test_board_update_copy, &si_board_update_copy);
    init_assist();
    test_g_ota_flag.state = OTA_S_UPDATING;
    test_g_ota_flag.restart_cnt = 0;
    test_g_ota_flag.cur_state = 0;
    test_g_ota_flag.cur_offset = 0;
    test_g_ota_flag.image_length = 0;
    test_g_ota_flag.crc = calc_crc32(0, &test_g_ota_flag, sizeof(ota_default_flag) - sizeof(uint32_t));
    test_g_ota_flag.old_image_length = OTA_IMAGE_DOWNLOAD_SIZE;
    memset(test_g_ota_flag.image_integrity, 0, OTA_IMAGE_INTEGRITY_LENGTH);
    flag = 1;
    ota_default_init();

    int ret = ota_default_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_default_jump_to_application() is failed");
    cleanStub(&si_calc_crc32);
}

// test ota.c

void TestOtaDefault::test_ota_register_module()
{
    ota_module *test_module = (ota_module *)malloc(sizeof(ota_module));
    memset(test_module, 0, sizeof(ota_module));
//    test_module->func_init = ota_init;
//    test_module->func_update_process = ota_update_process;
//    test_module->func_jump_to_application = ota_jump_to_application;
//    test_module->func_roll_back_image = ota_roll_back_image;    
    ota_register_module(test_module);
    free(test_module);
}

void TestOtaDefault::test_ota_register_assist()
{

}

void TestOtaDefault::test_ota_update_process()
{   
    init_assist();
    test_g_ota_flag.state = OTA_S_IDLE;
    test_g_ota_flag.restart_cnt = 0;
    test_g_ota_flag.cur_state = 0;
    test_g_ota_flag.cur_offset = 0;
    test_g_ota_flag.image_length = 0;
    test_g_ota_flag.crc = calc_crc32(0, &test_g_ota_flag, sizeof(ota_default_flag) - sizeof(uint32_t));
    test_g_ota_flag.old_image_length = OTA_IMAGE_DOWNLOAD_SIZE;
    memset(test_g_ota_flag.image_integrity, 0, OTA_IMAGE_INTEGRITY_LENGTH);
    flag = 1;
    ota_default_init();   

    int ret = ota_update_process();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_update_process() is failed");
}

void TestOtaDefault::test_ota_jump_to_application()
{
    int ret = ota_jump_to_application();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_jump_to_application() is failed");
}

void TestOtaDefault::test_ota_roll_back_image()
{
    int ret = ota_roll_back_image();
    TEST_ASSERT_MSG((OTA_ERRNO_OK == ret),"ota_roll_back_image() is failed");

    stubInfo si;
    setStub((void *)board_rollback_copy, (void *)test_board_rollback_copy, &si);
    ret = ota_roll_back_image();
    TEST_ASSERT_MSG((-1 == ret),"ota_roll_back_image() is failed");

    ota_sha256_init(NULL);
    ota_sha256_free(NULL);
}

//void TestOtaDefault::test_calc_crc32()
//{
//    int ret = calc_crc32(0, NULL, 0);
//    TEST_ASSERT_MSG((0 == ret),"calc_crc32() is failed");
//}

TestOtaDefault::TestOtaDefault()
{
    TEST_ADD(TestOtaDefault::test_ota_default_init);
    TEST_ADD(TestOtaDefault::test_ota_default_set_reboot);
    TEST_ADD(TestOtaDefault::test_ota_default_check_update_state);

    // USE_BOOTLOADER
    TEST_ADD(TestOtaDefault::test_ota_default_update_process);
    TEST_ADD(TestOtaDefault::test_ota_default_jump_to_application);
    TEST_ADD(TestOtaDefault::test_ota_default_roll_back_image);

    // test ota.c
    TEST_ADD(TestOtaDefault::test_ota_register_module);
    TEST_ADD(TestOtaDefault::test_ota_register_assist);
    //TEST_ADD(TestOtaDefault::test_ota_init);
    TEST_ADD(TestOtaDefault::test_ota_update_process);
    TEST_ADD(TestOtaDefault::test_ota_jump_to_application);
    TEST_ADD(TestOtaDefault::test_ota_roll_back_image);
    TEST_ADD(TestOtaDefault::test_prv_get_update_record);
    // USE_BOOTLOADER NO!
//    TEST_ADD(TestOtaDefault::test_ota_set_reboot);
//    TEST_ADD(TestOtaDefault::test_ota_check_update_state);

    // test ota_crc.c
//    TEST_ADD(TestOtaDefault::test_calc_crc32);

}

TestOtaDefault::~TestOtaDefault()
{
}

//protected:
void TestOtaDefault::setup()
{
    std::cout << "setup";
}

void TestOtaDefault::tear_down()
{
    std::cout << "test_down";
}




