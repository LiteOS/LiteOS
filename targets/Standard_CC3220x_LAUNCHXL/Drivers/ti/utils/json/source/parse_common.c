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
 * */
/*****************************************************************************/
/* Parsing code has been moved out of json.c,                                */
/*   to limit the size of the source-file.                                   */
/* Sadly, this means some extra function-calls ==> code and stack waste.     */
/*****************************************************************************/
#include "json_engine.h"
#include "utils.h"
#include "parse_common.h"

/*---------------------------------------------------------------------------*/
/* See JSON syntax at http://www.json.org                                    */
/*---------------------------------------------------------------------------*/
#define TOKEN_TYPE__START                       0x00u
#define TOKEN_TYPE__END_OF_FILE                 0x01u
#define TOKEN_TYPE__OBJECT_START                0x02u  /* "{" */
#define TOKEN_TYPE__OBJECT_END                  0x03u  /* "}" */
#define TOKEN_TYPE__ARRAY_START                 0x04u  /* "[" */
#define TOKEN_TYPE__ARRAY_END                   0x06u  /* "]" */
#define TOKEN_TYPE__COMMA                       0x07u  /* "]" or "," or "[" reached when value was expected */
#define TOKEN_TYPE__EMPTY_VALUE                 0x08u  /* "]" or "," or "[" reached when value was expected */
#define TOKEN_TYPE__PROPERTY_NAME               0x09u  /* including quotes and colon */
#define TOKEN_TYPE__BASIC_PROPERTY_TYPE         0x10u  /* Used only in Templetize */
#define TOKEN_TYPE__VALUE                       0x20u  /* Used only in Parsing JSON */
#define TOKEN_TYPE__CATCH_ALL                   0xF0u
#define TOKEN_TYPE__UNKNOWN                     0xFFu

#define POSITION_OF_ENTRY__IRRELEVANT           0xFFFFu

typedef struct string_dictionary_TAG
{
    const char *    string;
    uint16_t stringLen;
    uint16_t code;
} string_dictionary_T;

#define DICTIONARY_DEFAULT_AT_END           NULL
/*-------------------------------------------------------------------------*/
/* Note that LITERAL_STRING_AND_LEN() must only accept string literals,    */
/*  and that THERE IS NO CHECK  -totally programmer's responsibility.      */
/*-------------------------------------------------------------------------*/
#define     LITERAL_STRING_AND_LEN(literal_string)        literal_string, \
    (sizeof(literal_string) - 1)
/*---------------------------------------------------------------------------*/
/* Make sure all values remain within one byte (0x00-0xFF),   */
/* since they are used as uint8_t in json_parser_sm_T           */
/*------------------------------------------------------------*/
typedef enum sm_state_id_TAG
{
    STATE_START = 0x01,
    STATE_OBJECT = 0x02,
    STATE_OBJECT_VALUE = 0x03,
    STATE_ARRAY_VALUE = 0x04,
    STATE_OBJECT_VALUE_END = 0x05,
    STATE_ARRAY_VALUE_END = 0x06,
    STATE_EXPECT_END = 0x0E,
    STATE_END = 0x0F,

    STATE___DECIDED_BY_POP = 0xFF
} sm_state_id_T;

typedef enum sm_transition_func_rc_TAG
{
    SM_TRANSITION__SUCCEEDED_AND_SET_STATE = -3,
    SM_TRANSITION__KEEP_TRYING = -2,
    SM_TRANSITION__FAILED = -1,
    SM_TRANSITION__SUCCEEDED = 0

                               /* Positive value ==> Keep strying with value as temporary state */
} sm_func_rc_T;

#if defined(ALLOW_PARSING__JSON)

typedef union value_option_TAG
{
    int32_t int32;
    uint32_t uint32;
    uint32_t real32;
    uint32_t ureal32;
    uint8_t boolean;
    uint16_t stringOffset;
} value_option_T;

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct value_TAG
{
    bool isValid;
    bool isNull;
    value_option_T option;
} value_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#endif /* defined(ALLOW_PARSING__JSON) */

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct generalized_value_TAG
{
    uint16_t propertyType;
#if defined(ALLOW_PARSING__JSON)
    value_T value;
#endif
} generalized_value_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct property_in_map_TAG
{
    property_table_entry__array_T *arrayStart;
    property_table_entry_T *item;
} property_in_map_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct sm_state_TAG
{
    parse_pass_T parsePassType;
    io_data_stream_cb_T output;
    in_data_stream_cb_T input;
    parser_nesting_cb_T nesting;
    generalized_value_T latestValue;
    uint16_t positionForStrings;
    uint16_t recentStringOffset;
    uint16_t recentStringLen;
    uint16_t recentStringCanonizedLen;
    sm_state_id_T stateID;
    json_rc_T bestCaseRc;
    uint16_t tentativeHash;
#if defined(ALLOW_PARSING__JSON)
    property_in_map_T propertyFromTemplate;
#endif
    bool bFullParse;
} sm_state_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

typedef sm_func_rc_T (*sm_transition_func_T)(_IO_ sm_state_T *   state);

#ifdef _MSC_VER
    #pragma warning(disable:4820) /* bytes padding added after data member */
#endif

typedef struct json_sm_TAG
{
    uint8_t currentState;                     /* Could have been 'sm_state_T', but that would have taken more bytes */
    uint8_t triggerToken;
    uint8_t newState;                     /* Could have been 'sm_state_T', but that would have taken more bytes */
    sm_transition_func_T stateTransitionFunc;
} json_parser_sm_T;

#ifdef _MSC_VER
    #pragma warning(default:4820)
#endif

static sm_func_rc_T SmDoNothing(_IO_ sm_state_T *state);
static sm_func_rc_T SmIncreaseNestingIntoObject(_IO_ sm_state_T *state);
static sm_func_rc_T SmIncreaseNestingIntoArray(_IO_ sm_state_T *state);
static sm_func_rc_T SmDecreaseNestingAndPop(_IO_ sm_state_T *state);
static sm_func_rc_T SmIncorporatePropertyName(_IO_ sm_state_T *state);
static sm_func_rc_T SmNextArrayValue(_IO_ sm_state_T *state);
static sm_func_rc_T SmIncorporateValue(_IO_ sm_state_T *state);
static sm_func_rc_T SmIncorporateArrayDefaultValue(_IO_ sm_state_T *state);
static sm_func_rc_T SmKeepTryingWithStateStart(_IO_ sm_state_T *state);

static sm_func_rc_T SmAddDefaultArrayValue_DecreaseNestingAndPop(
    _IO_ sm_state_T *state);

#if defined(ALLOW_PARSING__TEMPLATE)

static sm_func_rc_T IncorporateValue_Common_Template(
    _IO_ io_data_stream_cb_T *output,
    _I_ uint16_t hash,
    _I_ uint16_t property_type);
#endif

static sm_func_rc_T IncreaseNesting_Common(_IO_ sm_state_T *state);
static json_rc_T IdentifyStringLiteral(_IO_ sm_state_T *state);
static bool IsRawTypeExpected(_IO_ sm_state_T *state);
static json_rc_T PrepareRawString(_IO_ sm_state_T *state);
static json_rc_T FindInDictionary_AndSkip(__O uint16_t *found_code,
                                          _I_ string_dictionary_T *dictionary,
                                          _IO_ in_data_stream_cb_T *input_text);

static const json_parser_sm_T gStateMachineRules[] =
{
    { STATE_START, TOKEN_TYPE__OBJECT_START, STATE_OBJECT,
      SmIncreaseNestingIntoObject},
    { STATE_START, TOKEN_TYPE__ARRAY_START, STATE_ARRAY_VALUE,
      SmIncreaseNestingIntoArray},

    { STATE_OBJECT, TOKEN_TYPE__PROPERTY_NAME, STATE_OBJECT_VALUE,
      SmIncorporatePropertyName},
    { STATE_OBJECT, TOKEN_TYPE__OBJECT_END, STATE___DECIDED_BY_POP,
      SmDecreaseNestingAndPop},

#if defined(ALLOW_PARSING__TEMPLATE)
    { STATE_ARRAY_VALUE, TOKEN_TYPE__BASIC_PROPERTY_TYPE, STATE_ARRAY_VALUE_END,
      SmIncorporateValue},
#endif
#if defined(ALLOW_PARSING__JSON)
    { STATE_ARRAY_VALUE, TOKEN_TYPE__VALUE, STATE_ARRAY_VALUE_END,
      SmIncorporateValue},
#endif
    { STATE_ARRAY_VALUE, TOKEN_TYPE__OBJECT_END, STATE___DECIDED_BY_POP,
      SmDecreaseNestingAndPop},
    { STATE_ARRAY_VALUE, TOKEN_TYPE__COMMA, STATE_ARRAY_VALUE,
      SmIncorporateArrayDefaultValue},
    { STATE_ARRAY_VALUE, TOKEN_TYPE__ARRAY_END, STATE___DECIDED_BY_POP,
      SmAddDefaultArrayValue_DecreaseNestingAndPop},
    { STATE_ARRAY_VALUE, TOKEN_TYPE__CATCH_ALL, 0, SmKeepTryingWithStateStart},

#if defined(ALLOW_PARSING__TEMPLATE)
    { STATE_OBJECT_VALUE, TOKEN_TYPE__BASIC_PROPERTY_TYPE,
      STATE_OBJECT_VALUE_END, SmIncorporateValue},
#endif
#if defined(ALLOW_PARSING__JSON)
    { STATE_OBJECT_VALUE, TOKEN_TYPE__VALUE, STATE_OBJECT_VALUE_END,
      SmIncorporateValue},
#endif
    { STATE_OBJECT_VALUE, TOKEN_TYPE__CATCH_ALL, 0, SmKeepTryingWithStateStart},

    { STATE_OBJECT_VALUE_END, TOKEN_TYPE__COMMA, STATE_OBJECT, SmDoNothing},
    { STATE_OBJECT_VALUE_END, TOKEN_TYPE__OBJECT_END, STATE___DECIDED_BY_POP,
      SmDecreaseNestingAndPop},

    { STATE_ARRAY_VALUE_END, TOKEN_TYPE__COMMA, STATE_ARRAY_VALUE,
      SmNextArrayValue},
    { STATE_ARRAY_VALUE_END, TOKEN_TYPE__ARRAY_END, STATE___DECIDED_BY_POP,
      SmDecreaseNestingAndPop},

    { STATE_EXPECT_END, TOKEN_TYPE__END_OF_FILE, STATE_END, SmDoNothing}
};
/*****************************************************************************/
#if defined(ALLOW_PARSING__TEMPLATE)

