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
  mqtt_common.h

  This module outlines the interfaces that are common to both client amd
  server components. The applications are not expected to utilize the
  services outlined in this module.
*/

#ifndef ti_net_mqtt_common_MQTT__include
#define ti_net_mqtt_common_MQTT__include

/** @file mqtt_common.h
    This file incorporates constructs that are common to both client and server
    implementation.

    The applications are not expected to utilize the routines made available in
    this module module.

    @note the routines in this module do not check for availability and
    correctness of the input parameters

    @warning The module is expected to under-go changes whilst incorporating
    support for the server. Therefore, it is suggested that applications do
    not rely on the services provided in this module.
*/

#include <ti/net/slnetsock.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_COMMON_VERSTR "1.2.0" /**< Version of Common LIB */

#define MQTT_MIN(a, b) ((a > b)? b : a)

/** MQTT Message Types */
#define MQTT_CONNECT      0x01
#define MQTT_CONNACK      0x02
#define MQTT_PUBLISH      0x03
#define MQTT_PUBACK       0x04
#define MQTT_PUBREC       0x05
#define MQTT_PUBREL       0x06
#define MQTT_PUBCOMP      0x07
#define MQTT_SUBSCRIBE    0x08
#define MQTT_SUBACK       0x09
#define MQTT_UNSUBSCRIBE  0x0A
#define MQTT_UNSUBACK     0x0B
#define MQTT_PINGREQ      0x0C
#define MQTT_PINGRSP      0x0D
#define MQTT_DISCONNECT   0x0E

#ifndef MQTT_QOS_0
#define MQTT_QOS_0  (0)
#endif
#ifndef MQTT_QOS_1
#define MQTT_QOS_1  (1)
#endif
#ifndef MQTT_QOS_2
#define MQTT_QOS_2  (2)
#endif
#ifndef MQTT_PUBLISH_RETAIN
#define MQTT_PUBLISH_RETAIN (8)
#endif

#define MQTT_MAX_FH_LEN   0x05    /**< MAX Length of Fixed Header */

#define MQTT_VH_SESSION_PRESENT 0x01 /**< Session Present Flag */

/** Max number of bytes in remaining length field */
#define MQTT_MAX_REMLEN_BYTES  (MQTT_MAX_FH_LEN - 1)

#define MQTT_MAKE_FH_BYTE1(msgType,  flags) (uint8_t)((msgType << 4) | flags)

#define MQTT_MAKE_FH_FLAGS(bool_dup, enum_qos, bool_retain)                  \
        (uint8_t)(((bool_dup << 3) | (enum_qos << 1) | bool_retain) & 0xF)

#define MQTT_FH_BYTE1_IS_RETAIN(fhByte1)  ((fhByte1 & 0x1)? true : false)
#define MQTT_FH_BYTE1_IS_DUP(fhByte1)     ((fhByte1 & 0x8)? true : false)
#define MQTT_FH_BYTE1_DUP_VAL(bool_val) (uint8_t)(bool_val << 3)
#define MQTT_FH_BYTE1_QOS(fhByte1)     (MQTT_QOS)((fhByte1 & 0x6) >> 1)

#define QID_VMASK           0x3
#define QOS_VALUE(enum_qos) (uint8_t)(enum_qos & QID_VMASK)
#define QFL_VALUE           0x80  /**< QOS Failure value (SUBACK) */

/** @} */

#define MQTT_PACKET_FHEADER_BUF(mqp)  (mqp->buffer + mqp->offset)
#define MQTT_PACKET_VHEADER_BUF(mqp)  (MQTT_PACKET_FHEADER_BUF(mqp) + mqp->fhLen)
#define MQTT_PACKET_PAYLOAD_BUF(mqp)  (MQTT_PACKET_VHEADER_BUF(mqp) + mqp->vhLen)

#define MQTT_PACKET_CONTENT_LEN(mqp)  (mqp->fhLen + mqp->vhLen + mqp->plLen)
#define MQTT_PACKET_FREEBUF_LEN(mqp)  (mqp->maxlen - mqp->offset -      \
                               MQTT_PACKET_CONTENT_LEN(mqp))

