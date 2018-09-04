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

#include <ti/net/atcmd/atcmd_netutil.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDNETUTIL_TRUE_RANDOM_MAX_BUFF_LEN       (172)
#define ATCMDNETUTIL_PUBLIC_KEY_MAX_BUFF_LEN        (255)
//*****************************************************************************
// typedefs
//*****************************************************************************

typedef struct _ATCmdNetUtil_Get_t_
{
    uint16_t 	option;
    uint32_t 	id;
    uint8_t 	format;
    uint16_t 	len; 
    uint8_t     *value;
}ATCmdNetUtil_Get_t;

typedef struct _ATCmdNetUtil_Cmd_t_
{
    uint16_t 	cmd;
    uint8_t     *attrib;
    uint16_t 	attribLen; 
    uint8_t     inValueFormat;
    uint8_t     *inValue;
    uint16_t 	inValueLen; 
    uint8_t     *outValue;
    uint16_t 	outValueLen; 
}ATCmdNetUtil_Cmd_t;

//*****************************************************************************
// AT Command Net Crypto Utility Routines
//*****************************************************************************

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetUtil_cmdFree(ATCmdNetUtil_Cmd_t *params)
{    
    if (params != NULL)
    {
        if (params->attrib != NULL)
        {
            free(params->attrib);
        }
        if (params->inValue != NULL)
        {
            free(params->inValue);
        }
        if (params->outValue != NULL)
        {
            free(params->outValue);
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
int32_t ATCmdNetUtil_cmdParse(char *buff, ATCmdNetUtil_Cmd_t *params)
{
    int32_t ret = 0;
    uint16_t inputDataLen;
    uint16_t inputSigLen;
    uint8_t optional = 1;

    /* option */
    if ((ret = StrMpl_getListVal(ATCmd_netUtilCmd, sizeof(ATCmd_netUtilCmd)/sizeof(StrMpl_List_t), &buff, &params->cmd, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    /* values */
    switch (params->cmd)
    {
        case SL_NETUTIL_CRYPTO_CMD_SIGN_MSG:
            params->attribLen = sizeof(SlNetUtilCryptoCmdSignAttrib_t);
            params->attrib = malloc(params->attribLen);
            params->outValueLen = ATCMDNETUTIL_PUBLIC_KEY_MAX_BUFF_LEN;
            params->outValue = malloc(params->outValueLen);
            if ((params->attrib == NULL) || (params->outValue == NULL))
            {
                return -1;
            }
            /* key index */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdSignAttrib_t *)(params->attrib))->ObjId, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            ((SlNetUtilCryptoCmdSignAttrib_t *)(params->attrib))->Flags = 0;
            ((SlNetUtilCryptoCmdSignAttrib_t *)(params->attrib))->SigType = SL_NETUTIL_CRYPTO_SIG_SHAwECDSA; /* this is the only type supported */

            /* data in format */
            if ((ret = StrMpl_getVal(&buff, &params->inValueFormat , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            
            /* data length */
            if ((ret = StrMpl_getVal(&buff, &params->inValueLen, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
        	    return ret;
            }
            /* data */
            if (params->inValueFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert length to binary length */
                inputDataLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->inValueLen); 
                params->inValue = malloc(inputDataLen);
                if (params->inValue == NULL)
                {
                    return -1;
                }
                if (StrMpl_decodeBase64((uint8_t *)buff, params->inValueLen, params->inValue, (uint32_t *)&inputDataLen) < 0)
                {
                    return -1;
                }
                params->inValueLen = inputDataLen;
            }
            else
            {
                params->inValue = malloc(params->inValueLen);
                if (params->inValue == NULL)
                {
                    return -1;
                }

                memcpy(params->inValue, buff, params->inValueLen);
            }
            
            break;
        case SL_NETUTIL_CRYPTO_CMD_VERIFY_MSG:
            params->attribLen = sizeof(SlNetUtilCryptoCmdVerifyAttrib_t);
            params->attrib = malloc(params->attribLen);
            params->outValueLen = 4;
            params->outValue = malloc(params->outValueLen);
            if ((params->attrib == NULL) || (params->outValue == NULL))
            {
                return -1;
            }
            /* key index */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->ObjId, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->Flags = 0;
            ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->SigType = SL_NETUTIL_CRYPTO_SIG_SHAwECDSA; /* this is the only type supported */

            /* data in format */
            if ((ret = StrMpl_getVal(&buff, &params->inValueFormat , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            
            /* data length */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->MsgLen, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
        	    return ret;
            }
            /* signature length */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->SigLen, ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
        	    return ret;
            }
            /* convert length to binary length */
            if (params->inValueFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                inputDataLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->MsgLen);
                inputSigLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->SigLen);
            }
            else
            {
                inputDataLen = ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->MsgLen;
                inputSigLen = ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->SigLen;              
            }
            /* allocate the proper buffer */
            params->inValueLen = inputDataLen + inputSigLen;
            params->inValue = malloc(params->inValueLen);              
            if (params->inValue == NULL)
            {
                return -1;
            }
            /* data and signature */
            if (params->inValueFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                if (StrMpl_decodeBase64((uint8_t *)(buff) , ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->MsgLen, params->inValue, (uint32_t *)&inputDataLen) < 0)
                {
                    return -1;
                }
                if (StrMpl_decodeBase64((uint8_t *)(buff + ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->MsgLen), ((SlNetUtilCryptoCmdVerifyAttrib_t *)(params->attrib))->SigLen, params->inValue + inputDataLen, (uint32_t *)&inputSigLen) < 0)
                {
                    return -1;
                }
            }
            else
            {
                memcpy(params->inValue, buff, inputDataLen + inputSigLen);
            }
            break;
        case SL_NETUTIL_CRYPTO_CMD_TEMP_KEYS:
            params->attribLen = sizeof(SlNetUtilCryptoCmdKeyMgnt_t);
            params->attrib = malloc(params->attribLen);
            if (params->attrib == NULL)
            {
                return -1;
            }
            /* key index */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->ObjId, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            /* action */
            if ((ret = StrMpl_getListVal(ATCmd_netUtilTempKeys, sizeof(ATCmd_netUtilTempKeys)/sizeof(StrMpl_List_t), &buff, &((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->SubCmd, ATCMD_DELIM_TRM, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            break;
        case SL_NETUTIL_CRYPTO_CMD_INSTALL_OP:
            params->attribLen = sizeof(SlNetUtilCryptoCmdKeyMgnt_t);
            params->attrib = malloc(params->attribLen);
            if (params->attrib == NULL)
            {
                return -1;
            }
            /* key index */
            if ((ret = StrMpl_getVal(&buff, &((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->ObjId, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
            {
                return ret;
            }
            /* action */
            if ((ret = StrMpl_getListVal(ATCmd_netUtilInstallOp, sizeof(ATCmd_netUtilInstallOp)/sizeof(StrMpl_List_t), &buff, &((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->SubCmd, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
            {
                return ret;
            }
            if (((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->SubCmd == SL_NETUTIL_CRYPTO_INSTALL_SUB_CMD)
            {
                params->inValue = malloc(sizeof(SlNetUtilCryptoPubKeyInfo_t) + SL_FS_MAX_FILE_NAME_LENGTH + SL_FS_MAX_FILE_NAME_LENGTH);
                if (params->inValue == NULL)
                {
                    return -1;
                }
                /* key algo */
                if ((ret = StrMpl_getListVal(ATCmd_netUtilPubKeyAlgo, sizeof(ATCmd_netUtilPubKeyAlgo)/sizeof(StrMpl_List_t), &buff, &((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->KeyAlgo, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8 )) < 0)
                {
                    return ret;
                }
                if (((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->KeyAlgo == SL_NETUTIL_CRYPTO_PUB_KEY_ALGO_EC)
                {
                    ((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->KeyParams.EcParams.CurveType = SL_NETUTIL_CRYPTO_EC_CURVE_TYPE_NAMED; /* custom is not supported yet */
                    optional = 0;
                }
                /* EC Named Curve identifier */
                if ((ret = StrMpl_getListVal(ATCmd_netUtilEcNamedCurve, sizeof(ATCmd_netUtilEcNamedCurve)/sizeof(StrMpl_List_t), &buff, &((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->KeyParams.EcParams.CurveParams.NamedCurveParams, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_8)) < 0)
                {
                    if ((ret != STRMPL_ERROR_PARAM_MISSING) || (optional == 0))
                    {
                        return ret;
                    }
                }
                /* cert file nmae */
                if ((ret = StrMpl_getStr(&buff, (void *)(params->inValue + sizeof(SlNetUtilCryptoPubKeyInfo_t)), ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH, ATCmd_excludeDelimStr)) < 0)
                {
                    if (ret != STRMPL_ERROR_PARAM_MISSING)
                    {
                        return ret;
                    }
                }
                ((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->CertFileNameLen = strlen((char *)params->inValue + sizeof(SlNetUtilCryptoPubKeyInfo_t));

                /* key file name */
                if ((ret = StrMpl_getStr(&buff, (void *)(params->inValue + sizeof(SlNetUtilCryptoPubKeyInfo_t) + ((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->CertFileNameLen), ATCMD_DELIM_TRM, SL_FS_MAX_FILE_NAME_LENGTH,ATCmd_excludeDelimStr)) < 0)
                {
                    return ret;
                }
                ((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->KeyFileNameLen = strlen((char *)params->inValue + sizeof(SlNetUtilCryptoPubKeyInfo_t) + ((SlNetUtilCryptoPubKeyInfo_t *)(params->inValue))->CertFileNameLen);
            }
            else if (((SlNetUtilCryptoCmdKeyMgnt_t *)(params->attrib))->SubCmd == SL_NETUTIL_CRYPTO_UNINSTALL_SUB_CMD)
            {
                /* all params are ignored */
            }
            else
            {
                return -1;
            }
            break;
        default:
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
int32_t ATCmdNetUtil_cmdResult(void *args, int32_t num, char *buff)
{    
    int32_t ret = 0;
    ATCmdNetUtil_Cmd_t *params = (ATCmdNetUtil_Cmd_t *)args;
    uint16_t dataLen = params->outValueLen;

    StrMpl_setStr(ATCmd_netUtilCmdStr,&buff,ATCMD_DELIM_EVENT);

    switch (params->cmd)
    {
        case SL_NETUTIL_CRYPTO_CMD_SIGN_MSG:
            /* format */
            StrMpl_setVal(&params->inValueFormat, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            if (params->inValueFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert length from binary to base64 */
                dataLen = StrMpl_getBase64EncBufSize(params->outValueLen);
            }
            /* signature length */
            StrMpl_setVal(&dataLen, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

            /* signature */
            if (params->inValueFormat == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert data to base64 */
                StrMpl_encodeBase64(params->outValue, params->outValueLen, (uint8_t *)buff, (uint32_t *)&dataLen);
            }
            else
            {
                memcpy(buff,params->outValue,params->outValueLen);
            }
            break;
        case SL_NETUTIL_CRYPTO_CMD_VERIFY_MSG:
            StrMpl_setVal(params->outValue, &buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            break;
        default:
            ret = -1;
    }
    ATCmdNetUtil_cmdFree(params);
    return ret;
}

/*!
    \brief          Net Utility CMD callback.

    This routine activates a network utility-related command

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetUtil_cmdCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetUtil_Cmd_t *params;

    params = malloc(sizeof(ATCmdNetUtil_Cmd_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetUtil_Cmd_t));
    
    /* Call the command parser */
    ret = ATCmdNetUtil_cmdParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetUtil_cmdFree(params);
	    return -1;
    }
    
    /* set netcfg option */
    ret = sl_NetUtilCmd(params->cmd, params->attrib  ,params->attribLen,params->inValue,params->inValueLen,params->outValue,&params->outValueLen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetUtil_cmdFree(params);
    }
    else
    {
        if ((params->cmd == SL_NETUTIL_CRYPTO_CMD_SIGN_MSG) ||(params->cmd == SL_NETUTIL_CRYPTO_CMD_VERIFY_MSG))
        {
            ATCmd_commandResult(ATCmdNetUtil_cmdResult,params,0);
        }
        else
        {
            ATCmdNetUtil_cmdFree(params);
        }
        ATCmd_okResult();
    }

    return ret;
}


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdNetUtil_getFree(ATCmdNetUtil_Get_t *params)
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
int32_t ATCmdNetUtil_getParse(char *buff, ATCmdNetUtil_Get_t *params)
{
    int32_t ret = 0;
    
    /* option */
    if ((ret = StrMpl_getListVal(ATCmd_netUtilGet, sizeof(ATCmd_netUtilGet)/sizeof(StrMpl_List_t), &buff, &params->option, ATCMD_DELIM_ARG, STRMPL_FLAG_PARAM_SIZE_16 )) < 0)
    {
        return ret;
    }

    /* value */
    switch (params->option)
    {
        case SL_NETUTIL_CRYPTO_PUBLIC_KEY:
            /* data format */
            if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            params->id = 1;
            params->len = ATCMDNETUTIL_PUBLIC_KEY_MAX_BUFF_LEN;
            break;
        case SL_NETUTIL_TRUE_RANDOM:
            /* number of random numbers */
            if ((ret = StrMpl_getVal(&buff, &params->len , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
            {
                return ret;
            }
            params->id = 0;
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
int32_t ATCmdNetUtil_getResult(void *args, int32_t num, char *buff)
{    
    int32_t ret = 0;
    ATCmdNetUtil_Get_t *params = (ATCmdNetUtil_Get_t *)args;
    uint16_t dataLen = params->len;

    StrMpl_setStr(ATCmd_netUtilGetStr,&buff,ATCMD_DELIM_EVENT);

    switch (params->option)
    {
        case SL_NETUTIL_CRYPTO_PUBLIC_KEY:
            /* format */
            StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);
            if (params->format == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert length from binary to base64 */
                dataLen = StrMpl_getBase64EncBufSize(params->len);
            }
            /* length */
            StrMpl_setVal(&dataLen, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

            if (params->format == ATCMD_DATA_FORMAT_BASE64)
            {
                /* convert data to base64 */
                StrMpl_encodeBase64(params->value, params->len, (uint8_t *)buff, (uint32_t *)&dataLen);
            }
            else
            {
                memcpy(buff,params->value,params->len);
            }
            break;
        case SL_NETUTIL_TRUE_RANDOM:
            StrMpl_setArrayVal(params->value,&buff,params->len,ATCMD_DELIM_TRM,ATCMD_DELIM_ARRAY,STRMPL_FLAG_PARAM_HEX | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
            break;
        default:
            ret = -1;
    }
    ATCmdNetUtil_getFree(params);
    return ret;
}

/*!
    \brief          Net Utility Get callback.

    This routine retrieves configurations of a network utility

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdNetUtil_getCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdNetUtil_Get_t *params;

    params = malloc(sizeof(ATCmdNetUtil_Get_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdNetUtil_Get_t));
    
    /* Call the command parser */
    ret = ATCmdNetUtil_getParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdNetUtil_getFree(params);
	    return -1;
    }
    
    /* set netapp option */
    ret = sl_NetUtilGet(params->option, params->id, params->value, &params->len);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdNetUtil_getFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdNetUtil_getResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

