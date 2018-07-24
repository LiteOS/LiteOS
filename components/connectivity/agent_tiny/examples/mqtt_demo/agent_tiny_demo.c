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
#include "atiny_log.h"

void message_cb(cloud_msg_t *msg);

#define DEFAULT_SERVER_IPV4 "192.168.1.100"
#ifdef WITH_DTLS
#define DEFAULT_SERVER_PORT "8883"
#define AGENT_TINY_DEMO_PSK_ID "testID"
#define AGENT_TINY_DEMO_PSK_LEN (3)
unsigned char g_demo_psk[AGENT_TINY_DEMO_PSK_LEN] = {0xab, 0xcd, 0xef};
#else
#define DEFAULT_SERVER_PORT "1883"
#endif /* WITH_DTLS */

#define AGENT_TINY_DEMO_CLIENT_ID "liteos_test"
#define AGENT_TINY_DEMO_USERNAME NULL
#define AGENT_TINY_DEMO_PASSWORD NULL

#define AGENT_TINY_DEMO_PUB_TOPIC "/pub_test"
#define AGENT_TINY_DEMO_SUB_TOPIC "/helloworld"

static void* g_phandle = NULL;
static atiny_device_info_t g_device_info;
static atiny_param_t g_atiny_params;

atiny_interest_uri_t g_interest_uris[ATINY_INTEREST_URI_MAX_NUM] = 
{
    {
        .uri = AGENT_TINY_DEMO_SUB_TOPIC,
        .qos = CLOUD_QOS_MOST_ONCE,
        .cb = message_cb
    },
};

void message_cb(cloud_msg_t *msg)
{
    ATINY_LOG(LOG_DEBUG, "%.*s : %.*s", msg->uri_len, msg->uri, msg->payload_len,  (char *)msg->payload);
}

void app_data_report(void)
{
    cloud_msg_t report_data;
    int ret;
    int cnt = 0;
    char payload[30] = {0};

    report_data.uri = AGENT_TINY_DEMO_PUB_TOPIC;
    report_data.qos = CLOUD_QOS_MOST_ONCE;
    report_data.method = CLOUD_METHOD_POST;
    report_data.payload = payload;
    while(1)
    {
        sprintf(payload, "publish message number %d", cnt);
        report_data.payload_len= strlen(payload);
        payload[report_data.payload_len] = '\0';
        cnt++;
        ret = atiny_data_send(g_phandle, &report_data, NULL);
        ATINY_LOG(LOG_DEBUG, "report ret:%d", ret);
        (void)LOS_TaskDelay(250*2);
    }
}

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

void agent_tiny_entry(void)
{
    UINT32 uwRet = LOS_OK;
    atiny_param_t* atiny_params;
    atiny_device_info_t *device_info = &g_device_info;

    device_info->client_id = AGENT_TINY_DEMO_CLIENT_ID;
    device_info->user_name = AGENT_TINY_DEMO_USERNAME;
    device_info->password = AGENT_TINY_DEMO_PASSWORD;
    device_info->will_flag = MQTT_WILL_FLAG_FALSE;
    device_info->will_options = NULL;
    memcpy(device_info->interest_uris, g_interest_uris, sizeof(g_interest_uris));

    atiny_params = &g_atiny_params;
    atiny_params->server_ip = DEFAULT_SERVER_IPV4;
    atiny_params->server_port = DEFAULT_SERVER_PORT;
#ifdef WITH_DTLS
    atiny_params->security_type = CLOUD_SECURITY_TYPE_PSK;
    atiny_params->u.psk.psk_id = (unsigned char *)AGENT_TINY_DEMO_PSK_ID;
    atiny_params->u.psk.psk_id_len = strlen(AGENT_TINY_DEMO_PSK_ID);
    atiny_params->u.psk.psk = g_demo_psk;
    atiny_params->u.psk.psk_len = AGENT_TINY_DEMO_PSK_LEN;
#else
    atiny_params->security_type = CLOUD_SECURITY_TYPE_NONE;
#endif /* WITH_DTLS */

    if(ATINY_OK != atiny_init(atiny_params, &g_phandle))
    {
        return;
    }

    uwRet = creat_report_task();
    if(LOS_OK != uwRet)
    {
        return;
    }

    (void)atiny_bind(device_info, g_phandle);
    return ;
}
