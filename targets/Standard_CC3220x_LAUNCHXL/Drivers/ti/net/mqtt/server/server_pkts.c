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

#include <ti/net/slneterr.h>

#include "server_pkts.h"

#define MUTEX_LOCKIN() if(mutexLockin) mutexLockin(MQTTServerPkts_mutex);
#define MUTEX_UNLOCK() if(mutexUnlock) mutexUnlock(MQTTServerPkts_mutex);

#ifndef CFG_SR_MQTT_CTXS
#define MAX_NWCONN 6
#else
#define MAX_NWCONN CFG_SR_MQTT_CTXS
#endif

#define NETWORK_CLOSE_FLAG         0x00200000
#define NW_CONN_ERROR_FLAG         0x00400000
#define RCVD_CONN_MSG_FLAG         0x00800000

#define NEED_NET_CLOSE(clCtx) (clCtx->flags & NETWORK_CLOSE_FLAG)

#define LOOP_DLEN sizeof(MQTTServerPkts_loopData)

#define MQP_MAX_TOPICS      16
#define MQP_SUBACK_PAY_OFS (MQTT_MAX_FH_LEN + 2)

#define BRK_IF_RD_ERR_UTF8(buf, end, utf8)      \
        if(rd_buf_utf8(buf, end, utf8) < 0)     \
                break;

#define IO_MON_NO_TIMEOUT (0xFFFFFFFF)

#define RET_IF_RD_CONN_ERROR(buf, end, utf8)                    \
{                                                               \
                int32_t len = mqp_buf_rd_utf8(buf, end, utf8);  \
                if(len < 0) { return 0x00FF; }                  \
                buf += len;                                     \
}

static pthread_mutex_t *MQTTServerPkts_mutex = NULL;
static void (*mutexLockin)(pthread_mutex_t *) = NULL;
static void (*mutexUnlock)(pthread_mutex_t *) = NULL;

uint32_t MQTTServerPkts_closeInProgress = 0;

static MQTTServerPkts_MsgCBs_t MQTTServerPkts_usrObj;
static MQTTServerPkts_MsgCBs_t *MQTTServerPkts_usrCBs = NULL;
static const MQTT_DeviceNetServices_t *MQTTServerPkts_netOps = NULL;

static MQTT_ClientCtx_t MQTTServerPkts_contexts[MAX_NWCONN];

static MQTT_ClientCtx_t *MQTTServerPkts_usedCtxs = NULL;
static MQTT_ClientCtx_t *MQTTServerPkts_freeCtxs = NULL;

int32_t MQTTServerPkts_recvHvec[MAX_NWCONN + 1 + 1 + 1];

static int32_t MQTTServerPkts_listenNet = -1;

static MQTT_Packet_t MQTTServerPkts_rxMqp;
static uint8_t MQTTServerPkts_rxb[MQP_SERVER_RX_LEN];
static uint16_t MQTTServerPkts_listenerPort = 0;
static MQTT_SecureConn_t MQTTServerPkts_srvSecure;

static int32_t MQTTServerPkts_loopbNet = -1;
static const uint8_t MQTTServerPkts_loopData[] = { 0x00, 0x01 };
static uint16_t MQTTServerPkts_loopbackPort = 0;
static bool MQTTServerPkts_pendingTrigs = false;


static int32_t mqp_buf_rd_utf8(const uint8_t *buf, const uint8_t *end, MQTT_UTF8String_t *utf8)
{
    const uint8_t *ref = buf; /* Reference */
    uint16_t len = 0; /* UTF8 Size */

    if (end - buf < 2)
    {
        return -1; /* No valid buffer to hold UTF8 size */
    }
    buf += MQTT_bufRdNbo2B(buf, &len);
    if (end - buf < len)
    {
        return -1; /* No valid buffer to hold UTF8 name */
    }
    utf8->length = len;
    utf8->buffer = len ? (char *)buf : NULL;

    return buf + len - ref;
}

static void cl_ctx_init(void)
{
    int32_t i = 0;

    for (i = 0; i < MAX_NWCONN; i++)
    {
        MQTT_ClientCtx_t *clCtx = MQTTServerPkts_contexts + i;

        MQTT_clCtxReset(clCtx);

        clCtx->next = MQTTServerPkts_freeCtxs;
        MQTTServerPkts_freeCtxs = clCtx;
    }
}

static void cl_ctx_free(MQTT_ClientCtx_t *clCtx)
{
    MQTT_clCtxReset(clCtx);
    clCtx->next = MQTTServerPkts_freeCtxs;
    MQTTServerPkts_freeCtxs = clCtx;

    return;
}

