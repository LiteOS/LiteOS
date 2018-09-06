/**
 *Copyright: Copyright (c) 2018
 *Created on 2018-8-6
 *Author: IoTCluB
 *Version 0.1
 *Title: Club oled.h
 *Addr: www.iot-club.cn
**/

#ifndef __CLUB_OLED__
#define __CLUB_OLED__

#include "sys_init.h"
#include "club_func.h"
#include "oled.h"

#define OLED_MAX_QUEUE 5


typedef struct{
	char 	*data;		//数据地址指针
	UINT32	dataLen;	//数据长度
	UINT8	x;			//x 地址
	UINT8 	y;			//y 地址
	UINT8	size;		//字体大小
	UINT8 	font;		//选择字体
	UINT8	autoEnter;	//是否自动换行
}OLED_Data_Type;


void club_oled_init();
void proc_oled_task(UINT32 uwArg);
void club_oled_send_data(char *data, UINT8 dataLen, UINT8 x, UINT8 y, UINT8 size);

extern UINT32 g_oledQueue;



#endif
