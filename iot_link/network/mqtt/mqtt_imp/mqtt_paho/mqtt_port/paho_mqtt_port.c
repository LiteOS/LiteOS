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
#include "MQTTClient.h"

#ifdef WITH_DTLS
#include "dtls_interface.h"
#endif

#include <time.h>
#include <sal.h>
#include <mqtt_al.h>
#include <paho_mqtt_port.h>

#define cn_mqtt_cmd_timeout_ms (10 * 1000)
#define cn_mqtt_events_handle_period_ms (1*1000)
#define cn_mqtt_keepalive_interval_s (100)
#define cn_mqtt_sndbuf_size (1024 * 2)
#define cn_mqtt_rcvbuf_size (1024 * 2)

typedef struct
{
    Network        network;
    MQTTClient       client;
    void          *task;    //create a task for this client
    void          *rcvbuf;  //for the mqtt engine used
    void          *sndbuf;
}paho_mqtt_cb_t;

///< waring: the paho mqtt has the opposite return code with normal socket read and write


#ifdef WITH_DTLS

static int __tls_read(mbedtls_ssl_context *ssl, unsigned char *buffer, int len, int timeout)
{
    int ret= -1;
    int rcvlen = -1;

    if(NULL == ssl || NULL == buffer)
    {
        return -1;
    }

    rcvlen = dtls_read(ssl,buffer,len, timeout);

    if(rcvlen == 0)
    {
        ret = -1;
    }
    else if(rcvlen < 0)
    {
        ret = 0;
    }
    else
    {
        ret = rcvlen;
    }

    return ret;
}
static int __tls_write(mbedtls_ssl_context *ssl, unsigned char *buffer, int len, int timeout)
{
    int ret= -1;
    int sndlen = -1;

    if(NULL == ssl || NULL == buffer)
    {
        return -1;
    }

    sndlen = dtls_write(ssl,buffer,len);

    if(sndlen == 0)
    {
        ret = -1;
    }
    else if(sndlen < 0)
    {
        ret = 0;
    }
    else
    {
        ret = sndlen;
    }

    return ret;
}

#define PORT_BUF_LEN 16
static int __tls_connect(Network *n, char *addr, int port)
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
        goto exit;
    }

    shakehand_info.client_or_server = MBEDTLS_SSL_IS_CLIENT;
    shakehand_info.udp_or_tcp = MBEDTLS_NET_PROTO_TCP;
    shakehand_info.u.c.host = addr;
    snprintf(port_buf, PORT_BUF_LEN, "%d", port);
    shakehand_info.u.c.port = port_buf;

    ret = dtls_shakehand(ssl, &shakehand_info);
    if (ret != 0)
    {
        goto exit;
    }

    n->ctx = ssl;
    return 0;
exit:
    dtls_ssl_destroy(ssl);
    return -1;
}

static void __tls_disconnect(void *ctx)
{
    if(NULL != ctx)
    {
        dtls_ssl_destroy(ctx);
    }

    return;
}


#endif

///< receve function: return code:0 means timeout -1:failed  > receive length
static int __socket_read(void *ctx, unsigned char *buf, int len, int timeout)
{
    int fd;
    int ret = 0;
    int rcvlen = -1;

    struct timeval timedelay;

    if((NULL == ctx)||(NULL== buf))
    {
        return ret;
    }

    fd = (int)ctx;

    timedelay.tv_sec = timeout/1000;
    timedelay.tv_usec = (timeout%1000)*1000;

    ///< set the recv timeout
    if(0 != sal_setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&timedelay,sizeof(timedelay)))
    {
        return ret;  //could not support the rcv timeout
    }

    rcvlen = sal_recv(fd,buf,len,0);
    if(rcvlen == 0)
    {
        ret = -1;
    }
    else if(rcvlen < 0)
    {
        ret = 0;
    }
    else
    {
        ret = rcvlen;
    }
    return ret;
}


///< receve function: return code:0 means timeout -1:failed  > receive length
static int __socket_write(void *ctx, unsigned char *buf, int len, int timeout)
{
    int fd;
    int ret = 0;
    int sndlen = -1;

    struct timeval timedelay;

    if((NULL == ctx)||(NULL== buf))
    {
        return ret;
    }

    fd = (int)ctx;

    timedelay.tv_sec = timeout/1000;
    timedelay.tv_usec = (timeout%1000)*1000;

    ///< set the recv timeout
    if(0 != sal_setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&timedelay,sizeof(timedelay)))
    {
        return ret;  //could not support the rcv timeout
    }

    sndlen = sal_send(fd,buf,len,0);
    if(sndlen == 0)
    {
        ret = -1;
    }
    else if(sndlen < 0)
    {
        ret = 0;
    }
    else
    {
        ret = sndlen;
    }
    return ret;
}
static void __socket_disconnect(void *ctx)
{
    sal_closesocket((int)ctx);
    return;
}
static int __socket_connect(Network *n, char *host, int port)
{
    int ret = -1;
    int fd = -1;
    struct sockaddr_in addr;
    fd = sal_socket(AF_INET,SOCK_STREAM,0);

    if(fd == -1)
    {
        return ret;
    }


    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    if(-1 == sal_connect(fd,(struct sockaddr *)&addr,sizeof(addr)))
    {
        sal_closesocket(fd);
    }
    else
    {
        ret = 0;
        n->ctx = (void *)fd;
    }

    return ret;
}

static int __io_read(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;

    if((NULL == n) || (NULL == buffer ))
    {
        return ret;
    }

    switch(n->arg.type)
    {
        case en_mqtt_al_security_none :
            ret = __socket_read(n->ctx, buffer, len, timeout_ms);
            break;
#ifdef WITH_DTLS
        case en_mqtt_al_security_psk:
        case en_mqtt_al_security_cas:
            ret = __tls_read(n->ctx, buffer, len, timeout_ms);
            break;
        case en_mqtt_al_security_cacs:
            break;    // TODO -- NOT IMPLEMENT YET
#endif
        default :
            break;
    }
    return ret;
}

