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

#ifndef __ATCMD_NETAPP_H__
#define __ATCMD_NETAPP_H__

#include <stdint.h>

/*!
	\defgroup NetApp 
	\short Activates networking applications, such as: HTTP Server, DHCP Server, Ping, DNS and mDNS

*/
/*!

    \addtogroup NetApp
    @{

*/
/*!
    \brief          Netapp start callback.

    This routine starts a network application
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_startCallback(void *arg);

/*!
    \brief          Netapp stop callback.

    This routine stops a network application
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_stopCallback(void *arg);

/*!
    \brief          Netapp Get Host By Name callback.

    This routine get host IP by name
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_getHostByNameCallback(void *arg);

/*!
    \brief          Netapp Get Host By Service callback.

    This routine get host IP by service
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_getHostByServiceCallback(void *arg);

/*!
    \brief          Netapp Get callback.

    This routine gets network applications configurations
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_getCallback(void *arg);

/*!
    \brief          Netapp Set callback.

    This routine sets network applications configurations
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_setCallback(void *arg);

/*!
    \brief          Netapp Send callback.

    This routine sends Netapp response or data
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_sendCallback(void *arg);

/*!
    \brief          Netapp Recv callback.

    This routine receives data from the network processor
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_recvCallback(void *arg);

/*!
    \brief          Netapp Ping callback.

    This routine send ping to network hosts
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_pingCallback(void *arg);

/*!
    \brief          Netapp GetServiceList  callback.

    This routine get service list 
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_getServiceListCallback(void *arg);

/*!
    \brief          Netapp RegisterMDNS callback.

    This routine register a new mDNS service 
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_registerServiceCallback(void *arg);

/*!
    \brief          Netapp UnregisterMDNS callback.

    This routine unregister a mDNS service 
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdNetapp_unregisterServiceCallback(void *arg);

/*!

 Close the Doxygen group.
 @}

 */

#endif /* __ATCMD_NETAPP_H__ */
