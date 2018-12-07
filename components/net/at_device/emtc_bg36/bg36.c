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
#include <ctype.h>
#if defined(WITH_AT_FRAMEWORK)
#include "bg36.h"

extern char rbuf[AT_DATA_LEN];
extern char wbuf[AT_DATA_LEN];
extern at_task at;
at_adaptor_api at_interface;
#define MAX_BG36_SOCK_NUM 5
#define UDP_PROTO   17
#define MAX_SOCK_NUM 11
#define MAX_SOCKID 11
at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart_port = AT_USART_PORT,
    .buardrate = AT_BUARDRATE,
    .linkid_num = AT_MAX_LINK_NUM,
    .user_buf_len = MAX_AT_USERDATA_LEN,
    .cmd_begin = AT_CMD_BEGIN,
    .line_end = AT_LINE_END,
    .mux_mode = 1, //support multi connection mode
    .timeout = BG36_TIMEOUT,   //  ms
};

typedef struct
{
    uint32_t data_len;
    int link_idx;
    bool valid_flag;
}emtc_data_ind_info_s;

emtc_socket_info sockinfo[MAX_SOCK_NUM];
static emtc_data_ind_info_s data_ind_info;
static int sockid = 0;

int bg36_cmd(char *cmd, int32_t len, const char *suffix, char *resp_buf, int* resp_len)
{
	return at.cmd((int8_t *)cmd, len, suffix, resp_buf, resp_len);
}

static int bg36_alloc_sock(int socket)
{
    int idx;
    uint32_t i;

    for (i = 0; i < MAX_BG36_SOCK_NUM; ++i)
    {
        if (sockinfo[i].used_flag  && (sockinfo[i].socket == socket))
        {
            return i;
        }
    }

    idx  = (socket % MAX_BG36_SOCK_NUM);
    if (!sockinfo[idx].used_flag)
    {
        sockinfo[idx].used_flag = 1;
        return idx;
    }

    for (i = 0; i < MAX_BG36_SOCK_NUM; ++i)
    {
        if (!sockinfo[i].used_flag)
        {
            sockinfo[i].used_flag = 1;
            return i;
        }
    }
    AT_LOG("save socket fail %d", socket);
    return MAX_BG36_SOCK_NUM;
}

static int bg36_sock_to_idx(int socket)
{
    int idx;

    idx  = (socket % MAX_SOCK_NUM);

    if (sockinfo[idx].used_flag && (socket == sockinfo[idx].socket))
    {
        return idx;
    }

    for (uint32_t i = 0; i < MAX_SOCK_NUM; ++i)
    {
        if (sockinfo[i].used_flag && (socket == sockinfo[i].socket))
        {
            return i;
        }
    }

    return MAX_SOCK_NUM;
}

static int32_t bg36_cmd_with_2_suffix(const int8_t *cmd, int  len,
                        const char* suffix_ok, const char* suffix_err,  char *resp_buf, uint32_t* resp_len)
{

    const char *suffix[2] = {0};
    at_cmd_info_s cmd_info = {0};

    suffix[0] = suffix_ok;
    suffix[1] = suffix_err;

    cmd_info.suffix = suffix;
    cmd_info.suffix_num = array_size(suffix);

    cmd_info.resp_buf = resp_buf;
    cmd_info.resp_len = resp_len;

	if (at.cmd_multi_suffix(cmd, len, &cmd_info) != AT_OK)
    {
        return AT_FAILED;
    }

    if (cmd_info.match_idx != 0)
    {
        AT_LOG("cmd_info.match_idx %d", cmd_info.match_idx);
        return AT_FAILED;
    }

    return AT_OK;
}

static void bg36_close_sock(int sock)
{
    const char *cmd = "AT+QICLOSE=";
    char buf[64];
    int cmd_len;

    cmd_len = snprintf(buf, sizeof(buf), "%s%d\r", cmd, sock);
    bg36_cmd_with_2_suffix((int8_t*)buf, cmd_len, "OK", "ERROR", NULL,NULL);
}

