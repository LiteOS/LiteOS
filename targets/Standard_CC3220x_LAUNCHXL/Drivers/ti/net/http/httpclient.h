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
/**
 *  @file  ti/net/http/httpclient.h
 *
 *  @addtogroup ti_net_http_HTTPClient HTTP Client
 *
 *  @brief      The HTTP client provides APIs to connect to, and
 *              communicate with, HTTP Servers.
 *
 *  ## Library Usage ##
 *
 *  To use the HTTPClient APIs, the application should include its header file
 *  as follows:
 *  @code
 *  #include <ti/net/http/httpclient.h>
 *  @endcode
 *
 *  And, add the following HTTP library to the link line:
 *  @code
 *  .../source/ti/net/http/{toolchain}/{isa}/httpclient_{profile}.a
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_net_http_HTTPClient__include
#define ti_net_http_HTTPClient__include

/*! @ingroup ti_net_http_HTTPClient */
/*@{*/

#include <stdbool.h>

#include <ti/net/slnetsock.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HTTPClient_DOMAIN_BUFLEN                         (100)

/** Size of the buffer which is used to store response values */
#define HTTPClient_RES_HFIELD_BUFFER_SIZE                (300)

/** Minimum workspace size user should alloc in order to create client handle */
#define HTTPClient_MIN_USER_WORKSPACE_SIZE               (500)
#define HTTPClient_MAX_REDIRECTION_ATTEMPTS              (3)
#define HTTPClient_MAX_REQUEST_ATTEMPTS                  (2)
#define HTTPClient_MAX_RESPONSE_HEADER_FILEDS            (25)

#define HTTPClient_PORT                                  (80)
#define HTTPClient_SECURE_PORT                           (443)

/* HTTP response header fields */
#define HTTPClient_HFIELD_RES_AGE                        (0)
#define HTTPClient_HFIELD_RES_ALLOW                      (1)
#define HTTPClient_HFIELD_RES_CACHE_CONTROL              (2)
#define HTTPClient_HFIELD_RES_CONNECTION                 (3)
#define HTTPClient_HFIELD_RES_CONTENT_ENCODING           (4)
#define HTTPClient_HFIELD_RES_CONTENT_LANGUAGE           (5)
#define HTTPClient_HFIELD_RES_CONTENT_LENGTH             (6)
#define HTTPClient_HFIELD_RES_CONTENT_LOCATION           (7)
#define HTTPClient_HFIELD_RES_CONTENT_RANGE              (8)
#define HTTPClient_HFIELD_RES_CONTENT_TYPE               (9)
#define HTTPClient_HFIELD_RES_DATE                       (10)
#define HTTPClient_HFIELD_RES_ETAG                       (11)
#define HTTPClient_HFIELD_RES_EXPIRES                    (12)
#define HTTPClient_HFIELD_RES_LAST_MODIFIED              (13)
#define HTTPClient_HFIELD_RES_LOCATION                   (14)
#define HTTPClient_HFIELD_RES_PROXY_AUTHENTICATE         (15)
#define HTTPClient_HFIELD_RES_RETRY_AFTER                (16)
#define HTTPClient_HFIELD_RES_SERVER                     (17)
#define HTTPClient_HFIELD_RES_SET_COOKIE                 (18)
#define HTTPClient_HFIELD_RES_TRAILER                    (19)
#define HTTPClient_HFIELD_RES_TRANSFER_ENCODING          (20)
#define HTTPClient_HFIELD_RES_UPGRADE                    (21)
#define HTTPClient_HFIELD_RES_VARY                       (22)
#define HTTPClient_HFIELD_RES_VIA                        (23)
#define HTTPClient_HFIELD_RES_WWW_AUTHENTICATE           (24)
#define HTTPClient_HFIELD_RES_WARNING                    (25)

#define HTTPClient_REQUEST_HEADER_MASK                   (0x80000000)

