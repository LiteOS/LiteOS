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
#include <ti/net/ota/otauser.h>
#include <ti/net/ota/source/OtaHttpClient.h>

#define CRLF_STR_LEN           (2)
#define DECIMAL_BASE           (10)
#define CONTENT_LENGTH_STR_LEN (14)

int16_t HttpClient_Connect(uint8_t *ServerName, int32_t IpAddr, int32_t Port, int32_t Secured, int32_t NonBlocking)
{
    int32_t Status;
    SlSockAddrIn_t  Addr;
    int32_t AddrSize;
    int16_t SockId = 0;
    uint32_t ServerIp;
    SlTimeval_t tTimeout;
    SlSocklinger_t linger;

    if (IpAddr == 0)
    {
        /* get host IP by name */
        Status = sl_NetAppDnsGetHostByName((int8_t *)ServerName, (uint16_t)strlen((const char *)ServerName), (unsigned long *)&ServerIp, SL_AF_INET);
        if(0 > Status )
        {
            _SlOtaLibTrace(("HttpClient_Connect: ERROR sl_NetAppDnsGetHostByName, status=%ld\r\n", Status));
            return OTA_HTTP_CLIENT_ERROR_CONNECT_GET_HOST_BY_NAME;
        }
    }
    else
    {
        ServerIp = IpAddr;
    }

    _SlOtaLibTrace(("HttpClient_Connect: IP_ADDR=%u.%u.%u.%u\r\n", ((char *)&ServerIp)[3]&0xff, ((char *)&ServerIp)[2]&0xff, ((char *)&ServerIp)[1]&0xff, ((char *)&ServerIp)[0]&0xff));

    if (Port == SOCKET_PORT_DEFAULT)
    {
        Port = Secured ? SOCKET_PORT_HTTPS : SOCKET_PORT_HTTP;
    }

    /* create socket */
    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons(((uint16_t)Port));
    Addr.sin_addr.s_addr = sl_Htonl(ServerIp);

    AddrSize = sizeof(SlSockAddrIn_t);

    SockId = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, (Secured? SL_SEC_SOCKET : 0));
    if( SockId < 0 )
    {
        _SlOtaLibTrace(("http_connect_server: ERROR Socket Open, status=%d\r\n", SockId));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SOCKET;
    }

    /* set recieve timeout */
    tTimeout.tv_sec=2;
    tTimeout.tv_usec=0;
    Status = sl_SetSockOpt(SockId, SL_SOL_SOCKET, SL_SO_RCVTIMEO, &tTimeout, sizeof(SlTimeval_t));
    if( Status < 0 )
    {
        sl_Close(SockId);
        _SlOtaLibTrace(("HttpClient_Connect: ERROR sl_SetSockOpt SL_SO_RCVTIMEO, status=%d\r\n", Status));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SET_SOC_OPT;
    }

    /* close the file socket with linger time of 1 seconds */
    /* It solves a case when the close socket failed after several seconds in the next OTA download retry */
    linger.l_onoff = 1;
    linger.l_linger = 1; /* one second */
    Status = sl_SetSockOpt(SockId, SL_SOL_SOCKET, SL_SO_LINGER, &linger, sizeof(linger));
    if( Status < 0 )
    {
        sl_Close(SockId);
        _SlOtaLibTrace(("HttpClient_Connect: ERROR sl_SetSockOpt SL_SO_LINGER, status=%d\r\n", Status));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SET_SOC_OPT;
    }

#ifdef OTA_SERVER_ROOT_CA_CERT
    /* Using the root ca for server authentication */
    Status = sl_SetSockOpt(SockId, SL_SOL_SOCKET, SL_SO_SECURE_FILES_CA_FILE_NAME, OTA_SERVER_ROOT_CA_CERT, strlen(OTA_SERVER_ROOT_CA_CERT));
    if( Status < 0 )
    {
        sl_Close(SockId);
        _SlOtaLibTrace(("HttpClient_Connect: ERROR sl_SetSockOpt SL_SO_SECURE_FILES_CA_FILE_NAME=%s, status=%d\r\n", OTA_SERVER_ROOT_CA_CERT, Status));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SET_SOC_OPT;
    }

    /* domain name verification */
    Status = sl_SetSockOpt(SockId, SL_SOL_SOCKET, SL_SO_SECURE_DOMAIN_NAME_VERIFICATION, (const char *)ServerName, strlen((const char *)ServerName));
    if( Status < 0 )
    {
        sl_Close(SockId);
        _SlOtaLibTrace(("HttpClient_Connect: ERROR sl_SetSockOpt SL_SO_SECURE_DOMAIN_NAME_VERIFICATION=%s, status=%d\r\n", ServerName, Status));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_SET_SOC_OPT;
    }
