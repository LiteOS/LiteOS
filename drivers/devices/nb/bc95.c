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

int32_t nb_reboot(void)
{
    memset(sockinfo, 0, MAX_SOCK_NUM * sizeof(struct _remote_info_t));
    return at.cmd((int8_t*)AT_NB_reboot, strlen(AT_NB_reboot), "OK", NULL);
}

int32_t nb_hw_detect(void)//"AT+CFUN?\r"
{
    return at.cmd((int8_t*)AT_NB_hw_detect, strlen(AT_NB_hw_detect), "+CFUN:1", NULL);
}

int32_t nb_check_csq(void)
{
    char *cmd = "AT+CSQ\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "+CSQ:", NULL);
}

int32_t nb_set_cdpserver(char* host, char* port)
{
    char *cmd = "AT+NCDP=";
    char *cmd2 = "AT+NCDP?";
	char *cmdNNMI = "AT+NNMI=1\r";
	//char *cmdCGP = "AT+CGPADDR";
	char tmpbuf[128] = {0};
	int ret = -1;
    char ipaddr[100] = {0};
    if(strlen(host) > 70 || strlen(port) > 20)
    {
        return ret;
    }

    snprintf(ipaddr, sizeof(ipaddr) - 1, "%s,%s\r", host, port);

	snprintf(tmpbuf, sizeof(tmpbuf) - 1, "%s%s%c", cmd, ipaddr, '\r');
    ret = at.cmd((int8_t*)tmpbuf, strlen(tmpbuf), "OK", NULL);
	if(ret < 0)
	{
		return ret;
	}
	ret = at.cmd((int8_t*)cmd2, strlen(cmd2), ipaddr, NULL);
	LOS_TaskDelay(1000);
	ret = at.cmd((int8_t*)cmdNNMI, strlen(cmdNNMI), "OK", NULL);
	//ret = at.cmd((int8_t*)cmdCGP, strlen(cmdCGP), NULL, NULL);
	return ret;
}

int32_t nb_send_psk(char* pskid, char* psk)
{
    char* cmds = "AT+QSECSWT";//AT+QSECSWT=1,100    OK
    char* cmdp = "AT+QSETPSK";//AT+QSETPSK=86775942,E6F4C799   OK
    sprintf(wbuf, "%s=%d,%d\r", cmds, 1, 100);//min
    at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
    snprintf(wbuf, AT_DATA_LEN, "%s=%s,%s\r", cmdp, pskid, psk);
    return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
}

int32_t nb_set_no_encrypt(void)
{
    char* cmd = "AT+QSECSWT=0\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL);
}

int32_t nb_send_str(const char* buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret;
    char* str = NULL;
    int curcnt = 0;
    static int sndcnt = 0;
    memset(wbuf, 0, AT_DATA_LEN);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN, "%s%d,%s%c",cmd1,(int)len/2,buf,'\r');
    ret = at.cmd((int8_t*)wbuf, strlen(wbuf), NULL, NULL);
    if(ret < 0)
        return -1;
    ret = at.cmd((int8_t*)cmd2, strlen(cmd2), "SENT=", rbuf);
    if(ret < 0)
        return -1;
    str = strstr(rbuf,"SENT=");
    if(str == NULL)
        return -1;
    sscanf(str,"SENT=%d,%s",&curcnt,wbuf);
    if(curcnt == sndcnt)
        return -1;
    sndcnt = curcnt;
    return ret;
}

int32_t nb_send_payload(const char* buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret;
    char* str = NULL;
    int curcnt = 0;
    static int sndcnt = 0;
    if(buf == NULL || len > AT_MAX_PAYLOADLEN)
    {
        AT_LOG("payload too long");
        return -1;
    }
    memset(tmpbuf, 0, AT_DATA_LEN);
    memset(wbuf, 0, AT_DATA_LEN);
    str_to_hex(buf, len, tmpbuf);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN,"%s%d,%s%c",cmd1,(int)len,tmpbuf,'\r');
    ret = at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
    if(ret < 0)
        return -1;
    ret = at.cmd((int8_t*)cmd2, strlen(cmd2), "SENT=", rbuf);
    if(ret < 0)
        return -1;
    str = strstr(rbuf,"SENT=");
    if(str == NULL)
        return -1;
    sscanf(str,"SENT=%d,%s",&curcnt,wbuf);
    if(curcnt == sndcnt)
        return -1;
    sndcnt = curcnt;
    return ret;
}

int nb_query_ip(void)
{
	char *cmd = "AT+CGPADDR\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "+CGPADDR:0,", NULL);
}

int32_t nb_get_netstat(void)
{
	char *cmd = "AT+CGATT?\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "CGATT:1", NULL);
}


int32_t nb_create_udpsock(const int8_t * host, int port, int32_t proto)
{
	int socket;
	char *cmd = "AT+NSOCR=DGRAM,17,";//udp
    memset(rbuf, 0, AT_DATA_LEN);
	memset(wbuf, 0, AT_DATA_LEN);

	//nb_set_cdpserver(host,strlen(host));

    snprintf(wbuf, AT_DATA_LEN, "%s%d,1\r", cmd, port);//udp
	at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", rbuf);
	sscanf(rbuf, "%d\r%s",&socket, tmpbuf);
	//neul_bc95_hex_to_str(tmpbuf, readlen*2, coapmsg);
    if (socket >= 0)
    {
        return socket;
    }
    return -1;
}

