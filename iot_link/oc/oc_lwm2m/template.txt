/*
 * app.c
 *
 *  Created on: 2019Äê4ÔÂ16ÈÕ
 *      Author: zhangqf
 */

#include <stdint.h>
#include <stddef.h>

#include <at.h>
#include <los_dev.h>
#include <osport.h>
#include <oc_lwm2m_agent.h>
#include <boudica150_oc.h>


#define cn_app_bands    "5,8,20"
#define cn_app_plmn     "46011";
#define cn_app_apn      "1,\"IP\",\"HUAWEI.COM\""

#define cn_app_server   "139.159.140.34"
#define cn_app_port     "5683"

#define cn_app_light           0
#define cn_app_ledcmd          1
#define cn_app_csq             2
#define cn_app_iocmd           3

#pragma pack(1)
typedef struct
{
    uint8_t msgid;
    char    intensity[5];
}tag_app_light_intensity;


typedef struct
{
    uint8_t msgid;
    char    led[3];
}tag_app_led_cmd;

typedef struct
{
    uint8_t msgid;
    char    csq[3];
}tag_app_net_csq;
#pragma pack()

//if your command is very fast,please use a queue here--TODO
#define cn_app_rcv_buf_len 128
static uint8_t    s_rcv_buffer[cn_app_rcv_buf_len];
static uint32_t   s_rcv_datalen;
static semp_t     s_rcv_sync;

//use this function to push all the message to the buffer
static int32_t app_msg_deal(uint8_t *msg, int32_t len)
{
    int32_t ret = -1;

    if(len <= cn_app_rcv_buf_len)
    {
        memcpy(s_rcv_buffer,msg,len);
        s_rcv_datalen = len;

        semp_post(s_rcv_sync);

        ret = 0;

    }
    return ret;
}


static int32_t app_cmd_task_entry()
{
    int32_t ret = -1;
    uint8_t msgid;

    tag_app_led_cmd *led_cmd;


    while(1)
    {
        if(semp_pend(s_rcv_sync,0x7fffffff))
        {
            msgid = s_rcv_buffer[0];

            switch (msgid)
            {
                case cn_app_ledcmd:

                    led_cmd = (tag_app_led_cmd *)s_rcv_buffer;
                    printf("LEDCMD:msgid:%d msg:%s \n\r",led_cmd->msgid,led_cmd->led);

                    //if you need response,do it here--TODO

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

    tag_oc_config_param  oc_param;

    tag_app_light_intensity  light;
    tag_app_net_csq          csq;
    //system frame work initilized here
    los_driv_init();      //which means you could use the driver framwork

    //do the at module initialize:use uart 2
    extern bool_t uart_at_init(s32_t baudrate);
    uart_at_init(9600);

    los_at_init("atdev"); //which means you could use the at framework

    //////////////////////APPLICATION INITIALIZE HERE/////////////////////
    oc_lwm2m_agent_init();
    boudica150_init(NULL,NULL,cn_app_bands);

    oc_param.server = cn_app_server;
    oc_param.port = cn_app_port;
    if(0 == oc_lwm2m_config(&oc_param))   //success ,so we could receive and send
    {
        //install a dealer for the led message received
        oc_lwm2m_install_msgdealer(app_msg_deal,NULL,0);

        while(1) //--TODO ,you could add your own code here
        {
            light.msgid = cn_app_light;
            memcpy(light.intensity,"12345",5);
            oc_lwm2m_report((uint8_t *)&light,sizeof(light),1000); //report the light message

            csq.msgid = cn_app_csq;
            memcpy(csq.csq,"012",3);
            oc_lwm2m_report((uint8_t *)&csq,sizeof(csq),1000); //report the light message

            task_sleepms(10*1000);
        }
    }

    return ret;
}



int app_init()
{
    semp_create(&s_rcv_sync,1,0);
    task_create("app_report",(fnTaskEntry)app_report_task_entry,0x1000,NULL,NULL,2);
    task_create("app_command",(fnTaskEntry)app_cmd_task_entry,0x1000,NULL,NULL,3);

    return 0;
}




