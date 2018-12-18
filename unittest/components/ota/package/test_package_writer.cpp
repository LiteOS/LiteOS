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
#include "test_package_writer.h"
#include "package_writer.h"
#include "package_head.h"
#include "package_device.h"

#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
#include "package_sha256_rsa2048.h"
#elif (PACK_CHECKSUM == PACK_SHA256)
#include "package_sha256.h"
#endif

static int funcno;

extern "C"
{
	static const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	static const char *rsa_E = "10001";
	static ota_key_s v_key = {rsa_N,rsa_E};

	static pack_head_s v_head;
	static pack_hardware_s v_hardware;
	static void pack_wr_free_save_buffer(pack_writer_s *writer);
	static int pack_wr_check(pack_writer_s *writer);
	static int pack_wr_write_stored_data(pack_writer_s *writer);
	
	static void ___reset(struct pack_checksum_alg_tag_s *thi){}
	static int ___update(struct pack_checksum_alg_tag_s *thi, const uint8_t *buff, uint16_t len){
		return 0;
	}
	static int ___check(struct pack_checksum_alg_tag_s *thi, const uint8_t  *checksum, uint16_t checksum_len){return 0;}
	static void ___destroy(struct pack_checksum_alg_tag_s *thi){}
	
	static int ___read_software(struct pack_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 1;}
	static int ___write_softwareTrue(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_softwareFault(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software_fault(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return -1;}
	static void ___set_flash_type(struct pack_hardware_tag_s *thi, ota_flash_type_e type){}
	static uint32_t ___get_block_size(struct pack_hardware_tag_s *thi){return 4096;}
	static uint32_t ___get_block_size_fault(struct pack_hardware_tag_s *thi){return 0;}
	static uint32_t ___get_max_size(struct pack_hardware_tag_s *thi){return 0;}
	static int ___head_update_check(const uint8_t *head_buff , uint16_t len, void *param){return 0;}
	

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
	static int stub_pack_wr_write_stored_data(pack_writer_s *writer){return PACK_OK;}
	static pack_storage_device_s for_log_device;
	static pack_params_s * stub_pack_get_params_2(){
		pack_storage_device_s *device = &for_log_device;
		device->params.printf = printf;
		return &device->params;
	}
	void *atiny_malloc(size_t size);
	void atiny_free(void *ptr);
}// extern "C"



TestPackageWrite::TestPackageWrite()
{
    TEST_ADD(TestPackageWrite::test_pack_wr_init);
    TEST_ADD(TestPackageWrite::test_pack_wr_free_save_buffer);
    TEST_ADD(TestPackageWrite::test_pack_wr_destroy);
    TEST_ADD(TestPackageWrite::test_pack_wr_set_device);
    TEST_ADD(TestPackageWrite::test_pack_wr_check);
    TEST_ADD(TestPackageWrite::test_pack_wr_write_stored_data);
    TEST_ADD(TestPackageWrite::test_pack_wr_write);
    TEST_ADD(TestPackageWrite::test_pack_wr_write_end);
}
void TestPackageWrite::test_pack_wr_init(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_writer_s a;
	pack_wr_init(&a);
	cleanStub(&si_stub_log);
}
void TestPackageWrite::test_pack_wr_free_save_buffer(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_writer_s a;
	a.buffer = NULL;
	pack_wr_free_save_buffer(&a);
	char * tmp = atiny_malloc(4*sizeof(char));
	a.buffer = tmp;
	pack_wr_free_save_buffer(&a);
	if(tmp!=NULL){
		atiny_free(tmp);
		tmp = NULL;
	}
	cleanStub(&si_stub_log);
}
void TestPackageWrite::test_pack_wr_destroy(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_writer_s a;
	a.buffer = NULL;
	pack_wr_destroy(&a);
	cleanStub(&si_stub_log);
}
void TestPackageWrite::test_pack_wr_set_device(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_writer_s a;
	a.buffer = NULL;
	a.hardware = NULL;
	pack_hardware_s b;
	
	pack_wr_set_device(&a,&b);
	cleanStub(&si_stub_log);
}
void TestPackageWrite::test_pack_wr_check(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_writer_s a;
	a.hardware = NULL;
	ret = pack_wr_check(&a);
	TEST_ASSERT(ret == PACK_ERR);
	pack_hardware_s b;
	a.hardware = &b;
	ret = pack_wr_check(&a);
	TEST_ASSERT(ret == PACK_OK);
	cleanStub(&si_stub_log);
}

void TestPackageWrite::test_pack_wr_write_stored_data(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_writer_s v_pws;

	char* p_char = atiny_malloc(8*sizeof(char));
	v_pws.offset = 0;
	v_pws.buffer = p_char;
	v_pws.buffer_stored_len = 10;
	__test_init_hardware();
	v_pws.hardware = &v_hardware;
	ret = pack_wr_write_stored_data(&v_pws);
	TEST_ASSERT(ret == PACK_ERR);
	
	v_pws.hardware->write_software = ___write_software_fault;
	ret = pack_wr_write_stored_data(&v_pws);
	TEST_ASSERT(ret == PACK_ERR);
	///
	v_pws.offset = 2;
	v_pws.hardware->write_software = ___write_softwareTrue;
	ret = pack_wr_write_stored_data(&v_pws);
	TEST_ASSERT(ret == PACK_OK);
	///
	if(p_char!=NULL){
		atiny_free(p_char);
		p_char = NULL;
	}
	cleanStub(&si_stub_log);
}
void TestPackageWrite::test_pack_wr_write(){
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	__test_init_hardware();
	int ret = 0;
	stubInfo si_si1;
	setStub((void*)pack_wr_write_stored_data,(void*)stub_pack_wr_write_stored_data,&si_si1);
	
	pack_writer_s v_pwr;
	uint32_t offset = 4000;
	
	char* p_char = atiny_malloc(8*sizeof(char));
	const uint8_t *buff = p_char;
	
	uint16_t len = 16;
	
	
	v_pwr.hardware = &v_hardware;
	v_pwr.offset_flag = 0;
	v_pwr.buffer = NULL;
	printf("1\n");
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_OK == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	v_pwr.buffer = p_char;
	//
	printf("2\n");
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_OK == ret);
	v_pwr.buffer = NULL;
	v_pwr.hardware->get_block_size = ___get_block_size_fault;
	//offset = 8;
	printf("3\n");
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_ERR == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	v_pwr.hardware->get_block_size = ___get_block_size;
	offset = 4096;
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_OK == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	ret = pack_wr_write(&v_pwr,offset,buff,0);
	TEST_ASSERT(PACK_OK == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	len = offset*3;
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_ERR == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	
	v_pwr.offset_flag = 0;
	offset = 4096*2+4000;
	v_pwr.buffer_stored_len = 5000;
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_ERR == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	///
	v_pwr.hardware->write_software = ___write_softwareFault;
	v_pwr.offset_flag = 0;
	offset = 4096*2+4000;
	v_pwr.buffer_stored_len = 5000;
	ret = pack_wr_write(&v_pwr,offset,buff,len);
	TEST_ASSERT(PACK_OK == ret);
	if(v_pwr.buffer!=NULL){
		atiny_free(v_pwr.buffer);
		v_pwr.buffer = NULL;
	}
	
	
	
	///
	cleanStub(&si_si1);
	if(p_char!=NULL){
		atiny_free(p_char);
	}
	cleanStub(&si_stub_log);
	cleanStub(&si_stub_malloc);
}
void TestPackageWrite::test_pack_wr_write_end(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	pack_writer_s a;
	a.buffer_stored_len = 0;
	
	ret = pack_wr_write_end(&a);
	TEST_ASSERT(PACK_OK == ret);
	cleanStub(&si_stub_log);
}



TestPackageWrite::~TestPackageWrite()
{  
}

void TestPackageWrite::setup()
{
    //printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestPackageWrite::tear_down()
{
    //printf("exit from funcno %d,%s\n", funcno,__FILE__);
}


