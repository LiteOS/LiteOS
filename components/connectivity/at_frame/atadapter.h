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

#ifndef __AT_ADAPTER_H__
#define __AT_ADAPTER_H__

#include "los_queue.h"
#include "los_mux.h"
#include "los_task.h"
#include "los_sem.h"

#include "atiny_socket.h"
#include "at_api_interface.h"

/* MACRO DEFINE */
//#define AT_INTO
#ifdef AT_INTO
#define AT_LOG(fmt, arg...)  printf("[%s:%d][I]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define AT_LOG(fmt, arg...)
#endif

//#define AT_DEBUG
#ifdef AT_DEBUG
#define AT_LOG_DEBUG(fmt, arg...)  printf("[%s:%d][D]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define AT_LOG_DEBUG(fmt, arg...)
#endif
//#define USE_USARTRX_DMA


#define AT_OK    		 0
#define AT_FAILED 		-1

#define AT_LINK_UNUSE		0
#define AT_LINK_INUSE 		1

#define AT_MUXMODE_SINGLE   0
#define AT_MUXMODE_MULTI    1

/* VARIABLE DECLEAR */

/* TYPE REDEFINE */
typedef int32_t (*oob_callback)(void* arg, int8_t* buf, int32_t buflen);

typedef struct {
    uint32_t len;
    uint8_t *addr;
}QUEUE_BUFF;


typedef struct {
	UINT32 fd;		//convert between socket_fd and linkid
	UINT32 qid;    // queue id
	UINT32 usable;
}at_link;

typedef struct _listner{
	struct _listner * next;
	int8_t * suffix;
	int8_t * resp;
	uint32_t resp_len;
}at_listener;

#define OOB_MAX_NUM 5
#define OOB_CMD_LEN  40
#define AT_DATA_LEN 1024
typedef struct oob_s{
	char featurestr[OOB_CMD_LEN];
	int len;
	oob_callback callback;
	void* arg;
}oob_t;

typedef struct at_oob_s{
	oob_t oob[OOB_MAX_NUM];
	int32_t oob_num;
} at_oob_t;

typedef struct __config{
	char * name;
	uint32_t usart_port;
	uint32_t buardrate;
	uint32_t linkid_num;
	uint32_t user_buf_len; /* malloc 3 block memory for intener use, len * 3 */
#ifdef USE_USARTRX_DMA
	uint32_t recv_buf_len;
#endif
	char * cmd_begin;
	char * line_end;
	uint32_t  mux_mode;
	uint32_t timeout;  //command respond timeout
}at_config;

typedef struct at_task{

	uint32_t  tsk_hdl;
	uint32_t recv_sem;
	uint32_t resp_sem;
	uint32_t cmd_mux;
	uint8_t  *recv_buf;  
	uint8_t  *cmdresp;/*AT cmd response,default 512 bytes*/
	uint8_t  *userdata;  /*data form servers,default 512 bytes*/
	uint32_t  mux_mode;
	at_link  *linkid;
	at_listener * head;
	uint32_t timeout; //command respond timeout

	void    (*init)();
	int32_t (*cmd)(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
	int32_t (*write)(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
	/* get unused linkid, use in multi connection mode*/
	int32_t (*get_id)();
	/* register uset msg process to the listener list */
	int32_t (*oob_register)(char* featurestr,int strlen, oob_callback callback);
	void (*deinit)();
} at_task;

void* at_malloc(size_t size);
void at_free(void* ptr);
extern int at_update_result_send(void);
extern uint16_t at_fota_timer;
#endif
