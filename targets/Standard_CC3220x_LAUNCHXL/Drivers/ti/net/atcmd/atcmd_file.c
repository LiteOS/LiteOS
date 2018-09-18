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

#include <ti/net/atcmd/atcmd_file.h>
#include <ti/net/atcmd/atcmd_defs.h>
#include <ti/net/atcmd/atcmd_event.h>
#include <ti/net/atcmd/atcmd_gen.h>
#include <ti/net/atcmd/atcmd.h>
#include "unistd.h"

//*****************************************************************************
// defines
//*****************************************************************************
#define ATCMDFILE_SHA256_LEN        (32)
#define ATCMDFILE_MAX_FILES_ENTRIES (6)
//*****************************************************************************
// typedefs
//*****************************************************************************
typedef struct _ATCmdFile_Open_t_
{
    int32_t id;
    char *filename;
    uint32_t mode;
    uint32_t token;
    uint32_t filesize;
}ATCmdFile_Open_t;

typedef struct _ATCmdFile_Close_t_
{
    int32_t id;
    char *certFilename;
    char *signature;
    uint32_t signatureLen;
}ATCmdFile_Close_t;

typedef struct _ATCmdFile_Ctl_t_
{
    SlFsCtl_e command;
    uint32_t token;
    uint8_t *filename;
    uint8_t *data;
    uint16_t dataLen;
    uint8_t *outputData;
    uint16_t OutputDataLen;
    uint32_t newToken;
}ATCmdFile_Ctl_t;

typedef struct _ATCmdFile_Del_t_
{
    uint8_t *filename;
    uint32_t token;
}ATCmdFile_Del_t;

typedef struct _ATCmdFile_FileListEntry_t_
{
		SlFileAttributes_t attribute;
		char fileName[SL_FS_MAX_FILE_NAME_LENGTH];
}ATCmdFile_FileListEntry_t;

typedef struct _ATCmdFile_GetFileList_t_
{
    int32_t index;
    uint8_t maxEntries;
    uint8_t maxEntryLen;
    ATCmdFile_FileListEntry_t  *entry;
}ATCmdFile_GetFileList_t;

typedef struct _ATCmdFile_GetInfo_t_
{
    uint8_t *filename;
    uint32_t token;
    SlFsFileInfo_t info;
}ATCmdFile_GetInfo_t;

typedef struct _ATCmdFile_t_
{
    int32_t     id;
    uint32_t    offset;
    int32_t     len;
    uint8_t     format;
    uint8_t     *data;
}ATCmdFile_t;

//*****************************************************************************
// AT Command File System Routines
//*****************************************************************************


