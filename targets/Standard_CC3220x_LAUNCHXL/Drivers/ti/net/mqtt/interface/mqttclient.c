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

#include <ti/net/mqtt/mqttclient.h>
#include <ti/net/mqtt/platform/mqtt_net_func.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>

#ifndef CFG_SL_CL_BUF_LEN
#define MQTTCLIENT_BUF_LEN          1024 /*Buffer length*/
#else
#define MQTTCLIENT_BUF_LEN          CFG_SL_CL_BUF_LEN
#endif

#ifndef CFG_SL_CL_MAX_MQP
#define MQTTCLIENT_MAX_MQP          2 /* # of buffers */
#else
#define MQTTCLIENT_MAX_MQP          CFG_SL_CL_MAX_MQP
#endif

/*Defining QOS value. Getting QOS value from fixed header*/
#define MQTTClient_mqpPubQos(mqp)        ((mqp->fhByte1 >>1)  & 0x03)

#define MQTTClient_ackRxSignalWait(ackSyncObj)     sem_wait(ackSyncObj)
#define MQTTClient_ackRxSignalPost(ackSyncObj)     sem_post(ackSyncObj)

#define MQTTClient_str2UTFConv(utf_s, str) {utf_s.buffer = (char *)str; utf_s.length = strlen(str);}

/*Defining Event Messages*/
#define MQTTCLIENT_ACK "Ack Received from server"
#define MQTTCLIENT_ERROR "Connection Lost with broker"
#define MQTTCLIENT_DISCONNECT_CB  0xCB /* Custom define for implementation ease */

#define MQTTCLIENT_MAX_SIMULTANEOUS_SUB_TOPICS 4

#define MQTTCLIENT_MAX_SIMULTANEOUS_UNSUB_TOPICS 4

#ifndef CFG_CL_MQTT_CTXS
#define MQTTCLIENT_MAX_SIMULTANEOUS_SERVER_CONN 4
#else
#define MQTTCLIENT_MAX_SIMULTANEOUS_SERVER_CONN CFG_MQTT_CL_CTXS
#endif

#define MQTTClient_rxTxSemWait()     sem_wait(MQTTClient_Lib_CB.MQTTClient_syncRxTx)
#define MQTTClient_rxTxSemPost()     sem_post(MQTTClient_Lib_CB.MQTTClient_syncRxTx)

#define MQTTClient_mutexLock()   pthread_mutex_lock(MQTTClient_Lib_CB.MQTTClient_mutex); // forever
#define MQTTClient_mutexUnlock() pthread_mutex_unlock(MQTTClient_Lib_CB.MQTTClient_mutex);

/* MQTT Lib structure which holds Initialization Data */
typedef struct _MQTTClient_LibCfg_t_
{
    /**< Loopback port is used to manage lib internal functioning in case of connections to
         multiple servers simultaneously is desired. */
    uint16_t loopbackPort;  /**< Loopback port = 0, implies connection to only single server */
                            /**< Loopback port != 0, implies connection to multiple servers */
    uint32_t respTime;      /**< Reasonable response time (seconds) from server */
} MQTTClient_LibCfg_t;


typedef struct _MQTTClient_ConnectCfg_t_
{
    bool clean; /**< used to specify a clean or persistent session with broker */
    uint16_t keepAliveTimeout;

} MQTTClient_ConnectCfg_t;


typedef struct _MQTTClient_Ctx_t_
{
    /* Client information details */
    char *clientID;
    char *usrName;
    char *usrPwd;
    MQTTClient_Will mqttWill;

    /* Client management variables */
    bool inUse;
    char awaitedAck;
    uint16_t connAck;

    /*Sync Object used to ensure single in-flight message -
     used in blocking mode to wait on ack*/
    sem_t *ackSyncObj;
    /* Suback QoS pointer to store passed by application */
    char *subackQos;

    /* Application information */
    void *appHndl;
    MQTTClient_CallBack appCBs;

    /* Library information */
    void *cliHndl;
    bool blockingSend;
    bool connectedState;

}MQTTClient_Ctx_t;

MQTTClient_LibCfg_t MQTTClient_clientCfg =
{
    0,
    30
};

int32_t           MQTTClient_closeContext = 0;
MQTTClient_Ctx_t *MQTTClient_clientContext = NULL;

static MQTTClient_Ctx_t MQTTClient_ctx[MQTTCLIENT_MAX_SIMULTANEOUS_SERVER_CONN];

/* Creating a pool of MQTT constructs that can be used by the MQTT Lib
 MQTTClient_packet => pointer to a pool of the mqtt packet constructs
 MQTTClient_buffer => the buffer area which is attached with each of MQTTClient_packet*/
static MQTT_Packet_t MQTTClient_packet[MQTTCLIENT_MAX_MQP];
static uint8_t MQTTClient_buffer[MQTTCLIENT_MAX_MQP][MQTTCLIENT_BUF_LEN];

/*Task Priority and Response Time*/
uint16_t MQTTClient_waitSecs;

