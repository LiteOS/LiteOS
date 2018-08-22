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
/* Standard includes */
#include <stdlib.h>
#include <stdint.h>

#include <ti/net/ota/otauser.h>
#include <ti/net/ota/source/CdnClient.h>
#include <ti/net/ota/source/OtaArchive.h>
#include <ti/net/ota/source/OtaLib.h>
#include <ti/net/ota/source/OtaHttpClient.h>
#include <ti/net/ota/source/OtaJson.h>

OtaLib_t *pOtaLib;

int16_t OTA_init(OTA_runningMode runningMode, OTA_memBlock* pMemBlock, OTA_eventHandler eventHandler)
{
    _SlOtaLibTrace(("OTA_init: sizeof CdnClient=%d, sizeof OtaArchive=%d\r\n", sizeof(pOtaLib->CdnClient), sizeof(pOtaLib->OtaArchive)));
    _SlOtaLibTrace(("OTA_init: sizeof OtaLib_t=%d, sizeof OTA_memBlock=%d\r\n", sizeof(OtaLib_t), sizeof(OTA_memBlock)));
    if (sizeof(OtaLib_t) > sizeof(OTA_memBlock))
    {
        _SlOtaLibTrace(("OTA_init: ERROR buffer size OtaLib_t=%d < OTA_memBlock=%d\r\n", sizeof(OtaLib_t), sizeof(OTA_memBlock)));
        return OTA_INIT_ERROR;
    }
    pOtaLib = (OtaLib_t *)pMemBlock;

    memset(pOtaLib, 0, sizeof(OtaLib_t));
    pOtaLib->State = OTA_STATE_IDLE;

    _SlOtaLibTrace(("OTA_init: OTA lib version = %s\r\n", OTA_LIB_VERSION));

    CdnClient_Init(&pOtaLib->CdnClient, pOtaLib->NetBuf);
    OtaArchive_init(&pOtaLib->OtaArchive);

    /* init statistics */
    return OTA_STATUS_OK;
}

uint8_t *_ExtractFileVersion(uint8_t *pFileName, uint32_t FileNameSize)
{
    uint8_t *pVersionFileName = pFileName;

    /* VENDOR_DIR cannot be empty - the EXTLIB_OTA_SET_OPT_VENDOR_ID will fail first */
    
    /* minimum 2 slashes - in poll mode, filename must be "/VENDOR_DIR/file.tar", in push mode there are no slashaes */
    while (pVersionFileName != NULL)
    {
        pVersionFileName = (uint8_t *)Str_FindChar(pFileName, '/', FileNameSize);
        if (pVersionFileName != NULL)
        {
            /* go after the slash */
            pFileName = pVersionFileName+1;
        }
    } 

    return pFileName;
}

void _OtaCleanToIdle(OtaLib_t *pOtaLib)
{
    /*_SlOtaLibTrace(("_OtaCleanToIdle: close OTA client and CDN client and back to IDLE\r\n");*/
    CdnClient_CloseServer(&pOtaLib->CdnClient);
    CdnClient_CloseFileServer(&pOtaLib->CdnClient);

    pOtaLib->pOtaFileName = NULL;
    pOtaLib->State = OTA_STATE_IDLE;
    /* don't clean consecutive errors */
}

