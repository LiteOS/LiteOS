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


#ifndef SL_OTA_ARCHIVE_STANDALONE
#include <ti/net/ota/otauser.h>
#endif
#include <ti/net/ota/source/OtaJson.h>
#include <ti/net/ota/source/OtaArchive.h>

/* Tar header format */
#define TAR_FILE_NAME_OFFSET     (0)
#define TAR_FILE_SIZE_OFFSET     (124)
#define TAR_FILE_SIZE_LEN        (12)
#define TAR_FILE_TYPE_OFFSET     (156)
#define TAR_FILE_TYPE_LEN        (1)
#define TAR_FILE_TYPE_DIRECTORY  ('5')
#define TAR_FILE_TYPE_FILE       ('0')

/* bundle cmd file "ota.cmd" and its ECDSA signature "ota.sign" - will not be saved in the file system */
#define GET_ENTIRE_FILE_CONTINUE                          (8)
#define GET_ENTIRE_FILE_DONE                              (9)
#define ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE                (10)
#define ARCHIVE_STATUS_BUNDLE_CMD_DOWNLOAD_DONE           (11)
#define ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_CONTINUE      (12)
#define ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_DOWNLOAD_DONE (13)

#define ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_NOT_VALID     (-14)

#define HEXADECIMAL_BASE                                  (16)

#define BUNDLE_CMD_FILE_NAME                              "ota.cmd"
#define BUNDLE_CMD_SIGNATURE_FILE_NAME                    "ota.sign"
#define OTA_CERTIFICATE_NAME                              "dummy_ota_vendor_cert.der"
#define BUNDLE_CMD_MAX_OBJECT_SIZE                        (1000)
#define OTA_CERTIFICATE_INDEX                             (1)
#define OTA_FORCE_SIGNATURE_VERIFICATION                  (TRUE) /* When set to TRUE, the TAR file MUST contain the bundle cmd signature file */

#define SHA256_DIGEST_SIZE                                CryptoCC32XX_SHA256_DIGEST_SIZE

int16_t GetEntireFile(uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t FileSize, char **pFile);
void    String2Hex(uint8_t* str, uint8_t* hex, uint8_t len);
int16_t verifySignature(char *pSig, uint32_t SigFileSize, uint8_t *pDigest);
int16_t _BundleCmdFile_Parse(OtaArchive_BundleCmdTable_t *pBundleCmdTable, uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t CmdFileSize, uint8_t *pDigest, uint8_t *pInternalBuf);
int16_t _BundleCmdSignatureFile_Parse(OtaArchive_BundleCmdTable_t *pBundleCmdTable, uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t SigFileSize, uint8_t *pDigest);
OtaArchive_BundleFileInfo_t *_BundleCmdFile_GetInfoByFileName(uint8_t *pFileName, OtaArchive_BundleCmdTable_t *pBundleCmdTable);

char template_OtaCmd [] = "{\"bundle\": int32,\"certificate\": string,\"filename\": string,\"secured\": int32,\"signature_base64\": string,\"digest\": string}";

extern OtaJson OtaJsonObj;

CryptoCC32XX_Handle cryptoHandle = NULL;

char * pTempBuf = NULL;

/* Used for parsing the bundle command file */
uint8_t  internalBuf[BUNDLE_CMD_MAX_OBJECT_SIZE] = {0};

/* Set as global for reuse in several HASH calculations */
CryptoCC32XX_HmacParams HmacParams;
uint8_t                 Digest[SHA256_DIGEST_SIZE] = {0};

/* version file module functions */
#define OTA_VERSION_FILENAME    "ota.dat"
int16_t _ReadOtaVersionFile(OtaVersionFile_t *pOtaVersionFile);
int16_t _SaveOtaVersionFile(OtaVersionFile_t *pOtaVersionFile);


/* Tar archive parsing support functions */ 
int32_t OtaArchive_ParseOctec(uint8_t *pBuf, int32_t BufSize);
int16_t OtaArchive_CheckEndOfArchive(uint8_t *pBuf);
int16_t OtaArchive_CloseAbort(int32_t FileHandle);

int16_t GetEntireFile(uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t FileSize, char **pFile)
{
    int16_t        copyLen = 0;
    static bool    firstRun = TRUE;
    static int16_t TotalRecvBufLen = 0;

    if (firstRun)
    {
        TotalRecvBufLen = RecvBufLen;
        firstRun = FALSE;
        if (TotalRecvBufLen < FileSize)
        {
            /* Didn't receive the entire file in the first run. */
            /* Allocate a buffer in the size of the entire file and fill it in each round. */
            pTempBuf = (char*)malloc(FileSize+1);
            if (pTempBuf == NULL)
            {
                /* Allocation failed, return error. */
                return -1;
            }
            memcpy(pTempBuf, (char *)pRecvBuf, RecvBufLen);
            *ProcessedSize = RecvBufLen;

            /* didn't receive the entire file, try in the next packet */
            return GET_ENTIRE_FILE_CONTINUE;
        }
        else
        {
            /* Received the entire file in the first run. */
            /* No additional memory allocation is needed. */
            *ProcessedSize = FileSize;
            *pFile = (char *)pRecvBuf;
        }
    }
    else
    {
        /* Avoid exceeding buffer size (FileSize + 1) */
        if (RecvBufLen > ((FileSize + 1) -TotalRecvBufLen))
        {
            copyLen = ((FileSize + 1) -TotalRecvBufLen);
        }
        else
        {
            copyLen = RecvBufLen;
        }

        /* Copy the received buffer from where we stopped the previous copy */
        memcpy(&(pTempBuf[TotalRecvBufLen]), (char *)pRecvBuf, copyLen);

        *ProcessedSize = copyLen;
        TotalRecvBufLen += copyLen;

        if (TotalRecvBufLen < FileSize)
        {
            /* didn't receive the entire file, try in the next packet */
            return GET_ENTIRE_FILE_CONTINUE;
        }

        /* At this point we have the whole file */
        *pFile = (char *)pTempBuf;
    }

    /* Set static variables to initial values to allow retry in case of a warning during the OTA process */
    firstRun = TRUE;
    TotalRecvBufLen = 0;

    return GET_ENTIRE_FILE_DONE;
}

void String2Hex(uint8_t* str, uint8_t* hex, uint8_t len)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t lenInHex = 0;
    char    tmp[3] = "";
    char    *pTmpEnd = tmp + 1;

    /* Hexadecimal number requires double length when represented as string */
    if (len % 2)
    {
        /* len is odd */
        lenInHex = len/2 + 1;
    }
    else
    {
        lenInHex = len/2;
    }
    for (i=0; i<lenInHex; i++)
    {
        tmp[0] = str[j];
        tmp[1] = str[j+1];
        j += 2;
        hex[i] = strtol(tmp, &pTmpEnd, HEXADECIMAL_BASE);
    }
}

