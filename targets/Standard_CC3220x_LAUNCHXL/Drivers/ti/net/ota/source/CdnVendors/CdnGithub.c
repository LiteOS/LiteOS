/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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

#include <ti/net/ota/otauser.h>
#include <ti/net/ota/source/OtaHttpClient.h>
#include <ti/net/ota/source/OtaJson.h>
#include <ti/net/ota/source/CdnVendors/CdnGithub.h>

#if OTA_SERVER_TYPE == OTA_SERVER_GITHUB

/* Github requests */
#define OTA_SERVER_REST_REQ_DIR         "/contents/" /* returns files/folder list */
#define OTA_SERVER_REST_HDR             "User-Agent: "

/* Github response metadata names to parse */
#define JSON_FILE_SIZE      "size"
#define JSON_FILE_NAME      "download_url"

/* JSON key path length of the different fields in the metadata JSON file */
#define METADATA_FILENAME_PATH_LENGTH (30)
#define METADATA_FILESIZE_PATH_LENGTH (30)

char template_GithubMetadata [] = "{\"#\":[{\"size\": int32,\"download_url\": string}]}";

extern OtaJson OtaJsonObj;

/* GITHUB API - build get contents request, example:
    GET /repos/CC3X20/SL_OTA/contents/OTA_R2 HTTP/1.1
    Host: api.github.com
*/
int16_t  CdnGithub_SendReqDir(int16_t SockId, uint8_t *pSendBuf, uint8_t *pServerName, uint8_t *pVendorDir, uint8_t *pVendorToken)
{
    uint8_t ReqDirCmdBuf[100];

    strcpy((char *)ReqDirCmdBuf, OTA_VENDOR_ROOT_DIR);
    strcat((char *)ReqDirCmdBuf, OTA_SERVER_REST_REQ_DIR);

    _SlOtaLibTrace(("CdnGithub_SendReqDir: uri=%s\r\n", ReqDirCmdBuf));
    return HttpClient_SendReq (SockId, pSendBuf, (uint8_t *)"GET ", pServerName, ReqDirCmdBuf , pVendorDir, (uint8_t *)OTA_SERVER_REST_HDR, pVendorToken);
}