static const string_dictionary_T gPropertyTypeSpecifier[] =
{
    { LITERAL_STRING_AND_LEN ("int32"), PROPERTY_TYPE__INT32        },
    { LITERAL_STRING_AND_LEN ("uint32"), PROPERTY_TYPE__UINT32       },
    { LITERAL_STRING_AND_LEN ("string"), PROPERTY_TYPE__STRING       },
    { LITERAL_STRING_AND_LEN ("raw"), PROPERTY_TYPE__RAW          },
    { LITERAL_STRING_AND_LEN ("boolean"), PROPERTY_TYPE__BOOLEAN      },
    { LITERAL_STRING_AND_LEN ("real32"), PROPERTY_TYPE__REAL32__BASE },
    { LITERAL_STRING_AND_LEN ("ureal32"), PROPERTY_TYPE__UREAL32__BASE},
    { DICTIONARY_DEFAULT_AT_END, 0, PROPERTY_TYPE__UNKNOWN      }
};

#endif
/*****************************************************************************/
#if defined(ALLOW_PARSING__JSON)

static const string_dictionary_T gKeywordDictionary[] =
{
    { LITERAL_STRING_AND_LEN ("true"), JSON_VALUE__TRUE   },
    { LITERAL_STRING_AND_LEN ("false"), JSON_VALUE__FALSE  },
    { LITERAL_STRING_AND_LEN ("null"), JSON_VALUE__NULL   },
    { DICTIONARY_DEFAULT_AT_END, 0, JSON_VALUE__UNKNOWN}
};

#endif
/*****************************************************************************/
static _INLINE_ json_rc_T ExpectCharacter(
    _IO_ in_data_stream_cb_T *  input_text,
    _I_ char expected_char)
{
    if((input_text->position < input_text->dataBufSize)
       &&  (input_text->dataBuf[input_text->position] == expected_char))
    {
        input_text->position++;     /* Skip expected_char */

        SkipWhitespace (input_text);

        return (JSON_RC__OK);
    }
    else
    {
        return (JSON_RC__NOT_FOUND);
    }
}

/*****************************************************************************/
static _INLINE_ void IgnoreCurrentAndHigherNestingLeaves(
    _IO_ in_data_stream_cb_T *   input_text)
{
    int32_t nesting_level = 0;

    while((nesting_level >= 0)
          && (input_text->position < input_text->dataBufSize))
    {
        if((input_text->dataBuf[input_text->position] == '{')
           || (input_text->dataBuf[input_text->position] == '['))
        {
            nesting_level++;
        }
        else if((input_text->dataBuf[input_text->position] == '}')
                || (input_text->dataBuf[input_text->position] == ']'))
        {
            nesting_level--;
        }

        input_text->position++;
    }
}

/*****************************************************************************/
json_rc_T FindPropertyInBranch(
    __O const property_table_entry__array_T * *  found_array_start,
    __O const property_table_entry_T * *
    found_property,
    _I_ void *
    json_internal,
    _I_ parser_nesting_cb_T *
    parser_nesting,
    _I_ uint16_t parent_branch_nesting_level,
    _I_ uint16_t sought_hash,
    _I_ uint16_t sought_array_index,
    _I_ expanding_array_behavior_T
    expanding_array_behavior)
{
    const json_internal_header_T *  internal_header =
        (const json_internal_header_T *)json_internal;
    const uint8_t *                   property_table_position =
        (const uint8_t *)(internal_header + 1);
    const property_table_entry_T *  property_table_entry =
        (const property_table_entry_T *)property_table_position;
    const uint8_t *                   end_of_object = property_table_position +
                                                      internal_header->
                                                      propertyTableSize;
    uint16_t nesting_level;
    bool found_item_in_branch = false;
    uint16_t sought_hash_in_branch;
    uint16_t sought_index_in_branch = 0;
    uint16_t array_index;
    json_rc_T rc = JSON_RC__OK;

    *found_array_start = NULL;
    *found_property = NULL;

    for(nesting_level = 0;
        (nesting_level <= parent_branch_nesting_level + 1)  &&
        (rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE);
        nesting_level++)
    {
        array_index = 0;

        found_item_in_branch = false;

        if(nesting_level <= parent_branch_nesting_level)
        {
            sought_hash_in_branch = parser_nesting->stack[nesting_level].hash;

            if(nesting_level > 0)
            {
                /************************************************************/
                /* Setting sought_index_in_branch for non-array is harmless */
                /************************************************************/
                sought_index_in_branch =
                    parser_nesting->stack[nesting_level -
                                          1].currentMemberInArray;

                if((expanding_array_behavior == ARRAYS__KEEP_AS_IN_TEMPLATE)
                   && (sought_index_in_branch >=
                       parser_nesting->stack[nesting_level -
                                             1].
                       membersCountInArray_expectedByTemplate))
                {
                    sought_index_in_branch =
                        parser_nesting->stack[nesting_level -
                                              1].
                        membersCountInArray_expectedByTemplate - 1;
                }
            }
        }
        else
        {
            sought_hash_in_branch = sought_hash;
            sought_index_in_branch = sought_array_index;
        }

        while((property_table_position < end_of_object)  &&
              (!found_item_in_branch))
        {
            property_table_entry =
                (const property_table_entry_T *)property_table_position;

            if((nesting_level == 0)  ||
               (!parser_nesting->stack[nesting_level - 1].isArray))
            {
                if(property_table_entry->common.propertyHash ==
                   sought_hash_in_branch)
                {
                    found_item_in_branch = true;    /* Found by hash */
                }
            }
            else
            {
                if(array_index == sought_index_in_branch)
                {
                    found_item_in_branch = true;    /* Found by array index */
                }
            }

            if(!found_item_in_branch)
            {
                UpdateBestCaseRc (&rc,
                                  SkipPropertyTableEntry (&
                                                          property_table_position,
                                                          SKIP_COMPLEX_OBJECTS));

                if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
                {
                    return (rc);
                }
            }

            ++array_index;      /* Harmless if not an array */
        }

        if(!found_item_in_branch)
        {
            return (JSON_RC__NOT_FOUND);
        }

        if(nesting_level <= parent_branch_nesting_level)
        {
            if(IS_SINGLE_VALUE (property_table_entry->common.propertyType))
            {
                return (JSON_RC__UNEXPECTED_ERROR);
            }

            end_of_object = (const uint8_t *)&property_table_entry->common
                            + COMPLEX_OBJECT_LENGTH (
                property_table_entry->common.propertyType);

            UpdateBestCaseRc (&rc,
                              SkipPropertyTableEntry (&property_table_position,
                                                      GO_INTO_COMPLEX_OBJECTS));
        }
    }

    if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (rc);
    }

    if(IS_ARRAY (property_table_entry->common.propertyType))
    {
        *found_array_start = &property_table_entry->array;
    }

    if(sought_array_index == ARRAY_INDEX__NONE)
    {
        *found_property = property_table_entry;

        return (rc);
    }

    if(*found_array_start == NULL)
    {
        return (JSON_RC__VALUE_NOT_AN_ARRAY);
    }

    UpdateBestCaseRc (&rc,
                      FindIndexIn_OrJustAfter_Array (&property_table_entry,
                                                     sought_array_index,
                                                     expanding_array_behavior));

    *found_property = property_table_entry;

    return (rc);
}

/*****************************************************************************/
static void DetermineHashForNewArrayMember(_IO_ sm_state_T *     state)
{
    parser_nesting_node_T *    this_array =
        &state->nesting.stack[state->nesting.position - 1];
    parser_nesting_node_T *    current_item_in_array =
        &state->nesting.stack[state->nesting.position    ];

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_JSON_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__JSON)
    {
        expanding_array_behavior_T expanding_array_behavior;
        json_rc_T rc;

        if(state->bFullParse)
        {
            expanding_array_behavior = ARRAYS__ALLOW_TO_EXPAND;
        }
        else
        {
            expanding_array_behavior = ARRAYS__KEEP_AS_IN_TEMPLATE;     /* Good enough for determining hash */
        }

        rc = FindPropertyInBranch (
            (const property_table_entry__array_T **)&state->
            propertyFromTemplate.arrayStart,                                                        /* Lying about 'const'ness is a sin */
            (const property_table_entry_T        **)&state->
            propertyFromTemplate.item,
            state->output.dataBuf,                                                                  /* JSON internal representation */
            &state->nesting,
            state->nesting.position - 2,
            this_array->hash,
            this_array->currentMemberInArray,
            expanding_array_behavior);

        if(rc != JSON_RC__NOT_FOUND)
        {
            UpdateBestCaseRc (&state->bestCaseRc, rc);
        }

        if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            rc = EnsureArrayAccomodatesIndex (
                state->output.dataBuf,
                state->propertyFromTemplate.
                arrayStart,
                this_array->currentMemberInArray,
                state->bFullParse);

            UpdateBestCaseRc (&state->bestCaseRc, rc);
        }

        if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            state->propertyFromTemplate.item = NULL;
            state->propertyFromTemplate.arrayStart = NULL;
        }

        if(this_array->currentMemberInArray
           >= this_array->membersCountInArray_expectedByTemplate)
        {
            /***************************************************************/
            /* Beyond this_array->MembersCountInArray_ExpectedByTemplate,  */
            /*  the hash value remains the same.                           */
            /***************************************************************/

            return;
        }
    }
