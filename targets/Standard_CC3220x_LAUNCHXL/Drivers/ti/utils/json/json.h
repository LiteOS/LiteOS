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
/**
 *  @file  ti/utils/json/json.h
 *
 *  @addtogroup ti_utils_json_JSON JSON Parser/Builder
 *
 *  @brief      The JSON library provides APIs to parse and build
 *              JSON objects.
 *
 *  ## Library Usage ##
 *
 *  To use the JSON APIs, the application should include its header file
 *  as follows:
 *  @code
 *  #include <ti/utils/json/json.h>
 *  @endcode
 *
 *  And, add the following JSON library to the link line:
 *  @code
 *  .../source/ti/utils/json/{toolchain}/{isa}/json_{profile}.a
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_utils_json_Json__include
#define ti_utils_json_Json__include

/*! @ingroup ti_utils_json_JSON */
/*@{*/

/*
 *  ======== json.h ========
 *  @brief Contains JSON library APIs
 */

/*
 * ==========================json scenario examples=========================
 *
 *  Create json from       | Parse the json and get | Parse the json, change
 *  scratch and build it   | its value              | its value and build it
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_createTemplate | | |Json_createTemplate | | |Json_createTemplate |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *             |           |            |           |            |
 *             v           |            v           |            v
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_createObject   | | |Json_createObject   | | |Json_createObject   |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *             |           |            |           |            |
 *             v           |            v           |            v
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_setValue       | | |Json_parse          | | |Json_parse          |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *             |           |            |           |            |
 *             v           |            v           |            v
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_build          | | |Json_getValue       | | |Json_getValue       |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *             |           |            |           |            |
 *             v           |            v           |            v
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_destroyObject  | | |Json_destroyObject  | | |Json_setValue       |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *             |           |            |           |            |
 *             v           |            v           |            v
 *  +--------------------+ | +--------------------+ | +--------------------+
 *  |Json_destroyTemplate| | |Json_destroyTemplate| | |Json_build          |
 *  +--------------------+ | +--------------------+ | +--------------------+
 *                         |                        |            |
 *                         |                        |            v
 *                         |                        | +--------------------+
 *                         |                        | |Json_destroyObject  |
 *                         |                        | +--------------------+
 *                         |                        |            |
 *                         |                        |            v
 *                         |                        | +--------------------+
 *                         |                        | |Json_destroyTemplate|
 *                         |                        | +--------------------+
 *
 * =========================================================================
 */

#include <stdlib.h>
#include <string.h>
#include "json_engine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t Json_Handle;

#define JSON_DEFAULT_SIZE                       (1024u)

/*!
 *  @brief      This function creates internal template from the
 *              template text.
 *
 *  @param[out] templateHandle      template handle
 *  @param[in]  templateString      json template text
 *  @param[in]  templateStringLen   json template text length
 *
 *  @remark     The user must free the created template using
 *              Json_destroyTemplate().
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  Json_Handle templateHandle1;
 *  char *templatestr = "{"
 *          "\"name\":string,"
 *          "\"age\":int32,"
 *          "\"car models\":[string,string,string]}";
 *
 *  ret = Json_createTemplate(&templateHandle1, templatestr,
 *          strlen(templatestr));
 *  @endcode
 *
 *  @sa Json_destroyTemplate()
 */
int16_t Json_createTemplate(Json_Handle *templateHandle,
    const char *templateString, uint16_t templateStringLen);


/*!
 *  @brief      This function frees the internal template memory
 *
 *  @param[in]  templateHandle      template handle
 *
 *  @remark     After destroying the template, the Json object will have a
 *              @c NULL pointer in it (see the object struct -
 *              JSON_templateInternal *jsonTemplate).  It is
 *              recommended to first destroy the Json object and then
 *              the template.
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;

 *  ret  = Json_destroyTemplate(templateHandle1);
 *  @endcode
 *
 *  @sa Json_createTemplate()
 */
int16_t Json_destroyTemplate(Json_Handle templateHandle);

/*!
 *  @brief      This function creates an empty Json object
 *
 *  @param[out]   objHandle         json object handle
 *  @param[in]    templateHandle    json template handle
 *  @param[in]    maxObjectSize     json object max size or 0 for
 *                                   #JSON_DEFAULT_SIZE
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @remark     The user must free the created object using Json_destroyObject()
 *
 *  @remark     The user must create a template before its json object
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  Json_Handle h;
 *
 *  ret = Json_createObject(&h, templateHandle, 1024);
 *  @endcode
 *
 *  @sa     Json_createTemplate()
 *  @sa     Json_destroyObject()
 */
int16_t Json_createObject(Json_Handle *objHandle, Json_Handle templateHandle,
        uint16_t maxObjectSize);


/*!
 *  @brief      Free the internal json memory
 *
 *  @param[in]  objHandle       json handle, created by Json_createObject()
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *
 *  ret = Json_destroyObject(h);
 *  @endcode
 *
 *  @sa     Json_createObject()
 */