//Direct Push Mode
int32_t bg36_data_handler(void *arg, int8_t *buf, int32_t len)
{
    int32_t sockid=0;
    int32_t data_len=0;
    const char *p1 = NULL, *p2 = NULL;
    int link_idx=0;
    QUEUE_BUFF qbuf;
    int ret=0;
    //int i=0;
    int offset = 0;
    //+QIURC: "recv",0,6
    //aaaaaa

    //p2+=strlen(AT_DATAF_PREFIX);
    while(offset < len)
    {
        p1 = strstr((char *)(buf+offset), "recv");
        if (p1 == NULL)
        {
            AT_LOG("not recv, offset:%d len:%d",offset, (int)len);
            return AT_OK;
        }
        p1 += strlen("\"recv\"");
        sockid = chartoint(p1+1);
        link_idx = bg36_sock_to_idx(sockid);
        if (link_idx >= MAX_SOCK_NUM)
        {
            AT_LOG("invalid sock id %ld", sockid);
            return AT_OK;
        }
        p2 = strstr((char *)(p1+1), ",");
        if (p2 == NULL)
        {
            AT_LOG("invalid data %ld", sockid);
            return AT_FAILED;
        }
        data_len = chartoint(p2+1);
        qbuf.addr = at_malloc(data_len);
        if (qbuf.addr == NULL)
        {
            AT_LOG("at_malloc null");
            return AT_OK;
        }

        p1 = strstr(p2, "\r\n");

        if (p1 != NULL)
        {
            memcpy(qbuf.addr, p1+2, data_len);
            qbuf.len = data_len;
            ret = LOS_QueueWriteCopy(at.linkid[link_idx].qid, &qbuf, sizeof(qbuf), 0);
            if (LOS_OK != ret)
            {
                AT_LOG("LOS_QueueWriteCopy failed! ret %d", ret);
                at_free(qbuf.addr);
            }
            offset+=data_len+strlen("+QIURC: \"recv\",");

        }
        else
        {
            AT_LOG("recv data null!");
            return AT_OK;
        }
    }

    return AT_OK;
}

int32_t bg36_cmd_match(const char *buf, char* featurestr,int len)
{
    return (strstr((char *)buf, featurestr) != NULL) ? 0: -1;
}

int send_sock_cmd(const int8_t * host, int port_i, char* service_type)
{
    int rbuflen = 64;
    int cnt = 0;
    char inbuf[64] = {0};
    int err = -1;
    char cmd[64] = {0};
    char tmpbuf[32] = {0};
    int conid;
    char* sockptr;
    while((err != 0) && (++cnt < 50))
    {
        snprintf(cmd, 64, "%s,%d,\"%s\",\"%s\",%d,0,1\r", QIOPEN_SOCKET, sockid % MAX_SOCKID, service_type, host, port_i);
        bg36_cmd(cmd, strlen(cmd), "+QIOPEN:", inbuf,&rbuflen);
        AT_LOG("inbuf:%s",inbuf);
        sockptr = strstr(inbuf,"+QIOPEN:");
        if(sockptr != NULL)
            sscanf(sockptr,"+QIOPEN: %d,%d%s",&conid,&err, tmpbuf);
        AT_LOG("conid:%d err:%d",conid,err);
        if(err)
            sockid++;
    }

    return (err == 0) ? conid : AT_FAILED;

}
int32_t bg36_create_socket(const int8_t * host, const int8_t *port, int32_t proto, char* service_type, int with_force)
{
    int ret = AT_FAILED;
    int port_i = 0;
    int rbuflen = 64;
    char inbuf[64] = {0};
    char tmpbuf[32] = {0};
    int conid, err;
    char* str;
    char cmd[64] = {0};//AT+QIOPEN=1,0,"TCP","220.180.239.212",8009,0,1
    sscanf((char*)port, "%d", &port_i);
    AT_LOG("get port = %d\r\n", port_i);

    if (at.mux_mode != AT_MUXMODE_MULTI)
    {
        AT_LOG("Only support in multi mode!\r\n");
        return -1;
    }

    int id = at.get_id();
    ret = LOS_QueueCreate("dataQueue", 16, &at.linkid[id].qid, 0, sizeof(QUEUE_BUFF));
    if (ret != LOS_OK)
    {
        AT_LOG("init dataQueue failed!");
        at.linkid[id].usable = AT_LINK_UNUSE;
        return AT_FAILED;
    }

    snprintf(cmd, 64, "%s,%d,\"%s\",\"%s\",%d,0,1\r", QIOPEN_SOCKET, (sockid++) % MAX_SOCKID, service_type, host, port_i);
    bg36_cmd(cmd, strlen(cmd), "+QIOPEN", inbuf,&rbuflen);
    str = strstr(inbuf, "+QIOPEN");
    if (str == NULL)
    {
        if(with_force)
        {
            send_sock_cmd(host, port_i, service_type);
        }
        else
        {
            AT_LOG("open socket failed");
            return AT_FAILED;
        }
    }
    sscanf(str,"+QIOPEN: %d,%d%s",&conid,&err,tmpbuf);//¡°+QIOPEN: <connectID>,<err>
    //AT_LOG("conid:%d err:%d",conid,err);
    if(err != 0)
    {
        if(with_force)
        {
            conid = send_sock_cmd(host, port_i, service_type);
        }
        else
        {
            AT_LOG("open socket err:%d",err);
            return AT_FAILED;
        }

    }
    if (conid < 0)
    {
        AT_LOG("create socket failed");
        return AT_FAILED;
    }

    ret = bg36_alloc_sock(conid);
    if (ret >= MAX_SOCK_NUM)
    {

        AT_LOG("sock num exceeded,socket is %d", conid);
        goto CLOSE_SOCk;
    }

    sockinfo[ret].socket = conid;
    sockinfo[ret].used_flag = true;
    AT_LOG("create socket success!\nsockid:%d ret:%d",conid,ret);
    return ret;

CLOSE_SOCk:
    AT_LOG("create socket failed!\nsockid:%d ret:%d",conid,ret);
    bg36_close_sock(conid);
    return AT_FAILED;

}

