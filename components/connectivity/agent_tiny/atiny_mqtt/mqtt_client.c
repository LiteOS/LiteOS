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

#include <stdlib.h>
#include "atiny_mqtt/mqtt_client.h"
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "flash_manager.h"
#include "cJSON.h"
#include "hmac.h"

#include  <mqtt_al.h>
#include  "atiny_error.h"
#include "log/atiny_log.h"



#define VARIABLE_SIZE (4 + 1)
#define CMD_TOPIC_FMT            "/huawei/v1/devices/%s/command/%s"
#define DATA_TOPIC_FMT           "/huawei/v1/devices/%s/data/%s"
#define SECRET_NOTIFY_TOPIC_FMT  "/huawei/v1/products/%s/sn/%s/secretNotify"
#define SECRET_ACK_TOPIC_FMT     "/huawei/v1/products/%s/sn/%s/secretACK"

#define MQTT_TIME_BUF_LEN 11

#define IS_VALID_NAME_LEN(name) (strnlen((name), STRING_MAX_LEN + 1) <= STRING_MAX_LEN)

typedef enum
{
    MQTT_CONNECT_WITH_PRODUCT_ID,
    MQTT_CONNECT_WITH_DEVICE_ID
}mqtt_dynamic_connect_state_e;

typedef struct
{
    mqtt_dynamic_connect_state_e  state;
    mqtt_static_connection_info_s save_info;
    char                         *got_password;
    uint8_t                       connection_update_flag;
    uint8_t                       has_device_id;                      //
}mqtt_dynamic_info_s;

struct mqtt_client_tag_s
{
    mqtt_device_info_s       device_info;
    mqtt_param_s             params;
    mqtt_dynamic_info_s      dynamic_info;
    char                    *sub_topic;
    void                    *mqtthandle;
    uint8_t                  init_flag;
};

static mqtt_client_s g_mqtt_client;

//return 0 copy ok else failed
static int mqtt_strdup(mqtt_al_string_t *dst,const mqtt_al_string_t *src)
{
	int ret = -1;
	dst->data = atiny_malloc(src->len);
	if(NULL != dst->data)
	{
		memcpy(dst->data,src->data,src->len);
		dst->len = src->len;
		ret = 0;
	}

	return ret;
}


static int mqtt_cmd_ioctl(mqtt_cmd_e cmd, void *arg, int32_t len)
{
    mqtt_client_s* handle = &g_mqtt_client;

    if (handle->params.cmd_ioctl != NULL)
    {
        return handle->params.cmd_ioctl(cmd, arg, len);
    }
    ATINY_LOG(LOG_FATAL, "cmd_ioctl null");
    return ATINY_ERR;
}

static void mqtt_free_params(mqtt_param_s *param)
{

    atiny_free(param->server_ip);
    atiny_free(param->server_port);
    switch(param->info.type)
    {
		case en_mqtt_al_security_psk:
			atiny_free(param->info.u.psk.psk_id.data);
			atiny_free(param->info.u.psk.psk_key.data);
			break;
		case en_mqtt_al_security_cas:
			atiny_free(param->info.u.cas.ca_crt.data);
			break;
		default:
			break;
    }
}

static int mqtt_check_param(const mqtt_param_s *param)
{
    if ((param->server_ip == NULL)        \
        || (param->server_port == NULL)   \
		|| (param->info.type == en_mqtt_al_security_none)             \
        || (param->info.type >= en_mqtt_al_security_end)              \
        || (param->cmd_ioctl == NULL))
    {
        ATINY_LOG(LOG_FATAL, "invalid param, sec type %d", param->info.type);
        return ATINY_ARG_INVALID;
    }
    if (param->info.type == en_mqtt_al_security_psk)
    {
        if ((param->info.u.psk.psk_key.data == NULL)
            || (param->info.u.psk.psk_key.len <= 0)
            || (param->info.u.psk.psk_id.data == NULL)
            || (param->info.u.psk.psk_id.len <= 0))
        {
            ATINY_LOG(LOG_FATAL, "invalid psk");
            return ATINY_ARG_INVALID;
        }
    }

    if (param->info.type == en_mqtt_al_security_cas)
    {
        if ((param->info.u.cas.ca_crt.data == NULL)||\
        	(param->info.u.cas.ca_crt.len <=0))
        {
            ATINY_LOG(LOG_FATAL, "invalid ca");
            return ATINY_ARG_INVALID;
        }
    }

    return ATINY_OK;
}

