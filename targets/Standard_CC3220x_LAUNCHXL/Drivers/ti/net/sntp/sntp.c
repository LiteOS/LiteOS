/*
 * Copyright (c) 2014-2018, Texas Instruments Incorporated
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
/*
 *  ======== sntp.c ========
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <ti/net/slnetsock.h>
#include <ti/net/slnetutils.h>
#include <ti/net/sntp/sntp.h>

#define SNTP_PORT           123

/* KOD error code: rate exceeded, server requesting NTP client to back off */
#define SNTP_KOD_RATE_STR "RATE"
#define SNTP_KOD_RATE_CODE 3

/* KOD error code: access denied, server requests client to end all comm */
#define SNTP_KOD_DENY_STR "DENY"
#define SNTP_KOD_DENY_CODE 2

/* KOD error code: access denied, server requests client to end all comm */
#define SNTP_KOD_RSTR_STR "RSTR"
#define SNTP_KOD_RSTR_CODE 1

/* Size of KOD error codes */
#define SNTP_KOD_ERROR_CODE_SIZE 4

/* Use NTP version 4 */
#define SNTP_VERSION 4

/* Flag value for unsync'ed leap indicator field, signifying server error */
#define SNTP_NOSYNC 3

/* NTP mode defined in RFC 4330 */
#define SNTP_MODE_CLIENT 3

/* SNTP Header (as specified in RFC 4330) */
typedef struct _SNTP_Header_t_
{
    /*
     *  'flags' stores three values:
     *
     *    - 2 bit Leap Indicator (LI)
     *    - 3 bit Version Number (VN)
     *    - 3 bit Mode.
     */
    uint8_t flags;
    uint8_t stratum;
    uint8_t poll;
    int8_t  precision;
    int32_t  rootDelay;
    uint32_t  rootDispersion;
    uint32_t  referenceID;

    /* NTP time stamps */
    uint32_t referenceTS[2];
    uint32_t originateTS[2];
    uint32_t receiveTS[2];
    uint32_t transmitTS[2];
} SNTP_Header_t;

static const char * SNTP_internalServersList[] = {
    "time.google.com",
    "time.nist.gov",
    "utcnist.colorado.edu",
    "utcnist2.colorado.edu",
    "nist-time-server.eoni.com",
};

static int32_t getAddrByName(const char *name, uint32_t *addr,
        uint16_t *family);
static int32_t hasKissCode(char *str);
static int32_t getTime(SlNetSock_Addr_t *server, uint16_t ifID,
        SlNetSock_Timeval_t *timeout, uint64_t *ntpTimeStamp);

/*
 *  ======== getAddrByName ========
 *  Retrieve host IP address corresponding to a host name
 */
static int32_t getAddrByName(const char *name, uint32_t *addr, uint16_t *family)
{
    int32_t       ifID;
    uint16_t      addrLen = 1;

    /* Query DNS for IPv4 address. */
    ifID = SlNetUtil_getHostByName(0, (char *)name, strlen(name), addr, &addrLen, SLNETSOCK_AF_INET);

    if(ifID < 0)
    {
        /* If call fails, try again for IPv6. */
        ifID = SlNetUtil_getHostByName(0, (char *)name, strlen(name), addr, &addrLen, SLNETSOCK_AF_INET6);
        if(ifID < 0)
        {
            /* return an error */
            return -1;
        }
        else
        {
            *family = SLNETSOCK_AF_INET6;
        }
    }
    else
    {
        *family = SLNETSOCK_AF_INET;
    }

    /* Return the interface ID */
    return (ifID);
}

/*
 *  ======== hasKissCode ========
 */
