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

#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "mqtt_client.h"
#include "atiny_log.h"
#include "MQTTClient.h"

#define MQTT_CONN_FAILED_MAX_TIMES (6)
#define MQTT_CONN_FAILED_BASE_DELAY (1000)

#define MQTT_VERSION_3_1 (3)
#define MQTT_VERSION_3_1_1 (4)
#define MQTT_CLEAN_SESSION_TRUE (1)
#define MQTT_CLEAN_SESSION_FALSE (0)
#define MQTT_TOPIC_SUBSCRIBED_TRUE (1)
#define MQTT_TOPIC_SUBSCRIBED_FALSE (0)

void mqtt_message_arrived(MessageData* md);
void device_info_member_free(atiny_device_info_t* info);

unsigned char mqtt_sendbuf[MQTT_SENDBUF_SIZE];
unsigned char mqtt_readbuf[MQTT_READBUF_SIZE];

typedef struct
{
    atiny_device_info_t device_info;
    MQTTClient client;
    atiny_param_t atiny_params;
    char atiny_quit;
} handle_data_t;

static handle_data_t g_atiny_handle;

void atiny_param_member_free(atiny_param_t* param)
{
    if(NULL == param)
    {
        return;
    }

    if(NULL != param->server_ip)
    {
        atiny_free(param->server_ip);
        param->server_ip = NULL;
    }

    if(NULL != param->server_port)
    {
        atiny_free(param->server_port);
        param->server_port = NULL;
    }

    switch(param->security_type)
    {
        case CLOUD_SECURITY_TYPE_PSK:
            if(NULL != param->u.psk.psk_id)
            {
                atiny_free(param->u.psk.psk_id);
                param->u.psk.psk_id = NULL;
            }
            if(NULL != param->u.psk.psk)
            {
                atiny_free(param->u.psk.psk);
                param->u.psk.psk = NULL;
            }
            break;
        case CLOUD_SECURITY_TYPE_CA:
            if(NULL != param->u.ca.ca_crt)
            {
                atiny_free(param->u.ca.ca_crt);
                param->u.ca.ca_crt = NULL;
            }
            if(NULL != param->u.ca.server_crt)
            {
                atiny_free(param->u.ca.server_crt);
                param->u.ca.server_crt = NULL;
            }
            if(NULL != param->u.ca.server_key)
            {
                atiny_free(param->u.ca.server_key);
                param->u.ca.server_key = NULL;
            }
            break;
        default:
            break;
    }

    return;
}

int atiny_param_dup(atiny_param_t* dest, atiny_param_t* src)
{
    if(NULL == dest || NULL == src)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return -1;
    }
    dest->server_ip = atiny_strdup((const char *)(src->server_ip));
    if(NULL == dest->server_ip)
        goto atiny_param_dup_failed;

    dest->server_port = atiny_strdup((const char *)(src->server_port));
    if(NULL == dest->server_port)
        goto atiny_param_dup_failed;

    switch(src->security_type)
    {
        case CLOUD_SECURITY_TYPE_PSK:
            dest->u.psk.psk_id = (unsigned char *)atiny_strdup((const char *)(src->u.psk.psk_id));
            if(NULL == dest->u.psk.psk_id)
                goto atiny_param_dup_failed;
            dest->u.psk.psk = (unsigned char *)atiny_malloc(src->u.psk.psk_len);
            if(NULL == dest->u.psk.psk)
                goto atiny_param_dup_failed;
            memcpy(dest->u.psk.psk, src->u.psk.psk, src->u.psk.psk_len);
            break;
        case CLOUD_SECURITY_TYPE_CA:
            dest->u.ca.ca_crt = atiny_strdup((const char *)(src->u.ca.ca_crt));
            if(NULL == dest->u.ca.ca_crt)
                goto atiny_param_dup_failed;
            dest->u.ca.server_crt = atiny_strdup((const char *)(src->u.ca.server_crt));
            if(NULL == dest->u.ca.server_crt)
                goto atiny_param_dup_failed;
            dest->u.ca.server_key = atiny_strdup((const char *)(src->u.ca.server_key));
            if(NULL == dest->u.ca.server_key)
                goto atiny_param_dup_failed;
            break;
        default:
            break;
    }

    return 0;
atiny_param_dup_failed:
    atiny_param_member_free(dest);
    return -1;
}

