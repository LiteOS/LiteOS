/** 
  ******************************************************************************
  * @文件   NBxx_01_Udp.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明   
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/


#ifndef __NBXX_01_UDP_H
#define __NBXX_01_UDP_H

#include "nb-iot_app.h"

#ifdef __cplusplus
 extern "C" {
#endif
   
   
   
   
   
#pragma  pack(push)  //保存对齐状态  
#pragma  pack(1)    
typedef struct{ 
  char   UDP_server_ip[16+1];   // 服务器地址
  uint16_t  UDP_server_port;  // 服务器端口号 
  uint8_t ack_en;
  uint16_t rev_timeout;
}Udp_info;

#pragma  pack(1) 
typedef struct
{
  uint16_t socket;
  uint8_t ip_addr[4];
  uint16_t port;
  uint16_t len;
  char data[NBxx_BUFFER_SIZE];
}NB_RECV_PACK;
#pragma pack(pop)

#define NO_ACK  0
#define ACK_EN  1



extern NB_RECV_PACK  nb_recv_pack;
extern Udp_info UDP_run_info;
void initUdp(uint8_t *ip,uint16_t port);   
void createUDPSocket(uint16_t port);
void RetrysendUdpData(void);
void sendUdpData(const uint8_t * ip,uint16_t dst_port,const uint8_t * hex,uint16_t datalen,uint8_t ack);
//void sendUdpData_with_ack(const uint8_t * ip,uint16_t dst_port,const uint8_t * hex,uint16_t datalen,uint16_t revtimeout,uint8_t *revbuf,uint16_t *revlen);
bool receive_udp_data(uint8_t ack_en);   
void report_udp(void);   
RevStatus convert_rec_data(char *buf);
void setServer_down_CmdNum(uint8_t num);
uint8_t getServer_down_CmdNum(void);  
   
#ifdef __cplusplus
 }
#endif

#endif   
   