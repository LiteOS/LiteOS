#ifndef __NB_NEUL_BC95_H__
#define __NB_NEUL_BC95_H__

#include "at_api_interface.h"

#define CLOUD_IP  "218.4.33.71,5683"
#define AT_NB_LINE_END 			"\r\n"

#define AT_NB_reboot    		"AT+NRB\r"
#define AT_NB_hw_detect    		"AT+CFUN?\r"
#define AT_NB_get_auto_connect    		"AT+NCONFIG?\r"
#define AT_CMD_PREFIX      "+NNMI:"

#define AT_MODU_NAME    	"nb_neul95"
#define AT_USART   			USART3
#define AT_BUARDRATE   		9600
#define AT_USART_IRQn   	USART3_IRQn
#define AT_CMD_TIMEOUT		10000    //ms
#define AT_MAX_LINK_NUM     4

#define AT_LINE_END 		"\r\n"
#define AT_CMD_BEGIN		"\r\n"
#define MAX_AT_USERDATA_LEN 2048

typedef struct _remote_info_t
{
    unsigned short port;
    int socket;
    char ip[16];
}remote_info;//后续创建socket时需要该struct保存
#endif