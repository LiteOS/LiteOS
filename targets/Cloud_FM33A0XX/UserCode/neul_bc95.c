/*!
    \file  neul_bc95.c
    \brief NEUL BC95 IoT model driver 
*/

#include <string.h>
#include "stdlib.h"
#include "neul_bc95.h"
#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"
#include "los_sem.h"

#include "los_uart.h"


#define CN_UART_AT    3



/* USER CODE BEGIN Private defines */
typedef struct
{
	char net_time[30];
	char net_csq[5];
	char net_cfun[5];
	char net_ip[20];
	char net_cscon[5];
	char net_cereg[5];
	char net_cgatt[5];
	char net_npsmr[5];
	char net_nsocr[10];
	char net_nsocl[10];
	char net_nsost[30];
	char net_nsonmi[10];
	char net_nsorf[30];
	char net_nmgr[30];
	char net_Signal_power[10];
	char net_Total_power[10];
	char net_TX_power[10];
	char net_TX_time[10];
	char net_RX_time[10];
	char net_SCell_ID[10];
	char net_ECL[10];
	char net_SNR[10];
	char net_EARFCN[10];
	char net_PCI[10];
	char net_RSRQ[10];
}msg_sys_type;

msg_sys_type bc95_net_data;


#define NEUL_MAX_BUF_SIZE 512
#define NEUL_MAX_SOCKETS 1 // the max udp socket links
#define NEUL_IP_LEN 16
#define NEUL_MANUFACT_LEN 20

static char neul_bc95_buf[NEUL_MAX_BUF_SIZE];
static char neul_bc95_wbuf[NEUL_MAX_BUF_SIZE];
static char neul_bc95_tmpbuf[NEUL_MAX_BUF_SIZE];

//static int uart_data_read(char *buf, int maxrlen, int mode);
//static int uart_data_write(const char *buf, int writelen, int mode);
int uart_data_read(char *buf, int maxrlen, int mode, int timeout);
int uart_data_write(char *buf, int writelen, int mode);


static neul_dev_operation_t neul_ops = {
    uart_data_read,
    uart_data_write
};
static remote_info udp_socket[NEUL_MAX_SOCKETS] = {
	{0, -1, {0}}
};
static neul_dev_info_t neul_dev = {
    neul_bc95_buf,
    neul_bc95_wbuf,
    0,
    udp_socket,
    &neul_ops,
    {0}
};


/* ============================================================
func name   :   neul_bc95_set_upd_param
discription :   this func just called after the socket created
                set the remote ip address and port the socket sendto or receive fro
param       :   
             rip  @ input param, remote client/server ip address
             port @ input param, remote client/server port
             socket @ input param, local socket handle
return      :  0 mean ok, !0 means param err
Revision    : 
=============================================================== */
int neul_bc95_set_upd_param(const char *rip, const unsigned short port, const int socket)
{
    if (NULL == rip || 0 == port)
    {
        return -1;
    }
    if (strlen(rip) >= NEUL_IP_LEN)
    {
        return -2;
    }
    neul_dev.remotecount++;
    (neul_dev.addrinfo+socket)->port = port;
    (neul_dev.addrinfo+socket)->socket = socket;
    memcpy((neul_dev.addrinfo+socket)->ip, rip, strlen(rip));
    return 0;
}

/* ============================================================
func name   :   neul_bc95_set_upd_param
discription :   this func just called after the socket created
                set the remote ip address and port the socket sendto or receive fro
param       :   
             socket @ input param, local socket handle that need to reset
return      :  0 mean ok, !0 means param err
Revision    : 
=============================================================== */
static int neul_bc95_reset_upd_param(const int socket)
{
    if (socket < 0)
    {
        return -1;
    }
    neul_dev.remotecount--;
    (neul_dev.addrinfo+socket)->port = 0;
    (neul_dev.addrinfo+socket)->socket = -1;
    memset((neul_dev.addrinfo+socket)->ip, 0, NEUL_IP_LEN);
    return 0;
}

