/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
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

#include <ti/net/slneterr.h>
#include <ti/net/slnetutils.h>

#include "httpclient_internal.h"

static struct SlNetSock_Addr_t proxyAddr = {0};


static const char *HTTPStr_VER1_1      = "HTTP/1.1";
static const char *HTTPStr_VER1_0      = "HTTP/1.0";

/* HTTP Request Methods */
const char *HTTP_METHOD_GET     = "GET";
const char *HTTP_METHOD_POST    = "POST";
const char *HTTP_METHOD_HEAD    = "HEAD";
const char *HTTP_METHOD_OPTIONS = "OPTIONS";
const char *HTTP_METHOD_PUT     = "PUT";
const char *HTTP_METHOD_DELETE  = "DELETE";
const char *HTTP_METHOD_CONNECT = "CONNECT";

static const char * headerFieldStr[] = {
"Age",
"Allow",
"Cache-Control",
"Connection",
"Content-Encoding",
"Content-Language",
"Content-Length",
"Content-Location",
"Content-Range",
"Content-Type",
"Date",
"ETag",
"Expires",
"Last-Modified",
"Location",
"Proxy-Authenticate",
"Retry-After",
"Server",
"Set-Cookie",
"Trailer",
"Transfer-Encoding",
"Upgrade",
"Vary",
"Via",
"Www-Authenticate",
"Warning",
"Accept",
"Accept-Charset",
"Accept-Encoding",
"Accept-Language",
"Authorization",
"Cookie",
"Expect",
"Forwarded",
"From",
"Host",
"If-Match",
"If-Modified-Since",
"If-None-Match",
"If-Range",
"If-Unmodified-Since",
"Origin",
"Proxy-Authorization",
"Range",
"TE",
"User-Agent",
};

static uint8_t HeadersHashTable[MAX_HASH_TABLE_SIZE] = {
15,1,0,7,0,0,0,0,0,0,0,20,0,0,0,0,0,0,
23,24,0,0,0,0,0,0,0,0,6,0,0,0,10,0,0,11,
0,0,0,16,0,17,21,0,0,0,0,0,0,0,3,13,2,19,
0,0,25,0,0,0,0,0,5,0,0,4,12,0,22,18,0,8,0,
0,0,14,9,0,0,0,
};

/* Prototype for local functions */
static int16_t httpProxyTunnel(HTTPClient_CB *cli, const char *hostName);
static int16_t getStatus(HTTPClient_CB *cli);
static int16_t readLine(HTTPClient_CB *cli, char *line, int len, bool *moreFlag, uint32_t buffereddBodyLen);
static int16_t sprsend(HTTPClient_CB *cli, uint8_t flags, const char * fmt, ...);
static int16_t startSecureMode(HTTPClient_CB * cli, const char *domain,HTTPClient_extSecParams *exSecParams);
static int16_t sendReqHeaders(HTTPClient_CB *cli,uint32_t name ,uint32_t flags);
static int16_t manageResponseHeaders(HTTPClient_CB *cli, uint8_t *flags);
static void clearReqHeaders(HTTPClient_CB *cli, bool persistent);
static void clearResponseValues(HTTPClient_CB *cli);
static int16_t countChar(const char *str, char c);
static int16_t parseURI(const char *uri, uint16_t *portOutput,char *domainOutput, uint16_t *flags);
static int8_t resHeaderNameToHash(const char *name);
static bool validateResponseHeader(int8_t resNum, const char *resHeader);
static void skipWhiteSpace(char **str);
static int16_t resHeaderFieldHandler(HTTPClient_CB *cli, char *name, char *value);
static int16_t removeRequestHeader(HTTPClient_CB *cli, uint32_t header, uint32_t flag);
static Req_HField *searchRequestHeader(HTTPClient_CB *cli, uint32_t reqHeader , bool persistent);
static int16_t redirect(HTTPClient_CB *cli, int16_t status, const char *method);
static int16_t locationParser(HTTPClient_CB *cli,const char *uri, uint8_t *flags);
static int16_t readRawResponseBody(HTTPClient_CB *cli, char *body, uint32_t len);
static int16_t getChunkedData(HTTPClient_CB *cli, char *body, uint32_t len,bool *moreFlag);
static int16_t sendRequest(HTTPClient_CB *cli, const char *method,const char *requestURI, const char *body, uint32_t bodyLen, uint32_t flags);
static int16_t readResponseHeader(HTTPClient_CB *cli, char *header, uint8_t *moreFlag);
static void clearNonPersistResponseFilter(HTTPClient_CB * cli);
static int16_t stringncasecmp(const char *s1, const char *s2, size_t n);
static int16_t stringcasecmp (const char *s1, const char *s2);

static int16_t SlNetSock_connectURI(int16_t *sd, const char *URI, SlNetSockSecAttrib_t *secAttrib, uint32_t ifBitmap, int16_t *flags);
//*****************************************************************************
//
// SlNetSock_connectURI - Initiate a connection on a socket by URL
// Temporary implemantaion until pushed into SlNetSock.
//*****************************************************************************
static int16_t SlNetSock_connectURI(int16_t *sd, const char *URI, SlNetSockSecAttrib_t *secAttrib, uint32_t ifBitmap, int16_t *flags)
{
    uint32_t          addr[4];
    uint16_t          ipAddrLen = 1;
    SlNetSock_Addr_t  serverAddr;
    SlNetSocklen_t    serverAddrSize;
    int32_t           retVal;
    char              domainBuff[HTTPClient_DOMAIN_BUFLEN];
    char             *pDomainBuff    = domainBuff;
    uint16_t          domainBuffLen  = HTTPClient_DOMAIN_BUFLEN;
    uint16_t          port           = 0;
    int16_t           virtualSd;
    uint16_t          URIFlags;

    if (NULL != flags)
    {
        *flags &= ~ (CONNECTURI_PARSER_FLAGS);
    }

    /* Parse the URI input into port number, address (Domain, IPv4 or IPv6)
       and flags which represent the address type and if security is needed */
    retVal = parseURI(URI, &port,domainBuff, &URIFlags);

    if (SLNETERR_RET_CODE_OK != retVal)
    {
        return SLNETERR_RET_CODE_INVALID_INPUT;
    }

    /* Check if sd provided, if so retrieve related parameters */
    if (NULL != sd)
    {
        /* When sd is provided we can know the ifID so we can update the
           ifBitmap accordingly */
        retVal = SlNetSock_getIfID(*sd);
        if (SLNETERR_RET_CODE_OK > retVal)
        {
            return retVal;
        }
        ifBitmap = retVal;
        virtualSd = *sd;
    }

    /* Check if the parsed address is not IPv6 address */
    if (0 == (ISIPV6 & URIFlags))
    {
        /* Check if the address is IPv4 and return its IP */
        retVal = SlNetUtil_getHostByName(ifBitmap, domainBuff, strlen(domainBuff), addr, &ipAddrLen, SLNETSOCK_AF_INET);

        if (retVal >= 0)
        {
            /* fill the answer fields with IPv4 parameters */
            URIFlags &= ~ISIPV6;

            /* fill the answer fields with IPv4 parameters */
            serverAddrSize = sizeof(SlNetSock_AddrIn_t);
            ((SlNetSock_AddrIn_t *)&serverAddr)->sin_family      = SLNETSOCK_AF_INET;
            ((SlNetSock_AddrIn_t *)&serverAddr)->sin_port        = SlNetUtil_htons(port);
            ((SlNetSock_AddrIn_t *)&serverAddr)->sin_addr.s_addr = SlNetUtil_htonl(addr[0]);
        }
    }
    /* Check if the previous SlNetUtil_getHostByName failed */
    if ( (retVal < 0) || (0 != (ISIPV6 & URIFlags)) )
    {
        /* Check if the parsed address is IPv6 address */
        if(0 != (ISIPV6 & URIFlags))
        {
            /* When using IPv6, the domainBuff contain the characters
               "[" at the top of the string and "]" at the end of the
               string, the following lines remove those characters in
               order to use this parameter as a clean IPv6 string */
            domainBuffLen = strlen(domainBuff);
            if (domainBuffLen < 2)
            {
                return SLNETERR_RET_CODE_INVALID_INPUT;
            }
            pDomainBuff++;
            pDomainBuff[domainBuffLen - 2] = '\0';
            /* Use Pton in order to retrive the binary address from the string */
            retVal = SlNetUtil_inetPton(SLNETSOCK_AF_INET6, pDomainBuff, addr);

        }
        else
        {
            URIFlags |= ISDOMAIN;
            retVal = SlNetUtil_getHostByName(ifBitmap, pDomainBuff, strlen(pDomainBuff), addr, &ipAddrLen, SLNETSOCK_AF_INET6);
        }


        /* URI isn't IPv6 domain or IPv6 address, return error code */
        if(retVal < 0)
        {
            /* if the request failed twice, return error code. */
            return retVal;
        }
        else
        {
            /* fill the answer fields with IPv6 parameters */
            URIFlags |= ISIPV6;

            /* fill the answer fields with IPv6 parameters */
            serverAddrSize = sizeof(SlNetSock_AddrIn6_t);
            ((SlNetSock_AddrIn6_t *)&serverAddr)->sin6_family = SLNETSOCK_AF_INET6;
            ((SlNetSock_AddrIn6_t *)&serverAddr)->sin6_port   = SlNetUtil_htons(port);
            memcpy(((SlNetSock_AddrIn6_t *)&serverAddr)->sin6_addr._S6_un._S6_u32, addr, 16);
        }
    }
    /* The return value of the getHostByName function is the ifID of the
       interface which answered to the request */
    ifBitmap = retVal;

    /* When sd isn't provided, create one */
    if (NULL == sd)
    {
        retVal = SlNetSock_create(serverAddr.sa_family, SLNETSOCK_SOCK_STREAM, SLNETSOCK_PROTO_TCP, ifBitmap, 0);
        virtualSd = retVal;
    }

    if (0 <= virtualSd)
    {

        retVal = 0;

        /* Check if the requested URI is secured and if the security
           attributes has been provided, if so bind the security
           attributes                                                    */
        if ( (0 != (ISSECURED & URIFlags)) && (NULL != secAttrib) )
        {
            /* Function exists in the interface of the socket
               descriptor, dispatch the startSec command             */
            retVal = SlNetSock_startSec(virtualSd, secAttrib, SLNETSOCK_SEC_BIND_CONTEXT_ONLY);
        }
        if (SLNETERR_RET_CODE_OK == retVal)
        {
            /* Function exists in the interface of the socket descriptor,
               dispatch the Connect command                              */
            retVal = SlNetSock_connect(virtualSd, (const SlNetSock_Addr_t *)&serverAddr, serverAddrSize);
        }
        else
        {
            /* Non mandatory function doesn't exists, return error code  */
            retVal = SLNETERR_RET_CODE_DOESNT_SUPPORT_NON_MANDATORY_FXN;
        }
    }

    /* If a error found and the function created socket, we need to close it */
    if (SLNETERR_RET_CODE_OK != retVal)
    {
        if ( (NULL == sd) && (0 <= virtualSd) )
        {
            SlNetSock_close(virtualSd);
        }
        if (NULL != flags)
        {
            *flags &= ~TLSSTARTED;
        }
        virtualSd = retVal;
    }
    else if (NULL != flags)
    {
        *flags |= URIFlags;
    }
    return virtualSd;
}



