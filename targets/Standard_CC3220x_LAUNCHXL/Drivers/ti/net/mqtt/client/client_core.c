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
 client_core.c

 The module provides implementation to the public interface of the MQTT
 Client Library.
 */

//*****************************************************************************
// includes
//*****************************************************************************
#include "client_core.h"


//*****************************************************************************
//defines
//*****************************************************************************

#define MQTTCLIENTCORE_LOOP_DLEN  sizeof(MQTTClientCore_loopData)
#define MQTTCLIENTCORE_VH_MSG_LEN 4

#ifndef CFG_CL_MQTT_CTXS
#define MQTTCLIENTCORE_MAX_NWCONN 4
#else
#define MQTTCLIENTCORE_MAX_NWCONN CFG_CL_MQTT_CTXS
#endif

#define MQTTCLIENTCORE_CLEAN_SESSION_FLAG    0x00010000
#define MQTTCLIENTCORE_CONNACK_AWAIT_FLAG    0x00020000
#define MQTTCLIENTCORE_NOW_CONNECTED_FLAG    0x00040000
#define MQTTCLIENTCORE_KA_PINGER_RSP_FLAG    0x00080000
#define MQTTCLIENTCORE_USER_PING_RSP_FLAG    0x00100000
#define MQTTCLIENTCORE_NETWORK_CLOSE_FLAG    0x00200000
#define MQTTCLIENTCORE_DO_CONNACK_TO_FLAG    0x00400000
#define MQTTCLIENTCORE_APP_RECV_TASK_FLAG    MQTTCLIENTCORE_CFG_APP_HAS_RTSK
#define MQTTCLIENTCORE_GROUP_CONTEXT_FLAG    MQTTCLIENTCORE_CFG_MK_GROUP_CTX
#define MQTTCLIENTCORE_USE_PROTO_V31_FLAG    MQTTCLIENTCORE_CFG_PROTOCOL_V31

#define MQTTClientCore_isProtoVer31(clCtx)   (clCtx->flags & MQTTCLIENTCORE_USE_PROTO_V31_FLAG)
#define MQTTClientCore_awaitsConnack(clCtx)  (clCtx->flags & MQTTCLIENTCORE_CONNACK_AWAIT_FLAG)
#define MQTTClientCore_hasConnection(clCtx)  (clCtx->flags & MQTTCLIENTCORE_NOW_CONNECTED_FLAG)
#define MQTTClientCore_awaitsKAPing(clCtx)   (clCtx->flags & MQTTCLIENTCORE_KA_PINGER_RSP_FLAG)
#define MQTTClientCore_awaitsPingRsp(clCtx)  (clCtx->flags & MQTTCLIENTCORE_USER_PING_RSP_FLAG)
#define MQTTClientCore_isClnSession(clCtx)   (clCtx->flags & MQTTCLIENTCORE_CLEAN_SESSION_FLAG)
#define MQTTClientCore_recvTaskAvbl(clCtx)   (clCtx->flags & MQTTCLIENTCORE_APP_RECV_TASK_FLAG)
#define MQTTClientCore_groupMember(clCtx)    (clCtx->flags & MQTTCLIENTCORE_GROUP_CONTEXT_FLAG)
#define MQTTClientCore_needNetClose(clCtx)   (clCtx->flags & MQTTCLIENTCORE_NETWORK_CLOSE_FLAG)
#define MQTTClientCore_cfgConnackTo(clCtx)   (clCtx->flags & MQTTCLIENTCORE_DO_CONNACK_TO_FLAG)
#define MQTTClientCore_ctxCBsPtr(clCtx)     &(CLIENT(clCtx)->appCtxCBs)
#define CLIENT(clCtx)                        ((MQTTClientCore_ClientDesc_t *)clCtx)

/* Only if MQP has good RX data i.e. this macro shouldn't be used for bad RX */
#define MQTTClientCore_mqpRxDoNotRptSet(mqp) (mqp->sessionDataPresent = 1)

#define MQTTClientCore_retIfinvalidUTF8(utf8)                                     \
        if(false == isValidUTF8String(utf8)) {return -1;}


#define MQTTClientCore_mutexLockin() if(MQTTClientCore_mutex_lockin)              \
        MQTTClientCore_mutex_lockin(MQTTClientCore_mutex);
#define MQTTClientCore_mutexUnlock() if(MQTTClientCore_mutex_unlock)              \
        MQTTClientCore_mutex_unlock(MQTTClientCore_mutex);
#define MQTTClientCore_txPartInUse(txPart) (txPart->length - txPart->offset)

/* Some servers terminate connection right at expiration of keep-alive.
 Strictly for the debug purposes, the following macro can be used to
 add to the termination period */
#define MQTTCLIENTCORE_KA_NW_ADD_DBG         0  /* Seconds */


//*****************************************************************************
// typedefs
//*****************************************************************************

/* Buffer Pool and management, other registrations and initialization */
typedef enum
{
    ModuleState_WaitInit = 0x00,
    ModuleState_InitDone = 0x01
}MQTTClientCore_ModuleStates;

/* Construct to manage TX for network that requires LIB to send a partial and
 incremental data to support restrictive TCP segments. Specifically, for the
 deployments, in which the network layer supports small segments, there can
 be only one in-flight message.
 */
typedef struct _MQTTClientCore_txPartPkt_t_
{

    /* Refers to MQP, if TX for it is active, otherwise, set it to NULL */
    MQTT_Packet_t *mqpTx;

    union
    {
        uint8_t vhMsg[MQTTCLIENTCORE_VH_MSG_LEN]; /* VH msg for MQP = NULL */
        const uint8_t *buffer; /* Refers to data in MQP */
    };

    uint32_t length; /* Length of entire data */
    uint32_t offset; /* Next data for sending */
}MQTTClientCore_txPartPkt_t;


typedef struct _MQTTClientCore_ClientDesc_t_
{

    /* ALERT:  "context" must be first elem in this structure, do not move */
    MQTT_ClientCtx_t context;


    /* Order/Sequence: Client ID, Will Topic, Will Msg, Username, Password */
    const MQTT_UTF8String_t *connPayloadUTF8[5]; /* Ref: CONNECT Payload */
    uint8_t willOpts;

    /* Wait-List for Level 1 ACK(s), which are PUBACK, PUBREC, UN/SUBACK */
    MQTT_AckWlist_t qosAck1WaitList;

    /* Circular queue to track QOS2 PUBLISH packets from the server. They
     are tracked for the duration of PUBLISH-RX to PUBREL-RX.
     */
    MQTT_PubQOS2CQ_t rxQos2CircularQueue;

    /* Circular queue to track QOS2 PUBLISH packets from the client. They
     are tracked for the duration of PUBREC-RX to PUBOMP-RX.
     */
    MQTT_PubQOS2CQ_t txQos2CircularQueue;

    MQTTClientCore_CtxCBs_t appCtxCBs; /* Callback funcs from app */

    MQTTClientCore_txPartPkt_t txPart;/* Reference to partial TX PKT */
    MQTT_Packet_t *mqpRx; /* Reference to partial RX PKT */
    void *app;

    uint32_t nwconnOpts;
    char *serverAddr;
    uint16_t portNumber;
    MQTT_SecureConn_t nwSecurity;

}MQTTClientCore_ClientDesc_t;

//*****************************************************************************
//globals
//*****************************************************************************

static MQTT_Packet_t *MQTTClientCore_freeList = NULL;

MQTTClientCore_ClientDesc_t clients[MQTTCLIENTCORE_MAX_NWCONN];

static pthread_mutex_t *MQTTClientCore_mutex = NULL;
static void (*MQTTClientCore_mutex_lockin)(pthread_mutex_t *) = NULL;
static void (*MQTTClientCore_mutex_unlock)(pthread_mutex_t *) = NULL;

static const MQTT_DeviceNetServices_t *MQTTClientCore_netOps = NULL;

static uint16_t MQTTClientCore_msgID = 0xFFFF;

static int32_t MQTTClientCore_loopbNet = -1;
static const uint8_t MQTTClientCore_loopData[] = { 0x00, 0x01 };
static uint16_t MQTTClientCore_loopbPortid = 0;
static bool MQTTClientCore_grpHasCBFn = false;

static MQTT_ClientCtx_t *MQTTClientCore_freeCtxs = NULL; /* CTX construct available */
static MQTT_ClientCtx_t *MQTTClientCore_usedCtxs = NULL; /* Relevant only for group */
static MQTT_ClientCtx_t *MQTTClientCore_connCtxs = NULL; /* Relevant only for group */

/* Define protocol information for the supported versions */
static uint8_t MQTTClientCore_mqtt310[] = { 0x00, 0x06, 'M', 'Q', 'I', 's', 'd', 'p', 0x03 };
static uint8_t MQTTClientCore_mqtt311[] = { 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04 };

/* GROUP LISTEN PORT + VEC END */
static int32_t MQTTClientCore_recvHvec[MQTTCLIENTCORE_MAX_NWCONN + 1 + 1];
static int32_t MQTTClientCore_sendHvec = -1;
static int32_t MQTTClientCore_rsvdHvec = -1;

static MQTTClientCore_ModuleStates MQTTClientCore_clLibState = ModuleState_WaitInit;

extern int32_t MQTTClient_closeContext;

static inline uint16_t assignNewMsgID(void)
{
    return (MQTTClientCore_msgID += 2);
}

