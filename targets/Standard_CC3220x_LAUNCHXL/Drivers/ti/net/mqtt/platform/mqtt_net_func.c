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

#include <unistd.h>

#include <ti/net/slneterr.h>
#include <ti/net/slnetutils.h>

#include "mqtt_net_func.h"

#define LISTEN_QUE_SIZE 2

//*****************************************************************************
//
//! \brief Open a TCP socket and modify its properties i.e security options if req.
//! Socket properties modified in this function are based on the options set
//! outside the scope of this function.
//! Returns a valid handle on success, otherwise a negative number.
//
//*****************************************************************************
static int32_t createSocket(uint32_t nwconnOpts,
        MQTT_SecureConn_t *nwSecurityOpts, const char *serverAddr, bool isServer)
{
    int32_t  socketFd;
    int32_t  status;
    uint32_t dummyVal = 1;
    SlNetSockSecAttrib_t *secAttrib;
    SlNetSockSecAttrib_e attribName;
    //local variables for creating secure socket
    uint8_t  SecurityMethod;
    uint32_t SecurityCypher;

    int8_t   i;

    //If TLS is required
    if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_SEC) != 0)
    {
        /* Create security attribute */
        secAttrib = SlNetSock_secAttribCreate();

        /* Check if the function failed */
        if (NULL == secAttrib)
        {
            return MQTT_PACKET_ERR_ALLOC_FAILED;
        }

        socketFd = SlNetSock_create(SLNETSOCK_AF_INET, SLNETSOCK_SOCK_STREAM, SLNETSOCK_PROTO_TCP, 0, 0);
        if (socketFd < 0)
        {
            SlNetSock_secAttribDelete(secAttrib);
            return (socketFd);
        }

        SecurityMethod = *((uint8_t *)(nwSecurityOpts->method));
        SecurityCypher = *((uint32_t *)(nwSecurityOpts->cipher));

        /* Domain name verification only works with URL address
           Check if Skip domain name verification is disabled
           and if the address input is url address.             */
        if (((nwconnOpts & MQTT_DEV_NETCONN_OPT_SKIP_DOMAIN_NAME_VERIFICATION) == 0) &&
            ((nwconnOpts & MQTT_DEV_NETCONN_OPT_URL) != 0))
        {
            status = SlNetSock_secAttribSet(secAttrib, SLNETSOCK_SEC_ATTRIB_DOMAIN_NAME, (void *)serverAddr, strlen(serverAddr));
            if (status < 0)
            {
                SlNetSock_secAttribDelete(secAttrib);
                SlNetSock_close(socketFd);
                return (status);
            }
        }

        /* Check if Skip certificate catalog verification is
           enabled.                                             */
        if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_SKIP_CERTIFICATE_CATALOG_VERIFICATION) != 0)
        {
            status = SlNetSock_secAttribSet(secAttrib, SLNETSOCK_SEC_ATTRIB_DISABLE_CERT_STORE, (void *)&dummyVal, sizeof(dummyVal));
            if (status < 0)
            {
                SlNetSock_secAttribDelete(secAttrib);
                SlNetSock_close(socketFd);
                return (status);
            }
        }

        if (nwSecurityOpts->nFile < 1 || nwSecurityOpts->nFile > 4)
        {
            SlNetSock_secAttribDelete(secAttrib);
            SlNetSock_close(socketFd);
            /* security files missing or wrong number of security files
               Did not create socket */
            return MQTT_PACKET_ERR_FNPARAM;
        }

        //Set Socket Options that were just defined
        status = SlNetSock_secAttribSet(secAttrib, SLNETSOCK_SEC_ATTRIB_METHOD, (void *)&(SecurityMethod), sizeof(SecurityMethod));
        if (status < 0)
        {
            SlNetSock_secAttribDelete(secAttrib);
            SlNetSock_close(socketFd);
            return (status);
        }

        status = SlNetSock_secAttribSet(secAttrib, SLNETSOCK_SEC_ATTRIB_CIPHERS, (void *)&(SecurityCypher), sizeof(SecurityCypher));
        if (status < 0)
        {
            SlNetSock_secAttribDelete(secAttrib);
            SlNetSock_close(socketFd);
            return (status);
        }

        if (nwSecurityOpts->nFile == 1)
        {
            status = SlNetSock_secAttribSet(secAttrib, SLNETSOCK_SEC_ATTRIB_PEER_ROOT_CA,
                                            (void *)nwSecurityOpts->files[0], strlen(nwSecurityOpts->files[0]));
            if (status < 0)
            {
                SlNetSock_secAttribDelete(secAttrib);
                SlNetSock_close(socketFd);
                return (status);
            }
        }
        else
        {
            for (i = 0; i < nwSecurityOpts->nFile; i++)
            {
                if (NULL != nwSecurityOpts->files[i])
                {
                    attribName = (SlNetSockSecAttrib_e)(SLNETSOCK_SEC_ATTRIB_PRIVATE_KEY + i);
                    status = SlNetSock_secAttribSet(secAttrib, attribName,
                                                    (void *)nwSecurityOpts->files[i], strlen(nwSecurityOpts->files[i]));
                    if (status < 0)
                    {
                        SlNetSock_secAttribDelete(secAttrib);
                        SlNetSock_close(socketFd);
                        return (status);
                    }
                }
            }
        }
        status = SlNetSock_startSec(socketFd, secAttrib, isServer ?
                (SLNETSOCK_SEC_BIND_CONTEXT_ONLY | SLNETSOCK_SEC_IS_SERVER) :
                SLNETSOCK_SEC_BIND_CONTEXT_ONLY);
        if (status < 0)
        {
            SlNetSock_secAttribDelete(secAttrib);
            SlNetSock_close(socketFd);
            return (status);
        }
        SlNetSock_secAttribDelete(secAttrib);

    }
    // If no TLS required
    else
    {
        // check to create a udp or tcp socket
        if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_UDP) != 0)
        {
            socketFd = SlNetSock_create(SLNETSOCK_AF_INET, SLNETSOCK_SOCK_DGRAM, SLNETSOCK_PROTO_UDP, 0, 0);
        }
        else // socket for tcp
        {
            socketFd = SlNetSock_create(SLNETSOCK_AF_INET, SLNETSOCK_SOCK_STREAM,
            SLNETSOCK_PROTO_TCP, 0, 0); // consider putting 0 in place of SLNETSOCK_PROTO_TCP
        }
    }

    return (socketFd);

}

