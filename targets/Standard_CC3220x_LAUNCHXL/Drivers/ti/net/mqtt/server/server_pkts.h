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

/*-----------------------------------------------------------------------------
 * Note: Do not create additional dependency of this file on any header other
 * than mqtt_common.h. Specifically, server_pkts.[hc] in conjunction with the
 * mqtt_common.[hc] files must be facilitated to create a stand-alone library.
 *-----------------------------------------------------------------------------
 */

/**@file server_pkts.h
   The C library provisions the interface / API(s) for the MQTT Server Packet LIB.

   This is a light-weight library that enables the services of the MQTT protcol
   for user's server application(s) to exchange the MQTT packets with one or
   more remote clients. The Server Packet LIB is a simple and easy-to-use
   implementation to support both un-packing of the messages received from the
   remote clients and formation of packets to be sent to the remote clients.

   The library is targeted to conform to MQTT 3.1.1 specification.

   The Server Packet LIB is a highly portable software and implies a very limited
   set of dependencies on a platform. Importantly, these limited dependencies are
   common features used in the embedded and the networking world, and can be
   easily adapted to the target platforms. The services of the library are
   multi-task safe. Platform specific atomicity constructs are used, through
   abstractions, by the library to maintain data coherency and synchronization.
   In addition, the library can be configured to support several in-flight
   messages.

   The Server Packet LIB can support multiple and simultaneous MQTT connections
   from clients. However, the responsibility of managing the clients and topics,
   authentication and approval for connections and supporting any other needs
   that specific to the deployment remains with the user application.

   The number of the network connections that the Server Packet LIB can support
   is configurable through the compile line option / flag <b> -DCFG_SR_MQTT_CTXS
   </b>. In addition, the maximum length of the RX buffer used by the server is
   also configurable through the compile line option / flag <b>
   -DCFG_SR_MAX_MQP_RX_LEN </b>.

   @note Any future extensions & development must follow the following guidelines.
   A new API or an extension to the existing API
   a) must be rudimentary
   b) must not imply a rule or policy (including a state machine)
   b) must ensure simple design and implementation
*/

#ifndef ti_net_mqtt_server_MQTTServerPkts__include
#define ti_net_mqtt_server_MQTTServerPkts__include

#include <stdbool.h>
#include <pthread.h>