static int32_t hasKissCode(char *str)
{
    if (strncmp((char *)SNTP_KOD_RATE_STR, str, SNTP_KOD_ERROR_CODE_SIZE) == 0)
    {
        return (SNTP_KOD_RATE_CODE);
    }
    else if (strncmp((char *)SNTP_KOD_DENY_STR, str, SNTP_KOD_ERROR_CODE_SIZE) == 0)
    {
        return (SNTP_KOD_DENY_CODE);
    }
    else if (strncmp((char *)SNTP_KOD_RSTR_STR, str, SNTP_KOD_ERROR_CODE_SIZE) == 0)
    {
        return (SNTP_KOD_RSTR_CODE);
    }
    else
    {
        return (0);
    }
}

/*
 *  ======== getTime ========
 */
static int32_t getTime(SlNetSock_Addr_t *server, uint16_t ifID,
        SlNetSock_Timeval_t *timeout, uint64_t *ntpTimeStamp)
{
    SNTP_Header_t sntpPkt;
    int32_t ret = 0;
    int16_t sd = -1;
    SlNetSocklen_t saLen;
    uint32_t integrityCheck;
    struct timespec tspec;

    if (server->sa_family == SLNETSOCK_AF_INET)
    {
        saLen = sizeof(SlNetSock_AddrIn_t);
    }
    else if (server->sa_family == SLNETSOCK_AF_INET6)
    {
        saLen = sizeof(SlNetSock_AddrIn6_t);
    }
    else
    {
        return (SNTP_EINVALIDFAMILY);
    }

    /* Create a UDP socket to communicate with NTP server */
    sd = SlNetSock_create(server->sa_family, SLNETSOCK_SOCK_DGRAM, SLNETSOCK_PROTO_UDP, ifID, 0);
    if (sd < 0)
    {
        return (SNTP_ESOCKCREATEFAIL);
    }

    ret = SlNetSock_connect(sd, server, saLen);
    if (ret < 0)
    {
        SlNetSock_close(sd);
        return (SNTP_ECONNECTFAIL);
    }

    if (timeout != NULL)
    {
        /* Set the timeout for the server response to the user's value */
        ret = SlNetSock_setOpt(sd, SLNETSOCK_LVL_SOCKET, SLNETSOCK_OPSOCK_RCV_TIMEO, timeout, sizeof(SlNetSock_Timeval_t));
        if (ret < 0)
        {
            SlNetSock_close(sd);
            return (SNTP_ESOCKOPTFAIL);
        }
    }

    /* Initialize the SNTP packet, setting version and mode = client */
    memset(&sntpPkt, 0, sizeof(SNTP_Header_t));
    sntpPkt.flags = SNTP_VERSION << 3;
    sntpPkt.flags |= SNTP_MODE_CLIENT;

    /* Set packet's transmit time as integrity check value */
    clock_gettime(CLOCK_REALTIME, &tspec);
    integrityCheck = tspec.tv_sec;
    sntpPkt.transmitTS[0] = integrityCheck;

    /* Send out our SNTP request to the current server */
    ret = SlNetSock_send(sd, (void *)&sntpPkt, sizeof(SNTP_Header_t), 0);
    if (ret < 0)
    {
        SlNetSock_close(sd);
        return (SNTP_ESENDFAIL);
    }

    memset(&sntpPkt, 0, sizeof(SNTP_Header_t));

    /* Retrieve the NTP packet from the socket and update our time. */
    ret = SlNetSock_recv(sd, &sntpPkt, sizeof(SNTP_Header_t), 0);
    if ((ret < 0) || (ret != sizeof(SNTP_Header_t)) || (sntpPkt.originateTS[0] != integrityCheck))
    {
        SlNetSock_close(sd);
        return (SNTP_ERECVFAIL);
    }

    /* Check for errors in server response */
    if (sntpPkt.stratum == 0)
    {
        /* Per RFC5905, we MUST handle Kiss O' Death packet */
        if ((sntpPkt.flags >> 6) == SNTP_NOSYNC)
        {
            /* KOD recv'd. Inspect kiss code & handle accordingly */
            ret = hasKissCode((char *)&sntpPkt.referenceID);

            if (ret == SNTP_KOD_RATE_CODE)
            {
                SlNetSock_close(sd);
                return (SNTP_ERATEBACKOFF);
            }
            /* Check for fatal kiss codes */
            else if ((ret == SNTP_KOD_DENY_CODE) || (ret == SNTP_KOD_RSTR_CODE))
            {
                SlNetSock_close(sd);
                return (SNTP_EFATALNORETRY);
            }
            /* Per RFC5905, other kiss codes are ignored */
        }
        else
        {
            /*
             *  A server response with stratum == 0, with no kiss
             *  code, is a fatal error. Mark server as invalid
             */
            SlNetSock_close(sd);
            return (SNTP_EINVALIDRESP);
        }
    }

    /* return the time in seconds */
    sntpPkt.transmitTS[0] = SlNetUtil_ntohl(sntpPkt.transmitTS[0]);
    sntpPkt.transmitTS[1] = SlNetUtil_ntohl(sntpPkt.transmitTS[1]);

    *ntpTimeStamp = ((uint64_t)sntpPkt.transmitTS[0] << 32) | sntpPkt.transmitTS[1];

    SlNetSock_close(sd);
    return (0);
}