#endif

    /*******************************************************************/
    /* Array members are unnamed.                                      */
    /* Hashing them is done by adding '0' to the hash for each member. */
    /*******************************************************************/
    current_item_in_array->hash = NextHash (current_item_in_array->hash, 0);
}

/*****************************************************************************/
static _INLINE_ void InvalidateLatestValue(_IO_ sm_state_T *    state)
{
#if defined(ALLOW_PARSING__JSON)
    state->latestValue.value.isNull = false;       /* Make working assumptions  */
    state->latestValue.value.isValid = false;      /* Make working assumptions  */
#endif

    state->latestValue.propertyType = PROPERTY_TYPE__UNKNOWN;
}

/*****************************************************************************/
#if defined(ALLOW_PARSING__JSON)

/*****************************************************************************/
static _INLINE_ json_rc_T Real_PreAnalyzeNumber(
    _I_ in_data_stream_cb_T *  input_text,
    __O int32_t *
    digits__whole)
{
    json_rc_T rc = JSON_RC__OK;

#ifndef SUPPORT_REAL_NUMBERS

    PRETEND_TO_USE_PARAMETER (input_text);

    *digits__whole = MAX_INT32;

#else /* SUPPORT_REAL_NUMBERS is defined */
    {
        in_data_stream_cb_T input_copy = *input_text;/* Copy of control-block.  Not touching the actual input */
        bool dot_found = false;
        bool exponent_is_negative = false;
        uint32_t unsigned_exponent = 0u;
        int32_t signed_exponent = 0;

        *digits__whole = 0u;

        SkipWhitespace (&input_copy);

        while((input_copy.dataBuf[input_copy.position] == '.')
              ||
              ((input_copy.dataBuf[input_copy.position] >= '0')
               && (input_copy.dataBuf[input_copy.position] <= '9')))
        {
            if(input_copy.dataBuf[input_copy.position] == '.')
            {
                dot_found = true;
            }

            if(!dot_found)
            {
                ++*digits__whole;
            }

            ++input_copy.position;
        }

        SkipWhitespace (&input_copy);

        if((ExpectCharacter (&input_copy, 'e') == JSON_RC__OK)
           || (ExpectCharacter (&input_copy, 'E') == JSON_RC__OK))
        {
            SkipWhitespace (&input_copy);

            ExpectCharacter (&input_copy, '+'); /* Don't care if found.  Not strict about "+-3" instead of "-3" */

            if(ExpectCharacter (&input_copy, '-') == JSON_RC__OK)
            {
                exponent_is_negative = true;
            }

            SkipWhitespace (&input_copy);

            rc =
                ReadIntegerAndSkip (&unsigned_exponent, &input_copy, 0u,
                                    MAX_INT32,
                                    0u);

            if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
            {
                return (rc);
            }

            if(exponent_is_negative)
            {
                signed_exponent = -(int32_t)unsigned_exponent;
            }
            else
            {
                signed_exponent = (int32_t)unsigned_exponent;
            }
        }

        *digits__whole += signed_exponent;
    }
#endif  /* SUPPORT_REAL_NUMBERS */

    return (rc);
}

/*****************************************************************************/
static _INLINE_ json_rc_T Real_AnalyzeNumber(
    _IO_ in_data_stream_cb_T *  input_text,
    __O uint32_t *
    mantissa_whole,
    __O uint32_t *
    mantissa_fraction,
    __O uint32_t *
    digits__fraction,
    _I_ int32_t digits__whole,
    _I_ bool mantissa_is_negative)
{
    uint32_t whole_digits_remaining;
    uint8_t digit;
    uint32_t updated_value;

    *mantissa_whole = 0u;
    *mantissa_fraction = 0u;

    if(digits__whole >= 0)
    {
        *digits__fraction = 0u;
        whole_digits_remaining = (uint32_t)digits__whole;
    }
    else
    {
        *digits__fraction = (uint32_t)-digits__whole;
        whole_digits_remaining = 0u;
    }

    while(
#ifdef SUPPORT_REAL_NUMBERS
        (input_text->dataBuf[input_text->position] == '.')
        ||
#endif
        ((input_text->dataBuf[input_text->position] >= '0')
         && (input_text->dataBuf[input_text->position] <= '9')))
    {
#ifdef SUPPORT_REAL_NUMBERS
        if(input_text->dataBuf[input_text->position] == '.')
        {
            /* Ignore dot in this pass */
        }
        else
#endif
        {
            digit = input_text->dataBuf[input_text->position] - '0';

            if(whole_digits_remaining > 0)
            {
                /******************************************/
                /* Dealing with whole part of the number  */
                /******************************************/
                updated_value = *mantissa_whole * 10u + digit;

                if((mantissa_is_negative  && (*mantissa_whole > MAX_INT32 / 10u))
                   || (mantissa_is_negative  && (updated_value > MAX_INT32))
                   || ((!mantissa_is_negative) &&
                       (*mantissa_whole > MAX_UINT32 / 10u))
                   || ((!mantissa_is_negative) && (updated_value < digit)))
                {
                    if(mantissa_is_negative)
                    {
                        *mantissa_whole = MIN_INT32;   /* Will remain MIN_INT32 after negating */
                    }
                    else
                    {
                        *mantissa_whole = MAX_UINT32;
                    }

                    return (JSON_RC__RECOVERED__NUMBER_OVERFLOW);
                }
                else
                {
                    *mantissa_whole = updated_value;
                }

                whole_digits_remaining--;
            }
#ifdef SUPPORT_REAL_NUMBERS
            else
            {
                /*********************************************/
                /* Dealing with fraction part of the number  */
                /*********************************************/
                updated_value = *mantissa_fraction * 10u + digit;

                if((*mantissa_fraction > MAX_UINT32 / 10u)
                   || (updated_value > 999999999uL))/*@ limitation */
                {
                    return (JSON_RC__RECOVERED__NUMBER_ACCURACY_LOSS);
                }
                else
                {
                    *mantissa_fraction = updated_value;

                    ++*digits__fraction;
                }
            }
#endif
        }

        input_text->position++;
    }

    return (JSON_RC__OK);
}

/*****************************************************************************/
static _INLINE_ json_rc_T Real_Calculate(__O uint16_t *    q_left,
                                         __O uint16_t *    q_right,
                                         _IO_ uint32_t *    mantissa,
                                         _I_ uint32_t callers_mantissa_fraction,
                                         _I_ uint32_t digits__fraction,
                                         _I_ bool mantissa_is_negative)
{
#ifndef SUPPORT_REAL_NUMBERS

    PRETEND_TO_USE_PARAMETER (callers_mantissa_fraction);
    PRETEND_TO_USE_PARAMETER (digits__fraction);
    PRETEND_TO_USE_PARAMETER (mantissa_is_negative);

    *q_right = 0u;

    if(*mantissa >= (1u << (PROPERTY_TYPE__REAL__MAX_BITS - 1u)))
    {
        *q_left = PROPERTY_TYPE__REAL__MAX_BITS;
    }
    else
    {
        *q_left = PROPERTY_TYPE__REAL__MAX_BITS - 1u;   /* Could be less, but there is no need for accuracy here */
    }

#else /* SUPPORT_REAL_NUMBERS is defined */

    uint32_t mantissa_fraction = callers_mantissa_fraction;
    uint16_t binary_digits;
    uint32_t factor = 1u;
    uint32_t msb;
    uint32_t i;

    *q_right = 0u;

    for(i = 0u; i < digits__fraction; i++)
    {
        if(factor > MAX_UINT32 / 10u)
        {
            return (JSON_RC__NOT_SUPPORTED);
        }

        factor *= 10u;
    }

    if(mantissa_is_negative)
    {
        msb = 1u << (PROPERTY_TYPE__REAL__MAX_BITS - 2u);   /* Leave room for sign-bit */
    }
    else
    {
        msb = 1u << (PROPERTY_TYPE__REAL__MAX_BITS - 1u);   /* No need for room for sign-bit */
    }

    while((mantissa_fraction > 0u)
          &&  ((*mantissa & msb) == 0u))
    {
        ++*q_right;

        *mantissa <<= 1;

        mantissa_fraction <<= 1;

        if(mantissa_fraction >= factor)
        {
            *mantissa |= 1u;

            mantissa_fraction -= factor;
        }
    }

    binary_digits = PROPERTY_TYPE__REAL__MAX_BITS;

    while((binary_digits > 1u)
          &&  ((*mantissa & (1u << (binary_digits - 1))) == 0u))
    {
        binary_digits--;
    }

    *q_left = binary_digits - *q_right;

    if(mantissa_fraction > 0u)
    {
        return (JSON_RC__RECOVERED__NUMBER_ACCURACY_LOSS);
    }
    else
#endif  /* SUPPORT_REAL_NUMBERS */

    {
        return (JSON_RC__OK);
    }
}