/*---------------------------------------------------------------------
 * Heleper MACROS for PUBLISH-RX Message Processing
 *---------------------------------------------------------------------
 */

/** @defgroup rxpub_help_group Helper Macros for RX PUBLISH
    @{
*/

/** Yields pointer to topic content */
#define MQTT_PACKET_PUB_TOP_BUF(mqp) (MQTT_PACKET_VHEADER_BUF(mqp) + 2)

/** Length or size of topic content */
#define MQTT_PACKET_PUB_TOP_LEN(mqp) (mqp->vhLen - 2 - (mqp->msgID? 2 : 0))

/** Yields pointer to payload data */
#define MQTT_PACKET_PUB_PAY_BUF(mqp) (mqp->plLen? MQTT_PACKET_PAYLOAD_BUF(mqp) : NULL)

/** Length or size of payload data */
#define MQTT_PACKET_PUB_PAY_LEN(mqp) (mqp->plLen)

/** @} */

/** @cond
    CONNECT Message Flags, as outlined in the MQTT Specification
*/
#define MQTT_CONN_MSG_FLAG_WILL_RETAIN 0x20
#define MQTT_CONN_MSG_FLAG_WILL_CONFIG 0x04
#define MQTT_CONN_MSG_FLAG_CLEAN_START 0x02
#define MQTT_CONN_MSG_FLAG_USER_NAME 0x80
#define MQTT_CONN_MSG_FLAG_PASS_WORD 0x40
/**
    @endcond
*/

/** @cond
    CONNACK 8bit Return Code, as outlined in the MQTT Specification
*/
#define MQTT_CONNACK_RC_REQ_ACCEPT   0x00
#define MQTT_CONNACK_RC_BAD_PROTOV   0x01
#define MQTT_CONNACK_RC_CLI_REJECT   0x02
#define MQTT_CONNACK_RC_SVR_UNAVBL   0x03
#define MQTT_CONNACK_RC_BAD_USRPWD   0x04
#define MQTT_CONNACK_RC_NOT_AUTHED   0x05
/**
   @endcond
*/

/** @defgroup lib_err_group LIBRARY Generated Error Codes
    Library provides these codes as return values in several routines

    @{
*/
#define MQTT_PACKET_ERR_NETWORK        (-1)  /**< Problem in network (sock err) */
#define MQTT_PACKET_ERR_TIMEOUT        (-2)  /**< Net transaction has timed out */
#define MQTT_PACKET_ERR_NET_OPS        (-3)  /**< Platform Net Ops un-available */
#define MQTT_PACKET_ERR_FNPARAM        (-4)  /**< Invalid parameter(s) provided */
#define MQTT_PACKET_ERR_PKT_AVL        (-5)  /**< No pkts are available in pool */
#define MQTT_PACKET_ERR_PKT_LEN        (-6)  /**< Inadequate free buffer in pkt */
#define MQTT_PACKET_ERR_NOTCONN        (-7)  /**< Lib isn't CONNECTED to server */
#define MQTT_PACKET_ERR_BADCALL        (-8)  /**< Irrelevant call for LIB state */
#define MQTT_PACKET_ERR_CONTENT        (-9)  /**< MSG / Data content has errors */
#define MQTT_PACKET_ERR_LIBQUIT       (-10)  /**< Needs reboot library has quit */
#define MQTT_PACKET_ERR_REMLSTN       (-11)  /**< No remote listener for socket */
#define MQTT_PACKET_ERR_MAXCONN       (-12)  /**< Max available conn reached    */
#define MQTT_PACKET_ERR_ALLOC_FAILED  (-13)  /**< Memory allication failed      */


#define MQTT_PACKET_ERR_NOT_DEF       (-32)  /**< Value other than defined ones */
/** @} */

/** @defgroup dev_netconn_opt_grp Options for platform to configure network
    @{
 */
