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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/Power.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>
#include <ti/net/utils/map_util.h>

#include "pthread.h"
#include "mqueue.h"
#include "unistd.h"


//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMD_TASKSTACKSIZE               		(4096)        /* Stack size in bytes */
#define ATCMD_SPAWN_TASK_PRIORITY         		(9)
#define ATCMD_BLOCKED_CMD_TASK_STACK_SIZE       (2048)
#define ATCMD_MAX_BLOCKED_TASK                  (5)
#define ATCMD_BLOCKED_TASK_PRIORITY             (5)
#define ATCMD_BLOCKED_TASK_STATE_FREE           (0)
#define ATCMD_BLOCKED_TASK_STATE_BUSY           (1)
#define ATCMD_BLOCKED_TASK_STATE_EXIT           (2)
#define ATCMD_DELIM_ENTRE_CMD                   '+'

//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmd_BlockedTask_t_
{
    pthread_t       thread;
    ATCmd_List_t    command;
    uint8_t         state;
}ATCmd_BlockedTask_t;

//*****************************************************************************
// Globals
//*****************************************************************************
mqd_t     ATCmd_rxQueue;
Map  *ATCmd_mapHanlde = NULL;    
ATCmd_BlockedTask_t ATCmd_blockedTask[ATCMD_MAX_BLOCKED_TASK];

//*****************************************************************************
// SimpleLink Callback Functions
//*****************************************************************************

void SimpleLinkNetAppRequestMemFreeEventHandler (uint8_t *buffer)
{
  // do nothing...
}

void SimpleLinkNetAppRequestEventHandler (SlNetAppRequest_t *pNetAppRequest, SlNetAppResponse_t *pNetAppResponse)
{
  // do nothing...
}

//*****************************************************************************
//
//! \brief The Function Handles WLAN Events
//!
//! \param[in]  pWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    ATCmdEvent_compose(pWlanEvent,sizeof(SlWlanEvent_t), ATCmdEvent_wlanCallback);
}

//*****************************************************************************
//
//! \brief The Function Handles the Fatal errors
//!
//! \param[in]  slFatalErrorEvent - Pointer to Fatal Error Event info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *slFatalErrorEvent)
{

    ATCmdEvent_compose(slFatalErrorEvent,sizeof(SlDeviceFatal_t), ATCmdEvent_fatalErrorCallback);
}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{

    ATCmdEvent_compose(pNetAppEvent,sizeof(SlNetAppEvent_t), ATCmdEvent_netappCallback);
}

//*****************************************************************************
//
//! \brief This function handles HTTP server events
//!
//! \param[in]  pServerEvent - Contains the relevant event information
//! \param[in]    pServerResponse - Should be filled by the user with the
//!                                      relevant response information
//!
//! \return None
//!
//****************************************************************************
void SimpleLinkHttpServerEventHandler(SlNetAppHttpServerEvent_t *pHttpEvent,
                                    SlNetAppHttpServerResponse_t *pHttpResponse)
{
    // Unused in this application
}

//*****************************************************************************
//
//! \brief This function handles General Events
//!
//! \param[in]     pDevEvent - Pointer to General Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
   //
    // Most of the general errors are not FATAL are are to be handled
    // appropriately by the application
    //
    ATCmdEvent_compose(pDevEvent,sizeof(SlDeviceEvent_t), ATCmdEvent_deviceGeneralCallback);

}

//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    ATCmdEvent_compose(pSock,sizeof(SlSockEvent_t), ATCmdEvent_sockCallback);
}

//*****************************************************************************
//
//! \brief Block Command Task
//!
//! \param[in]  arg - pointer to ATCmd_BlockedTask_t struct
//!
//! \return None
//!
//*****************************************************************************
void *ATCmd_blockedCmdTask(void *arg)
{
    ATCmd_BlockedTask_t *blockedTask = (ATCmd_BlockedTask_t *)arg; 

    blockedTask->command.callback((void *)(blockedTask->command.cmd));
    blockedTask->state = ATCMD_BLOCKED_TASK_STATE_FREE;
    pthread_exit(0);
    return NULL;
}


//*****************************************************************************
//
//! \brief The Function creates AT blocked Command task
//!
//! \param[in]  buffer -
//!
//! \return status
//!
//*****************************************************************************
int32_t ATCmd_blockedCmd(ATCmd_List_t *list, char *buff)
{    
    int32_t             status = 0;
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    uint8_t i;

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = ATCMD_BLOCKED_TASK_PRIORITY;
    pthread_attr_setschedparam(&pAttrs, &priParam);
    pthread_attr_setstacksize(&pAttrs, ATCMD_BLOCKED_CMD_TASK_STACK_SIZE);
    pthread_attr_setdetachstate(&pAttrs,PTHREAD_CREATE_DETACHED);

    /* create maximum 5 tasks in parallel */
    for (i = 0; i < ATCMD_MAX_BLOCKED_TASK; i++)
    {
        if (ATCmd_blockedTask[i].state == ATCMD_BLOCKED_TASK_STATE_FREE)
        {
            ATCmd_blockedTask[i].command.callback = list->callback;
            ATCmd_blockedTask[i].command.cmd = buff;
            ATCmd_blockedTask[i].state = ATCMD_BLOCKED_TASK_STATE_BUSY;
            break;
        }
    }
    if (i < ATCMD_MAX_BLOCKED_TASK)
    {
        status = pthread_create(&ATCmd_blockedTask[i].thread, &pAttrs, ATCmd_blockedCmdTask, (void *)&ATCmd_blockedTask[i]);
        if(status != 0)
        {
            status = -1;
        }
    }
    else
    {
        status = -1;
    }

    if (status == -1)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);        
    }
    return status;
}

