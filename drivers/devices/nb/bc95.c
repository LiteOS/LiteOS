
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "bc95.h"
#include "atadapter.h"
#include "at_api_interface.h"
#include "atiny_socket.h"

extern at_task at;
at_adaptor_api at_interface;

char rbuf[1064] = {0};
char wbuf[1064] = {0};
char tmpbuf[1064]={0}; //用于转换hex
char coapmsg[536]={0};


int32_t nb_data_handler(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("cmd in");
	return 0;
}

static int neul_bc95_hex_to_str(const char *bufin, int len, char *bufout)
{
    int i = 0;
    unsigned char tmp2 = 0x0;
    unsigned int tmp = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i = i+2)
    {
        tmp2 =  bufin[i];
        tmp2 =  tmp2 <= '9'?tmp2-0x30:tmp2-0x37;
        tmp =  bufin[i+1];
        tmp =  tmp <= '9'?tmp-0x30:tmp-0x37;
        bufout[i/2] =(tmp2<<4)|(tmp&0x0F);
    }
    return 0; 
}

static int neul_bc95_str_to_hex(const char *bufin, int len, char *bufout)
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

int32_t nb_reset(void)
{
    return at.cmd((int8_t*)AT_NB_reboot, strlen(AT_NB_reboot), "REBOOTING", NULL);
}

int32_t nb_hw_detect(void)//"AT+CFUN?\r"
{
    return at.cmd((int8_t*)AT_NB_hw_detect, strlen(AT_NB_hw_detect), "+CFUN:1", NULL);
}

int nb_get_msg(void)
{
	char *cmdcnt = "AT+NQMGR\r\n";
	char *cmdmsg = "AT+NMGR\r\n";
	char rbuf[1024]={0};
	char wbuf[1024]={0};
	int msgcnt = 0;
	int readlen = 0;
	at.cmd((int8_t*)cmdcnt, strlen(cmdcnt), "BUFFERED", rbuf);
	sscanf(rbuf, "\r\nBUFFERED=%d,%s", &msgcnt, wbuf);
    if (msgcnt < 0 )
    {
        return 0;
    }
	while(msgcnt > 0)
	{
		AT_LOG("msgcnt:%d\n",msgcnt);
		memset(rbuf, 0, 1024);
		at.cmd((int8_t*)cmdmsg, strlen(cmdmsg), "OK", rbuf);
		sscanf(rbuf, "\r\n%d,%s\r\n\r\nOK\r\n",&readlen, tmpbuf);
		if(readlen>0)
		{
			neul_bc95_hex_to_str(tmpbuf, readlen*2, coapmsg);
			AT_LOG("get cmd\n");
			AT_LOG("msg:%s\n",coapmsg);
			//ioctl(coapmsg,n);
		}
		msgcnt--;
	}
	return 0;
}

int32_t nb_send_test(void)
{
	char *cmd = "AT+NMGS=2,0101";
    return at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL);
}

int32_t nb_read(void)
{
	char *cmd = "AT+NMGR";
    return at.cmd((int8_t*)cmd, strlen(cmd), "2", NULL);
}

int32_t nb_set_cdpserver(const char *ipaddr)
{
    char *cmd = "AT+NCDP=";
    char *cmd2 = "AT+NCDP?";
	char *cmdNNMI = "AT+NNMI=1\r";
	//char *cmdCGP = "AT+CGPADDR";
	char tmpbuf[1064] = {0};
	int ret;
	
	sprintf(tmpbuf, "%s%s%c", cmd, ipaddr, '\r');
    ret = at.cmd((int8_t*)tmpbuf, strlen(tmpbuf), "OK", NULL);
	if(ret <= 0)
	{
		return ret;
	}
	ret = at.cmd((int8_t*)cmd2, strlen(cmd2), ipaddr, NULL);
	LOS_TaskDelay(1000);
	ret = at.cmd((int8_t*)cmdNNMI, strlen(cmdNNMI), "OK", NULL);
	//ret = at.cmd((int8_t*)cmdCGP, strlen(cmdCGP), NULL, NULL);
	return ret;
}

int32_t nb_get_auto_connect(void)
{
    return at.cmd((int8_t*)AT_NB_get_auto_connect, strlen(AT_NB_get_auto_connect), "AUTOCONNECT,TRUE", NULL);//"AUTOCONNECT,TRUE"
}

int32_t nb_send_coap_payload(int32_t id ,const uint8_t *buf, uint32_t len)
{
	char *cmd1 = "AT+NMGS=";
	char *cmd2 = "AT+NQMGS\r";
	char tmpbuf[1064] = {0};
	char cmd[1064] = {0};
	int ret;
	char* str = NULL;
	int curcnt = 0;
	static int sndcnt = 0;
	neul_bc95_hex_to_str((const char*)buf, len, tmpbuf);
	sprintf(cmd, "%s%d,%s%c",cmd1,strlen(tmpbuf),tmpbuf,'\r');
	ret = at.cmd((int8_t*)cmd, strlen(cmd), "OK", NULL);
	if(ret < 0)
		return -1;
	ret = at.cmd((int8_t*)cmd2, strlen(cmd2), "SENT=", rbuf);
	if(ret < 0)
		return -1;
	str = strstr(rbuf,"SENT=");
	sscanf(str,"SENT=%d,%s",&curcnt,wbuf);
	if(curcnt == sndcnt)
		return -1;
	sndcnt = curcnt;
	return 0;	
}

