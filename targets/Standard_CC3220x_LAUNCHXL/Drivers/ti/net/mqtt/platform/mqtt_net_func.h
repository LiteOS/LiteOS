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

#ifndef ti_net_mqtt_platform_MQTTNetFunc__include
#define ti_net_mqtt_platform_MQTTNetFunc__include

#include <ti/net/mqtt/client/client_core.h>         /* conn options */

#ifdef __cplusplus
extern "C" {
#endif

// MACRO to include receive time out feature
#define SOC_RCV_TIMEOUT_OPT     1

int32_t MQTTNet_commOpen(uint32_t nwconnOpts, const char *serverAddr, uint16_t portNumber,
                const MQTT_SecureConn_t *nwSecurity);
int32_t MQTTNet_tcpSend(int32_t comm, const uint8_t *buf, uint32_t len, void *ctx);
int32_t MQTTNet_tcpRecv(int32_t comm, uint8_t *buf, uint32_t len, uint32_t waitSecs, bool *timedOut,
              void *ctx);
int32_t MQTTNet_commClose(int32_t comm);
uint32_t MQTTNet_rtcSecs(void);

/*-----------------functions added for server -----------------------------*/

int32_t MQTTNet_tcpListen(uint32_t nwconnInfo,  uint16_t portNumber,
                const MQTT_SecureConn_t *nwSecurity);

int32_t MQTTNet_tcpSelect(int32_t *recvCvec, int32_t *sendCvec, int32_t *rsvdCvec,
                uint32_t waitSecs);


int32_t MQTTNet_tcpAccept(uint32_t nwconnInfo, int32_t listenHnd,
                uint8_t *clientIP, uint32_t *ipLen);

/*----------------- adding functions for udp functionalities -------------------*/

/** Send a UDP packet

    @param[in] comm communication entity handle; socket handle in this case
    @param[in] buf buf to be sent in the udp packet
    @param[in] len length of the buffer buf
    @param[in] destPort destination port number
    @param[in] destIP ip address of the destination in dot notation, interpreted as a string;
                Only IPV4 is supported currently.
    @param[in] ipLen length of string destIP; currently not used
    @return number of bytes sent or error returned by sl_SendTo call
*/
int32_t MQTTNet_sendTo(int32_t comm, const uint8_t *buf, uint32_t len, uint16_t destPort, const uint8_t *destIP, uint32_t ipLen);

/** Receive a UDP packet

    @param[in] comm communication entity handle; socket handle in this case
    @param[out] buf into which received UDP packet is written
    @param[in] maximum len length of the buffer buf
    @param[out] fromPort port number of UDP packet source
    @param[out] fromIP ip address of the UDP packet source.
                The ip address is to be interpreted as a uint32_t number in network byte ordering
                Only IPV4 is supported currently.
    @param[out] ipLen length of string fromIP;
               currently always populated with 4 as the address is a IPV4 address
    @return number of bytes received or error returned by sl_RecvFrom call
*/
int32_t MQTTNet_recvFrom(int32_t comm, uint8_t *buf, uint32_t len, uint16_t *fromPort, uint8_t *fromIP, uint32_t *ipLen);

#ifdef __cplusplus
}
#endif

#endif