/* HTTP request header fields */
#define HTTPClient_HFIELD_REQ_ACCEPT                     (26                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_ACCEPT_CHARSET             (27                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_ACCEPT_ENCODING            (28                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_ACCEPT_LANGUAGE            (29                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_ALLOW                      (HTTPClient_HFIELD_RES_ALLOW               | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_AUTHORIZATION              (30                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CACHE_CONTROL              (HTTPClient_HFIELD_RES_CACHE_CONTROL       | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CONNECTION                 (HTTPClient_HFIELD_RES_CONNECTION          | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CONTENT_ENCODING           (HTTPClient_HFIELD_RES_CONTENT_ENCODING    | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CONTENT_LANGUAGE           (HTTPClient_HFIELD_RES_CONTENT_LANGUAGE    | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CONTENT_LOCATION           (HTTPClient_HFIELD_RES_CONTENT_LOCATION    | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_CONTENT_TYPE               (HTTPClient_HFIELD_RES_CONTENT_TYPE        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_COOKIE                     (31                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_DATE                       (HTTPClient_HFIELD_RES_DATE                | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_EXPECT                     (32                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_FORWARDED                  (33                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_FROM                       (34                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_HOST                       (35                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_IF_MATCH                   (36                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_IF_MODIFIED_SINCE          (37                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_IF_NONE_MATCH              (38                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_IF_RANGE                   (39                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_IF_UNMODIFIED_SINCE        (40                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_ORIGIN                     (41                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_PROXY_AUTHORIZATION        (42                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_RANGE                      (43                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_TE                         (44                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_TRANSFER_ENCODING          (HTTPClient_HFIELD_RES_TRANSFER_ENCODING   | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_UPGRADE                    (HTTPClient_HFIELD_RES_UPGRADE             | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_USER_AGENT                 (45                                        | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_VIA                        (HTTPClient_HFIELD_RES_VIA                 | HTTPClient_REQUEST_HEADER_MASK)
#define HTTPClient_HFIELD_REQ_WARNING                    (HTTPClient_HFIELD_RES_WARNING             | HTTPClient_REQUEST_HEADER_MASK)

#define HTTPClient_MAX_NUMBER_OF_HEADER_FIELDS           (46)

/* SetOpt options */
/** Enable / Disable redirect feature */
#define HTTPClient_REDIRECT_FEATURE                      (48)

/** Clear response filter to default(all enabled) */
#define HTTPClient_RESPONSE_FILTER_CLEAR                 (49)

/** Enable / Disable  the option for tls downgrade */
#define HTTPClient_REDIRECT_TLS_DOWNGRADE                (50)

/**
 *  @brief      Attaches callback which will handle the server authentication
 *              response (401)
 */
#define HTTPClient_AUTHENTICATE_SERVER_CALLBACK          (51)

/* HTTP Client Error Codes */

/*!
 *  @brief Internal send buffer is not big enough
 *
 *  Define HTTPClient_BUF_LEN in httpclient_internal.h and rebuild the library.
 */
#define HTTPClient_ESENDBUFSMALL                         (-3001)

/*!
 *  @brief Buffer inserted into HTTPClient_getOpt() is not big enough.
 */
#define HTTPClient_EGETOPTBUFSMALL                       (-3002)

/*!
 *  @brief Response recieved from the server is not a valid HTTP/1.1 or HTTP/1.0 response
 */
#define HTTPClient_ERESPONSEINVALID                      (-3003)

/*!
 *  @brief Operation could not be completed. Try again.
 */
#define HTTPClient_EINPROGRESS                           (-3004)

/*!
 *  @brief Input domain name length is too long to be read into buffer.
 *
 *  Modify the HTTPClient_DOMAIN_BUFLEN and rebuild the library.
 *
 *  @sa HTTPClient_DOMAIN_BUFLEN
 */
#define HTTPClient_EDOMAINBUFSMALL                       (-3005)

/*!
 *  @brief Allocation failed during the CB creation.
 *
 *  Check whether there is free memory for CB allocation.
 */
#define HTTPClient_ECBALLOCATIONFAILED                   (-3006)

/*!
 *  @brief Body size is too small.
 *
 *  Body which was entered into the request was too small,
 *  This error won't occur if the body size will be at least
 *  HTTPClient_BUF_LEN.
 */
#define HTTPClient_EBODYBUFSMALL                         (-3008)

/*!
 *  @brief Invalid de-referencing a NULL pointer.
 *
 *  Check that a NULL pointer wasn't dereferenced in the function.
 */
#define HTTPClient_ENULLPOINTER                          (-3009)

/*!
 *  @brief Request header allocation failed.
 */
#define HTTPClient_EREQUESTHEADERALLOCFAILED             (-3010)

/*!
 *  @brief Request header wasn't found in the req header list.
 */
#define HTTPClient_EREQHEADERNOTFOUND                    (-3011)