int16_t verifySignature(char *pSig, uint32_t SigFileSize, uint8_t *pDigest)
{
    SlNetUtilCryptoPubKeyInfo_t      *pInfoKey;
    SlNetUtilCryptoCmdKeyMgnt_t      keyAttrib;
    SlNetUtilCryptoCmdVerifyAttrib_t verAttrib;
    uint16_t                         resultLen = 0;
    uint8_t                          buf[SL_NETUTIL_CMD_BUFFER_SIZE];
    uint8_t                          *name;
    uint8_t                          *verifyBuf;
    int16_t                          Status;
    int32_t                          verifyResult;

    /* Allocate a buffer for the signature verification process */
    verifyBuf = (uint8_t *)malloc(SHA256_DIGEST_SIZE + SigFileSize);
    if (verifyBuf == NULL)
    {
        /* Allocation failed, return error. */
        return -1;
    }
 
    keyAttrib.ObjId = OTA_CERTIFICATE_INDEX;
    keyAttrib.SubCmd = SL_NETUTIL_CRYPTO_UNINSTALL_SUB_CMD;
    /* Uninstall the key  to ensure that a new certificate wasn't updated via OTA. Such scenario could replace the key with a certificate */
    Status = sl_NetUtilCmd(SL_NETUTIL_CRYPTO_CMD_INSTALL_OP, (uint8_t *)&keyAttrib,sizeof(SlNetUtilCryptoCmdKeyMgnt_t), NULL, 0 , NULL, &resultLen);

    keyAttrib.ObjId = OTA_CERTIFICATE_INDEX;
    keyAttrib.SubCmd = SL_NETUTIL_CRYPTO_INSTALL_SUB_CMD;
    pInfoKey = (SlNetUtilCryptoPubKeyInfo_t *)buf;
    pInfoKey->KeyAlgo = SL_NETUTIL_CRYPTO_PUB_KEY_ALGO_EC;
    pInfoKey->KeyParams.EcParams.CurveType = SL_NETUTIL_CRYPTO_EC_CURVE_TYPE_NAMED;
    pInfoKey->KeyParams.EcParams.CurveParams.NamedCurveParams = SL_NETUTIL_CRYPTO_EC_NAMED_CURVE_SECP256R1;
    pInfoKey->CertFileNameLen = strlen(OTA_CERTIFICATE_NAME)+1;
    pInfoKey->KeyFileNameLen = 0;
    name = ((_u8 *)pInfoKey) + sizeof(SlNetUtilCryptoPubKeyInfo_t);
    strcpy((char *)name, OTA_CERTIFICATE_NAME);
    /* Install new key from the certificate in the file system */
    Status = sl_NetUtilCmd(SL_NETUTIL_CRYPTO_CMD_INSTALL_OP,(_u8 *)&keyAttrib, sizeof(SlNetUtilCryptoCmdKeyMgnt_t),
                          (_u8 *)pInfoKey, sizeof(SlNetUtilCryptoPubKeyInfo_t) + pInfoKey->CertFileNameLen, NULL, &resultLen);
    if(Status < 0)
    {
        free(verifyBuf);
        /* Failed to install the certificate */
        return Status;
    }

    /* Verify the signature using the installed certificate */
    memcpy(verifyBuf, pDigest, SHA256_DIGEST_SIZE);
    memcpy(verifyBuf + SHA256_DIGEST_SIZE, pSig, SigFileSize);

    verAttrib.Flags = 0;
    verAttrib.ObjId = OTA_CERTIFICATE_INDEX;
    verAttrib.SigType = SL_NETUTIL_CRYPTO_SIG_DIGESTwECDSA;
    verAttrib.MsgLen = SHA256_DIGEST_SIZE;
    verAttrib.SigLen = SigFileSize;

    resultLen = 4;
    Status = sl_NetUtilCmd(SL_NETUTIL_CRYPTO_CMD_VERIFY_MSG, (_u8 *)&verAttrib,
                           sizeof(SlNetUtilCryptoCmdVerifyAttrib_t), verifyBuf, SHA256_DIGEST_SIZE + SigFileSize,
                           (_u8 *)&verifyResult , &resultLen);

    /* In case the signature is not valid, return a special error code */
    if((Status == 0) && (verifyResult < 0))
    {
        free(verifyBuf);
        /* The process succeeded (Status == 0) and the verification failed - Security Alert */
        return ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_NOT_VALID;
    }
    free(verifyBuf);
    return Status;
}

/* OTA Metadata - parse metadata JSON response, example
[
    {
        "filename": "/sys/servicepack.ucf",
        "signature_base64": "p+rEqMPTX8iT1GA348e8KR7I9FeJX7F5oEsBKhF89C0yTH9cIwkXChbQBz9GC4dEIN4rcrR37SnlhTMmxkJw8Dcas7zoz/F95Qiu5bQYeJXwg1AAloIFbG2K9xBXqKIWsSjG56vkad7C/hpXopsMkWTBamWeaudZFoMOb+xbOKCdd/94rxgPpDFKzdbSRrWwKPC0r9h+ToZi+l1ULjRi7fnXm/I8W4BzWlQfIZoj+MFg0el0PFWWKj9M6hPOcygFcyqyf5WRrlzxPc5vv4wDHVSg8DjRUc0jpA9um/4m/6cRtw+DboxP+6FOq0PHwHMXsQ7V052q1YAkQcamFlzflA==",
        "certificate": "",
        "digest":"281277f7aeab79e7a07457e5e2c6f2269a1b857c914e7d25ee637120be4279bf",
        "secured": 1,
        "bundle": 1
    },
    {
        "filename": "/html/html_sample.txt",
        "signature_base64": "",
        "certificate": "",
        "digest":"",
        "secured": 0,
        "bundle": 0
    }
]*/