/*
 *  ======== getCliState ========
 *  Get the state of HTTP client instance for the input flag
 */
static inline bool getCliState(HTTPClient_CB *cli, uint32_t flag)
{
    return ((cli->state) & flag);
}

/*
 *  ======== stringcasecmp ========
 *  Compare two strings ignoring case.
 */

static int16_t stringcasecmp (const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int16_t result;

    if (p1 == p2)
    {
        return 0;
    }

    while ((result = tolower(*p1) - tolower(*p2++)) == 0)
    {
        if (*p1++ == '\0')
        {
            break;
        }
    }
    return result;
}

/*
 *  ======== stringncasecmp ========
 *  Compare two strings ignoring case.
 *    no null terminated required.
 */
static int16_t stringncasecmp(const char *s1, const char *s2, size_t n)
{
    int16_t   compareVal;

    if (n != 0)
    {
        do {
            compareVal = tolower(*s1) - tolower(*s2);
            if (compareVal != 0)
            {
                return (compareVal);
            }
            ++s1;
            ++s2;
            if (*s1 == '\0')
            {
                break;
            }
        } while (--n != 0);
    }
    return (0);
}

/*
 *  ======== setCliState ========
 *  Set or clear the flag in the HTTP client instance
 */
static inline void setCliState(HTTPClient_CB *cli, uint32_t flag, bool value)
{
    if (value)
    {
        cli->state |= flag;
    }
    else
    {
        cli->state &= ~flag;
    }
}

/*
 *  ======== isRedirect ========
 *  Check if the HTTP status code is redirectional codes
 */
static inline int isRedirect(int16_t status)
{
    return (((status >= HTTP_SC_MOVED_PERMANENTLY) && (status < HTTP_SC_SEE_OTHER)) || (status == HTTP_SC_TEMPORARY_REDIRECT));
}

/*
 *  ======== locationParser ========
 *  Parses the value of the location header for the redirect handler.
 */
