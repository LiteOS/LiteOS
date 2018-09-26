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

/*
  mqtt_client.h

  This module enumerates the public interfaces / API of the MQTT Client
  Library.
*/

#ifndef ti_net_mqtt_client_MQTTClientCore__include
#define ti_net_mqtt_client_MQTTClientCore__include

/**@file mqtt_client.h
   This C library provisions the interface / API(s) for the MQTT Client.

   This is a light-weight library to enable the services of the MQTT protocol
   for one or more client applications (that would typically run on a resource
   constrained system). The key consideration in the design of small footprint
   library has been the abstraction of the low level details of the message
   transactions with the server and yet, provide rudimentary API(s), such that,
   the capabilities and features of the protocol are available to be utilized
   by existing and new applications in an un-restrictive manner.

   The library is targeted to conform to MQTT 3.1.1 specification.

   The MQTT Client library is a highly portable software and implies a very
   limited set of dependencies on a platform. Importantly, these limited
   dependencies are the commonly used features used in the embedded and the
   networking world, and can be easily adapted to the target platforms.

   The services of the library are multi-task safe. Platform specific atomicity
   constructs are used, through abstractions, by the library to maintain data
   coherency and synchronization. In addition, the library can be configured to
   support several in-flight messages.

   The client library supports multiple and simultaneous MQTT connections to one
   or more servers. In this client LIB, the reference to an individual connection
   in conjunction with the associated configuration parameters has been termed as
   a 'context'. Therefore, the client library supports multiple 'contexts'. An
   application that intends to make use of the client library must set-up or
   create a 'context' prior to establishing a connection with the server. The
   application can choose to destroy the 'context' after the connection with the
   server has been terminated. The client LIB can only support a finite set of
   'contexts' and the number can be configured by using a compiler line option /
   flag <b> -DCFG_CL_MQTT_CTXS </b>.

   Once, the 'context' is set-up, the application can send and receive the MQTT
   packets to / from the server. Among several features supported by the client
   LIB, the 'context' manages the keep-alive mechanism by automatically sending
   PING request to the server, if there has been no other packet send to the
   server with the keep-alive duration.

   @note Any future extensions & development must follow the following guidelines.
   A new API or an extension to the existing API
   a) must be rudimentary
   b) must not imply a rule or policy (including a state machine)
   b) must ensure simple design and implementation

*/

#include <ti/net/mqtt/common/mqtt_common.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MQTTCLIENTCORE_VERSTR "1.1.0" /**< Version of Client LIB */


/** Helper macros to derive 16 bit CONNACK Return Code from broker.
 */
#define MQTTClientCore_vhbConnackRC(vhBuf) (vhBuf[1])     /**< CONNACK  VH:: Return Code */
#define MQTTClientCore_packetConnackRC(mqp)    (mqp->buffer[3])/**< CONNACK MQP:: Return Code */

#define MQTTClientCore_vhbConnackSP(vhBuf) (vhBuf[0] & MQTT_VH_SESSION_PRESENT)/**< CONNACK VH:: Session Bit */
#define MQTTClientCore_packetConnackSP(mqp)    (mqp->buffer[2] & 0x1) /**< CONNACK MQP:: \
                                                         Session Bit */

#define MQTTClientCore_vhbConnackVH16(vhBuf)((vhBuf[0] << 8) | vhBuf[1])
#define MQTTClientCore_packetConnackVH16(mqp)   ((mqp->buffer[2] << 8) | mqp->buffer[3])

/** @defgroup mqtt_ctx_cfg_opt_grp Options for application to config CTX
@{
*/
#define MQTTCLIENTCORE_CFG_PROTOCOL_V31  0x0001 /**< Assert for ver3.1, default is 3.1.1 */
#define MQTTCLIENTCORE_CFG_APP_HAS_RTSK  0x0002 /**< Assert if APP has dedicated RX Task */
#define MQTTCLIENTCORE_CFG_MK_GROUP_CTX  0x0004 /**< Assert if task manages > 1 contexts */
/** @} */

/** @defgroup mqtt_netconn_opt_grp Options for App to configure network
@{
*/
#define MQTTCLIENTCORE_NETCONN_OPT_IP6  MQTT_DEV_NETCONN_OPT_IP6 /**<@ref dev_netconn_opt_grp */
#define MQTTCLIENTCORE_NETCONN_OPT_URL  MQTT_DEV_NETCONN_OPT_URL /**<@ref dev_netconn_opt_grp */
#define MQTTCLIENTCORE_NETCONN_OPT_SEC  MQTT_DEV_NETCONN_OPT_SEC /**<@ref dev_netconn_opt_grp */
/** @} */

//*****************************************************************************
// typedefs
//*****************************************************************************

