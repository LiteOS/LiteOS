
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "los_nb_api.h"
#include "at_api_interface.h"
#include "atiny_socket.h"

extern at_task at;
extern char rbuf[1064];
extern char wbuf[1064];
extern char tmpbuf[1064]; //ÓÃÓÚ×ª»»hex
extern char coapmsg[536];

#define MAX_SOCK_NUM 5
remote_info sockinfo[MAX_SOCK_NUM];

int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("cmd in");
	return 0;
}

int los_nb_init(const int8_t* host, const int8_t* port, sec_param_s* psk)
{
    int ret;
    int timecnt = 0;
    at.init();

    memset(sockinfo, 0, MAX_SOCK_NUM * sizeof(struct _remote_info_t));

    nb_reset();
    if(psk != NULL)//encryption v1.9
    {
        char* cmds = "AT+QSECSWT";//AT+QSECSWT=1,100    OK
        char* cmdp = "AT+QSETPSK";//AT+QSETPSK=86775942,E6F4C799   OK
        sprintf(wbuf, "%s=%d,%d\r", cmds, 1, 100);//min
        at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
        sprintf(wbuf, "%s=%s,%s\r", cmdp, psk->psk, psk->pskid);
        at.cmd((int8_t*)wbuf, strlen(wbuf), "OK", NULL);
    }

    while(1)
    {
        ret = nb_hw_detect();
        if(ret != AT_FAILED)
            break;
        LOS_TaskDelay(1000);
    }
    //nb_get_auto_connect();
    //nb_connect(NULL, NULL, NULL);

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
    memset(wbuf, 0, 1064);

    sprintf(wbuf, "%s,%s\r", (char *)host, (char *)port);
	nb_set_cdpserver((const char *)wbuf);
    return ret;
}

int los_nb_report(const char* buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    char tmpbuf[1064] = {0};
    char cmd[1064] = {0};
    int ret;
    char* str = NULL;
    int curcnt = 0;
    static int sndcnt = 0;
    neul_bc95_str_to_hex((const char*)buf, len, tmpbuf);
    sprintf(cmd, "%s%d,%s%c",cmd1,(int)len,tmpbuf,'\r');
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

int los_nb_notify(oob_callback callback)
{
    return at.oob_register(AT_CMD_PREFIX,strlen(AT_CMD_PREFIX), callback);
}

int los_nb_deinit(void)
{
    return nb_reset();;
}

#endif
