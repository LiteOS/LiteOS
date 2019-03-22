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
#include "los_task.h"
#include "MQTTClient.h"

#ifdef WITH_DTLS

#include "dtls_interface.h"

#endif

#include "osdepends/atiny_osdep.h"
#include "log/atiny_log.h"
#include "sal/atiny_socket.h"
#include "mqtt_al.h"


#ifndef cn_mqtt_cmd_timeout_ms
#define cn_mqtt_cmd_timeout_ms (10 * 1000)
#endif

#ifndef cn_mqtt_events_handle_period_ms
#define cn_mqtt_events_handle_period_ms (1*1000)
#endif

#ifndef cn_mqtt_keepalive_interval_s
#define cn_mqtt_keepalive_interval_s (100)
#endif

#ifndef cn_mqtt_sndbuf_size
#define cn_mqtt_sndbuf_size (1024 * 2)
#endif

#ifndef cn_mqtt_rcvbuf_size
#define cn_mqtt_rcvbuf_size (1024 * 2)
#endif

typedef struct
{
	Network 	   network;
	MQTTClient	   client;
	int            task;    //create a task for this client
	void          *rcvbuf;  //for the mqtt engine used
	void          *sndbuf;
}paho_mqtt_cb_t;

static int los_mqtt_raw_read(void *ctx, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = atiny_net_recv_timeout(ctx, buffer, len, timeout_ms);
    /* 0 is timeout for mqtt for normal select */
    if (ret == ATINY_NET_TIMEOUT)
    {
        ret = 0;
    }
    return ret;
}

#ifdef WITH_DTLS
static int los_mqtt_tls_read(mbedtls_ssl_context *ssl, unsigned char *buffer, int len, int timeout_ms)
{
    int ret;

    if(NULL == ssl || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    mbedtls_ssl_conf_read_timeout(ssl->conf, timeout_ms);

    ret = mbedtls_ssl_read(ssl, buffer, len);
    if(ret == MBEDTLS_ERR_SSL_WANT_READ
            || ret == MBEDTLS_ERR_SSL_WANT_WRITE
            || ret == MBEDTLS_ERR_SSL_TIMEOUT)
        ret = 0;

    return ret;
}
#endif

static int los_read(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;

    if((NULL == n) || (NULL == buffer ))
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    switch(n->arg.type)
    {
        case en_mqtt_al_security_none :
            ret = los_mqtt_raw_read(n->ctx, buffer, len, timeout_ms);
            break;
#ifdef WITH_DTLS
        case en_mqtt_al_security_psk:
        case en_mqtt_al_security_cas:
            ret = los_mqtt_tls_read(n->ctx, buffer, len, timeout_ms);
            break;
        case en_mqtt_al_security_cacs:
        	break;    // TODO -- NOT IMPLEMENT YET
#endif
        default :
            ATINY_LOG(LOG_WARNING, "unknow proto : %d", n->arg.type);
            break;
    }

    return ret;
}


static int los_write(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;

    if((NULL == n) || (NULL == buffer ))
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    switch(n->arg.type)
    {
    case en_mqtt_al_security_none :
        ret = atiny_net_send_timeout(n->ctx, buffer, len, timeout_ms);
        break;
#ifdef WITH_DTLS
    case en_mqtt_al_security_psk:
    case en_mqtt_al_security_cas:
        ret = dtls_write(n->ctx, buffer, len);
        break;
    case en_mqtt_al_security_cacs:
    	break;    // TODO -- NOT IMPLEMENT YET
#endif
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d",n->arg.type);
        break;
    }

    return ret;
}

static int los_net_raw_connect(Network *n, char *addr, int port)
{
    char port_str[16];

    (void)snprintf(port_str, sizeof(port_str), "%u", port);
    port_str[sizeof(port_str) - 1] = '\0';
    n->ctx = atiny_net_connect(addr, port_str, ATINY_PROTO_TCP);
    if (n->ctx == NULL)
    {
        ATINY_LOG(LOG_FATAL, "atiny_net_connect fail");
        return ATINY_NET_ERR;
    }

    return ATINY_NET_OK;
}


#ifdef WITH_DTLS

#define PORT_BUF_LEN 16
static int los_net_tls_connect(Network *n, char *addr, int port)
{
    int ret;
    mbedtls_ssl_context *ssl;
    dtls_shakehand_info_s shakehand_info;
    dtls_establish_info_s establish_info;
    char port_buf[PORT_BUF_LEN];

    memset(&shakehand_info, 0, sizeof(dtls_shakehand_info_s));

    if (n->arg.type == en_mqtt_al_security_psk)
    {
        establish_info.psk_or_cert = VERIFY_WITH_PSK;
        establish_info.v.p.psk =(const unsigned char *) n->arg.u.psk.psk_key.data;
        establish_info.v.p.psk_len = n->arg.u.psk.psk_key.len;
        establish_info.v.p.psk_identity = (unsigned char *)n->arg.u.psk.psk_id.data;

        shakehand_info.psk_or_cert = VERIFY_WITH_PSK;


    }
    else if(n->arg.type == en_mqtt_al_security_cas)
    {
        establish_info.psk_or_cert = VERIFY_WITH_CERT;
        establish_info.v.c.ca_cert = (const unsigned char *)n->arg.u.cas.ca_crt.data;
        establish_info.v.c.cert_len = n->arg.u.cas.ca_crt.len;
        shakehand_info.psk_or_cert = VERIFY_WITH_CERT;
    }
    establish_info.udp_or_tcp = MBEDTLS_NET_PROTO_TCP;

    ssl = (void *)dtls_ssl_new(&establish_info, MBEDTLS_SSL_IS_CLIENT);
    if (NULL == ssl)
    {
        ATINY_LOG(LOG_ERR, "create ssl context failed");
        goto exit;
    }

    shakehand_info.client_or_server = MBEDTLS_SSL_IS_CLIENT;
    shakehand_info.udp_or_tcp = MBEDTLS_NET_PROTO_TCP;
    shakehand_info.u.c.host = addr;
    atiny_snprintf(port_buf, PORT_BUF_LEN, "%d", port);
    shakehand_info.u.c.port = port_buf;

    ret = dtls_shakehand(ssl, &shakehand_info);
    if (ret != 0)
    {
        ATINY_LOG(LOG_ERR, "ssl shake hand failed");
        goto exit;
    }

    n->ctx = ssl;
    return 0;
exit:
    dtls_ssl_destroy(ssl);
    return -1;
}
#endif

static int los_net_connect(Network *n, char *addr, int port)
{
    int ret = -1;

    if((NULL == n) || (NULL == addr ))
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return -1;
    }

    switch(n->arg.type)
    {
    case en_mqtt_al_security_none :
        ret = los_net_raw_connect(n, addr, port);
        break;
#ifdef WITH_DTLS
    case en_mqtt_al_security_psk:
    case en_mqtt_al_security_cas:
        ret = los_net_tls_connect(n, addr, port);
        break;
    case en_mqtt_al_security_cacs:
    	break ;   //not implement yet
#endif
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d\n", n->arg.type);
        break;
    }

    return ret;
}

