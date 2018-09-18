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
 /** ============================================================================
 *  @file       str_mpl.h
 *
 *  @brief      String Manipulation for SimpleLink SDK
 *
 *  ============================================================================
 */

/*! @cond*/

#ifndef __STR_MPL_H__
#define __STR_MPL_H__

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdint.h>

//*****************************************************************************
// defines
//*****************************************************************************
#define STRMPL_ERROR_PARAM_MISSING          (-1)
#define STRMPL_ERROR_MEM_ALLOCATION         (-2)
#define STRMPL_ERROR_DELIM_MISSING          (-3)
#define STRMPL_ERROR_WRONG_PARAM            (-4)
#define STRMPL_ERROR_WRONG_SIZE             (-5)

#define STRMPL_FLAG_PARAM_SIZE_8             (0x00000001)
#define STRMPL_FLAG_PARAM_SIZE_16            (0x00000002)
#define STRMPL_FLAG_PARAM_SIZE_32            (0x00000004)

#define STRMPL_FLAG_PARAM_SIGNED             (0x00000100)
#define STRMPL_FLAG_PARAM_UNSIGNED           (0x00000200)
#define STRMPL_FLAG_PARAM_DEC                (0x00000400)
#define STRMPL_FLAG_PARAM_HEX                (0x00000800)

//*****************************************************************************
// typedefs
//*****************************************************************************

typedef struct _StrMpl_List_t_
{
    char        *str;
    int32_t     value;
}StrMpl_List_t;

/*! @endcond*/

//*****************************************************************************
// function prototypes
//*****************************************************************************
/*!
    \brief          String manipulation - get number of parameters.

    This routine count only the number of delimiters and does not check if 
    parameter exist or not.

    \param          src             -   points to source string
    \param          intraDelim      -   hold expected delimiter between parameters 
    \param          termDelim       -   hold expected delimiter after last parameter 
                                        current and next parameter  
    \param          excludeDelim    -   points to array of 2 chars which hold the 
                                        expected open and close exclude delimiters

    \return         return number of parameters;
*/
uint32_t StrMpl_getNumVals(char *src, char intraDelim, char termDelim, char *excludeDelim);

/*!
    \brief          String manipulation - converts a given string to lowercase.

    \param          str             -   points to source string

    \return         None
*/
void StrMpl_lwr(char *str);

/*!
    \brief          String manipulation - remove spaces from a given string.
    
    This routine remove spaces from a given string unless it enclosed with exclude delimiters.

    \param          str             -   points to source string
    \param          delim           -   hold the expected delimiter
    \param          excludeDelim    -   points to array of 2 chars which hold the
                                        expected open and close exclude delimiters
    \return         None
*/
void StrMpl_rmspc(char *str, char delim, char *excludeDelim);

/*!
    \brief          String manipulation - Get list numeric value.
    
    This routine search given string in given list and return the appropriate value.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter 

    \param          list            -   points to given list
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given source string 
    \param          des             -   points to destination numeric value 
    \param          delim           -   hold the expected delimiter after source string
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des, char delim, uint32_t flags);

/*!
    \brief          String manipulation - Set list string value.
    
    This routine search for given value in given list and print
    the appropriate string and given delimiter in destination string 
    It also updates the destination string pointer to point the end of string. 

    \param          list            -   points to given list
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given source value 
                                        (source value size should be defined in flags)
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source string
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and/or
                                        STRMPL_FLAG_PARAM_SIGNED - for signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for unsigned numbers
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return STRMPL_ERROR_WRONG_PARAM;
*/
int32_t StrMpl_setListStr(StrMpl_List_t *list, uint32_t listSize, void *src, char **des, char delim, uint32_t flags);

/*!
    \brief          String manipulation - Get numeric value.
    
    This routine scan for numeric value in a given source string
    numeric value expected to be decimal or hexadecimal (with prefix of 0x),
    convert it to numeric value and places it in destination buffer.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter 

    \param          src             -   points to given source string 
    \param          des             -   points to destination buffer
                                        (destination value size should be defined in flags)    
    \param          delim           -   hold the expected delimiter after expected string
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getVal(char **src, void *des, char delim, uint32_t flags);

/*!
    \brief          String manipulation - Set numeric value.
    
    This routine print a given numeric value and a given delimiter in destination string 
    It also updates the destination string pointer to point the end of string. 

    \param          src             -   points to given source value 
                                        (source value size should be defined in flags)
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source string
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and
                                        STRMPL_FLAG_PARAM_SIGNED - for print signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for print unsigned numbers
                                        and
                                        STRMPL_FLAG_PARAM_DEC - for print decimal number
                                        STRMPL_FLAG_PARAM_HEX - for print hexadecimal number
    \return         function return 0
*/
int32_t StrMpl_setVal(void *src, char **des, char delim, uint32_t flags);

