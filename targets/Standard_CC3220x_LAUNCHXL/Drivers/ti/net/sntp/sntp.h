/*
 * Copyright (c) 2013-2018, Texas Instruments Incorporated
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
 *  ======== sntp.h ========
 */
/**
 *  @file  ti/net/sntp/sntp.h
 *
 *  @addtogroup ti_net_sntp_SNTP SNTP Client
 *
 *  @brief      The SNTP client provides APIs to synchronize the local time
 *              with a server that provides time synchronization
 *              services using Network Time Protocol (NTP).
 *
 *  There are 3 general use cases supported:
 *      1. You want to use default NTP servers (SNTP_getTime(), with a
 *         NULL server list)
 *      2. You have the string names of one or more NTP servers
 *         (SNTP_getTime(), with your server list)
 *      3. You have the sockaddr of the NTP server
 *         (SNTP_getTimeByAddr(), with your sockaddr)
 *
 *  The SNTP client service will return the seconds passed since
 *  January 1st 1900.
 *
 *  ## Important note on NTP protocol ##
 *
 *  The NTP protocol clearly specifies that an SNTP client must never
 *  send requests to an NTP server in intervals less than 15 seconds.
 *  It is important to respect this NTP requirement and it is the
 *  user's responsibility to ensure that SNTP_getTime() and/or
 *  SNTP_getTimeByAddr() are not called more than once, in any 15
 *  second time period, to contact the same NTP server.
 *
 *  ## Library Usage ##
 *
 *  To use the SNTP client APIs, the application should include its header file
 *  as follows:
 *  @code
 *  #include <ti/net/sntp/sntp.h>
 *  @endcode
 *
 *  And, add the following SNTP library to the link line:
 *  @code
 *  .../source/ti/net/sntp/{toolchain}/{isa}/sntp_{profile}.a
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_net_sntp_SNTP__include
#define ti_net_sntp_SNTP__include

/*! @ingroup ti_net_sntp_SNTP */
/*@{*/