/** Callbacks to be invoked by MQTT Client library onto Client application */
typedef struct _MQTTClientCore_CtxCBs_t_
{

        /** Provides a PUBLISH message from server to the client application.
            The application can utilize the associated set of helper macros to
            get references to the topic and the data information contained in
            the message. @ref rxpub_help_group

            Depending upon the QoS level of the message, the MQTT client library
            shall dispatch the corresponding acknowledgment (PUBACK or PUBREC) to
            the server, thereby, relieving application of this support.

            If the application completes the processing of the packet within the
            implementation of this callback routine, then a value of 'true' must
            be returned to the client LIB 'context'. The library, in this case,
            does not handover the packet to application and instead, frees it up
            on return from this routine.

            If the application intends to defer the processing of the PUBLISH
            message to a different execution task, then it must takeover the
            ownership of the packet by returning a value of 'false' to the client
            LIB 'context. In this arrangement, the client LIB 'context' hands
            over the packet to the application. The responsibility of storing,
            managing and eventually freeing up the packet back to the pool, now,
            lies with the app.

            @param[in] app application to which this PUBLISH message is targeted
            @see MQTTClientCore_createCtx
            @param[in] dup asserted to indicate a DUPLICATE PUBLISH
            @param[in] qos quality of Service of the PUBLISH message
            @param[in] retain Asserted to indicate message at new subscription
            @param[in] mqp Packet Buffer that holds the PUBLISH message

            @return true to indicate that processing of the packet has been
            completed and it can freed-up and returned back to the pool by
            the library. Otherwise, false.
        */
        bool (*publishRx)(void *app,
                           bool dup, MQTT_QOS qos, bool retain,
                           MQTT_Packet_t *mqp);

        /** Notifies the client application about an ACK or a response from the
            server. Following are the messages that are notified by the client
            LIB to the application.

            CONNACK, PINGRSP, PUBACK, PUBCOMP, SUBACK, UNSUBACK

            @param[in] app application to which this ACK message is targeted
            @see MQTTClientCore_createCtx
            @param[in] msgType Type of the MQTT message
            @param[in] msgID transaction identity of the message
            @param[in] buf refers to contents of message and depends on msgType
            @param[in] len length of the buf
            @return none

            @note The size of the buf parameter i.e len is non-zero for the
            SUBACK and CONNACK messages. For SUBACK the buf carries an array of
            QOS responses provided by the server. For CONNACK, the buf carries
            variable header contents. Helper macro MQTTClientCore_vhbConnackRC( ) and
            MQTTClientCore_vhbConnackSP( ) can be used to access contents provided by the
            server. For all other messages, the value of len parameter is zero.

            @note The parameter msgID is not relevant for the messages CONNACK
            and PINGRSP and is set to zero.
        */
        void (*ackNotify)(void *app, uint8_t msgType, uint16_t msgID, uint8_t *buf, uint32_t len);

        /** Notifies the client application about the termination of connection
            with the server. After servicing this callback, the application can
            destroy associated context if it no longer required

            @param[in] app application whose connection got terminated
            @see MQTTClientCore_createCtx
            @param[in] cause reason that created disconnection
            (@ref lib_err_group)
        */
        void (*disconnCB)(void *app, int32_t cause);
}MQTTClientCore_CtxCBs_t;

typedef struct _MQTTClientCore_CtxCfg_t_
{


        uint16_t   configOpts;  /**< Context config Opt, @ref mqtt_ctx_cfg_opt_grp */


        uint32_t   nwconnOpts;  /**< Network Options,  @ref mqtt_netconn_opt_grp */

        char   *serverAddr;  /**< Reference to '\0' terminated address string */
        uint16_t   portNumber;  /**< Network Listening Port number of the server */
        MQTT_SecureConn_t *nwSecurity;  /**< Refer to @ref mqtt_netsec_grp */
}MQTTClientCore_CtxCfg_t;


/** Contruct / Data to initialize MQTT Client Library */
typedef struct _MQTTClientCore_LibCfg_t_
{

        /** If an application has more than one contexts (i.e. grouped contexts)
            to manage in a single task, then a non-zero value must be provided.
            Otherwise, this parameter must be set to zero.
        */
        uint16_t   loopbackPort;
        bool  grpUsesCbfn;  /**< Assert if grouped contexts use call-backs */

        /** For a multi-task environment, provide a handle to platform mutex */
        pthread_mutex_t *mutex;
        void (*mutexLockin)(pthread_mutex_t *mutex); /**< Take platform mutex function */
        void (*mutexUnlock)(pthread_mutex_t *mutex); /**< Give platform mutex function */

}MQTTClientCore_LibCfg_t;

//*****************************************************************************
// function prototypes
//*****************************************************************************

/*------------------------------------------------------------------------------
 *  MQTT Client Messaging Routines / Services
 *------------------------------------------------------------------------------
 */

