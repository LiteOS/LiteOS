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

#ifndef ti_utils_json_Parse_common__include
#define ti_utils_json_Parse_common__include


#include "json_engine.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
    #pragma warning (disable:4710) /* function not inlined */
    #pragma warning (disable:4711) /* function selected for automatic inline expansion */
#endif

//For IOT - 20 seems enough
#define JSON_MAXIMUM_NESTING    20

#define JSON_DATA_MASK__STUCTURE_VERSION        0x0FFFu
#define JSON_DATA_MASK__DATA_TYPE               0xF000u
#define JSON_DATA_TYPE__TEMPLATE                0x1000u /*@ Full/Minimal template - same or different data-type? */
#define JSON_DATA_TYPE__INTERNAL_REPRESENTATION 0x2000u

#define JSON_DATA_STRUCTURE_VERSION__TEMPLATE   0x0002u

#define JSON_VALUE__TRUE                        0x0001u
#define JSON_VALUE__FALSE                       0x0000u
#define JSON_VALUE__NULL                        0x00FFu

#define JSON_VALUE__UNKNOWN                     0xFFFFu

#define ARRAY_INDEX__NONE                       0xFFFFu
#define ARRAY_INDEX__MINIMUM                    0x0000u
#define ARRAY_INDEX__MAXIMUM                    0x00FFu
#define ARRAY_INDEX__DEFAULT                    0x0000u

#define PROPERTY_TYPE__UNKNOWN                  0x0000  /* Occupy 0x0000 */
#define PROPERTY_TYPE__STRING_BASE              0x0100
#define PROPERTY_TYPE__STRING                   0x0108  /* Reserve 0x0100-0x013F.  FU:  lower 5 bits = log2(max number of chars) */
                                                        /*                         FU:  0x20         = allow UNICODE             */
#define PROPERTY_TYPE__RAW                      0x0140  /* raw string format for holding a JSON object without parsing. TODO: need to check if using base string is OK, change name to RAW_STRING? */
#define PROPERTY_TYPE__UINT32_BASE              0x0200
#define PROPERTY_TYPE__UINT32                   0x021F  /* Reserve 0x0200-0x02FF.  FU:  LSB = number of highest bit */
#define PROPERTY_TYPE__INT32_BASE               0x0300
#define PROPERTY_TYPE__INT32                    0x031F  /* Reserve 0x0300-0x03FF.  FU:  LSB = number of highest bit */
#define PROPERTY_TYPE__BOOLEAN_BASE             0x0400  /* Reserve 0x0A00-0x0AFF.  FU:  LSB = number of highest bit in bit-field */
#define PROPERTY_TYPE__BOOLEAN                  0x0400  /* Reserve 0x1000-0x10FF.  FU:  LSB = number of highest bit in bit-field */
#define PROPERTY_TYPE__UREAL32__BASE            0x0800  /* Occupy  0x0800-0x0BFF */
#define PROPERTY_TYPE__REAL32__BASE             0x0C00  /* Occupy  0x0C00-0x0FFF */

#define PROPERTY_TYPE_ALL_MASK                  0x0F00
#define PROPERTY_TYPE_UPPER_MASK                0x0C00

#define PROPERTY_TYPE__OBJECT__BASE             0x8000  /* Bits 0-14 are used for the byte-count of the unnamed property */
#define PROPERTY_TYPE__ARRAY__BASE              0x4000  /* Bits 0-11 are used for default type */
#define PROPERTY_VALIDITY_BIT                   0x2000
#define PROPERTY_NULLITY_BIT                    0x1000
#define PROPERTY_TYPE__SINGLE_VALUE__BASE       0x0000

#define PROPERTY_TYPE__MASK__OBJECT_OR_ARRAY_OR_VALUE   0xC000u
#define PROPERTY_TYPE__MASK__REAL_OR_NONREAL            0x0800u
#define PROPERTY_TYPE__MASK__NONREAL_TYPE               0x0700u
#define PROPERTY_TYPE__MASK__RAW_TYPE_WITHOUT_FLAGS     0x0FFFu

#define PROPERTY_TYPE__CLEAN(type)   ((type) & ~(PROPERTY_VALIDITY_BIT | PROPERTY_NULLITY_BIT))