#define MQTT_DEV_NETCONN_OPT_IP4                                   0x00  /**< Assert for IPv4                        */
#define MQTT_DEV_NETCONN_OPT_TCP                                   0x01  /**< Assert to indicate TCP net connection  */
#define MQTT_DEV_NETCONN_OPT_UDP                                   0x02  /**< Assert to create a local UDP port bind */
#define MQTT_DEV_NETCONN_OPT_IP6                                   0x04  /**< Assert for IPv6, otherwise it is IPv4  */
#define MQTT_DEV_NETCONN_OPT_URL                                   0x08  /**< Assert if the network address is a URL */
#define MQTT_DEV_NETCONN_OPT_SEC                                   0x10  /**< Assert to indicate a secure connection */

#define MQTT_DEV_NETCONN_OPT_SKIP_DOMAIN_NAME_VERIFICATION         0x20  /**< Assert to skip domain name verification*/
#define MQTT_DEV_NETCONN_OPT_SKIP_CERTIFICATE_CATALOG_VERIFICATION 0x40  /**< Assert to skip certificate catalog
                                                                              verification                           */
#define MQTT_DEV_NETCONN_OPT_SKIP_DATE_VERIFICATION                0x80  /**< Assert to skip date verification       */
 /** @} */

#define MQTT_MAX_PUBREL_INFLT 32

#define MQTT_KA_TIMEOUT_NONE 0xffffffff  /* Different than KA SECS = 0 */


//*****************************************************************************
// typedefs
//*****************************************************************************

/** @defgroup mqp_group MQTT Packet (MQP) Buffer structure
    The core construct to encapsulate, construct and process a message

    @{
*/
typedef struct _MQTT_Packet_t_
{
    uint8_t                 msgType;            /**< MQTT  Message  Type */
    uint8_t                 fhByte1;            /**< Fixed Header: Byte1 */

    uint16_t                msgID;              /**< Msg transaction  ID */

    uint8_t                 nRefs;              /**< # users of this msg */
    uint8_t                 pad[3];

    uint8_t                 offset;             /**< Start of data index */
    uint8_t                 fhLen;              /**< Fix Header Length   */
    uint16_t                vhLen;              /**< Var Header Length   */
    uint32_t                plLen;              /**< Pay Load   Length   */

    uint32_t                sessionDataPresent;

    uint32_t                maxlen;             /**< Maximum buffer size */
    uint8_t                 *buffer;            /**< The attached buffer */

    /** Method to free this packet to a particular pool */
    void                    (*free)(struct _MQTT_Packet_t_ *mqp);

    struct _MQTT_Packet_t_  *next;
}MQTT_Packet_t;

/** Description of UTF8 information as used by MQTT Library. */
typedef struct _MQTT_UTF8String_t_
{
    const char *buffer;   /**< Refers to UTF8 content */
    uint16_t   length;   /**< Length of UTF8 content */

}MQTT_UTF8String_t;

/*
   Wait-List of MQTT Messages for which acknowledge is pending from remote node.
*/
typedef struct _MQTT_AckWlist_t_
{
    MQTT_Packet_t *head;  /* Points to head of single linked-list. */
    MQTT_Packet_t *tail;  /* Points to tail of single linked-list. */

}MQTT_AckWlist_t;

/** MQTT Quality of Service */
typedef enum
{
    MQTT_QOS0 = 0,  /**< QoS Level 0 */
    MQTT_QOS1 = 1,  /**< QoS Level 1 */
    MQTT_QOS2 = 2   /**< QoS Level 2 */

}MQTT_QOS;

/** Construct to create Topic to SUBSCRIBE */
typedef struct _MQTT_UTF8StrQOS_t_
{

    const char *buffer;  /**< Refers to UTF8 content */
    uint16_t   length;   /**< Length of UTF8 content */
    MQTT_QOS   qosreq;   /**< QoS Level  for content */

}MQTT_UTF8StrQOS_t;


