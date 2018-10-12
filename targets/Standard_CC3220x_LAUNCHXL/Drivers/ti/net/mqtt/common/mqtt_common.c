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

#include "mqtt_common.h"

#define RET_IF_ERR_IN_NET_RECV(net, buf, len, waitSecs, timedOut, ctx) \
        rv = netOps->recv(net, buf, len, waitSecs, timedOut, ctx);     \
        if(rv < 1) {return MQTT_PACKET_ERR_NETWORK;}

#define MQTT_MAX_REMLEN_BYTES  (MQTT_MAX_FH_LEN - 1)

#define MSG_TYPE(fhByte1)  ((uint8_t)(((fhByte1) & 0xf0) >> 4))

static bool mqpProcVhMsgIdRx(MQTT_Packet_t *mqpRaw)
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

void MQTT_packetFree(MQTT_Packet_t *mqp)
{
    if ((NULL != mqp->free) && (0 == --mqp->nRefs))
    {
        mqp->free(mqp);
    }
}

void MQTT_packetReset(MQTT_Packet_t *mqp)
{
    /* Fields not handled here are meant to be left unaltered. */
    mqp->msgType            = 0x00;
    mqp->fhByte1            = 0x00;
    mqp->msgID              = 0;
    mqp->fhLen              = 0x00;
    mqp->vhLen              = 0;
    mqp->plLen              = 0;
    mqp->sessionDataPresent = 0;
}

void MQTT_packetInit(MQTT_Packet_t *mqp, uint8_t offset)
{
    MQTT_packetReset(mqp);

    mqp->offset = offset;
    mqp->nRefs  = 0x01;
    mqp->next   = NULL;
}

int32_t MQTT_packetBufWrUtf8(uint8_t *buf, const MQTT_UTF8String_t *utf8)
{
    uint8_t *ref = buf;

    buf += MQTT_bufWrNbo2B(buf, utf8->length);
    buf += MQTT_bufWrNbytes(buf, (uint8_t*)utf8->buffer, utf8->length);

    return (buf - ref);
}

int32_t MQTT_packetBufTailWrRemlen(uint8_t *buf, uint32_t remlen)
{
    uint8_t val[MQTT_MAX_REMLEN_BYTES];
    uint8_t i = 0;

    do
    {
        val[i] = remlen & 0x7F; /* MOD 128 */
        remlen = remlen >> 7; /* DIV 128 */

        if (remlen)
        {
            val[i] |= 0x80;
        }
        i++;

    } while (remlen > 0);

    MQTT_bufWrNbytes(buf + MQTT_MAX_REMLEN_BYTES - i, val, i);

    return i; /* # bytes written in buf */
}

int32_t MQTT_packetBufRdRemlen(uint8_t *buf, uint32_t *remlen)
{
    uint32_t val = 0;
    uint32_t mul = 0;
    int32_t i = 0;

    do
    {
        val += (buf[i] & 0x7F) << mul;
        mul += 7;

    } while ((buf[i++] & 0x80) && (i < MQTT_MAX_REMLEN_BYTES));

    *remlen = val;

    /* Return -1 if value was not found */
    return ((buf[i - 1] & 0x80) ? -1 : i);
}

int32_t MQTT_packetPubAppendTopic(MQTT_Packet_t *mqp, const MQTT_UTF8String_t *topic, uint16_t msgID)
{
    uint8_t *buf = MQTT_PACKET_VHEADER_BUF(mqp);
    uint8_t *ref = buf;

    if (0 != mqp->vhLen)
    {
        return -1; /* Topic has been already added */
    }
    if (MQTT_PACKET_FREEBUF_LEN(mqp) < (topic + msgID ? 2 : 0))
    {
        return MQTT_PACKET_ERR_PKT_LEN; /* Can't WR topic */
    }
    buf += MQTT_packetBufWrUtf8(buf, topic);

    if (0 != msgID)
    { /* MSG ID 0 indicates ==> QoS0 */
        mqp->msgID = msgID;
        buf += MQTT_bufWrNbo2B(buf, mqp->msgID);
    }

    mqp->vhLen += buf - ref;

    return (buf - ref);
}

