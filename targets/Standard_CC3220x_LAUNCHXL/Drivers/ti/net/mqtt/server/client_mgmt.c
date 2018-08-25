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

#include "server_util.h"
#include "client_mgmt.h"
#include "server_pkts.h"

#ifndef CFG_SR_MAX_CL_ID_SIZE
#define MAX_CLIENT_ID_LEN 64
#else
#define MAX_CLIENT_ID_LEN CFG_SR_MAX_CL_ID_SIZE
#endif

#ifndef CFG_SR_MAX_NUM_CLIENT
#define MAX_CLIENTS       16 /* Must not exceed 32 */
#else
#define MAX_CLIENTS       CFG_SR_MAX_NUM_CLIENT

#if (CFG_SR_MAX_NUM_CLIENT > 32)
#error "CFG_SR_MAX_NUM_CLIENT must not exceed 32"
#endif
#endif

#define MQTTCLIENTMGMT_MQ_CONNECT_FLAG  0x00000001
#define MQTTCLIENTMGMT_CLEAN_SESS_FLAG  0x00000002
#define MQTTCLIENTMGMT_ASSIGNMENT_FLAG  0x00000004   /* No state or connected */

#define MQTTClientMgmt_isClUsrFree(usr) ((0 == usr->flags) && ('\0' == usr->clientID[0]))
#define MQTTClientMgmt_isClInactive(usr) ((0 == usr->flags) && ('\0' != usr->clientID[0]))

#define MQTTClientMgmt_needToWaitListPublish(qos, usr)     \
        ((((MQTT_QOS1 == qos) && hasCleanSession(usr)) ||  \
          ((MQTT_QOS0 == qos)))?                           \
         false : true)

#define MQTTClientMgmt_mqpMapGet(mqp)        (mqp->sessionDataPresent)
#define MQTTClientMgmt_mqpMapSet(mqp, clMap) (mqp->sessionDataPresent |=  clMap)
#define MQTTClientMgmt_mqpMapClr(mqp, clMap) (mqp->sessionDataPresent &= ~clMap)

#define MQTTClientMgmt_bmap(usr)             (1 << usr->index)

//*****************************************************************************
// typedefs
//*****************************************************************************

/*
 Data structure which holds will data message.
 */
typedef struct _MQTTClientMgmt_WillParams_t_
{
    MQTT_UTF8String_t willMsg;   /**< Will message */
    uint8_t           willQoS;   /**< Will QoS     */
    bool              retain;    /**< Retain Flag  */
    void *leafWithClientWill;    /**< leaf corresponding to will Topic */

}MQTTClientMgmt_WillParams_t;

typedef struct _MQTTClientMgmt_usr_t_
{
    void *ctx; /* Client net */
    void *app; /* Client app */
    MQTTClientMgmt_WillParams_t *will; /* Will parameters */
    uint32_t index;
    uint32_t n_sub;
    uint32_t flags;
    char clientID[MAX_CLIENT_ID_LEN];
    MQTT_PubQOS2CQ_t qos2_rx_cq;
    MQTT_PubQOS2CQ_t qos2_tx_cq;

}MQTTClientMgmt_usr_t;

//*****************************************************************************
//globals
//*****************************************************************************
MQTTClientMgmt_usr_t users[MAX_CLIENTS];

static MQTT_AckWlist_t MQTTClientMgmt_wlQosAck1 = { NULL, NULL };
static MQTT_AckWlist_t *MQTTClientMgmt_pWlQosAck1 = &MQTTClientMgmt_wlQosAck1;
static MQTT_AckWlist_t MQTTClientMgmt_wlMqpSess = { NULL, NULL };
static MQTT_AckWlist_t *MQTTClientMgmt_pWlMqpSess = &MQTTClientMgmt_wlMqpSess;