static bool txPartSetup(MQTTClientCore_txPartPkt_t *txPart, const uint8_t *buffer, uint32_t length, MQTT_Packet_t *mqpTx)
{
    if (mqpTx)
    {
        txPart->buffer = buffer;
    }
    else
    {
        if (MQTTCLIENTCORE_VH_MSG_LEN < length)
        {
            return false;
        }
        MQTT_bufWrNbytes(txPart->vhMsg, buffer, length);
    }

    txPart->length = length;
    txPart->mqpTx = mqpTx;

    return true;
}

static void _txPartReset(MQTTClientCore_txPartPkt_t *txPart)
{
    txPart->vhMsg[0] = 0x00;
    txPart->mqpTx = NULL;
    txPart->length = 0;
    txPart->offset = 0;
}

static void txPartReset(MQTTClientCore_txPartPkt_t *txPart)
{
    MQTT_Packet_t *mqpTx = txPart->mqpTx;

    if (mqpTx)
    {
        MQTT_packetFree(mqpTx);
    }
    _txPartReset(txPart);

    return;
}

static const uint8_t *txPartBufP(MQTTClientCore_txPartPkt_t *txPart)
{
    MQTT_Packet_t *mqpTx = txPart->mqpTx;
    uint32_t offset = txPart->offset;

    return (mqpTx ? txPart->buffer + offset : txPart->vhMsg + offset);
}

static void txPartAddup(MQTTClientCore_txPartPkt_t *txPart, uint32_t offset)
{
    txPart->offset += offset;
}

static void clCtxFreeup(MQTT_ClientCtx_t *clCtx)
{
    clCtx->next = MQTTClientCore_freeCtxs;
    MQTTClientCore_freeCtxs = clCtx;

    return;
}

static void clientReset(MQTTClientCore_ClientDesc_t *client)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = &client->appCtxCBs;
    int32_t i = 0;

    MQTT_clCtxReset((MQTT_ClientCtx_t *)(client));

    for (i = 0; i < 5; i++)
    {
        client->connPayloadUTF8[i] = NULL;
    }
    client->willOpts = 0;

    MQTT_packetAckWlistPurge(&client->qosAck1WaitList);

    MQTT_qos2PubCqReset(&client->rxQos2CircularQueue);
    MQTT_qos2PubCqReset(&client->txQos2CircularQueue);

    ctxCBs->publishRx = NULL;
    ctxCBs->ackNotify = NULL;
    ctxCBs->disconnCB = NULL;

    txPartReset(&client->txPart);
    client->mqpRx = NULL;
    client->app = NULL;

    client->nwconnOpts = 0;
    client->serverAddr = NULL;
    client->portNumber = 0;

    MQTT_secureConnStructInit(&client->nwSecurity);

    return;
}

static void clientDescInit(void)
{
    int32_t i = 0;
    MQTTClientCore_ClientDesc_t *client;
    MQTT_ClientCtx_t *clCtx;

    for (i = 0; i < MQTTCLIENTCORE_MAX_NWCONN; i++)
    {
        client = clients + i;
        clCtx = (MQTT_ClientCtx_t *)(client);

        /* Initialize certain fields to defaults */
        client->qosAck1WaitList.head = NULL;
        client->qosAck1WaitList.tail = NULL;
        client->txPart.mqpTx = NULL;

        clientReset(client); /* Reset remaining */

        clCtx->next = MQTTClientCore_freeCtxs;
        MQTTClientCore_freeCtxs = clCtx;
    }

    return;
}

static void mqpFreeLocked(MQTT_Packet_t *mqp)
{
    MQTTClientCore_mutexLockin();
    MQTT_packetFree(mqp);
    MQTTClientCore_mutexUnlock();
}

//*****************************************************************************
//
//! \brief prevent certain good and non-callback MQP being reported to app
//!
//*****************************************************************************
static bool mqpDoNotReportCor(MQTT_Packet_t *mqp)
{
    bool rv = (1 == mqp->sessionDataPresent) ? true : false;

    mqp->sessionDataPresent = 0;
    return rv;
}

static int32_t loopbTrigger(void)
{
    uint8_t ip_addr[] = { 127, 0, 0, 1 };

    return ((-1 != MQTTClientCore_loopbNet) ?
            MQTTClientCore_netOps->sendTo(MQTTClientCore_loopbNet, MQTTClientCore_loopData,
            MQTTCLIENTCORE_LOOP_DLEN, MQTTClientCore_loopbPortid, ip_addr, 4) : MQTT_PACKET_ERR_LIBQUIT);
}

static void session311fix(MQTT_ClientCtx_t *clCtx)
{
    MQTT_AckWlist_t *wl = &CLIENT(clCtx)->qosAck1WaitList;
    MQTT_Packet_t *elem = wl->head;
    MQTT_Packet_t *next;

    while (elem)
    {
        next = elem->next;
        if (MQTT_PUBLISH != elem->msgType)
        {
            MQTT_packetAckWlistRemove(wl, elem->msgID);
        }
        elem = next;
    }

    return;
}

static void sessionDelete(MQTT_ClientCtx_t *clCtx)
{
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);

    /* Cleaning session */
    MQTT_qos2PubCqReset(&client->rxQos2CircularQueue);
    MQTT_qos2PubCqReset(&client->txQos2CircularQueue);

    MQTT_packetAckWlistPurge(&client->qosAck1WaitList);

    return;
}

//*****************************************************************************
//
//! \brief Routine to manage error conditions in client - close the network connection
//!
//*****************************************************************************
static void doNetClose(MQTT_ClientCtx_t *clCtx)
{
    MQTT_Packet_t *mqpRx = CLIENT(clCtx)->mqpRx;
    int32_t net = clCtx->net;

    if (-1 == net)
    {
        return; /* network closed already, must not happen */
    }
    if (MQTTClientCore_isClnSession(clCtx))
    {
        sessionDelete(clCtx);
    }
    else if (!MQTTClientCore_isProtoVer31(clCtx))
    {
        /* Version 3.1.1 doesn't need SUB and UNSUB re-send */
        session311fix(clCtx);
    }

    txPartReset(&CLIENT(clCtx)->txPart); /* Part TX, if any */

    CLIENT(clCtx)->mqpRx = NULL;
    if (mqpRx && mqpRx->free)
    {
        MQTT_packetFree(mqpRx);
    }
    clCtx->flags &= ~(MQTTCLIENTCORE_CONNACK_AWAIT_FLAG | MQTTCLIENTCORE_NOW_CONNECTED_FLAG |
    MQTTCLIENTCORE_KA_PINGER_RSP_FLAG | MQTTCLIENTCORE_USER_PING_RSP_FLAG |
    MQTTCLIENTCORE_NETWORK_CLOSE_FLAG | MQTTCLIENTCORE_DO_CONNACK_TO_FLAG);

    clCtx->net = -1;
    MQTTClientCore_netOps->close(net);

    return;
}

static void doNetCloseRx(MQTT_ClientCtx_t *clCtx, int32_t cause)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);

    /* RX closing */
    doNetClose(clCtx);
    if (ctxCBs->disconnCB)
    {
        ctxCBs->disconnCB(CLIENT(clCtx)->app, cause);
    }
    if (MQTTClientCore_groupMember(clCtx))
    {
        MQTT_clCtxRemove(&MQTTClientCore_usedCtxs, clCtx);
    }
    return;
}

static void doNetCloseTx(MQTT_ClientCtx_t *clCtx, char *cause)
{
    /* TX closing */
    if (MQTTClientCore_recvTaskAvbl(clCtx))
    {
        clCtx->flags |= MQTTCLIENTCORE_NETWORK_CLOSE_FLAG;
        if (MQTTClientCore_groupMember(clCtx))
        {
            loopbTrigger();
        }
    }
    else
    {
        doNetClose(clCtx); /* No RX Task, close now */
    }

    return;
}

/*----------------------------------------------------------------------------
 * QoS2 PUB RX Message handling mechanism and associated house-keeping
 *--------------------------------------------------------------------------*/

static bool qos2PubRxLogup(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    return MQTT_qos2PubCqLogup(&CLIENT(clCtx)->rxQos2CircularQueue, msgID);
}

static bool ack2MsgIdLogup(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    return MQTT_qos2PubCqLogup(&CLIENT(clCtx)->txQos2CircularQueue, msgID);
}

static bool qos2PubRxUnlog(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    return MQTT_qos2PubCqUnlog(&CLIENT(clCtx)->rxQos2CircularQueue, msgID);
}

static bool ack2MsgIdUnlog(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);
    MQTTClientCore_CtxCBs_t *ctxCBs;

    if (MQTT_qos2PubCqUnlog(&client->txQos2CircularQueue, msgID))
    {
        ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);
        if (ctxCBs->ackNotify)
        {
            ctxCBs->ackNotify(client->app, MQTT_PUBCOMP, msgID, NULL, 0);
        }
        return true;
    }

    return false;
}

static bool qos2PubRxIsDone(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    return MQTT_qos2PubCqCheck(&CLIENT(clCtx)->rxQos2CircularQueue, msgID);
}

static bool awaitsPkts(MQTT_ClientCtx_t *clCtx)
{
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);

    return (client->qosAck1WaitList.head || MQTT_qos2PubCqCount(&client->rxQos2CircularQueue) ||
            MQTT_qos2PubCqCount(&client->txQos2CircularQueue) ? true : false);
}

static inline int32_t lenErrFreeMqp(MQTT_Packet_t *mqp)
{
    mqpFreeLocked(mqp);
    return MQTT_PACKET_ERR_PKT_LEN;
}