int16_t OTA_set(OTA_option option, int32_t optionLen, uint8_t *pOptionVal, int32_t flags)
{
    int16_t Status = OTA_STATUS_OK;

    switch (option)
    {
        case EXTLIB_OTA_SET_OPT_SERVER_INFO:
            memcpy(&pOtaLib->OtaServerInfo, pOptionVal, sizeof(Ota_optServerInfo));
            break;

        case EXTLIB_OTA_SET_OPT_VENDOR_ID:
            if (strlen((const char *)pOptionVal) >= MAX_VENDIR_DIR_SIZE)
            {
                return OTA_OPT_ERROR_VENDOR_DIR_SIZE;
            }
            strcpy((char *)pOtaLib->VendorDir, (const char *)pOptionVal);
            break;

        case EXTLIB_OTA_SET_OPT_ACCEPT_UPDATE:
            /* do nothing, the OTA is already on process and not in IDLE state */
            break;

        case EXTLIB_OTA_SET_OPT_DECLINE_UPDATE:

            /* check if after OTA_STATE_CHECK_ARCHIVE_NEW_UPDATE state */
            if (pOtaLib->State != OTA_STATE_REQ_FILE_URL)
            {
                _SlOtaLibTrace(("OTA_set: ERROR EXTLIB_OTA_SET_OPT_DECLINE_UPDATE in wrong state = %d\r\n", pOtaLib->State));
                return OTA_OPT_ERROR_WRONG_STATE;
            }

            /* close all connection from last OTA updates and back to IDLE */
            pOtaLib->ConsecutiveOtaErrors = 0;
            _OtaCleanToIdle(pOtaLib);
            break;

        case EXTLIB_OTA_SET_OPT_IMAGE_COMMIT:

            Status = OtaArchive_commit();
            if (Status < 0)
            {
                _SlOtaLibTrace(("OTA_set: ERROR OtaArchive_Commit, status=%d\r\n", Status));
                return OTA_OPT_ERROR_COMMIT;
            }


            break;

        case EXTLIB_OTA_SET_OPT_IMAGE_ROLLBACK:

            Status = OtaArchive_rollback();
            if (Status < 0)
            {
                if (Status == SL_ERROR_FS_BUNDLE_NOT_IN_CORRECT_STATE)
                {
                    _SlOtaLibTrace(("OTA_set: ERROR OtaArchive_Rollback, status=%d - ignored SL_ERROR_FS_BUNDLE_NOT_IN_CORRECT_STATE\r\n", Status));
                    /* Ignore error calling rollback in the incorrect state */
                    return OTA_STATUS_OK;
                }
                _SlOtaLibTrace(("OTA_set: ERROR OtaArchive_Rollback, status=%d\r\n", Status));
                return OTA_OPT_ERROR_ROLLBACK;
            }
            break;

        default:
            _SlOtaLibTrace(("OTA_set: option %ld is not implemented\r\n", option));
            return OTA_OPT_ERROR_OPTION_CODE;
    }

    return Status;
}

int16_t OTA_get(OTA_option option, int32_t *pOptionLen, uint8_t *pOptionVal)
{
	Ota_optVersionsInfo *pVersionsInfo;
    int16_t Status = OTA_STATUS_OK;

    switch (option)
    {
        case EXTLIB_OTA_GET_OPT_IS_ACTIVE:
            *(int32_t *)pOptionVal = (pOtaLib->State != OTA_STATE_IDLE);
            *pOptionLen = sizeof(int32_t);
            break;

        case EXTLIB_OTA_GET_OPT_VERSIONS:
        
            pVersionsInfo = (Ota_optVersionsInfo *)pOptionVal;
            /* get the current version from ota.dat file */
            OtaArchive_getCurrentVersion(pVersionsInfo->CurrentVersion);
            /* extract the new version from OtaFileName if exists */
            if(pOtaLib->pOtaFileName == NULL)
            {
                /* can be if the OTA not in the right state and still didn't read the file name */
                /* OTA file not exists (no upadte or state before reading the ota new update */
                memset(pVersionsInfo->NewVersion, '0', VERSION_STR_SIZE);
            }
            else
            {
                uint8_t *pVersionFileName = _ExtractFileVersion(pOtaLib->pOtaFileName, MAX_CDN_FILE_NAME_SIZE);
                /* extract version from file */
                memcpy(pVersionsInfo->NewVersion, pVersionFileName, VERSION_STR_SIZE);
            }
            pVersionsInfo->NewVersion[VERSION_STR_SIZE] = 0; /* null terminated string */
            break;

        case EXTLIB_OTA_GET_OPT_IS_PENDING_COMMIT:

            *(int32_t *)pOptionVal = OtaArchive_getPendingCommit();
            break;

        default:
            _SlOtaLibTrace(("OTA_get: option %ld is not implemented\r\n", option));
            Status = OTA_OPT_ERROR_OPTION_CODE;
    }

    return Status;
}

#define BACK_TO_IDLE 0x1