int16_t _BundleCmdFile_Parse(OtaArchive_BundleCmdTable_t *pBundleCmdTable, uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t CmdFileSize, uint8_t *pDigest, uint8_t *pInternalBuf)
{
    OtaArchive_BundleFileInfo_t CurrBundleFile;
    int16_t                     retVal = 0;
    uint8_t                     *pBuf=pRecvBuf;
    uint8_t                     *pStartFileBuf = NULL;
    uint8_t                     *pEndFileBuf = NULL;
    static uint16_t             internalBufLen = 0;

    *ProcessedSize = 0;

    /* If cryptoHandle was not already created, try to create it */
    /* cryptoHandle is used to calculate SHA-256 of the bundle command file (the digest will be used later on when verifying the bundle command signature) */
    if (cryptoHandle == NULL)
    {
        CryptoCC32XX_init();
        CryptoCC32XX_HmacParams_init(&HmacParams);
        /* The bundle command file received in chunks, therefore the digest calculation will be done in several iterations. */
        HmacParams.moreData = 1;
        cryptoHandle = CryptoCC32XX_open(0, CryptoCC32XX_HMAC);
        if (cryptoHandle == NULL)
        {
            return -1;
        }
    }

    /* The bundle command file is in JSON format (array of JSON objects). */
    /* The JSON parser must receive a valid JSON. Since the bundle file may be */
    /* very large and in order to avoid the need to hold the entire file in RAM, */
    /* split the bundle file to valid JSON objects and parse each one. */
    do
    {
        if (internalBufLen > 0)
        {
            /* If the internal buffer is not empty, find the end object and copy the received data up to it to the internal buffer */
            pEndFileBuf = OtaJson_FindEndObject(pRecvBuf, pRecvBuf+RecvBufLen);
            if (pEndFileBuf == NULL)
            {
                if ((internalBufLen + RecvBufLen) < BUNDLE_CMD_MAX_OBJECT_SIZE)
                {
                    /* Append the entire received buffer to the internal one */
                    memcpy(&(pInternalBuf[internalBufLen]), (char *)pRecvBuf, RecvBufLen);
                    internalBufLen += RecvBufLen;
                    *ProcessedSize = RecvBufLen;
                    return ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE;
                }
                else
                {
                    /* A single JSON object should not exceed the buffer size */
                    /* Please update BUNDLE_CMD_MAX_OBJECT_SIZE */
                    _SlOtaLibTrace(("Internal buffer size exceeded, please update BUNDLE_CMD_MAX_OBJECT_SIZE\r\n"));
                    return -1;
                }
            }
            /* Append the beginning of the received buffer (until the end object) */
            memcpy(&(pInternalBuf[internalBufLen]), (char *)pRecvBuf, pEndFileBuf - pRecvBuf);
            internalBufLen += (pEndFileBuf - pRecvBuf);
            *ProcessedSize = pEndFileBuf - pRecvBuf;
        }
        else
        {
            /* check if the start of the object, { , is in the current buffer, if not try in next packet */
            pStartFileBuf = OtaJson_FindStartObject(pBuf, pRecvBuf+RecvBufLen);
            if (pStartFileBuf == NULL)
            {
                /* No start object in current buffer. */

                /* Verify all bytes were received */
                if ((CmdFileSize - (pBundleCmdTable->TotalParsedBytes)) < RecvBufLen)
                {
                    /* Should get here when all JSON objects were parsed, the remaining bytes are only needed for the HASH calculation */
                    /* Write the last iteration to the HASH module */
                    HmacParams.moreData = 0;
                    CryptoCC32XX_sign(cryptoHandle, CryptoCC32XX_HMAC_SHA256 , pRecvBuf, (CmdFileSize - (pBundleCmdTable->TotalParsedBytes)), pDigest, &HmacParams);
                    /* Increase the number of bytes that was sent to the HMAC module */
                    pBundleCmdTable->TotalParsedBytes += (CmdFileSize - (pBundleCmdTable->TotalParsedBytes));

                    return ARCHIVE_STATUS_BUNDLE_CMD_DOWNLOAD_DONE;
                }
                else
                {
                    /* Write the entire buffer, and ask for the additional data */
                    CryptoCC32XX_sign(cryptoHandle, CryptoCC32XX_HMAC_SHA256 , pRecvBuf, RecvBufLen, pDigest, &HmacParams);
                    /* Increase the number of bytes that was sent to the HASH module */
                    pBundleCmdTable->TotalParsedBytes += RecvBufLen;
                    *ProcessedSize = RecvBufLen;
                    return ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE;
                }
            }

            /* Found start of object, mark the data that is before it as processed */
            *ProcessedSize = pStartFileBuf - pRecvBuf;

            /* check if the whole object, until }, is in the current buffer, if not try in next packet */
            pEndFileBuf = OtaJson_FindEndObject(pStartFileBuf+1, pRecvBuf+RecvBufLen);
            if (pEndFileBuf == NULL)
            {
                /* If pBuf == pRecvBuf didn't find end of object at the entire buffer, copy the received buffer to an internal one */
                if (pBuf == pRecvBuf)
                {
                    memcpy(pInternalBuf, pRecvBuf, RecvBufLen);
                    internalBufLen += RecvBufLen;
                    *ProcessedSize = RecvBufLen;
                    return ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE;
                }
                else
                {
                    /* Before returning write the processed bytes to the HMAC module */
                    CryptoCC32XX_sign(cryptoHandle, CryptoCC32XX_HMAC_SHA256 , pRecvBuf, *ProcessedSize, pDigest, &HmacParams);
                    /* Increase the number of bytes that was sent to the HMAC module */
                    pBundleCmdTable->TotalParsedBytes += *ProcessedSize;

                    /* no need to update *ProcessedSize, object not found, maybe will be in next packet */
                    return ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE;
                }
            }

            /* at this point we have the whole object - the file record */
            *ProcessedSize = pEndFileBuf - pRecvBuf;
            pBuf = pEndFileBuf;
        }

        if (internalBufLen == 0)
        {
            /* Initialize the JSON module */
            retVal = OtaJson_init(template_OtaCmd, (char **)&pStartFileBuf, pEndFileBuf - pStartFileBuf);
        }
        else
        {
            /* Before returning write the processed bytes to the HMAC module */
            CryptoCC32XX_sign(cryptoHandle, CryptoCC32XX_HMAC_SHA256 , pInternalBuf, internalBufLen, pDigest, &HmacParams);
            /* Increase the number of bytes that was sent to the HMAC module */
            pBundleCmdTable->TotalParsedBytes += internalBufLen;

            /* Initialize the JSON module */
            retVal = OtaJson_init(template_OtaCmd, (char **)&pInternalBuf, internalBufLen);
        }

        if (retVal < 0)
        {
            /* Initializing the JSON module failed, return error */
            return retVal;
        }

        /* Initialize data containers before parsing */
        memset (&CurrBundleFile, 0, sizeof(CurrBundleFile));

        /* Get current filename*/
        retVal = OtaJson_getFilename(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing filename failed, return error */
            return retVal;
        }

        /* Get signature of the current file */
        retVal = OtaJson_getSignature(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing signature failed, return error */
            return retVal;
        }

        /* Get SHA 256 digest of the current file */
        retVal = OtaJson_getSha256Digest(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing SHA 256 digest failed, return error */
            return retVal;
        }

        /* Get certificate of the current file */
        retVal = OtaJson_getCertificate(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing certificate failed, return error */
            return retVal;
        }

        /* Check if the current file is secured */
        retVal = OtaJson_getSecureField(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing secureField failed, return error */
            return retVal;
        }

        /* Check if the current file is part of a bundle */
        retVal = OtaJson_getBundleField(&CurrBundleFile);
        if (retVal < 0)
        {
            /* Parsing bundleField failed, return error */
            return retVal;
        }

        /* Finished parsing the current file. Update BundleFileInfo and continue to the next one. */
        if (pBundleCmdTable->NumFiles < MAX_BUNDLE_CMD_FILES)
        {
            pBundleCmdTable->BundleFileInfo[pBundleCmdTable->NumFiles++] = CurrBundleFile;
            _SlOtaLibTrace(("[_BundleCmdFile_Parse]    bundle cmd file=%s, sig_len=%d, SHA_256_Digets=%s,  cert=%s, secured=%d, bundle=%d\r\n", CurrBundleFile.FileNameBuf, CurrBundleFile.SignatureLen, CurrBundleFile.Sha256Digest, CurrBundleFile.CertificateFileNameBuf, CurrBundleFile.Secured, CurrBundleFile.Bundle));
        }
        else
        {
            /* Fatal error, the object already checked before the parsing */
            _SlOtaLibTrace(("[_BundleCmdFile_Parse] ERROR bundle cmd file=%s, place for only %d JSON objects\r\n", CurrBundleFile.FileNameBuf, MAX_BUNDLE_CMD_FILES));
            _SlOtaLibTrace(("                            user can use more files by increasing MAX_BUNDLE_CMD_FILES=%d\r\n", MAX_BUNDLE_CMD_FILES));
            OtaJson_destroy();
            return ARCHIVE_STATUS_ERROR_BUNDLE_CMD_MAX_OBJECT;
        }

        /* In case we finished parsing an object from the internal buffer, return and initialize the internal buffer parameters */
        if(internalBufLen)
        {
            /* Initialize the internal JSON */
            memset (&pInternalBuf, 0, sizeof(pInternalBuf));
            internalBufLen = 0;
            return ARCHIVE_STATUS_BUNDLE_CMD_CONTINUE;
        }

        OtaJson_destroy();
    }while (*pBuf == ',');

    /* Write the last iteration to the HMAC module */
    HmacParams.moreData = 0;
    CryptoCC32XX_sign(cryptoHandle, CryptoCC32XX_HMAC_SHA256 , pRecvBuf, (CmdFileSize - (pBundleCmdTable->TotalParsedBytes)), pDigest, &HmacParams);

    return ARCHIVE_STATUS_BUNDLE_CMD_DOWNLOAD_DONE;
}