/* ============================================================
func name   :  neul_bc95_AT_CGMM
discription :  Request Manufacturer Identification
param       :  None 
return      :  0 bc95 is connectted to mcu ok , !0 means not ok
Revision    : 
=============================================================== */
int neul_bc95_AT_CGMM(void)
{
    int ret = 0;
    char *p = "AT+CGMM\r";
    char *str = NULL;
    ret = neul_dev.ops->dev_write(p, strlen(p), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    if (NULL == neul_dev.rbuf)
    {
        return -2;
    }
    memset(neul_dev.rbuf, 0, 20);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 20, 0, 200);
    if (ret <= 0)
    {
        //read bc95 manufacturer info failed
        return -3;
    }
    str = strstr(neul_dev.rbuf,"Quectel");
    if (!str)
    {
        //can't find bc95 manufacturer info
        return -4;
    }
    return 0;
}
/* ============================================================
func name   :  neul_bc95_hw_detect
discription :  detect bc95 hardware 
param       :  None 
return      :  0 bc95 is connectted to mcu ok , !0 means not ok
Revision    : 
=============================================================== */
int neul_bc95_hw_detect(void)
{
    int ret = 0;
    char *p = "AT+CGMM\r";
    char *str = NULL;
    ret = neul_dev.ops->dev_write(p, strlen(p), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    if (NULL == neul_dev.rbuf)
    {
        return -2;
    }
    //neul_bc95_sleep(10);
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 manufacturer info failed
        return -3;
    }
    str = strstr(neul_dev.rbuf,"BC95HB-02-STD_850");
    if (!str)
    {
        //can't find bc95 manufacturer info
        return -4;
    }
    return 0;
}

/* ============================================================
func name   :  neul_bc95_set_imei
discription :  set bc95 imei info 
param       :  
               imei @ input param, the imei data set to bc95
               len @ input param, lenght of the data
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_set_imei(const char *imei, int len)
{
    int ret = 0;
    char *p = "AT+NTSETID=1,";
    char *p2 = "AT+CGSN=1\r";
    char *str = NULL;
    //int cmdlen = 0;
    if (NULL == imei || len <= 0)
    {
        return -1;
    }
    if (NULL == neul_dev.wbuf)
    {
        return -2;
    }
    memset(neul_dev.wbuf, 0, 100);
    //cmdlen = strlen(p);
    //memcpy(neul_dev.wbuf, p, cmdlen);
    //memcpy(neul_dev.wbuf+cmdlen, imei, len);
    sprintf(neul_dev.wbuf, "%s%s%c", p, imei, '\r');
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 16);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 16, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -3;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (NULL == str)
    {
        //read imei info
        ret = neul_dev.ops->dev_write(p2, strlen(p2), 0);
        if (ret < 0)
        {
            //write data to bc95 failed
            return -1;
        }
        memset(neul_dev.rbuf, 0, 64);
        ret = neul_dev.ops->dev_read(neul_dev.rbuf, 16, 0, 200);
        if (ret <= 0)
        {
            //read bc95 read set return value info failed
            return -3;
        }
        str = strstr(neul_dev.rbuf,"OK");
        if (NULL == str)
        {
            return -1;
        }
        str = strstr(neul_dev.rbuf,"+CGSN:");
        if (str && strlen(neul_dev.rbuf) >= 22)
        {
            return 0;
        }
        return -1;
    }
    return 0;
}

/* ============================================================
func name   :  int neul_bc95_check_imei
discription :  check if bc95 imei is setted 
param       :  None
return      :  1 imei already set, <=0 not set
Revision    : 
=============================================================== */
int neul_bc95_check_imei(void)
{
    int ret = 0;
    char *p2 = "AT+CGSN=1\r";
    char *str = NULL;
    int retry = 0;
    //int cmdlen = 0;

    if (NULL == neul_dev.wbuf)
    {
        return -2;
    }
    memset(neul_dev.wbuf, 0, 100);
    //cmdlen = strlen(p);
    //memcpy(neul_dev.wbuf, p, cmdlen);
    //memcpy(neul_dev.wbuf+cmdlen, imei, len);
		//read imei info
    while(retry < 3)
    {
        retry++;
		ret = neul_dev.ops->dev_write(p2, strlen(p2), 0);
		if (ret < 0)
		{
            //write data to bc95 failed
            return -1;
		}
		memset(neul_dev.rbuf, 0, 64);
		ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 300);
		if (ret <= 0)
		{
            //read bc95 read set return value info failed
            return -3;
		}
		str = strstr(neul_dev.rbuf,"OK");
		if (NULL == str)
		{
            str = strstr(neul_dev.rbuf,"ERROR");
            if (NULL != str)
            {
                continue;
            }
		}
		str = strstr(neul_dev.rbuf,"+CGSN:");
		if (str && strlen(neul_dev.rbuf) >= 22)
		{
            return 1;
		}
    }
    return 0;
}