static int __io_write(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;

    if((NULL == n) || (NULL == buffer ))
    {
        return -1;
    }

    switch(n->arg.type)
    {
    case en_mqtt_al_security_none :
        ret = __socket_write(n->ctx, buffer, len, timeout_ms);
        break;
#ifdef WITH_DTLS
    case en_mqtt_al_security_psk:
    case en_mqtt_al_security_cas:
        ret = __tls_write(n->ctx, buffer, len,timeout_ms);
        break;
    case en_mqtt_al_security_cacs:
        break;    // TODO -- NOT IMPLEMENT YET
#endif
    default :
        break;
    }

    return ret;
}

static int __io_connect(Network *n, char *addr, int port)
{
    int ret = -1;

    if((NULL == n) || (NULL == addr ))
    {
        return -1;
    }

    switch(n->arg.type)
    {
    case en_mqtt_al_security_none :
        ret = __socket_connect(n, addr, port);
        break;
#ifdef WITH_DTLS
    case en_mqtt_al_security_psk:
    case en_mqtt_al_security_cas:
        ret = __tls_connect(n, addr, port);
        break;
    case en_mqtt_al_security_cacs:
        break ;   //not implement yet
#endif
    default :
        break;
    }

    return ret;
}



static void __io_disconnect(Network *n)
{
    switch(n->arg.type)
    {
        case en_mqtt_al_security_none :
            __socket_disconnect(n->ctx);
            break;
    #ifdef WITH_DTLS
        case en_mqtt_al_security_psk:
        case en_mqtt_al_security_cas:
            __tls_disconnect(n->ctx);
            break;
    #endif
        default :
            break;
    }

    n->ctx = NULL;

    return;
}

///////////////////////CREATE THE API FOR THE MQTT_AL///////////////////////////
static  int __loop_entry(void *arg)
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
            osal_task_sleep(1);///< when disconnect, this has been killed
        }
    }
    return 0;
}


static     MQTTClient       *s_static_client_debug = NULL;
static void * __connect(mqtt_al_conpara_t *conparam)
{

    void             *ret  = NULL;
    Network          *n = NULL;
    MQTTClient       *c = NULL;
    paho_mqtt_cb_t   *cb = NULL;

    MQTTPacket_connectData option = MQTTPacket_connectData_initializer;
    MQTTConnackData conack;
    //malloc a handle
    cb = osal_malloc(sizeof(paho_mqtt_cb_t));
    if(NULL == cb)
    {
        goto EXIT_CB_MEM_ERR;
    }

    memset(cb,0,sizeof(paho_mqtt_cb_t));
    cb->task = NULL;
    conparam->conret = cn_mqtt_al_con_code_err_unkown;

    //do the net connect
    n = &cb->network;
    n->mqttread = __io_read;
    n->mqttwrite = __io_write;

    if(NULL == conparam->security)
    {
        n->arg.type = en_mqtt_al_security_none;
    }
    else
    {
        n->arg = *conparam->security;
    }
    if(0 != __io_connect(n,conparam->serveraddr.data,conparam->serverport))
    {
        goto EXIT_NET_CONNECT_ERR;
    }
    //then do the mqtt config
    cb->rcvbuf = osal_malloc(cn_mqtt_rcvbuf_size) ;
    cb->sndbuf = osal_malloc(cn_mqtt_sndbuf_size) ;
    if((NULL == cb->rcvbuf) || (NULL == cb->sndbuf))
    {
        goto EIXT_BUF_MEM_ERR;
    }
    c = &cb->client;
    if(MQTT_SUCCESS != MQTTClientInit(c, n, cn_mqtt_cmd_timeout_ms,\
            cb->sndbuf, cn_mqtt_sndbuf_size, cb->rcvbuf, cn_mqtt_rcvbuf_size))
    {
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

    if((MQTT_SUCCESS != MQTTConnectWithResults(c, &option,&conack)) || \
         (conack.rc != cn_mqtt_al_con_code_ok))
    {
        conparam->conret = conack.rc;
        goto EXIT_MQTT_CONNECT;
    }
    else
    {
        conparam->conret = conack.rc;
    }
    //create the loop task here
    cb->task = osal_task_create("paho",__loop_entry,cb,0x800,NULL,4);
    if(NULL == cb->task)
    {
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
    __io_disconnect(n);
    osal_free(cb->rcvbuf);
    osal_free(cb->sndbuf);

EXIT_NET_CONNECT_ERR:
    osal_free(cb);
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
    __io_disconnect(n);
    //kill the thread
    osal_task_kill(cb->task);
    //deinit the mqtt
    MQTTClientDeInit(c);
    //free the memory
    osal_free(cb->rcvbuf);
    osal_free(cb->sndbuf);
    osal_free(cb);

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

    if(MQTT_SUCCESS == MQTTSubscribeWithResultsArgs(c,para->topic.data,para->qos,\
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

    if(MQTT_SUCCESS == MQTTUnsubscribe(c,para->topic.data))
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
    if(MQTT_SUCCESS ==  MQTTPublish(c, para->topic.data, &msg))
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


int mqtt_install_pahomqtt()
{
    int ret = -1;

    mqtt_al_op_t paho_mqtt_op =
    {
        .connect = __connect,
        .disconnect = __disconnect,
        .subscribe = __subscribe,
        .unsubscribe = __unsubscribe,
        .publish = __publish,
        .check_status = __check_status,
    };

    ret = mqtt_al_install(&paho_mqtt_op);

    return ret;
}



