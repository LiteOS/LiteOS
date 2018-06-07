#ifndef __USART2_H
#define	__USART2_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "cmsis_os.h"

#define AT_MODU_NAME    	"SIM900A"
#define AT_USART   			USART2
#define AT_BUARDRATE   		115200
#define AT_USART_IRQn   	USART2_IRQn
#define AT_CMD_TIMEOUT		10000    //ms
#define AT_MAX_LINK_NUM     4

#define AT_LINE_END 		"\r"
#define AT_CMD_BEGIN		"\r\n"
#define MAX_AT_USERDATA_LEN 2048

#define AT_CMD_AT    		"AT"
#define AT_CMD_CPIN         "AT+CPIN?"//check sim card
#define AT_CMD_COPS         "AT+COPS?"//check register network
#define AT_CMD_CLOSE    	"AT+CIPCLOSE"
#define AT_CMD_SHUT    		"AT+CIPSHUT"
#define AT_CMD_ECHO_OFF 	"ATE0"
#define AT_CMD_ECHO_ON  	"ATE1"
#define AT_CMD_MUX 			"AT+CIPMUX"
#define AT_CMD_CLASS        "AT+CGCLASS"//set MS type
#define AT_CMD_PDP_CONT   	"AT+CGDCONT"//configure pdp context
#define AT_CMD_PDP_ATT    	"AT+CGATT"//pdp attach network
#define AT_CMD_PDP_ACT		"AT+CGACT"//active pdp context
#define AT_CMD_CSTT			"AT+CSTT"//start task
#define AT_CMD_CIICR		"AT+CIICR"//start gprs connect
#define AT_CMD_CIFSR		"AT+CIFSR"//get local ip
#define AT_CMD_CIPHEAD		"AT+CIPHEAD"
#define AT_CMD_CONN			"AT+CIPSTART"
#define AT_CMD_SEND			"AT+CIPSEND"
#define AT_CMD_CLOSE		"AT+CIPCLOSE"

#define AT_DATAF_PREFIX      "\r\n+IPD"
#define AT_DATAF_PREFIX_MULTI      "\r\n+RECEIVE"
#define  SIM900A_DELAY       osDelay

#endif /* __USART2_H */

