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
#include <ti/net/atcmd/atcmd_wlan.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDWLAN_MAX_SCAN_RESULTS              (30)
#define ATCMDWLAN_MAX_KEY_LEN                   (64)
#define ATCMDWLAN_MAX_USER_LEN                  (32)
#define ATCMDWLAN_MAX_ANON_USER_LEN             (32)

//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmdWlan_Scan_t_
{
    uint8_t                 numOfentries;       /* Number of Scan Entries to retrieve from the NWP */
    uint8_t                 index;              /* The netEntries array position to start write results from */
    SlWlanNetworkEntry_t    *netEntries;        /* The netEntries array */
}ATCmdWlan_Scan_t;


typedef struct _ATCmdWlan_Connect_t_
{
    char                    *ssid;             /* Ap's SSID */
    uint8_t                 *bssid;            /* AP's MAC Address */
    SlWlanSecParams_t       secParams;         /* Security parameters - Security Type and Password */
    SlWlanSecParamsExt_t    *secParamsEnt;     /* Enterprise parameters - Security Type and Password */
}ATCmdWlan_Connect_t;

typedef struct _ATCmdWlan_ProfileAdd_t_
{
    char                    *ssid;              /* Ap's SSID */
    uint8_t                 *bssid;             /* AP's MAC Address */
    SlWlanSecParams_t       secParams;          /* Security parameters - Security Type and Password */
    SlWlanSecParamsExt_t    *secParamsEnt;      /* Enterprise parameters - Security Type and Password */
    uint32_t                priority;           /* Profile priority. Lowest priority: 0, Highest priority: 15. */
}ATCmdWlan_ProfileAdd_t;

typedef struct _ATCmdWlan_ProfileGet_t_
{
    char                    ssid[SL_WLAN_SSID_MAX_LENGTH+1];              /* Ap's SSID */
    uint8_t                 bssid[SL_WLAN_BSSID_LENGTH];             /* AP's MAC Address */
    SlWlanSecParams_t       secParams;          /* Security parameters - Security Type and Password */
    SlWlanGetSecParamsExt_t *secParamsEnt;      /* Enterprise parameters - Security Type and Password */
    uint32_t                priority;           /* Profile priority. Lowest priority: 0, Highest priority: 15. */
}ATCmdWlan_ProfileGet_t;

typedef struct _ATCmdWlan_Policy_t_
{
    uint8_t                 type;
    uint8_t                 policy;
    uint8_t                 valLen;
    uint8_t                 *value;
}ATCmdWlan_Policy_t;

typedef struct _ATCmdWlan_PolicyGet_t_
{
    uint8_t                 type;
    uint8_t                 policy;
    uint8_t                 valLen;
    uint8_t                 value[8];
}ATCmdWlan_PolicyGet_t;

typedef struct _ATCmdWlan_Set_t_
{
    uint16_t                id;
    uint16_t                option;
    uint16_t                len;
    uint8_t                 *value;
}ATCmdWlan_Set_t;


//*****************************************************************************
// AT Command Wlan Routines
//*****************************************************************************


/*!
    \brief          Prints WLAN profile add command help menu.

    \param          arg       -   Points to arguments.

    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             ATCmdWlan_profileAddCallback
*/
int32_t ATCmdWlan_connectFree(ATCmdWlan_Connect_t *connectParams)
{    
    if (connectParams->ssid != NULL)
    {
        free(connectParams->ssid);
    }

    if (connectParams->bssid != NULL)
    {
        free(connectParams->bssid);
    }

    if (connectParams->secParams.Key != NULL)
    {
        free(connectParams->secParams.Key);
    }

    if (connectParams->secParamsEnt != NULL)
    {
        if (connectParams->secParamsEnt->User != NULL)
        {
            free(connectParams->secParamsEnt->User);
        }
        if (connectParams->secParamsEnt->AnonUser!= NULL)
        {
            free(connectParams->secParamsEnt->AnonUser);
        }
        free(connectParams->secParamsEnt);
    }
            
    return 0;
}


int32_t ATCmdWlan_connectParse(char *buff, ATCmdWlan_Connect_t *params)
{
    int32_t ret = 0;

    /* SSID*/
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->ssid), ATCMD_DELIM_ARG, SL_WLAN_SSID_MAX_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }

    /* BSSID */
    params->bssid = malloc(SL_WLAN_BSSID_LENGTH);
    if (params->bssid == NULL)
    {
        return -1;
    }
    if ((ret = StrMpl_getArrayVal(&buff, (void *)params->bssid, SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
    {
        if (ret == STRMPL_ERROR_PARAM_MISSING)
        {
            free(params->bssid);
            params->bssid = NULL;
        }
        else
        {
            return ret;
        }
    }

    /* SecurityType */
    if ((ret = StrMpl_getListVal(ATCmd_wlanSecurity, sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t), &buff, &params->secParams.Type, ATCMD_DELIM_ARG, sizeof(params->secParams.Type))) < 0)
    {
        return ret;
    }

    /* SecurityKey */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParams.Key, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_KEY_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
            
    params->secParamsEnt = malloc(sizeof(SlWlanSecParamsExt_t));
    if (params->secParamsEnt == NULL)
    {
        return -1;
    }
    memset(params->secParamsEnt, 0x0, sizeof(SlWlanSecParamsExt_t));

    /* SecurityExtUser */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParamsEnt->User, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_USER_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* SecurityExtAnonUser*/
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParamsEnt->AnonUser, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_ANON_USER_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* SecurityExtEapMethod */
    if ((ret = StrMpl_getListVal(ATCmd_wlanEap, sizeof(ATCmd_wlanEap)/sizeof(StrMpl_List_t), &buff, &params->secParamsEnt->EapMethod, ATCMD_DELIM_TRM, sizeof(params->secParamsEnt->EapMethod))) < 0)
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
    \brief          WLAN connect callback.

    This routine shows how to Connect to an AP.
    As a part of the connection process, we also demonstrate how to set a static IP,
    Static default gateway address and static DNS.
    Enterprise credentials are also shown for users who wish to connect to an enterprise network.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           For more information about Security types, Enterprise networks,
                    And driver asynchronous events related to connection process,
                    please refer to CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \sa             setStaticIPConfig

*/
int32_t ATCmdWlan_connectCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_Connect_t  connectParams;
    
    memset(&connectParams, 0x0, sizeof(ATCmdWlan_Connect_t));
    
    /* Call the command parser */
    ret = ATCmdWlan_connectParse((char *)arg,&connectParams);    

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_connectFree(&connectParams);        
	    return -1;
    }

    if ((connectParams.secParamsEnt->User == NULL) && (connectParams.secParamsEnt->AnonUser == NULL))
    {
        free (connectParams.secParamsEnt);
        connectParams.secParamsEnt = NULL;
    }
    else
    {
        if (connectParams.secParamsEnt->User != NULL)
        {
            connectParams.secParamsEnt->UserLen = strlen((char const *)(connectParams.secParamsEnt->User));
        }
        if (connectParams.secParamsEnt->AnonUser != NULL)
        {
            connectParams.secParamsEnt->AnonUserLen = strlen((char const *)(connectParams.secParamsEnt->AnonUser));
        }
    }

    connectParams.secParams.KeyLen = strlen((char const *)(connectParams.secParams.Key));

    /* Connect to ENT AP */
    ret = sl_WlanConnect((const signed char *)connectParams.ssid, strlen((const char *)(connectParams.ssid)), connectParams.bssid, &(connectParams.secParams), connectParams.secParamsEnt);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdWlan_connectFree(&connectParams);        

    return ret;
}