static int mqtt_dup_param(mqtt_param_s *dest, const mqtt_param_s *src)
{
    memset(dest, 0, sizeof(*dest));

    dest->info.type = src->info.type;
    dest->cmd_ioctl = src->cmd_ioctl;

    dest->server_ip = atiny_strdup(src->server_ip);
    if(NULL == dest->server_ip)
    {
        ATINY_LOG(LOG_FATAL, "atiny_strdup NULL");
        return ATINY_MALLOC_FAILED;
    }

    dest->server_port = atiny_strdup(src->server_port);
    if(NULL == dest->server_port)
    {
        ATINY_LOG(LOG_FATAL, "atiny_strdup NULL");
        goto mqtt_param_dup_failed;
    }

    switch(src->info.type)
    {
        case en_mqtt_al_security_psk:

            if(0 != mqtt_strdup(&dest->info.u.psk.psk_id,&src->info.u.psk.psk_id))
            {
                ATINY_LOG(LOG_FATAL, "atiny_strdup NULL");
                goto mqtt_param_dup_failed;
            }

            if(0 != mqtt_strdup(&dest->info.u.psk.psk_key,&src->info.u.psk.psk_key))
            {
                ATINY_LOG(LOG_FATAL, "atiny_strdup NULL");
                goto mqtt_param_dup_failed;
            }
            break;

        case en_mqtt_al_security_cas:
            if(0 != mqtt_strdup(&dest->info.u.cas.ca_crt,&src->info.u.cas.ca_crt))
            {
                ATINY_LOG(LOG_FATAL, "atiny_strdup NULL");
                goto mqtt_param_dup_failed;
            }
            break;
        default:
            break;
    }

    return ATINY_OK;

mqtt_param_dup_failed:
    mqtt_free_params(dest);
    return ATINY_MALLOC_FAILED;
}


static void mqtt_free_dynamic_info(mqtt_client_s* handle)
{
    if (handle->sub_topic)
    {
        atiny_free(handle->sub_topic);
        handle->sub_topic = NULL;
    }
    atiny_free(handle->dynamic_info.save_info.deviceid);
    atiny_free(handle->dynamic_info.got_password);
}

static void mqtt_free_device_info(mqtt_device_info_s *info)
{

    atiny_free(info->password);
    if(MQTT_STATIC_CONNECT == info->connection_type)
    {
        atiny_free(info->u.s_info.deviceid);
    }
    else
    {
        atiny_free(info->u.d_info.productid);
        atiny_free(info->u.d_info.nodeid);
    }
}

static int mqtt_check_device_info(const mqtt_device_info_s *info)
{
    if((info->connection_type >= MQTT_MAX_CONNECTION_TYPE)
        || (info->codec_mode >= MQTT_MAX_CODEC_MODE)
        || (info->sign_type >= MQTT_MAX_SIGN_TYPE)
        || (NULL == info->password)
        || (!IS_VALID_NAME_LEN(info->password)))
    {
        ATINY_LOG(LOG_FATAL, "invalid device info con_type %d codec_mode %d signe type %d",
            info->connection_type, info->codec_mode, info->sign_type);
        return ATINY_ARG_INVALID;
    }

    if ((info->connection_type == MQTT_STATIC_CONNECT)
        && ((NULL == info->u.s_info.deviceid)
        || (!IS_VALID_NAME_LEN(info->u.s_info.deviceid))))
    {
        ATINY_LOG(LOG_FATAL, "invalid static device info con_type %d codec_mode %d signe type %d",
            info->connection_type, info->codec_mode, info->sign_type);
        return ATINY_ARG_INVALID;
    }

    if ((info->connection_type == MQTT_DYNAMIC_CONNECT)
        &&((NULL == info->u.d_info.productid)
        || (!IS_VALID_NAME_LEN(info->u.d_info.productid))
        || (NULL == info->u.d_info.nodeid)
        || !(IS_VALID_NAME_LEN(info->u.d_info.nodeid))))
    {
        ATINY_LOG(LOG_FATAL, "invalid dynamic device info con_type %d codec_mode %d signe type %d",
            info->connection_type, info->codec_mode, info->sign_type);
        return ATINY_ARG_INVALID;
    }

    return ATINY_OK;

}