int32_t bg36_bind(const int8_t * host, const int8_t *port, int32_t proto)
{
    AT_LOG("DIDNT go bind!");
    return bg36_create_socket(host, port, proto, "TCP", 1);
}

int32_t bg36_connect(const int8_t * host, const int8_t *port, int32_t proto)
{
    char *cmd2 = "AT+QISTATE=1,";
    char cmd[64] = {0};
    int ret;
    ret = bg36_create_socket(host, port, proto, "TCP", 1);
    if(ret < 0 || ret >= 11)
    {
        return AT_FAILED;
    }
    snprintf(cmd, 64, "%s%d\r", cmd2,sockinfo[ret].socket);
    bg36_cmd(cmd, strlen(cmd), "+QISTATE:", NULL, NULL);
    return ret;
}

int32_t bg36_send(int32_t id , const uint8_t *buf, uint32_t len)
{
    char *cmd1 = "AT+QISEND=0,";
    char cmd[64] = {0};
	if (id >= MAX_SOCK_NUM)
    {
        AT_LOG("invalid args");
        return AT_FAILED;
    }
    snprintf(cmd, 64,"%s%d%c",cmd1,(int)len,'\r');
    (void)bg36_cmd(cmd, strlen(cmd), ">", NULL, NULL);
    bg36_cmd((char *)buf, len, "OK", NULL, NULL);
    return len;
}

