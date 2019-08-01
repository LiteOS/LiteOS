/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-05-05 19:15  zhangqianfu  The first version:
 *
 *  1, only support CRT tls mode
 *  2, the data encode only support the json mode
 *  3, the passwd only support the no check time mode
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <link_misc.h>

#include <osal.h>
#include <mqtt_al.h>
#include <oc_mqtt_al.h>
#include <sal.h>

#include <cJSON.h>           //json mode
#include "hmac.h"            //used to generate the user passwd
#include <atiny_mqtt.h>

#define VARIABLE_SIZE (4 + 1)
#define cn_cmd_topic_fmt               "/huawei/v1/devices/%s/command/%s"
#define cn_data_topic_fmt              "/huawei/v1/devices/%s/data/%s"
#define cn_secret_notify_topic_fmt     "/huawei/v1/products/%s/sn/%s/secretNotify"
#define cn_secret_ack_topic_fmt        "/huawei/v1/products/%s/sn/%s/secretACK"

#define cn_bs_request_topic_fmt               "/huawei/v1/devices/%s/iodpsData"
#define cn_bs_message_topic_fmt               "/huawei/v1/devices/%s/iodpsCommand"


#ifndef cn_mqtt_keepalive_interval_s
#define cn_mqtt_keepalive_interval_s (100)
#endif

/* the unit is milisecond */
#ifndef cn_mqtt_write_for_secret_timeout
#define cn_mqtt_write_for_secret_timeout (30 * 1000)
#endif

/* MQTT retry connection delay interval. The delay inteval is
(cn_mqtt_conn_failed_base_delay << MIN(coutinious_fail_count, cn_mqtt_conn_failed_max_times)) */
#ifndef cn_mqtt_conn_failed_max_times
#define cn_mqtt_conn_failed_max_times  6
#endif

/*The unit is millisecond*/
#ifndef cn_mqtt_conn_failed_base_delay
#define cn_mqtt_conn_failed_base_delay 1000
#endif

#define MQTT_TIME_BUF_LEN 11


#define cn_string_max_len  127

#define is_valid_string(name) (strnlen((name), cn_string_max_len + 1) <= cn_string_max_len)

#define cn_bs_rcv_buf_len 256
static char            s_rcv_buffer[cn_bs_rcv_buf_len];
static int             s_rcv_datalen;


typedef enum
{
    en_dynamic_connect_with_product_id = 0,
    en_dynamic_connect_with_device_id
}en_dynamic_connect_state;

typedef struct
{
    tag_oc_mqtt_config  config;

    unsigned int        b_flag_update:1;          ///< when get device id and passwd should set it
    unsigned int        b_flag_get_passswd:1;     ///< this bit to be set when dynamic and received secret
    unsigned int        b_flag_connect_dynamic:1; ///< this bit to be set when dynamic and not get secret yet
    unsigned int        b_flag_rcv_snd:1;         ///< this bit to be set when could receive and send
    unsigned int        b_flag_stop:1;            ///< this bit to be set when stop the engine
    //unsigned int        b_flag_bs_finished:1;     ///< this bit to be set when bootstrap is finished
    char               *get_device_id;            ///< when dynamic mode,this is received from the server
    char               *get_device_passwd;        ///< when dynamic mode,this is received from the server
    ///< the following used for the mqtt connect
    void         *mqtt_handle;
    char         *mqtt_client_id;
    char         *mqtt_user;
    char         *mqtt_passwd;
    char         *mqtt_subtopic;
    char         *mqtt_pubtopic;

}tag_oc_mqtt_agent_cb;


/**
 * @brief: this is used to storage the dynamic message to the rom
 *
 * the fomat is following:
 *
 * 4Byte    4Byte  32Byte   JsonString
 *
 * Magic    Length  hmac    JsonString(the string length is Length)
 *
 * */

#define cn_secret_magic  0xaa55aa55
typedef struct
{
    uint32_t magic;
    uint32_t length;
    uint8_t  hmac[32];
}tag_oc_secret_rom;

#define cn_secret_index_product_id_name     "productid"
#define cn_secret_index_product_pass_name   "productname"
#define cn_secret_index_note_id_name        "noteid"
#define cn_secret_index_time_name           "time"
#define cn_secret_index_device_id_name      "deviceid"
#define cn_secret_index_device_pass_name    "secret"

#define cn_check_time_value                 "2018111517"

static osal_semp_t     s_agent_sync;
static char            iot_server_ip[16];
static char            iot_server_port[6];  ///< i think max is 65536, so string is at most 6 bytes


