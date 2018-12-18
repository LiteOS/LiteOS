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

#include "test_package_checksum.h"
#include "package_checksum.h"
#include "package.h"
#include <string.h>
#include "package_head.h"
#include "package_sha256.h"
#include "package_device.h"

#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
#include "package_sha256_rsa2048.h"
#elif (PACK_CHECKSUM == PACK_SHA256)
#include "package_sha256.h"
#else
#endif

static int funcno;

extern "C"
{
    #include "adapter_layer.h"
    #include "package.h"
	#include "package_writer.h"
	void *atiny_malloc(size_t size);
	void atiny_free(void *ptr);
	static void ___reset(struct pack_checksum_alg_tag_s *thi){
	}
	static int ___update(struct pack_checksum_alg_tag_s *thi, const uint8_t *buff, uint16_t len){
		return PACK_OK;
	}
	static int ___updateFalse(){
		return PACK_ERR;
	}
	static int ___check(struct pack_checksum_alg_tag_s *thi, const uint8_t  *checksum, uint16_t checksum_len){return 0;}
	static void ___destroy(struct pack_checksum_alg_tag_s *thi){}
	
	struct pack_checksum_tag_s
	{
		uint32_t offset;
		bool offset_flag;
		pack_head_s *head;
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
		pack_sha256_rsa2048_s alg;
	#elif (PACK_CHECKSUM == PACK_SHA256)
		pack_sha256_s alg;
	#endif
	};
	typedef struct pack_checksum_tag_s pack_checksum_s;

	static int ___read_software(struct pack_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 0;}
	static void ___set_flash_type(struct pack_hardware_tag_s *thi, ota_flash_type_e type){}
	static uint32_t ___get_block_size(struct pack_hardware_tag_s *thi){return 0;}
	static uint32_t ___get_max_size(struct pack_hardware_tag_s *thi){return 0;}
	static int ___head_update_check(const uint8_t *head_buff , uint16_t len, void *param){return 0;}
	
	static pack_head_s v_head;
	static pack_hardware_s v_hardware;
	static const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	static const char *rsa_E = "10001";
	static ota_key_s v_key = {rsa_N,rsa_E};
	static void __test_init_hardware(){
		v_hardware.read_software = ___read_software;
		v_hardware.write_software = ___write_software;
		v_hardware.set_flash_type = ___set_flash_type;
		v_hardware.get_block_size = ___get_block_size;
		v_hardware.get_max_size = ___get_max_size;
	}
	static void __test_set_head(){
		v_head.hardware = &v_hardware;
		v_head.update_check = ___head_update_check;
		v_head.key = v_key;
		v_head.buff = "Hello";
		v_head.stored_len = 16;
		v_head.head_len = 16;
		v_head.checksum = NULL;
		v_head.checksum_pos = "world";
		v_head.checksum_len = 16;
	}
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
		pack_sha256_rsa2048_s v_alg;
	#elif (PACK_CHECKSUM == PACK_SHA256)
		pack_sha256_s v_alg;
	#endif
	pack_checksum_alg_s v_base;
	static void __test_set_base(){
		v_base.reset = ___reset;
		v_base.update = ___update;
		v_base.check = ___check;
		v_base.destroy = ___destroy;
	}
	static void __test_set_alg(){
		mbedtls_sha256_context v_sha256_context;// null
		memset( &v_sha256_context, 0, sizeof( mbedtls_sha256_context ) );
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
	
		pack_sha256_s v_sha256;
		v_sha256.base = v_base;
		
		v_sha256.sha256_context = v_sha256_context;
		
		v_alg.sha256 = v_sha256;
		pack_head_s * p_head = &v_head;
		v_alg.head = p_head;
	
		v_sha256.base = v_base;
	#elif (PACK_CHECKSUM == PACK_SHA256)
		v_alg.base = v_base;
		v_alg.sha256_context = v_sha256_context
	#endif
	}
	static const char* stub_pack_head_get_head_info(){
		return NULL;
	}
	static unsigned int stub_pack_head_get_head_len(const pack_head_s *head){
		return 1;
	}
	static void* stub_atiny_malloc(size_t size){return NULL;}
	static pack_storage_device_s for_log_device;
	static pack_params_s * stub_pack_get_params_2(){
		pack_storage_device_s *device = &for_log_device;
		device->params.printf = printf;
		return &device->params;
	}
}//extern "C"
void TestPackageChecksum::test_pack_checksum_delete(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	__test_init_hardware();
	__test_set_head();
	pack_checksum_s * p = NULL;
	pack_checksum_delete(NULL);
	p = atiny_malloc(sizeof(pack_checksum_s)); 
	p->offset = 2;
	p->offset_flag = 1;

	__test_set_base();
	__test_set_alg();
	p->head = &v_head;
	//p->head = NULL; same rate as above
	p->alg = v_alg; 
	pack_checksum_delete(p);
	if(p!=NULL)atiny_free(p);
	cleanStub(&si_stub_log);
}