static MQTT_ClientCtx_t *cl_ctx_alloc(void)
{
    MQTT_ClientCtx_t *clCtx = MQTTServerPkts_freeCtxs;

    if (clCtx)
    {
        MQTTServerPkts_freeCtxs = clCtx->next;
        clCtx->next = NULL;
    }

    return clCtx;
}

static inline bool had_rcvd_conn_msg(MQTT_ClientCtx_t *clCtx)
{
    return (clCtx->flags & RCVD_CONN_MSG_FLAG);
}

static inline void set_rcvd_conn_msg(MQTT_ClientCtx_t *clCtx)
{
    clCtx->flags |= RCVD_CONN_MSG_FLAG;
}

static void used_ctxs_insert(MQTT_ClientCtx_t *clCtx)
{
    MQTT_clCtxTimeoutInsert(&MQTTServerPkts_usedCtxs, clCtx);
}

static void used_ctxs_remove(MQTT_ClientCtx_t *clCtx)
{
    MQTT_clCtxRemove(&MQTTServerPkts_usedCtxs, clCtx);
}

static int32_t loopb_trigger(void)
{
    uint8_t ip_addr[] = { 127, 0, 0, 1 };
    int32_t rv = 0;

    if ((-1 != MQTTServerPkts_loopbNet) && (false == MQTTServerPkts_pendingTrigs))
    {
        rv = MQTTServerPkts_netOps->sendTo(MQTTServerPkts_loopbNet, MQTTServerPkts_loopData, LOOP_DLEN, MQTTServerPkts_loopbackPort, ip_addr, 4);
        if (0 == rv)
        {
            MQTTServerPkts_pendingTrigs = true;
        }
    }

    return rv;
}

static void do_net_close_rx(MQTT_ClientCtx_t *clCtx, bool due2err)
{
    /* RX closing Net */
    MQTTServerPkts_netOps->close(clCtx->net);
    clCtx->net = -1;

    if (clCtx->usr)
    {
        MQTTServerPkts_usrCBs->onClNetClose(clCtx->usr, due2err);
    }
    used_ctxs_remove(clCtx);
    cl_ctx_free(clCtx);
}

static void do_net_close_tx(MQTT_ClientCtx_t *clCtx, bool due2err)
{
    if (due2err)
    {
        clCtx->flags |= NW_CONN_ERROR_FLAG;
    }
    clCtx->flags |= NETWORK_CLOSE_FLAG;

    loopb_trigger();
}

static int32_t cl_ctx_send(MQTT_ClientCtx_t *clCtx, uint8_t *buf, uint32_t len)
{
    int32_t rv = MQTTServerPkts_netOps->send(clCtx->net, buf, len, NULL);

    if (rv <= 0)
    {
        do_net_close_tx(clCtx, true);
        rv = MQTT_PACKET_ERR_NETWORK;
    }

    return rv;
}

static int32_t vh_msg_send(MQTT_ClientCtx_t *clCtx, uint8_t msgType, MQTT_QOS qos,
                            bool hasVH, uint16_t vhData)
{
    uint8_t buf[4];
    uint32_t len = 2;

    if (false == had_rcvd_conn_msg(clCtx))
    {
        return MQTT_PACKET_ERR_NOTCONN;
    }
    buf[0] = MQTT_MAKE_FH_BYTE1(msgType, MQTT_MAKE_FH_FLAGS(false, qos, false));
    buf[1] = hasVH ? 2 : 0;

    if (hasVH)
    {
        len += MQTT_bufWrNbo2B(buf + 2, vhData);
    }
    return cl_ctx_send(clCtx, buf, len);
}

static int32_t _mqtt_vh_msg_send(void *ctxCl, uint8_t msgType, MQTT_QOS qos, bool hasVH, uint16_t vhData)
{
    MQTT_ClientCtx_t *clCtx = (MQTT_ClientCtx_t *)ctxCl;

    return (clCtx ? vh_msg_send(ctxCl, msgType, qos, hasVH, vhData) : -1);
}

int32_t MQTTServerPkts_sendVhMsg(void *ctxCl, uint8_t msgType, MQTT_QOS qos, bool hasVH, uint16_t vhData)
{
    return _mqtt_vh_msg_send(ctxCl, msgType, qos, hasVH, vhData);
}

int32_t MQTTServerPkts_sendLockedVhMsg(void *ctxCl, uint8_t msgType, MQTT_QOS qos,
                                bool hasVH, uint16_t vhData)
{
    int32_t rv;

    MUTEX_LOCKIN();
    rv = _mqtt_vh_msg_send(ctxCl, msgType, qos, hasVH, vhData);
    MUTEX_UNLOCK();

    return rv;
}