int neul_bc95_udp_read(int socket,char *buf, int maxrlen, int mode)
{
    //AT+NSORF=0,4 
    char *cmd = "AT+NSORF=";
    char *str = "AT+NSORF=0,4";
    int rlen = 2;
    int rskt = -1;
    int port = 0;
    int readleft = 0;

    if (socket < 0 || NULL == buf || maxrlen <= 0)
    {
        return -1;
    }
    memset(rbuf, 0, 1064);
	memset(wbuf, 0, 1064);
    //oob_register:+NSONMI:
	//sscanf(rbuf,"\r+NSONMI:0,%d\r",&rlen);
		
    sprintf(wbuf, "%s%d,%d\r", cmd, socket, maxrlen);
    at.cmd((int8_t*)str, strlen(str), "OK", rbuf);

    sscanf(rbuf, "\r%d,%s,%d,%d,%s,%d\r%s", &rskt,tmpbuf,&port,&rlen,tmpbuf+22,&readleft,wbuf);
    if (rlen>0)
    {
        neul_bc95_str_to_hex((const char *)(tmpbuf+22),rlen*2, buf);
    }
	
    return rlen;
}

int32_t nb_check_csq(void)
{
	char *cmd = "AT+CSQ\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), NULL, NULL);
}

int nb_query_ip()
{
	char *cmd = "AT+CGPADDR\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "+CGPADDR:0,", NULL);

}

int32_t nb_get_netstat(void)
{
	char *cmd = "AT+CGATT?\r";
    return at.cmd((int8_t*)cmd, strlen(cmd), "CGATT:1", NULL);
}


int32_t nb_create_udpsock(const int8_t * host, const int8_t *port, int32_t proto)
{
	int socket;
	char *cmd = "AT+NSOCR=DGRAM,17,";//udp
	char wbuf[1024]={0};
	char rbuf[1024]={0};
	//nb_set_cdpserver(host,strlen(host));

    sprintf(wbuf, "%s%d,1\r", cmd, (int)port);
	at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", rbuf);
	sscanf(rbuf, "%d\r%s",&socket, tmpbuf);
	//neul_bc95_hex_to_str(tmpbuf, readlen*2, coapmsg);
    if (socket >= 0)
    {
        return socket;
    }
    return -1;

}

int32_t nb_close_udpsock(int socket)
{
    char *cmd = "AT+NSOCL=";

    memset(wbuf, 0, 1064);
    sprintf(wbuf, "%s%d\r", cmd, socket);

	at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", rbuf);
	//删除数组对应元素
    if (socket >= 0)
    {
        return socket;
    }
    return -1;

}

int32_t nb_init(void)
{
	char* ipaddr = CLOUD_IP;
	int ret;
    at.init();
	at.oob_register("+NNMI",strlen("+NNMI"), nb_data_handler);

    nb_reset();
	while(1)
	{
		ret = nb_hw_detect();
		if(ret != AT_FAILED)
			break;
		LOS_TaskDelay(1000);
	}
	nb_set_cdpserver(ipaddr);//
	LOS_TaskDelay(3000);
	//nb_get_auto_connect();
	//nb_connect(NULL, NULL, NULL);
    return AT_OK;
}

int32_t nb_connect(const int8_t * host, const int8_t *port, int32_t proto)
{
	int ret = 0;
	int timecnt = 0;
	while(timecnt < 120)
	{
		ret = nb_get_netstat();
		//nb_check_csq();
		if(ret != AT_FAILED)
		{
			ret = nb_query_ip();
			break;
		}
		LOS_TaskDelay(1000);
		timecnt++;
	}
	if(ret != AT_FAILED)
	{
		ret = nb_query_ip();
	}
	//nb_set_cdpserver((const char *)host);
	ret = nb_create_udpsock(NULL, port, 0);
    return ret;

}

int32_t nb_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
	char *cmd = "AT+NSOST=";
	//char *str = "AT+NSOST=0,192.53.100.53,5683,1,11\r";
	
	memset(wbuf, 0, 1064);
	memset(tmpbuf, 0, 1064);
	neul_bc95_str_to_hex((const char *)buf, len, tmpbuf);
	sprintf(wbuf, "%s%d,%s,%d,%d,%s\r",cmd,(int)id,"192.53.100.53",(int)5683,(int)len,tmpbuf);//这些参数需要socket结构体保存
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
}

int32_t nb_recv(int32_t id , int8_t  *buf, uint32_t len)
{
    return -1;
}

int32_t nb_recv_timeout(int32_t id , int8_t  *buf, uint32_t len, int32_t timeout)
{
    return -1;
}

int32_t nb_close(int32_t socket)
{
    char *cmd = "AT+NSOCL=";
	memset(wbuf, 0, 1064);
	sprintf(wbuf, "%s%d\r", cmd, (int)socket);
	return at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
	//全局存储remote信息
}

int32_t nb_recv_cb(int32_t id)
{
    return -1;
}

int32_t nb_deinit(void)
{
    return nb_reset();
}

at_config at_user_conf = {
    .name = AT_MODU_NAME,
    .usart = USART3,
    .buardrate = AT_BUARDRATE,
    .irqn = AT_USART_IRQn,
    .linkid_num = AT_MAX_LINK_NUM,
    .user_buf_len = MAX_AT_USERDATA_LEN,
    .cmd_begin = AT_CMD_BEGIN,
    .line_end = AT_LINE_END,
    .mux_mode = 1, //support multi connection mode
    .timeout = AT_CMD_TIMEOUT,   //  ms
};

at_adaptor_api at_interface = {
    .init = nb_init,    

    .connect = nb_connect,
    .send = nb_send,

    .recv_timeout = nb_recv_timeout,
    .recv = nb_recv,

    .close = nb_close,
    .recv_cb = nb_recv_cb,

    .deinit = nb_deinit,
};
#endif
