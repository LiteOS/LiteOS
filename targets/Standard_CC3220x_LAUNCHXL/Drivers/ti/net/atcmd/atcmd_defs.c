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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/net/mqtt/mqttclient.h>
#include <ti/net/http/httpclient.h>

#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_device.h>
#include <ti/net/atcmd/atcmd_wlan.h>
#include <ti/net/atcmd/atcmd_socket.h>
#include <ti/net/atcmd/atcmd_file.h>
#include <ti/net/atcmd/atcmd_netapp.h>
#include <ti/net/atcmd/atcmd_netcfg.h>
#include <ti/net/atcmd/atcmd_netutil.h>
#include <ti/net/atcmd/atcmd_mqtt.h>
#include <ti/net/atcmd/atcmd_http.h>

ATCmd_List_t ATCmd_list[] =
{
                                    /* command */               /* Command callback */                    /* Command usage */              /* Blocked command */  /* Num parameters*/
/* Start cmd        */              { ATCmd_devStartStr,              ATCmdDev_startCallback,             ATCmd_devStartUsageStr,                   0,                  0 },
/* Stop cmd         */              { ATCmd_devStopStr,               ATCmdDev_stopCallback,              ATCmd_devStopUsageStr,                    0,                  1 },
/* Test cmd         */              { ATCmd_devTestStr,               ATCmdDev_testCallback,              ATCmd_devTestUsageStr,                    0,                  0 },
/* Get cmd          */              { ATCmd_devGetStr,                ATCmdDev_getCallback,               ATCmd_devGetUsageStr,                     0,                  2 },
/* Set cmd          */              { ATCmd_devSetStr,                ATCmdDev_setCallback,               ATCmd_devSetUsageStr,                     0,                  0xFF},
/* Connect cmd      */              { ATCmd_wlanConnectStr,           ATCmdWlan_connectCallback,          ATCmd_wlanConnectUsageStr,                0,                  7},
/* Scan cmd         */              { ATCmd_wlanScanStr,              ATCmdWlan_scanCallback,             ATCmd_wlanScanUsageStr,                   1,                  2},
/* Disconnect cmd   */              { ATCmd_wlanDisconnectStr,        ATCmdWlan_disconnectCallback,       ATCmd_wlanDisconnectUsageStr,             0,                  0},
/* Add profile cmd  */              { ATCmd_wlanProfileAddStr,        ATCmdWlan_profileAddCallback,       ATCmd_wlanProfileAddUsageStr,             0,                  8},
/* Get profile cmd  */              { ATCmd_wlanProfileGetStr,        ATCmdWlan_profileGetCallback,       ATCmd_wlanProfileGetUsageStr,             0,                  1},
/* Del profile cmd  */              { ATCmd_wlanProfileDelStr,        ATCmdWlan_profileDelCallback,       ATCmd_wlanProfileDelUsageStr,             0,                  1 },
/* Set policy cmd   */              { ATCmd_wlanPolicySetStr,         ATCmdWlan_policySetCallback,        ATCmd_wlanPolicySetUsageStr,              0,                  3 },
/* Get policy cmd   */              { ATCmd_wlanPolicyGetStr,         ATCmdWlan_policyGetCallback,        ATCmd_wlanPolicyGetUsageStr,              0,                  1 },
/* Set mode cmd     */              { ATCmd_wlanSetModeStr,           ATCmdWlan_setModeCallback,          ATCmd_wlanSetModeUsageStr,                0,                  1 },
/* Set  cmd         */              { ATCmd_wlanSetStr,               ATCmdWlan_setCallback,              ATCmd_wlanSetUsageStr,                    0,                  0xFF},
/* Get  cmd         */              { ATCmd_wlanGetStr,               ATCmdWlan_getCallback,              ATCmd_wlanGetUsageStr,                    0,                  0xFF},
/* Socket  cmd      */              { ATCmd_sockSocketStr,            ATCmdSock_socketCallback,           ATCmd_sockSocketUsageStr,                 0,                  3 },
/* Close  cmd       */              { ATCmd_sockCloseStr,             ATCmdSock_closeCallback,            ATCmd_sockCloseUsageStr,                  1,                  1 },
/* Accept  cmd      */              { ATCmd_sockAcceptStr,            ATCmdSock_acceptCallback,           ATCmd_sockAcceptUsageStr,                 1,                  2 },
/* Bind  cmd        */              { ATCmd_sockBindStr,              ATCmdSock_bindCallback,             ATCmd_sockBindUsageStr,                   0,                  4 },
/* Listen  cmd      */              { ATCmd_sockListenStr,            ATCmdSock_listenCallback,           ATCmd_sockListenUsageStr,                 0,                  2 },
/* Connect  cmd     */              { ATCmd_sockConnectStr,           ATCmdSock_connectCallback,          ATCmd_sockConnectUsageStr,                1,                  4 },
/* Select  cmd      */              { ATCmd_sockSelectStr,            ATCmdSock_selectCallback,           ATCmd_sockSelectUsageStr,                 1,                  4 },
/* Setsockopt  cmd  */              { ATCmd_sockSetSockOptStr,        ATCmdSock_setSockOptCallback,       ATCmd_sockSetSockOptUsageStr,             0,                  0xFF },
/* Getsockopt  cmd  */              { ATCmd_sockGetSockOptStr,        ATCmdSock_getSockOptCallback,       ATCmd_sockGetSockOptUsageStr,             0,                  3 },
/* send  cmd        */              { ATCmd_sockSendStr,              ATCmdSock_sendCallback,             ATCmd_sockSendUsageStr,                   0,                  0xFF },
/* recv  cmd        */              { ATCmd_sockRecvStr,              ATCmdSock_recvCallback,             ATCmd_sockRecvUsageStr,                   1,                  3 },
/* sendto  cmd      */              { ATCmd_sockSendToStr,            ATCmdSock_sendToCallback,           ATCmd_sockSendToUsageStr,                 0,                  0xFF },
/* recvfrom  cmd    */              { ATCmd_sockRecvFromStr,          ATCmdSock_recvFromCallback,         ATCmd_sockRecvFromUsageStr,               1,                  6 },
/* file open  cmd   */              { ATCmd_fileOpenStr,              ATCmdFile_openCallback,             ATCmd_fileOpenUsageStr,                   1,                  3 },
/* file close  cmd  */              { ATCmd_fileCloseStr,             ATCmdFile_closeCallback,            ATCmd_fileCloseUsageStr,                  1,                  3 },
/* file ctl  cmd    */              { ATCmd_fileCtlStr,               ATCmdFile_ctlCallback,              ATCmd_fileCtlUsageStr,                    1,                  4 },
/* file del  cmd    */              { ATCmd_fileDelStr,               ATCmdFile_delCallback,              ATCmd_fileDelUsageStr,                    1,                  2 },
/* file getfile list cmd */         { ATCmd_fileGetFileListStr,       ATCmdFile_getFileListCallback,      ATCmd_fileGetFileListUsageStr,            1,                  0 },
/* file getinfo cmd */              { ATCmd_fileGetInfoStr,           ATCmdFile_getInfoCallback,          ATCmd_fileGetInfoUsageStr,                1,                  2 },
/* file read cmd    */              { ATCmd_fileReadStr,              ATCmdFile_readCallback,             ATCmd_fileReadUsageStr,                   1,                  4 },
/* file write cmd   */              { ATCmd_fileWriteStr,             ATCmdFile_writeCallback,            ATCmd_fileWriteUsageStr,                  1,                  0xFF },
/* netapp start cmd */              { ATCmd_netappStartStr,           ATCmdNetapp_startCallback,          ATCmd_netappStartUsageStr,                0,                  1 },
/* netapp stop cmd  */              { ATCmd_netappStopStr,            ATCmdNetapp_stopCallback,           ATCmd_netappStopUsageStr,                 0,                  1 },
/* netapp getHostByName */          { ATCmd_netappGetHostByNameStr,   ATCmdNetapp_getHostByNameCallback,  ATCmd_netappGetHostByNameUsageStr,        1,                  2 },
/* netapp getHostByService cmd  */  { ATCmd_netappGetHostByServiceStr,ATCmdNetapp_getHostByServiceCallback,ATCmd_netappGetHostByServiceUsageStr,    1,                  2 },
/* netapp send cmd */               { ATCmd_netappSendStr,            ATCmdNetapp_sendCallback,           ATCmd_netappSendUsageStr,                 0,                  0xFF },
/* netapp recv cmd  */              { ATCmd_netappRecvStr,            ATCmdNetapp_recvCallback,           ATCmd_netappRecvUsageStr,                 1,                  3 },
/* netapp ping cmd */               { ATCmd_netappPingStr,            ATCmdNetapp_pingCallback,           ATCmd_netappPingUsageStr,                 1,                  7 },
/* netapp getServiceList cmd  */    { ATCmd_netappGetServiceListStr,  ATCmdNetapp_getServiceListCallback, ATCmd_netappGetServiceListUsageStr,       0,                  3 },
/* netapp registerService cmd */    { ATCmd_netappRegisterServiceStr, ATCmdNetapp_registerServiceCallback,ATCmd_netappRegisterServiceUsageStr,      0,                  5 },
/* netapp unregisterService cmd  */ { ATCmd_netappUnregisterServiceStr,ATCmdNetapp_unregisterServiceCallback,ATCmd_netappUnregisterServiceUsageStr, 0,                  2 },
/* netapp set cmd  */               { ATCmd_netappSetStr,             ATCmdNetapp_setCallback,            ATCmd_netappSetUsageStr,                  0,                  0xFF },
/* netapp get cmd  */               { ATCmd_netappGetStr,             ATCmdNetapp_getCallback,            ATCmd_netappGetUsageStr,                  0,                  2 },
/* netcfg set cmd  */               { ATCmd_netcfgSetStr,             ATCmdNetcfg_setCallback,            ATCmd_netcfgSetUsageStr,                  0,                  0xFF },
/* netcfg get cmd  */               { ATCmd_netcfgGetStr,             ATCmdNetcfg_getCallback,            ATCmd_netcfgGetUsageStr,                  0,                  1 },
/* netutil cmd cmd  */              { ATCmd_netUtilCmdStr,            ATCmdNetUtil_cmdCallback,           ATCmd_netUtilCmdUsageStr,                 1,                  0xFF },
/* netutil get cmd  */              { ATCmd_netUtilGetStr,            ATCmdNetUtil_getCallback,           ATCmd_netUtilGetUsageStr,                 0,                  2 },
/* mqtt client create       */      { ATCmd_mqttCreateStr,            ATCmdMqtt_createCallback,           ATCmd_mqttCreateUsageStr,                 0,                  13},
/* mqtt client delete       */      { ATCmd_mqttDeleteStr,            ATCmdMqtt_deleteCallback,           ATCmd_mqttDeleteUsageStr,                 0,                  1},
/* mqtt client connect      */      { ATCmd_mqttConnectStr,           ATCmdMqtt_connectCallback,          ATCmd_mqttConnectUsageStr,                0,                  1},
/* mqtt client disconnect   */      { ATCmd_mqttDisconnectStr,        ATCmdMqtt_disconnectCallback,       ATCmd_mqttDisconnectUsageStr,             0,                  1},
/* mqtt client publish      */      { ATCmd_mqttPublishStr,           ATCmdMqtt_publishCallback,          ATCmd_mqttPublishUsageStr,                0,                  6},
/* mqtt client subscribe    */      { ATCmd_mqttSubscribeStr,         ATCmdMqtt_subscribeCallback,        ATCmd_mqttSubscribeUsageStr,              0,                  0xFF},
/* mqtt client unsubscribe  */      { ATCmd_mqttUnsubscribeStr,       ATCmdMqtt_unsubscribeCallback,      ATCmd_mqttUnsubscribeUsageStr,            0,                  0xFF},
/* mqtt client set          */      { ATCmd_mqttSetStr,               ATCmdMqtt_setCallback,              ATCmd_mqttSetUsageStr,                    0,                  0xFF},
/* http client create       */      { ATCmd_httpCreateStr,            ATCmdHttp_createCallback,           ATCmd_httpCreateUsageStr,                 0,                  0},
/* http client destroy      */      { ATCmd_httpDestroyStr,           ATCmdHttp_destroyCallback,          ATCmd_httpDestroyUsageStr,                0,                  1},
/* http client connect      */      { ATCmd_httpConnectStr,           ATCmdHttp_connectCallback,          ATCmd_httpConnectUsageStr,                1,                  6},
/* http client disconnect   */      { ATCmd_httpDisconnectStr,        ATCmdHttp_disconnectCallback,       ATCmd_httpDisconnectUsageStr,             0,                  1},
/* http client send request */      { ATCmd_httpSendReqStr,           ATCmdHttp_sendReqCallback,          ATCmd_httpSendReqUsageStr,                0,                  7},
/* http client response body*/      { ATCmd_httpReadResBodyStr,       ATCmdHttp_respBodyCallback,         ATCmd_httpReadResBodyUsageStr,            0,                  3},
/* http client set header   */      { ATCmd_httpSetHeaderStr,         ATCmdHttp_setHeaderCallback,        ATCmd_httpSetHeaderUsageStr,              0,                  0xFF},
/* http client get header   */      { ATCmd_httpGetHeaderStr,         ATCmdHttp_getHeaderCallback,        ATCmd_httpGetHeaderUsageStr,              0,                  4},
/* http client set option   */      { ATCmd_httpSetOptStr,            ATCmdHttp_setOptCallback,           ATCmd_httpSetOptUsageStr,                 0,                  3},
/* http client set proxy    */      { ATCmd_httpSetProxyStr,          ATCmdHttp_setProxyCallback,         ATCmd_httpSetProxyUsageStr,               0,                  3}

};

