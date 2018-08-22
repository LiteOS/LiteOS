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

/* AT Header files */
#include <ti/net/atcmd/atcmd_socket.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>

//*****************************************************************************
// defines
//*****************************************************************************


//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmdSock_t_
{
    int16_t         sd;
    SlSocklen_t     addrlen;
    SlSockAddr_t    addr;
    int8_t          padding[12];
}ATCmdSock_t;

typedef struct _ATCmdSock_Socket_t_
{
    int16_t         domain;
    int16_t         type;
    int16_t         protocol;
}ATCmdSock_Socket_t;

typedef struct _ATCmdSock_Accept_t_
{
    int16_t         sd;
    int16_t         domain;
    ATCmdSock_t     ret;
}ATCmdSock_Accept_t;

typedef struct _ATCmdSock_Listen_t_
{
    int16_t         sd;
    int16_t         backlog;
}ATCmdSock_Listen_t;

typedef struct _ATCmdSock_Select_t_
{
    int16_t         nfds;
    SlTimeval_t     timeout;
    SlFdSet_t       readsds;
}ATCmdSock_Select_t;

typedef struct _ATCmdSock_SetSockOpt_t_
{
    int16_t 	    sd;
    int16_t 	    level;
    int16_t 	    optname;
    void * 	        optval;
    SlSocklen_t 	optlen; 
}ATCmdSock_SetSockOpt_t;

typedef struct _ATCmdSock_tcp_t_
{
    int16_t         sd;
    uint8_t         format;
    int16_t         flags;
    int16_t         len;
    void            *data;
}ATCmdSock_tcp_t;

typedef struct _ATCmdSock_udp_t_
{
    ATCmdSock_t     target;
    uint8_t         format;
    int16_t         flags;
    int16_t         len;
    void            *data;
}ATCmdSock_udp_t;

//*****************************************************************************
// AT Command Socket Routines
//*****************************************************************************


/*!
    \brief          Return socket result.

    This routine send the number of created socket.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdSock_socketResult(void *args, int32_t num, char *buff)
{    
    int16_t sd = (int16_t)(num);

    StrMpl_setStr(ATCmd_sockSocketStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&sd,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);
    return 0;
}

/*!
    \brief          Sock Socket parser.

    This routine parse create socket command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_socketParse(char *buff, ATCmdSock_Socket_t *params)
{
    int32_t ret = 0;
    
    /* domain*/
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->domain, ATCMD_DELIM_ARG, sizeof(params->domain))) < 0)
    {
        return ret;
    }

    /* type */
    if ((ret = StrMpl_getListVal(ATCmd_sockType, sizeof(ATCmd_sockType)/sizeof(StrMpl_List_t), &buff, &params->type, ATCMD_DELIM_ARG, sizeof(params->type))) < 0)
    {
        return ret;
    }

    /* protocol */
    if ((ret = StrMpl_getListVal(ATCmd_sockProtocol, sizeof(ATCmd_sockProtocol)/sizeof(StrMpl_List_t), &buff, &params->protocol, ATCMD_DELIM_TRM, sizeof(params->protocol))) < 0)
    {
        return ret;
    }
    return ret;
}

/*!
    \brief          Sock Socket callback.

    This routine create socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_socketCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_Socket_t  sockParams;
    
    memset(&sockParams, 0x0, sizeof(ATCmdSock_Socket_t));

    /* Call the command parser */
    ret = ATCmdSock_socketParse((char *)arg,&sockParams);
	
	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }

    /* create socket */
    ret = sl_Socket(sockParams.domain, sockParams.type, sockParams.protocol);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_socketResult,NULL,ret);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Return close result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdSock_closeResult(void *args, int32_t num, char *buff)
{    
    int16_t sock = (int16_t)num;

    StrMpl_setStr(ATCmd_sockCloseStr,&buff,ATCMD_DELIM_EVENT);

    StrMpl_setVal(&sock,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);
    return 0;
}

