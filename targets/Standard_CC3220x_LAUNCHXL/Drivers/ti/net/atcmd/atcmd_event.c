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
#include "unistd.h"
#include "mqueue.h"

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/utils/str_mpl.h>

/* AT Header files */
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd.h>
#include <ti/net/atcmd/atcmd_gen.h>

//*****************************************************************************
// defines
//*****************************************************************************


//*****************************************************************************
// typedefs
//*****************************************************************************


/******************************************************************************
// AT Events Routines
******************************************************************************/

/*!
    \brief          Compose event.

    \param          event      -   Points to event struct data.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdEvent_compose(void *event, uint32_t size, int32_t (*callback)(void *, int32_t, char *))
{
    void *params;
    ATCmd_Event_t eventMsg;

    params = malloc(size);
    if (params == NULL)
    {
        return -1;
    }
    memcpy(params,event,size);

    eventMsg.callback = callback;
    eventMsg.args = params;
    ATCmd_signalEvent(&eventMsg);  

    return 0;

}

/*!
    \brief          Fatal Error Event.

    This routine send Fatal Error event to host as AT event.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdEvent_fatalErrorCallback(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    SlDeviceFatal_t  *params = (SlDeviceFatal_t *)(args);

    StrMpl_setStr(ATCmd_eventFatalErrorStr,&buff,ATCMD_DELIM_EVENT);

    StrMpl_setListStr(ATCmd_eventDeviceId, sizeof(ATCmd_eventDeviceId)/sizeof(StrMpl_List_t), &params->Id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
    switch (params->Id)
    {
        case SL_DEVICE_EVENT_FATAL_DEVICE_ABORT:
            /* code */
            StrMpl_setVal(&params->Data.DeviceAssert.Code,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* value */
            StrMpl_setVal(&params->Data.DeviceAssert.Value,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_DEVICE_EVENT_FATAL_DRIVER_ABORT:
	    case SL_DEVICE_EVENT_FATAL_SYNC_LOSS:
            break;
	    case SL_DEVICE_EVENT_FATAL_NO_CMD_ACK:
            /* code */
            StrMpl_setVal(&params->Data.NoCmdAck.Code,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
	    case SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT:
            /* code */
            StrMpl_setVal(&params->Data.CmdTimeout.Code,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        default:
            ret = -1;
            break;
    }

    free (params);
    return ret;
}

/*!
    \brief          Device General Event.

    This routine send device general event to host as AT event.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdEvent_deviceGeneralCallback(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    SlDeviceEvent_t  *params = (SlDeviceEvent_t *)(args);

    if ((params->Id == SL_DEVICE_EVENT_RESET_REQUEST) || (params->Id == SL_DEVICE_EVENT_ERROR))
    {
        StrMpl_setStr(ATCmd_eventGeneralStr,&buff,ATCMD_DELIM_EVENT);
        StrMpl_setListStr(ATCmd_eventDeviceId, sizeof(ATCmd_eventDeviceId)/sizeof(StrMpl_List_t), &params->Id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&params->Data.Error.Code,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_16 | STRMPL_FLAG_PARAM_SIGNED);
        StrMpl_setListStr(ATCmd_eventDeviceSender, sizeof(ATCmd_eventDeviceSender)/sizeof(StrMpl_List_t), &params->Data.Error.Source,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else
    {
        ret = -1;
    }
    free (params);
    return ret;
}


/*!
    \brief          wlan async Event.

    This routine send wlan event to host as AT event.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdEvent_wlanCallback(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    SlWlanEvent_t  *params = (SlWlanEvent_t *)(args);

    StrMpl_setStr(ATCmd_eventWlanStr,&buff,ATCMD_DELIM_EVENT);

    StrMpl_setListStr(ATCmd_eventWlanId, sizeof(ATCmd_eventWlanId)/sizeof(StrMpl_List_t), &params->Id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
    switch (params->Id)
    {
        case SL_WLAN_EVENT_CONNECT:
            /* ssid */
            params->Data.Connect.SsidName[params->Data.Connect.SsidLen] = 0;
            StrMpl_setStr((char *)params->Data.Connect.SsidName,&buff,ATCMD_DELIM_ARG);
            /* bssid */
            StrMpl_setArrayVal(params->Data.Connect.Bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_WLAN_EVENT_DISCONNECT:
            /* ssid */
            params->Data.Disconnect.SsidName[params->Data.Disconnect.SsidLen] = 0;
            StrMpl_setStr((char *)params->Data.Disconnect.SsidName,&buff,ATCMD_DELIM_ARG);
            /* bssid */
            StrMpl_setArrayVal(params->Data.Disconnect.Bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Reason */
            StrMpl_setVal(&params->Data.Disconnect.ReasonCode,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_WLAN_EVENT_STA_ADDED:
        case SL_WLAN_EVENT_STA_REMOVED:
            /* mac */
            StrMpl_setArrayVal(params->Data.STAAdded.Mac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_WLAN_EVENT_P2P_CONNECT:
            /* ssid */
            params->Data.P2PConnect.SsidName[params->Data.P2PConnect.SsidLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PConnect.SsidName,&buff,ATCMD_DELIM_ARG);
            /* mac */
            StrMpl_setArrayVal(params->Data.P2PConnect.Bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* go device name */
            params->Data.P2PConnect.GoDeviceName[params->Data.P2PConnect.GoDeviceNameLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PConnect.GoDeviceName,&buff,ATCMD_DELIM_TRM);
            break;
        case SL_WLAN_EVENT_P2P_DISCONNECT:
            /* ssid */
            params->Data.P2PDisconnect.SsidName[params->Data.P2PDisconnect.SsidLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PDisconnect.SsidName,&buff,ATCMD_DELIM_ARG);
            /* mac */
            StrMpl_setArrayVal(params->Data.P2PDisconnect.Bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Reason */
            StrMpl_setVal(&params->Data.P2PDisconnect.ReasonCode,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* go device name */
            params->Data.P2PDisconnect.GoDeviceName[params->Data.P2PDisconnect.GoDeviceNameLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PDisconnect.GoDeviceName,&buff,ATCMD_DELIM_TRM);
            break;
        case SL_WLAN_EVENT_P2P_CLIENT_ADDED:
        case SL_WLAN_EVENT_P2P_CLIENT_REMOVED:
            /* mac */
            StrMpl_setArrayVal(params->Data.P2PClientAdded.Mac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* go device name */
            params->Data.P2PClientAdded.ClDeviceName[params->Data.P2PClientAdded.ClDeviceNameLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PClientAdded.ClDeviceName,&buff,ATCMD_DELIM_ARG);
            /* own ssid */
            params->Data.P2PClientAdded.OwnSsid[params->Data.P2PClientAdded.OwnSsidLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PClientAdded.OwnSsid,&buff,ATCMD_DELIM_TRM);
            break;
        case SL_WLAN_EVENT_P2P_DEVFOUND:
        case SL_WLAN_EVENT_P2P_REQUEST:
            /* go device name */
            params->Data.P2PDevFound.GoDeviceName[params->Data.P2PDevFound.GoDeviceNameLen] = 0;
            StrMpl_setStr((char *)params->Data.P2PDevFound.GoDeviceName,&buff,ATCMD_DELIM_ARG);
            /* mac */
            StrMpl_setArrayVal(params->Data.P2PDevFound.Mac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* wps */
            StrMpl_setListStr(ATCmd_eventWlanWpsMethod, sizeof(ATCmd_eventWlanWpsMethod)/sizeof(StrMpl_List_t), &params->Data.P2PDevFound.WpsMethod,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_WLAN_EVENT_P2P_CONNECTFAIL:
            /* status */
            StrMpl_setListStr(ATCmd_eventWlanStatus, sizeof(ATCmd_eventWlanStatus)/sizeof(StrMpl_List_t), &params->Data.P2PConnectFail.Status,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_WLAN_EVENT_PROVISIONING_STATUS:
            /* Provisioning status */
            StrMpl_setListStr(ATCmd_eventWlanProvisioningStatus, sizeof(ATCmd_eventWlanProvisioningStatus)/sizeof(StrMpl_List_t), &params->Data.ProvisioningStatus.ProvisioningStatus,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            /* role */
            StrMpl_setListStr(ATCmd_wlanRole, sizeof(ATCmd_wlanRole)/sizeof(StrMpl_List_t), &params->Data.ProvisioningStatus.Role,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            /* wlan status */
            StrMpl_setListStr(ATCmd_eventWlanStatus, sizeof(ATCmd_eventWlanStatus)/sizeof(StrMpl_List_t), &params->Data.ProvisioningStatus.WlanStatus,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            /* ssid */
            params->Data.ProvisioningStatus.Ssid[params->Data.ProvisioningStatus.Ssidlen] = 0;
            StrMpl_setStr((char *)params->Data.ProvisioningStatus.Ssid,&buff,ATCMD_DELIM_TRM);
            break;
        case SL_WLAN_EVENT_PROVISIONING_PROFILE_ADDED:
            /* status */
            StrMpl_setListStr(ATCmd_eventWlanProvisioningStatus, sizeof(ATCmd_eventWlanProvisioningStatus)/sizeof(StrMpl_List_t), &params->Data.ProvisioningProfileAdded.Status,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
            /* ssid */
            params->Data.ProvisioningProfileAdded.Ssid[params->Data.ProvisioningProfileAdded.SsidLen] = 0;
            StrMpl_setStr((char *)params->Data.ProvisioningProfileAdded.Ssid,&buff,ATCMD_DELIM_TRM);
            break;
        default:
            ret = -1;
            break;
    }

    free (params);
    return ret;
}

/*!
    \brief          socket async Event.

    This routine send socket event to host as AT event.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdEvent_sockCallback(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    SlSockEvent_t  *params = (SlSockEvent_t *)(args);

    StrMpl_setStr(ATCmd_eventSockStr,&buff,ATCMD_DELIM_EVENT);

    StrMpl_setListStr(ATCmd_eventSockId, sizeof(ATCmd_eventSockId)/sizeof(StrMpl_List_t), &params->Event,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
    switch (params->Event)
    {
        case SL_SOCKET_TX_FAILED_EVENT:
            /* sd */
            StrMpl_setVal(&params->SocketAsyncEvent.SockTxFailData.Sd,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* status */
            StrMpl_setVal(&params->SocketAsyncEvent.SockTxFailData.Status,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);
            break;
        case SL_SOCKET_ASYNC_EVENT:
            /* sd */
            StrMpl_setVal(&params->SocketAsyncEvent.SockAsyncData.Sd,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* type */
            StrMpl_setListStr(ATCmd_eventSockType, sizeof(ATCmd_eventSockType)/sizeof(StrMpl_List_t), &params->SocketAsyncEvent.SockAsyncData.Type,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
            /* val */
            StrMpl_setVal(&params->SocketAsyncEvent.SockAsyncData.Val,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);
            break;
        default:
            ret = -1;
            break;
    }

    free (params);
    return ret;
}

/*!
    \brief          netapp async Event.

    This routine send netapp event to host as AT event.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdEvent_netappCallback(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    SlNetAppEvent_t  *params = (SlNetAppEvent_t *)(args);
    uint8_t ip[4];

    StrMpl_setStr(ATCmd_eventNetappStr,&buff,ATCMD_DELIM_EVENT);

    StrMpl_setListStr(ATCmd_eventNetappId, sizeof(ATCmd_eventNetappId)/sizeof(StrMpl_List_t), &params->Id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
    switch (params->Id)
    {
        case SL_NETAPP_EVENT_IPV4_ACQUIRED:
            /* address */
            ATCmd_valToIPv4(params->Data.IpAcquiredV4.Ip, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Gateway */
            ATCmd_valToIPv4(params->Data.IpAcquiredV4.Gateway, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DNS */
            ATCmd_valToIPv4(params->Data.IpAcquiredV4.Dns, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETAPP_EVENT_IPV6_ACQUIRED:
            /* address */
            StrMpl_setArrayVal(params->Data.IpAcquiredV6.Ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* dns */
            StrMpl_setArrayVal(params->Data.IpAcquiredV6.Dns,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        case SL_NETAPP_EVENT_IP_COLLISION:
            /* address */
            ATCmd_valToIPv4(params->Data.IpCollision.IpAddress, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* DHCP mac */
            StrMpl_setArrayVal(params->Data.IpCollision.DhcpMac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Conflict mac */
            StrMpl_setArrayVal(params->Data.IpCollision.ConflictMac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
    	case SL_NETAPP_EVENT_DHCPV4_LEASED:
            /* address */
            ATCmd_valToIPv4(params->Data.IpLeased.IpAddress, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* lease time */
            StrMpl_setVal(&params->Data.IpLeased.LeaseTime,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            /* bssid */
            StrMpl_setArrayVal(params->Data.IpLeased.Mac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
    	case SL_NETAPP_EVENT_DHCPV4_RELEASED:
            /* address */
            ATCmd_valToIPv4(params->Data.IpReleased.IpAddress, ip);
            StrMpl_setArrayVal(ip,&buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* bssid */
            StrMpl_setArrayVal(params->Data.IpReleased.Mac,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            /* Reason */
            StrMpl_setVal(&params->Data.IpReleased.Reason,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            break;
    	case SL_NETAPP_EVENT_IPV4_LOST:
            /* status */
            StrMpl_setVal(&params->Data.IpV4Lost.Status,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);
            break;
    	case SL_NETAPP_EVENT_DHCP_IPV4_ACQUIRE_TIMEOUT:
            /* status */
            StrMpl_setVal(&params->Data.DhcpIpAcquireTimeout.Status,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);
            break;
    	case SL_NETAPP_EVENT_IPV6_LOST:
            /* ip lost */
            StrMpl_setArrayVal(params->Data.IpV6Lost.IpLost,&buff,4,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        default:
            ret = -1;
            break;
    }

    free (params);
    return ret;
}



