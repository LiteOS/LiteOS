/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  @file  ti/net/mqtt/mqttclient.h
 *
 *  @addtogroup ti_net_mqtt_MQTTClient MQTT Client
 *
 *  @brief      The SimpleLink MQTTClient Module provides an easy-to-use
 *              API to enable constrained and deeply embedded
 *              micro-controller based products to interact with cloud or
 *              network based MQTT servers. The users of MQTTClient services,
 *              while benefiting from the abstraction of the MQTT protocol,
 *              will find them suitable for varied deployments of MQTT
 *              subscribers and/or publishers.

 * \section descrypt_sec Description

 The SL MQTT Client Layer, in addition to providing services to the application,
 encompasses a RTOS task to handle the incoming messages from the server. Such
 a dedicated context to process the messages from the server facilitates the
 apps to receive data (i.e. PUBLISH messages) even when they are blocked, whilst
 awaiting ACK for a previous transaction with the server. The receive task in
 the SL MQTT Layer can not be disabled anytime, however its system wide priority
 is configurable and can be set.

 Some of the salient features of the SL MQTT Layer are

 - Easy-to-use, intuitive and small set of MQTT API
 - App can indicate its choice to await ACK for a message transaction
 - Supports MQTT 3.1 protocol

 * \section config_sec Configuration
 The SL implementation enables the application developers to configure the
 following parameters using the compile line flags (-D option)
 * - <b> CFG_SL_CL_MAX_MQP: </b> the count of TX + RX resources in the buffer pool
 for the library to use. \n\n
 * - <b> CFG_SL_CL_BUF_LEN: </b> the length of the TX and RX buffers in the pool. \n\n
 * - <b> CFG_SL_CL_STACK: </b> Max stack (bytes) for RX Task executed by SL-MQTT. \n\n
 * - <b> CFG_CL_MQTT_CTXS: </b> the max number of simultaneous network connections
 to one or more servers. \n\n

 * \note An app that has chosen not to await an ACK from the server for an
 scheduled transaction can benefit from the availability of control to
 pursue other activities to make overall progress in the system. However,
 an attempt to schedule another transaction with the server, while the
 previous one is still active, will depend on the availability of buffers for transaction.


 \subsection seq_subsec Typical Sequences:

 - Publishers:  INIT --> CONTEXT_CREATE --> CONNECT --> PUBLISH (TX) --> DISCONNECT --> CONTEXT_DELETE --> EXIT
 - Subscribers: INIT --> CONTEXT_CREATE --> CONNECT --> SUBSCRIBE --> PUBLISH (RX) --> UNSUBSCRIBE --> DISCONNECT --> CONTEXT_DELETE --> EXIT

 *  ## Library Usage ##
 *
 *  To use the MQTTClient APIs, the application should include its header file
 *  as follows:
 *  @code
 *  #include <ti/net/mqtt/mqttclient.h>
 *  @endcode
 *
 *  And, add the following MQTT library to the link line:
 *  @code
 *  .../source/ti/net/mqtt/{toolchain}/{isa}/mqtt_{profile}.a
 *  @endcode
 */

#ifndef ti_net_mqtt_MQTTClient__include
#define ti_net_mqtt_MQTTClient__include

/*! @ingroup ti_net_mqtt_MQTTClient */
/*@{*/

//*****************************************************************************
// includes
//*****************************************************************************
#include <string.h>
#include <stdbool.h>
#include <ti/net/mqtt/common/mqtt_common.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************