/*****************************************************************************/
static _INLINE_ json_rc_T IdentifyAndSkip_LiteralNumber(
    _IO_ in_data_stream_cb_T *  input_text,
    __O value_T *
    value,
    __O uint16_t *
    found_property_type,
    __O json_rc_T  *
    internal_rc)
{
    bool mantissa_is_negative = false;
    uint32_t mantissa;
    uint32_t mantissa_fraction;
    int32_t digits__whole;
    uint32_t digits__fraction;
    uint16_t q_left;
    uint16_t q_right;
    json_rc_T rc;

    ExpectCharacter (input_text, '+');   /* Don't care if found.  Not strict about "+-3" instead of "-3" */

    if(ExpectCharacter (input_text, '-') == JSON_RC__OK)
    {
        mantissa_is_negative = true;
    }

    rc = Real_PreAnalyzeNumber (input_text, &digits__whole);

    UpdateBestCaseRc (internal_rc, rc);

    if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (rc);
    }

    rc =
        Real_AnalyzeNumber (input_text, &mantissa, &mantissa_fraction,
                            &digits__fraction,
                            digits__whole,
                            mantissa_is_negative);

    UpdateBestCaseRc (internal_rc, rc);

    if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (rc);
    }

    while((input_text->dataBuf[input_text->position] == 'E')
          || (input_text->dataBuf[input_text->position] == 'e')
          || (input_text->dataBuf[input_text->position] == '+')
          || (input_text->dataBuf[input_text->position] == '-')
          || (input_text->dataBuf[input_text->position] == '.')
          || (input_text->dataBuf[input_text->position] <= ' ')/* All whitespace */
          ||
          ((input_text->dataBuf[input_text->position] >= '0')
           && (input_text->dataBuf[input_text->position] <= '9')))
    {
        ++input_text->position;/* Skip exponent.  It was already considered  */
    }

    rc =
        Real_Calculate (&q_left, &q_right, &mantissa, mantissa_fraction,
                        digits__fraction,
                        mantissa_is_negative);

    UpdateBestCaseRc (internal_rc, rc);

    if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (rc);
    }

    if(mantissa_is_negative)
    {
        q_left++;

        value->option.real32 = -(int32_t)mantissa;/* Same as 'value->Option.Int32 = -(int32_t)mantissa' */

        *found_property_type = PROPERTY_TYPE__REAL32__BASE;
    }
    else
    {
        value->option.ureal32 = mantissa;/* Same as 'value->Option.Uint32 = mantissa' */

        *found_property_type = PROPERTY_TYPE__UREAL32__BASE;
    }

    if((q_left == 0u)  &&  (q_right == 0u))
    {
        q_left = 1u;     /* At least 1 bit in the number */
    }

    if(q_left <= PROPERTY_TYPE__REAL__Q_LEFT__MAX_VALUE)
    {
        *found_property_type |= (q_left << PROPERTY_TYPE__REAL__Q_LEFT__SHIFT);
        *found_property_type |= (q_right << PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT);
    }
    else
    {
        /* Special mark of <32,0> */

        *found_property_type |=
            (PROPERTY_TYPE__REAL__Q_LEFT__MAX_VALUE <<
             PROPERTY_TYPE__REAL__Q_LEFT__SHIFT);
        *found_property_type |=
            (PROPERTY_TYPE__REAL__Q_RIGHT__MAX_VALUE <<
             PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT);
    }

    value->isNull = false;
    value->isValid = true;

    return (rc);
}

/*****************************************************************************/
static void AdjustNumberToTemplate(_IO_ generalized_value_T *  latest_value,
                                   _IO_ json_rc_T *            best_case_rc,
                                   _I_ uint16_t property_type_from_template)
{
    bool template_is_signed;
    bool value_is_signed;
    uint16_t q_left__template;
    uint16_t q_right__template;
    uint16_t q_left__value;
#ifdef SUPPORT_REAL_NUMBERS
    uint16_t q_right__value;

    if(((property_type_from_template & PROPERTY_TYPE_ALL_MASK) ==
        PROPERTY_TYPE__INT32_BASE)
       || ((property_type_from_template & PROPERTY_TYPE_ALL_MASK) ==
           PROPERTY_TYPE__UINT32_BASE))
#endif
    {
        q_left__template = PROPERTY_TYPE__REAL__MAX_BITS;
        q_right__template = 0u;
    }
#ifdef SUPPORT_REAL_NUMBERS
    else
    {
        q_left__template = PROPERTY_TYPE__QLEFT  (property_type_from_template);
        q_right__template = PROPERTY_TYPE__QRIGHT (property_type_from_template);
    }
#endif

    if(((property_type_from_template & PROPERTY_TYPE_ALL_MASK) ==
        PROPERTY_TYPE__INT32_BASE)
       || ((property_type_from_template & PROPERTY_TYPE_UPPER_MASK) ==
           PROPERTY_TYPE__REAL32__BASE))
    {
        template_is_signed = true;
    }
    else
    {
        template_is_signed = false;
    }

    if((latest_value->propertyType & PROPERTY_TYPE_UPPER_MASK) ==
       PROPERTY_TYPE__REAL32__BASE)
    {
        value_is_signed = true;
    }
    else
    {
        value_is_signed = false;
    }

#ifdef SUPPORT_REAL_NUMBERS
    if(PROPERTY_TYPE__REAL__Q__INDICATES_UINT32 (latest_value->propertyType))
#else
    if((!value_is_signed)
       &&      template_is_signed
       &&  ((uint32_t)latest_value->value.option.real32 <
            (1u << (PROPERTY_TYPE__REAL__MAX_BITS - 1))))
    {
        q_left__value = PROPERTY_TYPE__REAL__MAX_BITS - 1u;
    }
    else
#endif
    {
        q_left__value = PROPERTY_TYPE__REAL__MAX_BITS;
#ifdef SUPPORT_REAL_NUMBERS
        q_right__value = 0u;
#endif
    }
#ifdef SUPPORT_REAL_NUMBERS
    else
    {
        q_left__value = PROPERTY_TYPE__QLEFT  (latest_value->propertyType);
        q_right__value = PROPERTY_TYPE__QRIGHT (latest_value->propertyType);
    }

    if(q_right__template < q_right__value)
    {
        UpdateBestCaseRc (best_case_rc,
                          JSON_RC__RECOVERED__NUMBER_ACCURACY_LOSS);

        latest_value->value.option.ureal32 >>=
            (q_right__value - q_right__template);
    }
    else
    {
        latest_value->value.option.ureal32 <<=
            (q_right__template - q_right__value);
    }

    if(value_is_signed  &&  template_is_signed)
    {
        if(q_left__template < q_left__value)
        {
            UpdateBestCaseRc (best_case_rc, JSON_RC__RECOVERED__NUMBER_OVERFLOW);

            latest_value->value.option.real32 = 0;   /* Will become Most-negative-number after next command */
        }
        /************************************************************************************************/
        /* '- 1u' to make sure the sign-bit is set to 1, even if it was 0 and now needs to be saturated */
        /************************************************************************************************/
        latest_value->value.option.real32 |= PROPERTY_TYPE__REAL__ALL_1s <<
                                             (q_left__template +
                                              q_right__template - 1u);
    }
    else if((!value_is_signed)  &&  (!template_is_signed))
    {
        if(q_left__template < q_left__value)
        {
            UpdateBestCaseRc (best_case_rc, JSON_RC__RECOVERED__NUMBER_OVERFLOW);

            latest_value->value.option.ureal32 = PROPERTY_TYPE__REAL__ALL_1s;   /* Will become Most-positive-number after next command */
        }

        latest_value->value.option.ureal32 &=
            (uint32_t)PROPERTY_TYPE__REAL__ALL_1s >>
            (PROPERTY_TYPE__REAL__MAX_BITS -
             (q_left__template + q_right__template));
    }
    else
#endif
    if(value_is_signed  &&  (!template_is_signed))
    {
        UpdateBestCaseRc (best_case_rc, JSON_RC__RECOVERED__NUMBER_OVERFLOW);

        latest_value->value.option.ureal32 = 0u;
    }
    else if((!value_is_signed)  &&  template_is_signed)
    {
        if(q_left__template < q_left__value + 1u)   /* '+1u' to leave room for sign-bit */
        {
            UpdateBestCaseRc (best_case_rc, JSON_RC__RECOVERED__NUMBER_OVERFLOW);

            /************************/
            /* Most-Positive-Number */
            /************************/
            latest_value->value.option.real32 = PROPERTY_TYPE__REAL__ALL_1s
                                                >> (
                PROPERTY_TYPE__REAL__MAX_BITS
                - (q_left__template +
                   q_right__template)
                + 1u);
        }
    }

    latest_value->propertyType = property_type_from_template;
}

/*****************************************************************************/
/* 'IdentifyAndSkip_LiteralValue()' Identifies numbers, true/false & null.   */
/* Literal strings are handled by 'IdentifyStringLiteral()'.                 */
/*****************************************************************************/
static _INLINE_ json_rc_T IdentifyAndSkip_LiteralValue(
    _IO_ sm_state_T *    state)
{
    char first_char_in_literal;
    uint16_t json_value;
    json_rc_T rc = JSON_RC__OK;

    InvalidateLatestValue (state);

    if(state->input.position >= state->input.dataBufSize)
    {
        return (JSON_RC__NOT_FOUND);
    }

    first_char_in_literal = state->input.dataBuf[state->input.position];

    if((first_char_in_literal == '-')
       || (first_char_in_literal == '+')
       || (first_char_in_literal == '.')
       || ((first_char_in_literal >= '0')
           && (first_char_in_literal <= '9')))
    {
        rc =
            IdentifyAndSkip_LiteralNumber (&state->input,
                                           &state->latestValue.value,
                                           &state->latestValue.propertyType,
                                           &state->bestCaseRc);

        if((rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
           &&  (state->propertyFromTemplate.item != NULL))
        {
            AdjustNumberToTemplate (
                &state->latestValue, &state->bestCaseRc,
                state->propertyFromTemplate.item->common.propertyType);
        }
    }
    else
    {
        UpdateBestCaseRc (&rc, FindInDictionary_AndSkip (&json_value,
                                                         gKeywordDictionary,
                                                         &state->input));

        if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            if(json_value == JSON_VALUE__UNKNOWN)
            {
                rc = JSON_RC__NOT_FOUND;
            }
            else if(json_value == JSON_VALUE__NULL)
            {
                state->latestValue.value.isNull = true;
                state->latestValue.value.isValid = true;

                if(state->propertyFromTemplate.item == NULL)
                {
                    state->latestValue.propertyType = PROPERTY_TYPE__UNKNOWN;
                }
                else
                {
                    state->latestValue.propertyType =
                        state->propertyFromTemplate.item->common.propertyType;
                }
            }
            else
            {
                state->latestValue.propertyType = PROPERTY_TYPE__BOOLEAN;

                state->latestValue.value.option.boolean = (uint8_t)json_value;

                state->latestValue.value.isValid = true;
            }
        }
    }

    if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        if(state->latestValue.value.isValid)
        {
            if((state->propertyFromTemplate.item == NULL)
               || (state->latestValue.propertyType !=
                   state->propertyFromTemplate.item->common.propertyType))
            {
                state->latestValue.value.isValid = false;

                UpdateBestCaseRc (&state->bestCaseRc,
                                  JSON_RC__RECOVERED__INVALID_VALUE);
            }
        }
    }

    return (rc);
}

