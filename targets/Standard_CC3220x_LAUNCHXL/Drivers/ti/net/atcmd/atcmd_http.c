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

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/http/httpclient.h>
#include <ti/net/utils/str_mpl.h>

/* AT Header files */
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd.h>
#include <ti/net/atcmd/atcmd_http.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd_event.h>

#include <pthread.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDHTTP_CONNECT_MAX_HOST_NAME         (430)

//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmdHttp_Connect_t_
{
    char *hostName;
    uint32_t flags;
    HTTPClient_extSecParams exSecParams;
}ATCmdHttp_Connect_t;

typedef struct _ATCmdHttp_SendReq_t_
{
    char *method;
    char *requestURI;
    uint32_t flags;
    uint8_t bodyFormat;
    uint32_t bodyLen;
    char *body;
}ATCmdHttp_SendReq_t;

typedef struct _ATCmdHttp_RespBody_t_
{
    uint32_t index;
    uint8_t moreDataflag;
    uint8_t format;
    uint32_t len;
    char *body;
}ATCmdHttp_RespBody_t;

typedef struct _ATCmdHttp_Header_t_
{
    uint32_t index;
    uint32_t option;
    uint32_t flags;
    uint8_t format;
    uint32_t len;
    void *value;
}ATCmdHttp_Header_t;

typedef struct _ATCmdHttp_Opt_t_
{
    uint32_t option;
    uint32_t flags;
    uint32_t len;
    uint32_t value;
}ATCmdHttp_Opt_t;

typedef struct _ATCmdHttp_Proxy_t_
{
    SlNetSock_Addr_t addr;
    int8_t           padding[12];
}ATCmdHttp_Proxy_t;

//*****************************************************************************
// global variables
//*****************************************************************************
HTTPClient_Handle ATCmdHttp_mapHanlde[SL_MAX_SOCKETS] = {0};

//*****************************************************************************
// HTTP AT Commands Routines
//*****************************************************************************

/* 
    Find available client index
*/
int32_t ATCmdHttp_findAvailableIndex(void)
{
    uint8_t index = 0;

    while ((ATCmdHttp_mapHanlde[index] != 0) && (index < SL_MAX_SOCKETS))
    {
        index++;
    }
    if (index == SL_MAX_SOCKETS)
    {
        return -1;
    }
    return index;
}

/* 
    Parse client index and get the corresponding handle
*/
HTTPClient_Handle ATCmdHttp_clientIndexParse(char **buff, uint32_t *index, char delim)
{    
    HTTPClient_Handle handle;

    /* Mode */
    if (StrMpl_getVal(buff, index ,delim, STRMPL_FLAG_PARAM_SIZE_32) < 0)
    {
        return NULL;
    }
    if (*index >= SL_MAX_SOCKETS)
    {
        return NULL;
    }
    /* Get client handle */
    handle = ATCmdHttp_mapHanlde[*index];

    return handle;
}


/*
    Compose and send HTTP client create result.
*/
int32_t ATCmdHttp_createResult(void *args, int32_t num, char *buff)
{
    
    StrMpl_setStr(ATCmd_httpCreateStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&num,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);

    return 0;
}


/*
    HTTP client create callback.
*/
int32_t ATCmdHttp_createCallback(void *arg)
{
    int16_t ret;
    HTTPClient_Handle handle;
    int32_t index;
    
    handle = HTTPClient_create(&ret, NULL);
	
	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
	    return -1;
    }
    else
    {
        index = ATCmdHttp_findAvailableIndex();
        if (index < 0)
        {
            ATCmd_errorResult(ATCmd_errorCmdStr,ret);
	        return -1;
        }
        else
        {
            ATCmdHttp_mapHanlde[index] = handle;
            ATCmd_commandResult(ATCmdHttp_createResult,NULL,index);
            ATCmd_okResult();
        }
    }
    return 0;
}

/*
    HTTP client destroy callback.
*/
int32_t ATCmdHttp_destroyCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    HTTPClient_Handle handle;
    
    /* parse client index */
    handle = ATCmdHttp_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_TRM);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }

    ret = HTTPClient_destroy(handle);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmdHttp_mapHanlde[index] = 0;
        ATCmd_okResult();
    }
    return ret;
}

