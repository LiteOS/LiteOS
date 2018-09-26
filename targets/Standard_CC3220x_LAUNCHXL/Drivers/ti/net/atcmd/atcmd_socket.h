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

#ifndef __ATCMD_SOCKET_H__
#define __ATCMD_SOCKET_H__

#include <stdint.h>

/*!
	\defgroup Socket 
	\short Controls standard client/server sockets programming options and capabilities

*/
/*!

    \addtogroup Socket
    @{

*/
/*!
    \brief          Sock Socket callback.

    This routine create socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_socketCallback(void *arg);

/*!
    \brief          Sock Close callback.

    This routine close socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_closeCallback(void *arg);

/*!
    \brief          Sock Accept callback.

    This routine Accept a connection on a socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_acceptCallback(void *arg);

/*!
    \brief          Sock Bind callback.

    This routine assign a name to a socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_bindCallback(void *arg);

/*!
    \brief          Sock Listen callback.

    This routine Listen for connections on a socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_listenCallback(void *arg);

/*!
    \brief          Sock Connect callback.

    This routine initiate a connection on a socket.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_connectCallback(void *arg);

/*!
    \brief          Sock Select callback.

    This routine monitor socket activity.
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_selectCallback(void *arg);

/*!
    \brief          Sock SetSockOpt callback.

    This routine Set socket options
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_setSockOptCallback(void *arg);

/*!
    \brief          Sock GetSockOpt callback.

    This routine get socket options
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_getSockOptCallback(void *arg);

/*!
    \brief          Sock Send callback.

    This routine write data to TCP socket
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_sendCallback(void *arg);

/*!
    \brief          Sock Recv callback.

    This routine     This routine read data from TCP socket
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_recvCallback(void *arg);

/*!
    \brief          Sock SendTo callback.

    This routine write data to socket
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_sendToCallback(void *arg);

/*!
    \brief          Sock RecvFrom callback.

    This routine read data from socket
    
    \param          arg       -   Points to command line buffer.

    \return                     Zero on success, or negative error code on failure
*/
int32_t ATCmdSock_recvFromCallback(void *arg);

/*!

 Close the Doxygen group.
 @}

 */



#endif /* __ATCMD_SOCKET_H__ */
