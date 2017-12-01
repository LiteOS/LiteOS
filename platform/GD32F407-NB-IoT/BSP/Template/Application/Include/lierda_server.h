/** 
  ******************************************************************************
  * @文件   lierda_server.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-10
  * @说明   利尔达服务器通信协议
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/

#ifndef __LIERDA_SERVER_H
#define __LIERDA_SERVER_H

#include "stdint.h"
#include "stdbool.h"
#ifdef __cplusplus
 extern "C" {
#endif
   
//iot到服务器的定时数据
#pragma  pack(push)  //保存对齐状态  
#pragma  pack(1)  
typedef struct 
{ 
  uint16_t start;//0xffaa
  uint16_t cmd; //0Xd0d0
  uint8_t version;//00:period data;01:one key test;02:wdt
  uint16_t  len; //package len
  uint8_t device_sn_len; //
  uint8_t device_sn[15]; // 设备编号是可变长度
  uint32_t mesg_send_num;
  uint32_t mesg_acked_num;
  uint64_t unix_time;
  int16_t RSPR;
  int16_t SINR;
  uint8_t cover_level;
  uint16_t repat_num;
  uint8_t  flag;
  //--------------------填充数据-----------------------------
  uint16_t ap_data_len; //填充数据长度
  ////////应用数据结束
  
}Lierda_sever_UP_package ;   
 
#pragma  pack(1)  
typedef struct
{
  uint8_t type; //类型
  uint16_t temper; //0XD1D1
  uint16_t humidity;
  uint32_t light;
  uint8_t  gps_flag;
  uint32_t longitude;
  uint32_t latitude;
  uint8_t led_state;
  uint32_t pressure;
  uint8_t  battery;
}APP_data_t;

#pragma  pack(1)  
typedef struct
{
  uint16_t start;
  uint16_t cmd; //0XD1D1
  uint8_t version;
  uint16_t  len;
  uint8_t device_sn_len; //
  uint8_t device_sn[15]; // 设备编号是可变长度
  uint64_t unix_time;
  uint8_t cmd_num;

}Lierda_sever_DOWN_package;

#pragma  pack(1)  
typedef struct SCmd
{
  uint8_t       cmd;
  uint32_t       cmd_sn;
  uint8_t        flag;
//lgf0508 uint16_t       report_duaration;
  uint32_t       report_duaration;
  uint16_t        package_len;
//lgf0508  uint8_t         reserve_byte;
  uint16_t         reserve_byte;
  uint8_t         max_retry;
  uint8_t         net_mode; 
  struct SCmd *next;
}Server_cmd;

#pragma  pack(1) 
typedef struct 
{
 // uint8_t cmd;
  uint32_t cmd_sn;
  uint8_t flag;
}Nb_ack_cmd;

#pragma  pack(1) 
typedef struct
{
  uint16_t start;   
  uint16_t cmd;
  uint8_t version;
  uint16_t  len;
  uint8_t device_sn_len; //
  uint8_t device_sn[15]; // 设备编号是可变长度
  uint8_t cmd_num;
 
} Nb_ack_Lierda_sever_package;


#pragma pack(pop)

#define APP_DATA_LEN  sizeof(APP_data_t)
#define CMD0XC1_LEN  18
#define CMD0XC2_LEN  8

#define SERVER_DOWN_MAX_CMDNUM  5
extern Nb_ack_cmd server_down_cmd[];

extern  Nb_ack_Lierda_sever_package  nb_ack_sever_package;
extern Lierda_sever_UP_package nb_to_lierda_sever_package;  
extern Lierda_sever_DOWN_package lierda_sever_to_nb_package;
extern APP_data_t  app_data;
 
 
void updata_to_lierda_server_package(void);
uint16_t get_lierda_report_Package(void);   
bool parseServerAckPackage(char *data,uint16_t len);    
void set_AckedNum_add_1(void);
uint16_t configNbAckPackage(uint8_t *buf);
   
#ifdef __cplusplus
 }
#endif
   
#endif
   
   
   
   
   
   
   
   