static int16_t locationParser(HTTPClient_CB * cli,const char *uri, uint8_t *flags)
{
    uint16_t port = 0;
    int16_t ret;
    uint16_t uriFlags = 0;
    Req_HField *currHost;
    char     domainBuff[HTTPClient_DOMAIN_BUFLEN];

    if (uri == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    if (*uri == '/')
    {
        return(0);
    }
    ret = parseURI(uri,&port,domainBuff,&uriFlags);
    if (ret < 0)
    {
        return(ret);
    }
    /* Downgrade tls request by redirect */
    if ((!(uriFlags & ISSECURED)) && (getCliState(cli,ISSECURED_STATE)))
    {
        if (!getCliState(cli,ENABLE_TLS_DOWNGRADE_STATE))
        {
            return(HTTPClient_ETLSDOWNGRADEISFORBIDDEN);
        }
        else
        {
            *flags |= HOST_CHANGED;
        }
    }  /* Upgrade tls request by redirect */
    else if ((uriFlags & ISSECURED) && (!getCliState(cli,ISSECURED_STATE)))
    {
        *flags |= HOST_CHANGED;
    }
    currHost = searchRequestHeader(cli,HTTPClient_HFIELD_REQ_HOST,true);
    if (currHost)
    {
        if(strstr(uri,currHost->Value) == NULL)
        {
            *flags |= HOST_CHANGED;
        }
    }
    return(0);
}

/*
 *  ======== getChunkedData ========
 *  Get chucked transfer encoded data
 */
static int16_t getChunkedData(HTTPClient_CB *cli, char *body, uint32_t len, bool *moreFlag)
{
    bool mFlag = false;
    char crlf;
    int16_t ret = 0;
    uint32_t chunkLen;
    uint32_t buffereddBodyLen = cli->validBufEnd - cli->validBufStart;

    *moreFlag = true;

    /* Check if in the middle of reading response body? */
    chunkLen = cli->bodyLen;
    if (chunkLen == 0)
    {
        ret = readLine(cli, body, len, &mFlag, buffereddBodyLen);
        if (ret < 0)
        {
            return (ret);
        }
        /* Get current chunk length */
        chunkLen = strtoul(body, NULL, 16);
        /* don't need the remaining buffer */
        if (mFlag)
        {
            ret = readLine(cli, body, len, &mFlag , buffereddBodyLen);
            if (ret < 0)
            {
                return (ret);
            }
        }

        if (chunkLen == 0)
        {
            /* skip the rest (trailer headers) */
            if (getCliState(cli,TRAILER_STATE))
            {
                do
                {
                    cli->validBufStart = cli->buf;
                    cli->validBufEnd = cli->buf;
                    ret = SlNetSock_recv(cli->ssock,cli->validBufStart,HTTPClient_BUF_LEN - 1, 0);
                    if (ret < 0)
                    {
                        return (ret);
                    }
                    cli->validBufEnd += ret;
                    *cli->validBufEnd = '\0';
                }while(strstr(cli->validBufStart,HTTPClient_crlfcrlf) == NULL);
                setCliState(cli,TRAILER_STATE,false);
            }
            ret = readLine(cli, &crlf, sizeof(crlf), &mFlag, buffereddBodyLen);
            if (ret < 0)
            {
                return (ret);
            }
            *moreFlag = false;
            setCliState(cli,CHUNKED_STATE,false);
            cli->bodyLen = 0;

            return (0);
        }

        cli->bodyLen = chunkLen;
    }

    if (chunkLen < len)
    {
        len = chunkLen;
    }

    ret = readRawResponseBody(cli, body, len);
    if (ret > 0)
    {
        cli->bodyLen -= ret;
        /* Clean up the CRLF */
        if (cli->bodyLen == 0)
        {
            buffereddBodyLen = cli->validBufEnd - cli->validBufStart;
            chunkLen = readLine(cli, &crlf, sizeof(crlf), &mFlag, buffereddBodyLen);
            if (chunkLen != 0)
            {
                return (chunkLen);
            }
        }
    }
    return (ret);
}

/*
 *  ======== readRawResponseBody ========
 */
static int16_t readRawResponseBody(HTTPClient_CB *cli, char *body, uint32_t len)
{
    int16_t ret = 0;
    uint32_t buffereddBodyLen = cli->validBufEnd - cli->validBufStart;

    if(buffereddBodyLen > 0)
    {
        if (buffereddBodyLen > len)
        {
            memcpy(body, cli->validBufStart, len);
            cli->validBufStart += len;
            return (len);
        }
        else
        {
            memcpy(body, cli->validBufStart, buffereddBodyLen);
            cli->validBufStart += buffereddBodyLen;
        }
    }
    ret = SlNetSock_recv(cli->ssock, body + buffereddBodyLen, len - buffereddBodyLen, 0);
    if (ret < 0)
    {
        return(ret);
    }
    return (ret + buffereddBodyLen);
}

/*
 *  ======== redirect ========
 *  redirect handler
 */

static int16_t redirect(HTTPClient_CB *cli, int16_t status, const char *method)
{

    char uri[HTTPClient_DOMAIN_BUFLEN] = {0};
    char buf[HTTPClient_BUF_LEN] = {0};
    uint32_t uriSize = HTTPClient_DOMAIN_BUFLEN;
    uint8_t flags = 0;
    bool moreFlag;
    int16_t ret;
    if ( (strcmp(method,HTTP_METHOD_GET) == 0) || (strcmp(method,HTTP_METHOD_HEAD) == 0) || (status == HTTP_SC_SEE_OTHER) )
    {
        /* If the used method is HEAD, the server won't send body even if the content-length header is present */
        if (strcmp(method,HTTP_METHOD_HEAD) != 0)
        {
            /* Flush the body of the response if exist */
            do
            {
                ret = HTTPClient_readResponseBody(cli, buf,HTTPClient_BUF_LEN,&moreFlag);
                if (ret < 0)
                {
                    return (ret);
                }
            }while ((ret > 0) && moreFlag);
        }
        else
        {
            cli->bodyLen = 0;
        }
        ret = HTTPClient_getHeader(cli,HTTPClient_HFIELD_RES_LOCATION,uri,&uriSize,0);
        if (ret < 0)
        {
            return (ret);
        }

        ret = locationParser(cli,uri,&flags);
        if (ret < 0)
        {
            return (ret);
        }
    }
    else
    {
        return (HTTPClient_ERESPONSEISNOTREDIRECTABLE);
    }
    if (flags & HOST_CHANGED)
    {
        /* Clears previous host */
        ret = HTTPClient_setHeader(cli,HTTPClient_HFIELD_REQ_HOST,NULL,0,HTTPClient_HFIELD_PERSISTENT);
        if (ret < 0)
        {
            return (ret);
        }
        SlNetSock_close(cli->ssock);
        setCliState(cli,CONNECTED_STATE,false);
        ret = HTTPClient_connect(cli,uri,NULL,0);
        if (ret < 0)
        {
            return (ret);
        }
    }
    cli->validBufEnd = cli->buf;
    cli->validBufStart = cli->buf;
    switch (status)
    {
        case HTTP_SC_MOVED_PERMANENTLY:
        case HTTP_SC_FOUND:
        case HTTP_SC_TEMPORARY_REDIRECT:
        case HTTP_SC_SEE_OTHER:
            if (status == HTTP_SC_SEE_OTHER)
            {
                ret = sprsend(cli, 0, "%s %s %s\r\n", HTTP_METHOD_GET, uri,HTTPStr_VER1_1);
            }
            else
            {
                ret = sprsend(cli, 0, "%s %s %s\r\n", method, uri,HTTPStr_VER1_1);
            }
            if (ret < 0)
            {
                return (ret);
            }
            break;
    }
    ret = sendReqHeaders(cli,0,HTTPClient_HFIELD_PERSISTENT);
    if (ret < 0)
    {
        return (ret);
    }
    ret = sendReqHeaders(cli,0,HTTPClient_HFIELD_NOT_PERSISTENT);
    if (ret < 0)
    {
        return (ret);
    }
    /* End of headers section */
    ret = sprsend(cli, 0, "\r\n");
    if (ret < 0)
    {
        return (ret);
    }
    ret = sprsend(cli, SENDBUF, NULL);
    if (ret < 0)
    {
        return (ret);
    }
    return (0);
}

/*
 *  ======== validateResponseHeader ========
 */
static bool validateResponseHeader(int8_t resNum, const char *resHeader)
{
    return (stringcasecmp(headerFieldStr[resNum],resHeader) == 0);
}

/*
 *  ======== skipWhiteSpace ========
 *  Skip white spaces
 */
static void skipWhiteSpace(char **str)
{
    while(**str == ' ')
    {
        *str = (*str) + 1;
    }
}

/*
 *  ======== resHeaderNameToHash ========
 *  This funcion resolves response header name into enum
 */
static int8_t resHeaderNameToHash(const char *name)
{
    uint8_t i,nameLen;
    uint32_t sum = 0;
    char tempName[MAX_HEADER_LEN + 1] = {0};

    nameLen = strlen (name);
    if (nameLen > MAX_HEADER_LEN)
    {
        return(HEADER_NOT_SUPPORTED);
    }
    strcpy(tempName,name);
    for(i = 0;i < nameLen;i++)
    {
        if((name[i]>='A') && (name[i]<'Z'))
        {
          /* Switch upper case to lower case */
          tempName[i] = name[i]|0x60;
        }
    }

    /* Sum the ascii values of the header names */
    for (i = 0; i < nameLen; i++)
    {
        sum += tempName[i];
    }

    /* Sum the rest of the length until we get to the maximum length */
    for (; i < MAX_HEADER_LEN; i++)
    {
        sum++;
    }

    sum+=nameLen;

    sum = sum % MAX_HASH_TABLE_SIZE;

    if(sum == 52 && tempName[0] == 'l')
    {
        sum--;
    }
    return HeadersHashTable[sum];
}

/*
 *  ======== countChar ========
 *  return count of the given character in the string
 */
static int16_t countChar(const char *str, char c)
{
    const char * tmp = str;
    uint8_t count = 0;

    while( *tmp != '\0' )
    {
         if (*tmp == c)
            count++;
        tmp++;
    }
    return count;
}


/*
 *  ======== clearResponseValues ========
 *  Clears the buffer and the Map which stores the response values.
 */

static void clearResponseValues(HTTPClient_CB *cli)
{
    if ((cli->responseHFieldValue != NULL) && (cli->responseHFieldIterator != cli->responseHFieldValue))
    {
        memset(cli->responseHFieldValue,0,HTTPClient_RES_HFIELD_BUFFER_SIZE);
        memset(cli->responseHeaderMap,0,sizeof(uint32_t) * HTTPClient_MAX_RESPONSE_HEADER_FILEDS);
        cli->responseHFieldIterator = cli->responseHFieldValue;
        cli->responseHFieldFreeSize = HTTPClient_RES_HFIELD_BUFFER_SIZE;
    }
}

/*
 *  ======== clearNonPersistResponseFilter ========
 *  clear from the response filter the non persistent responses
 */

static void clearNonPersistResponseFilter(HTTPClient_CB *cli)
{
    /* Check for filter existence */
    if ((~(cli->responseHFieldFilter)) & RESPONSE_HFIELDS_ENABLED)
    {
        /* In case a filter exists, keep only the persistent one on */
        cli->responseHFieldFilter &= cli->responseHFieldFilterPersist;
    }
}
/*
 *  ======== removeRequestHeader ========
 *  Removes specific request header
 */

static int16_t removeRequestHeader(HTTPClient_CB *cli, uint32_t header, uint32_t flag)
{
    Req_HField ** head;

    if (flag & HTTPClient_HFIELD_PERSISTENT)
    {
        head = &(cli->reqHFieldPers);
    }
    else
    {
        head = &(cli->reqHField);
    }

    if ((*head)->Name == header)
    {
        Req_HField *temp = *head;
        *head = (*head)->Next;
        free(temp->Value);
        free(temp);
        return(0);
    }

    Req_HField *current = (*head)->Next;
    Req_HField *previous = *head;
    while (current != NULL && previous != NULL)
    {
        if (current->Name == header)
        {
            Req_HField * temp = current;
            previous->Next = current->Next;
            free(temp->Value);
            free(temp);
            return(0);
        }
        previous = current;
        current = current->Next;
    }

    return(HTTPClient_EREQHEADERNOTFOUND);
}
/*
 *  ======== searchRequestHeader ========
 *  Searches a specific reqHeader, if found return its pointer else NULL.
 */
static Req_HField *searchRequestHeader(HTTPClient_CB *cli, uint32_t reqHeader , bool persistent)
{
    Req_HField *head;

    if (persistent)
    {
        head = cli->reqHFieldPers;
    }
    else
    {
        head = cli->reqHField;
    }

    while (head != NULL)
    {
        if (head->Name == reqHeader)
        {
            return(head);
        }
        head = head->Next;
    }

    return(NULL);
}

/*
 *  ======== clearReqHeaders ========
 *  Clears one of the request headers list (persistent or not) according to the persistent parameter
 */
static void clearReqHeaders(HTTPClient_CB *cli, bool persistent)
{
    Req_HField *temp ,*head;

    if (persistent)
    {
        head = cli->reqHFieldPers;
    }
    else
    {
        head = cli->reqHField;
    }
    while (head != NULL)
    {
        temp = head;
        head = head->Next;
        if (temp->CustomName)
        {
            free(temp->CustomName);
        }
        free(temp->Value);
        free(temp);
    }
    if (persistent)
    {
        cli->reqHFieldPers = NULL;
    }
    else
    {
        cli->reqHField = NULL;
    }
}

/*
 *  ======== resHeaderFieldHandler ========
 *  Handling some of the response header-fields.
 */
static int16_t resHeaderFieldHandler(HTTPClient_CB *cli, char *name, char *value)
{
    const char chunk[] = "chunked";
    /* Checks is the body is chunked */
    if ((stringcasecmp(name, HeaderToStr(HTTPClient_HFIELD_RES_TRANSFER_ENCODING)) == 0) && (stringcasecmp(chunk, value) == 0))
    {
        setCliState(cli, CHUNKED_STATE, true);
    }
    /* Retrieve body length , when it's not chunked */
    else if (stringcasecmp(name, HeaderToStr(HTTPClient_HFIELD_RES_CONTENT_LENGTH)) == 0)
    {
        cli->bodyLen = strtoul(value, NULL, 10);
    }
    /* Recieving "Connection : close" means the connection will be closed after the response */
    else if ((stringcasecmp(name, HeaderToStr(HTTPClient_HFIELD_RES_CONNECTION)) == 0) && (stringcasecmp("close", value) == 0))
    {
        setCliState(cli, CONNECTED_STATE, false);
    }
    /* Recieving "Connection : keep-alive" means the connection will remain open after the response */
    else if ((stringcasecmp(name, HeaderToStr(HTTPClient_HFIELD_RES_CONNECTION)) == 0) && (stringcasecmp("keep-alive", value) == 0))
    {
        setCliState(cli, CONNECTED_STATE, true);
    }
    else if (stringcasecmp(name, HeaderToStr(HTTPClient_HFIELD_RES_TRAILER)) == 0)
    {
        setCliState(cli, TRAILER_STATE, true);
    }
    return (0);
}

/*
 *  ======== readResponseHeader ========
 *    Read and manage Header-fields
 */
static int16_t readResponseHeader(HTTPClient_CB *cli, char *header, uint8_t *moreFlag)
{
    char * eol = NULL; /* End of header field line */
    char * eoh = NULL; /* End of all header fields section */
    uint32_t notSentLen = 0;
    uint32_t currHeaderLen = 0;

    if (!((cli != NULL) && (header != NULL) && (moreFlag != NULL)))
    {
        return (HTTPClient_ENULLPOINTER);
    }

    *moreFlag = 0;

    if (getCliState(cli,HEADER_INCOMPLETE_STATE))
    {
        eoh = strstr(cli->validBufStart,HTTPClient_crlfcrlf);
        eol = strstr(cli->validBufStart,HTTPClient_crlf);
        if (eol && (eoh== NULL))
        {
            /* Clear the remaining of incomplete header-field */
            cli->validBufStart = eol + strlen(HTTPClient_crlf);
        }
        else if (eoh)
        {
            /* The last header in the buffer is larger than buffer */
            cli->validBufStart = eol + strlen(HTTPClient_crlfcrlf);
            setCliState(cli,HEADER_INCOMPLETE_STATE,false);
            return(LAST_HEADER_INCOMPLETE);
        }
        else if (eoh == NULL && eol == NULL)
        {
            /* After getting more data, the header and its value are not over yet */
            cli->validBufEnd = cli->buf;
            *moreFlag |= RECV_DATA;
            return(CONTINUE_AND_PULL);
        }
        setCliState(cli,HEADER_INCOMPLETE_STATE,false);
    }

    eoh = strstr(cli->validBufStart,HTTPClient_crlfcrlf);
    eol = strstr(cli->validBufStart,HTTPClient_crlf);
    if ((eoh) && (eol))
    {
        /* Current buffer have last header in it */
        currHeaderLen = eol - cli->validBufStart;
        if (eoh == eol)
        {
            /* The last header is the only one in the buffer */
            *eoh = '\0';
            memcpy(header,cli->validBufStart,currHeaderLen + 1);
            cli->validBufStart = eoh + strlen(HTTPClient_crlfcrlf);
            return(NO_MORE_HEADERS);
        }
        else if (eoh > eol)
        {
            /* The buffer include the last header-field and other previous header-fields */
            *eol = '\0';
            memcpy(header,cli->validBufStart,currHeaderLen + 1);
            cli->validBufStart = eol + strlen(HTTPClient_crlf);
            return(PARSE_AND_CONTINUE);
        }
    }
    else if (eol && (eoh == NULL))
    {
        /* Last header-field is not included in the buffer */
        currHeaderLen = eol - cli->validBufStart;
        *eol = '\0';
        memcpy(header,cli->validBufStart,currHeaderLen + 1);
        cli->validBufStart = eol + strlen(HTTPClient_crlf);
        return(PARSE_AND_CONTINUE);
    }
    else if ((eol == NULL) && (eoh == NULL))
    {
        if (cli->validBufStart == cli->buf)
        {
            /* This section means that the Header and its value are larger than buffer */
            memcpy(header,cli->validBufStart,HTTPClient_BUF_LEN);
            *(header + HTTPClient_BUF_LEN - 1) = '\0';
            cli->validBufEnd = cli->buf;
            /* Current header and value are incomplete */
            setCliState(cli,HEADER_INCOMPLETE_STATE,true);
            *moreFlag |= INCOMPLETE_VALUE;
            /* More data need to be pulled from the NWP */
            *moreFlag |= RECV_DATA;
            return(PARSE_AND_CONTINUE);
        }
        else
        {
            /* This sections  means that , The header and it value are cut before crlfcrlf */
            notSentLen =  cli->validBufEnd - cli->validBufStart;
            memcpy(cli->buf,cli->validBufStart,notSentLen);
            cli->validBufEnd = cli->buf + notSentLen;
            cli->validBufStart = cli->buf;
            *moreFlag |= RECV_DATA;
            return(CONTINUE_AND_PULL);
        }
    }
return(0);
}

/*
 *  ======== manageResponseHeaders ========
 *  Manage response Headers-Fields
 */

static int16_t manageResponseHeaders(HTTPClient_CB *cli, uint8_t *flags)
{
    int16_t ret;
    char header[HTTPClient_BUF_LEN];
    uint8_t statusFlag = 0;
    char * delim;
    char * tmpPtr;
    char * value;
    int8_t resNum;
    uint32_t recvBuf = 0;
    char errorStr[] = "HTTPClient_ERESPONSEVALUEBUFSMALL";

    do
    {
        delim = NULL;
        tmpPtr = NULL;
        if (statusFlag & RECV_DATA)
        {
            recvBuf = (HTTPClient_BUF_LEN - (cli->validBufEnd - cli->validBufStart));
            ret = SlNetSock_recv(cli->ssock,cli->validBufEnd,(recvBuf-1),0);
            if (ret < 0)
            {
                return(ret);
            }
            cli->validBufEnd += (ret);
            *cli->validBufEnd = '\0';
            statusFlag &= ~(RECV_DATA);
        }
        ret = readResponseHeader(cli,header,&statusFlag);
        if (ret < 0)
        {
            return (ret);
        }
        if (ret == CONTINUE_AND_PULL)
        {
            continue;
        }
        else if (ret == LAST_HEADER_INCOMPLETE)
        {
            break;
        }
        delim = strchr(header,':');
        if (delim)
        {
            *delim = '\0';
            tmpPtr = header;

            /* If Header field is "Location : " and it's incomplete mark it */
            if ((statusFlag & INCOMPLETE_VALUE) && !(stringcasecmp(HeaderToStr(HTTPClient_HFIELD_RES_LOCATION),tmpPtr)))
            {
                *flags |= INCOMPLETE_LOCATION;
            }

            resNum = resHeaderNameToHash(tmpPtr);
            if (resNum == HEADER_NOT_SUPPORTED)
            {
                continue;
            }
            if (validateResponseHeader(resNum,tmpPtr))
            {
                value = delim + 1;
                skipWhiteSpace(&value);
                resHeaderFieldHandler(cli,tmpPtr,value);
                /* Check if the response header is filtered */
                if (cli->responseHFieldFilter & NumberToBitField(resNum))
                {
                    if (statusFlag & INCOMPLETE_VALUE)
                    {
                        if ((strlen(errorStr) + 1) < cli->responseHFieldFreeSize)
                           {
                            memcpy(cli->responseHFieldIterator,errorStr,strlen(errorStr) + 1);
                            cli->responseHeaderMap[resNum] = cli->responseHFieldIterator;
                            cli->responseHFieldIterator += (strlen(errorStr) + 1);
                            cli->responseHFieldFreeSize -= (strlen(errorStr) + 1);
                        }
                    }
                    else
                    {
                        if ((strlen(value) + 1) < cli->responseHFieldFreeSize)
                        {
                            memcpy(cli->responseHFieldIterator,value,strlen(value) + 1);
                            cli->responseHeaderMap[resNum] = cli->responseHFieldIterator;
                            cli->responseHFieldIterator += (strlen(value) + 1);
                            cli->responseHFieldFreeSize -= (strlen(value) + 1);
                        }
                    }
                }
            }
        }
    } while (ret != NO_MORE_HEADERS);
    return(0);
}

/*
 *  ======== sendReqHeaders ========
 *  Sends the request headers-fields from the HTTPClient_Handle
 *  if name is not zero , it will send a specific request header-field,or
 *  will send all except the specific one. else if name is zero
 *  it will send all the request header fields according to flags
 */
static int16_t sendReqHeaders(HTTPClient_CB *cli,uint32_t name ,uint32_t flags)
{
    Req_HField *pReq;
    int16_t ret;

    if (flags & HTTPClient_HFIELD_PERSISTENT)
    {
        pReq = cli->reqHFieldPers;
    }
    else
    {
        pReq = cli->reqHField;
    }

    if ((name != 0) && (flags & ONE_REQ))
    {
        while (pReq != NULL)
        {
            if (name == pReq->Name)
            {
                ret = sprsend(cli, 0, "%s: %s\r\n", HeaderToStr(pReq->Name),pReq->Value);
                if (ret < 0)
                {
                    return (ret);
                }
                return(0);
            }
            pReq=pReq->Next;
        }
    }
    else if (((name != 0) && (flags & ALL_REQ_EXCEPT)) || (name == 0))
    {
        while (pReq != NULL)
        {
            /* Note we are only processing custom headers when name == 0 */
            if (pReq->CustomName != NULL) {
                ret = sprsend(cli, 0, "%s: %s\r\n", pReq->CustomName, pReq->Value);
                if (ret < 0) {
                    return (ret);
                }
            }
            else if (name != 0)
            {
                if (pReq->Name == name)
                {
                    pReq=pReq->Next;
                    continue;
                }
            }
            else {
                ret = sprsend(cli, 0, "%s: %s\r\n", HeaderToStr(pReq->Name),pReq->Value);

                if (ret < 0)
                {
                    return (ret);
                }
            }
            pReq=pReq->Next;
        }
        return(0);
    }

    return(HTTPClient_EREQHEADERNOTFOUND);
}

/*
 *  ======== startSecureMode ========
 */
static int16_t startSecureMode(HTTPClient_CB *cli, const char *domain, HTTPClient_extSecParams *exSecParams)
{
    SlNetSockSecAttrib_t *secAttribHdl = NULL;
    int ret;
    secAttribHdl = SlNetSock_secAttribCreate();
    if (secAttribHdl == NULL)
    {
        return(HTTPClient_ECANTCREATESECATTRIB);
    }
    if (exSecParams != NULL)
    {
        if (exSecParams->rootCa != NULL)
        {
            ret = SlNetSock_secAttribSet(secAttribHdl, SLNETSOCK_SEC_ATTRIB_PEER_ROOT_CA,(char *)exSecParams->rootCa, strlen(exSecParams->rootCa));
            if (ret < 0)
            {
                   return (ret);
            }
        }
        if (exSecParams->privateKey != NULL)
        {
            ret = SlNetSock_secAttribSet(secAttribHdl, SLNETSOCK_SEC_ATTRIB_PRIVATE_KEY, (char *)exSecParams->privateKey, strlen(exSecParams->privateKey));
            if (ret < 0)
            {
                   return (ret);
            }
        }
        if (exSecParams->clientCert != NULL)
        {
            ret = SlNetSock_secAttribSet(secAttribHdl, SLNETSOCK_SEC_ATTRIB_LOCAL_CERT, (char *)exSecParams->clientCert, strlen(exSecParams->clientCert));
            if (ret < 0)
            {
                   return (ret);
            }
        }
    }
    if (domain != NULL)
    {
        ret = SlNetSock_secAttribSet(secAttribHdl, SLNETSOCK_SEC_ATTRIB_DOMAIN_NAME,(char *)domain,strlen(domain));
        if (ret < 0)
        {
            return (ret);
        }
    }
    ret = SlNetSock_startSec(cli->ssock, secAttribHdl, SLNETSOCK_SEC_START_SECURITY_SESSION_ONLY|SLNETSOCK_SEC_BIND_CONTEXT_ONLY);
    return (ret);

}

/*
 *  ======== sprsend ========
 *  Constructs an HTTP Request line to send
 */
static int16_t sprsend(HTTPClient_CB *cli, uint8_t flags, const char *fmt, ...)
{
    char sendBuf[HTTPClient_BUF_LEN];
    int16_t len = 0;
    int16_t ret = 0;
    uint32_t bytesToSend = 0;
    uint32_t freeBytes = 0;
    uint8_t resendCount = 3;
    va_list ap;

    if (fmt == NULL)
    {
        len = 0;
    }
    else
    {
        va_start(ap, fmt);
        len = vsnprintf(sendBuf, HTTPClient_BUF_LEN,fmt, ap);
        va_end(ap);
    }
    if (len > HTTPClient_BUF_LEN)
    {
        return (HTTPClient_ESENDBUFSMALL);
    }

    freeBytes = (HTTPClient_BUF_LEN - (cli->validBufEnd - cli->validBufStart));
    if ((len < freeBytes) && (!(flags & SENDBUF)))
    {
        /* Buffer the data */
        memcpy(cli->validBufEnd,sendBuf,len);
        cli->validBufEnd += len;
    }
    else
    {
        /* Send the buffered data */
        bytesToSend = cli->validBufEnd - cli->validBufStart;
        do
        {
            ret = SlNetSock_send(cli->ssock, cli->validBufStart, bytesToSend, 0);

            if (ret < 0)
            {
                return (ret);
            }
            if (ret < bytesToSend)
            {
                cli->validBufStart += ret;
                resendCount--;
            }
            bytesToSend -= ret;
        }while ((bytesToSend > 0) && (resendCount > 0));

        if ((!resendCount) && (bytesToSend > 0))
        {
            return(HTTPClient_ESENDERROR);
        }
        cli->validBufStart = cli->buf;
        memcpy(cli->validBufStart,sendBuf,len);
        cli->validBufEnd = cli->validBufStart + len;
    }

    return (0);
}

/*
 *  ======== readLine ========
 *  Read a line from begining to the \n
 */
static int16_t readLine(HTTPClient_CB *cli, char *line, int len, bool *moreFlag, uint32_t buffereddBodyLen)
{
    char c;
    int i = 0;
    int ret = 0;

    *moreFlag = true;

    for (i = 0; i < len; i++)
    {
        if (buffereddBodyLen > 0)
        {
            c = *(cli->validBufStart);
            cli->validBufStart++;
            buffereddBodyLen--;
        }
        else
        {
            ret = SlNetSock_recv(cli->ssock, &c, 1, 0);
            if (ret < 0)
            {
                *moreFlag = false;
                return (ret);
            }
        }

        if (c == '\n')
        {
            line[i] = 0;
            /* Line read completed */
            *moreFlag = false;
            break;
        }
        else if ((c == '\r') || (i == 0 && c == ' '))
        {
            /* drop CR or drop leading SP*/
            i--;
        }
        else
        {
            line[i] = c;
        }

    }

    return (0);
}
/*
 *  ======== getStatus ========
 *  Processes the response line to get the status
 */
static int16_t getStatus(HTTPClient_CB *cli)
{
    uint32_t recvBuf;
    int16_t ret;
    uint32_t status;
    uint8_t vLen;
    char * delim = NULL;
    vLen = strlen(HTTPStr_VER1_1);

    /* Handling the response packet, initialize the buffer */
    cli->validBufEnd = cli->buf;
    cli->validBufStart = cli->buf;

    recvBuf = (HTTPClient_BUF_LEN - (cli->validBufEnd - cli->validBufStart));
    ret = SlNetSock_recv(cli->ssock,cli->validBufEnd,(recvBuf-1),0);
    if (ret < 0)
    {
        return(ret);
    }
    else if(ret == 0)
    {
        return (HTTPClient_ENOCONNECTION);
    }
    cli->validBufEnd += (ret);
    *cli->validBufEnd = '\0';

    /* Verify HTTP version is 1.1 or 1.0 */
    if (strncmp(cli->validBufStart, HTTPStr_VER1_1, vLen) && strncmp(cli->validBufStart, HTTPStr_VER1_0,vLen ))
    {
        /* not a valid HTTP version */
        return (HTTPClient_ERESPONSEINVALID);
    }

    /* In case of HTTP version 1.0 the connection is not persistent */
    if    (strncmp(cli->validBufStart, HTTPStr_VER1_0, vLen) == 0)
    {
        setCliState(cli,CONNECTED_STATE,false);
    }
    cli->validBufStart += vLen;

    /* Retrieve status number */
    status = strtoul(cli->validBufStart, NULL, 10);

    /* Move the iterator to after the HTTP reason */
    delim = strchr(cli->validBufStart,'\n');
    if (delim)
    {
        cli->validBufStart = delim + 1;
    }
    return (status);
}

/*
 *  ======== httpProxyTunnel ========
 *  HTTP tunnel through proxy
 */
static int16_t httpProxyTunnel(HTTPClient_CB *cli, const char *hostName)
{
    uint8_t redirectFlag = 0;
    int16_t ret = 0;

    /* Start line */
    ret = sprsend(cli, 0, "%s %s %s\r\n", HTTP_METHOD_CONNECT, hostName, HTTPStr_VER1_1);
    if (ret < 0)
    {
        return (ret);
    }
    /* Host header-field */
    ret = sendReqHeaders(cli,HTTPClient_HFIELD_REQ_HOST,HTTPClient_HFIELD_PERSISTENT|ONE_REQ);
    if (ret == HTTPClient_EREQHEADERNOTFOUND)
    {
        return(HTTPClient_EHOSTNOTFOUND);
    }
    else if (ret < 0 )
    {
        return (ret);
    }
    /* Proxy authorization */
    ret = sendReqHeaders(cli,HTTPClient_HFIELD_REQ_PROXY_AUTHORIZATION,HTTPClient_HFIELD_NOT_PERSISTENT|ONE_REQ);
    ret = sprsend(cli, 0,"\r\n");
    if (ret < 0)
    {
        return (ret);
    }
    ret = sprsend(cli, SENDBUF,NULL);
    if (ret < 0)
    {
        return (ret);
    }

    /* Clears the buffer which stores the response value in order to store new values */
    clearResponseValues(cli);
    ret = getStatus(cli);
    if (ret < 0)
    {
        return (ret);
    }

    manageResponseHeaders(cli,&redirectFlag);

    cli->bodyLen  = 0;

    return (0);
}

/*
 *  ======== parseURI ========
 */
static int16_t parseURI(const char *uri, uint16_t *portOutput,char *domainOutput, uint16_t *flags)
{
    char *domain;
    char *delim;
    uint16_t dlen;
    uint16_t port = 0;
    uint16_t intFlags = 0;

    domain = (char *)uri;
    /* check whether the host is secured or not */
    if (stringncasecmp(HTTPClient_HTTP_PREFIX, domain, (sizeof(HTTPClient_HTTP_PREFIX) - 1)) == 0)
    {
        domain = domain + sizeof(HTTPClient_HTTP_PREFIX) - 1;
    }
    else if (stringncasecmp(HTTPClient_HTTPS_PREFIX, domain, (sizeof(HTTPClient_HTTPS_PREFIX) - 1)) == 0)
    {
        intFlags |= ISSECURED;
        domain = domain + sizeof(HTTPClient_HTTPS_PREFIX) - 1;
    }

    /* check whether the domain is IPV6 */
    if ((strchr(domain, '[') != NULL) && (strchr(domain, ']')))
    {
        if (countChar(domain,':') >= 2)
        {
            intFlags |= ISIPV6;
        }
    }

    /* check whether we have port as part of the host name */
    if (!(intFlags & ISIPV6))
    {
        delim = strchr(domain, ':');
        if (delim != NULL)
        {
            dlen = delim - domain;
            port = strtoul((delim + 1), NULL, 10);
        }
        else
        {
            /* checks whether the host name has '/' at the end of it */
            delim = strchr(domain, '/');
            if (delim != NULL)
            {
                dlen = delim - domain;
            }
            else
            {
                dlen = strlen(domain);
            }
        }
    }
    else
    {
        delim = strstr(domain, "]:");
        if (delim != NULL)
        {
            port = strtoul((delim + 2), NULL, 10);
            /*    dlen should include both '[' and ']' */
            dlen = (delim + 1) - domain;
        }
        else
        {
            /* checks whether the host name has '/' at the end of it */
            delim = strstr(domain, "]/");
            if (delim != NULL)
            {
                dlen = (delim + 1) - domain;
            }
            else
            {
                dlen = strlen(domain);
            }
        }
    }
    if (dlen >= HTTPClient_DOMAIN_BUFLEN)
    {
        return (HTTPClient_EDOMAINBUFSMALL);
    }
    strncpy(domainOutput, domain, dlen);
    domainOutput[dlen] = '\0';
    if (!(intFlags & ISIPV6))
    {
        if ((countChar(domainOutput,'.') != 3) || ((countChar(domainOutput,'.') == 3) && (65 <= *domain) && (*domain <= 122)))
        {
            intFlags |= ISDOMAIN;
        }
    }
    /* Check if the port was a part of the URI, if not enter default
       parameter according to the prefix */
    if (0 != port)
    {
        if (port == HTTPClient_SECURE_PORT)
        {
            intFlags |= ISSECURED;
        }
        *portOutput = port;
    }
    else
    {
        intFlags |= ISDEFAULTPORT;
        if (intFlags & ISSECURED)
        {
            *portOutput = HTTPClient_SECURE_PORT;
        }
        else
        {
            *portOutput = HTTPClient_PORT;
        }
    }
    *flags = intFlags;
    return (0);
}

/*
 *  ======== initialize ========
 */
static void initializeCB(HTTPClient_CB *cli)
{
    memset(cli, 0, sizeof(HTTPClient_CB));
    cli->validBufEnd = cli->buf;
    cli->validBufStart = cli->buf;
    cli->ssock = -1;
    cli->responseHFieldFilter |= RESPONSE_HFIELDS_ENABLED;
    cli->responseHFieldFreeSize = HTTPClient_RES_HFIELD_BUFFER_SIZE;
    setCliState(cli,ENABLE_REDIRECT_STATE,true);
    setCliState(cli,ENABLE_TLS_DOWNGRADE_STATE,false);
}

/*
 *  ======== HTTPClient_disconnect ========
 */
int16_t HTTPClient_disconnect(HTTPClient_Handle client)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    int16_t ret = 0;

    if (cli != NULL)
    {
        ret = SlNetSock_close(cli->ssock);
        /* Clear non-persistent request headers */
        clearReqHeaders(cli,false);
        /* Clear persistent request headers */
        clearReqHeaders(cli,true);
        setCliState(cli, CONNECTED_STATE, false);
        setCliState(cli, ISSECURED_STATE, false);
    }
    return(ret);
}

