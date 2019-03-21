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

#ifndef LITEOS_MQTT_AL_H
#define LITEOS_MQTT_AL_H

#include <stdint.h>

#define WITH_MQTT    1


#if defined(__cplusplus)
extern "C" {
#endif

/** brief defines for all the ascii or data used in the mqtt engine */
typedef struct
{
	char *data;      ///< buffer to storage the data
	int   len;       ///< buffer data length
}mqtt_al_string_t;   //used to represent any type string (maybe not ascii)

/** @brief  this enum all the transport encode we support now*/
typedef enum
{
	en_mqtt_al_security_none = 0,    ///< no encode
	en_mqtt_al_security_psk,         ///< use the psk mode in transport layer
	en_mqtt_al_security_cas,	     ///< use the ca mode in transport layer,only check the server
	en_mqtt_al_security_cacs,	     ///< use the ca mode in transport layer,both check the server and client
	en_mqtt_al_security_end,         ///< the end for the mqtt
}en_mqtt_al_security_t;

/** @brief this data defines for the psk mode*/
typedef struct
{
	mqtt_al_string_t  psk_id;   ///< the psk id
	mqtt_al_string_t  psk_key;  ///< the psk key
}mqtt_al_security_psk_t;

/** @brief this data defines for the cas mode:only check the server  */
typedef struct
{
    mqtt_al_string_t  ca_crt;  ///<  the ca crt
}mqtt_al_security_cas_t;


/** @brief this data defines for the cacs mode:both check the server and client */
typedef struct
{
	mqtt_al_string_t ca_s_crt;     ///< the server crt
	mqtt_al_string_t ca_c_crt;     ///< the client crt
	mqtt_al_string_t ca_c_key;     ///< the client ca key
	mqtt_al_string_t ca_host;      ///< for peer certificate, maybe they need this
}mqtt_al_security_cacs_t;

/** @brief this data defines for the encode parameter for the connect */
typedef struct
{
	en_mqtt_al_security_t type;         ///< which security type of the data
    union
    {
		mqtt_al_security_psk_t   psk;   ///< psk data  if the type is en_mqtt_security_psk
		mqtt_al_security_cas_t   cas;   ///< cas data  if the type is en_mqtt_security_cas
		mqtt_al_security_cacs_t  cacs;  ///< cacs data if the type is en_mqtt_security_cacs
    }u;
}mqtt_al_security_para_t;

/** @brief enum all the qos supported for the application */
typedef enum
{
	en_mqtt_al_qos_0 = 0,     ///< mqtt QOS 0
	en_mqtt_al_qos_1,         ///< mqtt QOS 1
	en_mqtt_al_qos_2,         ///< mqtt QOS 2
	en_mqtt_al_qos_err
}en_mqtt_al_qos_t;

/** @brief defines the parameter used for the will message used in connect */
typedef struct
{
	mqtt_al_string_t  topic;    ///< will topic
	mqtt_al_string_t  msg;      ///< will message
	en_mqtt_al_qos_t  qos;      ///< will message qos
	int               retain;   ///< will message retain or not:1 retain while 0 not
}mqtt_al_willmsg_t;

/** @brief enum the mqtt version*/
typedef enum
{
	en_mqtt_al_version_3_1_0 = 0,
	en_mqtt_al_version_3_1_1,
}en_mqtt_al_verison;

/** @brief defines the paramter for the mqtt connect */
typedef struct
{
	mqtt_al_string_t               serveraddr;   ///< mqtt server:support domain name and dot format
	int                            serverport;   ///< mqtt server port
	mqtt_al_security_para_t       *security;     ///< if NULL,will use en_mqtt_security_none
	en_mqtt_al_verison             version;      ///< mqtt version will be used
	mqtt_al_string_t               clientid;     ///< mqtt connect client identifier
	mqtt_al_string_t               user;         ///< mqtt connect user
	mqtt_al_string_t               passwd;       ///< mqtt connect passwd
	int                            cleansession; ///< 1 clean the session while 0 not
	mqtt_al_willmsg_t             *willmsg;      ///< mqtt connect will message
	unsigned short                 keepalivetime;///< keep alive time
	int                            conret;       ///< mqtt connect code, return by server
	int                            timeout;      ///< how much time will be blocked
}mqtt_al_conpara_t;

/** @brief defines for the mqtt publish */
typedef struct
{
	mqtt_al_string_t    topic;    ///< selected publish topic
	mqtt_al_string_t    msg;      ///< message to be published
	en_mqtt_al_qos_t    qos;      ///< message qos
	int                 retain;   ///< message retain :1 retain while 0 not
	int                 timeout;  ///< how much time will blocked
}mqtt_al_pubpara_t;


/** @brief  defines for the message will be passed to the application dealer*/
typedef struct
{
	mqtt_al_string_t       topic; ///< message topic
	mqtt_al_string_t       msg;   ///< the topic message payload
	en_mqtt_al_qos_t       qos;   ///< message qos
	int                    dup;   ///< message dup times
	int                    retain;///< retain or not
}mqtt_al_msgrcv_t;

/** @brief  defines the mqtt received message dealer, called by mqtt engine*/
typedef void (*fn_msg_dealer)(void *arg,mqtt_al_msgrcv_t *msg);

/** @brief defines the mqtt subscribe parameter*/
typedef struct
{
	mqtt_al_string_t       topic;     ///< topic will be subscribe
	en_mqtt_al_qos_t       qos;       ///< qos requested
	fn_msg_dealer          dealer;    ///< message dealer:used to deal the received message
	void                  *arg;       ///< used for the message dealer
	int                    subret;    ///< subscribe result code
	int                    timeout;   ///< how much time will be blocked
}mqtt_al_subpara_t;

/** @brief defines the mqtt unsubscribe parameter*/
typedef struct
{
	mqtt_al_string_t       topic;    ///< the unsubscribe topic
}mqtt_al_unsubpara_t;

/** @brief  descript the mqtt engine status */
typedef enum
{
	en_mqtt_al_connect_ok     = 0,      ///< which means the mqtt is ok and could supply the service
	en_mqtt_al_connect_err,             ///< the mqtt no start or connect err
}en_mqtt_al_connect_state;

/** @brief defines for the mqtt connect code returned by the server */
#define cn_mqtt_al_con_code_ok                0   ///< has been accepted by the server
#define cn_mqtt_al_con_code_err_version       1   ///< server not support the version
#define cn_mqtt_al_con_code_err_clientID      2   ///< client identifier is error
#define cn_mqtt_al_con_code_err_netrefuse     3   ///< server service not ready yet
#define cn_mqtt_al_con_code_err_u_p           4   ///< bad user name or password
#define cn_mqtt_al_con_code_err_auth          5   ///< the client is not authorized
#define cn_mqtt_al_con_code_err_unkown        -1  ///< unknown reason


/** @brief defines for the mqtt subscribe code returned by the server */
#define cn_mqtt_al_sub_code_qos0      0     ///< success and max qos is QOS 0
#define cn_mqtt_al_sub_code_qos1      1     ///< success and max qos is QOS 1
#define cn_mqtt_al_sub_code_qos2      2     ///< success and max qos is QOS 2
#define cn_mqtt_al_sub_code_err       0x80  ///< subscribe failure

/**
 *@brief defines the operation will be registered  to the mqtt service
 *@brief parameter and return code refere to the corresponding api
 *
 */
typedef struct
{
	///< the mqtt init function
	int  (*  init)      (void);
	///< the mqtt deinit function
	int   (* deinit)    (void );
	///< connect to the server
	void* (* connect)   (mqtt_al_conpara_t *param);
	///< disconnect from the server
	int    (* disconnect)(void *handle );
	///< publish a message to the server
	int	   (* publish)    (void *handle, mqtt_al_pubpara_t *msg);
	///< subscribe a topic to the server
	int    (* subscribe)   (void *handle, mqtt_al_subpara_t *subpara);
	///< unsubscribe a topic to the server
	int    (* unsubscribe) (void *handle, mqtt_al_unsubpara_t *unsubpara);
	///< check the mqtt engine status
	en_mqtt_al_connect_state (* check_status) (void *handle);

}mqtt_al_op_t;


#ifdef WITH_MQTT
//////////////////////API USED FOR THE MQTT APPLICAITON/////////////////////////

/**
 * @brief maybe the mqtt lib need do some initialize
 *
 * @return 0 success while -1 failed
 *
 */
int mqtt_al_init();

/**
 * @brief     :when you don't want to use the mqtt service ,please call this function
 *
 * @return 0 success while -1 failed
 */
int  mqtt_al_deinit();
/**
 *@brief: you could use this function to connect to the mqtt server
 *
 *@param[in] conparam  the parameter we will use in connect, refer to the data mqtt_al_conpara_t
 *@
 *@return: first you should check the return value then the return code in conparam
 *
 *@retval NULL which means you could not get the connect to the server,maybe network reason
 *@retval handle, which means you get the context, please check the conparam for more
 */
void * mqtt_al_connect( mqtt_al_conpara_t *conparam);

/**
 * @brief  : you could use this function to disconnect from the mqtt server
 *
 * @param[in] handle  the handle we get from mqtt_al_connect

 * @return  0 success -1 failed

 */
int mqtt_al_disconnect(void *handle);


/**
 * @brief you could use this function to publish a message to the server
 *
 * @param[in] handle the handle we get from mqtt_al_connect
 *
 * @param[in] msg  the message we will publish, see the data mqtt_al_pubpara_t
 *
 * @return 0 success  -1  failed
 *
 */
int	mqtt_al_publish(void *handle, mqtt_al_pubpara_t *pubpara);

/**
 * @brief you could use this function subscribe a topic from the server
 *
 * @param[in] handle the handle we get from mqtt_al_connect
 *
 * @param[in] subpara  refer to the data mqtt_al_subpara_t
 *
 * @return 0 success  -1  failed
 *
 */
int mqtt_al_subscribe(void *handle, mqtt_al_subpara_t *subpara);

/**
 * @brief you could use this function unsubscribe a topic from the server
 *
 * @param[in] handle the handle we get from mqtt_al_connect
 *
 * @param[in] unsubpara  refer to the data mqtt_al_unsubpara_t
 *
 * @return 0 success  -1  failed
 *
 */
int mqtt_al_unsubscribe(void *handle, mqtt_al_unsubpara_t *unsubpara);

/**
 *  @brief the mqtt engine state now
 *
 *	@param[in]  handle the handle we get from mqtt_al_connect
 *
 *  @return refer to  en_mqtt_al_connect_state
 */
en_mqtt_al_connect_state mqtt_al_check_status(void *handle);

//////////////////////API USED FOR THE MQTT IMPLEMENT/////////////////////////

/**
 *@brief the mqtt lib should call this function to register its implement as a service
 *
 *@param[in] op  refer to the data  mqtt_al_op_t
 *
 *@return 0 success while -1 failed
 *
 */
int mqtt_al_install(mqtt_al_op_t *op);
/**
 *@brief call this function to unregister the mqtt service
 *
 *@param[in] op  refer to the data  mqtt_al_op_t
 *
 *@return 0 success while -1 failed
 *
 */
int mqtt_al_uninstall();

#else

#define mqtt_al_init()                         -1          ///< function compatible
#define mqtt_al_deinit()                       -1 		   ///< function compatible
#define mqtt_al_connect(handle, para)          NULL        ///< function compatible
#define mqtt_al_disconnect(handle)             -1          ///< function compatible
#define mqtt_al_publish(handle, msg)           -1          ///< function compatible
#define mqtt_al_subscribe(handle, subpara)     -1          ///< function compatible
#define mqtt_al_unsubscribe(handle,unsubpara)  -1          ///< function compatible
#define mqtt_al_check_status(handle)           en_mqtt_al_connect_err ///< function compatible
#define mqtt_al_install(opt)                   -1          ///< function compatible
#define mqtt_al_uninstall()                    -1          ///< function compatible

#endif



#if defined(__cplusplus)
}
#endif

#endif /* LITEOS_MQTT_AL_H */