//*****************************************************************************
// Internal Routines
//*****************************************************************************

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool isConnected(MQTTClientMgmt_usr_t *usr)
{
    return ((usr->flags & MQTTCLIENTMGMT_MQ_CONNECT_FLAG) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool hasCleanSession(MQTTClientMgmt_usr_t *usr)
{
    return ((usr->flags & MQTTCLIENTMGMT_CLEAN_SESS_FLAG) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool isAssigned(MQTTClientMgmt_usr_t *usr)
{
    return ((usr->flags & MQTTCLIENTMGMT_ASSIGNMENT_FLAG) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void setConnectState(MQTTClientMgmt_usr_t *usr, bool connected)
{
    if (connected)
    {
        usr->flags |= MQTTCLIENTMGMT_MQ_CONNECT_FLAG;
    }
    else
    {
        usr->flags &= ~MQTTCLIENTMGMT_MQ_CONNECT_FLAG;
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void setCleanSession(MQTTClientMgmt_usr_t *usr, bool cleanSession)
{
    if (cleanSession)
    {
        usr->flags |= MQTTCLIENTMGMT_CLEAN_SESS_FLAG;
    }
    else
    {
        usr->flags &= ~MQTTCLIENTMGMT_CLEAN_SESS_FLAG;
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void setAssigned(MQTTClientMgmt_usr_t *usr,bool assignment)
{
    if (assignment)
    {
        usr->flags |= MQTTCLIENTMGMT_ASSIGNMENT_FLAG;
    }
    else
    {
        usr->flags &= ~MQTTCLIENTMGMT_ASSIGNMENT_FLAG;
    }
}

//*****************************************************************************
//
//! \brief Clear all will dynamic memory fields.
//
//*****************************************************************************
static void clUsrWill(MQTTClientMgmt_usr_t *usr)
{
    /* If will message exists - free the allocated memory */
    if (NULL != usr->will)
    {
        /* the buffer parameter of the WillMsg is an
           allocated memory, free it */
        free((void *)(usr->will->willMsg.buffer));
        /* the will parameter is an allocated memory, free it */
        free((void *)(usr->will));
        usr->will = NULL;
    }
}

//*****************************************************************************
//
//! \brief Reset all the user fields
//
//*****************************************************************************
static void clUsrReset(MQTTClientMgmt_usr_t *usr)
{
    usr->ctx = NULL;
    usr->app = NULL;

    /* If will message exists - free the allocated memory */
    clUsrWill(usr);

    usr->n_sub = 0;
    usr->flags = 0;

    usr->clientID[0] = '\0';

    MQTT_qos2PubCqReset(&usr->qos2_rx_cq);
    MQTT_qos2PubCqReset(&usr->qos2_tx_cq);
}

//*****************************************************************************
//
//! \brief Initialize all users in the user list
//
//*****************************************************************************
static void clUsrInit(void)
{
    int32_t idx = 0;

    for (idx = 0; idx < MAX_CLIENTS; idx++)
    {
        MQTTClientMgmt_usr_t *usr = users + idx;
        usr->index = idx;

        clUsrReset(usr);
    }
}

//*****************************************************************************
//
//! \brief Free and clean specific user
//
//*****************************************************************************
static void clUsrFree(MQTTClientMgmt_usr_t *usr)
{
    clUsrReset(usr);
    usr->flags &= ~(MQTTCLIENTMGMT_MQ_CONNECT_FLAG |
    MQTTCLIENTMGMT_CLEAN_SESS_FLAG |
    MQTTCLIENTMGMT_ASSIGNMENT_FLAG);
}

/*----------------------------------------------------------------------------
 * QoS2 PUB RX Message handling mechanism and associated house-keeping
 *--------------------------------------------------------------------------*/
//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool qos2PubRxLogup(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    return (MQTT_qos2PubCqLogup(&usr->qos2_rx_cq, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool ack2MsgIdLogup(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    return (MQTT_qos2PubCqLogup(&usr->qos2_tx_cq, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool qos2PubRxUnlog(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    return (MQTT_qos2PubCqUnlog(&usr->qos2_rx_cq, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool ack2MsgIdUnlog(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    return (MQTT_qos2PubCqUnlog(&usr->qos2_tx_cq, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool qos2PubRxIsDone(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    return (MQTT_qos2PubCqCheck(&usr->qos2_rx_cq, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void ack2MsgIdDispatch(MQTTClientMgmt_usr_t *usr)
{
    MQTT_PubQOS2CQ_t     *tx_cq = &usr->qos2_tx_cq;
    MQTT_ListPubQOS2CQ_t *rdIdx = tx_cq->rdIdx;

    for (; NULL != rdIdx; rdIdx = rdIdx->next)
    {
        if (MQTTServerPkts_sendVhMsg(usr->ctx, MQTT_PUBREL, MQTT_QOS1,
                            true, rdIdx->msgID) <= 0)
        {
            break;
        }
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void ack2MsgIdPurge(MQTTClientMgmt_usr_t *usr)
{
    MQTT_qos2PubCqReset(&usr->qos2_tx_cq);
    MQTT_qos2PubCqReset(&usr->qos2_rx_cq);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline int32_t _pubDispatch(MQTTClientMgmt_usr_t *usr, MQTT_Packet_t *mqp, bool dup)
{
    /* Error, if any, is handled in 'cl_net_close()' ....*/
    return (MQTTServerPkts_dispatchPub(usr->ctx, mqp, dup));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void ack1WlMqpDispatch(MQTTClientMgmt_usr_t *usr)
{
    MQTT_Packet_t *mqp = NULL;

    for (mqp = MQTTClientMgmt_pWlQosAck1->head; NULL != mqp; mqp = mqp->next)
    {
        if (MQTTClientMgmt_mqpMapGet(mqp) & MQTTClientMgmt_bmap(usr))
        {
            _pubDispatch(usr, mqp, true);
        }
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline uint32_t _clBmapGet(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    return (MQTTClientMgmt_isClUsrFree(usr) ? 0 : MQTTClientMgmt_bmap(usr));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void pubDispatch(uint32_t clMap, MQTT_Packet_t *mqp)
{
    uint32_t sp_map = 0; /* client Map for sessions present */
    MQTT_QOS qos = MQTT_FH_BYTE1_QOS(mqp->fhByte1);/* QOS */
    int32_t i = 0;
    MQTT_Packet_t *cpy;
    MQTTClientMgmt_usr_t *usr;

    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clMap & (1 << i))
        {
            usr = users + i;
            if (isConnected(usr))
            {
                if ((_pubDispatch(usr, mqp, false) > 0) && MQTTClientMgmt_needToWaitListPublish(qos, usr))
                {
                    continue;/* Processing done; next CL */
                }
            }
            /* CL: unconnected / PUB Err / QOS1 PKT (clean sess) */
            clMap &= ~(1 << i);

            if (MQTTClientMgmt_isClInactive(usr))
            {
                sp_map |= (1 << i); /* CL: Maintain session */
            }
        }
    }

    if (sp_map)
    {
        cpy = MQTTServerUtil_mqpCopy(mqp); /* Make copy */
        if (cpy)
        {
            MQTTClientMgmt_mqpMapSet(cpy, sp_map);
            MQTT_packetAckWlistAppend(MQTTClientMgmt_pWlMqpSess, cpy);
        }
        else
        {
            sp_map = 0;
        }
    }

    if (clMap)
    { /* Wait List Publish */
        MQTTClientMgmt_mqpMapSet(mqp, clMap);
        MQTT_packetAckWlistAppend(MQTTClientMgmt_pWlQosAck1, mqp);
    }
    else
    {
        MQTT_packetFree(mqp); /* PUB MQP now has no more use; must be freed */
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline int32_t lenErrFreeMqp(MQTT_Packet_t *mqp)
{
    MQTT_packetFree(mqp);
    return MQTT_PACKET_ERR_PKT_LEN;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void wlRemove(MQTT_AckWlist_t *list, MQTT_Packet_t *prev, MQTT_Packet_t *elem)
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
    return;
}

//*****************************************************************************
//
//! \brief The sessWlMqpDispatch function sends stored messages for clients in
//! persistent session.
//
//*****************************************************************************
static void sessWlMqpDispatch(MQTTClientMgmt_usr_t *usr)
{
    MQTT_Packet_t *mqp = NULL;
    MQTT_Packet_t *prev = NULL;
    MQTT_Packet_t *next = NULL;
    MQTT_Packet_t *cpy;
    uint32_t clMap = MQTTClientMgmt_bmap(usr);
    uint32_t freeMsgCounter = MQTT_MAX_PUBREL_INFLT;    /* This variable will count the messages for the specific client */

    for (mqp = MQTTClientMgmt_pWlMqpSess->head; NULL != mqp; prev = mqp, mqp = next)
    {
        cpy = NULL;

        next = mqp->next; /* Store the next node pointer */

        /* Check if the MQP is associated with this client */
        if (0 == (MQTTClientMgmt_mqpMapGet(mqp) & clMap))
        {
            continue; /* MQP & CL: no association */
        }
        /* MQP has associated client(s) -  process it */

        /* Dissociate this client from MQP */
        MQTTClientMgmt_mqpMapClr(mqp, clMap);

        /* Check if mqp is not associated with other clients */
        if (0 == MQTTClientMgmt_mqpMapGet(mqp))
        {
            /* MQP w/ no clients,  remove from WL */
            wlRemove(MQTTClientMgmt_pWlMqpSess, prev, mqp);

            /* Ensure correct arrangement for WL  */
            cpy = mqp;
            mqp = prev;
        }
        else
        {
            /* MQP is associated w/ other clients */
            cpy = MQTTServerUtil_mqpCopy(mqp);

            /* use the copied mqp only for the specific client */
            MQTTClientMgmt_mqpMapGet(cpy) = 0;

            if (NULL == cpy)
            {
                continue;
            }
        }

        /* Check if there is no more free space for publish messages */
        if (0 == freeMsgCounter)
        {
            /* if there is no free space and no more receptions for the message, delete it */
            if (0 == MQTTClientMgmt_mqpMapGet(cpy))
            {
                MQTT_packetFree(cpy); /* PUB MQP now has no more use because we have reached maximum PUB allowed; must be freed */
            }
            /* continue to the next message without dispatching the message */
            continue;
        }
        else
        {
            /* There is still free space, so decrease the counter and dispatch the message */
            freeMsgCounter--;
        }

        /* Got packet from session, dispatch it to CL */
        pubDispatch(clMap, cpy);
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static MQTT_Packet_t *wlMqpUnmapFind(MQTT_AckWlist_t *wl, MQTTClientMgmt_usr_t *usr, uint16_t msgID, MQTT_Packet_t **prev)
{
    MQTT_Packet_t *mqp = NULL;

    *prev = NULL;
    for (mqp = wl->head; NULL != mqp; *prev = mqp, mqp = mqp->next)
    {
        if (msgID == mqp->msgID)
        {
            /* Found a MQP whose msgID matches with input */
            MQTTClientMgmt_mqpMapClr(mqp, MQTTClientMgmt_bmap(usr));
            return mqp;
        }
    }

    return NULL;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool wlRmfreeTry(MQTT_AckWlist_t *wl, MQTT_Packet_t *prev, MQTT_Packet_t *mqp)
{
    if (0 == MQTTClientMgmt_mqpMapGet(mqp))
    {
        wlRemove(wl, prev, mqp);
        MQTT_packetFree(mqp);

        return true;
    }

    return false;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool ack1UnmapRmfreeTry(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    MQTT_Packet_t *prev = NULL;
    MQTT_Packet_t *mqp = NULL;

    if (false == hasCleanSession(usr))
    {
        mqp = wlMqpUnmapFind(MQTTClientMgmt_pWlQosAck1, usr, msgID, &prev);
        if (mqp)
        {
            wlRmfreeTry(MQTTClientMgmt_pWlQosAck1, prev, mqp);
        }
    }

    return true;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void wlPurge(MQTT_AckWlist_t *wl, MQTTClientMgmt_usr_t *usr)
{
    MQTT_Packet_t *mqp = NULL;
    MQTT_Packet_t *prev = NULL;
    MQTT_Packet_t *next = NULL;
    uint32_t bmap = MQTTClientMgmt_bmap(usr);

    for (mqp = wl->head; NULL != mqp; prev = mqp, mqp = next)
    {
        next = mqp->next;

        /* Ideally, check should be done to see if usr and mqp are
         associated. If yes, then the bit should be cleared. At the
         moment, blind clearing of the bit has been implemented and
         it has no side effects.
         */
        MQTTClientMgmt_mqpMapClr(mqp, bmap);

        /* MQP with no clients has no more use, so try deleting MQP */
        if (wlRmfreeTry(wl, prev, mqp))
        {
            mqp = prev; /* MQP deleted, prep for next iteration */
        }
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void ack1WlMqpPurge(MQTTClientMgmt_usr_t *usr)
{
    wlPurge(MQTTClientMgmt_pWlQosAck1, usr);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void sessWlMqpPurge(MQTTClientMgmt_usr_t *usr)
{
    wlPurge(MQTTClientMgmt_pWlMqpSess, usr);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void sessionResume(MQTTClientMgmt_usr_t *usr)
{
    ack1WlMqpDispatch(usr);
    ack2MsgIdDispatch(usr);
    sessWlMqpDispatch(usr);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void sessionDelete(MQTTClientMgmt_usr_t *usr)
{
    ack1WlMqpPurge(usr);
    ack2MsgIdPurge(usr);
    sessWlMqpPurge(usr);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool hasSessionData(MQTTClientMgmt_usr_t *usr)
{
    uint32_t map = MQTTClientMgmt_bmap(usr);
    MQTT_Packet_t *elem;

    if (usr->n_sub)
    {
        return true;
    }
    elem = MQTTClientMgmt_pWlQosAck1->head;
    while (elem)
    {
        if (MQTTClientMgmt_mqpMapGet(elem) & map)
        {
            return true;
        }
        elem = elem->next;
    }

    elem = MQTTClientMgmt_pWlMqpSess->head;
    while (elem)
    {
        if (MQTTClientMgmt_mqpMapGet(elem) & map)
        {
            return true;
        }
        elem = elem->next;
    }

    return false;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool procPubRelRx(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    MQTTServerPkts_sendVhMsg(usr->ctx, MQTT_PUBCOMP, MQTT_QOS0, true, msgID);

    if (qos2PubRxIsDone(usr, msgID))
    {
        qos2PubRxUnlog(usr, msgID);
    }
    return true;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool procPubRecRx(MQTTClientMgmt_usr_t *usr, uint16_t msgID)
{
    MQTT_Packet_t *prev = NULL;
    MQTT_Packet_t *mqp = NULL;

    mqp = wlMqpUnmapFind(MQTTClientMgmt_pWlQosAck1, usr, msgID, &prev);
    if (mqp && ack2MsgIdLogup(usr, msgID))
    {

        wlRmfreeTry(MQTTClientMgmt_pWlQosAck1, prev, mqp);

        MQTTServerPkts_sendVhMsg(usr->ctx, MQTT_PUBREL, MQTT_QOS1, true, msgID);

        return true;
    }

    return false;
}


//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void assignClIndexAsId(MQTTClientMgmt_usr_t *usr)
{
    /* TBD: need a better implementation */
    char *clientID = usr->clientID;

    clientID[0] = 'S';
    clientID[1] = 'e';
    clientID[2] = 'l';
    clientID[3] = 'f';
    clientID[4] = '-';
    clientID[5] = ((usr->index & 0xf0) >> 4) + 0x30;
    clientID[6] = ((usr->index & 0x0f)) + 0x30;
    clientID[7] = '\0';

    /* Make sure that above array size is with in MAX_CLIENT_ID_LEN */

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static MQTTClientMgmt_usr_t *assignClUsr(char *clientID, uint8_t *vhBuf)
{
    MQTTClientMgmt_usr_t *usr;
    MQTTClientMgmt_usr_t *fr_usr = NULL;
    int32_t idx = 0;

    for (idx = 0; idx < MAX_CLIENTS; idx++)
    {
        usr = users + idx;
        if ((NULL == fr_usr) && MQTTClientMgmt_isClUsrFree(usr))
        {
            fr_usr = usr; /* Note 1st free usr */
        }
        if ((NULL == clientID) && (NULL != fr_usr))
        {
            /* Free usr is present to create CL-ID */
            break;
        }
        else if ((NULL != clientID) &&
                (0 == strncmp(usr->clientID, clientID,MAX_CLIENT_ID_LEN)))
        {
            /* Found a client obj with exact ID match */
            if (isConnected(usr))
            {
                /* Error: CL-ID is already active */
                vhBuf[1] = MQTT_CONNACK_RC_CLI_REJECT;
                usr = NULL;
            }
            break;
        }
    }

    if (idx == MAX_CLIENTS)
    { /* Did not find a match */
        usr = fr_usr;
        if (NULL == usr)
        {
            /* Server Resource unavailable */
            vhBuf[1] = MQTT_CONNACK_RC_SVR_UNAVBL;
        }
    }

    if (NULL != usr)
    {
        if (NULL == clientID)
        {
            assignClIndexAsId(usr); /* Get ID */
        }
        else if (MQTTClientMgmt_isClUsrFree(usr))
        {
            strncpy(usr->clientID, clientID,MAX_CLIENT_ID_LEN);
        }
        setAssigned(usr, true);
    }

    return usr;
}

//*****************************************************************************
// External Routines
//*****************************************************************************

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void MQTTClientMgmt_subCountAdd(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    if (isConnected(usr))
    {
        usr->n_sub++;
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void MQTTClientMgmt_subCountDel(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    if (isConnected(usr))
    {
        usr->n_sub--;
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
bool MQTTClientMgmt_qos2PubRxUpdate(void *usrCl, uint16_t msgID)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    return (usr && (qos2PubRxIsDone(usr, msgID) || qos2PubRxLogup(usr, msgID)));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
uint32_t MQTTClientMgmt_bmapGet(void *usrCl)
{
    return (usrCl ? _clBmapGet(usrCl) : 0);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void *MQTTClientMgmt_appHndlGet(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    return ((usr && isConnected(usrCl)) ? usr->app : NULL);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void *MQTTClientMgmt_willHndlGet(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    return ((usr && isConnected(usrCl)) ? usr->will : NULL);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void MQTTClientMgmt_pubDispatch(uint32_t clMap, MQTT_Packet_t *mqp)
{
    pubDispatch(clMap, mqp);
    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
int32_t MQTTClientMgmt_pubMsgSend(void *usrCl, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf, uint32_t dataLen, MQTT_QOS qos, bool retain)
{
    MQTT_Packet_t *mqp = NULL;

    if ((NULL == topic) || ((dataLen > 0) && (NULL == dataBuf)) || (NULL == usrCl))
    {
        return MQTT_PACKET_ERR_FNPARAM;
    }
    mqp = MQTTServerUtil_mqpAlloc(MQTT_PUBLISH, 2 + topic->length + 2 + dataLen);
    if (NULL == mqp)
    {
        return MQTT_PACKET_ERR_PKT_AVL;
    }
    if ((0 > MQTT_packetPubAppendTopic(mqp, topic, qos ? MQTTServerUtil_setMsgID() : 0)) || (dataLen && (0 > MQTT_packetPubAppendData(mqp, dataBuf, dataLen))))
    {
        return lenErrFreeMqp(mqp);
    }
    MQTT_packetPrepFh(mqp, MQTT_MAKE_FH_FLAGS(false, qos, retain));

    pubDispatch(_clBmapGet(usrCl), mqp);

    return MQTT_PACKET_CONTENT_LEN(mqp);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
bool MQTTClientMgmt_canSessionDelete(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    return (usr ? (hasCleanSession(usr) || !hasSessionData(usr)) : false);
}

//*****************************************************************************
//
//! \brief Free user data unless the user is connected with clean session
//! disabled
//
//*****************************************************************************
void MQTTClientMgmt_onNetClose(void *usrCl)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    /* Check if the user is in CONNACK state */
    if (isAssigned(usr))
    {
        /* Check if the user is in session data, if not clear user data */
        if (false == hasSessionData(usr))
        {
            clUsrFree(usr);
        }
    }
    /* If the user is in clean session, clear session and user data */
    else if (hasCleanSession(usr))
    {
        sessionDelete(usr);
        clUsrFree(usr);
    }
    else
    {
        /* If the user is in persistent session, change the user
           state to disconnect and free the will data */
        setConnectState(usr, false);
        clUsrWill(usr);
        usr->ctx = NULL;
        usr->app = NULL;
    }

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
bool MQTTClientMgmt_notifyAck(void *usrCl, uint8_t msgType, uint16_t msgID)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;
    bool rv = false;

    if (NULL == usr)
    {
        return rv;
    }
    switch (msgType)
    {
        case MQTT_PUBACK:
            rv = ack1UnmapRmfreeTry(usr, msgID);
            break;

        case MQTT_PUBREC:
            rv = procPubRecRx(usr, msgID);
            break;

        case MQTT_PUBREL:
            rv = procPubRelRx(usr, msgID);
            break;

        case MQTT_PUBCOMP:
            rv = ack2MsgIdUnlog(usr, msgID);
            break;

        default:
            break;
    }

    return rv;
}

//*****************************************************************************
//
//! \brief Assign user for the client and save the client parameters
//
//*****************************************************************************
uint16_t MQTTClientMgmt_connectRx(void *ctxCl, bool cleanSession, char *clientID, void *appCl, void *will, void **usrCl)
{
    uint8_t vhBuf[] = { 0x00, MQTT_CONNACK_RC_REQ_ACCEPT };
    MQTTClientMgmt_usr_t *usr;

    if (clientID && ('\0' == clientID[0]))
    {
        /* Shouldn't happen: CLIENT ID with a NUL only string */
        return MQTT_CONNACK_RC_CLI_REJECT;
    }
    usr = assignClUsr(clientID, vhBuf);
    if (NULL == usr)
    {
        return vhBuf[1]; /* Use vhBuf from assignClUsr() */
    }
    if ((false == cleanSession) && hasSessionData(usr))
    {
        vhBuf[0] = MQTT_VH_SESSION_PRESENT; /* Set Session Present Flag */
    }
    *usrCl = (void *)usr;

    usr->ctx = ctxCl;
    usr->app = appCl;
    usr->will = will;

    return ((vhBuf[0] << 8) | vhBuf[1]);
}

//*****************************************************************************
//
//! \brief Notify that CONNACK has been sent to the specified client.
//! This routine is invoked by the LIB to enable the application to make
//! progress and follow-up on the session information for the particular
//! client. Specifically, this routine facilitates the application to
//! either delete the session or re-send / sync-up the pending messages
//! associated with the client.
//
//*****************************************************************************
void MQTTClientMgmt_onConnackSend(void *usrCl, bool cleanSession)
{
    MQTTClientMgmt_usr_t *usr = (MQTTClientMgmt_usr_t *)usrCl;

    /* Check if the usr is connected and has CONNACK assigned */
    if (false == isAssigned(usr))
    {
        /* Fatal: CONNACK for unassigned user Id */
        return;
    }

    setAssigned(usr, false);
    setCleanSession(usr, cleanSession);
    setConnectState(usr, true);

    if (cleanSession)
    {
        sessionDelete(usr);
    }
    else
    {
        sessionResume(usr);
    }

    /* Server is now connected to client */
    return;
}

//*****************************************************************************
//
//! \brief Initialize the User list and the Message ID counter
//
//*****************************************************************************
int32_t MQTTClientMgmt_init(void)
{
    /* Initialize the user list */
    clUsrInit();
    /* Reset the Message ID counter */
    MQTTServerUtil_resetMsgID();

    return 0;
}