#define MQTTCLIENT_ERR_NETWORK     MQTT_PACKET_ERR_NETWORK  /**< Problem in network (sock err) */
#define MQTTCLIENT_ERR_TIMEOUT     MQTT_PACKET_ERR_TIMEOUT  /**< Net transaction has timed out */
#define MQTTCLIENT_ERR_NET_OPS     MQTT_PACKET_ERR_NET_OPS  /**< Platform Net Ops un-available */
#define MQTTCLIENT_ERR_FNPARAM     MQTT_PACKET_ERR_FNPARAM  /**< Invalid parameter(s) provided */
#define MQTTCLIENT_ERR_PKT_AVL     MQTT_PACKET_ERR_PKT_AVL  /**< No pkts are available in pool */
#define MQTTCLIENT_ERR_PKT_LEN     MQTT_PACKET_ERR_PKT_LEN  /**< Inadequate free buffer in pkt */
#define MQTTCLIENT_ERR_NOTCONN     MQTT_PACKET_ERR_NOTCONN  /**< Lib isn't CONNECTED to server */
#define MQTTCLIENT_ERR_BADCALL     MQTT_PACKET_ERR_BADCALL  /**< Irrelevant call for LIB state */
#define MQTTCLIENT_ERR_CONTENT     MQTT_PACKET_ERR_CONTENT  /**< MSG / Data content has errors */
#define MQTTCLIENT_ERR_LIBQUIT     MQTT_PACKET_ERR_LIBQUIT  /**< Needs reboot library has quit */
#define MQTTCLIENT_ERR_REMLSTN     MQTT_PACKET_ERR_REMLSTN  /**< No remote listener for socket */
#define MQTTCLIENT_OPERATION_CONNACK       MQTT_CONNACK     /**< CONNACK has been received from the server  */
#define MQTTCLIENT_OPERATION_EVT_PUBACK    MQTT_PUBACK      /**< PUBACK has been received from the server   */
#define MQTTCLIENT_OPERATION_PUBCOMP       MQTT_PUBCOMP     /**< PUBCOMP has been received from the server  */
#define MQTTCLIENT_OPERATION_SUBACK        MQTT_SUBACK      /**< SUBACK has been received from the server   */
#define MQTTCLIENT_OPERATION_UNSUBACK      MQTT_UNSUBACK    /**< UNSUBACK has been received from the server */

#define MQTTCLIENT_NETCONN_IP4     MQTT_DEV_NETCONN_OPT_IP4   /**< Assert for IPv4 connection                  */
#define MQTTCLIENT_NETCONN_IP6     MQTT_DEV_NETCONN_OPT_IP6   /**< Assert for IPv6 connection, otherwise  IPv4 */
#define MQTTCLIENT_NETCONN_URL     MQTT_DEV_NETCONN_OPT_URL   /**< Server address is an URL and not IP address */
#define MQTTCLIENT_NETCONN_SEC     MQTT_DEV_NETCONN_OPT_SEC   /**< Connection to server  must  be secure (TLS) */

#define MQTTCLIENT_NETCONN_SKIP_DOMAIN_NAME_VERIFICATION         MQTT_DEV_NETCONN_OPT_SKIP_DOMAIN_NAME_VERIFICATION          /**< Assert to skip domain name verification         */
#define MQTTCLIENT_NETCONN_SKIP_CERTIFICATE_CATALOG_VERIFICATION MQTT_DEV_NETCONN_OPT_SKIP_CERTIFICATE_CATALOG_VERIFICATION  /**< Assert to skip certificate catalog verification */
#define MQTTCLIENT_NETCONN_SKIP_DATE_VERIFICATION                MQTT_DEV_NETCONN_OPT_SKIP_DATE_VERIFICATION                 /**< Assert to skip date verification                */
	 /** @} */

//*****************************************************************************
// typedefs
//*****************************************************************************

typedef enum
{
    MQTTClient_USER_NAME      = 0,
    MQTTClient_PASSWORD       = 1,
    MQTTClient_WILL_PARAM     = 2,
    MQTTClient_CALLBACKS      = 3,
    MQTTClient_KEEPALIVE_TIME = 4,
    MQTTClient_CLEAN_CONNECT  = 5,
    MQTTClient_MAX_PARAM      = 6
} MQTTClient_Option;

/* callbacks   */

typedef enum
{
    MQTTClient_RECV_CB_EVENT       = 0,
    MQTTClient_OPERATION_CB_EVENT  = 1,
    MQTTClient_DISCONNECT_CB_EVENT = 2
} MQTTClient_EventCB;

typedef struct MQTTClient_OperationMetaDataCB
{
    uint32_t messageType;
} MQTTClient_OperationMetaDataCB;

