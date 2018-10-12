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
#include <ti/net/ota/source/OtaJson.h>
#include <ti/net/ota/source/CdnClient.h>

#define JSON_OBJ_SIZE               (2048)

/* Max value length of the different fields in the ota.cmd file (in bytes) */
#define MAX_FILENAME_LENGTH         (100)
#define MAX_SIGNATURE_LENGTH        (350)
#define MAX_SHA256DIGEST_LENGTH     (64)
#define MAX_CERTIFICATE_LENGTH      (20)
#define MAX_SECURED_LENGTH          (4)
#define MAX_BUNDLE_LENGTH           (4)

/* Max length of the download URL */
#define MAX_URL_LENGTH              (500)
/* Max length of the Metadata file size and name */
#define MAX_METADATA_FILESIZE       (10)
#define MAX_METADATA_FILENAME       (100)

/****************************************************************************
                      GLOBAL VARIABLES
****************************************************************************/
OtaJson OtaJsonObj = {0};

/*
 * base64 decode module
 * --------------------
 */
uint8_t B64DecTable[256];
uint8_t B64EncTable[64];

extern long atol(const char *_st);

/* Str_FindChar - find first charVal place */
uint8_t *Str_FindChar(uint8_t *pBuf, uint8_t CharVal, int32_t BufLen)
{
    for (; (BufLen >= 0) && (*pBuf != '\0'); pBuf++, BufLen--)
    {
        if (*pBuf == CharVal)
        {
            return pBuf;
        }
    }
    return NULL;
}

/* OtaJson_SkipWhiteSpace - skip all char from SPACE and below  */
uint8_t *OtaJson_SkipAllSpaces(uint8_t *pBuf, int32_t BufLen)
{
    for (; (BufLen >= 0) && (*pBuf != '\0'); pBuf++, BufLen--)
    {
        if (*pBuf>0x20)
        {
            return pBuf;
        }
    }
    return NULL;
}

void B64_Init(void)
{
    int8_t ch;
    int8_t i=0;

    /* build encode table */
    for (ch='A'; ch <= 'Z'; ch++) B64EncTable[i++] = ch;
    for (ch='a'; ch <= 'z'; ch++) B64EncTable[i++] = ch;
    for (ch='0'; ch <= '9'; ch++) B64EncTable[i++] = ch;
    B64EncTable[i++] = '+';
    B64EncTable[i++] = '/';

    /* build decode table */
    for (i=0; i<64; i++)
    {
        B64DecTable[(uint8_t) B64EncTable[i]] = i;
    }
}

uint8_t *B64_Decode(uint8_t *pInputData, int32_t InputLength, uint8_t *pOutputData, int32_t *OutputLength)
{
    uint32_t decode_value;
    uint32_t nibble6_1, nibble6_2, nibble6_3, nibble6_4;
    int32_t i, j;

    if (InputLength % 4 != 0)
    {
        return NULL;
    }

    *OutputLength = InputLength / 4 * 3;
    if (pInputData[InputLength - 1] == '=') (*OutputLength)--;
    if (pInputData[InputLength - 2] == '=') (*OutputLength)--;

    if (pOutputData == NULL)
    {
        return NULL;
    }

    for (i = 0, j = 0; i < InputLength;)
    {
        nibble6_1 = pInputData[i] == '=' ? 0 & i++ : B64DecTable[pInputData[i++]];
        nibble6_2 = pInputData[i] == '=' ? 0 & i++ : B64DecTable[pInputData[i++]];
        nibble6_3 = pInputData[i] == '=' ? 0 & i++ : B64DecTable[pInputData[i++]];
        nibble6_4 = pInputData[i] == '=' ? 0 & i++ : B64DecTable[pInputData[i++]];

        decode_value = (nibble6_1 << 3 * 6) + (nibble6_2 << 2 * 6)  + (nibble6_3 << 1 * 6) + (nibble6_4 << 0 * 6);

        if (j < *OutputLength) pOutputData[j++] = (decode_value >> 2 * 8) & 0xFF;
        if (j < *OutputLength) pOutputData[j++] = (decode_value >> 1 * 8) & 0xFF;
        if (j < *OutputLength) pOutputData[j++] = (decode_value >> 0 * 8) & 0xFF;
    }

    return pOutputData;
}

int16_t OtaJson_getArrayMembersCount(Json_Handle objHandle, const char * pKey)
{
    int16_t retVal;

    retVal = Json_getArrayMembersCount(objHandle, pKey);
    if (retVal < 0)
    {
        /* Failed to get array members count, free dynamic memory and return error */
        OtaJson_destroy();
        return -1;
    }
    /* In case of success return the array count */
    return retVal;
}