/** @defgroup client_api The Client Library API(s)
    @{
*/


/** Provides a new MSG Identifier for a packet dispatch to server

    @return MSG / PKT Transaction identifier
*/
uint16_t MQTTClientCore_newMsgID(void);

/** Ascertain whether connection / session with the server is active or not.
    Prior to sending out any information any message to server, the application
    can use this routine to check the status of the connection. If connection
    does not exist, then client should first CONNECT to the broker.

    A connection to server could have been closed/disconnected either due to
    keep alive time-out or due to error in RX / TX transactions.

    @note this API does not refer to network layer connection

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx

    @return true if connection is active otherwise false.
*/
bool MQTTClientCore_isConnected(void *ctx);

/** Send the CONNECT message to the server (and don't wait for CONNACK).
    This routine accomplishes multiple sequences. As a first step, it tries
    to establish a network connection with the server. Then, it populates
    an internally allocated packet buffer with all the previously provided
    payload data information, prepares the requisite headers and finally,
    dispatches the constructed message to the server.

    Prior to invoking this service, the client application should provision the
    intended payload contents of the CONNECT message by using the API(s) @ref
    MQTTClientCore_registerCtxInfo and @ref MQTTClientCore_registerWillInfo. And
    information about the server of interest must be provided in the client LIB
    'context' creation (@ref MQTTClientCore_createCtx).

    The client application must invoke an appropriate receive routine to know
    about the corresponding response CONNACK from the server. The client LIB will
    close the network connection to the server, if the server happens to refuse
    the CONNECT request.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] cleanSession asserted to delete references to previous session
    at both server and client
    @param[in] kaSecs Keep Alive Time
    @return number of bytes sent or LIB defined errors (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendConnectMsg(void *ctx, bool cleanSession, uint16_t kaSecs);

/** Send a PUBLISH message to the server (don't wait for PUBACK / PUBREC).
    This routine creates a PUBLISH message in an internally allocated packet
    buffer by embedding the 'topic' and 'data' contents, then prepares the
    packet header and finally, dispatches the message to the server.

    After the packet has been sent to the server, if the associated QoS of the
    dispatched packet is ether level 1 or 2, the client LIB 'context' will then
    store the packet until the time, a corresponding PUB-ACK (for QoS1) or
    PUB-REC (QoS2) message is received from the server.

    If the client LIB 'context' has been configured to assert 'clean session',
    then the references to all the stored and unacknowledged PUBLISH messages
    are dropped at the time of MQTT disconnection (or network disconnection).
    Otherwise, these unacknowledged packets continue to be available for the
    next iteration of the MQTT connection. However, if the client application
    asserts the 'clean session' parameter in the next iteration of the CONNECT
    operation, then references to all the stored PUBLISH messages, if any, are
    dropped.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] topic UTF8 based Topic Name for which data is being published.
    @param[in] dataBuf The binary data that is being published for the topic.
    @param[in] dataLen The length of the binary data.
    @param[in] qos quality of service of the message
    @param[in] retain should the server retain the message.
    @return on success, a transaction message id otherwise, LIB defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendPubMsg(void *ctx,
                             const  MQTT_UTF8String_t *topic,
                             const uint8_t *dataBuf, uint32_t dataLen,
                             MQTT_QOS qos,  bool retain);

/** Dispatch application constructed PUBLISH message to the server.
    Prior to sending the message to the server, this routine will prepare a fixed
    header to account for the size of the contents and the flags that have been
    indicated by the caller.

    After the packet has been sent to the server, if the associated QoS of the
    dispatched packet is ether level 1 or 2, the client LIB 'context' will then
    store the packet until the time, a corresponding PUB-ACK (for QoS1) or
    PUB-REC (QoS2) message is received from the server.

    If the client LIB 'context' has been configured to assert 'clean session',
    then the references to all the stored and unacknowledged PUBLISH messages
    are dropped at the time of MQTT disconnection (or network disconnection).
    Otherwise, these unacknowledged packets continue to be available for the
    next iteration of the MQTT connection. However, if the client application
    asserts the 'clean session' parameter in the next iteration of the CONNECT
    operation, then references to all the stored PUBLISH messages, if any, are
    dropped.

    The caller must populate the payload information with topic and data before
    invoking this service.

    This service facilitates direct writing of topic and (real-time) payload data
    into the buffer, thereby, avoiding power consuming and wasteful intermediate
    data copies.

    In case, the routine returns an error, the caller is responsible for freeing
    up or re-using the packet buffer. For all other cases, the client library
    will manage the return of the packet buffer to the pool.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] mqp app created PUBLISH message without the fixed header
    @param[in] qos QoS with which the message needs to send to server
    @param[in] retain Asserted if the message is to be retained by server.
    @return on success, the transaction Message ID, otherwise LIB defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_dispatchPub(void *ctx, MQTT_Packet_t *mqp,
                             MQTT_QOS qos, bool retain);

/** Send a SUBSCRIBE message to the server (and don't wait for SUBACK).
    This routine creates a SUBSCRIBE message in an internally allocated packet
    buffer by embedding the 'qosTopics', then prepares the message header and
    finally, dispatches the packet to the server.

    After the packet has been dispatched to the server, the library will store
    the packet until the time, a corresponding SUB-ACK has been received from
    the server. This mechanism enables the client LIB 'context' to trace the
    sequence of the message-ID and / or resend the SUB packets to the server.

    The client LIB 'context', if configured to operate in the MQTT 3.1.1 mode
    will drop or remove the un-acknowledged SUB messages at the time of the
    termination of the network connection.

    In the MQTT 3.1 mode, the client LIB 'context' will remove the unacknowledged
    SUB messages at the time of the termination of the network connection, if the
    'clean session' has been asserted. In case, the 'clean session' has not been
    asserted, the stored SUB messages will continue to be available for the next
    iteration of the MQTT connection. However, if the client application asserts
    the 'clean session' parameter in the next iteration of the CONNECT operation,
    then references to all the stored SUBSCRIBE messages, if any, are dropped.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] qosTopics an array of topic along-with its qos
    @param[in] count the number of elements in the array
    @return on success, the transaction Message ID, otherwise Lib defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendSubMsg(void *ctx, const MQTT_UTF8StrQOS_t *qosTopics, uint32_t count);

/** Dispatch application constructed SUBSCRIBE message to the server.
    Prior to sending the message to the server, this routine will prepare a fixed
    header to account for the size of the size of the contents.

    After the packet has been dispatched to the server, the library will store
    the packet until the time, a corresponding SUB-ACK has been received from
    the server. This mechanism enables the client LIB 'context' to trace the
    sequence of the message-ID and / or resend the SUB packets to the server.

    The client LIB 'context', if configured to operate in the MQTT 3.1.1 mode
    will drop or remove the un-acknowledged SUB messages at the time of the
    termination of the network connection.

    In the MQTT 3.1 mode, the client LIB 'context' will remove the unacknowledged
    SUB messages at the time of the termination of the network connection, if the
    'clean session' has been asserted. In case, the 'clean session' has not been
    asserted, the stored SUB messages will continue to be available for the next
    iteration of the MQTT connection. However, if the client application asserts
    the 'clean session' parameter in the next iteration of the CONNECT operation,
    then references to all the stored SUBSCRIBE messages, if any, are dropped.

    The caller must populate the payload information of topic along with qos
    before invoking this service.

    This service facilitates direct writing of topic and (real-time) payload data
    into the buffer, thereby, avoiding power consuming and wasteful intermediate
    data copies.

    In case, the routine returns an error, the caller is responsible for freeing
    up or re-using the packet buffer. For all other cases, the client library
    will manage the return of the packet buffer to the pool.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] mqp app created SUBSCRIBE message without the fixed header.
    @return on success, the transaction Message ID, otherwise Lib defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_dispatchSub(void *ctx, MQTT_Packet_t *mqp);

/** Send an UNSUBSCRIBE message to the server (and don't wait for UNSUBACK).
    This routine creates an UNSUBSCRIBE message in an internally allocated packet
    buffer by embedding the 'topics', then prepares the message header and
    finally, dispatches the packet to the server.

    After the packet has been dispatched to the server, the library will store
    the packet until the time, a corresponding UNSUB-ACK has been received from
    the server. This mechanism enables the client LIB 'context' to trace the
    sequence of the message-ID and / or resend the UNSUB packets to the server.

    The client LIB 'context', if configured to operate in the MQTT 3.1.1 mode
    will drop or remove the un-acknowledged SUB messages at the time of the
    termination of the network connection.

    In the MQTT 3.1 mode, the client LIB 'context' will remove the unacknowledged
    UNSUB messages at the time of the termination of the network connection, if
    the 'clean session' has been asserted. In case, the 'clean session' has not
    been asserted, the stored UNSUB messages will continue to be available for
    the next iteration of the MQTT connection. However, if the client application
    asserts the 'clean session' parameter in the next iteration of the CONNECT
    operation, then references to all the stored UNSUBSCRIBE messages, if any,
    are dropped.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] topics an array of topic to unsubscribe
    @param[in] count the number of elements in the array
    @return on success, the transaction Message ID, otherwise Lib defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendUnsubMsg(void *ctx, const MQTT_UTF8String_t *topics, uint32_t count);

/** Dispatch application constructed UNSUBSCRIBE message to the server.
    Prior to sending the message to the server, this routine will prepare a fixed
    header to account for the size of the size of the contents.

    After the packet has been dispatched to the server, the library will store
    the packet until the time, a corresponding UNSUB-ACK has been received from
    the server. This mechanism enables the client LIB 'context' to trace the
    sequence of the message-ID and / or resend the UNSUB packets to the server.

    The client LIB 'context', if configured to operate in the MQTT 3.1.1 mode
    will drop or remove the un-acknowledged SUB messages at the time of the
    termination of the network connection.

    In the MQTT 3.1 mode, the client LIB 'context' will remove the unacknowledged
    UNSUB messages at the time of the termination of the network connection, if
    the 'clean session' has been asserted. In case, the 'clean session' has not
    been asserted, the stored UNSUB messages will continue to be available for
    the next iteration of the MQTT connection. However, if the client application
    asserts the 'clean session' parameter in the next iteration of the CONNECT
    operation, then references to all the stored UNSUBSCRIBE messages, if any,
    are dropped.

    The caller must populate the payload information of topics before invoking
    this service.

    This service facilitates direct writing of topic and (real-time) payload data
    into the buffer, thereby, avoiding power consuming and wasteful intermediate
    data copies.

    In case, the routine returns an error, the caller is responsible for freeing
    up or re-using the packet buffer. For all other cases, the client library
    will manage the return of the packet buffer to the pool.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] Packet Buffer that holds UNSUBSCRIBE message without a fixed header
    @return on success, the transaction Message ID, otherwise LIB defined errors
    (@ref lib_err_group)
*/
int32_t MQTTClientCore_dispatchUnsub(void *ctx, MQTT_Packet_t *mqp);