int32_t MQTT_packetPubAppendData(MQTT_Packet_t *mqp, const uint8_t *dataBuf, uint32_t dataLen)
{
    uint8_t *buf = MQTT_PACKET_PAYLOAD_BUF(mqp) + mqp->plLen;
    uint8_t *ref = buf;

    if (0 == mqp->vhLen)
    {
        return -1; /* Must include topic first */
    }
    if (MQTT_PACKET_FREEBUF_LEN(mqp) < dataLen)
    {
        return MQTT_PACKET_ERR_PKT_LEN; /* Can't WR  data */
    }
    /* Including payload info for PUBLISH */
    buf += MQTT_bufWrNbytes(buf, dataBuf, dataLen);
    mqp->plLen += buf - ref;

    return (buf - ref);
}

bool MQTT_packetProcMsgIdAckRx(MQTT_Packet_t *mqpRaw, bool has_pl)
{
    if ((false == mqpProcVhMsgIdRx(mqpRaw)) || (has_pl ^ (!!mqpRaw->plLen)))
    {
        return false;
    }
    return true;
}

bool MQTT_packetProcPubRx(MQTT_Packet_t *mqpRaw)
{
    uint8_t *buf = MQTT_PACKET_VHEADER_BUF(mqpRaw);
    uint8_t *ref = buf;
    uint16_t tmp = 0;

    if (mqpRaw->plLen < (buf - ref + 2)) /* Length Check  */
    {
        return false; /* No space to hold Topic size */
    }
    buf += MQTT_bufRdNbo2B(buf, &tmp); /* Topic  Length */
    buf += tmp; /* Topic Content */

    if (MQTT_QOS0 != MQTT_FH_BYTE1_QOS(mqpRaw->fhByte1))
    {
        if (mqpRaw->plLen < (buf - ref + 2))
        {
            return false; /* No space to hold MSG ID */
        }
        buf += MQTT_bufRdNbo2B(buf, &mqpRaw->msgID);
    }

    mqpRaw->vhLen += buf - ref;
    mqpRaw->plLen -= buf - ref;

    return true;
}

bool MQTT_packetAckWlistAppend(MQTT_AckWlist_t *list, MQTT_Packet_t *elem)
{
    elem->next = NULL;

    if (list->tail)
    {
        list->tail->next = elem;
        list->tail = elem;
    }
    else
    {
        list->tail = elem;
        list->head = elem;
    }

    return true;
}

MQTT_Packet_t *MQTT_packetAckWlistRemove(MQTT_AckWlist_t *list, uint16_t msgID)
{
    MQTT_Packet_t *elem = list->head;
    MQTT_Packet_t *prev = NULL;

    while (elem)
    {
        if (msgID == elem->msgID)
        {
            if (prev)
            {
                prev->next = elem->next;
            }
            else
            {
                list->head = elem->next;
            }
            if (NULL == list->head)
            {
                list->tail = NULL;
            }
            if (list->tail == elem)
            {
                list->tail = prev;
            }
            break;
        }

        prev = elem;
        elem = elem->next;
    }

    return elem;
}

void MQTT_packetAckWlistPurge(MQTT_AckWlist_t *list)
{
    MQTT_Packet_t *elem = list->head;
    MQTT_Packet_t *next;

    while (elem)
    {
        next = elem->next;
        MQTT_packetFree(elem);
        elem = next;
    }

    list->head = NULL;
    list->tail = NULL;

    return;
}

int32_t MQTT_packetPrepFh(MQTT_Packet_t *mqp, uint8_t flags)
{
    uint32_t remlen = mqp->vhLen + mqp->plLen;
    uint8_t *buf = mqp->buffer + mqp->offset;
    uint8_t *ref = buf;

    buf -= MQTT_packetBufTailWrRemlen(buf - MQTT_MAX_REMLEN_BYTES, remlen);

    buf -= 1; /* Make space for FH Byte1 */
    mqp->fhByte1 = *buf = MQTT_MAKE_FH_BYTE1(mqp->msgType, flags);

    mqp->fhLen = ref - buf;
    mqp->offset -= ref - buf;

    return (ref - buf);
}

