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

#include "test_otacrc.h"
#include "ota_crc.h"
#include "string.h"
uint32_t buf[100];
void TestOtaCRC::test_ota_crc()
{
	uint32_t ret=111;
	ret=calc_crc32(0,NULL,10);
			if(ret==0)printf("参数错误：buf为NULL或者len小于0\n");
			else printf("calc_crc32 返回值为：%d\n",ret);
	ret=calc_crc32(100,buf,-1);
			if(ret==0)printf("参数错误：buf为NULL或者len小于0\n");
			else printf("calc_crc32 返回值为：%d\n",ret);
	ret=calc_crc32(100,buf,100);
			if(ret==0)printf("参数错误：buf为NULL或者len小于0\n");
			else printf("calc_crc32 返回值为：%d\n",ret);
}




TestOtaCRC::TestOtaCRC()
{
	printf("进入测试类的构造函数\n");
    TEST_ADD(TestOtaCRC::test_ota_crc);
	printf("将测试代码装载进类中\n");

}

TestOtaCRC::~TestOtaCRC()
{
}

void TestOtaCRC::setup()
{
    std::cout << "setup\n";
}

void TestOtaCRC::tear_down()
{
    std::cout << "test_down\n";
}


