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
#if defined WITH_AT_FRAMEWORK
#include "at_api_interface.h"
#if defined USE_NB_NEUL95
#include "los_nb_api.h"
#endif
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
    //MX_USART3_UART_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    printf("Welcome to IoT-Club, This is EVB-M1 Board.\r\n");
}

void user_hw_init(void)
{
    char *str = "IoT-Club good";
    OLED_Init();
    DHT11_Init();
		Init_BH1750();
    OLED_Clear();
    OLED_ShowCHinese(0,0,0);
    OLED_ShowCHinese(18,0,1);
    OLED_ShowCHinese(36,0,2);
    OLED_ShowCHinese(54,0,3);
    OLED_ShowCHinese(72,0,4);
    OLED_ShowCHinese(90,0,5);
    OLED_ShowString(0,2,(uint8_t*)str,16);
    LOS_HwiCreate(EXTI0_IRQn, 2,0,EXTI0_IRQHandler,NULL);
    LOS_HwiCreate(EXTI1_IRQn, 3,0,EXTI1_IRQHandler,NULL);
    LOS_HwiCreate(EXTI2_IRQn, 4,0,EXTI2_IRQHandler,NULL);
    LOS_HwiCreate(EXTI3_IRQn, 5,0,EXTI3_IRQHandler,NULL);
}

extern int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len);
VOID main_task(VOID)
{
    user_hw_init();
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    hieth_hw_init();
    net_init();
#elif defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#define AT_DTLS 0
#if AT_DTLS
    sec_param_s sec;
    sec.pskid = "868744031131026";
    sec.psk = "d1e1be0c05ac5b8c78ce196412f0cdb0";
#endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP1: Init NB Module( NB Init )");
    printf("\r\n=====================================================\r\n");
#if AT_DTLS
    los_nb_init((const int8_t*)"180.101.147.208",(const int8_t*)"5684",&sec);
#else
    los_nb_init((const int8_t*)"180.101.147.115",(const int8_t*)"5683",NULL);
#endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP2: Register Command( NB Notify )");
    printf("\r\n=====================================================\r\n");
    los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_data_ioctl);
    LOS_TaskDelay(3000);
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    los_nb_report("22", 2);
    los_nb_report("23", 2);
    los_nb_report("23", 1);
    //los_nb_deinit();
		
#elif defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)
    extern at_adaptor_api at_interface;
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    agent_tiny_entry();
#endif
}
UINT32 creat_main_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "main_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)main_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

int main(void)
{
    UINT32 uwRet = LOS_OK;

    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
	
    uwRet = creat_main_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Start();
}