/* ============================================================
func name   :  neul_bc95_set_band
discription :  set bc95 work band 
param       :  
               band @ input param, bc95 work band BC95-CM set to 8, 
                      BC95-SL set to 5, BC95-VF set to 20
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_set_band(int band)
{
    char *p = "AT+NBAND=";
    int ret = 0;
    char *str = NULL;
    memset(neul_dev.wbuf, 0, 16);
    if (band == 8 || band == 5 || band == 20)
    {
        sprintf(neul_dev.wbuf, "%s%d%c", p, band, '\r');
    }
    else
    {
        //error band for bc95
        return -1;
    }
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 16);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 16, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -3;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (NULL == str)
    {
        return -1;
    }
    return 0;
}

/* ============================================================
func name   :  neul_bc95_active_network
discription :  active bc95's network 
               this will take some time
param       :  None
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_active_network(void)
{
    int ret = 0;
    char *str = NULL;
    char *cmd0 = "AT+CGSN=1\r";
    char *cmd1 = "AT+CFUN=1\r"; 
    char *cmd2 = "AT+CIMI\r";
    //char *cmd3 = "AT+CGDCONT=1,\"IP\",\"HUAWEI.COM\"\r";
    char *cmd4 = "AT+CGATT=1\r";

    /* Get SN */
    ret = neul_dev.ops->dev_write(cmd0, strlen(cmd0), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"+CGSN:");
    if (!str)
    {
        return -1;
    }
    if (strlen(str) < 25)
    {
        return -1;
    }
    
    /* Config CFUN */
    ret = neul_dev.ops->dev_write(cmd1, strlen(cmd1), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    //neul_bc95_sleep(4000);//need wait 4 sconds or more, it's a little long
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 6, 0, 5000);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }    
 
    neul_bc95_sleep(1000);
    /* Get IMSI */
    ret = neul_dev.ops->dev_write(cmd2, strlen(cmd2), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    if (strlen(neul_dev.rbuf) < 19)
    {
        return -1;
    }
    
    /* set band to do... */
    ret = neul_bc95_set_band(5);
    if (ret < 0)
    {
        return -1;
    }
    #if 0
    /* Config apn */
    ret = neul_dev.ops->dev_write(cmd3, strlen(cmd3), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -2;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    #endif
    
    /* Active Network */
    ret = neul_dev.ops->dev_write(cmd4, strlen(cmd4), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }

    return 0;
}

/* ============================================================
func name   :  neul_bc95_get_netstat
discription :  get bc95's network status 
               
param       :  None
return      :  0 connected , !0 not connect
Revision    : 
=============================================================== */
int neul_bc95_get_netstat(void)
{
    char *cmd = "AT+CGATT?\r";
    char *str = NULL;
    int ret = 0;
    
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"CGATT:1");
    if (!str)
    {
        return -1;
    }
    
    return 0;
}

