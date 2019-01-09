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
#include <cpptest.h>
#include "test_sota_hal.h"
#include "stub.h"
#include <unistd.h>
#include <iostream>
#include <fstream>


extern "C"
{
	#include <stdio.h>
	#include <stdint.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <stdbool.h>
    extern int32_t crc_check(const unsigned char *message, int len); 
	extern int HexStrToByte(const unsigned char *bufin, unsigned char *bufout, int sourceLen);
}

void TestSotaHal::test_HexStrToByte()
{
    int ret = 0;
	unsigned char *bufin = NULL;
	unsigned char *bufout;
	int sourcelen = 100;
	ret = HexStrToByte(bufin,bufout,sourcelen);
	TEST_ASSERT_MSG((-1 == ret), "HexStrToByte(...) failed");
}

TestSotaHal::TestSotaHal()
{
	printf("in TestSotaHal consfunc\n");
    //TEST_ADD(TestSotaHal::test_crc_check);
	TEST_ADD(TestSotaHal::test_HexStrToByte);
}

TestSotaHal::~TestSotaHal()
{  
}

void TestSotaHal::setup()
{
    printf("come into test funcno \n");
}

void TestSotaHal::tear_down()
{
    printf("exit from funcno \n");
}


