/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/mqtt/mqttclient.h>
#include <ti/net/utils/str_mpl.h>

/* AT Header files */
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd.h>
#include <ti/net/atcmd/atcmd_mqtt.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd_event.h>

#include <pthread.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDMQTT_CREATE_MAX_SECURE_FILES       (4)
#define ATCMDMQTT_CREATE_MAX_URL_SERVER_NAME    (430)
#define ATCMDMQTT_CREATE_MAX_CLIENT_ID_LEN      (64)
#define ATCMDMQTT_CLIENT_RX_TASK_SIZE           (4096)
#define ATCMDMQTT_PUBLISH_MAX_TOPIC_LEN         (0x7FFF)
#define ATCMDMQTT_SET_MAX_USERNAME_LEN          (0xFFFF)
#define ATCMDMQTT_SET_MAX_PASSWORD_LEN          (0xFFFF)
#define ATCMDMQTT_SUBSCRIBE_MAX_TOPICS          (4)
#define ATCMDMQTT_MAX_NUM_OF_CLIENTS            (1)


//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmdMqtt_Set_t_
{
    uint16_t option;
    void *value;
    char *usrName;
    char *usrPwd;
    uint16_t willMsgLen;
    MQTTClient_Will mqttWill;
    uint16_t keepAliveTimeout;
    uint8_t clean;
}ATCmdMqtt_Set_t;

typedef struct _ATCmdMqtt_CB_t_
{
    MQTTClient_Handle   clientIndex[ATCMDMQTT_MAX_NUM_OF_CLIENTS];
    MQTTClient_Params   attrib;
    ATCmdMqtt_Set_t     *set;
    uint8_t             dataFormat;
    uint8_t             delRequest;
} ATCmdMqtt_CB_t;

typedef struct _ATCmdMqtt_Publish_t_
{
    char        *topic;
    uint16_t    topicLen;
    uint32_t    flags;
    uint16_t    msgLen;
    char        *msg;
}ATCmdMqtt_Publish_t;

typedef struct _ATCmdMqtt_Subscribe_t_
{
    uint8_t numberOfTopics;
    MQTTClient_SubscribeParams *value;
}ATCmdMqtt_Subscribe_t;

typedef struct _ATCmdMqtt_Unsubscribe_t_
{
    uint8_t numberOfTopics;
    MQTTClient_UnsubscribeParams *value;
}ATCmdMqtt_Unsubscribe_t;

typedef struct _ATCmdMqtt_Event_t_
{
    int32_t id;
    uint32_t metaDateLen;
    void * metaData;
    uint32_t dataLen;
    void *data;
}ATCmdMqtt_Event_t;

//*****************************************************************************
// global variables
//*****************************************************************************
ATCmdMqtt_CB_t ATCmdMqtt_cb = {{NULL},{0},NULL,ATCMD_DATA_FORMAT_BINARY,0};

//*****************************************************************************
// MQTT AT Commands Routines
//*****************************************************************************
int32_t ATCmdMqtt_setFree(ATCmdMqtt_Set_t *params);

/* 
    Parse client index and get the corresponding handle
*/
MQTTClient_Handle ATCmdMqtt_clientIndexParse(char **buff, uint32_t *index, char delim)
{    
    /* Mode */
    if (StrMpl_getVal(buff, index ,delim, STRMPL_FLAG_PARAM_SIZE_32) < 0)
    {
        return NULL;
    }
    if (*index > ATCMDMQTT_MAX_NUM_OF_CLIENTS)
    {
        return NULL;
    }
    /* return client handle */
    return ATCmdMqtt_cb.clientIndex[*index];
}