/* ============================================================
func name   :  neul_bc95_query_ip
discription :  query if bc95 get ip address 
               
param       :  None
return      :  0 get ip , !0 no ip
Revision    : 
=============================================================== */
int neul_bc95_query_ip(void)
{
    char *cmd = "AT+CGPADDR\r";
    char *str = NULL;
    int ret = 0;
    
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 64);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 64, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    //if get +CGPADDR:0 \r\n OK , no ip get ,need reboot
    str = strstr(neul_dev.rbuf,"+CGPADDR:0,");
    if (!str)
    {
        return -1;
    }
    
    return 0;
    
}

/* ============================================================
func name   :  neul_bc95_reboot
discription :  reboot bc95 
               
param       :  None
return      :  0 ok!  0 not ok
Revision    : 
=============================================================== */
int neul_bc95_reboot(void)
{
    char *cmd = "AT+NRB\r";
    char *str = NULL;
    int ret = 0;
    
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 64);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 64, 0, 6000);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    
    str = strstr(neul_dev.rbuf,"REBOOTING");
    if (!str)
    {
        return -1;
    }
    
    return 0;
}


/* ============================================================
func name   :  neul_bc95_hex_to_str
discription :  convert hex string to hex data 
               
param       :  bufin @ input param , the data that need to convert
               len @ input param, data length
               bufout @ output param, hex data after convert 
return      :  0 send ok , !0 not ok
Revision    : 
=============================================================== */
static int neul_bc95_str_to_hex(const unsigned char *bufin, int len, char *bufout)
{
    int i = 0;
    #if 0
    int tmp = 0;
    #endif
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i++)
    {
        #if 0
        tmp = bufin[i]>>4;
        bufout[i*2] = tmp > 0x09?tmp+0x37:tmp+0x30;
        tmp = bufin[i]&0x0F;
        bufout[i*2+1] = tmp > 0x09?tmp+0x37:tmp+0x30;
        #else
        sprintf(bufout+i*2, "%02X", bufin[i]);
        #endif
    }
    return 0; 
}

/* ============================================================
func name   :  neul_bc95_hex_to_str
discription :  convert char to hex string 
               
param       :  bufin @ input param , the data that need to convert
               len @ input param, data length
               bufout @ output param, hex string data 
return      :  0 convert ok , !0 not ok
Revision    : 
=============================================================== */
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
        #if 1
        tmp2 =  bufin[i];
        tmp2 =  tmp2 <= '9'?tmp2-0x30:tmp2-0x37;
        tmp =  bufin[i+1];
        tmp =  tmp <= '9'?tmp-0x30:tmp-0x37;
        bufout[i/2] =(tmp2<<4)|(tmp&0x0F);
        #else
        sscanf(bufin+i, "%02x", &tmp);
        bufout[i/2] = tmp;
        #endif
    }
    return 0; 
}

/* ============================================================
func name   :  neul_bc95_set_cdpserver
discription :  set bc95 cdp server 
               
param       :  ipaddr @ input param cdp server ip address
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_set_cdpserver(const char *ipaddr)
{
    char *cmd = "AT+NCDP=";
    char *cmd2 = "AT+NCDP?\r";
    char *str = NULL;
    int ret = 0;
    
    if (NULL == ipaddr || strlen(ipaddr) >= 16)
    {
        return -1;
    }
    memset(neul_dev.wbuf, 0, 64);
    sprintf(neul_dev.wbuf, "%s%s%c", cmd, ipaddr, '\r');
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    
    //query the setted ip back
    ret = neul_dev.ops->dev_write(cmd2, strlen(cmd2), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,ipaddr);
    if (!str)
    {
        return -1;
    }
    
    return 0;
}


/* ============================================================
func name   :  neul_bc95_send_coap_paylaod
discription :  send coap message(hex data) to cdp server 
               
param       :  buf @ input param , the data that send to cdp
               sendlen @ input param, data length
return      :  0 send ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_send_coap_paylaod(const char *buf, int sendlen)
{
    char *cmd = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret = 0;
    char *str = NULL;
    static int sndcnt = 0;
    int curcnt = 0;
    
    if (NULL == buf || sendlen >= 512)
    {
        return -1;
    }
    memset(neul_dev.wbuf, 0, 64);
    memset(neul_bc95_tmpbuf, 0, NEUL_MAX_BUF_SIZE);
    ret = neul_bc95_str_to_hex((unsigned char *)buf, sendlen, neul_bc95_tmpbuf);
    sprintf(neul_dev.wbuf, "%s%d,%s%c", cmd, sendlen, neul_bc95_tmpbuf, '\r');
		printf("%s\r",neul_dev.wbuf);
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    
    /* query the message if send */
    memset(neul_dev.rbuf, 0, 64);
    ret = neul_dev.ops->dev_write(cmd2, strlen(cmd2), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 64, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"SENT="); 
    if (!str)
    {
        return -1;
    }
    sscanf(str, "SENT=%d,%s", &curcnt, neul_dev.wbuf);
    if (curcnt == sndcnt)
    {
        return -1;
    }
    sndcnt = curcnt;
    return 0;
}

