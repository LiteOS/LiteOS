/*------------------------------------------------------------------------------
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

#include "test_package_head.h"
#include "package_head.h"
#include "package_writer.h"

#include "stddef.h"
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
	int g_max_size = 0;
	int pack_head_parse_head_len(pack_head_s *head, uint32_t offset, const uint8_t *buff,uint16_t len, uint16_t *used_len);
	
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
	
    #include "adapter_layer.h"
	
	static void ___reset(struct pack_checksum_alg_tag_s *thi){}
	static int ___update(struct pack_checksum_alg_tag_s *thi, const uint8_t *buff, uint16_t len){
		return 0;
	}
	static int ___check(struct pack_checksum_alg_tag_s *thi, const uint8_t  *checksum, uint16_t checksum_len){return 0;}
	static void ___destroy(struct pack_checksum_alg_tag_s *thi){}
	
	static int ___read_software(struct pack_hardware_tag_s *thi, uint32_t offset, uint8_t *buffer, uint32_t len){return 0;}
	static int ___write_software(struct pack_hardware_tag_s *thi, uint32_t offset, const uint8_t *buffer, uint32_t len){return 0;}
	static void ___set_flash_type(struct pack_hardware_tag_s *thi, ota_flash_type_e type){}
	static uint32_t ___get_block_size(struct pack_hardware_tag_s *thi){return 0;}
	static uint32_t ___get_max_size(struct pack_hardware_tag_s *thi){return g_max_size;}
	
	static int ___head_update_check(const uint8_t *head_buff , uint16_t len, void *param){return 0;}
	static int ___head_update_checkFalse(const uint8_t *head_buff , uint16_t len, void *param){return 1;}
	
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
	static int stub_pack_head_parse_head_len(pack_head_s *head, uint32_t offset, const uint8_t *buff,uint16_t len, uint16_t *used_len){
		return 0;
	}
	static void* p_bak_for_free = NULL;
	static void stub_atiny_free(void*para){
		p_bak_for_free = para;
	}
	static void stub_pack_checksum_delete(pack_checksum_s *thi){}
	static pack_storage_device_s for_log_device;
	static pack_params_s * stub_pack_get_params_2(){
		pack_storage_device_s *device = &for_log_device;
		device->params.printf = printf;
		return &device->params;
	}
	void *atiny_malloc(size_t size);
	void atiny_free(void *ptr);
	
	static void* stub_atiny_malloc(size_t size){return NULL;}
	static pack_checksum_s *stub_pack_checksum_create(pack_head_s *head){return NULL;}
	static int stub_pack_checksum_check(pack_checksum_s *thi, const uint8_t *expected_value, uint16_t len){
		return 0;
	}
	static void print_ss(const char* s){
		printf("=================================================================\n");
		printf("+-+-+-+-+-+-    %s    +-+-+-+-+-+-\n",s);
		printf("=================================================================\n");
	}
	static int static_int_count_pack_malloc=0;
	static void * stub_pack_malloc_count(size_t size){
		if(0==static_int_count_pack_malloc){
			static_int_count_pack_malloc++;
			print_ss("172");
			return atiny_malloc(size);
		}
		else{
			return NULL;
		}
	}
} //extern "C"
void TestPackageHead::test_pack_head_init(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_head_s* p = &v_head;
	__test_init_hardware();
	__test_set_head();
	pack_head_init(p);
	cleanStub(&si_stub_log);
}
void TestPackageHead::test_pack_head_destroy(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	pack_head_s * p = atiny_malloc(sizeof(pack_head_s)) ;
	pack_head_s * p_mark = p;
	char * p_char =atiny_malloc(8*sizeof(char));
	char * p_char2 = atiny_malloc(8*sizeof(char));
	p->buff = p_char;
	p->checksum_pos = p_char2;

	p->checksum = NULL;
	
	p->hardware = &v_hardware;
	p->update_check = ___head_update_check;
	p->key = v_key;
	
	pack_head_destroy(p);
	printf("@@@187\n");
	pack_checksum_s * p_cs = atiny_malloc(sizeof(pack_checksum_s)) ;
	pack_checksum_s * p_mark2 = p_cs;
	p->checksum = p_cs;
	
	stubInfo si_si1;
	setStub((void*)pack_checksum_delete,(void*)stub_pack_checksum_delete,&si_si1);
	
	pack_head_destroy(p);
	printf("@@@195\n");
	cleanStub(&si_si1);
	//if(p_cs!=NULL)atiny_free(p_cs) ;
	//
	//if(p_char!=NULL)atiny_free(p_char);
	//if(p_char2!=NULL)atiny_free(p_char2);
	//
	if(p_mark!=NULL){
		atiny_free(p_mark);
	}
	if(p_mark2!=NULL){
		atiny_free(p_mark2);
	}
	cleanStub(&si_stub_log);
	cleanStub(&si_stub_free);
}
void TestPackageHead::test_pack_head_parse_head_len(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	int ret = 0;
	pack_hardware_s l_hardware;
	l_hardware.read_software = ___read_software;
	l_hardware.write_software = ___write_software;
	l_hardware.set_flash_type = ___set_flash_type;
	l_hardware.get_block_size = ___get_block_size;
	l_hardware.get_max_size = ___get_max_size;
	int tmp_param = 13;
	char * l_buff1  = atiny_malloc(50*sizeof(char));
	
	for(int i=0;i<50;i++)
		*(l_buff1+1)=0x00;
	
	char l_buff2[50];
	
	pack_head_s l_head;
	l_head.hardware = &l_hardware;
	l_head.update_check = ___head_update_check;
	l_head.param = &tmp_param;
	l_head.key = v_key;
	l_head.buff = l_buff1;
	l_head.stored_len  = 8;
	l_head.head_len = 16;
	l_head.checksum = NULL;
	l_head.checksum_pos = l_buff2;
	l_head.checksum_len = 16;
	pack_head_s * p = &l_head;
	
	uint32_t offset = 10;//must < 12
	uint8_t  buff_safe[20] ;
	uint16_t len = 16;
	uint16_t used_len = 12;
	
	uint16_t* p_used_len = &used_len ;
	ret = pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);//if(offset > head->stored_len)
	TEST_ASSERT(PACK_ERR == ret);
	
	offset = 4;
	p->buff = NULL;
	stubInfo si__1;
	setStub((void*)pack_malloc,(void*)stub_atiny_malloc,&si__1);
	ret = pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);
	cleanStub(&si__1);
	
	ret = pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	if(p->buff!=NULL){
		p->buff = NULL;
	}

	l_head.stored_len  = 18;
	for(int i=0;i<50;i++)l_buff1[i]=0b00000000;
		
	l_head.buff = l_buff1;
	for(int i=0;i<20;i++)buff_safe[i]=0b00000000;
	
	//printf("l_head.buff is %p\n",l_head.buff);
	//printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t\n",l_head.buff[4],l_head.buff[5]\
			,l_head.buff[6],l_head.buff[7],l_head.buff[8],l_head.buff[9],l_head.buff[10],\
			l_head.buff[11]);
	
	l_head.head_len = 0;
	buff_safe[3] = 0xaa;
	buff_safe[6] = 0xff;

	stubInfo si;
	setStub((void*)atiny_free,(void*)stub_atiny_free,&si);
	ret = pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);//
	if(p_bak_for_free!=NULL){
		//atiny_free(p_bak_for_free);
		p_bak_for_free = NULL;
	}
	if(p->buff!=NULL){
		atiny_free(p->buff);
		p->buff = NULL;
	}
	TEST_ASSERT(PACK_OK == ret);
	stubInfo si_pack_malloc;
	setStub((void*)pack_malloc,(void*)stub_pack_malloc_count,&si_pack_malloc);
	static_int_count_pack_malloc=0;
	pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);
	cleanStub(&si_pack_malloc);
	len = 6;
	pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);
	
	cleanStub(&si);
	len = 16;
	buff_safe[6] = 0x00;
	ret = pack_head_parse_head_len(p,offset,buff_safe,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	
	
	
	if(l_head.buff!=NULL){
		//atiny_free(l_head.buff);
		l_head.buff = NULL;
	}
	if(l_buff1!=NULL){
		atiny_free(l_buff1);
		l_buff1 = NULL;
	}

	cleanStub(&si_stub_log);
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);
}

void TestPackageHead::test_pack_head_parse(){
	
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	stubInfo si_stub_malloc;
	setStub((void*)pack_malloc,(void*)atiny_malloc,&si_stub_malloc);
	stubInfo si_stub_free;
	setStub((void*)pack_free,(void*)atiny_free,&si_stub_free);
	
	pack_hardware_s l_hardware;
	l_hardware.read_software = ___read_software;
	l_hardware.write_software = ___write_software;
	l_hardware.set_flash_type = ___set_flash_type;
	l_hardware.get_block_size = ___get_block_size;
	l_hardware.get_max_size = ___get_max_size;
	int tmp_param = 13;
	char *l_buff1 = atiny_malloc(50*sizeof(char));
	char l_buff2[50];
	
	pack_head_s l_head;
	l_head.hardware = &l_hardware;
	l_head.update_check = ___head_update_check;
	l_head.param = &tmp_param;
	l_head.key = v_key;
	l_head.buff = l_buff1;
	l_head.stored_len  = 8;
	l_head.head_len = 16;
	pack_checksum_s * p_cksum = atiny_malloc(sizeof(pack_checksum_s));
		pack_checksum_alg_s l_base;
		l_base.destroy = ___destroy;
	#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
		p_cksum->alg.sha256.base = l_base;
	#elif (PACK_CHECKSUM == PACK_SHA256)
		p_cksum->alg.base = l_base;
	#endif
	pack_checksum_s* p_cksum_mark = p_cksum;
	l_head.checksum = p_cksum;
	l_head.checksum_pos = l_buff2;
	l_head.checksum_len = 16;
	int ret = 0;
	uint32_t offset = 2; //<12
	uint8_t buff[50];
	for(int i=0;i<50;i++)buff[i]=0x00;
	uint16_t len = 16;
	uint16_t used_len = 12;
	uint16_t * p_used_len = &used_len;
	
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	
	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	l_head.checksum_pos = NULL;
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	
	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	l_head.head_len = 14;
	offset = 13;
	l_head.stored_len  = 15;
	for(int i=0;i<20;i++)l_buff1[i] = 0b00000000;
	l_buff1[8] = 0b00000001;
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	
	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	g_max_size = 0x1000002;
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);

	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	l_head.head_len = 18;
	l_buff1[12]=0x00;
	l_buff1[13]=0x03;
	buff[0] = 0x03;//13-offset ;if offset < 13
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	
	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	l_head.checksum_pos = atiny_malloc(10*sizeof(char));
	char * p_tmp_char = l_head.checksum_pos;
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	if(l_buff1==NULL)l_buff1 = atiny_malloc(50*sizeof(char));
	if(p_cksum==NULL){
		p_cksum = atiny_malloc(sizeof(pack_checksum_s));
					pack_checksum_alg_s l_base2;
					l_base2.destroy = ___destroy;
			#if (PACK_CHECKSUM == PACK_SHA256_RSA2048)
				p_cksum->alg.sha256.base = l_base2;
			#elif (PACK_CHECKSUM == PACK_SHA256)
				p_cksum->alg.base = l_base2;
			#endif
	}
	buff[2] = 0x02;//13+2-offset; if offset <13+2
	l_head.checksum_pos = NULL;
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_OK == ret);
	
	
	if(p_cksum==NULL)p_cksum = atiny_malloc(sizeof(pack_checksum_s)) ;
	buff[0] = 0x04;
	stubInfo si_createChecksum;
	setStub((void*)pack_checksum_create,(void*)stub_pack_checksum_create,&si_createChecksum);
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);//head->checksum = pack_checksum_create(head);
	TEST_ASSERT(PACK_ERR == ret);
	cleanStub(&si_createChecksum);
	
	// note : (void)memcpy(head->buff + offset, buff, copy_len);
	buff[4] = 0xff; //>2 is fine
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	char buff_1[20] = {0,0,0,8};
	l_head.buff = buff_1;
	pack_head_parse(&l_head,offset,buff,len,p_used_len);
	l_head.buff = l_buff1;
	
	
	// ignore for depth
	//run: if((0 == len) && (head->stored_len < head->head_len))
	len = 0;
	l_head.stored_len  = 10;
	stubInfo si2;
	setStub((void*)pack_head_parse_head_len,(void*)stub_pack_head_parse_head_len,&si2);
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_OK == ret);
	
	// ignore for depth , follow above
	//run:if((head->head_len < PACK_HEADER_MIN_LEN)
	len = 1;
	l_head.head_len = 10; //<12
	ret = pack_head_parse(&l_head,offset,buff,len,p_used_len);
	TEST_ASSERT(PACK_ERR == ret);
	cleanStub(&si2);
    //
	//if(l_head.buff!=NULL){
	//	atiny_free(l_head.buff);
	//	l_head.buff = NULL;
	//}
	//if(l_head.checksum!=NULL){
	//	atiny_free(l_head.checksum);
	//	l_head.checksum = NULL;
	//}
	//if(p_cksum!=NULL){
	//	atiny_free(p_cksum);
	//	p_cksum = NULL;
	//}
	if(l_head.checksum_pos!=NULL){
		atiny_free(l_head.checksum_pos);
		l_head.checksum_pos = NULL;
	}
	cleanStub(&si_stub_log);
	cleanStub(&si_stub_malloc);
	cleanStub(&si_stub_free);

}

void TestPackageHead::test_pack_head_check(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	__test_init_hardware();
	__test_set_head();
	pack_head_s * p = &v_head;
	uint32_t len = 10;
	ret = pack_head_check(p,len);
	TEST_ASSERT(ret == PACK_ERR);
	
	v_head.head_len = 22; //>12
	v_head.stored_len = 22;
	ret = pack_head_check(p,len);
	TEST_ASSERT(ret == PACK_ERR);
	
	char buf[100];
	for(int i=0;i<100;i++)buf[i]=0b00000000;
	v_head.buff = buf;
	
	
	len = 0x000000;
	v_head.checksum_pos = NULL;
	ret = pack_head_check(p,len);
	TEST_ASSERT(ret == PACK_OK);
	
	v_head.checksum_pos = buf;
	v_head.checksum = NULL;
	ret = pack_head_check(p,len);
	TEST_ASSERT(ret == PACK_OK);
	pack_checksum_s l_s_checksum_1;
	v_head.checksum = &l_s_checksum_1;
	stubInfo si_pack_checksum_check;
	setStub((void*)pack_checksum_check,(void*)stub_pack_checksum_check,&si_pack_checksum_check);
	ret = pack_head_check(p,len);
	cleanStub(&si_pack_checksum_check);
	cleanStub(&si_stub_log);
	
	
	
}
void TestPackageHead::test_pack_head_get_head_len(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	__test_init_hardware();
	__test_set_head();
	pack_head_s * p = &v_head;
	ret = pack_head_get_head_len(p);
	TEST_ASSERT(ret == PACK_OK);
	cleanStub(&si_stub_log);
}
void TestPackageHead::test_pack_head_get_head_info(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	char* p_ret = NULL;
	__test_init_hardware();
	__test_set_head();
	pack_head_s * p = &v_head;
	p_ret = pack_head_get_head_info(p);
	TEST_ASSERT(NULL == p_ret);
	cleanStub(&si_stub_log);
}
void TestPackageHead::test_pack_head_set_head_info(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	int ret = 0;
	__test_init_hardware();
	__test_set_head();
	pack_head_s * p = &v_head;
	pack_device_info_s v_info;
	v_info.hardware = &v_hardware;
	v_info.key = v_key;
	ret = pack_head_set_head_info(p,&v_info);
	TEST_ASSERT(ret == PACK_OK);
	cleanStub(&si_stub_log);
}
void TestPackageHead::test_pack_head_get_checksum(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	pack_checksum_s* ret = NULL;
	pack_head_get_checksum(&v_head);
	TEST_ASSERT(ret == NULL);
	cleanStub(&si_stub_log);
}
void TestPackageHead::test_pack_head_get_key(){
	stubInfo si_stub_log;
	setStub((void*)pack_get_params,(void*)stub_pack_get_params_2,&si_stub_log);
	ota_key_s * ret = NULL;
	pack_head_get_key(&v_head);
	TEST_ASSERT(ret == NULL);
	cleanStub(&si_stub_log);
}

TestPackageHead::TestPackageHead()
{
	TEST_ADD(TestPackageHead::test_pack_head_init);
	TEST_ADD(TestPackageHead::test_pack_head_destroy);
	TEST_ADD(TestPackageHead::test_pack_head_parse_head_len);
	TEST_ADD(TestPackageHead::test_pack_head_parse);
	TEST_ADD(TestPackageHead::test_pack_head_check);
	TEST_ADD(TestPackageHead::test_pack_head_get_head_len);
	TEST_ADD(TestPackageHead::test_pack_head_get_head_info);
	TEST_ADD(TestPackageHead::test_pack_head_set_head_info);
	TEST_ADD(TestPackageHead::test_pack_head_get_checksum);
	TEST_ADD(TestPackageHead::test_pack_head_get_key);
}

TestPackageHead::~TestPackageHead()
{  

}
void TestPackageHead::setup()
{
    //printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestPackageHead::tear_down()
{
    //printf("exit from funcno %d,%s\n", funcno,__FILE__);
}