#endif /* ALLOW_PARSING__JSON */
/*****************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable:4100)  /* 'state' : unreferenced formal parameter */
#endif

static sm_func_rc_T SmDoNothing(_IO_ sm_state_T *   state)
{
    return (SM_TRANSITION__SUCCEEDED);
}

#ifdef _MSC_VER
#pragma warning (default:4100)  /* 'state' : unreferenced formal parameter */
#endif
/*****************************************************************************/
static sm_func_rc_T IncreaseNesting_Common(_IO_ sm_state_T *   state)
{
    if(state->nesting.position + 1 >= JSON_MAXIMUM_NESTING)
    {
        IgnoreCurrentAndHigherNestingLeaves (&state->input);

        UpdateBestCaseRc (&state->bestCaseRc,
                          JSON_RC__RECOVERED__NESTING_EXCEEDED__IGNORING_LEAVES);

        return (SM_TRANSITION__SUCCEEDED);
    }

    state->nesting.stack[state->nesting.position +
                         1].hash =
        state->nesting.stack[state->nesting.position].hash;

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    {
        state->nesting.stack[state->nesting.position].positionOfEntry =
            &state->output.dataBuf[state->output.position];
    }
#endif
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    else
#endif
#if defined(ALLOW_PARSING__JSON)
    {
        state->nesting.stack[state->nesting.position].positionOfEntry = NULL;
    }
#endif

    ++state->nesting.position;

    InvalidateLatestValue (state);

    return (SM_TRANSITION__SUCCEEDED);
}

