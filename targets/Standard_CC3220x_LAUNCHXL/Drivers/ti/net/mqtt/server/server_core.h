/*
 * Copyright (C) 2016-2018, Texas Instruments Incorporated
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

/**@file server_core.h
   The MQTT server daemon, a task, provisions the high level abstractions for the
   smart applications. This is an intelligent layer that utilizes the services
   of the MQTT Server Packet Library and is responsible for the functions of the
   topic management, the client management and support of multiple server
   applications.

   The light-weight server enables the services of the MQTT protocol for an
   application to either extend and / or control the existing functions to suit
   the deployment specific scenario. These applications in practice are the
   plug-ins / add-ons to the core server functionalities. Specifically,
   these applications, among others capabilities, can be used for analyzing and
   approving the credentials of the remote clients, acting as a bridge between
   a MQTT external client and the server, and a snooper to learn about all the
   data transactions to / from the server.

   The server is targeted to conform to MQTT 3.1.1 specification.

   The services of the server are multi-task safe. Platform specific atomicity
   constructs are used, through abstractions, by the server to maintain data
   coherency and synchronization.

   The server offers the following compile line configurable parameters (-D opt)
   - <b> CFG_SR_MAX_MQP_TX_LEN: </b> the constant buffer length allocated for a TX.
   \n\n
   - <b> CFG_SR_MAX_SUBTOP_LEN: </b> the maximum buffer size to hold a sub-topic.
   For e.g., in the topic /x/y/z, the phrase /x, /y and z are sub-topics.
   \n\n
   - <b> CFG_SR_MAX_TOPIC_NODE: </b> the maximum number of topic nodes in server.
   For e.g., in the topic /x/y/z, there are three nodes (/x, /y and z).
   \n\n
   - <b> CFG_SR_MAX_CL_ID_SIZE: </b> the maximum length of the client-id string.
   \n\n
   - <b> CFG_SR_MAX_NUM_CLIENT: </b> the maximum number of clients to be managed.
   Note this is different from the maximum number of 'contexts'. A large number
   of clients can be managed using fewer number of 'contexts' (connections).
   \n\n

   @note Any future extensions & development must follow the following guidelines.
   A new API or an extension to the existing API
   a) must be rudimentary
   b) must not imply a rule or policy (including a state machine)
   b) must ensure simple design and implementation
*/

#ifndef ti_net_mqtt_server_MQTTServerCore__include
#define ti_net_mqtt_server_MQTTServerCore__include

