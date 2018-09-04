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
#ifndef _OTA_ARCHIVE_H__
#define _OTA_ARCHIVE_H__

#ifdef    __cplusplus
extern "C" {
#endif

#include <ti/drivers/crypto/CryptoCC32XX.h>

#define OTA_ARCHIVE_VERSION    "OTA_ARCHIVE_2.0.0.4"

/* RunStatus */
#define ARCHIVE_STATUS_FORCE_READ_MORE                  (2L)
#define ARCHIVE_STATUS_DOWNLOAD_DONE                    (1L)
#define ARCHIVE_STATUS_CONTINUE                         (0L)
#define ARCHIVE_STATUS_OK                               (0L)
#define ARCHIVE_STATUS_ERROR_STATE                      (-20101L)
#define ARCHIVE_STATUS_ERROR_FILE_NAME_SLASHES          (-20102L)
#define ARCHIVE_STATUS_ERROR_BUNDLE_CMD_FILE_NAME       (-20103L)   
#define ARCHIVE_STATUS_ERROR_BUNDLE_CMD_SKIP_OBJECT     (-20104L)   
#define ARCHIVE_STATUS_ERROR_BUNDLE_CMD_ERROR           (-20105L)
#define ARCHIVE_STATUS_ERROR_OPEN_FILE                  (-20106L)
#define ARCHIVE_STATUS_ERROR_SAVE_CHUNK                 (-20107L)
#define ARCHIVE_STATUS_ERROR_CLOSE_FILE                 (-20108L)
#define ARCHIVE_STATUS_ERROR_BUNDLE_CMD_MAX_OBJECT      (-20109L)   
#define ARCHIVE_STATUS_ERROR_SECURITY_ALERT             (-20199L)

typedef enum
{
    ARCHIVE_STATE_IDLE = 0,
    ARCHIVE_STATE_PARSE_HDR,
    ARCHIVE_STATE_PARSE_CMD_FILE,
    ARCHIVE_STATE_PARSE_CMD_SIGNATURE_FILE,
    ARCHIVE_STATE_OPEN_FILE,
    ARCHIVE_STATE_SAVE_FILE,
    ARCHIVE_STATE_COMPLETE_PENDING_TESTING,
    ARCHIVE_STATE_PARSING_FAILED
} OtaArchiveState_e;

#define TAR_HDR_SIZE            512
#define MAX_SIGNATURE_SIZE      256
#define MAX_FILE_NAME_SIZE      128
#define MAX_SHA256_DIGEST_SIZE  65
#define MAX_BUNDLE_CMD_FILES    8

#ifdef SL_OTA_ARCHIVE_STANDALONE
#define VERSION_STR_SIZE        14      /* sizeof "YYYYMMDDHHMMSS"    */
extern int ltoa(long val, char *buffer);
extern int Report(const char *format, ...);
#define _SlOtaLibTrace(pargs) Report pargs
#endif

typedef struct  
{
	uint8_t  FileNameBuf[MAX_FILE_NAME_SIZE];
	uint8_t  CertificateFileNameBuf[MAX_FILE_NAME_SIZE];
	uint8_t  SignatureBuf[MAX_SIGNATURE_SIZE];
	uint32_t SignatureLen;
	uint8_t  Sha256Digest[MAX_SHA256_DIGEST_SIZE];
	uint16_t Sha256DigestLen;
	uint32_t Secured;
	uint32_t Bundle;
	uint8_t  SavedInFS;
} OtaArchive_BundleFileInfo_t;

typedef struct  
{
    int16_t                     NumFiles;
    int16_t                     NumFilesSavedInFS;
    OtaArchive_BundleFileInfo_t BundleFileInfo[MAX_BUNDLE_CMD_FILES];
    uint8_t                     VerifiedSignature;
    uint16_t                    TotalParsedBytes;
} OtaArchive_BundleCmdTable_t;

typedef struct
{
    /* File info from TAR file header */
	uint8_t  FileNameBuf[MAX_FILE_NAME_SIZE];
	uint8_t *pFileName;
	uint32_t FileSize;
	int16_t  FileType;

    uint32_t ulToken;
    int32_t  lFileHandle;
    uint32_t WriteFileOffset;
} TarObj_t;

typedef struct 
{
    char VersionFilename[VERSION_STR_SIZE+1];
} OtaVersionFile_t;

typedef struct
{
    OtaArchiveState_e           State;              /* internal archive state machine state */
    int32_t                     TotalBytesReceived; /* Should be keeped over states */
    TarObj_t                    CurrTarObj;         /* Current file info from the TAR file itself */
    OtaArchive_BundleCmdTable_t BundleCmdTable;     /* Table of files info from "ota.cmd" */
    int32_t                     SavingStarted;      /* if 1 on error need rollback */
    OtaVersionFile_t            OtaVersionFile;     /* save version file to save on download done */
} OtaArchive_t;

/*
  get a chunk of the the tar file and process that
*/
int16_t OtaArchive_init(OtaArchive_t *pOtaArchive);
int16_t OtaArchive_process(OtaArchive_t *pOtaArchive, uint8_t *pBuf, int16_t BufLen, int16_t *pProcessedBytes);
int16_t OtaArchive_abort(OtaArchive_t *pOtaArchive);
int16_t OtaArchive_getStatus(OtaArchive_t *pOtaArchive);
int16_t OtaArchive_rollback(void);
int16_t OtaArchive_commit(void);
int16_t OtaArchive_getPendingCommit(void);
int16_t OtaArchive_checkVersion(OtaArchive_t *pOtaArchive, uint8_t *pFileName);
int16_t OtaArchive_getCurrentVersion(uint8_t *pVersionBuf);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTA_ARCHIVE_H__ */
