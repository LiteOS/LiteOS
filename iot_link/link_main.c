/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-04-28 15:00  zhangqianfu  The first version  
 *
 */
//RTOS KERNEL
#include <osal.h>


#ifdef WITH_DTLS
#include <dtls_interface.h>

#endif


extern int netdriver_install();
__attribute__((weak)) int netdriver_install()
{
    printf("please remember to supply a netdriver---- please\n\r");

    return -1;
}

int link_main(void *args)
{
    ///< install the RTOS kernel for the link
    osal_init();

#if CONFIG_LITEOS_ENABLE
    #include <liteos_imp.h>
    osal_install_liteos();
#elif CONFIG_LINUX_ENABLE
    #include <linux_imp.h>
    #include <signal.h>
    sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);
    osal_install_linux();
#elif  CONFIG_MACOS_ENABLE
    #include <macos_imp.h>
    osal_install_macos();
#else
    #error("no os supplied yet");
#endif

#if CONFIG_STIMER_ENABLE
    #include <stimer.h>
    stimer_init();
#endif


#if CONFIG_SHELL_ENABLE
    #include <shell.h>
    ///< install the shell for the link
    extern void shell_uart_init(int baud);
    shell_uart_init(115200);
    shell_init();
#endif

#if CONFIG_DRIVER_ENABLE
    #include <driver.h>
    ///< install the driver framework for the link
    los_driv_init();
#endif

#if CONFIG_AT_ENABLE
    #include <at.h>
    #include <iot_link_config.h>
    extern bool_t uart_at_init(int baud);
    ///< install the at framework for the link
    uart_at_init(CONFIG_AT_BAUDRATE);
    at_init(CONFIG_AT_DEVICENAME);
#endif

    ///< install the cJSON, for the oc mqtt agent need the cJSON
#if CONFIG_JSON_ENABLE
    #include <cJSON.h>

    cJSON_Hooks  hook;
    hook.free_fn = osal_free;
    hook.malloc_fn = osal_malloc;
    cJSON_InitHooks(&hook);
#endif
    ///< install the tcpip stack and net driver for the link
#if CONFIG_TCPIP_ENABLE
    #include <sal.h>
    tcpipstack_init(10);

    #if CONFIG_LWIP_ENABLE
        #include <lwip_imp.h>
        tcpipstack_install_lwip(netdriver_install);
    #elif CONFIG_LINUX_SOCKET_ENABLE
        #include <linux_socket_imp.h>
        tcpipstack_install_linux_socket();
    #elif CONFIG_MACOS_SOCKET_ENABLE
        #include <macos_socket_imp.h>
        tcpipstack_install_macos_socket();
    #elif CONFIG_ESP8266_ENABLE
        //#include <esp8266.h>
        //tcpipstack_install_esp8266();
    #else

    #endif

#endif

#ifdef WITH_DTLS

    dtls_init();
#endif

//////////////////////////  MQTT PROTOCOL  /////////////////////////////////////
#if CONFIG_MQTT_ENABLE
    #include <mqtt_al.h>
    #include <paho_mqtt_port.h>

    mqtt_init();
    mqtt_install_pahomqtt();
#endif

//////////////////////////  OC MQTT && DEMOS  //////////////////////////////////

#if CONFIG_OC_MQTT_ENABLE
    #include <oc_mqtt_al.h>
    oc_mqtt_init();

#if CONFIG_ATINY_MQTT_ENABLE
    #include <atiny_mqtt.h>
    oc_mqtt_install_atiny_mqtt();
#endif

#if CONFIG_OC_MQTT_EC20_ENABLE
    #include <ec20_oc.h>
	ec20_init();
#endif

#if CONFIG_OC_MQTT_DEMO_ENABLE
    #include <oc_mqtt_demo.h>
    oc_mqtt_demo_main();

#endif

#ifdef CONFIG_OC_DEVELOP_ENABLE
    extern int oc_develop_demo();
    oc_develop_demo();
#endif


#ifdef CONFIG_OC_OCEANLINK_ENABLE
    extern int oc_oceanlink_demo();
    oc_oceanlink_demo();
#endif

#endif

////////////////////////////  OC LWM2M && DEMOS     /////////////////////////////

#if CONFIG_OC_LWM2M_ENABLE
    #include <oc_lwm2m_al.h>
    oc_lwm2m_init();

#if CONFIG_OC_LWM2M_AGENT_ENABLE
    #include <agent_lwm2m.h>
    oc_lwm2m_install_agent();
#endif

#if CONFIG_OC_LWM2M_BOUDICA150_ENABLE
    #include <boudica150_oc.h>
    #define cn_app_bands    "5,8,20"
    boudica150_init(NULL,NULL,cn_app_bands);
#endif

#if CONFIG_OC_LWM2M_DEMO_ENABLE
    #include <oc_lwm2m_demo.h>
    oc_lwm2m_demo_main();
#endif

#if CONFIG_OC_LWM2M_CLOUD_MAP_ENABLE
    #include <oc_cloud_map_demo.h>
    oc_cloud_map_main();
#endif

#endif

#if CONFIG_HELLO_WORLD_ENABLE
    #include <helloworld.h>
    hello_world_main();

#endif
    return 0;
}


