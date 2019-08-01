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


#ifndef __OC_MQTT_AL_H
#define __OC_MQTT_AL_H


#include <stddef.h>
#include <stdint.h>
#include <mqtt_al.h>

#include <cJSON.h>

typedef enum
{
    en_oc_mqtt_device_type_dynamic = 0,
    en_oc_mqtt_device_type_static,
}en_oc_mqtt_device_type;

typedef enum
{
    en_oc_mqtt_code_mode_binary = 0,   ///< the report and command is binary mode
    en_oc_mqtt_code_mode_json,         ///< the report and command is json mode
    en_oc_mqtt_code_mode_max,
}en_oc_mqtt_code_mode;

typedef enum
{
    en_mqtt_sign_type_hmacsha256_check_time_no = 0, //use HMACSHA256 to encode password but no check current time
    en_mqtt_sign_type_hmacsha256_check_time_yes   , //use HMACSHA256 to encode password and check current time
}en_oc_mqtt_sign_type;

typedef enum
{
    en_mqtt_auth_type_devid = 0,
    en_mqtt_auth_type_model = 1,
    en_mqtt_auth_type_nodeid = 2,
}en_oc_mqtt_auth_type;


/** @brief this is the message dealer module for the application*/
typedef int (*oc_mqtt_msgdealer)(void *handle,mqtt_al_msgrcv_t *msg);

/**
 * @brief: you must supply a media (nonvolatile,like the rom flash) for the dynamic mode
 *         and the media maybe 2kilo Bytes
 * */
typedef int (*secret_read) (int offset,char *buf,int len);
typedef int (*secret_write)(int offset,char *buf, int len);

typedef struct
{
    const char*  productid;
    const char*  productpasswd;
    const char*  notetid;
    secret_read  read_secret;
    secret_write write_secret;
}tag_oc_mqtt_device_dynamic;

typedef struct
{
    const char*  deviceid;
    const char*  devicepasswd;
}tag_oc_mqtt_device_static;

typedef enum
{
    en_oc_boot_strap_mode_factory = 0,
    en_oc_boot_strap_mode_client_initialize,
}en_oc_boot_strap_mode_t;

typedef enum
{
    en_oc_boot_strap_status_bs = 0,
    en_oc_boot_strap_status_hub,
    en_oc_boot_strap_status_max,
}en_oc_boot_strap_status_t;


typedef struct
{
    en_oc_boot_strap_mode_t  boot_mode;
    en_oc_boot_strap_status_t  boot_status;
    unsigned short lifetime;
    const char   *server;            ///< cdp server address
    const char   *port;              ///< cdp server port
    mqtt_al_security_para_t security;///< only support crt mode now
    en_oc_mqtt_code_mode    code_mode;   ///< cdp encode mode:now only support the json mode
    en_oc_mqtt_sign_type    sign_type;   ///< generate the passwd supported
    en_oc_mqtt_device_type  device_type;
    en_oc_mqtt_auth_type    auth_type;

    union
    {
        tag_oc_mqtt_device_dynamic d_device;
        tag_oc_mqtt_device_static  s_device;
    }device_info;

    oc_mqtt_msgdealer  msgdealer;///< when the agent receive any applciation data, please call this function
    oc_mqtt_msgdealer  bsinfo_dealer;
}tag_oc_mqtt_config;

///////////////////////////MQTT AGENT INTERFACE////////////////////////////////
typedef void* (*fn_oc_mqtt_config)(tag_oc_mqtt_config *param);                        ///< return the handle here
typedef int (*fn_oc_mqtt_deconfig)(void *handle);                                     ///< use the handle as the params
typedef int (*fn_oc_mqtt_report)(void *handle,char *msg,int len,en_mqtt_al_qos_t qos);    ///< use the handle and report params
/**
 * @brief this data structure defines the mqtt agent implement
 */
typedef struct
{
    const char         *name;     ///< this is the name for the ops
    fn_oc_mqtt_config   config;   ///< this function used for the configuration
    fn_oc_mqtt_report   report;   ///< this function used for the report data to the cdp
    fn_oc_mqtt_deconfig deconfig; ///< this function used for the deconfig
}tag_oc_mqtt_ops;

#if CONFIG_OC_MQTT_ENABLE
/**
 *@brief the mqtt agent should use this function to register the method for the application
 *
 *@param[in] opt, the operation method implement by the mqtt agent developer
 *@return 0 success while -1 failed
 */
int oc_mqtt_register(const tag_oc_mqtt_ops *opt);


//////////////////////////APPLICATION INTERFACE/////////////////////////////////

/* msg type, json name, its value is string*/
#define cn_msg_type_name "msgType"
/* msg type report data, json value */
#define cn_msg_type_device_req    "deviceReq"   ///< device report
#define cn_msg_type_cloud_req     "cloudReq"    ///< cloud command
#define cn_msg_type_device_resp   "deviceRsp"   ///< device response to the command

