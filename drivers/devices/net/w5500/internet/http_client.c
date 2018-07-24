/**
******************************************************************************
* @file   		http_client.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		http_client 演示函数 
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "http_client.h"
#include "w5500.h"
#include "w5500_conf.h"
#include "socket.h"
#include "utility.h"

uint8 temp_rh[2];													/*temp_rh[0]是湿度 temp_rh[1]是温度*/
char  postT[]={
"POST /v1.0/device/346169/sensor/385835/datapoints HTTP/1.1\r\n"
"Host:api.yeelink.net\r\n"
"Accept:*/*\r\n"
"U-ApiKey:97b7ab1eb1c309628ae74bf9e52b1d4a\r\n"
"Content-Length:12\r\n"
"Content-Type:application/x-www-form-urlencoded\r\n"
"Connection:close\r\n"
"\r\n"
"{\"value\":xx}\r\n"
};/*xx用来填充温度数值*/

char  postH[]={//提交湿度
"POST /v1.0/device/346169/sensor/385837/datapoints HTTP/1.1\r\n"
"Host:api.yeelink.net\r\n"
"Accept:*/*\r\n"
"U-ApiKey:97b7ab1eb1c309628ae74bf9e52b1d4a\r\n"
"Content-Length:12\r\n"
"Content-Type:application/x-www-form-urlencoded\r\n"
"Connection:close\r\n"
"\r\n"
"{\"value\":xx}\r\n"
};/*xx用来填充温度数值*/
char *post[]={postH,postT};

uint8 yeelink_server_ip[4]={42,96,164,52};							/*api.yeelink.net  的ip地址*/
uint8 yeelink_port = 80;												/*api.yeelink.net  的端口号*/	

uint8 buffer[1024]={0};

/**
*@brief		执行http client主函数
*@param		无
*@return	无
*/
void do_http_client(void)
{	
	static uint8 i=0;
	uint8 ch=SOCK_HUMTEM;
	uint16 anyport=3000;
	switch(getSn_SR(ch))													/*获取socket SOCK_TCPS 的状态*/
	{
		case SOCK_INIT:														/*socket初始化完成*/
			connect(ch, yeelink_server_ip ,yeelink_port);					/*打开socket端口*/
		break;	
		case SOCK_ESTABLISHED: 												/*socket连接建立*/
			if(getSn_IR(ch) & Sn_IR_CON)  
			{
				setSn_IR(ch, Sn_IR_CON);									/*清除接收中断标志*/
			}

			if( SUCCESS )			/*判断是否得到AM2302数据*/
			{
				temp_rh[0]=(uint8_t)10;				/*8bit 湿度整数	*/
				temp_rh[1]=(uint8_t)26;				/*8bit 温度整数*/
				memcpy(buffer,post[i],strlen(post[i]));	
        
				buffer[231]=temp_rh[i]/10+0x30;		  
				buffer[232]=temp_rh[i]%10+0x30;        
				send(ch,(const uint8 *)buffer,sizeof(buffer));				/*W5500向 Yeelink服务器发送数据*/        
				i=1;
				
			}
			wizDelayMs(1000);
			
		break;
		case SOCK_CLOSE_WAIT:  												/*socket等待关闭状态*/
			close(ch);  
		break;
		case SOCK_CLOSED: 													/*socket关闭*/
            if(i == 1)printf("Send to Yeelink:OK\r\n");   
            i = 0;    
			socket(ch, Sn_MR_TCP,anyport++ , 0x00);   
		break;
		default:
		break;
	}  
}