int32_t bg36_recv_timeout(int32_t id , uint8_t  *buf, uint32_t len,char* ipaddr,int* port, int32_t timeout)
{
    int rlen =0;
    int copylen =0;
    int ret;
    QUEUE_BUFF  qbuf;
    UINT32 qlen = sizeof(QUEUE_BUFF);

    if (id  >= MAX_SOCK_NUM)
    {
        AT_LOG("link id %d invalid", (int)id);
        return AT_FAILED;
    }
    //AT_LOG("\n\ntimeout:%d\n\n",timeout);
    if (sockinfo[id].buf == NULL)
    {
        ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout);
        if (ret != LOS_OK)
        {
             return AT_TIMEOUT;
        }
        AT_LOG("Read queue len:%d",(int)qbuf.len);
        sockinfo[id].buf = (char*)qbuf.addr;
        sockinfo[id].len = qbuf.len;
        sockinfo[id].offset = 0;
    }

    if(sockinfo[id].len - sockinfo[id].offset > len)
    {
        memcpy(buf, sockinfo[id].buf + sockinfo[id].offset, len);
        sockinfo[id].offset += len;
        return len;
    }
    else
    {
        copylen = sockinfo[id].len - sockinfo[id].offset;
        memcpy(buf, sockinfo[id].buf + sockinfo[id].offset, copylen);
        at_free(sockinfo[id].buf);
        sockinfo[id].offset = 0;
        sockinfo[id].buf = NULL;
        sockinfo[id].len = 0;
        rlen = copylen;
        while(rlen < len)
        {
            ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, 0);
            if (ret == LOS_OK)
            {
                sockinfo[id].buf = (char*)qbuf.addr;
                sockinfo[id].len = qbuf.len;
                sockinfo[id].offset = 0;
                if(len-rlen < qbuf.len)
                {
                    memcpy(buf+rlen, sockinfo[id].buf, len - rlen);
                    sockinfo[id].offset = len - rlen;
                    return len;
                }
                else
                {
                    memcpy(buf+rlen, sockinfo[id].buf, qbuf.len);
                    rlen += qbuf.len;
                    at_free(sockinfo[id].buf);
                    sockinfo[id].offset = 0;
                    sockinfo[id].buf = NULL;
                    sockinfo[id].len = 0;
                }
            }
            else
            {
                return rlen;
            }
        }

        return copylen;
    }
}

int32_t bg36_recv(int32_t id , uint8_t  *buf, uint32_t len)
{
    return bg36_recv_timeout(id, buf, len, NULL, NULL, LOS_WAIT_FOREVER);
}

int32_t bg36_close(int32_t id)
{
    char *cmd = "AT+QICLOSE=";
    char cmd1[64] = {0};
    int ret;
    snprintf(cmd1, 64,"%s%d\r",cmd,(int)id);
    ret = bg36_cmd(cmd1, strlen(cmd1), "OK", NULL, NULL);
    if(!ret)
    {
        sockinfo[(id % MAX_BG36_SOCK_NUM)].used_flag = 0;
    }
    return ret;
}
static int32_t bg36_init(void)
{
    int ret = AT_OK;
    int rbuflen = 64;
    char inbuf[64] = {0};
    char tmpbuf[64] = {0};
    int creg = 0;

    at.init(&at_user_conf);
    memset(&sockinfo, 0, sizeof(sockinfo));
    memset(&data_ind_info, 0, sizeof(emtc_data_ind_info_s));
    at.oob_register(AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX), bg36_data_handler, bg36_cmd_match);
    ret = bg36_cmd(ATI, strlen(ATI), "OK", NULL, NULL);
    ret = bg36_cmd(ATE0, strlen(ATE0), "OK", NULL, NULL);
    ret = bg36_cmd(CPIN, strlen(CPIN), "+CPIN: READY", NULL, NULL);
    while(1)
    {
        ret = bg36_cmd(QUERYCFATT, strlen(QUERYCFATT), "+CGATT", inbuf,&rbuflen);
        //bg36_cmd(QPING, strlen(QPING), "OK", NULL, NULL);
        if(strlen(inbuf)!=0)
            sscanf(inbuf,"\r\n+CGATT: %d\r\n%s",&creg,tmpbuf);
        AT_LOG("creg:%d", creg);
        if(creg == 1)
            break;
        memset(inbuf, 0, 64);
    }

    bg36_close(0);
    bg36_close(1);
    bg36_close(2);
    bg36_close(3);
    bg36_close(4);
    bg36_close(5);
    bg36_close(6);
    bg36_close(7);
    bg36_close(8);
    bg36_close(9);
    bg36_close(10);
    bg36_close(11);
    ret = bg36_cmd(QIACTQUERY, strlen(QIACTQUERY), "OK", NULL, NULL);

    memset(inbuf, 0, 64);

    return ret;
}

at_adaptor_api emtc_bg36_interface =
{
    .init = bg36_init,
    .bind = bg36_bind,
    .connect = bg36_connect,
    .send = bg36_send,
    .sendto = NULL,
    .recv_timeout = bg36_recv_timeout,
    .recv = bg36_recv,
    .recvfrom = NULL,
    .close = bg36_close,
    .recv_cb = NULL,
    .deinit = NULL,
};

#endif