#endif

    /* connect to the server */
    Status = sl_Connect(SockId, ( SlSockAddr_t *)&Addr, (uint16_t)AddrSize);

#ifdef OTA_SERVER_ROOT_CA_CERT
#ifdef OTA_SERVER_AUTH_IGNORE_DATA_TIME_ERROR
    if (Status == SL_ERROR_BSD_ESECDATEERROR)
    {
        _SlOtaLibTrace(("HttpClient_Connect: WARNING Socket Connect, status=%ld, Ignored...\r\n", Status));
        /* Ignore error -461 - connected with certificate date verification error */ 
    }
    else
#endif
#ifdef OTA_SERVER_AUTH_DISABLE_CERT_STORE
    if (Status == SL_ERROR_BSD_ESECUNKNOWNROOTCA)
    {
        _SlOtaLibTrace(("HttpClient_Connect: WARNING Socket Connect, status=%ld, Ignored...\r\n", Status));
        /* Ignore error -468 - connected but the root CA used to validate the peer is unknown */ 
    }
    else
#endif
    if ((Status == SL_ERROR_BSD_ESECUNKNOWNROOTCA) || (Status == SL_ERROR_BSD_ESECDATEERROR))
    {
        _SlOtaLibTrace(("HttpClient_Connect: WARNING Socket Connect, status=%ld, Ignored...\r\n", Status));
        /* Ignore error -468 - connected but the root CA used to validate the peer is unknown */ 
        /* Ignore error -461 - connected with certificate date verification error */ 
    }
    else
#else
    if (Status == SL_ERROR_BSD_ESECSNOVERIFY)   
    {
        _SlOtaLibTrace(("HttpClient_Connect: WARNING Socket Connect, status=%ld, Ignored...\r\n", Status));
        /* OTA_SERVER_CERT is not defined Ignore error -453 - connected secure socket without server authentication */ 
    }
    else
#endif
    if ( Status < 0 )
    {
        sl_Close(SockId);
        _SlOtaLibTrace(("HttpClient_Connect: ERROR Socket Connect, status=%ld\r\n", Status));
        return OTA_HTTP_CLIENT_ERROR_CONNECT_SL_CONNECT;
    }

    /* handle non blocking */
    if (NonBlocking)
    {
        sl_SetSockOpt(SockId, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &NonBlocking, sizeof(NonBlocking));
    }

    return SockId;
}

int16_t HttpClient_CloseConnection(int16_t SockId)
{
    return (sl_Close(SockId));
}

/*
    parse url into domain name and uri_name, example
    https://dl.cloud_storage_provider_1.com/1/view/3ntw3xgkgselev9/Apps/SL_OTA/IlanbSmallFile.txt
*/
uint8_t *HttpClient_ParseUrl(uint8_t *pUrlName, uint8_t *pServerNameBuf, int16_t *Status)
{
    uint8_t *pBuf;
    int32_t len;

    *Status = 0;
    if (pUrlName == NULL)
    {
        _SlOtaLibTrace(("HttpClient_ParseUrl: Error url name\r\n"));
        *Status = OTA_HTTP_CLIENT_ERROR_EMPTY_URL;
        return NULL;
    }

    /* extract domanin name, extract only the domain name between // to the first / */
    /* example: https://dl.cloud_storage_provider_1.com/1/view/3ntw3xgkgselev9/Apps/SL_OTA/IlanbSmallFile.txt */
    pBuf = pUrlName;
    pBuf = (uint8_t *)strstr((const char *)pBuf, "//");
    if (pBuf == NULL)
    {
        *Status = OTA_HTTP_CLIENT_ERROR_DOUBLE_SLASH_NOT_FOUND;
        return NULL;
    }

    pBuf += 2;
    /* ends with / */
    len = (int32_t)strstr((const char *)pBuf, "/") - (int32_t)pBuf;
    if (len <= 0)
    {
        *Status = OTA_HTTP_CLIENT_ERROR_END_SLASH_NOT_FOUND;
        return NULL;
    }
    strncpy ((char *)pServerNameBuf, (const char *)pBuf, len);
    pServerNameBuf[len] = 0;
    pBuf += len;

    /* return the URI pointer */
    return pBuf;
}