/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_openFree(ATCmdFile_Open_t *params)
{    
    if (params != NULL)
    {
        if (params->filename != NULL)
        {
            free(params->filename);
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
int32_t ATCmdFile_openParse(char *buff, ATCmdFile_Open_t *params)
{
    int32_t ret = 0;
    
    /* File name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->filename, ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }

    /* Options */
    if ((ret = StrMpl_getBitmaskListVal(ATCmd_fileOpenOption, sizeof(ATCmd_fileOpenOption)/sizeof(StrMpl_List_t), &buff, &params->mode, ATCMD_DELIM_ARG,ATCMD_DELIM_BIT, ATCmd_excludeDelimArray,STRMPL_FLAG_PARAM_SIZE_32 )) < 0)
    {
        return ret;
    }

    /* file size */
    if ((ret = StrMpl_getVal((char **)&buff, &params->filesize , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        if ((ret == STRMPL_ERROR_PARAM_MISSING) && ((params->mode & SL_FS_CREATE) == 0))
        {
            ret = 0;
        }
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
int32_t ATCmdFile_openResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_Open_t *params = (ATCmdFile_Open_t *)args;
    
    StrMpl_setStr(ATCmd_fileOpenStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setVal(&params->id,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setVal(&params->token,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    ATCmdFile_openFree(params);

    return 0;
}

/*!
    \brief          File Open callback.

    This routine open file in storage device

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_openCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_Open_t *params;

    params = malloc(sizeof(ATCmdFile_Open_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_Open_t));
    
    /* Call the command parser */
    ret = ATCmdFile_openParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_openFree(params);
	    return -1;
    }
    if (params->filesize > 0)
    {
        params->mode |= SL_FS_CREATE_MAX_SIZE(params->filesize);  
    }
    /* file open */
    params->id = sl_FsOpen((uint8_t *)params->filename,params->mode,(_u32 *)&params->token);

    if (params->id < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->id);
        ATCmdFile_openFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdFile_openResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_closeFree(ATCmdFile_Close_t *params)
{    
    if (params->certFilename != NULL)
    {
        free(params->certFilename);
    }
    if (params->signature != NULL)
    {
        free(params->signature);
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
int32_t ATCmdFile_closeParse(char *buff, ATCmdFile_Close_t *params)
{
    int32_t ret = 0;
    
    /* File ID */
    if ((ret = StrMpl_getVal(&buff, &params->id , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    
    /* Certificate file name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->certFilename, ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        if (ret != STRMPL_ERROR_PARAM_MISSING)
        {
            return ret;
        }
    }
    /* Signature */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->signature, ATCMD_DELIM_TRM, ATCMDFILE_SHA256_LEN ,ATCmd_excludeDelimStr)) < 0)
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
    \brief          File Close callback.

    This routine close file in storage device

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_closeCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_Close_t params;

    memset(&params, 0x0, sizeof(ATCmdFile_Close_t));
    
    /* Call the command parser */
    ret = ATCmdFile_closeParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_closeFree(&params);
	    return -1;
    }

    if (params.signature != NULL)
    {
        params.signatureLen = strlen(params.signature);
    }

    /* file close */
    ret = sl_FsClose(params.id, (uint8_t *)params.certFilename,(uint8_t *)params.signature,params.signatureLen);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    ATCmdFile_closeFree(&params);

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_ctlFree(ATCmdFile_Ctl_t *params)
{    
    if (params != NULL)
    {
        if (params->filename != NULL)
        {
            free(params->filename);
        }
        if (params->data!= NULL)
        {
            free(params->data);
        }
        if (params->outputData!= NULL)
        {
            free(params->outputData);
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
int32_t ATCmdFile_ctlParse(char *buff, ATCmdFile_Ctl_t *params)
{
    int32_t ret = 0;
    uint8_t optionalyToken = 0;
    uint8_t optionalyFilename = 0;
    uint8_t ignored_data;
 
    /* Command */
    if ((ret = StrMpl_getListVal(ATCmd_fileCtlCmd, sizeof(ATCmd_fileCtlCmd)/sizeof(StrMpl_List_t), &buff, &params->command, ATCMD_DELIM_ARG, sizeof(params->command))) < 0)
    {
        return ret;
    }
    if ((params->command != SL_FS_CTL_ROLLBACK) && (params->command != SL_FS_CTL_COMMIT) && (params->command != SL_FS_CTL_RENAME))
    {
        optionalyToken = 1;
        optionalyFilename = 1;        
    }
    /* secure token */
    if ((ret = StrMpl_getVal(&buff, &params->token , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        if ((ret != STRMPL_ERROR_PARAM_MISSING) || (optionalyToken == 0))
        {
            return ret;
        }
    }

    /* File name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->filename, ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        if ((ret != STRMPL_ERROR_PARAM_MISSING) || (optionalyFilename == 0))
        {
            return ret;
        }
        ret = 0;
    }

    /* data */
    if (params->command == SL_FS_CTL_RENAME)
    {
        if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->data, ATCMD_DELIM_TRM, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
        {
            return ret;
        }
        params->dataLen = 0;
    }
    else if (params->command == SL_FS_CTL_RESTORE)
    {
        params->dataLen = sizeof(SlFsRetToFactoryCommand_t);
        params->data = malloc(params->dataLen);
        if (params->data == NULL)
        {
            return -1;
        }
        if ((ret = StrMpl_getListVal(ATCmd_fileCtlCmdRestore, sizeof(ATCmd_fileCtlCmdRestore)/sizeof(StrMpl_List_t), &buff, (void *)params->data, ATCMD_DELIM_TRM, sizeof(SlFsRetToFactoryCommand_t))) < 0)
        {
            return ret;
        }
    }
    else
    {
        /* data parameter ignored for all other commands */
        if ((ret = StrMpl_getVal(&buff, (void *)&ignored_data , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
        {
            if (ret == STRMPL_ERROR_DELIM_MISSING)
            {
                return ret;
            }
            ret = 0;
        }

        /* only SL_FS_CTL_GET_STORAGE_INFO has output data */
        if (params->command == SL_FS_CTL_GET_STORAGE_INFO)
        {
            params->OutputDataLen = sizeof(SlFsControlGetStorageInfoResponse_t);
            params->outputData = malloc(params->OutputDataLen);
            if (params->outputData == NULL)
            {
                return -1;
            }
        }
        /* all other command has allocated data */
        else
        {
            params->dataLen = sizeof(SlFsControl_t);
            params->data = malloc(params->dataLen);
            if (params->data == NULL)
            {
                return -1;
            }
            /* set filters = 0 */
            ((SlFsControl_t *)(params->data))->IncludeFilters = 0;
        }
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
int32_t ATCmdFile_ctlResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_Ctl_t *params = (ATCmdFile_Ctl_t *)args;
    
    StrMpl_setStr(ATCmd_fileCtlStr,&buff,ATCMD_DELIM_EVENT);
    if ((params->command == SL_FS_CTL_ROLLBACK) || (params->command == SL_FS_CTL_COMMIT))
    {
        StrMpl_setVal(&params->newToken,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    }
    else
    {
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_ARG);
    }
    if (params->command == SL_FS_CTL_GET_STORAGE_INFO)
    {
        /* write first struct (all fields are 16 bits so we can write it as an array) */
        StrMpl_setArrayVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->DeviceUsage),&buff,7,ATCMD_DELIM_ARG,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_16 | STRMPL_FLAG_PARAM_UNSIGNED);
        /* write second struct */
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.MaxFsFiles),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.IsDevlopmentFormatType),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.Bundlestate),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.MaxFsFilesReservedForSysFiles),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.ActualNumOfUserFiles),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.ActualNumOfSysFiles),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_8 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.NumOfAlerts),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.NumOfAlertsThreshold),&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
        StrMpl_setVal(&(((SlFsControlGetStorageInfoResponse_t *)(params->outputData))->FilesUsage.FATWriteCounter),&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_16 | STRMPL_FLAG_PARAM_UNSIGNED);
   }
    else
    {
        StrMpl_setStr(NULL,&buff,ATCMD_DELIM_TRM);
    }
    ATCmdFile_ctlFree(params);

    return 0;
}

/*!
    \brief          File Control callback.

    This routine controls various file system operations

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_ctlCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_Ctl_t *params;

    params = malloc(sizeof(ATCmdFile_Ctl_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_Ctl_t));
    
    /* Call the command parser */
    ret = ATCmdFile_ctlParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_ctlFree(params);
	    return -1;
    }

    /* file close */
    ret = sl_FsCtl(params->command, params->token, params->filename,params->data,params->dataLen,params->outputData,params->OutputDataLen,(_u32 *)&params->newToken);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdFile_ctlFree(params);
    }
    else
    {
        if ((params->command == SL_FS_CTL_ROLLBACK) || (params->command == SL_FS_CTL_COMMIT) || (params->command == SL_FS_CTL_GET_STORAGE_INFO))
        {
            ATCmd_commandResult(ATCmdFile_ctlResult,params,0);
        }
        else
        {
            ATCmdFile_ctlFree(params);
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
int32_t ATCmdFile_delFree(ATCmdFile_Del_t *params)
{    
    if (params->filename != NULL)
    {
        free(params->filename);
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
int32_t ATCmdFile_delParse(char *buff, ATCmdFile_Del_t *params)
{
    int32_t ret = 0;    
    
    /* file name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->filename, ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    /* token */
    if ((ret = StrMpl_getVal(&buff, &params->token , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
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
    \brief          File Del callback.

    This routine delete file from storage device

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_delCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_Del_t params;

    memset(&params, 0x0, sizeof(ATCmdFile_Del_t));
    
    /* Call the command parser */
    ret = ATCmdFile_delParse((char *)arg, &params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_delFree(&params);
	    return -1;
    }

    /* file delete */
    ret = sl_FsDel(params.filename, params.token);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
    }
    else
    {
        ATCmd_okResult();
    }
    ATCmdFile_delFree(&params);

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_getFileListFree(ATCmdFile_GetFileList_t *params)
{    
    if (params != NULL)
    {
        if (params->entry != NULL)
        {
            free(params->entry);
        }
        free(params);
    }
    return 0;
}

/*!
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdFile_getFileListResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_GetFileList_t *params = (ATCmdFile_GetFileList_t *)args;
    ATCmdFile_FileListEntry_t *entry;

    if (num == 0xFFFFFF)
    {
        ATCmdFile_getFileListFree(params);
        return -1;
    }
    else
    {
        entry = params->entry + num;
        StrMpl_setStr(ATCmd_fileGetFileListStr,&buff,ATCMD_DELIM_EVENT);
        /* file name */
        StrMpl_setStr(entry->fileName,&buff,ATCMD_DELIM_ARG);
        /* FileMaxSize */
        StrMpl_setVal(&entry->attribute.FileMaxSize,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
        /* Properties */
        StrMpl_setBitmaskListStr(ATCmd_fileGetInfo, sizeof(ATCmd_fileGetInfo)/sizeof(StrMpl_List_t),&entry->attribute.Properties, &buff, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32);
        /* FileAllocatedBlocks */
        StrMpl_setVal(&entry->attribute.FileAllocatedBlocks,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);

        /* if last chunk - sign the buffer as availble */
        if (num == (params->maxEntries - 1))
        {
            params->entry->fileName[0] = 0;
            params->entry->attribute.FileMaxSize = 0;
        }
    }

    return 0;
}

/*!
    \brief          File Get File List callback.

    This routine get list of file names

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_getFileListCallback(void *arg)
{
    int32_t ret = 1;
    ATCmdFile_GetFileList_t *params;
    uint8_t i;

    params = malloc(sizeof(ATCmdFile_GetFileList_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_GetFileList_t));
    
    params->index = -1;
    params->maxEntryLen = sizeof(ATCmdFile_FileListEntry_t);
    params->maxEntries = ATCMDFILE_MAX_FILES_ENTRIES;
    params->entry = malloc(params->maxEntries * params->maxEntryLen);

    if (params->entry == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdFile_getFileListFree(params);
	    return -1;
    }
    /* sign the buffer entry as availble */
    params->entry->fileName[0] = 0;
    params->entry->attribute.FileMaxSize = 0;
    
    /* file get file list */
    while( ret > 0 )
    {
        while ((params->entry->fileName[0] != 0) || (params->entry->attribute.FileMaxSize != 0))
        {
            usleep(10);
        }
        ret = sl_FsGetFileList( (_i32 *)&params->index, params->maxEntries, params->maxEntryLen, (uint8_t *)params->entry, SL_FS_GET_FILE_ATTRIBUTES);
        if (ret < 0)
        {
            ATCmd_errorResult(ATCmd_errorCmdStr,ret);
            ATCmdFile_getFileListFree(params);
            break;
        }

        params->maxEntries = ret;
        for (i = 0; i < ret; i++)
        {
            ATCmd_commandResult(ATCmdFile_getFileListResult,params,i);
        }
    }
    if (ret == 0)
    {
        ATCmd_commandResult(ATCmdFile_getFileListResult,params,0xFFFFFF);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_getInfoFree(ATCmdFile_GetInfo_t *params)
{    
    if (params != NULL)
    {
        if (params->filename != NULL)
        {
            free(params->filename);
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
int32_t ATCmdFile_getInfoParse(char *buff, ATCmdFile_GetInfo_t *params)
{
    int32_t ret = 0;    
    
    /* file name */
    if ((ret = StrMpl_getAllocStr(&buff, (char **)&params->filename, ATCMD_DELIM_ARG, SL_FS_MAX_FILE_NAME_LENGTH ,ATCmd_excludeDelimStr)) < 0)
    {
        return ret;
    }
    /* token */
    if ((ret = StrMpl_getVal(&buff, &params->token , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
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
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdFile_getInfoResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_GetInfo_t *params = (ATCmdFile_GetInfo_t *)args;
    
    StrMpl_setStr(ATCmd_fileGetInfoStr,&buff,ATCMD_DELIM_EVENT);
    StrMpl_setBitmaskListStr(ATCmd_fileGetInfo, sizeof(ATCmd_fileGetInfo)/sizeof(StrMpl_List_t),&params->info.Flags, &buff, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_16 |STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setVal(&params->info.Len,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setVal(&params->info.MaxSize,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setArrayListStr(ATCmd_fileGetInfoToken, sizeof(ATCmd_fileGetInfoToken)/sizeof(StrMpl_List_t),&params->info.Token, &buff, 4, ATCMD_DELIM_ARG, ATCMD_DELIM_BIT, STRMPL_FLAG_PARAM_SIZE_32 |STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setVal(&params->info.StorageSize,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    StrMpl_setVal(&params->info.WriteCounter,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_UNSIGNED);
    ATCmdFile_getInfoFree(params);

    return 0;
}

/*!
    \brief          File Get Info callback.

    This routine get information of a file

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_getInfoCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_GetInfo_t *params;

    params = malloc(sizeof(ATCmdFile_GetInfo_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_GetInfo_t));
    
    /* Call the command parser */
    ret = ATCmdFile_getInfoParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_getInfoFree(params);
	    return -1;
    }

    /* file get info */
    ret = sl_FsGetInfo(params->filename, params->token, &params->info);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,ret);
        ATCmdFile_getInfoFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdFile_getInfoResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

/*!
    \brief          Free allocated memory

    \param          params       -   Points to buffer for deallocate.

    \return         Upon successful completion, the function shall return 0.

*/
int32_t ATCmdFile_writeReadFree(ATCmdFile_t *params)
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
int32_t ATCmdFile_readParse(char *buff, ATCmdFile_t *params)
{
    int32_t ret = 0;    
    
    /* id */
    if ((ret = StrMpl_getVal(&buff, &params->id , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    /* offset */
    if ((ret = StrMpl_getVal(&buff, &params->offset , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }
    /* length */
    if ((ret = StrMpl_getVal(&buff, &params->len , ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
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
int32_t ATCmdFile_readResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_t *params = (ATCmdFile_t *)args;
    uint32_t outputLen;
    
    StrMpl_setStr(ATCmd_fileReadStr,&buff,ATCMD_DELIM_EVENT);

    /* format */
    StrMpl_setVal(&params->format, &buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8 |STRMPL_FLAG_PARAM_UNSIGNED|STRMPL_FLAG_PARAM_DEC);

    outputLen = params->len;
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length from binary to base64 */
        outputLen = StrMpl_getBase64EncBufSize(params->len);
    }
    StrMpl_setVal(&outputLen,&buff,ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_SIGNED);
    
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert data to base64 */
        StrMpl_encodeBase64(params->data, params->len, (uint8_t *)buff, &outputLen);
    }
    else
    {
        memcpy(buff,params->data,params->len);
    }
    ATCmdFile_writeReadFree(params);

    return 0;
}

/*!
    \brief          File Read callback.

    This routine read block of data from a file in storage device

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_readCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_t *params;

    params = malloc(sizeof(ATCmdFile_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_t));
    
    /* Call the command parser */
    ret = ATCmdFile_readParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_writeReadFree(params);
	    return -1;
    }

    params->data = malloc (params->len);
    if (params->data == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        ATCmdFile_writeReadFree(params);
	    return -1;
    }
    
    /* file read */
    params->len = sl_FsRead(params->id, params->offset, params->data,params->len);

    if (params->len < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->len);
        ATCmdFile_writeReadFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdFile_readResult,params,0);
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
int32_t ATCmdFile_writeParse(char *buff, ATCmdFile_t *params)
{
    int32_t ret = 0;
    uint32_t outputLen;
    
    /* id */
    if ((ret = StrMpl_getVal(&buff, &params->id , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }
    if ((ret = StrMpl_getVal(&buff, &params->offset , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    /* data format */
    if ((ret = StrMpl_getVal(&buff, &params->format , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_8)) < 0)
    {
        return ret;
    }

    /* length */
    if ((ret = StrMpl_getVal(&buff, &params->len , ATCMD_DELIM_ARG,STRMPL_FLAG_PARAM_SIZE_32)) < 0)
    {
        return ret;
    }

    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        /* convert length to binary length */
        outputLen = StrMpl_getBase64DecBufSize((uint8_t *)buff,params->len); 
        params->data = malloc(outputLen);
        if (params->data == NULL)
        {
            return -1;
        }
        if (StrMpl_decodeBase64((uint8_t *)buff, params->len, params->data, &outputLen) < 0)
        {
            return -1;
        }
         params->len =  outputLen;     
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
    \brief          Return result.

    \param          arg       -   Points to list of arguments
                    buff      -   points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

*/
int32_t ATCmdFile_writeResult(void *args, int32_t num, char *buff)
{    
    ATCmdFile_t *params = (ATCmdFile_t *)args;
    
    StrMpl_setStr(ATCmd_fileWriteStr,&buff,ATCMD_DELIM_EVENT);
    if (params->format == ATCMD_DATA_FORMAT_BASE64)
    {
        params->len = StrMpl_getBase64EncBufSize(params->len);
    }
    
    StrMpl_setVal(&params->len,&buff,ATCMD_DELIM_TRM,STRMPL_FLAG_PARAM_DEC | STRMPL_FLAG_PARAM_SIZE_32 | STRMPL_FLAG_PARAM_SIGNED);
    ATCmdFile_writeReadFree(params);

    return 0;
}

/*!
    \brief          File Write callback.

    This routine write block of data to a file in storage device

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return an error;

*/
int32_t ATCmdFile_writeCallback(void *arg)
{
    int32_t ret = 0;
    ATCmdFile_t *params;

    params = malloc(sizeof(ATCmdFile_t));

    if (params == NULL)
    {
        ATCmd_errorResult(ATCmd_errorAllocStr,0);
        return -1;     
    }
    memset(params, 0x0, sizeof(ATCmdFile_t));
    
    /* Call the command parser */
    ret = ATCmdFile_writeParse((char *)arg, params);

    if (ret < 0)
    {
        ATCmd_errorResult(ATCmd_errorParseStr,ret);
        ATCmdFile_writeReadFree(params);
	    return -1;
    }
  
    /* file write */
    params->len = sl_FsWrite(params->id, params->offset, params->data,params->len);

    if (params->len < 0)
    {
        ATCmd_errorResult(ATCmd_errorCmdStr,params->len);
        ATCmdFile_writeReadFree(params);
    }
    else
    {
        ATCmd_commandResult(ATCmdFile_writeResult,params,0);
        ATCmd_okResult();
    }

    return ret;
}