const uint32_t ATCmd_maxCmd = (sizeof(ATCmd_list)/sizeof(ATCmd_List_t));

char ATCmd_wlanScanStr[]                    = "+wlanscan";
char ATCmd_wlanScanUsageStr[]               = "[index],[count]";
char ATCmd_wlanProfileAddStr[]              = "+wlanprofileadd";
char ATCmd_wlanProfileAddUsageStr[]         = "[SSID],[BSSID],[SecurityType],[SecurityKey],[SecurityExtUser],[SecurityExtAnonUser],[SecurityExtEapMethod],[Priority]";
char ATCmd_wlanProfileGetStr[]              = "+wlanprofileget";
char ATCmd_wlanProfileGetUsageStr[]         = "[Index]";
char ATCmd_wlanProfileDelStr[]              = "+wlanprofiledel";
char ATCmd_wlanProfileDelUsageStr[]         = "[Index]";
char ATCmd_wlanSetModeStr[]                 = "+wlansetmode";
char ATCmd_wlanSetModeUsageStr[]            = "[Role]";
char ATCmd_wlanSetStr[]                     = "+wlanset";
char ATCmd_wlanSetUsageStr[]                = "[ID],[Option],[Value1],..,[ValueX]";
char ATCmd_wlanGetStr[]                     = "+wlanget";
char ATCmd_wlanGetUsageStr[]                = "[ID],[Option]";
char ATCmd_wlanPolicySetStr[]               = "+wlanpolicyset";
char ATCmd_wlanPolicySetUsageStr[]          = "[Type],[Policy],[Value]";
char ATCmd_wlanPolicyGetStr[]               = "+wlanpolicyget";
char ATCmd_wlanPolicyGetUsageStr[]          = "[Type]";
char ATCmd_wlanConnectStr[]                 = "+wlanconnect";
char ATCmd_wlanConnectUsageStr[]            = "[SSID],[BSSID],[SecurityType],[SecurityKey],[SecurityExtUser],[SecurityExtAnonUser],[SecurityExtEapMethod]";
char ATCmd_wlanDisconnectStr[]              = "+wlandisconnect";
char ATCmd_wlanDisconnectUsageStr[]         = "[]";

char ATCmd_devStartStr[]                    = "+start";
char ATCmd_devStartUsageStr[]               = "[]";
char ATCmd_devStopStr[]                     = "+stop";
char ATCmd_devStopUsageStr[]                = "[timeout]";
char ATCmd_devTestStr[]                     = "+test";
char ATCmd_devTestUsageStr[]                = "[]";
char ATCmd_devGetStr[]                      = "+get";
char ATCmd_devGetUsageStr[]                 = "[ID],[Option]";
char ATCmd_devSetStr[]                      = "+set";
char ATCmd_devSetUsageStr[]                 = "[ID],[Option],[Value1],..,[ValueX]";

char ATCmd_sockSocketStr[]                  = "+socket";
char ATCmd_sockSocketUsageStr[]             = "[Family],[Type],[Protocol]";
char ATCmd_sockCloseStr[]                   = "+close";
char ATCmd_sockCloseUsageStr[]              = "[Socket]";
char ATCmd_sockAcceptStr[]                  = "+accept";
char ATCmd_sockAcceptUsageStr[]             = "[Socket],[Family]";
char ATCmd_sockBindStr[]                    = "+bind";
char ATCmd_sockBindUsageStr[]               = "[Socket],[Family],[Port],[IP Address]";
char ATCmd_sockListenStr[]                  = "+listen";
char ATCmd_sockListenUsageStr[]             = "[Socket],[Backlog]";
char ATCmd_sockConnectStr[]                 = "+connect";
char ATCmd_sockConnectUsageStr[]            = "[Socket],[Family],[Port],[IP Address]";
char ATCmd_sockSelectStr[]                  = "+select";
char ATCmd_sockSelectUsageStr[]             = "[nfds],[readsds],[timeout sec],[timeout usec]";
char ATCmd_sockSetSockOptStr[]              = "+setsockopt";
char ATCmd_sockSetSockOptUsageStr[]         = "[sd],[Level],[Option],[Value1],..,[ValueX]";
char ATCmd_sockGetSockOptStr[]              = "+getsockopt";
char ATCmd_sockGetSockOptUsageStr[]         = "[sd],[Level],[Option]";
char ATCmd_sockSendStr[]                    = "+send";
char ATCmd_sockSendUsageStr[]               = "[sd],[format],[length],[data]";
char ATCmd_sockRecvStr[]                    = "+recv";
char ATCmd_sockRecvUsageStr[]               = "[sd],[format],[length]";
char ATCmd_sockSendToStr[]                  = "+sendto";
char ATCmd_sockSendToUsageStr[]             = "[sd],[family],[port],[addr],[format],[length],[data]";
char ATCmd_sockRecvFromStr[]                = "+recvfrom";
char ATCmd_sockRecvFromUsageStr[]           = "[sd],[family],[port],[addr],[format],[length]";

char ATCmd_fileOpenStr[]                    = "+fileopen";
char ATCmd_fileOpenUsageStr[]               = "[Filename],[Option]";
char ATCmd_fileCloseStr[]                   = "+fileclose";
char ATCmd_fileCloseUsageStr[]              = "[FileID],[CeritificateFileName],[Signature]";
char ATCmd_fileCtlStr[]                     = "+filectl";
char ATCmd_fileCtlUsageStr[]                = "[Command],[Secure_Token],[Filename],[Data]";
char ATCmd_fileDelStr[]                     = "+filedel";
char ATCmd_fileDelUsageStr[]                = "[FileName],[SecureToken]";
char ATCmd_fileGetFileListStr[]             = "+filegetfilelist";
char ATCmd_fileGetFileListUsageStr[]        = "[Max Entries]";
char ATCmd_fileGetInfoStr[]                 = "+filegetinfo";
char ATCmd_fileGetInfoUsageStr[]            = "[FileName],[SecureToken]";
char ATCmd_fileReadStr[]                    = "+fileread";
char ATCmd_fileReadUsageStr[]               = "[FileID],[Offset],[Length]";
char ATCmd_fileWriteStr[]                   = "+filewrite";
char ATCmd_fileWriteUsageStr[]              = "[FileID],[Offset],[Data]";