int  atiny_init(atiny_param_t* atiny_params, void** phandle)
{
    if (NULL == atiny_params || NULL == phandle)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return ATINY_ARG_INVALID;
    }

    memset((void*)&g_atiny_handle, 0, sizeof(handle_data_t));
    g_atiny_handle.atiny_params = *atiny_params;
    if(0 != atiny_param_dup(&(g_atiny_handle.atiny_params), atiny_params))
        return ATINY_MALLOC_FAILED;

    g_atiny_handle.atiny_quit = 0;
    *phandle = &g_atiny_handle;

    return ATINY_OK;
}

void atiny_deinit(void* phandle)
{
    handle_data_t* handle;
    MQTTClient *client;
    Network* network;

    if(NULL == phandle)
    {
        ATINY_LOG(LOG_FATAL, "Parameter null");
        return;
    }
    handle = (handle_data_t*)phandle;
    client = &(handle->client);
    network = client->ipstack;
    if(0 == handle->atiny_quit)
    {
        handle->atiny_quit = 1;
        atiny_param_member_free(&(handle->atiny_params));
        if(client->mutex) atiny_mutex_lock(client->mutex);
        device_info_member_free(&(handle->device_info));
        if(client->mutex) atiny_mutex_unlock(client->mutex);
        (void)MQTTDisconnect(client);
        MQTTClientDeInit(client);
        NetworkDisconnect(network);
    }

    return;
}

int mqtt_add_interest_topic(char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb, char **topic_dup)
{
    int i, rc = -1;
    atiny_interest_uri_t* interest_uris = g_atiny_handle.device_info.interest_uris;

    if(!topic || !cb || !topic_dup || !(qos>=CLOUD_QOS_MOST_ONCE && qos<CLOUD_QOS_LEVEL_MAX))
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != interest_uris[i].uri && 0 == strcmp(interest_uris[i].uri, topic))
        {
            interest_uris[i].qos = qos;
            interest_uris[i].cb = cb;
            *topic_dup = interest_uris[i].uri;
            return 0;
        }
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(interest_uris[i].uri == NULL)
        {
            interest_uris[i].uri = atiny_strdup((const char *)(topic));
            if(NULL ==  interest_uris[i].uri)
            {
                rc = ATINY_MALLOC_FAILED;
                break;
            }
            interest_uris[i].qos = qos;
            interest_uris[i].cb = cb;
            *topic_dup = interest_uris[i].uri;
            rc = 0;
            break;
        }
    }

    if(i == ATINY_INTEREST_URI_MAX_NUM)
        ATINY_LOG(LOG_WARNING, "num of interest uris is up to %d", ATINY_INTEREST_URI_MAX_NUM);

    return rc;
}

int mqtt_is_topic_subscribed(const char *topic)
{
    int i, rc = MQTT_TOPIC_SUBSCRIBED_FALSE;
    atiny_interest_uri_t* interest_uris = g_atiny_handle.device_info.interest_uris;

    if(!topic)
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != interest_uris[i].uri && 0 == strcmp(interest_uris[i].uri, topic))
        {
            rc = MQTT_TOPIC_SUBSCRIBED_TRUE;
            break;
        }
    }

    return rc;
}

int mqtt_del_interest_topic(const char *topic)
{
    int i, rc = -1;
    atiny_interest_uri_t* interest_uris = g_atiny_handle.device_info.interest_uris;

    if(!topic)
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != interest_uris[i].uri && 0 == strcmp(interest_uris[i].uri, topic))
        {
            atiny_free(interest_uris[i].uri);
            interest_uris[i].uri = NULL;
            memset(&(interest_uris[i]), 0x0, sizeof(interest_uris[i]));
            rc = 0;
        }
    }

    return rc;
}

int mqtt_is_topic_subscribed_same(char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb)
{
    int rc = MQTT_TOPIC_SUBSCRIBED_FALSE, i;
    atiny_interest_uri_t* interest_uris = g_atiny_handle.device_info.interest_uris;

    if(!topic || !cb || !(qos>=CLOUD_QOS_MOST_ONCE && qos<CLOUD_QOS_LEVEL_MAX))
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != interest_uris[i].uri && 0 == strcmp(interest_uris[i].uri, topic)
            && interest_uris[i].qos == qos && interest_uris[i].cb == cb)
        {
            rc = MQTT_TOPIC_SUBSCRIBED_TRUE;
            break;
        }
    }

    return rc;
}