/*****************************************************************************/
static sm_func_rc_T SmIncreaseNestingIntoObject(_IO_ sm_state_T *   state)
{
    parser_nesting_node_T *  parent_nesting;
    sm_func_rc_T rc;

    rc = IncreaseNesting_Common (state);

    if(rc == SM_TRANSITION__SUCCEEDED)
    {
        parent_nesting = &state->nesting.stack[state->nesting.position - 1];

        parent_nesting->isArray = false;

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
        if(IS_TEMPLATE_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
        {
            rc = IncorporateValue_Common_Template (&state->output,
                                                   parent_nesting->hash,
                                                   PROPERTY_TYPE__OBJECT__BASE);
        }
#endif /* defined(ALLOW_PARSING__TEMPLATE) */
    }

    return (rc);
}

/*****************************************************************************/
static sm_func_rc_T SmIncreaseNestingIntoArray(_IO_ sm_state_T *   state)
{
    parser_nesting_node_T *   parent_nesting;
    sm_func_rc_T rc;

    rc = IncreaseNesting_Common (state);

    if(rc == SM_TRANSITION__SUCCEEDED)
    {
        parent_nesting = &state->nesting.stack[state->nesting.position - 1];

        parent_nesting->isArray = true;
        parent_nesting->currentMemberInArray = 0;

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
        if(IS_TEMPLATE_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
        {
            rc = IncorporateValue_Common_Template (&state->output,
                                                   parent_nesting->hash,
                                                   PROPERTY_TYPE__ARRAY__BASE);
        }
#endif /* defined(ALLOW_PARSING__TEMPLATE) */
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
        else
#endif
#if defined(ALLOW_PARSING__JSON)
        {
            expanding_array_behavior_T expanding_array_behavior;
            json_rc_T json_rc;

            if(state->bFullParse)
            {
                expanding_array_behavior = ARRAYS__ALLOW_TO_EXPAND;
            }
            else
            {
                expanding_array_behavior = ARRAYS__KEEP_AS_IN_TEMPLATE;
            }

            json_rc = FindPropertyInBranch (
                (const property_table_entry__array_T **)&state->
                propertyFromTemplate.arrayStart,/* Lying about 'const'ness is a sin */
                (const property_table_entry_T        **)&state->
                propertyFromTemplate.item,
                state->output.dataBuf,/* JSON internal representation */
                &state->nesting,
                state->nesting.position - 2,
                parent_nesting->hash,
                ARRAY_INDEX__NONE,
                expanding_array_behavior);

            if(json_rc == JSON_RC__NOT_FOUND)
            {
                /* Nothing needs to be done at this point in terms of error reporting */
            }
            else if(json_rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
            {
                return (SM_TRANSITION__FAILED);
            }
            else
            {
                UpdateBestCaseRc (&state->bestCaseRc, json_rc);
            }

            parent_nesting->currentMemberInArray = 0;

            if((json_rc == JSON_RC__NOT_FOUND)
               || (state->propertyFromTemplate.arrayStart == NULL))
            {
                parent_nesting->membersCountInArray_expectedByTemplate = 0;
            }
            else
            {
                parent_nesting->membersCountInArray_expectedByTemplate
                    = state->propertyFromTemplate.arrayStart->
                      membersCount_expectedByTemplate;
            }
        }
#endif

        DetermineHashForNewArrayMember (state);

        if(state->bestCaseRc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            rc = SM_TRANSITION__FAILED;
        }
    } /* if (rc == SM_TRANSITION__SUCCEEDED) */

    return (rc);
}

/*****************************************************************************/
static sm_func_rc_T SmDecreaseNestingAndPop(_IO_ sm_state_T *   state)
{
    parser_nesting_node_T *    current_nesting;

    InvalidateLatestValue (state);

    if(state->nesting.position > 0)
    {
        --state->nesting.position;
    }

    current_nesting = &state->nesting.stack[state->nesting.position];

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    {
        property_table_entry_T *     entry;

        if(current_nesting->isArray)
        {
            property_table_entry__array_T *     array_property;

            array_property =
                (property_table_entry__array_T *)current_nesting->
                positionOfEntry;

            array_property->membersCount =
                current_nesting->currentMemberInArray + 1;
        }

        entry = (property_table_entry_T *)current_nesting->positionOfEntry;

        entry->common.propertyType |=
            &state->output.dataBuf[state->output.position] -
            current_nesting->positionOfEntry;
    }
#endif /* defined(ALLOW_PARSING__TEMPLATE) */

    if(state->nesting.position == 0)
    {
        state->stateID = STATE_EXPECT_END;
    }
    else if(current_nesting[-1].isArray)
    {
        state->stateID = STATE_ARRAY_VALUE_END;
    }
    else
    {
        state->stateID = STATE_OBJECT_VALUE_END;
    }

    return (SM_TRANSITION__SUCCEEDED_AND_SET_STATE);
}

/*****************************************************************************/
static sm_func_rc_T SmAddDefaultArrayValue_DecreaseNestingAndPop(
    _IO_ sm_state_T *   state)
{
    sm_func_rc_T sm_rc__add;
    sm_func_rc_T sm_rc__pop;

    sm_rc__add = SmIncorporateArrayDefaultValue (state);

    InvalidateLatestValue (state);

    if(sm_rc__add < 0)
    {
        return (sm_rc__add);
    }

    sm_rc__pop = SmDecreaseNestingAndPop (state);

    if(sm_rc__pop < 0)
    {
        return (sm_rc__pop);
    }

    if(sm_rc__add == SM_TRANSITION__SUCCEEDED)
    {
        return (sm_rc__pop);/* ADD succeeded ==> check if more needs to be done fo POP */
    }
    else if(sm_rc__pop == SM_TRANSITION__SUCCEEDED)
    {
        return (sm_rc__add);/* POP succeeded ==> check if more needs to be done fo ADD */
    }
    else if(sm_rc__add == sm_rc__pop)
    {
        return (sm_rc__add);/* Same result.  Pass it on */
    }
    else
    {
        return (SM_TRANSITION__FAILED);/* Contradicting results.  FAIL */
    }
}

/*****************************************************************************/
static sm_func_rc_T SmNextArrayValue(_IO_ sm_state_T *   state)
{
    ++state->nesting.stack[state->nesting.position - 1].currentMemberInArray;

    DetermineHashForNewArrayMember (state);

    return (SM_TRANSITION__SUCCEEDED);
}

/*****************************************************************************/
static sm_func_rc_T SmIncorporatePropertyName(_IO_ sm_state_T *   state)
{
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_JSON_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__JSON)
    {
        expanding_array_behavior_T expanding_array_behavior;
        json_rc_T json_rc;

        if(state->bFullParse)
        {
            expanding_array_behavior = ARRAYS__ALLOW_TO_EXPAND;
        }
        else
        {
            expanding_array_behavior = ARRAYS__KEEP_AS_IN_TEMPLATE;
        }

        json_rc = FindPropertyInBranch (
            (const property_table_entry__array_T **)&state->
            propertyFromTemplate.arrayStart,/* Lying about 'const'ness is a sin */
            (const property_table_entry_T        **)&state->
            propertyFromTemplate.item,
            state->output.dataBuf,/* JSON internal representation */
            &state->nesting,
            state->nesting.position - 1,
            state->tentativeHash,
            ARRAY_INDEX__NONE,
            expanding_array_behavior);
        if(json_rc != JSON_RC__OK)
        {
            if(json_rc != JSON_RC__NOT_FOUND)
            {
                UpdateBestCaseRc (&state->bestCaseRc, json_rc);
            }

            state->propertyFromTemplate.arrayStart = NULL;
            state->propertyFromTemplate.item = NULL;
        }
    }
#endif /* defined(ALLOW_PARSING__JSON) */

    state->nesting.stack[state->nesting.position].hash = state->tentativeHash;

    state->recentStringOffset = 0u;
    state->recentStringLen = 0u;

    if(state->bestCaseRc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (SM_TRANSITION__SUCCEEDED);
    }
    else
    {
        return (SM_TRANSITION__FAILED);
    }
}

/*****************************************************************************/
#if defined(ALLOW_PARSING__TEMPLATE)

static sm_func_rc_T IncorporateValue_Common_Template(
    _IO_ io_data_stream_cb_T * output,
    _I_ uint16_t hash,
    _I_ uint16_t property_type)
{
    property_table_entry_T *   entry;
    uint16_t needed_size;

    entry = (property_table_entry_T *)&output->dataBuf[output->position];

    needed_size = SizeOfTemplateEntry (property_type);

    if(output->position + needed_size > output->dataBufSize)
    {
        return (SM_TRANSITION__FAILED);
    }

    /***********************************************************/
    /* MemSet(0) Not strictly necessary,                       */
    /*  but cleans struct padding bytes in the output,         */
    /*  which is good form in security-sensitive applications. */
    /***********************************************************/
    MemSet (entry, 0, needed_size);

    entry->common.propertyHash = hash;
    entry->common.propertyType = property_type;

    if(IS_ARRAY (property_type))
    {
        entry->array.membersCount = 0;      /* Will be revisited and updated later in the processing */
    }

    output->position += needed_size;

    return (SM_TRANSITION__SUCCEEDED);
}

#endif /* defined(ALLOW_PARSING__TEMPLATE) */
/*****************************************************************************/
static sm_func_rc_T SmIncorporateValue(_IO_ sm_state_T *   state)
{
    sm_func_rc_T rc = SM_TRANSITION__SUCCEEDED;

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE (state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    {
        rc = IncorporateValue_Common_Template (
            &state->output,
            state->nesting.stack[state->
                                 nesting.
                                 position].
            hash,
            state->latestValue.propertyType);
    }
#endif
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    else
#endif
#if defined(ALLOW_PARSING__JSON)
    {
        const void *    value_ptr;
        uint16_t value_size;
        json_rc_T json_rc;

        if((state->propertyFromTemplate.arrayStart == NULL)
           && (state->propertyFromTemplate.item == NULL))
        {
            UpdateBestCaseRc (&state->bestCaseRc,
                              JSON_RC__RECOVERED__IGNORED_UNKNOWN_VALUE);

            return (SM_TRANSITION__SUCCEEDED);
        }

        if(state->latestValue.value.isNull)
        {
            value_ptr = NULL;
            value_size = 0u;
        }
        else if(state->latestValue.propertyType == PROPERTY_TYPE__UNKNOWN)
        {
            /**************************************************/
            /* Value doesn't exist - it's invalid - continue  */
            /**************************************************/

            state->latestValue.propertyType =
                state->propertyFromTemplate.item->common.propertyType;

            return (SM_TRANSITION__SUCCEEDED);
        }
        else if(PROPERTY_TYPE__CLEAN (state->latestValue.propertyType)
                != PROPERTY_TYPE__CLEAN (state->propertyFromTemplate.item->
                                         common.propertyType))
        {
            return (SM_TRANSITION__FAILED);
        }
        else if(((PROPERTY_TYPE__CLEAN (state->latestValue.propertyType)) &
                 PROPERTY_TYPE__MASK__NONREAL_TYPE) ==
                PROPERTY_TYPE__STRING_BASE)
        {
            value_ptr = &state->input.dataBuf[state->recentStringOffset];
            if(PROPERTY_TYPE__CLEAN (state->latestValue.propertyType) ==
               PROPERTY_TYPE__STRING)
            {
                value_size = state->recentStringCanonizedLen;
            }
            else if(PROPERTY_TYPE__CLEAN (state->latestValue.propertyType) ==
                    PROPERTY_TYPE__RAW)
            {
                value_size = state->recentStringLen;
            }
            else
            {
                return (SM_TRANSITION__FAILED);
            }
        }
        else
        {
            value_ptr = &state->latestValue.value.option;

            UpdateBestCaseRc (&state->bestCaseRc,
                              DetermineValueSize (&value_size,
                                                  state->latestValue.
                                                  propertyType));

            if(state->bestCaseRc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
            {
                return (SM_TRANSITION__FAILED);
            }
        }

        /*************************************************************************/
        /* found_array_start  &  found_property are NOT CONST ANYMORE            */
        /*                 Deplorable in terms of nice writing, but saves space  */
        /*************************************************************************/

        json_rc = SetValueInProperty          (
            state->output.dataBuf,/* JSON internal representation */
            (property_table_entry__array_T *)
            state->propertyFromTemplate.arrayStart,
            (property_table_entry_T *)state
            ->propertyFromTemplate.item,
            state->nesting.stack[state->
                                 nesting.position - 1].currentMemberInArray,
            value_ptr,
            value_size,
            state->bFullParse);

        UpdateBestCaseRc (&state->bestCaseRc, json_rc);

        if(state->bestCaseRc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            return (SM_TRANSITION__FAILED);
        }
    }
#endif /* defined(ALLOW_PARSING__JSON) */

    return (rc);
}

/*****************************************************************************/
static sm_func_rc_T SmIncorporateArrayDefaultValue(_IO_ sm_state_T *   state)
{
    SmNextArrayValue (state);

    MemSet (&state->latestValue, 0, sizeof(state->latestValue));

    return (SmIncorporateValue (state));
}

/*****************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable:4100)  /* 'state' : unreferenced formal parameter */
#endif

static sm_func_rc_T SmKeepTryingWithStateStart(_IO_ sm_state_T *   state)
{
    return ((sm_func_rc_T)STATE_START);
}

#ifdef _MSC_VER
#pragma warning (default:4100)  /* 'state' : unreferenced formal parameter */
#endif
/*****************************************************************************/
static json_rc_T FindInDictionary_AndSkip(
    __O uint16_t *                 found_code,
    _I_ string_dictionary_T *
    dictionary,
    _IO_ in_data_stream_cb_T *
    input_text)
{
    const char *    input_text_start = (const char *)input_text->dataBuf +
                                       input_text->position;
    int found = -1;
    int i;

    for(i = 0; found < 0; i++)
    {
        /*******************************************************************************/
        /* Don't trust library 'memcmp()' to behave consistently across all platforms  */
        /* ==> Check for 0-size NULL string separately                                 */
        /*******************************************************************************/
        if(dictionary[i].string == DICTIONARY_DEFAULT_AT_END)
        {
            found = i;
        }
        else
        {
            if((input_text->dataBufSize - input_text->position >=
                dictionary[i].stringLen)
               && (StrNiCmp (input_text_start, dictionary[i].string,
                             dictionary[i].stringLen) == 0))
            {
                found = i;
            }
        }
    }

    if(found >= 0)
    {
        input_text->position += dictionary[found].stringLen;

        *found_code = dictionary[found].code;

        SkipWhitespace (input_text);

        return (JSON_RC__OK);
    }

    /**************************************************************************/
    /* If the dictionary is well constructed - with DICTIONARY_DEFAULT_AT_END */
    /*  - shouldn't reach this point.                                         */
    /**************************************************************************/
    return (JSON_RC__UNEXPECTED_ERROR);
}

/*****************************************************************************/
#if 0
static const char * FindInDictionary_Code(
    const string_dictionary_T *     dictionary,
    int dictionary_size,
    uint16_t code_to_find)
{
    int i;

    for(i = 0; i < dictionary_size; i++)
    {
        if(dictionary[i].Code == code_to_find)
        {
            return (dictionary[i].String);
        }
    }

    return (NULL);
}

#endif
/*****************************************************************************/
#if defined(ALLOW_PARSING__TEMPLATE)

static _INLINE_ json_rc_T IdentifyAndSkip_PropertyType(
    _IO_ in_data_stream_cb_T * input_text,
    __O uint16_t *
    found_property_type)
{
    uint32_t qleft;
    uint32_t qright;
    json_rc_T rc;

    /*******************************/
    /* Look for int32, real32, ... */
    /*******************************/
    rc = FindInDictionary_AndSkip (found_property_type,
                                   gPropertyTypeSpecifier,
                                   input_text);

    if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        return (rc);
    }

    if((*found_property_type != PROPERTY_TYPE__REAL32__BASE)
       && (*found_property_type != PROPERTY_TYPE__UREAL32__BASE))
    {
        return (rc);       /* Done identifying property type */
    }

    if(ExpectCharacter (input_text, '<') != JSON_RC__OK)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__PARSING_FAILURE);/* Original rc is less important */
    }

    SkipWhitespace (input_text);

    /*******************/
    /* Look for Q-LEFT */
    /*******************/
    if(JSON_RC__OK != ReadIntegerAndSkip (&qleft,
                                          input_text,
                                          0,
                                          1 << PROPERTY_TYPE__REAL__Q_BITS,
                                          PROPERTY_TYPE__REAL__Q_LEFT__DEFAULT))
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__PARSING_FAILURE);/* Original rc is less important */
    }

    if(ExpectCharacter (input_text, ',') != JSON_RC__OK)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__PARSING_FAILURE);/* Original rc is less important */
    }

    /********************/
    /* Look for Q-RIGHT */
    /********************/
    if(JSON_RC__OK != ReadIntegerAndSkip (&qright,
                                          input_text,
                                          0,
                                          (1 <<
                                           PROPERTY_TYPE__REAL__Q_BITS) - 1u,
                                          PROPERTY_TYPE__REAL__Q_RIGHT__DEFAULT))
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__PARSING_FAILURE);/* Original rc is less important */
    }

    if(ExpectCharacter (input_text, '>') != JSON_RC__OK)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__PARSING_FAILURE);/* Original rc is less important */
    }

    if(((qleft == 0)  &&  (qright == 0))
       ||  ((qleft == 0)  &&
            ((*found_property_type & PROPERTY_TYPE_UPPER_MASK) ==
             PROPERTY_TYPE__REAL32__BASE)))
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__ILLEGAL_Q_VALUES);

        qleft = PROPERTY_TYPE__REAL__Q_LEFT__DEFAULT;
        qright = PROPERTY_TYPE__REAL__Q_RIGHT__DEFAULT;
    }

    if(qleft > PROPERTY_TYPE__REAL__MAX_BITS)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__ILLEGAL_Q_VALUES);

        qleft = PROPERTY_TYPE__REAL__MAX_BITS;
    }

    if(qright + qleft > PROPERTY_TYPE__REAL__MAX_BITS)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__ILLEGAL_Q_VALUES);

        qright = PROPERTY_TYPE__REAL__MAX_BITS - qleft;
    }

    if(qleft >= PROPERTY_TYPE__REAL__MAX_BITS)
    {
        if(*found_property_type == PROPERTY_TYPE__REAL32__BASE)
        {
            *found_property_type = PROPERTY_TYPE__INT32;/* real32<32,0>  == int32 */
        }
        else
        {
            *found_property_type = PROPERTY_TYPE__UINT32;/* ureal32<32,0> == uint32 */
        }
        return (rc);
    }

    if(qright >= PROPERTY_TYPE__REAL__Q_RIGHT__MAX_VALUE)
    {
        UpdateBestCaseRc (&rc, JSON_RC__RECOVERED__ILLEGAL_Q_VALUES);

        qright = PROPERTY_TYPE__REAL__Q_RIGHT__MAX_VALUE;
    }

    *found_property_type |= qright << PROPERTY_TYPE__REAL__Q_RIGHT__SHIFT;
    *found_property_type |= qleft << PROPERTY_TYPE__REAL__Q_LEFT__SHIFT;

    return (rc);
}

