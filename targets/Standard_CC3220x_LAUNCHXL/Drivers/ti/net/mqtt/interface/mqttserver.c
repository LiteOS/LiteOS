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

#include <ti/net/mqtt/common/mqtt_common.h>
#include <ti/net/mqtt/mqttserver.h>
#include <ti/net/mqtt/server/server_core.h>
#include <ti/net/mqtt/server/client_mgmt.h>
#include <ti/net/mqtt/server/server_util.h>
#include <ti/net/mqtt/platform/mqtt_net_func.h>

/* POSIX Header files */
#include <pthread.h>

#define MQTTSERVER_LOOPBACK_PORT    (1882)

// Specifying Receive time out for the Receive task
#define MQTTSERVER_RCV_TIMEOUT      (30)

typedef struct _MQTTServer_Ctrl_t_
{
    const void *pluginHndl;
}MQTTServer_Ctrl_t;

extern int32_t MQTTServerPkts_recvHvec[];
extern uint32_t MQTTServerPkts_closeInProgress;

pthread_mutex_t *MQTTServer_mutex;
MQTTServer_Ctrl_t MQTTServer_appHndl;

MQTTServerCore_AppCfg_t MQTTServer_appConfig = { NULL };

/* Global that holds parameters on the server for */
/* Authentication of Clients by the Server        */
MQTTServer_LocalAuthenticationData MQTTServer_localAuthData;

/*Task Priority and Response Time*/
uint32_t MQTTServer_waitSecs = MQTTSERVER_RCV_TIMEOUT;

/* Network Services functions*/
MQTT_DeviceNetServices_t MQTTServer_netOps =
{
    MQTTNet_commOpen, MQTTNet_tcpSend, MQTTNet_tcpRecv, MQTTNet_sendTo,
    MQTTNet_recvFrom, MQTTNet_commClose, MQTTNet_tcpListen,
    MQTTNet_tcpAccept, MQTTNet_tcpSelect, MQTTNet_rtcSecs
};

MQTTServer_CallBack MQTTServer_cbs = NULL;

static uint16_t MQTTServerConnectCB(const MQTT_UTF8String_t *clientId,
                            const MQTT_UTF8String_t *username,
                            const MQTT_UTF8String_t *password, void **appUsr);

static void MQTTServerPublishCB(const MQTT_UTF8String_t *topic,
                        const uint8_t *payload, uint32_t payLen,
                        bool dup, uint8_t qos, bool retain);

static void MQTTServerDisconnCB(const void *appUsr, bool due2err);

static void MQTTServerMutexLock(pthread_mutex_t *mutexHndl);
static void MQTTServerMutexUnlock(pthread_mutex_t *mutexHndl);

MQTTServerCore_AppCBs_t MQTTServer_appCBs =
{
    MQTTServerConnectCB,
    MQTTServerPublishCB,
    MQTTServerDisconnCB
};

MQTTServerPkts_LibCfg_t MQTTServer_cfg =
{
    0,
    MQTTSERVER_LOOPBACK_PORT,
    &MQTTServer_mutex,
    MQTTServerMutexLock,
    MQTTServerMutexUnlock
};

static void MQTTServerMutexLock(pthread_mutex_t *mutexHndl)
{
    pthread_mutex_lock(mutexHndl);
}

static void MQTTServerMutexUnlock(pthread_mutex_t *mutexHndl)
{
    pthread_mutex_unlock(mutexHndl);
}