int mqtt_topic_subscribe(MQTTClient *client, char *topic, cloud_qos_level_e qos, atiny_rsp_cb cb)
{
     int rc = -1;
     char *topic_dup = NULL;

    if(!client || !topic || !cb || !(qos>=CLOUD_QOS_MOST_ONCE && qos<CLOUD_QOS_LEVEL_MAX))
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    if(client->mutex) atiny_mutex_lock(client->mutex);
    if(MQTT_TOPIC_SUBSCRIBED_TRUE == mqtt_is_topic_subscribed_same(topic, qos, cb))
    {
        if(client->mutex) atiny_mutex_unlock(client->mutex);
        return 0;
    }

    if(0 != mqtt_add_interest_topic(topic, qos, cb, &topic_dup))
    {
        if(client->mutex) atiny_mutex_unlock(client->mutex);
        return -1;
    }
    if(client->mutex) atiny_mutex_unlock(client->mutex);

    rc = MQTTSubscribe(client, topic_dup, (enum QoS)qos, mqtt_message_arrived);
    if(0 != rc)
        ATINY_LOG(LOG_ERR, "MQTTSubscribe %s[%d]", topic, rc);

    return rc;
}

int mqtt_topic_unsubscribe(MQTTClient *client, const char *topic)
{
    int rc = -1;

    if(!client || !topic)
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    if(client->mutex) atiny_mutex_lock(client->mutex);
    rc = mqtt_is_topic_subscribed(topic);
    if(client->mutex) atiny_mutex_unlock(client->mutex);

    if(MQTT_TOPIC_SUBSCRIBED_TRUE == rc)
    {
        rc = MQTTUnsubscribe(client, topic);
        if(0 != rc)
            ATINY_LOG(LOG_ERR, "MQTTUnsubscribe %s[%d]", topic, rc);
        else
        {
            if(client->mutex) atiny_mutex_lock(client->mutex);
            (void)mqtt_del_interest_topic(topic);
            if(client->mutex) atiny_mutex_unlock(client->mutex);
        }
    }

    return rc;
}

int mqtt_message_publish(MQTTClient *client, cloud_msg_t* send_data)
{
    int rc = -1;
    MQTTMessage message;

    if(!client || !send_data)
    {
        ATINY_LOG(LOG_FATAL, "invalid params");
        return -1;
    }

    memset(&message, 0x0, sizeof(message));
    message.qos = (enum QoS)send_data->qos;
    message.retained = 0;
    message.payload = send_data->payload;
    message.payloadlen = send_data->payload_len;
    if((rc = MQTTPublish(client, send_data->uri, &message)) != 0)
        ATINY_LOG(LOG_ERR, "Return code from MQTT publish is %d", rc);

    return rc;
}

void mqtt_message_arrived(MessageData* md)
{
    MQTTMessage* message;
    MQTTString* topic;
    atiny_interest_uri_t* interest_uris = g_atiny_handle.device_info.interest_uris;
    cloud_msg_t msg;
    int i;

    if(NULL == md)
    {
        ATINY_LOG(LOG_FATAL, "NULL == md");
        return;
    }

    if(NULL == md->topic_sub)
    {
        ATINY_LOG(LOG_FATAL, "NULL == md->topic_sub");
        return;
    }

    message = md->message;
    topic = md->topicName;

    ATINY_LOG(LOG_DEBUG, "[%s] %.*s : %.*s", md->topic_sub, topic->lenstring.len, topic->lenstring.data, message->payloadlen, (char *)message->payload);

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != interest_uris[i].uri && NULL != interest_uris[i].cb)
        {
            if(0 == strcmp(md->topic_sub, interest_uris[i].uri))
            {
                memset(&msg, 0x0, sizeof(msg));
                if(topic->cstring)
                {
                    msg.uri = topic->cstring;
                    msg.uri_len = strlen(topic->cstring);
                }
                else
                {
                    msg.uri = topic->lenstring.data;
                    msg.uri_len = topic->lenstring.len;
                }
                msg.method = CLOUD_METHOD_POST;
                msg.qos = (cloud_qos_level_e)message->qos;
                msg.payload_len = message->payloadlen;
                msg.payload = message->payload;
                interest_uris[i].cb(&msg);
            }
        }
    }

    return;
}