static int mqtt_dup_device_info(mqtt_device_info_s *dest, const mqtt_device_info_s *src)
{
    memset(dest, 0, sizeof(*dest));
    dest->connection_type = src->connection_type;
    dest->sign_type = src->sign_type;
    dest->codec_mode = src->codec_mode;
    dest->password = atiny_strdup(src->password);
    if (NULL == dest->password)
    {
        ATINY_LOG(LOG_FATAL, "atiny_strdup fail");
        return ATINY_MALLOC_FAILED;
    }

    if(MQTT_STATIC_CONNECT == src->connection_type)
    {
        dest->u.s_info.deviceid = atiny_strdup(src->u.s_info.deviceid);
        if (NULL == dest->u.s_info.deviceid)
        {
            ATINY_LOG(LOG_FATAL, "atiny_strdup fail");
            goto MALLOC_FAIL;
        }
    }
    else
    {

        dest->u.d_info.productid = atiny_strdup(src->u.d_info.productid);
        if (NULL == dest->u.d_info.productid)
        {
            ATINY_LOG(LOG_FATAL, "atiny_strdup fail");
            goto MALLOC_FAIL;
        }
        dest->u.d_info.nodeid = atiny_strdup(src->u.d_info.nodeid);
        if (NULL == dest->u.d_info.nodeid)
        {
            ATINY_LOG(LOG_FATAL, "atiny_strdup fail");
            goto MALLOC_FAIL;
        }
    }

    return ATINY_OK;

MALLOC_FAIL:
    mqtt_free_device_info(dest);
    return ATINY_MALLOC_FAILED;
}

static bool mqtt_is_connectting_with_deviceid(const mqtt_client_s* handle)
{
    return (MQTT_STATIC_CONNECT == handle->device_info.connection_type)
           || (handle->dynamic_info.state == MQTT_CONNECT_WITH_DEVICE_ID);
}
/**@brief  : which means use the dynamic mode or static mode
 *
 *
 * @return :the generate data,malloc from the heap ,need to release when not used
 *
 */

#define cn_client_id_fmt_static     "%s_%d_%d_%s"      ///<  deviceid  connect_type check_time time
#define cn_client_id_fmt_dynamic    "%s_%s_%d_%d_%s"   ///<  deviceid  noteid connect_type check_time time

static  char *generate_client_id(char *deviceid,char *productid,char *noteid,char *time,\
									bool connect_static,bool check_time)
{
	int   buflen;
	char *buf;
	char *ret = NULL;

	if(connect_static)
	{
		if(IS_VALID_NAME_LEN(time) && IS_VALID_NAME_LEN(deviceid))
		{
			buflen = strlen(deviceid)  + strlen(time) + \
					strlen(cn_client_id_fmt_static) + 1;
			buf = atiny_malloc(buflen);
			if (NULL != buf)
			{
				snprintf(buf,buflen,cn_client_id_fmt_static,deviceid,\
						connect_static?0:1,check_time,time);
				ret = buf;
			}
		}
	}
	else
	{
		if(IS_VALID_NAME_LEN(time) && IS_VALID_NAME_LEN(productid) && IS_VALID_NAME_LEN(noteid))
		{
			buflen = strlen(productid) + strlen (noteid) +strlen(time) +\
					strlen(cn_client_id_fmt_static) + 1;
			buf = atiny_malloc(buflen);
			if (NULL != buf)
			{
				snprintf(buf,buflen,cn_client_id_fmt_dynamic,productid,noteid,\
						connect_static?0:1,check_time,time);
				ret = buf;
			}
		}
	}

	return ret;
}

static  char *generate_client_passwd(char *passwd,char *time)
{
    char hmac[32];
    const uint32_t len = sizeof(hmac) * 2 + 1;
    char *ret = NULL;
    char *buf = NULL;
    uint32_t i;

    if( IS_VALID_NAME_LEN(passwd) &&  IS_VALID_NAME_LEN(time))
    {
        if(0 == hmac_generate_passwd(passwd,strlen(passwd),time,strlen(time),hmac,sizeof(hmac)))
        {
        	buf = atiny_malloc(len);
        	if(NULL != buf)
        	{
        	    for(i = 0; i < sizeof(hmac); i++)
        	    {
        	        snprintf(buf + i * 2, 3, "%02x", hmac[i]);
        	    }
        	    ret = buf;
        	}
        }
    }

    return ret;
}
static  char *generate_client_user(char *user)
{
	int   len;
	char *ret = NULL;

	if (IS_VALID_NAME_LEN(user))
	{
		len = strlen(user) + 1;
		ret = atiny_malloc( len);
		if (NULL != ret)
		{
			strncpy(ret,user,len-1);
			ret[len-1] = '\0';
		}
	}

	return ret;
}