int32_t mqtt_connack_send(void *ctxCl, uint8_t *vhBuf)
{
    MQTT_ClientCtx_t *clCtx = (MQTT_ClientCtx_t *)ctxCl;
    int32_t rv = vh_msg_send(clCtx, MQTT_CONNACK, MQTT_QOS0,
                            true, (vhBuf[0] << 8) | vhBuf[1]);

    if ((rv > 0) && (0x00 != vhBuf[1]))
    {
        do_net_close_tx(clCtx, true);
    }
    return rv;
}

static int32_t _mqtt_server_pub_dispatch(void *ctxCl, MQTT_Packet_t *mqp, bool dup)
{
    int32_t rv = 0;
    uint8_t *buf = MQTT_PACKET_FHEADER_BUF(mqp);

    if (dup)
    {
        *buf |= MQTT_FH_BYTE1_DUP_VAL(true);
    }
    rv = cl_ctx_send((MQTT_ClientCtx_t *)ctxCl, buf, MQTT_PACKET_CONTENT_LEN(mqp));

    *buf &= ~MQTT_FH_BYTE1_DUP_VAL(true);

    return rv;
}

int32_t MQTTServerPkts_dispatchPub(void *ctxCl, MQTT_Packet_t *mqp, bool dup)
{
    return _mqtt_server_pub_dispatch(ctxCl, mqp, dup);
}

int32_t MQTTServerPkts_dispatchLockedPub(void *ctxCl, MQTT_Packet_t *mqp, bool dup)
{
    int32_t rv;

    MUTEX_LOCKIN();
    rv = _mqtt_server_pub_dispatch(ctxCl, mqp, dup);
    MUTEX_UNLOCK();

    return rv;
}

static int32_t sub_ack_send(MQTT_ClientCtx_t *clCtx, uint8_t *buf, uint8_t pay_ofs, uint32_t payLen, uint16_t msgID)
{
    uint8_t *ref = buf += MQTT_MAX_FH_LEN;

    if (MQP_SUBACK_PAY_OFS != pay_ofs)
    {
        return MQTT_PACKET_ERR_PKT_LEN;
    }
    buf += MQTT_bufWrNbo2B(buf, msgID);
    ref -= MQTT_packetBufTailWrRemlen(ref - MQTT_MAX_REMLEN_BYTES, payLen + buf - ref);

    ref -= 1;
    *ref = MQTT_MAKE_FH_BYTE1(MQTT_SUBACK, MQTT_MAKE_FH_FLAGS(false, MQTT_QOS0, false));

    return cl_ctx_send(clCtx, ref, payLen + buf - ref);
}

static inline int32_t unsub_ack_send(MQTT_ClientCtx_t *clCtx, uint16_t msgID)
{
    return vh_msg_send(clCtx, MQTT_UNSUBACK, MQTT_QOS0, true, msgID);
}

static bool mqp_proc_vh_msg_id_rx(MQTT_Packet_t *mqpRaw)
{
    uint8_t *buf = MQTT_PACKET_VHEADER_BUF(mqpRaw);

    if (mqpRaw->plLen < 2)
    {
        return false; /* Bytes for MSG ID not available */
    }
    buf += MQTT_bufRdNbo2B(buf, &mqpRaw->msgID);
    mqpRaw->vhLen += 2;
    mqpRaw->plLen -= 2;

    return true;
}

static int32_t buf_rd_utf8_qos(uint8_t *buf, uint8_t *end, MQTT_UTF8StrQOS_t *utf8_qos)
{
    MQTT_UTF8String_t utf8 = { 0 };
    uint8_t *ref = buf;

    buf += mqp_buf_rd_utf8(buf, end, &utf8);

    /* Assess that UTF8 has been read and QOS can be read */
    if ((buf > ref) && (end > buf))
    {
        utf8_qos->buffer = utf8.buffer;
        utf8_qos->length = utf8.length;
        utf8_qos->qosreq = (MQTT_QOS) *buf++;

        return buf - ref;
    }

    return -1;
}

static bool _proc_sub_msg_rx(MQTT_Packet_t *mqpRaw, MQTT_UTF8StrQOS_t *qosTopics, uint32_t *nTopics)
{
    uint8_t *buf, *end;
    uint32_t i = 0;
    MQTT_UTF8StrQOS_t *qos_top;
    int32_t len;

    if (false == mqp_proc_vh_msg_id_rx(mqpRaw))
    {
        return false; /* Problem in contents received from client */
    }
    buf = MQTT_PACKET_PAYLOAD_BUF(mqpRaw);
    end = buf + mqpRaw->plLen;

    for (i = 0; (i < *nTopics) && (buf < end); i++)
    {
        qos_top = qosTopics + i;
        len = buf_rd_utf8_qos(buf, end, qos_top);
        if (len < 0)
        {
            break; /* Failed to read Topic */
        }
        buf += len;
    }

    *nTopics = i;

    return (((0 == i) || (buf != end)) ? false : true);
}