#include "server_pkts.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup server_daemon The Server Daemon API(s)
    @{
*/

typedef struct _MQTTServerCore_AppCBs_t_
{
    /** Connection request from remote client - assess credentials.
        This routine presents, to the application, the information about the
        credentials of the remote client that is trying to establish a MQTT
        connection with the server. The application shall utilize these data
        in conjunction with its policy to either allow or deny connection to
        the server.

        Should the application choose to allow the remote client to establish
        a MQTT connection, then it must provide in 'app-user' (place-holder),
        a handle that refers to the user of this connection. The server will
        provide this handle in follow-up routines to enable the application
        to refer to the associated connection in its implementation.

        @param[in] clientID UTF8 based ID of the remote client - is set to
        NULL to indicate a zero-length client id.
        @param[in] userName UTF8 based user-name provided by the remote
        client. It is set to NULL if the client did not provide an user name
        @param[in] password UTF8 based pass-word provided by the remote
        client. It is set to NULL if the client did not provide a pass-word
        @param[out] appUsr place-holder for application to provide a handle
        to the user of this specific connection / client.

        @return 16bit value for the variable header in the CONNACK message.
        The MSB in the return value refers to the 8bit parameter of the
        acknowledge flags and must be set 0. The LSB in the return value
        refers to the 8bit 'return code' parameter in the CONNACK message
        and must be set accordingly.
    */
    uint16_t  (*connect)(const MQTT_UTF8String_t *clientID,
                    const MQTT_UTF8String_t *userName,
                    const MQTT_UTF8String_t *password,
                    void **appUsr);

    /** Indicate a PUBLISH message from the network.
        This routine presents, to the application, the topic and the related
        data along with other qualifiers  published by one of the clients
        associated with this server. The application must enroll with the
        server the particular topic for which the data should be notified.

        @param[in] topic UTF8 topic Name for which data has been published
        @param[in] dataBuf the published binary data for the topic
        @param[in] dataLen the length of the binary data
        @param[in] dup is this a duplicate data from remote client?
        @param[in] qos quality of service of the message / data
        @param[in] retain should the server retain the data?

        @return none
    */
    void (*publish)(const MQTT_UTF8String_t *topic,
                    const uint8_t *payload, uint32_t payLen,
                    bool dup, uint8_t qos, bool retain);

    /** Notify disconnection to the remote client.
        This routine is invoked by the server to declare to the application
        to a particular client has been terminated and follow-up, if needed,
        can now commence. This routine aids the application by indicating if
        an error condition had caused the disconnection.

        @param[in] appUsr handle to refer to the user of the connection in
        the application
        @param[in] due2err has the connection been closed due to an error?

        @return none
    */
    void (*disconn)(const void *appUsr, bool due2err);

}MQTTServerCore_AppCBs_t;


/** Configuration for the applications that utilize the MQTT Server Daemon.
    At the moment this configuration is not used and has been incorporated
    to support forward compatibility (future use)
*/
typedef struct _MQTTServerCore_AppCfg_t_
{
    void *placeHolder; /**< Dummy, not used as of now */

}MQTTServerCore_AppCfg_t;


//*****************************************************************************
// function prototypes
//*****************************************************************************

/** Enroll with server to receive data for a topic
    This routine registers with the server, the specified topic for which the
    application should receive any published data from the network. Whenever,
    any of the remote clients that are connected to the server or applications,
    this or other, publishes data for the specified topic, the server will
    present the published information to this application.

    As part of the enrollment, the application should provide the maxmimum QoS
    with which the server should provide the published data. If the topic data
    received by the server bears a QoS higher than the one specified by the
    application, the server shall lower it to the QoS level preferred by the
    application. Otherwise, the QoS of the data shall be presented 'as-is'. In
    other words, the application should expect a published data with a lower QoS.

    @param[in] app_hnd handle to the application context in the server. This
    handle is provided by server @ref MQTTServerCore_appRegister()
    @param[in] topic UTF8 based string for which the application needs to start
    getting the published data
    @param[in] qos the maximum QoS the application intends to receive data for
    this particular topic

    @return 0 on success, otherwise a negative value on error.
*/
int32_t MQTTServerCore_topicEnroll(const void *app_hnd,
                             const MQTT_UTF8String_t *topic, MQTT_QOS qos);

/** Cancel previous enrollment to receive data for a topic
    This routines terminates the previous registration, if any, made by the
    application to receive any published data for the specified topic. Once,
    the enrollment is removed, the application, there after, will not receive
    any data for this topic from the server.

    @param[in] app_hnd handle to the application context in the server. This
    handle is provided by server @ref MQTTServerCore_appRegister()
    @param[in] topic UTF8 based string for which the application needs to stop
    getting the published data

    @return 0 on success, otherwise a negative value on error.
*/
int32_t MQTTServerCore_topicDisenroll(const void *app_hnd,
                                const MQTT_UTF8String_t *topic);

/** Send data to network for a topic
    This routine offers the binary data along-with associated properties for a
    specific topic to the server. The server, based on the subscriptions from the
    remote clients and the enrollments made by the local applications for this
    topic, will send the binary data and its qualifiers, adjusted for the maximum
    subscribed or enrolled QoS, to the remote clients and the local applications.

    @param[in] topic UTF8 topic Name for which data has been published
    @param[in] dataBuf the published binary data for the topic
    @param[in] dataLen the length of the binary data
    @param[in] qos quality of service of the message / data
    @param[in] retain should the server retain the data?

    @return on success, the length of data sent, otherwise -1 on error.
*/
int32_t MQTTServerCore_pubSend(const  MQTT_UTF8String_t *topic,
                             const uint8_t *dataBuf, uint32_t dataLen,
                             MQTT_QOS qos, bool retain);

/** Register an application with the server.
    This routine makes known to the server about an application identified by
    its name and creates a context / reference for the application in the
    server.

    An application intending to utilize the services of the MQTT server must be
    first registered with the server.

    @param[in] cbs callback routines from the application to be invoked by the
    server
    @param[in] name refers to the name of the application. The application must
    retain the memory used by the 'name' after the function call. The server
    does not copy the name into its internal buffers.

    @return a valid handle to context of application in the server, othewise
    NULL on error
*/
void *MQTTServerCore_appRegister(const MQTTServerCore_AppCBs_t *cbs,
                               const char *name);


/** Initialize the MQTT Server (Task / Daemon).
    This routine initializes the server implementation and sets it up using
    the provided configuration. The server implementation must be initialized
    prior to invoking of any other routine or service.

    This routine should be invoked as part of the platform initialization.

    @note This routine must be invoked only once in an run of the system. This
    routine internally invokes the @ref MQTTServerPkts_libInit( ) and therefore,
    there is no need to invoke the @ref MQTTServerPkts_libInit( ) explicitly.

    The server needs to be in a state to listen to incoming MQTT connection
    requests. Therefore, the platform sequence after provisioning the buffer
    using the API @ref MQTTServerPkts_registerNetSvc, must invoke the API @ref
    MQTTServerPkts_run, in an infinite loop, to keep the server daemon active.

    @param[in] libCfg configuration information for the MQTT server packet
    library.
    @param[in] appCfg configuration information for the server applications.

    @return 0 on success, otherwise -1 on error
*/
int32_t MQTTServerCore_init(const MQTTServerPkts_LibCfg_t *libCfg,
                     const MQTTServerCore_AppCfg_t *appCfg);

void MQTTServerCore_topicNodeExit(void);

/** @} */ /* End of server_daemon */
#ifdef __cplusplus
}
#endif

#endif