char ATCmd_netappStartStr[]                 = "+netappstart";
char ATCmd_netappStartUsageStr[]            = "[APP Bitmap]";
char ATCmd_netappStopStr[]                  = "+netappstop";
char ATCmd_netappStopUsageStr[]             = "[APP Bitmap]";
char ATCmd_netappGetHostByNameStr[]         = "+netappgethostbyname";
char ATCmd_netappGetHostByNameUsageStr[]    = "[HostName],[Family]";
char ATCmd_netappGetHostByServiceStr[]      = "+netappgethostbyservice";
char ATCmd_netappGetHostByServiceUsageStr[] = "[ServiceName],[Family]";
char ATCmd_netappSendStr[]                  = "+netappsend";
char ATCmd_netappSendUsageStr[]             = "[Handle],[Flags],[Data]";
char ATCmd_netappRecvStr[]                  = "+netapprecv";
char ATCmd_netappRecvUsageStr[]             = "[Handle]";
char ATCmd_netappPingStr[]                  = "+netappping";
char ATCmd_netappPingUsageStr[]             = "[Family],[Destination],[Size],[Delay],[Timeout],[Max Attempts],[Flags]";
char ATCmd_netappGetServiceListStr[]        = "+netappgetservicelist";
char ATCmd_netappGetServiceListUsageStr[]   = "[IndexOffset],[MaxServiceCount],[Flags]";
char ATCmd_netappRegisterServiceStr[]       = "+netappregisterservice";
char ATCmd_netappRegisterServiceUsageStr[]  = "[ServiceName],[Text],[Port],[TTL],[Options]";
char ATCmd_netappUnregisterServiceStr[]     = "+netappunregisterservice";
char ATCmd_netappUnregisterServiceUsageStr[]= "[ServiceName],[Options]";
char ATCmd_netappSetStr[]                   = "+netappset";
char ATCmd_netappSetUsageStr[]              = "[App ID],[Option],[Value1],..,[ValueX]";
char ATCmd_netappGetStr[]                   = "+netappget";
char ATCmd_netappGetUsageStr[]              = "[App ID],[Option]";

char ATCmd_netcfgSetStr[]                   = "+netcfgset";
char ATCmd_netcfgSetUsageStr[]              = "[ID],[Option],[Value1],..,[ValueX]";
char ATCmd_netcfgGetStr[]                   = "+netcfgget";
char ATCmd_netcfgGetUsageStr[]              = "[ID]";

char ATCmd_netUtilCmdStr[]                  = "+netutilcmd";
char ATCmd_netUtilCmdUsageStr[]             = "[Option],[ID],[Value1],..,[ValueX]";
char ATCmd_netUtilGetStr[]                  = "+netutilget";
char ATCmd_netUtilGetUsageStr[]             = "[Option],[ID]";

char ATCmd_mqttCreateStr[]                  = "+mqttcreate";
char ATCmd_mqttCreateUsageStr[]             = "[client ID],[flags],[address],[port],[method],[cipher],[private key],[Certificate],[CA],[DH key],[protocol],[blocking send],[data format]";
char ATCmd_mqttDeleteStr[]                  = "+mqttdelete";
char ATCmd_mqttDeleteUsageStr[]             = "[index]";
char ATCmd_mqttConnectStr[]                 = "+mqttconnect";
char ATCmd_mqttConnectUsageStr[]            = "[index]";
char ATCmd_mqttDisconnectStr[]              = "+mqttdisconnect";
char ATCmd_mqttDisconnectUsageStr[]         = "[index]";
char ATCmd_mqttPublishStr[]                 = "+mqttpublish";
char ATCmd_mqttPublishUsageStr[]            = "[index],[topic],[QoS],[retain],[message length][message]";
char ATCmd_mqttSubscribeStr[]               = "+mqttsubscribe";
char ATCmd_mqttSubscribeUsageStr[]          = "[index],[number of topics],[topic1],[QoS1],[persistent1],..,[topicX],[QoSX],[persistentX]";
char ATCmd_mqttUnsubscribeStr[]             = "+mqttunsubscribe";
char ATCmd_mqttUnsubscribeUsageStr[]        = "[index],[number of topics],[topic1],[persistent1],..,[topicX],[persistentX]";
char ATCmd_mqttSetStr[]                     = "+mqttset";
char ATCmd_mqttSetUsageStr[]                = "[index],[option],[value1],..,[valueX]";

char ATCmd_httpCreateStr[]                  = "+httpcreate";
char ATCmd_httpCreateUsageStr[]             = "[]";
char ATCmd_httpDestroyStr[]                 = "+httpdestroy";
char ATCmd_httpDestroyUsageStr[]            = "[index]";
char ATCmd_httpConnectStr[]                 = "+httpconnect";
char ATCmd_httpConnectUsageStr[]            = "[index],[host],[flags],[private key],[certificate],[ca]";
char ATCmd_httpDisconnectStr[]              = "+httpdisconnect";
char ATCmd_httpDisconnectUsageStr[]         = "[index]";
char ATCmd_httpSendReqStr[]                 = "+httpsendreq";
char ATCmd_httpSendReqUsageStr[]            = "[index],[method],[uri],[flags],[format],[length],[data]";
char ATCmd_httpReadResBodyStr[]             = "+httpreadresbody";
char ATCmd_httpReadResBodyUsageStr[]        = "[index],[format],[length]";
char ATCmd_httpSetHeaderStr[]               = "+httpsetheader";
char ATCmd_httpSetHeaderUsageStr[]          = "[index],[option],[flags],[format],[length],[data]";
char ATCmd_httpGetHeaderStr[]               = "+httpgetheader";
char ATCmd_httpGetHeaderUsageStr[]          = "[index],[option],[format],[length]";
char ATCmd_httpSetOptStr[]                  = "+httpsetopt";
char ATCmd_httpSetOptUsageStr[]             = "[index],[option],[value]";
char ATCmd_httpSetProxyStr[]                = "+httpsetproxy";
char ATCmd_httpSetProxyUsageStr[]           = "[family],[port],[address]";

char ATCmd_okStr[]                          = "OK";
char ATCmd_errorNumParamsStr[]              = "ERROR: number of parameters,xxxxxx";
char ATCmd_errorAllocStr[]                  = "ERROR: allocate,xxxxxx";
char ATCmd_errorParseStr[]                  = "ERROR: parse parameters,xxxxxx";
char ATCmd_errorCmdStr[]                    = "ERROR: process command,xxxxxx";
char ATCmd_errorExistCmdStr[]               = "ERROR: command not exist,xxxxxx";

char ATCmd_eventGeneralStr[]                = "+eventgeneral";
char ATCmd_eventWlanStr[]                   = "+eventwlan";
char ATCmd_eventSockStr[]                   = "+eventsock";
char ATCmd_eventNetappStr[]                 = "+eventnetapp";
char ATCmd_eventHttpServerStr[]             = "+eventhttpserver";
char ATCmd_eventFatalErrorStr[]             = "+eventfatalerror";
char ATCmd_eventMqttStr[]                   = "+eventmqtt";
char ATCmd_eventHttpStr[]                   = "+eventhttp";

char ATCmd_excludeDelimStr[2]               = {ATCMD_DELIM_STR,ATCMD_DELIM_STR};
char ATCmd_excludeDelimArray[2]             = {ATCMD_DELIM_OPEN_ARRAY,ATCMD_DELIM_CLOSE_ARRAY};




StrMpl_List_t ATCmd_devCfgId[3] =
{
    {"general", SL_DEVICE_GENERAL   },
    {"status", SL_DEVICE_STATUS},
    {"iot", SL_DEVICE_IOT}
};

StrMpl_List_t ATCmd_devGeneralOptions[3] =
{
    {"version", SL_DEVICE_GENERAL_VERSION},
    {"persistent", SL_DEVICE_GENERAL_PERSISTENT},
    {"time", SL_DEVICE_GENERAL_DATE_TIME}    
};

StrMpl_List_t ATCmd_devStatusOptions[6] =
{
    {"device", SL_DEVICE_EVENT_CLASS_DEVICE},
    {"wlan", SL_DEVICE_EVENT_CLASS_WLAN},
    {"bsd", SL_DEVICE_EVENT_CLASS_BSD},
    {"netapp", SL_DEVICE_EVENT_CLASS_NETAPP},
    {"netcfg", SL_DEVICE_EVENT_CLASS_NETCFG},
    {"fs", SL_DEVICE_EVENT_CLASS_FS}
};

StrMpl_List_t ATCmd_devIotOptions[1] =
{
    {"udid", SL_DEVICE_IOT_UDID}
};

StrMpl_List_t ATCmd_devStatusWlan[9] =
{
    {"wlanasynconnectedresponse", SL_DEVICE_EVENT_DROPPED_WLAN_WLANASYNCONNECTEDRESPONSE},
    {"wlanasyncdisconnectedresponse", SL_DEVICE_EVENT_DROPPED_WLAN_WLANASYNCDISCONNECTEDRESPONSE},
    {"sta_connected", SL_DEVICE_EVENT_DROPPED_WLAN_STA_CONNECTED},
    {"sta_disconnected", SL_DEVICE_EVENT_DROPPED_WLAN_STA_DISCONNECTED},
    {"p2p_dev_found", SL_DEVICE_EVENT_DROPPED_WLAN_P2P_DEV_FOUND},
    {"connection_failed", SL_DEVICE_EVENT_DROPPED_WLAN_CONNECTION_FAILED},
    {"p2p_neg_req_received", SL_DEVICE_EVENT_DROPPED_WLAN_P2P_NEG_REQ_RECEIVED},
    {"rx_filters", SL_DEVICE_EVENT_DROPPED_WLAN_RX_FILTERS},
    {"wlan_sta_connected", SL_DEVICE_STATUS_WLAN_STA_CONNECTED}
};

StrMpl_List_t ATCmd_devStatusDevice[1] =
{
    {"general_error", SL_DEVICE_EVENT_DROPPED_DEVICE_ASYNC_GENERAL_ERROR}
};

StrMpl_List_t ATCmd_devStatusBsd[1] =
{
    {"tx_failed", SL_DEVICE_EVENT_DROPPED_DEVICE_ASYNC_GENERAL_ERROR}
};