static void los_net_raw_disconnect(void *ctx)
{
    if(NULL == ctx)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return;
    }
    atiny_net_close(ctx);

    return;
}

#ifdef WITH_DTLS
static void los_net_tls_disconnect(void *ctx)
{
	if(NULL != ctx)
	{
	    dtls_ssl_destroy(ctx);
	}

	return;
}
#endif

static void los_net_disconnect(Network *n)
{
    switch(n->arg.type)
    {
		case en_mqtt_al_security_none :
			los_net_raw_disconnect(n->ctx);
			break;
	#ifdef WITH_DTLS
		case en_mqtt_al_security_psk:
		case en_mqtt_al_security_cas:
			los_net_tls_disconnect(n->ctx);
			break;
	#endif
		default :
			ATINY_LOG(LOG_FATAL, "unknow proto : %d\n",n->arg.type);
			break;
    }

    n->ctx = NULL;

    return;
}

///////////////////////CREATE THE API FOR THE MQTT_AL///////////////////////////

/** brief: create a new task and return the task number*/
static int task_create(const char *name,void *arg, int  (*fn_task_entry)(void *arg),\
						int stack_size,int prior)
{
	int ret = -1;

	TSK_INIT_PARAM_S task_init_param;

	memset (&task_init_param,0,sizeof(TSK_INIT_PARAM_S));

	task_init_param.pcName = (char *)name;
	task_init_param.uwArg = (unsigned int)arg;
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC) fn_task_entry;
	task_init_param.usTaskPrio = prior;
	task_init_param.uwStackSize = stack_size;

	if(LOS_OK  != LOS_TaskCreate((unsigned int *)&ret,&task_init_param))
	{
		ret = -1;
	}

	return ret;
}

static int task_delete(int taskno)
{
	int ret = -1;

	if(taskno != -1)
	{
		if(LOS_OK == LOS_TaskDelete(taskno))
		{
			ret = 0;
		}
	}

	return ret;
}

static  int mqtt_loop_entry(void *arg)
{
	paho_mqtt_cb_t *cb;
	cb = arg;
	while(1)
	{
		if((NULL != cb) && MQTTIsConnected(&cb->client))
		{
			MQTTYield(&cb->client,1000);
		}
		else
		{
			atiny_delay(1);
		}
	}
	return 0;
}