/* ============================================================
func name   :  neul_bc95_read_coap_msg
discription :  recv coap message(hex data) from cdp server 
               
param       :  buf @ input param , the data that send to cdp
               sendlen @ input param, data length
return      :  >=0 is read data length , <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_unrmsg_count(void)
{
    char *cmd = "AT+NQMGR\r\n";
    int ret = 0;
    char *str = NULL;
    int msgcnt = 0;
    
    memset(neul_dev.rbuf, 0, 128);
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 128, 0, 100);
    if (ret < 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"BUFFERED");
    if (!str)
    {
        return 0;
    }
    sscanf(neul_dev.rbuf, "\r\nBUFFERED=%d,%s", &msgcnt, neul_dev.wbuf);
    if (msgcnt < 0 )
    {
        return 0;
    }
    return msgcnt;
}

/* ============================================================
func name   :  neul_bc95_read_coap_msg
discription :  recv coap message(hex data) from cdp server 
               
param       :  buf @ input param , the data that send to cdp
               sendlen @ input param, data length
return      :  >=0 is read data length , <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_read_coap_msg(char *outbuf, int maxrlen)
{
    char *cmd = "AT+NMGR\r\n";
    int ret = 0;
    char *str = NULL;
    int readlen = 0;

    if (NULL == outbuf || maxrlen == 0)
    {
        return -1;
    }

    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, NEUL_MAX_BUF_SIZE);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, NEUL_MAX_BUF_SIZE, 0, 540);
    if (ret < 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str && strlen(neul_dev.rbuf) <=10 )
    {
        return 0;
    }
    sscanf(neul_dev.rbuf, "\r\n%d,%s\r\n\r\nOK\r\n",&readlen, neul_bc95_tmpbuf);
    if (readlen > 0)
    {
        neul_bc95_hex_to_str(neul_bc95_tmpbuf, readlen*2, outbuf);
        return readlen;
    }
    return 0;
}



/* ============================================================
func name   :  neul_bc95_get_time
discription :  Return Current Date & Time               
param       :  buf @ out param
return      : 0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_time(char *buf)
{
	  int ret = 0;
    char *cmd = "AT+CCLK?\r\n";
    
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CCLK:%s",buf );
	
    return 0;
}
/* ============================================================
func name   :  neul_bc95_get_csq
discription :  Get Signal Strength Indicator               
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_csq(void)
{
	  int ret = 0;
    char *cmd = "AT+CSQ\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CSQ:%d,",&buf);
		
    return buf;
}
/* ============================================================
func name   :  neul_bc95_get_cfun
discription :  Get Phone Functionality               
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_cfun(void)
{
	  int ret = 0;
    char *cmd = "AT+CFUN?\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CFUN:%d,",&buf);
		
    return buf;
}
/* ============================================================
func name   :  neul_bc95_get_cscon
discription :  Get Signalling Connection Status               
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_cscon(void)
{
	  int ret = 0;
    char *cmd = "AT+CSCON?\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CSCON:%d,%d\r",&ret,&buf);
		
    return buf;
}
/* ============================================================
func name   :  neul_bc95_get_cereg
discription :  Get Network Registration Status               
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_cereg(void)
{
	  int ret = 0;
    char *cmd = "AT+CEREG?\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CEREG:%d,%d\r",&ret,&buf);
		
    return buf;
}
/* ============================================================
func name   :  neul_bc95_get_cgatt
discription :  Request PS Attach or Detach               
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_cgatt(void)
{
	  int ret = 0;
    char *cmd = "AT+CGATT?\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CGATT:%d\r",&buf);
		
    return buf;
}
/* ============================================================
func name   :  neul_bc95_set_npsmr
discription : Set Power Saving Mode Status              
param       :  buf @ out param
return      : 0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_set_npsmr(void)
{
	  int ret = 0;
	  char *cmd = "AT+NPSMR=1\r\n";
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    return 0;
}
/* ============================================================
func name   :  neul_bc95_get_npsmr
discription :  Request Power Saving Mode Status              
param       :  buf @ out param
return      : >0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_npsmr(void)
{
	  int ret = 0;
    char *cmd = "AT+NPSMR?\r\n";
    int buf=0;
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+NPSMR:%d,%d\r",&ret,&buf);
    return buf;
}

/* ============================================================
func name   :  neul_bc95_get_id
discription :  Request IP address of the device             
param       :  buf @ out param
return      : 0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_id(char *buf)
{
	  int ret = 0;
    char *cmd = "AT+CGPADDR=0\r\n";
    char *str = NULL;		
		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r+CGPADDR:%s\r",buf);
    return 0;
}
/* ============================================================
func name   :  neul_bc95_get_nuestats
discription :  RQuery UE Statistics             
param       :  buf @ out param
return      : 0 mean ok, <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_get_nuestats(void)
{
	  int ret = 0;
    char *cmd = "AT+NUESTATS\r\n";
    char *str = NULL;		
    ret = neul_dev.ops->dev_write(cmd, strlen(cmd), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 1000);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 1000, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
		sscanf(neul_dev.rbuf, "\rSignal power:%s\rTotal power:%s\rTX power:%s\rTX time:%s\rRX time:%s\rCell ID:%s\rECL:%s\rSNR:%s\rEARFCN:%s\rPCI:%s\rRSRQ:%s\r",
		                          bc95_net_data.net_Signal_power,
															bc95_net_data.net_Total_power,
															bc95_net_data.net_TX_power,
															bc95_net_data.net_TX_time,
															bc95_net_data.net_RX_time,
															bc95_net_data.net_SCell_ID,
															bc95_net_data.net_ECL,
															bc95_net_data.net_SNR,
															bc95_net_data.net_EARFCN,
															bc95_net_data.net_PCI,
															bc95_net_data.net_RSRQ);
		//printf("%s",neul_dev.rbuf);
    return 0;
}

/* ============================================================
func name   :  neul_bc95_create_udpsocket
discription :  create udp socket 
               
param       :  localport @ input param , the port local socket used
return      :  >=0 socket handle , < 0 not ok
Revision    : 
=============================================================== */
int neul_bc95_create_udpsocket(unsigned short localport)
{
    char *cmd = "AT+NSOCR=DGRAM,17,";
    int ret = 0;
    char *str = NULL;
    int socket = -1;
    if (0 == localport)
    {
        return -1;
    }
    memset(neul_dev.wbuf, 0, 64);
    sprintf(neul_dev.wbuf, "%s%d,1\r", cmd, localport);
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "%d\r%s",&socket, neul_bc95_tmpbuf);
    if (socket >= 0)
    {
        return socket;
    }
    return -1;
}

