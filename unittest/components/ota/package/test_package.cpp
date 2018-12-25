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

#include "test_package.h"
#include "package.h"
#include "package_head.h"
#include "package_writer.h"
#include "package_device.h"
#include "cpptest.h"

#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
#include "package_sha256_rsa2048.h"
#elif (PACK_CHECKSUM == PACK_SHA256)
#include "package_sha256.h"
#endif

#define ATINY_FOTA_DOWNLOAD_FAIL 1
#define ATINY_FOTA_DOWNLOAD_OK 0

static int funcno;

extern "C"{

	#include "adapter_layer.h"
	void *atiny_malloc(size_t size);
	void atiny_free(void *ptr);	
	typedef struct pack_checksum_tag_s pack_checksum_s;
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
	static const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	static const char *rsa_E = "10001";
	static ota_key_s v_key = {rsa_N,rsa_E};
	#if 0
	typedef struct
    {
		pack_storage_device_api_s interface;
		pack_hardware_s hardware;
		pack_params_s params;
		pack_head_s head;
		pack_writer_s writer;
		uint32_t total_len;
		ota_flash_type_e type;
		int32_t init_flag;
    } pack_storage_device_s;
	#endif
	static void ___reset(struct pack_checksum_alg_tag_s *thi){}
	static int ___update(struct pack_checksum_alg_tag_s *thi, const uint8_t *buff, uint16_t len){
		return 0;
	}
	static int ___check(struct pack_checksum_alg_tag_s *thi, const uint8_t  *checksum, uint16_t checksum_len){return 0;}
	static void ___destroy(struct pack_checksum_alg_tag_s *thi){}
	
	static int ___read_software(struct pack_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software_fault(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return -1;}
	static void ___set_flash_type(struct pack_hardware_tag_s *thi, ota_flash_type_e type){}
	static uint32_t ___get_block_size(struct pack_hardware_tag_s *thi){return 128;}
	static uint32_t ___get_block_size_fault(struct pack_hardware_tag_s *thi){return 0;}
	static uint32_t ___get_max_size(struct pack_hardware_tag_s *thi){return 0;}
	static int ___head_update_check(const uint8_t *head_buff , uint16_t len, void *param){return 0;}
	
	static pack_head_s v_head;
	static pack_hardware_s v_hardware;
	
	void test_get_local_hardware(pack_hardware_s* p_hardware){
		p_hardware->read_software = ___read_software;
		p_hardware->write_software = ___write_software;
		p_hardware->set_flash_type = ___set_flash_type;
		p_hardware->get_block_size = ___get_block_size;
		p_hardware->get_max_size = ___get_max_size;
	}
	void test_get_local_head(pack_head_s * p_head ,pack_hardware_s * p_hw,const char* buff=NULL,const char *checksum_pos=NULL,pack_checksum_s* p_cksum = NULL){
		p_head->hardware = p_hw;
		p_head->update_check = ___head_update_check;
		p_head->key = v_key;
		p_head->param = NULL;
		p_head->buff = buff;
		p_head->stored_len = 8;
		p_head->head_len = 16;
		p_head->checksum = p_cksum;
		p_head->checksum_pos = checksum_pos;
		p_head->checksum_len = 16;
	}
	
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
		#define CONDITION_PARAM pack_sha256_rsa2048_s*
	#elif (PACK_CHECKSUM == PACK_SHA256)
		#define CONDITION_PARAM pack_sha256_s*
	#endif
	
	void test_get_local_alg(CONDITION_PARAM p_alg,pack_checksum_alg_s* p_base,mbedtls_sha256_context * p_context,pack_sha256_s* p_256,pack_head_s * p_head=NULL){
			p_base->reset = ___reset;
			p_base->update = ___update;
			p_base->check = ___check;
			p_base->destroy = ___destroy;
			
			memset( p_context, 0, sizeof( mbedtls_sha256_context ) );
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
				p_256->base = *p_base;
				p_256->sha256_context = *p_context;
			p_alg->sha256 = *p_256;
			p_alg->head = p_head;
	#elif (PACK_CHECKSUM == PACK_SHA256)
			p_alg->base = *p_base;
			p_alg->sha256_context = *p_context
	#endif
	}
	void test_get_local_writer(pack_writer_s* p_writer,pack_hardware_s* p_hardware=NULL){
		p_writer->offset = 2;
		p_writer->offset_flag = 1;
		p_writer->buffer = NULL;
		p_writer->block_size = 128;
		p_writer->buffer_stored_len = 10;
		p_writer->hardware = p_hardware;
	}
	static pack_writer_s v_writer;
	
	static void __test_init_writer(){
		v_writer.offset = 2;
		v_writer.offset_flag = 1;
		v_writer.buffer = NULL;
		v_writer.block_size = 128;
		v_writer.buffer_stored_len = 10;
		v_writer.hardware = &v_hardware;
	}
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
		v_head.param = NULL;
		v_head.buff = "Hello";
		v_head.stored_len = 8;
		v_head.head_len = 16;
		v_head.checksum = NULL;
		v_head.checksum_pos = "world";
		v_head.checksum_len = 16;
	}
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
		static pack_sha256_rsa2048_s v_alg;
	#elif (PACK_CHECKSUM == PACK_SHA256)
		static pack_sha256_s v_alg;
	#endif
	static pack_checksum_alg_s v_base;
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


	static int stub_flag_enable_hwpatch(const uint8_t *patch_head, int32_t patch_len){
		return 0;
	}
	
	int pack_storage_active_software(pack_storage_device_api_s *thi);
	
	static int ___test_read_flash(ota_flash_type_e type, void *buf, int32_t len, uint32_t location){
		return 0;
	}
	static int ___test_write_flash(ota_flash_type_e type, const void *buf, int32_t len, uint32_t location){
		return 0;
	}
	
	static int stub_pack_head_parse(pack_head_s *head, uint32_t offset, const uint8_t *buff,uint16_t len, uint16_t *used_len){
		return 0;
	}
	static int stub_pack_wr_write(pack_writer_s *writer, uint32_t offset, const uint8_t *buff, uint16_t len){return 0;}
	static int stub_pack_checksum_update_data(pack_checksum_s *thi, uint32_t offset, const uint8_t *buff,uint16_t len, pack_hardware_s *hardware){
		return 0;
	}
	static pack_storage_device_s for_log_device;
	static pack_params_s * stub_pack_get_params_2(){
		pack_storage_device_s *device = &for_log_device;
		device->params.printf = printf;
		return &device->params;
	}
	static int static_int_for_local_calloc;
	static int* stub_pack_malloc_for_local_calloc(){
		return &static_int_for_local_calloc; 
	}
}//extern "C"