/** @defgroup mqtt_netsec_grp Information to establish a secure connection.
    This is implementation specific and is targeted for the network services.

    Specifically, the MQTT implementation makes no assumption or use of this
    construct. The client library merely passes information from the app to
    the network service layer.
    Note: value of nFile can vary from 1 to 4, with corresponding pointers to
    the files in files field. Value of 1(nFile) will assume the corresponding
    pointer is for CA File Name. Any other value of nFile expects the files to
    be in following order:
    1.  Private Key File
    2.  Certificate File Name
    3.  CA File Name
    4.  DH Key File Name

example:
If you want to provide only CA File Name, following are the two way of doing it:
for nFile = 1
char *security_file_list[] = {"/cert/testcacert.der"};
for nFile = 4
char *security_file_list[] = {NULL, NULL, "/cert/testcacert.der", NULL};

where files = security_file_list
    @{
*/
typedef struct _MQTT_SecureConn_t_
{
    void     *method;  /**< Reference to information about protocol or methods */
    void     *cipher;  /**< Reference to information about cryptography ciphers*/
    uint32_t nFile;    /**< Count of secure connection related files, certs... */
    char     **files;  /**< Reference to array of file-names used for security */

}MQTT_SecureConn_t;

/** @} */

/** @defgroup net_ops_group Abstraction of Network Services on a platform
    Services to enable the MQTT Client-Server communication over network

    These services are invoked by the MQTT Library.

    @{
*/
typedef struct _MQTT_DeviceNetServices_t_
{


    /** Set up a communication channel with a server or set up a local port.
        This routine opens up either a "connection oriented" communication
        channel with the specified server or set up a local configuration for
        "connectionless" transactions.

        @param[in] nwconnOpts Implementation specific construct to enumerate
        server address and / or connection related details
        @param[in] serverAddr URL or IP address (string) or other server
        reference. For setting up a local (UDP) port, set it to NULL.
        @param[in] portNumber Network port number, typically, 1883 or 8883
        for remote severs. For setting up a local (UDP) port, use an intended
        port number.
        @param[in] nwSecurity Information to establish a secure connection
        with server. Set it to NULL, if not used. @ref mqtt_netsec_grp
        @return a valid handle to connection, otherwise NULL
    */
    int32_t (*open)(uint32_t nwconnOpts, const char *serverAddr, uint16_t portNumber,
                const MQTT_SecureConn_t *nwSecurity);

    /** Send data onto the "connection oriented" network.
        The routine blocks till the time, the data has been copied into the
        network stack for dispatch on to the "connection oriented" network.

        @param[in] comm handle to network connection as returned by open().
        @param[in] buf refers to the data that is intended to be sent
        @param[in] len length of the data
        @param[in] ctx reference to the MQTT connection context
        @return on success, the number of bytes sent, 0 on connection reset,
        otherwise -1
    */
    int32_t   (*send)(int32_t comm, const uint8_t *buf, uint32_t len, void *ctx);

    /** Receive data from the "connection oriented" channel.
        The routine blocks till the time, there is either a data that has
        been received from the server or the time to await data from the
        server has expired.

        @param[in] comm Handle to network connection as returned by
        accept().
        @param[out] buf place-holder to which data from network should be
        written into.
        @param[in] len maximum length of 'buf'
        @param[in] waitSecs maximum time to await data from network. If
        exceeded, the routine returns error with the errTimeout flag set
        as true.
        @param[out] errTimeout if set, indicates that error is due to
        timeout.
        @param[in] ctx reference to the MQTT connection context
        @return on success, number of bytes received, 0 on connection reset,
        otherwise -1 on error. In case, error (-1) is due to the time-out,
        then the implementation should set flag errTimeout as true.
    */
    int32_t   (*recv)(int32_t comm, uint8_t *buf, uint32_t len, uint32_t waitSecs,
                  bool *errTimeout, void *ctx);

    /** Send data to particular port on the specified network element.
        The routine blocks till the time, the data has been copied into the
        network stack for dispatch to the "specified" network entity.

        @param[in] comm handle to network connection as returned by open().
        @param[in] buf refers to data that is intended to be sent
        @param[in] len length of the data
        @param[in] destPort network port to which data is to be sent.
        @param[in] destIP IP address of the entity to which data is to be
        sent.
        @param[in] ipLen length of the destination IP address.
        @return on success, the number of bytes sent, 0 on connection reset,
        otherwise -1.
    */
    int32_t   (*sendTo)(int32_t comm, const uint8_t *buf, uint32_t len, uint16_t destPort,
                       const uint8_t *destIP, uint32_t ipLen);

    /** Receive data on a local port sent by any network element.
        The routine blocks till the time, data has been received on the local
        port from any remote network element.

        @param[in] comm handle to network connection as return by open().
        @param[in] buf place-holder to which data from network should be
        written into.
        @param[in] len maximum length of 'buf'
        @param[out] fromPort place-holder for the port of the sender network
        entity
        @param[out] fromIP place-holder to retrieve the IP address of the
        sender network entity. The memory space must be provisioned to store
        at least 16 bytes.
        @param[in, out] ipLen length of IP address. It is provided by
        the caller to declare the length of the place holder and updated by
        routine to indicate the length of the remote network entity's IP
        address.
        @return on success, number of bytes received, 0 on connection reset,
        otherwise -1 on error.
    */
    int32_t   (*recvFrom)(int32_t comm, uint8_t *buf, uint32_t len, uint16_t *fromPort,
                       uint8_t *fromIP, uint32_t *ipLen);

    /** Close communication connection */
    int32_t   (*close)(int32_t comm);

    /** Listen to incoming connection from clients.
        This routine prepares the system to listen on the specified port
        for the incoming network connections from the remote clients.

        @param[in] nwconnOpts Implementation specific construct to
        enumerate server address and / or connection related details
        @param[in] portNumber Network port number, typically, 1883 or 8883
        @param[in] nwSecurity Information to establish a secure connection
        with client. Set it to NULL, if not used. @ref mqtt_netsec_grp
        @return a valid handle to listening construct, otherwise NULL
    */
    int32_t (*listen)(uint32_t nwconnOpts, uint16_t portNumber,
                  const MQTT_SecureConn_t *nwSecurity);

    /** Accept an incoming connection.
        This routine creates a new communication channel for the (remote)
        requesting client.

        @param[in] nwconnInfo parameter holds if the connection needs to be
        secured or not.
        @param[in] listen handle to listen for the incoming connection
        requests from the remote clients.
        @param[out] clientIP IP address of the connected client. This value
        is valid only on successful return of the routine. The place holder
        must provide memory to store at least 16 bytes.
        @param[in, out] ipLen Length of IP address. It is provided by
        the caller to declare the length of the place holder and updated by
        routine to indicate the length of the connected client's IP address.
        @return on success, a valid handle to the new connection, otherwise
        NULL
    */
    int32_t (*accept)(uint32_t nwconnInfo, int32_t listen, uint8_t *clientIP, uint32_t *ipLen);

    /** Monitor activity on communication handles.
        The routine blocks for the specified period of time to monitor
        activity, if any, on each of the communication handle that has
        been provided in one or more vector sets. At the expiry of the
        wait time, this function must identify the handles, on which,
        activities were observed.

        A particular collection of communication handles are packed as
        an array or in a vector and is passed to the routine. A NULL
        handle in the vector indicates the termination of the vector
        and can effectively used to account for the size of the vector.

        Similarly, at end the end of the wait period, the routine must
        provide a vector of the handles for which activity was observed.

        @param[in, out] recv_hvec a vector of handles which must be
        monitored for receive activities.
        @param[in, out] send_hvec a vector of handles which must be
        monitored for send activities.
        @param[in, out] rsvd_hvec reserved for future use.
        @param[in] waitSecs time to wait and monitor activity on
        communication handles provided in one or more sets. If set
        to 0, the routine returns immediately.
        @return on success, the total number of handles for which activity
        was observed. This number can be 0, if no activity was observed on
        any of the provided handle in the specified time. Otherwise, -1 on
        error.
    */
    int32_t   (*ioMon)(int32_t *recvCvec, int32_t *sendCvec,
                    int32_t *rsvdCvec,  uint32_t waitSecs);

    /** Get Time (in seconds).
        Provides a monotonically incrementing value of a time  service in
        unit of seconds. The implementation should ensure that associated
        timer hardware or the clock module remains active through the low
        power states of the system. Such an arrangement ensures that MQTT
        Library is able to track the Keep-Alive time across the cycles of
        low power states. It would be typical of battery operated systems
        to transition to low power states during the period of inactivity
        or otherwise to conserve battery.

        In the absence of a sustained time reference across the low power
        states, if the system transitions away from the active state, the
        MQTT Library, then may not be able to effectively monitor the Keep
        Alive duration.

        It is the responsibility of the implementation to manage the roll-
        over problem of the hardware and ensure the integrity of the time
        value is maintained.

       @return time in seconds
    */
    uint32_t   (*time)(void);

}MQTT_DeviceNetServices_t;