//*****************************************************************************
//
//! \brief  Open a TCP socket with required properties
//! Also connect to the server.
//! Returns a valid handle on success, NULL on failure.
//
//*****************************************************************************
int32_t MQTTNet_commOpen(uint32_t nwconnOpts, const char *serverAddr, uint16_t portNumber, const MQTT_SecureConn_t *nwSecurity)
{
    int32_t status;
    int32_t socketFd;
    SlNetSock_AddrIn_t LocalAddr; //address of the server to connect to
    int32_t LocalAddrSize;
    uint32_t uiIP[4];
    uint16_t AddrLen = 1;

    // create socket
    socketFd = createSocket(nwconnOpts, (MQTT_SecureConn_t *)nwSecurity, serverAddr, false);

    if (socketFd < 0)
    {
        /* ERROR: Could not create a socket */
        return socketFd;
    }

    // Fill with default IPv4 parameters, overwrite when using IPv6
    LocalAddr.sin_family = SLNETSOCK_AF_INET;
    LocalAddrSize = sizeof(SlNetSock_AddrIn_t);
    LocalAddr.sin_port = SlNetUtil_htons(portNumber);

    if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_UDP) != 0)
    {
        //filling the UDP server socket address
        LocalAddr.sin_addr.s_addr = 0;

        status = SlNetSock_bind(socketFd, (SlNetSock_Addr_t *)&LocalAddr, LocalAddrSize);
        if (status < 0)
        {
            // error
            SlNetSock_close(socketFd);
            return status;
        }
    }
    else
    {
        // do tcp connection
        // get the ip address of server to do tcp connect
        if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_URL) != 0)
        {
            // server address is a URL
            status = SlNetUtil_getHostByName(0, (char *)serverAddr, strlen(serverAddr), (uint32_t *)uiIP, &AddrLen, SLNETSOCK_AF_INET);

            if (status < 0)
            {
                /* ERROR: Could not resolve the ip address of the server */
                SlNetSock_close(socketFd);
                return (status);
            }
            // convert the address to network byte order as the function returns
            // in host byte order
            LocalAddr.sin_addr.s_addr = SlNetUtil_htonl(uiIP[0]);
        }
        else // server address is a string in dot notation
        {
            if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_IP6) != 0)
            {
                LocalAddr.sin_family = SLNETSOCK_AF_INET;
                LocalAddrSize = sizeof(SlNetSock_AddrIn6_t);

                // server address is an IPV6 address string
                /* ERROR: Currently do not support IPV6 addresses */
                SlNetSock_close(socketFd);
                return (-1);
            }
            else
            {
                // address is an IPv4 string
                // get the server ip address in Network Byte order
                status = SlNetUtil_inetPton (SLNETSOCK_AF_INET, serverAddr, (void *)uiIP);
                if (0 == status)
                {
                    /* ERROR: Could not resolve the ip address of the server */
                    SlNetSock_close(socketFd);
                    return (-1);
                }
                LocalAddr.sin_addr.s_addr = uiIP[0];
            }
        }

        // do tcp connect
        status = SlNetSock_connect(socketFd, (SlNetSock_Addr_t *)&LocalAddr, LocalAddrSize);

        if (status < 0)
        {
            /* ERROR: SlNetSock_connect failed */

            if ((SLNETERR_ESEC_SNO_VERIFY != status) || ((SLNETERR_ESEC_DATE_ERROR != status) && (nwconnOpts & MQTT_DEV_NETCONN_OPT_SKIP_DATE_VERIFICATION)))
            {
                /* ERROR: Could not establish connection to server, Closing the socket */
                SlNetSock_close(socketFd);
                return (status);
            }
            // else - SLNETERR_ESEC_SNO_VERIFY == status or SLNETERR_ESEC_DATE_ERROR == status
            /* ERROR: Could not establish secure connection to server,
               Continuing with unsecured connection to server */
        }

        if ((nwconnOpts & MQTT_DEV_NETCONN_OPT_SEC) != 0)
        {
            status = SlNetSock_startSec(socketFd, NULL, SLNETSOCK_SEC_START_SECURITY_SESSION_ONLY);
            if (status < 0)
            {
                SlNetSock_close(socketFd);
                return (status);
            }
        }

        // Success - Connected to server

    } // end of doing binding port to udp socket or doing tcp connect

    return (socketFd);

}