static int __init()
{

#ifdef WITH_DTLS
    dtls_init();
#endif

    ATINY_LOG(LOG_DEBUG, "MQTT PAHO INIT\r\n");
	return 0;
}

static int __deinit()
{
    ATINY_LOG(LOG_DEBUG, "MQTT PAHO DEINIT\r\n");
	return 0;
}

static 	MQTTClient       *s_static_client_debug = NULL;


static void * __connect(mqtt_al_conpara_t *conparam)
{

	void             *ret  = NULL;
	Network          *n = NULL;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;

    MQTTPacket_connectData option = MQTTPacket_connectData_initializer;
    MQTTConnackData conack;
	//malloc a handle
	cb = atiny_malloc(sizeof(paho_mqtt_cb_t));
	if(NULL == cb)
	{
	    ATINY_LOG(LOG_FATAL, " PAHO MEM ERR\r\n");
	    goto EXIT_CB_MEM_ERR;
	}

	memset(cb,0,sizeof(paho_mqtt_cb_t));
	cb->task = -1;
	conparam->conret = cn_mqtt_al_con_code_err_unkown;

	//do the net connect
	n = &cb->network;
	n->mqttread = los_read;
	n->mqttwrite = los_write;

	if(NULL == conparam->security)
	{
		n->arg.type = en_mqtt_al_security_none;
	}
	else
	{
		n->arg = *conparam->security;
	}
	if(0 != los_net_connect(n,conparam->serveraddr.data,conparam->serverport))
	{
	    ATINY_LOG(LOG_FATAL, " PAHO NET CONNECT ERR\r\n");
	    goto EXIT_NET_CONNECT_ERR;
	}
	//then do the mqtt config
	cb->rcvbuf = atiny_malloc(cn_mqtt_rcvbuf_size) ;
	cb->sndbuf = atiny_malloc(cn_mqtt_sndbuf_size) ;
	if((NULL == cb->rcvbuf) || (NULL == cb->sndbuf))
	{
	    ATINY_LOG(LOG_FATAL, " PAHO BUF MALLOC ERR\r\n");
	    goto EIXT_BUF_MEM_ERR;
	}
	c = &cb->client;
    if(PAHO_SUCCESS != MQTTClientInit(c, n, cn_mqtt_cmd_timeout_ms,\
    		cb->sndbuf, cn_mqtt_sndbuf_size, cb->rcvbuf, cn_mqtt_rcvbuf_size))
    {
	    ATINY_LOG(LOG_FATAL, " PAHO INIT ERR\r\n");
	    goto EXIT_MQTT_INIT;
    }
    //then do make the mqtt connect param
    if(conparam->version == en_mqtt_al_version_3_1_0)
    {
    	option.MQTTVersion = 3 ;
    }
    else
    {
    	option.MQTTVersion = 4 ;
    }

    option.clientID.lenstring.len = conparam->clientid.len;
    option.clientID.lenstring.data = conparam->clientid.data;

    option.keepAliveInterval = conparam->keepalivetime;

    option.cleansession = conparam->cleansession;

    if(NULL != conparam->willmsg)
    {

    	option.willFlag = 1;
    	option.will.qos = conparam->willmsg->qos;
    	option.will.retained = conparam->willmsg->retain;

    	option.will.topicName.lenstring.len = conparam->willmsg->topic.len;
    	option.will.topicName.lenstring.data = conparam->willmsg->topic.data;

    	option.will.message.lenstring.len =conparam->willmsg->msg.len;
    	option.will.message.lenstring.data =conparam->willmsg->msg.data;

    }
    else
    {
    	option.willFlag = 0;
    }

    option.username.lenstring.len = conparam->user.len;
    option.username.lenstring.data = conparam->user.data;

    option.password.lenstring.len = conparam->passwd.len;
    option.password.lenstring.data = conparam->passwd.data;

    if((PAHO_SUCCESS != MQTTConnectWithResults(c, &option,&conack)) || \
    	 (conack.rc != cn_mqtt_al_con_code_ok))
    {
        conparam->conret = conack.rc;
	    ATINY_LOG(LOG_FATAL, " PAHO CONNECT ERR\r\n");
	    goto EXIT_MQTT_CONNECT;
    }
    else
    {
        conparam->conret = conack.rc;
    }
    //create the loop task here
    cb->task = task_create("paho_mqtt",cb,mqtt_loop_entry,0x800,4);
    if(cb->task == -1)
    {
	    ATINY_LOG(LOG_FATAL, " PAHO MAIN TASK ERR\r\n");
	    goto EXIT_MQTT_MAINTASK;
    }
    s_static_client_debug = c;

    ret = cb;
    return ret;

EXIT_MQTT_MAINTASK:
	MQTTDisconnect(c);
EXIT_MQTT_CONNECT:
EXIT_MQTT_INIT:
EIXT_BUF_MEM_ERR:
	los_net_disconnect(n);
	atiny_free(cb->rcvbuf);
	atiny_free(cb->sndbuf);

EXIT_NET_CONNECT_ERR:
	atiny_free(cb);
EXIT_CB_MEM_ERR:
	return NULL;
}