int16_t Json_destroyObject(Json_Handle objHandle);

/*!
 *  @brief This function converts the json text into internal representation
 *
 *  @param[in]  objHandle       json object handle
 *  @param[in]  jsonText        pointer to the json text
 *  @param[in]  jsonTextLen     json text size
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  char *jsonBuf = "{\"name\":\"John\","
 *          "\"age\":30,"
 *          "\"car models\":[\"toyota\",\"fiat\",\"volvo\"]}";
 *
 *  ret = Json_parse(h, jsonBuf, strlen(jsonBuf));
 *  @endcode
 *
 *  @sa     Json_createObject()
 */
int16_t Json_parse(Json_Handle objHandle, char *jsonText, uint16_t jsonTextLen);


/*!
 *  @brief      Retrieve the number of array elements in the provided key
 *
 *  @param[in]  objHandle       json object handle
 *  @param[in]  pKey            pointer to the key of the requested array
 *
 *  @remark     @c pKey must be NULL terminated.
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  uint16_t arrayCount;
 *  char *jsonBuf = "{"
 *          "\"name\":\"John\","
 *          "\"age\":30,"
 *          "\"car models\":[\"toyota\",\"fiat\",\"volvo\"]};
 *  char *key =  "\"car models\"";
 *
 *  ret = Json_parse(h, jsonBuf, strlen(jsonBuf));
 *
 *  arrayCount = Json_getArrayMembersCount(h, key);
 *  @endcode
 *
 *  @sa     Json_parse()
 */
int16_t Json_getArrayMembersCount(Json_Handle objHandle, const char *pKey);


/*!
 *  @brief This function retrieves value from json
 *
 *  @param[in]      objHandle       json object handle
 *  @param[in]      pKey            pointer to the key of the requested value
 *  @param[out]     pValue          [optional] pointer to the retrieved value
 *  @param[in,out]  maxValueSize    input, number of bytes @c pValue can
 *                                  hold (if @c pValue is not NULL).
 *                                  output, number of bytes required
 *                                  to hold requested value.
 *
 *  @remark     @c pKey must be NULL terminated
 *
 *  @remark     if @c pValue is @c NULL, @c maxValueSize will return the
 *              requested value size
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  char *jsonBuf = "{"
 *          "\"name\":\"John\","
 *          "\"age\":30,"
 *          "\"car models\":[\"toyota\",\"fiat\",\"volvo\"]};
 *  char *key =  "\"name\"";
 *  char   value[5];
 *  uint16_t valueSize = 5;
 *
 *  ret = Json_parse(h, jsonBuf, strlen(jsonBuf));
 *
 *  ret = Json_getValue(h, key, value, &valueSize);
 *  @endcode
 *
 *  @sa     Json_parse()
 */
int16_t Json_getValue(Json_Handle objHandle, const char *pKey, void *pValue,
        uint16_t *maxValueSize);


/*!
 *  @brief      Sets the value for the provided key
 *
 *  @param[in]  objHandle   json object handle
 *  @param[in]  pKey        pointer to the key of the value to be changed
 *  @param[in]  pValue      pointer to the value to be set
 *  @param[in]  valueSize   size of the value
 *
 *  @remark     @c pKey must be NULL terminated
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  char *jsonBuf = "{"
 *          "\"name\":\"John\","
 *          "\"age\":30,"
 *          "\"car models\":[\"toyota\",\"fiat\",\"volvo\"]};
 *  char *key =  "\"age\"";
 *  uint16_r value = 29;
 *  uint16_t valueSize = sizeof(value);
 *
 *  ret = Json_parse(h, jsonBuf, strlen(jsonBuf));
 *
 *  ret = Json_setValue(h, key, value, valueSize);
 *  @endcode
 *
 *  @sa     Json_parse()
 */
int16_t Json_setValue(Json_Handle objHandle, const char *pKey, void *pValue,
        uint16_t valueSize);


/*!
 *  @brief This function builds the internal json into a text json
 *
 *  @param[in]      objHandle   json object handle
 *  @param[out]     pJsonText   pointer to buffer to output the json text
 *  @param[in,out]  maxTxtLen   input, maximum buffer size.
 *                              output, used buffer size.
 *
 *  @return     Success: #JSON_RC__OK
 *  @return     Failure: negative error code
 *
 *  @par        Example
 *  @code
 *  uint16_t ret;
 *  char *key =  "\"age\"";
 *  uint16_r value = 29;
 *  uint16_t valueSize = sizeof(value);
 *  char builtBuff[100];
 *  uint16_t builtBuffSize = 100;
 *  ret = Json_setValue(h, key, value, valueSize);
 *
 *  ret = Json_build(h, builtBuf, &builtBuffSize);
 *  @endcode
 *
 *  @sa     Json_parse()
 *  @sa     Json_setValue()
 */
int16_t Json_build(Json_Handle objHandle, char *pJsonText, uint16_t *maxTxtLen);

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