static bool proc_sub_msg_rx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    uint32_t nTopics = MQP_MAX_TOPICS;
    uint16_t msgID;
    MQTT_UTF8StrQOS_t qosTopics[MQP_MAX_TOPICS];
    uint8_t ack[MQP_MAX_TOPICS + MQP_SUBACK_PAY_OFS];

    if (false == _proc_sub_msg_rx(mqpRaw, qosTopics, &nTopics))
    {
        return false;
    }
    msgID = mqpRaw->msgID;

    /* All topics have been now put in array, pass-on info to upper layer */
    if (MQTTServerPkts_usrCBs->subMsgRx(clCtx->usr, qosTopics, nTopics, msgID, ack + MQP_SUBACK_PAY_OFS))
    {
        sub_ack_send(clCtx, ack, MQP_SUBACK_PAY_OFS, nTopics, msgID);
        return true;
    }

    return false;
}

static bool _proc_unsub_msg_rx(MQTT_Packet_t *mqpRaw, MQTT_UTF8String_t *topics, uint32_t *nTopics)
{
    uint8_t *buf, *end;
    uint32_t i = 0;
    MQTT_UTF8String_t *topic;
    int32_t len;

    if (false == mqp_proc_vh_msg_id_rx(mqpRaw))
    {
        return false; /* Problem in contents received from client */
    }
    buf = MQTT_PACKET_PAYLOAD_BUF(mqpRaw);
    end = buf + mqpRaw->plLen;

    for (i = 0; (i < *nTopics) && (buf < end); i++)
    {
        topic = topics + i;
        len = mqp_buf_rd_utf8(buf, end, topic);
        if (len < 0)
        {
            break; /* Failed to read Topic */
        }
        buf += len;
    }

    *nTopics = i;

    return (((0 == i) || (buf != end)) ? false : true);
}

static bool proc_unsub_msg_rx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    uint32_t nTopics = MQP_MAX_TOPICS;
    uint16_t msgID;
    MQTT_UTF8String_t topics[MQP_MAX_TOPICS];

    if (false == _proc_unsub_msg_rx(mqpRaw, topics, &nTopics))
    {
        return false;
    }
    msgID = mqpRaw->msgID;

    /* All topics have been now put in array, pass-on info to upper layer */
    if (MQTTServerPkts_usrCBs->unSubMsg(clCtx->usr, topics, nTopics, msgID))
    {
        unsub_ack_send(clCtx, msgID);
        return true;
    }

    return false;
}

static bool proc_pingreq_rx(MQTT_ClientCtx_t *clCtx)
{
    vh_msg_send(clCtx, MQTT_PINGRSP, MQTT_QOS0, false, 0x00);
    return true;
}

static bool proc_disconn_rx(MQTT_ClientCtx_t *clCtx)
{
    do_net_close_rx(clCtx, false);
    return true;
}

static bool proc_pub_msg_rx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    bool rv = MQTT_packetProcPubRx(mqpRaw);
    uint8_t B = mqpRaw->fhByte1;
    MQTT_QOS qos = MQTT_FH_BYTE1_QOS(B);
    MQTT_UTF8String_t topic;
    uint16_t msgID = 0;

    if (false == rv)
    {
        return rv; /* Didn't get a good PUB Packet */
    }
    msgID = mqpRaw->msgID;

    topic.buffer = (char *)MQTT_PACKET_PUB_TOP_BUF(mqpRaw);
    topic.length = MQTT_PACKET_PUB_TOP_LEN(mqpRaw);

    rv = MQTTServerPkts_usrCBs->pubMsgRx(clCtx->usr, &topic, MQTT_PACKET_PUB_PAY_BUF(mqpRaw),
        MQTT_PACKET_PUB_PAY_LEN(mqpRaw), msgID, MQTT_FH_BYTE1_IS_DUP(B), qos, MQTT_FH_BYTE1_IS_RETAIN(B));
    if (false == rv)
    {
        return rv;
    }
    if (MQTT_QOS1 == qos)
    {
        vh_msg_send(clCtx, MQTT_PUBACK, MQTT_QOS0, true, msgID);
    }
    if (MQTT_QOS2 == qos)
    {
        vh_msg_send(clCtx, MQTT_PUBREC, MQTT_QOS0, true, msgID);
    }
    return rv;
}

static bool proc_ack_msg_rx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    if ((false == MQTT_packetProcMsgIdAckRx(mqpRaw, false)) ||
        (false == MQTTServerPkts_usrCBs->ackNotify(clCtx->usr, mqpRaw->msgType, mqpRaw->msgID)))
    {
        return false;
    }

    return true;
}