StrMpl_List_t ATCmd_devStatusNetapp[8] =
{
    {"ipacquired", SL_DEVICE_EVENT_DROPPED_NETAPP_IPACQUIRED},
    {"ipacquired_v6", SL_DEVICE_EVENT_DROPPED_NETAPP_IPACQUIRED_V6},
    {"ip_leased", SL_DEVICE_EVENT_DROPPED_NETAPP_IP_LEASED},
    {"ip_released", SL_DEVICE_EVENT_DROPPED_NETAPP_IP_RELEASED},
    {"ipv4_lost", SL_DEVICE_EVENT_DROPPED_NETAPP_IPV4_LOST},
    {"dhcp_acquire_timeout", SL_DEVICE_EVENT_DROPPED_NETAPP_DHCP_ACQUIRE_TIMEOUT},
    {"ip_collision", SL_DEVICE_EVENT_DROPPED_NETAPP_IP_COLLISION},
    {"ipv6_lost", SL_DEVICE_EVENT_DROPPED_NETAPP_IPV6_LOST}
};

StrMpl_List_t ATCmd_wlanSecurity[7] =
{
    {"open",SL_WLAN_SEC_TYPE_OPEN    },
    {"wep", SL_WLAN_SEC_TYPE_WEP   },
    {"wpa_wpa2", SL_WLAN_SEC_TYPE_WPA_WPA2   },
    {"wps_pbc", SL_WLAN_SEC_TYPE_WPS_PBC   },
    {"wps_pin", SL_WLAN_SEC_TYPE_WPS_PIN   },
    {"wpa_ent", SL_WLAN_SEC_TYPE_WPA_ENT   },
    {"wep_shared", SL_WLAN_SEC_TYPE_WEP_SHARED   }
};

StrMpl_List_t ATCmd_wlanScanSecurity[5] =
{
    {"open", SL_WLAN_SECURITY_TYPE_BITMAP_OPEN   },
    {"wep", SL_WLAN_SECURITY_TYPE_BITMAP_WEP   },
    {"wpa", SL_WLAN_SECURITY_TYPE_BITMAP_WPA   },
    {"wpa2",  SL_WLAN_SECURITY_TYPE_BITMAP_WPA2  },
    {"wpa_wpa2", SL_WLAN_SECURITY_TYPE_BITMAP_WPA | SL_WLAN_SECURITY_TYPE_BITMAP_WPA2 }
};

StrMpl_List_t ATCmd_wlanScanCipher[5] =
{
    {"none", 0   },
    {"wep40", SL_WLAN_CIPHER_BITMAP_WEP40   },
    {"wep104", SL_WLAN_CIPHER_BITMAP_WEP104   },
    {"tkip",  SL_WLAN_CIPHER_BITMAP_TKIP  },
    {"ccmp", SL_WLAN_CIPHER_BITMAP_CCMP }
};

StrMpl_List_t ATCmd_wlanScanKeyMgmt[3] =
{
    {"none", 0   },
    {"802_1_x", SL_WLAN_KEY_MGMT_SUITE_802_1_X   },
    {"psk", SL_WLAN_KEY_MGMT_SUITE_PSK}
};

StrMpl_List_t ATCmd_wlanEap[9] =
{
    {"tls", SL_WLAN_ENT_EAP_METHOD_TLS   },
    {"ttls_tls", SL_WLAN_ENT_EAP_METHOD_TTLS_TLS   },
    {"ttls_mschapv2", SL_WLAN_ENT_EAP_METHOD_TTLS_MSCHAPv2   },
    {"ttls_psk", SL_WLAN_ENT_EAP_METHOD_TTLS_PSK   },
    {"peap0_tls", SL_WLAN_ENT_EAP_METHOD_PEAP0_TLS   },
    {"peap0_mschapv2", SL_WLAN_ENT_EAP_METHOD_PEAP0_MSCHAPv2   },
    {"peap0_psk", SL_WLAN_ENT_EAP_METHOD_PEAP0_PSK   },
    {"peap1_tls", SL_WLAN_ENT_EAP_METHOD_PEAP1_TLS   },
    {"peap1_psk", SL_WLAN_ENT_EAP_METHOD_PEAP1_PSK   }
};

StrMpl_List_t ATCmd_wlanPolicy[4] =
{
    {"connection", SL_WLAN_POLICY_CONNECTION   },
    {"scan", SL_WLAN_POLICY_SCAN   },
    {"pm", SL_WLAN_POLICY_PM   },
    {"p2p", SL_WLAN_POLICY_P2P   }
};

StrMpl_List_t ATCmd_wlanConnPolicy[4] =
{
    {"auto", SL_WLAN_CONNECTION_POLICY(1,0,0,0) },
    {"fast", SL_WLAN_CONNECTION_POLICY(0,1,0,0)   },
    {"p2p",  SL_WLAN_CONNECTION_POLICY(0,0,1,0)  },
    {"auto_provisioning",  SL_WLAN_CONNECTION_POLICY(0,0,0,1)  }
};

StrMpl_List_t ATCmd_wlanScanPolicy[3] =
{
    {"hidden_ssid", SL_WLAN_SCAN_POLICY(1,1)   },
    {"no_hidden_ssid", SL_WLAN_SCAN_POLICY(1,0)   },
    {"disable_scan", SL_WLAN_DISABLE_SCAN   }
};

StrMpl_List_t ATCmd_wlanPmPolicy[5] =
{
    {"normal", SL_WLAN_NORMAL_POLICY   },
    {"low_latency", SL_WLAN_LOW_LATENCY_POLICY   },
    {"low_power", SL_WLAN_LOW_POWER_POLICY   },
    {"always_on", SL_WLAN_ALWAYS_ON_POLICY   },
    {"long_sleep", SL_WLAN_LONG_SLEEP_INTERVAL_POLICY   }
};

StrMpl_List_t ATCmd_wlanP2pRolePolicy[3] =
{
    {"client",  SL_WLAN_P2P_ROLE_CLIENT  },
    {"group_owner", SL_WLAN_P2P_ROLE_GROUP_OWNER   },
    {"negotiate", SL_WLAN_P2P_ROLE_NEGOTIATE   },
};

StrMpl_List_t ATCmd_wlanP2pNegPolicy[3] =
{
    {"active", SL_WLAN_P2P_NEG_INITIATOR_ACTIVE   },
    {"passive", SL_WLAN_P2P_NEG_INITIATOR_PASSIVE   },
    {"rand_backoff", SL_WLAN_P2P_NEG_INITIATOR_RAND_BACKOFF   }
};

StrMpl_List_t ATCmd_wlanRole[3] =
{
    {"sta", ROLE_STA   },
    {"ap", ROLE_AP   },
    {"p2p", ROLE_P2P   }
};

StrMpl_List_t ATCmd_wlanCfg[6] =
{
    {"ap", SL_WLAN_CFG_AP_ID   },
    {"general", SL_WLAN_CFG_GENERAL_PARAM_ID   },
    {"p2p", SL_WLAN_CFG_P2P_PARAM_ID   },
    {"access_list",SL_WLAN_CFG_AP_ACCESS_LIST_ID},
    {"rx_filter", SL_WLAN_RX_FILTERS_ID   },
    {"connection",SL_WLAN_CONNECTION_INFO}
};

StrMpl_List_t ATCmd_wlanApCfg[12] =
{
    {"ssid", SL_WLAN_AP_OPT_SSID   },
    {"channel", SL_WLAN_AP_OPT_CHANNEL   },
    {"hidden_ssid", SL_WLAN_AP_OPT_HIDDEN_SSID   },
    {"security", SL_WLAN_AP_OPT_SECURITY_TYPE   },
    {"password", SL_WLAN_AP_OPT_PASSWORD   },
    {"max_stations", SL_WLAN_AP_OPT_MAX_STATIONS   },
    {"max_sta_aging", SL_WLAN_AP_OPT_MAX_STA_AGING   },
    {"access_list_mode", SL_WLAN_AP_ACCESS_LIST_MODE   },
    {"access_list_add_mac", SL_WLAN_AP_ACCESS_LIST_ADD_MAC   },
    {"access_list_del_mac", SL_WLAN_AP_ACCESS_LIST_DEL_MAC   },
    {"access_list_del_idx", SL_WLAN_AP_ACCESS_LIST_DEL_IDX   },
    {"access_list_num_entries", SL_WLAN_AP_ACCESS_LIST_NUM_ENTRIES}
};

StrMpl_List_t ATCmd_wlanGenCfg[7] =
{
    {"country_code", SL_WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE    },
    {"sta_tx_power", SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER    },
    {"ap_tx_power",  SL_WLAN_GENERAL_PARAM_OPT_AP_TX_POWER   },
    {"info_element",  SL_WLAN_GENERAL_PARAM_OPT_INFO_ELEMENT   },
    {"scan_params",   SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS },
    {"suspend_profiles",  SL_WLAN_GENERAL_PARAM_OPT_SUSPEND_PROFILES   },
    {"disable_ent_server_auth",SL_WLAN_GENERAL_PARAM_DISABLE_ENT_SERVER_AUTH     }
};

StrMpl_List_t ATCmd_wlanP2pCfg[2] =
{
    {"dev_type",  SL_WLAN_P2P_OPT_DEV_TYPE  },
    {"channel_n_regs", SL_WLAN_P2P_OPT_CHANNEL_N_REGS   }
};

StrMpl_List_t ATCmd_wlanRxFilterCfg[4] =
{
    {"state", SL_WLAN_RX_FILTER_STATE   },
    {"sys_state", SL_WLAN_RX_FILTER_SYS_STATE   },
    {"remove", SL_WLAN_RX_FILTER_REMOVE   },
    {"store",  SL_WLAN_RX_FILTER_STORE  }
};

StrMpl_List_t ATCmd_wlanApAccessList[2] =
{
    {"disable", SL_WLAN_AP_ACCESS_LIST_MODE_DISABLED   },
    {"deny_list", SL_WLAN_AP_ACCESS_LIST_MODE_DENY_LIST   }
};

StrMpl_List_t ATCmd_wlanInfoElementRole[2] =
{
    {"ap", SL_WLAN_INFO_ELEMENT_AP_ROLE },
    {"p2p", SL_WLAN_INFO_ELEMENT_P2P_GO_ROLE   }
};

StrMpl_List_t ATCmd_wlanConnStatus[5] =
{	
    {"disconnected",SL_WLAN_DISCONNECTED  },
    {"station_connected",SL_WLAN_CONNECTED_STA },
    {"p2pcl_connected",SL_WLAN_CONNECTED_P2PCL },
    {"p2pgo_connected",SL_WLAN_CONNECTED_P2PGO },
    {"ap_connected_stations",SL_WLAN_AP_CONNECTED_STATIONS }
};