int32_t MQTTNet_tcpSend(int32_t comm, const uint8_t *buf, uint32_t len, void *ctx)
{
    int32_t status;

    /* TCP send invoked */

    status = SlNetSock_send(comm, buf, len, 0);

    return (status);

}

int32_t MQTTNet_tcpRecv(int32_t comm, uint8_t *buf, uint32_t len, uint32_t waitSecs, bool *timedOut, void *ctx)
{
    int32_t status;
    int32_t socketFd = comm;

#ifdef SOC_RCV_TIMEOUT_OPT

    // socket receive time out options
    SlNetSock_Timeval_t timeVal;

    // recv time out options
    timeVal.tv_sec = waitSecs;      // Seconds
    timeVal.tv_usec = 0;          // Microseconds. 10000 microseconds resolution

    // setting receive timeout option on socket

    status = SlNetSock_setOpt(socketFd, SLNETSOCK_LVL_SOCKET, SLNETSOCK_OPSOCK_RCV_TIMEO, &timeVal, sizeof(timeVal));

    //end of setting receive timeout option on socket

#endif

    /* TCP recv invoked */
    *timedOut = 0;

    status = SlNetSock_recv(socketFd, buf, len, 0);

    if (0 == status)
    {
        /* Connection Closed by peer */
    }

    if (SLNETERR_BSD_EAGAIN == status)
    {
        /* ERROR: Recv Time out error on server socket */
        *timedOut = 1;
    }

    return (status);

}