int16_t _BundleCmdSignatureFile_Parse(OtaArchive_BundleCmdTable_t *pBundleCmdTable, uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *ProcessedSize, uint32_t SigFileSize, uint8_t *pDigest)
{
    int16_t retVal = 0;
    char *  pSig = NULL;

    /* Get the entire signature file */
    retVal = GetEntireFile(pRecvBuf, RecvBufLen, ProcessedSize, SigFileSize, &pSig);
    if (retVal < 0)
    {
        return retVal;
    }
    if (retVal == GET_ENTIRE_FILE_CONTINUE)
    {
        return ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_CONTINUE;
    }

    /* Verify the signature using ECDSA */
    retVal = verifySignature(pSig, SigFileSize, pDigest);
    if (retVal < 0)
    {
        _SlOtaLibTrace(("[_BundleCmdSignatureFile_Parse] signature verification failed!\r\n"));
        return retVal;
    }

    pBundleCmdTable->VerifiedSignature = 1;

    return ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_DOWNLOAD_DONE;
}

OtaArchive_BundleFileInfo_t *_BundleCmdFile_GetInfoByFileName(uint8_t *pFileName, OtaArchive_BundleCmdTable_t *pBundleCmdTable)
{
    int16_t i;

    for (i=0; i<pBundleCmdTable->NumFiles; i++)
    {
        OtaArchive_BundleFileInfo_t *pBundleFileInfo = &pBundleCmdTable->BundleFileInfo[i];
        if (strcmp((const char *)pFileName, (const char *)pBundleFileInfo->FileNameBuf) == 0)
        {
            return pBundleFileInfo;
        }
    }

    /* not found, use default values */
    return NULL;
}


/*
 * archive archive parsing support functions
 * -------------------------------------
 */
/* Parse an octal number, ignoring leading and trailing nonsense. */
int32_t OtaArchive_ParseOctec(uint8_t *pBuf, int32_t BufSize)
{
    int32_t value = 0;

    while (*pBuf < '0' || *pBuf > '7') 
    {
        ++pBuf;
        --BufSize;
    }
    while (*pBuf >= '0' && *pBuf <= '7' && BufSize > 0) 
    {
        value *= 8;
        value += *pBuf - '0';
        ++pBuf;
        --BufSize;
    }
    return value;
}

/* Returns true if this is 512 zero bytes. */
int16_t OtaArchive_CheckEndOfArchive(uint8_t *pBuf)
{
    int16_t Offset;

    for (Offset = TAR_HDR_SIZE-1; Offset >= 0; --Offset)
    {
        if (pBuf[Offset] != '\0')
        {
            return 0;
        }
    }
    return 1;
}

int16_t OtaArchive_CloseAbort(int32_t FileHandle)
{
    uint8_t abortSig = 'A';
    /* Close the file with signature 'A' which is ABORT */
    return sl_FsClose(FileHandle, 0, &abortSig, 1);
}

/*
 * version file module funtions
 * ----------------------------
 * the archive filename is in the format /OTA_R2/20160301_2_00.tar
 */
int16_t _ReadOtaVersionFile(OtaVersionFile_t *pOtaVersionFile)
{
    int32_t lFileHandle;
    int16_t Status;

    lFileHandle = (int32_t)sl_FsOpen((uint8_t *)OTA_VERSION_FILENAME, SL_FS_READ, 0);
    if(lFileHandle < 0)
    {
        if (lFileHandle == SL_ERROR_FS_FILE_NOT_EXISTS)
        {
            _SlOtaLibTrace(("_ReadOtaVersionFile: file %s, status=SL_ERROR_FS_FILE_NOT_EXISTS\r\n", OTA_VERSION_FILENAME));
        }
        else
        {
            _SlOtaLibTrace(("_ReadOtaVersionFile: Error sl_FsOpen %s, Status=%d\r\n", OTA_VERSION_FILENAME, lFileHandle));
        }
        return (int16_t)lFileHandle;
    }

    Status = (int16_t)sl_FsRead(lFileHandle , 0, (uint8_t *)pOtaVersionFile, sizeof(OtaVersionFile_t));
    if(Status < 0)
    {
        _SlOtaLibTrace(("_ReadOtaVersionFile: Error sl_FsRead, Status=%d\r\n", Status));
        return Status;
    }

    Status = sl_FsClose(lFileHandle, NULL, NULL, 0);
    if(Status < 0)
    {
        _SlOtaLibTrace(("_ReadOtaVersionFile: Error sl_FsClose, Status=%d\r\n", Status));
        return Status;
    }

    return Status;
}

int16_t _SaveOtaVersionFile(OtaVersionFile_t *pOtaVersionFile)
{
    int32_t  lFileHandle;
    int16_t  Status;
    uint32_t ulToken = 0;
    int32_t  OpenFlags = 0;
    
    /* dont delete the file sl_FsDel(OTA_VERSION_FILENAME, ulToken); */

    /* Open the file as bundle !!!*/
    OpenFlags = SL_FS_CREATE;
    OpenFlags |= SL_FS_OVERWRITE;
    OpenFlags |= SL_FS_CREATE_NOSIGNATURE;
    OpenFlags |= SL_FS_CREATE_FAILSAFE;
    OpenFlags |= SL_FS_WRITE_BUNDLE_FILE;

    _SlOtaLibTrace(("_SaveOtaVersionFile: save version file also in the bundle, version=%s\r\n", pOtaVersionFile->VersionFilename));
    lFileHandle = sl_FsOpen((uint8_t *)OTA_VERSION_FILENAME,	OpenFlags| SL_FS_CREATE_MAX_SIZE( sizeof(OtaVersionFile_t) ), (unsigned long *)&ulToken);
    if(lFileHandle < 0)
    {
        _SlOtaLibTrace(("_SaveOtaVersionFile: Error sl_FsOpen %s, Status=%d\r\n", OTA_VERSION_FILENAME, lFileHandle));
        return (int16_t)lFileHandle;
    }

    Status = (int16_t)sl_FsWrite(lFileHandle , 0, (uint8_t *)pOtaVersionFile, sizeof(OtaVersionFile_t));
    if(Status < 0)
    {
        _SlOtaLibTrace(("_SaveOtaVersionFile: Error sl_FsRead, Status=%d\r\n", Status));
        return Status;
    }

    Status = sl_FsClose(lFileHandle, NULL, NULL, 0);
    if(Status < 0)
    {
        _SlOtaLibTrace(("_SaveOtaVersionFile: Error sl_FsClose, Status=%d\r\n", Status));
        return Status;
    }

    return Status;
}