int mqtt_subscribe_interest_topics(MQTTClient *client, atiny_interest_uri_t interest_uris[ATINY_INTEREST_URI_MAX_NUM])
{
    int i, rc = ATINY_ARG_INVALID;

    if(NULL == client || NULL == interest_uris)
    {
        ATINY_LOG(LOG_FATAL, "Parameter null");
        return ATINY_ARG_INVALID;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(client->mutex) atiny_mutex_lock(client->mutex);
        if(NULL == interest_uris[i].uri || '\0' == interest_uris[i].uri[0] || NULL == interest_uris[i].cb
            || !(interest_uris[i].qos>=CLOUD_QOS_MOST_ONCE && interest_uris[i].qos<CLOUD_QOS_LEVEL_MAX))
        {
             if(client->mutex) atiny_mutex_unlock(client->mutex);
            continue;
        }
        if(client->mutex) atiny_mutex_unlock(client->mutex);
        rc = MQTTSubscribe(client, interest_uris[i].uri, (enum QoS)interest_uris[i].qos, mqtt_message_arrived);
        ATINY_LOG(LOG_DEBUG, "MQTTSubscribe %s[%d]", interest_uris[i].uri, rc);
        if(rc != 0)
        {
            rc = ATINY_SOCKET_ERROR;
            break;
        }
    }

    return rc;
}

void will_options_member_free(cloud_will_options_t *will_options)
{
    if(NULL == will_options)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return;
    }

    if(NULL != will_options->topic_name)
    {
        atiny_free(will_options->topic_name);
        will_options->topic_name = NULL;
    }
    if(NULL != will_options->topic_msg)
    {
        atiny_free(will_options->topic_msg);
        will_options->topic_msg = NULL;
    }

    return;
}

void device_info_member_free(atiny_device_info_t* info)
{
    int i;

    if(NULL == info)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return;
    }

    if(NULL != info->client_id)
    {
        atiny_free(info->client_id);
        info->client_id = NULL;
    }
    if(NULL != info->user_name)
    {
        atiny_free(info->user_name);
        info->user_name = NULL;
    }
    if(NULL != info->password)
    {
        atiny_free(info->password);
        info->password = NULL;
    }

    if(MQTT_WILL_FLAG_TRUE == info->will_flag)
    {
        if(NULL != info->will_options)
        {
            will_options_member_free(info->will_options);
            atiny_free(info->will_options);
            info->will_options = NULL;
        }
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL != info->interest_uris[i].uri)
        {
            atiny_free(info->interest_uris[i].uri);
            info->interest_uris[i].uri = NULL;
        }
    }

    return;
}

int device_info_dup(atiny_device_info_t* dest, atiny_device_info_t* src)
{
    int i;

    if(NULL == dest || NULL == src)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return -1;
    }

    dest->client_id = atiny_strdup((const char *)(src->client_id));
    if(NULL == dest->client_id)
        goto device_info_dup_failed;

    if(NULL != dest->user_name)
    {
        dest->user_name = atiny_strdup((const char *)(src->user_name));
        if(NULL == dest->user_name)
            goto device_info_dup_failed;
    }

    if(NULL != dest->password)
    {
        dest->password = atiny_strdup((const char *)(src->password));
        if(NULL == dest->password)
            goto device_info_dup_failed;
    }

    dest->will_flag = src->will_flag;

    if(MQTT_WILL_FLAG_TRUE == dest->will_flag && NULL != src->will_options)
    {
        dest->will_options = (cloud_will_options_t *)atiny_malloc(sizeof(cloud_will_options_t));
        if(NULL == dest->will_options)
            goto device_info_dup_failed;

        dest->will_options->topic_name= atiny_strdup((const char *)(src->will_options->topic_name));
        if(NULL == dest->will_options->topic_name)
            goto device_info_dup_failed;
        dest->will_options->topic_msg= atiny_strdup((const char *)(src->will_options->topic_msg));
        if(NULL == dest->will_options->topic_msg)
            goto device_info_dup_failed;

        dest->will_options->retained = src->will_options->retained;
        dest->will_options->qos = src->will_options->qos;
    }

    for(i=0; i<ATINY_INTEREST_URI_MAX_NUM; i++)
    {
        if(NULL == src->interest_uris[i].uri || '\0' == src->interest_uris[i].uri[0] || NULL == src->interest_uris[i].cb
            || !(src->interest_uris[i].qos>=CLOUD_QOS_MOST_ONCE && src->interest_uris[i].qos<CLOUD_QOS_LEVEL_MAX))
            continue;
        dest->interest_uris[i].uri = atiny_strdup(src->interest_uris[i].uri);
        if(NULL == dest->interest_uris[i].uri)
            goto device_info_dup_failed;
        dest->interest_uris[i].qos = src->interest_uris[i].qos;
        dest->interest_uris[i].cb = src->interest_uris[i].cb;
    }

    return 0;