/** Send a PINGREQ message to the server (and don't wait for PINGRSP).

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @return number of bytes sent or Lib define errors (@ref lib_err_group)
*/

int32_t MQTTClientCore_sendPingreq(void *ctx);

/** Send a DISCONNECT message to the server.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @return number of bytes sent or Lib define errors (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendDisconn(void *ctx);


/** Send remaining data or contents of the scheduled message to the server.
    This routine tries to send the remaining data in an active transfer of a
    message to the server. This service is valid, only if the network layer of
    the platform is not able to send out the entire message in one TCP packet
    and has to "back-off" or "give up the control" before it can schedule or
    dispatch the next packet. In such a scenario, the network layer sends the
    first part (segment) of the scheduled message in the mqtt_xxx_send() API and
    the subsequent parts or the segments are sent using this routine.

    This routine is not applicable to the platforms or the scenarios, where the
    implementation of the platform can segment the MQTT message in a manner to
    schedule consecutive or back-to-back blocking socket transactions.
    Specifically, this API must be used by an application, only if the network
    layer can indicate in an asynchronous manner, its readiness to send the next
    packet to the server. And the mechanism to indicate readiness of the network
    layer for the next send transaction is out of band and out of scope for the
    Client LIB and depends on the platform.

    A platform that resorts to partial send of a message and has to back-off from
    transmission implies the following the considerations on to the application.
    (a) The next segment / part of the currently active MQTT packet can be sent
    or scheduled only after receiving the indication, to do so, from the network
    layer.
    (b) The next or new MQTT message (or its first segment) can be scheduled for
    transmission only after receiving the indication for completion of handling
    of the last segment of currently active message.

    @note The application developer should refer to the platform specific network
    implementation for details.

    The routine returns the number of remaining bytes in the message to be sent.
    However, as described earlier, the application is expected to wait for an
    indication about the readiness of the network layer prior to sending or
    scheduling another segment, if so available, to the server. Now, the new
    segment can be a next part of the currently active message or it can be the
    first segment of a new message. A return value of zero means that there is
    no more data left in the scheduled message to be sent to the server and the
    application should wait for an appropriate event to indicate the transmission
    of the last segment.

    In case of an error, the transfer of the remaining segments or parts of the
    scheduled message is aborted. Depending on the configuration of the 'clean
    session' in-conjunction with the revision of the MQTT protocol, the active
    message would be stored for re-transmission, MQTT connection is established
    again. To store a message for re-transmission, at least one segment of the
    message must have been successfully sent to the server.

    @note This API must be used by the application only if the platform has the
    capability to indicate the completion of the sending of an active segment.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @return the number of bytes remaining to be sent in the message. Otherwise,
    LIB defined errors (@ref lib_err_group)
*/
int32_t MQTTClientCore_sendProgress(void *ctx);