bool MQTTClient_multiSrvConn = false;
MQTTClient_ConnectCfg_t MQTTClient_connectCfg = { true, 25 };

/* MQTT Quality of Service */
static const MQTT_QOS MQTTClient_qos[] =
{
    MQTT_QOS0,
    MQTT_QOS1,
    MQTT_QOS2
};

/* Network Services functions*/
MQTT_DeviceNetServices_t MQTTClient_net =
{
    MQTTNet_commOpen, MQTTNet_tcpSend, MQTTNet_tcpRecv, MQTTNet_sendTo,
    MQTTNet_recvFrom,  MQTTNet_commClose, MQTTNet_tcpListen,
    MQTTNet_tcpAccept, MQTTNet_tcpSelect, MQTTNet_rtcSecs
};

typedef struct _MQTTClient_LibControlBlock_t_
{
    sem_t *MQTTClient_waitForMqttClose;

    /* Lock Object to be passed to the MQTT lib */
    pthread_mutex_t *MQTTClient_libLock;

    /* Synchronization object used between the Rx and App task */
    sem_t *MQTTClient_syncRxTx;

    pthread_mutex_t *MQTTClient_mutex; /* Multi-task Lock */

}MQTTClient_LibControlBlock_t;

MQTTClient_LibControlBlock_t MQTTClient_Lib_CB;

static inline void SetCondAwaitedAckLocked(MQTTClient_Ctx_t *clientCtx, char cond, char val)
{
    MQTTClient_mutexLock();
    if(cond == clientCtx->awaitedAck)
    {
        clientCtx->awaitedAck = val;
    }
    MQTTClient_mutexUnlock();
}

static void mutexLockup(pthread_mutex_t *mqttLibLock)
{
    pthread_mutex_lock(mqttLibLock); //forever
}

static void mutexUnlock(pthread_mutex_t *mqttLibLock)
{
    pthread_mutex_unlock(mqttLibLock);
}

//*****************************************************************************
//
//! \brief Wait for the response (callback) from the server (network)
//
//*****************************************************************************
int32_t _sl_ExtLib_ServerAckLockedWait(MQTTClient_Ctx_t *clientCtx)
{
    int32_t rc = 0;

    MQTTClient_mutexLock();
    if (true == clientCtx->blockingSend)
    {
        MQTTClient_mutexUnlock();
        MQTTClient_ackRxSignalWait(clientCtx->ackSyncObj);
        MQTTClient_mutexLock();
        rc = (MQTT_DISCONNECT == clientCtx->awaitedAck) ? MQTT_PACKET_ERR_NOTCONN : 0;
    }

    MQTTClient_mutexUnlock();

    return rc;
}

//*****************************************************************************
//
//! \brief Set (unprotected) in the implementation the value of the ACK from the server
//
//*****************************************************************************
bool _sl_ExtLib_AwaitedAckSet(MQTTClient_Ctx_t *clientCtx, uint8_t newAwaited)
{
    bool rc = false;

    if (MQTT_DISCONNECT != clientCtx->awaitedAck)
    {
        clientCtx->awaitedAck = newAwaited;
        rc = true;
    }

    return rc;
}

//*****************************************************************************
//
//! \brief Set (protected) in the implementation the value of the ACK from the server
//
//*****************************************************************************
bool _sl_ExtLib_AwaitedAckLockedSet(MQTTClient_Ctx_t *clientCtx, uint8_t newAwaited)
{
    bool rc;

    MQTTClient_mutexLock();
    rc = _sl_ExtLib_AwaitedAckSet(clientCtx, newAwaited);
    MQTTClient_mutexUnlock();

    return rc;
}

static void processNotifyAckCB(void *app, uint8_t msgType, uint16_t msgID, uint8_t *buf, uint32_t len)
{
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)app;
    int32_t loopcnt;
    MQTTClient_OperationMetaDataCB  eventCB;

    MQTTClient_mutexLock();

    switch (msgType)
    {

        case MQTT_CONNACK:
            clientCtx->connAck = ((buf[0] << 8) | buf[1]);
            clientCtx->awaitedAck = 0;
            eventCB.messageType = MQTT_CONNACK;
            /* Notify the application which ConnAck return code received from the server */
            clientCtx->appCBs( MQTTClient_OPERATION_CB_EVENT, (void *)&eventCB, sizeof(eventCB), (void *)&(clientCtx->connAck), sizeof(clientCtx->connAck));
            MQTTClient_ackRxSignalPost(clientCtx->ackSyncObj);
            break;
        case MQTT_SUBACK:
            if (true == clientCtx->blockingSend)
            {
                for (loopcnt = 0; loopcnt < len; loopcnt++)
                {
                    clientCtx->subackQos[loopcnt] = buf[loopcnt];
                }

                if (clientCtx->awaitedAck == msgType)
                {
                    clientCtx->awaitedAck = 0;
                    MQTTClient_ackRxSignalPost(clientCtx->ackSyncObj);
                }
            }
            else
            {
                eventCB.messageType = MQTT_SUBACK;
                clientCtx->appCBs( MQTTClient_OPERATION_CB_EVENT, (void *)&eventCB, sizeof(eventCB), (void *)buf, len);
            }
            break;
        default:
            if (true == clientCtx->blockingSend)
            {
                if (clientCtx->awaitedAck == msgType)
                {
                    clientCtx->awaitedAck = 0;
                    MQTTClient_ackRxSignalPost(clientCtx->ackSyncObj);
                }
            }
            else
            {
                eventCB.messageType = msgType;
                clientCtx->appCBs( MQTTClient_OPERATION_CB_EVENT, (void *)&eventCB, sizeof(eventCB), (void *)MQTTCLIENT_ACK, strlen(MQTTCLIENT_ACK));
            }
            break;

    }

    MQTTClient_mutexUnlock();

    return;
}

