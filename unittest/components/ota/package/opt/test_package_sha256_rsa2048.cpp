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

#include "test_package_sha256_rsa2048.h"

#define FOTA_PACK_SHA256_RSA2048_CHECKSUM_LEN 256
#define FOTA_PACK_SHA256_CHECKSUM_LEN 32

extern "C"
{
	void *atiny_malloc(size_t size);
	void atiny_free(void *ptr);
	static const char *rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	static const char *rsa_E = "10001";
	static ota_key_s v_key = {rsa_N,rsa_E};
	
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
	static int stub_mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s ){return 0;}
	static int stub_mbedtls_mpi_read_stringFalse( mbedtls_mpi *X, int radix, const char *s ){
		if(s[0]=='d')return 0;
		return -1;
	}

		
	static int stub_mbedtls_rsa_check_pubkey( const mbedtls_rsa_context *ctx ){return 0;}
	static int stub_mbedtls_rsa_check_pubkeyFalse( const mbedtls_rsa_context *ctx ){return -1;}
	static ota_key_s * stub_pack_head_get_key(pack_head_s *head){
		static ota_key_s tmp;
		tmp.rsa_E = NULL;
		char c[20];
		tmp.rsa_N = c;
		return &tmp;
	}
	//static void* stub_atiny_malloc(size_t size){return NULL;}
	static int stub_mbedtls_rsa_pkcs1_verify( mbedtls_rsa_context *ctx,int (*f_rng)(void *, unsigned char *, size_t),void *p_rng,int mode,
	mbedtls_md_type_t md_alg,unsigned int hashlen,const unsigned char *hash,const unsigned char *sig ){return 0;}
}// extern "C"
TestPackageSha256Rsa2048::TestPackageSha256Rsa2048()
{
	TEST_ADD(TestPackageSha256Rsa2048::test_pack_sha256_rsa2048_init);
	TEST_ADD(TestPackageSha256Rsa2048::test_pack_sha256_rsa2048_check);
	TEST_ADD(TestPackageSha256Rsa2048::test_mbedtls_rsa_check_pubkey);
	//TEST_ADD(TestPackageSha256Rsa2048::test_mbedtls_rsa_pkcs1_verify);
	
}
void TestPackageSha256Rsa2048::test_mbedtls_rsa_check_pubkey(){
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	int ret = 0;
	pack_sha256_rsa2048_s sha_rsa_a;
	pack_sha256_rsa2048_s* p_a = &sha_rsa_a;
	pack_head_s head_b ;
	pack_head_s* p_b = &head_b;
	
	pack_sha256_s v_sha256;
	
	pack_checksum_alg_s l_base;
	l_base.check = ___check;
	v_sha256.base = l_base;
	
	mbedtls_sha256_context v_sha256_context;
	v_sha256.sha256_context = v_sha256_context;
	
	p_a->sha256 = v_sha256;
	p_a->head = NULL;
	
	ret = pack_sha256_rsa2048_init(p_a,p_b);
	TEST_ASSERT(PACK_OK == ret);
	
	///
	stubInfo si_stub2;
	setStub((void*)mbedtls_mpi_read_string,(void*)stub_mbedtls_mpi_read_string,&si_stub2);
	
	stubInfo si_stub3;
	setStub((void*)mbedtls_rsa_check_pubkey,(void*)stub_mbedtls_rsa_check_pubkeyFalse,&si_stub3);
	
	stubInfo si_stub4;
	setStub((void*)mbedtls_rsa_pkcs1_verify,(void*)stub_mbedtls_rsa_pkcs1_verify,&si_stub4);
	
	pack_checksum_alg_s l_base_2;
	l_base_2.reset = ___reset;
	l_base_2.update = ___update;
	l_base_2.check = ___check;
	l_base_2.destroy = ___destroy;
	uint8_t checksum[20]; 
	uint16_t checksum_len = 10;
	
	checksum_len = 256;
	pack_sha256_rsa2048_s *test_rsa = (pack_sha256_rsa2048_s *)(&l_base_2);
		pack_head_s l_head;
		l_head.key = {NULL,NULL};
	test_rsa->head = &l_head;
	
	
		char buf_E[20] = {'c','a','t','\0'};
		char buf_N[20] = {'c','o','g','\0'};
	l_head.key = {buf_N,buf_E};
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
	

	////
	cleanStub(&si_stub4);
	cleanStub(&si_stub3);
	cleanStub(&si_stub2);
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);
}
void TestPackageSha256Rsa2048::test_mbedtls_rsa_pkcs1_verify(){
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	int ret = 0;
	
	pack_sha256_rsa2048_s sha_rsa_a;
	pack_sha256_rsa2048_s* p_a = &sha_rsa_a;
	pack_head_s head_b ;
	pack_head_s* p_b = &head_b;
	
	pack_sha256_s v_sha256;
	
	pack_checksum_alg_s l_base;
	l_base.check = ___check;
	v_sha256.base = l_base;
	
	mbedtls_sha256_context v_sha256_context;
	v_sha256.sha256_context = v_sha256_context;
	
	p_a->sha256 = v_sha256;
	p_a->head = NULL;
	
	ret = pack_sha256_rsa2048_init(p_a,p_b);
	TEST_ASSERT(PACK_OK == ret);
	
	///
	stubInfo si_stub2;
	setStub((void*)mbedtls_mpi_read_string,(void*)stub_mbedtls_mpi_read_string,&si_stub2);
	
	stubInfo si_stub3;
	setStub((void*)mbedtls_rsa_check_pubkey,(void*)stub_mbedtls_rsa_check_pubkey,&si_stub3);
	
	stubInfo si_stub4;
	setStub((void*)mbedtls_rsa_pkcs1_verify,(void*)stub_mbedtls_rsa_pkcs1_verify,&si_stub4);
	
	pack_checksum_alg_s l_base_2;
	l_base_2.reset = ___reset;
	l_base_2.update = ___update;
	l_base_2.check = ___check;
	l_base_2.destroy = ___destroy;
	uint8_t checksum[20]; 
	uint16_t checksum_len = 10;
	
	checksum_len = 256;
	pack_sha256_rsa2048_s *test_rsa = (pack_sha256_rsa2048_s *)(&l_base_2);
		pack_head_s l_head;
		l_head.key = {NULL,NULL};
	test_rsa->head = &l_head;
	
	
		char buf_E[20] = {'c','a','t','\0'};
		char buf_N[20] = {'c','o','g','\0'};
	l_head.key = {buf_N,buf_E};
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
	

	////
	cleanStub(&si_stub4);
	cleanStub(&si_stub3);
	cleanStub(&si_stub2);
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);
}