//*****************************************************************************
//
//! \brief  Since, the network connection is a TCP socket stream, it is imperative that
//! adequate checks are put in place to identify a MQTT packet and isolate it
//! for further processing. The intent of following routine is to read just one
//! packet from continuous stream and leave rest for the next iteration.
//
//*****************************************************************************
int32_t MQTT_PacketRecv(int32_t net, const MQTT_DeviceNetServices_t *netOps, MQTT_Packet_t *mqp, uint32_t waitSecs, bool *timedOut, void *ctx)
{
    uint8_t *buf = MQTT_PACKET_FHEADER_BUF(mqp);
    uint8_t *ref = buf;
    uint8_t fhLen = mqp->fhLen;
    uint32_t plLen = mqp->plLen, remlen = 0;
    int32_t rv;

    while (2 > fhLen)
    {
        RET_IF_ERR_IN_NET_RECV(net, buf + fhLen, 1, waitSecs, timedOut, ctx);

        mqp->fhLen = ++fhLen;
    }

    while (buf[fhLen - 1] & 0x80)
    {
        if (fhLen > MQTT_MAX_FH_LEN)
        {
            return MQTT_PACKET_ERR_NOT_DEF;/* Shouldn't happen */
        }
        RET_IF_ERR_IN_NET_RECV(net, buf + fhLen, 1, waitSecs, timedOut, ctx);

        mqp->fhLen = ++fhLen;
    }

    MQTT_packetBufRdRemlen(buf + 1, &remlen);
    if (mqp->maxlen < (remlen + fhLen))
    {
        return MQTT_PACKET_ERR_PKT_LEN; /* Inadequate free buffer */
    }
    buf += fhLen; /* Try to read all data that follows FH */
    while (plLen < remlen)
    {
        RET_IF_ERR_IN_NET_RECV(net, buf + plLen, remlen - plLen, waitSecs, timedOut, ctx);

        mqp->plLen = plLen += rv;
    }

    /* Set up MQTT Packet for received data from broker */
    MQTT_bufWrNbytes(MQTT_PACKET_FHEADER_BUF(mqp), ref, fhLen);
    mqp->fhByte1 = *ref;
    mqp->msgType = MSG_TYPE(*ref);

    return (fhLen + remlen);
}

void MQTT_secureConnStructInit(MQTT_SecureConn_t *nwSecurity)
{
    nwSecurity->method = nwSecurity->cipher = NULL;
    nwSecurity->files = NULL;
    nwSecurity->nFile = 0;

    return;
}
//*****************************************************************************
//
//! \brief QoS2 PUB RX Message handling mechanism and associated house-keeping
//
//*****************************************************************************
void MQTT_qos2PubCqReset(MQTT_PubQOS2CQ_t *cq)
{
    MQTT_ListPubQOS2CQ_t *temp;

    cq->nFree = MQTT_MAX_PUBREL_INFLT;
    cq->rdIdx = NULL;
    cq->wrIdx = NULL;
    while (NULL != cq->idVec)
    {
        temp = cq->idVec;
        cq->idVec = cq->idVec->next;
        free(temp);
    }

    return;
}

bool MQTT_qos2PubCqLogup(MQTT_PubQOS2CQ_t *cq, uint16_t msgID)
{
    MQTT_ListPubQOS2CQ_t * temp;
    if (0 != cq->nFree)
    {
        temp = (MQTT_ListPubQOS2CQ_t *)calloc(1, sizeof(MQTT_ListPubQOS2CQ_t));
        /* Check if memory allocation was successful */
        if (NULL == temp )
        {
            return false;
        }

        temp->msgID = msgID;
        if ( (NULL == cq->wrIdx) || (NULL == cq->idVec) )
        {
            cq->idVec = temp;
            cq->wrIdx = cq->idVec;
        }
        else
        {
            cq->wrIdx->next = temp;
            temp->prev = cq->wrIdx;
            cq->wrIdx = temp;
        }
        cq->nFree--;
        return true;
    }

    /* Must ensure through an out-of-band arrangement that a remote doesn't
     push more than MQTT_MAX_PUBREL_INFLT in-flight QOS2 PUB packets to local.
     If it still happens that the local receives more in-flight messages,
     then it has no option but to close the network connection.

     Closing of network connection is left to invoker to of this service.
     */

    return false;
}