/* HTTP Get stcuture:
    GET <URI> HTTP/1.1
    Host: <Domain>
    Authorization: Bearer <Access Token>
*/
int16_t HttpClient_SendReq(int16_t SockId, uint8_t *pHttpReqBuf, uint8_t *pReqMethod, uint8_t *pServerName, uint8_t *pUriPrefix, uint8_t *pUriVal, uint8_t *pHdrName, uint8_t *pHdrVal)
{
    int16_t Len;

    /* start with method GET/POST/PUT */
    strcpy((char *)pHttpReqBuf, (const char *)pReqMethod);

    /* fill uri_req_prefix */
    if (pUriPrefix && strlen((const char *)pUriPrefix))
    {
        strcat((char *)pHttpReqBuf, (const char *)pUriPrefix);
    }
    /* fill request URI */
    if (pUriVal && strlen((const char *)pUriVal))
    {
        strcat((char *)pHttpReqBuf, (const char *)pUriVal);
    }

    /* fill domain */
    strcat((char *)pHttpReqBuf, " HTTP/1.1\r\nhost: ");
    strcat((char *)pHttpReqBuf, (const char *)pServerName);
    strcat((char *)pHttpReqBuf, "\r\n");

    /* fill access_token */
    if (pHdrName && strlen((const char *)pHdrName))
    {
        strcat((char *)pHttpReqBuf, (const char *)pHdrName);
        strcat((char *)pHttpReqBuf, (const char *)pHdrVal);
        strcat((char *)pHttpReqBuf, "\r\n");
    }

    strcat((char *)pHttpReqBuf, "\r\n\0");


    /* Send the prepared request */
    Len = sl_Send(SockId, pHttpReqBuf, (int16_t)strlen((const char *)pHttpReqBuf), 0);

    return Len;
}

int16_t HttpClient_RecvSkipHdr(int16_t SockId, uint8_t *pRespBuf, int16_t RespBufSize, uint32_t *contentLen)
{
    int16_t Len;
    int32_t Retry=0;
    uint8_t *pBuf;
    uint8_t *pEndBuf;
    uint8_t *pEndOfContentLen;
    uint8_t *pStartOfContentLen;
    int16_t ProcessedBytes;

    Len = HttpClient_Recv(SockId, pRespBuf, RespBufSize, 0, MAX_EAGAIN_RETRIES);
    if (Len < 0)
    {
        _SlOtaLibTrace(("HttpClient_RecvSkipHdr: ERROR HttpClient_Recv, status=%ld\r\n", Len));
        return Len;
    }

    /* check place for "HTTP/1.1 200" */
    if (Len < HTTP_HEADER_MIN_SIZE)
    {
        _SlOtaLibTrace(("HttpClient_RecvSkipHdr: ERROR header len, len=%d\r\n", Len));
        return OTA_HTTP_CLIENT_ERROR_RESP_HDR_SIZE;
    }

    /* Check for HTTP success */
    if (strncmp (HTTP_HEADER_OK, (const char *)&pRespBuf[HTTP_HEADER_MIN_SIZE-3], 3))
    {
        pRespBuf[HTTP_HEADER_MIN_SIZE]=0; /* just for printing */
        _SlOtaLibTrace(("HttpClient_RecvSkipHdr: http error code %s\r\n", pRespBuf));
        return OTA_HTTP_CLIENT_ERROR_RESP_STATUS_NOT_OK;
    }
    pEndBuf = pRespBuf + Len;
    pBuf = &pRespBuf[HTTP_HEADER_MIN_SIZE];

    /* look for end of response header*/
    while (1)
    {
        while ((pEndBuf - pBuf) >= 4)
        {
            /* Search for CR+LF+CR+Lf */
            if (strncmp ((const char *)pBuf, "\r\n\r\n", 4) == 0)
            {
                int16_t NewSize;
                pBuf += 4;
                /* Now append some data more */
                ProcessedBytes = pBuf-pRespBuf;

                NewSize = Len-ProcessedBytes;
                memcpy(pRespBuf, pBuf, NewSize);
                memset(pRespBuf+NewSize, 0, ProcessedBytes);
                return NewSize;
            }

            /* Search for CR+LF */
            if (strncmp ((const char *)pBuf, "\r\n", CRLF_STR_LEN) == 0)
            {
                /* Check if the caller is interested in the value of "Content-Length" header */
                if (contentLen != NULL)
                {
                    /* At this point pBuf + CRLF length will be the beginning of an header */
                    /* Check if this header is "Content-Length" */
                    if (strncmp ((const char *)pBuf+CRLF_STR_LEN, "Content-Length", CONTENT_LENGTH_STR_LEN) == 0)
                    {
                        /* Found the content-length header, parse its value. */
                        /* HTTP header format is  field-name ":" [ field-value ] CR+LF */

                        /* Find the next CR+LF (end of the "Content-length" field value) */
                        /* Point to the beginning of the field value and search for the end of it (+1 is for skipping ':') */
                        pStartOfContentLen = pBuf + CRLF_STR_LEN + CONTENT_LENGTH_STR_LEN + 1;
                        pEndOfContentLen = pStartOfContentLen;

                        while (strncmp ((const char *)pEndOfContentLen, "\r\n", CRLF_STR_LEN) != 0)
                        {
                            pEndOfContentLen += 1;
                        }

                        /* convert the "content-length" value from string in decimal to int */
                        *contentLen = (int)strtol((const char *)pStartOfContentLen, (char **)&pEndOfContentLen, DECIMAL_BASE);
                    }
                }
            }

            pBuf+=1; /* skip only two bytes */
        }

        /* End of packet, try recv new packet */
        ProcessedBytes = pBuf-pRespBuf;
        Len = HttpClient_RecvAppend(SockId, pRespBuf, RespBufSize, Len, ProcessedBytes);
        if (Len <= 0)
        {
            _SlOtaLibTrace(("HttpClient_RecvSkipHdr: ERROR HttpClient_Recv, status=%ld\r\n", Len));
            return Len;
        }
        pEndBuf = pRespBuf + Len;
        pBuf = pRespBuf;

        if (Retry++ > HTTP_MAX_RETRIES)
        {
            _SlOtaLibTrace(("HttpClient_RecvSkipHdr: search end of header max retries\r\n"));
            return OTA_HTTP_CLIENT_ERROR_RESP_HDR_END_NOT_FOUND;
        }
    }
}

