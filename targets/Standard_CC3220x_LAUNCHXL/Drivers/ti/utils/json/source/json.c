/*
 * Copyright (c) 2014-2018, Texas Instruments Incorporated
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

#include "json.h"

typedef struct
{
    char *  data;
    uint16_t len;
    uint32_t validNum;
}JSON_templateInternal;

typedef struct
{
    char *  jsonInternal;
    uint16_t jsonInternalSize;
    uint16_t jsonInternalSizeMAX;
    JSON_templateInternal *jsonTemplate;
    uint32_t validNum;
}JSON_objectInternal;

/* Header needed in order to create template lib representation */
#define TEMPLATE_MIN_HEADER_SIZE                (12u)
#define VALIDATION_NUMBER                       (0xDEADFACE)

#define ARRAY_TO_OBJ_EXTRA_CHARS_NUM            (6)

extern int sprintf(char *str, const char *format, ...);

/* Utility function - In case input_text contains white spaces at the beginning, skip it */
static void skipWS(char **input_text, uint16_t *length)
{
    uint32_t position = 0;

    while((position < *length) && ((*input_text)[position] <= ' '))
    {
        position++;
    }

    *length -= position;
    *input_text += position;
}

int16_t Json_createTemplate(Json_Handle *templateHandle,
        const char *templateString, uint16_t templateStringLen)
{
    json_rc_T rcode;
    uint16_t minimalTemplateSize = 0;
    JSON_templateInternal *pInternalTemplate;

    pInternalTemplate = (JSON_templateInternal *)(malloc(sizeof(JSON_templateInternal)));
    if (pInternalTemplate)
    {
        pInternalTemplate->data = (char *)malloc(TEMPLATE_MIN_HEADER_SIZE + templateStringLen);
        if (pInternalTemplate->data)
        {
            pInternalTemplate->len = TEMPLATE_MIN_HEADER_SIZE + templateStringLen;
            /* Transforming templateString into lib internal representation */
            rcode = __JSON_Templetize (pInternalTemplate->data, &pInternalTemplate->len, &minimalTemplateSize, templateString, templateStringLen);
            if (rcode == JSON_RC__OK)
            {
                /* Setting the validNum with validation number to validate the Json_Handle */
                pInternalTemplate->validNum = VALIDATION_NUMBER;
                /* Returning the template handle we created */
                *templateHandle = (Json_Handle)pInternalTemplate;
                return(rcode);
            }
            else
            {
                free(pInternalTemplate->data);
                free(pInternalTemplate);
                return(rcode);
            }
        }
        free(pInternalTemplate);
        return(JSON_RC__MEMORY_ALLOCATION_ERROR);
    }
    else
    {
        return(JSON_RC__MEMORY_ALLOCATION_ERROR);
    }
}