static bool processPublishRxCB(void *app, bool dup, MQTT_QOS qos, bool retain, MQTT_Packet_t *mqp)
{
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)app;
    MQTTClient_RecvMetaDataCB   metaData;

    MQTTClient_mutexLock();

    /*If incoming message is a publish message from broker */
    /* Invokes the event handler with topic,topic length,payload and
     payload length values*/
    if (MQTT_DISCONNECT != clientCtx->awaitedAck)
    {
        metaData.topic  = (char const *)MQTT_PACKET_PUB_TOP_BUF(mqp);
        metaData.topLen = MQTT_PACKET_PUB_TOP_LEN(mqp);
        metaData.dup    = dup;
        metaData.qos    = MQTTClient_mqpPubQos(mqp);
        metaData.retain = retain;

        clientCtx->appCBs(MQTTClient_RECV_CB_EVENT, &metaData, sizeof(metaData), MQTT_PACKET_PUB_PAY_BUF(mqp), MQTT_PACKET_PUB_PAY_LEN(mqp));
    }

    MQTTClient_mutexUnlock();
    return true;
}

static void processDisconnCB(void *app, int32_t cause)
{
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)app;

    MQTTClient_mutexLock();

    if (MQTT_DISCONNECT != clientCtx->awaitedAck)
    {
        clientCtx->awaitedAck = MQTT_DISCONNECT;
        MQTTClient_ackRxSignalPost(clientCtx->ackSyncObj);
    }
    clientCtx->appCBs(MQTTClient_DISCONNECT_CB_EVENT, NULL,0,NULL,0);

    MQTTClient_mutexUnlock();

    return;
}

static MQTTClient_Ctx_t *getAvailableClictxMem(void)
{
    int32_t loopcnt;
    int32_t maxStore;

    if (false == MQTTClient_multiSrvConn)
    {
        maxStore = 1;
    }
    else
    {
        maxStore = MQTTCLIENT_MAX_SIMULTANEOUS_SERVER_CONN;
    }

    for (loopcnt = 0; loopcnt < maxStore; loopcnt++)
    {
        if (false == MQTTClient_ctx[loopcnt].inUse)
        {
            MQTTClient_ctx[loopcnt].inUse = true;
            return (&(MQTTClient_ctx[loopcnt]));
        }
    }

    return NULL;
}