/*
 *  ======== SNTP_getTime ========
 */
int32_t SNTP_getTime(const char *srvList[], const uint32_t srvCount,
        SlNetSock_Timeval_t *timeout, uint64_t *ntpTimeStamp)
{
    int32_t ret = 0;
    int32_t ifID;
    SlNetSock_AddrIn_t sa4;
    SlNetSock_AddrIn6_t sa6;
    SlNetSock_Addr_t *sa;
    uint32_t addr[4];
    uint16_t family;
    uint32_t i;
    char **list;
    size_t count;

    if ((srvCount == 0) || (srvList == NULL))
    {
        count = (sizeof (SNTP_internalServersList) / sizeof (const char *));
        list = (char **)SNTP_internalServersList;
    }
    else
    {
        count = srvCount;
        list = (char **)srvList;
    }

    for (i = 0; i < count; i++)
    {
        memset(&addr, 0, sizeof(addr));
        ifID = getAddrByName(list[i], addr, &family);
        if(ifID >= 0)
        {
            if (family == SLNETSOCK_AF_INET)
            {
                sa4.sin_family = SLNETSOCK_AF_INET;
                sa4.sin_port = SlNetUtil_htons(SNTP_PORT);
                sa4.sin_addr.s_addr = SlNetUtil_htonl(addr[0]);
                sa = (SlNetSock_Addr_t *)&sa4;
            }
            else
            {
                sa6.sin6_family = SLNETSOCK_AF_INET6;
                sa6.sin6_port = SlNetUtil_htons(SNTP_PORT);
                sa6.sin6_addr._S6_un._S6_u32[0] = SlNetUtil_htonl(addr[0]);
                sa6.sin6_addr._S6_un._S6_u32[1] = SlNetUtil_htonl(addr[1]);
                sa6.sin6_addr._S6_un._S6_u32[2] = SlNetUtil_htonl(addr[2]);
                sa6.sin6_addr._S6_un._S6_u32[3] = SlNetUtil_htonl(addr[3]);
                sa = (SlNetSock_Addr_t *)&sa6;
            }
        }
        else
        {
            ret = SNTP_EGETHOSTBYNAMEFAIL;
            continue;
        }

        ret = getTime(sa, ifID, timeout, ntpTimeStamp);
        if (ret == 0)
        {
            break;
        }
    }

    return (ret);
}

/*
 *  ======== SNTP_getTimeByAddr ========
 */
int32_t SNTP_getTimeByAddr(SlNetSock_Addr_t *server, SlNetSock_Timeval_t *timeout,
        uint64_t *ntpTimeStamp)
{
    return (getTime(server, 0, timeout, ntpTimeStamp));
}
