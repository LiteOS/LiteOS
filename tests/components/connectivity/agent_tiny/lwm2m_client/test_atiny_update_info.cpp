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

/* Define to prevent recursive inclusion ------------------------------------*/
#include <fstream>
#include <memory>
#include "agenttiny.h"
#include "adapter_layer.h"
#include "atiny_log.h"
#include "test_atiny_update_info.h"
extern "C"{  

    struct atiny_update_info_tag_s
    {
        atiny_fota_storage_device_s *device;
    };

    int write_update_info_test(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
    {
        return 0;
    }
    int read_update_info_test(atiny_fota_storage_device_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)   
    {
        return 0;
    }
}

void TestAtinyUpdateInfo::test_atiny_update_info_set()
{
    atiny_update_info_s *thi = NULL;
    atiny_fota_storage_device_s *device = NULL;
    int result = 0;
    
    result = atiny_update_info_set(thi, device);
    TEST_ASSERT(result == -1);

    thi = (atiny_update_info_s *)malloc(sizeof(atiny_fota_storage_device_s));
    TEST_ASSERT(thi != NULL);
    device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    TEST_ASSERT(device != NULL);
    result = atiny_update_info_set(thi, device);
    TEST_ASSERT(result == 0);

    free(device);
    free(thi);
}

void TestAtinyUpdateInfo::test_atiny_update_info_write()
{   
    atiny_update_info_s thi;
    atiny_update_info_e type = TOCKEN_INFO;
    uint8_t *info = NULL;
    uint32_t len = 0;
    int result = 0;

    result = atiny_update_info_write(NULL, type, info, len);
    TEST_ASSERT(result == -1);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    thi.device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    TEST_ASSERT(thi.device != NULL);
    thi.device->write_update_info = write_update_info_test;
    info = (uint8_t*)"6789";
    
    result = atiny_update_info_write(&thi, type, info, len);
    TEST_ASSERT(result == 0);

    type = FW_DOWNLOAD_INFO;
    result = atiny_update_info_write(&thi, type, info, len);
    TEST_ASSERT(result == 0);

    free(thi.device);
    
}

void TestAtinyUpdateInfo::test_atiny_update_info_read()
{
    atiny_update_info_s thi;
    atiny_update_info_e type = TOCKEN_INFO;
    uint8_t *info = NULL;
    uint32_t len = 0;
    int result = 0;

    result = atiny_update_info_read(NULL, type, info, len);

    thi.device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    TEST_ASSERT(thi.device != NULL);
    thi.device->read_update_info = read_update_info_test;
    info = (uint8_t*)"6789";
    
    result = atiny_update_info_read(&thi, type, info, len);
    TEST_ASSERT(result == 0);

    type = FW_DOWNLOAD_INFO;
    result = atiny_update_info_read(&thi, type, info, len);
    TEST_ASSERT(result == 0);

    free(thi.device);    
}

void TestAtinyUpdateInfo::test_atiny_update_info_get_instance()
{
    atiny_update_info_s * info = NULL;

    info = atiny_update_info_get_instance();
    TEST_ASSERT(info != NULL);
}



TestAtinyUpdateInfo::TestAtinyUpdateInfo()
{
    TEST_ADD(TestAtinyUpdateInfo::test_atiny_update_info_set);
    TEST_ADD(TestAtinyUpdateInfo::test_atiny_update_info_write);
    TEST_ADD(TestAtinyUpdateInfo::test_atiny_update_info_read);
    TEST_ADD(TestAtinyUpdateInfo::test_atiny_update_info_get_instance);

}

void TestAtinyUpdateInfo::setup()
{
    std::cout << "in steup\n";
}

void TestAtinyUpdateInfo::tear_down()
{
    std::cout << "in teardown\n";
}