bool MQTT_qos2PubCqUnlog(MQTT_PubQOS2CQ_t *cq, uint16_t msgID)
{
    MQTT_ListPubQOS2CQ_t * temp;

    /* Simple implementation. Should be good for embedded system */
    if (cq->nFree < MQTT_MAX_PUBREL_INFLT)
    {
        /* Follow-up messages for QOS2 PUB must be transacted in the
         same order as the initial sequence of QOS2 PUB dispatches.
         Therefore, checking the first entry should be OK
         */
        if (NULL == cq->rdIdx)
        {
            cq->rdIdx = cq->idVec;
        }
        if (msgID == cq->rdIdx->msgID)
        {
            if (NULL != cq->rdIdx->next)
            {
                if (NULL != cq->rdIdx->prev)
                {
                    cq->rdIdx->prev->next = cq->rdIdx->next;
                    cq->rdIdx->next->prev = cq->rdIdx->prev;
                }
                else
                {
                    cq->rdIdx->next->prev = NULL;
                }
                temp = cq->rdIdx->next;
            }
            else
            {
                if (NULL != cq->rdIdx->prev)
                {
                    cq->rdIdx->prev->next = NULL;
                }
                else
                {
                    cq->idVec = NULL;
                }
                temp = cq->idVec;
            }
            free(cq->rdIdx);
            cq->rdIdx = temp;
            cq->nFree++;
            return true;
        }
        else
        {
            if (NULL == cq->rdIdx->next)
            {
                cq->rdIdx = cq->idVec;
            }
            else
            {
                cq->rdIdx = cq->rdIdx->next;
            }
        }
    }
    return false;
}

bool MQTT_qos2PubCqCheck(MQTT_PubQOS2CQ_t *cq, uint16_t msgID)
{
    uint8_t nFree = cq->nFree;
    uint8_t i = 0;
    bool    msgEql = false;

    /* Go through the vector to see if packet ID is still active */
    for (i = 0; i < (MQTT_MAX_PUBREL_INFLT - nFree); i++)
    {
        if (NULL == cq->rdIdx)
        {
            cq->rdIdx = cq->idVec;
        }
        if (msgID == cq->rdIdx->msgID)
        {
            msgEql = true;
        }
        if (NULL == cq->rdIdx->next)
        {
            cq->rdIdx = cq->idVec;
        }
        else
        {
            cq->rdIdx = cq->rdIdx->next;
        }
        if (true == msgEql)
        {
            return true;
        }
    }

    return false;
}

//*****************************************************************************
//
//! \brief Client Context related details and associated house-keeping
//
//*****************************************************************************
void MQTT_clCtxReset(MQTT_ClientCtx_t *clCtx)
{
    clCtx->usr = NULL;
    clCtx->net = -1;
    clCtx->ipLen = 16;

    clCtx->timeout = 0;
    clCtx->kaSecs = 0;

    clCtx->flags = 0;

    return;
}

void MQTT_clCtxTimeoutInsert(MQTT_ClientCtx_t **head, MQTT_ClientCtx_t *elem)
{
    MQTT_ClientCtx_t *curr;
    MQTT_ClientCtx_t *prev = NULL;

    if (NULL == *head)
    {
        *head = elem;
        return;
    }

    curr = *head;
    while (curr)
    {
        if (elem->timeout < curr->timeout)
        {
            if (NULL == prev)
            {
                elem->next = *head;
                *head = elem;
            }
            else
            {
                prev->next = elem;
                elem->next = curr;
            }

            break;
        }

        prev = curr;
        curr = curr->next;
    }

    if (NULL == curr)
    {
        prev->next = elem;
    }
}

void MQTT_clCtxRemove(MQTT_ClientCtx_t **head, MQTT_ClientCtx_t *elem)
{
    MQTT_ClientCtx_t *curr = *head;
    MQTT_ClientCtx_t *prev = NULL;

    while (curr)
    {
        if (curr == elem)
        {
            if (NULL == prev)
            {
                *head = elem->next;
                prev = *head;
            }
            else
            {
                prev->next = elem->next;
            }

            elem->next = NULL;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void MQTT_clCtxTimeoutUpdate(MQTT_ClientCtx_t *clCtx, uint32_t nowSecs)
{
    uint32_t timeout = MQTT_KA_TIMEOUT_NONE;
    uint16_t kaSecs = clCtx->kaSecs;

    if ((0 == kaSecs) && (MQTT_KA_TIMEOUT_NONE == clCtx->timeout))
    {
        return;
    }
    if (0 != kaSecs)
    {
        timeout = nowSecs + kaSecs;
    }
    clCtx->timeout = timeout;

    return;
}
