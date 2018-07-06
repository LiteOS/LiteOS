/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#ifndef __AT_API_INTERFACE_H__
#define __AT_API_INTERFACE_H__

#include <stdint.h>
#include <stdio.h>

typedef struct {
	int32_t  (*init)(void);  /*初始化，初始化串口、IP网络等*/
	int8_t (*get_localmac)(int8_t *mac);/*获取本地MAC*/
	int8_t (*get_localip)(int8_t *ip, int8_t * gw, int8_t * mask);/*获取本地IP*/
	/*建立TCP或者UDP连接*/
	int32_t  (*connect)(const int8_t * host, const int8_t *port, int32_t proto);
	/*发送，当命令发送后，如果超过一定的时间没收到应答，要返回错误*/
	int32_t  (*send)(int32_t id , const uint8_t  *buf, uint32_t len);
    int32_t  (*recv_timeout)(int32_t id , int8_t  *buf, uint32_t len, int32_t timeout);
	int32_t  (*recv)(int32_t id , int8_t  *buf, uint32_t len);
   
	int32_t  (*close)(int32_t id);/*关闭连接*/
	int32_t  (*recv_cb)(int32_t id);/*收到各种事件处理，暂不实现 */
	int32_t  (*deinit)(void);
}at_adaptor_api;

int32_t at_api_register(at_adaptor_api *api);
int32_t at_api_send(int32_t id , const uint8_t  *buf, uint32_t len);
int32_t at_api_recv(int32_t id, unsigned char* buf, size_t len);
int32_t at_api_recv_timeout(int32_t id, unsigned char* buf, size_t len, uint32_t timeout);
int32_t at_api_connect(const char* host, const char* port, int proto);
int32_t at_api_close(int32_t fd);
//declear in device drivers
extern at_adaptor_api at_interface;

#endif