/*
 *  ======== HTTPClient_create ========
 */

HTTPClient_Handle HTTPClient_create(int16_t *status, void *params)
{
    HTTPClient_CB *cli = NULL;
    int16_t ret = 0;
    cli = (HTTPClient_CB *)malloc(sizeof(HTTPClient_CB));
    if (cli != NULL)
    {
        initializeCB(cli);
        cli->responseHFieldValue = (char *)malloc(HTTPClient_RES_HFIELD_BUFFER_SIZE);
        if (cli->responseHFieldValue != NULL)
        {
            cli->responseHFieldIterator = cli->responseHFieldValue;
        }
        else
        {
            free(cli);
            *status = HTTPClient_ECBALLOCATIONFAILED;
            return(NULL);
        }
    }
    else
    {
        ret = HTTPClient_ECBALLOCATIONFAILED;
    }
    *status = ret;
    return ((HTTPClient_Handle)cli);
}

/*
 *  ======== HTTPClient_destroy ========
 */
int16_t HTTPClient_destroy(HTTPClient_Handle client)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    if (cli == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    /* Clear non-persistent request headers */
    clearReqHeaders(cli,false);
    /* Clear persistent request headers */
    clearReqHeaders(cli,true);
    free(cli->responseHFieldValue);
    free(cli);
    return(0);
}