StrMpl_List_t ATCmd_sockDomain[3] =
{
    {"inet", SL_AF_INET   },
    {"inet6", SL_AF_INET6   },
    {"rf", SL_AF_RF   }
};

StrMpl_List_t ATCmd_sockType[3] =
{
    {"stream", SL_SOCK_STREAM   },
    {"dgram", SL_SOCK_DGRAM    },
    {"raw", SL_SOCK_RAW    }
};

StrMpl_List_t ATCmd_sockProtocol[4] =
{
    {"tcp", SL_IPPROTO_TCP   },
    {"udp", SL_IPPROTO_UDP   },
    {"raw", SL_IPPROTO_RAW   },
    {"sec", SL_SEC_SOCKET    }
        
};

StrMpl_List_t ATCmd_sockLevel[3] =
{
    {"socket", SL_SOL_SOCKET   },
    {"ip", SL_IPPROTO_IP   },
    {"phy", SL_SOL_PHY_OPT  }
};

StrMpl_List_t ATCmd_sockSocketOpt[17] =
{
    {"keepalive",SL_SO_KEEPALIVE             },
    {"keepalivetime",SL_SO_KEEPALIVETIME         },
    {"rx_no_ip_boundary",SL_SO_RX_NO_IP_BOUNDARY     },
    {"rcvtimeo",SL_SO_RCVTIMEO              },
    {"rcvbuf",SL_SO_RCVBUF                },
    {"nonblocking",SL_SO_NONBLOCKING           },
    {"secmethod",SL_SO_SECMETHOD             },
    {"secure_mask",SL_SO_SECURE_MASK           },
    {"secure_files_ca_file_name",SL_SO_SECURE_FILES_CA_FILE_NAME             },
    {"secure_files_private_key_file_name",SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME    }, 
    {"secure_files_certificate_file_name",SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME    },
    {"secure_files_dh_key_file_name",SL_SO_SECURE_FILES_PEER_CERT_OR_DH_KEY_FILE_NAME         },
    {"change_channel",SL_SO_CHANGE_CHANNEL                        }, 
    {"secure_alpn",SL_SO_SECURE_ALPN                           }, 
    {"linger",SL_SO_LINGER          },
    {"secure_ext_client_chlng_resp",SL_SO_SECURE_EXT_CLIENT_CHLNG_RESP    },
    {"secure_domain_name_verification",SL_SO_SECURE_DOMAIN_NAME_VERIFICATION           }
};

StrMpl_List_t ATCmd_sockIpOpt[6] =
{
    {"multicast_ttl",SL_IP_MULTICAST_TTL              },
    {"add_membership",SL_IP_ADD_MEMBERSHIP         },
    {"drop_membership",SL_IP_DROP_MEMBERSHIP    },
    {"raw_rx_no_header",SL_IP_RAW_RX_NO_HEADER          },
    {"hdrincl",SL_IP_HDRINCL                        }, 
    {"raw_ipv6_hdrincl",SL_IP_RAW_IPV6_HDRINCL} 
};

StrMpl_List_t ATCmd_sockPhyOpt[7] =
{
    {"phy_rate",SL_SO_PHY_RATE               },
    {"phy_tx_power",SL_SO_PHY_TX_POWER          },
    {"phy_num_frames_to_tx",SL_SO_PHY_NUM_FRAMES_TO_TX     },
    {"phy_preamble",SL_SO_PHY_PREAMBLE           },
    {"phy_tx_inhibit_threshold",SL_SO_PHY_TX_INHIBIT_THRESHOLD                         }, 
    {"phy_tx_timeout",SL_SO_PHY_TX_TIMEOUT                            },
    {"phy_allow_acks",SL_SO_PHY_ALLOW_ACKS               }
};

StrMpl_List_t ATCmd_sockSocketSecMethod[5] =
{
    {"sslv3",SL_SO_SEC_METHOD_SSLV3},
    {"tlsv1",SL_SO_SEC_METHOD_TLSV1},
    {"tlsv1_1",SL_SO_SEC_METHOD_TLSV1_1},
    {"tlsv1_2",SL_SO_SEC_METHOD_TLSV1_2},
    {"sslv3_tlsv1_2",SL_SO_SEC_METHOD_SSLv3_TLSV1_2} 
};

StrMpl_List_t ATCmd_sockSocketCipher[23] =
{
    {"ssl_rsa_with_rc4_128_sha",                       SL_SEC_MASK_SSL_RSA_WITH_RC4_128_SHA                                  },
    {"ssl_rsa_with_rc4_128_md5",                       SL_SEC_MASK_SSL_RSA_WITH_RC4_128_MD5                                  },
    {"tls_rsa_with_aes_256_cbc_sha",                   SL_SEC_MASK_TLS_RSA_WITH_AES_256_CBC_SHA                              },
    {"tls_dhe_rsa_with_aes_256_cbc_sha",               SL_SEC_MASK_TLS_DHE_RSA_WITH_AES_256_CBC_SHA                          },
    {"tls_ecdhe_rsa_with_aes_256_cbc_sha",             SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA                         },
    {"tls_ecdhe_rsa_with_rc4_128_sha",                 SL_SEC_MASK_TLS_ECDHE_RSA_WITH_RC4_128_SHA                            },
    {"tls_rsa_with_aes_128_cbc_sha256",                SL_SEC_MASK_TLS_RSA_WITH_AES_128_CBC_SHA256                           }, 
    {"tls_rsa_with_aes_256_cbc_sha256",                SL_SEC_MASK_TLS_RSA_WITH_AES_256_CBC_SHA256                          },
    {"tls_ecdhe_rsa_with_aes_128_cbc_sha256",          SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256                     },
    {"tls_ecdhe_ecdsa_with_aes_128_cbc_sha256",        SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256               },
    {"tls_ecdhe_ecdsa_with_aes_128_cbc_sha",           SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA                  },
    {"tls_ecdhe_ecdsa_with_aes_256_cbc_sha",           SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA                  },
    {"tls_rsa_with_aes_128_gcm_sha256",                SL_SEC_MASK_TLS_RSA_WITH_AES_128_GCM_SHA256                       },
    {"tls_rsa_with_aes_256_gcm_sha384",                SL_SEC_MASK_TLS_RSA_WITH_AES_256_GCM_SHA384                       },
    {"tls_dhe_rsa_with_aes_128_gcm_sha256",            SL_SEC_MASK_TLS_DHE_RSA_WITH_AES_128_GCM_SHA256                   },
    {"tls_dhe_rsa_with_aes_256_gcm_sha384",            SL_SEC_MASK_TLS_DHE_RSA_WITH_AES_256_GCM_SHA384                   },
    {"tls_ecdhe_rsa_with_aes_128_gcm_sha256",          SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256                 }, 
    {"tls_ecdhe_rsa_with_aes_256_gcm_sha384",          SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384                 },
    {"tls_ecdhe_ecdsa_with_aes_128_gcm_sha256",        SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256               },
    {"tls_ecdhe_ecdsa_with_aes_256_gcm_sha384",        SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384               },
    {"tls_ecdhe_ecdsa_with_chacha20_poly1305_sha256",  SL_SEC_MASK_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256         },
    {"tls_ecdhe_rsa_with_chacha20_poly1305_sha256",    SL_SEC_MASK_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256           },
    {"tls_dhe_rsa_with_chacha20_poly1305_sha256",      SL_SEC_MASK_TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256             }
};

StrMpl_List_t ATCmd_sockSocketAlpn[6] =
{
    {"h1",SL_SECURE_ALPN_H1},
    {"h2",SL_SECURE_ALPN_H2},
    {"h2c",SL_SECURE_ALPN_H2C},
    {"h2_14",SL_SECURE_ALPN_H2_14},
    {"h2_16",SL_SECURE_ALPN_H2_16}, 
    {"full_list",SL_SECURE_ALPN_FULL_LIST} 
};

StrMpl_List_t ATCmd_sockPhyThreshold[6] =
{
    {"min",SL_TX_INHIBIT_THRESHOLD_MIN},
    {"low",SL_TX_INHIBIT_THRESHOLD_LOW},
    {"default",SL_TX_INHIBIT_THRESHOLD_DEFAULT},
    {"med",SL_TX_INHIBIT_THRESHOLD_MED},
    {"high",SL_TX_INHIBIT_THRESHOLD_HIGH}, 
    {"max",SL_TX_INHIBIT_THRESHOLD_MAX} 
};

StrMpl_List_t ATCmd_fileOpenOption[11] =
{
    {"read",SL_FS_READ},
    {"write",SL_FS_WRITE},
    {"create",SL_FS_CREATE},
    {"overwrite",SL_FS_OVERWRITE},
    {"create_failsafe",SL_FS_CREATE_FAILSAFE}, 
    {"create_secure",SL_FS_CREATE_SECURE}, 
    {"create_nosignature",SL_FS_CREATE_NOSIGNATURE},
    {"create_static_token",SL_FS_CREATE_STATIC_TOKEN},
    {"create_vendor_token",SL_FS_CREATE_VENDOR_TOKEN},
    {"create_public_write",SL_FS_CREATE_PUBLIC_WRITE},
    {"create_public_read",SL_FS_CREATE_PUBLIC_READ} 
};

StrMpl_List_t ATCmd_fileCtlCmd[7] =
{
    {"restore",SL_FS_CTL_RESTORE},
    {"rollback",SL_FS_CTL_ROLLBACK},
    {"commit",SL_FS_CTL_COMMIT},
    {"rename",SL_FS_CTL_RENAME},
    {"get_storage_info",SL_FS_CTL_GET_STORAGE_INFO}, 
    {"bundle_rollback",SL_FS_CTL_BUNDLE_ROLLBACK}, 
    {"bundle_commit",SL_FS_CTL_BUNDLE_COMMIT} 
};

StrMpl_List_t ATCmd_fileCtlCmdRestore[2] =
{
    {"factory_image",SL_FS_FACTORY_RET_TO_IMAGE},
    {"factory_default",SL_FS_FACTORY_RET_TO_DEFAULT}
};

