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

#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "bc95.h"

extern at_task at;
at_adaptor_api at_interface;
extern char rbuf[AT_DATA_LEN];
extern char wbuf[AT_DATA_LEN];

char tmpbuf[AT_DATA_LEN]={0}; //transform to hex

remote_info sockinfo[MAX_SOCK_NUM];

char pskidbuf[AT_DATA_LEN] = {0};
char ipbuf[AT_DATA_LEN] = {0};
char ipaddr[AT_DATA_LEN] = {0};
at_param nb_bc95_init[] =
{
    //inbuf,expectret,maxtry,sleeptime,timeout,outbuf,func
    //{"AT+QSECSWT=1,100","OK",1,0,10000,NULL,NULL},
    //{pskidbuf,"OK",1,0,10000,NULL,NULL},
    {"AT+CFUN?\r","+CFUN:1",12,0,AT_CMD_TIMEOUT,NULL,NULL},
    //{"AT+CGATT?\r","CGATT:1",120,0,AT_CMD_TIMEOUT,NULL,NULL},
    {"AT+CGPADDR\r","+CGPADDR:0,",12,0,AT_CMD_TIMEOUT,NULL,NULL},
    {ipbuf,"OK",1,0,AT_CMD_TIMEOUT,NULL,NULL},
    {"AT+NCDP?",ipaddr,1,0,AT_CMD_TIMEOUT,NULL,NULL},
    {"AT+NNMI=1\r","OK",1,0,AT_CMD_TIMEOUT,NULL,NULL}
};

at_param nb_reboot[] =
{
    {"AT+NRB\r","OK",1,0,AT_CMD_TIMEOUT,NULL,NULL}
};

char pskbuf[AT_DATA_LEN] = {0};
at_param nb_setpsk[] =
{
    {"AT+QSECSWT=1,100\r","OK",1,0,AT_CMD_TIMEOUT,NULL,NULL},
    {pskbuf,"OK",1,0,AT_CMD_TIMEOUT,NULL,NULL}
};

at_param nb_cleanpsk[] =
{
    {"AT+QSECSWT=0\r","OK",1,0,AT_CMD_TIMEOUT,NULL,NULL}
};

char payload[AT_DATA_LEN] = {0};
at_param nb_snddata[] =
{
    //inbuf,expectret,maxtry,sleeptime,timeout,outbuf,func
    {payload,"OK",1,0,AT_CMD_TIMEOUT,NULL,NULL}
    //{"AT+NQMGS\r","OK",1,0,AT_CMD_TIMEOUT,rbuf,NULL}

};

int str_to_hex(const char *bufin, int len, char *bufout)
{
    int i = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i++)
    {
        sprintf(bufout+i*2, "%02X", bufin[i]);
    }
    return 0;
}

int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("entry!");
    return 0;
}

void nb_setip(char *host, char *port)
{
    sprintf(ipaddr, "%s,%s\r", host, port);
    sprintf(ipbuf, "%s%s%c", "AT+NCDP=", ipaddr, '\r');
}

void nb_setpskbuf(char *pskid, char *psk)
{
    snprintf(pskbuf, AT_DATA_LEN, "%s=%s,%s\r", "AT+QSETPSK", pskid, psk);
}

void nb_setsnddata(const char* buf, int len)
{
    memset(tmpbuf, 0, AT_DATA_LEN);
    memset(payload, 0, AT_DATA_LEN);
    str_to_hex(buf, len, tmpbuf);
    snprintf(payload, AT_DATA_LEN,"%s%d,%s%c","AT+NMGS=",(int)len,tmpbuf,'\r');
}

int32_t nb_init(void)
{
    at.init();
    memset(at_cmds, 0, AT_PAR_MAX*sizeof(at_cmd_t));
    at_cmd_register(NB_REBOOT,nb_reboot,1);
    at_cmd_register(NB_INIT,nb_bc95_init,5);
    at_cmd_register(NB_SND,nb_snddata,1);
    at_cmd_register(NB_SETPSK,nb_setpsk,2);
    at_cmd_register(NB_CLEANPSK,nb_cleanpsk,1);
    return 0;
}

at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart_port = AT_USART_PORT,
    .buardrate = AT_BUARDRATE,
    .linkid_num = AT_MAX_LINK_NUM,
    .user_buf_len = MAX_AT_USERDATA_LEN,
    .cmd_begin = AT_CMD_BEGIN,
    .line_end = AT_LINE_END,
    .mux_mode = 1, //support multi connection mode
    .timeout = AT_CMD_TIMEOUT,   //  ms
};

at_adaptor_api at_interface = {
    .init = NULL,

    .connect = NULL,
    .send = NULL,

    .recv_timeout = NULL,
    .recv = NULL,

    .close = NULL,
    .recv_cb = NULL,

    .deinit = NULL,
};
#endif
