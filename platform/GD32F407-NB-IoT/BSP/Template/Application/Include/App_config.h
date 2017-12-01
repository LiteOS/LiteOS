/** 
  ******************************************************************************
  * @文件   App_config.h
  * @作者   罗桂发
  * @版本   V1.0.0
  * @日期   2017-05-03 
  * @说明    文件
  ********  利尔达科技集团股份有限公司  www.lierda.com ***********************           
  *
***/
#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum{
  Conn_off=0,
  Conn_trying=1,
  Conn_on=2,
}ConnState;   



typedef enum {
  Net_type_UDP,
  Net_type_TCP,
  Net_type_COAP
}Net_type;

/**
 * 配置表结构体
 */
#define ENV_PARAM_USED_SIZE        (4+1+1+8+1+4 + 118)
typedef struct env_parameters
{
    /**
     * 参数表序列号，如果参数在ram中，此参数不会被用到，但在flash中    
     * 此参数标明了此参数块是最新的参数配置表，此变量越大，参数越新
     *
     */
    uint32_t sequence_num;  // 4
 
    uint8_t crc_checksum; // 1
 
     /********* 以下为运行时可配参数据  **********************/
    char   net_type[5]; // 通信协议 UDP/TCP/CoAP
    char   server_ip[16+1];   // 服务器地址
    char   server_port[6];  // 服务器端口号
     
    int16_t rsrpthreshold;
    int16_t sinrthreshold;
    
    uint32_t report_period;  //定时上报周期（秒）
}env_parameters;

#if !defined(ENV_SIZE)
#define ENV_SIZE                     (sizeof(env_parameters))
#endif










 
extern volatile ConnState isConn;
extern  env_parameters Sys_parameters_working;

void init_sys_parameters(void);
Net_type get_net_type(void);
void setConnect(ConnState state);
ConnState getConnectState(void);
void updata_sensor(void);

void setMaxRetryCnt(uint8_t rty_max_num);
void ClearCurrent_rty_num(void);
uint8_t getCurrent_rty_num(void);
uint8_t getMax_rty_num(void);
void current_rty_num_add_1(void);

bool isFirstPowerOn(void);
void set_FirstPowerOn_flag(void);
void clearFirstPowerOn_flag(void);






#ifdef __cplusplus
 }
#endif

#endif