int32_t MQTTNet_commClose(int32_t comm)
{
    int32_t status;

    status = SlNetSock_close(comm);
    usleep(1000);

    return (status);
}

//*****************************************************************************
//
//! \brief Provides a monotonically incrementing value of a time  service in
//! unit of seconds. The implementation should ensure that associated timer
//! hardware or the clock module remains active through the low power states
//! of the system. Such an arrangement ensures that MQTT Library is able to
//! track the Keep-Alive time across the cycles of low power states. It would
//! be typical of battery operated systems to transition to low power states
//! during the period of inactivity or otherwise to conserve battery.
//
//*****************************************************************************
uint32_t MQTTNet_rtcSecs(void)
{
    struct timespec ts;

    ts.tv_sec  = 0;
    ts.tv_nsec = 0;

    /* Get time since the beginning of the Epoch */
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec);
}


int32_t MQTTNet_tcpListen(uint32_t nwconnInfo, uint16_t portNumber, const MQTT_SecureConn_t *nwSecurity)
{
    SlNetSock_AddrIn_t localAddr;
    int32_t sockID;
    int32_t addrSize;
    int32_t status;

    //filling the TCP server socket address
    localAddr.sin_family = SLNETSOCK_AF_INET;
    localAddr.sin_port = SlNetUtil_htons(portNumber);
    localAddr.sin_addr.s_addr = 0;
    addrSize = sizeof(SlNetSock_AddrIn_t);

    // creating a TCP socket
    sockID = createSocket(nwconnInfo, (MQTT_SecureConn_t *) nwSecurity, NULL, true);
    if (sockID < 0)
    {
        /* ERROR: Could not create a socket */
        return (sockID);
    }

    // binding the TCP socket to the TCP server address
    status = SlNetSock_bind(sockID, (SlNetSock_Addr_t *) &localAddr, addrSize);
    if (status < 0)
    {
        /* ERROR: Could not bind a socket */
        SlNetSock_close(sockID);
        return status;
    }

    // putting the socket for listening to the incoming TCP connection
    status = SlNetSock_listen(sockID, LISTEN_QUE_SIZE);
    if (status < 0)
    {
        /* ERROR: Could not set a socket to listen */
        SlNetSock_close(sockID);
        return status;
    }

    return (sockID);

}

