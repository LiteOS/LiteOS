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
#include "ota.h"
static int funcno;
extern "C"{

#include "adapter_layer.h"

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


static int test_write_software(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    return 0;
}


static int test_fota_pack_head_check(const fota_pack_head_s *head, uint32_t len)
{
    return FOTA_ERR;
}

static int test_fota_fmw_wr_write(fota_firmware_writer_s *writer, uint32_t offset, const uint8_t *buff, uint16_t len)
{
    return FOTA_OK;
}

static int flag_fota_pack_head_parse;
static int test_fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff,
                                 uint16_t len, uint16_t *used_len)
{   
    if (flag_fota_pack_head_parse == 0)
    {
        return FOTA_OK;
        used_len = (uint16_t *)12;
    }
    else
        return FOTA_ERR;
}


static int test_hal_fota_write_software(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    return 0;
}
static int  test_hal_fota_write_software_end(atiny_fota_storage_device_s *thi, atiny_download_result_e result, uint32_t total_len)
{
    return 0;
}

static int test_hal_fota_active_software(atiny_fota_storage_device_s *thi)
{
    return 0;
}

static int test_hal_fota_get_software_result(atiny_fota_storage_device_s *thi)
{
    return 0;
}

static int test_hal_fota_write_update_info(atiny_fota_storage_device_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
{
    return 0;
}

static int test_hal_fota_read_update_info(atiny_fota_storage_device_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    return 0;
}

static uint32_t test_hal_fota_get_block_size(struct fota_hardware_api_tag_s *thi, uint32_t offset)
{
    return 0x1000;/*4096*/
}

static uint32_t test_hal_fota_get_max_size(struct fota_hardware_api_tag_s *thi)
{
    return 0x00040000;
}

static int test_hal_fota_read_software(struct fota_hardware_api_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)
{
    return 0;
}

static atiny_fota_storage_device_s storage_device = {test_hal_fota_write_software, test_hal_fota_write_software_end, test_hal_fota_active_software,
                                   test_hal_fota_get_software_result, test_hal_fota_write_update_info, test_hal_fota_read_update_info};
static fota_hardware_s hardware = {test_hal_fota_get_block_size, test_hal_fota_get_max_size, test_hal_fota_read_software};

}

void TestFotaPackageStorageDivice::test_fota_set_pack_device()
{
    int ret = 0;
    ota_assist assist;
	assist.func_printf = printf;
    assist.func_ota_read = hal_spi_flash_read;
    assist.func_ota_write = hal_spi_flash_erase_write;
    ota_register_assist(&assist);

	ota_init();

	fota_pack_device_info_s device_info;
	
    const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
    const char *rsa_E = "10001";

    device_info.hardware = &hardware;
    device_info.storage_device = &storage_device;
    device_info.head_info_notify = NULL;
    device_info.key.rsa_N = rsa_N;
    device_info.key.rsa_E = rsa_E;

    ret = fota_set_pack_device(NULL,NULL);
	TEST_ASSERT(FOTA_ERR == ret);
	
    ret = fota_set_pack_device(fota_get_pack_device(), &device_info);
    TEST_ASSERT(FOTA_OK == ret);
	

  
    
}

void TestFotaPackageStorageDivice::test_fota_pack_storage_write_software_end()
{
    int ret = 0;
    atiny_fota_storage_device_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;

    interface = fota_get_pack_device();
    ret = interface->write_software_end(NULL, ATINY_FOTA_DOWNLOAD_OK, 0);
    TEST_ASSERT(FOTA_ERR == ret);

#if 1

    device = (fota_pack_storage_device_s *)interface;
    
    device->writer.buffer_stored_len = 0;
    ret = interface->write_software_end(interface, ATINY_FOTA_DOWNLOAD_FAIL, 0);
    TEST_ASSERT(FOTA_OK == ret);

    stubInfo si_fota_pack_head_check;
    setStub((void *)fota_pack_head_check, (void *)test_fota_pack_head_check, &si_fota_pack_head_check);
    
    ret = interface->write_software_end(interface, ATINY_FOTA_DOWNLOAD_OK, 0);
    TEST_ASSERT(FOTA_ERR == ret);

    cleanStub(&si_fota_pack_head_check);
#endif
}

void TestFotaPackageStorageDivice::test_fota_pack_storage_write_software()
{
    int ret = 0;
    uint32_t len = 0;
    uint32_t offset = 0;
    atiny_fota_storage_device_s * interface = NULL;
    
    interface = fota_get_pack_device();
    ret = interface->write_software(NULL, offset, NULL, len);
    TEST_ASSERT(FOTA_ERR == ret);
#if 1
    stubInfo si_fota_pack_head_parse;
    setStub((void *)fota_pack_head_parse, (void *)test_fota_pack_head_parse, &si_fota_pack_head_parse);
    
    len = 12;

    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(FOTA_ERR == ret);

    
    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(FOTA_ERR == ret);

    stubInfo si_fota_fmw_wr_write; 
    setStub((void *)fota_fmw_wr_write, (void *)test_fota_fmw_wr_write, &si_fota_fmw_wr_write);

    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(FOTA_ERR == ret);


    cleanStub(&si_fota_fmw_wr_write);
    cleanStub(&si_fota_pack_head_parse);
#endif
}

void TestFotaPackageStorageDivice::test_fota_pack_storage_active_software()
{
    atiny_fota_storage_device_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;
    interface = fota_get_pack_device();
    device = (fota_pack_storage_device_s *)interface;
    interface->active_software(interface);
    interface->get_software_result(interface);
    interface->read_update_info(interface, 0, NULL, 0);
    interface->write_update_info(interface, 0, NULL, 0);
}

TestFotaPackageStorageDivice::TestFotaPackageStorageDivice()
{ 
    
	
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_set_pack_device);
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_pack_storage_write_software_end);
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_pack_storage_write_software);
    TEST_ADD(TestFotaPackageStorageDivice::test_fota_pack_storage_active_software);
}

TestFotaPackageStorageDivice::~TestFotaPackageStorageDivice()
{
}

void TestFotaPackageStorageDivice::setup()
{
    
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
    
	
}

void TestFotaPackageStorageDivice::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}