int16_t OtaArchive_init(OtaArchive_t* pOtaArchive)
{
    memset(pOtaArchive, 0, sizeof(OtaArchive_t));
    pOtaArchive->TotalBytesReceived = 0;

    _SlOtaLibTrace(("OtaArchive_Init: OTA archive version = %s\r\n", OTA_ARCHIVE_VERSION));
    pOtaArchive->State = ARCHIVE_STATE_IDLE;
    pOtaArchive->CurrTarObj.lFileHandle = -1;
    pOtaArchive->BundleCmdTable.NumFiles = 0;

    B64_Init();
    return ARCHIVE_STATUS_OK;
}

int16_t OtaArchive_abort(OtaArchive_t *pOtaArchive)
{
    if (pOtaArchive->SavingStarted)
    {
        /* abort current file */
        if (pOtaArchive->CurrTarObj.lFileHandle >= 0)
        {
            OtaArchive_CloseAbort(pOtaArchive->CurrTarObj.lFileHandle);
        }
        /* do rollback */
        OtaArchive_rollback();
    }

    pOtaArchive->State = ARCHIVE_STATE_IDLE;
    pOtaArchive->CurrTarObj.lFileHandle = -1;
    pOtaArchive->BundleCmdTable.NumFiles = 0;
    pOtaArchive->SavingStarted = 0;

    return ARCHIVE_STATUS_OK;
}


int16_t OtaArchive_getStatus(OtaArchive_t *pOtaArchive)
{
    return pOtaArchive->State;
}

int16_t OtaArchive_rollback()
{
    SlFsControl_t FsControl;
    int16_t       Status;

    FsControl.IncludeFilters = 0;
    Status = (int16_t)sl_FsCtl(SL_FS_CTL_BUNDLE_ROLLBACK, 0, NULL ,(uint8_t *)&FsControl, sizeof(SlFsControl_t), NULL, 0 , NULL);
    if( 0 > Status )
    {
        _SlOtaLibTrace(("OtaArchive_Rollback: ERROR sl_FsCtl, Status=%d\r\n", Status));
    }
    return Status;
}

int16_t OtaArchive_getPendingCommit()
{
    SlFsControlGetStorageInfoResponse_t SlFsControlGetStorageInfoResponse;
    int16_t                             Status;

    /* read bundle state and check if is "PENDING COMMIT" */
    Status = (int16_t)sl_FsCtl( ( SlFsCtl_e)SL_FS_CTL_GET_STORAGE_INFO, 0, NULL , NULL , 0, (uint8_t *)&SlFsControlGetStorageInfoResponse, sizeof(SlFsControlGetStorageInfoResponse_t), NULL );
    if (0 > Status)
    {
        _SlOtaLibTrace(("OtaArchive_IsPendingCommit: ERROR sl_FsCtl SL_FS_CTL_GET_STORAGE_INFO, Status=%d\r\n", Status));
        return Status;
    }

    return (SL_FS_BUNDLE_STATE_PENDING_COMMIT == (SlFsBundleState_e)SlFsControlGetStorageInfoResponse.FilesUsage.Bundlestate);
}

int16_t OtaArchive_commit()
{
    SlFsControl_t FsControl;
    int16_t       Status;

    FsControl.IncludeFilters = 0;
    Status = (int16_t)sl_FsCtl(SL_FS_CTL_BUNDLE_COMMIT, 0, NULL ,(uint8_t *)&FsControl, sizeof(SlFsControl_t), NULL, 0 , NULL);
    if( 0 > Status )
    {
        _SlOtaLibTrace(("OtaArchive_Commit: ERROR sl_FsCtl, Status=%d\r\n", Status));
    }
    return Status;
}

int16_t OtaArchive_checkVersion(OtaArchive_t *pOtaArchive, uint8_t *pVersionFileName)
{
    int16_t  Status = 0;
    uint8_t  OldVersionBuf[VERSION_STR_SIZE+1];
    uint8_t  NewVersionBuf[VERSION_STR_SIZE+1];
    uint32_t OldVersion;
    uint32_t NewVersion;

    memcpy(NewVersionBuf, pVersionFileName, VERSION_STR_SIZE);
    NewVersionBuf[VERSION_STR_SIZE]=0;
    NewVersion = atol((const char *)NewVersionBuf);

    Status = _ReadOtaVersionFile(&pOtaArchive->OtaVersionFile);
    if(Status < 0)
    {
        _SlOtaLibTrace(("OtaArchive_CheckVersion: can't open version file, sign it as old version\r\n", Status));
        _SlOtaLibTrace(("OtaArchive_CheckVersion: accept the new version = %s\r\n", pVersionFileName));
        Status = 1;
    }
    else
    {
        /* compare versions - only 8 digits */
        memcpy(OldVersionBuf, pOtaArchive->OtaVersionFile.VersionFilename, VERSION_STR_SIZE);
        OldVersionBuf[VERSION_STR_SIZE]=0;
        OldVersion = atol((const char *)OldVersionBuf);

        _SlOtaLibTrace(("    OtaArchive_CheckVersion: current version str = %s, decimal = %d\r\n", OldVersionBuf, OldVersion));
        _SlOtaLibTrace(("    OtaArchive_CheckVersion: new     version str = %s, decimal = %d\r\n", NewVersionBuf, NewVersion));
        if (NewVersion > OldVersion)
        {
            _SlOtaLibTrace(("    OtaArchive_CheckVersion: newer version update - %s\r\n", NewVersionBuf));
            Status = 1;
        }
        else
        {
            _SlOtaLibTrace(("    OtaArchive_CheckVersion: older version update - %s\r\n", NewVersionBuf));
            Status = 0;
        }
    }

    /* Keep the new version as candidate to the new version, will be saved if download complete */
    strcpy((char *)&pOtaArchive->OtaVersionFile.VersionFilename, (const char *)NewVersionBuf);

    return Status;
}

int16_t OtaArchive_getCurrentVersion(uint8_t *pVersionBuf)
{
    OtaVersionFile_t OtaVersionFile;
    int16_t          Status;

    Status = _ReadOtaVersionFile(&OtaVersionFile);
    if(Status < 0)
    {
        /* if no file set version to 0000000000 */
        memset(pVersionBuf, '0', VERSION_STR_SIZE);
    }
    else
    {
        /* extract version from file */
        memcpy(pVersionBuf, OtaVersionFile.VersionFilename, VERSION_STR_SIZE);
    }

    pVersionBuf[VERSION_STR_SIZE] = 0; /* null terminated string */
    return Status;
}