device_info_dup_failed:
    device_info_member_free(dest);
    return -1;
}

int atiny_isconnected(void* phandle)
{
    handle_data_t* handle;
    MQTTClient *client;

    if (NULL == phandle)
    {
        ATINY_LOG(LOG_ERR, "invalid args");
        return ATINY_ARG_INVALID;
    }

    handle = (handle_data_t*)phandle;
    client = &(handle->client);

    return MQTTIsConnected(client);
}

int atiny_bind(atiny_device_info_t* device_info, void* phandle)
{
    Network n;
    handle_data_t* handle;
    MQTTClient *client;
    atiny_param_t *atiny_params;
    atiny_device_info_t* device_info_t;
    int rc = -1, conn_failed_cnt = 0;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    if ((NULL == device_info) || (NULL == phandle))
    {
        ATINY_LOG(LOG_FATAL, "Parameter null");
        return ATINY_ARG_INVALID;
    }

    if(NULL == device_info->client_id)
    {
        ATINY_LOG(LOG_FATAL, "Parameter null");
        return ATINY_ARG_INVALID;
    }

    if(device_info->will_flag == MQTT_WILL_FLAG_TRUE && NULL == device_info->will_options)
    {
        ATINY_LOG(LOG_FATAL, "Parameter null");
        return ATINY_ARG_INVALID;
    }

    handle = (handle_data_t*)phandle;
    client = &(handle->client);
    atiny_params = &(handle->atiny_params);

    if(0 != device_info_dup(&(handle->device_info), device_info))
    {
        atiny_deinit(phandle);
        return ATINY_MALLOC_FAILED;
    }
    device_info_t = &(handle->device_info);

    NetworkInit(&n);

    switch(atiny_params->security_type)
    {
        case CLOUD_SECURITY_TYPE_NONE:
            n.proto = MQTT_PROTO_NONE;
            break;
        case CLOUD_SECURITY_TYPE_PSK:
            n.proto = MQTT_PROTO_TLS_PSK;
            n.psk.psk_id = atiny_params->u.psk.psk_id;
            n.psk.psk_id_len = atiny_params->u.psk.psk_id_len;
            n.psk.psk = atiny_params->u.psk.psk;
            n.psk.psk_len = atiny_params->u.psk.psk_len;
            break;
        case CLOUD_SECURITY_TYPE_CA:
            ATINY_LOG(LOG_INFO, "CLOUD_SECURITY_TYPE_CA unsupported now" );
            return ATINY_ARG_INVALID;
        default:
            ATINY_LOG(LOG_WARNING, "invalid security_typ : %d", atiny_params->security_type);
            break;
    }

    memset(client, 0x0, sizeof(MQTTClient));
    MQTTClientInit(client, &n, MQTT_COMMAND_TIMEOUT_MS, mqtt_sendbuf, MQTT_SENDBUF_SIZE, mqtt_readbuf, MQTT_READBUF_SIZE);

    data.willFlag = device_info_t->will_flag;
    data.MQTTVersion = MQTT_VERSION_3_1;
    data.clientID.cstring = device_info_t->client_id;
    data.username.cstring = device_info_t->user_name;
    data.password.cstring = device_info_t->password;
    if(device_info_t->will_flag == MQTT_WILL_FLAG_TRUE)
    {
        data.will.topicName.cstring = device_info_t->will_options->topic_name;
        data.will.message.cstring = device_info_t->will_options->topic_msg;
        data.will.qos= device_info_t->will_options->qos;
        data.will.retained = device_info_t->will_options->retained;
    }
    data.keepAliveInterval = MQTT_KEEPALIVE_INTERVAL_S;
    data.cleansession = MQTT_CLEAN_SESSION_TRUE;

    while(handle->atiny_quit == 0)
    {
        if(conn_failed_cnt > 0)
        {
            ATINY_LOG(LOG_INFO, "reconnect delay : %d", conn_failed_cnt);
            (void)LOS_TaskDelay(MQTT_CONN_FAILED_BASE_DELAY<<conn_failed_cnt);
        }
        rc = NetworkConnect(&n, atiny_params->server_ip, atoi(atiny_params->server_port));
        ATINY_LOG(LOG_DEBUG, "NetworkConnect : %d", rc);
        if(rc != 0)
        {
            if(conn_failed_cnt < MQTT_CONN_FAILED_MAX_TIMES)
                conn_failed_cnt++;
            continue;
        }

        ATINY_LOG(LOG_DEBUG, "Send mqtt CONNECT to %s:%s", atiny_params->server_ip, atiny_params->server_port);
        rc = MQTTConnect(client, &data);
        ATINY_LOG(LOG_DEBUG, "CONNACK : %d", rc);
        if(0 != rc)
        {
            ATINY_LOG(LOG_ERR, "MQTTConnect failed");
            if(conn_failed_cnt < MQTT_CONN_FAILED_MAX_TIMES) 
                conn_failed_cnt++;
            goto connect_again;
        }

        if(ATINY_SOCKET_ERROR == mqtt_subscribe_interest_topics(client, device_info_t->interest_uris))
        {
            ATINY_LOG(LOG_ERR, "mqtt_subscribe_interest_topics failed");
            if(conn_failed_cnt < MQTT_CONN_FAILED_MAX_TIMES)
                conn_failed_cnt++;
            goto connect_again;
        }

        conn_failed_cnt = 0;
        while (rc >= 0 && MQTTIsConnected(client) && handle->atiny_quit == 0)
        {
            rc = MQTTYield(client, MQTT_EVENTS_HANDLE_PERIOD_MS);
        }
connect_again:
        (void)MQTTDisconnect(client);
        NetworkDisconnect(&n);
        data.cleansession = MQTT_CLEAN_SESSION_FALSE;
        ATINY_LOG(LOG_ERR, "connect_again");
    }
    return ATINY_OK;
}