/*
 *  ======== HTTPClient_connect ========
 */
int16_t HTTPClient_connect(HTTPClient_Handle client, const char *hostName,HTTPClient_extSecParams *exSecParams, uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    int16_t   skt;
    int32_t   ret;
    int32_t   slen;
    uint16_t  port = 0;
    uint16_t  intFlags = 0;
    char      domainBuff[HTTPClient_DOMAIN_BUFLEN];
    char     *domainPtr = domainBuff;
    uint32_t  hostLen = 0;
    struct SlNetSock_Addr_t *sa;


    if (cli == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    if (!getCliState(cli, CONNECTED_STATE))
    {
        cli->validBufStart = cli->buf;
        cli->validBufEnd = cli->buf;
        ret = parseURI(hostName,&port,domainBuff,&intFlags);
        if (ret < 0)
        {
            return(ret);
        }
        hostLen = strlen(domainBuff);

        if ((port != 0) && !(intFlags & ISDEFAULTPORT))
        {
            if ((HTTPClient_DOMAIN_BUFLEN - HTTPClient_MAX_PORT_LEN - 1) < (hostLen))
            {
                return (HTTPClient_EDOMAINBUFSMALL);
            }
            sprintf(&domainBuff[hostLen],":%d",port);
        }
        if (!(flags & HTTPClient_HOST_EXIST))
        {
            if (searchRequestHeader(cli,HTTPClient_HFIELD_REQ_HOST,true) == NULL)
            {
                ret = HTTPClient_setHeader(cli,HTTPClient_HFIELD_REQ_HOST,(char *)domainPtr,strlen(domainPtr) + 1,HTTPClient_HFIELD_PERSISTENT);
                if (ret < 0)
                {
                    return(ret);
                }
            }
            else
            {
                return(HTTPClient_EHOSTHEADERALREADYEXIST);
            }
        }
        domainBuff[hostLen] = '\0';
        if (intFlags & ISIPV6)
        {
            /* Remove HTTP IPV6 brackets '[]' */
            domainPtr++;
            domainPtr[hostLen - 2] = '\0';
        }
        if (flags & HTTPClient_IGNORE_PROXY)
        {
            /* Ignore proxy for this client */
            setCliState(cli,IGNORE_PROXY_STATE,true);
        }
        if ((proxyAddr.sa_family != 0) && (!getCliState(cli,IGNORE_PROXY_STATE)))
        {
            /* Connecting to HTTP server via proxy */
            sa = &proxyAddr;
            if (sa->sa_family == SLNETSOCK_AF_INET6)
            {
                slen = sizeof(struct SlNetSock_AddrIn6_t);
                skt = SlNetSock_create(SLNETSOCK_AF_INET6,SLNETSOCK_SOCK_STREAM,SLNETSOCK_PROTO_TCP,0,0);
            }
            else
            {
                slen = sizeof(struct SlNetSock_AddrIn_t);
                skt = SlNetSock_create(SLNETSOCK_AF_INET,SLNETSOCK_SOCK_STREAM,SLNETSOCK_PROTO_TCP,0,0);
            }
            if (skt >= 0)
            {
                cli->ssock = skt;
            }
            else
            {
                HTTPClient_disconnect(cli);
                /* Return error code */
                return (skt);
            }
            ret = SlNetSock_connect(skt,sa,slen);
            if(ret < 0 )
            {
                HTTPClient_disconnect(cli);
                return (ret);
            }
            /* Connection finished succesfully */
            /* Tunnel using HTTP CONNECT */
            ret = httpProxyTunnel(cli,hostName);
            if (ret != 0)
            {
                HTTPClient_disconnect(cli);
                return(ret);
            }
        }
        else
        {
            skt =  SlNetSock_connectURI(NULL,hostName,NULL,0, NULL);
            if(skt < 0 )
            {
                HTTPClient_disconnect(cli);
                return (skt);
            }
            cli->ssock = skt;
            /* Connection finished succesfully */
        }
        if ((intFlags & ISSECURED) || getCliState(cli, ISSECURED_STATE))
        {
            if (!(intFlags & ISDOMAIN))
            {
                domainPtr = NULL;
            }
            ret = startSecureMode(cli,domainPtr,exSecParams);
            if ((ret < 0) && (ret != SLNETERR_ESEC_UNKNOWN_ROOT_CA) && (ret != SLNETERR_ESEC_HAND_SHAKE_TIMED_OUT) && (ret != SLNETERR_ESEC_DATE_ERROR) && (ret != SLNETERR_ESEC_SNO_VERIFY))
            {
                HTTPClient_disconnect(cli);
                return (ret);
            }
            /* TLS was successfull, saves security attributes for reconnect */
            setCliState(cli, ISSECURED_STATE, true);
            cli->secParams = exSecParams;
        }
        setCliState(cli, CONNECTED_STATE, true);
        return (0);
    }
    return (HTTPClient_ECLIENTALREADYCONNECTED);
}

int16_t HTTPClient_sendRequest(HTTPClient_Handle client, const char *method,const char *requestURI, const char *body, uint32_t bodyLen, uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    int16_t ret;
    uint8_t retryCounter = HTTPClient_MAX_REQUEST_ATTEMPTS;

    do
    {
        ret = sendRequest(cli,method,requestURI,body,bodyLen,flags);
        if (((ret <= SLNETERR_BSD_SOC_ERROR) && (ret >= SLNETERR_BSD_EALREADY)) || (ret == HTTPClient_ENOCONNECTION))
        {
            /* In case the connection is not persistent or sudden disconnection occured, try to reconnect */
            SlNetSock_close(cli->ssock);
            setCliState(cli,CONNECTED_STATE,false);
            retryCounter--;
        }
        else if (ret == HTTP_SC_UNAUTHORIZED)
        {
            retryCounter--;
        }
    }while((((ret <= SLNETERR_BSD_SOC_ERROR) && (ret >= SLNETERR_BSD_EALREADY)) || (ret == HTTP_SC_UNAUTHORIZED) || (ret == HTTPClient_ENOCONNECTION)) && (retryCounter > 0));

    /* Clears the non-persistent response filter */
    clearNonPersistResponseFilter(cli);
    /* Clears the non-persistent request headers */
    clearReqHeaders(cli,false);

    return (ret);
}

/*
 *  ======== HTTPClient_sendRequest ========
 */

static int16_t sendRequest(HTTPClient_CB *cli, const char *method,const char *requestURI, const char *body, uint32_t bodyLen, uint32_t flags)
{
    int16_t ret;
    int16_t status;
    uint8_t redirectCount = 0;
    Req_HField *hostHeader;
    char *bodyPtr = (char *)body;
    uint32_t bytesToSend = bodyLen;
    char buf[HTTPClient_BUF_LEN] = {0};
    uint32_t bufLen = HTTPClient_BUF_LEN;
    bool moreFlag;
    uint8_t redirectFlag = 0;
    bool waitBlocked = false;

    if ((cli == NULL) || (method == NULL) || (requestURI == NULL))
    {
        return (HTTPClient_ENULLPOINTER);
    }

    /* If client was disconnected, try to reconnect */
    if (!getCliState(cli, CONNECTED_STATE))
    {
        if ((hostHeader = searchRequestHeader(cli,HTTPClient_HFIELD_REQ_HOST,true)) != NULL)
        {
            ret = HTTPClient_connect(cli,hostHeader->Value,NULL,HTTPClient_HOST_EXIST);
            if (ret < 0)
            {
                return (ret);
            }
        }
        else
        {
            if (*requestURI == '/')
            {
                return(HTTPClient_ENOTABSOLUTEURI);
            }
            ret = HTTPClient_connect(cli,requestURI,NULL,0);
            if (ret < 0)
            {
                return (ret);
            }
        }
    }

    /* Build request packet */
    cli->validBufStart = cli->buf;
    cli->validBufEnd = cli->buf;
    /* Not in chunked state */
    if(!getCliState(cli, CHUNKED_REQ_STATE))
    {
        /* validity check */
        if (flags & HTTPClient_CHUNK_END)
        {
            return (HTTPClient_EWRONGAPIPARAMETER);
        }
        /* Start line */
        ret = sprsend(cli, 0, "%s %s %s\r\n", method, requestURI? requestURI : "/",HTTPStr_VER1_1);
        if (ret < 0)
        {
            return (ret);
        }
        /* Non persistent request headers */
        ret = sendReqHeaders(cli,0,HTTPClient_HFIELD_NOT_PERSISTENT);
        if (ret < 0)
        {
            return (ret);
        }
        /* Persistent request headers */
        ret = sendReqHeaders(cli,0,HTTPClient_HFIELD_PERSISTENT);
        if (ret < 0)
        {
            return (ret);
        }
        if (NULL != body)
        {
            if (flags & HTTPClient_CHUNK_START)
            {
                ret = sprsend(cli, 0,"Transfer-Encoding: chunked\r\n");
                if (ret < 0)
                {
                    return (ret);
                }
            }
            else if (flags == 0)
            {
                ret = sprsend(cli, 0,"Content-Length: %d\r\n",bodyLen);
                if (ret < 0)
                {
                    return (ret);
                }
            }
        }
        /* End of headers section */
        ret = sprsend(cli, 0,"\r\n");
        if (ret < 0)
        {
            return (ret);
        }
        if ((!(flags == HTTPClient_CHUNK_END)) && (!(flags == HTTPClient_CHUNK_START)))
        {
            /* In case of content-length - wait for response */
            waitBlocked = true;
        }
    }
    /* chunked state */
    else
    {
        /* validity check */
        if((flags & HTTPClient_CHUNK_START) || (NULL == body))
        {
            return (HTTPClient_EWRONGAPIPARAMETER);
        }
    }

    /* in case a body exists send it */
    if(body)
    {
        /* In case of chunk, add the chunk length add the beginning of the body */
        if ((flags & HTTPClient_CHUNK_START) || (getCliState(cli, CHUNKED_REQ_STATE)))
        {
            ret = sprsend(cli, 0, "%x \r\n",bodyLen);
            if (ret < 0)
            {
                return (ret);
            }
        }
        /* Send the buf */
        ret = sprsend(cli, SENDBUF,NULL);
        if (ret < 0)
        {
            return (ret);
        }
        /* Send the body payload */
        while(bytesToSend != 0)
        {
            ret = SlNetSock_send(cli->ssock, bodyPtr, bytesToSend, 0);
            if (ret < 0)
            {
                return (ret);
            }
            bytesToSend-=ret;
            if (ret < bytesToSend)
            {
                bodyPtr+=ret;
            }
        }
        ret = sprsend(cli, 0, "\r\n");
        if (ret < 0)
        {
            return (ret);
        }
        if (flags & HTTPClient_CHUNK_START)
        {
            setCliState(cli,CHUNKED_REQ_STATE,true);
        }
        else if (flags & HTTPClient_CHUNK_END)
        {
            ret = sprsend(cli, 0, "0\r\n\r\n");
            if (ret < 0)
            {
                return (ret);
            }
            setCliState(cli,CHUNKED_REQ_STATE,false);
            waitBlocked = true;
        }
        /* Send the buffer */
        ret = sprsend(cli, SENDBUF,NULL);
        if (ret < 0)
        {
            return (ret);
        }
    }
    else
    {
        /* There is no body, send the start line and headers */
        ret = sprsend(cli, SENDBUF,NULL);
        if (ret < 0)
        {
            return (ret);
        }
    }
    if (waitBlocked)
    {
        do
        {
            clearResponseValues(cli);
            status = getStatus(cli);
            if (status < 0)
            {
                return (status);
            }
            if (getCliState(cli,ENABLE_REDIRECT_STATE) && (isRedirect(status)))
            {
                /* In order to redirect, location header must be parsed and saved. */
                /* If the user didn't enable the filter for the Location response, its enabled internally */
                if (!(cli->responseHFieldFilter & NumberToBitField(HTTPClient_HFIELD_RES_LOCATION)))
                {
                    cli->responseHFieldFilter |= NumberToBitField(HTTPClient_HFIELD_RES_LOCATION);
                    setCliState(cli,LOCATION_ADDED_STATE,true);
                }
            }
            /* Parse all headers */
            manageResponseHeaders(cli,&redirectFlag);
            if (getCliState(cli,ENABLE_REDIRECT_STATE) && (isRedirect(status)))
            {
                if (getCliState(cli,LOCATION_ADDED_STATE))
                {
                    /* If Location header was filtered in internally, remove it from filter list */
                    cli->responseHFieldFilter &= ~(NumberToBitField(HTTPClient_HFIELD_RES_LOCATION));
                    setCliState(cli,LOCATION_ADDED_STATE,false);
                }
                if (redirectFlag & INCOMPLETE_LOCATION)
                {
                    /* Location header is incomplete redirect is imposible */
                    return( HTTPClient_EREDIRECTLOCATIONFAIL );
                }
                if (redirectCount < HTTPClient_MAX_REDIRECTION_ATTEMPTS)
                {
                    ret = redirect(cli,status,method);
                    if (ret < 0)
                    {
                        return(ret);
                    }
                }
                redirectCount++;
            }
            else if ((status == HTTP_SC_UNAUTHORIZED) && (cli->httpAuthCallback != NULL))
            {
                /* Check if the Www-Authenticate is a valid response header */
                if ((cli->responseHeaderMap[WWW_AUTHENTICATE_HASH_INDEX] != NULL) && (strstr(cli->responseHeaderMap[WWW_AUTHENTICATE_HASH_INDEX],"HTTPClient_ERESPONSEVALUEBUFSMALL") == NULL))
                {
                    /* Execute the authentication callback which was registered by the user */
                    ret = cli->httpAuthCallback(cli->responseHeaderMap[WWW_AUTHENTICATE_HASH_INDEX],strlen(cli->responseHeaderMap[WWW_AUTHENTICATE_HASH_INDEX]),buf,&bufLen);
                    if (ret < 0 )
                    {
                        return(ret);
                    }
                    /* Add client authorization request header */
                    ret = HTTPClient_setHeader(cli,HTTPClient_HFIELD_REQ_AUTHORIZATION,buf,bufLen,HTTPClient_HFIELD_NOT_PERSISTENT);
                    /* If the used method is HEAD, the server won't send body even if the content-length header is present */
                    if (strcmp(method,HTTP_METHOD_HEAD) != 0)
                    {
                        /* Flush the body of the response if exist        */
                        do
                        {
                            ret = HTTPClient_readResponseBody(cli, buf,HTTPClient_BUF_LEN,&moreFlag);
                            if (ret < 0)
                            {
                                return (ret);
                            }
                        }while ((ret > 0) && moreFlag);
                    }
                    else
                    {
                        cli->bodyLen = 0;
                    }
                }
                /* Stop the do while loop and exit the sendRequest() function in-order to re-send the request
                with the generated authorization header */
                break;
            }
            else
            {
                break;
            }
        }while(status != HTTP_SC_OK && redirectCount <= HTTPClient_MAX_REDIRECTION_ATTEMPTS);
        if (flags & HTTPClient_DROP_BODY)
        {
            /* If the used method is HEAD, the server won't send body even if the content-length header is present */
            if (strcmp(method,HTTP_METHOD_HEAD) != 0)
            {
                /* Flush the body of the response if exist    */
                do
                {
                    ret = HTTPClient_readResponseBody(cli, buf,HTTPClient_BUF_LEN,&moreFlag);
                    if (ret < 0)
                    {
                        return (ret);
                    }
                }while ((ret > 0) && moreFlag);
            }
            else
            {
                cli->bodyLen = 0;
            }
        }
        return(status);
    }
    return (0);
}

int16_t HTTPClient_readResponseBody(HTTPClient_Handle client, char *body, uint32_t bodyLen,bool *moreDataFlag)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    int16_t ret = 0;
    uint32_t buffereddBodyLen = 0;

    if ((body == NULL) || (cli == NULL) || (moreDataFlag == NULL))
    {
        return(HTTPClient_ENULLPOINTER);
    }

    buffereddBodyLen = cli->validBufEnd - cli->validBufStart;

    if (buffereddBodyLen > 0)
    {
        if (bodyLen < buffereddBodyLen)
        {
            return(HTTPClient_EBODYBUFSMALL);
        }
        if (!getCliState(cli, CHUNKED_STATE))
        {
            memcpy(body,cli->validBufStart,buffereddBodyLen);
            cli->bodyLen -= buffereddBodyLen;
            cli->validBufEnd = cli->buf;
            cli->validBufStart = cli->buf;
        }
    }
    *moreDataFlag = false;

    if (getCliState(cli, CHUNKED_STATE))
    {
        ret = getChunkedData(cli, body, bodyLen, moreDataFlag);
    }
    else
    {
        if (cli->bodyLen)
        {
            if (cli->bodyLen < (bodyLen - buffereddBodyLen))
            {
                bodyLen = cli->bodyLen;
                ret = SlNetSock_recv(cli->ssock, body + buffereddBodyLen, bodyLen, 0);
            }
            else
            {
                ret = SlNetSock_recv(cli->ssock, body + buffereddBodyLen, bodyLen - buffereddBodyLen, 0);
            }
            if (ret > 0)
            {
                cli->bodyLen -= ret;
                *moreDataFlag = cli->bodyLen ? true : false;
            }
        }
        /* Total bytes that were recieved and which were buffered during header parsing */
        ret += buffereddBodyLen;
    }
    return (ret);
}