/** @} */ /* MQTT_DeviceNetServices_t */

/* List structure which hold the QoS2 PUB RX Message-IDs */
typedef struct _MQTT_ListPubQOS2CQ_t_
{
    uint16_t msgID;                      /* Buffer to store RX Message-IDs */
    struct _MQTT_ListPubQOS2CQ_t_ *next; /* Index to next Message-ID       */
    struct _MQTT_ListPubQOS2CQ_t_ *prev; /* Index to previous Message-ID   */
}MQTT_ListPubQOS2CQ_t;


/* Data structure for managing the QoS2 PUB RX packets and follow-ups */
/* Circular Queue CQ to track QOS2 PUB RX messages */
typedef struct _MQTT_PubQOS2CQ_t_
{
    MQTT_ListPubQOS2CQ_t *idVec;  /* Vector to store RX Message-IDs */
    MQTT_ListPubQOS2CQ_t *rdIdx;  /* Index to Read  next Message-ID */
    MQTT_ListPubQOS2CQ_t *wrIdx;  /* Index to Write next Message-ID */
    uint8_t  nFree;               /* Num of free elements in vector */
}MQTT_PubQOS2CQ_t;

typedef struct _MQTT_ClientCtx_t_
{
    void                        *usr;  /* Client Usr */
    int32_t                     net;  /* Socket HND */

    uint8_t                     remoteIP[16];
    uint32_t                    ipLen;

    uint32_t                    timeout;
    uint16_t                    kaSecs;

    uint32_t                    flags;

    struct _MQTT_ClientCtx_t_   *next;

}MQTT_ClientCtx_t;