static int  secret_get_devceid_passwd(tag_oc_mqtt_agent_cb  *cb)
{
    int ret = -1;

    cJSON *root = NULL;
    cJSON *tmp;
    char  *buf;
    int    len;
    tag_oc_secret_rom  rom;

    memset(&rom,0,sizeof(rom));

    cb->config.device_info.d_device.read_secret(0,(char *)&rom, sizeof(rom));

    if(rom.magic != cn_secret_magic)
    {
        goto ERR_MAGIC;
    }

    len = rom.length;
    buf = osal_malloc(len);
    if(NULL == buf)
    {
        goto ERR_BUF_MALLOC;
    }

    cb->config.device_info.d_device.read_secret(sizeof(rom),buf,len);
    root = cJSON_Parse(buf);
    if(NULL == root)
    {
        goto EXIT_JSON_PARSE;
    }

    tmp = cJSON_GetObjectItem(root, cn_secret_index_product_id_name);
    if((NULL == tmp) || (!cJSON_IsString(tmp))||\
       (0 != strcmp(cb->config.device_info.d_device.productid,tmp->valuestring)))
    {
        goto EXIT_PRODUCT_ID_ERR;
    }

    tmp = cJSON_GetObjectItem(root, cn_secret_index_product_pass_name);
    if((NULL == tmp) || (!cJSON_IsString(tmp))||\
       (0 != strcmp(cb->config.device_info.d_device.productpasswd,tmp->valuestring)))
    {
        goto EXIT_PRODUCT_PASS_ERR;
    }

    tmp = cJSON_GetObjectItem(root, cn_secret_index_note_id_name);
    if((NULL == tmp) || (!cJSON_IsString(tmp))||\
       (0 != strcmp(cb->config.device_info.d_device.notetid,tmp->valuestring)))
    {
        goto EXIT_NOTEID_ERR;
    }

    ///< ok now, we get it
    tmp = cJSON_GetObjectItem(root, cn_secret_index_device_id_name);
    if((NULL == tmp) || (!cJSON_IsString(tmp)))
    {
        goto EXIT_JSON_DEVICE_ID_ERR;
    }
    cb->get_device_id = osal_strdup(tmp->valuestring);
    if(NULL == cb->get_device_id)
    {
        goto EXIT_COPY_DEVICE_ID_ERR;
    }

    tmp = cJSON_GetObjectItem(root, cn_secret_index_device_pass_name);
    if((NULL == tmp) || (!cJSON_IsString(tmp)))
    {
        goto EXIT_JSON_DEVICE_PASS_ERR;
    }
    cb->get_device_passwd = osal_strdup(tmp->valuestring);
    if(NULL == cb->get_device_passwd)
    {
        goto EXIT_COPY_DEVICE_PASS_ERR;
    }
    cb->b_flag_get_passswd =1;

    //release the resouce
    osal_free(buf);
    cJSON_Delete(root);

    ret = 0;
    return ret;

EXIT_COPY_DEVICE_PASS_ERR:
EXIT_JSON_DEVICE_PASS_ERR:
    osal_free(cb->get_device_id);
    cb->get_device_id = NULL;
EXIT_COPY_DEVICE_ID_ERR:
EXIT_JSON_DEVICE_ID_ERR:
EXIT_NOTEID_ERR:
EXIT_PRODUCT_PASS_ERR:
EXIT_PRODUCT_ID_ERR:
EXIT_JSON_PARSE:
    osal_free(buf);
ERR_BUF_MALLOC:
ERR_MAGIC:
    return ret;
}


static int  secret_save_devceid_passwd(tag_oc_mqtt_agent_cb  *cb)
{
    int ret = -1;

    cJSON *root;
    cJSON *tmp;
    char  *jsonbuf;
    char  *buf;
    int    len;
    tag_oc_secret_rom rom;

    root = cJSON_CreateObject();
    if(NULL == root)
    {
        goto EXIT_JSON_ROOT;
    }

    tmp = cJSON_CreateString(cb->config.device_info.d_device.productid);
    if(NULL == tmp)
    {
        goto EXIT_JSON_TMP;
    }
    cJSON_AddItemToObject(root,cn_secret_index_product_id_name,tmp);

    tmp = cJSON_CreateString(cb->config.device_info.d_device.productpasswd);
    if(NULL == tmp)
    {
        goto EXIT_JSON_TMP;
    }
    cJSON_AddItemToObject(root,cn_secret_index_product_pass_name,tmp);

    tmp = cJSON_CreateString(cb->config.device_info.d_device.notetid);
    if(NULL == tmp)
    {
        goto EXIT_JSON_TMP;
    }
    cJSON_AddItemToObject(root,cn_secret_index_note_id_name,tmp);

    tmp = cJSON_CreateString(cb->get_device_id);
    if(NULL == tmp)
    {
        goto EXIT_JSON_TMP;
    }
    cJSON_AddItemToObject(root,cn_secret_index_device_id_name,tmp);

    tmp = cJSON_CreateString(cb->get_device_passwd);
    if(NULL == tmp)
    {
        goto EXIT_JSON_TMP;
    }
    cJSON_AddItemToObject(root,cn_secret_index_device_pass_name,tmp);

    jsonbuf = cJSON_Print(root);
    if(NULL== jsonbuf)
    {
        goto EXIT_JSON_BUF;
    }

    len = strlen(jsonbuf);
    buf = osal_malloc(len + sizeof(rom));
    if(NULL == buf)
    {
        goto EXIT_SECRET_BUF;
    }
    rom.magic = cn_secret_magic;
    rom.length = len;
    hmac_generate_passwd(jsonbuf,len,(char *)&rom.magic,sizeof(rom.magic),(char *)rom.hmac,sizeof(rom.hmac));
    memcpy(buf,&rom,sizeof(rom));
    memcpy(buf+ sizeof(rom),jsonbuf,len);
    cb->config.device_info.d_device.write_secret(0,buf,len +sizeof(rom));

    osal_free(jsonbuf);
    osal_free(buf);
    cJSON_Delete(root);
    ret = 0;
    return ret;


EXIT_SECRET_BUF:
    osal_free(jsonbuf);
EXIT_JSON_BUF:
EXIT_JSON_TMP:
    cJSON_Delete(root);
EXIT_JSON_ROOT:
    return ret;
}


