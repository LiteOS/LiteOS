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
#include <ti/net/ota/source/CdnVendors/CdnDropboxV2.h>


#if OTA_SERVER_TYPE == OTA_SERVER_DROPBOX_V2

/* Dropbox V2 requests */
#define OTA_SERVER_REST_REQ_DIR       "/2/files/list_folder"            /* returns files/folder list */
#define OTA_SERVER_REST_REQ_FILE_URL  "/2/files/get_temporary_link"     /* returns A url that serves the media directly */
#define OTA_SERVER_REST_HDR           "Authorization: Bearer "

/* dropbox V2 response metadata names to parse */
#define JSON_FILE_SIZE  "size"
#define JSON_FILE_NAME  "path_display"
#define JSON_FILE_URL   "link"

#define CRLF_STR_LEN                (2)
#define CONTENT_LENGTH_MAX_STR_LEN  (4) /* Length of the vendor dir (maximum is 50) and the URL length (maximum is 512) in string format - 3 chars + null terminator */
#define HEXADECIMAL_BASE            (16)

/* JSON key path length of the different fields in the metadata JSON file */
#define METADATA_FILENAME_PATH_LENGTH (30)
#define METADATA_FILESIZE_PATH_LENGTH (30)

char template_DropboxV2URL [] = "{\"metadata\":raw,\"link\": string}";
char template_DropboxV2Metadata [] = "{\"entries\":[{\"path_display\": string,\"size\": int32}]}";

extern OtaJson OtaJsonObj;

/* DROPBOX API V2 - build list_folder, example
    POST /2/files/list_folder HTTP/1.1
    Host: api.dropboxapi.com
    Authorization: Bearer -----------AAAAAAAAABZFti2KbmMpg1sayxzcywgzWjxOzAnWfloOb4jb-3SIN
    Content-Type: Application/Json
    Content-Length: 23

    {"path": "/OTA_CC3120"}
*/
int16_t  CdnDropboxV2_SendReqDir(int16_t SockId, uint8_t *pSendBuf, uint8_t *pServerName, uint8_t *pVendorDir, uint8_t *pVendorToken)
{
    uint8_t ReqDirCmdBuf[200];

    /* Headers */
    strcpy((char *)ReqDirCmdBuf, (const char *)pVendorToken);
    strcat((char *)ReqDirCmdBuf, "\r\nContent-Type: Application/Json\r\nContent-Length:  ");
    snprintf((char *)&ReqDirCmdBuf[strlen((const char *)ReqDirCmdBuf)], CONTENT_LENGTH_MAX_STR_LEN, "%d", 13 /* {"path": "/"} */ + strlen((const char *)pVendorDir));
    strcat((char *)ReqDirCmdBuf, "\r\n\r\n");

    /* Data */
    strcat((char *)ReqDirCmdBuf, "{\"path\": \"/");
    strcat((char *)ReqDirCmdBuf, (const char *)pVendorDir);
    strcat((char *)ReqDirCmdBuf, "\"}");

    _SlOtaLibTrace(("CdnDropbox_SendReqDir: uri=%s\r\n", OTA_SERVER_REST_REQ_DIR));
    return HttpClient_SendReq (SockId, pSendBuf, (uint8_t *)"POST ", pServerName, (uint8_t *)OTA_SERVER_REST_REQ_DIR , ""/*pVendorDir*/, (uint8_t *)OTA_SERVER_REST_HDR, ReqDirCmdBuf/*pVendorToken*/);
}

