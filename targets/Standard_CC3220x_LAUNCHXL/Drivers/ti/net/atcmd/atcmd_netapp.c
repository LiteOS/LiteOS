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

#include <ti/net/utils/str_mpl.h>

#include <ti/net/atcmd/atcmd_netapp.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDNETAPP_MAX_BUFFER_LEN              (1480)
#define ATCMDNETAPP_MAX_SECURITY_FILE_NAME_SIZE (96)
#define ATCMDNETAPP_MAX_DOMAIN_NAME_LEN         (63)
#define ATCMDNETAPP_MAX_SIZE_OF_DEVICE_NAME     (80)


const uint16_t ATCmdNetapp_serviceSize[] =
{
	0,                                                           /* 0 - Default value */
	(uint16_t)sizeof(SlNetAppGetFullServiceWithTextIpv4List_t),  /* 1 - SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV4_TYPE */
	(uint16_t)sizeof(SlNetAppGetFullServiceIpv4List_t),          /* 2 - SL_NETAPP_FULL_SERVICE_IPV4_TYPE */
	(uint16_t)sizeof(SlNetAppGetShortServiceIpv4List_t),         /* 3 - SL_NETAPP_SHORT_SERVICE_IPV4_TYPE */
	(uint16_t)sizeof(SlNetAppGetFullServiceWithTextIpv6List_t),  /* 4 - SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV6_TYPE */
	(uint16_t)sizeof(SlNetAppGetFullServiceIpv6List_t),          /* 5 - SL_NETAPP_FULL_SERVICE_IPV6_TYPE */
	(uint16_t)sizeof(SlNetAppGetShortServiceIpv6List_t)          /* 6 - SL_NETAPP_SHORT_SERVICE_IPV6_TYPE */
};

//*****************************************************************************
// typedefs
//*****************************************************************************

typedef struct _ATCmdNetapp_t_
{
    uint16_t    handle;
    uint16_t    len;
    uint32_t    flags;
    uint8_t     format;
    uint8_t     *data;
}ATCmdNetapp_t;

typedef struct _ATCmdNetapp_GetHostByName_t_
{
    int8_t          *hostName;
    uint16_t        nameLen;
    uint8_t         family;
    uint32_t        addr[4]; 
}ATCmdNetapp_GetHostByName_t;

typedef struct _ATCmdNetapp_GetHostByService_t_
{
    int8_t          *serviceName;
    uint8_t         serviceLen;
    uint8_t         family;
    uint32_t        addr[4]; 
    uint32_t        port;
    uint16_t        textLen;
    int8_t          *text;
}ATCmdNetapp_GetHostByService_t;

typedef struct _ATCmdNetapp_Ping_t_
{
    SlNetAppPingCommand_t   pingParams;
    uint8_t                 family;
    SlNetAppPingReport_t    report;
}ATCmdNetapp_Ping_t;

typedef struct _ATCmdNetapp_GetServiceList_t_
{
    uint8_t  offest;
    uint8_t  maxCount;
	uint8_t  flags;
    int16_t  count;
    int8_t  *buffer;
    uint32_t bufferLen;
}ATCmdNetapp_GetServiceList_t;

typedef struct _ATCmdNetapp_RegisterService_t_
{
    int8_t      *serviceName; 
    uint8_t     serviceNameLen;
    int8_t      *text;
    uint8_t     textLen;
    uint16_t    port;
    uint32_t    ttl;
    uint32_t    options;
}ATCmdNetapp_RegisterService_t;

typedef struct _ATCmdNetapp_UnregisterService_t_
{
    int8_t      *serviceName; 
    uint8_t     serviceNameLen;
    uint32_t    options;
}ATCmdNetapp_UnregisterService_t;

typedef struct _ATCmdNetapp_Set_t_
{
    uint8_t 	id;
    uint8_t 	option;
    uint8_t 	len; 
    uint8_t     *value;
}ATCmdNetapp_Set_t;


//*****************************************************************************
// AT Command Netapp Routines
//*****************************************************************************


/*!
    Netapp start callback.

    This routine starts a network application

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_startCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t appBitMap;

    appBitMap = 0x0;
    
    /* Call the command parser */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappType, sizeof(ATCmd_netappType)/sizeof(StrMpl_List_t), (char **)&arg, &appBitMap, ATCMD_DELIM_TRM,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }

    /* app ID should exclude the SL_NETAPP_DEVICE_ID and SL_NETAPP_DNS_CLIENT_ID */
    if (appBitMap > 0x0F)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,0);
	    return -1;        
    }
    /* start netapp */
    ret = sl_NetAppStart(appBitMap);

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