int32_t MqttClientCtxCreate(const MQTTClient_Params *ctxCfg)
{
    MQTTClient_Ctx_t *clientCtxPtr;
    MQTTClientCore_CtxCfg_t libCtxCfg;
    MQTTClientCore_CtxCBs_t libCliCBs;
    MQTT_SecureConn_t libNwSecurity;
    int32_t retval;

    /* Get a client context storage area */
    clientCtxPtr = getAvailableClictxMem();
    if (clientCtxPtr == NULL)
    {
        return -1;
    }

    /* Create the sync object to signal on arrival of ACK packet */
    clientCtxPtr->ackSyncObj = (sem_t *)malloc(sizeof(sem_t));

    /* Check if the creation of the semaphore succeeded */
    if (NULL == clientCtxPtr->ackSyncObj)
    {
        /* Malloc failed, return error code */
        return -1;
    }

    retval = sem_init(clientCtxPtr->ackSyncObj, 0, 0);
    if (0 != retval)
    {
        clientCtxPtr->inUse = false;
        return -1;
    }
    sem_trywait(clientCtxPtr->ackSyncObj);

    /* Initialize the ACK awaited */
    clientCtxPtr->awaitedAck = MQTT_DISCONNECT;

    /* Initialize the client lib */
    libCtxCfg.configOpts = ctxCfg->mqttMode31;

    if (true == MQTTClient_multiSrvConn)
    {
        libCtxCfg.configOpts |= (MQTTCLIENTCORE_CFG_APP_HAS_RTSK | MQTTCLIENTCORE_CFG_MK_GROUP_CTX);
    }
    else
    {
        libCtxCfg.configOpts |= MQTTCLIENTCORE_CFG_APP_HAS_RTSK;
    }

    /* get the network connection options */
    clientCtxPtr->blockingSend = ctxCfg->blockingSend;
    libCtxCfg.nwconnOpts = ctxCfg->connParams->netconnFlags;
    libCtxCfg.serverAddr = (char *)ctxCfg->connParams->serverAddr;
    libCtxCfg.portNumber = ctxCfg->connParams->port;

    /* Initialize secure socket parameters */
    /* Check if the secure connection flag in on      */
    if ((libCtxCfg.nwconnOpts & MQTT_DEV_NETCONN_OPT_SEC) != 0)
    {
        /* Check if the UDP socket flag in off        */
        if ((libCtxCfg.nwconnOpts & MQTT_DEV_NETCONN_OPT_UDP) == 0)
        {
            /* Check if the secure connection
               parameters are set */
            if (ctxCfg->connParams->cipher && ctxCfg->connParams->nFiles)
            {
                libCtxCfg.nwconnOpts |= MQTT_DEV_NETCONN_OPT_TCP;
                libCtxCfg.nwSecurity = &libNwSecurity;
                /* initialize secure socket parameters */
                libCtxCfg.nwSecurity->cipher = (void *)&(ctxCfg->connParams->cipher);
                libCtxCfg.nwSecurity->method = (void *)&(ctxCfg->connParams->method);
                libCtxCfg.nwSecurity->nFile  = ctxCfg->connParams->nFiles;
                libCtxCfg.nwSecurity->files  = (char **)ctxCfg->connParams->secureFiles;
            }
            else
            {
                /* Not all the secure connection parameters are set
                   Secure connection flag is on, missing security parameters */
                return -1;
            }
        }
        else
        {
            /* Check if the UDP socket and the secure connection flags are on.
               This state isn't allowed
               Secure connection is on, UDP Socket is not allowed */
            return -1;
        }
    }
    else
    {
        /* Secure connection flag in off */
        /* If the secure connection parameters are set when Secure connection
           flag in off, discard the security parameters */
        /* Check if UDP socket flag is off            */
        if ((libCtxCfg.nwconnOpts & MQTT_DEV_NETCONN_OPT_UDP) == 0)
        {
            libCtxCfg.nwconnOpts |= MQTT_DEV_NETCONN_OPT_TCP;
        }
        libCtxCfg.nwSecurity = NULL;
    }

    libCliCBs.publishRx = processPublishRxCB;
    libCliCBs.ackNotify = processNotifyAckCB;
    libCliCBs.disconnCB = processDisconnCB;

    retval = MQTTClientCore_createCtx(&libCtxCfg, &libCliCBs, clientCtxPtr, &clientCtxPtr->cliHndl);

    if (retval < 0)
    {
        clientCtxPtr->inUse = false;
        return -1;
    }

    MQTTClient_clientContext = clientCtxPtr;
    return 0;
}

//*****************************************************************************
//
//! \brief  Deletes the specified client context.
//!
//! \param[in] cli_ctx refers to client context to be deleted
//! \return Success (0) or Failure (< 0)
//
//*****************************************************************************
int32_t MqttClientCtxDelete(void)
{
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;
    int32_t retval;

    retval = MQTTClientCore_deleteCtx(clientCtx->cliHndl);
    if ((retval >= 0) || (1 == MQTTClient_closeContext))
    {
        /* Delete the semaphore */
        sem_destroy(clientCtx->ackSyncObj);
        free(clientCtx->ackSyncObj);
        clientCtx->ackSyncObj = NULL;
        /* Free up the context */
        memset(clientCtx, 0, sizeof(MQTTClient_Ctx_t));
    }

    return ((retval < 0) ? -1 : 0);
}

//*****************************************************************************
//
//! \brief  SUBSCRIBE a set of topics.
//! To receive data about a set of topics from the server, the app through
//! this routine must subscribe to those topic names with the server. The
//! caller can indicate whether the routine should block until a time, the
//! message has been acknowledged by the server.
//!
//! In case, the app has chosen not to await for the ACK from the server,
//! the SL MQTT implementation will notify the app about the subscription
//! through the callback routine.
//!
//! \param[in] cli_ctx refers to the handle to the client context
//! \param[in] topics set of topic names to subscribe. It is an array of
//! pointers to NUL terminated strings.
//! \param[in,out] qos array of qos values for each topic in the same order
//! of the topic array. If configured to await for SUB-ACK from server, the
//! array will contain qos responses for topics from the server.
//! \param[in] count number of such topics
//!
//! \return Success(transaction Message ID) or Failure(< 0)
//
//*****************************************************************************
int32_t MqttClientSub(MQTTClient_SubscribeParams *pValue, int32_t count)
{
    int32_t ret = MQTT_PACKET_ERR_FNPARAM, i;
    char qosLevel[MQTTCLIENT_MAX_SIMULTANEOUS_SUB_TOPICS];
    MQTT_UTF8StrQOS_t qosTopics[MQTTCLIENT_MAX_SIMULTANEOUS_SUB_TOPICS];
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;

    if(count > MQTTCLIENT_MAX_SIMULTANEOUS_SUB_TOPICS)
    {
        return ret; /* client not connected*/
    }

    for(i = 0; i < count; i++)
    {
        qosTopics[i].buffer = ( const char *)((pValue[i]).topic);
        qosTopics[i].qosreq = MQTTClient_qos[(pValue[i]).qos];
        qosLevel[i] = qosTopics[i].qosreq;
        qosTopics[i].length = strlen(qosTopics[i].buffer);
    }

    ret = MQTT_PACKET_ERR_NOTCONN;

    MQTTClient_mutexLock();
    if(_sl_ExtLib_AwaitedAckSet(clientCtx, MQTT_SUBACK) == false)
    {
        MQTTClient_mutexUnlock();
        return ret;
    }

    clientCtx->subackQos = qosLevel;
    MQTTClient_mutexUnlock();

    /* Send the subscription MQTT message */
    ret = MQTTClientCore_sendSubMsg(clientCtx->cliHndl, qosTopics, count);
    if(ret < 0)
    {
        SetCondAwaitedAckLocked(clientCtx, MQTT_SUBACK, 0);
        return ret;
    }

    ret = _sl_ExtLib_ServerAckLockedWait(clientCtx);

    return ((ret >= 0)? 0: ret);
}