int16_t _OtaCheckConsecutiveErrors(OtaLib_t *pOtaLib, int16_t OtaStatus, int32_t Options)
{
    int16_t Status;

    if (++pOtaLib->ConsecutiveOtaErrors >= MAX_CONSECUTIVE_OTA_ERRORS)
    {
        _SlOtaLibTrace(("\n_OtaCheckConsecutiveErrors: ConsecutiveOtaErrors=%d/%d, MAX_CONSECUTIVE_OTA_ERRORS!!! \r\n", pOtaLib->ConsecutiveOtaErrors, MAX_CONSECUTIVE_OTA_ERRORS));
        pOtaLib->ConsecutiveOtaErrors = 0;
        Status = OTA_RUN_ERROR_CONSECUTIVE_OTA_ERRORS;
    }
    else
    {
        _SlOtaLibTrace(("\n_OtaCheckConsecutiveErrors: ConsecutiveOtaErrors=%d/%d, return only WARNNING\r\n", pOtaLib->ConsecutiveOtaErrors, MAX_CONSECUTIVE_OTA_ERRORS));
        Status = OtaStatus;
    }

    if (Options & BACK_TO_IDLE)
    {
        pOtaLib->State = OTA_STATE_IDLE;
        _OtaCleanToIdle(pOtaLib);
    }

    return Status;
}