static int mqtt_send_packet_len(cloud_msg_t* send_data)
{
    int rem_len = 0;

    if (NULL == send_data || NULL == send_data->uri
        || !(send_data->qos>=CLOUD_QOS_MOST_ONCE && send_data->qos<CLOUD_QOS_LEVEL_MAX)
        || !(send_data->method>=CLOUD_METHOD_GET&& send_data->method<CLOUD_METHOD_MAX))
    {
        return -1;
    }

    rem_len += 2 + strlen(send_data->uri) + send_data->payload_len;
    if (send_data->qos > 0)
        rem_len += 2; /* packetid */

    rem_len += 1; /* header byte */

    /* now remaining_length field */
    if (rem_len < 128)
        rem_len += 1;
    else if (rem_len < 16384)
        rem_len += 2;
    else if (rem_len < 2097151)
        rem_len += 3;
    else
        rem_len += 4;
    return rem_len;
}

int atiny_data_send(void* phandle, cloud_msg_t* send_data, atiny_rsp_cb cb)
{
    handle_data_t* handle;
    MQTTClient *client;
    int rc= -1;

    if (NULL == phandle || NULL == send_data || NULL == send_data->uri 
        || !(send_data->qos>=CLOUD_QOS_MOST_ONCE && send_data->qos<CLOUD_QOS_LEVEL_MAX)
        || !(send_data->method>=CLOUD_METHOD_GET&& send_data->method<CLOUD_METHOD_MAX))
    {
        ATINY_LOG(LOG_ERR, "invalid args");
        return ATINY_ARG_INVALID;
    }

    handle = (handle_data_t*)phandle;
    client = &(handle->client);

    if(MQTTIsConnected(client) != 1)
        return -1;

    switch(send_data->method)
    {
        case CLOUD_METHOD_GET:
            if(NULL == cb)
                return ATINY_ARG_INVALID;
            rc = mqtt_topic_subscribe(client, send_data->uri, send_data->qos, cb);
            break;
        case CLOUD_METHOD_POST:
            if(send_data->payload_len<= 0 || NULL == send_data->payload || mqtt_send_packet_len(send_data) > MQTT_SENDBUF_SIZE)
                return ATINY_ARG_INVALID;
            rc = mqtt_message_publish(client, send_data);
            break;
        case CLOUD_METHOD_DEL:
            rc = mqtt_topic_unsubscribe(client, send_data->uri);
            break;
        default:
            ATINY_LOG(LOG_WARNING, "unsupported method : %d", send_data->method);
    }

    return rc;
}