static int __disconnect(void *handle)
{
	int ret = -1;

	Network          *n = NULL;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;


	if (NULL == handle)
	{
		return ret;
	}

	cb = handle;

	c = &cb->client;
	n = &cb->network;
	//mqtt disconnect
	MQTTDisconnect(c);
	//net disconnect
	los_net_disconnect(n);
	//kill the thread
	task_delete(cb->task);
	//deinit the mqtt
	MQTTClientDeInit(c);
	//free the memory
	atiny_free(cb->rcvbuf);
	atiny_free(cb->sndbuf);
	atiny_free(cb);

	return 0;
}

/**
 * Copyright (c) 2009-2018 Roger Light <roger@atchoo.org>
 * licensed under the Eclipse Public License 1.0 and the Eclipse Distribution License 1.0
 */
///< we changge the lib to support the args
static void general_dealer(MessageData *data)
{
	mqtt_al_msgrcv_t   msg;
	fn_msg_dealer      dealer;
	msg.dup = data->message->dup;
	msg.qos = data->message->qos;
	msg.retain = data->message->retained;
	msg.msg.len = data->message->payloadlen;
	msg.msg.data = data->message->payload;

	if(data->topicName->lenstring.len)
	{
		msg.topic.data = data->topicName->lenstring.data;
		msg.topic.len = data->topicName->lenstring.len;
	}
	else
	{
		msg.topic.data = data->topicName->cstring;
		msg.topic.len = strlen(data->topicName->cstring);
	}

	if(NULL != data->arg)
	{
		dealer = data->arg;
		dealer(NULL,&msg);  ///<   the args not implement yet
	}
}


//////////////////////END --PATCH FOR PAHO MQTT/////////////////////////////////
static int __subscribe(void *handle,mqtt_al_subpara_t *para)
{
	int ret = -1;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;

	MQTTSubackData   ack;

	if((NULL == handle) ||(NULL == para))
	{
		return ret;
	}

	cb = handle;
	c = &cb->client;

	if(PAHO_SUCCESS == MQTTSubscribeWithResultsArgs(c,para->topic.data,para->qos,\
			general_dealer,&ack,para->dealer))
	{
		para->subret = ack.grantedQoS;
		ret = 0;
	}

	return ret;
}

static int __unsubscribe(void *handle,mqtt_al_unsubpara_t *para)
{
	int ret = -1;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;

	if((NULL == handle) ||(NULL == para))
	{
		return ret;
	}

	cb = handle;
	c = &cb->client;

	if(PAHO_SUCCESS == MQTTUnsubscribe(c,para->topic.data))
	{
		ret = 0;
	}

	return ret;

}

static int __publish(void *handle, mqtt_al_pubpara_t *para)
{
	MQTTMessage  msg;
	int ret = -1;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;

	if((NULL == handle) ||(NULL == para))
	{
		return ret;
	}

	cb = handle;
	c = &cb->client;

	memset(&msg,0,sizeof(msg));
	msg.retained = para->retain;
	msg.qos = QOS0 + para->qos;
	msg.payload = para->msg.data;
	msg.payloadlen = para->msg.len;
    if(PAHO_SUCCESS ==  MQTTPublish(c, para->topic.data, &msg))
    {
    	ret = 0;
    }

	return ret;
}

static en_mqtt_al_connect_state __check_status(void *handle)
{
	int ret = en_mqtt_al_connect_err;
	MQTTClient       *c = NULL;
	paho_mqtt_cb_t   *cb = NULL;

	if(NULL == handle)
	{
		return ret;
	}

	cb = handle;
	c = &cb->client;

	if(MQTTIsConnected(c))
	{
		ret = en_mqtt_al_connect_ok;
	}

	return ret;
}


int mqtt_lib_load()
{
	int ret = -1;
	mqtt_al_op_t paho_mqtt_op =
	{
		.init = __init ,
		.deinit = __deinit,
		.connect = __connect,
		.disconnect = __disconnect,
		.subscribe = __subscribe,
		.unsubscribe = __unsubscribe,
		.publish = __publish,
		.check_status = __check_status,
	};

	if(0 != mqtt_al_install(&paho_mqtt_op))
	{
        ATINY_LOG(LOG_FATAL, "register paho mqtt to mqtt_al err!\r\n");
        ret = 0;
	}

	return ret;
}



