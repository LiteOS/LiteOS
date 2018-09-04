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
#ifndef __CDN_CLIENT_H__
#define __CDN_CLIENT_H__

#ifdef    __cplusplus
extern "C" {
#endif

#define MAX_DIR_FILES             4
#define MAX_CDN_FILE_NAME_SIZE  128

typedef struct  
{
    uint8_t   OtaFileName[MAX_CDN_FILE_NAME_SIZE+1];
    int32_t  OtaFileSize;
} OtaFileInfo_t; 

typedef struct 
{
    int16_t  NumFiles;
    OtaFileInfo_t OtaFileInfo[MAX_DIR_FILES];
} OtaDirData_t; 

typedef struct 
{
    /* Keep OTA parameters */
	Ota_optServerInfo *pOtaServerInfo;

    /* Keep OTA dir info */
    int32_t CurrDirFileIndex;
    OtaDirData_t OtaDirData;

    /* CdnDirServer connection info */
    int16_t ServerSockId;
    int32_t PortNum;
    int16_t FileSockId;
    uint8_t *pRecvChunk;
    uint8_t *pNetBuf;

} CdnClient_t; 

/* Internal return values */
#define CDN_STATUS_OK                                   (0L)
#define CDN_STATUS_ERROR_SL_SEND                        (-20201L)
#define CDN_STATUS_ERROR_SL_RECV_HDR_EAGAIN             (-20202L)
#define CDN_STATUS_ERROR_PARSE_URL_NOT_FOUND            (-20203L)
#define CDN_STATUS_ERROR_PARSE_DIR_NO_ARRAY             (-20204L)
#define CDN_STATUS_ERROR_PARSE_DIR_NO_OBJECT            (-20205L)
#define CDN_STATUS_ERROR_PARSE_DIR_NO_FILE_SIZE         (-20206L)
#define CDN_STATUS_ERROR_PARSE_DIR_NO_FILE_NAME         (-20207L)

int16_t  CdnClient_Init(CdnClient_t *pCdnClient, uint8_t *pNetBuf);
int16_t  CdnClient_ConnectServer(CdnClient_t *pCdnClient, Ota_optServerInfo *pOtaServerInfo);
int16_t  CdnClient_CloseServer(CdnClient_t *pCdnClient);
int16_t  CdnClient_ReqOtaDir(CdnClient_t *pCdnClient, uint8_t *pVendorDir);
uint8_t  *CdnClient_GetNextDirFile(CdnClient_t *pCdnClient, int32_t *OtaFileSize);
int16_t  CdnClient_ReqFileUrl(CdnClient_t *pCdnClient, uint8_t *pOtaFileName, uint8_t *pFileUrlBuf, uint32_t FileUrlBufSize);
int16_t  CdnClient_ConnectFileServer(CdnClient_t *pCdnClient, uint8_t *pFileUrl, int32_t SecuredConnection);
int16_t  CdnClient_ReqFileContent(CdnClient_t *pCdnClient, uint8_t *pFileUrl);
int16_t  CdnClient_CloseFileServer(CdnClient_t *pCdnClient);
int16_t  CdnClient_RecvSkipHdr(int16_t SockId, uint8_t *pRespBuf, int16_t RespBufSize);
int16_t  CdnClient_RecvAppend(int16_t SockId, uint8_t *pRecvBuf, int16_t RecvBufSize, int16_t RecvLen, int16_t RecvOffset);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __CDN_CLIENT_H__ */
