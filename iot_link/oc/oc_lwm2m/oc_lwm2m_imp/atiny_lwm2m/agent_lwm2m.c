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
 *  2019-05-21 16:25  zhangqianfu  The first version  
 *
 */

#include <osal.h>
#include <oc_lwm2m_al.h>
#include <agenttiny.h>


///< uptils now, we don't support the multi handle mode

typedef struct
{
    atiny_device_info_t device_info;
    atiny_param_t       tiny_param;
    oc_config_param_t   config_para;
    void               *agent_handle;
    void               *task_handle;
}oc_lwm2m_imp_agent_t;

static oc_lwm2m_imp_agent_t  *s_oc_lwm2m_agent = NULL;


int lwm2m_agent_receive(char *msg, int len)
{
    if((NULL != s_oc_lwm2m_agent) && (NULL != s_oc_lwm2m_agent->config_para.rcv_func))
    {
        s_oc_lwm2m_agent->config_para.rcv_func(s_oc_lwm2m_agent->config_para.usr_data,msg,len);
    }

    return 0;
}



static int __agent_task_entry(void *args)
{
    oc_lwm2m_imp_agent_t  *ret = NULL;
    ret = args;

    atiny_bind(&ret->device_info, ret->agent_handle);

    while(1)
    {
        osal_task_sleep(1000);
    }

    return 0;
}


static void *__agent_config(oc_config_param_t *param)
{

    oc_lwm2m_imp_agent_t  *ret = NULL;

    if(NULL != s_oc_lwm2m_agent)
    {
        return ret;
    }

    ret = osal_zalloc(sizeof(oc_lwm2m_imp_agent_t));

    if(NULL == ret)
    {
        return ret;
    }

    ret->config_para = *param;
    ///< initialize the param that agent tiny need
    atiny_param_t *atiny_params;
    atiny_security_param_t  *iot_security_param = NULL;
    atiny_security_param_t  *bs_security_param = NULL;

    atiny_device_info_t *device_info = &ret->device_info;
    device_info->endpoint_name = ret->config_para.app_server.ep_id;
    device_info->manufacturer = "Lwm2mFota";
    device_info->dev_type = "Lwm2mFota";
    device_info->manufacturer = "Agent_Tiny";

    atiny_params = &ret->tiny_param;
    atiny_params->server_params.binding = "UQ";
    atiny_params->server_params.life_time = 20;
    atiny_params->server_params.storing_cnt = 0;

    atiny_params->server_params.bootstrap_mode = param->boot_mode;
    atiny_params->server_params.hold_off_time = 10;

    //pay attention: index 0 for iot server, index 1 for bootstrap server.
    iot_security_param = &(atiny_params->security_params[0]);
    bs_security_param = &(atiny_params->security_params[1]);


    iot_security_param->server_ip = ret->config_para.app_server.address;
    iot_security_param->server_port = ret->config_para.app_server.port;
    iot_security_param->psk_Id = ret->config_para.app_server.psk_id;
    iot_security_param->psk = ret->config_para.app_server.psk;
    iot_security_param->psk_len = ret->config_para.app_server.psk_len;

    bs_security_param->server_ip = ret->config_para.boot_server.address;
    bs_security_param->server_port = ret->config_para.boot_server.port;
    bs_security_param->psk_Id = ret->config_para.boot_server.psk_id;
    bs_security_param->psk = ret->config_para.boot_server.psk;
    bs_security_param->psk_len = ret->config_para.boot_server.psk_len;


    if(ATINY_OK != atiny_init(atiny_params, &ret->agent_handle))
    {
        osal_free(ret);
        ret = NULL;

        return ret;
    }

    ///< create the task to do the instance

    ret->task_handle = osal_task_create("agent_lwm2m",__agent_task_entry,ret,0x1000,NULL,10);
    if(NULL == ret->task_handle)
    {
        atiny_deinit(ret->agent_handle);
        osal_free(ret);
        ret = NULL;
        return ret;
    }

    s_oc_lwm2m_agent= ret;

    return ret;
}

static int __agent_deconfig(void *handle)
{
    oc_lwm2m_imp_agent_t  *ret = handle;

    if(NULL == handle)
    {
        return -1;
    }

    atiny_deinit(ret->agent_handle);

    osal_task_kill(ret->task_handle);

    osal_free(ret);

    s_oc_lwm2m_agent = NULL;

    return 0;
}

void ack_callback(atiny_report_type_e type, int cookie, data_send_status_e status)
{
    printf("type:%d cookie:%d status:%d\n", type, cookie, status);
}

static int __agent_report(void *handle, char *msg, int len, int timeout)
{

    oc_lwm2m_imp_agent_t  *agent = handle;
    data_report_t report_data;

    printf("%s:report:len:%d  timeout:%d\n\r",__FUNCTION__,len,timeout);

    report_data.buf = (uint8_t *)msg;
    report_data.callback = ack_callback;
    report_data.cookie = 0;
    report_data.len = len;
    report_data.type = APP_DATA;

    if(ATINY_OK  != atiny_data_report(agent->task_handle, &report_data))
    {
        return -1;
    }
    else
    {
        return 0;
    }

}

const oc_lwm2m_opt_t  s_oc_lwm2m_agent_opt = \
{
    .config = __agent_config,
    .deconfig = __agent_deconfig,
    .report = __agent_report,
};

int oc_lwm2m_install_agent()
{
    int ret = -1;

    ret = oc_lwm2m_register("oc_lwm2m_agent",&s_oc_lwm2m_agent_opt);

    return ret;
}