void TestPackage::test_pack_storage_write_software_end(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_storage_device_api_s* p_api = pack_get_device();
	pack_storage_device_s s_device;
	pack_storage_device_s* p_device = &s_device;

	pack_writer_s s_writer;
	pack_head_s s_head;
	__test_init_writer();
	p_device->writer = v_writer;//
	
	__test_init_hardware();
	__test_set_head();
	p_device->head = v_head;//

	pack_storage_device_api_s a;
	pack_storage_device_api_s * p = &a;
	pack_download_result_e b = PACK_DOWNLOAD_FAIL; //select 0 once
	uint32_t total_len = 16;
	int ret = 0;
	ret = p_api->write_software_end(NULL,b,total_len);
	TEST_ASSERT(PACK_ERR == ret);
	///

	///
	p = (pack_storage_device_api_s*)p_device;
	
	ret = p_api->write_software_end(p,b,total_len);
	TEST_ASSERT(PACK_OK==ret);
	
	pack_head_s * p_l_head = atiny_malloc(sizeof(pack_head_s));
	char* buff = atiny_malloc(8*sizeof(char));
	char * rec_buff = buff;
	p_l_head->buff = buff;
	p_l_head->checksum_pos = NULL;
	p_l_head->checksum = NULL;
	b = PACK_DOWNLOAD_OK;
	
	pack_head_s * p_rec = p_l_head;
	p_device->head = *p_l_head;
	
	p = (pack_storage_device_api_s*)p_device;
	ret = p_api->write_software_end(p,b,total_len);
	TEST_ASSERT(PACK_ERR==ret);
	
	if(p_l_head==p_rec){
		atiny_free(p_l_head);
	}
	if(rec_buff==buff){
		atiny_free(buff);
	}
	cleanStub(&si_stub_log);
}