//*****************************************************************************
//
//! \brief The Function receive AT Command response (events)
//!
//! \param[in]  buffer - Points to buffer allocated by the Application
//! \param[in]  nonblock - set 0 for waits forever on queue, otherwise set 1 
//!
//! \return status
//!
//*****************************************************************************
int32_t ATCmd_recv(char *buffer, uint8_t nonblock)
{
    ATCmd_Event_t msg;
    int status;
    struct mq_attr attr;

    mq_getattr(ATCmd_rxQueue, &attr);
    if (((attr.mq_flags == O_NONBLOCK) && (nonblock == 0)) ||
        ((attr.mq_flags == 0) && (nonblock == 1)))
    {
        attr.mq_flags = nonblock?O_NONBLOCK:0;
        mq_setattr(ATCmd_rxQueue, &attr,NULL);       
    }
    status = mq_receive(ATCmd_rxQueue, (char *)&msg, sizeof (ATCmd_Event_t), NULL);
    if (status >= 0)
    {
        if (msg.callback != NULL)
        {
            status = msg.callback(msg.args, msg.num, buffer);
        }
        else
        {
            strcpy(buffer,(char *)msg.args);
        }
        return status;
    }
    return -1;
}

//*****************************************************************************
//
//! \brief The Function send AT Command
//!
//! \param[in]  buffer - Points to buffer allocated by the Application
//!
//! \return None
//!
//*****************************************************************************
int32_t ATCmd_send(char *buffer)
{
    char *token = buffer;
    int32_t ret = 0;
    char excludeDelim[2] = {ATCMD_DELIM_STR,ATCMD_DELIM_STR};
    ATCmd_List_t *list = NULL;
    char *params;
    char *eol;

    /* Remove end of line (if exist) */
    eol = (buffer + strlen(buffer) - 1);
    if ((*eol == '\n') || (*eol == '\r'))
    {
        *eol = 0;
    }
    
    /* detrmine parameters */
    params = strchr(token, ATCMD_DELIM_CMD);
    if (params != NULL)
    {
        *params = 0;
        params++;
    }
    else
    {
        params = strchr(token, 0);
    }
    /* Remove spaces from command name */
    StrMpl_rmspc(token,ATCMD_DELIM_ENTRE_CMD,NULL);
    /* Change to lower case */
    StrMpl_lwr(token);
    if (strncmp(token, ATCMD_CMD_PREFIX,strlen(ATCMD_CMD_PREFIX)) == 0)
    {
        /* Jump over AT prefix */
        token += strlen(ATCMD_CMD_PREFIX);

        /* Get command index */
        list = (ATCmd_List_t *)Map_GetValue(ATCmd_mapHanlde, token);

        if (list != NULL)
        {
            /* move to the params */
            token = params;
            /* Remove spaces from params */
            StrMpl_rmspc(token,ATCMD_DELIM_ARG,excludeDelim);

            if (ATCmd_checkHelp(token))
            {
                /* Send usage string */                
                ATCmd_commandResult(ATCmd_commandUsage,list->usage,0);
                ATCmd_okResult();
            }
            else if (ATCmd_checkNumParams(token,list->numParams))
            {
                /* Number params incorrect */                
                ATCmd_errorResult(ATCmd_errorNumParamsStr,0);
                ret = -1;
            }
            else if ((list->blocked) == 1)
            {
                /* Blocked command */                
                ret = ATCmd_blockedCmd(list,token);
            }
            else 
            {
                /* Dispatch command callback */
                ret = list->callback((void *)(token));
            }
        }
    }

    if (list == NULL)
    {
        ATCmd_errorResult(ATCmd_errorExistCmdStr,0);
        ret = -1;
    }
 
    return(ret);
}


//*****************************************************************************
//
//! \brief The Function creates AT Command module
//!
//! \param[in]  buffer -
//!
//! \return None
//!
//*****************************************************************************
int32_t ATCmd_create(void)
{
    int32_t             status = 0;
    pthread_t           hostThread = (pthread_t)NULL;
    pthread_attr_t      hostAttr;
    struct sched_param  hostSched;
    struct mq_attr      attr;
    uint32_t            i;

    /* Start the SimpleLink Host */
    pthread_attr_init(&hostAttr);
    hostSched.sched_priority = ATCMD_SPAWN_TASK_PRIORITY;
    status = pthread_attr_setschedparam(&hostAttr, &hostSched);
    status |= pthread_attr_setstacksize(&hostAttr, ATCMD_TASKSTACKSIZE);

    status = pthread_create(&hostThread, &hostAttr, sl_Task, NULL);

    /* init blocked task */
    for (i = 0; i < ATCMD_MAX_BLOCKED_TASK; i++)
    {
        ATCmd_blockedTask[i].thread = NULL;
        ATCmd_blockedTask[i].state = ATCMD_BLOCKED_TASK_STATE_FREE;
    }
    /* AT RX events queue */
    attr.mq_maxmsg = ATCMD_RCV_QUEUE_SIZE;
    attr.mq_msgsize = sizeof (ATCmd_Event_t);
    ATCmd_rxQueue = mq_open("at_events", O_CREAT, 0, &attr);
    if(ATCmd_rxQueue == NULL)
    {
        return(-1);
    }

    /* init the cmd table */
    ATCmd_mapHanlde = Map_Create(MAP_ATTR_KEY_TYPE_STR);
    for (i = 0; i < ATCmd_maxCmd; i++)
    {
        Map_Insert(ATCmd_mapHanlde, ATCmd_list[i].cmd,  &ATCmd_list[i]);
    }
    
    return (0);
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
