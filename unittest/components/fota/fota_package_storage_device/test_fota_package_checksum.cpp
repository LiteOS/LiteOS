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
#include "fota_package_storage_device.h"
#include <string.h>
#include "fota_package_head.h"
#include "ota_sha256.h"

static int funcno;
#if (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256_RSA2048)
#include "fota_package_sha256_rsa2048.h"
#elif (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256)
#include "fota_package_sha256.h"
#endif


struct fota_pack_checksum_tag_s
{
    uint32_t offset;
    bool offset_flag;
    fota_pack_head_s *head;
#if (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256_RSA2048)

    fota_pack_sha256_rsa2048_s alg;
#elif (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256)
    fota_pack_sha256_s alg;
#else
#endif

};




extern "C"
{
    #include "adapter_layer.h"
    #include "ota.h"
	#include "atiny_fota_api.h"
	#include "fota_firmware_writer.h"
    typedef struct
    {
    pack_storage_device_api_s interface;
    pack_storage_device_api_s *storage_device;
    fota_hardware_s *hardware;

    fota_pack_head_s head;
    fota_firmware_writer_s writer;
    fota_pack_checksum_s *checksum;
    uint32_t total_len;
    int init_flag;
    }fota_pack_storage_device_s;
	static int g_result = 0;
	static int test_hal_fota_write_software(pack_storage_device_api_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
    {
			return 0;
    }

		
		static int test_hal_fota_active_software(pack_storage_device_api_s *thi)
		{
			return 0;
		}
		
		static int test_hal_fota_get_software_result(pack_storage_device_api_s *thi)
		{
			return 0;
		}
		
		static int test_hal_fota_write_update_info(pack_storage_device_api_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len)
		{
			return 0;
		}
		
		static int test_hal_fota_read_update_info(pack_storage_device_api_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len)
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

			return g_result;
		}
		
		
		static pack_storage_device_api_s storage_device = {test_hal_fota_write_software, test_hal_fota_write_software_end, test_hal_fota_active_software,
										   test_hal_fota_get_software_result, test_hal_fota_write_update_info, test_hal_fota_read_update_info};
	
	
	
		static fota_hardware_s hardware = {test_hal_fota_get_block_size, test_hal_fota_get_max_size, test_hal_fota_read_software};
	
	
    static void* stub_atiny_malloc_fail(size_t size)
    {

        return NULL;

	    
    }

	
    
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_create()
{

    ota_assist assist;
	assist.func_printf = printf;
    assist.func_ota_read = hal_spi_flash_read;
    assist.func_ota_write = hal_spi_flash_erase_write;
	printf("come into test_fota_pack_checksum_create\n");
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
    (void)fota_set_pack_device(fota_get_pack_device(), &device_info);
	
    fota_pack_checksum_s * thi;
    fota_pack_head_s head;
    memset(&head,0,sizeof(fota_pack_head_s));

	stubInfo si_atiny_malloc;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail, &si_atiny_malloc);
	thi = fota_pack_checksum_create(&head);
    TEST_ASSERT_MSG((thi == NULL), "fota_pack_head_parse_head_len() is failed");
	cleanStub(&si_atiny_malloc);

    thi = fota_pack_checksum_create(&head);
    TEST_ASSERT_MSG((thi != NULL), "fota_pack_checksum_create() is failed");
	fota_pack_checksum_delete(NULL);
    fota_pack_checksum_delete(thi);
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_delete()
{
    /*test in test_fota_pack_checksum_create*/
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_update_head()
{
    /*source code not exist*/
}

void TestFotaPackageCheckSum::test_fota_pack_checksum_update_data()
{
    pack_storage_device_api_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;
	uint8_t buff[16] = {0,0,0,12,0,0,0,16,0,0,0,22};
	uint16_t used_len;

    interface = fota_get_pack_device();
    device = (fota_pack_storage_device_s *)interface;

    int ret = fota_pack_checksum_update_data(NULL, 0, NULL, 0, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");

    fota_pack_checksum_s * thi = fota_pack_checksum_create(&device->head);
	
    ret = fota_pack_checksum_update_data(thi, 0, NULL, 0, NULL);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");

    ret = fota_pack_checksum_update_data(thi, 0, NULL, 12, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");

    
    g_result = 1;
    thi->offset_flag = true;
    thi->offset = 12;
    ret = fota_pack_checksum_update_data(thi, 12, buff, 12, NULL);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");

	g_result = 0;
	
	ret = fota_pack_checksum_update_data(thi, 0, buff, 12, device->hardware);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");
    
    thi->offset_flag = false;

	thi->head->head_len = 12 - 1;/*#define FOTA_PACK_HEADER_MIN_LEN 12  */
	ret = fota_pack_checksum_update_data(thi, 2, buff, 12, NULL);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");
	
	
    ret = fota_pack_checksum_update_data(thi, 2, buff, 12, device->hardware);
    TEST_ASSERT_MSG((FOTA_OK == ret), "fota_pack_checksum_update_data() is failed");
    
    
    free(thi->head->buff);
	thi->head->buff = NULL;
	thi->head->head_len = 12;
	thi->head->stored_len = 12;
	ret = fota_pack_checksum_update_data(thi, 2, buff, 12, device->hardware);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");
		
    

	printf("start here test_fota_pack_checksum_update_data,%p,%p!!!\n",device->hardware,&hardware);
	g_result = 1;
	thi->head->head_len = 12;
	thi->head->stored_len = 12;
	thi->head->buff = (uint8_t *)malloc(12);
    ret = fota_pack_checksum_update_data(thi, 2, buff, 12, device->hardware);
	printf("+++++ret is %d in fota_pack_checksum_update_data\n",ret);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_update_data() is failed");
    g_result = 0;
    printf("over here test_fota_pack_checksum_update_data!!!\n");
    fota_pack_checksum_delete(thi);
	
}    

void TestFotaPackageCheckSum::test_fota_pack_checksum_check()
{
    fota_pack_checksum_s * thi = NULL;
    fota_pack_head_s head;
    memset(&head,0,sizeof(fota_pack_head_s));
	
    int ret = fota_pack_checksum_check(thi, NULL, 0);
    TEST_ASSERT_MSG((FOTA_ERR == ret), "fota_pack_checksum_check() is failed");

    thi = fota_pack_checksum_create(&head);
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

TestFotaPackageCheckSum::~TestFotaPackageCheckSum()
{  
}


void TestFotaPackageCheckSum::setup()
{
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestFotaPackageCheckSum::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}