/* DROPBOX API - parse metadata JSON response, example
{
    "entries": [
        {
            ".tag": "file",
            "name": "2016052417_CC3220RS.tar",
            "path_lower": "/ota_r2/2016052417_cc3220rs.tar",
            "path_display": "/OTA_R2/2016052417_CC3220RS.tar",
            "id": "id:K3LU5u7LSVAAAAAAAAAAAQ",
            "client_modified": "2016-06-08T05:12:19Z",
            "server_modified": "2016-06-08T05:12:19Z",
            "rev": "28c3e23446f31",
            "size": 123392
        }
    ],
    "cursor": "------------12ZuNG9Ez31UwPIop4m82EZEac2YKTS8Ry2l3dO8ZzI6whdBc0chq6B4rI_F7HbOxJuPhxYtExaHoZ39_IGCjKeCI5q6RkK2TYksi1M32_8y5ms0v_8ER52y1lCunSYJ7MSAIZLF8rLvTN-",
    "has_more": false
}*/
int16_t  CdnDropboxV2_ParseRespDir(int16_t SockId, uint8_t *pRespBuf, OtaDirData_t *pOtaDirData)
{
    OtaFileInfo_t CurrFileInfo;
    int16_t RespLen;
    uint8_t i = 0;
    uint32_t jsonLen;
    int16_t  retVal;
    uint8_t *iterator;
    uint8_t *pEndBuf;
    int16_t files_num = 0;
    int16_t len = 0;
    int16_t ProcessedSize;
    static bool firstRun = TRUE;
    char * pTempBuf = NULL;
    char *pJsonArrayKey =  "\"entries\"";

    char fileNamePathBuf [METADATA_FILENAME_PATH_LENGTH];
    char fileSizePathBuf [METADATA_FILESIZE_PATH_LENGTH];

    pOtaDirData->NumFiles = 0;

    /* check HTTP status OK 200 and skip all HTTP headers */
    RespLen = HttpClient_RecvSkipHdr(SockId, pRespBuf, NET_BUF_SIZE, NULL);
    if (RespLen < 0)
    {
        _SlOtaLibTrace(("CdnDropbox_ParseRespDir: ERROR HttpClient_RecvSkipHdr, status=%ld\r\n", RespLen));
        return RespLen;
    }

    /* The HTTP response Transfer-Encoding is "chunked", therefore the */
    /* JSON file length is stated at the beginning of the message body. */
    /* Search for CR+LF in order to parse the JSON length. */
    pEndBuf = pRespBuf + RespLen;
    iterator = pRespBuf;

    /* Avoid exceeding array size */
    while ((pEndBuf - iterator) >= CRLF_STR_LEN)
    {
        /* Search for CR+LF */
        if (strncmp ((const char *)iterator, "\r\n", CRLF_STR_LEN) == 0)
        {
            /* Found the CR+LF, pass the beginning of the buffer in order to convert the length from string in HEX to int */
            jsonLen = (int)strtol((const char *)pRespBuf, (char **)&iterator, HEXADECIMAL_BASE);
            break;
        }
        else
        {
            iterator+=1;
        }
    }

    /* ProcessedSize is the length of the actual JSON file that was received */
    ProcessedSize = pEndBuf - (iterator + CRLF_STR_LEN);

    _SlOtaLibTrace(("RespLen is %d, ProcessedSize is: %d\r\n", RespLen, ProcessedSize));

    /* Set pRespBuf to point to the beginning of the JSON file */
    pRespBuf = (iterator + CRLF_STR_LEN);

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
    retVal = OtaJson_init(template_DropboxV2Metadata, &(OtaJsonObj.jsonBuffer.fileBuffer), jsonLen);

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
        /* Failed to get array members count, return error */
        return files_num;
    }

    for (i = 0; i < files_num; i++)
    {
        /* Initialize data containers before parsing */
        memset (&CurrFileInfo, 0, sizeof(CurrFileInfo));

        memset (&fileNamePathBuf, 0, sizeof(fileNamePathBuf));
        snprintf(fileNamePathBuf, METADATA_FILENAME_PATH_LENGTH, "\"entries\".[%d].\"path_display\"",i);

        /* Parse the metadata file name */
        retVal = OtaJson_getMetadataFileName(CurrFileInfo.OtaFileName, fileNamePathBuf);
        if (retVal < 0)
        {
            /* Parsing metadata file name failed, return error */
            return retVal;
        }

        memset (&fileSizePathBuf, 0, sizeof(fileSizePathBuf));
        snprintf(fileSizePathBuf, METADATA_FILESIZE_PATH_LENGTH, "\"entries\".[%d].\"size\"",i);

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

    OtaJson_destroy();

    return pOtaDirData->NumFiles;
}

/* DROPBOX V2 API - build get_temporary_link, example
    POST /2/files/get_temporary_link HTTP/1.1
    Host: api.dropboxapi.com
    Authorization: Bearer -----------AAAAAAAAABZFti2KbmMpg1sayxzcywgzWjxOzAnWfloOb4jb-3SIN
    Content-Type: Application/Json
    Content-Length: 43

    {"path": "/ota_r2/2016052417_cc3220rs.tar"}
*/
int16_t  CdnDropboxV2_SendReqFileUrl(int16_t SockId, uint8_t *pSendBuf, uint8_t *pServerName, uint8_t *pFileName, uint8_t *pVendorToken)
{
    uint8_t ReqDirCmdBuf[200];

    /* Headers */
    strcpy((char *)ReqDirCmdBuf, (const char *)pVendorToken);
    strcat((char *)ReqDirCmdBuf, "\r\nContent-Type: Application/Json\r\nContent-Length:  ");
    snprintf((char *)&ReqDirCmdBuf[strlen((const char *)ReqDirCmdBuf)], CONTENT_LENGTH_MAX_STR_LEN, "%d", 12 /* {"path": ""} */ + strlen((const char *)pFileName));
    strcat((char *)ReqDirCmdBuf, "\r\n\r\n");

    /* Data */
    strcat((char *)ReqDirCmdBuf, "{\"path\": \"");
    strcat((char *)ReqDirCmdBuf, (const char *)pFileName);
    strcat((char *)ReqDirCmdBuf, "\"}");

    _SlOtaLibTrace(("CdnDropbox_SendReqFileUrl: uri=%s\r\n", OTA_SERVER_REST_REQ_FILE_URL));
    return HttpClient_SendReq(SockId, pSendBuf, (uint8_t *)"POST ", pServerName, (uint8_t *)OTA_SERVER_REST_REQ_FILE_URL , "" /*pFileName*/, (uint8_t *)OTA_SERVER_REST_HDR, ReqDirCmdBuf/*pVendorToken*/);
}

