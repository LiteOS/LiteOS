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
 *  2019-05-14 17:21  zhangqianfu  The first version  
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <osal.h>
#include <oc_lwm2m_al.h>

#define cn_endpoint_id        "lwm2m_001"
#define cn_app_server         "119.3.251.30"
#define cn_app_port           "5683"

#define cn_app_connectivity    0
#define cn_app_lightstats      1
#define cn_app_light           2
#define cn_app_ledcmd          3
#define cn_app_cmdreply        4

#pragma pack(1)
typedef struct
{
    int8_t msgid;
    int16_t rsrp;
    int16_t ecl;
    int16_t snr;
    int32_t cellid;
}app_connectivity_t;

typedef struct
{
    int8_t msgid;
    int16_t tog;
}app_toggle_t;

typedef struct
{
    int8_t msgid;
    int16_t intensity;
}app_light_intensity_t;


typedef struct
{
    int8_t msgid;
    uint16_t mid;
    char led[3];
}app_led_cmd_t;

typedef struct
{
    int8_t msgid;
    uint16_t mid;
    int8_t errorcode;
    char curstats[3];
}app_cmdreply_t;

#pragma pack()




//if your command is very fast,please use a queue here--TODO
#define cn_app_rcv_buf_len 128
static int             s_rcv_buffer[cn_app_rcv_buf_len];
static int             s_rcv_datalen;
static osal_semp_t     s_rcv_sync;

static void           *s_lwm2m_handle = NULL;



//use this function to push all the message to the buffer
static int app_msg_deal(void *usr_data,char *msg, int len)
{
    int ret = -1;

    if(len <= cn_app_rcv_buf_len)
    {
        if (msg[0] == 0xaa && msg[1] == 0xaa)
        {
            printf("OC respond message received! \n\r");
            return ret;
        }
        memcpy(s_rcv_buffer,msg,len);
        s_rcv_datalen = len;

        osal_semp_post(s_rcv_sync);

        ret = 0;

    }
    return ret;
}


static int app_cmd_task_entry()
{
    int ret = -1;
    app_led_cmd_t *led_cmd;
    app_cmdreply_t replymsg;
    int8_t msgid;

    while(1)
    {
        if(osal_semp_pend(s_rcv_sync,cn_osal_timeout_forever))
        {
            msgid = s_rcv_buffer[0] & 0x000000FF;
            switch (msgid)
            {
                case cn_app_ledcmd:
                    led_cmd = (app_led_cmd_t *)s_rcv_buffer;
                    printf("LEDCMD:msgid:%d mid:%d msg:%s \n\r",led_cmd->msgid,ntohs(led_cmd->mid),led_cmd->led);
                    //add command action--TODO
                    if (led_cmd->led[0] == 'O' && led_cmd->led[1] == 'N')
                    {
                        //if you need response message,do it here--TODO
                        replymsg.msgid = cn_app_cmdreply;
                        replymsg.mid = led_cmd->mid;
                        printf("reply mid is %d. \n\r",ntohs(replymsg.mid));
                        replymsg.errorcode = 0;
                        replymsg.curstats[0] = 'O';
                        replymsg.curstats[1] = 'N';
                        replymsg.curstats[2] = ' ';
                        oc_lwm2m_report(s_lwm2m_handle,(char *)&replymsg,sizeof(replymsg),1000);    ///< report cmd reply message
                    }

                    else if (led_cmd->led[0] == 'O' && led_cmd->led[1] == 'F' && led_cmd->led[2] == 'F')
                    {

                        //if you need response message,do it here--TODO
                        replymsg.msgid = cn_app_cmdreply;
                        replymsg.mid = led_cmd->mid;
                        printf("reply mid is %d. \n\r",ntohs(replymsg.mid));
                        replymsg.errorcode = 0;
                        replymsg.curstats[0] = 'O';
                        replymsg.curstats[1] = 'F';
                        replymsg.curstats[2] = 'F';
                        oc_lwm2m_report(s_lwm2m_handle,(char *)&replymsg,sizeof(replymsg),1000);    ///< report cmd reply message
                    }
                    else
                    {

                    }
                    break;
                default:
                    break;
            }
        }
    }

    return ret;
}



static int app_report_task_entry()
{
    int ret = -1;
    int lux = 0;

    oc_config_param_t      oc_param;
    app_light_intensity_t  light;

    memset(&oc_param,0,sizeof(oc_param));

    oc_param.app_server.ep_id = cn_endpoint_id;


    oc_param.boot_server.address = cn_app_server;
    oc_param.boot_server.port = cn_app_port;
    oc_param.boot_server.ep_id = cn_endpoint_id;

    oc_param.boot_mode = en_oc_boot_strap_mode_client_initialize;
    oc_param.rcv_func = app_msg_deal;

    s_lwm2m_handle = oc_lwm2m_config(&oc_param);

    if(NULL != s_lwm2m_handle)   //success ,so we could receive and send
    {
        //install a dealer for the led message received
        while(1) //--TODO ,you could add your own code here
        {

            osal_task_sleep(10*1000);

            lux++;
            lux= lux%10000;

            light.msgid = cn_app_light;
            light.intensity = htons(lux);
            oc_lwm2m_report(s_lwm2m_handle,(char *)&light,sizeof(light),1000); ///< report the light message
        }
    }

    return ret;
}




int oc_lwm2m_demo_main()
{
    osal_semp_create(&s_rcv_sync,1,0);

    osal_task_create("app_report",app_report_task_entry,NULL,0x1000,NULL,2);
    osal_task_create("app_command",app_cmd_task_entry,NULL,0x1000,NULL,3);

    return 0;
}



