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


//#ifdef ATCMD_HTTP_CLIENT_SUPPORT

#ifndef __ATCMD_HTTP_CLIENT_H__
#define __ATCMD_HTTP_CLIENT_H__

#include <stdint.h>

/*!
	\defgroup Http
	\short Controls HTTP Client programming options and capabilities

*/
/*!

    \addtogroup Http
    @{

*/
/*!
    \brief          Http Client Create callback.

    This routine initialize the HTTP client instance.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_createCallback(void *arg);

/*!
    \brief          Http Client destroy callback.

    This routine destroy http client instance.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_destroyCallback(void *arg);

/*!
    \brief          Http Client Connect callback.

    This routine connect to http server.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_connectCallback(void *arg);

/*!
    \brief          Http Client Disconnect callback.

    This routine disconnect from http server.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_disconnectCallback(void *arg);


/*!
    \brief          Http Client send request callback.

    This routine send request to http server
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_sendReqCallback(void *arg);

/*!
    \brief          Http Client response callback.

    This routine read response body data from HTTP server
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_respBodyCallback(void *arg);

/*!
    \brief          Http Client Set header callback.

    This routine set http client header configuration
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_setHeaderCallback(void *arg);

/*!
    \brief          Http Client Get header callback.

    This routine get http client header configuration
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_getHeaderCallback(void *arg);

/*!
    \brief          Http Client Set callback.

    This routine set http client configuration
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_setOptCallback(void *arg);


/*!
    \brief          Http Client Set proxy callback.

    This routine set http client proxy
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdHttp_setProxyCallback(void *arg);

/*!

 Close the Doxygen group.
 @}

 */



#endif /* __ATCMD_HTTP_CLIENT_H__ */

//#endif