int16_t OTA_run()
{
    CdnClient_t *pCdnClient = (CdnClient_t *)&pOtaLib->CdnClient;
    int32_t NumDirFiles;
    int16_t Status;
    int16_t ProcessedBytes;
    uint8_t *pVersionFileName;

    switch (pOtaLib->State)
    {
        case OTA_STATE_IDLE:
            /* serverInfo and vendorDir must be init before running OTA */
            if ((pOtaLib->OtaServerInfo.ServerName[0] == 0) /*|| (pOtaLib->VendorDir[0] == 0)*/)
            {
                return OTA_RUN_ERROR_NO_SERVER_NO_VENDOR;
            }

            pOtaLib->State = OTA_STATE_CONNECT_SERVER;
            break;

        case OTA_STATE_CONNECT_SERVER:
            _SlOtaLibTrace(("OTA_run: call CdnClient_ConnectServer OTA server=%s\r\n", pOtaLib->OtaServerInfo.ServerName));
            Status = CdnClient_ConnectServer(pCdnClient, &pOtaLib->OtaServerInfo);
            if( Status < 0)
            {
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ConnectServer, Status=%ld\r\n", Status));
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_CONNECT_FILE_SERVER, BACK_TO_IDLE);
                return Status;
            }
            pOtaLib->State = OTA_STATE_REQ_OTA_DIR;
            break;

        case OTA_STATE_REQ_OTA_DIR:
            _SlOtaLibTrace(("OTA_run: CdnClient_ReqOtaDir, VendorDir=%s\r\n", pOtaLib->VendorDir));
            NumDirFiles = CdnClient_ReqOtaDir(pCdnClient, pOtaLib->VendorDir);
            if (NumDirFiles < 0)
            {
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ReqOtaDir, Status=%d\r\n", NumDirFiles));
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_OTA_DIR, BACK_TO_IDLE);
                return Status;
            }
            if (NumDirFiles == 0)
            {
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ReqOtaDir - no updates\r\n"));
                pOtaLib->ConsecutiveOtaErrors = 0;
                pOtaLib->State = OTA_STATE_IDLE;
                _OtaCleanToIdle(pOtaLib);
                return OTA_RUN_STATUS_NO_UPDATES;
            }
            _SlOtaLibTrace(("OTA_run: CdnClient_ReqOtaDir, NumDirFiles=%ld\r\n", NumDirFiles));
            pOtaLib->State = OTA_STATE_CHECK_ARCHIVE_NEW_UPDATE;
            break;

        case OTA_STATE_CHECK_ARCHIVE_NEW_UPDATE:

            _SlOtaLibTrace(("OTA_run: CdnClient_GetNextDirFile\r\n"));
            pOtaLib->pOtaFileName = CdnClient_GetNextDirFile(pCdnClient, &pOtaLib->OtaFileSize);

            /* check if last file in the list, still without tar file */
            if( pOtaLib->pOtaFileName == NULL)
            {
                /* tar file not found, return no updates */
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ReqOtaDir - tar not found, no updates\r\n"));
                pOtaLib->ConsecutiveOtaErrors = 0;
                pOtaLib->State = OTA_STATE_IDLE;
                _OtaCleanToIdle(pOtaLib);
                return OTA_RUN_STATUS_NO_UPDATES;
            }

            _SlOtaLibTrace(("OTA_run: CdnClient_GetNextDirFile: file=%s, size=%ld\r\n", pOtaLib->pOtaFileName, pOtaLib->OtaFileSize));

            if (strstr((const char *)pOtaLib->pOtaFileName, ".tar") == NULL) 
            {
                _SlOtaLibTrace(("OTA_run: WARNING, not a tar file, filename=%s\r\n", pOtaLib->pOtaFileName));
                /* Stay on state, check next file version */
                pOtaLib->State = OTA_STATE_CHECK_ARCHIVE_NEW_UPDATE;
                return OTA_RUN_STATUS_CONTINUE;
            }

            /* continue anyway, user app can override this decision */
            pOtaLib->State = OTA_STATE_REQ_FILE_URL;


            /* Init the Tar parser module */
            OtaArchive_init(&pOtaLib->OtaArchive);

            /* host should check the version */
            pVersionFileName = _ExtractFileVersion(pOtaLib->pOtaFileName, MAX_CDN_FILE_NAME_SIZE);
            if (OtaArchive_checkVersion(&pOtaLib->OtaArchive, pVersionFileName))
            {
                return OTA_RUN_STATUS_CHECK_NEWER_VERSION;
            }
            else
            {
                return OTA_RUN_STATUS_CHECK_OLDER_VERSION;
            }

        case OTA_STATE_REQ_FILE_URL:

            _SlOtaLibTrace(("OTA_run: Call CdnClient_ReqFileUrl, filename = %s\r\n", pOtaLib->pOtaFileName));
            Status = CdnClient_ReqFileUrl(pCdnClient, pOtaLib->pOtaFileName, pOtaLib->FileUrlBuf, sizeof(pOtaLib->FileUrlBuf));
            if( Status < 0)
            {
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ReqFileUrl, Status=%d\r\n", Status));
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_FILE_URL, BACK_TO_IDLE);
                return Status;
            }

            /* In this stage the CDN server can be closed */
            CdnClient_CloseServer(&pOtaLib->CdnClient);

            pOtaLib->State = OTA_STATE_CONNECT_FILE_SERVER;
            break;

        case OTA_STATE_CONNECT_FILE_SERVER:

            _SlOtaLibTrace(("OTA_run: Call CdnClient_ConnectFileServer, url = %s\r\n", pOtaLib->FileUrlBuf));
            Status = CdnClient_ConnectFileServer(&pOtaLib->CdnClient, pOtaLib->FileUrlBuf, pOtaLib->OtaServerInfo.SecuredConnection);
            if( Status < 0)
            {
                /* stay on this state for another retry */
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ConnectFileServer, Status=%d\r\n", Status));
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_CONNECT_FILE_SERVER, BACK_TO_IDLE);
                return Status;
            }
            pOtaLib->State = OTA_STATE_REQ_FILE_CONTENT;
            break;

        case OTA_STATE_REQ_FILE_CONTENT:

            _SlOtaLibTrace(("OTA_run: Call CdnClient_ReqFileContent, url = %s\r\n", pOtaLib->FileUrlBuf));
            Status = CdnClient_ReqFileContent(&pOtaLib->CdnClient, pOtaLib->FileUrlBuf);
            if( Status < 0)
            {
                _SlOtaLibTrace(("OTA_run: ERROR CdnClient_ReqFileContent, Status=%d\r\n", Status));
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_REQ_FILE_CONTENT, BACK_TO_IDLE);
                return Status;
            }

            pOtaLib->State = OTA_STATE_PREPARE_DOWNLOAD;
            break;

        case OTA_STATE_PREPARE_DOWNLOAD:

            /* prepare receive buffer */
            pOtaLib->pRecvChunk = pOtaLib->NetBuf;
            pOtaLib->RecvChunkOffset = 0;
            pOtaLib->RecvChunkForceReadMode = 0;

            pOtaLib->RecvChunkSize = CdnClient_RecvSkipHdr(pCdnClient->FileSockId, pOtaLib->pRecvChunk, NET_BUF_SIZE);
            if (0 >= pOtaLib->RecvChunkSize)
            {
                _SlOtaLibTrace(("OTA_run: ERROR downloading, CdnClient_RecvSkipHdr Status=%ld\r\n", pOtaLib->RecvChunkSize));
                /* Stop the parsing of the archive file */
                OtaArchive_abort(&pOtaLib->OtaArchive);
                Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_FILE_HDR, BACK_TO_IDLE);
                return Status;
            }

            pOtaLib->State = OTA_STATE_DOWNLOADING; 
            break;

        case OTA_STATE_DOWNLOADING:

            /* Check if to read more chunk */
            if ((pOtaLib->RecvChunkOffset != 0) || (pOtaLib->RecvChunkSize == 0) || pOtaLib->RecvChunkForceReadMode)
            {
                int16_t UnprocessedSize = pOtaLib->RecvChunkSize-pOtaLib->RecvChunkOffset;
                int16_t Len;

                if (UnprocessedSize >= TAR_HDR_SIZE)
                {
                    /* No need to read more, enough unprocessed bytes to decode next TAR file header */
                    memcpy(&pOtaLib->pRecvChunk[0], &pOtaLib->pRecvChunk[pOtaLib->RecvChunkOffset], UnprocessedSize);
                    Len = UnprocessedSize;
                }
                else
                {
                    Len = CdnClient_RecvAppend(pCdnClient->FileSockId, pOtaLib->pRecvChunk, NET_BUF_SIZE, pOtaLib->RecvChunkSize, pOtaLib->RecvChunkOffset);
                }
                if (0 >= Len)
                {
                    _SlOtaLibTrace(("OTA_run: ERROR downloading, CdnClient_RecvAppend Status=%ld\r\n", Len));
                    /* Stop the parsing of the archive file */  
                    OtaArchive_abort(&pOtaLib->OtaArchive);
                    Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_RECV_APPEND, BACK_TO_IDLE);
                    return Status;
                }
                pOtaLib->RecvChunkOffset = 0;
                pOtaLib->RecvChunkSize = Len;
                pOtaLib->RecvChunkForceReadMode = 0;
            }

            Status = OtaArchive_process(&pOtaLib->OtaArchive, pOtaLib->pRecvChunk, pOtaLib->RecvChunkSize, &ProcessedBytes);
            pOtaLib->RecvChunkForceReadMode = (Status == ARCHIVE_STATUS_FORCE_READ_MORE);
            pOtaLib->RecvChunkOffset += ProcessedBytes;
            if( Status < 0)
            {
                if (Status == ARCHIVE_STATUS_ERROR_SECURITY_ALERT)
                {
                    _SlOtaLibTrace(("OTA_run: SECURITY ALERT OtaArchive_RunParse, Status=%d\r\n", Status));
                    /* On SECURITY ALERT, stop all, no consecutive errors check */
                    pOtaLib->State = OTA_STATE_IDLE;
                    _OtaCleanToIdle(pOtaLib);
                    Status = OTA_RUN_ERROR_SECURITY_ALERT;
                }
                else
                {
                    _SlOtaLibTrace(("OTA_run: ERROR OtaArchive_RunParse, Status=%d\r\n", Status));
                    Status = _OtaCheckConsecutiveErrors(pOtaLib, OTA_RUN_STATUS_CONTINUE_WARNING_FAILED_DOWNLOAD_AND_SAVE, BACK_TO_IDLE);
                }
                return Status;
            }
            else if (Status == ARCHIVE_STATUS_DOWNLOAD_DONE)
            {
                _SlOtaLibTrace(("OTA_run: ---- Download file completed %s\r\n", pOtaLib->pOtaFileName));
                CdnClient_CloseFileServer(&pOtaLib->CdnClient);
                pOtaLib->State = OTA_STATE_WAIT_CONFIRM; /* not real state, cannot go to idle, user app should reset */
                pOtaLib->ConsecutiveOtaErrors = 0;

                return OTA_RUN_STATUS_DOWNLOAD_DONE;
            }
            break;

        case OTA_STATE_WAIT_CONFIRM:
            /* not real state, cannot go to idle, user app should reset */
            _SlOtaLibTrace(("sl_OtaRun ERROR: unexpected state %d\r\n", pOtaLib->State));
            return OTA_RUN_STATUS_DOWNLOAD_DONE;

        default:
            _SlOtaLibTrace(("sl_OtaRun ERROR: unexpected state %d\r\n", pOtaLib->State));
            return OTA_RUN_ERROR_UNEXPECTED_STATE;

    }

    return OTA_RUN_STATUS_CONTINUE;
}

