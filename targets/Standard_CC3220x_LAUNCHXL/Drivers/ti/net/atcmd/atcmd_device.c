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

#include <ti/net/atcmd/atcmd_device.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDDEV_IOT_UDID_SIZE  (16)
//*****************************************************************************
// typedefs
//*****************************************************************************

typedef struct _ATCmdDev_t_
{
    uint8_t     id;
    uint8_t     option;
    uint16_t    len;
    uint8_t     *value;
}ATCmdDev_t;

//*****************************************************************************
// AT Command device Routines
//*****************************************************************************


/*!
    \brief          Device start callback.

    This routine starts the NetWork Process

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdDev_startCallback(void *arg)
{
    int32_t ret = 0;
    
    /* start NWP */
    ret = sl_Start(0,0,0);

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
    \brief          device stop callback.

    This routine stops the NetWork Process

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdDev_stopCallback(void *arg)
{
    int32_t ret = 0;
    uint16_t timeout;

    timeout = 0x0;
    
    /* Call the command parser */
    if ((ret = StrMpl_getVal((char **)&arg, &timeout, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16)) < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
	    return ret;
    }

    /* stop nwp */
    ret = sl_Stop(timeout);

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
    \brief          test callback.

    This routine only return OK as                                                                                                

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdDev_testCallback(void *arg)
{
    int32_t ret = 0;
    
    ATCmd_okResult();

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdDev_setFree(ATCmdDev_t *params)
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
int32_t ATCmdDev_setParse(char *buff, ATCmdDev_t *params)
{
    int32_t ret = 0;
    
    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_devCfgId, sizeof(ATCmd_devCfgId)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }

    /* option */
    switch (params->id)
    {
        case SL_DEVICE_GENERAL:
            if ((ret = StrMpl_getListVal(ATCmd_devGeneralOptions, sizeof(ATCmd_devGeneralOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_GENERAL_PERSISTENT:
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

                case SL_DEVICE_GENERAL_DATE_TIME:
                    params->len = sizeof(SlDateTime_t);
                    params->value = malloc(params->len);
                    if (params->value == NULL)
                    {
                        return -1;
                    }
                    /* hour */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_hour, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                	    return ret;
                    }
                    /* minutes */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_min, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                        return ret;
                    }    
                    /* seconds */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_sec, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                	    return ret;
                    }

                    /* day */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_day, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                        return ret;
                    }
                    /* month */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_mon, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                	    return ret;
                    }
                    /* year */
                    if ((ret = StrMpl_getVal(&buff, &((SlDateTime_t *)(params->value))->tm_year, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
                    {
                        return ret;
                    }
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
    \brief          Device Set callback.

    This routine sets device configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdDev_setCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdDev_t params;

    memset(&params, 0x0, sizeof(ATCmdDev_t));
    
    /* Call the command parser */
    ret = ATCmdDev_setParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdDev_setFree(&params);
	    return -1;
    }
    
    /* set device option */
    ret = sl_DeviceSet(params.id,params.option,params.len,params.value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }

    ATCmdDev_setFree(&params);
    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdDev_getFree(ATCmdDev_t *params)
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
int32_t ATCmdDev_getParse(char *buff, ATCmdDev_t *params)
{
    int32_t ret = 0;
    
    /* ID */
    if ((ret = StrMpl_getListVal(ATCmd_devCfgId, sizeof(ATCmd_devCfgId)/sizeof(StrMpl_List_t), &buff, &params->id, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
    {
        return ret;
    }

    /* option */
    switch (params->id)
    {
        case SL_DEVICE_GENERAL:
            if ((ret = StrMpl_getListVal(ATCmd_devGeneralOptions, sizeof(ATCmd_devGeneralOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_GENERAL_VERSION:
                    params->len = sizeof(SlDeviceVersion_t);
                    break;
                case SL_DEVICE_GENERAL_PERSISTENT:
                    params->len = sizeof(uint8_t);
                    break;
                case SL_DEVICE_GENERAL_DATE_TIME:
                    params->len = sizeof(SlDateTime_t);
                    break;
                default:
                    return -1;
            }
            break;

        case SL_DEVICE_STATUS:
            if ((ret = StrMpl_getListVal(ATCmd_devStatusOptions, sizeof(ATCmd_devStatusOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_EVENT_CLASS_DEVICE:
                case SL_DEVICE_EVENT_CLASS_WLAN:
                case SL_DEVICE_EVENT_CLASS_BSD:
                case SL_DEVICE_EVENT_CLASS_NETAPP:
                    params->len = sizeof(uint32_t);
                    break;
                default:
                    return -1;
            }
            break;

        case SL_DEVICE_IOT:
            if ((ret = StrMpl_getListVal(ATCmd_devIotOptions, sizeof(ATCmd_devIotOptions)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
            {
                return ret;
            }
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_IOT_UDID:
                    params->len = sizeof(uint8_t) * 16;
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
int32_t ATCmdDev_getResult(void *args, int32_t num, char *buff)
{    
    int32_t ret = 0;
    ATCmdDev_t *params = (ATCmdDev_t *)args;
    
    StrMpl_setStr(ATCmd_devGetStr,&buff,ATCMD_DELIM_EVENT);

    switch (params->id)
    {
        case SL_DEVICE_GENERAL:
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_GENERAL_VERSION:
                    /* chip id */
                    StrMpl_setVal(&((SlDeviceVersion_t *)(params->value))->ChipId, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_HEX);
                    /* FW version */
                    StrMpl_setArrayVal(((SlDeviceVersion_t *)(params->value))->FwVersion, &buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    /* PHY version */
                    StrMpl_setArrayVal(((SlDeviceVersion_t *)(params->value))->PhyVersion, &buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    /* NWP version */
                    StrMpl_setArrayVal(((SlDeviceVersion_t *)(params->value))->NwpVersion, &buff,4,ATCMD_DELIM_ARG,ATCMD_DELIM_INTER,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    /* ROM version */
                    StrMpl_setVal(&((SlDeviceVersion_t *)(params->value))->RomVersion, &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_16|STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    break;
                case SL_DEVICE_GENERAL_PERSISTENT: 
                    StrMpl_setVal(params->value, &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    break;
                case SL_DEVICE_GENERAL_DATE_TIME:
                    /* hour */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_hour, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    /* minutes */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_min, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    /* seconds */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_sec, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    /* day */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_day, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                     /* month */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_mon, &buff, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    /* year */
                    StrMpl_setVal(&((SlDateTime_t *)(params->value))->tm_year, &buff, ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED | STRMPL_FLAG_PARAM_DEC);
                    break;

                default:
                    ret = -1;
            }
            break;

        case SL_DEVICE_STATUS:

            /* value */
            switch (params->option)
            {
                case SL_DEVICE_EVENT_CLASS_DEVICE:
                    StrMpl_setBitmaskListStr(ATCmd_devStatusDevice, sizeof(ATCmd_devStatusDevice)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
                    break;
                case SL_DEVICE_EVENT_CLASS_WLAN:
                    StrMpl_setBitmaskListStr(ATCmd_devStatusWlan, sizeof(ATCmd_devStatusWlan)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
                    break;
                case SL_DEVICE_EVENT_CLASS_BSD:
                    StrMpl_setBitmaskListStr(ATCmd_devStatusBsd, sizeof(ATCmd_devStatusBsd)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
                    break;
                case SL_DEVICE_EVENT_CLASS_NETAPP:
                    StrMpl_setBitmaskListStr(ATCmd_devStatusNetapp, sizeof(ATCmd_devStatusNetapp)/sizeof(StrMpl_List_t), params->value, &buff, ATCMD_DELIM_TRM, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
                    break;
                default:
                    ret = -1;
            }                  
            break;

        case SL_DEVICE_IOT:
            /* value */
            switch (params->option)
            {
                case SL_DEVICE_IOT_UDID:
                    StrMpl_setArrayVal(params->value, &buff,ATCMDDEV_IOT_UDID_SIZE,ATCMD_DELIM_TRM,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
                    break;
                default:
                    ret = -1;
            }                  
            break;

        default:
            ret = -1;
    }
    ATCmdDev_getFree(params);

    return ret;
}

/*!
    \brief          Device Get callback.

    This routine gets device configurations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdDev_getCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdDev_t *params;

    params = malloc(sizeof(ATCmdDev_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdDev_t));
    
    /* Call the command parser */
    ret = ATCmdDev_getParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdDev_getFree(params);
	    return -1;
    }
    
    /* get device option */
    ret = sl_DeviceGet(params->id,&params->option,&params->len,params->value);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdDev_getFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdDev_getResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