static uint16_t MQTTServerConnectCB(const MQTT_UTF8String_t *clientId,
                            const MQTT_UTF8String_t *username,
                            const MQTT_UTF8String_t *password, void **appUsr)
{
    MQTTServer_ConnectMetaDataCB meta;

    meta.clientId = (char *)MQ_CONN_UTF8_BUF(clientId);
    meta.clientIdLen = MQ_CONN_UTF8_LEN(clientId);
    meta.userName = (char *)MQ_CONN_UTF8_BUF(username);
    meta.usernameLen = MQ_CONN_UTF8_LEN(username);
    meta.password = (char *)MQ_CONN_UTF8_BUF(password);
    meta.passwordLen = MQ_CONN_UTF8_LEN(password);
    meta.usr = appUsr;

    /* When the server set user name, that means that local               */
    /* authentication, Authentication of Clients by the Server, is        */
    /* required                                                           */
    if (strlen(MQTTServer_localAuthData.userName) > 0)
    {
        /* Comparing the server user name with the user name that the     */
        /* client sent in the connection request                                    */
        if ((username != NULL) && strcmp(MQTTServer_localAuthData.userName, meta.userName) == 0)
        {
            /* Comparing the server password with the password that the   */
            /* client sent in the connection request                      */
            if ((password != NULL) && strcmp(MQTTServer_localAuthData.password, meta.password) == 0)
            {
                MQTTServer_cbs(MQTTServer_CONNECT_CB_EVENT, &meta, sizeof(meta), NULL, 0 );
                return 0;
            }
        }
    }
    else
    {
        /* No local authentication is required                            */
        MQTTServer_cbs(MQTTServer_CONNECT_CB_EVENT, &meta, sizeof(meta), NULL, 0 );
        return 0;
    }

    /* Local authentication is required and the user name or password doesn't */
    /* not match                                                              */
    MQTTServer_cbs(MQTTServer_CONNECT_CB_EVENT, &meta, sizeof(meta), (void *)MQTT_CONNACK_RC_BAD_USRPWD, sizeof(int) );
    return MQTT_CONNACK_RC_BAD_USRPWD;
}

static void MQTTServerPublishCB(const MQTT_UTF8String_t *topic,
                        const uint8_t *payload, uint32_t payLen,
                        bool dup, uint8_t qos, bool retain)
{
    MQTTServer_RecvMetaDataCB meta;

    meta.topic  = (char *)MQ_CONN_UTF8_BUF(topic);
    meta.topLen = MQ_CONN_UTF8_LEN(topic);
    meta.qos    = qos;
    meta.dup    = dup;
    meta.retain = retain;
    MQTTServer_cbs(MQTTServer_RECV_CB_EVENT, &meta, sizeof(MQTTServer_RecvMetaDataCB), (char *)payload, payLen );
}

static void MQTTServerDisconnCB(const void *appUsr, bool due2err)
{
    MQTTServer_DisconnectMetaDataCB meta;

    meta.dueToErr = due2err;
    meta.user = (char *)appUsr;
    MQTTServer_cbs(MQTTServer_DISCONNECT_CB_EVENT, &meta, sizeof(MQTTServer_DisconnectMetaDataCB), NULL, 0 );
}

uint16_t MQTTServerSend(MQTTServer_Handle hdl, void *meta, uint16_t metaLen, void *data, uint16_t dataLen, uint32_t flags)
{
    char qos_level = (flags & (MQTT_QOS_1 | MQTT_QOS_1 | MQTT_QOS_2));
    bool retain =  (flags & MQTT_PUBLISH_RETAIN);
    MQTT_UTF8String_t topicUTF8 = { NULL };

    topicUTF8.buffer = (char *)meta;
    topicUTF8.length = strlen(meta);

    return (MQTTServerCore_pubSend(&topicUTF8, data, dataLen, (MQTT_QOS) qos_level, retain));
}

int32_t MQTTServerTopicEnroll(const char *topic)
{
    MQTT_UTF8String_t topicUTF8 = { NULL };

    topicUTF8.buffer = (char *)topic;
    topicUTF8.length = strlen(topic);

    return (MQTTServerCore_topicEnroll((void *)(MQTTServer_appHndl.pluginHndl), &topicUTF8, MQTT_QOS2));

}

int32_t MQTTServerTopicDisenroll(const char *topic)
{
    MQTT_UTF8String_t topicUTF8 = { NULL };

    topicUTF8.buffer = (char *)topic;
    topicUTF8.length = strlen(topic);

    return (MQTTServerCore_topicDisenroll((void *)&(MQTTServer_appHndl.pluginHndl), &topicUTF8));
}