//*****************************************************************************
//
//! \brief UNSUBSCRIBE a set of topics.
//! The app should use this service to stop receiving data for the named
//! topics from the server. The caller can indicate whether the routine
//! should block until a time, the message has been acknowledged by the
//! server.
//!
//! In case, the app has chosen not to await for the ACK from the server,
//! the SL MQTT implementation will notify the app about the subscription
//! through the callback routine.
//!
//! \param[in] cli_ctx refers to the handle to the client context
//! \param[in] topics set of topics to be unsubscribed. It is an array of
//! pointers to NUL terminated strings.
//! \param[in] count number of topics to be unsubscribed
//!
//! \return Success(transaction Message ID) or Failure(< 0)
//
//*****************************************************************************
int32_t MqttClientUnsub(MQTTClient_UnsubscribeParams *pValue, int32_t count)
{
    int32_t ret = MQTT_PACKET_ERR_FNPARAM, i;
    MQTT_UTF8String_t unsubTopics[MQTTCLIENT_MAX_SIMULTANEOUS_UNSUB_TOPICS];
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;

    if (count > MQTTCLIENT_MAX_SIMULTANEOUS_UNSUB_TOPICS)
    {
        return ret;
    }

    for (i = 0; i < count; i++)
    {
        MQTTClient_str2UTFConv(unsubTopics[i], (pValue[i]).topic);
    }

    ret = MQTT_PACKET_ERR_NOTCONN;
    if (_sl_ExtLib_AwaitedAckLockedSet(clientCtx, MQTT_UNSUBACK) == false)
    {
        return ret;
    }
    /* Send the unsubscription MQTT message */
    ret = MQTTClientCore_sendUnsubMsg(clientCtx->cliHndl, unsubTopics, count);
    if (ret < 0)
    {
        SetCondAwaitedAckLocked(clientCtx, MQTT_UNSUBACK, 0);
        return ret;
    }

    ret = _sl_ExtLib_ServerAckLockedWait(clientCtx);

    return ((ret >= 0) ? 0 : ret);
}

int32_t MqttClientConfig(const MQTTClient_LibCfg_t *cfg)
{
    MQTTClientCore_LibCfg_t libCfg;
    int32_t retval;

    /* Setup the MQTT client lib configurations */
    libCfg.loopbackPort = cfg->loopbackPort;

    if (cfg->loopbackPort)
    {
        MQTTClient_multiSrvConn = true;
        libCfg.grpUsesCbfn = true; /* Does use the group callback func */
    }
    else
    {
        MQTTClient_multiSrvConn = false;
        libCfg.grpUsesCbfn = false; /* Doesn't use the group callback func */

        /* Create the sync object between Rx and App tasks */
        MQTTClient_Lib_CB.MQTTClient_syncRxTx = (sem_t *)malloc(sizeof(sem_t));

        /* Check if the creation of the semaphore succeeded */
        if (NULL == MQTTClient_Lib_CB.MQTTClient_syncRxTx)
        {
            /* Malloc failed, return error code */
            return -1;
        }

        retval = sem_init(MQTTClient_Lib_CB.MQTTClient_syncRxTx, 0, 0);
        /* If semaphore couldn't be created return error   */
        if (0 != retval)
        {
            /* could not create SyncRxTx semaphore */
            return -1;
        }

        sem_trywait(MQTTClient_Lib_CB.MQTTClient_syncRxTx);
    }

    MQTTClient_waitSecs = cfg->respTime;
    /* Setup the mutex operations */
    MQTTClient_Lib_CB.MQTTClient_libLock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    /* Check if the creation of the mutex succeeded */
    if (NULL == MQTTClient_Lib_CB.MQTTClient_libLock)
    {
        /* Malloc failed, return error code */
        return -1;
    }

    retval = pthread_mutex_init(MQTTClient_Lib_CB.MQTTClient_libLock, (const pthread_mutexattr_t *)NULL);
    if (0 != retval)
    {
        /* could not create Lib Lock mutex */
        return -1;
    }
    /* setup the mutex operations */
    MQTTClient_Lib_CB.MQTTClient_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    /* Check if the creation of the mutex succeeded */
    if (NULL == MQTTClient_Lib_CB.MQTTClient_mutex)
    {
        /* Malloc failed, return error code */
        return -1;
    }

    retval = pthread_mutex_init(MQTTClient_Lib_CB.MQTTClient_mutex, (const pthread_mutexattr_t *)NULL);
    if (0 != retval)
    {
        /* could not create Multi-task Lock mutex */
        return -1;
    }
    libCfg.mutex = MQTTClient_Lib_CB.MQTTClient_libLock;
    libCfg.mutexLockin = mutexLockup;
    libCfg.mutexUnlock = mutexUnlock;
    /* Initialize client library */
    if (MQTTClientCore_initLib(&libCfg) < 0)
    {
        return -1;
    }

    return 0;
}