/* GITHUB API - parse response for directory files request, json RESPONSE, example:
    [
        {
            "name": "20151217_1_28.tar",
            "path": "OTA_R2/20151217_1_28.tar",
            "sha": "7b30216698eca244ac4851cc9f81aa022f5837ec",
            "size": 260096,
            "url": "https://api.github.com/repos/CC3X20/SL_OTA/contents/OTA_R2/20151217_1_28.tar?ref=master",
            "html_url": "https://github.com/CC3X20/SL_OTA/blob/master/OTA_R2/20151217_1_28.tar",
            "git_url": "https://api.github.com/repos/CC3X20/SL_OTA/git/blobs/7b30216698eca244ac4851cc9f81aa022f5837ec",
            "download_url": "https://raw.githubusercontent.com/CC3X20/SL_OTA/master/OTA_R2/20151217_1_28.tar",
            "type": "file",
            "_links": {
                "self": "https://api.github.com/repos/CC3X20/SL_OTA/contents/OTA_R2/20151217_1_28.tar?ref=master",
                "git": "https://api.github.com/repos/CC3X20/SL_OTA/git/blobs/7b30216698eca244ac4851cc9f81aa022f5837ec",
                "html": "https://github.com/CC3X20/SL_OTA/blob/master/OTA_R2/20151217_1_28.tar"
            }
        }
    ]
*/
int16_t  CdnGithub_ParseRespDir(int16_t SockId, uint8_t *pRespBuf, OtaDirData_t *pOtaDirData)
{
    OtaFileInfo_t CurrFileInfo;
    int16_t RespLen;
    uint32_t jsonLen;
    uint8_t i = 0;
    int16_t  retVal;

    int16_t files_num = 0;
    int16_t len = 0;
    int16_t ProcessedSize;
    static bool firstRun = TRUE;
    char * pTempBuf = NULL;
    char *pJsonArrayKey =  "\"#\"";

    char fileNamePathBuf [METADATA_FILENAME_PATH_LENGTH];
    char fileSizePathBuf [METADATA_FILESIZE_PATH_LENGTH];

    pOtaDirData->NumFiles = 0;

    /* check HTTP status OK 200 and skip all HTTP headers */
    RespLen = HttpClient_RecvSkipHdr(SockId, pRespBuf, NET_BUF_SIZE, &jsonLen);
    if (RespLen < 0)
    {
        _SlOtaLibTrace(("CdnGithub_ParseRespDir: ERROR HttpClient_RecvSkipHdr, status=%ld\r\n", RespLen));
        return RespLen;
    }

    ProcessedSize = RespLen;

    /* After parsing the JSON file length, verify this packet holds the entire file */
    while (ProcessedSize < jsonLen)
    {
        if (firstRun)
        {
            /* The JSON parser must have the entire file in order to parse it successfully. */
            /* Allocate a buffer in the size of the entire JSON file and fill it in each round. */
            pTempBuf = (char*)malloc(jsonLen+1);
            if (pTempBuf == NULL)
            {
                /* Allocation failed, return error. */
                return -1;
            }
            memcpy(pTempBuf, (char *)pRespBuf, ProcessedSize);
            firstRun = FALSE;
        }
        else
        {
            /* Copy the received buffer from where we stopped the previous copy */
            memcpy(&(pTempBuf[ProcessedSize]), (char *)pRespBuf, (jsonLen + 1 - ProcessedSize));
            ProcessedSize+=len;

            if (ProcessedSize >= jsonLen)
            {
                /* received the entire JSON file. break*/
                break;
            }
        }

        /* Get the rest of the JSON file */
        len = HttpClient_RecvAppend(SockId, pRespBuf, NET_BUF_SIZE, ProcessedSize, ProcessedSize);

        _SlOtaLibTrace(("len is: %d\r\n", len));
        _SlOtaLibTrace(("After recvAppend pRespBuf is: %s\r\n", pRespBuf));
    }

    _SlOtaLibTrace(("the entire JSON pRespBuf is: %s\r\n", pTempBuf));

    /* At this point the entire JSON file was received */
    /* Set the fileBuffer to point to the beginning of the JSON file */
    if(firstRun)
    {
        /* Received the entire file in the first run. */
        /* No additional memory allocation was needed. */
        OtaJsonObj.jsonBuffer.fileBuffer = (char *)pRespBuf;
        OtaJsonObj.jsonBuffer.fileBuffer[jsonLen] = '\0';
    }
    else
    {
        OtaJsonObj.jsonBuffer.fileBuffer = (char *)pTempBuf;
        OtaJsonObj.jsonBuffer.fileBuffer[jsonLen] = '\0';
    }

    /* Set static variable to initial value to allow retry in case of a warning during the OTA process */
    firstRun = TRUE;

    /* Initialize the JSON module */
    retVal = OtaJson_init(template_GithubMetadata, &(OtaJsonObj.jsonBuffer.fileBuffer), jsonLen);

    /* After calling OtaJson_init, the Json Parser module holds an internal representation */
    /* of the Json file, there is no longer a use in the text representation. */
    /* Free dynamic memory that was allocated to contain the Json text representation */
    /* (whether OtaJson_init returned success or failure) */
    if (pTempBuf != NULL)
    {
        free(pTempBuf);
    }

    if (retVal < 0)
    {
        _SlOtaLibTrace(("CdnDropbox_ParseRespDir: ERROR OtaJson_init, retVal=%d\r\n", retVal));
        /* Initializing the JSON module failed, return error */
        return retVal;
    }

    files_num = OtaJson_getArrayMembersCount(OtaJsonObj.jsonObjHandle, pJsonArrayKey);
    if (files_num < 0)
    {
        /* Failed to get array members count, free dynamic memory and return error */
        return files_num;
    }

    for (i = 0; i < files_num; i++)
    {
        /* Initialize data containers before parsing */
        memset (&CurrFileInfo, 0, sizeof(CurrFileInfo));

        memset (&fileNamePathBuf, 0, sizeof(fileNamePathBuf));
        snprintf(fileNamePathBuf, METADATA_FILENAME_PATH_LENGTH, "\"#\".[%d].\"download_url\"",i);

        /* Parse the metadata file name */
        retVal = OtaJson_getMetadataFileName(CurrFileInfo.OtaFileName, fileNamePathBuf);
        if (retVal < 0)
        {
            /* Parsing metadata file name failed, return error */
            return retVal;
        }

        memset (&fileSizePathBuf, 0, sizeof(fileSizePathBuf));
        snprintf(fileSizePathBuf, METADATA_FILESIZE_PATH_LENGTH, "\"#\".[%d].\"size\"",i);

        /* Parse the metadata file size */
        retVal = OtaJson_getMetadataFileSize((uint32_t *)&CurrFileInfo.OtaFileSize, fileSizePathBuf);
        if (retVal < 0)
        {
            /* Parsing metadata file size failed, return error */
            return retVal;
        }

        pOtaDirData->OtaFileInfo[pOtaDirData->NumFiles++] = CurrFileInfo;
        _SlOtaLibTrace(("    OtaDir FileName=%s, FileSize=%ld\r\n", CurrFileInfo.OtaFileName, CurrFileInfo.OtaFileSize));
    }

    /* Flush the rest of the response - assuming full buffer means more data to flush */
    do
    {
        RespLen = HttpClient_Recv(SockId, &pRespBuf[0], NET_BUF_SIZE, 0, 1 /*MAX_EAGAIN_RETRIES*/);
        if (RespLen <= 0)
        {
            break;
        }
    } while(RespLen == NET_BUF_SIZE);

    /* Free dynamic memory */
    OtaJson_destroy();

    return pOtaDirData->NumFiles;
}

int16_t  CdnGithub_SendReqFileUrl(int16_t SockId, uint8_t *pRespBuf, uint8_t *pServerName, uint8_t *pFileName, uint8_t *pVendorToken)
{
    /* First check if the filename already contain the full file URL */

    /* Github, no need to implement, the file url are already in the filename */
    strcpy((char *)pRespBuf, (const char *)pFileName);
    return strlen((const char *)pFileName);
}

int16_t  CdnGithub_ParseRespFileUrl(uint16_t SockId, uint8_t *pSendBuf, uint8_t *pFileUrl, uint32_t FileUrlBufSize)
{
    /* Github, The SendBuf(pRespBuf) already have the URL (implemented in CdnGithub_SendReqFileUrl)*/
    strcpy((char *)pFileUrl, (const char *)pSendBuf);
    return CDN_STATUS_OK;
}

/* GITHUB API - build get file content request, example
    GET /CC3X20/SL_OTA/master/OTA_R2/20151217_1_28.tar HTTP/1.1
    Host: raw.githubusercontent.com
*/
int16_t CdnGithub_SendReqFileContent(int16_t SockId, uint8_t *pSendBuf, uint8_t *pFileServerName, uint8_t *pFileName)
{
    _SlOtaLibTrace(("CdnGithub_SendReqFileContent: file=%s\r\n", pFileName));
    return HttpClient_SendReq(SockId, pSendBuf, (uint8_t *)"GET ", pFileServerName, NULL, pFileName, NULL, NULL);
 }

#endif /* #if OTA_SERVER_TYPE == OTA_SERVER_GITHUB */
