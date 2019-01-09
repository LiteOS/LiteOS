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
#include "stdio.h"
#include "test_flag_manager.h" 
#include "flag_manager.h" 
#include "string.h" 

#define FLASH_UNIT_SIZE (256)
#define FLASH_FLAG_SIZE (512)
 
int print1(void *buf, int32_t len)
{
	printf("call print1 buf:%s len:%d\n",buf,len);
	return 1;
}
int print2(const void *buf, int32_t len)
{
	printf("call print2 buf:%s len:%d\n",buf,len);
	return 1;
}
int print3(void *buf, int32_t len)
{
	printf("call print3 buf:%s len:%d\n",buf,len);
	return 0;
}

int print4(const void *buf, int32_t len)
{
	printf("call print4 buf:%s len:%d\n",buf,len);
	return 0;
}


void TestFlagManager::test_flag_init()
{
	int ret=0;
	 
	flag_op_s test_flag={NULL,NULL};
	 
	ret = flag_init(NULL);
	TEST_ASSERT(-1 == ret);
	
	test_flag.func_flag_read=print1;
	test_flag.func_flag_write=NULL;
	
	ret = flag_init(&test_flag);
	TEST_ASSERT(-1 == ret);
	test_flag.func_flag_read=NULL;
	test_flag.func_flag_write=print2;
	ret = flag_init(&test_flag);
	TEST_ASSERT(-1 == ret);
	 
	test_flag.func_flag_read=print1;
	test_flag.func_flag_write=print2;
	ret = flag_init(&test_flag);
	TEST_ASSERT(0 == ret);
}

int ttt[FLASH_FLAG_SIZE]={0};
void TestFlagManager::test_flag_read()
{
	 
	int ret =0;
	flag_type_e test_flag_type = FLAG_BOOTLOADER;
	void *test_buf=NULL;
	int32_t len = 100;
	flag_op_s test_flag={print1,print2};
 
	test_buf=NULL;
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
	
	test_buf=ttt;
	len=-1;
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
	
	len=FLASH_UNIT_SIZE+1;
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
	
	len=FLASH_UNIT_SIZE;
	ret = flag_init(&test_flag); 
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);

	test_flag.func_flag_read=print3;
	test_flag.func_flag_write=print4;
	ret = flag_init(&test_flag); 
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(0 == ret);
	 
	test_flag_type = FLAG_BOOTLOADER;
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(0 == ret);
	
	test_flag_type = FLAG_APP;
	ret=flag_read(test_flag_type, test_buf,len);
    TEST_ASSERT(0 == ret);
	
	test_flag_type =(flag_type_e)2;
	ret=flag_read(test_flag_type, test_buf,len);
	TEST_ASSERT(0 == ret);

}
 
void TestFlagManager::test_flag_write()
{
	 
	int ret =0;

	printf("come into test_flag_write *********************************\n");
 
	flag_type_e test_flag_type = FLAG_BOOTLOADER;
	void *test_buf=NULL;
	int32_t len = 100;
	flag_op_s test_flag={print1,print2};
	 
	test_buf=NULL;
	ret=flag_write(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);

	printf("over come into test_flag_write *********************************\n");
	
	test_buf=ttt;
	len=-1;
	ret=flag_write(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
	
	len=FLASH_UNIT_SIZE+1;
	ret=flag_write(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
	
	len=FLASH_UNIT_SIZE;
	ret = flag_init(&test_flag); 
	ret=flag_write(test_flag_type, test_buf,len);
	TEST_ASSERT(-1 == ret);
 
	test_flag.func_flag_read=print3;
	test_flag.func_flag_write=print4;
	ret = flag_init(&test_flag); 
	ret=flag_write(test_flag_type, test_buf,len);
	TEST_ASSERT(0 == ret);
 
	ret=flag_write(FLAG_BOOTLOADER, test_buf,len);
	TEST_ASSERT(0 == ret);
	ret=flag_write(FLAG_APP, test_buf,len);
	TEST_ASSERT(0 == ret);
	ret=flag_write(FLAG_INVALID, test_buf,len);
	TEST_ASSERT(0 == ret);
 
}



TestFlagManager::TestFlagManager()
{
	
    TEST_ADD(TestFlagManager::test_flag_init);
	TEST_ADD(TestFlagManager::test_flag_read);
	TEST_ADD(TestFlagManager::test_flag_write);
	

}

TestFlagManager::~TestFlagManager()
{
}

void TestFlagManager::setup()
{
    std::cout << "setup\n";
}

void TestFlagManager::tear_down()
{
    std::cout << "test_down\n";
}


