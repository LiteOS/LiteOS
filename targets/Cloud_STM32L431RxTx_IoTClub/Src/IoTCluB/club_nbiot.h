/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club oled.h
 *Addr: www.iot-club.cn
**/

#ifndef __CLUB_NBIOT__
#define __CLUB_NBIOT__

#include "sys_init.h"
#include "club_include.h"
#include "los_nb_api.h"
//#include "cmsis_os.h"
#include "club_func.h"

#define NBIOT_MAX_QUEUE 20
#define NBIOT_MAX_MEM	(1024 * 2)


typedef struct{
	UINT8 *IPAddr;
	UINT8 *Port;
}platformAddrType;

typedef enum {
	NB_STAT_OK = 0,
	NB_STAT_ERR_SIM,
	NB_STAT_ERR_NET,
}NBIOT_CON_STAT;

typedef enum {
	NB_MSG_REBOOT = 0,		//重启
	NB_MSG_SEND_DATA,		//发送数据，后面紧跟param1 和 param2
	NB_MSG_READ_CSQ,
}NBIOT_MSG_TYPE;

typedef struct{
	NBIOT_MSG_TYPE 	msgType;
	UINT32			*msgParam1;
	UINT32 			*msgParam2;
}NBIOT_Queue_Type;

typedef struct{
	char *data;
	UINT32 dataLen;
}NBIOT_Data_Type;


void proc_nbiot_task(UINT32 uwArg);
void club_nbiot_init(char *ipAddr, char *port);
void club_nbiot_URC_call(char *URCStr, UINT16 dataLen, oob_callback callback);
int club_nbiot_send(char *data, UINT32 dataLen);
NBIOT_CON_STAT club_nbiot_connect();




#endif
