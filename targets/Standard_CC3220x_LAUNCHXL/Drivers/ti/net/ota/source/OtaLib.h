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
#ifndef __OTA_LIB_H__
#define __OTA_LIB_H__

#ifdef    __cplusplus
extern "C" {
#endif

/******************************************************************************
 * OTA states
******************************************************************************/
typedef enum
{
    OTA_STATE_IDLE = 0,
    OTA_STATE_CONNECT_SERVER,
    OTA_STATE_REQ_OTA_DIR,
    OTA_STATE_CHECK_ARCHIVE_NEW_UPDATE,
    OTA_STATE_REQ_FILE_URL,
    OTA_STATE_CONNECT_FILE_SERVER,
    OTA_STATE_REQ_FILE_CONTENT,
    OTA_STATE_PREPARE_DOWNLOAD,
    OTA_STATE_DOWNLOADING,
    OTA_STATE_WAIT_CONFIRM,
    OTA_STATE_NUMBER_OF_STATES
} OtaState_e;

#define MAX_VENDIR_DIR_SIZE     50

typedef struct
{
    OtaState_e  State;
    uint32_t        Options;

    /* Keep OTA parameters */
    uint8_t         VendorDir[MAX_VENDIR_DIR_SIZE];
    Ota_optServerInfo OtaServerInfo;

    /* OTA objects */
    CdnClient_t  CdnClient;
    OtaArchive_t OtaArchive;

    /* OtaDir and FileUrl info */
    uint8_t  *pOtaFileName;
    int32_t OtaFileSize;
    uint8_t  FileUrlBuf[MAX_URL_SIZE];

    /* file content buffer */
    uint8_t *pRecvChunk;
    int16_t RecvChunkSize;
    int16_t RecvChunkOffset;
    int16_t RecvChunkForceReadMode;
    int16_t RecvChunkReserved;

    /* send/recv buffer - must be 4 bytes alignment */
    uint8_t NetBuf[NET_BUF_SIZE+1];

    /* Retries */
    int8_t  ConsecutiveOtaErrors;

} OtaLib_t;

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __OTA_LIB_H__ */