/*!
 *  @brief Host request header wasn't found.
 *
 *  Host header-field is mandatory.
 */
#define HTTPClient_EHOSTNOTFOUND                         (-3012)

/*!
 *  @brief Client is already connected.
 */
#define HTTPClient_ECLIENTALREADYCONNECTED               (-3013)

/*!
 *  @brief Response is not redirectable.
 */
#define HTTPClient_ERESPONSEISNOTREDIRECTABLE            (-3014)

/*!
 *  @brief Send couldn't be completed.
 */
#define HTTPClient_ESENDERROR                            (-3015)

/*!
 *  @brief Location Header fields value couldn't be read completely
 *
 *  Since the "Location" header-field is essential for the redirect mechanism
 *  it is imposible to complete the redirect without it.
 *  This error indicates that the buffer for the Location header field value
 *  was too small. Enlarge HTTPClient_BUF_LEN in order to fix
 *  this error, or disable the redirect mechanism.
 */
#define HTTPClient_EREDIRECTLOCATIONFAIL                 (-3016)

/*!
 *  @brief TLS downgrade is forbidden.
 *
 *  This error indicates that after redirect occured, the server asked
 *  the client to redirect from secured server into non-secured server.
 *  This option is forbidden by default, to enable use HTTPClient_setOpt()
 */
#define HTTPClient_ETLSDOWNGRADEISFORBIDDEN              (-3017)

/*!
 *  @brief Wrong API parameter.
 *
 *  Wrong parameter entered into the API.
 */
#define HTTPClient_EWRONGAPIPARAMETER                    (-3018)

/*!
 *  @brief HOST already exist.
 *
 *  Host header is already exist, having multiple
 *  Host headers is invalid. Remove the existing host
 *  Header.
 */
#define HTTPClient_EHOSTHEADERALREADYEXIST               (-3019)

/*!
 *  @brief Client is disconnected.
 *
 *  SlNetSock_recv return 0 which means the client is disconnected.
 */
#define HTTPClient_ENOCONNECTION                         (-3020)

/*!
 *  @brief URI is not absolute
 *
 *  When Host header is not found, reconnection can be established only
 *  with absolute URI.
 */
#define HTTPClient_ENOTABSOLUTEURI                       (-3021)

/*!
 *  @brief Error during creation of security attribute
 *
 *  Error occured during the creationg of security attribue
 */
