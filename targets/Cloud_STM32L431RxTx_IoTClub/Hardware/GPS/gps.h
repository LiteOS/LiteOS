#ifndef __gps_h__
#define __gps_h__
#include "stm32l4xx_hal.h"
#include "string.h"
#include "stdio.h"

/***************************************************\
*GPS NMEA-0183协议重要参数结构体定义
*卫星信息
\***************************************************/
__packed typedef struct
{
	uint32_t latitude_bd;					//纬度   分扩大100000倍，实际要除以100000
	uint8_t nshemi_bd;						//北纬/南纬,N:北纬;S:南纬	
	uint32_t longitude_bd;			    	//经度 分扩大100000倍,实际要除以100000
	uint8_t ewhemi_bd;						//东经/西经,E:东经;W:西经
}gps_msg;
/***************************************************\
*其他函数声明
\***************************************************/
uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cx);
uint32_t NMEA_Pow(uint8_t m,uint8_t n);
int NMEA_Str2num(uint8_t *buf,uint8_t*dx);

/***************************************************\
*初始化GPS
\***************************************************/
void GPS_Init(void);

/***************************************************\
*GPS数据接口
\***************************************************/
void NMEA_BDS_GPRMC_Analysis(gps_msg *gpsmsg,uint8_t *buf);
#endif
