/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club nbiot.h
 *Addr: www.iot-club.cn
**/
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)

#include "club_nbiot.h"

UINT32 				g_nbiotQueue = 0;	//nbiot消息句柄
platformAddrType 	platformAddr = {0};
NBIOT_Queue_Type	nbiot_queue;
UINT32 				nbiot_data_pool[NBIOT_MAX_MEM/4];
//地址赋值

extern int32_t nb_cmd_data_ioctl(void* arg,int8_t * buf, int32_t len);
void club_nbiot_init(char *ipAddr, char *port){
	
	UINT32 uwRet;
	platformAddr.IPAddr 	= 	(UINT8*)ipAddr;
	platformAddr.Port 		=	(UINT8*)port;


	
	//创建一个NBIOT数据队列
	uwRet = LOS_QueueCreate("NBIOT", NBIOT_MAX_QUEUE, &g_nbiotQueue, 0, sizeof(NBIOT_Queue_Type));
	if(uwRet != LOS_OK){
		CLUB_DEBUG("NBIOT QUEUE %d create ERROR:%d\r\n", g_nbiotQueue, uwRet);
	}else{
		CLUB_DEBUG("NBIOT QUEUE %d create OK:%d\r\n", g_nbiotQueue, uwRet);
	}
	
	//申请一段存储临时发送数据的内存
	uwRet  = LOS_MemInit(nbiot_data_pool, NBIOT_MAX_MEM);
	if(uwRet != LOS_OK){
		CLUB_DEBUG("NBIOT data pool is error:%d\r\n", uwRet);
	}else{
		CLUB_DEBUG("NBIOT data pool is ok\r\n");
	}
	
	
	//连接至平台
	club_nbiot_connect();
	
	//注册平台数据回调
	club_nbiot_URC_call("+NNMI", strlen("+NNMI:"), nb_cmd_data_ioctl);

}

//连接到平台
//NB_STAT_OK
//NB_STAT_ERR_SIM
//NB_STAT_ERR_NET

NBIOT_CON_STAT club_nbiot_connect(){
	los_nb_init((const int8_t* )(platformAddr.IPAddr), (const int8_t*)(platformAddr.Port), NULL);
	return NB_STAT_OK;
}

//注册URC上报消息，一旦收到注册消息调用回调函数
void club_nbiot_URC_call(char *URCStr, UINT16 dataLen, oob_callback callback){
	los_nb_notify(URCStr, dataLen, callback);
}

//重启NBIOT
int club_nbiot_reset(){
	return 0;
}

//发送消息到OC平台
int club_nbiot_send(char *data, UINT32 dataLen){

	UINT32 uwRet;
	UINT32 *dataBufTemp 					= 	NULL;
	
	NBIOT_Queue_Type	nbiotSendTemp 	= 	{NB_MSG_SEND_DATA, 0, 0};
	dataBufTemp 						= 	LOS_MemAlloc(nbiot_data_pool, dataLen);	//申请一段内存
	nbiotSendTemp.msgParam1 			=	(UINT32*)dataBufTemp;					//数据指针地址
	nbiotSendTemp.msgParam2				= 	(UINT32*)dataLen;						//数据长度
	
	if(dataBufTemp != NULL){
		memcpy(dataBufTemp,data,dataLen);
		//内存申请成功，以消息方式发送给NB任务
		uwRet = LOS_QueueWriteCopy(g_nbiotQueue, &nbiotSendTemp, sizeof(nbiotSendTemp), LOS_NO_WAIT);
		if(uwRet != LOS_OK){
			CLUB_DEBUG("Send MSG Data error:%d\r\n", uwRet);
		}	
		
	}
	else{
		
		//内存申请失败
		CLUB_DEBUG("Mem is error\r\n");
		
	}

	return uwRet;
}

//nbiot任务
void proc_nbiot_task(UINT32 uwArg){
	
	UINT32 uwRet, dataLen;
	int ret;
	NBIOT_Queue_Type	nbiotRecv;
	UINT32 *pSendData = NULL;
	UINT32 SendDataLen = 0;
	
	printf("This is nbiot task\r\n");
	for(;;){
		uwRet = LOS_QueueReadCopy(g_nbiotQueue, &nbiotRecv, &dataLen, LOS_WAIT_FOREVER);
		if(uwRet != LOS_OK){
			//读取消息失败
			CLUB_DEBUG("Com Recv error:%d\r\n", uwRet);
		}	
		
		switch(nbiotRecv.msgType){
			case NB_MSG_REBOOT:		//收到重启NB命令
			{
				break;
			}
			case NB_MSG_SEND_DATA:	//收到发送NB消息命令
			{
				pSendData = (UINT32 *)(nbiotRecv.msgParam1);
				SendDataLen = (UINT32)(nbiotRecv.msgParam2);
				ret = los_nb_report((const char*)pSendData, SendDataLen);				
				if(ret >= 0)
				printf("ocean_send_data OK!\n");                                                //发生成功
		    else                                                                            //发送失败
				printf("ocean_send_data Fail!\n"); 			
				LOS_MemFree(nbiot_data_pool, pSendData);	//释放内存
				break;
			}
			default:
				break;
			
		}

	}	
	
}
#endif