static int32_t isValidUTF8String(const MQTT_UTF8String_t *utf8)
{
    /* Valid topic should be > 0 byte and must hosted in usable buffer */
    return ((utf8->length > 0) && (NULL != utf8->buffer)) ? true : false;
}

static bool isConnected(MQTT_ClientCtx_t *clCtx)
{
    return ((MQTTClientCore_hasConnection(clCtx) && !MQTTClientCore_needNetClose(clCtx))
            ? true : false);
}

/*----------------------------------------------------------------------------
 * MQTT TX Routines
 *--------------------------------------------------------------------------*/

static void usedCtxsTOSort(MQTT_ClientCtx_t *cl_ctx_TO)
{
    MQTT_clCtxRemove(&MQTTClientCore_usedCtxs, cl_ctx_TO);
    MQTT_clCtxTimeoutInsert(&MQTTClientCore_usedCtxs, cl_ctx_TO);
}

static inline int32_t netSend(int32_t net, const uint8_t *buf, uint32_t len, void *ctx)
{
    int32_t rv = MQTTClientCore_netOps->send(net, buf, len, ctx);

    if (rv <= 0)
    {
        rv = MQTT_PACKET_ERR_NETWORK;
    }
    return rv;
}

//*****************************************************************************
//
//! \brief this routine tries to free MQP On completion of the send of the packet
//!
//*****************************************************************************
static int32_t clCtxPartSend(MQTT_ClientCtx_t *clCtx)
{
    MQTTClientCore_txPartPkt_t *txPart = &CLIENT(clCtx)->txPart;
    const uint8_t *buf = txPartBufP(txPart);
    uint32_t len = MQTTClientCore_txPartInUse(txPart);

    int32_t rv = netSend(clCtx->net, buf, len, (void *)clCtx);
    if (rv > 0)
    { /* Do Follow-up for a good send */
        if (MQTTClientCore_hasConnection(clCtx))
        {
            /* Update TX timeout, if 'CTX' is connected */
            MQTT_clCtxTimeoutUpdate(clCtx, MQTTClientCore_netOps->time());

            /* After update, 'CTX'(s) sorting is a must */
            if (MQTTClientCore_groupMember(clCtx))
            {
                usedCtxsTOSort(clCtx);
            }
        }

        if (rv != len)
        {
            /* Partial data was sent */
            txPartAddup(txPart, rv);
        }
        else
        {
            txPartReset(txPart); /* Try Freeing MQP */
        }
    }
    else
    {
        doNetCloseTx(clCtx, "snd-err");
    }

    return rv;
}

static int32_t clCtxSeg1Send(MQTT_ClientCtx_t *clCtx, const uint8_t *buf, uint32_t len,
                                bool is_conn_msg, MQTT_Packet_t *mqpTx)
{
    MQTTClientCore_txPartPkt_t *txPart = &CLIENT(clCtx)->txPart;

    /* For CONNECT msg, a client context mustn't be already connected.
     For others msgs, a client context must be connected to server */
    if (false == (is_conn_msg ^ isConnected(clCtx)))
    {
        return MQTT_PACKET_ERR_NOTCONN;
    }
    if (MQTTClientCore_txPartInUse(txPart))
    {
        return MQTT_PACKET_ERR_BADCALL;
    }
    txPartSetup(txPart, buf, len, mqpTx);

    return clCtxPartSend(clCtx);
}

static int32_t wrConnectPayload(MQTT_ClientCtx_t *clCtx, uint8_t *buf, uint32_t fsz, uint8_t *conn_opts)
{
    /* UTF8 usage selection order: Client, W-Topic, W-Msg, User-Name, Pwd */
    uint8_t utf8_sel[] = { 0x00, MQTT_CONN_MSG_FLAG_WILL_CONFIG, 0x00, MQTT_CONN_MSG_FLAG_USER_NAME, MQTT_CONN_MSG_FLAG_PASS_WORD };
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);
    uint8_t *ref = buf;
    int32_t i = 0;
    const MQTT_UTF8String_t *utf8;

    for (i = 0; i < 5; i++)
    {
        uint16_t len = 2;
        utf8 = client->connPayloadUTF8[i];
        if (NULL == utf8)
        {
            /* UTF8 absent: Client ID (i = 0) and Will MSG (i = 2)
             set zero byte length in the CONNECT message */
            if (0 != i)
            {
                if (!((2 == i) && (*conn_opts & MQTT_CONN_MSG_FLAG_WILL_CONFIG)))
                {
                    continue; /* Others, just pass */
                }
            }
        }
        else
        {
            len += utf8->length;
        }

        if (fsz < (buf - ref + len))
        {
            return MQTT_PACKET_ERR_PKT_LEN; /* Payload: no space left */
        }
        if (2 == len)
        {
            buf += MQTT_bufWrNbo2B(buf, 0); /*  WR 0 byte length */
        }
        else
        {
            buf += MQTT_packetBufWrUtf8(buf, utf8);
        }

        *conn_opts |= utf8_sel[i]; /* Enable message flags */
    }

    return buf - ref;
}

static inline uint16_t getConnectVhLen(MQTT_ClientCtx_t *clCtx)
{
    return ((MQTTClientCore_isProtoVer31(clCtx) ?
            sizeof(MQTTClientCore_mqtt310) : sizeof(MQTTClientCore_mqtt311)) + 3);
}

static int32_t wrConnectVh(MQTT_ClientCtx_t *clCtx, uint8_t *buf, uint16_t kaSecs, uint8_t conn_opts)
{
    uint8_t *ref = buf;

    if (MQTTClientCore_isProtoVer31(clCtx))
    {
        buf += MQTT_bufWrNbytes(buf, MQTTClientCore_mqtt310, sizeof(MQTTClientCore_mqtt310));
    }
    else
    {
        buf += MQTT_bufWrNbytes(buf, MQTTClientCore_mqtt311, sizeof(MQTTClientCore_mqtt311));
    }
    *buf++ = conn_opts;
    buf += MQTT_bufWrNbo2B(buf, kaSecs + MQTTCLIENTCORE_KA_NW_ADD_DBG);

    return buf - ref;
}

static int32_t netConnect(MQTT_ClientCtx_t *clCtx)
{
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);

    if (MQTTClientCore_needNetClose(clCtx))
    {
        return MQTT_PACKET_ERR_NOT_DEF;
    }
    if (NULL == MQTTClientCore_netOps)
    {
        return MQTT_PACKET_ERR_NET_OPS;
    }
    clCtx->net = MQTTClientCore_netOps->open(client->nwconnOpts | MQTT_DEV_NETCONN_OPT_TCP,
                client->serverAddr, client->portNumber, &client->nwSecurity);

    return clCtx->net;
}

static int32_t clCtxConnStateTryLocked(MQTT_ClientCtx_t *clCtx, const uint8_t *buf, uint32_t len, uint16_t kaSecs, bool cleanSession, MQTT_Packet_t *mqpTx)
{
    int32_t rv;

    MQTTClientCore_mutexLockin();
    rv = netConnect(clCtx);
    if (rv >= 0)
    {
        /* Ensure LIB is initialized & CTX isn't awaiting CONNACK */
        rv = MQTT_PACKET_ERR_BADCALL;
        if (false == ((ModuleState_InitDone != MQTTClientCore_clLibState) || (MQTTClientCore_awaitsConnack(clCtx))))
        {
            rv = clCtxSeg1Send(clCtx, buf, len, true, mqpTx);
        }
        if (rv >= 0)
        {
            /* Successfully sent CONNECT msg - let's do housekeeping */
            clCtx->timeout = MQTTClientCore_netOps->time();/* Fixup: CONN TX Time */
            clCtx->flags |= MQTTCLIENTCORE_DO_CONNACK_TO_FLAG | MQTTCLIENTCORE_CONNACK_AWAIT_FLAG;
            clCtx->flags |= cleanSession ? MQTTCLIENTCORE_CLEAN_SESSION_FLAG : 0;

            clCtx->kaSecs = kaSecs;

            if (MQTTClientCore_groupMember(clCtx))
            {
                clCtx->next = MQTTClientCore_connCtxs;
                MQTTClientCore_connCtxs = clCtx;

                /* First entry in 'MQTTClientCore_connCtxs': schedule a move to
                 'used_conn' (for house-keeping and tracking) */
                if (NULL == clCtx->next)
                {
                    rv = loopbTrigger();
                }
            }
        }
    }

    MQTTClientCore_mutexUnlock();

    return rv;
}

static int32_t connectMsgSend(MQTT_ClientCtx_t *clCtx, bool cleanSession, uint16_t kaSecs)
{
    MQTT_Packet_t *mqp = MQTTClientCore_allocSend(MQTT_CONNECT);
    uint8_t *buf, *ref, conn_opts = cleanSession ? MQTT_CONN_MSG_FLAG_CLEAN_START : 0;
    int32_t rv = MQTT_PACKET_ERR_PKT_LEN;
    uint32_t fsz; /* Free buffer size in PKT */
    uint16_t vhl = getConnectVhLen(clCtx);

    if (NULL == mqp)
    {
        return MQTT_PACKET_ERR_PKT_AVL;
    }

    fsz = MQTT_PACKET_FREEBUF_LEN(mqp);
    if (fsz >= vhl)
    {
        mqp->vhLen = vhl; /* Reserve buffer for variable header */
        buf = ref = MQTT_PACKET_PAYLOAD_BUF(mqp);/* Get started to incorporate payload */

        rv = wrConnectPayload(clCtx, buf, fsz - vhl, &conn_opts);/* Payload data */
        if (rv >= 0)
        {
            buf += rv;
            mqp->plLen = buf - ref;

            wrConnectVh(clCtx, ref - vhl, kaSecs,
            CLIENT(clCtx)->willOpts | conn_opts); /* Var Header */

            MQTT_packetPrepFh(mqp, MQTT_MAKE_FH_FLAGS(false, MQTT_QOS0, false));/* Fix Header */

            ref = MQTT_PACKET_FHEADER_BUF(mqp);

            rv = clCtxConnStateTryLocked(clCtx, ref, buf - ref, kaSecs, cleanSession, mqp);
            if (rv > 0)
            {
                mqp = NULL; /* On success, send() routine would have freed-up */
            }
        }
    }
    if (mqp)
    {
        mqpFreeLocked(mqp);
    }

    return rv;
}