int16_t HttpClient_Recv(int16_t SockId, void *pBuf, int16_t BufLen, int16_t Flags, int16_t MaxEagain)
{
    int16_t Len;

    do
    {
        Len = sl_Recv(SockId, pBuf, BufLen, Flags);
        if (Len != SL_ERROR_BSD_EAGAIN)
        {
            break;
        }

    } while(--MaxEagain);

    if (Len == SL_ERROR_BSD_EAGAIN)
    {
        return OTA_HTTP_CLIENT_ERROR_RECV_MAX_EAGAIN_RETRIES;
    }
    if (Len == 0)
    {
        return OTA_HTTP_CLIENT_ERROR_RECV_LEN_0;
    }
    return Len;
}

int16_t HttpClient_RecvAppend(int16_t SockId, uint8_t *pRecvBuf, int16_t RecvBufSize, int16_t RecvLen, int16_t RecvOffset)
{
    int16_t UnprocessedSize = RecvLen - RecvOffset;
    int16_t BytesReceived;

    if ((UnprocessedSize < 0) || (RecvOffset  < 0))
    {
        _SlOtaLibTrace(("HttpClient_RecvAppend: ERROR RecvLen=%d, RecvOffset=%d\r\n", RecvLen, RecvOffset));
        return OTA_HTTP_CLIENT_ERROR_OFFSET_AND_SIZE;
    }

    /* copy the unprocessed part to the start of the buffer */
    if (UnprocessedSize > 0)
    {
        memcpy(&pRecvBuf[0], &pRecvBuf[RecvOffset], UnprocessedSize);
    }
    RecvLen = UnprocessedSize;

    /* read more data */
    memset(&pRecvBuf[UnprocessedSize], 0, RecvBufSize-UnprocessedSize);
    BytesReceived = HttpClient_Recv(SockId, &pRecvBuf[UnprocessedSize], RecvBufSize-UnprocessedSize, 0, MAX_EAGAIN_RETRIES); 
    if (0 >= BytesReceived)
    {
        _SlOtaLibTrace(("HttpClient_RecvAppend: ERROR HttpClient_Recv, status=%ld\r\n", BytesReceived));
        return BytesReceived;
    }
    RecvLen += BytesReceived;
    /* The buffer ihas reserved 1 byte for NULL */
    pRecvBuf[RecvLen] = 0;

    return RecvLen;
}

