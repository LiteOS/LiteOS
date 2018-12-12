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
#include "fota_package_storage_device.h"

#include "stddef.h"
#include "ota_sha256.h"

static int funcno;

#if (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256_RSA2048)
#include "fota_package_sha256_rsa2048.h"
#elif (FOTA_PACK_CHECKSUM == FOTA_PACK_SHA256)
#include "fota_package_sha256.h"
#else
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
#endif

};


extern "C"
{
    #include "adapter_layer.h"
	#include "ota.h"
	#include "atiny_fota_api.h"
	
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
    extern int fota_pack_head_parse_head_len(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *used_len);
    extern int fota_pack_head_parse(fota_pack_head_s *head, uint32_t offset, const uint8_t *buff, uint16_t len, uint16_t *used_len);
    extern int fota_pack_head_check(const fota_pack_head_s *head, uint32_t len);
    //extern int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_hardware_s *hardware, head_update_check update_check, void *param);
    extern int fota_pack_head_set_head_info(fota_pack_head_s *head, fota_pack_device_info_s *device_info);
	extern fota_pack_checksum_s *fota_pack_head_get_checksum(fota_pack_head_s *head);
    extern void fota_pack_head_init(fota_pack_head_s *head);


	
	#if 1
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
		return 0;
	}
	
	
	static pack_storage_device_api_s storage_device = {test_hal_fota_write_software, test_hal_fota_write_software_end, test_hal_fota_active_software,
									   test_hal_fota_get_software_result, test_hal_fota_write_update_info, test_hal_fota_read_update_info};



	static fota_hardware_s hardware = {test_hal_fota_get_block_size, test_hal_fota_get_max_size, test_hal_fota_read_software};



	#endif
	static void* stub_atiny_malloc_fail(size_t size)
    {

        return NULL;

	    
    }
	
    static int g_last_fail_no = 0;
	static int i = 0;
	static void* stub_atiny_malloc_last_fail(size_t size)
    {
        i++;
		
        if(i > g_last_fail_no)
        {
            //printf(" i is %d, g_last_fail_no is %d, return NULL\n",i,g_last_fail_no);
            return NULL;
        }
	    else
	    {
	        //printf(" i is %d, g_last_fail_no is %d, return good\n",i,g_last_fail_no);
	        return malloc(size);
	    }
    }
	static int g_head_update_check = 0;
    static int stub_head_update_check(const uint8_t *head_buff , uint16_t len, void *param)
    {
        return g_head_update_check;
    }
	
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
    int ret = 0;
    pack_storage_device_api_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;
	uint8_t buff[16] = {0,0,0,12,0,0,0,16,0,0,0,22};
	uint16_t used_len;

    interface = fota_get_pack_device();
    device = (fota_pack_storage_device_s *)interface;
    
    device->writer.buffer_stored_len = 0;
	
#if 1	
    ret = fota_pack_head_parse_head_len(&device->head, 1, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
	
    fota_pack_head_destroy(&device->head);
	stubInfo si_atiny_malloc;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail, &si_atiny_malloc);
	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
	
	cleanStub(&si_atiny_malloc);
#endif	

	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
    buff[3] = 0;
	buff[7] = 25;
	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
#if 1
	stubInfo si_malloc_last;
	setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_last_fail, &si_malloc_last);
	buff[3] = 0;
	buff[7] = 18;
	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
	cleanStub(&si_malloc_last);
	
    fota_pack_head_destroy(&device->head);
	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 16, &used_len);
	
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse_head_len() is failed");
    
	fota_pack_head_destroy(&device->head);
	buff[7] = 18;
	ret = fota_pack_head_parse_head_len(&device->head, 0, buff, 8, &used_len);
	
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse_head_len() is failed");

	ret = fota_pack_head_parse_head_len(&device->head, 9, buff, 2, &used_len);
	
	
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse_head_len() is failed");
#endif
	

}

void TestFotaPackageHead::test_fota_pack_head_destroy()
{

    
}



void TestFotaPackageHead::test_fota_pack_head_parse()
{
    int ret = 0;
    pack_storage_device_api_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;
	uint8_t buff[20] = {0,0,0,1,0,0,0,20,0,0,0,22};
	uint16_t used_len;

    interface = fota_get_pack_device();
    device = (fota_pack_storage_device_s *)interface;
	
    fota_pack_head_destroy(&device->head);
	
    ret = fota_pack_head_parse(&device->head, 1, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

    buff[3] = 0;
	//printf("fota_pack_head_parse start here\n");
    ret = fota_pack_head_parse(&device->head, 0, buff, 6, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
    //printf("fota_pack_head_parse start over\n");

	ret = fota_pack_head_parse(&device->head, 8, buff, 2, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");


    fota_pack_head_destroy(&device->head);
	ret = fota_pack_head_parse(&device->head, 17, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");

	buff[13]=3;/*FOTA_PACK_TLV_T_SHA256_RSA2048*/
	ret = fota_pack_head_parse(&device->head, 0, buff, 16, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");


	fota_pack_head_destroy(&device->head);
	
	buff[15]=4;/*>0*/
	buff[16]=1;
	buff[17]=2;
	buff[18]=3;
	buff[19]=5;
	device->head.checksum_pos = (uint8_t *)atiny_malloc(4);
	memset(device->head.checksum_pos,0x5A,4);
	device->head.checksum = fota_pack_checksum_create(&device->head);
    ret = fota_pack_head_parse(&device->head, 0, buff, 20, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
	
	fota_pack_head_destroy(&device->head);
	//printf("hello this\n");
    buff[7]=20;
	buff[15]=4;/*>0*/
	buff[16]=1;
	buff[17]=2;
	buff[18]=3;
	buff[19]=5;
	ret = fota_pack_head_parse(&device->head, 0, buff, 20, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
	buff[7]=20;
	//printf("hello done this\n");
    fota_pack_head_destroy(&device->head);
	
	buff[13]=2;/*FOTA_PACK_TLV_T_SHA256_RSA2048*/
	//printf("hello this\n");
    buff[7]=20;
	buff[15]=4;/*>0*/
	buff[16]=1;
	buff[17]=2;
	buff[18]=3;
	buff[19]=5;
	ret = fota_pack_head_parse(&device->head, 0, buff, 20, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_ERR), "fota_pack_head_parse() is failed");
	buff[7]=20;
	//printf("hello done this\n");
	
    stubInfo si_head_len;
    setStub((void *)fota_pack_head_parse_head_len, (void *)si_fota_pack_head_parse_head_len, &si_head_len);

    cleanStub(&si_head_len);

	
    buff[7] = 12;
	ret = fota_pack_head_parse(&device->head, 0, buff, 12, &used_len);
    TEST_ASSERT_MSG((ret == FOTA_OK), "fota_pack_head_parse() is failed");
    
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
    (void)fota_set_pack_device(fota_get_pack_device(), &device_info);
	
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_parse_head_len);
    
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_parse);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_check);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_set_head_info);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_get_checksum);
    TEST_ADD(TestFotaPackageHead::test_fota_pack_head_init);
	//TEST_ADD(TestFotaPackageHead::test_fota_pack_head_destroy);
}

TestFotaPackageHead::~TestFotaPackageHead()
{  
    pack_storage_device_api_s * interface = NULL;
    fota_pack_storage_device_s *device = NULL;

    interface = fota_get_pack_device();
    device = (fota_pack_storage_device_s *)interface;
    fota_pack_head_destroy(&device->head);
	fota_fmw_wr_destroy(&device->writer);
}


void TestFotaPackageHead::setup()
{
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestFotaPackageHead::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}




