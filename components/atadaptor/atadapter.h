#ifndef __AT_ADAPTER_H__
#define __AT_ADAPTER_H__

#include "esp8266.h"
#include "los_queue.h"
#include "atiny_adapter.h"

/* MACRO DEFINE */
#ifdef AT_DEBUG
#define AT_LOG(fmt, arg...)  printf("[%s:%d]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define AT_LOG(fmt, arg...)
#endif

#define AT_OK    		 0
#define AT_FAILED 		-1

#define AT_LINK_UNUSE		0
#define AT_LINK_INUSE 		1

#define AT_MUXMODE_SIGNEL   0
#define AT_MUXMODE_MULTI    1

/* VARIABLE DECLEAR */

/* TYPE REDEFINE */
typedef int32_t (*event_cb)(int8_t * buf, int32_t len);

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
	int8_t * perfix;
	int8_t * suffix;
	int8_t * resp;
	uint32_t resp_len;
	
	int32_t (*callback)(int8_t *p, int32_t len);
}at_listner;

typedef struct at_task{
	uint32_t  tsk_hdl;
	uint32_t recv_sem;
	uint32_t resp_sem;
	uint8_t  *recv_buf;  /*底层接收缓存区，默认4k大小*/
	uint8_t  *cmdresp;/*AT命令的返回，默认512字节*/
	uint8_t  *userdata;  /*来自对端的数据，默认512字节*/
	uint32_t  mux_mode;/*0:单连接，1：多连接*/
	at_listner * head;

	int32_t (*cmd)(int8_t * buf, int32_t len, char * rep1, char * rep2);
	int32_t (*write)(int8_t * buf, int32_t len);
	void    (*init)(void);
	int32_t (*get_id)();
} at_task;

/* FUNCTION */
void at_init();
int32_t at_read(int32_t id, int8_t * buf, uint32_t len, int32_t timeout);
int32_t at_write(int8_t * buf, int32_t len);
int32_t at_get_unuse_linkid();
void at_listener_list_add(at_listner * p);
void at_listner_list_del(at_listner * p);

#endif