///< check the config parameters
static int check_clone_config_params(tag_oc_mqtt_agent_cb *cb,tag_oc_mqtt_config *config)
{
    int ret = -1;
    if(NULL == config->server)
    {
        goto EXIT_ERR;
    }
    if(NULL == config->port)
    {
        goto EXIT_ERR;
    }

    if(en_mqtt_al_security_cas != config->security.type)
    {
        goto EXIT_ERR;
    }

    if(en_oc_mqtt_code_mode_json != config->code_mode )
    {
        goto EXIT_ERR;
    }

    if(en_mqtt_sign_type_hmacsha256_check_time_no != config->sign_type )
    {
        goto EXIT_ERR;
    }

    if(en_oc_mqtt_device_type_static == config->device_type)
    {
        if((NULL == config->device_info.s_device.deviceid) ||\
           (NULL == config->device_info.s_device.devicepasswd))
        {
            goto EXIT_ERR;
        }
    }
    else
    {
        if((NULL == config->device_info.d_device.productid) ||\
           (NULL == config->device_info.d_device.productid) ||\
           (NULL == config->device_info.d_device.notetid) ||\
           (NULL == config->device_info.d_device.read_secret) ||\
           (NULL == config->device_info.d_device.write_secret))
        {
            goto EXIT_ERR;
        }
    }

    if((en_oc_mqtt_device_type_static == config->device_type) &&\
        (en_mqtt_auth_type_model == config->auth_type))
    {
        goto EXIT_ERR;
    }

    if((en_oc_mqtt_device_type_dynamic == config->device_type) &&\
        (en_mqtt_auth_type_model != config->auth_type))
    {
        goto EXIT_ERR;
    }

    memset(cb,0,sizeof(tag_oc_mqtt_agent_cb));
    cb->config = *config;

    ret = 0;
    return ret;


EXIT_ERR:
    return ret;
}


#define cn_client_id_fmt_static     "%s_%d_%d_%s"      ///<  deviceid  connect_type check_time time
#define cn_client_id_fmt_dynamic    "%s_%s_%d_%d_%s"   ///<  deviceid  noteid connect_type check_time time

static const char *s_codec_mode[en_oc_mqtt_code_mode_max] = {"binary", "json"};
///< generate the client_id user passwd for the mqtt need


static int free_mqtt_para(tag_oc_mqtt_agent_cb *cb)
{
    if(NULL != cb->mqtt_client_id)
    {
        osal_free(cb->mqtt_client_id);
        cb->mqtt_client_id = NULL;
    }

    if(NULL != cb->mqtt_user)
    {
        osal_free(cb->mqtt_user);
        cb->mqtt_user = NULL;
    }

    if(NULL != cb->mqtt_passwd)
    {
        osal_free(cb->mqtt_passwd);
        cb->mqtt_passwd = NULL;
    }

    if(NULL != cb->mqtt_subtopic)
    {
        osal_free(cb->mqtt_subtopic);
        cb->mqtt_subtopic = NULL;
    }

    if(NULL != cb->mqtt_pubtopic)
    {
        osal_free(cb->mqtt_pubtopic);
        cb->mqtt_pubtopic = NULL;
    }

    return 0;
}