/* ============================================================
func name   :  neul_bc95_close_udpsocket
discription :  close udp socket 
               
param       :  socket @ input param , the socket handle that need close
return      :  0 close ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_close_udpsocket(int socket)
{
    char *cmd = "AT+NSOCL=";
    int ret = 0;
    char *str = NULL;
    
    memset(neul_dev.wbuf, 0, 64);
    sprintf(neul_dev.wbuf, "%s%d\r", cmd, socket);
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }

    neul_bc95_reset_upd_param(socket);
    
    return 0;
}

/* ============================================================
func name   :  neul_bc95_socket_config_remoteinfo
discription :  set remote address info that socket will send data to 
               
param       :  socket @ input param , the socket handle that need close
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_socket_config_remoteinfo(int socket, const char *remoteip, unsigned short port)
{
    int ret = 0;
    if (socket < 0 || NULL == remoteip || port == 0)
    {
        return -1;
    }
    ret = neul_bc95_set_upd_param(remoteip, port, socket);
    return ret;
}

/* ============================================================
func name   :  neul_bc95_udp_send
discription :  send data to socket 
               
param       :  socket @ input param , the data will send to this socket
               buf @ input param, the data buf
               sendlen @ input param, the send data length
return      :  0 send data ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_udp_send(int socket, const char *buf, int sendlen)
{
    //AT+NSOST=0,192.53.100.53,5683,25,400241C7B17401724D0265703D323031363038323331363438
    char *cmd = "AT+NSOST=";
    int ret = 0;
    char *str = NULL;
    
    sscanf(neul_dev.rbuf, "%d\r%s",&socket, neul_bc95_tmpbuf);
    if (socket < 0 || NULL == buf || 0 == sendlen)
    {
        return -1;
    }
    
    memset(neul_dev.wbuf, 0, NEUL_MAX_BUF_SIZE);
    memset(neul_bc95_tmpbuf, 0, NEUL_MAX_BUF_SIZE);
    neul_bc95_hex_to_str((const char *)buf, sendlen, neul_bc95_tmpbuf);
    sprintf(neul_dev.wbuf, "%s%d,%s,%d,%d,%s\r", cmd, socket,
            (neul_dev.addrinfo+socket)->ip,
            (neul_dev.addrinfo+socket)->port,
            sendlen,
            neul_bc95_tmpbuf);
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    return 0;
}

/* ============================================================
func name   :  neul_bc95_udp_read
discription :  read data from socket 
               
param       :  socket @ input param , the data will read from this socket
               buf @ out param, store the data read
               maxrlen @ input param, the max read data length
return      :  >0 read data length , <0 not ok
Revision    : 
=============================================================== */
int neul_bc95_udp_read(int socket,char *buf, int maxrlen, int mode)
{
    //AT+NSORF=0,4 
    char *cmd = "AT+NSORF=";
    int ret = 0;
    char *str = NULL;
    int rlen = 0;
    int rskt = -1;
    int port = 0;
    int readleft = 0;

    if (socket < 0 || NULL == buf || maxrlen <= 0)
    {
        return -1;
    }
    memset(neul_dev.rbuf, 0, 100);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 100, 0, 200);		
		sscanf(neul_dev.rbuf,"\r+NSONMI:0,%d\r",&rlen);
		
    sprintf(neul_dev.wbuf, "%s%d,%d\r", cmd, socket, maxrlen);
    ret = neul_dev.ops->dev_write(neul_dev.wbuf, strlen(neul_dev.wbuf), 0);
    if (ret < 0)
    {
        //write data to bc95 failed
        return -1;
    }
    memset(neul_dev.rbuf, 0, 200);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 200, 0, 200);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    sscanf(neul_dev.rbuf, "\r%d,%s,%d,%d,%s,%d\r%s", &rskt,
                            neul_bc95_tmpbuf,
                            &port,
                            &rlen,
                            neul_bc95_tmpbuf+22,
                            &readleft,
		                        neul_dev.wbuf);
  	printf("neul_dev.rbuf:%s\n",neul_dev.rbuf);	
    if (rlen>0)
    {
        neul_bc95_str_to_hex((const unsigned char *)(neul_bc95_tmpbuf+22),rlen*2, buf);
    }
	
    return rlen;
}