/** Block on the 'context' to receive a message type with-in specified wait time.
    This service is valid only for the configuration, where the application has
    not provided the callbacks to the client LIB 'context'. The caller must
    provide a packet buffer of adequate size to hold the expected message from
    the server.

    The wait time implies the maximum intermediate duration between the reception
    of two successive messages from the server. If no message is received before
    the expiry of the wait time, the routine returns. However, the routine would
    continue to block, in case, messages are being received within the successive
    period of wait time and these messages are not the one that client is waiting
    for.

    For platforms that can receive a MQTT message across multiple network packets
    or in segments, it is necessary for the application to provide the same and
    unmodified packet buffer 'mqp' across all iterations of this routine when it
    returns with a value of MQTT_PACKET_ERR_TIMEOUT. Such an arrangement enables the LIB
    implementation to iteratively build from the consecutive multiple RX network
    packets / segments, a MQTT message into the packet buffer 'mqp'. Of-course,
    the application can always choose to assign a new 'mqp' once, a complete MQTT
    message has been received.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] msgType message type to receive. A value of 0 would imply that
    caller is ready to receive the next message, whatsoever, from the  server.
    @param[out] mqp packet buffer to hold the message received from the server.
    @param[in] waitSecs maximum Time to wait for a message from the server.
    @return On success, the number of bytes received for 'msgType' from server,
    otherwise LIB defined error values (@ref lib_err_group)
*/
int32_t MQTTClientCore_ctxAwaitMsg(void *ctx, uint8_t msgType, MQTT_Packet_t *mqp,
                              uint32_t waitSecs);