//*****************************************************************************
// function prototypes
//*****************************************************************************

static inline uint32_t MQTT_bufWrNbytes(uint8_t *dst, const uint8_t *src, uint32_t n)
{
    uint32_t c = n;

    while(c--)
    {
        *dst++ = *src++;
    }
    return n;
}

static inline uint32_t MQTT_bufSet(uint8_t *dst, uint8_t val, uint32_t n)
{
    uint32_t c = n;

    while(c--)
    {
        *dst++ = val;
    }
    return n;
}

/** Writing 2 bytes entity in network byte order */
static inline uint32_t MQTT_bufWrNbo2B(uint8_t *buf, uint16_t val)
{
    buf[0] = (uint8_t)((val >> 8) & 0xFF); /* MSB */
    buf[1] = (uint8_t)((val)      & 0xFF); /* LSB */
    return 2;
}

/** Reading 2 bytes entity in network byte order */
static inline uint32_t MQTT_bufRdNbo2B(const uint8_t *buf, uint16_t *val)
{
    if (buf != NULL)
    {
        *val = (uint16_t)((buf[0] << 8) | (buf[1]));
        return 2;
    }
    else
    {
        return 0;
    }
}


/*---------------------------------------------------------------------
 * Common Operations
 *---------------------------------------------------------------------
 */

/** Free a MQTT Packet Buffer
    Puts back the packet buffer in to the appropriate pool.

    @param[in] mqp packet buffer to be freed
    @return none
*/
void MQTT_packetFree(MQTT_Packet_t *mqp);