#define HTTPClient_ECANTCREATESECATTRIB                  (-3022)

 /* HTTP Status Codes */
 enum HTTP_STATUS_CODE {
    HTTP_SC_CONTINUE                          = 100, /*!< Informational */
    HTTP_SC_SWITCHING_PROTOCOLS               = 101, /*!< Informational */

    HTTP_SC_OK                                = 200, /*!< Success */
    HTTP_SC_CREATED                           = 201, /*!< Success */
    HTTP_SC_ACCEPTED                          = 202, /*!< Success */
    HTTP_SC_NON_AUTHORITATIVE_INFORMATION     = 203, /*!< Success */
    HTTP_SC_NO_CONTENT                        = 204, /*!< Success */
    HTTP_SC_RESET_CONTENT                     = 205, /*!< Success */
    HTTP_SC_PARTIAL_CONTENT                   = 206, /*!< Success */
    HTTP_SC_MULTI_STATUS                      = 207, /*!< Success */

    HTTP_SC_MULTIPLE_CHOICES                  = 300, /*!< Redirection */
    HTTP_SC_MOVED_PERMANENTLY                 = 301, /*!< Redirection */
    HTTP_SC_FOUND                             = 302, /*!< Redirection */
    HTTP_SC_SEE_OTHER                         = 303, /*!< Redirection */
    HTTP_SC_NOT_MODIFIED                      = 304, /*!< Redirection */
    HTTP_SC_TEMPORARY_REDIRECT                = 307, /*!< Redirection */

    HTTP_SC_BAD_REQUEST                       = 400, /*!< Client Error */
    HTTP_SC_UNAUTHORIZED                      = 401, /*!< Client Error */
    HTTP_SC_PAYMENT_REQUIRED                  = 402, /*!< Client Error */
    HTTP_SC_FORBIDDEN                         = 403, /*!< Client Error */
    HTTP_SC_NOT_FOUND                         = 404, /*!< Client Error */
    HTTP_SC_METHOD_NOT_ALLOWED                = 405, /*!< Client Error */
    HTTP_SC_NOT_ACCEPTABLE                    = 406, /*!< Client Error */
    HTTP_SC_PROXY_AUTHENTICATION_REQUIRED     = 407, /*!< Client Error */
    HTTP_SC_REQUEST_TIMEOUT                   = 408, /*!< Client Error */
    HTTP_SC_CONFLICT                          = 409, /*!< Client Error */
    HTTP_SC_GONE                              = 410, /*!< Client Error */
    HTTP_SC_LENGTH_REQUIRED                   = 411, /*!< Client Error */
    HTTP_SC_PRECONDITION_FAILED               = 412, /*!< Client Error */
    HTTP_SC_REQUEST_ENTITY_TOO_LARGE          = 413, /*!< Client Error */
    HTTP_SC_REQUEST_URI_TOO_LONG              = 414, /*!< Client Error */
    HTTP_SC_UNSUPPORTED_MEDIA_TYPE            = 415, /*!< Client Error */
    HTTP_SC_REQUESTED_RANGE_NOT_SATISFAIABLE  = 416, /*!< Client Error */
    HTTP_SC_EXPECTATION_FAILED                = 417, /*!< Client Error */
    HTTP_SC_UNPROCESSABLE_ENTITY              = 422, /*!< Client Error */
    HTTP_SC_UPGRADE_REQUIRED                  = 426, /*!< Client Error */
    HTTP_SC_TOO_MANY_REQUESTS                 = 429, /*!< Client Error */

    HTTP_SC_INTERNAL_SERVER_ERROR             = 500, /*!< Server Error */
    HTTP_SC_NOT_IMPLEMENTED                   = 501, /*!< Server Error */
    HTTP_SC_BAD_GATEWAY                       = 502, /*!< Server Error */
    HTTP_SC_SERVICE_UNAVAILABLE               = 503, /*!< Server Error */
    HTTP_SC_GATEWAY_TIMEOUT                   = 504, /*!< Server Error */
    HTTP_SC_HTTP_VERSION_NOT_SUPPORTED        = 505, /*!< Server Error */

    HTTP_SC_STATUS_CODE_END                   = 600
};


/* HTTPClient_connect flags */
/** If proxy is set, this flag makes the connection without the proxy */
#define HTTPClient_IGNORE_PROXY                (0x01)

/* If the user already added "Host:" header, set this flag */
#define HTTPClient_HOST_EXIST                  (0x02)

/* HTTPClient_setHeader flags */
/** Header Field added is not persistent */
#define HTTPClient_HFIELD_NOT_PERSISTENT       (0x01)

/** Header Field added is persistent */
#define HTTPClient_HFIELD_PERSISTENT           (0x02)

/* HTTPClient_sendRequest flags */
/** Sets the client's request state into chunked body */
#define HTTPClient_CHUNK_START                 (0x01)

/** Sets the client's request state out of chunked body and sends last chunk */
#define HTTPClient_CHUNK_END                   (0x02)

/** Flushes the response body */
#define HTTPClient_DROP_BODY                   (0x04)


/* HTTP Request Methods */
extern const char *HTTP_METHOD_GET;
extern const char *HTTP_METHOD_POST;
extern const char *HTTP_METHOD_HEAD;
extern const char *HTTP_METHOD_OPTIONS;
extern const char *HTTP_METHOD_PUT;
extern const char *HTTP_METHOD_DELETE;
extern const char *HTTP_METHOD_CONNECT;

/* structure user need to use for security */
typedef struct HTTPClient_extSecParams
{
    const char *privateKey; /* Private key */
    const char *clientCert; /* Client certificate */
    const char *rootCa; /* Root CA */
}HTTPClient_extSecParams;

/*!
    \brief  HTTPClient callback for server authentication handling
            This function is responsible for generating the value for the
            authorization request header.

    \param[in]    serverAuthBuff    Buffer containing authentication information (Www-Authenticate response header)
                                    received from server.

    \param[in]    serverAuthLen     Server authentication buffer length.

    \param[out]   clientAuthBuff    Buffer containing Authorization request header.

    \param[inout] clientAuthLen     Input is the maximum size of the client Authorization request header buffer (HTTPClient_BUF_LEN),
                                    Output is the actual size of the client Authorization request header buffer.

    \return 0 on success or error code on failure.
 */