int16_t HTTPClient_setHeader(HTTPClient_Handle client, uint32_t option, void *value, uint32_t len, uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    uint8_t intFlags = 0;
    Req_HField *node;
    uint32_t headerType = 0;
    int16_t ret;

    if (cli == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    /* Check if option is a valid header */
    if ((option & ~(HTTPClient_REQUEST_HEADER_MASK)) < HTTPClient_MAX_NUMBER_OF_HEADER_FIELDS )
    {
        intFlags |= ISHEADER;
        /* Check if option is a request header */
        if (option & HTTPClient_REQUEST_HEADER_MASK)
        {
            intFlags |= ISREQUEST;
        }
    }

    headerType = (flags & HTTPClient_HFIELD_PERSISTENT)?HTTPClient_HFIELD_PERSISTENT:HTTPClient_HFIELD_NOT_PERSISTENT;
    if (intFlags & ISHEADER)
    {
        if (intFlags & ISREQUEST)
        {
            /* Insert request header into control-block */
            if (value != NULL)
            {
                /* Allocating request header node */
                node = (Req_HField *)malloc(sizeof(Req_HField));
                if (node != NULL)
                {
                    node->CustomName = NULL;
                    node->Value = (char *)malloc(len);
                    if (node->Value == NULL)
                    {
                        free(node);
                        return(HTTPClient_EREQUESTHEADERALLOCFAILED);
                    }
                    /* Fill the request name and value */
                    node->Name = option;
                    memcpy(node->Value,value,len);
                }
                else
                {
                    return (HTTPClient_EREQUESTHEADERALLOCFAILED);
                }
                /* Push the request-header to the start of the linked list */
                if (headerType == HTTPClient_HFIELD_PERSISTENT)
                {
                    if (cli->reqHFieldPers == NULL)
                    {
                        cli->reqHFieldPers = node;
                        node->Next = NULL;
                    }
                    else
                    {
                        node->Next = cli->reqHFieldPers;
                        cli->reqHFieldPers = node;
                    }
                }
                else
                {
                    if (cli->reqHField == NULL)
                    {
                        cli->reqHField = node;
                        node->Next = NULL;
                    }
                    else
                    {
                        node->Next = cli->reqHField;
                        cli->reqHField = node;
                    }
                }
            }
            else if (value == NULL)
            {
                /* Remove request header  from control-block */
                ret = removeRequestHeader(cli,option,headerType);
                if (ret < 0)
                {
                    return(ret);
                }
            }
        }
        else
        {
            /* Check if all response header-fields are enbled */
            if (!(~(cli->responseHFieldFilter) & RESPONSE_HFIELDS_ENABLED))
            {
                cli->responseHFieldFilter &= RESPONSE_HFIELDS_DISABLED;
            }
            cli->responseHFieldFilter |= NumberToBitField(option);
            if (flags & HTTPClient_HFIELD_PERSISTENT)
            {
                cli->responseHFieldFilterPersist |= NumberToBitField(option);
            }
            else if (flags & HTTPClient_HFIELD_NOT_PERSISTENT)
            {
                cli->responseHFieldFilterPersist &= ~(NumberToBitField(option));
            }
        }
        return(0);
    }
    else
    {
        return (HTTPClient_EWRONGAPIPARAMETER);
    }
}

int16_t HTTPClient_setHeaderByName(HTTPClient_Handle client, uint32_t option, const char* name, void *value, uint32_t len, uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    uint8_t intFlags = 0;
    Req_HField *node;
    uint32_t headerType = 0;

    if ((cli == NULL) || (name == NULL))
    {
        return(HTTPClient_ENULLPOINTER);
    }

    /* Check if option is a request header */
    if ((option & HTTPClient_REQUEST_HEADER_MASK) == 0)
    {
        return (HTTPClient_EWRONGAPIPARAMETER);
    }

    /* TODO: do we need to deal with custom response headers? */
    intFlags |= ISHEADER;
    intFlags |= ISREQUEST;

    headerType = (flags & HTTPClient_HFIELD_PERSISTENT) ?
            HTTPClient_HFIELD_PERSISTENT:HTTPClient_HFIELD_NOT_PERSISTENT;
    if (intFlags & ISHEADER)
    {
        if (intFlags & ISREQUEST)
        {
            /* Insert request header into control-block */
            if (value != NULL)
            {
                /* Allocating request header node */
                node = (Req_HField *)malloc(sizeof(Req_HField));
                if (node != NULL)
                {
                    node->Value = (char *)malloc(len);
                    if (node->Value == NULL)
                    {
                        free(node);
                        return(HTTPClient_EREQUESTHEADERALLOCFAILED);
                    }
                    /* Fill the request name and value */
                    node->CustomName = (char *)malloc(strlen(name) + 1);
                    if (node->CustomName == NULL)
                    {
                        free(node->Value);
                        free(node);
                        return (HTTPClient_EREQUESTHEADERALLOCFAILED);
                    }
                    strcpy(node->CustomName, name);
                    memcpy(node->Value,value,len);
                }
                else
                {
                    return (HTTPClient_EREQUESTHEADERALLOCFAILED);
                }
                /* Push the request-header to the start of the linked list */
                if (headerType == HTTPClient_HFIELD_PERSISTENT)
                {
                    if (cli->reqHFieldPers == NULL)
                    {
                        cli->reqHFieldPers = node;
                        node->Next = NULL;
                    }
                    else
                    {
                        node->Next = cli->reqHFieldPers;
                        cli->reqHFieldPers = node;
                    }
                }
                else
                {
                    if (cli->reqHField == NULL)
                    {
                        cli->reqHField = node;
                        node->Next = NULL;
                    }
                    else
                    {
                        node->Next = cli->reqHField;
                        cli->reqHField = node;
                    }
                }
            }
        }

        return(0);
    }
    else
    {
        return (HTTPClient_EWRONGAPIPARAMETER);
    }
}


int16_t HTTPClient_getHeader(HTTPClient_Handle client, uint32_t option, void *value, uint32_t *len ,uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;
    uint8_t intFlags = 0;

    if ((value == NULL) || (len == NULL) || (cli == NULL))
    {
        return (HTTPClient_ENULLPOINTER);
    }

    /* Check if option is a valid header */
    if ((option & ~(HTTPClient_REQUEST_HEADER_MASK)) < HTTPClient_MAX_NUMBER_OF_HEADER_FIELDS )
    {
        intFlags |= ISHEADER;
        /* Check if option is a request header */
        if (option & HTTPClient_REQUEST_HEADER_MASK)
        {
            intFlags |= ISREQUEST;
        }
    }
    if (intFlags & ISHEADER)
    {
        if (!(intFlags & ISREQUEST))
        {
            /* Option is a response header */
            if (*len < strlen(cli->responseHeaderMap[option]))
            {
                return (HTTPClient_EGETOPTBUFSMALL);
            }
            memcpy((char *)value,cli->responseHeaderMap[option],strlen(cli->responseHeaderMap[option])+1);
            *len = strlen(cli->responseHeaderMap[option]);
            return (0);
        }
    }
    return (HTTPClient_EWRONGAPIPARAMETER);
}

/*
 *  ======== HTTPClient_setOpt ========
 */

int16_t HTTPClient_setOpt(HTTPClient_Handle client, uint32_t option, void *value, uint32_t len, uint32_t flags)
{
    HTTPClient_CB *cli = (HTTPClient_CB *)client;

    if (cli == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    if (value != NULL)
    {
        switch (option) {
            case HTTPClient_REDIRECT_FEATURE:
                if (*(uint8_t *)value)
                {
                    setCliState(cli,ENABLE_REDIRECT_STATE,true);
                }
                else
                {
                    setCliState(cli,ENABLE_REDIRECT_STATE,false);
                }
                break;
            case HTTPClient_RESPONSE_FILTER_CLEAR:
                if (*(uint8_t *)value)
                {
                    cli->responseHFieldFilter |= RESPONSE_HFIELDS_ENABLED;
                }
                break;
            case HTTPClient_REDIRECT_TLS_DOWNGRADE:
                if (*(uint8_t *)value)
                {
                    setCliState(cli,ENABLE_TLS_DOWNGRADE_STATE,true);
                }
                else
                {
                    setCliState(cli,ENABLE_TLS_DOWNGRADE_STATE,false);
                }
                break;
            case HTTPClient_AUTHENTICATE_SERVER_CALLBACK:
                cli->httpAuthCallback = (HTTPClient_authenticationCallback)value;
                break;
            default:
                return (HTTPClient_EWRONGAPIPARAMETER);
            }
    }
    else
    {
        return (HTTPClient_ENULLPOINTER);
    }
    return(0);
}

/*
 *  ======== HTTPClient_getOpt ========
 */

int16_t HTTPClient_getOpt(HTTPClient_Handle client, uint32_t option, void* value, uint32_t *len ,uint32_t flags)
{
    HTTPClient_CB * cli = (HTTPClient_CB *)client;

    if (cli == NULL)
    {
        return(HTTPClient_ENULLPOINTER);
    }
    return(0);

}
/*
 *  ======== HTTPClient_setProxy ========
 */
void HTTPClient_setProxy(const struct SlNetSock_Addr_t *addr)
{
    if (addr != NULL)
    {
        proxyAddr = *addr;
    }
}