void TestPackageChecksum::test_pack_checksum_create(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	pack_checksum_s * ret = NULL;
	stubInfo si_stub;
	setStub((void*)atiny_malloc,(void*)stub_atiny_malloc,&si_stub);
	ret = pack_checksum_create(NULL);
    TEST_ASSERT(NULL == ret);
	cleanStub(&si_stub);
	
	
	
	__test_init_hardware();
	__test_set_head();
	
	pack_head_s* p = &v_head;
	
	ret = pack_checksum_create(p);
	TEST_ASSERT(NULL != ret);
	atiny_free(ret);
	ret = NULL;
	
	
	
	p->buff = NULL;
	p->head_len = 0;
	ret = pack_checksum_create(p);
	TEST_ASSERT(NULL != ret);
	atiny_free(ret);
	ret = NULL;
	
	v_head.head_len = 10; //<12
	char buff[8];
	p->buff = buff;
	
	stubInfo stub_info;
	setStub((void*)pack_head_get_head_info,(void*)stub_pack_head_get_head_info,&stub_info);
	stubInfo stub_info2;
	setStub((void*)pack_head_get_head_len,(void*)stub_pack_head_get_head_len,&stub_info2);
	
	
	
	ret = pack_checksum_create(p);
	cleanStub(&stub_info);
	cleanStub(&stub_info2);
	TEST_ASSERT(NULL != ret);
	atiny_free(ret);
	ret = NULL;
	cleanStub(&si_stub_log);
	cleanStub(&si_stub_malloc);
}

void TestPackageChecksum::test_pack_checksum_update_data(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	int ret = 0;
	pack_checksum_s thi;
	pack_checksum_s* p_thi = &thi;
	p_thi->offset = 4;
	p_thi->offset_flag = 1;
	p_thi->head = &v_head;
	
	__test_set_head();
	
	__test_set_base();
	__test_set_alg();
	p_thi->alg = v_alg;
	
	uint32_t offset = 2;
	const uint8_t * buff = "Hello";
	uint16_t len = 16;
	__test_init_hardware();
	pack_hardware_s * p_hardw = &v_hardware;
	
	ret = pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_ERR == ret);
	
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	ret = pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_OK == ret);
	len = 0;
	ret = pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_OK == ret);
	len = 16;
	const uint8_t * buff2 = NULL;
	ret = pack_checksum_update_data(p_thi,offset,buff2,len,p_hardw);
	TEST_ASSERT(PACK_ERR == ret);
	
	p_thi->offset_flag = 0;
	p_thi->head->head_len = 0;
	pack_hardware_s * p_hardw_null = NULL;
	pack_checksum_update_data(p_thi,offset,buff,len,p_hardw_null);
	TEST_ASSERT(PACK_ERR == ret);
	p_thi->offset_flag = 0;
	p_thi->head->head_len = 0;
	pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_ERR == ret);
	p_thi->offset_flag = 1;
	p_thi->offset = offset;
#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
	p_thi->alg.sha256.base.update = ___updateFalse;
#elif (PACK_CHECKSUM == PACK_SHA256)
	p_thi->alg.base.update = ___updateFalse;
#endif
	ret = pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_ERR == ret);
	
#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
	p_thi->alg.sha256.base.update = ___update;
#elif (PACK_CHECKSUM == PACK_SHA256)
	p_thi->alg.base.update = ___update;
#endif
	ret = pack_checksum_update_data(p_thi,offset,buff,len,p_hardw);
	TEST_ASSERT(PACK_OK == ret);
	
	cleanStub(&si_stub_log);
	cleanStub(&si_stub_free);
	cleanStub(&si_stub_malloc);
	
}
void TestPackageChecksum::test_pack_checksum_check(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_checksum_s a;
	pack_checksum_s * p = &a;
	p->head = &v_head;
	p->alg = v_alg;
	
	ret  =  pack_checksum_check(p,4,4);
	TEST_ASSERT(PACK_OK == ret);
	cleanStub(&si_stub_log);
}
TestPackageChecksum::TestPackageChecksum()
{
    TEST_ADD(TestPackageChecksum::test_pack_checksum_delete);
	TEST_ADD(TestPackageChecksum::test_pack_checksum_create);
	TEST_ADD(TestPackageChecksum::test_pack_checksum_update_data);
	TEST_ADD(TestPackageChecksum::test_pack_checksum_check);
}

TestPackageChecksum::~TestPackageChecksum()
{  
}


void TestPackageChecksum::setup()
{
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestPackageChecksum::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}