//*****************************************************************************
//
//! \brief To be used for the following messages: PUBLISH, SUBSCRIBE, UNSUBSCRIBE
//! Dispatches msg to broker over socket. Frees-up MQP, in case, MSG has QoS0 or
//! if client-lib allocated MQP encounters an error in dispatch.
//! Returns, on success, number of bytes transfered, otherwise -1
//!
//*****************************************************************************
static int32_t _msgDispatch(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqp, MQTT_QOS qos, bool retain)
{
    bool not_qos0 = (MQTT_QOS0 != qos) ? true : false;
    uint16_t msgID = mqp->msgID;
    int32_t rv = MQTT_PACKET_ERR_NETWORK;

    MQTT_packetPrepFh(mqp, MQTT_MAKE_FH_FLAGS(false, qos, retain));

    MQTTClientCore_mutexLockin();

    if (not_qos0)
    {
        mqp->nRefs++; /* Need to enlist, do not free-up MQP */
    }
    /* Frees-up MQP, if full pkt has been sent successfully to net */
    rv = clCtxSeg1Send(clCtx, MQTT_PACKET_FHEADER_BUF(mqp), MQTT_PACKET_CONTENT_LEN(mqp), false, mqp);

    if (rv <= 0)
    { /* Error in sending the MQP - free it up */
        if (not_qos0)
        {
            mqp->nRefs--;
        }
        MQTT_packetFree(mqp); /* Error: Explicitly free-up the MQP */
    }
    else
    {
        rv = msgID; /* Make progress for a good send to the server  */

        if (not_qos0) /* Enlist non QOS0 MQP to await ACK from server */
        {
            MQTT_packetAckWlistAppend(&CLIENT(clCtx)->qosAck1WaitList, mqp);
        }
    }

    MQTTClientCore_mutexUnlock();

    return rv;
}

static int32_t msgDispatchNoFree(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqp, MQTT_QOS qos, bool retain)
{
    if ((NULL == mqp) || (NULL == clCtx))
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    mqp->nRefs++; /* Ensures caller that MQP is not freed-up */

    return _msgDispatch(clCtx, mqp, qos, retain);
}

static int32_t tailIncorpMsgID(MQTT_Packet_t *mqp)
{
    uint8_t *buf = MQTT_PACKET_FHEADER_BUF(mqp) + mqp->vhLen;

    if (0 == mqp->msgID)
    {
        mqp->msgID = assignNewMsgID();
        buf += MQTT_bufWrNbo2B(buf, mqp->msgID);
        mqp->vhLen += 2;

        return 2;
    }

    return 0;
}

static int32_t bufUTF8WrTry(uint8_t *buf, uint32_t fsz, const MQTT_UTF8String_t *topic, uint8_t qid)
{
    uint8_t *ref = buf;

    if (fsz < (topic->length + 2 + (QFL_VALUE == qid) ? 0 : 1))
    {
        return MQTT_PACKET_ERR_PKT_LEN; /* No buf */
    }
    if (false == isValidUTF8String(topic))
    {
        return MQTT_PACKET_ERR_CONTENT;/* Invalid */
    }
    buf += MQTT_packetBufWrUtf8(buf, topic);
    if (QFL_VALUE != qid)
    {
        *buf++ = qid;
    }
    return buf - ref;
}

static int32_t utf8ArraySend(MQTT_ClientCtx_t *clCtx, const MQTT_UTF8StrQOS_t *subsc_vec, const MQTT_UTF8String_t *unsub_vec, uint32_t n_elem)
{
    MQTT_Packet_t *mqp;
    uint8_t *ref, *buf, *end;
    uint32_t i;
    const MQTT_UTF8String_t *topic;
    MQTT_UTF8String_t topreq;
    int32_t rv;

    if ((NULL == clCtx) || !((!!subsc_vec) ^ (!!unsub_vec)) || (0 == n_elem))
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    mqp = MQTTClientCore_allocSend(subsc_vec ?
            MQTT_SUBSCRIBE : MQTT_UNSUBSCRIBE);
    if (NULL == mqp)
    {
        return MQTT_PACKET_ERR_PKT_AVL;
    }
    buf = MQTT_PACKET_VHEADER_BUF(mqp);
    end = MQTT_PACKET_FREEBUF_LEN(mqp) + buf; /* End of free buffer */
    if ((end - buf) < 2)
    {
        return lenErrFreeMqp(mqp);/* MSG-ID: no space */
    }
    buf += tailIncorpMsgID(mqp);
    ref = buf;

    for (i = 0; i < n_elem; i++)
    {
        if (subsc_vec)
        {
            topreq.length = subsc_vec[i].length;
            topreq.buffer = subsc_vec[i].buffer;
            topic = &topreq;
        }
        else
        {
            topic = unsub_vec + i;
        }
        rv = bufUTF8WrTry(buf, end - buf, topic, subsc_vec ? (uint8_t) subsc_vec[i].qosreq : QFL_VALUE);
        if (rv < 0)
        {
            mqpFreeLocked(mqp);
            return rv;
        }

        buf += rv;
    }

    mqp->plLen = buf - ref; /* Total length of topics data */

    return _msgDispatch(clCtx, mqp, MQTT_QOS1, false);
}

static int32_t vhMsgSend(MQTT_ClientCtx_t *clCtx, uint8_t msgType, MQTT_QOS qos, bool hasVH, uint16_t vhData)
{
    uint8_t buf[4];
    uint32_t len = 2;

    buf[0] = MQTT_MAKE_FH_BYTE1(msgType, MQTT_MAKE_FH_FLAGS(false, qos, false));
    buf[1] = hasVH ? 2 : 0;

    if (hasVH)
    {
        len += MQTT_bufWrNbo2B(buf + 2, vhData);
    }
    return clCtxSeg1Send(clCtx, buf, len, false, NULL);
}

static int32_t pingReqSend(MQTT_ClientCtx_t *clCtx, uint32_t rsp_flag)
{
    int32_t rv = 0;
    uint8_t buf[2];

    buf[0] = MQTT_MAKE_FH_BYTE1(MQTT_PINGREQ, MQTT_MAKE_FH_FLAGS(false, MQTT_QOS0, false));
    buf[1] = 0;

    /* Note: in case of error in network send, cl_ctx_send() may
     try to terminate connection with server. */
    rv = clCtxSeg1Send(clCtx, buf, 2, false, NULL);
    if (rv > 0)
    {
        clCtx->flags |= rsp_flag;
    }
    return rv;
}

static bool ack1WlMqpDispatch(MQTT_ClientCtx_t *clCtx)
{
    MQTT_AckWlist_t *wlist = &CLIENT(clCtx)->qosAck1WaitList;
    MQTT_Packet_t *mqp = NULL;
    bool rv = true;
    uint8_t *buf;

    for (mqp = wlist->head; mqp && (true == rv); mqp = mqp->next)
    {
        buf = MQTT_PACKET_FHEADER_BUF(mqp);
        mqp->fhByte1 = *buf |= MQTT_FH_BYTE1_DUP_VAL(true);

        mqp->nRefs++; /* Ensures MQP is not freed by following */
        if (clCtxSeg1Send(clCtx, buf, MQTT_PACKET_CONTENT_LEN(mqp),false, mqp) <= 0)
        {
            mqp->nRefs--; /* Error: need to retract nRefs */
            rv = false;
        }
    }

    return rv;
}

static bool ack2MsgIdDispatch(MQTT_ClientCtx_t *clCtx)
{
    MQTT_PubQOS2CQ_t     *tx_cq = &CLIENT(clCtx)->txQos2CircularQueue;
    MQTT_ListPubQOS2CQ_t *rdIdx = tx_cq->rdIdx;
    bool                  rv    = true;

    for (; (NULL != rdIdx) && (true == rv); rdIdx = rdIdx->next)
    {
        if (vhMsgSend(clCtx, MQTT_PUBREL, MQTT_QOS1,true, rdIdx->msgID) <= 0)
        {
            rv = false;
        }
    }

    return rv;
}

static void sessionResume(MQTT_ClientCtx_t *clCtx)
{
    /* Re-send ACK awaited QoS1/2 msgs */
    if (ack1WlMqpDispatch(clCtx))
    {
        ack2MsgIdDispatch(clCtx);
    }
    return;
}

static bool ack1WlRmfree(MQTT_AckWlist_t *wl, uint16_t msgID)
{
    MQTT_Packet_t *mqp = MQTT_packetAckWlistRemove(wl, msgID);

    if (NULL != mqp)
    {
        MQTT_packetFree(mqp);
        return true;
    }

    /* Err: Unexpected ACK  */
    return false;
}