#include <stdint.h>
#include <ti/net/slnetsock.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @name SNTP Error Codes
 *  @{
 */
/*!
 *  @brief Failed to resolve Host address
 */
#define SNTP_EGETHOSTBYNAMEFAIL     (-100)

/*!
 *  @brief Input arguments are invalid
 */
#define SNTP_EINVALIDARGS           (-101)

/*!
 *  @brief Failed to create a socket
 */
#define SNTP_ESOCKCREATEFAIL        (-102)

/*!
 *  @brief The input socket address is not of AF_INET (IPv4) or AF_INET6
 *         (IPv6) family type
 */
#define SNTP_EINVALIDFAMILY         (-103)

/*!
 *  @brief Failed to set receive timeout on socket
 */
#define SNTP_ESOCKOPTFAIL           (-104)

/*!
 *  @brief Failed to connect to the NTP server
 */
#define SNTP_ECONNECTFAIL           (-105)

/*!
 *  @brief Failed to send a time request to the NTP server
 */
#define SNTP_ESENDFAIL              (-106)

/*!
 *  @brief Failed to recieve the new time from the NTP server
 */
#define SNTP_ERECVFAIL              (-107)

/*!
 *  @brief NTP Server requests to reduce the update rate (RFC 5905 kiss code
 *         RATE)
 */
#define SNTP_ERATEBACKOFF           (-108)

/*!
 *  @brief NTP Server invalid or server requests to end all communications (RFC
 *         5905 kiss code DENY or RSTR)
 */
#define SNTP_EFATALNORETRY          (-109)

/*!
 *  @brief Server response with stratum == 0, with no kiss code
 */
#define SNTP_EINVALIDRESP           (-110)

/*! @} */

/*!
 *  @brief  Obtain the UTC time from NTP servers list
 *
 *  @param[in]  srvList      optional string array of server names
 *  @param[in]  srvCount     number of entities in @c srvList
 *  @param[in]  timeout      length of time to wait for NTP server reply
 *  @param[out] ntpTimeStamp time value from server in NTP timestamp format
 *
 *  @remark     @c srvList can be @c NULL to use the default list of NTP
 *              servers.  In this case, @c srvCount must be set to zero.
 *
 *  @remark     If @c srvList is non-NULL, @c srvCount must indicate the
 *              number of servers in the @c srvList array.
 *
 *  @remark     Both IPv4 and IPv6 unicast server addresses are supported
 *              (address is internally resolved).
 *
 *  @remark     If @c srvList is NULL and @c srvCount is zero, a default
 *              list of time servers will be used.
 *
 *  @remark     The caller is responsible for correctly setting @c timeout.
 *              If a value of NULL is passed, the default timeout behavior for
 *              the socket will apply.
 *
 *  @remark     @c ntpTimeStamp contains time values which match the NTP
 *              timestamp specified by RFC 4330. That is, the upper 32 bits
 *              will contain the number of seconds since January 1st 1900,
 *              and the lower 32 bits will contain the seconds fraction. Both
 *              values are unsigned entities.
 *
 *  @return     Success: 0
 *  @return     Failure: SNTP error
 *
 *  @par    Examples
 *  @code
 *  int32_t  retVal;
 *  uint32_t seconds;
 *  uint32_t secondsFraction;
 *  SlNetSock_Timeval_t timeval;
 *  uint64_t ntpTimeStamp = 0;
 *
 *  timeval.tv_sec  = 5;
 *  timeval.tv_usec = 0;
 *  retVal = SNTP_getTime(NULL, 0, &timeval, &ntpTimeStamp);
 *
 *  if (retVal == 0) {
 *      // The seconds value is stored in the upper 32 bits
 *      seconds = (0xFFFFFFFF00000000 & ntpTimeStamp) >> 32;
 *      // The seconds fraction is stored in the lower 32 bits
 *      secondsFraction = ntpTimeStamp;
 *  }
 *  @endcode
 */
int32_t SNTP_getTime(const char *srvList[], const uint32_t srvCount,
        SlNetSock_Timeval_t *timeout, uint64_t *ntpTimeStamp);

/*!
 *  @brief       Obtain the UTC time from NTP server address
 *
 *  @param[in]  server       IPv4 or IPv6 address of the NTP server
 *  @param[in]  timeout      length of time to wait for NTP server reply
 *  @param[out] ntpTimeStamp time value from server in NTP timestamp format
 *
 *  @remark     The server address can be either an IPv4 address
 *              (SlNetSock_AddrIn_t) or an IPv6 (SlNetSock_AddrIn6_t).
 *              The generic SlNetSock_Addr_t type of the @c server
 *              parameter is to allow either IPv4 or IPv6 structs to
 *              be provided.
 *
 *  @remark     The caller is responsible for correctly setting @c timeout.
 *              If a value of NULL is passed, the default timeout behavior for
 *              the socket will apply.
 *
 *  @return     Success: 0
 *  @return     Failure: SNTP error
 *
 *  @par    Examples
 *  @code
 *  int32_t  retVal;
 *  uint32_t seconds;
 *  uint32_t secondsFraction;
 *  SlNetSock_Timeval_t timeval;
 *  uint64_t ntpTimeStamp = 0;
 *  SlNetSock_AddrIn_t ipv4addr;
 *
 *  ipv4addr.sin_family = SLNETSOCK_AF_INET;
 *  ipv4addr.sin_port = SlNetUtil_htons(SNTP_PORT);
 *  ipv4addr.sin_addr.s_addr = SlNetUtil_htonl(addr);
 *
 *  timeval.tv_sec  = 5;
 *  timeval.tv_usec = 0;
 *  retVal = SNTP_getTimeByAddr((SlNetSock_Addr_t *)&ipv4addr, &timeval,
 *          &ntpTimeStamp);
 *
 *  if (retVal == 0) {
 *      // The seconds value is stored in the upper 32 bits
 *      seconds = (0xFFFFFFFF00000000 & ntpTimeStamp) >> 32;
 *      // The seconds fraction is stored in the lower 32 bits
 *      secondsFraction = ntpTimeStamp;
 *  }
 *  @endcode
 */
int32_t SNTP_getTimeByAddr(SlNetSock_Addr_t *server,
        SlNetSock_Timeval_t *timeout, uint64_t *ntpTimeStamp);

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