/** Helper function to receive any message from the server.
    Refer to MQTTClientCore_ctxAwaitMsg for the details.
    @see MQTTClientCore_ctxAwaitMsg
*/
static inline
int32_t MQTTClientCore_ctxRecv(void *ctx, MQTT_Packet_t *mqp, uint32_t waitSecs)
{
    /* Receive next and any MQTT Message from the broker */
    return MQTTClientCore_ctxAwaitMsg(ctx, 0x00, mqp, waitSecs);
}

/** Run the context for the specified wait time.
    This service is valid only for the configuration, where the application has
    populated the callbacks that can be invoked by the client LIB 'context'.

    This routine yields the control back to the application after the duration
    of the wait time. Such an arrangement enable the application to make overall
    progress to meet its intended functionality.

    The wait time implies the maximum intermediate duration between the reception
    of two successive messages from the server. If no message is received before
    the expiry of the wait time, the routine returns. However, the routine would
    continue to block, in case, messages are being received within the successive
    period of the wait time.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] waitSecs maximum time to wait for a message from the server

    @return MQTT_PACKET_ERR_NOTCONN if MQTT connection is closed by the application,
    MQTT_PACKET_ERR_TIMEOUT if there was no MQTT transaction in the interval of wait time
    and other values (@ref lib_err_group)
*/
int32_t MQTTClientCore_ctxRun(void *ctx, uint32_t waitSecs);

/** Block to receive any message for the grouped contexts within specified time.
    This service is valid only for the set-up, where the application has not
    configured the grouped contexts in the callback mode. The caller must provide
    a packet buffer of adequate size to hold the expected message from the server.

    The wait time implies the maximum intermediate duration between the reception
    of two successive messages from the server. If no message is received before
    the expiry of the wait time, the routine returns. However, the routine would
    continue to block, in case, messages are being received within the successive
    period of wait time.

    In this revision of the LIB, the underlying implementation will be provide a
    valid value in the 'app' field for the all returned values, including errors,
    other than the cases of 'MQTT_PACKET_ERR_TIMEOUT' and 'MQTT_PACKET_ERR_LIBQUIT'. The value in
    'app', when valid refers to a previously configured handle to an application
    and identifies the 'context' for which the routine has been returned.

    @param[out] mqp packet buffer to hold the message received from the server.
    @param[in]  waitSecs Maximum Time to wait for a message from the server.
    @param[out] app place holder to indicate application handle for the packet.
    @return On success, the number of bytes received for 'msgType' from server,
    otherwise LIB defined error values (@ref lib_err_group)

    @note if the value of MQTT_PACKET_ERR_LIBQUIT is returned, then system must be
    restarted.

    @warning This routine must not be used on platforms that can receive a MQTT
    message across multiple network packets or segments.
*/
int32_t MQTTClientCore_awaitMsg(MQTT_Packet_t *mqp, uint32_t waitSecs, void **app);

/** Run the LIB for the specified wait time.
    This service is valid only for the set-up of grouped contexts, where the
    application has populated the callbacks that can be invoked by the LIB.

    This routine yields the control back to the application after the specified
    duration of wait time. Such an arrangement enable the application to
    make overall progress to meet it intended functionality.

    The wait time implies the maximum intermediate duration between the reception
    of two successive messages from the server. If no message is received before
    the expiry of the wait time, the routine returns. However, the routine would
    continue to block, in case, messages are being received within the successive
    period of wait time.

    @param[in] waitSecs maximum time to wait for a message from the server

    @return on connection close by client app, number of bytes received for the
    last msg from broker, otherwise LIB defined error values.

    @note if the value of MQTT_PACKET_ERR_LIBQUIT is returned, then system must be
    restarted.
*/
int32_t MQTTClientCore_run(uint32_t waitSecs);

