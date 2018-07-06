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

#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include "mqtt_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_WILL_FLAG_TRUE (1)
#define MQTT_WILL_FLAG_FALSE (0)
#define MQTT_RETAIN_FLAG_TRUE (1)
#define MQTT_RETAIN_FLAG_FALSE (0)

typedef enum
{
    ATINY_OK                   = 0,
    ATINY_ARG_INVALID          = -1,
    ATINY_BUF_OVERFLOW         = -2,
    ATINY_MSG_CONGEST          = -3,
    ATINY_MALLOC_FAILED        = -4,
    ATINY_RESOURCE_NOT_FOUND   = -5,
    ATINY_RESOURCE_NOT_ENOUGH  = -6,
    ATINY_CLIENT_UNREGISTERED  = -7,
    ATINY_SOCKET_CREATE_FAILED = -8,
    ATINY_SOCKET_ERROR = -9,
} atiny_error_e;

typedef enum cloud_security_type
{
    CLOUD_SECURITY_TYPE_NONE,
    CLOUD_SECURITY_TYPE_PSK,
    CLOUD_SECURITY_TYPE_CA,
    CLOUD_SECURITY_TYPE_MAX
}cloud_security_type_e;

typedef enum cloud_qos_level
{
    CLOUD_QOS_MOST_ONCE,    // LWM2M: NON
    CLOUD_QOS_LEAST_ONCE,   // LWM2M: CON
    CLOUD_QOS_ONLY_ONCE,
    CLOUD_QOS_LEVEL_MAX
}cloud_qos_level_e;

typedef enum cloud_method
{
    CLOUD_METHOD_GET,    // MQTT: subscribe
    CLOUD_METHOD_POST,  // MQTT: publish
    CLOUD_METHOD_DEL,    // MQTT: unsubscribe
    CLOUD_METHOD_MAX
}cloud_method_e;

typedef struct cloud_will_options
{
    char *topic_name;
    char *topic_msg;
    char retained;
    cloud_qos_level_e qos;
}cloud_will_options_t;

typedef struct cloud_msg
{
    char *uri;
    int uri_len;
    cloud_qos_level_e qos;
    cloud_method_e method;
    unsigned int payload_len;
    void *payload;
}cloud_msg_t;

typedef void (*atiny_rsp_cb)(cloud_msg_t *msg);

typedef struct atiny_interest_uri
{
    char *uri;
    cloud_qos_level_e qos;
    atiny_rsp_cb cb;
}atiny_interest_uri_t;

typedef struct atiny_device_info
{
    char *client_id;
    char *user_name;
    char *password;
    char will_flag;
    cloud_will_options_t *will_options;
    atiny_interest_uri_t interest_uris[ATINY_INTEREST_URI_MAX_NUM];
}atiny_device_info_t;

typedef struct cloud_security_psk
{
    unsigned char *psk_id;
    int psk_id_len;
    unsigned char *psk;
    int psk_len;
}cloud_security_psk_t;

typedef struct cloud_security_ca
{
    char *ca_crt;
    char *server_crt;
    char *server_key;
}cloud_security_ca_t;

typedef struct atiny_param
{
    char *server_ip;
    char *server_port;
    cloud_security_type_e security_type;
    union
    {
        cloud_security_psk_t psk;
        cloud_security_ca_t ca;
    }u;
}atiny_param_t;

int  atiny_init(atiny_param_t* atiny_params, void** phandle);
void atiny_deinit(void* phandle);
int atiny_isconnected(void* phandle);
int atiny_bind(atiny_device_info_t* device_info, void* phandle);
int atiny_data_send(void* phandle, cloud_msg_t* send_data, atiny_rsp_cb cb);

#ifdef __cplusplus
}
#endif

#endif /* __MQTT_CLIENT_H__ */

