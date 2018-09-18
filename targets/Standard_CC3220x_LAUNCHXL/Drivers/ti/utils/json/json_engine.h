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
 *  @file  ti/utils/json/json_engine.h
 *
 *  @addtogroup ti_utils_json_JSON JSON Parser/Builder
 */

#ifndef ti_utils_json_Json_engine__include
#define ti_utils_json_Json_engine__include

/*! @ingroup ti_utils_json_JSON */
/*@{*/

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*****************************************************************************

    API Prototypes

*****************************************************************************/

/** @cond INTERNAL */

#define __O
#define _IO_
#define _I_             const

/** @endcond */

typedef enum json_rc_TAG
{
    JSON_RC__OK                                      =      0  ,
    JSON_RC__RECOVERED__IGNORED_UNKNOWN_VALUE        =     -1  ,

    JSON_RC__RECOVERED__NESTING_EXCEEDED__IGNORING_LEAVES =-4  ,
    JSON_RC__RECOVERED__PARSING_FAILURE              =     -5  ,
    JSON_RC__RECOVERED__INVALID_VALUE                =    -22  ,
    JSON_RC__RECOVERED__NUMBER_ACCURACY_LOSS         =    -23  ,
    JSON_RC__RECOVERED__NUMBER_OVERFLOW              =    -24  ,
    JSON_RC__RECOVERED__ILLEGAL_Q_VALUES             =    -25  ,
    JSON_RC__RECOVERED__STRING_TRUNCATED             =    -28  ,

    JSON_RC__REWIND_REQUIRED                         =    -99  ,
    JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE        =   -100  ,

    JSON_RC__PARSING_BUFFER_SIZE_EXCEEDED            =   -101  ,
    JSON_RC__PARSING_FAILURE                         =   -102  ,
    JSON_RC__PARSING_FAILURE__INCOMPLETE_STRING      =   -103  ,
    JSON_RC__NO_HASH_FOUND                           =   -104  ,
    JSON_RC__BUILDING_UNKNOWN_ATOM                   =   -105  ,
    JSON_RC__BUILDING_BUFFER_SIZE_EXCEEDED           =   -106  ,
    JSON_RC__BUILDING_PARSED_DATA_EXHAUSTED          =   -107  ,
    JSON_RC__WRONG_VALUE_SIZE_FOR_TYPE               =   -108  ,
    JSON_RC__VALUE_NOT_AN_ARRAY                      =   -109  ,
    JSON_RC__INDEX_FAR_BEYOND_ARRAY_END              =   -110  ,
    JSON_RC__PARSING_BUFFER_SIZE_WOULD_HAVE_EXCEEDED =   -111  ,
    JSON_RC__NOT_SUPPORTED                           =   -128  ,


    JSON_RC__NOT_FOUND                               =   -200  ,
    JSON_RC__VALUE_IS_NULL                           =   -201  ,
    JSON_RC__VALUE_NOT_VALID                         =   -202  ,
    JSON_RC__NESTING_EXCEEDED                        =   -204  ,

    JSON_RC__MEMORY_ALLOCATION_ERROR                 =   -300  ,
    JSON_RC__INVALID_TEMPLATE_HANDLE                 =   -301  ,
    JSON_RC__INVALID_OBJECT_HANDLE                   =   -302  ,

    JSON_RC__UNEXPECTED_ERROR                        = -16384

} json_rc_T;

/** @cond INTERNAL */

#define JSON_PARSE_FLAGS__ESTIMATE_ONLY             0x80000000u

/*!
    \brief     External function for initializing internal representation buffer

    \return    json_rc_T

    \param[out]   json_internal         Buffer for internal representation of data
    \param[inout] json_internal_size    On call - max buffer size.  On return - used buffer size - minimal:  With variable-length fields (strings) assumed empty
    \param[in]    json_template         Buffer containing template describing the JSON.  This function can use either the minimal template or the full template
    \param[in]    json_template_size    Size of template

    \sa
    \note
    \warning
    \par
    \code
         //@ TBD code sample


    \endcode
 */
json_rc_T __JSON_Init(__O void *   json_internal,
                      _IO_ uint16_t * json_internal_size,
                      _I_ void *   json_template,
                      _I_ uint16_t json_template_size);

/*!
    \brief     External function for parseing a JSON text-buffer into internal representation

    \return    json_rc_T

    \param[out]   json_internal         Buffer for internal representation of data
    \param[inout] json_internal_size    On call - max buffer size.  On return - used buffer size
    \param[in]    json_text             JSON text string
    \param[in]    json_text_size        JSON text string size
    \param[in]    json_template         Buffer containing template describing the JSON.  This function can use either the minimal template or the full template
    \param[in]    json_template_size    Size of template
    \param[in]    flags                 May be 0 or JSON_PARSE_FLAGS__ESTIMATE_ONLY

    \sa
    \note
    \warning
    \par
    \code
         //@ TBD code sample


    \endcode
 */
json_rc_T __JSON_Parse(__O void *   json_internal,
                       _IO_ uint16_t * json_internal_size,
                       _I_ char *   json_text,
                       _I_ uint16_t json_text_size,
                       _I_ void *   json_template,
                       _I_ uint16_t json_template_size,
                       _I_ uint32_t flags);

/*!
    \brief     External function for building a JSON text-buffer from internal representation

    \return    json_rc_T

    \param[out]   json_text             Buffer for JSON text string
    \param[inout] json_text_size        On call - max buffer size.  On return - used buffer size
    \param[out]   json_internal         Internal representation of data
    \param[in]    json_template         Buffer containing template describing the JSON.  This function must use the full template


    \sa
    \note
                        The output of __JSON_Build() contains only the values marked as "valid" in the internal representation
    \warning
    \par
    \code
         //@ TBD code sample


    \endcode
 */
