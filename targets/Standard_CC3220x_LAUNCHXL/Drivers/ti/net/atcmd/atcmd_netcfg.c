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

#include <ti/net/atcmd/atcmd_netcfg.h>
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

typedef struct _ATCmdNetcfg_t_
{
    uint16_t 	id;
    uint16_t 	option;
    uint16_t 	len; 
    uint8_t     *value;
}ATCmdNetcfg_t;


//*****************************************************************************
// AT Command Netcfg Routines
//*****************************************************************************


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetcfg_setFree(ATCmdNetcfg_t *params)
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
int32_t ATCmdNetcfg_setParse(char *buff, ATCmdNetcfg_t *params)
{
    int32_t ret = 0;
    uint8_t optional = 0;
    uint8_t ip[4];
    
    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_netcfgId, sizeof(ATCmd_netcfgId)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    if ((params->id == SL_NETCFG_MAC_ADDRESS_SET) || 
        (params->id == SL_NETCFG_AP_STATION_DISCONNECT) || 
        (params->id == SL_NETCFG_IPV4_DNS_CLIENT))
    {
        optional = 1;
    }
    /* Option */
    if ((ret = StrMpl_getListVal(ATCmd_netcfgOption, sizeof(ATCmd_netcfgOption)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        if ((ret != STRMPL_ERROR_PARAM_MISSING) || (optional == 0))
        {
            return ret;
        }
    }

    /* value */
    switch (params->id)
    {
        case SL_NETCFG_MAC_ADDRESS_SET:
        case SL_NETCFG_AP_STATION_DISCONNECT:
            params->option = 1;
            params->value = malloc(SL_WLAN_BSSID_LENGTH);
            if (params->value == NULL)
            {
                return -1;
            }
            if ((ret = StrMpl_getArrayVal(&buff, (void *)params->value, SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
            {
                return ret;        
            }
            params->len = SL_WLAN_BSSID_LENGTH;
            break;
        case SL_NETCFG_IF:
            if (params->option == SL_NETCFG_IF_STATE)
            {
                params->len = sizeof(uint32_t);
                params->value = malloc(params->len);
                if (params->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getBitmaskListVal(ATCmd_netcfgIfState, sizeof(ATCmd_netcfgIfState)/sizeof(StrMpl_List_t), &buff, (void *)params->value, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, ATCmd_excludeDelimArray, STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                {
                    if (ret != STRMPL_ERROR_PARAM_MISSING)
                    {
                        return ret;
                    }
                    ret = 0;
                }
            }
            else
            {
                return -1;
            }
            break;
        case SL_NETCFG_IPV4_STA_ADDR_MODE:
        case SL_NETCFG_IPV4_AP_ADDR_MODE:
            switch (params->option)
            {
                case SL_NETCFG_ADDR_STATIC:
                    params->value = malloc(sizeof(SlNetCfgIpV4Args_t));
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    /* ip address */
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetCfgIpV4Args_t *)(params->value))->Ip = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
                    /* Subnet mask */
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetCfgIpV4Args_t *)(params->value))->IpMask = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
                    /* Default gateway address */
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetCfgIpV4Args_t *)(params->value))->IpGateway = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
                    /* DNS server address */
                    if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
                    {
                	    return ret;
                    }
                    ((SlNetCfgIpV4Args_t *)(params->value))->IpDnsServer = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
                    params->len = sizeof(SlNetCfgIpV4Args_t);
                    break;
                case SL_NETCFG_ADDR_DHCP:
                case SL_NETCFG_ADDR_DHCP_LLA:
                case SL_NETCFG_ADDR_RELEASE_IP_SET:
                case SL_NETCFG_ADDR_RELEASE_IP_OFF:
                    if (params->id == SL_NETCFG_IPV4_AP_ADDR_MODE)
                    {
                        return -1;
                    }
                    params->len = 0;
                    break;
                default:
                    return -1;
            }
            break;
        case SL_NETCFG_IPV6_ADDR_LOCAL:
            switch (params->option)
            {
                case SL_NETCFG_ADDR_STATIC:
                    params->value = malloc(sizeof(SlNetCfgIpV6Args_t));
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    /* ip address */
                    if ((ret = StrMpl_getArrayVal(&buff, (void *)((SlNetCfgIpV6Args_t *)(params->value))->Ip,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
                    {
                        return ret;
                    }
                    params->len = sizeof(SlNetCfgIpV6Args_t);
                    break;
                case SL_NETCFG_ADDR_STATEFUL:
                case SL_NETCFG_ADDR_STATELESS:
                    params->len = 0;
                    break;
                default:
                    return -1;
            }
            break;
        case SL_NETCFG_IPV6_ADDR_GLOBAL:
            switch (params->option)
            {
                case SL_NETCFG_ADDR_STATIC:
                    params->value = malloc(sizeof(SlNetCfgIpV6Args_t));
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    /* ip address */
                    if ((ret = StrMpl_getArrayVal(&buff, (void *)((SlNetCfgIpV6Args_t *)(params->value))->Ip,4,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
                    {
                        return ret;
                    }
                    /* DNS server address */
                    if ((ret = StrMpl_getArrayVal(&buff, (void *)((SlNetCfgIpV6Args_t *)(params->value))->IpDnsServer,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_32,ATCmd_excludeDelimArray)) < 0)
                    {
                        return ret;
                    }
                    params->len = sizeof(SlNetCfgIpV6Args_t);
                    break;
                case SL_NETCFG_ADDR_STATEFUL:
                case SL_NETCFG_ADDR_STATELESS:
                    params->len = 0;
                    break;
                default:
                    return -1;
            }
            break;
        case SL_NETCFG_IPV4_DNS_CLIENT:
            params->value = malloc(sizeof(SlNetCfgIpV4DnsClientArgs_t));
            if (params->value == NULL)
            {
                return -1;
            }
            params->option = 0;
            /* DNS server address */
            if ((ret = StrMpl_getArrayVal(&buff,(void *)ip,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER, STRMPL_FLAG_PARAM_SIZE_8 ,ATCmd_excludeDelimArray)) < 0)
            {
        	    return ret;
            }
            ((SlNetCfgIpV4DnsClientArgs_t *)(params->value))->DnsSecondServerAddr = SL_IPV4_VAL(ip[0],ip[1],ip[2],ip[3]);
            params->len = sizeof(SlNetCfgIpV4DnsClientArgs_t);
            break;

        default:
            return -1;
    }

    return ret;
}


/*!
    \brief          Netcfg Set callback.

    This routine sets network configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetcfg_setCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetcfg_t params;

    memset(&params, 0x0, sizeof(ATCmdNetcfg_t));
    
    /* Call the command parser */
    ret = ATCmdNetcfg_setParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetcfg_setFree(&params);
	    return -1;
    }
    
    /* set netcfg option */
    ret = sl_NetCfgSet(params.id,params.option,params.len,params.value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdNetcfg_setFree(&params);
    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetcfg_getFree(ATCmdNetcfg_t *params)
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
int32_t ATCmdNetcfg_getParse(char *buff, ATCmdNetcfg_t *params)
{
    int32_t ret = 0;
    
    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_netcfgId, sizeof(ATCmd_netcfgId)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    /* value */
    switch (params->id)
    {
        case SL_NETCFG_MAC_ADDRESS_GET:
            params->len = SL_WLAN_BSSID_LENGTH;
            break;
        case SL_NETCFG_IF:
            params->len = sizeof(uint32_t);
            break;
        case SL_NETCFG_IPV4_STA_ADDR_MODE:
        case SL_NETCFG_IPV4_AP_ADDR_MODE:
            params->len = sizeof(SlNetCfgIpV4Args_t);
            break;
        case SL_NETCFG_IPV6_ADDR_LOCAL:
        case SL_NETCFG_IPV6_ADDR_GLOBAL:
            params->len = sizeof(SlNetCfgIpV6Args_t);
            break;
        case SL_NETCFG_IPV4_DHCP_CLIENT:
            params->len = sizeof(SlNetCfgIpv4DhcpClient_t);
            break;
        case SL_NETCFG_IPV4_DNS_CLIENT:
            params->len = sizeof(SlNetCfgIpV4DnsClientArgs_t);
            break;
        case SL_NETCFG_AP_STATIONS_NUM_CONNECTED:
            params->len = sizeof(uint8_t);
            break;
        case SL_NETCFG_AP_STATIONS_INFO_LIST:
            params->len = sizeof(SlNetCfgStaInfo_t) * 4;
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
int32_t ATCmdNetcfg_getResult(void *args, int32_t num, char *buff)
{    
    int32_t ret = 0;
    ATCmdNetcfg_t *params = (ATCmdNetcfg_t *)args;
    uint8_t ip[4];
    uint8_t i, stationListSize;
    char delim;

    StrMpl_setStr(ATCmd_netcfgGetStr,&buff,ATCMD_DELIM_EVENT);

    switch (params->id)
    {
        case SL_NETCFG_MAC_ADDRESS_GET:
            StrMpl_setArrayVal(params->value,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETCFG_IF:
            StrMpl_setBitmaskListStr(ATCmd_netcfgIfState, sizeof(ATCmd_netcfgIfState)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
            break;
        case SL_NETCFG_IPV4_STA_ADDR_MODE:
        case SL_NETCFG_IPV4_AP_ADDR_MODE:
            StrMpl_setListStr(ATCmd_netcfgOption, sizeof(ATCmd_netcfgOption)/sizeof(StrMpl_List_t), &params->option,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED);
            /* address */
            ATCmd_valToIPv4(((SlNetCfgIpV4Args_t *)(params->value))->Ip, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Subnet mask */
            ATCmd_valToIPv4(((SlNetCfgIpV4Args_t *)(params->value))->IpMask, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Gateway */
            ATCmd_valToIPv4(((SlNetCfgIpV4Args_t *)(params->value))->IpGateway, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DNS */
            ATCmd_valToIPv4(((SlNetCfgIpV4Args_t *)(params->value))->IpDnsServer, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETCFG_IPV6_ADDR_LOCAL:
        case SL_NETCFG_IPV6_ADDR_GLOBAL:
            StrMpl_setListStr(ATCmd_netcfgOption, sizeof(ATCmd_netcfgOption)/sizeof(StrMpl_List_t), &params->option,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED);
            /* address */
            StrMpl_setArrayVal(((SlNetCfgIpV6Args_t *)(params->value))->Ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETCFG_IPV4_DHCP_CLIENT:
            /* address */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->Ip, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Subnet mask */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->Mask, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Gateway */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->Gateway, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DNS 1 */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->Dns[0], ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DNS 2 */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->Dns[1], ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DHCP Server */
            ATCmd_valToIPv4(((SlNetCfgIpv4DhcpClient_t *)(params->value))->DhcpServer, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Lease Time */
            StrMpl_setVal(&((SlNetCfgIpv4DhcpClient_t *)(params->value))->LeaseTime, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            /* Time To Renew */
            StrMpl_setVal(&((SlNetCfgIpv4DhcpClient_t *)(params->value))->TimeToRenew, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            /* Dhcp State */
            StrMpl_setListStr(ATCmd_netcfgDhcpState, sizeof(ATCmd_netcfgDhcpState)/sizeof(StrMpl_List_t), &((SlNetCfgIpv4DhcpClient_t *)(params->value))->DhcpState,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            break;

        case SL_NETCFG_IPV4_DNS_CLIENT:
            /* DNS */
            ATCmd_valToIPv4(((SlNetCfgIpV4DnsClientArgs_t *)(params->value))->DnsSecondServerAddr, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETCFG_AP_STATIONS_NUM_CONNECTED:
            StrMpl_setVal(params->value, &buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            break;
        case SL_NETCFG_AP_STATIONS_INFO_LIST:
            stationListSize = params->len / sizeof(SlNetCfgStaInfo_t);
            delim = ATCMD_DELIM_LIST;
            for (i=0; i<stationListSize; i++)
            {
                if (i == (stationListSize - 1))
                {
                    delim = ATCMD_DELIM_TRM;
                }
                /* address */
                ATCmd_valToIPv4(((SlNetCfgStaInfo_t *)(params->value))->Ip, ip);
                StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                /* MAC address */
                StrMpl_setArrayVal(((SlNetCfgStaInfo_t *)(params->value))->MacAddr,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                /* name */
                StrMpl_setStr((char *)((SlNetCfgStaInfo_t *)(params->value))->Name,&buff,delim);
            }
            break;

        default:
            ret = -1;
    }
    ATCmdNetcfg_getFree(params);
    return ret;
}

/*!
    \brief          Netcfg Get callback.

    This routine gets network configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetcfg_getCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetcfg_t *params;
    uint16_t *pOption = NULL;
    
    params = malloc(sizeof(ATCmdNetcfg_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetcfg_t));
    
    /* Call the command parser */
    ret = ATCmdNetcfg_getParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetcfg_getFree(params);
	    return -1;
    }
    if ((params->id != SL_NETCFG_IF) && (params->id != SL_NETCFG_AP_STATIONS_NUM_CONNECTED))
    {
        pOption = &params->option;
    }

    /* set netapp option */
    ret = sl_NetCfgGet(params->id,pOption,&params->len,params->value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetcfg_getFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetcfg_getResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