/** Resets the attributes of MQTT Packet Holder to its init state
    Not all fields are reset - entities such as offset, nRefs in addition
    to buffer information are not updated.

    @param[in] mqp packet buffer to be reset
    @return none

    @see MQTT_packetInit
*/
void MQTT_packetReset(MQTT_Packet_t *mqp);

/** Initializes attributes of the MQTT Packet Holder.
    This routine sets number of users of the MQTT Packet Holder to 1. However,
    it leaves, if already provisioned, the reference to buffer and its size
    un-altered.

    @param[in] mqp packet buffer to be initialized
    @param[in] offset index in buffer to indicate start of the contents
    @return none
*/
void MQTT_packetInit(MQTT_Packet_t *mqp, uint8_t offset);

/** Initialize MQTT Packet Holder and attach the buffer */
static
inline void MQTT_packetBufferAttach(MQTT_Packet_t *mqp, uint8_t *buffer, uint32_t length,
                              uint8_t offset)
{
    MQTT_packetInit(mqp, offset);

    mqp->buffer = buffer;
    mqp->maxlen = length;
    mqp->free   = NULL;

    return;
}

/** Write UTF8 information into the buffer.
    The UTF8 information includes content and its length.

    @warning The routine does not check for correctness of the parameters.

    @param[in] buf refers to memory to write UTF8 information into
    @param[in] utf8 contains UTF8 information to be written
    @return on success, number of bytes written, otherwise -1 on error.
*/
int32_t MQTT_packetBufWrUtf8(uint8_t *buf, const MQTT_UTF8String_t *utf8);

/** Write the MQTT construct 'Remaining Length' into trailing end of buffer.
    The 'remaining length' is written in the format as outlined in the MQTT
    specification.

    The implementation assumes availability of at-least 4 bytes in the buffer.
    Depending on the value of 'Remaining Length' appropriate trailing bytes in
    the buffer would be used.

    @param[in] buf refers to memory to tail-write 'Remaining Length' into
    @param[in] remlen The 'Remaining Length' value
    @return in success, number of trailing bytes used, otherwise -1 on error
*/
int32_t MQTT_packetBufTailWrRemlen(uint8_t *buf, uint32_t remlen);

/** Read MQTT construct 'Remaining Length' from leading bytes of the buffer.
    The 'remaining length' is written in the format as outlined in the MQTT
    specification.

    @param[in] buf refers to memory to head-read 'Remaining Length' from
    @param[in] remlen place-holder for The 'Remaining Length' value
    @return in success, number of header bytes read, otherwise -1 on error
*/
int32_t MQTT_packetBufRdRemlen(uint8_t *buf, uint32_t *remlen);

/** Include variable header Topic as part of PUB Message construction.
    Inclusion of a Topic also encompasses incorporation of the message ID.

    The topic refers to the subject for which data will be published by
    the client or the server. The topic entity must be appended into the
    packet buffer prior to the inclusion of the payload (data).

    @warning This routine does not check for correctness of the input
    parameters.

    @param[in] mqp packet buffer in which topic must be included.
    @param[in] topic UTF8 information
    @param[in] msgID Message or Packet transaction ID
    @return on success, number of bytes appended, otherwise -1 on error.

    @note A 'topic' must be appended prior to inclusion of pulished data.
*/
int32_t
MQTT_packetPubAppendTopic(MQTT_Packet_t *mqp, const MQTT_UTF8String_t *topic,
                     uint16_t msgID);

/** Include payload data for publishing
    The payload data is associated with a topic.

    @warning This routine does not check for correctness of the input
    parameters.

    @param[in] mqp packet buffer in which payload data must be included.
    @param[in] dataBuf data to be included in the packet buffer
    @param[in] dataLen length of the data to be included in the packet.
    @return on success, number of bytes appended, otherwise -1 on error.

    @note A 'topic' must be appended prior to inclusion of pulished data.
*/
int32_t MQTT_packetPubAppendData(MQTT_Packet_t *mqp, const uint8_t *dataBuf,
                        uint32_t dataLen);

