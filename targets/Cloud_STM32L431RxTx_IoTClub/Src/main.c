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

#include "sys_init.h"
#include "agent_tiny_demo.h"
#include "nb-iot_demo.h"
#include "club_include.h"
#include "club_func.h"
#include "club_nbiot.h"

#if defined WITH_AT_FRAMEWORK
#include "at_api_interface.h"
#endif  

#if defined USE_NB_NEUL95
#include "los_nb_api.h"
#endif

UINT32 g_TskHandle;

VOID HardWare_Init(VOID)
{
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    dwt_delay_init(SystemCoreClock);
	  hal_rng_config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    printf("Welcome to IoT-Club, This is EVB-M1 Board.\r\n");
}

void user_hw_init(void)
{
	club_oled_init();
	club_key_init();
}



/*
* NAME :proc_main_task
*/
void proc_main_task(UINT32 uwArg){
	printf("Sys is run\r\n");
	
	//用户程序初始化
	user_hw_init();	
	
	/******************************************************/
	// 网络选择
	#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
		nb_iot_entry();		
	#elif defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)
		extern at_adaptor_api at_interface;
		at_api_register(&at_interface);
		agent_tiny_entry();
	#else
		agent_tiny_entry();
	#endif	
	/******************************************************/
	
	
}


// int nb_start_callback(void* arg,int8_t * buf, int32_t len)
// {
//     if (NULL == buf || len <= 0)
//     {
//         CLUB_DEBUG("param invailed!\r\n");
//         return -1;
//     }
//     CLUB_DEBUG("cmd is:%s\r\n",buf);
// 	return 0;
// }






int main(void)
{
    UINT32 uwRet = LOS_OK;
    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK){
        return LOS_NOK;
    }

    uwRet = creat_all_task();
    if (uwRet != LOS_OK){
        return LOS_NOK;
    }

    LOS_Start();
}

