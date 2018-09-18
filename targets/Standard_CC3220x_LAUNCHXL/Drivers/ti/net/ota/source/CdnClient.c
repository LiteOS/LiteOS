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
#include <ti/net/ota/source/CdnVendors/CdnVendors.h>
#include <ti/net/ota/source/CdnClient.h>


int16_t CdnClient_Init(CdnClient_t *pCdnClient, uint8_t *pNetBuf)
{
    memset(pCdnClient, 0, sizeof(CdnClient_t));
    pCdnClient->ServerSockId = -1;
    pCdnClient->FileSockId = -1;
    pCdnClient->pNetBuf = pNetBuf;

    return CDN_STATUS_OK;
}

int16_t CdnClient_ConnectServer(CdnClient_t *pCdnClient, Ota_optServerInfo *pOtaServerInfo)
{
    pCdnClient->pOtaServerInfo = pOtaServerInfo;

    /* Connect to the OTA server */
    _SlOtaLibTrace(("CdnClient_ConnectServer: HttpClient_Connect %s\r\n", pOtaServerInfo->ServerName));
    pCdnClient->ServerSockId = HttpClient_Connect(pOtaServerInfo->ServerName, pOtaServerInfo->IpAddress, SOCKET_PORT_DEFAULT, pOtaServerInfo->SecuredConnection, SOCKET_BLOCKING);
    if (pCdnClient->ServerSockId < 0)
    {
        _SlOtaLibTrace(("CdnClient_ConnectServer: ERROR HttpClient_Connect, Status=%d\r\n", pCdnClient->ServerSockId));
        return pCdnClient->ServerSockId;
    }

    return CDN_STATUS_OK;
}

int16_t CdnClient_ReqOtaDir(CdnClient_t *pCdnClient, uint8_t *pVendorDir)
{
	Ota_optServerInfo *pOtaServerInfo = pCdnClient->pOtaServerInfo;
    int16_t NumDirFiles;
    int32_t Len;

    pCdnClient->CurrDirFileIndex = 0;
    Len = CdnVendor_SendReqDir(pCdnClient->ServerSockId, pCdnClient->pNetBuf, pOtaServerInfo->ServerName, pVendorDir, pOtaServerInfo->VendorToken);
    if (Len <= 0)
    {
        _SlOtaLibTrace(("CdnClient_ReqOtaDir: ERROR CdnVendor_SendReqDir, Status=%ld\r\n", Len));
        return CDN_STATUS_ERROR_SL_SEND;
    }

    NumDirFiles = CdnVendor_ParseRespDir(pCdnClient->ServerSockId, pCdnClient->pNetBuf, &pCdnClient->OtaDirData);

    return NumDirFiles;
}

uint8_t *CdnClient_GetNextDirFile(CdnClient_t *pCdnClient, int32_t *OtaFileSize)
{
    if (pCdnClient->CurrDirFileIndex >= pCdnClient->OtaDirData.NumFiles)
    {
        /* no more files to update */
        return NULL;
    }
    *OtaFileSize = pCdnClient->OtaDirData.OtaFileInfo[pCdnClient->CurrDirFileIndex].OtaFileSize;
    return pCdnClient->OtaDirData.OtaFileInfo[pCdnClient->CurrDirFileIndex++].OtaFileName;
}

int16_t CdnClient_ReqFileUrl(CdnClient_t *pCdnClient, uint8_t *pOtaFileName, uint8_t *pFileUrlBuf, uint32_t FileUrlBufSize)
{
	Ota_optServerInfo *pOtaServerInfo = pCdnClient->pOtaServerInfo;
    int16_t Status=0;

    memset(pFileUrlBuf, 0, MAX_URL_SIZE);

    Status = CdnVendor_SendReqFileUrl(pCdnClient->ServerSockId, pCdnClient->pNetBuf, pOtaServerInfo->ServerName, pOtaFileName, pOtaServerInfo->VendorToken); 
    if (Status < 0)
    {
        _SlOtaLibTrace(("CdnClient_ReqFileUrl: Error CdnVendor_SendReqFileUrl Status=%ld\r\n", Status));
        return Status;
    }

    Status = CdnVendor_ParseRespFileUrl(pCdnClient->ServerSockId, pCdnClient->pNetBuf, pFileUrlBuf, FileUrlBufSize);
    if (Status < 0)
    {
        _SlOtaLibTrace(("CdnClient_ReqFileUrl: Error CdnVendor_ParseRespFileUrl Status=%ld\r\n", Status));
        return Status;
    }

    return CDN_STATUS_OK;
}