static void rx_timeout_update(MQTT_ClientCtx_t *clCtx)
{
    if (false == had_rcvd_conn_msg(clCtx))
    {
        return;
    }
    MQTT_clCtxTimeoutUpdate(clCtx, MQTTServerPkts_netOps->time());

    used_ctxs_remove(clCtx);
    used_ctxs_insert(clCtx);

    return;
}

static bool proc_protocol_info(MQTT_UTF8String_t *utf8, uint8_t ver)
{
    const char *buf = utf8->buffer;

    /* Check for protocol version 3.1.1 */
    if ((4 == utf8->length) && (buf[0] == 'M') && (buf[1] == 'Q') && (buf[2] == 'T') &&
        (buf[3] == 'T') && (0x04 == ver))
    {
        return true;
    }
    /* Check for protocol version 3.1   */
    if ((6 == utf8->length) && (buf[0] == 'M') && (buf[1] == 'Q') && (buf[2] == 'I') &&
        (buf[3] == 's') && (buf[4] == 'd') && (buf[5] == 'p') && (0x03 == ver))
    {
        return true;
    }
    return false;
}

static uint16_t proc_connect_vh_rx(MQTT_Packet_t *mqpRaw, uint8_t *connFlags, uint16_t *kaSecs)
{
    MQTT_UTF8String_t utf8 = { 0 };
    uint8_t *buf = MQTT_PACKET_PAYLOAD_BUF(mqpRaw);
    uint8_t *end = buf + mqpRaw->plLen;
    uint8_t *ref = buf;

    buf += mqp_buf_rd_utf8(buf, end, &utf8);
    if (end - buf < 1)
    {
        return MQTT_CONNACK_RC_BAD_PROTOV; /* No proto ver */
    }
    if (false == proc_protocol_info(&utf8, *buf++))
    {
        return MQTT_CONNACK_RC_BAD_PROTOV;
    }
    *connFlags = *buf++;

    if (end - buf < 2)
    {
        return 0xFF; /* Bad packet composition */
    }
    *kaSecs = (buf[0] << 8) | buf[1];
    buf += 2;
    *kaSecs += *kaSecs >> 1;

    mqpRaw->vhLen = buf - ref;
    mqpRaw->plLen -= buf - ref;

    return 0;
}

uint16_t proc_connect_pl_rx(const uint8_t *buf, const uint8_t *end, uint8_t connFlags, MQTT_UTF8String_t *free_utf8s, MQTT_UTF8String_t **used_refs)
{
    MQTT_UTF8String_t *utf8;

    utf8 = used_refs[0] = free_utf8s + 0;
    RET_IF_RD_CONN_ERROR(buf, end, utf8);

    if (connFlags & MQTT_CONN_MSG_FLAG_WILL_CONFIG)
    {
        utf8 = used_refs[1] = free_utf8s + 1;
        RET_IF_RD_CONN_ERROR(buf, end, utf8);

        utf8 = used_refs[2] = free_utf8s + 2;
        RET_IF_RD_CONN_ERROR(buf, end, utf8);
    }

    if ((0 == (connFlags & MQTT_CONN_MSG_FLAG_USER_NAME)) && (0 != (connFlags & MQTT_CONN_MSG_FLAG_PASS_WORD)))
    {
        return 0x00FF; /* Bad combination */
    }

    if (connFlags & MQTT_CONN_MSG_FLAG_USER_NAME)
    {
        utf8 = used_refs[3] = free_utf8s + 3;
        RET_IF_RD_CONN_ERROR(buf, end, utf8);
    }

    if (connFlags & MQTT_CONN_MSG_FLAG_PASS_WORD)
    {
        utf8 = used_refs[4] = free_utf8s + 4;
        RET_IF_RD_CONN_ERROR(buf, end, utf8);
    }

    return 0;
}

static bool proc_connect_rx(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    MQTT_UTF8String_t *used_refs[5] = { NULL, NULL, NULL, NULL, NULL };
    MQTT_UTF8String_t free_utf8s[5];
    uint8_t connFlags;
    uint8_t *buf;
    uint8_t *end;
    bool cleanSession = false;
    uint16_t ack_vh16 = 0; /* Variable Header of CONNACK (response) Message */

    set_rcvd_conn_msg(clCtx);

    ack_vh16 = proc_connect_vh_rx(mqpRaw, &connFlags, &clCtx->kaSecs);
    if (ack_vh16 == 0)
    {
        buf = MQTT_PACKET_PAYLOAD_BUF(mqpRaw);
        end = buf + mqpRaw->plLen;
        /* Make sure that the end of the buffer is clean */
        *end = 0;

        ack_vh16 = proc_connect_pl_rx(buf, end, connFlags, free_utf8s, used_refs);
        if (ack_vh16 == 0)
        {
            cleanSession = (connFlags & MQTT_CONN_MSG_FLAG_CLEAN_START) ? true : false;
            ack_vh16 = (!used_refs[0]->length && !cleanSession) ?
            MQTT_CONNACK_RC_CLI_REJECT : 0; /* Validate 0 byte Client ID */

            if (ack_vh16 == 0)
            {
                /* UTF8 Order: Client ID, Will Topic, Will Msg, User Name, Pass Word */
                ack_vh16 = MQTTServerPkts_usrCBs->connectRx(clCtx, connFlags, &used_refs[0], &clCtx->usr);
            }
        }
    }

    if (0xFF != (ack_vh16 & 0xFF))
    {
        vh_msg_send(clCtx, MQTT_CONNACK, MQTT_QOS0, true, ack_vh16);
    }

    if (MQTT_CONNACK_RC_REQ_ACCEPT == (ack_vh16 & 0xFF))
    {
        rx_timeout_update(clCtx);
        MQTTServerPkts_usrCBs->onConnackSend(clCtx->usr, cleanSession);
    }
    else
    {
        return false;
    }

    return true;
}