int16_t OtaJson_init(char *template, char **text, uint16_t textLen)
{
    int16_t         retVal;
    int16_t         templateSize;
    uint16_t        objSize = JSON_OBJ_SIZE;
    Json_Filename_t templateBuff;

    templateBuff.fileBuffer = template;
    templateSize = strlen(template);

    retVal = Json_createTemplate(&OtaJsonObj.templateHandle , templateBuff.fileBuffer, templateSize);
    if (retVal<0)
    {
        _SlOtaLibTrace(("Error: %d  , Couldn't create template \r\n",retVal));
        return -1;
    }

    retVal = Json_createObject(&OtaJsonObj.jsonObjHandle,OtaJsonObj.templateHandle,objSize);
    if (retVal < 0)
    {
        /* Failed to create an object, free the allocated template */
        Json_destroyTemplate(OtaJsonObj.templateHandle);
        _SlOtaLibTrace(("Error: %d  , Couldn't create Json object \r\n",retVal));
        return -1;
    }

    retVal = Json_parse(OtaJsonObj.jsonObjHandle,*text,textLen);
    if (retVal<0)
    {
        _SlOtaLibTrace(("Error: %d  , Couldn't parse the received Json file \r\n", retVal));

        /* Failed to parse the JSON file, free the allocated memory */
        OtaJson_destroy();

        return retVal;
    }

    return 0;
}

int16_t OtaJson_destroy(void)
{
    int16_t         retVal;

    retVal = Json_destroyObject (OtaJsonObj.jsonObjHandle);
    if (retVal<0)
    {
        return -1;
    }

    retVal = Json_destroyTemplate (OtaJsonObj.templateHandle);
    if (retVal<0)
    {
        return -1;
    }

    memset (&(OtaJsonObj.jsonBuffer.fileBuffer), 0, sizeof(OtaJsonObj.jsonBuffer.fileBuffer));
    memset (&OtaJsonObj.jsonBuffer, 0, sizeof(OtaJsonObj.jsonBuffer));

    return 0;
}

int16_t OtaJson_getFilename (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    uint16_t filenameLen = MAX_FILENAME_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"filename\"", CurrBundleFile->FileNameBuf, &filenameLen);

    if (retVal == JSON_RC__OK)
    {
        /* filenameLen is the actual size of the value that was parsed */
        if (filenameLen == 0)
        {
            /* Free dynamic memory */
            OtaJson_destroy();
            CurrBundleFile->FileNameBuf[0] = 0;
            return ARCHIVE_STATUS_ERROR_BUNDLE_CMD_FILE_NAME;
        }
        else
        {
            /* Parsed the filename successfully */
            CurrBundleFile->FileNameBuf[strlen((char *)CurrBundleFile->FileNameBuf)] = 0;
            return 0;
        }
    }

    /* Parsing file name failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

int16_t OtaJson_getSignature (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    uint16_t SignatureLen = MAX_SIGNATURE_LENGTH;
    int16_t  retVal;
    uint8_t  Base64DecodeBuf[512];

    memset (&Base64DecodeBuf, 0, sizeof(Base64DecodeBuf));

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"signature_base64\"", Base64DecodeBuf, &SignatureLen);

    if (retVal == JSON_RC__OK)
    {
        /* SignatureLen is the actual size of the value that was parsed */
        if (SignatureLen == 0)
        {
            /* set default - no signature */
            CurrBundleFile->SignatureBuf[0] = 0;
            CurrBundleFile->SignatureLen = 0;
        }
        else
        {
            /* signature exists, decode it */
            B64_Decode(Base64DecodeBuf, (int32_t)strlen((const void *)Base64DecodeBuf), CurrBundleFile->SignatureBuf, (int32_t *)&(CurrBundleFile->SignatureLen));
        }
        return 0;
    }

    /* Parsing signature failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

int16_t OtaJson_getSha256Digest (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    CurrBundleFile->Sha256DigestLen = MAX_SHA256DIGEST_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"digest\"", CurrBundleFile->Sha256Digest, &(CurrBundleFile->Sha256DigestLen));

    if (retVal == JSON_RC__OK)
    {
        /* Sha256DigestLen is the actual size of the value that was parsed */
        if (CurrBundleFile->Sha256DigestLen == 0)
        {
            CurrBundleFile->Sha256Digest[0] = 0;
        }
        else
        {
            /* Parsed the SHA 256 digest successfully */
            CurrBundleFile->Sha256Digest[CurrBundleFile->Sha256DigestLen] = 0;
        }
        return 0;
    }

    /* Parsing SHA 256 digest failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

int16_t OtaJson_getCertificate (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    uint16_t CertificateLen = MAX_CERTIFICATE_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"certificate\"", CurrBundleFile->CertificateFileNameBuf, &CertificateLen);

    if (retVal == JSON_RC__OK)
    {
        /* CertificateLen is the actual size of the value that was found */
        if (CertificateLen == 0)
        {
            /* set default - no certificate file name */
            CurrBundleFile->CertificateFileNameBuf[0] = 0;
        }
        return 0;
    }

    /* Parsing certificate failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

int16_t OtaJson_getSecureField (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    uint16_t securedLen = MAX_SECURED_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"secured\"", &(CurrBundleFile->Secured), &securedLen);

    if (retVal == JSON_RC__OK)
    {
        /* securedLen is the actual size of the value that was found */
        if (securedLen == 0)
        {
            /* set default - not secured */
            CurrBundleFile->Secured = 0;
        }
        return 0;
    }

    /* Parsing secure field failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

int16_t OtaJson_getBundleField (OtaArchive_BundleFileInfo_t *CurrBundleFile)
{
    uint16_t bundleLen = MAX_BUNDLE_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, "\"bundle\"", &(CurrBundleFile->Bundle), &bundleLen);

    if (retVal == JSON_RC__OK)
    {
        /* bundleLen is the actual size of the value that was found */
        if (bundleLen == 0)
        {
            /* set default - bundle */
            CurrBundleFile->Bundle = 1;
        }
        return 0;
    }

    /* Parsing bundle field failed, free dynamic memory and return error */
    OtaJson_destroy();
    return -1;
}