/*!
    \brief          Parse Scan command.

    This routine takes a ATCmdWlan_Scan_t structure, and fill it's content with
    parameters taken from command line. In case of a parsing error or invalid parameters,
    this function sets default values.

    \param          arg         -   Points to command line buffer.
                                    Contains the command line typed by user.

    \param          scanParams  -   Points to command structure provided by the scan callback.
                                    This structure will later be read by the scan callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error, and show
                    the Scan command help menu.

    \sa             ATCmdWlan_scanUsage
*/
int32_t ATCmdWlan_scanParse(char *buff, ATCmdWlan_Scan_t *scanParams)
{
    int32_t ret = 0;
    
    /* Index*/
    if ((ret = StrMpl_getVal(&buff, &(scanParams->index), ATCMD_DELIM_ARG, sizeof(scanParams->index))) < 0)
    {
        return ret;
    }

    /* num of results */
    if ((ret = StrMpl_getVal(&buff, &(scanParams->numOfentries), ATCMD_DELIM_TRM, sizeof(scanParams->numOfentries) )) < 0)
    {
        return ret;
    }

    return ret;
}


/*!
    \brief          free scan command allocations.

    \param          arg       -   NULL
                    buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             ATCmdWlan_scanCallback
*/
int32_t ATCmdWlan_scanFree(ATCmdWlan_Scan_t *scanParams)
{    
    if (scanParams != NULL)
    {
        if (scanParams->netEntries != NULL)
        {
            free (scanParams->netEntries);
        }
        free (scanParams);
    }
    return 0;
}

