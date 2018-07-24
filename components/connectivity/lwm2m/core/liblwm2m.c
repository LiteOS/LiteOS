/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>

*/

#include "internals.h"
#include "atiny_adapter.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>


lwm2m_context_t * lwm2m_init(void * userData)
{
    lwm2m_context_t * contextP;

    LOG("Entering");
    contextP = (lwm2m_context_t *)lwm2m_malloc(sizeof(lwm2m_context_t));
    if (NULL != contextP)
    {
        memset(contextP, 0, sizeof(lwm2m_context_t));
        contextP->userData = userData;
        lwm2m_rand((void *)&contextP->nextMID, sizeof(contextP->nextMID));
    }

    return contextP;
}

#ifdef LWM2M_CLIENT_MODE


static lwm2m_event_handler_t event_handler = NULL;
void lwm2m_register_event_handler(lwm2m_event_handler_t callback)
{
    event_handler = callback;
}

void lwm2m_notify_even(module_type_t type, int code, const char* arg, int arg_len)
{
    if(event_handler != NULL)
    {
        event_handler(type, code, arg, arg_len);
    }
}

void lwm2m_deregister(lwm2m_context_t * context)
{
    lwm2m_server_t * server = context->serverList;

    LOG("Entering");
    while (NULL != server)
    {
        registration_deregister(context, server);
        server = server->next;
    }
}

static void prv_deleteServer(lwm2m_server_t * serverP, void *userData)
{
    // TODO parse transaction and observation to remove the ones related to this server
    if (serverP->sessionH != NULL)
    {
         lwm2m_close_connection(serverP->sessionH, userData);
    }
    if (NULL != serverP->location)
    {
        lwm2m_free(serverP->location);
    }
    free_block1_buffer(serverP->block1Data);
    lwm2m_free(serverP);
}

static void prv_deleteServerList(lwm2m_context_t * context)
{
    while (NULL != context->serverList)
    {
        lwm2m_server_t * server;
        server = context->serverList;
        context->serverList = server->next;
        prv_deleteServer(server, context->userData);
    }
}

#ifdef LWM2M_BOOTSTRAP
static void prv_deleteBootstrapServer(lwm2m_server_t * serverP, void *userData)
{
    // TODO should we free location as in prv_deleteServer ?
    // TODO should we parse transaction and observation to remove the ones related to this server ?
    if (serverP->sessionH != NULL)
    {
         lwm2m_close_connection(serverP->sessionH, userData);
    }
    free_block1_buffer(serverP->block1Data);
    lwm2m_free(serverP);
}

static void prv_deleteBootstrapServerList(lwm2m_context_t * context)
{
    while (NULL != context->bootstrapServerList)
    {
        lwm2m_server_t * server;
        server = context->bootstrapServerList;
        context->bootstrapServerList = server->next;
        prv_deleteBootstrapServer(server, context->userData);
    }
}
#endif

static void prv_deleteObservedList(lwm2m_context_t * contextP)
{
    atiny_mutex_lock(contextP->observe_mutex);
    while (NULL != contextP->observedList)
    {
        lwm2m_observed_t * targetP;
        lwm2m_watcher_t * watcherP;

        targetP = contextP->observedList;
        contextP->observedList = contextP->observedList->next;

        lwm2m_notify_even(MODULE_URI, OBSERVE_UNSUBSCRIBE, (char*)&(targetP->uri), sizeof(targetP->uri));

        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            if (watcherP->parameters != NULL) lwm2m_free(watcherP->parameters);
        }
        LWM2M_LIST_FREE(targetP->watcherList);

        lwm2m_free(targetP);
    }
    atiny_mutex_unlock(contextP->observe_mutex);
}
#endif

void prv_deleteTransactionList(lwm2m_context_t * context)
{
    while (NULL != context->transactionList)
    {
        lwm2m_transaction_t * transaction;

        transaction = context->transactionList;
        context->transactionList = context->transactionList->next;
        if (transaction->callback != NULL)
        {
            transaction->callback(transaction, NULL);
        }
        transaction_free(transaction);
    }
}

