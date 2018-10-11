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
#include "main.h"
#include "sys_init.h"

#include "agenttiny_lwm2m/agent_tiny_demo.h"
#include "fs/fs_demo.h"

#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"

#if defined WITH_AT_FRAMEWORK
#include "at_frame/at_api.h"
#if defined USE_NB_NEUL95
#include "nb_iot/los_nb_api.h"
#include "at_device/bc95.h"
#endif
#endif
#ifdef SUPPORT_DTLS_SRV
#include "test_dtls_server.h"
#endif
UINT32 g_TskHandle;

void USART3_UART_Init(void);
VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    Debug_USART1_UART_Init();
    hal_rng_config();
    dwt_delay_init(SystemCoreClock);
}

void demo_agenttiny_with_lwip(void)
{
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    hieth_hw_init();
    net_init();
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_LWIP\n");
#endif
}
void demo_nbiot_without_agenttiny(void)
{
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
    #define AT_DTLS 0
    #if AT_DTLS
    sec_param_s sec;
    sec.setpsk = 1;
    sec.pskid = "868744031131026";
    sec.psk = "d1e1be0c05ac5b8c78ce196412f0cdb0";
    #endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP1: Init NB Module( NB Init )");
    printf("\r\n=====================================================\r\n");
#if AT_DTLS
    los_nb_init((const int8_t *)"180.101.147.115", (const int8_t *)"5684", &sec);
#else
    los_nb_init((const int8_t *)"180.101.147.115", (const int8_t *)"5683", NULL); //"139.159.140.34"
#endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP2: Register Command( NB Notify )");
    printf("\r\n=====================================================\r\n");
    
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    los_nb_report("22", 2);
    los_nb_report("23", 1);
    
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_NB_NEUL95\n");
#endif

}
void demo_agenttiny_with_nbiot(void)
{
#if defined(WITH_AT_FRAMEWORK) && (defined(USE_NB_NEUL95))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry============================\n");
    los_nb_init((const int8_t *)"172.25.233.98",(const int8_t *)"5600",NULL);
    void agent_tiny_entry(void);
    los_nb_notify("\r\n+NSONMI:",strlen("\r\n+NSONMI:"),NULL,nb_cmd_match);
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_NB_NEUL95\n");
#endif
}
void demo_agenttiny_with_gprs(void)
{
#if defined(WITH_AT_FRAMEWORK) && (defined(USE_SIM900A))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry  USE_SIM900A============================\n");
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_SIM900A\n");
#endif
}

void demo_agenttiny_with_wifi(void)
{

#if defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry  USE_ESP8266============================\n");
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_ESP8266\n");
#endif

}


VOID main_task(VOID)
{
    //fs_demo();
    demo_agenttiny_with_nbiot();
}

UINT32 creat_main_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "main_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)main_task;

#ifdef CONFIG_FEATURE_FOTA
    task_init_param.uwStackSize = 0x2000; /* fota use mbedtls bignum to verify signature  consuming more stack  */
#else
    task_init_param.uwStackSize = 0x2000;
#endif

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

#if defined(USE_PPPOS)
    #include "osport.h"
    extern void uart_init(void);  //this uart used for the pppos interface
    uart_init();
    extern VOID *main_ppp(UINT32  args);
    task_create("main_ppp", main_ppp, 0x800, NULL, NULL, 0);
#endif
    uwRet = creat_main_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

#if defined(WITH_DTLS) && defined(SUPPORT_DTLS_SRV)
    uwRet = create_dtls_server_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }
#endif

    (void)LOS_Start();
    return 0;
}