typedef struct MQTTClient_RecvMetaDataCB
{
    const char *topic;
    int32_t topLen;
    bool dup;
    uint8_t qos;
    bool retain;
} MQTTClient_RecvMetaDataCB;


/** Secure Socket Parameters to open a secure connection */
/*
 Note: value of n_files can vary from 1 to 4, with corresponding pointers to
 the files in files field. Value of 1(n_files) will assume the corresponding
 pointer is for CA File Name. Any other value of n_files expects the files to
 be in following order:
 1.  Private Key File
 2.  Certificate File Name
 3.  CA File Name
 4.  DH Key File Name

 example:
 If you want to provide only CA File Name, following are the two way of doing it:
 for n_files = 1
 char *security_file_list[] = {"/cert/testcacert.der"};
 for n_files = 4
 char *security_file_list[] = {NULL, NULL, "/cert/testcacert.der", NULL};

 where secure_files = security_file_list
 */

typedef struct MQTTClient_ConnParams
{
    uint32_t netconnFlags; /**< Enumerate connection type  */
    const char *serverAddr; /**< Server Address: URL or IP  */
    uint16_t port; /**< Port number of MQTT server */
    uint8_t method; /**< Method to tcp secured socket */
    uint32_t cipher; /**< Cipher to tcp secured socket */
    uint32_t nFiles; /**< Number of files for secure transfer */
    char * const *secureFiles; /* SL needs 4 files*/
} MQTTClient_ConnParams;

typedef struct MQTTClient_Params
{
    /** client id */
    char *clientId;

    /**
     *  Operate in MQTT 3.1 mode:
     *    - false: 3.1.1 (default)
     *    - true - 3.1
     */
    bool mqttMode31;

    bool blockingSend;
    MQTTClient_ConnParams *connParams; // pointer to connection param

} MQTTClient_Params;

typedef void (*MQTTClient_CallBack)(int32_t event, void *metaData, uint32_t metaDateLen, void *data, uint32_t dataLen);
typedef void *MQTTClient_Handle;

typedef struct MQTTClient_SubscribeParams
{
    char *topic;
    MQTTClient_CallBack callback; // optional - if NULL use the default callback
    uint8_t qos;
    uint8_t persistent;
} MQTTClient_SubscribeParams;

typedef struct MQTTClient_UnsubscribeParams
{
    char* topic;
    uint8_t persistent;
} MQTTClient_UnsubscribeParams;

/*  Data structure which holds a will data message. */

typedef struct MQTTClient_Will
{
    const char *willTopic; /**< Will Topic   */
    const char *willMsg;   /**< Will message */
    int8_t      willQos;   /**< Will Qos     */
    bool        retain;    /**< Retain Flag  */
} MQTTClient_Will;

//*****************************************************************************
// function prototypes
//*****************************************************************************

/**
 Creates the SL MQTT Implementation.
 A caller must call this function in order the create and initialize
 the MQTT implementation prior to using its services.

 \param[in] defaultCallback async event handler
 \param[in] params          parameters

 \return Success Handle or Failure NULL
 */
MQTTClient_Handle MQTTClient_create(MQTTClient_CallBack defaultCallback,
        MQTTClient_Params *params);

/**
 This function deletes an MQTT Client instance.
 It closes the connection if it exists and releases all resources.

 \param[in]  handle to the client context

 \return Success (0) or Failure (-1)
 */
int16_t MQTTClient_delete(MQTTClient_Handle handle);

/**
 MQTT client state machine
 This function need to be called from the context
 created by the user in case of OS environment.

 \param[in]  handle to the client context

 \return Success (0) or Failure (Negative number)
 */
int16_t MQTTClient_run(MQTTClient_Handle handle);

/**
 Function connects MQTT client to a broker

 \param[in]  handle to the client context

 \return Success (0) or Failure (Negative number)
 */
int16_t MQTTClient_connect(MQTTClient_Handle handle);

/**
 DISCONNECT from the server.
 The caller must use this service to close the connection with the
 server.

 \param[in]  handle to the client context

 \return Success (0) or Failure (Negative number)
 */