typedef int16_t (*HTTPClient_authenticationCallback)(const char *serverAuthBuff, uint32_t serverAuthLen, char *clientAuthBuff, uint32_t *clientAuthLen);

typedef void *HTTPClient_Handle;

/*!
    \brief  Allocate and initialize a new HTTPClient instance object and return its handle.

    \param[out]   status    pointer to return status

    \param[in]    params    Special parameters for creating the instance
                            (currently there are no special parameters)

    \return handle on success or NULL failure.
 */
HTTPClient_Handle HTTPClient_create(int16_t * status, void *params);

/*!
    \brief  Destroy the HTTP client instance and free the previously allocated
            instance object.

    \param[in]  client Pointer to the HTTP client instance

    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_destroy(HTTPClient_Handle client);

/*!
    \brief  Open a connection to an HTTP server.
            A user can connect to a HTTP server using TLS,proxy or both.

    \param[in]  client         Instance of an HTTP client

    \param[in]  hostName       IP address or URL of the HTTP server.

    \param[in]  exSecParams    external parameters for configuring security.

    \param[in]  flags          Special flags for connection:
                               - #HTTPClient_IGNORE_PROXY -  Ignore the proxy even if set.
                               - #HTTPClient_HOST_EXIST   -  Host header was added manually, HTTPClient_connect() won't
                                                             add host internally.
    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_connect(HTTPClient_Handle client, const char *hostName, HTTPClient_extSecParams *exSecParams, uint32_t flags);

/*!
    \brief  Disconnect from the HTTP server.

    \param[in]  client Instance of the HTTP client

    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_disconnect(HTTPClient_Handle client);


/*!
    \brief  Make an HTTP request to the HTTP server

    Sends an HTTP request-line, header fields and body to the requested URI.
    After sending the request, the request function waits for the response Status
    and Header-Fields.
    According to the response status, the request function determines whether to
    return to user or to call a redirect/callback pre-defined function.


    \param[in]  client     Instance of an HTTP client.

    \param[in]  method     HTTP method.

    \param[in]  requestURI The path on the server to open.

    \param[in]  body       The body the user wishes to send in in the request,
                           The body can be chunked or one body buffer.

    \param[in]  bodyLen    Length of the body sent in the request.

    \param[in]  flags      Special flags when the user wishes not to use the
                           default settings.
                           - #HTTPClient_CHUNK_START - First request body chunk.
                           - #HTTPClient_CHUNK_END - Last request body chunk.
                           - #HTTPClient_DROP_BODY - only keep the status code
                             and response headers, the response body will be
                             dropped.

    \note       - If user wishes to use TLS connection then before calling
                  HTTPClient_sendRequest(), HTTPClient_connect() should be
                  called.
                - If disconnection happened prior to HTTPClient_sendRequest(),
                  HTTPClient_sendRequest() will reconnect internally.
                - When sending a body in a request, the "Content-length: " and
                  "Transfer-Encoding: Chunked" headers will be added
                  automatically.

    \return     Response status code on success or error code on failure.
 */
int16_t HTTPClient_sendRequest(HTTPClient_Handle client, const char *method,const char *requestURI, const char *body, uint32_t bodyLen, uint32_t flags);

/*!
    \brief  Read the response body data from the HTTP server
            The function handles both oneshot and chunked body responses

            Make a call to this function only after the call to
            HTTPClient_sendRequest().

    \param[in]     client        Instance of an HTTP client

    \param[out]    body          Response body buffer

    \param[in]     bodyLen       Length of response body buffer

    \param[out]    moreDataFlag  Set if more data is available

    \note   Make a call to this function only after the call to
            HTTPClient_sendRequest(). This function need to be called until
            @c moreDataFlag will be false(All data was read).

    \return The number of characters read on success or error code on failure
 */
int16_t HTTPClient_readResponseBody(HTTPClient_Handle client, char *body, uint32_t bodyLen, bool *moreDataFlag);