static int connect_server(mqtt_client_s* handle)
{
	bool  connect_static;
	bool  check_time;
    char time[MQTT_TIME_BUF_LEN];
    char *passwd   = NULL;
    char *deviceid = NULL;
    char *productid= NULL;
    char *noteid   = NULL;
    char *user;
    mqtt_al_conpara_t conpara;
    int   ret = ATINY_ERR;

    connect_static = mqtt_is_connectting_with_deviceid(handle);
    check_time = handle->device_info.sign_type;

    if (connect_static)
    {
        if (handle->device_info.connection_type == MQTT_STATIC_CONNECT)
        {
        	deviceid = handle->device_info.u.s_info.deviceid;
        	passwd = handle->device_info.password;
        }
        else
        {
        	deviceid = handle->dynamic_info.save_info.deviceid;
        	passwd = handle->dynamic_info.got_password;
        }
        user = deviceid;
        ATINY_LOG(LOG_INFO, "try static connect");
    }
    else
    {
    	productid = handle->device_info.u.d_info.productid;
    	noteid = handle->device_info.u.d_info.nodeid;
    	passwd = handle->device_info.password;
        user = productid;
        ATINY_LOG(LOG_INFO, "try dynamic connect");
    }

    if (mqtt_cmd_ioctl(MQTT_GET_TIME, time, sizeof(time)) != ATINY_OK)
    {
        ATINY_LOG(LOG_FATAL, "mqtt_cmd_ioctl fail");
        return ret;
    }
    time[sizeof(time) - 1] = '\0';
    //GENERATE THE DATA THE CLOUD PLATFORM NEED
    memset(&conpara,0,sizeof(conpara));
    conpara.clientid.data = generate_client_id(deviceid,productid,noteid,time,connect_static,check_time);
    conpara.clientid.len = strlen(conpara.clientid.data);

    conpara.user.data = generate_client_user(user);
    conpara.user.len = strlen(conpara.user.data);

    conpara.passwd.data = generate_client_passwd(passwd,time);
    conpara.passwd.len = strlen(conpara.passwd.data);
    if((NULL ==conpara.clientid.data ) || (NULL == conpara.user.data) || (NULL == conpara.passwd.data))
    {
    	atiny_free(conpara.clientid.data);
    	atiny_free(conpara.user.data);
    	atiny_free(conpara.passwd.data);

    	return ret;
    }

    conpara.cleansession = 1;
    conpara.keepalivetime = MQTT_KEEPALIVE_INTERVAL_S;
    conpara.security = &handle->params.info;
    conpara.serveraddr.data = handle->params.server_ip;
    conpara.serveraddr.len = strlen(handle->params.server_ip);
    conpara.serverport = atoi (handle->params.server_port);
    conpara.timeout = 10000;
    conpara.version = en_mqtt_al_version_3_1_1;
    conpara.willmsg = NULL;
    ATINY_LOG(LOG_DEBUG, "CLIENTID:%s USER:%s PASSWD:%s \r\n",\
    		conpara.clientid.data,conpara.user.data,conpara.passwd.data);

    handle->mqtthandle = mqtt_al_connect(&conpara);
    if(NULL != handle->mqtthandle)
    {
        ret = ATINY_OK;
    }

	atiny_free(conpara.clientid.data);
	atiny_free(conpara.user.data);
	atiny_free(conpara.passwd.data);

    return ret;
}


static char *mqtt_get_topic(const mqtt_client_s* handle, const char *fmt, uint32_t fixed_size,
            const char *deviceid_or_productid, const char *sn_or_codec_mode)
{
    uint32_t len;
    char *topic;

    len = fixed_size + strnlen(deviceid_or_productid, STRING_MAX_LEN) + \
    	  strnlen(sn_or_codec_mode, STRING_MAX_LEN) + 1;
    topic = atiny_malloc(len);
    if (topic == NULL)
    {
        ATINY_LOG(LOG_FATAL, "atiny_malloc fail, len %d", len);
        return NULL;
    }

    (void)snprintf(topic, len, fmt, deviceid_or_productid, sn_or_codec_mode);

    return topic;
}