/*
    Free allocated memory for HTTP client connect command
*/
int32_t ATCmdHttp_connectFree(ATCmdHttp_Connect_t *params)
{    

    if (params->hostName != NULL)
    {
        free(params->hostName);
    }
    if (params->exSecParams.clientCert != NULL)
    {
        free((void *)params->exSecParams.clientCert);
    }
    if (params->exSecParams.privateKey != NULL)
    {
        free((void *)params->exSecParams.privateKey);
    }
    if (params->exSecParams.rootCa != NULL)
    {
        free((void *)params->exSecParams.rootCa);
    }
           
    return 0;
}

/*
    Parse HTTP client connect command.
*/
int32_t ATCmdHttp_connectParse(char *buff, ATCmdHttp_Connect_t *params)
{
    int32_t ret = 0;

    /* host name */
    if ((ret = StrMpl_getAllocStr(&buff, &params->hostName, ATCMD_DELIM_ARG, ATCMDHTTP_CONNECT_MAX_HOST_NAME ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }

    /* flags */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_httpConnectFlags, sizeof(ATCmd_httpConnectFlags)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
    /* Private key */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->exSecParams.privateKey, ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
    /* Client certificate */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->exSecParams.clientCert, ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
    /* Root CA */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->exSecParams.rootCa, ATCMD_DELIM_TRM, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
        ret = 0;
    }

    return ret;
}

/*
    HTTP client connect callback.
*/
int32_t ATCmdHttp_connectCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    HTTPClient_Handle handle;
    ATCmdHttp_Connect_t params;

    handle = ATCmdHttp_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    memset(&params,0,sizeof(ATCmdHttp_Connect_t));
    ret = ATCmdHttp_connectParse((char *)arg,&params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdHttp_connectFree(&params);
        return -1;
    }
    ret = HTTPClient_connect(handle,params.hostName,&params.exSecParams,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdHttp_connectFree(&params);
    return ret;
}

/*
    HTTP client disconnect callback.
*/
int32_t ATCmdHttp_disconnectCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t index;
    HTTPClient_Handle handle;
    
    handle = ATCmdHttp_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_TRM);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    ret = HTTPClient_disconnect(handle);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    return ret;
}

/*
    Free allocated memory for HTTP client send request command
*/
int32_t ATCmdHttp_sendReqFree(ATCmdHttp_SendReq_t *params)
{    
    if (params->requestURI != NULL)
    {
        free(params->requestURI);
    }

    if (params->body!= NULL)
    {
        free(params->body);
    }
           
    return 0;
}


/*
    Parse HTTP client send request command.
*/
int32_t ATCmdHttp_sendReqParse(char *buff, ATCmdHttp_SendReq_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen = 0;
    uint32_t method;

    /* method */
    if ((ret = StrMpl_getListVal(ATCmd_httpSendReqMethod, sizeof(ATCmd_httpSendReqMethod)/sizeof(StrMpl_List_t), &buff, &method, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    switch (method)
    {
        case ATCMDHTTP_METHOD_GET:
            params->method = (char *)HTTP_METHOD_GET;
            break;
        case ATCMDHTTP_METHOD_POST:
            params->method = (char *)HTTP_METHOD_POST;
            break;
        case ATCMDHTTP_METHOD_HEAD:
            params->method = (char *)HTTP_METHOD_HEAD;
            break;
        case ATCMDHTTP_METHOD_OPTIONS:
            params->method = (char *)HTTP_METHOD_OPTIONS;
            break;
        case ATCMDHTTP_METHOD_PUT:
            params->method = (char *)HTTP_METHOD_PUT;
            break;
        case ATCMDHTTP_METHOD_DELETE:
            params->method = (char *)HTTP_METHOD_DELETE;
            break;
        case ATCMDHTTP_METHOD_CONNECT:
            params->method = (char *)HTTP_METHOD_CONNECT;
            break;
    }
    /* request URI */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->requestURI), ATCMD_DELIM_ARG, 0xFFFF ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }

    /* flags */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_httpSendReqFlags, sizeof(ATCmd_httpSendReqFlags)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->bodyFormat , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
        params->bodyFormat = 0;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->bodyLen, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        if ((ret != STRMPL_ERROR_PARAM_MISSING) ||
            ((ret == STRMPL_ERROR_PARAM_MISSING) && 
            ((params->method == HTTP_METHOD_POST) ||
            (params->method == HTTP_METHOD_PUT))))
        {
            return ret;
        }
        params->bodyLen = 0;
        ret = 0;
    }

    if(params->bodyLen > 0)
    {
        /* data */
        if (params->bodyFormat == ATCMD_DATA_FORMAT_BASE64)
        {
            /* convert length to binary length */
            outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->bodyLen); 
            params->body = malloc(outputLen);
            if (params->body == NULL)
            {
                return -1;
            }
            if (StrMpl_decodeBase64((uint8_t *)buff, params->bodyLen, (uint8_t *)params->body, (uint32_t *)&outputLen) < 0)
            {
                return -1;
            }
            params->bodyLen = outputLen;        
        }
        else
        {
            params->body = malloc(params->bodyLen);
            if (params->body == NULL)
            {
                return -1;
            }

            memcpy(params->body, buff, params->bodyLen);
        }
    }
    return ret;
}