int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("entry!");

    //process data frame ,like +NSONMI:linkid,len
    int32_t ret = -1;
    int32_t sockid = 0, data_len = 0;
    char * p1, *p2;
    QUEUE_BUFF qbuf;
    p1 = (char *)buf;
    char* cmd = "AT+NSORF";

    if (strstr(p1, AT_DATAF_PREFIX) != NULL)
    {
        p2 = strstr(p1, ",");
        if (NULL == p2)
        {
            AT_LOG("got data prefix invailed!");
            goto END;
        }

        for (p2++; *p2 <= '9' && *p2 >= '0'; p2++)
        {
            sockid = sockid * 10 + (*p2 - '0');
        }

        for (p2++; *p2 <= '9' && *p2 >= '0' ;p2++)
        {
            data_len = (data_len * 10 + (*p2 - '0'));
        }

        qbuf.addr = at_malloc(data_len + 40);//extra space for ip and port
        if (NULL == qbuf.addr)
        {
            AT_LOG("malloc for qbuf failed!");
            goto END;
        }

        qbuf.len = data_len;
        memset(wbuf, 0, AT_DATA_LEN);
        memset(tmpbuf, 0, AT_DATA_LEN);
        snprintf(wbuf, AT_DATA_LEN, "%s=%d,%d\r", cmd, (int)sockid,(int)data_len);
        sprintf(tmpbuf, "%d,\r",(int)sockid);
        at.cmd((int8_t*)wbuf, strlen(wbuf), tmpbuf, (char *)qbuf.addr);

        if (LOS_OK != (ret = LOS_QueueWriteCopy(at.linkid[sockid].qid, &qbuf, sizeof(QUEUE_BUFF), 0)))
        {
            AT_LOG("LOS_QueueWriteCopy  failed!");
            at_free(qbuf.addr);
            goto END;
        }
        ret = data_len;
    }
    END:
    return ret;
}

int32_t nb_connect(const int8_t * host, const int8_t *port, int32_t proto)
{
	int ret = 0;
	//int timecnt = 0;
	static int localport = NB_STAT_LOCALPORT;

	do{
		ret = nb_create_udpsock(NULL, localport, 1);
		localport++;
	}while(ret < 0);

	localport--;
	if(ret >= MAX_SOCK_NUM)
	{
		AT_LOG_DEBUG("sock num exceeded,ret is %d",ret);
		return -1;
	}
	memcpy(sockinfo[ret].ip, (const char*)host,strlen((const char*)host));
	sockinfo[ret].port = *(unsigned short*)port;
	localport++;

    if (LOS_QueueCreate("dataQueue", 16, &at.linkid[ret].qid, 0, sizeof(QUEUE_BUFF)) != LOS_OK)
    {
        AT_LOG("init dataQueue failed, ret is %d!",ret);
        at.linkid[ret].usable = AT_LINK_UNUSE;//adapter other module.
        return -1;
    }

    return ret;

}

int32_t nb_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	char *cmd = "AT+NSOST=";
	//char *str = "AT+NMGS192.53.100.53,5683,1,11\r";
	int data_len = len/2;
    if(buf == NULL || data_len > AT_MAX_PAYLOADLEN)
    {
        AT_LOG("payload too long");
        return -1;
    }
	memset(wbuf, 0, AT_DATA_LEN);
	memset(tmpbuf, 0, AT_DATA_LEN);
	str_to_hex((const char *)buf, len, tmpbuf);
	snprintf(wbuf, AT_DATA_LEN, "%s%d,%s,%d,%d,%s\r",cmd,(int)id,sockinfo[id].ip,(int)sockinfo[id].port,(int)data_len,tmpbuf);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
}

int32_t nb_recv(int32_t id , uint8_t  *buf, uint32_t len)
{
    return nb_recv_timeout(id, buf, len, LOS_WAIT_FOREVER);
}

int32_t nb_recv_timeout(int32_t id , uint8_t  *buf, uint32_t len, int32_t timeout)
{
    int rlen = 0;
    int rskt = -1;
    int port = 0;
    int readleft = 0;
    int ret;
    QUEUE_BUFF	qbuf = {0, NULL};
    UINT32 qlen = sizeof(QUEUE_BUFF);

    memset(rbuf, 0, AT_DATA_LEN);
    memset(tmpbuf, 0, AT_DATA_LEN);

    ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout);
    if (ret != LOS_OK)
    {
        return -1;
    }

    sscanf((const char*)qbuf.addr, "\r%d,%s,%d,%d,%s,%d\r%s", &rskt,tmpbuf,&port,&rlen,tmpbuf+22,&readleft,rbuf);
    AT_LOG("ret = %x, len = %d, rxlen = %d, qbuf.len = %d", ret, (int)len, rlen, (int)qbuf.len);

    if (qbuf.len){
        rlen = (len < qbuf.len) ? len : qbuf.len;
        memcpy(buf, wbuf, rlen);
        at_free(qbuf.addr);
    }
    else
    {
        rlen = 0;
    }
    return rlen;

}

int32_t nb_close(int32_t socket)
{
    char *cmd = "AT+NSOCL=";
	memset(wbuf, 0, AT_DATA_LEN);
	sprintf(wbuf, "%s%d\r", cmd, (int)socket);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);

}

int32_t nb_recv_cb(int32_t id)
{
    return -1;
}

int32_t nb_deinit(void)
{
    return nb_reboot();
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

    .connect = nb_connect,
    .send = nb_send,

    .recv_timeout = nb_recv_timeout,
    .recv = nb_recv,

    .close = nb_close,
    .recv_cb = nb_recv_cb,

    .deinit = nb_deinit,
};
#endif