/*!
    \brief          Sock Close callback.

    This routine close socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_closeCallback(void *arg)
{
    int32_t ret = 0;
    int16_t sock;
    
    /* Call the command parser */
    if ((ret = StrMpl_getVal((char **)&arg, (void *)&sock, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
	
    /* create socket */
    ret = sl_Close(sock);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_closeResult,NULL,sock);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory for accept command

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_acceptFree(ATCmdSock_Accept_t *params)
{    
    if (params != NULL)
    {
        free(params);
    }  
    return 0;
}

/*!
    \brief          Sock Accept parser.

    This routine parse accept command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_acceptParse(char *buff, ATCmdSock_Accept_t *params)
{
    int32_t ret = 0;
    
    /* socket */
    if ((ret = StrMpl_getVal((char **)&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /* domain */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), &buff, &params->domain, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    /* address length */
    if (params->domain == SL_AF_INET)
	{
        params->ret.addrlen = sizeof(SlSockAddrIn_t);
	}
    else if (params->domain == SL_AF_INET6)
    {
        params->ret.addrlen = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        return -1;
    }
    
    return ret;
}

/*!
    \brief          Return accept result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdSock_acceptResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_Accept_t *params = (ATCmdSock_Accept_t *)args;
    void *addr = &params->ret.addr;
    uint8_t ip[4];
    
    StrMpl_setStr(ATCmd_sockAcceptStr,&buff,ATCMD_DELIM_EVENT);
    /* new socket descriptor */
    StrMpl_setVal(&params->ret.sd,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);
    /* family */
    StrMpl_setListStr(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t),&(params->ret.addr.sa_family),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED);

    if (params->ret.addr.sa_family == SL_AF_INET)
    {
        ((SlSockAddrIn_t *)(addr))->sin_port = sl_Htons(((SlSockAddrIn_t *)(addr))->sin_port);
        /* port */
        StrMpl_setVal(&(((SlSockAddrIn_t *)(addr))->sin_port),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
        /* address */
        ((SlSockAddrIn_t *)(addr))->sin_addr.s_addr = sl_Htonl(((SlSockAddrIn_t *)(addr))->sin_addr.s_addr);
        ATCmd_valToIPv4(((SlSockAddrIn_t *)(addr))->sin_addr.s_addr, ip);
        StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else if (params->ret.addr.sa_family == SL_AF_INET6)
    {
        ((SlSockAddrIn6_t *)(addr))->sin6_port = sl_Htons(((SlSockAddrIn6_t *)(addr))->sin6_port);
        /* port */
        StrMpl_setVal(&(((SlSockAddrIn6_t *)(addr))->sin6_port),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
        /* address */
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3]);
        StrMpl_setArrayVal(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    }

    ATCmdSock_acceptFree(params);
    return 0;
}

/*!
    \brief          Sock Accept callback.

    This routine Accept a connection on a socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_acceptCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_Accept_t *params;

    params = malloc(sizeof(ATCmdSock_Accept_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdSock_Accept_t));
    
    /* Call the command parser */
    ret = ATCmdSock_acceptParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_acceptFree(params);
	    return -1;
    }

    /* return command complete for blocked command */
    ATCmd_okResult();

    /* create socket */
    params->ret.sd = sl_Accept(params->sd,&params->ret.addr,&params->ret.addrlen);

    if (params->ret.sd < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->ret.sd);
        ATCmdSock_acceptFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_acceptResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Sock Bind parser.

    This routine parse bind or connect commands.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_parse(char **buff, ATCmdSock_t *params, char delim)
{
    int32_t ret = 0;
    void *addr = &params->addr;
    uint8_t ip[4];
    
    /* sd */
    if ((ret = StrMpl_getVal(buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /* family */
    if ((ret = StrMpl_getListVal(ATCmd_sockDomain, sizeof(ATCmd_sockDomain)/sizeof(StrMpl_List_t), buff, &params->addr.sa_family, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    if (params->addr.sa_family == SL_AF_INET)
    {
        /* port */
        if ((ret = StrMpl_getVal(buff, &((SlSockAddrIn_t *)(addr))->sin_port, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn_t *)(addr))->sin_port = sl_Htons(((SlSockAddrIn_t *)(addr))->sin_port);
        /* address */
        if ((ret = StrMpl_getArrayVal(buff,(void *)ip,4,delim,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn_t *)(addr))->sin_addr.s_addr = sl_Htonl(SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]));
        /* length */
        params->addrlen = sizeof(SlSockAddrIn_t);
    }
    else if (params->addr.sa_family == SL_AF_INET6)
    {
        /* port */
        if ((ret = StrMpl_getVal(buff, &((SlSockAddrIn6_t *)(addr))->sin6_port, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
        {
    	    return ret;
        }
        ((SlSockAddrIn6_t *)(addr))->sin6_port = sl_Htons(((SlSockAddrIn6_t *)(addr))->sin6_port);
        /* address */
        if ((ret = StrMpl_getArrayVal(buff, (void *)(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32),4,delim,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
        {
            return ret;
        }
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3]);
        /* length */
        params->addrlen = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        return -1;
    }
    
    return ret;
}


/*!
    \brief          Sock Bind callback.

    This routine assign a name to a socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_bindCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_t params;

    memset(&params, 0x0, sizeof(ATCmdSock_t));
    
    /* Call the command parser */
    ret = ATCmdSock_parse((char **)&arg, &params,ATCMD_DELIM_TRM);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
    
    /* bind socket */
    ret = sl_Bind(params.sd,&params.addr,params.addrlen);

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
    \brief          Sock Listen parser.

    This routine parse listen command.

    \param          buff       -   Points to command line buffer.
    \param          params     -   Points to create socket struct.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_listenParse(char *buff, ATCmdSock_Listen_t *params)
{
    int32_t ret = 0;

    /* socket */
    if ((ret = StrMpl_getVal((char **)&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /* num of connections */
    if ((ret = StrMpl_getVal((char **)&buff, &params->backlog, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }
    
    return ret;
}

/*!
    \brief          Sock Listen callback.

    This routine Listen for connections on a socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_listenCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_Listen_t params;

    memset(&params, 0x0, sizeof(ATCmdSock_Listen_t));
    
    /* Call the command parser */
    ret = ATCmdSock_listenParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return -1;
    }
	
    /* create socket */
    ret = sl_Listen(params.sd,params.backlog);

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
    \brief          Free allocated memory for accept command

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_connectFree(ATCmdSock_t *params)
{    
    if (params != NULL)
    {
        free(params);
    }  
    return 0;
}

/*!
    \brief          Return connect result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdSock_connectResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_t *params = (ATCmdSock_t *)args;
    void *addr = &params->addr;
    uint8_t ip[4];
    
    StrMpl_setStr(ATCmd_sockConnectStr,&buff,ATCMD_DELIM_EVENT);
    if (params->addr.sa_family == SL_AF_INET)
    {
        /* port */
        ((SlSockAddrIn_t *)(addr))->sin_port = sl_Htons(((SlSockAddrIn_t *)(addr))->sin_port);
        StrMpl_setVal(&(((SlSockAddrIn_t *)(addr))->sin_port),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
        /* address */
        ((SlSockAddrIn_t *)(addr))->sin_addr.s_addr = sl_Htonl(((SlSockAddrIn_t *)(addr))->sin_addr.s_addr);
        ATCmd_valToIPv4(((SlSockAddrIn_t *)(addr))->sin_addr.s_addr, ip);
        StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else if (params->addr.sa_family == SL_AF_INET6)
    {
        /* port */
        ((SlSockAddrIn6_t *)(addr))->sin6_port = sl_Htons(((SlSockAddrIn6_t *)(addr))->sin6_port);
        StrMpl_setVal(&(((SlSockAddrIn6_t *)(addr))->sin6_port),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
        /* address */
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[0]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[1]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[2]);
        ((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3] = sl_Htonl(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32[3]);
        StrMpl_setArrayVal(((SlSockAddrIn6_t *)(addr))->sin6_addr._S6_un._S6_u32,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    }

    ATCmdSock_connectFree(params);

    return 0;
}


/*!
    \brief          Sock Connect callback.

    This routine initiate a connection on a socket.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_connectCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_t *params;

    params = malloc(sizeof(ATCmdSock_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdSock_t));
    
    /* Call the command parser */
    ret = ATCmdSock_parse((char **)&arg, params,ATCMD_DELIM_TRM);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_connectFree(params);
	    return -1;
    }  
    
    /* return command complete for blocked command */
    ATCmd_okResult();

    /* connect */
    ret = sl_Connect(params->sd,&params->addr,params->addrlen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdSock_connectFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_connectResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_selectFree(ATCmdSock_Select_t *params)
{    
    if (params != NULL)
    {
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
int32_t ATCmdSock_selectParse(char *buff, ATCmdSock_Select_t *params)
{
    int32_t ret = 0;

    /* nfds */
    if ((ret = StrMpl_getVal((char **)&buff, &params->nfds, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /* readsds */
    if ((ret = StrMpl_getArrayBitVal((char **)&buff, &params->readsds.fd_array[0],ATCMD_DELIM_ARG,ATCMD_DELIM_BIT,ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    
    /* timeout - seconds */
    if ((ret = StrMpl_getVal((char **)&buff, &params->timeout.tv_sec, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }
    
    /* timeout - microseconds */
    if ((ret = StrMpl_getVal((char **)&buff, &params->timeout.tv_usec, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
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
int32_t ATCmdSock_selectResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_Select_t *params = (ATCmdSock_Select_t *)args;
    
    StrMpl_setStr(ATCmd_sockSelectStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setArrayBitVal(&params->readsds.fd_array[0],&buff,ATCMD_DELIM_TRM,ATCMD_DELIM_BIT,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);

    ATCmdSock_selectFree(params);

    return 0;
}

/*!
    \brief          Sock Select callback.

    This routine monitor socket activity.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_selectCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_Select_t *params;

    params = malloc(sizeof(ATCmdSock_Select_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdSock_Select_t));
    
    /* Call the command parser */
    ret = ATCmdSock_selectParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_selectFree(params);
	    return -1;
    }
    else
    {
        ATCmd_okResult();
    }
    
    /* select */
    ret = sl_Select(params->nfds,&params->readsds,NULL,NULL,&params->timeout);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdSock_selectFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_selectResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_setSockOptFree(ATCmdSock_SetSockOpt_t *params)
{    
    if (params->optval != NULL)
    {
        free(params->optval);
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
int32_t ATCmdSock_setSockOptParse(char *buff, ATCmdSock_SetSockOpt_t *params)
{
    int32_t ret = 0;
    uint8_t ip[4];
    
    /* SD */
    if ((ret = StrMpl_getVal((char **)&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /*Level */
    if ((ret = StrMpl_getListVal(ATCmd_sockLevel, sizeof(ATCmd_sockLevel)/sizeof(StrMpl_List_t), &buff, &params->level, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }
    /* option */
    switch (params->level)
    {
        case SL_SOL_SOCKET:
            if ((ret = StrMpl_getListVal(ATCmd_sockSocketOpt, sizeof(ATCmd_sockSocketOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
        case SL_IPPROTO_IP:
            if ((ret = StrMpl_getListVal(ATCmd_sockIpOpt, sizeof(ATCmd_sockIpOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
        case SL_SOL_PHY_OPT:
            if ((ret = StrMpl_getListVal(ATCmd_sockPhyOpt, sizeof(ATCmd_sockPhyOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
    }

    /* value */
    switch (params->optname)
    {
        case SL_SO_KEEPALIVE:
        case SL_SO_KEEPALIVETIME:
        case SL_SO_RX_NO_IP_BOUNDARY:
        case SL_SO_RCVBUF:
        case SL_SO_NONBLOCKING :
        case SL_SO_CHANGE_CHANNEL:
        case SL_SO_PHY_RATE:
        case SL_SO_PHY_TX_POWER:
        case SL_SO_PHY_NUM_FRAMES_TO_TX:
        case SL_SO_PHY_PREAMBLE:
        case SL_IP_RAW_RX_NO_HEADER:
        case SL_IP_HDRINCL:
        case SL_IP_RAW_IPV6_HDRINCL:
        case SL_SO_PHY_TX_TIMEOUT:
        case SL_SO_PHY_ALLOW_ACKS:
            
            params->optlen = sizeof(uint32_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getVal((char **)&buff, params->optval , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
            {
                return ret;
            }
            break;
        case SL_SO_RCVTIMEO:
            params->optlen = sizeof(SlTimeval_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getVal((char **)&buff, &(((SlTimeval_t *)(params->optval))->tv_sec), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            
            if ((ret = StrMpl_getVal((char **)&buff, &(((SlTimeval_t *)(params->optval))->tv_usec), ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            break;
        case SL_SO_SECMETHOD:
            params->optlen = sizeof(uint8_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getListVal(ATCmd_sockSocketSecMethod, sizeof(ATCmd_sockSocketSecMethod)/sizeof(StrMpl_List_t), &buff, params->optval, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            break;
        case SL_SO_SECURE_MASK:
            params->optlen = sizeof(uint32_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getBitmaskListVal(ATCmd_sockSocketCipher, sizeof(ATCmd_sockSocketCipher)/sizeof(StrMpl_List_t), &buff, params->optval, ATCMD_DELIM_TRM,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                if (ret != STRMPL_ERROR_PARAM_MISSING)
                {
                    return ret;
                }
                /* set to the default value */
                *(uint32_t *)(params->optval) = SL_SEC_MASK_SECURE_DEFAULT;
            }
            break;
        case SL_SO_SECURE_FILES_CA_FILE_NAME:
        case SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME:
        case SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME:
        case SL_SO_SECURE_FILES_PEER_CERT_OR_DH_KEY_FILE_NAME:
        case SL_SO_SECURE_DOMAIN_NAME_VERIFICATION:
            if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->optval, ATCMD_DELIM_TRM, 255 ,ATCmd_excludeDelimStr)) < 0)
            {
                return ret;
            }
            params->optlen = strlen((char *)params->optval);
            break;
        case SL_SO_SECURE_ALPN:
            params->optlen = sizeof(uint32_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getBitmaskListVal(ATCmd_sockSocketAlpn, sizeof(ATCmd_sockSocketAlpn)/sizeof(StrMpl_List_t), &buff, params->optval, ATCMD_DELIM_TRM,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            break;

        case SL_IP_MULTICAST_TTL:
            params->optlen = sizeof(uint8_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getVal((char **)&buff, params->optval , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            break;
        case SL_IP_ADD_MEMBERSHIP:
        case SL_IP_DROP_MEMBERSHIP:
            params->optlen = sizeof(SlSockIpMreq_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
            {
        	    return ret;
            }
            ((SlSockIpMreq_t *)(params->optval))->imr_multiaddr.s_addr = sl_Htonl(SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]));
            if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
            {
        	    return ret;
            }
            ((SlSockIpMreq_t *)(params->optval))->imr_interface.s_addr = sl_Htonl(SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]));
            break;
        case SL_SO_PHY_TX_INHIBIT_THRESHOLD :
            params->optlen = sizeof(uint32_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getListVal(ATCmd_sockPhyThreshold, sizeof(ATCmd_sockPhyThreshold)/sizeof(StrMpl_List_t), &buff, params->optval, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
            {
                return ret;
            }
            break;
        case SL_SO_LINGER:
            params->optlen = sizeof(SlSocklinger_t);
            params->optval = malloc(params->optlen);
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            if ((ret = StrMpl_getVal((char **)&buff, &(((SlSocklinger_t *)(params->optval))->l_onoff), ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            
            if ((ret = StrMpl_getVal((char **)&buff, &(((SlSocklinger_t *)(params->optval))->l_linger), ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            break;
        case SL_SO_SECURE_EXT_CLIENT_CHLNG_RESP:
            params->optval = malloc(sizeof(int32_t));
            if (params->optval == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
            params->optlen = sizeof(int32_t);            
            break;

    }
    return ret;
}


/*!
    \brief          Sock SetSockOpt callback.

    This routine Set socket options

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_setSockOptCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_SetSockOpt_t params;

    memset(&params, 0x0, sizeof(ATCmdSock_SetSockOpt_t));
    
    /* Call the command parser */
    ret = ATCmdSock_setSockOptParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_setSockOptFree(&params);
	    return -1;
    }
    
    /* set sock opt */
    ret = sl_SetSockOpt(params.sd,params.level,params.optname,(const void *)params.optval,params.optlen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdSock_setSockOptFree(&params);
    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_getSockOptFree(ATCmdSock_SetSockOpt_t *params)
{    
    if (params != NULL)
    {
        if (params->optval != NULL)
        {
            free(params->optval);
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
int32_t ATCmdSock_getSockOptParse(char *buff, ATCmdSock_SetSockOpt_t *params)
{
    int32_t ret = 0;
    
    /* SD */
    if ((ret = StrMpl_getVal((char **)&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }
    /*Level */
    if ((ret = StrMpl_getListVal(ATCmd_sockLevel, sizeof(ATCmd_sockLevel)/sizeof(StrMpl_List_t), &buff, &params->level, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }
    /* option */
    switch (params->level)
    {
        case SL_SOL_SOCKET:
            if ((ret = StrMpl_getListVal(ATCmd_sockSocketOpt, sizeof(ATCmd_sockSocketOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
        case SL_IPPROTO_IP:
            if ((ret = StrMpl_getListVal(ATCmd_sockIpOpt, sizeof(ATCmd_sockIpOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
        case SL_SOL_PHY_OPT:
            if ((ret = StrMpl_getListVal(ATCmd_sockPhyOpt, sizeof(ATCmd_sockPhyOpt)/sizeof(StrMpl_List_t), &buff, &params->optname, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
    }

    /* value */
    switch (params->optname)
    {
        case SL_SO_KEEPALIVE:
        case SL_SO_KEEPALIVETIME:
        case SL_SO_RX_NO_IP_BOUNDARY:
        case SL_SO_RCVBUF:
        case SL_SO_NONBLOCKING :
        case SL_SO_CHANGE_CHANNEL:
        case SL_SO_PHY_RATE:
        case SL_SO_PHY_TX_POWER:
        case SL_SO_PHY_NUM_FRAMES_TO_TX:
        case SL_SO_PHY_PREAMBLE:
        case SL_IP_RAW_RX_NO_HEADER:
        case SL_IP_HDRINCL:
        case SL_IP_RAW_IPV6_HDRINCL:
        case SL_SO_PHY_TX_TIMEOUT:
        case SL_SO_PHY_ALLOW_ACKS:
        case SL_SO_SECURE_MASK:
        case SL_SO_SECURE_ALPN:            
        case SL_SO_PHY_TX_INHIBIT_THRESHOLD :
        case SL_SO_SECURE_EXT_CLIENT_CHLNG_RESP:
            params->optlen = sizeof(uint32_t);
            break;
        case SL_SO_RCVTIMEO:
            params->optlen = sizeof(SlTimeval_t);
            break;
        case SL_SO_SECMETHOD:
        case SL_IP_MULTICAST_TTL:
            params->optlen = sizeof(uint8_t);
            break;
        case SL_SO_SECURE_FILES_CA_FILE_NAME:
        case SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME:
        case SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME:
        case SL_SO_SECURE_FILES_PEER_CERT_OR_DH_KEY_FILE_NAME:
        case SL_SO_SECURE_DOMAIN_NAME_VERIFICATION:
            params->optlen = 255;
            break;
        case SL_IP_ADD_MEMBERSHIP:
        case SL_IP_DROP_MEMBERSHIP:
            params->optlen = sizeof(SlSockIpMreq_t);
            break;
        case SL_SO_LINGER:
            params->optlen = sizeof(SlSocklinger_t);
            break;

    }
    params->optval = malloc(params->optlen);
    if(params->optval == NULL)
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
int32_t ATCmdSock_getSockOptResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_SetSockOpt_t *params = (ATCmdSock_SetSockOpt_t *)args;
    uint8_t val[4];
    
    StrMpl_setStr(ATCmd_sockGetSockOptStr,&buff,ATCMD_DELIM_EVENT);
    switch (params->optname)
    {
        case SL_SO_KEEPALIVE:
        case SL_SO_KEEPALIVETIME:
        case SL_SO_RX_NO_IP_BOUNDARY:
        case SL_SO_RCVBUF:
        case SL_SO_NONBLOCKING :
        case SL_SO_CHANGE_CHANNEL:
        case SL_SO_PHY_RATE:
        case SL_SO_PHY_TX_POWER:
        case SL_SO_PHY_NUM_FRAMES_TO_TX:
        case SL_SO_PHY_PREAMBLE:
        case SL_IP_RAW_RX_NO_HEADER:
        case SL_IP_HDRINCL:
        case SL_IP_RAW_IPV6_HDRINCL:
        case SL_SO_PHY_TX_TIMEOUT:
        case SL_SO_PHY_ALLOW_ACKS:           
            StrMpl_setVal(params->optval,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_SO_RCVTIMEO:
            StrMpl_setVal(&(((SlTimeval_t *)(params->optval))->tv_sec),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_SIGNED);
            StrMpl_setVal(&(((SlTimeval_t *)(params->optval))->tv_usec),&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_SIGNED);
            break;
        case SL_SO_SECMETHOD:
            StrMpl_setListStr(ATCmd_sockSocketSecMethod, sizeof(ATCmd_sockSocketSecMethod)/sizeof(StrMpl_List_t),params->optval,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_SO_SECURE_MASK:
            StrMpl_setBitmaskListStr(ATCmd_sockSocketCipher, sizeof(ATCmd_sockSocketCipher)/sizeof(StrMpl_List_t),params->optval, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_SO_SECURE_FILES_CA_FILE_NAME:
        case SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME:
        case SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME:
        case SL_SO_SECURE_FILES_PEER_CERT_OR_DH_KEY_FILE_NAME:
        case SL_SO_SECURE_DOMAIN_NAME_VERIFICATION:
            StrMpl_setStr((char *)params->optval,&buff,ATCMD_DELIM_TRM);
            break;
        case SL_SO_SECURE_ALPN:
            if (*(uint32_t *)params->optval == SL_SECURE_ALPN_FULL_LIST)
            {
                StrMpl_setListStr(ATCmd_sockSocketAlpn, sizeof(ATCmd_sockSocketAlpn)/sizeof(StrMpl_List_t),params->optval,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED);
            }
            else
            {
                StrMpl_setBitmaskListStr(ATCmd_sockSocketAlpn, sizeof(ATCmd_sockSocketAlpn)/sizeof(StrMpl_List_t),params->optval, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED);
            }
            break;

        case SL_IP_MULTICAST_TTL:
            StrMpl_setVal(params->optval,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_IP_ADD_MEMBERSHIP:
        case SL_IP_DROP_MEMBERSHIP:
            ((SlSockIpMreq_t *)(params->optval))->imr_multiaddr.s_addr = sl_Htonl(((SlSockIpMreq_t *)(params->optval))->imr_multiaddr.s_addr);
            ATCmd_valToIPv4(((SlSockIpMreq_t *)(params->optval))->imr_multiaddr.s_addr, val);
            StrMpl_setArrayVal(val,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            ((SlSockIpMreq_t *)(params->optval))->imr_interface.s_addr = sl_Htonl(((SlSockIpMreq_t *)(params->optval))->imr_interface.s_addr);
            ATCmd_valToIPv4(((SlSockIpMreq_t *)(params->optval))->imr_interface.s_addr, val);
            StrMpl_setArrayVal(val,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_SO_PHY_TX_INHIBIT_THRESHOLD :
            StrMpl_setListStr(ATCmd_sockPhyThreshold, sizeof(ATCmd_sockPhyThreshold)/sizeof(StrMpl_List_t),params->optval,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_SO_LINGER:
            StrMpl_setVal(&(((SlSocklinger_t *)(params->optval))->l_onoff),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            StrMpl_setVal(&(((SlSocklinger_t *)(params->optval))->l_linger),&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;

    }
    ATCmdSock_getSockOptFree(params);

    return 0;
}

/*!
    \brief          Sock GetSockOpt callback.

    This routine get socket options

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_getSockOptCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_SetSockOpt_t *params;

    params = malloc(sizeof(ATCmdSock_SetSockOpt_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdSock_SetSockOpt_t));
    
    /* Call the command parser */
    ret = ATCmdSock_getSockOptParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_getSockOptFree(params);
	    return -1;
    }
    
    /* set sock opt */
    ret = sl_GetSockOpt(params->sd,params->level,params->optname,params->optval,&params->optlen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdSock_getSockOptFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_getSockOptResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_sendFree(ATCmdSock_tcp_t *params)
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
int32_t ATCmdSock_sendParse(char *buff, ATCmdSock_tcp_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen;
    
    /* sd */
    if ((ret = StrMpl_getVal(&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
	    return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
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
    \brief          Sock Send callback.

    This routine write data to TCP socket

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_sendCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_tcp_t params;

    memset(&params, 0x0, sizeof(ATCmdSock_tcp_t));
    
    /* Call the command parser */
    ret = ATCmdSock_sendParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_sendFree(&params);
	    return -1;
    }

    /* send */
    ret = sl_Send(params.sd, params.data, params.len, params.flags);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdSock_sendFree(&params);
    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_recvFree(ATCmdSock_tcp_t *params)
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
int32_t ATCmdSock_recvParse(char *buff, ATCmdSock_tcp_t *params)
{
    int32_t ret = 0;
    
    /* sd */
    if ((ret = StrMpl_getVal(&buff, &params->sd, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
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

    params->data = malloc(params->len);
    if (params->data == NULL)
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
int32_t ATCmdSock_recvResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_tcp_t *params = (ATCmdSock_tcp_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_sockRecvStr,&buff,ATCMD_DELIM_EVENT);
    /* sd */
    StrMpl_setVal(&params->sd, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);

    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    /* length */
    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    StrMpl_setVal(&outputLen, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
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
    ATCmdSock_recvFree(params);

    return 0;
}

/*!
    \brief          Sock Recv callback.

    This routine     This routine read data from TCP socket

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_recvCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_tcp_t *params;

    params = malloc(sizeof(ATCmdSock_tcp_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    
    memset(params, 0x0, sizeof(ATCmdSock_tcp_t));
    
    /* Call the command parser */
    ret = ATCmdSock_recvParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_recvFree(params);
	    return -1;
    }

    /* return command complete for blocked command */
    ATCmd_okResult();

    /* recv */
    params->len = sl_Recv(params->sd, params->data, params->len, params->flags);

    if (params->len < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->len);
        ATCmdSock_recvFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_recvResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_sendToFree(ATCmdSock_udp_t *params)
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
int32_t ATCmdSock_sendToParse(char *buff, ATCmdSock_udp_t *params)
{
    int32_t ret = 0;
    uint16_t outputLen;
    
    /* parse the sd and the addr */
    if ((ret = ATCmdSock_parse(&buff, &params->target,ATCMD_DELIM_ARG)) < 0)
    {
	    return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    
    /* data length */
    if ((ret = StrMpl_getVal(&buff, &params->len, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16)) < 0)
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
    \brief          Sock SendTo callback.

    This routine write data to socket

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_sendToCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_udp_t params;

    memset(&params, 0x0, sizeof(ATCmdSock_udp_t));
    
    /* Call the command parser */
    ret = ATCmdSock_sendToParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_sendToFree(&params);
	    return -1;
    }

    /* send */
    ret = sl_SendTo(params.target.sd, params.data, params.len, params.flags,&params.target.addr,params.target.addrlen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdSock_sendToFree(&params);
    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdSock_recvFromFree(ATCmdSock_udp_t *params)
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
int32_t ATCmdSock_recvFromParse(char *buff, ATCmdSock_udp_t *params)
{
    int32_t ret = 0;
    
    /* parse the sd and the addr */
    if ((ret = ATCmdSock_parse(&buff, &params->target,ATCMD_DELIM_ARG)) < 0)
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
    
    params->data = malloc(params->len);
    if (params->data == NULL)
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
int32_t ATCmdSock_recvFromResult(void *args, int32_t num, char *buff)
{    
    ATCmdSock_udp_t *params = (ATCmdSock_udp_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_sockRecvFromStr,&buff,ATCMD_DELIM_EVENT);

    /* sd */
    StrMpl_setVal(&params->target.sd, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);

    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
    
    /* length */
    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    StrMpl_setVal(&outputLen, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

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
    ATCmdSock_recvFromFree(params);

    return 0;
}

/*!
    \brief          Sock RecvFrom callback.

    This routine read data from socket

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdSock_recvFromCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdSock_udp_t *params;

    params = malloc(sizeof(ATCmdSock_udp_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    
    memset(params, 0x0, sizeof(ATCmdSock_udp_t));
    
    /* Call the command parser */
    ret = ATCmdSock_recvFromParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdSock_recvFromFree(params);
	    return -1;
    }

    /* return command complete for blocked command */
    ATCmd_okResult();

    /* recv from */
    params->len = sl_RecvFrom(params->target.sd, params->data, params->len, params->flags,&params->target.addr,&params->target.addrlen);

    if (params->len < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->len);
        ATCmdSock_recvFromFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdSock_recvFromResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}