/*!
    \brief          Return Scan results.

    This routine send the retrieved scan results to the Host.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             ATCmdWlan_scanResult

*/
int32_t ATCmdWlan_scanResult(void *args, int32_t num, char *buff)
{
    ATCmdWlan_Scan_t *scanParams = (ATCmdWlan_Scan_t *)(args);
    SlWlanNetworkEntry_t  *netEntry;
    uint8_t param;
    
    if(num < scanParams->numOfentries)
    {
        StrMpl_setStr(ATCmd_wlanScanStr,&buff,ATCMD_DELIM_EVENT);

        netEntry = scanParams->netEntries + num;
        StrMpl_setStr((char *)netEntry->Ssid,&buff,ATCMD_DELIM_ARG);
        StrMpl_setArrayVal(netEntry->Bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&netEntry->Rssi,&buff,ATCMD_DELIM_ARG,sizeof(netEntry->Rssi)|STRMPL_FLAG_PARAM_DEC| STRMPL_FLAG_PARAM_SIGNED);
        StrMpl_setVal(&netEntry->Channel,&buff,ATCMD_DELIM_ARG,sizeof(netEntry->Channel)|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
        param = SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(netEntry->SecurityInfo);
        StrMpl_setListStr(ATCmd_wlanScanSecurity,sizeof(ATCmd_wlanScanSecurity)/sizeof(StrMpl_List_t),(void *)&param,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        param = SL_WLAN_SCAN_RESULT_HIDDEN_SSID(netEntry->SecurityInfo);
        StrMpl_setVal(&param,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC| STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        param = SL_WLAN_SCAN_RESULT_UNICAST_CIPHER_BITMAP(netEntry->SecurityInfo);
        StrMpl_setListStr(ATCmd_wlanScanCipher,sizeof(ATCmd_wlanScanCipher)/sizeof(StrMpl_List_t),&param,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        param = SL_WLAN_SCAN_RESULT_KEY_MGMT_SUITES_BITMAP(netEntry->SecurityInfo);
        StrMpl_setListStr(ATCmd_wlanScanKeyMgmt,sizeof(ATCmd_wlanScanKeyMgmt)/sizeof(StrMpl_List_t),&param,&buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        return 0;
    }
    else
    {
        ATCmdWlan_scanFree(scanParams);
        return -1;
    }
}


/*!
    \brief          Scan callback.

    This routine shows how to retrieve scan results form the NWP.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           If scans aren't active, this function triggers one scan
                    and later prints the results.

    \sa             ATCmdWlan_scanParse

*/
int32_t ATCmdWlan_scanCallback(void *arg)
{

    int32_t         ret = -1;
    ATCmdWlan_Scan_t      *scanParams;
    uint8_t         i;

    scanParams = malloc(sizeof(ATCmdWlan_Scan_t));
    if (scanParams == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return (-1);
    }
    memset(scanParams, 0x0, sizeof(ATCmdWlan_Scan_t));

    /* Call the command parser */
    ret = ATCmdWlan_scanParse((char *)arg, scanParams);

    if(ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_scanFree(scanParams);
        return -1;
    }

    if (scanParams->numOfentries > ATCMDWLAN_MAX_SCAN_RESULTS)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,0);
        ATCmdWlan_scanFree(scanParams);
        return -1;
    }
    /* Clear the results buffer */
    scanParams->netEntries = malloc(scanParams->numOfentries * sizeof(SlWlanNetworkEntry_t));
    if (scanParams->netEntries == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdWlan_scanFree(scanParams);
        return -1;
    }

    memset(scanParams->netEntries, 0x0, scanParams->numOfentries * sizeof(SlWlanNetworkEntry_t));

    /* Get scan results from NWP - results would be placed inside the provided buffer */
    ret = sl_WlanGetNetworkList(scanParams->index, scanParams->numOfentries, scanParams->netEntries);

    if(ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdWlan_scanFree(scanParams);
    }
    else
    {
        for (i=0;i < scanParams->numOfentries;i++)
        {
            if ((scanParams->netEntries + i)->SsidLen > 0)
            {
                ATCmd_commandResult(ATCmdWlan_scanResult,scanParams,i);
            }
        }
        /* send this result only to free the allocated scan buffer */        
        ATCmd_commandResult(ATCmdWlan_scanResult,scanParams,i);
        ATCmd_okResult();
    }
    
    return ret;
}

/*!
    \brief          Disconnect callback.

    This routine shows how to disconnect a device from an AP.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             ParseCmd

*/
int32_t ATCmdWlan_disconnectCallback(void *arg)
{
    int32_t          ret;

    /* After calling sl_WlanDisconnect(), we expect WLAN disconnect asynchronous event.
     * Cleaning the former connection information from the application control block
     * is handled in that event handler, as well as getting the disconnect reason.
     */
    ret = sl_WlanDisconnect();
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
    \brief          Parse profile Add command.

    This routine takes a ATCmdWlan_ProfileAdd_t structure, and fill it's content with
    parameters taken from command line. It sets some default values pre-parsing,
    and checks the parameters validity.
    In case of a parsing error or invalid parameters, this function send error.

    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          profileAddParams  -   Points to command structure provided by the profileAdd callback.
                                       This structure will later be read by the profileAdd callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would send error.

    \sa             ATCmdWlan_profileAddCallback
*/
int32_t ATCmdWlan_profileAddParse(char *buff, ATCmdWlan_ProfileAdd_t *params)
{
    int32_t ret = 0;
    
    /* SSID*/
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&(params->ssid), ATCMD_DELIM_ARG, SL_WLAN_SSID_MAX_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* BSSID */
    params->bssid = malloc(SL_WLAN_BSSID_LENGTH);
    if (params->bssid == NULL)
    {
        return -1;
    }
    if ((ret = StrMpl_getArrayVal(&buff, (void *)params->bssid, SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
    {
        if (ret == STRMPL_ERROR_PARAM_MISSING)
        {
            free(params->bssid);
            params->bssid = NULL;
        }
        else
        {
            return ret;
        }
    }

    /* SecurityType */
    if ((ret = StrMpl_getListVal(ATCmd_wlanSecurity, sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t), &buff, &params->secParams.Type, ATCMD_DELIM_ARG, sizeof(params->secParams.Type))) < 0)
    {
        return ret;
    }

    /* SecurityKey */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParams.Key, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_KEY_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
            
    params->secParamsEnt = malloc(sizeof(SlWlanSecParamsExt_t));
    if (params->secParamsEnt == NULL)
    {
        return -1;
    }
    memset(params->secParamsEnt, 0x0, sizeof(SlWlanSecParamsExt_t));

    /* SecurityExtUser */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParamsEnt->User, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_USER_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* SecurityExtAnonUser*/
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->secParamsEnt->AnonUser, ATCMD_DELIM_ARG, ATCMDWLAN_MAX_ANON_USER_LEN ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* SecurityExtEapMethod */
    if ((ret = StrMpl_getListVal(ATCmd_wlanEap, sizeof(ATCmd_wlanEap)/sizeof(StrMpl_List_t), &buff, &params->secParamsEnt->EapMethod, ATCMD_DELIM_ARG, sizeof(params->secParamsEnt->EapMethod))) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }

    /* priority */
    if ((ret = StrMpl_getVal(&buff, &params->priority, ATCMD_DELIM_TRM, sizeof(params->priority))) < 0)
    {
        return ret;
    }

    return ret;
}

/*!
    \brief          Return profile Add result.

    This routine send the profile index to the Host.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_profileAddResult(void *args, int32_t num, char *buff)
{
    
    StrMpl_setStr(ATCmd_wlanProfileAddStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&num,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_SIGNED);

    return 0;
}

/*!
    \brief          Prints WLAN profile add command help menu.

    \param          arg       -   Points to arguments.

    \param          buff      -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             ATCmdWlan_profileAddCallback
*/
int32_t ATCmdWlan_profileAddFree(ATCmdWlan_ProfileAdd_t *profileAddParams)
{    
    if (profileAddParams->ssid != NULL)
    {
        free(profileAddParams->ssid);
    }
    if (profileAddParams->bssid != NULL)
    {
        free(profileAddParams->bssid);
    }
    if (profileAddParams->secParams.Key != NULL)
    {
        free(profileAddParams->secParams.Key);
    }
    if (profileAddParams->secParamsEnt != NULL)
    {
        if (profileAddParams->secParamsEnt->User != NULL)
        {
            free(profileAddParams->secParamsEnt->User);
        }
        if (profileAddParams->secParamsEnt->AnonUser!= NULL)
        {
            free(profileAddParams->secParamsEnt->AnonUser);
        }
        free(profileAddParams->secParamsEnt);
    }
            
    return 0;
}

/*!
    \brief          WLAN profile add callback.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_profileAddCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_ProfileAdd_t  profileAddParams;

    memset(&profileAddParams, 0x0, sizeof(ATCmdWlan_ProfileAdd_t));
    
    /* Call the command parser */
    ret = ATCmdWlan_profileAddParse((char *)arg,&profileAddParams);

	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_profileAddFree(&profileAddParams);
	    return -1;
    }

    if ((profileAddParams.secParamsEnt->User == NULL) && (profileAddParams.secParamsEnt->AnonUser == NULL))
    {
        free (profileAddParams.secParamsEnt);
        profileAddParams.secParamsEnt = NULL;
    }
    else
    {
        if (profileAddParams.secParamsEnt->User != NULL)
        {
            profileAddParams.secParamsEnt->UserLen = strlen((char const *)(profileAddParams.secParamsEnt->User));
        }
        if (profileAddParams.secParamsEnt->AnonUser != NULL)
        {
            profileAddParams.secParamsEnt->AnonUserLen = strlen((char const *)(profileAddParams.secParamsEnt->AnonUser));
        }
    }

    profileAddParams.secParams.KeyLen = strlen((char const *)(profileAddParams.secParams.Key));
    
    /* add the profile */
    ret = sl_WlanProfileAdd((const signed char *)profileAddParams.ssid, strlen((const char *)(profileAddParams.ssid)), profileAddParams.bssid, &profileAddParams.secParams, profileAddParams.secParamsEnt,profileAddParams.priority,0);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_commandResult(ATCmdWlan_profileAddResult,NULL,ret);
        ATCmd_okResult();
    }

    ATCmdWlan_profileAddFree(&profileAddParams);

    
    return ret;
}


/*!
    \brief          Return profile Add result.

    This routine send the profile index to the Host.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_profileGetResult(void *args, int32_t num, char *buff)
{
    ATCmdWlan_ProfileGet_t *profileGetParams = (ATCmdWlan_ProfileGet_t *)args;

    StrMpl_setStr(ATCmd_wlanProfileGetStr,&buff,ATCMD_DELIM_EVENT);
    /* ssid */
    StrMpl_setStr(profileGetParams->ssid,&buff,ATCMD_DELIM_ARG);
    /* bssid */
    if ((profileGetParams->bssid[0] == 0) && (profileGetParams->bssid[1] == 0) && (profileGetParams->bssid[2] == 0) &&
        (profileGetParams->bssid[3] == 0) && (profileGetParams->bssid[4] == 0) && (profileGetParams->bssid[5] == 0))
    {
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
    }
    else
    {
        StrMpl_setArrayVal(profileGetParams->bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    /* SecurityType */
    StrMpl_setListStr(ATCmd_wlanSecurity,sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t),&profileGetParams->secParams.Type,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
    /* SecurityKey */
    StrMpl_setStr((char *)profileGetParams->secParams.Key,&buff,ATCMD_DELIM_ARG);
    if (profileGetParams->secParamsEnt!= NULL)
    {
        /* SecurityExtUser */
        StrMpl_setStr((char *)profileGetParams->secParamsEnt->User,&buff,ATCMD_DELIM_ARG);
        /* SecurityExtAnonUser */
        StrMpl_setStr((char *)profileGetParams->secParamsEnt->AnonUser,&buff,ATCMD_DELIM_ARG);
        /* SecurityExtEapMethod */
        StrMpl_setListStr(ATCmd_wlanEap,sizeof(ATCmd_wlanEap)/sizeof(StrMpl_List_t),&profileGetParams->secParamsEnt->EapMethod,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else
    {
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
    }

    /* Profile priority */
    StrMpl_setVal(&profileGetParams->priority,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
    
    free(profileGetParams);

    return 0;
}

int32_t ATCmdWlan_profileGetCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_ProfileGet_t  *params;
    uint16_t index;
    int16_t ssidLen = 0;
	
    params = malloc(sizeof(ATCmdWlan_ProfileGet_t));
    if (params == NULL)
    {
       ATCmd_errorResult(ATCmd_errorAllocStr,0);
       return -1;
    }
    memset(params, 0x0, sizeof(ATCmdWlan_ProfileGet_t));

    /* Call the command parser */
    if ((ret = StrMpl_getVal((char **)&arg,&index,ATCMD_DELIM_TRM,sizeof(index))) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        free(params);
        return (-1);
    }
	
    /* add the profile */
    ret = sl_WlanProfileGet(index, (signed char *)params->ssid,&ssidLen, params->bssid, &params->secParams, params->secParamsEnt,(unsigned long *)&params->priority);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        free(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdWlan_profileGetResult,params,0);
        ATCmd_okResult();
    }
    
    return ret;
}


/*!
    \brief          WLAN profile del callback.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_profileDelCallback(void *arg)
{
    int32_t ret = 0;
    uint16_t index;

    /* Call the command parser */
    if ((ret = StrMpl_getVal((char **)&arg,&index,ATCMD_DELIM_TRM,sizeof(index))) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        return (-1);
    }
    
    /* delete the profile */
    ret = sl_WlanProfileDel(index);

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
int32_t ATCmdWlan_policySetFree(ATCmdWlan_Policy_t *params)
{    
    if (params->value != NULL)
    {
        free(params->value);
    }  
    return 0;
}

/*!
    \brief          Parse policy set command.


    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          connectParams  -   Points to command structure provided by the connect callback.
                                       This structure will later be read by the connect callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would send error.

    \sa             cmdConnectCallback
*/
int32_t ATCmdWlan_policySetParse(char *buff, ATCmdWlan_Policy_t *params)
{
    int32_t ret = 0;
    uint8_t p2pRole;
    uint8_t p2pInit;
    
    /* Type */
    if ((ret = StrMpl_getListVal(ATCmd_wlanPolicy,sizeof(ATCmd_wlanPolicy)/sizeof(StrMpl_List_t),&buff,&params->type,ATCMD_DELIM_ARG,sizeof(params->type))) < 0)
    {
        return ret;
    }
    /* Policy */
    if (params->type == SL_WLAN_POLICY_CONNECTION)
    {
        if ((ret = StrMpl_getBitmaskListVal(ATCmd_wlanConnPolicy, sizeof(ATCmd_wlanConnPolicy)/sizeof(StrMpl_List_t),&buff,&params->policy,ATCMD_DELIM_ARG,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
        {
            if (ret != STRMPL_ERROR_PARAM_MISSING)
            {
                return ret;
            }
        }
    }
    else if (params->type == SL_WLAN_POLICY_SCAN)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanScanPolicy,sizeof(ATCmd_wlanScanPolicy)/sizeof(StrMpl_List_t),&buff,&params->policy,ATCMD_DELIM_ARG,sizeof(params->policy))) < 0)
        {
            return ret;
        }
        /* scan interval  */
        params->valLen = sizeof(uint32_t);
        params->value = malloc(params->valLen);
        if (params->value == NULL)
        {
            return -1;
        }
        if ((params->policy == SL_WLAN_SCAN_POLICY(1,1)) || (params->policy == SL_WLAN_SCAN_POLICY(1,0)))
        {                

            if ((ret = StrMpl_getVal(&buff,(void *)params->value,ATCMD_DELIM_TRM,params->valLen)) < 0)
            {
                return ret;
            }
        }
        else
        {
            /* disable scan by set scan interval = 0 */ 
            *(uint32_t *)(params->value) = 0;
        }
    }
    else if (params->type == SL_WLAN_POLICY_PM)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanPmPolicy,sizeof(ATCmd_wlanPmPolicy)/sizeof(StrMpl_List_t),&buff,&params->policy,ATCMD_DELIM_ARG,sizeof(params->policy))) < 0)
        {
            return ret;
        }
        if (params->policy == SL_WLAN_LONG_SLEEP_INTERVAL_POLICY)
        {
            params->valLen = sizeof(SlWlanPmPolicyParams_t); 
            params->value = malloc(params->valLen);
            if (params->value == NULL)
            {
                return -1;
            }
            memset(params->value,0,params->valLen);
            StrMpl_getVal(&buff,&(((SlWlanPmPolicyParams_t *)(params->value))->MaxSleepTimeMs),ATCMD_DELIM_TRM,sizeof(((SlWlanPmPolicyParams_t *)(params->value))->MaxSleepTimeMs) );
        }
    }
    else if (params->type == SL_WLAN_POLICY_P2P)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanP2pRolePolicy,sizeof(ATCmd_wlanP2pRolePolicy)/sizeof(StrMpl_List_t),&buff,&p2pRole,ATCMD_DELIM_ARG,sizeof(p2pRole))) < 0)
        {
            return ret;
        }

        if ((ret = StrMpl_getListVal(ATCmd_wlanP2pNegPolicy,sizeof(ATCmd_wlanP2pNegPolicy)/sizeof(StrMpl_List_t),&buff,&p2pInit,ATCMD_DELIM_TRM,sizeof(p2pInit))) < 0)
        {
            return ret;
        }
            
        params->policy = SL_WLAN_P2P_POLICY(p2pRole,p2pInit);
    }
    return (0);
}