StrMpl_List_t ATCmd_fileGetInfo[13] =
{
    {"open_write",SL_FS_INFO_OPEN_WRITE},
    {"open_read",SL_FS_INFO_OPEN_READ},
    {"must_commit",SL_FS_INFO_MUST_COMMIT},
    {"bundle_file",SL_FS_INFO_BUNDLE_FILE},
    {"pending_commit",SL_FS_INFO_PENDING_COMMIT},
    {"pending_bundle_commit",SL_FS_INFO_PENDING_BUNDLE_COMMIT},
    {"not_failsafe",SL_FS_INFO_NOT_FAILSAFE},
    {"not_valid",SL_FS_INFO_NOT_VALID},
    {"sys_file",SL_FS_INFO_SYS_FILE},
    {"secure",SL_FS_INFO_SECURE},
    {"nosignature",SL_FS_INFO_NOSIGNATURE},
    {"public_write",SL_FS_INFO_PUBLIC_WRITE},
    {"public_read",SL_FS_INFO_PUBLIC_READ}
};

StrMpl_List_t ATCmd_fileGetInfoToken[4] =
{
    {"master",SL_FS_TOKEN_MASTER},
    {"write_read",SL_FS_TOKEN_WRITE_READ},
    {"write_only",SL_FS_TOKEN_WRITE_ONLY},
    {"read_only",SL_FS_TOKEN_READ_ONLY}
};

StrMpl_List_t ATCmd_netappType[6] =
{
    {"http_server",SL_NETAPP_HTTP_SERVER_ID},
    {"dhcp_server",SL_NETAPP_DHCP_SERVER_ID},
    {"mdns",SL_NETAPP_MDNS_ID},
    {"dns_server",SL_NETAPP_DNS_SERVER_ID},
    {"device",SL_NETAPP_DEVICE_ID},
    {"dns_client",SL_NETAPP_DNS_CLIENT_ID}
};

StrMpl_List_t ATCmd_netappFlags[3] =
{
    {"continuation",SL_NETAPP_REQUEST_RESPONSE_FLAGS_CONTINUATION},
    {"metadata",SL_NETAPP_REQUEST_RESPONSE_FLAGS_METADATA},
    {"accumulation",SL_NETAPP_REQUEST_RESPONSE_FLAGS_ACCUMULATION}
};

StrMpl_List_t ATCmd_netappServiceListFlags[6] =
{
    {"full_ipv4_with_text",SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV4_TYPE},
    {"full_ipv4",SL_NETAPP_FULL_SERVICE_IPV4_TYPE},
    {"short_ipv4",SL_NETAPP_SHORT_SERVICE_IPV4_TYPE},
    {"full_ipv6_with_text",SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV6_TYPE},
    {"full_ipv6",SL_NETAPP_FULL_SERVICE_IPV6_TYPE},
    {"short_ipv6",SL_NETAPP_SHORT_SERVICE_IPV6_TYPE}
};

StrMpl_List_t ATCmd_netappRegServiceOptions[5] =
{
    {"is_unique_bit",SL_NETAPP_MDNS_OPTIONS_IS_UNIQUE_BIT},
    {"ipv6_ipv4_service",SL_NETAPP_MDNS_IPV6_IPV4_SERVICE},
    {"ipv6_only_service",SL_NETAPP_MDNS_IPV6_ONLY_SERVICE},
    {"update_text",SL_NETAPP_MDNS_OPTION_UPDATE_TEXT},
    {"is_not_persistent",SL_NETAPP_MDNS_OPTIONS_IS_NOT_PERSISTENT}
};

StrMpl_List_t ATCmd_netappHttpSrvOptions[14] =
{
    {"prim_port_num",SL_NETAPP_HTTP_PRIMARY_PORT_NUMBER},
    {"auth_check",SL_NETAPP_HTTP_AUTH_CHECK},
    {"auth_name",SL_NETAPP_HTTP_AUTH_NAME},
    {"auth_password",SL_NETAPP_HTTP_AUTH_PASSWORD},
    {"auth_realm",SL_NETAPP_HTTP_AUTH_REALM},
    {"rom_pages_access",SL_NETAPP_HTTP_ROM_PAGES_ACCESS},
    {"second_port_num",SL_NETAPP_HTTP_SECONDARY_PORT_NUMBER},
    {"second_port_en",SL_NETAPP_HTTP_SECONDARY_PORT_ENABLE},
    {"prim_port_sec_en",SL_NETAPP_HTTP_PRIMARY_PORT_SECURITY_MODE},
    {"priv_key_file",SL_NETAPP_HTTP_PRIVATE_KEY_FILENAME},
    {"dev_cert_file",SL_NETAPP_HTTP_DEVICE_CERTIFICATE_FILENAME},
    {"ca_cert_file",SL_NETAPP_HTTP_CA_CERTIFICATE_FILE_NAME},
    {"tmp_register_service",SL_NETAPP_HTTP_TEMP_REGISTER_MDNS_SERVICE_NAME},
    {"tmp_unregister_service",SL_NETAPP_HTTP_TEMP_UNREGISTER_MDNS_SERVICE_NAME}
};

StrMpl_List_t ATCmd_netappMdnsOptions[3] =
{
    {"cont_query",SL_NETAPP_MDNS_CONT_QUERY_OPT},
    {"qevetn_mask",SL_NETAPP_MDNS_QEVETN_MASK_OPT},
    {"timing_params",SL_NETAPP_MDNS_TIMING_PARAMS_OPT}
};

StrMpl_List_t ATCmd_netappDevOptions[2] =
{
    {"urn",SL_NETAPP_DEVICE_URN},
    {"domain",SL_NETAPP_DEVICE_DOMAIN}
};

StrMpl_List_t ATCmd_netappDnsClientOptions[1] =
{
    {"time",SL_NETAPP_DNS_CLIENT_TIME}
};

StrMpl_List_t ATCmd_netappDhcpSrvOptions[1] =
{
    {"basic",SL_NETAPP_DHCP_SRV_BASIC_OPT}
};

StrMpl_List_t ATCmd_netappMdnsTypes[20] =
{
    {"ipp",SL_NETAPP_MASK_IPP_TYPE_OF_SERVICE},
    {"deviceinfo",SL_NETAPP_MASK_DEVICE_INFO_TYPE_OF_SERVICE},
    {"http",SL_NETAPP_MASK_HTTP_TYPE_OF_SERVICE},
    {"https",SL_NETAPP_MASK_HTTPS_TYPE_OF_SERVICE},
    {"workstation",SL_NETAPP_MASK_WORKSATION_TYPE_OF_SERVICE},
    {"guid",SL_NETAPP_MASK_GUID_TYPE_OF_SERVICE},
    {"h323",SL_NETAPP_MASK_H323_TYPE_OF_SERVICE},
    {"ntp",SL_NETAPP_MASK_NTP_TYPE_OF_SERVICE},
    {"objective",SL_NETAPP_MASK_OBJECITVE_TYPE_OF_SERVICE},
    {"rdp",SL_NETAPP_MASK_RDP_TYPE_OF_SERVICE},
    {"remote",SL_NETAPP_MASK_REMOTE_TYPE_OF_SERVICE},
    {"rtsp",SL_NETAPP_MASK_RTSP_TYPE_OF_SERVICE},
    {"sip",SL_NETAPP_MASK_SIP_TYPE_OF_SERVICE},
    {"smb",SL_NETAPP_MASK_SMB_TYPE_OF_SERVICE},
    {"soap",SL_NETAPP_MASK_SOAP_TYPE_OF_SERVICE},
    {"ssh",SL_NETAPP_MASK_SSH_TYPE_OF_SERVICE},
    {"telnet",SL_NETAPP_MASK_TELNET_TYPE_OF_SERVICE},
    {"tftp",SL_NETAPP_MASK_TFTP_TYPE_OF_SERVICE},
    {"xmpp",SL_NETAPP_MASK_XMPP_CLIENT_TYPE_OF_SERVICE},
    {"raop",SL_NETAPP_MASK_RAOP_TYPE_OF_SERVICE}
};

StrMpl_List_t ATCmd_netcfgId[12] =
{
    {"set_mac_addr",SL_NETCFG_MAC_ADDRESS_SET},
    {"get_mac_addr",SL_NETCFG_MAC_ADDRESS_GET},
    {"ap_stations_connected",SL_NETCFG_AP_STATIONS_NUM_CONNECTED},
    {"ap_stations_info",SL_NETCFG_AP_STATIONS_INFO_LIST},
    {"ap_station_disconnect",SL_NETCFG_AP_STATION_DISCONNECT},
    {"if",SL_NETCFG_IF},
    {"ipv4_sta_addr",SL_NETCFG_IPV4_STA_ADDR_MODE},
    {"ipv4_ap_addr",SL_NETCFG_IPV4_AP_ADDR_MODE},
    {"ipv6_addr_local",SL_NETCFG_IPV6_ADDR_LOCAL},
    {"ipv6_addr_global",SL_NETCFG_IPV6_ADDR_GLOBAL},
    {"ipv4_dhcp_client",SL_NETCFG_IPV4_DHCP_CLIENT},
    {"ipv4_dns_client",SL_NETCFG_IPV4_DNS_CLIENT}
};

StrMpl_List_t ATCmd_netcfgOption[8] =
{
    {"state",SL_NETCFG_IF_STATE},
    {"dhcp",SL_NETCFG_ADDR_DHCP},
    {"dhcp_lla",SL_NETCFG_ADDR_DHCP_LLA},
    {"static",SL_NETCFG_ADDR_STATIC},
    {"stateless",SL_NETCFG_ADDR_STATELESS},
    {"stateful",SL_NETCFG_ADDR_STATEFUL},
    {"release_ip_set",SL_NETCFG_ADDR_RELEASE_IP_SET},
    {"release_ip_off",SL_NETCFG_ADDR_RELEASE_IP_OFF}
};