MQTTServer_Handle MQTTServer_create(MQTTServer_CallBack defaultCallback,  MQTTServer_Params *attrib)
{
    MQTTServer_Handle  handle = (MQTTServer_Handle)1;
    int32_t ret;

    MQTTServerPkts_closeInProgress = 0;

    MQTTServer_cbs = defaultCallback;
    MQTTServer_cfg.listenerPort =  attrib->connParams->port;
    MQTTServer_cfg.secure.method =  &(attrib->connParams->method);
    MQTTServer_cfg.secure.cipher = &(attrib->connParams->cipher);
    MQTTServer_cfg.secure.nFile = attrib->connParams->nFiles;
    MQTTServer_cfg.secure.files = (char **) (attrib->connParams->secureFiles);

    /* Initialize UserName and password to NULL */
    MQTTServer_localAuthData.userName = NULL;
    MQTTServer_localAuthData.password = NULL;

    MQTTServer_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    /* Check if the creation of the mutex succeeded */
    if (NULL == MQTTServer_mutex)
    {
        /* Malloc failed, return error code */
        return NULL;
    }

    ret = pthread_mutex_init(MQTTServer_mutex, (const pthread_mutexattr_t *)NULL);
    if (0 == ret)
    {
        ret = MQTTServerCore_init(&MQTTServer_cfg, &MQTTServer_appConfig);
        if(0 == ret)
        {
            /* registering the device specific implementations for net operations */
            ret = MQTTServerPkts_registerNetSvc(&MQTTServer_netOps);
            if(0 == ret)
            {
                /* registering the apps callbacks */
                MQTTServer_appHndl.pluginHndl = MQTTServerCore_appRegister(&MQTTServer_appCBs, "temp");
                if (MQTTServer_appHndl.pluginHndl == NULL)
                {
                    return NULL;
                }
            }
        }
    }

    if(0 == ret)
    {
        return handle;
    }
    else
    {
        return NULL;
    }
}

int16_t MQTTServer_delete(MQTTServer_Handle handle)
{
    int32_t rdIdx = 0;
    int32_t fd;
    int32_t retCode;

    MQTTServerPkts_closeInProgress = 1;
    fd = MQTTServerPkts_recvHvec[rdIdx];
    while (-1 != fd)
    {
        MQTTServer_netOps.close(fd);
        fd = MQTTServerPkts_recvHvec[++rdIdx];
    }

    retCode = pthread_mutex_destroy(MQTTServer_mutex);
    free(MQTTServer_mutex);
    MQTTServer_mutex = NULL;

    /* Initialize and free allocated memory for the clients */
    MQTTClientMgmt_init();

    /* Delete and free the topic node tree */
    MQTTServerCore_topicNodeExit();

    return retCode;
}

int16_t MQTTServer_run(MQTTServer_Handle handle)
{
    MQTTServerPkts_run(MQTTServer_waitSecs);
    return 0;
}

int16_t MQTTServer_subscribe(MQTTServer_Handle handle, MQTTServer_SubscribeParams *value, uint8_t numberOfTopics)
{
    int i;

    for( i = 0; i < numberOfTopics; i++ )
    {
        MQTTServerTopicEnroll((char *)value[i].topic);
    }
    return 0;
}

int16_t MQTTServer_unsubscribe(MQTTServer_Handle handle, MQTTServer_UnSubscribeParams *value, uint8_t numberOfTopics)
{
    int i;

    for( i = 0; i < numberOfTopics; i++ )
    {
        MQTTServerTopicDisenroll((char *)value[i].topic);
    }
    return 0;
}

int16_t MQTTServer_publish(MQTTServer_Handle handle, char *topic,  uint16_t topicLen, char *msg, uint16_t msgLen, uint32_t flags)
{
    return (MQTTServerSend(handle, topic, topicLen, msg, msgLen, flags));
}

//*****************************************************************************
//
//! \brief  Set User name and password on the server for Clients Authentication
//   Note: Only when setting the User name the authentication mechanism works
//
//*****************************************************************************
int16_t MQTTServer_set(MQTTServer_Handle handle, uint16_t option, void *value, uint16_t valueLength)
{
    int32_t retVal = 0;

    switch (option)
    {
        case MQTTServer_USER_NAME:
            /* Save the reference to the User name */
            MQTTServer_localAuthData.userName = (char *)value;
            break;

        case MQTTServer_PASSWORD:
            /* Save the reference to the password */
            MQTTServer_localAuthData.password = (char *)value;
            break;
        default:
            break;
    }
    return retVal;
}
