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

/* Includes -----------------------------------------------------------------*/
#include "test_util_timer.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "util_timer.h"
#include "liblwm2m.h"


}
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
Test_UTIL_TIMER::Test_UTIL_TIMER()
{
    TEST_ADD(Test_UTIL_TIMER::test_timer_init);
	
    TEST_ADD(Test_UTIL_TIMER::test_timer_start);
    TEST_ADD(Test_UTIL_TIMER::test_timer_step);
    
}
Test_UTIL_TIMER::~Test_UTIL_TIMER()
{

}

void Test_UTIL_TIMER::test_timer_init(void)
{
	util_timer_t util_time;

	timer_init(&util_time,50,NULL,NULL);	
}

void Test_UTIL_TIMER::test_timer_start(void)
{
    util_timer_t util_time;
    timer_start(&util_time);
}
void util_time_callback(void *param)
{
    return;
}

void Test_UTIL_TIMER::test_timer_step(void)
{
    util_timer_t util_time;
    util_time.startFlag = false;
	timer_step(&util_time);	
    util_time.startFlag = true;
	timer_step(&util_time);	

	util_time.expireTime = lwm2m_gettime() -100;
	util_time.callback = util_time_callback;
	timer_step(&util_time);	

	
}


/* Private functions --------------------------------------------------------*/