#define PROPERTY_TYPE__REAL__Q_BITS             5u      /* For now: Up to 31 significant bits */
#define PROPERTY_TYPE__REAL__Q_BITS_FOR_MASK    PROPERTY_TYPE__REAL__Q_BITS
#define PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT     0u
#define PROPERTY_TYPE__REAL__Q_LEFT__SHIFT      (PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT + PROPERTY_TYPE__REAL__Q_BITS_FOR_MASK)                    /* 6u     */
#define PROPERTY_TYPE__REAL__Q_RIGHT__MASK      (((1u << PROPERTY_TYPE__REAL__Q_BITS) - 1u) << PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT)
#define PROPERTY_TYPE__REAL__Q_LEFT__MASK       (((1u << PROPERTY_TYPE__REAL__Q_BITS) - 1u) << PROPERTY_TYPE__REAL__Q_LEFT__SHIFT)
#define PROPERTY_TYPE__REAL__Q_RIGHT__MAX_VALUE ( (1u << PROPERTY_TYPE__REAL__Q_BITS) - 1u)   /* 31 */
#define PROPERTY_TYPE__REAL__Q_LEFT__MAX_VALUE  ( (1u << PROPERTY_TYPE__REAL__Q_BITS) - 1u)   /* 31 */
#define PROPERTY_TYPE__REAL__MAX_BITS           (sizeof(uint32_t) * 8u)                         /* 32 */
#define PROPERTY_TYPE__REAL__ALL_1s             (~(uint32_t)0u)

#define PROPERTY_TYPE__QLEFT(type)             ((type & PROPERTY_TYPE__REAL__Q_LEFT__MASK)  >> PROPERTY_TYPE__REAL__Q_LEFT__SHIFT)
#define PROPERTY_TYPE__QRIGHT(type)            ((type & PROPERTY_TYPE__REAL__Q_RIGHT__MASK) >> PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT)

#define PROPERTY_TYPE__REAL__Q__INDICATES_UINT32(type) (   (PROPERTY_TYPE__QLEFT (type) == PROPERTY_TYPE__REAL__Q_LEFT__MAX_VALUE)  \
                                                        && (PROPERTY_TYPE__QRIGHT(type) == PROPERTY_TYPE__REAL__Q_RIGHT__MAX_VALUE))

#define IS_SINGLE_VALUE(      property_type)   (((property_type) & PROPERTY_TYPE__MASK__OBJECT_OR_ARRAY_OR_VALUE) == PROPERTY_TYPE__SINGLE_VALUE__BASE)
#define IS_ARRAY(             property_type)   (((property_type) & PROPERTY_TYPE__MASK__OBJECT_OR_ARRAY_OR_VALUE) == PROPERTY_TYPE__ARRAY__BASE)
#define IS_OBJECT(            property_type)   ( (property_type) & PROPERTY_TYPE__OBJECT__BASE)

#define COMPLEX_OBJECT_LENGTH(property_type)   ((property_type)  &  ~PROPERTY_TYPE__MASK__OBJECT_OR_ARRAY_OR_VALUE)

#define PROPERTY_TYPE__ARRAY__SIZE__BITS            8u  /* Up to 255 array items */
#define PROPERTY_TYPE__ARRAY__SIZE__BITS_FOR_MASK   8u  /* Up to 255 array items */
#define PROPERTY_TYPE__ARRAY__SIZE__SHIFT           0u
#define PROPERTY_TYPE__ARRAY__SIZE__MASK            (((1u << PROPERTY_TYPE__ARRAY__SIZE__BITS_FOR_MASK) - 1u) << PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT)    /* 0x0FC0 */
#define PROPERTY_TYPE__ARRAY__SIZE__MIN             0
#define PROPERTY_TYPE__ARRAY__SIZE__MAX             ((1u << PROPERTY_TYPE__ARRAY__SIZE__BITS) - 1u)    /* 255 */

#if PROPERTY_TYPE__REAL__Q_BITS_FOR_MASK < PROPERTY_TYPE__REAL__Q_BITS
    #error Bad bits-allocation for Q
#endif
#if PROPERTY_TYPE__REAL__Q_BITS_FOR_MASK + PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT > 16u
    #error Field for right-Q spills over beyond uint16_t
#endif
#if PROPERTY_TYPE__REAL__Q_BITS_FOR_MASK + PROPERTY_TYPE__REAL__Q_LEFT__SHIFT > 16u
    #error Field for left-Q spills over beyond uint16_t
#endif
#if PROPERTY_TYPE__ARRAY__SIZE__BITS_FOR_MASK < PROPERTY_TYPE__ARRAY__SIZE__BITS
    #error Error in bits-allocation for Array Size
#endif
#if PROPERTY_TYPE__ARRAY__SIZE__BITS_FOR_MASK + PROPERTY_TYPE__REAL__Q_LEFT__SHIFT > 16u
    #error Field for array-size spills over beyond uint16_t
#endif
/*------------------------------------------------*/
/* __DEFAULTs For recovering from parsing errors. */
/*------------------------------------------------*/
#define PROPERTY_TYPE__REAL__Q_RIGHT__DEFAULT ((PROPERTY_TYPE__REAL__MAX_BITS/2) << PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT)
#define PROPERTY_TYPE__REAL__Q_LEFT__DEFAULT  ((PROPERTY_TYPE__REAL__MAX_BITS-PROPERTY_TYPE__REAL__MAX_BITS/2) << PROPERTY_TYPE__REAL__Q_LEFT__SHIFT)