/*!
    Netapp stop callback.

    This routine stops a network application

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_stopCallback(void *arg)
{
    int32_t ret = 0;
    uint32_t appBitMap;
    
    appBitMap = 0x0;
    
    /* Call the command parser */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappType, sizeof(ATCmd_netappType)/sizeof(StrMpl_List_t), (char **)&arg, &appBitMap, ATCMD_DELIM_TRM,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
    /* app ID should exclude the SL_NETAPP_DEVICE_ID and SL_NETAPP_DNS_CLIENT_ID */
    if (appBitMap > 0x0F)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,0);
	    return -1;        
    }

    /* stop netapp */
    ret = sl_NetAppStop(appBitMap);

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

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_getHostByNameFree(ATCmdNetapp_GetHostByName_t *params)
{    
    if (params != NULL)
    {
        if (params->hostName != NULL)
        {
            free(params->hostName);
        }
        free(params);
    }  
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getHostByNameParse(char *buff, ATCmdNetapp_GetHostByName_t *params)
{
    int32_t ret = 0;
    
    /* host name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->hostName, ATCMD_DELIM_ARG, 0xFF ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    params->nameLen = strlen((char *)params->hostName);

    /* family */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->family, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16) ) < 0)
    {
        return ret;
    }

    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_getHostByNameResult(void *args, int32_t num, char *buff)
{    
    ATCmdNetapp_GetHostByName_t *params = (ATCmdNetapp_GetHostByName_t *)args;
    uint8_t ip[4];
    
    StrMpl_setStr(ATCmd_netappGetHostByNameStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setStr((char *)params->hostName,&buff,ATCMD_DELIM_ARG);
    
    if (params->family == SL_AF_INET)
    {
        /* address */
        ATCmd_valToIPv4(params->addr[0], ip);
        StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else if (params->family == SL_AF_INET6)
    {
        /* address */
        StrMpl_setArrayVal(params->addr,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    ATCmdNetapp_getHostByNameFree(params);

    return 0;
}

/*!
    Netapp Get Host By Name callback.

    This routine get host IP by name

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getHostByNameCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_GetHostByName_t *params;

    params = malloc(sizeof(ATCmdNetapp_GetHostByName_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetapp_GetHostByName_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_getHostByNameParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_getHostByNameFree(params);
	    return -1;
    }

    /* Get host by name */
    ret = sl_NetAppDnsGetHostByName(params->hostName, params->nameLen, (_u32 *)params->addr, params->family);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetapp_getHostByNameFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetapp_getHostByNameResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_getHostByServiceFree(ATCmdNetapp_GetHostByService_t *params)
{    
    if (params != NULL)
    {
        if (params->serviceName != NULL)
        {
            free(params->serviceName);
        }
        if (params->text != NULL)
        {
            free(params->text);
        }
        free(params);
    }  
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getHostByServiceParse(char *buff, ATCmdNetapp_GetHostByService_t *params)
{
    int32_t ret = 0;
    
    /* service name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->serviceName, ATCMD_DELIM_ARG, SL_NETAPP_MAX_SERVICE_NAME_SIZE ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    params->serviceLen = strlen((char *)params->serviceName);

    /* family */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->family, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }

    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_getHostByServiceResult(void *args, int32_t num, char *buff)
{    
    ATCmdNetapp_GetHostByService_t *params = (ATCmdNetapp_GetHostByService_t *)args;
    uint8_t ip[4];
    
    StrMpl_setStr(ATCmd_netappGetHostByServiceStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setStr((char *)params->serviceName,&buff,ATCMD_DELIM_ARG);
    
    /* port */
    StrMpl_setVal(&params->port,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    if (params->family == SL_AF_INET)
    {
        /* address */
        ATCmd_valToIPv4(params->addr[0], ip);
        StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else if (params->family == SL_AF_INET6)
    {
        /* address */
        StrMpl_setArrayVal(params->addr,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    StrMpl_setStr((char *)params->text,&buff,ATCMD_DELIM_TRM);
    ATCmdNetapp_getHostByServiceFree(params);

    return 0;
}

/*!
    Netapp Get Host By Service callback.

    This routine get host IP by service

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getHostByServiceCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_GetHostByService_t *params;

    params = malloc(sizeof(ATCmdNetapp_GetHostByService_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetapp_GetHostByService_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_getHostByServiceParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_getHostByServiceFree(params);
	    return -1;
    }

    params->textLen = SL_NETAPP_MAX_SERVICE_TEXT_SIZE;
    params->text = malloc(params->textLen);
    if (params->text == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdNetapp_getHostByServiceFree(params);
	    return -1;
    }
    /* Get host by service */
    ret = sl_NetAppDnsGetHostByService(params->serviceName, params->serviceLen, params->family,(_u32 *)params->addr,(_u32 *)&params->port,&params->textLen,params->text);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetapp_getHostByServiceFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetapp_getHostByServiceResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_sendFree(ATCmdNetapp_t *params)
{    
    if (params->data != NULL)
    {
        free(params->data);
    }  
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_sendParse(char *buff, ATCmdNetapp_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen;
    
    /* handle */
    if ((ret = StrMpl_getVal(&buff, &params->handle, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }

    /* flags */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappFlags, sizeof(ATCmd_netappFlags)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_ARG,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
	    return ret;
    }
    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /* data */
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length to binary length */
        outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->len); 
        params->data = malloc(outputLen);
        if (params->data == NULL)
        {
            return -1;
        }
        if (StrMpl_decodeBase64((uint8_t *)buff, params->len, params->data, (uint32_t *)&outputLen) < 0)
        {
            return -1;
        }
        params->len = outputLen;        
    }
    else
    {
        params->data = malloc(params->len);
        if (params->data == NULL)
        {
            return -1;
        }

        memcpy(params->data, buff, params->len);
    }
    
    return ret;
}


/*!
    Netapp Send callback.

    This routine sends Netapp response or data following a Netapp request event

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_sendCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_t params;

    memset(&params, 0x0, sizeof(ATCmdNetapp_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_sendParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_sendFree(&params);
	    return -1;
    }

    /* netapp send */
    ret = sl_NetAppSend(params.handle, params.len, params.data,params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdNetapp_sendFree(&params);
    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_recvFree(ATCmdNetapp_t *params)
{    
    if (params != NULL)
    {
        if (params->data != NULL)
        {
            free(params->data);
        }
        free(params);
    }
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_recvParse(char *buff, ATCmdNetapp_t *params)
{
    int32_t ret = 0;
    
    /* handle */
    if ((ret = StrMpl_getVal(&buff, &params->handle, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    
    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_recvResult(void *args, int32_t num, char *buff)
{    
    ATCmdNetapp_t *params = (ATCmdNetapp_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_netappRecvStr,&buff,ATCMD_DELIM_EVENT);
    /* handle */
    StrMpl_setVal(&params->handle, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    /* flags */
    StrMpl_setBitmaskListStr(ATCmd_netappFlags, sizeof(ATCmd_netappFlags)/sizeof(StrMpl_List_t),&params->flags, &buff, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    /* length */
    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    StrMpl_setVal(&outputLen, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    /* data */
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert data to base64 */
        StrMpl_encodeBase64(params->data, params->len, (uint8_t *)buff, &outputLen);
    }
    else
    {
        memcpy(buff,params->data,params->len);
    }
    ATCmdNetapp_recvFree(params);

    return 0;
}

/*!
    Netapp Recv callback.

    This routine receives data from the network processor following a Netapp response event

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_recvCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_t *params;

    params = malloc(sizeof(ATCmdNetapp_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    
    memset(params, 0x0, sizeof(ATCmdNetapp_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_recvParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_recvFree(params);
	    return -1;
    }

    /* netapp recv */
    ret = sl_NetAppRecv(params->handle, &params->len, params->data,(_u32 *)&params->flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetapp_recvFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetapp_recvResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_pingParse(char *buff, ATCmdNetapp_Ping_t *params)
{
    int32_t ret = 0;
    uint8_t ip[4];
   
    /* family */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->family, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }
    if (params->family == SL_AF_INET)
    {
        /* address */
        if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray )) < 0)
        {
    	    return ret;
        }
        params->pingParams.Ip = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
    }
    else if (params->family == SL_AF_INET6)
    {
        /* address */
        if ((ret = StrMpl_getArrayVal(&buff, (void *)&params->pingParams.Ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
        {
            return ret;
        }
    }
    else
    {
        return -1;
    }
    
    /* size */
    if ((ret = StrMpl_getVal(&buff, &params->pingParams.PingSize, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }

    /* delay */
    if ((ret = StrMpl_getVal(&buff, &params->pingParams.PingIntervalTime , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    
    /* timeout */
    if ((ret = StrMpl_getVal(&buff, &params->pingParams.PingRequestTimeout, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    
    /* max attempts */
    if ((ret = StrMpl_getVal(&buff, &params->pingParams.TotalNumberOfAttempts , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    
    /* flags */
    if ((ret = StrMpl_getVal(&buff, &params->pingParams.Flags, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
	    return ret;
    }
    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_pingResult(void *args, int32_t num, char *buff)
{    
    SlNetAppPingReport_t *params = (SlNetAppPingReport_t *)args;
    
    StrMpl_setStr(ATCmd_netappPingStr,&buff,ATCMD_DELIM_EVENT);

    /* packets send */
    StrMpl_setVal(&params->PacketsSent, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    /* packets receive */
    StrMpl_setVal(&params->PacketsReceived, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    /* time */
    StrMpl_setVal(&params->AvgRoundTime, &buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    free(params);

    return 0;
}

void ATCmdNetapp_pingReport(SlNetAppPingReport_t *report)
{
    SlNetAppPingReport_t *params = malloc(sizeof(SlNetAppPingReport_t));

    if (params == NULL)
    {
        return;
    }
    memcpy(params,report,sizeof(SlNetAppPingReport_t));
    ATCmd_commandResult(ATCmdNetapp_pingResult,params,0);    
}

/*!
    Netapp Ping callback.

    This routine send ping to network hosts

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_pingCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_Ping_t params;

    memset(&params, 0x0, sizeof(ATCmdNetapp_Ping_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_pingParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }

    /* netapp ping */
    ret = sl_NetAppPing(&params.pingParams, params.family,&params.report,ATCmdNetapp_pingReport);

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
/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_getServiceListFree(ATCmdNetapp_GetServiceList_t *params)
{    
    if (params != NULL)
    {
        if (params->buffer != NULL)
        {
            free(params->buffer);
        }
        free(params);
    }
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getServiceListParse(char *buff, ATCmdNetapp_GetServiceList_t *params)
{
    int32_t ret = 0;
    
    /* offset */
    if ((ret = StrMpl_getVal(&buff, &params->offest, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
	    return ret;
    }

    /* max services count */
    if ((ret = StrMpl_getVal(&buff, &params->maxCount , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* flags */
    if ((ret = StrMpl_getListVal(ATCmd_netappServiceListFlags, sizeof(ATCmd_netappServiceListFlags)/sizeof(StrMpl_List_t), &buff, &params->flags, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }
    
    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_getServiceListResult(void *args, int32_t num, char *buff)
{    
    ATCmdNetapp_GetServiceList_t *params = (ATCmdNetapp_GetServiceList_t *)args;
    uint8_t ip[4],i;
    char delimiter;
    uint8_t numParams = ((params->flags == SL_NETAPP_SHORT_SERVICE_IPV4_TYPE) || (params->flags == SL_NETAPP_SHORT_SERVICE_IPV6_TYPE))? 2:
                        ((params->flags == SL_NETAPP_FULL_SERVICE_IPV4_TYPE) || (params->flags == SL_NETAPP_FULL_SERVICE_IPV6_TYPE))?4:5;

    StrMpl_setStr(ATCmd_netappGetServiceListStr,&buff,ATCMD_DELIM_EVENT);

    for (i=0;i < params->count;i++)
    {
        delimiter = ATCMD_DELIM_ARG;
        if ((params->flags == SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV4_TYPE) ||
            (params->flags == SL_NETAPP_FULL_SERVICE_IPV4_TYPE) ||
            (params->flags == SL_NETAPP_SHORT_SERVICE_IPV4_TYPE))
        {
            /* ip */
            ATCmd_valToIPv4(((SlNetAppGetShortServiceIpv4List_t *)(params->buffer))->service_ipv4, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            if (numParams == 2)
            {
                delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
            }
            /* port */
            StrMpl_setVal(&(((SlNetAppGetShortServiceIpv4List_t *)(params->buffer))->service_port), &buff,delimiter,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            if (numParams > 2)
            {
                /* service name */
                StrMpl_setStr((char *)(((SlNetAppGetFullServiceIpv4List_t *)(params->buffer))->service_name),&buff,ATCMD_DELIM_ARG);

                if (numParams == 4)
                {
                    delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
                }
                /* service host name */
                StrMpl_setStr((char *)(((SlNetAppGetFullServiceIpv4List_t *)(params->buffer))->service_host),&buff,delimiter);

                if (numParams > 4)
                {
                    delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
                    /* service text */
                    StrMpl_setStr((char *)(((SlNetAppGetFullServiceWithTextIpv4List_t *)(params->buffer))->service_text),&buff,delimiter);
                }
            }
        }
        else
        {
            /* ip */
            StrMpl_setArrayVal(((SlNetAppGetShortServiceIpv6List_t *)(params->buffer))->service_ipv6,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);


            if (numParams == 2)
            {
                delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
            }
            /* port */
            StrMpl_setVal(&(((SlNetAppGetShortServiceIpv6List_t *)(params->buffer))->service_port),&buff,delimiter,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            if (numParams > 2)
            {
                /* service name */
                StrMpl_setStr((char *)(((SlNetAppGetFullServiceIpv6List_t *)(params->buffer))->service_name),&buff,ATCMD_DELIM_ARG);

                if (numParams == 4)
                {
                    delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
                }
                /* service host name */
                StrMpl_setStr((char *)(((SlNetAppGetFullServiceIpv6List_t *)(params->buffer))->service_host),&buff,delimiter);

                if (numParams > 4)
                {
                    delimiter =  (i == (params->count - 1))?ATCMD_DELIM_TRM:ATCMD_DELIM_LIST;
                    /* service text */
                    StrMpl_setStr((char *)(((SlNetAppGetFullServiceWithTextIpv6List_t *)(params->buffer))->service_text),&buff,delimiter);
                }
            }
        }
    }

    ATCmdNetapp_getServiceListFree(params);

    return 0;
}


/*!
    Netapp GetServiceList  callback.

    This routine get service list 

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getServiceListCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_GetServiceList_t *params;

    params = malloc(sizeof(ATCmdNetapp_GetServiceList_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetapp_GetServiceList_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_getServiceListParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_getServiceListFree(params);
	    return -1;
    }
	params->bufferLen =  params->maxCount * ATCmdNetapp_serviceSize[params->flags];
    if (params->bufferLen > ATCMDNETAPP_MAX_BUFFER_LEN)
    {
        params->bufferLen = ATCMDNETAPP_MAX_BUFFER_LEN;
    }
    params->buffer = malloc(params->bufferLen);
    if (params->buffer == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdNetapp_getServiceListFree(params);
        return -1;
    }
    /* netapp GetServiceList */
    params->count = sl_NetAppGetServiceList(params->offest, params->maxCount, params->flags,params->buffer,params->bufferLen);

    if (params->count < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->count);
        ATCmdNetapp_getServiceListFree(params);
        return -1;
    }
    else
    {
        ATCmd_commandResult(ATCmdNetapp_getServiceListResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_registerServiceFree(ATCmdNetapp_RegisterService_t *params)
{    
    if (params->serviceName != NULL)
    {
        free(params->serviceName);
    }
    if (params->text!= NULL)
    {
        free(params->text);
    }
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_registerServiceParse(char *buff, ATCmdNetapp_RegisterService_t *params)
{
    int32_t ret = 0;
    
    /* service name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->serviceName, ATCMD_DELIM_ARG, SL_NETAPP_MAX_SERVICE_NAME_SIZE ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    params->serviceNameLen = strlen((char *)params->serviceName);

    /* text name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->text, ATCMD_DELIM_ARG, SL_NETAPP_MAX_SERVICE_TEXT_SIZE ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    params->textLen = strlen((char *)params->text);

    /* port */
    if ((ret = StrMpl_getVal(&buff, &params->port, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }

    /* ttl */
    if ((ret = StrMpl_getVal(&buff, &params->ttl , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    
    /* options */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappRegServiceOptions, sizeof(ATCmd_netappRegServiceOptions)/sizeof(StrMpl_List_t), &buff, &params->options, ATCMD_DELIM_TRM , ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }
    
    return ret;
}

/*!
    Netapp RegisterMDNS callback.

    This routine register a new mDNS service 

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_registerServiceCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_RegisterService_t params;

    memset(&params, 0x0, sizeof(ATCmdNetapp_RegisterService_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_registerServiceParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_registerServiceFree(&params);
	    return -1;
    }

    if ((params.serviceNameLen + params.textLen) > SL_NETAPP_MDNS_MAX_SERVICE_NAME_AND_TEXT_LENGTH)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,0);
        ATCmdNetapp_registerServiceFree(&params);
	    return -1;
    }

    /* netapp MDNSRegisterService */
    ret = sl_NetAppMDNSRegisterService(params.serviceName, params.serviceNameLen, params.text,params.textLen,params.port,params.ttl,params.options);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    ATCmdNetapp_registerServiceFree(&params);

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_unregisterServiceFree(ATCmdNetapp_UnregisterService_t *params)
{    
    if (params->serviceName != NULL)
    {
        free(params->serviceName);
    }
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_unregisterServiceParse(char *buff, ATCmdNetapp_UnregisterService_t *params)
{
    int32_t ret = 0;

    /* service name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->serviceName, ATCMD_DELIM_ARG, SL_NETAPP_MAX_SERVICE_NAME_SIZE ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
    params->serviceNameLen = strlen((char *)params->serviceName);
    
    /* options */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappRegServiceOptions, sizeof(ATCmd_netappRegServiceOptions)/sizeof(StrMpl_List_t), &buff, &params->options, ATCMD_DELIM_TRM , ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
        ret = 0;
    }
    
    return ret;
}

/*!
    Netapp UnregisterMDNS callback.

    This routine unregister a mDNS service 

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_unregisterServiceCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_UnregisterService_t params;

    memset(&params, 0x0, sizeof(ATCmdNetapp_UnregisterService_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_unregisterServiceParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_unregisterServiceFree(&params);
	    return -1;
    }

    /* netapp MDNSUnRegisterService */
    ret = sl_NetAppMDNSUnRegisterService(params.serviceName, params.serviceNameLen, params.options);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    ATCmdNetapp_unregisterServiceFree(&params);

    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_setFree(ATCmdNetapp_Set_t *params)
{    
    if (params->value != NULL)
    {
        free(params->value);
    }  
    return 0;
}

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_setParse(char *buff, ATCmdNetapp_Set_t *params)
{
    int32_t ret = 0;
    uint8_t ip[4];
    
    /* App ID */
    if ((ret = StrMpl_getListVal(ATCmd_netappType, sizeof(ATCmd_netappType)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }

    /* option */
    switch (params->id)
    {
        case SL_NETAPP_HTTP_SERVER_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappHttpSrvOptions, sizeof(ATCmd_netappHttpSrvOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_HTTP_AUTH_CHECK:
                case SL_NETAPP_HTTP_ROM_PAGES_ACCESS:
                case SL_NETAPP_HTTP_SECONDARY_PORT_ENABLE:
                case SL_NETAPP_HTTP_PRIMARY_PORT_SECURITY_MODE: 
                    params->len = sizeof(uint8_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getVal(&buff, (void *)params->value , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                    {
                        return ret;
                    }
                    break;
                case SL_NETAPP_HTTP_PRIMARY_PORT_NUMBER: 
                case SL_NETAPP_HTTP_SECONDARY_PORT_NUMBER: 
                    params->len = sizeof(uint16_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getVal(&buff, (void *)params->value , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
                    {
                        return ret;
                    }
                    break;
                case SL_NETAPP_HTTP_AUTH_NAME:
                case SL_NETAPP_HTTP_AUTH_PASSWORD:
                case SL_NETAPP_HTTP_AUTH_REALM:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, SL_NETAPP_MAX_AUTH_NAME_LEN ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;

                case SL_NETAPP_HTTP_PRIVATE_KEY_FILENAME:
                case SL_NETAPP_HTTP_DEVICE_CERTIFICATE_FILENAME:
                case SL_NETAPP_HTTP_CA_CERTIFICATE_FILE_NAME:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, ATCMDNETAPP_MAX_SECURITY_FILE_NAME_SIZE ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;

                case SL_NETAPP_HTTP_TEMP_REGISTER_MDNS_SERVICE_NAME:
                case SL_NETAPP_HTTP_TEMP_UNREGISTER_MDNS_SERVICE_NAME:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, ATCMDNETAPP_MAX_SIZE_OF_DEVICE_NAME ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;

                default:
                    return -1;
            }
            break;

        case SL_NETAPP_DHCP_SERVER_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDhcpSrvOptions, sizeof(ATCmd_netappDhcpSrvOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DHCP_SRV_BASIC_OPT:
                    params->len = sizeof(SlNetAppDhcpServerBasicOpt_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppDhcpServerBasicOpt_t *)(params->value))->lease_time), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }            
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetAppDhcpServerBasicOpt_t *)(params->value))->ipv4_addr_start = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetAppDhcpServerBasicOpt_t *)(params->value))->ipv4_addr_last = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);

                    break;
                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_MDNS_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappMdnsOptions, sizeof(ATCmd_netappMdnsOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_MDNS_CONT_QUERY_OPT:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, ATCMDNETAPP_MAX_SIZE_OF_DEVICE_NAME ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;
                case SL_NETAPP_MDNS_QEVETN_MASK_OPT:
                    params->len = sizeof(uint32_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getBitmaskListVal(ATCmd_netappMdnsTypes, sizeof(ATCmd_netappMdnsTypes)/sizeof(StrMpl_List_t), &buff, (void *)params->value, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                        return ret;
                    }
                    break;

                case SL_NETAPP_MDNS_TIMING_PARAMS_OPT:
                    params->len = sizeof(SlNetAppServiceAdvertiseTimingParameters_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->t), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }            
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->p), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->k), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }            
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->RetransInterval), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->Maxinterval), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }            
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->max_time), ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
                    {
                        return ret;
                    }
                    
                    break;

                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_DEVICE_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDevOptions, sizeof(ATCmd_netappDevOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DEVICE_URN:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, SL_NETAPP_MAX_DEVICE_URN_LEN ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;

                case SL_NETAPP_DEVICE_DOMAIN:
                    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->value, ATCMD_DELIM_TRM, ATCMDNETAPP_MAX_DOMAIN_NAME_LEN ,ATCmd_excludeDelimStr)) < 0)
                    {
                        return ret;
                    }
                    params->len = strlen((char *)params->value);
                    break;
                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_DNS_CLIENT_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDnsClientOptions, sizeof(ATCmd_netappDnsClientOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DNS_CLIENT_TIME:
                    params->value = malloc(sizeof(SlNetAppDnsClientTime_t));
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppDnsClientTime_t *)(params->value))->MaxResponseTime), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
                    {
                        return ret;
                    }            
                    if ((ret = StrMpl_getVal(&buff, &(((SlNetAppDnsClientTime_t *)(params->value))->NumOfRetries), ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
                    {
                        return ret;
                    }
                    params->len = sizeof(SlNetAppDnsClientTime_t);
                    break;
                default:
                    return -1;
            }                  
            break;

        default:
            return -1;
    }

    return ret;
}


/*!
    Netapp Set callback.

    This routine sets network applications configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_setCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_Set_t params;

    memset(&params, 0x0, sizeof(ATCmdNetapp_Set_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_setParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_setFree(&params);
	    return -1;
    }
    
    /* set netapp option */
    ret = sl_NetAppSet(params.id,params.option,params.len,params.value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdNetapp_setFree(&params);
    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetapp_getFree(ATCmdNetapp_Set_t *params)
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

/*!
    \brief          Parse command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getParse(char *buff, ATCmdNetapp_Set_t *params)
{
    int32_t ret = 0;
    
    /* App ID */
    if ((ret = StrMpl_getListVal(ATCmd_netappType, sizeof(ATCmd_netappType)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }

    /* option */
    switch (params->id)
    {
        case SL_NETAPP_HTTP_SERVER_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappHttpSrvOptions, sizeof(ATCmd_netappHttpSrvOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_HTTP_AUTH_CHECK:
                case SL_NETAPP_HTTP_ROM_PAGES_ACCESS:
                case SL_NETAPP_HTTP_SECONDARY_PORT_ENABLE:
                case SL_NETAPP_HTTP_PRIMARY_PORT_SECURITY_MODE: 
                    params->len = sizeof(uint8_t);
                    break;
                case SL_NETAPP_HTTP_PRIMARY_PORT_NUMBER: 
                case SL_NETAPP_HTTP_SECONDARY_PORT_NUMBER: 
                    params->len = sizeof(uint16_t);
                    break;
                case SL_NETAPP_HTTP_AUTH_NAME:
                case SL_NETAPP_HTTP_AUTH_PASSWORD:
                case SL_NETAPP_HTTP_AUTH_REALM:
                    params->len = SL_NETAPP_MAX_AUTH_NAME_LEN;
                    break;

                case SL_NETAPP_HTTP_PRIVATE_KEY_FILENAME:
                case SL_NETAPP_HTTP_DEVICE_CERTIFICATE_FILENAME:
                case SL_NETAPP_HTTP_CA_CERTIFICATE_FILE_NAME:
                    params->len = ATCMDNETAPP_MAX_SECURITY_FILE_NAME_SIZE;
                    break;

                case SL_NETAPP_HTTP_TEMP_REGISTER_MDNS_SERVICE_NAME:
                case SL_NETAPP_HTTP_TEMP_UNREGISTER_MDNS_SERVICE_NAME:
                    params->len = ATCMDNETAPP_MAX_SIZE_OF_DEVICE_NAME;
                    break;

                default:
                    return -1;
            }
            break;

        case SL_NETAPP_DHCP_SERVER_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDhcpSrvOptions, sizeof(ATCmd_netappDhcpSrvOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DHCP_SRV_BASIC_OPT:
                    params->len = sizeof(SlNetAppDhcpServerBasicOpt_t);
                    break;
                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_MDNS_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappMdnsOptions, sizeof(ATCmd_netappMdnsOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_MDNS_CONT_QUERY_OPT:
                    params->len = ATCMDNETAPP_MAX_SIZE_OF_DEVICE_NAME;
                    break;
                case SL_NETAPP_MDNS_QEVETN_MASK_OPT:
                    params->len = sizeof(uint32_t);
                    break;

                case SL_NETAPP_MDNS_TIMING_PARAMS_OPT:                  
                    params->len = sizeof(SlNetAppServiceAdvertiseTimingParameters_t);
                    break;

                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_DEVICE_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDevOptions, sizeof(ATCmd_netappDevOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DEVICE_URN:
                    params->len = SL_NETAPP_MAX_DEVICE_URN_LEN;
                    break;

                case SL_NETAPP_DEVICE_DOMAIN:
                    params->len = ATCMDNETAPP_MAX_DOMAIN_NAME_LEN;
                    break;
                default:
                    return -1;
            }                  
            break;

        case SL_NETAPP_DNS_CLIENT_ID:
            if ((ret = StrMpl_getListVal(ATCmd_netappDnsClientOptions, sizeof(ATCmd_netappDnsClientOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DNS_CLIENT_TIME:
                    params->len = sizeof(SlNetAppDnsClientTime_t);
                    break;
                default:
                    return -1;
            }                  
            break;

        default:
            return -1;
    }

    params->value = malloc(params->len);
    if (params->value == NULL)
    {
        return -1;
    }

    return ret;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdNetapp_getResult(void *args, int32_t num, char *buff)
{    
    int32_t ret = 0;
    ATCmdNetapp_Set_t *params = (ATCmdNetapp_Set_t *)args;
    uint8_t ip[4];
    
    StrMpl_setStr(ATCmd_netappGetStr,&buff,ATCMD_DELIM_EVENT);

    switch (params->id)
    {
        case SL_NETAPP_HTTP_SERVER_ID:
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_HTTP_AUTH_CHECK:
                case SL_NETAPP_HTTP_ROM_PAGES_ACCESS:
                case SL_NETAPP_HTTP_SECONDARY_PORT_ENABLE:
                case SL_NETAPP_HTTP_PRIMARY_PORT_SECURITY_MODE: 
                    StrMpl_setVal(params->value, &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    break;
                case SL_NETAPP_HTTP_PRIMARY_PORT_NUMBER: 
                case SL_NETAPP_HTTP_SECONDARY_PORT_NUMBER: 
                    StrMpl_setVal(params->value, &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    break;
                case SL_NETAPP_HTTP_AUTH_NAME:
                case SL_NETAPP_HTTP_AUTH_PASSWORD:
                case SL_NETAPP_HTTP_AUTH_REALM:
                    StrMpl_setStr((char *)params->value, &buff, ATCMD_DELIM_TRM);
                    break;

                default:
                    ret = -1;
            }
            break;

        case SL_NETAPP_DHCP_SERVER_ID:
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DHCP_SRV_BASIC_OPT:
                    StrMpl_setVal(&(((SlNetAppDhcpServerBasicOpt_t *)(params->value))->lease_time), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    ATCmd_valToIPv4(((SlNetAppDhcpServerBasicOpt_t *)(params->value))->ipv4_addr_start, ip);
                    StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    ATCmd_valToIPv4(((SlNetAppDhcpServerBasicOpt_t *)(params->value))->ipv4_addr_last, ip);
                    StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    break;
                default:
                    ret = -1;
            }                  
            break;

        case SL_NETAPP_MDNS_ID:
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_MDNS_CONT_QUERY_OPT:
                    StrMpl_setStr((char *)params->value, &buff, ATCMD_DELIM_TRM);
                    break;
                case SL_NETAPP_MDNS_QEVETN_MASK_OPT:
                    StrMpl_setBitmaskListStr(ATCmd_netappMdnsTypes, sizeof(ATCmd_netappMdnsTypes)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
                    break;

                case SL_NETAPP_MDNS_TIMING_PARAMS_OPT:
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->t), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->p), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->k), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->RetransInterval), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->Maxinterval), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppServiceAdvertiseTimingParameters_t *)(params->value))->max_time), &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    
                    break;

                default:
                    ret = -1;
            }                  
            break;

        case SL_NETAPP_DEVICE_ID:
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DEVICE_URN:
                case SL_NETAPP_DEVICE_DOMAIN:
                    StrMpl_setStr((char *)params->value, &buff, ATCMD_DELIM_TRM);
                    break;
                default:
                    ret = -1;
            }                  
            break;

        case SL_NETAPP_DNS_CLIENT_ID:
            /* value */
            switch (params->option)
            {
                case SL_NETAPP_DNS_CLIENT_TIME:
                    StrMpl_setVal(&(((SlNetAppDnsClientTime_t *)(params->value))->MaxResponseTime), &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    StrMpl_setVal(&(((SlNetAppDnsClientTime_t *)(params->value))->NumOfRetries), &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    break;
                default:
                    ret = -1;
            }                  
            break;

        default:
            ret = -1;
    }
    ATCmdNetapp_getFree(params);

    return ret;
}

/*!
    Netapp Get callback.

    This routine gets network applications configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetapp_getCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetapp_Set_t *params;

    params = malloc(sizeof(ATCmdNetapp_Set_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetapp_Set_t));
    
    /* Call the command parser */
    ret = ATCmdNetapp_getParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetapp_getFree(params);
	    return -1;
    }
    
    /* set netapp option */
    ret = sl_NetAppGet(params->id,params->option,&params->len,params->value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetapp_getFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetapp_getResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