static bool _procPubRecRx(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    /* Follow-up messages for QOS2 PUB must be transacted in the
     same order as the initial sequence of QOS2 PUB dispatches.
     Therefore, checking the first entry should be OK
     */
    MQTT_Packet_t *mqp = CLIENT(clCtx)->qosAck1WaitList.head;

    if ((msgID == mqp->msgID) && ack2MsgIdLogup(clCtx, msgID))
    {

        ack1WlRmfree(&CLIENT(clCtx)->qosAck1WaitList, msgID);

        vhMsgSend(clCtx, MQTT_PUBREL, MQTT_QOS1, true, msgID);

        return true;
    }

    return false; /* Unexpected PUBREC or QOS2 store exceeded */
}

static bool _procPubRelRx(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    /* For a PUB-REL RX, send PUBCOMP to let server make progress */
    vhMsgSend(clCtx, MQTT_PUBCOMP, MQTT_QOS0, true, msgID);

    if (qos2PubRxIsDone(clCtx, msgID))
    {
        qos2PubRxUnlog(clCtx, msgID); /* Expunge record */
    }
    return true;
}

//*****************************************************************************
//
//! \brief Process ACK Message from Broker.
//! Returns true on success, otherwise false.
//! Used for: PUBACK, SUBACK and UNSUBACK
//!
//*****************************************************************************
static bool _procAckMsgRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);
    uint16_t msgID = mqpRaw->msgID;
    uint32_t len = mqpRaw->plLen;

    /* Caters to SUB-ACK, UNSUB-ACK and PUB-ACK Messages */
    if (false == ack1WlRmfree(&client->qosAck1WaitList, msgID))
    {
        return false; /* Err: MSG_ID was not awaited */
    }
    if (ctxCBs->ackNotify)
    {
        ctxCBs->ackNotify(client->app, mqpRaw->msgType, msgID, len ? MQTT_PACKET_PAYLOAD_BUF(mqpRaw) : NULL, len);
    }
    return true;
}

static bool procAckMsgRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    uint8_t msgType = mqpRaw->msgType;
    bool rv = false;
    uint16_t msgID = 0;

    if (false == MQTT_packetProcMsgIdAckRx(mqpRaw, MQTT_SUBACK == msgType))
    {
        return rv; /* Problem in contents received from server */
    }
    msgID = mqpRaw->msgID;

    if (MQTT_PUBREC == msgType)
    {
        rv = _procPubRecRx(clCtx, msgID);
        if (rv)
        {
            MQTTClientCore_mqpRxDoNotRptSet(mqpRaw); /* Don't report to App */
        }
    }
    else if (MQTT_PUBREL == msgType)
    {
        rv = _procPubRelRx(clCtx, msgID);
        if (rv)
        {
            MQTTClientCore_mqpRxDoNotRptSet(mqpRaw); /* Don't report to App */
        }
    }
    else if (MQTT_PUBCOMP == msgType)
    {
        rv = ack2MsgIdUnlog(clCtx, msgID);
    }
    else
    {
        rv = _procAckMsgRx(clCtx, mqpRaw);
    }

    return rv;
}

static bool procPubMsgRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);
    bool good_pub = MQTT_packetProcPubRx(mqpRaw);
    uint8_t B = mqpRaw->fhByte1;
    MQTT_QOS qos = MQTT_FH_BYTE1_QOS(B);
    uint16_t msgID = 0;

    if (false == good_pub)
    {
        return false; /* Didn't get nicely composed PUB Packet */
    }
    msgID = mqpRaw->msgID;

    /* Irrespective of the result of the ACK through vhMsgSend(),
     the implementation has chosen to process the good PUB packet.
     Any error will be handled in next iteration of rx processing.
     */
    if (MQTT_QOS1 == qos)
    {
        vhMsgSend(clCtx, MQTT_PUBACK, MQTT_QOS0, true, msgID);
    }
    if (MQTT_QOS2 == qos)
    {
        /* Ensuring "only once" philosophy for MQTT QoS2 PUBs */
        if (qos2PubRxIsDone(clCtx, msgID))
        {
            /* Already delivered. Drop it & do not report */
            MQTTClientCore_mqpRxDoNotRptSet(mqpRaw);
            return true; /* No more follow-up; all's good */
        }

        if (false == qos2PubRxLogup(clCtx, msgID))
        {
            return false; /* Failed to record New RX PUB */
        }
        vhMsgSend(clCtx, MQTT_PUBREC, MQTT_QOS0, true, msgID);
    }

    /* QoS obliations completed, present PUBLISH RX packet to app */
    if (ctxCBs->publishRx)
    {
        /* App has chosen the callback method to  receive PKT */
        mqpRaw->nRefs++; /* Make app owner of this packet */
        if (ctxCBs->publishRx(CLIENT(clCtx)->app, MQTT_FH_BYTE1_IS_DUP(B), qos, MQTT_FH_BYTE1_IS_RETAIN(B), mqpRaw))
        {
            /* App has no use of PKT any more, so free it */
            mqpRaw->nRefs--; /* Take back ownership */
        }
    }

    return true;
}

static bool procConnackRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);
    uint8_t *buf = MQTT_PACKET_VHEADER_BUF(mqpRaw);

    mqpRaw->vhLen += 2;
    mqpRaw->plLen -= 2;

    if (0 != mqpRaw->plLen)
    {
        return false; /* There is no payload in message */
    }
    clCtx->flags &= ~(MQTTCLIENTCORE_DO_CONNACK_TO_FLAG | MQTTCLIENTCORE_CONNACK_AWAIT_FLAG);

    if (!(MQTTClientCore_vhbConnackRC(buf)))
    {
        clCtx->flags |= MQTTCLIENTCORE_NOW_CONNECTED_FLAG;
        MQTT_clCtxTimeoutUpdate(clCtx, MQTTClientCore_netOps->time()); /* start KA */

        /* Check if the client has the Clean session flag set
           or if the server doesn't have a session stored */
        if (MQTTClientCore_isClnSession(clCtx) || (0 == MQTTClientCore_vhbConnackSP(buf)))
        {
            sessionDelete(clCtx);
        }
        else
        {
            sessionResume(clCtx);
        }
    }
    if (ctxCBs->ackNotify)
    {
        ctxCBs->ackNotify(CLIENT(clCtx)->app, mqpRaw->msgType, 0, buf, 2);
    }
    return true;
}

static bool procPingrspRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    MQTTClientCore_CtxCBs_t *ctxCBs = MQTTClientCore_ctxCBsPtr(clCtx);

    if (0 != mqpRaw->plLen)
    {
        return false;
    }
    if (MQTTClientCore_awaitsKAPing(clCtx))
    {
        clCtx->flags &= ~MQTTCLIENTCORE_KA_PINGER_RSP_FLAG;
        return true;
    }

    if (MQTTClientCore_awaitsPingRsp(clCtx))
    {
        clCtx->flags &= ~MQTTCLIENTCORE_USER_PING_RSP_FLAG;
        if (ctxCBs->ackNotify)
        {
            ctxCBs->ackNotify(CLIENT(clCtx)->app, mqpRaw->msgType, 0, NULL, 0);
        }
        return true;
    }

    return false;
}

static bool connSentStateRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    bool rv = false;

    switch (mqpRaw->msgType)
    {

        case MQTT_CONNACK:
            /* Changes MQTT_ClientCtx_t->flags to CONNECTED */
            rv = procConnackRx(clCtx, mqpRaw);
            break;

        default:
            break;
    }

    return rv;
}

static bool connectedStateRx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    bool rv = false;

    switch (mqpRaw->msgType)
    {

        case MQTT_SUBACK:
        case MQTT_PUBACK:
        case MQTT_PUBREC:
        case MQTT_PUBREL:
        case MQTT_PUBCOMP:
        case MQTT_UNSUBACK:
            rv = procAckMsgRx(clCtx, mqpRaw);
            break;

        case MQTT_PINGRSP:
            rv = procPingrspRx(clCtx, mqpRaw);
            break;

        case MQTT_PUBLISH:
            rv = procPubMsgRx(clCtx, mqpRaw);
            break;

        case MQTT_CONNACK: /* not expected */
        default:
            break;
    }

    return rv;
}

static bool processRecv(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    bool rv;

    /* Working Principle: Only RX processing errors should be
     reported as 'false'. Status of TX as a follow-up to RX
     messages need not be reported by the xyz_rx() routines.
     Error observed in TX is either dealt in next iteration
     of RX loop (in case, there is a dedicated RX task for
     the CTX) or in TX routine itself (in case, there is no
     dedicated RX task for the CTX).
     */
    rv = MQTTClientCore_awaitsConnack(clCtx) ? connSentStateRx(clCtx, mqpRaw) : connectedStateRx(clCtx, mqpRaw);

    return rv;
}

static int32_t netRecv(int32_t net, MQTT_Packet_t *mqp, uint32_t waitSecs, void *ctx)
{
    bool timedOut = false;
    int32_t rv = MQTT_PacketRecv(net, MQTTClientCore_netOps, mqp, waitSecs, &timedOut, ctx);

    if (rv <= 0)
    {
        if (timedOut)
        {
            rv = MQTT_PACKET_ERR_TIMEOUT;
        }
    }

    return rv;
}

/*
 MQTT 3.1.1 implementation
 -------------------------

 Keep Alive Time is maximum interval within which a client should send a
 packet to broker. If there are either no packets to be sent to broker or
 no retries left, then client is expected to a send a PINGREQ within Keep
 Alive Time. Broker should respond by sending PINGRSP with-in reasonable
 time of 'waitSecs'. If Keep Alive Time is set as 0, then client is not
 expected to be disconnected due to in-activity of MQTT messages. Value
 of 'waitSecs' is assumed to be quite smaller than (non-zero) 'kaSecs'.
 */