int16_t CdnClient_CloseServer(CdnClient_t *pCdnClient)
{
    int16_t Status = CDN_STATUS_OK;

    if (pCdnClient->ServerSockId >= 0)
    {
        Status = HttpClient_CloseConnection(pCdnClient->ServerSockId);
        pCdnClient->ServerSockId = -1;
    }
    return Status;
}

int16_t CdnClient_ConnectFileServer(CdnClient_t *pCdnClient, uint8_t *pFileUrl, int32_t SecuredConnection)
{
    uint8_t ServerNameBuf[MAX_SERVER_NAME];
    uint8_t *pReqUriBuf;
    int16_t Status=0;

    /* extract CDN server name and file name */
    pReqUriBuf = HttpClient_ParseUrl((uint8_t *)pFileUrl, ServerNameBuf, &Status);
    if (pReqUriBuf == 0)
    {
        _SlOtaLibTrace(("CdnClient_ConnectFileServer: ERROR, HttpClient_ParseUrl, Status=%ld\r\n", Status));
        return Status;
    }

    /* connect to the CDN server */
    pCdnClient->PortNum = SOCKET_PORT_DEFAULT;
    pCdnClient->FileSockId = HttpClient_Connect(ServerNameBuf, 0, pCdnClient->PortNum, SecuredConnection, SOCKET_BLOCKING);
    if (pCdnClient->FileSockId < 0)
    {
        _SlOtaLibTrace(("CdnClient_ConnectFileServer: ERROR on HttpClient_Connect, Status=%d\r\n", pCdnClient->FileSockId));
        CdnClient_CloseServer(pCdnClient);
        return pCdnClient->FileSockId;
    }

    return CDN_STATUS_OK;
}

int16_t CdnClient_ReqFileContent(CdnClient_t *pCdnClient, uint8_t *pFileUrl)
{
    uint8_t ServerNameBuf[MAX_SERVER_NAME];
    uint8_t *pReqUriBuf;
    int32_t Len;
    int16_t status=0;

    /* extract CDN server name and file name */
    pReqUriBuf = HttpClient_ParseUrl((uint8_t *)pFileUrl, ServerNameBuf, &status);
    if (pReqUriBuf == 0)
    {
        _SlOtaLibTrace(("CdnClient_ReqFileContent: ERROR, HttpClient_ParseUrl, Status=%ld\r\n", status));
        return status;
    }

    Len = CdnVendor_SendReqFileContent(pCdnClient->FileSockId, pCdnClient->pNetBuf, ServerNameBuf, pReqUriBuf);   
    if (Len <= 0)
    {
        _SlOtaLibTrace(("CdnClient_ReqFileContent: ERROR, sl_Send, Status=%ld\r\n", Len));
        return CDN_STATUS_ERROR_SL_SEND;
    }

    /* The file is prepared to be parsed and save in the file system */
    return CDN_STATUS_OK;
}

int16_t CdnClient_RecvSkipHdr(int16_t SockId, uint8_t *pRespBuf, int16_t RespBufSize)
{
    return (HttpClient_RecvSkipHdr(SockId, pRespBuf, RespBufSize, NULL));
}

int16_t CdnClient_RecvAppend(int16_t SockId, uint8_t *pRecvBuf, int16_t RecvBufSize, int16_t RecvLen, int16_t RecvOffset)
{
    return (HttpClient_RecvAppend(SockId, pRecvBuf, RecvBufSize, RecvLen, RecvOffset));
}

int16_t CdnClient_CloseFileServer(CdnClient_t *pCdnClient)
{
    int16_t Status = CDN_STATUS_OK;

    if (pCdnClient->FileSockId >= 0)
    {
        Status = HttpClient_CloseConnection(pCdnClient->FileSockId);
        pCdnClient->FileSockId = -1;
    }
    return Status;
}