static char *mqtt_get_device_topic(const mqtt_client_s* handle, const char *fmt, uint32_t fixed_size)
{
    char *deviceid;
    const char *codec_mode[MQTT_MAX_CODEC_MODE] = {"binary", "json"};

    deviceid = ((handle->device_info.connection_type == MQTT_STATIC_CONNECT) ?
                handle->device_info.u.s_info.deviceid : handle->dynamic_info.save_info.deviceid);

    return mqtt_get_topic(handle, fmt, fixed_size, deviceid, codec_mode[handle->device_info.codec_mode]);

}

static char *mqtt_get_secret_topic(const mqtt_client_s* handle, const char *fmt, uint32_t fixed_size)
{
    return mqtt_get_topic(handle, fmt, fixed_size,
            handle->device_info.u.d_info.productid, handle->device_info.u.d_info.nodeid);
}

static int mqtt_parse_secret_topic(mqtt_client_s* handle, const char *payload, uint32_t len)
{

    //TODO:parse json, log
    cJSON *msg_type;
    cJSON *deviceid;
    cJSON *secret;
    cJSON * root = NULL;
    int ret = ATINY_ERR;


    root = cJSON_Parse(payload);
    if (root == NULL)
    {
        ATINY_LOG(LOG_ERR, "err secret notify, len %d, msg %s", len, payload);
         goto EXIT;
    }

    msg_type = cJSON_GetObjectItem(root, MQTT_MSG_TYPE);
    if ((msg_type == NULL) || (msg_type->valuestring == NULL)
        || (strncmp(msg_type->valuestring, "cloudSendSecret", STRING_MAX_LEN + 1) != 0))
    {
        ATINY_LOG(LOG_ERR, "msg_type not right");
        goto EXIT;
    }

    deviceid = cJSON_GetObjectItem(root, "deviceid");
    if ((deviceid == NULL) || (deviceid->valuestring == NULL)
        || (!IS_VALID_NAME_LEN(deviceid->valuestring)))
    {
        ATINY_LOG(LOG_ERR, "deviceid not right");
        goto EXIT;
    }

    secret = cJSON_GetObjectItem(root, "secret");
    if ((secret == NULL) || (secret->valuestring == NULL)
        || (!IS_VALID_NAME_LEN(secret->valuestring)))
    {
        ATINY_LOG(LOG_ERR, "secret not right");
        goto EXIT;
    }

    handle->dynamic_info.has_device_id = false;
    atiny_free(handle->dynamic_info.save_info.deviceid);
    handle->dynamic_info.save_info.deviceid = atiny_strdup(deviceid->valuestring);
    if (handle->dynamic_info.save_info.deviceid == NULL)
    {
        ATINY_LOG(LOG_INFO, "atiny_strdup null");
        goto EXIT;
    }
    atiny_free(handle->dynamic_info.got_password);
    handle->dynamic_info.got_password = atiny_strdup(secret->valuestring);
    if (handle->dynamic_info.got_password == NULL)
    {
        ATINY_LOG(LOG_INFO, "atiny_strdup null");
        goto EXIT;
    }
    handle->dynamic_info.has_device_id = true;
    handle->dynamic_info.connection_update_flag = true;
    ret = ATINY_OK;
    ATINY_LOG(LOG_INFO, "get secret info right");
EXIT:
    if (root)
    {
        cJSON_Delete(root);
    }
    return ret;
}

static void mqtt_send_secret_ack(mqtt_client_s* handle)
{

	mqtt_al_pubpara_t  pubpara;
    char* topic = mqtt_get_secret_topic(handle, SECRET_ACK_TOPIC_FMT,\
    		                            sizeof(SECRET_ACK_TOPIC_FMT) - VARIABLE_SIZE);
    if (topic == NULL)
    {
        return;
    }

    memset(&pubpara, 0, sizeof(pubpara));
    pubpara.qos = en_mqtt_al_qos_1 ;
    pubpara.retain = 0;
    pubpara.timeout = 1000;
    pubpara.topic.data = topic;
    pubpara.topic.len =strlen(topic);

    if(0 != mqtt_al_publish(handle->mqtthandle,&pubpara))
    {
        ATINY_LOG(LOG_FATAL, "MQTTPublish fail");
    }
    atiny_free(topic);

    return;
}