/*!
    \brief          WLAN policy set callback.


    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_policySetCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_Policy_t  params;
    
    memset(&params, 0x0, sizeof(ATCmdWlan_Policy_t));
    /* Call the command parser */
    ret = ATCmdWlan_policySetParse((char *)arg, &params);

    if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_policySetFree(&params);	    
        return -1;
    }

    /* Set policy */
    ret = sl_WlanPolicySet(params.type, params.policy, params.value, params.valLen);
    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdWlan_policySetFree(&params);
    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdWlan_policyGetFree(ATCmdWlan_PolicyGet_t *params)
{    
    if (params != NULL)
    {
        free(params);
    }  
    return 0;
}


/*!
    \brief          Return policy Get result.

    This routine puts profile information on given buffer.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_policyGetResult(void *args, int32_t num, char *buff)
{
    ATCmdWlan_PolicyGet_t  *policyParams = (ATCmdWlan_PolicyGet_t *)(args);
    uint8_t p2pRole;
    uint8_t p2pInit;

    StrMpl_setStr(ATCmd_wlanPolicyGetStr,&buff,ATCMD_DELIM_EVENT);

    /* Policy */
    if (policyParams->type == SL_WLAN_POLICY_CONNECTION)
    {
        StrMpl_setBitmaskListStr(ATCmd_wlanConnPolicy,sizeof(ATCmd_wlanConnPolicy)/sizeof(StrMpl_List_t),&policyParams->policy,&buff, ATCMD_DELIM_TRM,ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_8);
    }
    else if (policyParams->type == SL_WLAN_POLICY_SCAN)
    {
        StrMpl_setListStr(ATCmd_wlanScanPolicy,sizeof(ATCmd_wlanScanPolicy)/sizeof(StrMpl_List_t),&policyParams->policy,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        if ((policyParams->policy == SL_WLAN_SCAN_POLICY(1,1)) || (policyParams->policy == SL_WLAN_SCAN_POLICY(1,0)))
        {                
            if (policyParams->valLen > 0)
            {
                StrMpl_setVal(policyParams->value,&buff,ATCMD_DELIM_TRM,policyParams->valLen | STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            }
        }
    }
    else if (policyParams->type == SL_WLAN_POLICY_PM)
    {
        StrMpl_setListStr(ATCmd_wlanPmPolicy,sizeof(ATCmd_wlanPmPolicy)/sizeof(StrMpl_List_t),&policyParams->policy,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        if (policyParams->policy == SL_WLAN_LONG_SLEEP_INTERVAL_POLICY)
        {
            if (policyParams->valLen > 0)
            {
                StrMpl_setVal(&((SlWlanPmPolicyParams_t *)(policyParams->value))->MaxSleepTimeMs,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16 | STRMPL_FLAG_PARAM_DEC|STRMPL_FLAG_PARAM_UNSIGNED);
            }
        }
    }
    else if (policyParams->type == SL_WLAN_POLICY_P2P)
    {
        p2pRole = ((policyParams->policy >> 0) & 0xF);
        p2pInit = ((policyParams->policy >> 5) & 0x3);
        StrMpl_setListStr(ATCmd_wlanP2pRolePolicy,sizeof(ATCmd_wlanP2pRolePolicy)/sizeof(StrMpl_List_t),&p2pRole,&buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setListStr(ATCmd_wlanP2pNegPolicy,sizeof(ATCmd_wlanP2pNegPolicy)/sizeof(StrMpl_List_t),&p2pInit,&buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
    }

    ATCmdWlan_policyGetFree(policyParams);
    return 0;
}


/*!
    \brief          WLAN policy get callback.


    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_policyGetCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_PolicyGet_t  *params;

    params = malloc(sizeof(ATCmdWlan_PolicyGet_t));
    if (params == NULL)
    {
       ATCmd_errorResult(ATCmd_errorAllocStr,0);
       return -1;
    }
    memset(params, 0x0, sizeof(ATCmdWlan_PolicyGet_t));

    /* Call the command parser */
    /* Type */
    if ((ret = StrMpl_getListVal(ATCmd_wlanPolicy,sizeof(ATCmd_wlanPolicy)/sizeof(StrMpl_List_t),(char **)&arg,&params->type,ATCMD_DELIM_TRM,sizeof(params->type)) ) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_policyGetFree(params);
        return -1;
    }

    /* Get policy */
    params->valLen = sizeof(params->value);
    ret = sl_WlanPolicyGet(params->type, &params->policy, params->value, &params->valLen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdWlan_policyGetFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdWlan_policyGetResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          wlan set mode callback.

    This routine Sets WLAN mode.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_setModeCallback(void *arg)
{
    int32_t ret;
    uint8_t mode;
    
    /* Mode */
    if ((ret = StrMpl_getListVal(ATCmd_wlanRole,sizeof(ATCmd_wlanRole)/sizeof(StrMpl_List_t),(char **)&arg,&mode,0,sizeof(mode)) ) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        return -1;
    }

    ret = sl_WlanSetMode(mode);

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
int32_t ATCmdWlan_setFree(ATCmdWlan_Set_t *params)
{    
    if (params->value != NULL)
    {
        free(params->value);
    }  
    return 0;
}

/*!
    \brief          Parse wlan set command.


    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          params  -   Points to command structure provided by the set callback.
                                       This structure will later be read by the set callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would send error.

*/
int32_t ATCmdWlan_setParse(char *buff, ATCmdWlan_Set_t *setParams)
{
    int32_t ret = 0;
    uint8_t i;

    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_wlanCfg,sizeof(ATCmd_wlanCfg)/sizeof(StrMpl_List_t),&buff,&setParams->id, ATCMD_DELIM_ARG, sizeof(setParams->id) )) < 0)
    {
        return ret;
    }

    /* Option */
    if (setParams->id == SL_WLAN_CFG_AP_ID)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanApCfg,sizeof(ATCmd_wlanApCfg)/sizeof(StrMpl_List_t),&buff,&setParams->option,ATCMD_DELIM_ARG, sizeof(setParams->option))) < 0)
        {
            return ret;
        }
        switch (setParams->option)
        {
            case SL_WLAN_AP_OPT_SSID:
                if ((ret = StrMpl_getAllocStr(&buff, (char **)&(setParams->value), ATCMD_DELIM_TRM, SL_WLAN_SSID_MAX_LENGTH ,ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                setParams->len = strlen((char const *)(setParams->value));
                break;

            case SL_WLAN_AP_OPT_CHANNEL:
            case SL_WLAN_AP_OPT_HIDDEN_SSID:
            case SL_WLAN_AP_OPT_MAX_STATIONS:
            case SL_WLAN_AP_ACCESS_LIST_DEL_IDX:
                setParams->len = 1;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getVal(&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                {
                    return ret;
                }
                
                break;
            case SL_WLAN_AP_OPT_SECURITY_TYPE:
                setParams->len = 1;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getListVal(ATCmd_wlanSecurity,sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t),&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                {
                    return ret;
                }
                
                if ((*(setParams->value)) > SL_WLAN_SEC_TYPE_WPA_WPA2)
                {
                    return -1;                   
                }
                break;
            case SL_WLAN_AP_OPT_PASSWORD:
                if ((ret = StrMpl_getAllocStr(&buff, (char **)&(setParams->value), ATCMD_DELIM_TRM, ATCMDWLAN_MAX_KEY_LEN ,ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                setParams->len = strlen((char const *)(setParams->value));                
                break;
            case SL_WLAN_AP_OPT_MAX_STA_AGING:
                setParams->len = 2;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getVal(&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16)) < 0)
                {
                    return ret;
                }
                break;
            case SL_WLAN_AP_ACCESS_LIST_MODE:
                setParams->len = 1;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getListVal(ATCmd_wlanApAccessList,sizeof(ATCmd_wlanApAccessList)/sizeof(StrMpl_List_t),&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                {
                    return ret;
                }
                break;
                
            case SL_WLAN_AP_ACCESS_LIST_ADD_MAC:
            case SL_WLAN_AP_ACCESS_LIST_DEL_MAC:
                setParams->len = SL_WLAN_BSSID_LENGTH;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getArrayVal(&buff, (void *)(setParams->value), SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
                {
                    return ret;
                }
                break;
            default:
                break;
        }                
    }
    else if (setParams->id == SL_WLAN_CFG_GENERAL_PARAM_ID)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanGenCfg,sizeof(ATCmd_wlanGenCfg)/sizeof(StrMpl_List_t),&buff,&setParams->option,ATCMD_DELIM_ARG, sizeof(setParams->option))) < 0)
        {
            return ret;
        }
        switch (setParams->option)
        {
            case SL_WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE:
                if ((ret = StrMpl_getAllocStr(&buff, (char **)&(setParams->value), ATCMD_DELIM_TRM, 2 ,ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                setParams->len = strlen((char const *)(setParams->value));                
                break;

            case SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER:
            case SL_WLAN_GENERAL_PARAM_OPT_AP_TX_POWER:
                setParams->len = 1;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getVal(&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                {
                    return ret;
                }
                if ((*(setParams->value)) > 15)
                {
                    return -1;                   
                }
                break;
            case SL_WLAN_GENERAL_PARAM_OPT_INFO_ELEMENT:
                setParams->len = sizeof(SlWlanSetInfoElement_t);
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }

                /* index */
                if ((ret = StrMpl_getVal(&buff, &(((SlWlanSetInfoElement_t *)(setParams->value))->Index), ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                {
                    return ret;
                }
                                
                /* Role */
                if ((ret = StrMpl_getListVal(ATCmd_wlanInfoElementRole,sizeof(ATCmd_wlanInfoElementRole)/sizeof(StrMpl_List_t),&buff, &(((SlWlanSetInfoElement_t *)(setParams->value))->Role), ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                {
                    return ret;
                }
                
                /* Info element ID */
                if ((ret = StrMpl_getVal(&buff, &(((SlWlanSetInfoElement_t *)(setParams->value))->IE.Id), ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                {
                    return ret;
                }

                /* Organization unique ID */
                for (i = 0; i < 3; i++)
                {
                    if ((ret = StrMpl_getVal(&buff, &(((SlWlanSetInfoElement_t *)(setParams->value))->IE.Oui[i]), ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                    {
                        return ret;
                    }
                }

                /* Info element */
                if ((ret = StrMpl_getStr(&buff, (void *)(((SlWlanSetInfoElement_t *)(setParams->value))->IE.Data), ATCMD_DELIM_TRM, sizeof(((SlWlanSetInfoElement_t *)(setParams->value))->IE.Data), ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                ((SlWlanSetInfoElement_t *)(setParams->value))->IE.Length = strlen((char const *)(((SlWlanSetInfoElement_t *)(setParams->value))->IE.Data));                

                break;
                
            case SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS:
                setParams->len = sizeof(SlWlanScanParamCommand_t);
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }

                /* channel mask */
                if ((ret = StrMpl_getVal(&buff, &(((SlWlanScanParamCommand_t *)(setParams->value))->ChannelsMask), ATCMD_DELIM_ARG, sizeof((((SlWlanScanParamCommand_t *)(setParams->value))->ChannelsMask)))) < 0)
                {
                    return ret;
                }
                                
                /* RSSI threshold */
                if ((ret = StrMpl_getVal(&buff, &(((SlWlanScanParamCommand_t *)(setParams->value))->RssiThreshold), ATCMD_DELIM_TRM, sizeof(((SlWlanScanParamCommand_t *)(setParams->value))->RssiThreshold) )) < 0)
                {
                    return ret;
                }
                break;
                
            case SL_WLAN_GENERAL_PARAM_OPT_SUSPEND_PROFILES:
            case SL_WLAN_GENERAL_PARAM_DISABLE_ENT_SERVER_AUTH:
                setParams->len = 1;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getVal(&buff, (void *)(setParams->value), ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                {
                    return ret;
                }
                break;
        }
    }
    else if (setParams->id == SL_WLAN_CFG_P2P_PARAM_ID)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanP2pCfg,sizeof(ATCmd_wlanP2pCfg)/sizeof(StrMpl_List_t),&buff,&setParams->option,ATCMD_DELIM_ARG, sizeof(setParams->option))) < 0)
        {
            return ret;
        }
        switch (setParams->option)
        {
            case SL_WLAN_P2P_OPT_DEV_TYPE:
                if ((ret = StrMpl_getAllocStr(&buff, (char **)&(setParams->value), ATCMD_DELIM_TRM, 17 ,ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                setParams->len = strlen((char const *)(setParams->value));                

                break;

            case SL_WLAN_P2P_OPT_CHANNEL_N_REGS:
                setParams->len = 4;
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getArrayVal(&buff, (void *)(setParams->value),setParams->len, ATCMD_DELIM_TRM,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
                {
                    return ret;
                }                
                break;
        }
    }

    else if (setParams->id == SL_WLAN_RX_FILTERS_ID)
    {
        if ((ret = StrMpl_getListVal(ATCmd_wlanRxFilterCfg,sizeof(ATCmd_wlanRxFilterCfg)/sizeof(StrMpl_List_t),&buff,&setParams->option,ATCMD_DELIM_ARG, sizeof(setParams->option))) < 0)
        {
            return ret;
        }
        switch (setParams->option)
        {
            case SL_WLAN_RX_FILTER_STATE:
            case SL_WLAN_RX_FILTER_REMOVE:
                setParams->len = sizeof(SlWlanRxFilterOperationCommandBuff_t);
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getArrayVal(&buff, (void *)((SlWlanRxFilterOperationCommandBuff_t *)(setParams->value))->FilterBitmap,sizeof(SlWlanRxFilterIdMask_t),ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
                {
                    return ret;
                }

                break;
            case SL_WLAN_RX_FILTER_SYS_STATE:
                setParams->len = sizeof(SlWlanRxFilterSysFiltersSetStateBuff_t);
                setParams->value = malloc(setParams->len);
                if (setParams->value == NULL)
                {
                    return -1;
                }
                if ((ret = StrMpl_getArrayVal(&buff, (void *)((SlWlanRxFilterSysFiltersSetStateBuff_t *)(setParams->value))->FilterBitmap,sizeof(SlWlanRxFilterSysFiltersMask_t),ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8,ATCmd_excludeDelimArray)) < 0)
                {
                    return ret;
                }
                break;
        }
    }
    
    return 0;
}

/*!
    \brief          wlan set callback.

    This routine Sets WLAN configurations.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_setCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_Set_t  setParams;
	
    memset(&setParams, 0x0, sizeof(ATCmdWlan_Set_t));

    /* Call the command parser */
    ret = ATCmdWlan_setParse(arg , &setParams);
	
	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_setFree(&setParams);
        return -1;
    }

    /* Set policy */
    ret = sl_WlanSet(setParams.id, setParams.option, setParams.len, setParams.value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    ATCmdWlan_setFree(&setParams);

    return ret;
}


/*!
    \brief          Parse wlan get command.


    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          params  -   Points to command structure provided by the set callback.
                                       This structure will later be read by the set callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would send error.

*/
int32_t ATCmdWlan_getParse(char *buff, ATCmdWlan_Set_t *getParams)
{
    int32_t ret = 0;
    
    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_wlanCfg,sizeof(ATCmd_wlanCfg)/sizeof(StrMpl_List_t),&buff,&getParams->id, ATCMD_DELIM_ARG, sizeof(getParams->id) )) < 0)
    {
        return ret;
    }

    switch (getParams->id)
    {
        case SL_WLAN_CFG_AP_ID:
            if ((ret = StrMpl_getListVal(ATCmd_wlanApCfg,sizeof(ATCmd_wlanApCfg)/sizeof(StrMpl_List_t),&buff,&getParams->option, ATCMD_DELIM_TRM, sizeof(getParams->option) )) < 0)
            {
                return ret;
            }
            switch (getParams->option)
            {
                case SL_WLAN_AP_OPT_SSID:
                    getParams->len = SL_WLAN_SSID_MAX_LENGTH + 1;
                    break;
                case SL_WLAN_AP_OPT_CHANNEL:
                case SL_WLAN_AP_OPT_HIDDEN_SSID:
                case SL_WLAN_AP_OPT_SECURITY_TYPE:
                case SL_WLAN_AP_OPT_MAX_STATIONS:
                case SL_WLAN_AP_ACCESS_LIST_NUM_ENTRIES:
                    getParams->len = 1;
                    break;
                case SL_WLAN_AP_OPT_PASSWORD:
                    getParams->len = ATCMDWLAN_MAX_KEY_LEN;
                    break;
                case SL_WLAN_AP_OPT_MAX_STA_AGING:
                    getParams->len = 2;
                    break;
                default:
                    return -1;
            }
            
            break;
        case SL_WLAN_CFG_GENERAL_PARAM_ID:
            if ((ret = StrMpl_getListVal(ATCmd_wlanGenCfg,sizeof(ATCmd_wlanGenCfg)/sizeof(StrMpl_List_t),&buff,&getParams->option, ATCMD_DELIM_TRM, sizeof(getParams->option))) < 0)
            {
                return ret;
            }
            switch (getParams->option)
            {
                case SL_WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE:
                    getParams->len = 3;
                    break;

                case SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER:
                case SL_WLAN_GENERAL_PARAM_OPT_AP_TX_POWER:
                    getParams->len = sizeof(int32_t);
                    break;
                    
                case SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS:
                    getParams->len = sizeof(SlWlanScanParamCommand_t);
                    break;                    
                default:
                    return -1;
            }
            break;
        case SL_WLAN_CFG_P2P_PARAM_ID:
            if ((ret = StrMpl_getListVal(ATCmd_wlanP2pCfg,sizeof(ATCmd_wlanP2pCfg)/sizeof(StrMpl_List_t),&buff,&getParams->option, ATCMD_DELIM_TRM, sizeof(getParams->option))) < 0)
            {
                return ret;
            }
            switch (getParams->option)
            {
                case SL_WLAN_P2P_OPT_DEV_TYPE:
                    getParams->len = 18;
                    break;
                case SL_WLAN_P2P_OPT_CHANNEL_N_REGS:
                    getParams->len = 4;
                    break;
                default:
                    return -1;
            }
            break;
        case SL_WLAN_CFG_AP_ACCESS_LIST_ID:
            if ((ret = StrMpl_getVal(&buff,&getParams->option, ATCMD_DELIM_TRM, sizeof(getParams->option) )) < 0)
            {
                return ret;
            }
            getParams->len = SL_WLAN_MAX_ACCESS_LIST_STATIONS * SL_WLAN_BSSID_LENGTH;
           break;
        case SL_WLAN_RX_FILTERS_ID:
            if ((ret = StrMpl_getListVal(ATCmd_wlanRxFilterCfg,sizeof(ATCmd_wlanRxFilterCfg)/sizeof(StrMpl_List_t),&buff,&getParams->option, ATCMD_DELIM_TRM, sizeof(getParams->option))) < 0)
            {
                return ret;
            }
            switch (getParams->option)
            {
                case SL_WLAN_RX_FILTER_STATE:
                    getParams->len = sizeof(SlWlanRxFilterIdMask_t);
                    break;
                case SL_WLAN_RX_FILTER_SYS_STATE:
                    getParams->len = sizeof(SlWlanRxFilterSysFiltersMask_t);
                    break;
                default:
                    return -1;
            }
            
            break;
        case SL_WLAN_CONNECTION_INFO:
            getParams->len = sizeof(SlWlanConnStatusParam_t);
            break;
        default:
            break;
    }
    
    getParams->value = malloc(getParams->len);
    if (getParams->value == NULL)
    {
        return -1;
    }
    return 0;
}

/*!
    \brief          free wlan get command allocations.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdWlan_getFree(ATCmdWlan_Set_t *getParams)
{    
    if (getParams != NULL)
    {
        if (getParams->value!= NULL)
        {
            free (getParams->value);
        }
        free (getParams);
    }
    return 0;
}

/*!
    \brief          Return WLAN Get result.

    This routine puts profile information on given buffer.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_getResult(void *args, int32_t num, char *buff)
{
    int32_t ret = 0;
    ATCmdWlan_Set_t  *getParams = (ATCmdWlan_Set_t *)(args);
    uint8_t numEntries;
    uint8_t i;
    char delim;
    void *bssid = NULL;

    StrMpl_setStr(ATCmd_wlanGetStr,&buff,ATCMD_DELIM_EVENT);

    if (getParams->id == SL_WLAN_CFG_AP_ID)
    {
        switch (getParams->option)
        {
            case SL_WLAN_AP_OPT_SSID:
            case SL_WLAN_AP_OPT_PASSWORD:
                StrMpl_setStr((char *)getParams->value,&buff,ATCMD_DELIM_TRM);
                break;
            case SL_WLAN_AP_OPT_CHANNEL:
            case SL_WLAN_AP_OPT_HIDDEN_SSID:
            case SL_WLAN_AP_OPT_MAX_STATIONS:
            case SL_WLAN_AP_ACCESS_LIST_NUM_ENTRIES:
            case SL_WLAN_AP_OPT_MAX_STA_AGING:
                StrMpl_setVal(getParams->value,&buff,ATCMD_DELIM_TRM,getParams->len|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                break;
            case SL_WLAN_AP_OPT_SECURITY_TYPE:
                StrMpl_setListStr(ATCmd_wlanSecurity, sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t), getParams->value,&buff,ATCMD_DELIM_TRM,getParams->len|STRMPL_FLAG_PARAM_UNSIGNED);
                break;
            default:
                break;
        }                
    }
    else if (getParams->id == SL_WLAN_CFG_GENERAL_PARAM_ID)
    {
        switch (getParams->option)
        {
            case SL_WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE:
                StrMpl_setStr((char *)getParams->value,&buff,ATCMD_DELIM_TRM);
                break;

            case SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER:
            case SL_WLAN_GENERAL_PARAM_OPT_AP_TX_POWER:
                StrMpl_setVal(getParams->value,&buff,ATCMD_DELIM_TRM,getParams->len|STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_DEC);
                break;
                
            case SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS:
                StrMpl_setVal(&((SlWlanScanParamCommand_t *)(getParams->value))->ChannelsMask,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_HEX);
                StrMpl_setVal(&((SlWlanScanParamCommand_t *)(getParams->value))->RssiThreshold,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32|STRMPL_FLAG_PARAM_SIGNED|STRMPL_FLAG_PARAM_HEX);
                break;                    
        }
    }
    else if (getParams->id == SL_WLAN_CFG_P2P_PARAM_ID)
    {
        switch (getParams->option)
        {
            case SL_WLAN_P2P_OPT_DEV_TYPE:
                StrMpl_setStr((char *)getParams->value,&buff,ATCMD_DELIM_TRM);
                break;
              
            case SL_WLAN_P2P_OPT_CHANNEL_N_REGS:
                for (i = 0; i < 4; i++)
                {
                    if (i < 3)
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    }
                    else
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
                    }
                }
                break;                    
        }
    }
    else if (getParams->id == SL_WLAN_CFG_AP_ACCESS_LIST_ID)
    {
        numEntries = getParams->len / SL_WLAN_BSSID_LENGTH;
        delim = ATCMD_DELIM_LIST;
        for (i=0; i<numEntries; i++)
        {
            if (i == (numEntries - 1))
            {
                delim = ATCMD_DELIM_TRM;
            }
            StrMpl_setArrayVal(&getParams->value[i * SL_WLAN_BSSID_LENGTH],&buff,SL_WLAN_BSSID_LENGTH,delim,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        }
    }
    else if (getParams->id == SL_WLAN_RX_FILTERS_ID)
    {
        switch (getParams->option)
        {
            case SL_WLAN_RX_FILTER_STATE:
                for (i = 0; i < 16; i++)
                {
                    if (i < 15)
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_HEX);
                    }
                    else
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_HEX);
                    }
                }
                break;
              
            case SL_WLAN_RX_FILTER_SYS_STATE:
                for (i = 0; i < 4; i++)
                {
                    if (i < 3)
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_HEX);
                    }
                    else
                    {
                        StrMpl_setVal(&getParams->value[i],&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_HEX);
                    }
                }
                break;                    
        }
    }
    else if (getParams->id == SL_WLAN_CONNECTION_INFO)
    {
        StrMpl_setListStr(ATCmd_wlanRole, sizeof(ATCmd_wlanRole)/sizeof(StrMpl_List_t), &((SlWlanConnStatusParam_t *)getParams->value)->Mode,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setListStr(ATCmd_wlanConnStatus, sizeof(ATCmd_wlanConnStatus)/sizeof(StrMpl_List_t), &((SlWlanConnStatusParam_t *)getParams->value)->ConnStatus,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        if (((SlWlanConnStatusParam_t *)getParams->value)->ConnStatus == SL_WLAN_DISCONNECTED)
        {
            StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
        }
        else
        {
            StrMpl_setListStr(ATCmd_wlanSecurity, sizeof(ATCmd_wlanSecurity)/sizeof(StrMpl_List_t), &((SlWlanConnStatusParam_t *)getParams->value)->SecType,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8|STRMPL_FLAG_PARAM_UNSIGNED);
        }
        StrMpl_setStr((char *)(((SlWlanConnStatusParam_t *)getParams->value)->ConnectionInfo.StaConnect.SsidName),&buff,ATCMD_DELIM_ARG);   
        if (((SlWlanConnStatusParam_t *)getParams->value)->ConnectionInfo.StaConnect.SsidLen > 0)
        {
            bssid = (void *)(((SlWlanConnStatusParam_t *)getParams->value)->ConnectionInfo.StaConnect.Bssid);
        }
        if (((SlWlanConnStatusParam_t *)getParams->value)->Mode == ROLE_P2P)
        {
            StrMpl_setArrayVal(bssid ,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_ARG,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            StrMpl_setStr((char *)(((SlWlanConnStatusParam_t *)getParams->value)->ConnectionInfo.P2PConnect.GoDeviceName),&buff,ATCMD_DELIM_TRM);
        }
        else
        {
            StrMpl_setArrayVal(bssid,&buff,SL_WLAN_BSSID_LENGTH,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        }
    }
    else
    {
        ret = -1;
    }
    
    ATCmdWlan_getFree(getParams);
    return ret;
}


/*!
    \brief          wlan get callback.

    This routine Gets WLAN configurations.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdWlan_getCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdWlan_Set_t  *params;
	
    params = malloc(sizeof(ATCmdWlan_Set_t));
    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;
    }
    memset(params, 0x0, sizeof(ATCmdWlan_Set_t));

    /* Call the command parser */
    ret = ATCmdWlan_getParse(arg , params);
	
	if(ret < 0)
	{
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdWlan_getFree(params);
        return -1;
    }

    /* wlan set */
    ret = sl_WlanGet(params->id, &params->option, &params->len, params->value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdWlan_getFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdWlan_getResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