#endif /* defined(ALLOW_PARSING__JSON) */
/*****************************************************************************/
static _INLINE_ json_rc_T IdentifyToken(_IO_ sm_state_T *    state,
                                        __O uint8_t *         token)
{
    json_rc_T rc = JSON_RC__OK;

    *token = TOKEN_TYPE__UNKNOWN;

    if(state->input.position == state->input.dataBufSize)
    {
        *token = TOKEN_TYPE__END_OF_FILE;
    }
    else if(IsRawTypeExpected(state))
    {
        /* Raw format */
        rc = PrepareRawString (state);

        if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            state->latestValue.propertyType = PROPERTY_TYPE__RAW;
            state->latestValue.value.option.stringOffset =
                state->recentStringOffset;

            *token = TOKEN_TYPE__VALUE;
        }
        else if(rc == JSON_RC__NOT_FOUND)
        {
            rc = JSON_RC__OK;
        }
        else
        {
            UpdateBestCaseRc (&state->bestCaseRc, rc);
        }
    }
    else if((state->stateID == STATE_ARRAY_VALUE) &&
            (ExpectCharacter (&state->input, '[') == JSON_RC__OK))
    {
        *token = TOKEN_TYPE__CATCH_ALL;
        rc = JSON_RC__NOT_SUPPORTED;
        UpdateBestCaseRc (&state->bestCaseRc, rc);
    }

    if(*token == TOKEN_TYPE__UNKNOWN)  /* Continue the parsing */
    {
        if(ExpectCharacter (&state->input, '{') == JSON_RC__OK)
        {
            *token = TOKEN_TYPE__OBJECT_START;
        }
        else if(ExpectCharacter (&state->input, '}') == JSON_RC__OK)
        {
            *token = TOKEN_TYPE__OBJECT_END;
        }
        else if(ExpectCharacter (&state->input, '[') == JSON_RC__OK)
        {
            *token = TOKEN_TYPE__ARRAY_START;
        }
        else if(ExpectCharacter (&state->input, ']') == JSON_RC__OK)
        {
            *token = TOKEN_TYPE__ARRAY_END;
        }
        else if(ExpectCharacter (&state->input, ',') == JSON_RC__OK)
        {
            *token = TOKEN_TYPE__COMMA;
        }
        else if((rc = IdentifyStringLiteral (state)) >
                JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            UpdateBestCaseRc (&state->bestCaseRc, rc);

    #if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
            if(IS_TEMPLATE_PARSE (state->parsePassType))
    #endif
    #if defined(ALLOW_PARSING__TEMPLATE)
            {
                *token = TOKEN_TYPE__PROPERTY_NAME;
            }
    #endif
    #if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
            else
    #endif
    #if defined(ALLOW_PARSING__JSON)
            {
                if((state->stateID == STATE_OBJECT_VALUE)
                   ||  (state->stateID == STATE_ARRAY_VALUE))
                {
                    state->latestValue.propertyType = PROPERTY_TYPE__STRING;
                    state->latestValue.value.option.stringOffset =
                        state->recentStringOffset;

                    *token = TOKEN_TYPE__VALUE;
                }
                else
                {
                    *token = TOKEN_TYPE__PROPERTY_NAME;
                }
            }
    #endif
            if(*token == TOKEN_TYPE__PROPERTY_NAME)
            {
                if(ExpectCharacter (&state->input, ':') != JSON_RC__OK)
                {
                    UpdateBestCaseRc (&state->bestCaseRc,
                                      JSON_RC__RECOVERED__PARSING_FAILURE);
                }

                SkipWhitespace (&state->input);
            }
        }
        else if(rc == JSON_RC__NOT_FOUND)/* IdentifyStringLiteral() simply didn't find a string literal */
        {
            rc = JSON_RC__OK;

    #if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
            if(IS_TEMPLATE_PARSE (state->parsePassType))
    #endif
    #if defined(ALLOW_PARSING__TEMPLATE)
            {
                UpdateBestCaseRc (&rc,
                                  IdentifyAndSkip_PropertyType (&state->input,
                                                                &state->
                                                                latestValue.
                                                                propertyType));

                if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
                {
                    *token = TOKEN_TYPE__BASIC_PROPERTY_TYPE;
                }
            }
    #endif /* ALLOW_PARSING__TEMPLATE */
    #if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
            else
    #endif
    #if defined(ALLOW_PARSING__JSON)
            {
                UpdateBestCaseRc (&rc, IdentifyAndSkip_LiteralValue (state));

                if(rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
                {
                    *token = TOKEN_TYPE__VALUE;
                }
            }
    #endif /* ALLOW_PARSING__JSON */
        }
    }

    if((rc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
       && (*token == TOKEN_TYPE__UNKNOWN))
    {
        rc = JSON_RC__NOT_FOUND;
    }

    UpdateBestCaseRc (&rc, state->bestCaseRc);

    return (rc);
}

/*****************************************************************************/
static sm_func_rc_T StateMachineClick(_IO_ sm_state_T *  state,
                                      _I_ uint8_t token)
{
    sm_state_id_T current_state;
    sm_func_rc_T transition_rc;
    int rule = 0;
    bool restart = false;

    current_state = state->stateID;

    while(rule < ARRAY_SIZEOF(gStateMachineRules))
    {
        if((current_state == gStateMachineRules[rule].currentState)
           && ((token == gStateMachineRules[rule].triggerToken)
               || (TOKEN_TYPE__CATCH_ALL ==
                   gStateMachineRules[rule].triggerToken)))
        {
            transition_rc = gStateMachineRules[rule].stateTransitionFunc (state);

            if(transition_rc > 0)
            {
                current_state = (sm_state_id_T)transition_rc;

                restart = true;
            }
            else if(transition_rc == SM_TRANSITION__KEEP_TRYING)
            {
                /* Do nothing.  Just keep trying to match */
            }
            else if(transition_rc == SM_TRANSITION__SUCCEEDED_AND_SET_STATE)
            {
                /**********************/
                /* Already set state. */
                /**********************/
                return (SM_TRANSITION__SUCCEEDED);
            }
            else
            {
                if(transition_rc == SM_TRANSITION__SUCCEEDED)
                {
                    state->stateID =
                        (sm_state_id_T)gStateMachineRules[rule].newState;
                }

                return (transition_rc);
            }
        }

        if(restart)
        {
            rule = 0;

            restart = false;
        }
        else
        {
            rule++;
        }
    }

    return (SM_TRANSITION__FAILED);
}

/*****************************************************************************/
static json_rc_T IdentifyStringLiteral(_IO_ sm_state_T *    state)
{
    uint16_t index_into_input;
    int32_t canonized_value;
    json_rc_T rc;

    SkipWhitespace (&state->input);

    if(state->input.position >= state->input.dataBufSize)
    {
        return (JSON_RC__NOT_FOUND);
    }

    if(state->input.dataBuf[state->input.position] != '"')
    {
        return (JSON_RC__NOT_FOUND);
    }

    index_into_input = state->input.position + 1;   /* First character in string is just after the quotes */

    state->recentStringOffset = index_into_input;

    state->recentStringCanonizedLen = 0u;

    state->tentativeHash =
        state->nesting.stack[state->nesting.position - 1].hash;

    while((index_into_input < state->input.dataBufSize)
          && (state->input.dataBuf[index_into_input] != '"'))
    {
        rc = GetCanonizedCharacter (&canonized_value,
                                    &index_into_input,
                                    (const char *)state->input.dataBuf,
                                    state->input.dataBufSize);

        if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            return (rc);
        }
        else
        {
            UpdateBestCaseRc (&state->bestCaseRc, rc);
        }

        ++state->recentStringCanonizedLen;

        if(canonized_value != CANONIZED_VALUE__NOT_FOUND)
        {
            state->tentativeHash =
                NextHash (state->tentativeHash,
                          (uint8_t)(canonized_value & 0xFFu));

            /*******************************************************/
            /* writing unicode in little-endian, regardless of BOM */
            /*******************************************************/
            if(canonized_value & CANONIZED_VALUE__UNICODE)
            {
                state->tentativeHash =
                    NextHash (state->tentativeHash,
                              (uint8_t)((canonized_value >> 8) & 0xFFu));
            }
        }
    }

    if((index_into_input >= state->input.dataBufSize)
       || (state->input.dataBuf[index_into_input] != '"'))
    {
        return (JSON_RC__PARSING_FAILURE__INCOMPLETE_STRING);
    }

    index_into_input++;

    state->input.position = index_into_input;

    state->recentStringLen = index_into_input - state->recentStringOffset - 1;  /* No need for the '"' */

#if defined(ALLOW_PARSING__TEMPLATE)
    if(IS_TEMPLATE_PASS_FINAL (state->parsePassType))
    {
        if(state->positionForStrings + state->recentStringLen + 1 >=
           state->output.dataBufSize)
        {
            return (JSON_RC__PARSING_BUFFER_SIZE_EXCEEDED);
        }

        MemCpy (&state->output.dataBuf[state->positionForStrings],
                &state->input.dataBuf[state->recentStringOffset],
                state->recentStringLen);

        state->output.dataBuf[state->positionForStrings +
                              state->recentStringLen] = '\0';

        state->positionForStrings += state->recentStringLen + 1;
    }
#endif

    SkipWhitespace (&state->input);

    return (state->bestCaseRc);
}