StrMpl_List_t ATCmd_netcfgIfState[12] =
{
    {"ipv6_sta_local",SL_NETCFG_IF_IPV6_STA_LOCAL},
    {"ipv6_sta_global",SL_NETCFG_IF_IPV6_STA_GLOBAL},
    {"disable_ipv4_dhcp",SL_NETCFG_IF_DISABLE_IPV4_DHCP},
    {"ipv6_local_static",SL_NETCFG_IF_IPV6_LOCAL_STATIC},
    {"ipv6_local_stateless",SL_NETCFG_IF_IPV6_LOCAL_STATELESS},
    {"ipv6_local_stateful",SL_NETCFG_IF_IPV6_LOCAL_STATEFUL},
    {"ipv6_global_static",SL_NETCFG_IF_IPV6_GLOBAL_STATIC},
    {"ipv6_global_stateful",SL_NETCFG_IF_IPV6_GLOBAL_STATEFUL},
    {"disable_ipv4_lla",SL_NETCFG_IF_DISABLE_IPV4_LLA},
    {"enable_dhcp_release",SL_NETCFG_IF_ENABLE_DHCP_RELEASE},
    {"ipv6_global_stateless",SL_NETCFG_IF_IPV6_GLOBAL_STATELESS},
    {"disable_fast_renew",SL_NETCFG_IF_DISABLE_FAST_RENEW}
};

StrMpl_List_t ATCmd_netcfgDhcpState[6] =
{
    {"unknown",SL_NETCFG_DHCP_CLIENT_UNKNOWN},
    {"disabled",SL_NETCFG_DHCP_CLIENT_DISABLED},
    {"enabled",SL_NETCFG_DHCP_CLIENT_ENABLED},
    {"bound",SL_NETCFG_DHCP_CLIENT_BOUND},
    {"renew",SL_NETCFG_DHCP_CLIENT_RENEW},
    {"rebind",SL_NETCFG_DHCP_CLIENT_REBIND}
};

StrMpl_List_t ATCmd_eventDeviceId[7] =
{
    {"device_abort",SL_DEVICE_EVENT_FATAL_DEVICE_ABORT},
    {"driver_abort",SL_DEVICE_EVENT_FATAL_DRIVER_ABORT},
    {"sync_loss",SL_DEVICE_EVENT_FATAL_SYNC_LOSS},
    {"no_cmd_ack",SL_DEVICE_EVENT_FATAL_NO_CMD_ACK},
    {"cmd_timeout",SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT},
    {"reset_request",SL_DEVICE_EVENT_RESET_REQUEST},
    {"error",SL_DEVICE_EVENT_ERROR}
};

StrMpl_List_t ATCmd_eventDeviceSender[5] =
{
    {"other",SL_DEVICE_EVENT_FATAL_DEVICE_ABORT},
    {"wlan",SL_DEVICE_EVENT_FATAL_DRIVER_ABORT},
    {"netcfg",SL_DEVICE_EVENT_FATAL_SYNC_LOSS},
    {"netapp",SL_DEVICE_EVENT_FATAL_NO_CMD_ACK},
    {"security",SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT}
};

StrMpl_List_t ATCmd_eventWlanId[14] =
{
    {"connect",SL_WLAN_EVENT_CONNECT},
    {"disconnect",SL_WLAN_EVENT_DISCONNECT},
    {"sta_added",SL_WLAN_EVENT_STA_ADDED},
    {"sta_removed",SL_WLAN_EVENT_STA_REMOVED},
    {"p2p_connect",SL_WLAN_EVENT_P2P_CONNECT},
    {"p2p_disconnect",SL_WLAN_EVENT_P2P_DISCONNECT},
    {"p2p_client_added",SL_WLAN_EVENT_P2P_CLIENT_ADDED},
    {"p2p_client_removed",SL_WLAN_EVENT_P2P_CLIENT_REMOVED},
    {"p2p_devfound",SL_WLAN_EVENT_P2P_DEVFOUND},
    {"p2p_request",SL_WLAN_EVENT_P2P_REQUEST},
    {"p2p_connectfail",SL_WLAN_EVENT_P2P_CONNECTFAIL},
    {"rxfilter",SL_WLAN_EVENT_RXFILTER},
    {"provisioning_status",SL_WLAN_EVENT_PROVISIONING_STATUS},
    {"provisioning_profile_added",SL_WLAN_EVENT_PROVISIONING_PROFILE_ADDED}
};

StrMpl_List_t ATCmd_eventWlanWpsMethod[6] =
{
    {"default",SL_WLAN_P2P_WPS_METHOD_DEFAULT},
    {"pin_user",SL_WLAN_P2P_WPS_METHOD_PIN_USER},
    {"pin_machine",SL_WLAN_P2P_WPS_METHOD_PIN_MACHINE},
    {"rekey",SL_WLAN_P2P_WPS_METHOD_REKEY},
    {"pbc",SL_WLAN_P2P_WPS_METHOD_PBC},
    {"registrar",SL_WLAN_P2P_WPS_METHOD_REGISTRAR}
};

StrMpl_List_t ATCmd_eventWlanStatus[4] =
{
    {"disconnected",SL_WLAN_STATUS_DISCONNECTED},
    {"scaning",SL_WLAN_STATUS_SCANING},
    {"connecting",SL_WLAN_STATUS_CONNECTING},
    {"connected",SL_WLAN_STATUS_CONNECTED}
};

StrMpl_List_t ATCmd_eventWlanProvisioningStatus[18] =
{
    {"general_error",SL_WLAN_PROVISIONING_GENERAL_ERROR},
    {"network_not_found",SL_WLAN_PROVISIONING_CONFIRMATION_STATUS_FAIL_NETWORK_NOT_FOUND},
    {"connection_failed",SL_WLAN_PROVISIONING_CONFIRMATION_STATUS_FAIL_CONNECTION_FAILED},
    {"ip_not_acquired",SL_WLAN_PROVISIONING_CONFIRMATION_STATUS_CONNECTION_SUCCESS_IP_NOT_ACQUIRED},
    {"feedback_failed",SL_WLAN_PROVISIONING_CONFIRMATION_STATUS_SUCCESS_FEEDBACK_FAILED},
    {"success",SL_WLAN_PROVISIONING_CONFIRMATION_STATUS_SUCCESS},
    {"abort",SL_WLAN_PROVISIONING_ERROR_ABORT},
    {"invalid_param",SL_WLAN_PROVISIONING_ERROR_ABORT_INVALID_PARAM},
    {"http_server_disabled",SL_WLAN_PROVISIONING_ERROR_ABORT_HTTP_SERVER_DISABLED},
    {"profile_list_full",SL_WLAN_PROVISIONING_ERROR_ABORT_PROFILE_LIST_FULL},
    {"provisioning_already_started",SL_WLAN_PROVISIONING_ERROR_ABORT_PROVISIONING_ALREADY_STARTED},
    {"auto_started",SL_WLAN_PROVISIONING_AUTO_STARTED},
    {"stopped",SL_WLAN_PROVISIONING_STOPPED},
    {"smart_config_synced",SL_WLAN_PROVISIONING_SMART_CONFIG_SYNCED},
    {"smart_config_sync_timeout",SL_WLAN_PROVISIONING_SMART_CONFIG_SYNC_TIMEOUT},
    {"wlan_connect",SL_WLAN_PROVISIONING_CONFIRMATION_WLAN_CONNECT},
    {"ip_acquired",SL_WLAN_PROVISIONING_CONFIRMATION_IP_ACQUIRED},
    {"ext_cfg_ready",SL_WLAN_PROVISIONING_EXTERNAL_CONFIGURATION_READY}
};

StrMpl_List_t ATCmd_eventSockId[2] =
{
    {"tx_failed",SL_SOCKET_TX_FAILED_EVENT},
    {"async_event",SL_SOCKET_ASYNC_EVENT}
};

StrMpl_List_t ATCmd_eventSockType[6] =
{
    {"ssl_accept",SL_SSL_ACCEPT},
    {"rx_frag_too_big",SL_RX_FRAGMENTATION_TOO_BIG},
    {"other_side_close_ssl",SL_OTHER_SIDE_CLOSE_SSL_DATA_NOT_ENCRYPTED},
    {"connected_secured",SL_SSL_NOTIFICATION_CONNECTED_SECURED},
    {"connected_secured",SL_SSL_NOTIFICATION_CONNECTED_SECURED},
    {"wrong_root_ca",SL_SSL_NOTIFICATION_WRONG_ROOT_CA}
};

StrMpl_List_t ATCmd_eventNetappId[8] =
{
    {"ipv4_acquired",SL_NETAPP_EVENT_IPV4_ACQUIRED},
    {"ipv6_acquired",SL_NETAPP_EVENT_IPV6_ACQUIRED},
    {"ip_collision",SL_NETAPP_EVENT_IP_COLLISION},
    {"dhcpv4_leased",SL_NETAPP_EVENT_DHCPV4_LEASED},
    {"dhcpv4_released",SL_NETAPP_EVENT_DHCPV4_RELEASED},
    {"ipv4_lost",SL_NETAPP_EVENT_IPV4_LOST},
    {"dhcp_ipv4_acquire_timeout",SL_NETAPP_EVENT_DHCP_IPV4_ACQUIRE_TIMEOUT},
    {"ipv6_lost",SL_NETAPP_EVENT_IPV6_LOST}
};

StrMpl_List_t ATCmd_eventHttpServerId[2] =
{
    {"get_token",SL_NETAPP_EVENT_HTTP_TOKEN_GET},
    {"post_token",SL_NETAPP_EVENT_HTTP_TOKEN_POST}
};

StrMpl_List_t ATCmd_netUtilGet[2] =
{
    {"public_key",SL_NETUTIL_CRYPTO_PUBLIC_KEY},
    {"true_random",SL_NETUTIL_TRUE_RANDOM}
};

StrMpl_List_t ATCmd_netUtilCmd[5] =
{
    {"create_cert",SL_NETUTIL_CRYPTO_CMD_CREATE_CERT},
    {"sign_msg",SL_NETUTIL_CRYPTO_CMD_SIGN_MSG},
    {"verify_msg",SL_NETUTIL_CRYPTO_CMD_VERIFY_MSG},
    {"temp_keys",SL_NETUTIL_CRYPTO_CMD_TEMP_KEYS},
    {"install_op",SL_NETUTIL_CRYPTO_CMD_INSTALL_OP}
};

StrMpl_List_t ATCmd_netUtilTempKeys[2] =
{
    {"create",SL_NETUTIL_CRYPTO_TEMP_KEYS_CREATE},
    {"remove",SL_NETUTIL_CRYPTO_TEMP_KEYS_REMOVE}
};