static void mqtt_recv_secret_topic(void *arg,mqtt_al_msgrcv_t  *msg)
{
	//for we must add the '/0' to the end to make sure the json parse correct
    char  *buf;
    mqtt_client_s* handle = &g_mqtt_client;

    if ((msg == NULL) || ( msg->msg.data == NULL))
    {
        ATINY_LOG(LOG_FATAL, "null point or msg err");
        return;
    }

    buf = atiny_malloc(msg->msg.len + 1);
    if(NULL == buf)
    {
    	return;
    }
    memcpy(buf,msg->msg.data,msg->msg.len);
    buf[msg->msg.len] = '\0';

    if (mqtt_parse_secret_topic(handle,buf, msg->msg.len) == ATINY_OK)
    {
        flash_info_s flash_info;
        flash_info.items[PRODUCT_IDX] = handle->device_info.u.d_info.productid;
        flash_info.items[NODEID_IDX] = handle->device_info.u.d_info.nodeid;
        flash_info.items[DEVICEID_IDX] = handle->dynamic_info.save_info.deviceid;
        flash_info.items[PASSWORD_IDX] = handle->dynamic_info.got_password;
        if (flash_manager_write(&flash_info) == ATINY_OK)
        {
            mqtt_send_secret_ack(handle);
        }

    }

    atiny_free(buf);

}

static void mqtt_recv_cmd_topic(void *arg,mqtt_al_msgrcv_t  *msg)
{
	//for we must add the '/0' to the end to make sure the json parse correct
    char  *buf;

    if ((msg == NULL) || ( msg->msg.data == NULL))
    {
        ATINY_LOG(LOG_FATAL, "null point or msg err");
        return;
    }

    buf = atiny_malloc(msg->msg.len + 1);
    if(NULL == buf)
    {
    	return;
    }
    memcpy(buf,msg->msg.data,msg->msg.len);
    buf[msg->msg.len] = '\0';
    mqtt_cmd_ioctl(MQTT_RCV_MSG, buf, msg->msg.len);
    atiny_free(buf);
}

static int mqtt_subscribe_topic(mqtt_client_s* handle)
{
    char *topic;
    void (*topi_callback)(void *arg,mqtt_al_msgrcv_t *msg);
    mqtt_al_subpara_t   subpara;
    int rc = ATINY_ERR;

    if (handle->sub_topic)
    {
        atiny_free(handle->sub_topic);
        handle->sub_topic = NULL;
    }

    if (mqtt_is_connectting_with_deviceid(handle))
    {
        topic = mqtt_get_device_topic(handle, CMD_TOPIC_FMT,\
        		                      sizeof(CMD_TOPIC_FMT) - VARIABLE_SIZE);
        topi_callback = mqtt_recv_cmd_topic;
        ATINY_LOG(LOG_INFO, "try subcribe static topic");
    }
    else
    {
        topic = mqtt_get_secret_topic(handle, SECRET_NOTIFY_TOPIC_FMT, \
        		                      sizeof(SECRET_NOTIFY_TOPIC_FMT) - VARIABLE_SIZE);;
        topi_callback = mqtt_recv_secret_topic;
         ATINY_LOG(LOG_INFO, "try subcribe dynamic topic");
    }

    if (topic == NULL)
    {
        return rc;
    }

    memset(&subpara,0,sizeof(subpara));
    subpara.arg = NULL;
    subpara.dealer = topi_callback;
    subpara.qos = en_mqtt_al_qos_1;
    subpara.topic.data = topic;
    subpara.topic.len = strlen(topic);
    if(0 != mqtt_al_subscribe(handle->mqtthandle,&subpara))
    {
        ATINY_LOG(LOG_FATAL, "MQTTSubscribe fail,rc=%d, topic=%s", rc, topic);
        atiny_free(topic);
    }
    else
    {
        handle->sub_topic = topic;
        rc = ATINY_OK;
    }
    return rc;
}

static inline void mqtt_inc_fail_cnt(int32_t *conn_failed_cnt)
{
    if(*conn_failed_cnt < MQTT_CONN_FAILED_MAX_TIMES)
    {
        (*conn_failed_cnt)++;
    }
}