/* DROPBOX API - parse media JSON response, example
{
    "metadata": {
        "name": "2016052417_CC3220RS.tar",
        "path_lower": "/ota_r2/2016052417_cc3220rs.tar",
        "path_display": "/OTA_R2/2016052417_CC3220RS.tar",
        "id": "id:K3LU5u7LSVAAAAAAAAAAAQ",
        "client_modified": "2016-06-08T05:12:19Z",
        "server_modified": "2016-06-08T05:12:19Z",
        "rev": "28c3e23446f31",
        "size": 123392
    },
    "link": "https://dl.dropboxusercontent.com/apitl/1/-----------QCJwBbXnW5wtnvyozgFTJ_q11_AFd4asAKS1zEusm4kWN25Gd5QIdt678G_QnYTXYCEN05qimZuZwQ4M5qXJ2LHryZy1JA0_3rx-EEyCZ6pXb9JB0q-Wz9vAD7fmXkPDocQHAr2gVbck2osLQ9HhwDr_XruXw1FJo_2Fb6IE2dpBUDExXKnSqdI6sMHD0uI0jFuheOCKapW6YsEsRQUkh8tZDNphWbNB-nloJ6NEqPA845I7cZshlb5eslY"
}*/
int16_t CdnDropboxV2_ParseRespFileUrl(uint16_t SockId, uint8_t *pRespBuf, uint8_t *pFileUrl, uint32_t FileUrlBufSize)
{
    uint8_t *iterator;
    uint8_t *pEndBuf;
    int16_t RespLen;
    uint32_t jsonLen;
    int16_t  retVal;

    _SlOtaLibTrace(("HTTP request is:\r\n%s\r\n\n",pRespBuf));

    /* check HTTP status OK 200 and skip all HTTP headers */
    RespLen = HttpClient_RecvSkipHdr(SockId, pRespBuf, NET_BUF_SIZE, NULL);
    if (RespLen < 0)
    {
        _SlOtaLibTrace(("CdnDropbox_ParseRespDir: ERROR HttpClient_RecvSkipHdr, status=%ld\r\n", RespLen));
        return RespLen;
    }

    /* The HTTP response Transfer-Encoding is "chunked", therefore the */
    /* JSON file length is stated at the beginning of the message body. */
    /* Search for CR+LF in order to parse the JSON length. */
    pEndBuf = pRespBuf + RespLen;
    iterator = pRespBuf;

    /* Avoid exceeding array size */
    while ((pEndBuf - iterator) >= CRLF_STR_LEN)
    {
        /* Search for CR+LF */
        if (strncmp ((const char *)iterator, "\r\n", CRLF_STR_LEN) == 0)
        {
            /* Found the CR+LF, pass the beginning of the buffer in order to convert the length from string in HEX to int */
            jsonLen = (int)strtol((const char *)pRespBuf, (char **)&iterator, HEXADECIMAL_BASE);
            break;
        }
        else
        {
            iterator+=1;
        }
    }

    /* Set the fileBuffer to point to the beginning of the JSON file */
    OtaJsonObj.jsonBuffer.fileBuffer = (char *)iterator + CRLF_STR_LEN;
    OtaJsonObj.jsonBuffer.fileBuffer[jsonLen] = '\0';

    /* Initialize the JSON module */
    retVal = OtaJson_init(template_DropboxV2URL, &(OtaJsonObj.jsonBuffer.fileBuffer), jsonLen);
    if (retVal < 0)
    {
        /* Initializing the JSON module failed, return error */
        _SlOtaLibTrace(("CdnDropbox_ParseRespDir: ERROR OtaJson_init, retVal=%d\r\n", retVal));
        return retVal;
    }

    /* "link" is the key for the JSON object that holds the download URL */
    retVal = OtaJson_getURL(pFileUrl, "\"link\"");
    if (retVal < 0)
    {
        /* Parsing failed, return error */
        _SlOtaLibTrace(("CdnDropbox_ParseRespDir: ERROR OtaJson_getURL, retVal=%d\r\n", retVal));
        return retVal;
    }

    /* free dynamic memory */
    OtaJson_destroy();

    return CDN_STATUS_OK;
}

/* DROPBOX API - build get file request, example
    GET /1/view/e2scsc5ku4hnjyi/Apps/SL_OTA/OTA_R2/20151217_2_00.tar HTTP/1.1
    Host: dl.dropboxusercontent.com
*/
int16_t CdnDropboxV2_SendReqFileContent(int16_t SockId, uint8_t *pSendBuf, uint8_t *pFileServerName, uint8_t *pFileName)
{
    _SlOtaLibTrace(("CdnDropbox_SendReqFileContent: file=%s\r\n", pFileName));
    return HttpClient_SendReq(SockId, pSendBuf, (uint8_t *)"GET ", pFileServerName, NULL, pFileName, NULL, NULL);
 }

#endif /* #if OTA_SERVER_TYPE == OTA_SERVER_DROPBOX */