/*!
    \brief  Setting HTTP Client Header-field configurations.

    \param[in]     client   Instance of an HTTP client

    \param[in]     option   Options for setting could be one of the following:
                             -Header-Fields ID -
                                 Request -  headers - sets the headers-fields which will be used in requests.
                                 Response - headers - sets the headers-fields wanted to be filtered in a response.
                                                  (if no request headers are set, all the headers will be available with
                                                   size constraints)

    \param[in]     value    Value for setting could be any related value for
                            the corresponding option.
                            Value can be set to NULL when request header wanted to be removed.

    \param[in]     len       Length of the value.

    \param[in]     flags    Flags for settings need be one of the following:
                              - #HTTPClient_HFIELD_NOT_PERSISTENT - Header-Field is not persistent.
                              - #HTTPClient_HFIELD_PERSISTENT - Header-Field is persistent.

    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_setHeader(HTTPClient_Handle client, uint32_t option, void *value, uint32_t len, uint32_t flags);

/*!
    \brief  Setting HTTP Client Header-field configurations by header name.
            Both standard (as defined by the HTTP RFC spec) and non-standard
            header names are supported.
            This API currently only supports request headers.

            When a given standard HTTP header is set, it is important to
            consistently set it using one of HTTPClient_setHeaderByName() or
            HTTPClient_setHeader(). The ordering of the values may not be
            preserved if both APIs are used to set the same header.

    \param[in]     client   Instance of an HTTP client

    \param[in]     option   Options for setting could be one of the following:
                                 HTTPClient_REQUEST_HEADER_MASK - sets a header-field which will be used in requests.

    \param[in]     name     Name of header. Must be NULL-terminated.

    \param[in]     value    Value for setting could be any related value for
                            the corresponding header.

    \param[in]     len      Length of the value.

    \param[in]     flags    Flags for settings need be one of the following:
                              - #HTTPClient_HFIELD_NOT_PERSISTENT - Header-Field is not persistent.
                              - #HTTPClient_HFIELD_PERSISTENT - Header-Field is persistent.

    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_setHeaderByName(HTTPClient_Handle client, uint32_t option, const char *name, void *value, uint32_t len, uint32_t flags);

/*!
    \brief  Getting HTTP Client Header-field configurations.

    \param[in]     client   Instance of an HTTP client

    \param[in]     option   Options for getting could be one of the following:
                            -Header-Fields ID
                                Response - headers - getting response headers-field value (only if value was received in a filtered response).

    \param[out]    value    Value for getting, could be any related value for
                            the corresponding option.

    \param[inout]  len      Inputs Length of the value and output the actual length.

    \param[in]     flags    Flags for getting special configurations.

    \return 0 on success or error code on failure.
 */

int16_t HTTPClient_getHeader(HTTPClient_Handle client, uint32_t option, void *value, uint32_t *len, uint32_t flags);

/*!
    \brief  Setting HTTP Client configurations.

    \param[in]     client   Instance of an HTTP client

    \param[in]     option   Options for setting could be one of the following:
                            -Client instance parameters:
                                  - #HTTPClient_REDIRECT_FEATURE       - Enable(true) or disable(false) the redirect feature (Enabled by default).
                                  - #HTTPClient_RESPONSE_FILTER_CLEAR  - Clears(true) the filter for the responses to default (Nothing filtered).
                                  - #HTTPClient_REDIRECT_TLS_DOWNGRADE - Enable(true) or disable(false) the ability to downgrade tls by redirect (disabled by default).
                                  - #HTTPClient_AUTHENTICATE_SERVER_CALLBACK - Register callback for handling server authentication (see HTTPClient_authenticationCallback())
                                                                             value for this option is the function to register.
    \param[in]     value    Value for setting could be any related value for
                            the corresponding option.

    \param[in]     len      Length of the value.

    \param[in]     flags    Flags for settings special configurations.

    \return 0 on success or error code on failure.
 */
int16_t HTTPClient_setOpt(HTTPClient_Handle client, uint32_t option, void *value, uint32_t len, uint32_t flags);

/*!
    \brief  Getting HTTP Client configurations.

    \param[in]     client   Instance of an HTTP client

    \param[in]     option   Options for getting client settings.

    \param[out]    value    Value for getting, could be any related value for
                            the corresponding option.

    \param[inout]  len      Inputs Length of the value and output the actual length.

    \param[in]     flags    Flags for getting special configurations

    \note          Currently there are no client configurations to get.

    \return 0 on success or error code on failure.
 */

int16_t HTTPClient_getOpt(HTTPClient_Handle client, uint32_t option, void *value, uint32_t *len ,uint32_t flags);

/*!
    \brief Set the proxy address

    \param[in]  addr IP address of the proxy server
 */
void HTTPClient_setProxy(const SlNetSock_Addr_t *addr);

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