/* ============================================================
func name   :  neul_bc95_set_auto_connect
discription :  set bc95 module auto connect network
               
param       :  None
return      :  0 set ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_set_auto_connect(int flag)
{
    int ret = 0;
    char *str = NULL;
    char *p1 = "AT+NCONFIG=AUTOCONNECT,TRUE\r";
    char *p2 = "AT+NCONFIG=AUTOCONNECT,FALSE\r";

    if (flag)
    {
        //set auto connect
        ret = neul_dev.ops->dev_write(p1, strlen(p1), 0);
    }
    else
    {
        //not auto connect
        ret = neul_dev.ops->dev_write(p2, strlen(p2), 0);
    }
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 300);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    return 0;
}

int neul_bc95_get_auto_connect()
{
    int ret = 0;
    char *str = NULL;
    char *p = "AT+NCONFIG?\r";
    char *p1 = "AUTOCONNECT,TRUE";
    char *p2 = "AUTOCONNECT,FALSE";

    ret = neul_dev.ops->dev_write(p, strlen(p), 0);
    
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 106, 0, 500);
    if (ret <= 0)
    {
        //read bc95 read set return value info failed
        return -1;
    }
    str = strstr(neul_dev.rbuf,"OK");
    if (!str)
    {
        return -1;
    }
    str = strstr(neul_dev.rbuf,p1);
    if (str)
    {
        return 1;
    } 
    str = strstr(neul_dev.rbuf,p2);
    if (str)
    {
        return 2;
    }
    return -1;
}

/* ============================================================
func name   :  neul_bc95_hw_init
discription :  init neul bc95 device 
               
param       :  None
return      :  0 init ok , !0 not ok
Revision    : 
=============================================================== */
int neul_bc95_hw_init(void)
{
    int ret = 0;

    neul_bc95_reboot();
    neul_bc95_sleep(4000);
    ret = neul_bc95_hw_detect();
    if (ret < 0)
    {
        return ret;
    }
    ret = neul_bc95_get_auto_connect();
    if (ret < 0)
    {
        return ret;
    }
    if (ret == 1)
    {
        return 0;
    }
//    neul_bc95_prepare_transmit();
//    //not auto connect
//    neul_bc95_set_auto_connect(0);
//    neul_bc95_sleep(1000);

//    neul_bc95_prepare_transmit();
//    neul_bc95_sleep(200);
//    ret = neul_bc95_active_network();
    
    return ret;
}
int neul_bc95_prepare_transmit(void)
{
    int ret = 0;
    char *p1 = "AT\r\n";
    
    memset(neul_dev.rbuf, 0, 32);
    ret = neul_dev.ops->dev_write(p1, strlen(p1), 0);
    ret = neul_dev.ops->dev_read(neul_dev.rbuf, 32, 0, 500);
    uart_data_flush();
    return ret;
}
int neul_bc95_sleep(int ticks)
{
    LOS_TaskDelay(ticks);
    return 0;
}

