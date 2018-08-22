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

#ifndef __ATCMD_DEFS_H__
#define __ATCMD_DEFS_H__

#include <stdint.h>
#include <ti/net/utils/str_mpl.h>

#define ATCMD_CMD_PREFIX            "at" 
#define ATCMD_DELIM_ARG             (44) //','
#define ATCMD_DELIM_CMD             (61) //'='
#define ATCMD_DELIM_EVENT           (58) //':'
#define ATCMD_DELIM_BIT             (124)//'|'
#define ATCMD_DELIM_LIST            (59) //';'
#define ATCMD_DELIM_INTER           (46) //'.'
#define ATCMD_DELIM_STR             (34) //'"'
#define ATCMD_DELIM_ARRAY           (58) //':'
#define ATCMD_DELIM_OPEN_ARRAY      (91) //'['
#define ATCMD_DELIM_CLOSE_ARRAY     (93) //']'
#define ATCMD_DELIM_TRM             (0)

#define ATCMD_DATA_FORMAT_BINARY    (0)
#define ATCMD_DATA_FORMAT_BASE64    (1)

#define ATCMD_RCV_QUEUE_SIZE        (10)

#define ATCMDHTTP_METHOD_GET     1
#define ATCMDHTTP_METHOD_POST    2
#define ATCMDHTTP_METHOD_HEAD    3
#define ATCMDHTTP_METHOD_OPTIONS 4
#define ATCMDHTTP_METHOD_PUT     5
#define ATCMDHTTP_METHOD_DELETE  6
#define ATCMDHTTP_METHOD_CONNECT 7

typedef struct _ATCmd_List_t_
{
    char        *cmd;
    int32_t     (*callback)(void *);
    char        *usage;
    uint8_t     blocked;
    uint8_t     numParams; /* case the number of params is not fixed, set this variable to 0xFF */
}ATCmd_List_t;

typedef struct _ATCmd_Event_t_
{
    int32_t (*callback)(void *, int32_t, char *);
    void    *args;
    int32_t num;
}ATCmd_Event_t;

extern ATCmd_List_t ATCmd_list[];

extern StrMpl_List_t ATCmd_devCfgId[3];
extern StrMpl_List_t ATCmd_devGeneralOptions[3];
extern StrMpl_List_t ATCmd_devStatusOptions[6];
extern StrMpl_List_t ATCmd_devIotOptions[1];
extern StrMpl_List_t ATCmd_devStatusWlan[9];
extern StrMpl_List_t ATCmd_devStatusDevice[1];
extern StrMpl_List_t ATCmd_devStatusBsd[1];
extern StrMpl_List_t ATCmd_devStatusNetapp[8];

extern StrMpl_List_t ATCmd_wlanSecurity[7];
extern StrMpl_List_t ATCmd_wlanScanSecurity[5];
extern StrMpl_List_t ATCmd_wlanScanCipher[5];
extern StrMpl_List_t ATCmd_wlanScanKeyMgmt[3];
extern StrMpl_List_t ATCmd_wlanEap[9];
extern StrMpl_List_t ATCmd_wlanPolicy[4];
extern StrMpl_List_t ATCmd_wlanConnPolicy[4];
extern StrMpl_List_t ATCmd_wlanScanPolicy[3];
extern StrMpl_List_t ATCmd_wlanPmPolicy[5];
extern StrMpl_List_t ATCmd_wlanP2pRolePolicy[3];
extern StrMpl_List_t ATCmd_wlanP2pNegPolicy[3];
extern StrMpl_List_t ATCmd_wlanRole[3];
extern StrMpl_List_t ATCmd_wlanCfg[6];
extern StrMpl_List_t ATCmd_wlanApCfg[12];
extern StrMpl_List_t ATCmd_wlanGenCfg[7];
extern StrMpl_List_t ATCmd_wlanP2pCfg[2];
extern StrMpl_List_t ATCmd_wlanRxFilterCfg[4];
extern StrMpl_List_t ATCmd_wlanApAccessList[2];
extern StrMpl_List_t ATCmd_wlanInfoElementRole[2];
extern StrMpl_List_t ATCmd_wlanConnStatus[5];

