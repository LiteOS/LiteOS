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
#ifndef WITH_AT_FRAMEWORK
#include "agent_tiny_demo.h"
#else
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#endif
#if defined WITH_AT_FRAMEWORK
#include "at_api_interface.h"
#if defined USE_NB_NEUL95
#include "los_nb_api.h"
#endif
#endif
UINT32 g_TskHandle;
UINT32 g_dtls_server_TskHandle;

void USART3_UART_Init(void);
VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    Debug_USART1_UART_Init();
    hal_rng_config();
    dwt_delay_init(SystemCoreClock);
}

extern int32_t nb_data_ioctl(void *arg, int8_t *buf, int32_t len);

VOID main_task(VOID)
{
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    hieth_hw_init();
    net_init();
#elif defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
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
    //los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_data_ioctl);
    //osDelay(3000);
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    los_nb_report("22", 2);
    los_nb_report("23", 1);
    //los_nb_deinit();

#elif defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266) || defined(USE_SIM900A))
    extern at_adaptor_api at_interface;
    at_api_register(&at_interface);
    agent_tiny_entry();
#endif
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    agent_tiny_entry();
#endif
}

#if defined(WITH_DTLS) && defined(SUPPORT_DTLS_SRV)

#define SERVER_PSK  "11223344556677881122334455667788"
#define SERVER_IDENTITY "testserver1"

#include "dtls_interface.h"

VOID dtls_server_task(VOID)
{
	mbedtls_net_context * bind_fd = atiny_malloc(sizeof(mbedtls_net_context)) ;
    mbedtls_net_context *  cli_fd = atiny_malloc(sizeof(mbedtls_net_context));
    int ret ;
    
    LOS_TaskDelay(1000);
    mbedtls_ssl_context *ssl = NULL;

    ret = dtls_bind(bind_fd, NULL, "5685", 1);
    
    ssl = dtls_ssl_new_with_psk(SERVER_PSK, strlen(SERVER_PSK), SERVER_IDENTITY, MBEDTLS_SSL_IS_SERVER);
    do {    
        unsigned char buf[64] = {0};

        ret = dtls_accept(bind_fd, cli_fd, NULL, 0, 0);
        mbedtls_ssl_set_bio(ssl, cli_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

        //new psk and handshake should been done for each client, now only for frist connection.
        do{
            ret = mbedtls_ssl_handshake(ssl);
        }
        while ((ret == MBEDTLS_ERR_SSL_WANT_READ ||
                ret == MBEDTLS_ERR_SSL_WANT_WRITE));

        ret = dtls_read(ssl, buf, sizeof(buf), 5000);
        printf("%s:%d --- dtls read buf = %s\r\n", __func__, __LINE__, buf);
        
        unsigned char hello[] = "hello this\r\n";
        ret = dtls_write(ssl, hello, sizeof(hello));

        LOS_TaskDelay(2000);
    }while(0);

    if (ssl)dtls_ssl_destroy(ssl);
    mbedtls_net_free(bind_fd);

}

UINT32 create_dtls_server_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 3;
    task_init_param.pcName = "dtls_server_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)dtls_server_task;

    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&g_dtls_server_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

#endif

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
    task_init_param.uwStackSize = 0x1000;
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
#else
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
#endif
    (void)LOS_Start();
    return 0;
}
