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


//#ifdef ATCMD_MQTT_CLIENT_SUPPORT

#ifndef __ATCMD_MQTT_CLIENT_H__
#define __ATCMD_MQTT_CLIENT_H__

#include <stdint.h>

/*!
	\defgroup Mqtt
	\short Controls MQTT Client programming options and capabilities

*/
/*!

    \addtogroup Mqtt
    @{

*/
/*!
    \brief          Mqtt Client Create callback.

    This routine initialize the MQTT client instance.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_createCallback(void *arg);

/*!
    \brief          Mqtt Client delete callback.

    This routine delete mqtt client instance.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_deleteCallback(void *arg);

/*!
    \brief          Mqtt Client Connect callback.

    This routine connect to mqtt broker.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_connectCallback(void *arg);

/*!
    \brief          Mqtt Client Disconnect callback.

    This routine disconnect from mqtt broker.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_disconnectCallback(void *arg);


/*!
    \brief          Mqtt Client Publish callback.

    This routine send message to specific topic
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_publishCallback(void *arg);

/*!
    \brief          Mqtt Client Subscribe callback.

    This routine subscribe a topic for retrieving published messages
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_subscribeCallback(void *arg);

/*!
    \brief          Mqtt Client Unsubscribe callback.

    This routine unsubscribe a topic
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_unsubscribeCallback(void *arg);

/*!
    \brief          Mqtt Client Set callback.

    This routine set mqtt client configuration
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdMqtt_setCallback(void *arg);


/*!

 Close the Doxygen group.
 @}

 */



#endif /* __ATCMD_MQTT_CLIENT_H__ */

//#endif