void lwm2m_close(lwm2m_context_t * contextP)
{
#ifdef LWM2M_CLIENT_MODE

    LOG("Entering");
    lwm2m_deregister(contextP);
    prv_deleteServerList(contextP);
#ifdef LWM2M_BOOTSTRAP
    prv_deleteBootstrapServerList(contextP);
#endif
    prv_deleteObservedList(contextP);
    lwm2m_free(contextP->endpointName);
    //lwm2m_free(contextP->bs_server_uri);
    if (contextP->msisdn != NULL)
    {
        lwm2m_free(contextP->msisdn);
    }
    if (contextP->altPath != NULL)
    {
        lwm2m_free(contextP->altPath);
    }

#endif

#ifdef LWM2M_SERVER_MODE
    while (NULL != contextP->clientList)
    {
        lwm2m_client_t * clientP;

        clientP = contextP->clientList;
        contextP->clientList = contextP->clientList->next;

        registration_freeClient(clientP);
    }
#endif

    prv_deleteTransactionList(contextP);
    lwm2m_free(contextP);
}

#ifdef LWM2M_CLIENT_MODE
static int prv_refreshServerList(lwm2m_context_t * contextP)
{
    lwm2m_server_t * targetP;
    lwm2m_server_t * nextP;

    // Remove all servers marked as dirty
    targetP = contextP->bootstrapServerList;
    contextP->bootstrapServerList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            targetP->status = STATE_DEREGISTERED;
            contextP->bootstrapServerList = (lwm2m_server_t *)LWM2M_LIST_ADD(contextP->bootstrapServerList, targetP);
        }
        else
        {
            prv_deleteServer(targetP, contextP->userData);
        }
        targetP = nextP;
    }
    targetP = contextP->serverList;
    contextP->serverList = NULL;
    while (targetP != NULL)
    {
        nextP = targetP->next;
        targetP->next = NULL;
        if (!targetP->dirty)
        {
            // TODO: Should we revert the status to STATE_DEREGISTERED ?
            contextP->serverList = (lwm2m_server_t *)LWM2M_LIST_ADD(contextP->serverList, targetP);
        }
        else
        {
            prv_deleteServer(targetP, contextP->userData);
        }
        targetP = nextP;
    }

    return object_getServers(contextP, false);
}
//result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray)
int lwm2m_configure(lwm2m_context_t * contextP,
                    const char * endpointName,
                    const char * msisdn,
                    const char * altPath,
                    uint16_t numObject,
                    lwm2m_object_t * objectList[])
{
    int i;
    uint8_t found;

    //LOG_ARG("endpointName: \"%s\", msisdn: \"%s\", altPath: \"%s\", numObject: %d", endpointName, msisdn, altPath, numObject);
    // This API can be called only once for now
    if (contextP->endpointName != NULL || contextP->objectList != NULL) return COAP_400_BAD_REQUEST;

    if (endpointName == NULL) return COAP_400_BAD_REQUEST;
    if (numObject < 3) return COAP_400_BAD_REQUEST;
    // Check that mandatory objects are present
    found = 0;
    for (i = 0 ; i < numObject ; i++)
    {
        if(objectList[i] == NULL) // happens when undef CONFIG_FEATURE_FOTA
            continue;
        if (objectList[i]->objID == LWM2M_SECURITY_OBJECT_ID) found |= 0x01;
        if (objectList[i]->objID == LWM2M_SERVER_OBJECT_ID) found |= 0x02;
        if (objectList[i]->objID == LWM2M_DEVICE_OBJECT_ID) found |= 0x04;
    }
    if (found != 0x07) return COAP_400_BAD_REQUEST;
    if (altPath != NULL)
    {
        if (0 == utils_isAltPathValid(altPath))
        {
            return COAP_400_BAD_REQUEST;
        }
        if (altPath[1] == 0)
        {
            altPath = NULL;
        }
    }
    contextP->endpointName = lwm2m_strdup(endpointName);
    if (contextP->endpointName == NULL)
    {
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    if (msisdn != NULL)
    {
        contextP->msisdn = lwm2m_strdup(msisdn);
        if (contextP->msisdn == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    if (altPath != NULL)
    {
        contextP->altPath = lwm2m_strdup(altPath);
        if (contextP->altPath == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }

    for (i = 0; i < numObject; i++)
    {
        if(objectList[i] == NULL) // happens when undef CONFIG_FEATURE_FOTA
            continue;
        objectList[i]->next = NULL;
        contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_ADD(contextP->objectList, objectList[i]);
    }

    return COAP_NO_ERROR;
}

int lwm2m_add_object(lwm2m_context_t * contextP,
                     lwm2m_object_t * objectP)
{
    lwm2m_object_t * targetP;

    LOG_ARG("ID: %d", objectP->objID);
    targetP = (lwm2m_object_t *)LWM2M_LIST_FIND(contextP->objectList, objectP->objID);
    if (targetP != NULL) return COAP_406_NOT_ACCEPTABLE;
    objectP->next = NULL;

    contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_ADD(contextP->objectList, objectP);

    if (contextP->state == STATE_READY)
    {
        return lwm2m_update_registration(contextP, 0, true);
    }

    return COAP_NO_ERROR;
}

int lwm2m_remove_object(lwm2m_context_t * contextP,
                        uint16_t id)
{
    lwm2m_object_t * targetP;

    LOG_ARG("ID: %d", id);
    contextP->objectList = (lwm2m_object_t *)LWM2M_LIST_RM(contextP->objectList, id, &targetP);

    if (targetP == NULL) return COAP_404_NOT_FOUND;

    if (contextP->state == STATE_READY)
    {
        return lwm2m_update_registration(contextP, 0, true);
    }

    return 0;
}


static void lwm2m_reset_register(lwm2m_context_t * context)
{
    lwm2m_server_t * server = context->serverList;

    LOG("Entering");
    while (NULL != server)
    {
        registration_reset(context, server);
        server = server->next;
    }
}

int lwm2m_reconnect(lwm2m_context_t * context)
{
    if(NULL == context)
    {
        LOG("context null point");
        return COAP_405_METHOD_NOT_ALLOWED;
    }

    lwm2m_reset_register(context);
    prv_deleteObservedList(context);
    prv_deleteTransactionList(context);
    context->state = ((context->serverList != NULL) ? STATE_REGISTER_REQUIRED : STATE_BOOTSTRAP_REQUIRED);
    lwm2m_notify_even(MODULE_LWM2M, STATE_REG_FAILED, NULL, 0);
    return COAP_NO_ERROR;
}

#endif

/*
 * add date:     2018-06-04
 * description:  in order to mark the server of the bootstrapServerList and the serverList. think the server's data are
 * 				 dirty, should be delete from the list, and will be new soon.
 * return:       none
 * pay attention:
 *              the function in fact have been defined as prv_tagAllServer in another file. defined here again, in order to
 *              not include and contact closely with another file.
 *
 */
static void lwm2m_bootstrap_tag_IOT_server(lwm2m_context_t * contextP)
{
    lwm2m_server_t * targetP;

    targetP = contextP->serverList;
    while (targetP != NULL)
    {
        targetP->dirty = true;
        targetP = targetP->next;
    }
}


/*
 * add date:     2018-06-04
 * modify data:  2018-06-25
 * description:  only in the bootstrap sequence mode. when plan to change from factory to server, we should set the flag true for
 *               bootstrap(factory regist directly, flag is false,did not have bootstrapServerList). when set ok, we should
 *               start from the contextP->state = STATE_INITIAL
 *
 * return:
                success:  0
                fail:    -1
 *
 */
int lwm2m_bootstrap_sequence_factory_to_server_initiated(lwm2m_context_t * contextP)
{

    LOG("[bootstrap_tag]: --------lwm2m_bootstrap_sequence_factory_to_server_initiated----------");

    //if regist fail, mean all iot server could not regist well, so tag it.
     lwm2m_bootstrap_tag_IOT_server(contextP);  //only iot server

    //in the server initiated mod, will have a connection in the bootstrap_start , the bootstrap server uri/psk/bootstrap flag have been
    //set in the security_instance_t already. and the security_instance_t's some info also have been set in the bootstrapServerList

    //pay attention : we should get into lwm2m_step again, so the process : contextP->state = STATE_INITIAL; run
    //prv_refreshServerList, we have delete all iot_server, and delete the instance for iot server, so not any instance in serverList, only
    //the right instance of bootstrapserverlist , get into bootstrap process.

    return 0;
}


/*
 * add date:     2018-06-04
 * description:  only in the bootstrap sequence mode. when plan to change from server to client. we have bootstrapServerList, but some
 *               info, such as uri may not ok.and in client mode, we must have the suitable bs server ip. or else send request to who?
 * return:
                 success:  0
                 fail:    -1
 *
 */
int lwm2m_bootstrap_sequence_server_to_client_initiated(lwm2m_context_t * contextP)
{
    LOG("[bootstrap_tag]: --------lwm2m_bootstrap_sequence_server_to_client_initiated----------");
    //if regist fail, mean all iot server could not regist well, so tag it.
    lwm2m_bootstrap_tag_IOT_server(contextP);  ////only iot server

    ///no need write uri info, because have been stored in the instance for bootstrap server
    return 0;
}


/*
 * author:       twx378188
 * add date:     2018-06-04
 * description:  only used in the situation: regist fail. when regist fail, we could regist again and again. and in the bootstrap
 *               sequence mode, we could make some more, write new info and use bootstrap to get new info to regist.
 * return:
 *               success: 0
 *               fail:    -1
 * pay attention:
 *              regist using old info a period, how long ?
 *
 *
 */
static void lwm2m_bootstrap_process_reg_failed(lwm2m_context_t * contextP)
{
    static time_t start_cnt;
    static bool time_start = false;
    bool to_register_entrance = false;
    const int REGIST_INTERVAL_TIME = 100;
    time_t end_cnt = lwm2m_gettime();

    if(NULL == contextP)
    {
        LOG("[bootstrap_tag]: contextP is NULL, in bootstrap_dealwith_reg_failed");
        return;
    }

    if(contextP->bs_sequence_state == BS_SEQUENCE_STATE_FACTORY)
    {
        if(!time_start)
        {
            time_start = true;
            start_cnt = lwm2m_gettime();
        }
        end_cnt = lwm2m_gettime();

        //let's regist using old info for a period
        if(end_cnt >= REGIST_INTERVAL_TIME + start_cnt)
        {
            time_start = false;
            if(lwm2m_bootstrap_sequence_factory_to_server_initiated(contextP) == 0)
            {
                contextP->state = STATE_INITIAL;
                contextP->regist_first_flag = false;
                contextP->bs_sequence_state = BS_SEQUENCE_STATE_SERVER_INITIATED;
            }
            else
            {
                to_register_entrance = true;
            }
        }
        else
        {
            to_register_entrance = true;
        }

    }
    else if(contextP->bs_sequence_state == BS_SEQUENCE_STATE_SERVER_INITIATED)
    {
        if(!time_start)
        {
            time_start = true;
            start_cnt = lwm2m_gettime();
        }

        end_cnt = lwm2m_gettime();
        if(end_cnt >= REGIST_INTERVAL_TIME + start_cnt)
        {
            if(lwm2m_bootstrap_sequence_server_to_client_initiated(contextP) == 0)
            {
                contextP->state = STATE_INITIAL;
                contextP->regist_first_flag = false;
                contextP->bs_sequence_state = BS_SEQUENCE_STATE_CLIENT_INITIATED;
            }
            else
            {
                to_register_entrance = true;
            }
        }
        else
        {
            to_register_entrance = true;
        }
    }
    else if((contextP->bs_sequence_state == BS_SEQUENCE_STATE_CLIENT_INITIATED) || (contextP->bs_sequence_state == NO_BS_SEQUENCE_STATE))
    {
        to_register_entrance = true;
    }


    if(to_register_entrance)
    {
        contextP->state = STATE_REGISTER_REQUIRED;  //no other method to try registe, only again and again
        lwm2m_notify_even(MODULE_LWM2M,STATE_REG_FAILED, NULL, 0);
    }
}


int lwm2m_step(lwm2m_context_t * contextP,
               time_t * timeoutP)
{
    time_t tv_sec;
    int result;

    LOG_ARG("timeoutP: %" PRId64, *timeoutP);
    tv_sec = lwm2m_gettime();
    //if (tv_sec < 0) return COAP_500_INTERNAL_SERVER_ERROR;

#ifdef LWM2M_CLIENT_MODE
    LOG_ARG("State: %s", STR_STATE(contextP->state));
    // state can also be modified in bootstrap_handleCommand().

next_step:
    switch (contextP->state)
    {
    case STATE_INITIAL:
        if (0 != prv_refreshServerList(contextP))
        {
            LOG("prv_refreshServerList fail");
            return COAP_503_SERVICE_UNAVAILABLE;
        }

        if (contextP->serverList != NULL)
        {
            if(contextP->regist_first_flag)
            {
                contextP->state = STATE_REGISTER_REQUIRED;
            }
            else
            {
                contextP->state = STATE_BOOTSTRAP_REQUIRED;
            }
        }
        else
        {
            // Bootstrapping
            contextP->state = STATE_BOOTSTRAP_REQUIRED;
        }
        goto next_step;
        //break;

    case STATE_BOOTSTRAP_REQUIRED:
#ifdef LWM2M_BOOTSTRAP
        if (contextP->bootstrapServerList != NULL)
        {
            bootstrap_start(contextP);
            contextP->state = STATE_BOOTSTRAPPING;
            bootstrap_step(contextP, tv_sec, timeoutP);
						break;
        }
        else
#endif
        {
            return COAP_503_SERVICE_UNAVAILABLE;
        }

#ifdef LWM2M_BOOTSTRAP
    case STATE_BOOTSTRAPPING:
        switch (bootstrap_getStatus(contextP))
        {
        case STATE_BS_FINISHED:
            contextP->state = STATE_INITIAL;
            contextP->regist_first_flag = true;
            goto next_step;
            break;

        case STATE_BS_FAILED:
            return COAP_503_SERVICE_UNAVAILABLE;

        default:
            // keep on waiting
            bootstrap_step(contextP, tv_sec, timeoutP);
            break;
        }
        break;
#endif
    case STATE_REGISTER_REQUIRED:
        result = registration_start(contextP);
        LOG_ARG("[bootstrap_tag]: ---the return value result = %d of registration_start-----",result);
        if (COAP_NO_ERROR != result)
        {
            if(contextP->bs_sequence_state == BS_SEQUENCE_STATE_FACTORY)
            {
                //for the bs sequence mode, and in the state BS_SEQUENCE_STATE_FACTORY, even if fail, still have a chance
                //to get regist info from bs server. so not return. after get into registration_getStatus, judge all server
                //reg fail---because all server's are not in good state.
                contextP->state = STATE_REGISTERING;
            }
            else
            {
                return result;
            }
        }
        contextP->state = STATE_REGISTERING;
        break;

    case STATE_REGISTERING:
    {
        switch (registration_getStatus(contextP))
        {
        case STATE_REGISTERED:
            contextP->state = STATE_READY;
            lwm2m_notify_even(MODULE_LWM2M,STATE_REGISTERED, NULL, 0);
            break;

        case STATE_REG_FAILED:
            // TODO avoid infinite loop by checking the bootstrap info is different
            //contextP->state = STATE_BOOTSTRAP_REQUIRED;

        	//if regist fail, we may regist again or use other method to try  according the init param
            lwm2m_bootstrap_process_reg_failed(contextP);

        	//contextP->state = STATE_REGISTER_REQUIRED;
        	//lwm2m_notify_even(MODULE_LWM2M,STATE_REG_FAILED, NULL, 0);
            break;

        case STATE_REG_PENDING:
        default:
            // keep on waiting
            break;
        }
    }
    break;

    case STATE_READY:
        if (registration_getStatus(contextP) == STATE_REG_FAILED)
        {
            // TODO avoid infinite loop by checking the bootstrap info is different
            //contextP->state = STATE_BOOTSTRAP_REQUIRED;
            contextP->state = STATE_REGISTER_REQUIRED;
            goto next_step;
//            break;
        }
        break;

    default:
        // do nothing
        break;
    }

    observe_step(contextP, tv_sec, timeoutP);
#endif

    registration_step(contextP, tv_sec, timeoutP);
    transaction_step(contextP, tv_sec, timeoutP);

    LOG_ARG("Final timeoutP: %" PRId64, *timeoutP);
#ifdef LWM2M_CLIENT_MODE
    LOG_ARG("Final state: %s", STR_STATE(contextP->state));
#endif
    return 0;
}