static uint32_t recv_hvec_load(int32_t *recv_hvec, uint32_t size, MQTT_ClientCtx_t *list)
{
    int32_t i = 0;

    for (i = 0; (i < size) && (NULL != list); i++, list = list->next)
    {
        recv_hvec[i] = list->net;
    }
    return i;
}

static bool process_recv(MQTT_ClientCtx_t *clCtx, MQTT_Packet_t *mqpRaw)
{
    uint8_t msgType = mqpRaw->msgType;
    bool rv = false;

    if ((MQTT_CONNECT != msgType) ^ had_rcvd_conn_msg(clCtx))
    {
        return rv;
    }
    /* Proto Violation */

    rx_timeout_update(clCtx);

    switch (msgType)
    {
        case MQTT_CONNECT:
            rv = proc_connect_rx(clCtx, mqpRaw);
            break;

        case MQTT_DISCONNECT:
            rv = proc_disconn_rx(clCtx);
            break;

        case MQTT_SUBSCRIBE:
            rv = proc_sub_msg_rx(clCtx, mqpRaw);
            break;

        case MQTT_UNSUBSCRIBE:
            rv = proc_unsub_msg_rx(clCtx, mqpRaw);
            break;

        case MQTT_PINGREQ:
            rv = proc_pingreq_rx(clCtx);
            break;

        case MQTT_PUBLISH:
            rv = proc_pub_msg_rx(clCtx, mqpRaw);
            break;

        case MQTT_PUBACK:
        case MQTT_PUBREC:
        case MQTT_PUBREL:
        case MQTT_PUBCOMP:
            rv = proc_ack_msg_rx(clCtx, mqpRaw);
            break;

        default:
            break;
    }
    return rv;
}

//*****************************************************************************
//
//! \brief Terminate net connections which haven't received PKTs beyond expected time.
//! Caller must ensure atomic enviroment for execution of this routine.
//
//*****************************************************************************
static void ka_sequence(uint32_t *secs2wait)
{
    MQTT_ClientCtx_t *clCtx = MQTTServerPkts_usedCtxs; /* Sorted for timeout (ascend) */
    uint32_t nowSecs = MQTTServerPkts_netOps->time();
    MQTT_ClientCtx_t *next;
    bool due2err;
    bool teardown;

    while (NULL != clCtx)
    {
        next = clCtx->next;
        due2err = false;
        teardown = false;

        if (NEED_NET_CLOSE(clCtx))
        {
            teardown = true;
            if (clCtx->flags & NW_CONN_ERROR_FLAG)
            {
                due2err = true;
            }
        }

        if (!(clCtx->timeout > nowSecs))
        {
            teardown = true;
            due2err = true;
        }
        if (teardown)
        {
            clCtx->flags &= ~(NW_CONN_ERROR_FLAG |
            NETWORK_CLOSE_FLAG);

            /* Close network: Timeout or TX err */
            do_net_close_rx(clCtx, due2err);
        }

        clCtx = next;
    }

    clCtx = MQTTServerPkts_usedCtxs;
    if (((NULL != clCtx) && (MQTT_KA_TIMEOUT_NONE == clCtx->timeout)) || ((NULL == clCtx)))
    {
        *secs2wait = IO_MON_NO_TIMEOUT;
    }
    else
    {
        *secs2wait = clCtx->timeout - nowSecs;
    }

    return;
}

