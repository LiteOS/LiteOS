/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: At Device BC95 HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#ifndef _BC95_H
#define _BC95_H

#include "at_frame/at_main.h"

#define AT_MODU_NAME            "nb_neul95"

#define AT_NB_LINE_END          "\r\n"
#define AT_NB_reboot            "AT+NRB\r"
#define AT_NB_hw_detect         "AT+CFUN?\r"
#define AT_NB_get_auto_connect  "AT+NCONFIG?\r"
#define AT_CMD_PREFIX           "\r\n+NNMI:"
#define AT_DATAF_PREFIX         "+NSONMI:"
#define CGATT                   "AT+CGATT?\r"
#define CGATT_ATTACH            "AT+CGATT=1\r"
#define CGATT_DEATTACH          "AT+CGATT=0\r"

#define AT_LINE_END             "\r\n"
#define AT_CMD_BEGIN            "\r\n"

#define AT_USART_PORT           3
#define AT_BUARDRATE            9600
#define AT_CMD_TIMEOUT          10000 // ms
#define AT_MAX_LINK_NUM         4
#define AT_MAX_PAYLOADLEN       512

#define NB_STAT_LOCALPORT       56

#define MAX_SOCK_NUM            5
#define UDP_PROTO               17

#if defined STM32F103xE
#define MAX_AT_USERDATA_LEN     (1024 * 2)
#else
#define MAX_AT_USERDATA_LEN     (1024)
#endif

#define IP_LEN                  16
typedef struct _socket_info_t {
    int socket;
    short localport;
    char localip[IP_LEN];
    short remoteport;
    char remoteip[IP_LEN];
    bool used_flag;
} socket_info; // struct to save socket info

int str_to_hex(const char *bufin, int len, char *bufout);
int32_t nb_set_cdpserver(char *host, char *port);
int32_t nb_hw_detect(void);
int32_t nb_get_netstat(void);
int nb_query_ip(void);
int32_t nb_send_payload(const char *buf, int len);
int32_t nb_check_csq(void);
int32_t nb_send_psk(char *pskid, char *psk);
int32_t nb_set_no_encrypt(void);
int32_t nb_reboot(void);
int32_t nb_recv_timeout(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout);
int32_t nb_cmd_match(const char *buf, char *featurestr, int len);
void nb_step(void);
void nb_reattach(void);

#endif /* _BC95_H */
