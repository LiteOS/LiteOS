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

#include "test_fota_package_storage_device.h"
#include "fota_package_storage_device.h"
#include "fota_package_head.h"
#include "fota_firmware_writer.h"

typedef struct
{
    atiny_fota_storage_device_s interface;
    atiny_fota_storage_device_s *storage_device;
    fota_hardware_s *hardware;

    fota_pack_head_s head;
    fota_firmware_writer_s writer;
    fota_pack_checksum_s *checksum;
    uint32_t total_len;
    int init_flag;
}fota_pack_storage_device_s;

extern int fota_set_pack_device(atiny_fota_storage_device_s *device,  fota_pack_device_info_s *device_info);

static int test_write_software_end(atiny_fota_storage_device_s *thi, atiny_download_result_e result, uint32_t total_len)
{
    return FOTA_OK;
}

void TestFotaPackageStorageDivice::test_fota_set_pack_device()
{
  
    atiny_fota_storage_device_s * device = NULL;
    fota_pack_device_info_s * device_info = NULL;
    int ret = 0;
    
    device = fota_get_pack_device();
    ret = fota_set_pack_device(device, device_info);

    device_info = (fota_pack_device_info_s *)malloc(sizeof(fota_pack_device_info_s));
    memset(device_info, 0 ,sizeof(fota_pack_device_info_s));
    ret = fota_set_pack_device(device, device_info);
    TEST_ASSERT_MSG(FOTA_ERR == ret, "fota_set_pack_device() is failed");

    device_info->hardware = (fota_hardware_s *)malloc(sizeof(fota_hardware_s));
    memset(device_info->hardware, 0, sizeof(fota_hardware_s));
    device_info->storage_device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    memset(device_info->storage_device, 0, sizeof(atiny_fota_storage_device_s));
    ret = fota_set_pack_device(device, device_info);
    TEST_ASSERT_MSG(FOTA_OK == ret, "fota_set_pack_device() is failed");

    free(device_info->hardware);
    free(device_info->storage_device);
    free(device_info);
}

void TestFotaPackageStorageDivice::test_fota_pack_storage_write_software_end()
{
    int ret = 0;
    atiny_fota_storage_device_s * interface = NULL;
    interface = fota_get_pack_device();
    interface->write_software_end(NULL, ATINY_FOTA_DOWNLOAD_OK, 0);
    TEST_ASSERT(0 == ret);

    fota_pack_storage_device_s *device = (fota_pack_storage_device_s *)malloc(sizeof(fota_pack_storage_device_s));
    memset(device, 0, sizeof(fota_pack_storage_device_s));
    device->storage_device = (atiny_fota_storage_device_s *)malloc(sizeof(atiny_fota_storage_device_s));
    memset(device->storage_device, 0, sizeof(atiny_fota_storage_device_s));
    device->storage_device->write_software_end = test_write_software_end;
    device->writer.buffer_stored_len = 0;

    interface->write_software_end(device->storage_device, ATINY_FOTA_DOWNLOAD_FAIL, 0);

    free(device->storage_device);
    free(device);
}

TestFotaPackageStorageDivice::TestFotaPackageStorageDivice()
{
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_set_pack_device);
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_pack_storage_write_software_end);
}