extern StrMpl_List_t ATCmd_sockDomain[3];
extern StrMpl_List_t ATCmd_sockType[3];
extern StrMpl_List_t ATCmd_sockProtocol[4];
extern StrMpl_List_t ATCmd_sockLevel[3];
extern StrMpl_List_t ATCmd_sockSocketOpt[17];
extern StrMpl_List_t ATCmd_sockIpOpt[6];
extern StrMpl_List_t ATCmd_sockPhyOpt[7];
extern StrMpl_List_t ATCmd_sockSocketSecMethod[5];
extern StrMpl_List_t ATCmd_sockSocketCipher[23];
extern StrMpl_List_t ATCmd_sockSocketAlpn[6];
extern StrMpl_List_t ATCmd_sockPhyThreshold[6];

extern StrMpl_List_t ATCmd_fileOpenOption[11];
extern StrMpl_List_t ATCmd_fileCtlCmd[7];
extern StrMpl_List_t ATCmd_fileCtlCmdRestore[2];
extern StrMpl_List_t ATCmd_fileGetInfo[13];
extern StrMpl_List_t ATCmd_fileGetInfoToken[4];

extern StrMpl_List_t ATCmd_netappType[6];
extern StrMpl_List_t ATCmd_netappFlags[3];
extern StrMpl_List_t ATCmd_netappServiceListFlags[6];
extern StrMpl_List_t ATCmd_netappRegServiceOptions[5];
extern StrMpl_List_t ATCmd_netappHttpSrvOptions[14];
extern StrMpl_List_t ATCmd_netappMdnsOptions[3];
extern StrMpl_List_t ATCmd_netappDevOptions[2];
extern StrMpl_List_t ATCmd_netappDnsClientOptions[1];
extern StrMpl_List_t ATCmd_netappDhcpSrvOptions[1];
extern StrMpl_List_t ATCmd_netappMdnsTypes[20];

extern StrMpl_List_t ATCmd_netcfgId[12];
extern StrMpl_List_t ATCmd_netcfgOption[8];
extern StrMpl_List_t ATCmd_netcfgIfState[12];
extern StrMpl_List_t ATCmd_netcfgDhcpState[6];
    
extern StrMpl_List_t ATCmd_eventDeviceId[7];
extern StrMpl_List_t ATCmd_eventDeviceSender[5];
extern StrMpl_List_t ATCmd_eventWlanId[14];
extern StrMpl_List_t ATCmd_eventWlanWpsMethod[6];
extern StrMpl_List_t ATCmd_eventWlanStatus[4];
extern StrMpl_List_t ATCmd_eventWlanProvisioningStatus[18];
extern StrMpl_List_t ATCmd_eventSockId[2];
extern StrMpl_List_t ATCmd_eventSockType[6];
extern StrMpl_List_t ATCmd_eventNetappId[8];
extern StrMpl_List_t ATCmd_eventHttpServerId[2];

extern StrMpl_List_t ATCmd_netUtilGet[2];
extern StrMpl_List_t ATCmd_netUtilCmd[5];
extern StrMpl_List_t ATCmd_netUtilTempKeys[2];
extern StrMpl_List_t ATCmd_netUtilInstallOp[2];
extern StrMpl_List_t ATCmd_netUtilPubKeyAlgo[2];
extern StrMpl_List_t ATCmd_netUtilEcNamedCurve[2];

extern StrMpl_List_t ATCmd_mqttCreateFlags[7];
extern StrMpl_List_t ATCmd_mqttCreateMode[2];
extern StrMpl_List_t ATCmd_mqttQos[3];
extern StrMpl_List_t ATCmd_mqttSetOptions[5];
extern StrMpl_List_t ATCmd_mqttEventId[3];
extern StrMpl_List_t ATCmd_mqttEventOperationId[4];

extern StrMpl_List_t ATCmd_httpConnectFlags[2];
extern StrMpl_List_t ATCmd_httpSendReqFlags[3];
extern StrMpl_List_t ATCmd_httpSendReqMethod[7];
extern StrMpl_List_t ATCmd_httpSetHeaderFlags[2];
extern StrMpl_List_t ATCmd_httpHeaderOption[58];
extern StrMpl_List_t ATCmd_httpOptOption[3];


extern const uint32_t ATCmd_maxCmd;

