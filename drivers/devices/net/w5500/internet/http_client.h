#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "types.h"

#define yeelink_id				"yingshi"        //your yeelink ID
#define yeelink_pwd				"ing10bbs"     //your yeelink password
extern uint8 yeelink_server_ip[];
extern uint8 yeelink_port;

void do_http_client(void);//TCP Clinet回环演示函数

#endif 