/* OtaJson_FindStartObject - go to the first '{' */
uint8_t *OtaJson_FindStartObject(uint8_t *pBuf, uint8_t *pEndBuf)
{
    return Str_FindChar(pBuf, '{', pEndBuf-pBuf);
}

/* OtaJson_FindEndObject - go to end of object '}' */
uint8_t *OtaJson_FindEndObject(uint8_t *pBuf, uint8_t *pEndBuf)
{
    int32_t object_count=1;
    uint8_t *pStart;

    if (pBuf)
    {
        for( ; (pBuf < pEndBuf) && (*pBuf != 0); pBuf++)
        {
            /* Skip internal inside objects */
            if (*pBuf == '{') object_count++;
            if (*pBuf == '}') object_count--;
            if (object_count == 0)
            {
                pStart = ++pBuf;
                pBuf = OtaJson_SkipAllSpaces(pStart, pEndBuf-pBuf);
                return pBuf;
            }
        }
    }
    return NULL;
}

int16_t OtaJson_getURL (uint8_t *pFileUrl, const char *downloadKey)
{
    uint16_t urlLen = MAX_URL_LENGTH;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, downloadKey, pFileUrl, &urlLen);

    if (retVal == JSON_RC__OK)
    {
        /* urlLen is the actual size of the value that was found */
        if (urlLen == 0)
        {
            /* We must receive a URL */
            _SlOtaLibTrace(("[OtaJson_getURL] Url len is zero!"));
            retVal =  CDN_STATUS_ERROR_PARSE_URL_NOT_FOUND;
        }
    }
    else
    {
        /* free dynamic memory */
        _SlOtaLibTrace(("[OtaJson_getURL] Error: %d  , Couldn't get the data \r\n",retVal));
        OtaJson_destroy();
    }

    return retVal;
}

int16_t OtaJson_getMetadataFileName (uint8_t *pFileName, const char *FileNameKey)
{
    uint16_t fileNameLen = MAX_METADATA_FILENAME;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, FileNameKey, pFileName, &fileNameLen);

    if (retVal == JSON_RC__OK)
    {
        /* fileNameLen is the actual size of the value that was found */
        if (fileNameLen == 0)
        {
            /* Didn't receive a file name */
            _SlOtaLibTrace(("[OtaJson_getMetadataFileName] fileName Length is zero!"));
            retVal =  CDN_STATUS_ERROR_PARSE_DIR_NO_FILE_NAME;
        }
    }
    else
    {
        /* free dynamic memory */
        OtaJson_destroy();
        _SlOtaLibTrace(("[OtaJson_getMetadataFileName] Error: %d  , Couldn't get the data \r\n",retVal));
    }

    return retVal;
}

int16_t OtaJson_getMetadataFileSize (uint32_t *pFileSize, const char *fileSizeKey)
{
    uint16_t fileSizeLen = MAX_METADATA_FILESIZE;
    int16_t  retVal;

    retVal = Json_getValue(OtaJsonObj.jsonObjHandle, fileSizeKey, pFileSize, &fileSizeLen);

    if (retVal == JSON_RC__OK)
    {
        /* fileSizeLen is the actual size of the value that was found */
        if (fileSizeLen == 0)
        {
            /* We must receive a URL */
            _SlOtaLibTrace(("[OtaJson_getURL] URL len is zero!"));
            retVal =  CDN_STATUS_ERROR_PARSE_URL_NOT_FOUND;
        }
    }
    else
    {
        /* free dynamic memory */
        OtaJson_destroy();
        _SlOtaLibTrace(("[OtaJson_getURL] Error: %d  , Couldn't get the data \r\n",retVal));
    }

    return retVal;
}

