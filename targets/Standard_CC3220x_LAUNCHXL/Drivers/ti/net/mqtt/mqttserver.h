/*
 * Copyright (c) 2016-2018, Texas Instruments Incorporated
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
 *  @file  ti/net/mqtt/mqttserver.h
 *
 *  @addtogroup ti_net_mqtt_MQTTServer MQTT Server
 *

 *  ## Library Usage ##
 *
 *  To use the MQTTServer APIs, the application should include its header file
 *  as follows:
 *  @code
 *  #include <ti/net/mqtt/mqttserver.h>
 *  @endcode
 *
 *  And, add the following MQTT library to the link line:
 *  @code
 *  .../source/ti/net/mqtt/{toolchain}/{isa}/mqtt_{profile}.a
 *  @endcode
 */

#ifndef ti_net_mqtt_MQTTServer__include
#define ti_net_mqtt_MQTTServer__include

/*! @ingroup ti_net_mqtt_MQTTServer */
/*@{*/

//*****************************************************************************
// includes
//*****************************************************************************
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
// typedefs
//*****************************************************************************
typedef enum
{
    MQTTServer_USER_NAME = 0,
    MQTTServer_PASSWORD  = 1,
} MQTTServer_Option;

typedef void (*MQTTServer_CallBack)(int32_t event, void *metaData, uint32_t metaDateLen, void *data, uint32_t dataLen);
typedef void *MQTTServer_Handle;

typedef struct _MQTTServer_SubscribeParams_
{
    char *topic;
    MQTTServer_CallBack callback; /**< optional - if NULL use the default callback */
    uint8_t persistent;
} MQTTServer_SubscribeParams;

typedef struct _MQTTServer_UnSubscribeParams_
{
    char *topic;
    uint8_t persistent;
} MQTTServer_UnSubscribeParams;


typedef enum
{
    MQTTServer_RECV_CB_EVENT       = 0,
    MQTTServer_CONNECT_CB_EVENT    = 1,
    MQTTServer_DISCONNECT_CB_EVENT = 2
} MQTTServer_EventCB;

/* Structure that holds parameters on the server for */
/* Authentication of Clients by the Server           */
typedef struct _MQTTServer_LocalAuthenticationData_
{
    char *userName;
    char *password;
} MQTTServer_LocalAuthenticationData;

typedef struct _MQTTServer_ConnectMetaDataCB_
{
    char *clientId;
    int32_t clientIdLen;
    char *userName;
    int32_t usernameLen;
    char *password;
    int32_t passwordLen;
    void **usr;
} MQTTServer_ConnectMetaDataCB;

typedef struct _MQTTServer_RecvMetaDataCB_
{
    const char *topic;
    int32_t topLen;
    bool dup;
    uint8_t qos;
    bool retain;
} MQTTServer_RecvMetaDataCB;

typedef struct _MQTTServer_DisconnectMetaDataCB_
{
    char *user;
    bool dueToErr;
} MQTTServer_DisconnectMetaDataCB;

/** Secure Socket Parameters to open a secure connection */
typedef struct _MQTTServer_ConnParams_
{
    uint16_t port;             /**< Port number of MQTT server          */
    uint8_t method;            /**< Method to tcp secured socket        */
    uint32_t cipher;           /**< Cipher to tcp secured socket        */
    uint32_t nFiles;           /**< Number of files for secure transfer */
    char * const *secureFiles; /**< SL needs 4 files                    */
} MQTTServer_ConnParams;

typedef struct
{
    MQTTServer_ConnParams *connParams; // pointer to connection param
} MQTTServer_Params;


//*****************************************************************************
// function prototypes
//*****************************************************************************

/**
 Delete the MQTTServer

 Disconnect and remove all MQTT Server connections and data.

 \param[in]  handle Instance of the MQTTServer

 \return 0 on success or error code on failure.

 \sa MQTTServer_create()
 */
int16_t MQTTServer_delete(MQTTServer_Handle handle);

/**
 Create an MQTTServer instance

 A caller must initialize the MQTT Server implementation prior to
 using its services.

 \param[in]  attrib          parameters
 \param[in]  defaultCallback async event handler

 \return Success Handle or Failure NULL

 \sa MQTTServer_delete()
 */
MQTTServer_Handle MQTTServer_create(MQTTServer_CallBack defaultCallback,
        MQTTServer_Params *attrib);

/**
 Start an MQTTServer instance

 Main Task loop function that will start the MQTT server
 Connect and wait for data received

 \param[in]  handle  MQTTServer instance handle

 \return Success (0) or Failure (Negative number)

 \sa MQTTServer_create()
 */
int16_t MQTTServer_run(MQTTServer_Handle handle);

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
                           Topic name to subscribe. It is a pointer to NUL terminated strings. \n
                           Qos value for the topic. \n
                           persistent session flag in order to enable or disable persistent session. \n
                           callback function which is optional - if NULL use the default callback \n
 \param[in] numberOfTopics number of such topics

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTServer_subscribe(MQTTServer_Handle handle, MQTTServer_SubscribeParams *value, uint8_t numberOfTopics);

/**
 UNSUBSCRIBE a set of topics.
 The app should use this service to stop receiving data for the named
 topics from the server.

 \param[in] handle         refers to the handle to the client context
 \param[in] value          topics set of topics to be unsubscribed. It is an array of
                           pointers to NUL terminated strings.
 \param[in] numberOfTopics count number of topics to be unsubscribed

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTServer_unsubscribe(MQTTServer_Handle handle, MQTTServer_UnSubscribeParams *value, uint8_t numberOfTopics);

/**
 PUBLISH a named message to the server.
 In addition to the PUBLISH specific parameters, the caller can indicate
 whether the routine should block until the time, the message has been
 acknowledged by the server. This is applicable only for non-QoS0 messages.

 In case, the app has chosen not to await for the ACK from the server,
 the SL MQTT implementation will notify the app about the subscription
 through the callback routine.

 \param[in] handle    refers to the handle to the client context
 \param[in] topic     topic of the data to be published. It is NULL terminated.
 \param[in] topicLen  topic length.
 \param[in] msg       binary data to be published
 \param[in] msgLen    length of the msg
 \param[in] flags     QOS define MQTT_PUBLISH_QOS_0, MQTT_PUBLISH_QOS_1 or MQTT_PUBLISH_QOS_2
                      use MQTT_PUBLISH_RETAIN is message should be retained

 \return Success(transaction Message ID) or Failure(Negative number)
 */
int16_t MQTTServer_publish(MQTTServer_Handle handle, char *topic, uint16_t topicLen, char *msg, uint16_t msgLen, uint32_t flags);

/**
 Setting Server parameters.
 This function can set different parameters to the server.
 User name and password - for Clients Authentication

 \param[in] handle      refers to the handle to the client context
 \param[in] option      Define the actual option to set. Applicable values:
                           - #MQTTServer_USER_NAME
                           - #MQTTServer_PASSWORD
 \param[in] value       Specifies a value for the option
 \param[in] valueLength Specifies the length of the value

 \return Success(0) or Failure(Negative number)
 */
int16_t MQTTServer_set(MQTTServer_Handle handle, uint16_t option, void *value, uint16_t valueLength);

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