void TestPackage::test_pack_storage_write_software(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
    uint32_t len = 0;
    uint32_t offset = 0;
    pack_storage_device_api_s * interface = NULL;
    
    uint8_t buff[20] = {0,0,0,0,0,0,0,20,0,0,0,22};
    uint16_t used_len;
	
	
    
    interface = pack_get_device();
    ret = interface->write_software(NULL, offset, NULL, len);
    TEST_ASSERT(PACK_ERR == ret);

    stubInfo si_pack_head_parse;
    setStub((void *)pack_head_parse, (void *)stub_pack_head_parse, &si_pack_head_parse);
    
    len = 12;

    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(PACK_ERR == ret);

    
    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(PACK_ERR == ret);

    stubInfo si_pack_wr_write; 
    setStub((void *)pack_wr_write, (void *)stub_pack_wr_write, &si_pack_wr_write);

    ret = interface->write_software(interface, offset, NULL, len);
    TEST_ASSERT(PACK_ERR == ret);


    cleanStub(&si_pack_wr_write);
    cleanStub(&si_pack_head_parse);

	
    buff[13] = 3;
    buff[15] = 4;
    
	
	//pack_storage_device_api_s* p_writer = &v_writer;
	int len_4 = 2;
	char buf_3[8];
	
	stubInfo si_stub2;
	setStub((void*)pack_head_parse,(void*)stub_pack_head_parse,&si_stub2);
	ret = interface->write_software(interface, 0, buff, 20);
    TEST_ASSERT(PACK_ERR == ret);
	

	stubInfo si_stub3;
	setStub((void*)pack_wr_write,(void*)stub_pack_wr_write,&si_stub3);
	
	ret = interface->write_software(interface, 0, buff, 20);
    TEST_ASSERT(PACK_OK == ret);
	////
	pack_storage_device_s* p_device = (pack_storage_device_s*)interface;
	pack_checksum_s l_chsum;
	p_device->head.checksum = &l_chsum;
	stubInfo si_stub4;
	setStub((void*)pack_checksum_update_data,(void*)stub_pack_checksum_update_data,&si_stub4);
	
	
	
	ret = interface->write_software(interface, 0, buff, 20);
    TEST_ASSERT(PACK_OK == ret);
	cleanStub(&si_stub4);
	////
	cleanStub(&si_stub3);
	cleanStub(&si_stub2);
	
	
    buff[13] = 3;
    buff[15] = 4;
    ret = interface->write_software(interface, 13, buff, 20);
    TEST_ASSERT(PACK_ERR == ret);
	
	//pack_storage_device_s* p_device = NULL;
	p_device = (pack_storage_device_s*)interface;
	__test_set_head();
	p_device->head = v_head;
	ret = interface->write_software(interface, 13, buff, 20);
	TEST_ASSERT(PACK_ERR == ret);
	cleanStub(&si_stub_log);
}

void TestPackage::test_pack_storage_active_software(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_storage_device_api_s* p = NULL;
	ret  = pack_storage_active_software(p);
	TEST_ASSERT(PACK_ERR == ret);

	
	ota_flash_type_e m_type = OTA_FULL_SOFTWARE;
	pack_storage_device_s b;
	b.type = m_type;
	b.total_len = 16;
	p = (pack_storage_device_api_s*)(&b);
	
	m_type = OTA_DIFF_SOFTWARE;
	b.type = m_type;
	p = (pack_storage_device_api_s*)(&b);
	
	m_type = OTA_UPDATE_INFO;
	b.type = m_type;
	p = (pack_storage_device_api_s*)(&b);
	
	cleanStub(&si_stub_log);
}



void TestPackage::test_pack_get_device(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_storage_device_api_s* p = NULL;
	p = pack_get_device();
	TEST_ASSERT( p != NULL);
	cleanStub(&si_stub_log);
}

void* ___malloc(size_t size){
	return NULL;
}
void  ___free(void *ptr){}