extern char ATCmd_wlanScanStr[];
extern char ATCmd_wlanScanUsageStr[];
extern char ATCmd_wlanProfileAddStr[];
extern char ATCmd_wlanProfileAddUsageStr[];
extern char ATCmd_wlanProfileGetStr[];
extern char ATCmd_wlanProfileGetUsageStr[];
extern char ATCmd_wlanProfileDelStr[];
extern char ATCmd_wlanProfileDelUsageStr[];
extern char ATCmd_wlanPolicySetStr[];
extern char ATCmd_wlanPolicySetUsageStr[];
extern char ATCmd_wlanPolicyGetStr[];
extern char ATCmd_wlanPolicyGetUsageStr[];
extern char ATCmd_wlanSetModeStr[];
extern char ATCmd_wlanSetModeUsageStr[];
extern char ATCmd_wlanSetStr[];
extern char ATCmd_wlanSetUsageStr[];
extern char ATCmd_wlanGetStr[];
extern char ATCmd_wlanGetUsageStr[];
extern char ATCmd_wlanConnectStr[];
extern char ATCmd_wlanConnectUsageStr[];
extern char ATCmd_wlanDisconnectStr[];
extern char ATCmd_wlanDisconnectUsageStr[];

extern char ATCmd_devStartStr[];
extern char ATCmd_devStartUsageStr[];
extern char ATCmd_devStopStr[];
extern char ATCmd_devStopUsageStr[];
extern char ATCmd_devTestStr[];
extern char ATCmd_devTestUsageStr[];
extern char ATCmd_devGetStr[];
extern char ATCmd_devGetUsageStr[];
extern char ATCmd_devSetStr[];
extern char ATCmd_devSetUsageStr[];

extern char ATCmd_sockSocketStr[];
extern char ATCmd_sockSocketUsageStr[];
extern char ATCmd_sockCloseStr[];
extern char ATCmd_sockCloseUsageStr[];
extern char ATCmd_sockAcceptStr[];
extern char ATCmd_sockAcceptUsageStr[];
extern char ATCmd_sockBindStr[];
extern char ATCmd_sockBindUsageStr[];
extern char ATCmd_sockListenStr[];
extern char ATCmd_sockListenUsageStr[];
extern char ATCmd_sockConnectStr[];
extern char ATCmd_sockConnectUsageStr[];
extern char ATCmd_sockSelectStr[];
extern char ATCmd_sockSelectUsageStr[];
extern char ATCmd_sockSetSockOptStr[];
extern char ATCmd_sockSetSockOptUsageStr[];
extern char ATCmd_sockGetSockOptStr[];
extern char ATCmd_sockGetSockOptUsageStr[];
extern char ATCmd_sockSendStr[];
extern char ATCmd_sockSendUsageStr[];
extern char ATCmd_sockRecvStr[];
extern char ATCmd_sockRecvUsageStr[];
extern char ATCmd_sockSendToStr[];
extern char ATCmd_sockSendToUsageStr[];
extern char ATCmd_sockRecvFromStr[];
extern char ATCmd_sockRecvFromUsageStr[];

extern char ATCmd_fileOpenStr[];
extern char ATCmd_fileOpenUsageStr[];
extern char ATCmd_fileCloseStr[];
extern char ATCmd_fileCloseUsageStr[];
extern char ATCmd_fileCtlStr[];
extern char ATCmd_fileCtlUsageStr[];
extern char ATCmd_fileDelStr[];
extern char ATCmd_fileDelUsageStr[];
extern char ATCmd_fileGetFileListStr[];
extern char ATCmd_fileGetFileListUsageStr[];
extern char ATCmd_fileGetInfoStr[];
extern char ATCmd_fileGetInfoUsageStr[];
extern char ATCmd_fileReadStr[];
extern char ATCmd_fileReadUsageStr[];
extern char ATCmd_fileWriteStr[];
extern char ATCmd_fileWriteUsageStr[];