static int gen_mqtt_para(tag_oc_mqtt_agent_cb *cb)
{
    int ret = -1;
    int len;
    int i;
    unsigned char hmac[32];
    char  *time_value;
    char  *passwd;

    time_value = cn_check_time_value;

    if(cb->config.device_type == en_oc_mqtt_device_type_static)   ///< static device mode
    {
        len = strlen(cb->config.device_info.s_device.deviceid)  + \
              strlen(time_value) + strlen(cn_client_id_fmt_static) + 1;
        cb->mqtt_client_id = osal_malloc(len);
        if (cb->mqtt_client_id != NULL)
        {
            snprintf(cb->mqtt_client_id,len,cn_client_id_fmt_static,cb->config.device_info.s_device.deviceid,\
                    cb->config.auth_type,cb->config.sign_type,time_value);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strlen(cb->config.device_info.s_device.deviceid) +1;
        cb->mqtt_user = osal_malloc(len);
        if (cb->mqtt_user != NULL)
        {
            memcpy(cb->mqtt_user,cb->config.device_info.s_device.deviceid,len);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = sizeof(hmac) * 2 + 1;
        passwd = (char *)cb->config.device_info.s_device.devicepasswd;
        cb->mqtt_passwd = osal_malloc(len);
        if(NULL != cb->mqtt_passwd)
        {
            hmac_generate_passwd(passwd,strlen(passwd),time_value,strlen(time_value),\
                                 hmac,sizeof(hmac));

            for(i = 0; i < sizeof(hmac); i++)
            {
                snprintf(cb->mqtt_passwd + i * 2, 3, "%02x", hmac[i]);
            }
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strnlen(cb->config.device_info.s_device.deviceid, cn_string_max_len) +\
                strnlen(s_codec_mode[cb->config.code_mode], cn_string_max_len) +\
                strlen(cn_cmd_topic_fmt) + 1;
        cb->mqtt_subtopic = osal_malloc(len);
        if(NULL != cb->mqtt_subtopic)
        {
            snprintf(cb->mqtt_subtopic, len, cn_cmd_topic_fmt, \
                    cb->config.device_info.s_device.deviceid, s_codec_mode[cb->config.code_mode]);
        }
        else
        {
            goto EXIT_MALLOC;
        }


        len = strnlen(cb->config.device_info.s_device.deviceid, cn_string_max_len) +\
                strnlen(s_codec_mode[cb->config.code_mode], cn_string_max_len) +\
                strlen(cn_data_topic_fmt) + 1;
        cb->mqtt_pubtopic = osal_malloc(len);
        if(NULL != cb->mqtt_pubtopic)
        {
            snprintf(cb->mqtt_pubtopic, len, cn_data_topic_fmt, \
                    cb->config.device_info.s_device.deviceid, s_codec_mode[cb->config.code_mode]);
        }
        else
        {
            goto EXIT_MALLOC;
        }

    }
    else if(cb->b_flag_get_passswd)   ///< dynatic,but get the device id and passwd
    {

        len = strlen(cb->get_device_id)  + \
              strlen(time_value) + strlen(cn_client_id_fmt_static) + 1;
        cb->mqtt_client_id = osal_malloc(len);
        if (cb->mqtt_client_id != NULL)
        {
            snprintf(cb->mqtt_client_id,len,cn_client_id_fmt_static,cb->get_device_id,\
                    cb->config.auth_type,cb->config.sign_type,time_value);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strlen(cb->get_device_id) +1;
        cb->mqtt_user = osal_malloc(len);
        if (cb->mqtt_user != NULL)
        {
            memcpy(cb->mqtt_user,cb->get_device_id,len);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = sizeof(hmac) * 2 + 1;
        passwd = cb->get_device_passwd;
        cb->mqtt_passwd = osal_malloc(len);
        if(NULL != cb->mqtt_passwd)
        {
            hmac_generate_passwd(passwd,strlen(passwd),time_value,strlen(time_value),hmac,sizeof(hmac));

            for(i = 0; i < sizeof(hmac); i++)
            {
                snprintf(cb->mqtt_passwd + i * 2, 3, "%02x", hmac[i]);
            }
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strnlen(cb->get_device_id, cn_string_max_len) +\
                strnlen(s_codec_mode[cb->config.code_mode], cn_string_max_len) +\
                strlen(cn_cmd_topic_fmt) + 1;
        cb->mqtt_subtopic = osal_malloc(len);
        if(NULL != cb->mqtt_subtopic)
        {
            snprintf(cb->mqtt_subtopic, len, cn_cmd_topic_fmt, \
                    cb->get_device_id, s_codec_mode[cb->config.code_mode]);
        }
        else
        {
            goto EXIT_MALLOC;
        }


        len = strnlen(cb->get_device_id, cn_string_max_len) +\
                strnlen(s_codec_mode[cb->config.code_mode], cn_string_max_len) +\
                strlen(cn_data_topic_fmt) + 1;
        cb->mqtt_pubtopic = osal_malloc(len);
        if(NULL != cb->mqtt_pubtopic)
        {
            snprintf(cb->mqtt_pubtopic, len, cn_data_topic_fmt, \
                    cb->get_device_id, s_codec_mode[cb->config.code_mode]);
        }
        else
        {
            goto EXIT_MALLOC;
        }


    }
    else   ///< dynamic mode, have not get the device id and passwd yet
    {

        len = strlen(cb->config.device_info.d_device.productid) + strlen (cb->config.device_info.d_device.notetid) +\
                strlen(time_value) +strlen(cn_client_id_fmt_dynamic) + 1;
        cb->mqtt_client_id = osal_malloc(len);
        if (cb->mqtt_client_id != NULL)
        {
            snprintf(cb->mqtt_client_id,len,cn_client_id_fmt_dynamic,\
                    cb->config.device_info.d_device.productid,\
                    cb->config.device_info.d_device.notetid,
                    1,cb->config.sign_type,time_value);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strlen(cb->config.device_info.d_device.productid) +1;
        cb->mqtt_user = osal_malloc(len);
        if (cb->mqtt_user != NULL)
        {
            memcpy(cb->mqtt_user,cb->config.device_info.d_device.productid,len);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = sizeof(hmac) * 2 + 1;
        passwd = (char *) cb->config.device_info.d_device.productpasswd;
        cb->mqtt_passwd = osal_malloc(len);
        if(NULL != cb->mqtt_passwd)
        {
            hmac_generate_passwd(passwd,strlen(passwd),time_value,strlen(time_value),hmac,sizeof(hmac));

            for(i = 0; i < sizeof(hmac); i++)
            {
                snprintf(cb->mqtt_passwd + i * 2, 3, "%02x", hmac[i]);
            }
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strnlen(cb->config.device_info.d_device.productid, cn_string_max_len) +\
              strnlen(cb->config.device_info.d_device.notetid, cn_string_max_len) +\
              strlen(cn_secret_notify_topic_fmt) + 1;
        cb->mqtt_subtopic = osal_malloc(len);
        if(NULL != cb->mqtt_subtopic)
        {
            snprintf(cb->mqtt_subtopic, len, cn_secret_notify_topic_fmt, \
                    cb->config.device_info.d_device.productid, \
                    cb->config.device_info.d_device.notetid);
        }
        else
        {
            goto EXIT_MALLOC;
        }

        len = strnlen(cb->config.device_info.d_device.productid, cn_string_max_len) +\
              strnlen(cb->config.device_info.d_device.notetid, cn_string_max_len) +\
              strlen(cn_secret_ack_topic_fmt) + 1;
        cb->mqtt_pubtopic = osal_malloc(len);
        if(NULL != cb->mqtt_pubtopic)
        {
            snprintf(cb->mqtt_pubtopic, len, cn_secret_ack_topic_fmt, \
                    cb->config.device_info.d_device.productid, \
                    cb->config.device_info.d_device.notetid);
        }
        else
        {
            goto EXIT_MALLOC;
        }

    }

    ret = 0;
    return ret;


EXIT_MALLOC:
    free_mqtt_para(cb);
    return ret;
}

static int gen_bs_para(tag_oc_mqtt_agent_cb *cb)
{
    int ret = -1;
    int len;
    int i;
    unsigned char hmac[32];
    char  *time_value;
    char  *passwd;

    time_value = cn_check_time_value;

    len = strlen(cb->config.device_info.s_device.deviceid)  + \
          strlen(time_value) + strlen(cn_client_id_fmt_static) + 1;
    cb->mqtt_client_id = osal_malloc(len);
    if (cb->mqtt_client_id != NULL)
    {
        snprintf(cb->mqtt_client_id,len,cn_client_id_fmt_static,cb->config.device_info.s_device.deviceid,\
                cb->config.auth_type,cb->config.sign_type,time_value);
    }
    else
    {
        goto EXIT_MALLOC;
    }

    len = strlen(cb->config.device_info.s_device.deviceid) +1;
    cb->mqtt_user = osal_malloc(len);
    if (cb->mqtt_user != NULL)
    {
        memcpy(cb->mqtt_user,cb->config.device_info.s_device.deviceid,len);
    }
    else
    {
        goto EXIT_MALLOC;
    }

    len = sizeof(hmac) * 2 + 1;
    passwd = (char *)cb->config.device_info.s_device.devicepasswd;
    cb->mqtt_passwd = osal_malloc(len);
    if(NULL != cb->mqtt_passwd)
    {
        if(0 == hmac_generate_passwd(passwd,strlen(passwd),time_value,strlen(time_value),\
                             hmac,sizeof(hmac)))
        {
            for(i = 0; i < sizeof(hmac); i++)
            {
                snprintf(cb->mqtt_passwd + i * 2, 3, "%02x", hmac[i]);
            }
        }
    }
    else
    {
        goto EXIT_MALLOC;
    }

    len = strnlen(cb->config.device_info.s_device.deviceid, cn_string_max_len) +\
            strlen(cn_bs_message_topic_fmt) + 1;
    cb->mqtt_subtopic = osal_malloc(len);
    if(NULL != cb->mqtt_subtopic)
    {
        snprintf(cb->mqtt_subtopic, len, cn_bs_message_topic_fmt, \
                cb->config.device_info.s_device.deviceid);
    }
    else
    {
        goto EXIT_MALLOC;
    }


    len = strnlen(cb->config.device_info.s_device.deviceid, cn_string_max_len) +\
            strlen(cn_bs_request_topic_fmt) + 1;
    cb->mqtt_pubtopic = osal_malloc(len);
    if(NULL != cb->mqtt_pubtopic)
    {
        snprintf(cb->mqtt_pubtopic, len, cn_bs_request_topic_fmt, \
                cb->config.device_info.s_device.deviceid);
    }
    else
    {
        goto EXIT_MALLOC;
    }


    return 0;


EXIT_MALLOC:
    free_mqtt_para(cb);
    return ret;
}


static int connect_server(tag_oc_mqtt_agent_cb *cb)
{
    mqtt_al_conpara_t conpara;
    int   ret = -1;
    struct hostent* entry = NULL;

    //GENERATE THE DATA THE CLOUD PLATFORM NEED
    memset(&conpara,0,sizeof(conpara));

    conpara.clientid.data = cb->mqtt_client_id;
    conpara.clientid.len = strlen(conpara.clientid.data);

    conpara.user.data =  cb->mqtt_user;
    conpara.user.len = strlen(conpara.user.data);

    conpara.passwd.data = cb->mqtt_passwd;
    conpara.passwd.len = strlen(conpara.passwd.data);

    conpara.cleansession = 1;
    conpara.keepalivetime = cb->config.lifetime;
    conpara.security = &cb->config.security;

    while(NULL == entry)
    {
        printf("debug:%s\r\n", cb->config.server);
        entry = sal_gethostbyname(cb->config.server);
        if(entry && entry->h_addr_list[0])
        {
            inet_ntop(entry->h_addrtype, entry->h_addr_list[0], iot_server_ip, sizeof(iot_server_ip));
            printf("ip:%s\n", iot_server_ip);
            conpara.serveraddr.data = iot_server_ip;
        }
        else
        {
            printf("LOG ############NO IP ---TRY AGAIN\n\r");
            osal_task_sleep(1000);
        }
    }
    conpara.serveraddr.len = strlen(conpara.serveraddr.data);
    conpara.serverport = atoi(cb->config.port);
    conpara.timeout = 10000;
    conpara.version = en_mqtt_al_version_3_1_1;
    conpara.willmsg = NULL;
    printf("CLIENTID:%s \r\n USER:%s \r\n PASSWD:%s \r\n",\
            conpara.clientid.data,conpara.user.data,conpara.passwd.data);

    cb->mqtt_handle = mqtt_al_connect(&conpara);
    if(NULL != cb->mqtt_handle)
    {
        if(cb->b_flag_get_passswd || (cb->config.device_type == en_oc_mqtt_device_type_static))
        {
            cb->b_flag_rcv_snd =1;
        }
        ret = 0;
    }

    return ret;
}


///< ---TODO, test, for the paho-mqtt could not pass the parameters
static tag_oc_mqtt_agent_cb *s_oc_agent_cb;
static void secret_msg_dealer(void *arg,mqtt_al_msgrcv_t  *msg)
{
    cJSON  *root;
    cJSON  *device_id;
    cJSON  *passwd;
    //for we must add the '/0' to the end to make sure the json parse correct
    char  *buf;
    if ((msg == NULL) || ( msg->msg.data == NULL))
    {
        return;
    }
    buf = osal_malloc(msg->msg.len + 1);
    if(NULL == buf)
    {
        return;
    }
    memcpy(buf,msg->msg.data,msg->msg.len);
    buf[msg->msg.len] = '\0';

    root = cJSON_Parse(buf);
    if(NULL != root)
    {
        device_id = cJSON_GetObjectItem(root,cn_secret_index_device_id_name);
        passwd = cJSON_GetObjectItem(root,cn_secret_index_device_pass_name);

        if((NULL != device_id) &&(cJSON_IsString(device_id))&&\
                (NULL != passwd) &&(cJSON_IsString(passwd)))
        {
            s_oc_agent_cb->get_device_id = osal_strdup(device_id->valuestring);
            s_oc_agent_cb->get_device_passwd = osal_strdup(passwd->valuestring);
            secret_save_devceid_passwd(s_oc_agent_cb);
            s_oc_agent_cb->b_flag_get_passswd= 1;
            s_oc_agent_cb->b_flag_update =1;
        }
        cJSON_Delete(root);
    }
    osal_free(buf);
}

static void app_msg_dealer(void *arg,mqtt_al_msgrcv_t  *msg)
{
    //for we must add the '/0' to the end to make sure the json parse correct
    if ((msg == NULL) || ( msg->msg.data == NULL))
    {
        return;
    }
    
    if(s_oc_agent_cb->config.boot_status == en_oc_boot_strap_status_bs)
        s_oc_agent_cb->config.bsinfo_dealer(s_oc_agent_cb,msg);
    else
        s_oc_agent_cb->config.msgdealer(s_oc_agent_cb,msg);
}


static int subscribe_topic(tag_oc_mqtt_agent_cb *cb)
{
    int ret = -1;
    mqtt_al_subpara_t   subpara;

    memset(&subpara,0,sizeof(subpara));

    if((cb->config.device_type == en_oc_mqtt_device_type_static)||\
       (cb->b_flag_get_passswd))
    {
        subpara.dealer = app_msg_dealer;
    }
    else
    {
        subpara.dealer = secret_msg_dealer;
    }

    subpara.arg = cb;
    subpara.qos = en_mqtt_al_qos_1;
    subpara.topic.data = cb->mqtt_subtopic ;
    subpara.topic.len = strlen(cb->mqtt_subtopic);

    ret = mqtt_al_subscribe(cb->mqtt_handle,&subpara);

    return ret;
}



static int __oc_agent_engine(void *args)
{
    int conn_failed_cnt = 0;
    osal_loop_timer_t  timer;
    int ret = -1;

    tag_oc_mqtt_agent_cb  *cb;

    cb = (tag_oc_mqtt_agent_cb *)args;
    if(NULL == cb)
    {
        return ret;
    }
    if(cb->config.device_type == en_oc_mqtt_device_type_dynamic)
    {
        secret_get_devceid_passwd(cb);
    }

    while(cb->b_flag_stop == 0)
    {

        if(conn_failed_cnt > 0)
        {
            osal_task_sleep(cn_mqtt_conn_failed_base_delay << conn_failed_cnt);
        }

        free_mqtt_para(cb);
        if(-1 == gen_mqtt_para(cb))
        {
            osal_task_sleep(cn_mqtt_conn_failed_base_delay << conn_failed_cnt);
            continue; ///< generate all the parameters needed by the ocean connect server
        }

        if(0 != connect_server(cb))
        {
            conn_failed_cnt++;
            continue;
        }

        ret = subscribe_topic(cb);
        if(ret == -1)
        {
            mqtt_al_disconnect(cb->mqtt_handle);
            free_mqtt_para(cb);
            cb->b_flag_rcv_snd = 0;
        }

        s_oc_agent_cb = cb;

        if((cb->config.device_type == en_oc_mqtt_device_type_dynamic)&&\
                (cb->b_flag_get_passswd == 0))
        {
            osal_loop_timer_init(&timer);
            osal_loop_timer_count_down(&timer, cn_mqtt_write_for_secret_timeout);
        }
        do{
            if (cb->b_flag_update)
            {
                cb->b_flag_update = 0;
                break;
            }
            if((cb->config.device_type == en_oc_mqtt_device_type_dynamic)&&\
                    (cb->b_flag_get_passswd == 0) &&(osal_loop_timer_expired(&timer)))
            {
                break;
            }

            osal_task_sleep(100);
        }while(en_mqtt_al_connect_ok == mqtt_al_check_status(cb->mqtt_handle));

        mqtt_al_disconnect(cb->mqtt_handle);
        free_mqtt_para(cb);
        cb->b_flag_rcv_snd = 0;
    }

    ///< release all the resources
    s_oc_agent_cb = NULL;
    free_mqtt_para(cb);
    osal_free(cb->get_device_id);
    osal_free(cb->get_device_passwd);
    osal_free(cb);

    return ret;
}

static int __oc_bs_engine(void *args)
{
    int conn_failed_cnt = 0;
    int ret = -1;

    tag_oc_mqtt_agent_cb  *cb;

    cb = (tag_oc_mqtt_agent_cb *)args;
    if(NULL == cb)
    {
        return ret;
    }

    while(cb->b_flag_stop == 0)
    {
        if(conn_failed_cnt > 0)
        {
            osal_task_sleep(cn_mqtt_conn_failed_base_delay << conn_failed_cnt);
        }

        free_mqtt_para(cb);
        if(-1 == gen_bs_para(cb))
        {
            osal_task_sleep(cn_mqtt_conn_failed_base_delay << conn_failed_cnt);
            continue; ///< generate all the parameters needed by the ocean connect server
        }

        if(0 != connect_server(cb))
        {
            conn_failed_cnt++;
            printf("bs server conn_failed_cnt:%d\r\n", conn_failed_cnt);
            continue;
        }

        ret = subscribe_topic(cb);
        if(ret == -1)
        {
            mqtt_al_disconnect(cb->mqtt_handle);
            free_mqtt_para(cb);
            cb->b_flag_rcv_snd = 0;
            continue;
        }

        mqtt_al_pubpara_t pubpara;
        memset(&pubpara, 0, sizeof(pubpara));
        pubpara.qos = 1;
        pubpara.retain = 0;
        pubpara.timeout = 1000;
        pubpara.topic.data = cb->mqtt_pubtopic;
        pubpara.topic.len =strlen(cb->mqtt_pubtopic);
        pubpara.msg.data = '\0';
        pubpara.msg.len = 0;

        ret = mqtt_al_publish(cb->mqtt_handle, &pubpara);
        if(ret == -1)
        {
            mqtt_al_disconnect(cb->mqtt_handle);
            free_mqtt_para(cb);
            cb->b_flag_rcv_snd = 0;
            continue;
        }

        s_oc_agent_cb = cb;
        cb->b_flag_rcv_snd = 0;
        cb->b_flag_stop = 1;
    }

    return ret;
}

static int bs_msg_deal(void *handle,mqtt_al_msgrcv_t *msg)
{
    int ret = -1;
    struct hostent* entry = NULL;
    cJSON  *buf = NULL;
    cJSON  *address = NULL;
    char   *port = NULL;
    char   dnsbuf[64];

    memset(dnsbuf,0,sizeof(dnsbuf));
    printf("bs topic:%s qos:%d\n\r",msg->topic.data,msg->qos);

    if(msg->msg.len < cn_bs_rcv_buf_len)
    {
        memcpy(s_rcv_buffer,msg->msg.data,msg->msg.len );
        s_rcv_buffer[msg->msg.len] = '\0'; ///< the json need it
        s_rcv_datalen = msg->msg.len;

        printf("msg:%s\n\r",s_rcv_buffer);
        buf = cJSON_Parse(s_rcv_buffer);
        cJSON_Print(buf);

        if(NULL != buf)
        {
            address = cJSON_GetObjectItem(buf,"address");
            if(NULL != address)
            {
                printf("address:%s\n\r", address->valuestring);
            }
            else
                return ret;

            if(NULL != address)
            {
                port = strrchr(address->valuestring, ':');
                memcpy(iot_server_port, port+1, strlen(port)-1);
                memcpy(dnsbuf, address->valuestring, port - address->valuestring);
                printf("dns:%s len:%d\n\r",dnsbuf,strlen(dnsbuf));

                entry = NULL;
                while(NULL == entry)
                {
                    entry = sal_gethostbyname(dnsbuf);

                    osal_task_sleep(1000);
                    printf("LOG:########## WAIT FOR THE DNS RESOVLED\n\r");
                }

                if(entry && entry->h_addr_list[0])
                {
                    inet_ntop(entry->h_addrtype, entry->h_addr_list[0], iot_server_ip, sizeof(iot_server_ip));
                    printf("iot server ip:%s\r\n", iot_server_ip);
                }
            }

            cJSON_Delete(buf);
        }

        osal_semp_post(s_agent_sync);
        ret = 0;

    }
    return ret;
}


static void *__oc_config(tag_oc_mqtt_config *config)
{
    tag_oc_mqtt_agent_cb *ret = NULL;
    osal_semp_create(&s_agent_sync,1,0);

    ret = osal_zalloc(sizeof(tag_oc_mqtt_agent_cb));
    if(NULL == ret)
    {
        goto EXIT_CB_MEM;
    }

    if(0 != check_clone_config_params(ret,config))
    {
        goto EXIT_CHECK_CLONE;
    }

    if(config->boot_mode == en_oc_boot_strap_mode_client_initialize)
    {
        ret->config.boot_status = en_oc_boot_strap_status_bs;
        ret->config.bsinfo_dealer = bs_msg_deal;
        if(NULL == osal_task_create("oc_mqtt_bs",__oc_bs_engine,ret,0x1400,NULL,6))
        {
            goto EXIT_ENGINE_CREATE;
        }
    }
    else
    {
        osal_semp_post(s_agent_sync);
    }

    while(1)
    {
        if(osal_semp_pend(s_agent_sync,cn_osal_timeout_forever))
        {
            printf("create agent engine\r\n");
            if(s_oc_agent_cb && s_oc_agent_cb->mqtt_handle)
                mqtt_al_disconnect(s_oc_agent_cb->mqtt_handle);

            if(config->boot_mode == en_oc_boot_strap_mode_client_initialize)
            {
                ret->config.port = iot_server_port;
                ret->config.server = iot_server_ip;
                ret->b_flag_stop = 0;
            }
            ret->config.boot_status = en_oc_boot_strap_status_hub;
            if(NULL == osal_task_create("oc_mqtt_agent",__oc_agent_engine,ret,0x1400,NULL,6))
            {
                goto EXIT_ENGINE_CREATE;
            }
            break;
        }
    }
    return ret;


EXIT_ENGINE_CREATE:
EXIT_CHECK_CLONE:
    osal_free(ret);
    ret = NULL;
EXIT_CB_MEM:
    return ret;
}
static int __oc_report(void *handle,char *msg, int msg_len,en_mqtt_al_qos_t qos)
{
    int ret = -1;
    size_t payloadlen;
    mqtt_al_pubpara_t  pubpara;

    tag_oc_mqtt_agent_cb *cb = handle;

    if ((cb == NULL) || (qos >= en_mqtt_al_qos_err))
    {
        return ret;
    }
    if (msg == NULL || msg_len <= 0)
    {
        return ret;
    }

    if (!(cb->b_flag_rcv_snd &&(en_mqtt_al_connect_ok == mqtt_al_check_status(cb->mqtt_handle))))
    {
        return ret;
    }

    payloadlen = msg_len;
    memset(&pubpara, 0, sizeof(pubpara));

    pubpara.qos = qos ;
    pubpara.retain = 0;
    pubpara.timeout = 1000;
    pubpara.topic.data = cb->mqtt_pubtopic;
    pubpara.topic.len =strlen(cb->mqtt_pubtopic);
    pubpara.msg.data =(char *)msg;
    pubpara.msg.len = payloadlen;

    ret = mqtt_al_publish(cb->mqtt_handle,&pubpara);

    return ret;
}

static int __oc_deconfig(void *handle)
{
    int ret =-1;
    tag_oc_mqtt_agent_cb *cb = handle;

    if(NULL != cb)
    {
        cb->b_flag_stop =1;
        ret = 0;
    }

    return ret;
}

static const tag_oc_mqtt_ops s_oc_mqtt_ops_agent= \
{
    .config = __oc_config,
    .deconfig =__oc_deconfig,
    .report = __oc_report,
};

int oc_mqtt_install_atiny_mqtt()
{
    int ret;

    ret = oc_mqtt_register(&s_oc_mqtt_ops_agent);

    return ret;
}


