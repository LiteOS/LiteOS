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

#include <string.h>
#include <at.h>
#include <ec20_oc.h>
#include <oc_mqtt_al.h>
#include <osal.h>

#define cn_ec20_cmd_timeout  (2*1000)

static void  *s_oc_handle = NULL;


static bool_t ec20_atcmd(const char *cmd,const char *index)
{
    int ret = 0;
    ret = at_command((unsigned char *)cmd,strlen(cmd),index,NULL,0,cn_ec20_cmd_timeout);
    if(ret >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


static void* ec20_oc_config(tag_oc_mqtt_config *param)
{
    int ret = 0;
    char cmd[256];

    if(NULL != param)
    {
        memset(cmd,0,256);
        snprintf(cmd,256,"at+HWOCMQTTCONNECT=%d,%d,\"%s\",\"%s\",\"%s\",\"%s\"\r", param->boot_mode, param->lifetime, param->server, param->port, param->device_info.s_device.deviceid, param->device_info.s_device.devicepasswd);
        printf("cmd:%s\r\n", cmd);
        //ec20_atcmd("at+HWOCMQTTCONNECT=1,30,\"iot-bs.cn-north-4.myhuaweicloud.com\",\"8883\",\"sdk_0040\",\"f62fcf47d62c4ed18913\"","OK");
        ret = ec20_atcmd(cmd,"OK");
        if(ret)
            s_oc_handle = (void *)0xffffffff;
    }
    return s_oc_handle;
}


static int ec20_oc_deconfig(void *handle)
{
    s_oc_handle = NULL;
    return 0;
}

static int ec20_oc_report(void *handle,char *msg,int len,en_mqtt_al_qos_t qos)
{
    int ret = -1;

    char cmd[256];

    memset(cmd,0,256);
    snprintf(cmd,256,"AT+HWOCMQTTSEND=%d,%d,\"%s\"\r", qos, len, msg);

    //ec20_atcmd("at+HWOCMQTTCONNECT=1,30,\"iot-bs.cn-north-4.myhuaweicloud.com\",\"8883\",\"sdk_0040\",\"f62fcf47d62c4ed18913\"","OK");
    //ec20_atcmd("AT+HWOCMQTTSEND=0,116,\"{\"msgType\":\"deviceReq\",\"hasMore\":0,\"data\":[{\"serviceId\":\"LED\",\"serviceData\":{\"LED1\":1,\"LED2\":1,\"LED3\":1,\"LED4\":1}}]}\"","OK");
    printf("cmd:%s\r\n", cmd);

    ret = ec20_atcmd(cmd,"OK");

    if(ret > 0)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}



const tag_oc_mqtt_ops  g_ec20_oc_opt = \
{
    .config = ec20_oc_config,
    .deconfig = ec20_oc_deconfig,
    .report = ec20_oc_report,
};


int ec20_init(void)
{
    int ret = -1;

    ret = oc_mqtt_register(&g_ec20_oc_opt);

    return ret;
}