MQTTClient_Handle MQTTClient_create(MQTTClient_CallBack defaultCallback, MQTTClient_Params *params)
{
    MQTTClient_Handle  handle = (MQTTClient_Handle)1;
    int32_t retval;
    MQTTClient_closeContext = 0;

    retval = MqttClientConfig(&MQTTClient_clientCfg);
    if (0 == retval)
    {
        /* Initialize the control variables */
        memset(MQTTClient_ctx, 0, sizeof(MQTTClient_ctx));

        /* provide MQTT Lib information to create a pool of MQTT constructs. */
        retval = MQTTClientCore_registerBuffers(MQTTCLIENT_MAX_MQP, MQTTClient_packet, MQTTCLIENT_BUF_LEN, &MQTTClient_buffer[0][0]);
        if (0 == retval)
        {
            /* Register network services */
            retval = MQTTClientCore_registerNetSvc(&MQTTClient_net);
            if (0 == retval)
            {
                MQTTClient_mutexLock();
                if (NULL != MQTTClient_Lib_CB.MQTTClient_waitForMqttClose)
                {
                    sem_destroy(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
                    free(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
                    MQTTClient_Lib_CB.MQTTClient_waitForMqttClose = NULL;
                }
                MQTTClient_mutexUnlock();

                /* sync object for inter thread communication */
                MQTTClient_Lib_CB.MQTTClient_waitForMqttClose = (sem_t *)malloc(sizeof(sem_t));

                /* Check if the creation of the semaphore succeeded */
                if (NULL == MQTTClient_Lib_CB.MQTTClient_waitForMqttClose)
                {
                    /* Malloc failed, return error code */
                    return NULL;
                }

                retval = sem_init(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose, 0, 1);
            }
        }
    }
    if (0 != retval)
    {
        return NULL;
    }

    /* Set Client ID */
    if(MqttClientCtxCreate(params) )
    {
        return NULL;

    }
    MQTTClient_clientContext->clientID = params->clientId;
    MQTTClient_clientContext->appCBs = defaultCallback;
    return (MQTTClient_Handle)(handle);
}

int16_t MQTTClient_delete(MQTTClient_Handle handle)
{
    int32_t retval;
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;

    /* When closeContext equal to 1, the Receive Task (MQTTClient_run
       function) will post MQTTClient_waitForMqttClose and close */
    MQTTClient_closeContext = 1;

    // Disconnect from Broker
    if (false != clientCtx->connectedState)
    {
        MQTTClient_disconnect(handle);
    }

    //
    // deleting the context for the connection
    //
    MqttClientCtxDelete();

    MQTTClient_mutexLock();
    if (NULL != MQTTClient_Lib_CB.MQTTClient_waitForMqttClose)
    {
        MQTTClient_rxTxSemPost();
        sem_wait(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
        sem_destroy(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
        free(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
        MQTTClient_Lib_CB.MQTTClient_waitForMqttClose = NULL;
    }
    MQTTClient_mutexUnlock();

    /* Deinitialize the MQTT client lib */
    retval = MQTTClientCore_exitLib();
    if ((retval >= 0) || (1 == MQTTClient_closeContext))
    {
        if (MQTTClient_Lib_CB.MQTTClient_libLock != NULL)
        {
            /* Delete the MQTT lib lock object */
            pthread_mutex_destroy(MQTTClient_Lib_CB.MQTTClient_libLock);
            free(MQTTClient_Lib_CB.MQTTClient_libLock);
            MQTTClient_Lib_CB.MQTTClient_libLock = NULL;
        }

        if (MQTTClient_Lib_CB.MQTTClient_mutex != NULL)
        {
            /* Delete the MT lock object */
            pthread_mutex_destroy(MQTTClient_Lib_CB.MQTTClient_mutex);
            free(MQTTClient_Lib_CB.MQTTClient_mutex);
            MQTTClient_Lib_CB.MQTTClient_mutex = NULL;
        }

        /* Delete the Rx-Tx task sync object */
        sem_destroy(MQTTClient_Lib_CB.MQTTClient_syncRxTx);
        free(MQTTClient_Lib_CB.MQTTClient_syncRxTx);
        MQTTClient_Lib_CB.MQTTClient_syncRxTx = NULL;
    }

    return ((retval < 0) ? -1 : 0);
}

//*****************************************************************************
//
//! \brief Receive Task. Invokes in the context of a task
//
//*****************************************************************************
int16_t MQTTClient_run(MQTTClient_Handle handle)
{
    int32_t retval;

    /* The semaphore created with value 1, the following line will clear it */
    sem_wait(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);

    do
    {
        if (false == MQTTClient_multiSrvConn)
        {
            /* wait on broker connection */
            MQTTClient_rxTxSemWait();
            while (1)
            {
                /* Forcing to use the index 0 always - caution */
                retval = MQTTClientCore_ctxRun(MQTTClient_ctx[0].cliHndl, MQTTClient_waitSecs);

                if (((retval < 0) && (retval != MQTT_PACKET_ERR_TIMEOUT)) || (1 == MQTTClient_closeContext))
                {
                    sem_post(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
                    return 0;
                }
            }
        }
        else
        {
            retval = MQTTClientCore_run(MQTTClient_waitSecs);
            if (((retval < 0) && (retval != MQTT_PACKET_ERR_TIMEOUT)) || (1 == MQTTClient_closeContext))
            {
                sem_post(MQTTClient_Lib_CB.MQTTClient_waitForMqttClose);
                break;
            }
        }
    } while (1);

    return 0;
}

int16_t MQTTClient_connect(MQTTClient_Handle handle)
{
    int32_t ret = MQTT_PACKET_ERR_NOTCONN;
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;

    /*utf8 strings into which client info will be stored*/
    MQTT_UTF8String_t clientID, username, usrPwd, will_topic, willMsg;
    MQTT_UTF8String_t *usrname = NULL, *usrpasswd = NULL, *clientid = NULL;

    MQTTClient_mutexLock();

    clientCtx->connectedState = false;

    if (MQTT_DISCONNECT != clientCtx->awaitedAck)
    {
        clientCtx->connAck = 0;
        MQTTClient_mutexUnlock();
        return ret;
    }
    /* Provide Client ID,user name and password into MQTT Library */
    if (clientCtx->clientID != NULL)
    {
        MQTTClient_str2UTFConv(clientID, clientCtx->clientID);
        clientid = &clientID;
    }

    if (clientCtx->usrName != NULL)
    {
        MQTTClient_str2UTFConv(username, clientCtx->usrName);
        usrname = &username;
    }
    if (clientCtx->usrPwd != NULL)
    {
        MQTTClient_str2UTFConv(usrPwd, clientCtx->usrPwd);
        usrpasswd = &usrPwd;
    }

    ret = MQTTClientCore_registerCtxInfo(clientCtx->cliHndl, clientid, usrname, usrpasswd);
    if (ret < 0)
    {
        clientCtx->connAck = 0;
        MQTTClient_mutexUnlock();
        return ret;
    }

    /* Register a will message, if specified, into MQTT Library */
    if (NULL != clientCtx->mqttWill.willTopic)
    {
        MQTTClient_str2UTFConv(will_topic, clientCtx->mqttWill.willTopic);
        MQTTClient_str2UTFConv(willMsg, clientCtx->mqttWill.willMsg);
        ret = MQTTClientCore_registerWillInfo(clientCtx->cliHndl, &will_topic, &willMsg, MQTTClient_qos[clientCtx->mqttWill.willQos], clientCtx->mqttWill.retain);
        if (ret < 0)
        {
            clientCtx->connAck = 0;
            MQTTClient_mutexUnlock();
            return ret;
        }
    }

    clientCtx->connAck = 0;
    clientCtx->awaitedAck = MQTT_CONNACK;
    MQTTClient_mutexUnlock();

    /* Connect to the server */
    ret = MQTTClientCore_sendConnectMsg(clientCtx->cliHndl, MQTTClient_connectCfg.clean, MQTTClient_connectCfg.keepAliveTimeout);

    MQTTClient_mutexLock();
    if (false == MQTTClient_multiSrvConn)
    {
        /* Unblock the receive task here */
        MQTTClient_rxTxSemPost();
    }
    MQTTClient_mutexUnlock();

    /*Network or Socket Error happens*/
    if (ret < 0)
    {
        SetCondAwaitedAckLocked(clientCtx, MQTT_CONNACK, MQTT_DISCONNECT);

        return ret;
    }


    /* Wait for a CONNACK here */
    MQTTClient_ackRxSignalWait(clientCtx->ackSyncObj);

    MQTTClient_mutexLock();
    if (MQTT_DISCONNECT != clientCtx->awaitedAck)
    {
        /* Check the return code of connAck (LSB) */
        if( 0 == (clientCtx->connAck & 0xff) )
        {
            ret = clientCtx->connAck;
            clientCtx->connectedState = true;
        }
        else
        {
            /* Error received, Connection failed */
            ret = 0 - clientCtx->connAck;
            clientCtx->connectedState = false;
        }
    }
    MQTTClient_mutexUnlock();

    return ret;
}

int16_t MQTTClient_disconnect(MQTTClient_Handle handle)
{
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;
    int32_t retval = MQTT_PACKET_ERR_NOTCONN;

    clientCtx->connectedState = false;

    if (_sl_ExtLib_AwaitedAckLockedSet(clientCtx,MQTTCLIENT_DISCONNECT_CB) == true)
    {
        /* Already disconnected - quit */

        /* send the disconnect command. */
        retval = MQTTClientCore_sendDisconn(clientCtx->cliHndl);
        if (retval >= 0)
        {
            MQTTClient_ackRxSignalWait(clientCtx->ackSyncObj);
        }
    }
    return ((retval >= 0) ? 0 : retval);
}

uint16_t MqttClient_send(MQTTClient_Handle hdl, void *pMeta, uint16_t MetaLen, void *pData, uint16_t DataLen, uint32_t flags)
{
    int32_t ret = MQTT_PACKET_ERR_FNPARAM;
    MQTT_UTF8String_t topicUTF8;
    MQTTClient_Ctx_t *clientCtx = (MQTTClient_Ctx_t *)MQTTClient_clientContext;
    uint8_t qosLevel = (flags & (MQTT_QOS_0 | MQTT_QOS_1 | MQTT_QOS_2));
    bool retain =  (flags & MQTT_PUBLISH_RETAIN);
    uint8_t awaitedAcks[] = { 0 /* connected */, MQTT_PUBACK, MQTT_PUBCOMP };
    uint8_t awaitedAck;

    MQTTClient_str2UTFConv(topicUTF8, pMeta);

    awaitedAck = awaitedAcks[qosLevel];
    ret = MQTT_PACKET_ERR_NOTCONN;

    if (_sl_ExtLib_AwaitedAckLockedSet(clientCtx, awaitedAck) == false)
    {
        return ret;
    }
    /*publish the message*/
    ret = MQTTClientCore_sendPubMsg(clientCtx->cliHndl, &topicUTF8, pData, DataLen, MQTTClient_qos[qosLevel], retain);
    if (ret < 0)
    {
        SetCondAwaitedAckLocked(clientCtx, awaitedAck, 0);
        return ret;
    }

    if (qosLevel)
    {
        ret = _sl_ExtLib_ServerAckLockedWait(clientCtx);
    }
    return ((ret >= 0) ? 0 : ret);
}

int16_t MQTTClient_publish(MQTTClient_Handle handle, char *topic, uint16_t topicLen, char *msg, uint16_t msgLen, uint32_t flags)
{
    return (MqttClient_send(handle, (void *)topic, topicLen, (void *)msg, msgLen, flags));
}

int16_t MQTTClient_subscribe(MQTTClient_Handle handle, MQTTClient_SubscribeParams *value, uint8_t numberOfTopics)
{
    return (MqttClientSub((MQTTClient_SubscribeParams *)value, (int32_t)numberOfTopics));

}

int16_t MQTTClient_unsubscribe(MQTTClient_Handle handle, MQTTClient_UnsubscribeParams *value, uint8_t numberOfTopics)
{
    return (MqttClientUnsub((MQTTClient_UnsubscribeParams *)value, numberOfTopics));
}

int16_t MQTTClient_get(MQTTClient_Handle handle, uint16_t option, void *value, uint16_t valueLength)
{
    return  0;
}

int16_t MQTTClient_set(MQTTClient_Handle handle, uint16_t option, void *value, uint16_t valueLength)
{
    int32_t   retVal = 0;
    uint16_t *keep_alive;
    bool     *clean;

    switch (option)
    {
        case MQTTClient_USER_NAME:
            /* Save the reference to the Username */
            MQTTClient_clientContext->usrName = (char *)value;
            break;

        case MQTTClient_PASSWORD:
            /* Save the reference to the password */
            MQTTClient_clientContext->usrPwd = (char *)value;
            break;

        case MQTTClient_WILL_PARAM:
            /* Save the reference to will parameters*/
            MQTTClient_clientContext->mqttWill = *((MQTTClient_Will *)value);
            break;

        case MQTTClient_KEEPALIVE_TIME:
            keep_alive = (uint16_t *)value;
            MQTTClient_connectCfg.keepAliveTimeout = *keep_alive;
            break;
        case MQTTClient_CLEAN_CONNECT:
            clean = (bool *)value;
            MQTTClient_connectCfg.clean = *clean;
            break;
        default:
            break;
    }
    return retVal;
}