static void mqtt_read_flash_info(mqtt_client_s* handle)
{
    flash_info_s flash_info;

    if (handle->device_info.connection_type == MQTT_STATIC_CONNECT)
    {
        return;
    }

    memset(&handle->dynamic_info, 0, sizeof(handle->dynamic_info));
    memset(&flash_info, 0, sizeof(flash_info));
    if (flash_manager_read(&flash_info) != ATINY_OK)
    {
        return;
    }

    if ((strcmp(flash_info.items[PRODUCT_IDX], handle->device_info.u.d_info.productid) != 0)
        || (strcmp(flash_info.items[NODEID_IDX], handle->device_info.u.d_info.nodeid) != 0))
    {
        ATINY_LOG(LOG_INFO, "flash info for the nodeid and not use");
        flash_manager_destroy_flash_info(&flash_info);
        return;
    }

    ATINY_LOG(LOG_DEBUG, "mqtt read info deviceid %s,procid %s,nodid %s",
            flash_info.items[DEVICEID_IDX], flash_info.items[PRODUCT_IDX], flash_info.items[NODEID_IDX]);

    handle->dynamic_info.save_info.deviceid = flash_info.items[DEVICEID_IDX];
    flash_info.items[DEVICEID_IDX] = NULL;
    handle->dynamic_info.got_password =  flash_info.items[PASSWORD_IDX];
    flash_info.items[PASSWORD_IDX] = NULL;
    handle->dynamic_info.has_device_id = true;
    handle->dynamic_info.state = MQTT_CONNECT_WITH_DEVICE_ID;
    flash_manager_destroy_flash_info(&flash_info);
}

int  atiny_mqtt_init(const mqtt_param_s *params, mqtt_client_s **phandle)
{
    if (params == NULL || phandle == NULL
        || mqtt_check_param(params) != ATINY_OK)
    {
        ATINY_LOG(LOG_FATAL, "Invalid args");
        return ATINY_ARG_INVALID;
    }

    if (g_mqtt_client.init_flag)
    {
        ATINY_LOG(LOG_FATAL, "mqtt reinit");
        return ATINY_ERR;
    }

    memset(&g_mqtt_client, 0, sizeof(g_mqtt_client));

    cJSON_InitHooks(NULL);

    if (ATINY_OK != mqtt_dup_param(&(g_mqtt_client.params), params))
    {
        return ATINY_MALLOC_FAILED;
    }

    flash_manager_init(mqtt_cmd_ioctl);   ///< register the io to the flash manager

    *phandle = &g_mqtt_client;

    g_mqtt_client.init_flag = true;

    return ATINY_OK;
}
///< TODO wait for the secret info not implement yet
int atiny_mqtt_bind(const mqtt_device_info_s* device_info, mqtt_client_s* handle)
{
    int32_t conn_failed_cnt = 0;
//    Timer timer;
    int ret = ATINY_ERR;

    if (NULL == handle)
    {
        ATINY_LOG(LOG_FATAL, "handle null");
        return ATINY_ARG_INVALID;
    }

    if((device_info == NULL)
        || (mqtt_check_device_info(device_info) != ATINY_OK))
    {
        ATINY_LOG(LOG_FATAL, "parameter invalid");
        ret = ATINY_ARG_INVALID;
        goto  EXIT_ATINY_BIND_QUIT;
    }

    ret = mqtt_dup_device_info(&(handle->device_info), device_info);
    if (ret != ATINY_OK)
    {
        goto  EXIT_ATINY_BIND_QUIT;
    }

    mqtt_read_flash_info(handle);

    if(0 != mqtt_al_init())
    {
    	goto EXIT_ATINY_BIND_QUIT;
    }

    while(true)
    {
        if(conn_failed_cnt > 0)
        {
            ATINY_LOG(LOG_INFO, "reconnect delay : %d", conn_failed_cnt);
            (void)LOS_TaskDelay(MQTT_CONN_FAILED_BASE_DELAY << conn_failed_cnt);
        }

        if(connect_server(handle) != ATINY_OK)
        {
        	mqtt_inc_fail_cnt(&conn_failed_cnt);
        	continue;
        }

        if(ATINY_OK != mqtt_subscribe_topic(handle))
        {
        	mqtt_al_disconnect(handle->mqtthandle);
        	handle->mqtthandle = NULL;
        	mqtt_inc_fail_cnt(&conn_failed_cnt);
            ATINY_LOG(LOG_ERR, "mqtt_subscribe_topic failed");
            continue;
        }

        conn_failed_cnt = 0;
//        if (!mqtt_is_connectting_with_deviceid(handle))
//        {
//            TimerInit(&timer);
//            TimerCountdownMS(&timer, MQTT_WRITE_FOR_SECRET_TIMEOUT);
//        }
        while (en_mqtt_al_connect_ok == mqtt_al_check_status(handle->mqtthandle))
        {

            // receive secret info
            if (handle->dynamic_info.connection_update_flag)
            {
                ATINY_LOG(LOG_INFO, "recv secret info");
                ATINY_LOG(LOG_DEBUG, "secret info deviceid %s", handle->dynamic_info.save_info.deviceid);
                handle->dynamic_info.connection_update_flag = false;
                handle->dynamic_info.state = MQTT_CONNECT_WITH_DEVICE_ID;
                break;
            }

//            // wait secret info timeout.
//            if (!mqtt_is_connectting_with_deviceid(handle) && (TimerIsExpired(&timer)))
//            {
//                if (handle->dynamic_info.has_device_id)
//                {
//                    handle->dynamic_info.state = MQTT_CONNECT_WITH_DEVICE_ID;
//                }
//                break;
//            }
            atiny_delay(10);
        }

        mqtt_al_disconnect(handle->mqtthandle);
    }

    ATINY_LOG(LOG_FATAL, "ATINY MQTT MAIN THREAD QUIT");


EXIT_ATINY_BIND_QUIT:
    mqtt_free_dynamic_info(handle);
    mqtt_free_params(&(handle->params));
    mqtt_free_device_info(&(handle->device_info));
    handle->init_flag = false;

    mqtt_al_deinit();
    return ret;
}