static int32_t accept_ctx(int32_t net, uint32_t waitSecs)
{
    MQTT_ClientCtx_t *clCtx;
    int32_t rv;
    uint32_t nwconnInfo = 0;

    clCtx = cl_ctx_alloc();
    if (NULL == clCtx)
    {
        /* No free clCtx, max # of clients reached */
        return true;
    }

    if (*((uint32_t *)MQTTServerPkts_srvSecure.cipher))
    {
        nwconnInfo = MQTT_DEV_NETCONN_OPT_SEC;
    }

    clCtx->net = MQTTServerPkts_netOps->accept(nwconnInfo, net, clCtx->remoteIP, &clCtx->ipLen);
    if (0 > clCtx->net)
    {
        if (SLNETERR_BSD_ENSOCK == clCtx->net)
        {
            rv = MQTT_PACKET_ERR_MAXCONN;
        }
        else
        {
            /* Fatal, couldn't accept new incoming conn */
            rv = MQTT_PACKET_ERR_LIBQUIT;
        }
        cl_ctx_free(clCtx);

        return rv;
    }

    /* Timeout to receive MQTT_CONNECT */
    clCtx->timeout = waitSecs + MQTTServerPkts_netOps->time();

    used_ctxs_insert(clCtx);
    return true;
}

static MQTT_ClientCtx_t *net_cl_ctx_find(int32_t net)
{
    MQTT_ClientCtx_t *clCtx = MQTTServerPkts_usedCtxs;

    while (clCtx)
    {
        if (net == clCtx->net)
        {
            break;
        }
        clCtx = clCtx->next;
    }
    /*("NULL == clCtx - Did not find ctx */

    return clCtx;
}

static inline int32_t net_recv(int32_t net, MQTT_Packet_t *mqp, uint32_t waitSecs, bool *timedOut)
{
    int32_t rv = MQTT_PacketRecv(net, MQTTServerPkts_netOps, mqp, waitSecs, timedOut, NULL);

    if (rv <= 0)
    {
        rv = MQTT_PACKET_ERR_NETWORK;
    }
    return rv;
}

static int32_t proc_loopback_recv(int32_t net)
{
    uint8_t buf[LOOP_DLEN];
    /* Thanks for waking-up the thread, but we do nothing in this */
    int32_t rv = MQTTServerPkts_netOps->recvFrom(net, buf, LOOP_DLEN, NULL, NULL, 0);

    MQTTServerPkts_pendingTrigs = false;

    if (rv <= 0)
    {
        MQTTServerPkts_netOps->close(net);
        return MQTT_PACKET_ERR_LIBQUIT;
    }

    return rv;
}

static void proc_net_data_recv(int32_t net)
{
    MQTT_ClientCtx_t *clCtx = net_cl_ctx_find(net);
    bool dummy;
    int32_t rv;

    MQTT_packetReset(&MQTTServerPkts_rxMqp); /* Start w/ a clean buffer */

    rv = net_recv(net, &MQTTServerPkts_rxMqp, 0, &dummy);
    if ((rv > 0) && (false == process_recv(clCtx, &MQTTServerPkts_rxMqp)))
    {
        /* Working Principle: Only RX processing errors should be
         reported as 'false'. Status of TX as a follow-up to RX
         messages need not be reported by the xyz_rx() routines.
         Error observed in TX is dealt in the next iteration of
         the RX loop.
         */
        rv = MQTT_PACKET_ERR_CONTENT;
    }

    if (rv < 0)
    {
        do_net_close_rx(clCtx, true);
    }
}

static int32_t proc_recv_hnds_locked(int32_t *recv_hnds, int32_t n_hnds, uint32_t waitSecs)
{
    int32_t rv = 0;
    int32_t idx = 0;
    int32_t net;

    MUTEX_LOCKIN();

    for (idx = 0; (idx < n_hnds) && (0 <= rv); idx++)
    {
        net = recv_hnds[idx];
        if (net == MQTTServerPkts_listenNet)
        {
            rv = accept_ctx(MQTTServerPkts_listenNet, waitSecs);
        }
        else if (MQTTServerPkts_loopbackPort && (net == MQTTServerPkts_loopbNet))
        {
            rv = proc_loopback_recv(MQTTServerPkts_loopbNet);
        }
        else
        {
            proc_net_data_recv(net);
        }
    }

    MUTEX_UNLOCK();

    return rv;
}

