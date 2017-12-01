/** 
  ******************************************************************************
  * @文件   bsp_nbxx_01.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明   
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/

#ifndef __BSP_NBXX_01_H
#define __BSP_NBXX_01_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include "platform_config.h"
#include <stdbool.h> 
#include <stdio.h>
     
#define log_s   printf
     
#define DATA_REC_NO_FINISH  0
#define DATA_REC_IS_ING     1
#define DATA_REC_FINISH     2

   
   
   
   
#define NB_AT_CFUN0    0
#define NB_AT_CGSN     1
#define NB_AT_NRB      2 
#define NB_AT_CFUN1    3     
#define NB_AT_CIMI     4
#define NB_AT_CMEE     5
#define NB_AT_CGDCONT  6  
#define NB_AT_NNMI     7
#define NB_AT_CGATT    8
#define NB_AT_CSCON1   9
#define NB_AT_CGPADDR  10
#define NB_AT_GETSIGNAL 11
#define NB_AT_NSOCL    12
#define NB_AT_NSOCR    13
#define NB_SEND_DATA   14
#define NB_SEND_DATA_ACK 15
#define NB_AT_NSORF    16
    
/*
typedef enum
{
  NO_BACK=0,
  BACK_TIMEOUT,
  BACK_TRUE,
}NB_back_result; 
*/
#define NO_BACK  0x00
#define BACK_TIMEOUT  0x01
#define BACK_TRUE  0x02

#define NB_back_result  uint8_t
  
   
   
#pragma  pack(push)  //保存对齐状态  
#pragma  pack(1) 
typedef struct{
  char *cmdstr;
  uint16_t revtimeout;
  char *cmdtruebackstr;
  NB_back_result  revresult;
  uint8_t rty_num;
}NBIOT_CMD_Data_t;    
   
#pragma  pack(1)    
typedef struct {
  char rec_buffer[NBxx_BUFFER_SIZE * 3];
  char *p_read;
  char *p_write;
  uint8_t rec_data_finish_flag;
  uint16_t max_len;
}Nb_rec_buffer;


#pragma pack(pop)



extern Nb_rec_buffer nb_rec_buffer;

extern uint8_t aRxBuffer[];

extern volatile NBIOT_CMD_Data_t nbiot_cmd_data[];

uint8_t nbxx_01_current_cmd_get(void);
void nbxx_01_recvpacket_num_down_1(void);
uint8_t nbxx_01_recvpacket_num_get(void);
void nbxx_01_found_result_set(uint8_t cmdnum,char *findstr,NB_back_result *result);
void nbxx_01_uart_read(void);
uint16_t nbxx_01_revlen_get(void);
void nbxx_01_uart_init(void);
uint8_t nbxx_01_uart_send(const uint8_t *fmt, uint16_t len);
void nbxx_01_timeout_timer_init(uint16_t time);
void nbxx_01_set_ack_timeout(uint16_t time);
void nbxx_01_recvbuffer_clear(void);

#ifdef __cplusplus
}
#endif


#endif