int atiny_mqtt_data_send(mqtt_client_s *phandle, const char *msg,  uint32_t msg_len, mqtt_qos_e qos)
{
    int rc;
    char* topic;
    size_t payloadlen;
    mqtt_al_pubpara_t  pubpara;

    if ((phandle == NULL) || (qos >= MQTT_QOS_MAX))
    {
        ATINY_LOG(LOG_FATAL, "Parameter invalid");
        return ATINY_ARG_INVALID;
    }
    if (phandle->device_info.codec_mode == MQTT_CODEC_MODE_JSON)
    {
        if (msg == NULL || msg_len <= 0)
        {
            ATINY_LOG(LOG_FATAL, "msg invalid");
            return ATINY_ARG_INVALID;
        }
        payloadlen = strnlen(msg, msg_len);
    }
    else
    {
        if (msg == NULL && msg_len > 0)
        {
            ATINY_LOG(LOG_FATAL, "msg invalid");
            return ATINY_ARG_INVALID;
        }
        payloadlen = msg_len;
    }

    if (!atiny_mqtt_isconnected(phandle))
    {
        ATINY_LOG(LOG_FATAL, "not connected");
        return ATINY_ERR;
    }

    topic = mqtt_get_device_topic(phandle, DATA_TOPIC_FMT, \
    		                      sizeof(DATA_TOPIC_FMT) - VARIABLE_SIZE);
    if (topic == NULL)
    {
        return ATINY_MALLOC_FAILED;
    }
    memset(&pubpara, 0, sizeof(pubpara));

    pubpara.qos = qos ;
    pubpara.retain = 0;
    pubpara.timeout = 1000;
    pubpara.topic.data = topic;
    pubpara.topic.len =strlen(topic);
    pubpara.msg.data =(char *)msg;
    pubpara.msg.len = payloadlen;

    if(0 != mqtt_al_publish(phandle->mqtthandle,&pubpara))
    {
        ATINY_LOG(LOG_FATAL, "MQTTPublish fail");
        rc = ATINY_OK;
    }
    else
    {
    	rc = ATINY_ERR;
    }
    atiny_free(topic);

    return rc;
}

int atiny_mqtt_isconnected(mqtt_client_s* phandle)
{
	bool ret = false;
    if (NULL == phandle)
    {
        ATINY_LOG(LOG_ERR, "invalid args");
        return ret;
    }
    if(mqtt_is_connectting_with_deviceid(phandle) &&\
      (en_mqtt_al_connect_ok ==mqtt_al_check_status(phandle->mqtthandle)))
    {
    	ret = true;
    }

    return  ret;
}