#define MAX_STRING_LEN      16000u

typedef struct json_template_header_TAG
{
    uint16_t version;
    uint16_t hashSeed;
    uint16_t propertyTableSize;
} json_template_header_T;

typedef struct json_template_TAG
{
    json_template_header_T header;

    /*******************************************************************************/
    /* Then - A Per-Property Table                                                 */
    /* Then - Optional '\0'-separated strings for property-names, for JSON_Build() */
    /*******************************************************************************/
} json_template_T;

typedef struct json_internal_header_TAG
{
    uint16_t version;
    uint16_t hashSeed;
    uint16_t propertyTableSize;
    uint16_t maximumSize;
    uint16_t currentSize;
} json_internal_header_T;

typedef struct json_internal_TAG
{
    json_internal_header_T header;

    /**********************************************/
    /* Then - A Per-Property Table                */
    /* Then - An IsValid Bitmap                   */
    /* Then - An IsNull Bitmap                    */
    /* Then - '\0'-separated strings for literals */
    /**********************************************/
} json_internal_T;

typedef struct property_table_entry__common_TAG
{
    uint16_t propertyHash;
    uint16_t propertyType;
} property_table_entry__common_T;

#ifndef JSON_MAXIMUM_NESTING
    #define JSON_MAXIMUM_NESTING    32
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct property_table_entry__array_TAG
{
    property_table_entry__common_T common;
    uint8_t membersCount;
    uint8_t membersCount_expectedByTemplate;
} property_table_entry__array_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

typedef union   property_table_entry_TAG
{
    property_table_entry__common_T common;
    property_table_entry__array_T array;
} property_table_entry_T;

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct  parser_nesting_node_TAG
{
    uint16_t hash;
    bool isArray;
    uint8_t *positionOfEntry;
    uint8_t currentMemberInArray;
    uint8_t membersCountInArray_expectedByTemplate;
} parser_nesting_node_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct  parser_nesting_cb_TAG
{
    parser_nesting_node_T stack[JSON_MAXIMUM_NESTING];
    uint16_t position;
} parser_nesting_cb_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct  traverser_nesting_node_TAG
{
    uint16_t hash;
    bool isArray;
    const uint8_t *positionOfEntry_input;
    uint8_t *positionOfEntry_output;
    uint8_t membersCountInArray_counter;
    uint8_t membersCountInArray_inTemplate;
} traverser_nesting_node_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct  traverser_nesting_cb_TAG
{
    traverser_nesting_node_T stack[JSON_MAXIMUM_NESTING];
    uint16_t position;
} traverser_nesting_cb_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

typedef enum parse_pass_TAG
{
    /************************************************************************/
    /* Bit 0 == PREPASS    / FINAL         - may be 0 only for template     */
    /* Bit 1 == TEMPLATE   / JSON                                           */
    /* BIT 2 == FULL-PARSE / ESTIMATE-ONLY - may be 1 only for JSON_Parse() */
    /************************************************************************/

    PARSE_PASS__TEMPLATE_PREPASS = 0,
    PARSE_PASS__TEMPLATE_FINAL = 1,

    PARSE_PASS__JSON = 3,

    PARSE_PASS__JSON_ESTIMATE_ONLY = 7
} parse_pass_T;

#define IS_PREPASS(parse_type)   (((parse_type) & 1) == 0)
#define IS_FINAL_PASS(parse_type)   (!IS_PREPASS(parse_type))

#define IS_TEMPLATE_PARSE(parse_type)   (((parse_type) & 2) == 0)
#define IS_JSON_PARSE(parse_type)   (!IS_TEMPLATE_PARSE(parse_type))

#define IS_TEMPLATE_PASS_PREPASS(parse_type)   (((parse_type) & 3) == PARSE_PASS__TEMPLATE_PREPASS)
#define IS_TEMPLATE_PASS_FINAL(parse_type)   (((parse_type) & 3) == PARSE_PASS__TEMPLATE_FINAL)

#define IS_ESTIMATE_ONLY(parse_type)   (((parse_type) & 4) != 0)
#define IS_FULL_PARSE(parse_type)   (((parse_type) & 4) == 0)

json_rc_T ParseCommon(_I_ parse_pass_T parse_pass_type,
                      _IO_ void *output_buf,              /* Internal Representation OR Template */
                      _IO_ uint16_t *phase_output_size,
                      _I_ char *input_text,               /* JSON OR partly templetized JSON */
                      _I_ uint16_t input_text_size);

#ifdef __cplusplus
}
#endif

#endif