int16_t OtaArchive_process(OtaArchive_t *pOtaArchive, uint8_t *pRecvBuf, int16_t RecvBufLen, int16_t *RecvBufProcessed)
{
    OtaArchive_BundleFileInfo_t *pBundleFileInfo;
    TarObj_t                    *pTarObj = &pOtaArchive->CurrTarObj;
    int16_t                     Status;
    int16_t                     FileWriteChunkSize;
    int16_t                     ProcessedSize;
    int16_t                     SkipAlignSize;
    uint32_t                    FsOpenFlags;
    int32_t                     FileSize;
    uint32_t                    ulToken=0;
    uint8_t                     *pDigest = Digest;
    uint8_t                     *pInternalBuf = internalBuf;

    *RecvBufProcessed = 0;

    switch (pOtaArchive->State)
    {
        case ARCHIVE_STATE_IDLE:
            
            pOtaArchive->TotalBytesReceived = 0;

            _SlOtaLibTrace(("OtaArchive_RunParse: set state=ARCHIVE_STATE_PARSE_HDR\r\n"));
            pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
            pOtaArchive->BundleCmdTable.NumFiles = 0;
            pTarObj->lFileHandle = -1;
            break;

        case ARCHIVE_STATE_PARSE_HDR:

            _SlOtaLibTrace(("OtaArchive_RunParseTar: parsing archive file header\r\n"));
            /* Check alignment - Tar file header must be 512 bytes alignment */
            if (pOtaArchive->TotalBytesReceived % TAR_HDR_SIZE)
            {
                SkipAlignSize = TAR_HDR_SIZE - (pOtaArchive->TotalBytesReceived % TAR_HDR_SIZE);
                if (RecvBufLen <= SkipAlignSize)
                {
                    _SlOtaLibTrace(("OtaArchive_RunParseTar: not enough to skip RecvBufLen=%d, SkipAlignSize=%d\r\n", RecvBufLen, SkipAlignSize));
                    /* not enough length to skip the header alignment, Skip only the receive length, try with next packet */
                    *RecvBufProcessed += RecvBufLen;
                    pOtaArchive->TotalBytesReceived += RecvBufLen;
                    return ARCHIVE_STATUS_CONTINUE;
                }
                _SlOtaLibTrace(("OtaArchive_RunParseTar: skip block align  RecvBufLen=%d, SkipAlignSize=%d\r\n", RecvBufLen, SkipAlignSize));
                /* Skip block alignment */
                pRecvBuf += SkipAlignSize;
                RecvBufLen -= SkipAlignSize;
                *RecvBufProcessed += SkipAlignSize;
                pOtaArchive->TotalBytesReceived += SkipAlignSize;
            }

            /* Check enough data for TAR header - 512 bytes */
            if (TAR_HDR_SIZE > RecvBufLen)
            {
                if ((*RecvBufProcessed) == 0)
                {
                	/* Nothing processed - force read more data  */
                	return ARCHIVE_STATUS_FORCE_READ_MORE;
                }
                return ARCHIVE_STATUS_CONTINUE;
            }

            /* processed 512 bytes header */
            *RecvBufProcessed += TAR_HDR_SIZE;
            pOtaArchive->TotalBytesReceived += TAR_HDR_SIZE;

            /* Check end of Tar file */
            if (OtaArchive_CheckEndOfArchive(pRecvBuf)) 
            {
                _SlOtaLibTrace(("OtaArchive_RunParseTar: End of archive...\r\n"));

                /* In case force OTA signature verification is set, verify the signature was part of the TAR */
                if (OTA_FORCE_SIGNATURE_VERIFICATION == TRUE)
                {
                    if(pOtaArchive->BundleCmdTable.VerifiedSignature == 0)
                    {
                        _SlOtaLibTrace(("Signature file is missing from the TAR\r\n"));
                        pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                        OtaArchive_rollback();
                        return ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                    }
                }

                /* Verify all the files that are mentioned in the ota.cmd */
                /* were in the TAR and saved to the FileSystem successfully */
                if (pOtaArchive->BundleCmdTable.NumFiles == pOtaArchive->BundleCmdTable.NumFilesSavedInFS)
                {
                    /* save the version file with the new version */
                    /* The save is in bundle mode, if the update will be decline or failed, the bundle rollback will go back to the old file */
                    _SaveOtaVersionFile(&pOtaArchive->OtaVersionFile);

                    pOtaArchive->State = ARCHIVE_STATE_COMPLETE_PENDING_TESTING;
                    return ARCHIVE_STATUS_DOWNLOAD_DONE;
                }

                _SlOtaLibTrace(("Files are missing in the TAR\r\n"));
                pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                OtaArchive_rollback();
                return ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
            }

            /* Extract file info from the file header */
            memcpy(pTarObj->FileNameBuf, &pRecvBuf[TAR_FILE_NAME_OFFSET], MAX_FILE_NAME_SIZE);
            pTarObj->pFileName = pTarObj->FileNameBuf;
            pTarObj->FileSize = OtaArchive_ParseOctec(&pRecvBuf[TAR_FILE_SIZE_OFFSET], TAR_FILE_SIZE_LEN);
            pTarObj->FileType = pRecvBuf[TAR_FILE_TYPE_OFFSET];

            /* parse directory or file only */
            switch (pTarObj->FileType) 
            {
                case TAR_FILE_TYPE_DIRECTORY: 

                    /* skip directory name, the filenames in the archive file are with the full path */
                    _SlOtaLibTrace(("OtaArchive_RunParseTar: filetype=%c, directory=%s\r\n", pTarObj->FileType, pTarObj->pFileName));
                    pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
                    break;

                case TAR_FILE_TYPE_FILE:
                case '\0':

                    /* TAR file names are always in the form        "<dir>/<number>/filename", e.g "20151230/1/sys/servicepack.ucf" */
                    /* SFLASH file names are always without the     "<dir>/<number>/"          e.g            /sys/servicepack.ucf  */

                    /* go to the second slash, file name is         "<dir>/<number>/filename" */
                    pTarObj->pFileName = Str_FindChar(pTarObj->pFileName, '/', sizeof(pTarObj->FileNameBuf));
                    if (pTarObj->pFileName != NULL)
                    {
                        /* skip second slash */
                        pTarObj->pFileName = Str_FindChar(pTarObj->pFileName+1, '/', sizeof(pTarObj->FileNameBuf));
                    }
                    if (pTarObj->pFileName == NULL)
                    {
                        _SlOtaLibTrace(("OtaArchive_RunParseTar: error in filename pattern\r\n"));
                        OtaArchive_rollback();
                        pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                        return ARCHIVE_STATUS_ERROR_FILE_NAME_SLASHES;
                    }
                    /* Root files are always without slash - because of certificate name limitation */
                    /* FileName in root directory - if only one slash, ignore it */
                    if (Str_FindChar(pTarObj->pFileName+1, '/', strlen((const char *)pTarObj->FileNameBuf)) == NULL)
                    {
                        pTarObj->pFileName++;
                    }

                    _SlOtaLibTrace(("OtaArchive_RunParseTar: FileType=%c, FileName=%s, FileSize=%ld\r\n", pTarObj->FileType, pTarObj->pFileName, pTarObj->FileSize));
                    pTarObj->WriteFileOffset = 0;

                    /* check if bundle cmd file */
                    if (strcmp((const char *)pTarObj->pFileName, BUNDLE_CMD_FILE_NAME) == 0)
                    {
                        /* read the bundle cmd file */
                        pOtaArchive->State = ARCHIVE_STATE_PARSE_CMD_FILE;
                    }
                    else if (strcmp((const char *)pTarObj->pFileName, BUNDLE_CMD_SIGNATURE_FILE_NAME) == 0)
                    {
                        /* read the bundle cmd signature file */
                        pOtaArchive->State = ARCHIVE_STATE_PARSE_CMD_SIGNATURE_FILE;
                    }
                    else
                    {
                        /* open file storage and then read the file */
                        pOtaArchive->State = ARCHIVE_STATE_OPEN_FILE;
                    }
                    break;

                default: /* type=Unsupported */
                    _SlOtaLibTrace(("OtaArchive_RunParseTar: unsupported FileType=%d, FileName=%s\r\n", pTarObj->FileType, pTarObj->pFileName));
                    pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
                    break;
            }
            break;

        case ARCHIVE_STATE_PARSE_CMD_FILE:
            /* Parse BundleCmdFile */
            Status = _BundleCmdFile_Parse(&pOtaArchive->BundleCmdTable, pRecvBuf, RecvBufLen, &ProcessedSize, pTarObj->FileSize, pDigest, pInternalBuf);
            if (Status < 0)
            {
                /* No need to rollback, save files not started */
                pOtaArchive->State = ARCHIVE_STATE_IDLE;
                _SlOtaLibTrace(("OtaArchive_RunParseTar: ERROR in _BundleCmdFile_Parse, Status=%ld\r\n", Status));
                return Status;
            }

            *RecvBufProcessed += ProcessedSize; 
            pOtaArchive->TotalBytesReceived += ProcessedSize;

            if (Status == ARCHIVE_STATUS_BUNDLE_CMD_DOWNLOAD_DONE)
            {
                /* Complete parsing bundle cmd file, go to next file */
               pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
            }
            break;

        case ARCHIVE_STATE_PARSE_CMD_SIGNATURE_FILE:
            /* Parse the bundle cmd signature file */
            Status = _BundleCmdSignatureFile_Parse(&pOtaArchive->BundleCmdTable, pRecvBuf, RecvBufLen, &ProcessedSize, pTarObj->FileSize, pDigest);
            if (Status < 0)
            {
                pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                _SlOtaLibTrace(("OtaArchive_RunParseTar: ERROR in _BundleCmdSignatureFile_Parse, Status=%ld\r\n", Status));
                if (Status == ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_NOT_VALID)
                {
                    Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                }
                return Status;
            }

            *RecvBufProcessed += ProcessedSize;
            pOtaArchive->TotalBytesReceived += ProcessedSize;

            if (Status == ARCHIVE_STATUS_BUNDLE_CMD_SIGNATURE_DOWNLOAD_DONE)
            {
                /* Completed bundle cmd signature verification, go to next file */
               pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
            }
            break;

        case ARCHIVE_STATE_OPEN_FILE:

            _SlOtaLibTrace(("   Create/Open for write file %s\r\n", pTarObj->pFileName));

            /* get bundle cmd info for that file, optionally exists, this for open file flags */
            pBundleFileInfo = _BundleCmdFile_GetInfoByFileName(pTarObj->pFileName, &pOtaArchive->BundleCmdTable);

            /* Set open default flags  - non-secured, bundle mode, actual file size */
            FileSize = pTarObj->FileSize; 
            FsOpenFlags = SL_FS_CREATE;
            FsOpenFlags |= SL_FS_OVERWRITE;
            FsOpenFlags |= SL_FS_CREATE_NOSIGNATURE;
            FsOpenFlags |= SL_FS_WRITE_BUNDLE_FILE;
            FsOpenFlags |= SL_FS_CREATE_FAILSAFE;
            
            /* check if there was entry in bundle command file with other flags */
            if (pBundleFileInfo)
            {
                if (pBundleFileInfo->SavedInFS)
                {
                    /* File with the same name was already found in the TAR and saved to the file system */
                    /* return error */
                    Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                    OtaArchive_rollback();
                    pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                    return Status;
                }
                if (pBundleFileInfo->Bundle == 0)
                {
                    FsOpenFlags &= ~SL_FS_WRITE_BUNDLE_FILE;
                    FsOpenFlags &= ~SL_FS_CREATE_FAILSAFE;
                }
                if (pBundleFileInfo->Secured)
                {
                    FsOpenFlags |= SL_FS_CREATE_PUBLIC_WRITE;
                    FsOpenFlags |= SL_FS_CREATE_SECURE;
                    if (pBundleFileInfo->SignatureBuf && pBundleFileInfo->SignatureLen)
                    {
                        FsOpenFlags &= ~SL_FS_CREATE_NOSIGNATURE;
                    }
                }
            }
            else
            {
                /* All files in the TAR must be mentioned in the bundle command file */
                _SlOtaLibTrace(("\r\n A file was found in the TAR without being mentioned in the bundle command file \r\n"));
                Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                OtaArchive_rollback();
                pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                return Status;
            }

            /*  create a user file */
            pTarObj->lFileHandle = sl_FsOpen((uint8_t *)pTarObj->pFileName,	FsOpenFlags| SL_FS_CREATE_MAX_SIZE( FileSize ), (unsigned long *)&ulToken);

            if(pTarObj->lFileHandle < 0)
            {
                if (pTarObj->lFileHandle == SL_ERROR_FS_INVALID_TOKEN_SECURITY_ALERT)
                {
                    _SlOtaLibTrace(("\r\n"));
                    _SlOtaLibTrace(("OtaArchive_RunParseTar: !!!!!! SECURITY ALERT !!!!! on pOpenFile, Status=%ld\r\n", pTarObj->lFileHandle));
                    _SlOtaLibTrace(("\r\n"));
                    Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                }
                else
                {
                    _SlOtaLibTrace(("OtaArchive_RunParseTar ERROR from sl_FsOpen, Status=%d\r\n", pTarObj->lFileHandle));
                    Status = ARCHIVE_STATUS_ERROR_OPEN_FILE;
                }
                OtaArchive_rollback();
                pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                return Status;
            }

            /* Initialize the HMAC parameters before a new calculation */
            CryptoCC32XX_HmacParams_init(&HmacParams);
            HmacParams.moreData = 1;

            pOtaArchive->State = ARCHIVE_STATE_SAVE_FILE;

            break;
            
        case ARCHIVE_STATE_SAVE_FILE:
            
            FileWriteChunkSize = RecvBufLen;

            /* Check end of file */
            if ((pTarObj->WriteFileOffset + (int32_t)RecvBufLen) >= pTarObj->FileSize)
            {
                /* just bytes belong to this file */
                FileWriteChunkSize = (int16_t)(pTarObj->FileSize - pTarObj->WriteFileOffset);
                HmacParams.moreData = 0;
            }

            /* Write the packet to the file */
            Status = (int16_t)sl_FsWrite((int32_t)pTarObj->lFileHandle, pTarObj->WriteFileOffset, (uint8_t *)pRecvBuf, (uint32_t)FileWriteChunkSize);
            if (Status < 0)
            {
                _SlOtaLibTrace(("OtaArchive_RunParseTar: Error - SaveFileChunk - (%ld)\r\n", Status));
                OtaArchive_CloseAbort(pTarObj->lFileHandle);
                OtaArchive_rollback();
                pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
                return ARCHIVE_STATUS_ERROR_SAVE_CHUNK;
            }
            FileWriteChunkSize = Status; /* actual write size */
            pTarObj->WriteFileOffset += FileWriteChunkSize;

            *RecvBufProcessed += FileWriteChunkSize; 
            pOtaArchive->TotalBytesReceived += FileWriteChunkSize;

            _SlOtaLibTrace(("OtaArchive_RunParseTar: Write size %ld ",FileWriteChunkSize));
            _SlOtaLibTrace(("to file %s ", pTarObj->pFileName));
            _SlOtaLibTrace(("total %ld.\r\n", pTarObj->WriteFileOffset ));

            if (HmacParams.moreData)
            {
                CryptoCC32XX_verify(cryptoHandle, CryptoCC32XX_HMAC_SHA256, (uint8_t *)pRecvBuf, FileWriteChunkSize, NULL, &HmacParams);
            }
            else
            {
                /* get TAR metadata file, optionally exists */
                /* Needed for the last iteration of the hash verification */
                pBundleFileInfo = _BundleCmdFile_GetInfoByFileName(pTarObj->pFileName, &pOtaArchive->BundleCmdTable);
                if (pBundleFileInfo)
                {
                    /* In case this file has a digest field in the ota.cmd file, send it to the verify function */
                    if (pBundleFileInfo->Sha256DigestLen)
                    {
                        /* Convert the digest from string to bytes array in HEX format */
                        String2Hex(pBundleFileInfo->Sha256Digest, pDigest, pBundleFileInfo->Sha256DigestLen);

                        if (CryptoCC32XX_verify(cryptoHandle, CryptoCC32XX_HMAC_SHA256, (uint8_t *)pRecvBuf, FileWriteChunkSize, pDigest, &HmacParams) == CryptoCC32XX_STATUS_SUCCESS)
                        {
                            _SlOtaLibTrace(("\r\n Hash verification succeeded.\r\n"));
                        }
                        else
                        {
                            _SlOtaLibTrace(("\r\n Hash verification failed.\r\n"));
                            Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
 	                        OtaArchive_CloseAbort(pTarObj->lFileHandle);
                            OtaArchive_rollback();
                            pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;
							return Status;
                        }
                    }
                }
            }

            /* check EOF */
            if (pTarObj->WriteFileOffset == pTarObj->FileSize)
            {
                uint8_t  *pCertFileName = 0;
                 int32_t SignLen=0;
                uint8_t  *pSignBuf=0;

                _SlOtaLibTrace(("    Total archive file bytes %ld.\r\n", pOtaArchive->TotalBytesReceived ));
 
                /* get TAR metadata file, optionally exists */
                pBundleFileInfo = _BundleCmdFile_GetInfoByFileName(pTarObj->pFileName, &pOtaArchive->BundleCmdTable);
                if (pBundleFileInfo)
                {
                    if (pBundleFileInfo->SignatureLen != 0)
                    {
                        pSignBuf = pBundleFileInfo->SignatureBuf;
                        SignLen = sizeof(pBundleFileInfo->SignatureBuf);
                    }
                    if (pBundleFileInfo->CertificateFileNameBuf[0] != 0)
                    {
                        pCertFileName = pBundleFileInfo->CertificateFileNameBuf;
                    }
                }

                pOtaArchive->SavingStarted = 1; /* flag needed on Abort/Rollback */
                Status = sl_FsClose(pTarObj->lFileHandle, (uint8_t *)pCertFileName , (uint8_t *)pSignBuf, SignLen);
                if (Status < 0)
                {
                    if ((Status == SL_ERROR_FS_CERT_IN_THE_CHAIN_REVOKED_SECURITY_ALERT) ||
                        (Status == SL_ERROR_FS_WRONG_SIGNATURE_SECURITY_ALERT) ||
                        (Status == SL_ERROR_FS_CERT_CHAIN_ERROR_SECURITY_ALERT) ||
                        (Status == SL_ERROR_FS_INVALID_TOKEN_SECURITY_ALERT) ||
                        (Status == SL_ERROR_FS_SECURITY_ALERT))
                    {
                        _SlOtaLibTrace(("\r\n"));
                        _SlOtaLibTrace(("OtaArchive_RunParseTar: !!!!!! SECURITY ALERT !!!!! on pCloseFile, Status=%ld\r\n", Status));
                        _SlOtaLibTrace(("\r\n"));
                        Status = ARCHIVE_STATUS_ERROR_SECURITY_ALERT;
                    }
                    else
                    {
                        _SlOtaLibTrace(("OtaArchive_RunParseTar: error on pCloseFile, Status=%ld\r\n", Status));
                        Status = ARCHIVE_STATUS_ERROR_CLOSE_FILE;
                    }
                    OtaArchive_CloseAbort(pTarObj->lFileHandle);
                    OtaArchive_rollback();
                    pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;

                    return Status;
                }
               
                /* In case this file is mentioned in the ota.cmd file */
                /* mark that we saved this file, and increase the number of files we saved */
                if (pBundleFileInfo)
                {
                    pBundleFileInfo->SavedInFS = 1;
                    pOtaArchive->BundleCmdTable.NumFilesSavedInFS++;

                    _SlOtaLibTrace(("OtaArchive_RunParseTar: %d files that are mentioned in the ota.cmd were saved\r\n", pOtaArchive->BundleCmdTable.NumFilesSavedInFS));
                }

                _SlOtaLibTrace(("OtaArchive_RunParseTar: Downloading File Completed - Size=%ld\r\n", pTarObj->WriteFileOffset));
                pOtaArchive->State = ARCHIVE_STATE_PARSE_HDR;
                return ARCHIVE_STATUS_CONTINUE;
            }

            break;

        case ARCHIVE_STATE_COMPLETE_PENDING_TESTING:
            _SlOtaLibTrace(("OtaArchive_RunParseTar, ERROR state COMPLETE_PENDING_TESTING, User should not call run parser in this state %d\r\n"));
            /* Nothing to do here, Tar file parsing already completed  */
            /* User should do:
               1. Reset the MCU in order to test the bundle
               2. Check the start Status (connection, get IP, other app tests) 
               3. On success start, check if pending commit and set commit the bundle
               4. On failure, reset or set rollback
            */
            break;

        case ARCHIVE_STATE_PARSING_FAILED:
            _SlOtaLibTrace(("OtaArchive_RunParseTar, ERROR state ARCHIVE_STATE_PARSING_FAILED, User should not call init to restart the process %d\r\n"));
            /* Nothing to do here, Tar file parsing failed, user should init first  */
            break;
            
        default:
            _SlOtaLibTrace(("OtaArchive_RunParseTar: ERROR unexpected state %d\r\n", pOtaArchive->State));
            OtaArchive_rollback();
            pOtaArchive->State = ARCHIVE_STATE_PARSING_FAILED;

    }

    return ARCHIVE_STATUS_CONTINUE;
}