StrMpl_List_t ATCmd_netUtilInstallOp[2] =
{
    {"install",SL_NETUTIL_CRYPTO_INSTALL_SUB_CMD},
    {"uninstall",SL_NETUTIL_CRYPTO_UNINSTALL_SUB_CMD}
};

StrMpl_List_t ATCmd_netUtilPubKeyAlgo[2] =
{
    {"none",SL_NETUTIL_CRYPTO_PUB_KEY_ALGO_NONE},
    {"ec",SL_NETUTIL_CRYPTO_PUB_KEY_ALGO_EC}
};

StrMpl_List_t ATCmd_netUtilEcNamedCurve[2] =
{
    {"none",SL_NETUTIL_CRYPTO_EC_NAMED_CURVE_NONE},
    {"secp256r1",SL_NETUTIL_CRYPTO_EC_NAMED_CURVE_SECP256R1}
};

StrMpl_List_t ATCmd_mqttCreateFlags[7] =
{
    {"ip4",     MQTTCLIENT_NETCONN_IP4},
    {"ip6",     MQTTCLIENT_NETCONN_IP6},
    {"url",     MQTTCLIENT_NETCONN_URL},
    {"sec",     MQTTCLIENT_NETCONN_SEC},
    {"skip_domain_verify",  MQTTCLIENT_NETCONN_SKIP_DOMAIN_NAME_VERIFICATION},
    {"skip_cert_verify",    MQTTCLIENT_NETCONN_SKIP_CERTIFICATE_CATALOG_VERIFICATION},
    {"skip_date_verify",    MQTTCLIENT_NETCONN_SKIP_DATE_VERIFICATION}
};

StrMpl_List_t ATCmd_mqttCreateMode[2] =
{
    {"v3_1",     true},
    {"v3_1_1",   false}
};

StrMpl_List_t ATCmd_mqttQos[3] =
{
    {"qos0",     MQTT_QOS_0},
    {"qos1",     MQTT_QOS_1},
    {"qos2",     MQTT_QOS_2}
};

StrMpl_List_t ATCmd_mqttSetOptions[5] =
{
    {"user",     MQTTClient_USER_NAME},
    {"password", MQTTClient_PASSWORD},
    {"will",     MQTTClient_WILL_PARAM},
    {"keepalive",MQTTClient_KEEPALIVE_TIME},        
    {"clean",    MQTTClient_CLEAN_CONNECT}        
};

StrMpl_List_t ATCmd_mqttEventId[3] =
{
    {"operation", MQTTClient_OPERATION_CB_EVENT},
    {"recv",      MQTTClient_RECV_CB_EVENT},
    {"disconnect",MQTTClient_DISCONNECT_CB_EVENT}
};

StrMpl_List_t ATCmd_mqttEventOperationId[4] =
{
    {"connack", MQTTCLIENT_OPERATION_CONNACK},
    {"puback",  MQTTCLIENT_OPERATION_EVT_PUBACK},
    {"suback",  MQTTCLIENT_OPERATION_SUBACK},
    {"unsuback",MQTTCLIENT_OPERATION_UNSUBACK}
};

StrMpl_List_t ATCmd_httpConnectFlags[2] =
{
    {"ignore_proxy",   HTTPClient_IGNORE_PROXY},
    {"host_exist",     HTTPClient_HOST_EXIST}
};

StrMpl_List_t ATCmd_httpSendReqFlags[3] =
{
    {"chunk_start",   HTTPClient_CHUNK_START},      /* Sets the client's request state into chunked body */
    {"chunk_end",     HTTPClient_CHUNK_END},        /* Sets the client's request state out of chunked body nd sends last chunk */
    {"drop_body",     HTTPClient_DROP_BODY}         /* Flushes the response body */
};

StrMpl_List_t ATCmd_httpSendReqMethod[7] =
{
    {"get",         ATCMDHTTP_METHOD_GET},
    {"post",        ATCMDHTTP_METHOD_POST},
    {"head",        ATCMDHTTP_METHOD_HEAD},
    {"options",     ATCMDHTTP_METHOD_OPTIONS},
    {"put",         ATCMDHTTP_METHOD_PUT},
    {"del",         ATCMDHTTP_METHOD_DELETE},
    {"connect",     ATCMDHTTP_METHOD_CONNECT}
};

StrMpl_List_t ATCmd_httpSetHeaderFlags[2] =
{
    {"not_persistent", HTTPClient_HFIELD_NOT_PERSISTENT},   /* Header Field added is not persistent */
    {"persistent",     HTTPClient_HFIELD_PERSISTENT}        /* Header Field added is persistent */
};

StrMpl_List_t ATCmd_httpHeaderOption[58] =
{
    {"res_age",                 HTTPClient_HFIELD_RES_AGE},
    {"res_allow",               HTTPClient_HFIELD_RES_ALLOW},
    {"res_cache_control",       HTTPClient_HFIELD_RES_CACHE_CONTROL},
    {"res_connection",          HTTPClient_HFIELD_RES_CONNECTION},
    {"res_content_encoding",    HTTPClient_HFIELD_RES_CONTENT_ENCODING},
    {"res_content_language",    HTTPClient_HFIELD_RES_CONTENT_LANGUAGE},
    {"res_content_length",      HTTPClient_HFIELD_RES_CONTENT_LENGTH},
    {"res_content_location",    HTTPClient_HFIELD_RES_CONTENT_LOCATION},
    {"res_content_range",       HTTPClient_HFIELD_RES_CONTENT_RANGE},
    {"res_content_type",        HTTPClient_HFIELD_RES_CONTENT_TYPE},
    {"res_date",                HTTPClient_HFIELD_RES_DATE},
    {"res_etag",                HTTPClient_HFIELD_RES_ETAG},
    {"res_expires",             HTTPClient_HFIELD_RES_EXPIRES},
    {"res_last_modified",       HTTPClient_HFIELD_RES_LAST_MODIFIED},
    {"res_location",            HTTPClient_HFIELD_RES_LOCATION},
    {"res_proxy_auth",          HTTPClient_HFIELD_RES_PROXY_AUTHENTICATE},
    {"res_retry_after",         HTTPClient_HFIELD_RES_RETRY_AFTER},
    {"res_server",              HTTPClient_HFIELD_RES_SERVER},
    {"res_set_cookie",          HTTPClient_HFIELD_RES_SET_COOKIE},
    {"res_trailer",             HTTPClient_HFIELD_RES_TRAILER},
    {"res_tx_encoding",         HTTPClient_HFIELD_RES_TRANSFER_ENCODING},
    {"res_upgrade",             HTTPClient_HFIELD_RES_UPGRADE},
    {"res_vary",                HTTPClient_HFIELD_RES_VARY},
    {"res_via",                 HTTPClient_HFIELD_RES_VIA},
    {"res_www_auth",            HTTPClient_HFIELD_RES_WWW_AUTHENTICATE},
    {"res_warning",             HTTPClient_HFIELD_RES_WARNING},   
    {"req_accept",              HTTPClient_HFIELD_REQ_ACCEPT},
    {"req_accept_charset",      HTTPClient_HFIELD_REQ_ACCEPT_CHARSET},
    {"req_accept_encoding",     HTTPClient_HFIELD_REQ_ACCEPT_ENCODING},
    {"req_accept_language",     HTTPClient_HFIELD_REQ_ACCEPT_LANGUAGE},
    {"req_allow",               HTTPClient_HFIELD_REQ_ALLOW},
    {"req_auth",                HTTPClient_HFIELD_REQ_AUTHORIZATION},
    {"req_cache_control",       HTTPClient_HFIELD_REQ_CACHE_CONTROL},
    {"req_connection",          HTTPClient_HFIELD_REQ_CONNECTION},
    {"req_content_encoding",    HTTPClient_HFIELD_REQ_CONTENT_ENCODING},
    {"req_content_language",    HTTPClient_HFIELD_REQ_CONTENT_LANGUAGE},
    {"req_content_location",    HTTPClient_HFIELD_REQ_CONTENT_LOCATION},
    {"req_content_type",        HTTPClient_HFIELD_REQ_CONTENT_TYPE},
    {"req_cookie",              HTTPClient_HFIELD_REQ_COOKIE},
    {"req_date",                HTTPClient_HFIELD_REQ_DATE},
    {"req_expect",              HTTPClient_HFIELD_REQ_EXPECT},
    {"req_forwarded",           HTTPClient_HFIELD_REQ_FORWARDED},
    {"req_from",                HTTPClient_HFIELD_REQ_FROM},
    {"req_host",                HTTPClient_HFIELD_REQ_HOST},
    {"req_if_match",            HTTPClient_HFIELD_REQ_IF_MATCH},
    {"req_if_modified_since",   HTTPClient_HFIELD_REQ_IF_MODIFIED_SINCE},
    {"req_if_none_match",       HTTPClient_HFIELD_REQ_IF_NONE_MATCH},
    {"req_if_range",            HTTPClient_HFIELD_REQ_IF_RANGE},
    {"req_if_unmodified_since", HTTPClient_HFIELD_REQ_IF_UNMODIFIED_SINCE},
    {"req_origin",              HTTPClient_HFIELD_REQ_ORIGIN},
    {"req_proxy_auth",          HTTPClient_HFIELD_REQ_PROXY_AUTHORIZATION},
    {"req_range",               HTTPClient_HFIELD_REQ_RANGE},
    {"req_te",                  HTTPClient_HFIELD_REQ_TE},
    {"req_tx_encoding",         HTTPClient_HFIELD_REQ_TRANSFER_ENCODING},
    {"req_upgrade",             HTTPClient_HFIELD_REQ_UPGRADE},
    {"req_user_agent",          HTTPClient_HFIELD_REQ_USER_AGENT},
    {"req_via",                 HTTPClient_HFIELD_REQ_VIA},
    {"req_warning",             HTTPClient_HFIELD_REQ_WARNING}
};

StrMpl_List_t ATCmd_httpOptOption[3] =
{
    {"redirect_feature",        HTTPClient_REDIRECT_FEATURE},               /* Enable / Disable redirect feature */
    {"res_filter_clear",        HTTPClient_RESPONSE_FILTER_CLEAR},          /* Clear response filter to default(all enabled) */
    {"redirect_tls_downgrade",  HTTPClient_REDIRECT_TLS_DOWNGRADE}         /* Enable / Disable  the option for tls downgrade */
};

