/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
#ifndef __OTA_HTTP_CLIENT_H__
#define __OTA_HTTP_CLIENT_H__

#ifdef    __cplusplus
extern "C" {
#endif

#define HTTP_MAX_RETRIES          2   /* Max retries to skip http headers */
#define HTTP_HEADER_STR           "HTTP/1.1 200"
#define HTTP_HEADER_MIN_SIZE      12     /* HTTP/x.x YYY */
#define HTTP_HEADER_OK            "200"  /* OK Code      */

#define SOCKET_SECURED            1
#define SOCKET_NON_SECURED        0
#define SOCKET_BLOCKING           0
#define SOCKET_NON_BLOCKING       1
#define SOCKET_PORT_DEFAULT       0
#define SOCKET_PORT_HTTP          80
#define SOCKET_PORT_HTTPS         443

#define MAX_EAGAIN_RETRIES        15

#define OTA_HTTP_CLIENT_STATUS_OK                       (0L)
#define OTA_HTTP_CLIENT_ERROR_CONNECT_GET_HOST_BY_NAME  (-20301L)
#define OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SOCKET         (-20302L)
#define OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SET_SOC_OPT    (-20303L)
#define OTA_HTTP_CLIENT_ERROR_CONNECT_SL_CONNECT        (-20304L)
#define OTA_HTTP_CLIENT_ERROR_DOUBLE_SLASH_NOT_FOUND    (-20305L)
#define OTA_HTTP_CLIENT_ERROR_END_SLASH_NOT_FOUND       (-20306L)
#define OTA_HTTP_CLIENT_ERROR_EMPTY_URL                 (-20307L)
#define OTA_HTTP_CLIENT_ERROR_RESP_HDR_SIZE             (-20308L)
#define OTA_HTTP_CLIENT_ERROR_RESP_HDR_END_NOT_FOUND    (-20309L)
#define OTA_HTTP_CLIENT_ERROR_RESP_STATUS_NOT_OK        (-20310L)
#define OTA_HTTP_CLIENT_ERROR_OFFSET_AND_SIZE           (-20311L)
#define OTA_HTTP_CLIENT_ERROR_RECV_MAX_EAGAIN_RETRIES   (-20312L)
#define OTA_HTTP_CLIENT_ERROR_RECV_LEN_0                (-20313L)

int16_t  HttpClient_Connect(uint8_t *ServerName, int32_t IpAddr, int32_t Port, int32_t Secured, int32_t NonBlocking);
int16_t  HttpClient_CloseConnection(int16_t SockId);
int16_t  HttpClient_Recv(int16_t SockId, void *pBuf, int16_t Len, int16_t Flags, int16_t MaxEagain);
int16_t  HttpClient_RecvAppend(int16_t SockId, uint8_t *pRecvBuf, int16_t RecvBufSize, int16_t RecvLen, int16_t RecvOffset);
int16_t HttpClient_RecvSkipHdr(int16_t SockId, uint8_t *pRespBuf, int16_t RespBufSize, uint32_t *contentLen);
int16_t  HttpClient_SendReq(int16_t SockId, uint8_t *pHttpReqBuf, uint8_t *pReqMethod, uint8_t *pServerName, uint8_t *pUriPrefix, uint8_t *pUriVal, uint8_t *pHdrName, uint8_t *pHdrVal);
uint8_t  *HttpClient_ParseUrl(uint8_t *pUrlName, uint8_t *PServerName, int16_t *Status);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __OTA_HTTP_CLIENT_H__ */
