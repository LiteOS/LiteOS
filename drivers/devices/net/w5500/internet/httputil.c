/**
******************************************************************************
* @file    			httputil.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief   	  	http中需要的实用函数
******************************************************************************
*/
#include "httputil.h"
#include "webpge.h"
#include "utility.h"

#define DEBUG_HTTP

extern CONFIG_MSG  ConfigMsg;
extern char tx_buf[MAX_URI_SIZE];
extern char rx_buf[MAX_URI_SIZE];
extern uint8 reboot_flag;

uint8 boundary[64];
uint8 tmp_buf[1460]={0xff,};
extern uint8 pub_buf[1024];

/**
*@brief		将基本的配置信息设置到json_callback
*@param		无
*@return	无
*/
static void make_basic_config_setting_json_callback(int8* buf, CONFIG_MSG config_msg)
{
  sprintf(buf,"settingsCallback({\"ver\":\"%d.%d\",\
                \"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\
                \"ip\":\"%d.%d.%d.%d\",\
                \"gw\":\"%d.%d.%d.%d\",\
                \"sub\":\"%d.%d.%d.%d\",\
                });",config_msg.sw_ver[0],config_msg.sw_ver[1],
                config_msg.mac[0],config_msg.mac[1],config_msg.mac[2],config_msg.mac[3],config_msg.mac[4],config_msg.mac[5],
                config_msg.lip[0],config_msg.lip[1],config_msg.lip[2],config_msg.lip[3],
                config_msg.gw[0],config_msg.gw[1],config_msg.gw[2],config_msg.gw[3],
                config_msg.sub[0],config_msg.sub[1],config_msg.sub[2],config_msg.sub[3]
                );
}

/**
*@brief		完成http响应
*@param		无
*@return	无
*/
void do_https(void)
{
	uint8 ch=SOCK_HTTPS;												/*定义一个socket*/
	uint16 len;
	
	st_http_request *http_request;										/*定义一个结构指针*/
	memset(rx_buf,0x00,MAX_URI_SIZE);
	http_request = (st_http_request*)rx_buf;					 
	/* http service start */
	switch(getSn_SR(ch))												/*获取socket状态*/
	{
		case SOCK_INIT:													/*socket处于初始化状态*/
			listen(ch);
			break;
		
		case SOCK_LISTEN:												/*socket处于监听状态*/
			break;
		
		case SOCK_ESTABLISHED:											/*socket处于连接状态*/
			if(getSn_IR(ch) & Sn_IR_CON)
			{
				setSn_IR(ch, Sn_IR_CON);								/*清除中断标志位*/
			}
			if ((len = getSn_RX_RSR(ch)) > 0)		
			{
				len = recv(ch, (uint8*)http_request, len); 				/*接收http请求*/
				*(((uint8*)http_request)+len) = 0;
				proc_http(ch, (uint8*)http_request);					/*接收http请求并发送http响应*/
				disconnect(ch);
			}
			break;
			
		case SOCK_CLOSE_WAIT:   										/*socket处于等待关闭状态*/
			if ((len = getSn_RX_RSR(ch)) > 0)
			{
				len = recv(ch, (uint8*)http_request, len);				/*接收http请求*/      
				*(((uint8*)http_request)+len) = 0;
				proc_http(ch, (uint8*)http_request);					/*接收http请求并发送http响应*/
			}
			disconnect(ch);
			break;
			
		case SOCK_CLOSED:                   							/*socket处于关闭状态*/
			socket(ch, Sn_MR_TCP, 80, 0x00);   							/*打开socket*/
			break;
		
		default:
			break;
	}
}