void TestPackage::test_pack_read_software(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
///
	pack_params_s a;
	pack_params_s* p = &a;
	a.ota_opt.flash_block_size = 128;
	a.ota_opt.key = v_key;
	a.ota_opt.read_flash = ___test_read_flash;
	a.ota_opt.write_flash = ___test_write_flash;
	
	pack_init_device(p);

///
	pack_storage_device_api_s* p_api = pack_get_device();
	int ret = 0;
	
	pack_storage_device_s* p_device = NULL;
	p_device = (pack_storage_device_s*)p_api;
	p_device->params.ota_opt.flash_block_size  =128;
	p_device->params.ota_opt.key = v_key;
	p_device->params.ota_opt.read_flash = ___test_read_flash;
	p_device->params.ota_opt.write_flash = ___test_write_flash;


	pack_hardware_tag_s vl_hardw;
	uint32_t offset =2;
	uint8_t buffer[8] ;
	uint32_t len = 16;
	
	p_device->params.ota_opt.read_flash = ___test_read_flash;

	ret = p_device->hardware.read_software(&vl_hardw,offset,buffer,len);
	TEST_ASSERT( ret == PACK_OK);
	
	p_device->params.ota_opt.read_flash = NULL;
	ret = p_device->hardware.read_software(&vl_hardw,offset,buffer,len);
	TEST_ASSERT( ret == PACK_ERR);
	
	p_device->params.ota_opt.write_flash = ___test_write_flash;
	printf("456\n");
	p_device->params.ota_opt.flash_block_size = 128;
	
	//ret = p_device->hardware.get_block_size(NULL);
	ret = p_device->hardware.get_block_size(&vl_hardw);
	//TEST_ASSERT( ret == 128);
	
	ret = p_device->hardware.write_software(&vl_hardw,offset,buffer,len);
	//TEST_ASSERT( ret == PACK_OK);
	
	p_device->params.ota_opt.write_flash = NULL;
	ret = p_device->hardware.write_software(&vl_hardw,offset,buffer,len);
	//TEST_ASSERT( ret == PACK_ERR);
	
	p_device->hardware.set_flash_type(&vl_hardw,0);
	
	cleanStub(&si_stub_log);
	
}


void TestPackage::test_pack_init_device(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_params_s* p = NULL;
	ret = pack_init_device(p);
	TEST_ASSERT( ret == PACK_ERR);
	
	pack_params_s a;
	p = &a;
	a.ota_opt.flash_block_size = 128;
	a.ota_opt.key = v_key;
	a.ota_opt.read_flash = ___test_read_flash;
	a.ota_opt.write_flash = ___test_write_flash;
	pack_init_device(p);
	stubInfo si_pack_malloc;
	setStub((void*)pack_malloc,(void*)stub_pack_malloc_for_local_calloc,&si_pack_malloc);
	mbedtls_calloc(1,4);
	cleanStub(&si_pack_malloc);
	TEST_ASSERT( ret == PACK_ERR);
	cleanStub(&si_stub_log);
	
}
void TestPackage::test_pack_malloc(){
	pack_params_s a;
	pack_params_s* p = &a;
	a.ota_opt.flash_block_size = 128;
	a.ota_opt.key = v_key;
	a.ota_opt.read_flash = ___test_read_flash;
	a.ota_opt.write_flash = ___test_write_flash;
	a.malloc = atiny_malloc;
	pack_init_device(p);
	void* tmp_p = pack_malloc(128);
	if(tmp_p){
		atiny_free(tmp_p);
	}
}
//void TestPackage::test_pack_get_params(){}
TestPackage::TestPackage()
{ 	
	TEST_ADD(TestPackage::test_pack_storage_write_software);
	TEST_ADD(TestPackage::test_pack_storage_active_software);
	TEST_ADD(TestPackage::test_pack_get_device);
	TEST_ADD(TestPackage::test_pack_init_device);
	TEST_ADD(TestPackage::test_pack_storage_write_software_end);
	TEST_ADD(TestPackage::test_pack_read_software);
	TEST_ADD(TestPackage::test_pack_malloc);
	
	
}

TestPackage::~TestPackage()
{	
}

void TestPackage::setup()
{   
    //printf("come into test funcno %d,%s\n", ++funcno,__FILE__);	
}

void TestPackage::tear_down()
{
    //printf("exit from funcno %d,%s\n", funcno,__FILE__);
}