/** Construct a packet for Message ID enabled ACK received from network
    Process the raw ACK message information to update the packet holder.

    @warning This routine does not check for correctness of the input
    parameters.

    @param[in] mqpRaw holds a raw buffer from the network
    @param[in] hasPayload asserted, if ACK message should have a payload
    @return on success, true, otherwise false
*/
bool MQTT_packetProcMsgIdAckRx(MQTT_Packet_t *mqpRaw, bool hasPayload);

/** Construct a packet for PUBLISH message received from the network
    Process the raw PUB message information to update the packet holder.

    @warning This routine does not check for correctness of the input
    parameters.

    @param[in] mqpRaw holds a raw buffer from the network
    @return on success, true, other wise false
*/
bool MQTT_packetProcPubRx(MQTT_Packet_t *mqpRaw);

/*
   Add specified element into trailing end of list.

   Returns, on success, true, otherwise false.
*/
bool MQTT_packetAckWlistAppend(MQTT_AckWlist_t *list, MQTT_Packet_t *elem);

/*
   Removes element that has specified msgID from list.

   Returns, on success, pointer to removed element, otherwise NULL.
*/
MQTT_Packet_t *MQTT_packetAckWlistRemove(MQTT_AckWlist_t *list,
                                          uint16_t msgID);
/*
   Removes and frees all elements in list.
*/
void MQTT_packetAckWlistPurge(MQTT_AckWlist_t *list);


/** Prepare the Fixed-Header of the MQTT Packet (before being sent to network)
    Based on the contents of the mqtt packet and the combination of DUP, QoS
    and Retain flags as outlined the MQTT specification, the routine updates,
    among others, significant internal fields such as 'remaining length' and
    'fixed header length' in the packet construct and embeds the fixed header,
    so created, in the packet buffer.

    This service must be utilized on a packet that has been already populated
    with all the payload data, topics and other contents. The fixed header
    must be the final step in the composition of MQTT packet prior to its
    dispatch to the server.

    Returns size, in bytes, of the fixed-header, otherwise -1 on error.
*/
int32_t MQTT_packetPrepFh(MQTT_Packet_t *mqp, uint8_t flags);

/* Receive data from the specified network and read into the 'mqp' */
int32_t MQTT_PacketRecv(int32_t  net,     const MQTT_DeviceNetServices_t *netOps,
             MQTT_Packet_t *mqp, uint32_t waitSecs, bool *timedOut,void *ctx);

/* Initialize the MQTT_SecureConn_t data */
void MQTT_secureConnStructInit(MQTT_SecureConn_t *nwSecurity);

/* Reset the specified Circular Queue (CQ) */
void MQTT_qos2PubCqReset(MQTT_PubQOS2CQ_t *cq);

/* Append the message-id into the CQ tail. Return true on success, else false */
bool MQTT_qos2PubCqLogup(MQTT_PubQOS2CQ_t *cq, uint16_t msgID);

/* Remove the message-id from the CQ head. Return true on success, else false */
bool MQTT_qos2PubCqUnlog(MQTT_PubQOS2CQ_t *cq, uint16_t msgID);

/* Is the message-id available in the CQ ? Return true on success, else false */
bool MQTT_qos2PubCqCheck(MQTT_PubQOS2CQ_t *cq, uint16_t msgID);

/* Get the count of message-ID(s) available in the CQ */
static inline int32_t MQTT_qos2PubCqCount(MQTT_PubQOS2CQ_t *cq)
{
    return (MQTT_MAX_PUBREL_INFLT - cq->nFree);
}

void MQTT_clCtxReset(MQTT_ClientCtx_t *clCtx);

void MQTT_clCtxTimeoutInsert(MQTT_ClientCtx_t **head,MQTT_ClientCtx_t *elem);

void MQTT_clCtxRemove(MQTT_ClientCtx_t **head, MQTT_ClientCtx_t *elem);

void MQTT_clCtxTimeoutUpdate(MQTT_ClientCtx_t *clCtx, uint32_t nowSecs);

#ifdef __cplusplus
}
#endif

#endif