/**
*@brief		接收http请求报文并发送http响应
*@param		s: http服务器socket
*@param		buf：解析报文内容
*@return	无
*/
void proc_http(SOCKET s, uint8 * buf)
{
	int8* name; 											
	int8 req_name[32]={0x00,};											/*定义一个http响应报文的指针*/
	unsigned long file_len=0;											/*定义http请求报文头的结构体指针*/
	uint16 send_len=0;
	uint8* http_response;
	st_http_request *http_request;
	memset(tx_buf,0x00,MAX_URI_SIZE);
	http_response = (uint8*)rx_buf;
	http_request = (st_http_request*)tx_buf;
	parse_http_request(http_request, buf);    							/*解析http请求报文头*/

	switch (http_request->METHOD)		
  {
		case METHOD_ERR :												/*请求报文头错误*/
			memcpy(http_response, ERROR_REQUEST_PAGE, sizeof(ERROR_REQUEST_PAGE));
			send(s, (uint8 *)http_response, strlen((int8 const*)http_response));
			break;
		
		case METHOD_HEAD:												/*HEAD请求方式*/
			
		case METHOD_GET:												/*GET请求方式*/
			name = http_request->URI;
			if(strcmp(name,"/index.htm")==0 || strcmp(name,"/")==0 || (strcmp(name,"/index.html")==0))
			{
				file_len = strlen(INDEX_HTML);
				make_http_response_head((uint8*)http_response, PTYPE_HTML,file_len);
				send(s,http_response,strlen((char const*)http_response));
				send_len=0;
				while(file_len)
				{
					if(file_len>1024)
					{
						if(getSn_SR(s)!=SOCK_ESTABLISHED)
						{
							return;
						}
						send(s, (uint8 *)INDEX_HTML+send_len, 1024);
						send_len+=1024;
						file_len-=1024;
					}
					else
					{
						send(s, (uint8 *)INDEX_HTML+send_len, file_len);
						send_len+=file_len;
						file_len-=file_len;
					} 
				}
			}
			else if(strcmp(name,"/w5500.js")==0)
			{
				memset(tx_buf,0,MAX_URI_SIZE);
				make_basic_config_setting_json_callback(tx_buf,ConfigMsg);
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
				send(s, (u_char *)http_response, strlen((char const*)http_response));
			}
			break;
			
		case METHOD_POST:												/*POST请求*/
			mid(http_request->URI, "/", " ", req_name);					/*获取该请求的文件名*/
			if(strcmp(req_name,"config.cgi")==0)							  	
			{
				cgi_ipconfig(http_request);								/*将配置信息写进单片机eeprom*/
				make_cgi_response(5,(int8*)ConfigMsg.lip,tx_buf);	/*生成响应的文本部分*/        
				sprintf((char *)http_response,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
																		/*发送http响应*/
				send(s, (u_char *)http_response, strlen((char *)http_response));		
				disconnect(s);											/*断开socket连接*/				
				reboot_flag=1;											/*重启标志位置1*/
				return;
			}
			break;
			
		default :
			break;
	}
}

/**
*@brief		将配置信息写进单片机eeprom
*@param		http_request：定义一个http请求的结构体指针
*@return	无
*/
void cgi_ipconfig(st_http_request *http_request)
{ 
  uint8 * param;
  param = get_http_param_value(http_request->URI,"ip");		/*获取修改后的IP地址*/
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.lip);	
  }
  param = get_http_param_value(http_request->URI,"gw");		/*获取修改后的网关*/
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.gw);	
  }
  param = get_http_param_value(http_request->URI,"sub");	/*获取修改后的子网掩码*/
  if(param)
  {
    inet_addr_((uint8*)param, ConfigMsg.sub);		
  }
  write_config_to_eeprom();									/*将获取的网络参数写进eeprom*/
}

/**
*@brief		执行http响应
*@param		无  
*@return	无
*/
void make_cgi_response(uint16 delay, int8* url,int8* cgi_response_buf)
{
  sprintf(cgi_response_buf,"<html><head><title>iWeb - Configuration</title><script language=javascript>j=%d;function func(){document.getElementById('delay').innerText=' '+j + ' ';j--;setTimeout('func()',1000);if(j==0)location.href='http://%d.%d.%d.%d/';}</script></head><body onload='func()'>please wait for a while, the module will boot in<span style='color:red;' id='delay'></span> seconds.</body></html>",delay,url[0],url[1],url[2],url[3]);
  return;
}