static int32_t net_hnds_mon_loop(uint32_t waitSecs)
{
    /* Desc. for addl. Recv hnds --> LISTEN + LOOPBACK + VEC END */
    static int32_t send_hvec = -1;
    static int32_t rsvd_hvec = -1;

    uint32_t secs2wait = 0; /* Time-out across all active MQTT conns */
    int32_t n_hnds = 0;
    int32_t max_open_socket = MAX_NWCONN;
    int32_t highest_open_socket = 0;
    int32_t rv;
    int32_t *r_hvec;

    do
    {
        r_hvec = MQTTServerPkts_recvHvec + 0;

        /* MQTT Timeouts: close expired conns; get time to next
           expiration */
        ka_sequence(&secs2wait);

        /* Prepare an array of net handles to track. If the (intended)
         max number of the data connections have been reached,  then
         do not include the listener handle in the array. This setup
         will enable, at transport layer, ignoring of unwanted conn. */
        n_hnds = (int32_t) recv_hvec_load(r_hvec, MAX_NWCONN, MQTTServerPkts_usedCtxs);
        highest_open_socket = n_hnds;
        if ((n_hnds < MAX_NWCONN) && (n_hnds < max_open_socket))
        {
            r_hvec[n_hnds++] = MQTTServerPkts_listenNet;
        }
        if (MQTTServerPkts_loopbackPort)
        {
            r_hvec[n_hnds++] = MQTTServerPkts_loopbNet;
        }
        r_hvec[n_hnds++] = -1; /* Mark end of the array of net handles */

        if (MQTTServerPkts_closeInProgress != 1)
        {
            n_hnds = MQTTServerPkts_netOps->ioMon(MQTTServerPkts_recvHvec, &send_hvec, &rsvd_hvec, secs2wait);
        }
        else
        {
            return 0;
        }
        /* n_hnds values: Negative value means error, positive value
        means success, zero means timeout - ka_sequence function that
        is at the beginning of the loop deals with expired connections */
        if ((n_hnds < 0))
        {
            return MQTT_PACKET_ERR_LIBQUIT;
        }

        rv = proc_recv_hnds_locked(MQTTServerPkts_recvHvec, n_hnds, waitSecs);
        if (0 > rv)
        {
            if (MQTT_PACKET_ERR_MAXCONN == rv)
            {
                if  ( (MAX_NWCONN == max_open_socket) )
                {
                    max_open_socket = highest_open_socket;
                }
            }
            else
            {
                return rv;
            }
        }
    } while (1);

}

int32_t MQTTServerPkts_run(uint32_t waitSecs)   // TBD break into two functions
{
    uint32_t nwconnInfo = 0;

    /* MQTT Server Run invoked */
    if (NULL == MQTTServerPkts_netOps)
    {
        return MQTT_PACKET_ERR_NET_OPS;
    }

    if (MQTTServerPkts_loopbackPort)
    {
        MQTTServerPkts_loopbNet = MQTTServerPkts_netOps->open(MQTT_DEV_NETCONN_OPT_UDP, NULL, MQTTServerPkts_loopbackPort, NULL);
        if (-1 == MQTTServerPkts_loopbNet)
        {
            return MQTT_PACKET_ERR_LIBQUIT;
        }
    }
    if (*((uint32_t *)MQTTServerPkts_srvSecure.cipher))
    {
        nwconnInfo = MQTT_DEV_NETCONN_OPT_SEC;
    }
    MQTTServerPkts_listenNet = MQTTServerPkts_netOps->listen(nwconnInfo, MQTTServerPkts_listenerPort, &MQTTServerPkts_srvSecure);

    if (-1 == MQTTServerPkts_listenNet)
    {
        return MQTT_PACKET_ERR_LIBQUIT;
    }

    return net_hnds_mon_loop(waitSecs);
}

int32_t MQTTServerPkts_registerNetSvc(const MQTT_DeviceNetServices_t *net)
{
    if (net && net->open && net->send && net->recv && net->sendTo && net->recvFrom && net->close && net->listen && net->accept && net->ioMon && net->time)
    {
        MQTTServerPkts_netOps = net;
        return 0;
    }

    return -1;
}

int32_t MQTTServerPkts_libInit(const MQTTServerPkts_LibCfg_t *libCfg, const MQTTServerPkts_MsgCBs_t *msg_cbs)
{
    MQTTServerPkts_usedCtxs = NULL;

    cl_ctx_init();
    MQTTServerPkts_loopbNet = -1;
    MQTTServerPkts_pendingTrigs = false;
    MQTTServerPkts_listenNet = -1;

    if ((NULL == libCfg) || (0 == libCfg->listenerPort))
    {
        return -1;
    }

    MQTTServerPkts_loopbackPort = libCfg->loopbackPort;
    MQTTServerPkts_listenerPort = libCfg->listenerPort;
    memcpy(&MQTTServerPkts_srvSecure, &libCfg->secure, sizeof(MQTTServerPkts_srvSecure));
    MQTTServerPkts_mutex = *(libCfg->mutex);
    mutexLockin = libCfg->mutexLockin;
    mutexUnlock = libCfg->mutexUnlock;

    MQTTServerPkts_usrCBs = &MQTTServerPkts_usrObj;

    memcpy(MQTTServerPkts_usrCBs, msg_cbs, sizeof(MQTTServerPkts_MsgCBs_t));

    MQTT_packetBufferAttach(&MQTTServerPkts_rxMqp, MQTTServerPkts_rxb, MQP_SERVER_RX_LEN, 0);

    return 0;
}
