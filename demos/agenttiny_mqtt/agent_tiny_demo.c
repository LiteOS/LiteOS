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
#include "agent_tiny_demo.h"
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "atiny_mqtt/mqtt_client.h"
#include "osdepends/atiny_osdep.h"
#include "log/atiny_log.h"

//#define MQTT_DEMO_CONNECT_DYNAMIC

#define DEFAULT_SERVER_IPV4 "192.168.204.45"
#ifdef WITH_DTLS
#define DEFAULT_SERVER_PORT "8883"
#define AGENT_TINY_DEMO_PSK_ID "testID"
#define AGENT_TINY_DEMO_PSK_LEN (3)
unsigned char g_demo_psk[AGENT_TINY_DEMO_PSK_LEN] = {0xab, 0xcd, 0xef};
#else
#define DEFAULT_SERVER_PORT "1883"
#endif /* WITH_DTLS */


#ifndef MQTT_DEMO_CONNECT_DYNAMIC
#define AGENT_TINY_DEMO_PASSWORD "123"
#else
#define AGENT_TINY_DEMO_PASSWORD "1234"
#endif

#define AGENT_TINY_DEMO_DEVICEID "11223344"
#define AGENT_TINY_DEMO_PRODUCTID "112233"
#define AGENT_TINY_DEMO_NODEID "1122"



static mqtt_client_s *g_phandle = NULL;
static demo_param_s g_demo_param;




/*lint -e550*/
void app_data_report(void)
{
    int ret = 0;
    const char *msg = "hello";

    while(1)
    {
        if(g_phandle)
        {
            ret = atiny_mqtt_data_send(g_phandle, msg, strlen(msg), MQTT_QOS_ONLY_ONCE);
            ATINY_LOG(LOG_INFO, "report ret:%d", ret);
        }
        else
        {
            ATINY_LOG(LOG_ERR, "g_phandle null");
        }
        (void)LOS_TaskDelay(10 * 1000);
    }
}
/*lint +e550*/

UINT32 creat_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "app_data_report";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)app_data_report;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

static int demo_get_time(char *time, int32_t len)
{
    const int32_t min_time_len = 11;
    if ((time == NULL) || (len < min_time_len))
    {
        ATINY_LOG(LOG_ERR,"invalid param len %d", len);
        return ATINY_ERR;
    }
    (void)strncpy(time, "2018111517", len);
    return ATINY_OK;
}

static int demo_rcv_msg(const uint8_t *msg, int32_t len)
{
    char* print_msg;
    if ((msg == NULL) && len > 0)
    {
        ATINY_LOG(LOG_ERR, "invalid param len %ld", len);
        return ATINY_ERR;
    }
    if (msg == NULL)
    {
        msg = (const uint8_t *)"";
    }

    print_msg = atiny_malloc(len + 1);
    if (print_msg == NULL)
    {
        ATINY_LOG(LOG_ERR, "atiny_malloc null %ld", len);
        return ATINY_ERR;
    }
    if (len > 0)
    {
        memcpy(print_msg, msg, len);
    }
    print_msg[len] = '\0';
    ATINY_LOG(LOG_INFO, "rcv msg: len = %ld, msg:%s", len, (char *)msg);
    atiny_free(print_msg);
    return ATINY_OK;
}




static int demo_cmd_ioctl(mqtt_cmd_e cmd, void *arg, int32_t len)
{
    int result = ATINY_ERR;

//TODO:
    switch(cmd)
        {
        case MQTT_GET_TIME:
            result = demo_get_time(arg, len);
            break;
        case MQTT_RCV_MSG:
            result = demo_rcv_msg(arg, len);
            break;
        case MQTT_WRITE_FLASH_INFO:
            if (g_demo_param.write_flash_info == NULL)
            {

                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "write_flash_info null");
            }
            else
            {
                result = g_demo_param.write_flash_info(arg, len);
            }
            break;
        case MQTT_READ_FLASH_INFO:
            if (g_demo_param.read_flash_info == NULL)
            {

                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "read_flash_info null");
            }
            else
            {
                result = g_demo_param.read_flash_info(arg, len);
            }
            break;

        default:
            break;
        }
    return result;
}

void agent_tiny_entry(void)
{
    UINT32 uwRet = LOS_OK;
    mqtt_param_s atiny_params;
    mqtt_device_info_s device_info;

    if (g_demo_param.init)
    {
        g_demo_param.init();
    }

    atiny_params.server_ip = DEFAULT_SERVER_IPV4;
    atiny_params.server_port = DEFAULT_SERVER_PORT;
#ifdef WITH_DTLS
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_PSK;
    atiny_params.info.u.psk.psk_id = (unsigned char *)AGENT_TINY_DEMO_PSK_ID;
    atiny_params.info.u.psk.psk_id_len = strlen(AGENT_TINY_DEMO_PSK_ID);
    atiny_params.info.u.psk.psk = g_demo_psk;
    atiny_params.info.u.psk.psk_len = AGENT_TINY_DEMO_PSK_LEN;
#else
    atiny_params.info.security_type = MQTT_SECURITY_TYPE_NONE;
#endif /* WITH_DTLS */
    atiny_params.cmd_ioctl = demo_cmd_ioctl;

    if(ATINY_OK != atiny_mqtt_init(&atiny_params, &g_phandle))
    {
        return;
    }

    uwRet = creat_report_task();
    if(LOS_OK != uwRet)
    {
        return;
    }

    device_info.codec_mode = MQTT_CODEC_MODE_BINARY;
    device_info.sign_type = MQTT_SIGN_TYPE_HMACSHA256_NO_CHECK_TIME;
    device_info.password = AGENT_TINY_DEMO_PASSWORD;
#ifndef MQTT_DEMO_CONNECT_DYNAMIC
    device_info.connection_type = MQTT_STATIC_CONNECT;
    device_info.u.s_info.deviceid  = AGENT_TINY_DEMO_DEVICEID;
#else
    device_info.connection_type = MQTT_DYNAMIC_CONNECT;
    device_info.u.d_info.productid = AGENT_TINY_DEMO_PRODUCTID;
    device_info.u.d_info.nodeid = AGENT_TINY_DEMO_NODEID;
#endif
    (void)atiny_mqtt_bind(&device_info, g_phandle);
    return ;
}

void agent_tiny_demo_init(const demo_param_s *param)
{
    g_demo_param = *param;
}