int16_t MQTTClient_disconnect(MQTTClient_Handle handle);

/**
 PUBLISH a named message to the server.
 In addition to the PUBLISH specific parameters, the caller can indicate
 whether the routine should block until the time, the message has been
 acknowledged by the server. This is applicable only for non-QoS0 messages.

 In case, the app has chosen not to await for the ACK from the server,
 the SL MQTT implementation will notify the app about the subscription
 through the callback routine.

 \param[in] handle refers to the handle to the client context
 \param[in] topic  topic of the data to be published. It is NULL terminated.
 \param[in] topicLen  topic length.
 \param[in] msg   binary data to be published
 \param[in] msgLen    length of the msg
 \param[in] flags QOS define MQTT_PUBLISH_QOS_0, MQTT_PUBLISH_QOS_1 or MQTT_PUBLISH_QOS_2
 use MQTT_PUBLISH_RETAIN is message should be retained

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTClient_publish(MQTTClient_Handle handle, char *topic, uint16_t topicLen, char *msg, uint16_t msgLen, uint32_t flags);

/**
 SUBSCRIBE a set of topics.
 To receive data about a set of topics from the server, the app through
 this routine must subscribe to those topic names with the server. The
 caller can indicate whether the routine should block until a time, the
 message has been acknowledged by the server.

 In case, the app has chosen not to await for the ACK from the server,
 the SL MQTT implementation will notify the app about the subscription
 through the callback routine.

 \param[in] handle         refers to the handle to the client context
 \param[in] value          a pointer to a struct of subscribe parameters which hold: \n
                           Topic name to subscribe. It is a pointer to NULL terminated strings. \n
                           Qos value for the topic. \n
                           persistent session flag in order to enable or disable persistent session. \n
                           callback function which is optional - if NULL use the default callback \n
 \param[in] numberOfTopics number of such topics

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTClient_subscribe(MQTTClient_Handle handle, MQTTClient_SubscribeParams *value, uint8_t numberOfTopics);

/**
 UNSUBSCRIBE a set of topics.
 The app should use this service to stop receiving data for the named
 topics from the server.

 \param[in] handle         refers to the handle to the client context
 \param[in] value          topics set of topics to be unsubscribed. It is an array of
                           pointers to NULL terminated strings.
 \param[in] numberOfTopics count number of topics to be unsubscribed

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTClient_unsubscribe(MQTTClient_Handle handle, MQTTClient_UnsubscribeParams *value, uint8_t numberOfTopics);

/**
 Setting client parameters.
 This function can set different parameters to the client.
 Will message params, user name and password, keep alive time and
 clean/persistent session.

 \param[in] handle      refers to the handle to the client context
 \param[in] option      Define the actual option to set. Applicable values:
                           - #MQTTClient_USER_NAME
                           - #MQTTClient_PASSWORD
                           - #MQTTClient_WILL_PARAM
                           - #MQTTClient_KEEPALIVE_TIME
                           - #MQTTClient_CLEAN_CONNECT
 \param[in] value       Specifies a value for the option
 \param[in] valueLength Specifies the length of the value

 \return Success(0) or Failure(Negative number)

 \sa MQTTClient_Will
 */
int16_t MQTTClient_set(MQTTClient_Handle handle, uint16_t option, void *value, uint16_t valueLength);

/**
 Getting client parameters.
 This function can get different parameters from the client.
 Will message params, user name and password, keep alive time and
 clean/persistent session.

 \param[in] handle      refers to the handle to the client context
 \param[in] option      Define the actual option to set. Applicable values:
                           - #MQTTClient_USER_NAME
                           - #MQTTClient_PASSWORD
                           - #MQTTClient_WILL_PARAM
                           - #MQTTClient_KEEPALIVE_TIME
                           - #MQTTClient_CLEAN_CONNECT
 \param[in] value       Specifies a value for the option
 \param[in] valueLength Specifies the length of the value

 \return Success(0) or Failure(Negative number)

 \sa MQTTClient_Will
 */
int16_t MQTTClient_get(MQTTClient_Handle handle, uint16_t option, void *value, uint16_t valueLength);

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