/*****************************************************************************/
static bool IsRawTypeExpected(_IO_ sm_state_T *    state)
{
    bool retVal = false;

    if(!IS_TEMPLATE_PARSE (state->parsePassType))
    {
        if((state->stateID == STATE_OBJECT_VALUE) ||
           (state->stateID == STATE_ARRAY_VALUE))
        {
            if((state->propertyFromTemplate.item != NULL) &&
               (PROPERTY_TYPE__CLEAN (state->propertyFromTemplate.item->common.
                                      propertyType) == PROPERTY_TYPE__RAW))
            {
                retVal = true;
            }
        }
    }

    return (retVal);
}

/*****************************************************************************/
static json_rc_T PrepareRawString(_IO_ sm_state_T *    state)
{
    int32_t nesting_level;

    if(state->input.position >= state->input.dataBufSize)
    {
        return (JSON_RC__NOT_FOUND);
    }

    if(state->input.dataBuf[state->input.position] != '{')
    {
        return (JSON_RC__NOT_FOUND);
    }

    state->recentStringOffset = state->input.position; /* First character in raw string should includes the '{' */

    state->input.position++;

    nesting_level = 0;
    while((nesting_level >= 0)
          && (state->input.position < state->input.dataBufSize))
    {
        if((state->input.dataBuf[state->input.position] == '{')
           || (state->input.dataBuf[state->input.position] == '['))
        {
            nesting_level++;
        }
        else if((state->input.dataBuf[state->input.position] == '}')
                || (state->input.dataBuf[state->input.position] == ']'))
        {
            nesting_level--;
        }

        state->input.position++;
    }

    state->recentStringLen = state->input.position - state->recentStringOffset; /* Points to the closing '}'*/

    SkipWhitespace (&state->input);

    return (JSON_RC__OK);
}

/*****************************************************************************/
static _INLINE_ void InitializeState(_IO_ sm_state_T *  state,
                                     _IO_ void *        output_buf,
                                     _I_ uint16_t output_buf_size,
                                     _I_ char *        input_text,
                                     _I_ uint16_t input_text_size,
                                     _I_ parse_pass_T parse_pass_type)
{
    MemSet (state, 0, sizeof(*state));

    state->parsePassType = parse_pass_type;

    InvalidateLatestValue (state);

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE(state->parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    {
        json_template_T *   output_template = (json_template_T *)output_buf;

        if(IS_TEMPLATE_PASS_PREPASS (state->parsePassType))
        {
            /*************************************************************************************/
            /* For PARSE_PASS__TEMPLATE_PREPASS, PositionForStrings is ignored until end of func */
            /*************************************************************************************/
/*          state->PositionForStrings                 = 0u ; ... No need because of MemSet(0) */
/*          output_template->Header.PropertyTableSize = 0u ; ... No need because of MemSet(0) */
        }
        else
        {
            /*************************************************************************************/
            /* For PARSE_PASS__TEMPLATE_FINAL,   PositionForStrings is for property-names        */
            /*************************************************************************************/
            state->positionForStrings = sizeof(output_template->header)
                                        + output_template->header.
                                        propertyTableSize;
        }
    }
#endif /* defined(ALLOW_PARSING__TEMPLATE) */

    state->input.dataBuf = (const uint8_t *)input_text;
    state->input.dataBufSize = input_text_size;
/*  state->Input  .Position      =                 0u              ; ... No need because of MemSet(0) */

    state->output.dataBuf = (uint8_t *)output_buf;
    state->output.dataBufSize = output_buf_size;

    if(IS_TEMPLATE_PASS_PREPASS (parse_pass_type))
    {
        ((json_template_T *)state->output.dataBuf)->header.propertyTableSize =
            0u;
    }

    state->output.position =
        sizeof(((json_template_T *)state->output.dataBuf)->header);

/*  state->Nesting.Position      =  0u ; ... No need because of MemSet(0) */
    state->nesting.stack[0].hash =
        ((json_template_T *)state->output.dataBuf)->header.hashSeed;

    state->stateID = STATE_START;

/*  state->BestCaseRc            = JSON_RC__OK ; ... No need because of MemSet(0) */

    if(IS_FULL_PARSE (parse_pass_type))
    {
        state->bFullParse = true;
    }
}

/*****************************************************************************/
/* The only syntax-checking done here are:                                   */
/* 1. Looking for end-of-string according to json_text_len,                  */
/*      to ensure the func returns, and doesn't run beyond the string's end. */
/* 2. following the nesting level, to ensure no memory-overwrite occurs.     */
/*****************************************************************************/
json_rc_T ParseCommon(_I_ parse_pass_T parse_pass_type,
                      _IO_ void *              output_buf,
                      _IO_ uint16_t *            phase_output_size,
                      _I_ char *              input_text,/* Could be JSON, Could be partly templetized JSON */
                      _I_ uint16_t input_text_size)
{
    sm_state_T state;
    uint8_t token;
    uint16_t properties_counter = 0u;
    sm_func_rc_T transition_rc = SM_TRANSITION__SUCCEEDED;
    json_rc_T rc;

#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE(parse_pass_type))
    {
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    if(*phase_output_size < sizeof(json_template_header_T))
    {
        return (JSON_RC__PARSING_BUFFER_SIZE_EXCEEDED);
    }
#endif
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    else
#endif
#if defined(ALLOW_PARSING__JSON)
    if(*phase_output_size < sizeof(json_internal_header_T))
    {
        return (JSON_RC__PARSING_BUFFER_SIZE_EXCEEDED);
    }
#endif
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
}

#endif

    InitializeState (&state,
                     output_buf,
                     *phase_output_size,
                     input_text,
                     input_text_size,
                     parse_pass_type);

    SkipWhitespace (&state.input);
    /*-----------------------------------------------------------------------*/
    while((state.input.position <= state.input.dataBufSize)
          && (state.stateID != STATE_END)
          && (state.bestCaseRc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
          && (transition_rc == SM_TRANSITION__SUCCEEDED))
    {
        rc = IdentifyToken (&state, &token);

        if(rc < JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
        {
            return (rc);
        }
        else
        {
            UpdateBestCaseRc (&state.bestCaseRc, rc);
        }

        if(token == TOKEN_TYPE__PROPERTY_NAME)
        {
            properties_counter++;
        }

        transition_rc = StateMachineClick (&state, token);
    }
    /*-----------------------------------------------------------------------*/
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    if(IS_TEMPLATE_PARSE(state.parsePassType))
#endif
#if defined(ALLOW_PARSING__TEMPLATE)
    {
        json_template_header_T *   template_header;

        if(IS_FINAL_PASS (state.parsePassType))
        {
            *phase_output_size = state.positionForStrings;
        }
        else
        {
            template_header = (json_template_header_T *)output_buf;

            *phase_output_size = state.output.position;  /*@ Use json_header->MaximumSize, json_header->CurrentSize */

            template_header->propertyTableSize = state.output.position -
                                                 sizeof(json_template_header_T);
        }
    }
#endif /* defined(ALLOW_PARSING__TEMPLATE) */
#if defined(ALLOW_PARSING__TEMPLATE)  &&  defined(ALLOW_PARSING__JSON)
    else
#endif
#if defined(ALLOW_PARSING__JSON)
    {
        json_internal_header_T *   json_header =
            (json_internal_header_T *)output_buf;

        *phase_output_size = json_header->currentSize;
    }
#endif

    if(state.bestCaseRc > JSON_RC__RECOVERABLE_ERROR__MINIMUM_VALUE)
    {
        if(state.stateID != STATE_END)
        {
            return (JSON_RC__PARSING_FAILURE);
        }

        if(transition_rc != SM_TRANSITION__SUCCEEDED)
        {
            return (JSON_RC__PARSING_FAILURE);
        }
    }

    return (state.bestCaseRc);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