/*
    Compose and send HTTP client send request result.
*/
int32_t ATCmdHttp_sendReqResult(void *args, int32_t num, char *buff)
{
    
    StrMpl_setStr(ATCmd_httpSendReqStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&num,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);

    return 0;
}

/*
    HTTP client send request callback.
*/
int32_t ATCmdHttp_sendReqCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_SendReq_t  params;
    uint32_t index;
    HTTPClient_Handle handle;
    
    handle = ATCmdHttp_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }  
    memset(&params, 0x0, sizeof(ATCmdHttp_SendReq_t));
    
    /* Call the command parser */
    ret = ATCmdHttp_sendReqParse((char *)arg,&params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdHttp_sendReqFree(&params);        
	    return -1;
    }
    ret = HTTPClient_sendRequest(handle,params.method,params.requestURI,params.body,params.bodyLen,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_commandResult(ATCmdHttp_sendReqResult,NULL,ret);
        ATCmd_okResult();
    }

    ATCmdHttp_sendReqFree(&params);        

    return ret;
}

/*
    Free allocated memory for HTTP client read response body command
*/
int32_t ATCmdHttp_respBodyFree(ATCmdHttp_RespBody_t *params)
{    
    if (params != NULL)
    {
        if (params->body!= NULL)
        {
            free(params->body);
        }
        free(params);
    }
    return 0;
}


/*
    Parse HTTP client read response body command.
*/
int32_t ATCmdHttp_respBodyParse(char *buff, ATCmdHttp_RespBody_t *params)
{
    int32_t ret = 0;

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }

    return ret;
}

/*
    Compose and send HTTP client read response body result.
*/
int32_t ATCmdHttp_respBodyResult(void *args, int32_t num, char *buff)
{    
    ATCmdHttp_RespBody_t *params = (ATCmdHttp_RespBody_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_httpReadResBodyStr,&buff,ATCMD_DELIM_EVENT);

    /* handle index */
    StrMpl_setVal(&params->index, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    
    /* more data flag */
    StrMpl_setVal(&params->moreDataflag, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    /* length */
    StrMpl_setVal(&outputLen,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);

    /* body */
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert data to base64 */
        StrMpl_encodeBase64((uint8_t *)params->body, params->len, (uint8_t *)buff, &outputLen);
    }
    else
    {
        memcpy(buff,params->body,params->len);
    }
    ATCmdHttp_respBodyFree(params);

    return 0;
}

/*
    HTTP client read response body callback.
*/
int32_t ATCmdHttp_respBodyCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_RespBody_t  *params;
    HTTPClient_Handle handle;

    params = malloc(sizeof(ATCmdHttp_RespBody_t));
    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,ret);
	    return -1;
    }
    memset(params, 0x0, sizeof(ATCmdHttp_RespBody_t));

    handle = ATCmdHttp_clientIndexParse((char **)&arg,&params->index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }  

    
    /* Call the command parser */
    ret = ATCmdHttp_respBodyParse((char *)arg,params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdHttp_respBodyFree(params);        
	    return -1;
    }

    params->body = malloc (params->len);
    if (params->body == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdHttp_respBodyFree(params);
	    return -1;
    }

    ret = HTTPClient_readResponseBody(handle, params->body, params->len, (bool *)&params->moreDataflag);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdHttp_respBodyFree(params);        
    }
    else
    {
        params->len = ret;
        ATCmd_commandResult(ATCmdHttp_respBodyResult,params,0);
        ATCmd_okResult();
    }


    return ret;
}


