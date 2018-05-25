#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "at_api_interface.h"

#define WIFI_SSID      		"TP-LINK_80D8BE"
#define WIFI_PASSWD    		"87654321"

#define AT_MODU_NAME    	"ESP8266"
#define AT_USART   			USART3
#define AT_BUARDRATE   		115200
#define AT_USART_IRQn   	USART3_IRQn

#define AT_MAX_LINK_NUM     4

#define AT_CMD_RST    		"AT+RST\r\n"
#define AT_CMD_ECHO_OFF 	"ATE0\r\n"
#define AT_CMD_CWMODE  		"AT+CWMODE_CUR"
#define AT_CMD_JOINAP  		"AT+CWJAP_CUR"
#define AT_CMD_MUX 			"AT+CIPMUX"
#define AT_CMD_CONN			"AT+CIPSTART"
#define AT_CMD_SEND			"AT+CIPSEND"
#define AT_CMD_CLOSE		"AT+CIPCLOSE"

#define MAX_AT_RECV_LEN     1024
#define MAX_AT_USERDATA_LEN 512
#define MAX_AT_RESP_LEN     512
#define AT_DATAF_PREFIX      "\r\n+IPD"

typedef enum {
	STA = 1,
	AP, 
	ATA_AP,
}enum_net_mode;

#endif