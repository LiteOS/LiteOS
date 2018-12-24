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
#include "test_flag_manager.h"	//本类的头文件
#include "flag_manager.h"//待测源文件的头文件
#include "string.h"//标准库


#define FLASH_UNIT_SIZE (256)
#define FLASH_FLAG_SIZE (512)
/*
int flag_init(flag_op_s *flag)
{
    if (NULL == flag
        || NULL == flag->func_flag_read
        || NULL == flag->func_flag_write)
    return -1;

    g_flag_op.func_flag_read = flag->func_flag_read;
    g_flag_op.func_flag_write = flag->func_flag_write;

    return 0;
}
*/
int print1(void *buf, int32_t len)
{
	printf("正在调用print1函数 buf:%s len:%d\n",buf,len);
	return 1;
}
int print2(const void *buf, int32_t len)
{
	printf("正在调用print2函数 buf:%s len:%d\n",buf,len);
	return 1;
}
int print3(void *buf, int32_t len)
{
	printf("正在调用print3函数 buf:%s len:%d\n",buf,len);
	return 0;
}
void TestFlagManager::test_flag_init()
{
	int ret=0;
	//参数
	flag_op_s test_flag={NULL,NULL};
	//参数判空测试
	ret = flag_init(NULL);
	test_flag.func_flag_read=print1;
	test_flag.func_flag_write=NULL;
	ret = flag_init(&test_flag);
	test_flag.func_flag_read=NULL;
	test_flag.func_flag_write=print2;
	ret = flag_init(&test_flag);
	//主代码测试（赋值冗余，但是为了看的更直接，所以重复赋值）
	test_flag.func_flag_read=print1;
	test_flag.func_flag_write=print2;
	ret = flag_init(&test_flag);
}
/*
int flag_read(flag_type_e flag_type, void *buf, int32_t len)
{
    uint8_t flag_buf[FLASH_FLAG_SIZE];

    if (NULL == buf
        || len < 0
        || len > FLASH_UNIT_SIZE
        || g_flag_op.func_flag_read(flag_buf, FLASH_FLAG_SIZE) != 0)
        return -1;

    switch (flag_type)
    {
    case FLAG_BOOTLOADER:
        memcpy(buf, flag_buf, len);
        break;
    case FLAG_APP:
        memcpy(buf, flag_buf + FLASH_UNIT_SIZE, len);
        break;
    default:
        break;
    }

    return 0;
}
*/
int ttt[FLASH_FLAG_SIZE]={0};
void TestFlagManager::test_flag_read()
{
	//局部优先
	//flag_op_s g_flag_op={print1,print2};//无传参入口，影响不到待调函数的内部
	//返回值
	int ret =0;
	//参数
	flag_type_e test_flag_type = FLAG_BOOTLOADER;
	void *test_buf=NULL;
	int32_t len = 100;
	flag_op_s test_flag={print1,print2};
	//参数判空测试
	test_buf=NULL;
	ret=flag_read(test_flag_type, test_buf,len);
	
	
	test_buf=ttt;
	len=-1;
	ret=flag_read(test_flag_type, test_buf,len);
	
	
	len=FLASH_UNIT_SIZE+1;
	ret=flag_read(test_flag_type, test_buf,len);
	
	len=FLASH_UNIT_SIZE;
	ret = flag_init(&test_flag);//影响func_flag_read
	ret=flag_read(test_flag_type, test_buf,len);
	
	test_flag.func_flag_read=print3;
	ret = flag_init(&test_flag);//影响func_flag_read
	ret=flag_read(test_flag_type, test_buf,len);
	//主代码
	//1
	test_flag_type = FLAG_BOOTLOADER;
	ret=flag_read(test_flag_type, test_buf,len);
	//2
	test_flag_type = FLAG_APP;
	ret=flag_read(test_flag_type, test_buf,len);
	//3
	test_flag_type =(flag_type_e)2;
	ret=flag_read(test_flag_type, test_buf,len);
	
}
/*
int flag_write(flag_type_e flag_type, const void *buf, int32_t len)
{
    uint8_t flag_buf[FLASH_FLAG_SIZE];

    if (NULL == buf
        || len < 0
        || len > FLASH_UNIT_SIZE
        || g_flag_op.func_flag_read(flag_buf, FLASH_FLAG_SIZE) != 0)
        return -1;

    switch (flag_type)
    {
    case FLAG_BOOTLOADER:
        memcpy(flag_buf, buf, len);
        break;
    case FLAG_APP:
        memcpy(flag_buf + FLASH_UNIT_SIZE, buf, len);
        break;
    default:
        break;
    }
    return g_flag_op.func_flag_write(flag_buf, FLASH_FLAG_SIZE);
}
*/
void TestFlagManager::test_flag_write()
{
	//局部优先
	//flag_op_s g_flag_op={print1,print2};//无传参入口，影响不到待调函数的内部
	//返回值
	int ret =0;
	//参数
	flag_type_e test_flag_type = FLAG_BOOTLOADER;
	void *test_buf=NULL;
	int32_t len = 100;
	flag_op_s test_flag={print1,print2};
	//参数判空测试
	test_buf=NULL;
	ret=flag_write(test_flag_type, test_buf,len);
	
	
	test_buf=ttt;
	len=-1;
	ret=flag_write(test_flag_type, test_buf,len);
	
	
	len=FLASH_UNIT_SIZE+1;
	ret=flag_write(test_flag_type, test_buf,len);
	
	len=FLASH_UNIT_SIZE;
	ret = flag_init(&test_flag);//影响func_flag_read
	ret=flag_write(test_flag_type, test_buf,len);
	
	test_flag.func_flag_read=print3;
	ret = flag_init(&test_flag);//影响func_flag_read
	ret=flag_write(test_flag_type, test_buf,len);
	//主代码
	//1
	test_flag_type = FLAG_BOOTLOADER;
	ret=flag_write(test_flag_type, test_buf,len);
	//2
	test_flag_type = FLAG_APP;
	ret=flag_write(test_flag_type, test_buf,len);
	//3
	test_flag_type =(flag_type_e)2;
	ret=flag_write(test_flag_type, test_buf,len);
	
}



TestFlagManager::TestFlagManager()
{
	printf("进入测试类的构造函数\n");
    TEST_ADD(TestFlagManager::test_flag_init);
	TEST_ADD(TestFlagManager::test_flag_read);
	TEST_ADD(TestFlagManager::test_flag_write);
	printf("将测试代码装载进类中\n");

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