/*!
    \brief          String manipulation - Get array of numeric values.
    
    This routine scan for number of numeric values in a given source string
    numeric values expected to be decimal or hexadecimal (with prefix of 0x)
    and same delimiter between them, convert them to numeric value and place them in destination array.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter 

    \param          src             -   points to given source string 
    \param          des             -   points to destination array
                                        (destination array memebers size should be defined in flags)    
    \param          arrayLen        -   hold the expected number of numeric values to scan
    \param          delim           -   hold the expected delimiter after expected string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the array values 
                                        (this value could be NULL unless the delim and intraDelim have the same char)
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getArrayVal(char **src, void *des,uint32_t arrayLen, char delim, char intraDelim, uint32_t flags, char *excludeDelim);

/*!
    \brief          String manipulation - Set array of numeric values.
    
    This routine print a given numeric values array and a given delimiter in destination string 
    It also updates the destination string pointer to point the end of string. 

    \param          src             -   points to numeric values array 
                                        (source values size should be defined in flags)
    \param          des             -   points to destination string 
    \param          arrayLen        -   hold the array size
    \param          delim           -   hold the expected delimiter after source string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and
                                        STRMPL_FLAG_PARAM_SIGNED - for print signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for print unsigned numbers
                                        and
                                        STRMPL_FLAG_PARAM_DEC - for print decimal number
                                        STRMPL_FLAG_PARAM_HEX - for print hexadecimal number
    \return         function return 0
*/
int32_t StrMpl_setArrayVal(void *src, char **des, uint32_t arrayLen, char delim, char intraDelim, uint32_t flags);

/*!
    \brief          String manipulation - Get array of list values.  

    This routine scans number of strings in a given source string
    while same delimiter seperates between them, search them in a given list
    and place the appropriate values in destination array.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter 

    \param          list            -   points to given list 
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given source string 
    \param          des             -   points to destination array
                                        (destination array memebers size should be defined in flags)    
    \param          arrayLen        -   hold the expected number of numeric values to scan
    \param          delim           -   hold the expected delimiter after expected string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the array values 
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getArrayListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des,uint32_t arrayLen, char delim, char intraDelim, char *excludeDelim, uint32_t flags);

/*!
    \brief          String manipulation - Set array of list values.
    
    This routine search a given array values in given list and print
    the appropriate strings, delimiter between them and terminate delimiter in destination string. 
    It also updates the destination string pointer to point the end of string. 

    \param          list            -   points to given list 
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given array 
                                        (array members size should be defined in flags)
    \param          des             -   points to destination string 
    \param          arrayLen        -   hold the array size
    \param          delim           -   hold the expected delimiter after source string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and
                                        STRMPL_FLAG_PARAM_SIGNED - for print signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for print unsigned numbers
                                        and
                                        STRMPL_FLAG_PARAM_DEC - for print decimal number
                                        STRMPL_FLAG_PARAM_HEX - for print hexadecimal number
    \return         function return 0
*/
int32_t StrMpl_setArrayListStr(StrMpl_List_t *list, uint32_t listSize,void *src, char **des, uint32_t arrayLen, char delim, char intraDelim, uint32_t flags);


/*!
    \brief          String manipulation - Get array of bits values.  

    This routine scans number of numeric values represent as bits 
    in a given source string while same delimiter seperates between 
    them and place the values in destination value.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter

    \param          src             -   points to given source string 
    \param          des             -   points to destination value
                                        (destination value size should be defined in flags)    
    \param          delim           -   hold the expected delimiter after expected string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the bitmask values 
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte destination size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes destination size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes destination size
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getArrayBitVal(char **src, void *des, char delim, char intraDelim, char *excludeDelim, uint32_t flags);


/*!
    \brief          String manipulation - Set array of bits values.
    
    This routine takes a given numeric value and factorise it to bits,
    print the bits place, delimiter between them and terminate 
    delimiter in destination string. 
    It also updates the destination string pointer to point the end of string. 

    \param          src             -   points to given bitmask value 
                                        (bitmask value size should be defined in flags)
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and
                                        STRMPL_FLAG_PARAM_SIGNED - for print signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for print unsigned numbers
                                        and
                                        STRMPL_FLAG_PARAM_DEC - for print decimal number
                                        STRMPL_FLAG_PARAM_HEX - for print hexadecimal number
    \return         function return 0
*/
int32_t StrMpl_setArrayBitVal(void *src, char **des, char delim, char intraDelim, uint32_t flags);