#include <ti/net/mqtt/common/mqtt_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup server_pktlib The Server Library API(s)
    @{
*/

/** @defgroup conn_utf8_help_group Helper Macros for RX CONNECT
    @{
*/

/** Yields pointer to the UTF8 content */
#define MQ_CONN_UTF8_BUF(utf8)      ((utf8)? (utf8)->buffer : NULL)

/** Length or size of the UTF8 content */
#define MQ_CONN_UTF8_LEN(utf8)      ((utf8)? (utf8)->length : 0)

#define MQC_UTF8_CLIENTID(utf8Vec) (utf8Vec[0]) /**< Returns Client ID  */
#define MQC_UTF8_WILL_TOP(utf8Vec) (utf8Vec[1]) /**< Returns Will Topic */
#define MQC_UTF8_WILL_MSG(utf8Vec) (utf8Vec[2]) /**< Returns Will Data  */
#define MQC_UTF8_USERNAME(utf8Vec) (utf8Vec[3]) /**< Returns User Name  */
#define MQC_UTF8_PASSWORD(utf8Vec) (utf8Vec[4]) /**< Returns Pass Word  */

#define MQC_CLIENTID_BUF(utf8Vec)  MQ_CONN_UTF8_BUF(MQC_UTF8_CLIENTID(utf8Vec))
#define MQC_CLIENTID_LEN(utf8Vec)  MQ_CONN_UTF8_LEN(MQC_UTF8_CLIENTID(utf8Vec))

#define MQC_WILL_TOP_BUF(utf8Vec)  MQ_CONN_UTF8_BUF(MQC_UTF8_WILL_TOP(utf8Vec))
#define MQC_WILL_TOP_LEN(utf8Vec)  MQ_CONN_UTF8_LEN(MQC_UTF8_WILL_TOP(utf8Vec))

#define MQC_WILL_MSG_BUF(utf8Vec)  MQ_CONN_UTF8_BUF(MQC_UTF8_WILL_MSG(utf8Vec))
#define MQC_WILL_MSG_LEN(utf8Vec)  MQ_CONN_UTF8_LEN(MQC_UTF8_WILL_MSG(utf8Vec))

#define MQC_USERNAME_BUF(utf8Vec)  MQ_CONN_UTF8_BUF(MQC_UTF8_USERNAME(utf8Vec))
#define MQC_USERNAME_LEN(utf8Vec)  MQ_CONN_UTF8_LEN(MQC_UTF8_USERNAME(utf8Vec))

#define MQC_PASSWORD_BUF(utf8Vec)  MQ_CONN_UTF8_BUF(MQC_UTF8_PASSWORD(utf8Vec))
#define MQC_PASSWORD_LEN(utf8Vec)  MQ_CONN_UTF8_LEN(MQC_UTF8_PASSWORD(utf8Vec))

/** @} */

#ifndef CFG_SR_MAX_MQP_RX_LEN
#define MQP_SERVER_RX_LEN  1024  /**< Max size(B) of RX Buffer for MQTT Server */
#else
#define MQP_SERVER_RX_LEN  CFG_SR_MAX_MQP_RX_LEN
#endif

/** <b> Working Principle </b> for implementing the call-back services:
    Implementation of the call-back services should report in return value, only
    about errors found in the RX processing. Specifically, the status of TX as a
    follow-up to RX message (represented as a call-back service) need not be
    reported to the server packet library.

    Error observed in TX (supported by appropriate API(s) / services in the
    service packet library) is recorded in the 'context' and shall be dealt in
    the next iteration of RX loop.
*/
typedef struct _MQTTServerPkts_MsgCBs_t_
{
        /** Indicate the CONNECT Message from the client
            This routine provides, to the application, information about the
            connection request that a remote client has made. The application
            should utilize the credential and other data presented by the LIB
            to authenticate, analyze and finally, approve or deny the request.

            Application at its discretion can also imply deployment specific
            policies to make decision about allowing or refusing the request.

            The return value of this routine is a 16bit value that commensurates
            with the 'variable header' of the CONNACK message. Specifically, the
            LSB of the 16bit return value corresponds to the 8bit 'return code'
            parameter in the CONNACK message and the MSB to the 'acknowledge
            flags'. The application must set a valid return value.

            The LIB uses the return value to compose the CONNACK message to the
            remote client. If the LSB of return value is a 'non zero' value,
            then the LIB, after sending the CONNACK message to the remote client,
            will terminate the network connection.

            @param[in] ctxCl handle to the underlying network context in the LIB
            @param[in] connFlags options received in CONNECT message from server
            @param[in] utf8Vec vector / array of pointers to UTF8 information.
            The order of UTF8 information is client-id, will topic, will-message,
            user-name and pass-word. A NULL in vector element indicates absence
            of that particular UTF8 information.
            @param[out] usr place holder for application to provide connection
            specific handle. In subsequent calls from the implementation this
            handle will be passed as a parameter to enable application to refer
            to the particular active connection.

            @return 16bit value for the variable header of the CONNACK message,
            MSB is CONNACK-Flags, LSB is CONNACK-RC
        */
        uint16_t  (*connectRx)(void *ctxCl, uint8_t connFlags,
                           MQTT_UTF8String_t * const *utf8Vec, void **usr);

        /** Indicate the SUBSCRIBE Message from the client
            This routine provides, to the application, information about the
            topics that the remote client intends to subscribe to.

            On return from this routine, if the application has found a problem
            in the processing of message, then the LIB will simply terminate the
            associated network connection.

            @param[in] usrCl handle to connection context in the application
            @param[in] qosTopics an array of topic along-with its qos
            @param[in] nTopics the count / number of elements in the array
            @param[in] msgID the associated message ID provided by the client
            @param[in] acks place holder array for the application to provide
            finalized qos for each of the subscribed topic. The order of ack
            is same as that of qosTopics

            @return The application should return false, if it encounters any
            problem in the processing of topic. Otherwise, true.

            @note The memory space pointed by the 'buffer' field in the elements
            of 'qosTopics' array has an additional byte available beyond the
            size of memory indicated by the 'length' field. This extra byte can
            be used by the application to NUL terminate the buffer. <b> This
            quirk is applicable to this routine only. </b>
        */
        bool (*subMsgRx)(void *usrCl, const MQTT_UTF8StrQOS_t *qosTopics,
                          uint32_t nTopics, uint16_t msgID, uint8_t *acks);

        /** Indicate the UNSUBSCRIBE Message from the client
            This routine provides, to the application, information about the
            topics that the remote client intends to unsubscribe.

            On return from this routine, if the application has found a problem
            in the processing of message, then the LIB will simply terminate the
            associated network connection.

            @param[in] usrCl handle to connection context in the application
            @param[in] topics an array of topic in the message
            @param[in] nTopics the count / number of elements in the array
            @param[in] msgID the associated message ID provided by the client

            @return The application should return false, if it encounters any
            problem in the processing of topic. Otherwise, true.
        */
        bool (*unSubMsg)(void *usrCl, const MQTT_UTF8String_t *topics,
                           uint32_t nTopics, uint16_t msgID);

        /** Indicate the PUBLISH Message from the client.
            This routine provides, to the application, the binary data along-with
            its qualifiers and the topic to which a remote client has published
            data.

            On return from this routine, if the application has found a problem
            in processing of the contents of the PUBLISH message, the LIB will
            simply terminate the associated network connection. Otherwise,
            depending upon the QoS level of the PUBLISH message, the LIB shall
            dispatch the ACK (PUBACK or PUBREC) to the client, thereby,
            relieving the application from this support.

            @param[in] usrCl handle to connection context in the application
            @param[in] topic UTF8 Topic Name for which data has been published
            @param[in] dataBuf the published binary data for the topic
            @param[in] dataLen the length of the binary data
            @param[in] msgID the associated message ID provided by the client
            @param[in] dup has client indicated this as a duplicate message
            @param[in] qos quality of service of the message
            @param[in] retain should the server retain the data?

            @return The application should return false, if it encounters any
            problem in the processing of data, topic and related resources.
            Otherwise, true.
        */
        bool (*pubMsgRx)(void *usrCl, const MQTT_UTF8String_t *topic,
                           const uint8_t *dataBuf, uint32_t dataLen, uint16_t msgID,
                           bool dup, MQTT_QOS qos, bool retain);

        /** Notify the acknowledgment that was received from the remote client.
            Following are the messages that are notified by the server LIB.

            PUBACK, PUBREC, PUBREL, PUBCOMP

            On return from this routine, if the application has found problem
            in processing the ACK message, then the LIB will simply terminate
            the associated network connection

            @param[in] usrCl handle to connection context in the application
            @param[in] msgType refers to the type of ACK message
            @param[in] msgID the associated message ID provided by the client
            @return application should return false if the ACK was not expected
            by it or no reference was found for it. Otherwise true.
        */
        bool (*ackNotify)(void *usrCl, uint8_t msgType, uint16_t msgID);

        /** Notify that network connection to client has been closed.
            This routine is invoked by the LIB to declare to the application that
            the network connection to a particular client has been terminated and
            follow-up, if needed, can now commence. If configured, removal of the
            client session and / or dispatch of the WILL message, will be typical
            aspects, among others, to follow-up. The routine aids the application
            by indicating if an error condition had caused the closure.

            This routine is invoked by the LIB irrespective of the source entity,
            server or client, that has caused the closure of the network.

            @param[in] usrCl handle to connection context in the application
            @param[in] due2err has the connection been closed due to an error?
        */
        void (*onClNetClose)(void *usrCl, bool due2err);

        /** Notify that CONNACK has been sent to the specified client.
            This routine is invoked by the LIB to enable the application to make
            progress and follow-up on the session information for the particular
            client. Specifically, this routine facilitates the application to
            either delete the session or re-send / sync-up the pending messages
            associated with the client. The follow-up action is dependent upon
            the 'cleanSession' option in the CONNECT message from the client.
            @param[in] usrCl handle to connection context in the application
            @param[in] cleanSession was a clean session requested in CONNECT?
        */
        void (*onConnackSend)(void *usrCl, bool cleanSession);

}MQTTServerPkts_MsgCBs_t;


typedef struct _MQTTServerPkts_LibCfg_t_
{
        /** Port to listen to incoming network connections from the clients */
        uint16_t listenerPort;

        /** If the server application has more than one task and / or supports
            at-least one plug-in, then a non-zero value must be provided.
            Otherwise, this parameter must be set to zero. This parameter is
            used by the implementation to synchronize multiple tasks for the
            network connection.
        */
        uint16_t loopbackPort;

        /** For a multi-task environment, provide a handle to platform mutex */
        pthread_mutex_t **mutex;
        void (*mutexLockin)(pthread_mutex_t *mutex);
        void (*mutexUnlock)(pthread_mutex_t *mutex);

        MQTT_SecureConn_t  secure;
}MQTTServerPkts_LibCfg_t;

//*****************************************************************************
// function prototypes
//*****************************************************************************

/** Send a Variable Header only message to the client.
    Application should this routine to send PUBREL and PUBCOMP messages.

    @param[in] ctxCl handle to the underlying network context in the LIB. This
    handle is provided to the application by the LIB in the CONNECT callback.
    @param[in] msgType message type that has to be sent to the client
    @param[in] qos QoS with which the message needs to send to server
    @param[in] hasVH does this message has data in the variable header?
    @param[in] vhData data <MSB:LSB> to be included in the variable header
    @return on success, the number of bytes transferred. Otherwise, errors
    defined in @ref lib_err_group
*/
int32_t MQTTServerPkts_sendVhMsg(void *ctxCl, uint8_t msgType, MQTT_QOS qos,
                     bool hasVH, uint16_t vhData);

/** MQTTServerPkts_sendVhMsg() with mutual exclusion (in multi-task application).
    This routine ensures that the LIB sends the specified VH MSG onto the network
    in a manner that excludes execution of any other control. This API has been
    enabled to support the scenarios, where the multi-tasked user application has
    chosen to use a mutex object other than the one provisioned in the packet LIB
    to streamline / serialize accesses to the services of the packet LIB.

    Refer to @ref MQTTServerPkts_sendVhMsg for details
*/
int32_t MQTTServerPkts_sendLockedVhMsg(void *ctxCl, uint8_t msgType, MQTT_QOS qos,
                            bool hasVH, uint16_t vhData);

/** Dispatch application constructed PUBLISH message to the client.
    Prior to sending the message to the client, this routine shall update the
    fixed-header to account for the duplicate flag that has been indicated by
    the caller.

    The caller must populate the payload information of topic and data before
    invoking this service. In addition, the application must prepare, for the
    packet, the fix header leaving aside the duplicate flag - this flag shall
    be included in the fix header by the LIB.

    This service facilitates the application to re-use, iteratively, a single
    PUBLISH packet for multiple remote clients that have subscribed to the
    same topic for which the data is being published. The LIB does not free-up
    the MQTT packet after sending the packet to the remote client and the
    application is responsible for managing the packet container / memory

    @param[in] ctxCl handle to the underlying network context in the LIB. This
    handle is provided to the application by the LIB in the CONNECT callback.
    @param[in] mqp app created PUBLISH message along with the fixed header
    @param[in] dup is this a duplicate message that is being sent to client?
    @return on success, the number of bytes transferred. Otherwise, error
    defined in @ref lib_err_group
*/
int32_t MQTTServerPkts_dispatchPub(void *ctxCl, MQTT_Packet_t *mqp, bool dup);

/** MQTTServerPkts_dispatchPub() with mutual exclusion (in multi-task application).
    This routine ensures that the LIB sends the specified packet onto the network
    in a manner that excludes execution of any other control. This API has been
    enabled to support the scenarios, where the multi-tasked user application has
    chosen to use a mutex object other than the one provisioned in the packet LIB
    to streamline / serialize accesses to the services of the packet LIB.

    Refer to @ref MQTTServerPkts_dispatchPub for other details.
*/
int32_t MQTTServerPkts_dispatchLockedPub(void *ctxCl, MQTT_Packet_t *mqp,
                                    bool dup);

/** Run the server packet LIB for the specified wait time.
    This routine yields the control back to the application after the specified
    duration of wait time. Such an arrangement enable the application to make
    overall progress to meet it intended functionality.

    The wait time implies the maximum intermediate duration between the reception
    of two successive messages from the server. If no message is received before
    the expiry of the wait time, the routine returns. However, the routine would
    continue to block, in case, messages are being received within the successive
    period of wait time.

    @param[in] waitSecs maximum time to wait for a message from the server

    @note if the value of MQP_ERR_LIBQUIT is returned, then system must be
    restarted.
*/
int32_t MQTTServerPkts_run(uint32_t waitSecs);

/** Abstraction for the device specific network services
    Network services for communication with the clients

    @param[in] net refers to network services supported by the platform
    @return on success, 0, otherwise -1

    @ref net_ops_group
    @note all entries in net must be supported by the platform.
*/
int32_t MQTTServerPkts_registerNetSvc(const MQTT_DeviceNetServices_t *net);

/** Initialize the MQTT Server Packet library
    This routine initializes the packet and network constructs that are required
    to manage the multiple network connections. The server packet LIB must be
    initialized prior to invoking of any other routine or service.

    @note This routine must be invoked only once in an run of the system.

    If there are more than one application (tasks) that utilize the services
    of the server packet library, then certain configuration must be set in
    the LIB @see MQTTServerPkts_LibCfg_t

    The application should also provision the platform network specific network
    services into the packet library @see MQTTServerPkts_registerNetSvc.

    Once, the server LIB has been initialized successfully, it must be put into
    action, to listen to requests for incoming connections, by invoking the API
    @ref MQTTServerPkts_run.

    @param[in] cfg configuration information for the MQTT server packet library
    @param[in] cbs callback routines that LIB will invoke into the application

    @return 0 on success otherwise -1.
*/
int32_t MQTTServerPkts_libInit(const MQTTServerPkts_LibCfg_t *cfg,
                         const MQTTServerPkts_MsgCBs_t *cbs);

/** @} */ /* End of server_pktlib */
#ifdef __cplusplus
}
#endif

#endif