int16_t Json_destroyTemplate(Json_Handle templateHandle)
{
    /* Validating template handle */
    if ((templateHandle != 0) && (((JSON_templateInternal *)templateHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_templateInternal *pInternalTemplate = (JSON_templateInternal *)templateHandle;

        free(pInternalTemplate->data);

        /* pessimistically clear the template's state before freeing it */
        pInternalTemplate->data = NULL;
        pInternalTemplate->len = 0;
        /* initialize magic number to 0 */
        pInternalTemplate->validNum = 0;

        free(pInternalTemplate);

        return(JSON_RC__OK);
    }
    return(JSON_RC__INVALID_TEMPLATE_HANDLE);
}

int16_t Json_createObject(Json_Handle *objHandle, Json_Handle templateHandle,
        uint16_t maxObjectSize)
{
    uint16_t  internalInitBuffSize;
    json_rc_T rcode;
    JSON_objectInternal * jsonObject;

    /* Validating template handle */
    if ((templateHandle != 0) && (((JSON_templateInternal *)templateHandle)->validNum == VALIDATION_NUMBER))
    {
        if (maxObjectSize == 0)
        {
            maxObjectSize = JSON_DEFAULT_SIZE;
        }
        /* Filling the json object fields */
        jsonObject = (JSON_objectInternal *)(malloc(sizeof(JSON_objectInternal)));
        if (jsonObject)
        {
            jsonObject->jsonInternal = (char * )(malloc(maxObjectSize));
            jsonObject->jsonInternalSizeMAX = maxObjectSize;
            /* Setting the validNum with validation number to validate the handle */
            jsonObject->validNum = VALIDATION_NUMBER;
            jsonObject->jsonTemplate = (JSON_templateInternal *)templateHandle;

            internalInitBuffSize = jsonObject->jsonInternalSizeMAX;
            /* Creates empty Json ready for filling */
            rcode = __JSON_Init (jsonObject->jsonInternal, &internalInitBuffSize, jsonObject->jsonTemplate->data, jsonObject->jsonTemplate->len) ;
            if (rcode == JSON_RC__OK)
            {
                jsonObject->jsonInternalSize = internalInitBuffSize;
                /* Returning the Json object handle we created */
                *objHandle = (Json_Handle)jsonObject;
            }
            else {
                /* something went south, free memory */
                free(jsonObject->jsonInternal);
                free(jsonObject);
            }

            /* success or fail, return result from __JSON_Init() */
            return(rcode);
        }
        return(JSON_RC__MEMORY_ALLOCATION_ERROR);
    }
    return(JSON_RC__INVALID_TEMPLATE_HANDLE);
}

int16_t Json_destroyObject(Json_Handle objHandle)
{
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal * pJsonInternal = (JSON_objectInternal *)objHandle;
        free(pJsonInternal->jsonInternal);
        /* initialize the validation number to 0 */
        pJsonInternal->validNum = 0;
        free(pJsonInternal);
        return(JSON_RC__OK);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}

int16_t Json_parse(Json_Handle objHandle, char *jsonText, uint16_t jsonTextLen)
{
    json_rc_T rcode;
    char *ArrayToObj = NULL;
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal *pJsonInfo = (JSON_objectInternal *) objHandle;
        /* Validating that the template pointer in the Json object is valid */
        if (pJsonInfo->jsonTemplate->validNum == VALIDATION_NUMBER)
        {
            uint16_t jsonInternalBuffSize = pJsonInfo->jsonInternalSizeMAX;

            /* In case the jsonText contains white spaces at the beginning, skip it */
            skipWS (&jsonText, &jsonTextLen);

            /* In case the first char in the jsonText is "[", i.e., the jsonText is not a Json object, but a Json array, */
            /* convert the Json array to be a value of an object name "#" */
            if (jsonText[0] == '[')
            {
                /* 6 extra characters - {"#": [jsonText in array format] } */
                jsonTextLen += ARRAY_TO_OBJ_EXTRA_CHARS_NUM;

                ArrayToObj = (char * )(malloc(jsonTextLen));
                if (ArrayToObj != NULL)
                {
                    memset(ArrayToObj, 0, jsonTextLen);
                    /* convert the Json array to be a value of an object name "#" */
                    sprintf(ArrayToObj, "{\"#\":%s}",jsonText);
                    jsonText = ArrayToObj;
                }
                else
                {
                    return(JSON_RC__MEMORY_ALLOCATION_ERROR);
                }
            }

            /* Parsing the json text and filling the internal lib representation */
            rcode = __JSON_Parse(pJsonInfo->jsonInternal,&jsonInternalBuffSize,jsonText,jsonTextLen,pJsonInfo->jsonTemplate->data ,pJsonInfo->jsonTemplate->len , 0);

            if (ArrayToObj != NULL)
            {
                free(ArrayToObj);
            }

            if (rcode > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
            {
                /* Update current json internal representation size */
                pJsonInfo->jsonInternalSize = jsonInternalBuffSize;
                return(JSON_RC__OK);
            }
            else
            {
                return(rcode);
            }
        }
        return(JSON_RC__INVALID_TEMPLATE_HANDLE);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}

int16_t Json_getArrayMembersCount(Json_Handle objHandle, const char *pKey)
{
    json_rc_T rcode;
    uint16_t count;
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal * pJsonInfo  = (JSON_objectInternal *)objHandle;
        uint16_t keySize = strlen(pKey);
        /* Return wanted array members count */
        rcode = __JSON_GetArrayMembersCount (&count,pJsonInfo->jsonInternal , pKey, keySize);
        if (rcode == JSON_RC__OK)
        {
            /* In case of success return the array count */
            return(count);
        }
        return(rcode);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}

int16_t Json_getValue(Json_Handle objHandle, const char *pKey, void *pValue,
        uint16_t *maxValueSize)
{
    json_rc_T rcode;
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal *pJsonInfo = (JSON_objectInternal *) objHandle;
        uint16_t keySize = strlen(pKey);
        /* Retrieves wanted value */
        rcode = __JSON_GetValue(pValue, maxValueSize, pJsonInfo->jsonInternal, pKey, keySize) ;
        return (rcode);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}

int16_t Json_setValue(Json_Handle objHandle, const char *pKey, void *pValue,
        uint16_t valueSize)
{
    uint16_t usedSize;
    json_rc_T rcode;
    uint16_t keySize = strlen(pKey);
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal * pJsonInfo = (JSON_objectInternal *)objHandle;
        usedSize = pJsonInfo->jsonInternalSizeMAX;
        /* Set new value into the wanted key */
        rcode = __JSON_SetValue (pJsonInfo->jsonInternal, &usedSize, pValue, valueSize, pKey ,keySize) ;
        if (rcode==JSON_RC__OK)
        {
            /* Updating Json lib representation size after setting the new value */
            pJsonInfo->jsonInternalSize = usedSize;
            return(JSON_RC__OK);
        }
        return(rcode);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}

int16_t Json_build(Json_Handle objHandle, char *pJsonText, uint16_t *maxTxtLen)
{
    json_rc_T rcode;
    /* Validating object handle */
    if ((objHandle != 0) && (((JSON_objectInternal *)objHandle)->validNum == VALIDATION_NUMBER))
    {
        JSON_objectInternal *pJsonInfo =(JSON_objectInternal *)objHandle;
        /* Validating that the template pointer in the Json object is valid */
        if (pJsonInfo->jsonTemplate->validNum == VALIDATION_NUMBER)
        {
            /* Building Json into text string */
            rcode = __JSON_Build (pJsonText, maxTxtLen, pJsonInfo->jsonInternal, pJsonInfo->jsonTemplate->data) ;
            if (rcode>= JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
            {
                /* Making the text null terminated */
                pJsonText[*maxTxtLen] = '\0';
                return(JSON_RC__OK);
            }
            else
            {
                return(rcode);
            }
        }
        return(JSON_RC__INVALID_TEMPLATE_HANDLE);
    }
    return(JSON_RC__INVALID_OBJECT_HANDLE);
}