static void conn2usedCtxs(uint32_t waitSecs)
{
    while (MQTTClientCore_connCtxs)
    {
        MQTT_ClientCtx_t *clCtx = MQTTClientCore_connCtxs;
        MQTTClientCore_connCtxs = MQTTClientCore_connCtxs->next;

        MQTT_clCtxTimeoutInsert(&MQTTClientCore_usedCtxs, clCtx);
    }
}

static int32_t singleCtxKaSequence(MQTT_ClientCtx_t *clCtx, uint32_t waitSecs)
{
    uint32_t nowSecs = MQTTClientCore_netOps->time();

    if (MQTTClientCore_awaitsConnack(clCtx) && MQTTClientCore_cfgConnackTo(clCtx))
    {
        clCtx->timeout += waitSecs; /* Set CONNACK timeout value */
        clCtx->flags &= ~MQTTCLIENTCORE_DO_CONNACK_TO_FLAG;
    }

    if (clCtx->timeout > nowSecs)
    {
        return 1; /* Still have time for next message transaction */
    }
    if (isConnected(clCtx))
    {
        /* Timeout has happened. Check for PINGRESP if PINGREQ done.
         Otherwise, send PINGREQ (Are You there?) to the server. */
        if (!(MQTTClientCore_awaitsKAPing(clCtx)))
        {
            return pingReqSend(clCtx, MQTTCLIENTCORE_KA_PINGER_RSP_FLAG); /* Hello! */
        }
    }

    return -1;
}

static uint32_t singleCtxAdjWaitSecsGet(MQTT_ClientCtx_t *clCtx, uint32_t waitSecs)
{
    uint32_t secs = MQTTClientCore_netOps->time();

    /* If 'secs' has gone past cl-timeout, then force new timeout to 1 sec */
    secs = (clCtx->timeout > secs) ? clCtx->timeout - secs : 1;

    return (MQTT_KA_TIMEOUT_NONE != clCtx->timeout) ? MQTT_MIN(secs, waitSecs) : waitSecs;
}

static int32_t singleCtxRxPrep(MQTT_ClientCtx_t *clCtx, uint32_t *secs2wait)
{
    int32_t rv;

    if (-1 == clCtx->net)
    {
        return MQTT_PACKET_ERR_NOTCONN; /* Likely for a ctx w/o a receive task */
    }
    if (MQTTClientCore_needNetClose(clCtx))
    {
        rv = MQTT_PACKET_ERR_NOTCONN;
    }
    else if (0 > singleCtxKaSequence(clCtx, *secs2wait))
    {
        rv = MQTT_PACKET_ERR_NETWORK;
    }
    else
    {
        *secs2wait = singleCtxAdjWaitSecsGet(clCtx, *secs2wait);
        return 1;
    }

    doNetCloseRx(clCtx, rv);

    return rv;
}

static int32_t singleCtxRxPrepLocked(MQTT_ClientCtx_t *clCtx, uint32_t *secs2wait)
{
    int32_t rv;

    MQTTClientCore_mutexLockin();
    rv = singleCtxRxPrep(clCtx, secs2wait);
    MQTTClientCore_mutexUnlock();

    return rv;
}

static int32_t procCtxDataRecv(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqp, uint32_t waitSecs, void **app)
{
    int32_t rv = MQTT_PACKET_ERR_NOTCONN;
    int32_t net = clCtx->net;

    *app = clCtx->usr;

    rv = netRecv(net, mqp, waitSecs, (void *)clCtx);

    MQTTClientCore_mutexLockin();

    if ((rv > 0) && (false == processRecv(clCtx, mqp)))
    {
        rv = MQTT_PACKET_ERR_CONTENT;
    }
    /* RX: close the network connection to the server for this context, if
     (a) there is a processing / protocol error other than time-out
     (b) A good MQTT CONNACK has a return code - connection refused
     */
    if ((1 == MQTTClient_closeContext) || ((rv < 0) && (rv != MQTT_PACKET_ERR_TIMEOUT)) ||
        ((MQTT_CONNACK == mqp->msgType) && MQTTClientCore_packetConnackRC(mqp)))
    {
        doNetCloseRx(clCtx, rv);
    }
    MQTTClientCore_mutexUnlock();

    return rv;
}

static int32_t mqpSetupProcCtxDataRecv(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqp, uint32_t waitSecs, void **app)
{
    MQTT_Packet_t *mqpRx = CLIENT(clCtx)->mqpRx;
    int32_t rv;

    CLIENT(clCtx)->mqpRx = NULL;
    if (NULL != mqp)
    {
        /* Input MQP must be same as MQP for partial RX, if any */
        if (mqpRx)
        {
            if (mqp != mqpRx)
            {
                return MQTT_PACKET_ERR_FNPARAM;
            }
        }
        else
        {
            MQTT_packetReset(mqp);
        }
    }

    if (NULL == mqp)
    {
        mqp = mqpRx ? mqpRx : MQTTClientCore_allocRecv(0);
        if (NULL == mqp)
        {
            return MQTT_PACKET_ERR_PKT_AVL;
        }
    }

    rv = procCtxDataRecv(clCtx, mqp, waitSecs, app);
    if ((rv == MQTT_PACKET_ERR_TIMEOUT) && (1 != MQTTClient_closeContext))
    {
        CLIENT(clCtx)->mqpRx = mqp; /* Save partial RX MQP */
    }
    else
    {
        /* Control reaches here due to either an error in RX or the
         completion of RX. In both the cases, the MQP needs to be
         detached and processed. For completion of RX:
         callback mode: Application has used up MQP data; free it
         Non-callback mode: Application will now use complete MQP
         */
        CLIENT(clCtx)->mqpRx = NULL;
        if (mqp->free)
        {
            mqpFreeLocked(mqp); /* For only callback mode */
        }
    }

    return rv;
}

static int32_t clCtxRecv(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqp, uint32_t waitSecs)
{
    void *app = NULL;
    int32_t rv = 0;

    do
    {
        if (mqp && (NULL == CLIENT(clCtx)->mqpRx))
        {
            MQTT_packetReset(mqp);
        }
        rv = singleCtxRxPrepLocked(clCtx, &waitSecs);
        if (rv > 0)
        {
            rv = mqpSetupProcCtxDataRecv(clCtx, mqp, waitSecs, &app);
        }
        /* 'mqp' must be valid, if rv > 0 but including further
         & additional check for sake of static cod analysis.*/
    } while ((rv > 0) && mqp && mqpDoNotReportCor(mqp));

    return rv;
}

static MQTT_ClientCtx_t *groupCtxsKaSequence(uint32_t waitSecs)
{
    MQTT_ClientCtx_t *clCtx = MQTTClientCore_usedCtxs;
    MQTT_ClientCtx_t *next;

    while (clCtx)
    {
        next = clCtx->next;
        if (singleCtxRxPrepLocked(clCtx, &waitSecs) < 0)
        {
            /* 'CTX' no more eligible for operation
             and has been removed from used_list */
            if (false == MQTTClientCore_grpHasCBFn)
            {
                return clCtx;
            }
        }

        clCtx = next;
    }

    return NULL;
}

static uint32_t groupCtxsAdjWaitSecsGet(uint32_t waitSecs)
{
    return (MQTTClientCore_usedCtxs ? singleCtxAdjWaitSecsGet(MQTTClientCore_usedCtxs, waitSecs) : waitSecs);
}

//*****************************************************************************
//
//! \brief Caller must ensure atomic environment for execution of this routine
//!
//*****************************************************************************
static void recvHvecLoad(int32_t *hvec_recv, uint32_t size, MQTT_ClientCtx_t *list)
{
    int32_t i = 0;

    for (i = 0; (i < size) && (NULL != list); i++, list = list->next)
    {
        hvec_recv[i] = list->net;
    }
    hvec_recv[i] = -1;

    return;
}

static int32_t groupCtxsRxPrep(uint32_t waitSecs, void **app)
{
    /* CHK 'used ctx'(s) have live connection w/ server. If not, drop it */
    MQTT_ClientCtx_t *ctx_kaTO = groupCtxsKaSequence(waitSecs);
    int32_t n_hnds;

    if (ctx_kaTO)
    {
        *app = CLIENT(ctx_kaTO)->app;
        return MQTT_PACKET_ERR_NETWORK;
    }

    conn2usedCtxs(waitSecs); /* Now, add new 'ctx'(s) to 'used ctxs' */

    MQTTClientCore_recvHvec[0] = MQTTClientCore_loopbNet;
    recvHvecLoad(&MQTTClientCore_recvHvec[1], MQTTCLIENTCORE_MAX_NWCONN + 1, MQTTClientCore_usedCtxs);

    waitSecs = groupCtxsAdjWaitSecsGet(waitSecs);

    n_hnds = MQTTClientCore_netOps->ioMon(MQTTClientCore_recvHvec, &MQTTClientCore_sendHvec, &MQTTClientCore_rsvdHvec, waitSecs);
    if (0 == n_hnds)
    {
        n_hnds = MQTT_PACKET_ERR_TIMEOUT;
    }
    else if (n_hnds < 0)
    {
        n_hnds = MQTT_PACKET_ERR_LIBQUIT;
    }
    return n_hnds;
}