/*------------------------------------------------------------------------------
 *  MQTT Client Library: Packet Buffer Pool and its management
 *------------------------------------------------------------------------------
 */

/** Allocates a free MQTT Packet Buffer.
    The pool that will be used by the library to allocate a free MQP buffer
    must be configured (i.e. registered) a-priori by the app.

    The parameter 'offset' is used to specify the number of bytes that are
    reserved for the header in the buffer

    @param[in] msgType Message Type for which MQP buffer is being assigned.
    @param[in] offset Number of bytes to be reserved for MQTT headers.
    @return A NULL on error, otherwise a reference to a valid packet holder.

    @see mqtt_client_register_buffers
*/
MQTT_Packet_t *MQTTClientCore_alloc(uint8_t msgType, uint8_t offset);

/** Helper function to allocate a MQTT Packet Buffer for a message to be
    dispatched to server.

    @see to MQTTClientCore_alloc( ) for details.
*/
static inline MQTT_Packet_t *MQTTClientCore_allocSend(uint8_t msgType)
{
        return MQTTClientCore_alloc(msgType, MQTT_MAX_FH_LEN);
}

/** Helper function to allocate a MQTT Packet Buffer for a message to be
    received from the server.

    @see to MQTTClientCore_alloc( ) for details.
*/
static inline MQTT_Packet_t *MQTTClientCore_allocRecv(uint8_t msgType)
{
        return MQTTClientCore_alloc(msgType, 0);
}

/** Create a pool of MQTT Packet Buffers for the client library.
    This routine creates a pool of free MQTT Packet Buffers by attaching a buffer
    (buf) to a packet holder (mqp). The count of mqp elements and buf elements in
    the routine are same. And the size of the buffer in constant across all the
    elements.

    The MQTT Packet Buffer pool should support (a) certain number of in-flight and
    stored packets that await ACK(s) from the server (b) certain number of packets
    from server whose processing would be deferred by the client app (to another
    context) (c) a packet to create a CONNECT message to re-establish transaction
    with the server.

    A meaningful size of the pool is very much application specific and depends
    on the target functionality. For example, an application that intends to have
    only one in-flight message to the server would need at most three MQP buffers
    (1 for TX (for Qos1 or 2 store), 1 for RX and 1 for CONNECT message). If the
    application sends only QoS0 messages to the server, then the number of MQP
    buffers would reduce to two (i.e. 1 Tx to support CONNECT / PUB out and 1 RX)

    @param[in] mqpNum Number or count of elements in mqpVec and bufVec.
    @param[in] mqpVec An array of MQTT Packet Holder without a buffer.
    @param[in] bufLen The size or length of the buffer element in the 'bufVec'
    @param[in] bufVec An array of buffers.
    @retun  0 on success otherwise -1 on error.

    @note The parameters mqpVec and bufVec should be peristent entities.

    @see MQTTClientCore_awaitMsg
    @see MQTTClientCore_run
*/
int32_t MQTTClientCore_registerBuffers(uint32_t mqpNum, MQTT_Packet_t *mqpVec,
                                 uint32_t bufLen, uint8_t *bufVec);

/*------------------------------------------------------------------------------
 *  MQTT Client Library: Register application, platform information and services.
 *------------------------------------------------------------------------------
 */

/** Register application info and its credentials with the client library.
    This routine registers information for all the specified parameters,
    therefore, an update to single element would imply re-specification of
    the other parameters, as well.

    @note Contents embedded in the parameters is not copied by the routine,
    and instead a reference to the listed constructs is retained. Therefore,
    the app must enable the parameter contents for persistent.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] clientID MQTT UTF8 identifier of the client. If set to NULL,
    then the client will be treated as zero length entity.
    @param[in] userName MQTT UTF8 user name for the client. If not used,
    set it to NULL. If used, then it can't be of zero length.
    @param[in] password MQTT UTF8 pass word for the client. If not used, set
    it to NULL, If used, then it can't be of zero length.
    @return 0 on success otherwise -1

    User name without a pass word is a valid configuration. A pass word won't
    be processed if it is not associated with a valid user name.
*/
int32_t MQTTClientCore_registerCtxInfo(void *ctx,
                                  const MQTT_UTF8String_t *clientID,
                                  const MQTT_UTF8String_t *userName,
                                  const MQTT_UTF8String_t *password);