int32_t MQTTNet_tcpSelect(int32_t *recvCvec, int32_t *sendCvec, int32_t *rsvdCvec, uint32_t waitSecs)
{
    SlNetSock_Timeval_t tv;
    SlNetSock_Timeval_t *pTv;
    SlNetSock_SdSet_t rdfds;
    int32_t rdIdx = 0;
    int32_t wrIdx = 0;
    int32_t max_fd = 0;
    int32_t rv = 0;
    int32_t fd;

    tv.tv_sec = waitSecs;
    tv.tv_usec = 0;

    pTv = (0xFFFFFFFF != waitSecs) ? &tv : NULL;

    SlNetSock_sdsClrAll(&rdfds);

    while (-1 != recvCvec[rdIdx])
    {
        fd = recvCvec[rdIdx++];

        SlNetSock_sdsSet(fd, &rdfds);

        if (max_fd < fd)
            max_fd = fd;
    }

    rv = SlNetSock_select(max_fd + 1, &rdfds, NULL, NULL, pTv);

    if (rv == 0)
    {
        /* Connection Timed out */
        return rv;
    }

    if (rv < 0)
    {
        /* Select Failed */
        return rv;
    }

    rdIdx = 0;
    while (-1 != recvCvec[rdIdx])
    {
        fd = recvCvec[rdIdx++];
        if (SlNetSock_sdsIsSet(fd, &rdfds))
            recvCvec[wrIdx++] = fd;
    }

    recvCvec[wrIdx] = -1;

    return (wrIdx);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
int32_t MQTTNet_tcpAccept(uint32_t nwconnInfo, int32_t listenHnd, uint8_t *clientIP, uint32_t *ipLen)
{
    int32_t new_fd;
    int32_t retVal;
    SlNetSock_AddrIn_t clientAddr = { 0 }; // client address
    SlNetSocklen_t clAddrSize;

    clAddrSize = sizeof(clientAddr);

    new_fd = SlNetSock_accept(listenHnd, (SlNetSock_Addr_t *) &clientAddr, &clAddrSize);

    if (new_fd < 0)
    {
        /* ERROR: in accept */
        return (new_fd);
    }

    if ((nwconnInfo & MQTT_DEV_NETCONN_OPT_SEC) != 0)
    {
        retVal = SlNetSock_startSec(new_fd, NULL,
                SLNETSOCK_SEC_START_SECURITY_SESSION_ONLY | SLNETSOCK_SEC_IS_SERVER);
        if (retVal < 0)
        {
            SlNetSock_close(new_fd);
            return (retVal);
        }
    }

    clientIP[0] = (clientAddr.sin_addr.s_addr & 0xFF000000) >> 24;
    clientIP[1] = (clientAddr.sin_addr.s_addr & 0x00FF0000) >> 16;
    clientIP[2] = (clientAddr.sin_addr.s_addr & 0x0000FF00) >> 8;
    clientIP[3] = (clientAddr.sin_addr.s_addr & 0x000000FF);

    *ipLen = 4;

    return new_fd;

}


//*****************************************************************************
//
//! \brief  udp functionalities
//
//*****************************************************************************
int32_t MQTTNet_sendTo(int32_t comm, const uint8_t *buf, uint32_t len, uint16_t destPort, const uint8_t *destIP, uint32_t ipLen)
{
    int32_t sockID = (int32_t) comm;
    int32_t status;
    int32_t addrSize;
    SlNetSock_AddrIn_t sAddr;
    uint32_t destIp;

    //get destination ip
    destIp = (((uint32_t) destIP[0] << 24) | ((uint32_t) destIP[1] << 16) | (destIP[2] << 8) | (destIP[3]));

    //filling the UDP server socket address
    sAddr.sin_family = SLNETSOCK_AF_INET;
    sAddr.sin_port = SlNetUtil_htons((unsigned short) destPort);
    sAddr.sin_addr.s_addr = SlNetUtil_htonl(destIp);

    addrSize = sizeof(SlNetSock_AddrIn_t);

    // sending packet
    status = SlNetSock_sendTo(sockID, buf, len, 0, (SlNetSock_Addr_t *) &sAddr, addrSize);

    if (status <= 0)
    {
        // Error: Closed the UDP socket
        SlNetSock_close(sockID);
    }

    return (status);

}

//*****************************************************************************
//
//! \brief  udp functionalities
//
//*****************************************************************************
int32_t MQTTNet_recvFrom(int32_t comm, uint8_t *buf, uint32_t len, uint16_t *fromPort, uint8_t *fromIP, uint32_t *ipLen)
{

    int32_t sockID = (int32_t) comm;
    int32_t status;
    int32_t addrSize;
    SlNetSock_AddrIn_t fromAddr = { 0 };

    addrSize = sizeof(SlNetSock_AddrIn_t);

    status = SlNetSock_recvFrom(sockID, buf, len, 0, (SlNetSock_Addr_t *)&fromAddr, (SlNetSocklen_t *)&addrSize);

    if (status < 0)
    {
        // Error: Closed the UDP socket
        SlNetSock_close(sockID);
        return (status);
    }

    //else populate from ip, fromPort and ipLen parameters
    // refer to comments in .h
    if (fromPort)
    {
        *fromPort = fromAddr.sin_port;
    }
    if (fromIP)
    {
        fromIP[0] = (fromAddr.sin_addr.s_addr & 0xFF000000) >> 24;
        fromIP[1] = (fromAddr.sin_addr.s_addr & 0x00FF0000) >> 16;
        fromIP[2] = (fromAddr.sin_addr.s_addr & 0x0000FF00) >> 8;
        fromIP[3] = (fromAddr.sin_addr.s_addr & 0x000000FF);
        *ipLen = 4;
    }

    return (status);

}