//AT+NTSETID=1,460012345678966

//\r\nNeul \r\nOK\r\n
//typedef int (*neul_read)(char *buf, int maxrlen, int mode);
//typedef int (*neul_write)(const char *buf, int writelen, int mode);


//static int uart_data_read(char *buf, int maxrlen, int mode)
int uart_data_read(char *buf, int maxrlen, int mode, int timeout)
{
    int retlen = 0;
    if (NULL == buf || maxrlen <= 0 || timeout < 0)
    {
        return 0;
    }
    if(mode == 0)
    {
        //block mode
        retlen = los_dev_uart_read(CN_UART_AT, buf, maxrlen, timeout);
    }
    else
    {
        //none block mode
        retlen = los_dev_uart_read(CN_UART_AT, buf, maxrlen, 0);
    }
    
    return retlen;
}
//static int uart_data_write(const char *buf, int writelen, int mode)
int uart_data_write(char *buf, int writelen, int mode)
{
    if (NULL == buf || writelen <= 0)
    {
        return 0;
    }
    los_dev_uart_write(CN_UART_AT, buf, writelen, 500);
    return 0;
}

int uart_data_flush(void)
{
    memset(neul_bc95_buf, 0, NEUL_MAX_BUF_SIZE);
	  los_dev_uart_rflush(CN_UART_AT);
	
	
    return 0;
}
