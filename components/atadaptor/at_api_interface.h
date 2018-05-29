#ifndef __AT_API_INTERFACE_H__
#define __AT_API_INTERFACE_H__
#include "stm32f4xx_hal.h"

typedef struct {
	int8_t  * name;     /*模块的名字，字符串，例如SIM900A*/
//	uint32_t  mux_mode;/*0:单连接，1：多连接*/
	uint32_t  timeout;  /*超时时间*/
	int32_t  (*init)(void);  /*初始化，初始化串口、IP网络等*/
//	int32_t  (*get_id)(void);/*获取连接id，仅多连接时需要*/
	int8_t* (*get_localip)(void);/*获取本地IP*/
	/*建立TCP或者UDP连接*/
	int32_t  (*connect)(const int8_t * host, const int8_t *port, int32_t proto);
	/*发送，当命令发送后，如果超过一定的时间没收到应答，要返回错误*/
	int32_t  (*send)(int32_t id , const uint8_t  *buf, uint32_t len);
    int32_t  (*recv_timeout)(int32_t id , int8_t  *buf, uint32_t len, int32_t timeout);
	int32_t  (*recv)(int32_t id , int8_t  *buf, uint32_t len);
   
	int32_t  (*close)(int32_t id);/*关闭连接*/
	int32_t  (*recv_cb)(int32_t id);/*收到各种事件处理，暂不实现 

	*/
	int32_t  (*deinit)(void);
}at_adaptor_api;

int32_t at_api_register(at_adaptor_api *api);
int32_t at_api_init();
int32_t at_api_send(int32_t id , const uint8_t  *buf, uint32_t len);
int32_t at_api_recv_timeout(int32_t id, unsigned char* buf, size_t len, uint32_t timeout);
int32_t at_api_connect(const char* host, const char* port, int proto);
int32_t at_api_close(int32_t fd);

extern at_adaptor_api at_interface;

#endif