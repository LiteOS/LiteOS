#ifndef __AT_ADAPTER_H__
#define __AT_ADAPTER_H__

#include "los_queue.h"
#include "los_mux.h"
#include "los_task.h"
#include "los_sem.h"
#include "atiny_adapter.h"

/* MACRO DEFINE */
#define AT_INTO
#ifdef AT_INTO
#define AT_LOG(fmt, arg...)  printf("[%s:%d][I]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define AT_LOG(fmt, arg...)
#endif

#define AT_DEBUG
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
	int32_t fd;		//convert between socket_fd and linkid
	uint32_t qid;    // queue id
	uint32_t usable;
}at_link;

typedef struct _listner{
	struct _listner * next;
	int8_t * suffix;
	int8_t * resp;
	uint32_t resp_len;
}at_listener;

#define OOB_MAX_NUM 5
#define OOB_CMD_LEN  40
typedef struct oob_s{
	char featurestr[OOB_CMD_LEN];
	int len;
	oob_callback cb;
	void* arg;
}oob_t;

typedef struct at_oob_s{
	oob_t oob[OOB_MAX_NUM];
	int32_t oob_num;
} at_oob_t;

typedef struct __config{
	char * name;
	USART_TypeDef * usart;
	uint32_t buardrate;
	uint32_t irqn;
	uint32_t linkid_num;
	uint32_t user_buf_len; /* malloc 3 block memory for intener use, len * 3 */
	char * cmd_begin;
	char * line_end;
	uint32_t  mux_mode;/*0:单连接，1：多连接*/
	uint32_t timeout;  //命令响应超时时间
}at_config;

typedef struct at_task{

	uint32_t  tsk_hdl;
	uint32_t recv_sem;
	uint32_t resp_sem;
	uint32_t cmd_mux;
	uint8_t  *recv_buf;  /*底层接收缓存区，默认4k大小*/
	uint8_t  *cmdresp;/*AT命令的返回，默认512字节*/
	uint8_t  *userdata;  /*来自对端的数据，默认512字节*/
	uint32_t  mux_mode;/* 使用的连接模?0:单连接，1：多连接*/
	at_link  *linkid;
	at_listener * head;
	uint32_t timeout;  //命令响应超时时间

	void    (*init)();
	int32_t (*cmd)(int8_t * cmd, int32_t len, const char * suffix, char * rep_buf);
	int32_t (*write)(int8_t * cmd, int8_t * suffix, int8_t * buf, int32_t len);
	/* 获取未使用的linkid, 多链接模式下使用 */
	int32_t (*get_id)();
	/* 注册用户消息处理函数到监听列表中 */
	int32_t (*oob_register)(char* featurestr,int strlen, oob_callback callback);
} at_task;

#endif