/*
    Free allocated memory for HTTP client set header command
*/
int32_t ATCmdHttp_setHeaderFree(ATCmdHttp_Header_t *params)
{    
    if (params->value != NULL)
    {
        free(params->value);
    }
    return 0;
}

/*
    Parse HTTP client set header command.
*/
int32_t ATCmdHttp_setHeaderParse(char *buff, ATCmdHttp_Header_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen = 0;

    /* option */
    if ((ret = StrMpl_getListVal(ATCmd_httpHeaderOption, sizeof(ATCmd_httpHeaderOption)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    /* flags */
    if ((ret = StrMpl_getListVal(ATCmd_httpSetHeaderFlags, sizeof(ATCmd_httpSetHeaderFlags)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
        params->len = 0;
        ret = 0;
    }

    if(params->len > 0)
    {
        /* data */
        if (params->format == ATCMD_DATA_FORMAT_BASE64)
        {
            /* convert length to binary length */
            outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->len); 
            params->value = malloc(outputLen);
            if (params->value == NULL)
            {
                return -1;
            }
            if (StrMpl_decodeBase64((uint8_t *)buff, params->len, params->value, (uint32_t *)&outputLen) < 0)
            {
                return -1;
            }
            params->len = outputLen;        
        }
        else
        {
            //add 1 for the null termination
            params->value = malloc(params->len+1);
            if (params->value == NULL)
            {
                return -1;
            }

            memcpy(params->value, buff, params->len);
            ((char *)(params->value))[params->len] = '\0';
            //add 1 for the null termination
            params->len = params->len + 1;
        }
    }
    else
    {
        params->value = NULL;
    }
    return ret;
}


/*
    HTTP client set header callback.
*/
int32_t ATCmdHttp_setHeaderCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_Header_t  params;
    HTTPClient_Handle handle;
    
    handle = ATCmdHttp_clientIndexParse((char **)&arg,&params.index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    
    memset(&params, 0x0, sizeof(ATCmdHttp_Header_t));
    
    /* Call the command parser */
    ret = ATCmdHttp_setHeaderParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdHttp_setHeaderFree(&params);
	    return -1;
    }
    
    ret = HTTPClient_setHeader(handle,params.option,params.value,params.len,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdHttp_setHeaderFree(&params);

    return ret;
}

/*
    Free allocated memory for HTTP client get header command
*/
int32_t ATCmdHttp_getHeaderFree(ATCmdHttp_Header_t *params)
{    
    if (params != NULL)
    {
        if (params->value != NULL)
        {
            free(params->value);
        }
        free(params);
    }
    return 0;
}


/*
    Parse HTTP client get header command.
*/
int32_t ATCmdHttp_getHeaderParse(char *buff, ATCmdHttp_Header_t *params)
{
    int32_t ret = 0;

    /* option */
    if ((ret = StrMpl_getListVal(ATCmd_httpHeaderOption, sizeof(ATCmd_httpHeaderOption)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }

    return ret;
}

/*
    Compose and send HTTP client get header result.
*/
int32_t ATCmdHttp_getHeaderResult(void *args, int32_t num, char *buff)
{    
    ATCmdHttp_Header_t *params = (ATCmdHttp_Header_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_httpGetHeaderStr,&buff,ATCMD_DELIM_EVENT);

    /* handle index */
    StrMpl_setVal(&params->index, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    
    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    /* length */
    StrMpl_setVal(&outputLen,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);

    /* value */
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert data to base64 */
        StrMpl_encodeBase64((uint8_t *)params->value, params->len, (uint8_t *)buff, &outputLen);
    }
    else
    {
        memcpy(buff,params->value,params->len);
    }
    ATCmdHttp_getHeaderFree(params);

    return 0;
}

/*
    HTTP client get header callback.
*/
int32_t ATCmdHttp_getHeaderCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_Header_t  *params;
    HTTPClient_Handle handle;

    params = malloc(sizeof(ATCmdHttp_Header_t));
    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,ret);
	    return -1;
    }
    memset(params, 0x0, sizeof(ATCmdHttp_Header_t));

    handle = ATCmdHttp_clientIndexParse((char **)&arg,&params->index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }  

    
    /* Call the command parser */
    ret = ATCmdHttp_getHeaderParse((char *)arg,params);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdHttp_getHeaderFree(params);        
	    return -1;
    }

    params->value = malloc (params->len);
    if (params->value == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdHttp_getHeaderFree(params);
	    return -1;
    }

    ret = HTTPClient_getHeader(handle, params->option, params->value, &params->len, params->flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdHttp_getHeaderFree(params);        
    }
    else
    {
        ATCmd_commandResult(ATCmdHttp_getHeaderResult,params,0);
        ATCmd_okResult();
    }


    return ret;
}

/*
    Parse HTTP client set option command.
*/
int32_t ATCmdHttp_setOptParse(char *buff, ATCmdHttp_Opt_t *params)
{
    int32_t ret = 0;

    /* option */
    if ((ret = StrMpl_getListVal(ATCmd_httpOptOption, sizeof(ATCmd_httpOptOption)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
  
    /* value */
    if ((ret = StrMpl_getVal(&buff, &params->value, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }

    return ret;
}


/*
    HTTP client set option callback.
*/
int32_t ATCmdHttp_setOptCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_Opt_t  params;
    uint32_t index;
    HTTPClient_Handle handle;
    
    handle = ATCmdHttp_clientIndexParse((char **)&arg,&index,ATCMD_DELIM_ARG);
    if (handle == NULL)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,STRMPL_ERROR_WRONG_PARAM);
        return -1;
    }
    
    memset(&params, 0x0, sizeof(ATCmdHttp_Opt_t));
    
    /* Call the command parser */
    ret = ATCmdHttp_setOptParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
    
    ret = HTTPClient_setOpt(handle,params.option,(void *)&params.value,params.len,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    return ret;
}

/*
    Parse HTTP client set proxy command.
*/
int32_t ATCmdHttp_setProxyParse(char *buff, ATCmdHttp_Proxy_t *params)
{
    int32_t ret = 0;
    void *addr = &params->addr;
    uint8_t ip[4];

    /* family */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->addr.sa_family, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    if (params->addr.sa_family == SL_AF_INET)
    {
        /* port */
        if ((ret = StrMpl_getVal(&buff, &((SlSockAddrIn_t *)(addr))->sin_port, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn_t *)(addr))->sin_port = sl_Htons(((SlSockAddrIn_t *)(addr))->sin_port);
        /* address */
        if ((ret = StrMpl_getArrayVal(&buff,(void *)ip, 4, ATCMD_DELIM_TRM, ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn_t *)(addr))->sin_addr.s_addr = sl_Htonl(SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]));
    }
    else if (params->addr.sa_family == SL_AF_INET6)
    {
        /* port */
        if ((ret = StrMpl_getVal(&buff, &((SlSockAddrIn6_t *)(addr))->sin6_port, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn6_t *)(addr))->sin6_port = sl_Htons(((SlSockAddrIn6_t *)(addr))->sin6_port);
        /* address */
        if ((ret = StrMpl_getArrayVal(&buff, (void *)(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32), 4, ATCMD_DELIM_TRM, ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
        {
            return ret;
        }
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3]);
    }

    return ret;
}


/*
    HTTP client set proxy callback.
*/
int32_t ATCmdHttp_setProxyCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdHttp_Proxy_t  params;
    
    memset(&params, 0x0, sizeof(ATCmdHttp_Proxy_t));
    
    /* Call the command parser */
    ret = ATCmdHttp_setProxyParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
    
    HTTPClient_setProxy(&params.addr);

    ATCmd_okResult();

    return ret;
}


