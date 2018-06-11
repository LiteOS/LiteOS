#ifndef __NB_NEUL_BC95_H__
#define __NB_NEUL_BC95_H__

#include "at_api_interface.h"

#define CLOUD_IP  "218.4.33.71,5683"
#define AT_NB_LINE_END 			"\r\n"

#define AT_NB_reboot    		"AT+NRB\r"
#define AT_NB_hw_detect    		"AT+CFUN?\r"
#define AT_NB_get_auto_connect    		"AT+NCONFIG?\r"
#define AT_CMD_PREFIX      "+NNMI:"

typedef struct _remote_info_t
{
    unsigned short port;
    int socket;
    char ip[16];
}remote_info;//后续创建socket时需要该struct保存
#endif