/*!
    \brief          String manipulation - Get bitmask of list values.  

    This routine scans number of strings in a given source string
    while same delimiter seperates between them, search them in a given list
    and place the appropriate values as bitmask in destination value.
    it also replaces the expected delimiter with NULL and
    updates the source string pointer to point the next parameter 

    \param          list            -   points to given list 
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given source string 
    \param          des             -   points to destination value
                                        (destination value size should be defined in flags)    
    \param          delim           -   hold the expected delimiter after expected string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the bitmask values 
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getBitmaskListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des, char delim, char intraDelim, char *excludeDelim, uint32_t flags);

/*!
    \brief          String manipulation - Set bitmask of list values.
    
    This routine takes a given bitmask value and factorise it to bits.
    search the bits in given list and print the appropriate strings, 
    delimiter between them and terminate delimiter in destination string. 
    It also updates the destination string pointer to point the end of string. 

    \param          list            -   points to given list 
    \param          listSize        -   hold the given list size 
    \param          src             -   points to given bitmask value 
                                        (bitmask value size should be defined in flags)
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source string
    \param          intraDelim      -   hold the expected delimiter between numeric values
    \param          flags           -   hold one of the following:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
                                        and
                                        STRMPL_FLAG_PARAM_SIGNED - for print signed numbers
                                        STRMPL_FLAG_PARAM_UNSIGNED - for print unsigned numbers
                                        and
                                        STRMPL_FLAG_PARAM_DEC - for print decimal number
                                        STRMPL_FLAG_PARAM_HEX - for print hexadecimal number
    \return         function return 0
*/
int32_t StrMpl_setBitmaskListStr(StrMpl_List_t *list, uint32_t listSize,void *src, char **des, char delim, char intraDelim, uint32_t flags);

/*!
    \brief          String manipulation - Get sub string from string.
    
    This routine replaces the expected delimiter with NULL,
    scan for sub string in a given source string
    and places it in destination buffer.
    it also updates the source string pointer to point the next parameter 

    \param          src             -   points to given source string 
    \param          des             -   points to destination buffer
                                        (destination max length should be defined in maxSize)    
    \param          delim           -   hold the expected delimiter after expected string
    \param          maxSize         -   hold max destination buffer size
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the sub string 

    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getStr(char **src, char *des, char delim, uint32_t maxSize,char *excludeDelim);

/*!
    \brief          String manipulation - Get sub string from string with memory allocation.
    
    This routine replaces the expected delimiter with NULL,
    scan for sub string in a given source string, allocate the destination buffer
    and places the sub string in it.
    it also updates the source string pointer to point the next parameter 

    \param          src             -   points to given source string 
    \param          des             -   points to destination buffer
                                        (destination max length should be defined in maxSize)    
    \param          delim           -   hold the expected delimiter after expected string
    \param          maxSize         -   hold max destination buffer size
    \param          excludeDelim    -   points to array of 2 chars which define the
                                        boundaries of the sub string 

    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_getAllocStr(char **src, char **des, char delim, uint32_t maxSize,char *excludeDelim);

/*!
    \brief          String manipulation - Set sub string into string buffer.
    
    This routine print a given sub string and a given delimiter in destination string 
    It also updates the destination string pointer to point the end of string. 

    \param          src             -   points to given sub string 
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source sub string

    \return         function return 0
*/
int32_t StrMpl_setStr(char *src, char **des, char delim);

/*!
    \brief          Get Base64 decoded buffer size
    
    This routine calculate the expected raw data buffer size 
    by given base64 buffer and buffer size    

    \param          inputData       -  source buffer which hold the base64 data 
    \param          inputLength     -  length of base64 data 

    \return         function shall return expected size.
*/
uint32_t StrMpl_getBase64DecBufSize(uint8_t *inputData, uint32_t inputLength);

/*!
    \brief          Get Base64 encoded buffer size
    
    This routine calculate the expected Base64 buffer size 
    by given raw data size    

    \param          inputLength     -  raw buffer size

    \return         function shall return expected size.
*/
uint32_t StrMpl_getBase64EncBufSize(uint32_t inputLength);

/*!
    \brief          Decode Base64 data to raw data
    
    This routine decode a given data in Base64 format to raw data,
    and return it into a given buffer - outputData (which should be already allocated). 
    size of the outputData buffer also be returned

    \param          inputData       -  source buffer which hold the Base64 data 
    \param          inputLength     -  source buffer size 
    \param          outputData      -  destination buffer which hold the raw data 
    \param          outputLength    -  destination buffer size 

    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_decodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength);

/*!
    \brief          Encode raw data to Base64 format
    
    This routine encode a given raw data to base64 format and return it into
    a given buffer - outputData (which should be already allocated). 
    size of the outputData buffer also be returned

    \param          inputData       -  source buffer which hold the raw data 
    \param          inputLength     -  source buffer size 
    \param          outputData      -  destination buffer which hold the base64 data 
    \param          outputLength    -  destination buffer size 

    \return         Upon successful, the function shall return 0
                    In case of failure, this function would return one of the string manipulation errors;
*/
int32_t StrMpl_encodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength);

/** @} */



#endif /* __STR_MPL_H__ */