void TestPackageSha256Rsa2048::test_pack_sha256_rsa2048_check(){
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	int ret = 0;
	pack_sha256_rsa2048_s sha_rsa_a;
	pack_sha256_rsa2048_s* p_a = &sha_rsa_a;
	pack_head_s head_b ;
	pack_head_s* p_b = &head_b;
	
	pack_sha256_s v_sha256;
	
	pack_checksum_alg_s l_base;
	l_base.check = ___check;
	v_sha256.base = l_base;
	
	mbedtls_sha256_context v_sha256_context;
	v_sha256.sha256_context = v_sha256_context;
	
	p_a->sha256 = v_sha256;
	p_a->head = NULL;
	
	ret = pack_sha256_rsa2048_init(p_a,p_b);
	TEST_ASSERT(PACK_OK == ret);
	
	///
	stubInfo si_stub2;
	setStub((void*)mbedtls_mpi_read_string,(void*)stub_mbedtls_mpi_read_stringFalse,&si_stub2);
	
	stubInfo si_stub3;
	setStub((void*)mbedtls_rsa_check_pubkey,(void*)stub_mbedtls_rsa_check_pubkey,&si_stub3);
	stubInfo si_stub4;
	setStub((void*)mbedtls_rsa_pkcs1_verify,(void*)stub_mbedtls_rsa_pkcs1_verify,&si_stub4);
	pack_checksum_alg_s l_base_2;
	l_base_2.reset = ___reset;
	l_base_2.update = ___update;
	l_base_2.check = ___check;
	l_base_2.destroy = ___destroy;
	uint8_t checksum[20]; 
	uint16_t checksum_len = 10;
	
	checksum_len = 256;
	pack_sha256_rsa2048_s *test_rsa = (pack_sha256_rsa2048_s *)(&l_base_2);
		pack_head_s l_head;
		l_head.key = {NULL,NULL};
	test_rsa->head = &l_head;
	
	
		char buf_E[20] = {'c','a','t','\0'};
		char buf_N[20] = {'c','o','g','\0'};
	l_head.key = {buf_N,buf_E};
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
	
	buf_N[0] = 'd';
	printf("out buf_N is %s\n",buf_N);
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);

	////
	cleanStub(&si_stub3);
	cleanStub(&si_stub2);
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);
	cleanStub(&si_stub4);
}
void TestPackageSha256Rsa2048::test_pack_sha256_rsa2048_init(){
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	
	int ret = 0;
	
	pack_sha256_rsa2048_s sha_rsa_a;
	pack_sha256_rsa2048_s* p_a = &sha_rsa_a;
	pack_head_s head_b ;
	pack_head_s* p_b = &head_b;
	
	pack_sha256_s v_sha256;
	
	pack_checksum_alg_s l_base;
	l_base.check = ___check;
	v_sha256.base = l_base;
	
	mbedtls_sha256_context v_sha256_context;
	v_sha256.sha256_context = v_sha256_context;
	
	p_a->sha256 = v_sha256;
	p_a->head = NULL;
	
	ret = pack_sha256_rsa2048_init(p_a,p_b);
	TEST_ASSERT(PACK_OK == ret);
	
	
	stubInfo si_stub2;
	setStub((void*)mbedtls_mpi_read_string,(void*)stub_mbedtls_mpi_read_string,&si_stub2);
	
	stubInfo si_stub3;
	setStub((void*)mbedtls_rsa_check_pubkey,(void*)stub_mbedtls_rsa_check_pubkey,&si_stub3);
	stubInfo si_stub4;
	setStub((void*)mbedtls_rsa_pkcs1_verify,(void*)stub_mbedtls_rsa_pkcs1_verify,&si_stub4);
	pack_checksum_alg_s l_base_2;
	l_base_2.reset = ___reset;
	l_base_2.update = ___update;
	l_base_2.check = ___check;
	l_base_2.destroy = ___destroy;
	uint8_t checksum[20]; 
	uint16_t checksum_len = 10;
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
	
	checksum_len = 256;
	pack_sha256_rsa2048_s *test_rsa = (pack_sha256_rsa2048_s *)(&l_base_2);
		pack_head_s l_head;
		l_head.key = {NULL,NULL};
	test_rsa->head = &l_head;
	
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
		char buf_E[20];
		char buf_N[20];
	l_head.key = {buf_E,buf_N};
	p_a->sha256.base.check(&l_base_2,checksum,checksum_len);
	

	cleanStub(&si_stub4);
	cleanStub(&si_stub3);
	cleanStub(&si_stub2);
	
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);
	

}

void TestPackageSha256Rsa2048::setup()
{
    //std::cout<<"in steup\n";
}

void TestPackageSha256Rsa2048::tear_down()
{
    //std::cout<<"in teardown\n";
}



