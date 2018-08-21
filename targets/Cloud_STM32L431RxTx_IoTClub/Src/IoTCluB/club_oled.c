/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club oled.c
 *Addr: www.iot-club.cn
**/


#include "club_oled.h"


UINT32 			g_oledQueue = 0;	//OLED消息句柄
OLED_Data_Type	oledDataBuf;		//OLED数据


/* OLED 初始化 */
void club_oled_init(){
	UINT32 uwRet;
    char *str = "IoT-Club good";
    OLED_Init();   
    OLED_Clear();

	//创建一个OLED数据队列
	uwRet = LOS_QueueCreate("OLED_DATA", OLED_MAX_QUEUE, &g_oledQueue, 0, sizeof(OLED_Data_Type));
	if(uwRet != LOS_OK){
		CLUB_DEBUG("OLED %d create ERROR:%d\r\n", g_oledQueue, uwRet);
	}else{
		CLUB_DEBUG("OLED %d create OK:%d\r\n", g_oledQueue, uwRet);
	}

}

void club_oled_send_data(char *data, UINT8 dataLen, UINT8 x, UINT8 y, UINT8 size){
	UINT32 uwRet;
	OLED_Data_Type oledDataBufTemp = {0};
	oledDataBufTemp.data 	= data;
	oledDataBufTemp.dataLen = dataLen;
	oledDataBufTemp.x 		= x;
	oledDataBufTemp.y 		= y;
	oledDataBufTemp.size 	= size;
	uwRet = LOS_QueueWriteCopy(g_oledQueue, &oledDataBufTemp, sizeof(oledDataBufTemp), LOS_WAIT_FOREVER);
	if(uwRet != LOS_OK){
		CLUB_DEBUG("Send %d OLED data error:%d\r\n", g_oledQueue, uwRet);
	}else{
		CLUB_DEBUG("Send %d OLED data OK:%d\r\n", g_oledQueue, uwRet);
	}
}


//oled 刷新程序
void proc_oled_task(UINT32 uwArg){
	UINT32 uwRet, dataLen;
	OLED_Data_Type oledDataBufTemp = {0};
	char *str = "IoT-Club glld";
	printf("This is oled task\r\n");
	for(;;){
		printf("Begin read:%d\r\n", g_oledQueue);
		uwRet = LOS_QueueReadCopy(g_oledQueue, &oledDataBufTemp, &dataLen, LOS_WAIT_FOREVER);
		if(uwRet != LOS_OK){
			//读取消息失败
			CLUB_DEBUG("Recv error:%d\r\n", uwRet);
		}else{
			static UINT8 a = 0;

			
			//OLED_ShowString(oledDataBuf.,16);
			OLED_Clear();
			OLED_ShowString(oledDataBuf.x, oledDataBuf.y, (uint8_t *)oledDataBuf.data, oledDataBuf.size);
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(18,0,1);
			OLED_ShowCHinese(36,0,2);
			OLED_ShowCHinese(54,0,3);
			OLED_ShowCHinese(72,0,4);
			OLED_ShowCHinese(90,0,5);
			OLED_ShowNum(0, 2, a, 1, 16);
			a++;
			
			CLUB_DEBUG("Recv OK:%d\r\n", uwRet);
			//收到消息
		}
	}
	
}