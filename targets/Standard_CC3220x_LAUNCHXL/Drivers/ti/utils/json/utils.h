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

#ifndef ti_utils_json_Utils__include
#define ti_utils_json_Utils__include


#include "parse_common.h"
#include "json_engine.h"

#ifdef  __cplusplus
extern "C" {
#endif

#if !defined(ALLOW_PARSING__TEMPLATE)  &&  !defined(ALLOW_PARSING__JSON)
#error Must define at least one of ALLOW_PARSING__TEMPLATE, ALLOW_PARSING__JSON
#endif

#ifndef _INLINE_
#ifdef _MSC_VER
    #define _INLINE_     __inline   /* No need for __forceinline.  Let the optimizer decide */

    #pragma warning (disable:4710) /* function not inlined */
    #pragma warning (disable:4711) /* function selected for automatic inline expansion */
#elif defined __TI_COMPILER_VERSION__
    #define _INLINE_     __inline
#elif defined __GNUC__
    #define _INLINE_     inline
#elif defined __IAR_SYSTEMS_ICC__
    #define _INLINE_    inline
#elif defined __MINGW32__
    #define _INLINE_    inline
#else
    #error Unknown compiler.  Cannot define _INLINE_
#endif /* _MSC_VER */
#endif /* _INLINE_  */

#define ARRAY_SIZEOF(array)                 (sizeof(array) / sizeof(*(array)))

#define MAX_UINT32                          0xFFFFFFFFu

#define MAX_INT32                           ((1uL << (8*sizeof(int32_t) - 1)) - 1uL)
#define MIN_INT32                           ((1uL << (8 * sizeof(int32_t) - 1)))

#define MAX_UINT16                          ((1uL << (8*sizeof(uint16_t)   )) - 1uL)

#define MAX_UINT8                           ((1uL << (8*sizeof(int8_t )    )) - 1uL)

#ifdef USE__PRETEND_TO_USE_PARAMETER
    #define PRETEND_TO_USE_PARAMETER(parameter)     PretendToUseParameter((const void *)&(parameter))
#else
    #define PRETEND_TO_USE_PARAMETER(parameter)
#endif

#define CANONIZED_VALUE__NOT_FOUND    -1L
#define CANONIZED_VALUE__UNICODE      0x00FF0000

#define DEFAULT_HASH_STARTING_VALUE         0xA53Bu

typedef enum skipping_mode_TAG
{
    SKIP_COMPLEX_OBJECTS = 1,
    GO_INTO_COMPLEX_OBJECTS = 2
} skipping_mode_T;

typedef enum expanding_array_behavior_TAG
{
    ARRAYS__ALLOW_TO_EXPAND = 1,
    ARRAYS__KEEP_AS_IN_TEMPLATE = 2,
    ARRAYS__KEEP_AS_IS = 3
} expanding_array_behavior_T;

typedef struct  io_data_stream_cb_TAG
{
    uint8_t *dataBuf;
    uint16_t dataBufSize;
    uint16_t position;
} io_data_stream_cb_T;

typedef struct  in_data_stream_cb_TAG
{
    const uint8_t *dataBuf;
    uint16_t dataBufSize;
    uint16_t position;
} in_data_stream_cb_T;

#ifdef USE__STANDARD_LIBS
    #ifdef _MSC_VER
        #pragma warning(disable:4001)   /* nonstandard extension 'single line comment' was used */
    #endif

    #include <stddef.h>
    #include <stdlib.h>
    #include <string.h>

    #ifdef _MSC_VER
        #pragma warning(default:4001)   /* nonstandard extension 'single line comment' was used */
    #endif

    #define StrLen       strlen
    #define MemSet       memset
    #define MemCpy       memcpy
    #define MemMove      memmove

    #ifdef _MSC_VER
        #define StrNiCmp        _strnicmp
                #define LToA(val,buf)   _ltoa((val),(buf),10)
                #define UlToA(val,buf)  _ultoa((val),(buf),10)
    #elif defined __TI_COMPILER_VERSION__
                #include <strings.h>

                #define StrNiCmp     strncasecmp
                #define LToA         ltoa

void UlToA(_I_ uint32_t val,
           __O char *   buf);                                               /* No native _ultoa in __TI_COMPILER_VERSION__ */
    #elif defined __IAR_SYSTEMS_ICC__
        #include <string.h>
        #define StrNiCmp     strncasecmp
void LToA(_I_ int32_t val,
          __O char *    buf);                                         /* No native _ltoa in IAR */
void UlToA(_I_ uint32_t val,
           __O char *   buf);                                               /* No native _ultoa in IAR */
        #elif defined __GNUC__
                #include <strings.h>
                #define StrNiCmp     strncasecmp
void LToA(_I_ int32_t val,
          __O char *    buf);                                                 /* No native _ltoa in gcc */
void UlToA(_I_ uint32_t val,
           __O char *   buf);                                                       /* No native _ultoa in gcc */
    #else
        #error Unknown compiler.  Cannot define StrNiCmp
    #endif
#else
    #ifndef NULL
        #define NULL    ((void *)0)
    #endif

#ifndef _SIZE_T
typedef unsigned int size_t;            /*@ Should depend on machine */
#endif

size_t StrLen(_I_ char *    string);
void MemSet(__O void *    out_buf,
            _I_ int value,
            _I_ size_t size);
void MemCpy(__O void *    out_buf,
            _I_ void *   in_buf,
            _I_ size_t size);
void MemMove(__O void *    out_buf,
             _I_ void *   in_buf,
             _I_ size_t size);
int StrNiCmp(_I_ char *    string1,
             _I_ char *   string2,
             _I_ size_t size);
void LToA(_I_ int32_t val,
          __O char *   buf);
void UlToA(_I_ uint32_t val,
           __O char *   buf);
/*-----------------------------------------------------------------------*/
#endif /* USE__STANDARD_LIBS */
/*---------------------------------------------------------------------------*/
void SkipWhitespace(_IO_ in_data_stream_cb_T *input_text);

void PretendToUseParameter(const void *parameter);

json_rc_T DetermineValueSize(__O uint16_t *value_size,
                             _I_ uint16_t property_type);

json_rc_T CopyUnexpandedAndInvalidated(
    __O property_table_entry_T *new_member_location,
    __O uint16_t *new_member_size__unexpanded,
    _I_ property_table_entry_T *default_member__may_be_expanded);

json_rc_T FindPropertyByPropertyPath(
    __O property_table_entry__array_T **found_array_start,
    __O const property_table_entry_T **found_property,
    __O uint16_t *array_index,
    _IO_ void *json_internal,
    _I_ char *property_path,
    _I_ uint16_t property_path_size,
    _I_ expanding_array_behavior_T expanding_array_behavior);

json_rc_T FindPropertyInBranch(
    __O const property_table_entry__array_T **found_array_start,
    __O const property_table_entry_T **found_property,
    _I_ void *json_internal,
    _I_ parser_nesting_cb_T *pareser_nesting,
    _I_ uint16_t parent_branch_nesting_level,
    _I_ uint16_t sought_hash,
    _I_ uint16_t sought_array_index,
    _I_ expanding_array_behavior_T expanding_array_behavior);

json_rc_T FindLastExplicitlySpecifiedMember(
    __O const property_table_entry_T **default_member,
    __O uint16_t *default_member_size__unexpanded,
    _I_ property_table_entry__array_T *array_ptr);

json_rc_T FindIndexIn_OrJustAfter_Array(
    _IO_ const property_table_entry_T **property_table_entry,
    _I_ uint16_t sought_array_index,
    _I_ expanding_array_behavior_T expanding_array_behavior);

json_rc_T GetCanonizedCharacter (__O int32_t *canonized_value,
                                 _IO_ uint16_t *index_into_input,
                                 _I_  char input_data[],
                                 _I_ uint16_t input_data_size);

uint16_t NextHash(_I_ uint16_t starting_hash16,
                  _I_ uint8_t new_val8);

json_rc_T ReadIntegerAndSkip(__O uint32_t *integer,
                             _IO_ in_data_stream_cb_T *input_text,
                             _I_ uint32_t minimum_value,
                             _I_ uint32_t maximum_value,
                             _I_ uint32_t default_value);

json_rc_T SetValueInProperty(
    _IO_ void *json_internal,
    _IO_ property_table_entry__array_T *found_array_start,
    _IO_ property_table_entry_T *found_property,
    _I_ uint16_t array_index,
    _I_ void *value,
    _I_ uint16_t value_size,
    _I_ bool full_parse_mode);

json_rc_T SkipPropertyTableEntry(
    _IO_ const uint8_t **property_table_position,
    _I_ skipping_mode_T skipping_mode);

json_rc_T EnsureArrayAccomodatesIndex(
    _IO_ void *json_internal,
    _IO_ property_table_entry__array_T *array_start,
            _I_ uint16_t array_index,
    _I_ bool full_parse_mode);

void UpdateBestCaseRc(_IO_ json_rc_T *best_case_rc,
                      _I_ json_rc_T new_rc);
uint8_t SizeOfTemplateEntry(_I_ uint16_t property_type);


#ifdef __cplusplus
}
#endif

#endif