static int32_t procLoopbackRecv(int32_t net)
{
    uint8_t buf[MQTTCLIENTCORE_LOOP_DLEN];
    /* Thanks for waking-up thread, but ain't got much to do now */
    int32_t rv = MQTTClientCore_netOps->recvFrom(net, buf, MQTTCLIENTCORE_LOOP_DLEN, NULL, NULL, 0);

    if (rv <= 0)
    {
        MQTTClientCore_netOps->close(net);
        return MQTT_PACKET_ERR_LIBQUIT;
    }

    return rv;
}

static MQTT_ClientCtx_t *netClCtxFind(int32_t net)
{
    MQTT_ClientCtx_t *clCtx = MQTTClientCore_usedCtxs;

    while (clCtx && (net != clCtx->net))
    {
        clCtx = clCtx->next;
    }
    return clCtx;
}

static int32_t procNetDataRecv(int32_t net, MQTT_Packet_t *mqp, void **app)
{
    /* Note: MQTTClientCore_usedCtxs are always managed by a single RX task */
    MQTT_ClientCtx_t *clCtx = netClCtxFind(net);
    int32_t rv = MQTT_PACKET_ERR_NOTCONN;

    if (NULL == clCtx)
    {
        return rv; /* TX removed it interim, mustn't happen */
    }
    return mqpSetupProcCtxDataRecv(clCtx, mqp, 1, app);
}

static int32_t clRecv(MQTT_Packet_t *mqp, uint32_t waitSecs, void **app)
{
    int32_t rv = MQTT_PACKET_ERR_NETWORK;
    int32_t n_hnds = 0, idx = 0;
    int32_t net;

    rv = groupCtxsRxPrep(waitSecs, app);
    if (rv > 0)
    {
        n_hnds = rv;
    }
    for (idx = 0; (idx < n_hnds) && (rv > 0); idx++)
    {
        net = MQTTClientCore_recvHvec[idx];
        if (MQTTClientCore_loopbNet == net)
        {
            rv = procLoopbackRecv(net); /* UDP Packet */
        }
        else
        {
            rv = procNetDataRecv(net, mqp, app);
            if (false == MQTTClientCore_grpHasCBFn)
            {
                break; /* 'CTX': inform application */
            }
        }
    }

    return rv;
}

static int32_t grpNetSetupCreate()
{
    if (0 == MQTTClientCore_loopbPortid)
    {
        return MQTT_PACKET_ERR_NOT_DEF;
    }
    if (NULL == MQTTClientCore_netOps)
    {
        return MQTT_PACKET_ERR_NET_OPS;
    }
    if (-1 == MQTTClientCore_loopbNet)
    {
        MQTTClientCore_loopbNet = MQTTClientCore_netOps->open(MQTT_DEV_NETCONN_OPT_UDP, NULL, MQTTClientCore_loopbPortid, NULL);
        if (-1 == MQTTClientCore_loopbNet)
        {
            return MQTT_PACKET_ERR_LIBQUIT;
        }
    }

    return 1;
}

static MQTT_Packet_t *mqpAllocAtomic(void)
{
    MQTT_Packet_t *mqp = NULL;

    MQTTClientCore_mutexLockin();
    mqp = MQTTClientCore_freeList;
    if (mqp)
    {
        MQTTClientCore_freeList = mqp->next;
    }
    MQTTClientCore_mutexUnlock();

    return mqp;
}

static void mqpFree(MQTT_Packet_t *mqp)
{
    /* Must be used in a locked state */
    mqp->next = MQTTClientCore_freeList;
    MQTTClientCore_freeList = mqp;
}

static void clCtxSetup(MQTT_ClientCtx_t *clCtx, /* WR Object */
                        const MQTTClientCore_CtxCfg_t *ctxCfg,
                        const MQTTClientCore_CtxCBs_t *ctxCBs, void *app)
{
    MQTTClientCore_ClientDesc_t *client = CLIENT(clCtx);

    clCtx->flags = ctxCfg->configOpts;

    client->nwconnOpts = ctxCfg->nwconnOpts;
    client->serverAddr = ctxCfg->serverAddr;
    client->portNumber = ctxCfg->portNumber;

    client->app = app;

    if (NULL != ctxCfg->nwSecurity)
    {
        MQTT_bufWrNbytes((uint8_t *)&client->nwSecurity, (uint8_t *)ctxCfg->nwSecurity, sizeof(MQTT_SecureConn_t));
    }
    if (NULL != ctxCBs)
    {
        // set callback flag
        MQTTClientCore_CtxCBs_t *cbs_ctx = MQTTClientCore_ctxCBsPtr(client);
        cbs_ctx->publishRx = ctxCBs->publishRx;
        cbs_ctx->ackNotify = ctxCBs->ackNotify;
        cbs_ctx->disconnCB = ctxCBs->disconnCB;
    }

    return;
}

//*****************************************************************************
// MQTT Client Core API Routines
//*****************************************************************************

uint16_t MQTTClientCore_newMsgID(void)
{
    return assignNewMsgID();
}

bool MQTTClientCore_isConnected(void *ctx)
{
    return isConnected((MQTT_ClientCtx_t *)(ctx));
}

int32_t MQTTClientCore_sendProgress(void *ctx)
{
    MQTT_ClientCtx_t *clCtx = (MQTT_ClientCtx_t *)(ctx);
    MQTTClientCore_txPartPkt_t *txPart = NULL;
    int32_t rv = MQTT_PACKET_ERR_BADCALL;

    if (NULL == ctx)
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    txPart = &CLIENT(clCtx)->txPart;

    if (!MQTTClientCore_txPartInUse(txPart))
    {
        return rv;
    }
    rv = clCtxPartSend(clCtx);
    if (rv > 0)
    {
        rv = MQTTClientCore_txPartInUse(txPart);
    }
    return rv;
}

int32_t MQTTClientCore_sendConnectMsg(void *ctx, bool cleanSession, uint16_t kaSecs)
{
    return (ctx ? connectMsgSend((MQTT_ClientCtx_t *)(ctx), cleanSession, kaSecs) : -1);
}

int32_t MQTTClientCore_sendPubMsg(void *ctx, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf, uint32_t dataLen, MQTT_QOS qos, bool retain)
{
    MQTT_Packet_t *mqp = NULL;

    if ((NULL == ctx) || (NULL == topic) || ((dataLen > 0) && (NULL == dataBuf)))
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }

    if (false == isValidUTF8String(topic))
    {
        return MQTT_PACKET_ERR_CONTENT;
    }

    mqp = MQTTClientCore_allocSend(MQTT_PUBLISH);
    if (NULL == mqp)
    {
       return MQTT_PACKET_ERR_PKT_AVL;
    }

    if ((0 > MQTT_packetPubAppendTopic(mqp, topic, qos ? assignNewMsgID() : 0)) || (dataLen && (0 > MQTT_packetPubAppendData(mqp, dataBuf, dataLen))))
    {
        return lenErrFreeMqp(mqp);
    }

    return (_msgDispatch((MQTT_ClientCtx_t *)(ctx), mqp, qos, retain));
}

int32_t MQTTClientCore_dispatchPub(void *ctx, MQTT_Packet_t *mqp, MQTT_QOS qos, bool retain)
{
    return (msgDispatchNoFree((MQTT_ClientCtx_t *)(ctx), mqp, qos, retain));
}

int32_t MQTTClientCore_sendSubMsg(void *ctx, const MQTT_UTF8StrQOS_t *qosTopics, uint32_t count)
{
    return (utf8ArraySend((MQTT_ClientCtx_t *)(ctx), qosTopics, NULL, count));
}

int32_t MQTTClientCore_dispatchSub(void *ctx, MQTT_Packet_t *mqp)
{
    return (msgDispatchNoFree((MQTT_ClientCtx_t *)(ctx), mqp, MQTT_QOS1, false));
}

int32_t MQTTClientCore_sendUnsubMsg(void *ctx, const MQTT_UTF8String_t *topics, uint32_t count)
{
    return (utf8ArraySend((MQTT_ClientCtx_t *)(ctx), NULL, topics, count));
}

int32_t MQTTClientCore_dispatchUnsub(void *ctx, MQTT_Packet_t *mqp)
{
    return (msgDispatchNoFree((MQTT_ClientCtx_t *)(ctx), mqp, MQTT_QOS1, false));
}

int32_t MQTTClientCore_sendPingreq(void *ctx)
{
    int32_t rv = 0;

    MQTTClientCore_mutexLockin();
    rv = pingReqSend((MQTT_ClientCtx_t *)(ctx), MQTTCLIENTCORE_USER_PING_RSP_FLAG);
    MQTTClientCore_mutexUnlock();

    return rv;
}

int32_t MQTTClientCore_sendDisconn(void *ctx)
{
    uint8_t buf[2];

    buf[0] = MQTT_MAKE_FH_BYTE1(MQTT_DISCONNECT, MQTT_MAKE_FH_FLAGS(false, MQTT_QOS0, false));
    buf[1] = 0;

    MQTTClientCore_mutexLockin();
    /* Note: in case of error in network send, cl_ctx_send() may
     try to terminate connection with server. */
    if (clCtxSeg1Send((MQTT_ClientCtx_t *)(ctx), buf, 2, false, NULL) > 0)
    {
        /* Terminate connection on application's request */
        doNetCloseTx((MQTT_ClientCtx_t *)(ctx), "DISCONN");
    }
    MQTTClientCore_mutexUnlock();

    return 0;
}