extern char ATCmd_netappStartStr[];
extern char ATCmd_netappStartUsageStr[];
extern char ATCmd_netappStopStr[];
extern char ATCmd_netappStopUsageStr[];
extern char ATCmd_netappGetHostByNameStr[];
extern char ATCmd_netappGetHostByNameUsageStr[];
extern char ATCmd_netappGetHostByServiceStr[];
extern char ATCmd_netappGetHostByServiceUsageStr[];
extern char ATCmd_netappSendStr[];
extern char ATCmd_netappSendUsageStr[];
extern char ATCmd_netappRecvStr[];
extern char ATCmd_netappRecvUsageStr[];
extern char ATCmd_netappPingStr[];
extern char ATCmd_netappPingUsageStr[];
extern char ATCmd_netappGetServiceListStr[];
extern char ATCmd_netappGetServiceListUsageStr[];
extern char ATCmd_netappRegisterServiceStr[];
extern char ATCmd_netappRegisterServiceUsageStr[];
extern char ATCmd_netappUnregisterServiceStr[];
extern char ATCmd_netappUnregisterServiceUsageStr[];
extern char ATCmd_netappSetStr[];
extern char ATCmd_netappSetUsageStr[];
extern char ATCmd_netappGetStr[];
extern char ATCmd_netappGetUsageStr[];

extern char ATCmd_netcfgSetStr[];
extern char ATCmd_netcfgSetUsageStr[];
extern char ATCmd_netcfgGetStr[];
extern char ATCmd_netcfgGetUsageStr[];

extern char ATCmd_netUtilCmdStr[];
extern char ATCmd_netUtilCmdUsageStr[];
extern char ATCmd_netUtilGetStr[];
extern char ATCmd_netUtilGetUsageStr[];

extern char ATCmd_mqttCreateStr[];
extern char ATCmd_mqttCreateUsageStr[];
extern char ATCmd_mqttDeleteStr[];
extern char ATCmd_mqttDeleteUsageStr[];
extern char ATCmd_mqttConnectStr[];
extern char ATCmd_mqttConnectUsageStr[];
extern char ATCmd_mqttDisconnectStr[];
extern char ATCmd_mqttDisconnectUsageStr[];
extern char ATCmd_mqttPublishStr[];
extern char ATCmd_mqttPublishUsageStr[];
extern char ATCmd_mqttSubscribeStr[];
extern char ATCmd_mqttSubscribeUsageStr[];
extern char ATCmd_mqttUnsubscribeStr[];
extern char ATCmd_mqttUnsubscribeUsageStr[];
extern char ATCmd_mqttSetStr[];
extern char ATCmd_mqttSetUsageStr[];

extern char ATCmd_httpCreateStr[];
extern char ATCmd_httpCreateUsageStr[];
extern char ATCmd_httpDestroyStr[];
extern char ATCmd_httpDestroyUsageStr[];
extern char ATCmd_httpConnectStr[];
extern char ATCmd_httpConnectUsageStr[];
extern char ATCmd_httpDisconnectStr[];
extern char ATCmd_httpDisconnectUsageStr[];
extern char ATCmd_httpSendReqStr[];
extern char ATCmd_httpSendReqUsageStr[];
extern char ATCmd_httpReadResBodyStr[];
extern char ATCmd_httpReadResBodyUsageStr[];
extern char ATCmd_httpSetHeaderStr[];
extern char ATCmd_httpSetHeaderUsageStr[];
extern char ATCmd_httpGetHeaderStr[];
extern char ATCmd_httpGetHeaderUsageStr[];
extern char ATCmd_httpSetOptStr[];
extern char ATCmd_httpSetOptUsageStr[];
extern char ATCmd_httpGetOptStr[];
extern char ATCmd_httpGetOptUsageStr[];
extern char ATCmd_httpSetProxyStr[];
extern char ATCmd_httpSetProxyUsageStr[];

extern char ATCmd_okStr[];
extern char ATCmd_errorNumParamsStr[];
extern char ATCmd_errorAllocStr[];
extern char ATCmd_errorParseStr[];
extern char ATCmd_errorCmdStr[];
extern char ATCmd_errorExistCmdStr[];

extern char ATCmd_eventGeneralStr[];
extern char ATCmd_eventWlanStr[];
extern char ATCmd_eventSockStr[];
extern char ATCmd_eventNetappStr[];
extern char ATCmd_eventHttpServerStr[];
extern char ATCmd_eventFatalErrorStr[];
extern char ATCmd_eventMqttStr[];
extern char ATCmd_eventHttpStr[];

extern char ATCmd_excludeDelimStr[2];
extern char ATCmd_excludeDelimArray[2];



#endif /* __ATCMD_DEFS_H__ */

