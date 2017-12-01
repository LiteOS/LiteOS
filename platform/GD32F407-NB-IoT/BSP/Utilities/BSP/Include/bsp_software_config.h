/** 
  ******************************************************************************
  * @文件   bsp_software_config.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明   
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/


#ifndef __BSP_SOFTWARE_CONFIG_H
#define __BSP_SOFTWARE_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stdarg.h"





typedef enum {
  MEMS_SUCCESS				=		0x01,
  MEMS_ERROR				=		0x00	
} status_t;

typedef enum {
  MEMS_ENABLE				=		0x01,
  MEMS_DISABLE				=		0x00	
} State_t;


typedef enum {
  pdPASS				=		0x01,
  pdFAIL				=		0x00	
} BaseType_t;

typedef struct
{
  uint16_t Max_Len;     //最大数据长度
  uint16_t Data_Len;    //当前数据长度
  uint16_t Buffer_State; //Buffer状态
  uint8_t *Data_Buffer; //Buffer缓存区
}USER_BUFFER_Typedef;




typedef enum
{
  IDLE_STATE = 0,
  BUSY_STATE
}USER_BUFFER_STATE;

#define BIT0  0x01
#define BIT1  BIT0<<1
#define BIT2  BIT0<<2
#define BIT3  BIT0<<3
#define BIT4  BIT0<<4
#define BIT5  BIT0<<5
#define BIT6  BIT0<<6
#define BIT7  BIT0<<7


#ifdef __cplusplus
}
#endif



#endif