/** Register WILL information of the client application.
    This routine registers information for all the specified parameters,
    therefore, an update to single element would imply re-specification
    of the other parameters, as well.

    @note Contents embedded in the parameters is not copied by the routine,
    and instead a reference to the listed constructs is retained. Therefore,
    the app must enable the parameter contents for persistent.

    @param[in] ctx handle to the underlying network context in the LIB
    @see MQTTClientCore_createCtx
    @param[in] willTop UTF8 WILL Topic on which WILL message is to be published.
    @param[in] willMsg UTF8 WILL message.
    @param[in] willQos QOS for the WILL message
    @param[in] retain asserted to indicate that published WILL must be retained
    @return 0 on success otherwise -1.

    Both willTop and willMsg should be either present or should be NULL.
    willQos and retain are relevant only for a valid Topic and Message combo.
*/
int32_t MQTTClientCore_registerWillInfo(void *ctx,
                                  const MQTT_UTF8String_t  *willTop,
                                  const MQTT_UTF8String_t  *willMsg,
                                  MQTT_QOS willQos, bool retain);

/** Abstraction for the device specific network services.
    Network services for communication with the server

    @param[in] net refers to network services supported by the platform
    @return on success, 0, otherwise -1

    @ref net_ops_group
    @note all entries in net must be supported by the platform.
*/
int32_t MQTTClientCore_registerNetSvc(const MQTT_DeviceNetServices_t *net);


/** Create a Network Connection Context.
    This routine sets-up the parameters that are required to operate and manage
    the network connection with a MQTT server / broker. As part of the creation
    of a context, the implementation also records the handle, if provided, by
    the application. In addition, the invoker of the routine must facilitate a
    place holder to enable the client LIB to provision the reference to the
    'context', so created.

    Specifically, this routine associates or ties-up, in an one-to-one manner,
    the caller provided handle 'app' and the client LIB provisioned handle 'ctx'.
    The parameter 'app' is returned by the client LIB in certain other routines
    to indicate the underlying 'context' with which network transaction or event
    is associated. Similarly, the caller must specify the context handle 'ctx'
    for which the services are being invoked.

    A user or a task prior to utilizing the services of the library to schedule
    MQTT transactions must create a 'context'. A client LIB 'context' can be
    operated in two modes: (a) sync-wait or explicit receive mode and (b) the
    callback mode. Provisioning or absence of the callback parameter in this
    routine defines the mode of operation of the 'context'.

    Explicit receive mode is analogous to the paradigm of the socket programming
    in which an application utilize the recv() function call. It is anticipated
    that applications which would make use of limited set of MQTT messages may
    find this mode of operation useful. Applications which intend to operate the
    'context' in this mode must not provision any callbacks.

    On the other hand, certain applications, may prefer an asynchronous mode of
    operation and would want the client LIB 'context' to raise callbacks into
    the application, as and when, packets arrive from the server. And such
    applications must provide the callback routines.

    @param[in] ctxCfg configuration information for the Network Context.
    @param[in] ctxCBs callback routines. Must be set to NULL, if the application
    intends to operate the context in the sync-wait / explicit receive mode.
    @param[in] app     handle to application. Returned by LIB in other routines
    to refer to the underlying context.
    @param[out] ctx reference to the context created and is provisioned by the
    implementation. (Valid only if routine returns a success)

    @return 0 on success otherwise -1.
*/
int32_t MQTTClientCore_createCtx(const MQTTClientCore_CtxCfg_t *ctxCfg,
                           const MQTTClientCore_CtxCBs_t *ctxCBs,
                           void *app, void **ctx);

/** Delete a Network Connection Context.
    This routines destroys the previously created network 'context' and releases
    resources that would be assigned for maintaining the information about the
    'context'.

    A user or a task prior to deleting the 'context' must ensure that there is no
    active MQTT connection on this context.

    @param[in] ctx handle to network context to be deleted. The context must
    have been previously created.

    @return 0 on success otherwise -1
*/

int32_t MQTTClientCore_deleteCtx(void *ctx);


/** Initialize the MQTT client library.
    This routine initializes all the common constructs that are required to
    manage the multiple network connections. The client LIB must be initialized
    prior to invoking of any other routine or service.

    @note This routine must be invoked only once in an run of the system.

    Depending upon the deployment needs, this routine can be invoked either as
    part of the platform initialization sequence or as part of the application.
    Deployments that have more than one application utilizing the services of
    the client LIB should try to invoke the routine from the initialization
    sequence of the platform.

    In addition, if an application has to manage more than one network
    connections (i.e. in other words, if the application has to handle
    a group of connections), then certain configuration must be set in
    the LIB @see struct MQTTClientCore_LibCfg

    @note There must be only one group of network connections in the system.

    @param[in] cfg Configuration information for the MQTT client Library.

    @return 0 on success otherwise -1.
*/

int32_t MQTTClientCore_initLib(const MQTTClientCore_LibCfg_t  *cfg);

/** Exit the MQTT client library.
    @return 0 on success otherwise -1.
*/
int32_t MQTTClientCore_exitLib(void);

/** @} */ /* End group client_api */
#ifdef __cplusplus
}
#endif

#endif
