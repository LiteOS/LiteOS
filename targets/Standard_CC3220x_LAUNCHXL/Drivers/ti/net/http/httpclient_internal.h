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

#ifndef ti_net_http_HTTPClient_internal__include
#define ti_net_http_HTTPClient_internal__include

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <httpclient.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  Modify the HTTP client object's internal buffer length and rebuild the
 *  library if needed.
 */
#ifndef HTTPClient_BUF_LEN
#define HTTPClient_BUF_LEN                          (256)
#endif

/* HTTP Standart ports */
#define HTTPClient_MAX_PORT_LEN                     (6)

 /* HTTP Standart prefix */
#define HTTPClient_HTTP_PREFIX                      "http://"
#define HTTPClient_HTTPS_PREFIX                     "https://"

/* HTTP line-end patterns */
#define HTTPClient_crlf                             "\r\n"
#define HTTPClient_crlfcrlf                         "\r\n\r\n"

 /* HTTP client instance state flags */
#define HEADER_INCOMPLETE_STATE                     (0x001)
#define ENABLE_REDIRECT_STATE                       (0x002)
#define CHUNKED_STATE                               (0x004)
#define CHUNKED_REQ_STATE                           (0x010)
#define IGNORE_PROXY_STATE                          (0x020)
#define CONNECTED_STATE                             (0x040)
#define ISSECURED_STATE                             (0x080)
#define LOCATION_ADDED_STATE                        (0x100)
#define TRAILER_STATE                               (0x200)
#define ENABLE_TLS_DOWNGRADE_STATE                  (0x400)

/* URI Parser flags */
#define ISDOMAIN                                    (0x01)
#define ISIPV6                                      (0x02)
#define ISSECURED                                   (0x04)
#define TLSSTARTED                                  (0x08)
#define ISDEFAULTPORT                               (0x10)
#define CONNECTURI_PARSER_FLAGS (ISDOMAIN | ISIPV6 | ISSECURED | TLSSTARTED | ISDEFAULTPORT)

/* Set/Get  Opt flags  */
#define ISHEADER                                    (0x1)
#define ISREQUEST                                   (0x2)

/* Redirect locationParser flag
   Location value have different host */
#define HOST_CHANGED                                (0x8)

/* Req header fields send flags */
#define ONE_REQ                                     (0x4)/* Will send specific request header field */
#define ALL_REQ_EXCEPT                              (0x8)/* Will send all header fields expect the specified */

/* sprsend flags */
#define SENDBUF                                     (0x1)/* Send the internal cli->buf to the server */

/* Response header manager flags */
#define INCOMPLETE_LOCATION                         (0x1)
#define INCOMPLETE_VALUE                            (0x2)
#define RECV_DATA                                   (0x4)

/* ReadHeader function returns */
#define NO_MORE_HEADERS                             (0)
#define PARSE_AND_CONTINUE                          (1)
#define LAST_HEADER_INCOMPLETE                      (3)
#define CONTINUE_AND_PULL                           (4)

/* Header hash return code when the header is longer than MAX_HEADER_LEN */
#define HEADER_NOT_SUPPORTED                        (-1)

/* Max hash table size for responses */
#define MAX_HASH_TABLE_SIZE                         (80)
/* Www-Authenticate index in the hash table */
#define WWW_AUTHENTICATE_HASH_INDEX                 (24)
/* Max length of response header-field name */
#define MAX_HEADER_LEN                              (18)
/* All response header-fields are filtered in  */
#define RESPONSE_HFIELDS_ENABLED                    (0xFFFFFFFF)
/* All response header-fields are filtered out */
#define RESPONSE_HFIELDS_DISABLED                   (0x00000000)

 typedef struct Req_HField
 {
    uint32_t Name;
    char *Value;
    char *CustomName;
    struct Req_HField *Next;
 }Req_HField;


 /* HTTP Client  Control Block */
 typedef struct HTTPClient_CB {
    /* Client state */
    uint32_t state;
    /* Length of recieved body , Chunked or Not */
    uint32_t bodyLen;
    /* Open socket */
    int16_t ssock;
    /* Internal buffer */
    char buf[HTTPClient_BUF_LEN];
    /* Pointer to the end of the valid data in the internal buffer */
    char *validBufEnd;
    /* Internal buffer iterator which points to the valid data */
    char *validBufStart;
    /* Names of the files need for secure connection TLS */
    HTTPClient_extSecParams *secParams;
    /* Non persistent request header fields */
    Req_HField * reqHField;
    /* Persistent request header fields */
    Req_HField * reqHFieldPers;
    /* Array of pointers of the response headers */
    char * responseHeaderMap[HTTPClient_MAX_RESPONSE_HEADER_FILEDS];
    /* Buffer for storing response values */
    char * responseHFieldValue;
    /* Points to the beginning of the free part of the response buffer */
    char * responseHFieldIterator;
    /* Free size left in the response buffer */
    uint32_t  responseHFieldFreeSize;
    /* Bitmap for filtering which response is stored */
    uint32_t  responseHFieldFilter;
    /* Bitmap for indicating which filtered response is persistent */
    uint32_t  responseHFieldFilterPersist;
    /* Callback for server authentication */
    HTTPClient_authenticationCallback httpAuthCallback;
 } HTTPClient_CB;

/* Macro for retrieving Header-Field string */
#define HeaderToStr(x) (headerFieldStr[(x) & (~HTTPClient_REQUEST_HEADER_MASK)])
/* Macro for  switching from number to binary */
#define NumberToBitField(x) (1<<(x))

#ifdef __cplusplus
}
#endif

#endif