/*
   MQTT event callback which compose and send the received event to the host 
*/
int32_t ATCmdMqtt_eventCallback(void *args, int32_t num, char *buff)
{
    ATCmdMqtt_Event_t *event = (ATCmdMqtt_Event_t *)(args);
    MQTTClient_RecvMetaDataCB *header;
    uint32_t outputLen = 0;

    StrMpl_setStr(ATCmd_eventMqttStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setListStr(ATCmd_mqttEventId, sizeof(ATCmd_mqttEventId)/sizeof(StrMpl_List_t), &event->id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_SIGNED);
    switch((MQTTClient_EventCB)event->id)
    {
        case MQTTClient_OPERATION_CB_EVENT:
        {
            StrMpl_setListStr(ATCmd_mqttEventOperationId, sizeof(ATCmd_mqttEventOperationId)/sizeof(StrMpl_List_t), &((MQTTClient_OperationMetaDataCB *)event->metaData)->messageType,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
            switch (((MQTTClient_OperationMetaDataCB *)event->metaData)->messageType)
            {
                case MQTTCLIENT_OPERATION_CONNACK:
                {
                    StrMpl_setVal(event->data,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
                    break;
                }

                case MQTTCLIENT_OPERATION_EVT_PUBACK:
                case MQTTCLIENT_OPERATION_UNSUBACK:
                {
                    *(uint8_t *)(event->data) = 0;
                    StrMpl_setVal(event->data,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
                    break;
                }

                case MQTTCLIENT_OPERATION_SUBACK:
                {
                    StrMpl_setArrayVal(event->data,&buff,event->dataLen,ATCMD_DELIM_TRM,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    break;
                }

                default:
                    break;
            }
            break;
        }
        case MQTTClient_RECV_CB_EVENT:
        {
            header = (MQTTClient_RecvMetaDataCB *)(event->metaData);
            /* topic */
            //topic should be string but it been send from mqtt client without '/0' so we can not read it as string 
            memcpy(buff,header->topic,header->topLen);
            buff += header->topLen;
            *buff = ATCMD_DELIM_ARG;
            buff++;
            /* qos */
            StrMpl_setListStr(ATCmd_mqttQos, sizeof(ATCmd_mqttQos)/sizeof(StrMpl_List_t), &header->qos,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            /* retain */
            StrMpl_setVal(&header->retain,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* duplicate */
            StrMpl_setVal(&header->dup,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* format */
            StrMpl_setVal(&ATCmdMqtt_cb.dataFormat, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            /* data length */
            outputLen = event->dataLen;
            if (ATCmdMqtt_cb.dataFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert length from binary to base64 */
                outputLen = StrMpl_getBase64EncBufSize(event->dataLen);
            }
            StrMpl_setVal(&outputLen,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_SIGNED);

            /* data */
            if (ATCmdMqtt_cb.dataFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert data to base64 */
                StrMpl_encodeBase64(event->data, event->dataLen, (uint8_t *)buff, &outputLen);
            }
            else
            {
                memcpy(buff,event->data,event->dataLen);
            }
            break;
        }
        case MQTTClient_DISCONNECT_CB_EVENT:
            break;
    }
    if (event->metaData != NULL)
    {
        free(event->metaData);
    }
    if (event->data != NULL)
    {
        free(event->data);
    }
    free(event);

    return 0;
}

/*!
    MQTT client callback which receive events from the broker
*/
void ATCmdMqtt_clientCallback(int32_t id, void *metaData, uint32_t metaDateLen, void *data, uint32_t dataLen)
{
    ATCmdMqtt_Event_t event;

    memset(&event,0,sizeof(ATCmdMqtt_Event_t));
    event.id = id;

    /* meta data */
    event.metaDateLen = metaDateLen;
    if (metaDateLen > 0)
    {
        event.metaData = malloc(metaDateLen);
        if (event.metaData == NULL)
        {
            ATCmd_errorResult(ATCmd_errorAllocStr,0);
            return;
        }
        memcpy(event.metaData,metaData,metaDateLen);
    }
    /* data */
    event.dataLen = dataLen;
    if (dataLen > 0)
    {
        event.data = malloc(dataLen);
        if (event.data == NULL)
        {
            ATCmd_errorResult(ATCmd_errorAllocStr,0);
            return;
        }
        memcpy(event.data,data,dataLen);
    }
    /* send the event */
    ATCmdEvent_compose(&event, sizeof(ATCmdMqtt_Event_t), ATCmdMqtt_eventCallback);
}


/*
    MQTT client thread
*/
void *ATCmdMqtt_clientThread(void * pvParameters)
{
    while (1)
    {
        MQTTClient_run((MQTTClient_Handle)pvParameters);
        if (ATCmdMqtt_cb.delRequest == 1)
        {
            ATCmdMqtt_cb.delRequest = 0;
            break;
        }
    }
    pthread_exit(0);

    return NULL;
}

/*
    Free allocated memory for MQTT client instant
*/
int32_t ATCmdMqtt_clientFree(void)
{    
    uint8_t i;

    if (ATCmdMqtt_cb.attrib.clientId != NULL)
    {
        free(ATCmdMqtt_cb.attrib.clientId);
    }
    if (ATCmdMqtt_cb.attrib.connParams != NULL)
    {
        if (ATCmdMqtt_cb.attrib.connParams->serverAddr != NULL)
        {
            free((void *)ATCmdMqtt_cb.attrib.connParams->serverAddr);    
        }
        if (ATCmdMqtt_cb.attrib.connParams->secureFiles != NULL)
        {
            for (i=0;i < ATCMDMQTT_CREATE_MAX_SECURE_FILES;i++)
            {
                if (ATCmdMqtt_cb.attrib.connParams->secureFiles[i] != NULL)
                {
                    free(ATCmdMqtt_cb.attrib.connParams->secureFiles[i]);    
                }
            }
            free((void *)ATCmdMqtt_cb.attrib.connParams->secureFiles);    
        }
        free(ATCmdMqtt_cb.attrib.connParams);
    }
    if (ATCmdMqtt_cb.set != NULL)
    {
        ATCmdMqtt_setFree(ATCmdMqtt_cb.set);
    }
    /* delete client index */
    ATCmdMqtt_cb.clientIndex[0] = NULL;

    return 0;
}

/*
    Compose and send MQTT create client result.
*/
int32_t ATCmdMqtt_createResult(void *args, int32_t num, char *buff)
{
    
    StrMpl_setStr(ATCmd_mqttCreateStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&num,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);

    return 0;
}

/*
    Parse MQTT client create command.
*/
int32_t ATCmdMqtt_createParse(char *buff, MQTTClient_Params *params)
{
    int32_t ret = 0;
    uint8_t secure = 0;

    /* client ID */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->clientId), ATCMD_DELIM_ARG, ATCMDMQTT_CREATE_MAX_CLIENT_ID_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* flags */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_mqttCreateFlags, sizeof(ATCmd_mqttCreateFlags)/sizeof(StrMpl_List_t),&buff,&params->connParams->netconnFlags,ATCMD_DELIM_ARG,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    /* server address (ip or url) */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->connParams->serverAddr), ATCMD_DELIM_ARG, ATCMDMQTT_CREATE_MAX_URL_SERVER_NAME ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }

    /* port */
    if ((ret = StrMpl_getVal(&buff, &params->connParams->port , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
    {
        return ret;
    }

    if (params->connParams->netconnFlags & MQTTCLIENT_NETCONN_SEC != 0)
    {
        secure = 1;
    }
    /* method */
    if ((ret = StrMpl_getListVal(ATCmd_sockSocketSecMethod, sizeof(ATCmd_sockSocketSecMethod)/sizeof(StrMpl_List_t), &buff, &params->connParams->method, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        if (!((ret == STRMPL_ERROR_PARAM_MISSING) && (secure == 0)))
        {
            return ret;
        }
    }
    /* cipher */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_sockSocketCipher, sizeof(ATCmd_sockSocketCipher)/sizeof(StrMpl_List_t), &buff, &params->connParams->cipher, ATCMD_DELIM_ARG,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        if (!((ret == STRMPL_ERROR_PARAM_MISSING) && (secure == 0)))
        {
            return ret;
        }
        /* set to the default value */
        params->connParams->cipher = SLNETSOCK_SEC_CIPHER_FULL_LIST;
    }
    /* set the number of files to fix value */
    params->connParams->nFiles = 4;

    /* Private Key File */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->connParams->secureFiles[0]), ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* Certificate File Name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->connParams->secureFiles[1]), ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* CA File Name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->connParams->secureFiles[2]), ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (!((ret == STRMPL_ERROR_PARAM_MISSING) && (secure == 0)))
        {
            return ret;
        }
    }

    /* DH Key File Name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->connParams->secureFiles[3]), ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* mqtt mode */
    if ((ret = StrMpl_getListVal(ATCmd_mqttCreateMode, sizeof(ATCmd_mqttCreateMode)/sizeof(StrMpl_List_t), &buff, &params->mqttMode31, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    /* blocking Send */
    if ((ret = StrMpl_getVal(&buff, &params->blockingSend , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    /* data format */
    if ((ret = StrMpl_getVal(&buff, &ATCmdMqtt_cb.dataFormat, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }

    return ret;
}


/*
    MQTT client create callback.
*/
int32_t ATCmdMqtt_createCallback(void *arg)
{
    int32_t ret = 0;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    pthread_t rxThread = (pthread_t) NULL;
    MQTTClient_Handle handle;
    uint8_t index = 0;

    if (ATCmdMqtt_cb.clientIndex[index] != NULL)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,-1);
        return -1;        
    }
    memset(&ATCmdMqtt_cb.attrib, 0x0, sizeof(MQTTClient_Params));

    ATCmdMqtt_cb.attrib.connParams = malloc(sizeof(MQTTClient_ConnParams));
    if (ATCmdMqtt_cb.attrib.connParams == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;        
    }
    memset(ATCmdMqtt_cb.attrib.connParams,0,sizeof(MQTTClient_ConnParams));
    ATCmdMqtt_cb.attrib.connParams->secureFiles = malloc(sizeof(char *) * ATCMDMQTT_CREATE_MAX_SECURE_FILES);
    if (ATCmdMqtt_cb.attrib.connParams->secureFiles == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;        
    }
    memset((void *)ATCmdMqtt_cb.attrib.connParams->secureFiles,0,sizeof(char *) * ATCMDMQTT_CREATE_MAX_SECURE_FILES);
    
    /* Call the command parser */
    ret = ATCmdMqtt_createParse((char *)arg,&ATCmdMqtt_cb.attrib);
	
	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }

    /* create mqtt instant */
    handle = MQTTClient_create(ATCmdMqtt_clientCallback, &ATCmdMqtt_cb.attrib);
    if(handle == NULL)
    {
        ATCmdMqtt_clientFree();
        ATCmd_errorResult(ATCmd_errorCmdStr,-1);
        return -1;
    }

    /* Open Client Receive Thread start the receive task. Set priority and    */
    /* stack size attributes                                                  */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;
    ret = pthread_attr_setschedparam(&pAttrs, &priParam);
    ret |= pthread_attr_setstacksize(&pAttrs, ATCMDMQTT_CLIENT_RX_TASK_SIZE);
    ret |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);
    ret |= pthread_create(&rxThread, &pAttrs, ATCmdMqtt_clientThread, NULL);
    if (ret != 0)
    {
        ATCmdMqtt_clientFree();
        return -1;
    }
    /* insert the handle to list */
    ATCmdMqtt_cb.clientIndex[index] = handle;
    ATCmd_commandResult(ATCmdMqtt_createResult,NULL,index);
    ATCmd_okResult();
    return 0;
}

/*
    MQTT client callback.
*/
int32_t ATCmdMqtt_deleteCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    MQTTClient_Handle handle;
    
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_TRM);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }

    ATCmdMqtt_cb.delRequest = 1;
    ret = MQTTClient_delete(handle);
    ATCmdMqtt_clientFree();

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    return ret;
}

/*
    MQTT client connect callback.
*/
int32_t ATCmdMqtt_connectCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    MQTTClient_Handle handle;
    
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_TRM);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    ret = MQTTClient_connect(handle);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    return ret;
}

/*
    MQTT client disconnect callback.
*/
int32_t ATCmdMqtt_disconnectCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    MQTTClient_Handle handle;
    
    /* parse the client index */
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_TRM);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    /* mqtt client disconnect */
    ret = MQTTClient_disconnect(handle);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    return ret;
}

/*
    Free allocated memory for MQTT client publish command
*/
int32_t ATCmdMqtt_publishFree(ATCmdMqtt_Publish_t *params)
{    
    if (params->topic != NULL)
    {
        free(params->topic);
    }

    if (params->msg != NULL)
    {
        free(params->msg);
    }
           
    return 0;
}


/*
    Parse MQTT client publish command.
*/
int32_t ATCmdMqtt_publishParse(char *buff, ATCmdMqtt_Publish_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen = 0;
    uint8_t retain;

    /* topic */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->topic), ATCMD_DELIM_ARG, ATCMDMQTT_PUBLISH_MAX_TOPIC_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    /* topic length */
    params->topicLen = strlen(params->topic);

    /* qos */
    if ((ret = StrMpl_getListVal(ATCmd_mqttQos, sizeof(ATCmd_mqttQos)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    /* retain */
    if ((ret = StrMpl_getVal(&buff, &retain , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    if (retain != 0)
    {
        params->flags |= MQTT_PUBLISH_RETAIN;
    }

    /* message length */
    if ((ret = StrMpl_getVal(&buff, &params->msgLen , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
    {
        return ret;
    }

    /* message */
    if (ATCmdMqtt_cb.dataFormat == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length to binary length */
        outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->msgLen); 
        params->msg = malloc(outputLen);
        if (params->msg == NULL)
        {
            return -1;
        }
        if (StrMpl_decodeBase64((uint8_t *)buff, params->msgLen, (uint8_t *)params->msg, (uint32_t *)&outputLen) < 0)
        {
            return -1;
        }
         params->msgLen =  outputLen;     
    }
    else
    {
        params->msg = malloc(params->msgLen);
        if (params->msg == NULL)
        {
            return -1;
        }

        memcpy(params->msg, buff, params->msgLen);
    }

    return ret;
}


/*
    MQTT client publish command callback.
*/
int32_t ATCmdMqtt_publishCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdMqtt_Publish_t  params;
    uint32_t index;
    MQTTClient_Handle handle;
    
    /* parse client index */
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    
    memset(&params, 0x0, sizeof(ATCmdMqtt_Publish_t));
    
    /* Call the command parser */
    ret = ATCmdMqtt_publishParse((char *)arg,&params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdMqtt_publishFree(&params);        
	    return -1;
    }
    ret = MQTTClient_publish(handle,params.topic,params.topicLen,params.msg,params.msgLen,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdMqtt_publishFree(&params);        

    return ret;
}

/*
    Free allocated memory for MQTT client subscribe command
*/
int32_t ATCmdMqtt_subscribeFree(ATCmdMqtt_Subscribe_t *params)
{    
    uint8_t i;

    if (params->value != NULL)
    {
        for (i=0;i<params->numberOfTopics;i++)
        {
            if (params->value[i].topic != NULL)
            {
                free(params->value[i].topic);
            }
        }
        free(params->value);
    }
           
    return 0;
}


/*
    Parse MQTT client subscribe command.
*/
int32_t ATCmdMqtt_subscribeParse(char *buff, ATCmdMqtt_Subscribe_t *params)
{
    int32_t ret = 0;
    uint8_t i;
    char delim = ATCMD_DELIM_ARG;

    /* number of topics */
    if ((ret = StrMpl_getVal(&buff, &params->numberOfTopics, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    if (params->numberOfTopics > ATCMDMQTT_SUBSCRIBE_MAX_TOPICS)
    {
        return STRMPL_ERROR_WRONG_PARAM;
    }
    params->value = malloc(params->numberOfTopics * sizeof(MQTTClient_SubscribeParams));
    if (params->value == NULL)
    {
        return -1;
    }
    memset(params->value,0,params->numberOfTopics * sizeof(MQTTClient_SubscribeParams));
    for (i=0;i<params->numberOfTopics;i++)
    {
        /* topic */
        if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->value[i].topic), ATCMD_DELIM_ARG, ATCMDMQTT_PUBLISH_MAX_TOPIC_LEN ,ATCmd_excludeDelimStr)) < 0)
        {
            return ret;
        }

        /* qos */
        if ((ret = StrMpl_getListVal(ATCmd_mqttQos, sizeof(ATCmd_mqttQos)/sizeof(StrMpl_List_t), &buff, &params->value[i].qos, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
        {
            return ret;
        }

        if (i == (params->numberOfTopics -1))
        {
            delim = ATCMD_DELIM_TRM;
        }
        /* persistent */
        if ((ret = StrMpl_getVal(&buff, &params->value[i].persistent,delim,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
        {
            if (ret != STRMPL_ERROR_PARAM_MISSING)
            {
                return ret;
            }
            ret = 0;
        }
        params->value[i].callback = NULL;
    }
    return ret;
}


/*
    MQTT client subscribe command callback.
*/
int32_t ATCmdMqtt_subscribeCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdMqtt_Subscribe_t  params;
    uint32_t index;
    MQTTClient_Handle handle;
    
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    
    memset(&params, 0x0, sizeof(ATCmdMqtt_Subscribe_t));
    
    /* Call the command parser */
    ret = ATCmdMqtt_subscribeParse((char *)arg,&params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdMqtt_subscribeFree(&params);        
	    return -1;
    }
    ret = MQTTClient_subscribe(handle,params.value,params.numberOfTopics);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdMqtt_subscribeFree(&params);        

    return ret;
}

/*
    Free allocated memory for MQTT client unsubscribe command
*/
int32_t ATCmdMqtt_unsubscribeFree(ATCmdMqtt_Unsubscribe_t *params)
{    
    uint8_t i;

    if (params->value != NULL)
    {
        for (i=0;i<params->numberOfTopics;i++)
        {
            if (params->value[i].topic != NULL)
            {
                free(params->value[i].topic);
            }
        }
        free(params->value);
    }
           
    return 0;
}


/*
    Parse MQTT client unsubscribe command.
*/
int32_t ATCmdMqtt_unsubscribeParse(char *buff, ATCmdMqtt_Unsubscribe_t *params)
{
    int32_t ret = 0;
    uint8_t i;
    char delim = ATCMD_DELIM_ARG;

    /* number of topics */
    if ((ret = StrMpl_getVal(&buff, &params->numberOfTopics, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    if (params->numberOfTopics > ATCMDMQTT_SUBSCRIBE_MAX_TOPICS)
    {
        return STRMPL_ERROR_WRONG_PARAM;
    }
    
    params->value = malloc(params->numberOfTopics * sizeof(MQTTClient_UnsubscribeParams));
    if (params->value == NULL)
    {
        return -1;
    }
    memset(params->value,0,params->numberOfTopics * sizeof(MQTTClient_UnsubscribeParams));
    for (i=0;i<params->numberOfTopics;i++)
    {
        /* topic */
        if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->value[i].topic), ATCMD_DELIM_ARG, ATCMDMQTT_PUBLISH_MAX_TOPIC_LEN ,ATCmd_excludeDelimStr)) < 0)
        {
            return ret;
        }

        if (i == (params->numberOfTopics -1))
        {
            delim = ATCMD_DELIM_TRM;
        }
        /* persistent */
        if ((ret = StrMpl_getVal(&buff, &params->value[i].persistent,delim,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
        {
            if (ret != STRMPL_ERROR_PARAM_MISSING)
            {
                return ret;
            }
            ret = 0;
        }
    }
    return ret;
}


/*
    MQTT client unsubscribe command callback.
*/
int32_t ATCmdMqtt_unsubscribeCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdMqtt_Unsubscribe_t  params;
    uint32_t index;
    MQTTClient_Handle handle;
    
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    
    memset(&params, 0x0, sizeof(ATCmdMqtt_Unsubscribe_t));
    
    /* Call the command parser */
    ret = ATCmdMqtt_unsubscribeParse((char *)arg,&params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdMqtt_unsubscribeFree(&params);        
	    return -1;
    }
    ret = MQTTClient_unsubscribe(handle,params.value,params.numberOfTopics);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdMqtt_unsubscribeFree(&params);        

    return ret;
}

/*
    Free allocated memory for MQTT client set command
*/
int32_t ATCmdMqtt_setFree(ATCmdMqtt_Set_t *params)
{    
    if (params != NULL)
    {
        if (params->usrName != NULL)
        {
            free(params->usrName);
        }
        if (params->usrPwd != NULL)
        {
            free(params->usrPwd);
        }
        if (params->mqttWill.willTopic != NULL)
        {
            free((void *)params->mqttWill.willTopic);
        }
        if (params->mqttWill.willMsg != NULL)
        {
            free((void *)params->mqttWill.willMsg);
        }

        free(params);
    }
    return 0;
}

/*
    Parse MQTT client set command.
*/
int32_t ATCmdMqtt_setParse(char *buff, ATCmdMqtt_Set_t *params)
{
    int32_t ret = 0;
    uint32_t outputLen = 0;    

    /* Option */
    if ((ret = StrMpl_getListVal(ATCmd_mqttSetOptions, sizeof(ATCmd_mqttSetOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    /* value */
    switch (params->option)
    {
        case MQTTClient_USER_NAME:          
            if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->usrName), ATCMD_DELIM_TRM, ATCMDMQTT_SET_MAX_USERNAME_LEN ,ATCmd_excludeDelimStr)) < 0)
            {
                return ret;
            }
            params->value = (void *)(params->usrName);
            break;
        case MQTTClient_PASSWORD:          
            if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->usrPwd), ATCMD_DELIM_TRM, ATCMDMQTT_SET_MAX_PASSWORD_LEN ,ATCmd_excludeDelimStr)) < 0)
            {
                return ret;
            }
            params->value = (void *)(params->usrPwd);
           break;
        case MQTTClient_WILL_PARAM:
            /* will topic */
            if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->mqttWill.willTopic), ATCMD_DELIM_ARG, ATCMDMQTT_PUBLISH_MAX_TOPIC_LEN ,ATCmd_excludeDelimStr)) < 0)
            {
                return ret;
            }
            /* will qos */
            if ((ret = StrMpl_getListVal(ATCmd_mqttQos, sizeof(ATCmd_mqttQos)/sizeof(StrMpl_List_t), &buff, &params->mqttWill.willQos, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            /* will retain */
            if ((ret = StrMpl_getVal(&buff, &params->mqttWill.retain , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }

            /* will message length */
            if ((ret = StrMpl_getVal(&buff, &params->willMsgLen , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
            {
                return ret;
            }

            /* will message */
            if (ATCmdMqtt_cb.dataFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert length to binary length */
                outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->willMsgLen); 
                params->mqttWill.willMsg = malloc(outputLen);
                if (params->mqttWill.willMsg == NULL)
                {
                    return -1;
                }
                if (StrMpl_decodeBase64((uint8_t *)buff, params->willMsgLen, (uint8_t *)params->mqttWill.willMsg, (uint32_t *)&outputLen) < 0)
                {
                    return -1;
                }
            }
            else
            {
                params->mqttWill.willMsg = malloc(params->willMsgLen);
                if (params->mqttWill.willMsg == NULL)
                {
                    return -1;
                }
                memcpy((void *)params->mqttWill.willMsg, buff, params->willMsgLen);
            }
            params->value = (void *)&(params->mqttWill);
            break;
        case MQTTClient_KEEPALIVE_TIME:
            /* keepalive time */
            if ((ret = StrMpl_getVal(&buff, &params->keepAliveTimeout, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
            {
                return ret;
            }
            params->value = (void *)&(params->keepAliveTimeout);
            break;
        case MQTTClient_CLEAN_CONNECT:
            /* clean */
            if ((ret = StrMpl_getVal(&buff, &params->clean, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            params->value = (void *)&(params->clean);
            break;
    }
    return ret;
}


/*
    MQTT client set command callback.
*/
int32_t ATCmdMqtt_setCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    MQTTClient_Handle handle;
    
    handle = ATCmdMqtt_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }

    ATCmdMqtt_cb.set = malloc(sizeof(ATCmdMqtt_Set_t));
    if (ATCmdMqtt_cb.set == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;
    }
    memset(ATCmdMqtt_cb.set, 0x0, sizeof(ATCmdMqtt_Set_t));
    
    /* Call the command parser */
    ret = ATCmdMqtt_setParse((char *)arg, ATCmdMqtt_cb.set);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdMqtt_setFree(ATCmdMqtt_cb.set);
	    return -1;
    }
    
    ret = MQTTClient_set(handle,ATCmdMqtt_cb.set->option,ATCmdMqtt_cb.set->value,0);

    if (ret < 0)
    {
        ATCmdMqtt_setFree(ATCmdMqtt_cb.set);
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    /* allocation shoud be freed in ATCmdMqtt_clientFree */

    return ret;
}