json_rc_T __JSON_Build(__O char *    json_text,
                       _IO_ uint16_t *  json_text_size,
                       _I_ void *    json_internal,
                       _I_ void *    json_template);

/*!
    \brief     External function for getting a single value from internal representation

    \return    json_rc_T

    \param[out]   value_buffer          Buffer to hold the value. In case of NULL, just the value len is returned in value_buffer_size
    \param[inout] value_buffer_size     On call - max buffer size.  On return - used buffer size
    \param[in]    json_internal         Internal representation of data
    \param[in]    property_path         Path to JSON property to be gotten
    \param[in]    property_path_size    Size of Path to JSON property to be gotten


    \sa
    \note
                Currently supports only basic types:  int32, real32, boolean, string, raw
    \warning
    \par
    \code
                static const char *Properties[] =
                 {
                 "SomeInt32Property"           ,  // A value
                 "SomeObject.SomeArray[3]"     ,  // A value from an array
                 "SomeObject.SomeArray[3][2]"  ,  // A value from an array within an array
                 "Obj.Arr[3][2].ObjectWithin"     // A value from an object in an array within an array
                 } ;
                uint32_t   values[4];
                uint8_t   value_size;
                inti;
                json_rc_Trc = JSON_RC__OK;


                for (i = 0  ;  (rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)  &&  (i < 4)  ;  i++)
                {
                value_size = sizeof(value[i]) ;

                rc = __JSON_GetValue (&values[i]             ,  // value_buffer
                                      &value_size            ,  // value_buffer_size
                                      gJsonCb               ,  // json_internal
                                      Properties[i]         ,  // property_path
                                       StrLen(Properties[i]) ,  // property_path_size
                                      )

    \endcode
 */
json_rc_T __JSON_GetValue(__O void *    value_buffer,
                          _IO_ uint16_t *  value_buffer_size,
                          _I_ void *    json_internal,
                          _I_ char *    property_path,
                          _I_ uint16_t property_path_size);

/*!
    \brief     External function for setting a single value into internal representation

    \return    json_rc_T

    \param[inout] json_internal         Internal representation of data
    \param[inout] json_internal_size    On call - max buffer size.  On return - used buffer size
    \param[in]    value                 Value to be set
    \param[in]    value_size            Size of value to be set
    \param[in]    property_path         Path to JSON property to be gotten
    \param[in]    property_path_size    Size of Path to JSON property to be gotten

    \sa
    \note
    \warning
    \par
    \code
                static const char *Properties[] =
                                                        {
                                                                "SomeInt32Property"           ,  // A value
                                                                "SomeObject.SomeArray[3]"     ,  // A value from an array
                                                                "SomeObject.SomeArray[3][2]"  ,  // A value from an array within an array
                                                                "Obj.Arr[3][2].ObjectWithin"     // A value from an object in an array within an array
                                                        } ;
                static const uint32_t Values[4];
                int i;
                json_rc_T rc = JSON_RC__OK;


                for (i = 0  ;  (rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)  &&  (i < 4)  ;  i++)
                {
                        rc = __JSON_SetValue ( gJsonCb               ,  // json_internal
                                                                &gJsonCbSize           ,  // json_internal_size //@ need to improve
                                                                &Values[i]             ,  // value_buffer
                                                                 sizeof(Values[i])     ,  // value_buffer_size
                                                                 Properties[i]         ,  // property_path
                                                                 StrLen(Properties[i]) ,  // property_path_size
                }

    \endcode
 */
json_rc_T __JSON_SetValue(_IO_ void *    json_internal,
                          _IO_ uint16_t *  json_internal_size,
                          _I_ void *    value,
                          _I_ uint16_t value_size,
                          _I_ char *    property_path,
                          _I_ uint16_t property_path_size);

/*!
    \brief     External function for determining the number of members (whether NULL, valid or invalid) in an array

    \return    json_rc_T

    \param[out]   members_count_var     Variable to hold number of members
    \param[in]    json_internal         Internal representation of data
    \param[in]    property_path         Path to JSON array to be querried
    \param[in]    property_path_size    Size of Path to JSON array to be querried

    \sa
    \note
    \warning
    \par
    \code
        uint16_t array_members_count;
        const char  array_path[] = "Obj.ObjectWithin.Arr";
        json_rc_T rc;


        rc = __JSON_GetArrayMembersCount (&array_members_count   ,  // members_count_var
                                          gJsonCb               ,  // json_internal
                                          array_path            ,  // property_path
                                          StrLen(array_path)     );// property_path_size

    \endcode
 */
json_rc_T __JSON_GetArrayMembersCount(__O uint16_t *  members_count_var,
                                      _I_ void *    json_internal,
                                      _I_ char *    property_path,
                                      _I_ uint16_t property_path_size);
/*!
    \brief     External function for parsing a text-file into a template

    \return    json_rc_T

    \param[out]   output_template               Buffer for JSON template
    \param[inout] output_template_size          On call - max buffer size.  On return - used buffer size
    \param[out]   minimal_template_size         Size of minimal JSON template, without property-names
    \param[in]    partly_templetized_json       Buffer containing text-template describing the JSON
    \param[in]    partly_templetized_json_size  Size of the text-template

    \sa
    \note
    \warning
    \par
    \code
         //@ TBD code sample


    \endcode
 */

json_rc_T __JSON_Templetize(__O void *      output_template,
                            _IO_ uint16_t *    output_template_size,
                            __O uint16_t *    minimal_template_size,
                            _I_ char *      partly_templetized_json,
                            _I_ uint16_t partly_templetized_json_size);

/** @endcond */

/*! @} */
#ifdef __cplusplus
}
#endif

#endif