int32_t MQTTClientCore_ctxAwaitMsg(void *ctx, uint8_t msgType, MQTT_Packet_t *mqp, uint32_t waitSecs)
{
    MQTT_ClientCtx_t *clCtx = (MQTT_ClientCtx_t *)(ctx);
    int32_t rv = -1;

    if ((NULL == clCtx) || (NULL == mqp))
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    do
    {
        rv = clCtxRecv(clCtx, mqp, waitSecs);

    } while ((rv > 0) && (0 != msgType) && (msgType != mqp->msgType));

    return rv;
}

int32_t MQTTClientCore_ctxRun(void *ctx, uint32_t waitSecs)
{
    int32_t rv;

    if (NULL == ctx)
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    do
    {
        rv = clCtxRecv((MQTT_ClientCtx_t *)(ctx), NULL, waitSecs);

    } while (rv > 0);

    return rv;
}

int32_t MQTTClientCore_awaitMsg(MQTT_Packet_t *mqp, uint32_t waitSecs, void **app)
{
    int32_t rv = MQTT_PACKET_ERR_NOTCONN;

    *app = NULL;

    if (NULL == mqp)
    {
        return MQTT_PACKET_ERR_FNPARAM; /* Didn't get a valid MQP */
    }
    if (true == MQTTClientCore_grpHasCBFn)
    {
        return MQTT_PACKET_ERR_BADCALL; /* Err: LIB has CB config */
    }
    rv = grpNetSetupCreate();
    if (rv <= 0)
    {
        return rv;
    }
    do
    {
        rv = clRecv(mqp, waitSecs, app);

    } while ((rv > 0) && mqpDoNotReportCor(mqp));

    return rv;
}

int32_t MQTTClientCore_run(uint32_t waitSecs)
{
    void *app = NULL;
    int32_t rv = -1;

    if (false == MQTTClientCore_grpHasCBFn)
    {
        return MQTT_PACKET_ERR_BADCALL; /* Err: LIB has no CB config */
    }
    rv = grpNetSetupCreate();
    if (rv <= 0)
    {
        return rv;
    }
    do
    {
        rv = clRecv(NULL, waitSecs, &app);

    } while ((rv > 0) ||
            /* 'ctx' related errors are handled by the callbacks */
            ((rv != MQTT_PACKET_ERR_LIBQUIT) && (rv != MQTT_PACKET_ERR_TIMEOUT) && (rv != MQTT_PACKET_ERR_NETWORK)));

    return rv;
}

MQTT_Packet_t *MQTTClientCore_alloc(uint8_t msgType, uint8_t offset)
{
    MQTT_Packet_t *mqp = mqpAllocAtomic();

    if (NULL == mqp)
    {
        /* MQP allocation failed */
        return NULL;
    }

    MQTT_packetInit(mqp, offset);
    mqp->msgType = msgType;

    return mqp;
}

int32_t MQTTClientCore_registerBuffers(uint32_t mqpNum, MQTT_Packet_t *mqpVec, uint32_t bufLen, uint8_t *bufVec)
{
    uint32_t i, j;
    MQTT_Packet_t *mqp;

    if ((0 == mqpNum) || (0 == bufLen) || MQTTClientCore_freeList)
    {
        return -1;
    }
    for (i = 0, j = 0; i < mqpNum; i++, j += bufLen)
    {
        mqp = mqpVec + i;

        mqp->buffer = bufVec + j;
        mqp->maxlen = bufLen;

        mqp->free = mqpFree;
        mqp->next = MQTTClientCore_freeList;
        MQTTClientCore_freeList = mqp;
    }

    return 0;
}

int32_t MQTTClientCore_registerWillInfo(void *ctx, const MQTT_UTF8String_t *willTop, const MQTT_UTF8String_t *willMsg, MQTT_QOS willQos, bool retain)
{
    uint8_t B = 0;

    if ((NULL == ctx) || ((NULL == willTop) && (NULL != willMsg)))
    {
        return -1; /* Bad Combo */
    }
    if (NULL != willTop)
    {
        MQTTClientCore_retIfinvalidUTF8(willTop);

        B = QOS_VALUE(willQos) << 3;
        if (retain)
        {
            B |= MQTT_CONN_MSG_FLAG_WILL_RETAIN;
        }
        if (NULL != willMsg)
        {
            MQTTClientCore_retIfinvalidUTF8(willMsg);
        }
    }

    CLIENT(ctx)->connPayloadUTF8[1] = willTop;
    CLIENT(ctx)->connPayloadUTF8[2] = willMsg;

    CLIENT(ctx)->willOpts = B;

    return 0;
}

int32_t MQTTClientCore_registerCtxInfo(void *ctx, const MQTT_UTF8String_t *clientID, const MQTT_UTF8String_t *userName, const MQTT_UTF8String_t *password)
{
    const MQTT_UTF8String_t *users_pwd = NULL;

    if (NULL == ctx)
    {
        return -1;
    }
    if (NULL != clientID)
    {
        MQTTClientCore_retIfinvalidUTF8(clientID);
    }
    if (NULL != userName)
    {
        MQTTClientCore_retIfinvalidUTF8(userName);

        if (NULL != password)
        {
            MQTTClientCore_retIfinvalidUTF8(password);
        }
        users_pwd = password;
    }

    CLIENT(ctx)->connPayloadUTF8[0] = clientID;
    CLIENT(ctx)->connPayloadUTF8[3] = userName;
    CLIENT(ctx)->connPayloadUTF8[4] = users_pwd;

    return 0;
}

int32_t MQTTClientCore_registerNetSvc(const MQTT_DeviceNetServices_t *net)
{
    if (net && net->open && net->send && net->recv && net->sendTo && net->recvFrom && net->close && net->ioMon && net->time)
    {
        MQTTClientCore_netOps = net;
        return 0;
    }

    return -1;
}

int32_t MQTTClientCore_createCtx(const MQTTClientCore_CtxCfg_t *ctxCfg,
                    const MQTTClientCore_CtxCBs_t *ctxCBs, void *app, void **ctx)
{
    MQTT_ClientCtx_t *clCtx = NULL;

    if ((NULL == ctxCfg) || (NULL == ctxCfg->serverAddr) || (0 == ctxCfg->portNumber))
    {
        return -1;
    }
    if ((ctxCfg->configOpts & MQTTCLIENTCORE_CFG_MK_GROUP_CTX) && (MQTTClientCore_grpHasCBFn ^ (!!ctxCBs)))
    {
        return -1;
    }

    MQTTClientCore_mutexLockin();
    if (MQTTClientCore_freeCtxs)
    {
        clCtx = MQTTClientCore_freeCtxs;
        MQTTClientCore_freeCtxs = clCtx->next;
        clCtx->next = NULL;
    }
    MQTTClientCore_mutexUnlock();

    if (clCtx)
    {
        clCtxSetup(clCtx, ctxCfg, ctxCBs, app);
        *ctx = (void*) clCtx;
        return 0;
    }

    return -1;
}

int32_t MQTTClientCore_deleteCtx(void *ctx)
{
    MQTT_ClientCtx_t *clCtx = (MQTT_ClientCtx_t *)ctx;
    int32_t rv = -1; /* Not sure about deletion as yet */

    MQTTClientCore_mutexLockin();
    if ((NULL != clCtx) && (-1 == clCtx->net) && (!(awaitsPkts(clCtx))))
    {
        rv = 0; /* OK to delete ctx */
        clientReset(CLIENT(clCtx));
        clCtxFreeup(clCtx);
    }
    MQTTClientCore_mutexUnlock();
    return rv;
}

int32_t MQTTClientCore_initLib(const MQTTClientCore_LibCfg_t *lib_cfg_local)
{
    if (NULL == lib_cfg_local)
    {

    }
    if (ModuleState_InitDone == MQTTClientCore_clLibState)
    {
        /* Error trying to re-initialize */
        return -1;
    }

    MQTTClientCore_msgID = 0xFFFF;
    MQTTClientCore_usedCtxs = NULL;
    MQTTClientCore_freeCtxs = NULL;
    MQTTClientCore_connCtxs = NULL;
    MQTTClientCore_loopbNet = -1;
    MQTTClientCore_freeList = NULL;

    clientDescInit();

    MQTTClientCore_clLibState = ModuleState_InitDone;

    MQTTClientCore_loopbPortid = lib_cfg_local->loopbackPort;
    MQTTClientCore_grpHasCBFn  = lib_cfg_local->grpUsesCbfn;

    MQTTClientCore_mutex        = lib_cfg_local->mutex;
    MQTTClientCore_mutex_lockin = lib_cfg_local->mutexLockin;
    MQTTClientCore_mutex_unlock = lib_cfg_local->mutexUnlock;


    return 0;
}

int32_t MQTTClientCore_exitLib()
{
    MQTT_ClientCtx_t *clCtx = MQTTClientCore_freeCtxs;
    int32_t count = 0;

    if (MQTTClientCore_loopbNet != -1)
    {
        MQTTClientCore_netOps->close(MQTTClientCore_loopbNet);

    }

    if (clCtx->net != -1)
    {
        MQTTClientCore_netOps->close(clCtx->net);
    }

    while (clCtx)
    {
        clCtx = clCtx->next;
        count++;
        if (count > MQTTCLIENTCORE_MAX_NWCONN)
        {
            break;
        }
    }

    if (MQTTCLIENTCORE_MAX_NWCONN == count)
    {
        MQTTClientCore_clLibState = ModuleState_WaitInit;
        MQTTClientCore_freeCtxs = NULL;

        return 0;
    }

    MQTTClientCore_clLibState = ModuleState_WaitInit;
    return -1;
}