/* more data, json name, its value is int, 0 for no more data, 1 for more data */
#define cn_has_more_name "hasMore"
typedef enum
{
    en_oc_mqtt_has_more_no = 0,
    en_oc_mqtt_has_more_yes =1,
}en_oc_mqtt_has_more;


/* ServiceData array, json name, its value is ServiceData array */
#define cn_data_name "data"

/* ServiceData */
/* service id, json name, its value is string */
#define cn_service_id_name   "serviceId"

/* service data, json name, its value is an object defined in profile for the device */
#define cn_service_data_name "serviceData"

/* service data, json name, its value is string, format yyyyMMddTHHmmssZ such as 20161219T114920Z */
#define cn_service_time_name  "eventTime"


#define cn_cmd_name       "cmd"
#define cn_paras_name     "paras"
#define cn_mid_name       "mid"

#define cn_err_code_name  "errcode"
typedef enum
{
    en_oc_mqtt_err_code_ok = 0,
    en_oc_mqtt_err_code_err =1,
}en_oc_mqtt_err_code;

#define cn_body_name      "body"

/* deviceReq data msg jason format example to server
the message can be decoded in json or binary.
{
        "msgType":      "deviceReq",
        "hasMore":      0,
        "data": [{
                        "serviceId":    "serviceIdValue",
                        "serviceData": {
                                "defineData": "defineValue"
                        },
                        "eventTime":    "20161219T114920Z"
                }]
}
*/
/*
cloudReq data msg jason format example from server
{
    "msgType":"cloudReq",
    "serviceId":"serviceIdValue",
    "paras":{
        "paraName":"paraValue"
    },
    "cmd":"cmdValue",
    "hasMore":0,
    "mid":0
}
*/
/*
deviceRsp data msg jason format example to server
{
    "msgType":  "deviceRsp",
    "mid":  0,
    "errcode":  0,
    "hasMore":  0,
    "body": {
        "bodyParaName": "bodyParaValue"
    }
}
*/
typedef enum
{
    en_key_value_type_int = 0,
    en_key_value_type_string,
    en_key_value_type_array,
}en_value_type;

typedef struct
{
    char           *name;    ///< key name
    char           *buf;     ///< used to storage the key value
    int             len;     ///< how long the key value
    en_value_type   type;    ///< the value type
}tag_key_value;

typedef struct
{
    void            *next;
    tag_key_value    item;
}tag_key_value_list;

/**
 *
 * @brief:this data structure used for the message send and message receive
 *
 * */
typedef struct
{
    char                  *serviceid;
    tag_key_value_list    *paralst;
    char                  *eventtime;
    en_oc_mqtt_has_more    hasmore;
}tag_oc_mqtt_report;

///< use this function to format the message to json mode
cJSON *oc_mqtt_json_fmt_report(tag_oc_mqtt_report  *report);

/**
 * @brief: this data structure used for the command response
 *         when you receive a command, you should do the response
 * */
typedef struct
{

    int                     mid;     ///< should get from the command message
    en_oc_mqtt_err_code     errcode; ///< error code
    en_oc_mqtt_has_more     hasmore; ///< has more or not
    tag_key_value_list     *bodylst; ///< response body
}tag_oc_mqtt_response;
///< use this function to format the response message to json mode
cJSON *oc_mqtt_json_fmt_response(tag_oc_mqtt_response *response);

/**
 * @brief the application use this function to configure the lwm2m agent
 * @param[in] param, refer to tag_oc_mqtt_config
 * @return oc mqtt handle else NULL failed
 */
void *oc_mqtt_config(tag_oc_mqtt_config *param);
/**
 * @brief the application use this function to send the message to the cdp
 * @param[in] hanlde:the mqtt handle returned by oc_mqtt_config
 * @param[in] report:the message to report
 *
 * @return 0 success while <0 failed
 */
int oc_mqtt_report(void *handle,char *msg,int len, en_mqtt_al_qos_t qos);

/**
 *@brief: the application use this function to deconfigure the lwm2m agent
 *
 *@param[in] handle: returned by oc_mqtt_config
 *
 * return 0 success while <0 failed
 */

int oc_mqtt_deconfig(void *handle);

/**
 *@brief this is the oc mqtt  initialize function,must be called first
 *
 *@return 0 success while <0 failed
 */
int oc_mqtt_init();


#else   //not configure the lwm2m agent

#define oc_mqtt_register(opt)                                              -1

#define oc_mqtt_config(param)                                              NULL
#define oc_mqtt_deconfig(handle)                                           -1
#define oc_mqtt_json_fmt_report(report)                                    NULL
#define oc_mqtt_json_fmt_response(response)                                NULL

#define oc_mqtt_report(handle,report)                                      -1
#define oc_mqtt_init()                                                     -1

#endif

#endif /* __OC_MQTT_AL_H */
