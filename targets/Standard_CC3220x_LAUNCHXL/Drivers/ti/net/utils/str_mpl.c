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

//*****************************************************************************
// includes
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ti/net/utils/str_mpl.h>

//*****************************************************************************
//defines
//*****************************************************************************


//*****************************************************************************
// Globals
//*****************************************************************************
uint8_t StrMpl_base64EncTable[64] =  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                            'w', 'x', 'y', 'z', '0', '1', '2', '3',
                            '4', '5', '6', '7', '8', '9', '+', '/'};

uint8_t StrMpl_base64DecTable[123] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,
                            52,53,54,55,56,57,58,59,60,61, /* 0-9 */
                            0,0,0,0,0,0,0,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25, /* A-Z */
                            0,0,0,0,0,0,
                            26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51}; /* a-z */


//*****************************************************************************
//String manipulation internal routines 
//*****************************************************************************


/*!
    \brief          String manipulation - set value by size.

    This routine set destination value and cast it the requested size

    \param          src             -   points to source numeric value
    \param          des             -   points to destination numeric value 
    \param          size            -   hold the requested size:
                                        STRMPL_FLAG_PARAM_SIZE_8 - for 1 byte size
                                        STRMPL_FLAG_PARAM_SIZE_16 - for 2 bytes size
                                        STRMPL_FLAG_PARAM_SIZE_32 - for 4 bytes size
    
    \return         Upon successful, the function shall return 0.
                    In case unsupported size, this function would return STRMPL_ERROR_WRONG_SIZE;

*/
int32_t StrMpl_setBySize(void *src, void *des, uint32_t size)
{
    switch (size)
    {
        case STRMPL_FLAG_PARAM_SIZE_8:
            *(int8_t *)des = *(int8_t *)src;
        break;
        case STRMPL_FLAG_PARAM_SIZE_16:
            *(int16_t *)des = *(int16_t *)src;
        break;
        case STRMPL_FLAG_PARAM_SIZE_32:
            *(int32_t *)des = *(int32_t *)src;
        break;
        default:
            return STRMPL_ERROR_WRONG_SIZE;
    }
    return 0;
}


/*!
    \brief          String manipulation - search for exclude marks.

    This routine search for open exclude delimiter (first char in excludeDelim).
    Second, search for the close exclude delimiter and third, search for the parameter delimiter
    case modify is true, it replace the close exclude delimiter with NULL.

    \param          str             -   points to source string
    \param          modify          -   hold (1) for setting NULL instead of the close exclude delimiter and
                                        update source string pointer to the beginning of the exclude string
                                        hold (0) for only update source string pointer to after the exclude string
    \param          delim           -   hold the expected delimiter between 
                                        current and next parameter  
    \param          excludeDelim    -   points to array of 2 chars which hold the 
                                        expected open and close exclude delimiters
    \param          last            -   points to next parameter.

    \return         In case exclude delimiters were found and parameter delimiter was found, the function shall return 0.
                    In case no exclude delimiters were defined, this function would return -1;
                    In case no exclude delimiters were found, this function would return -1;

*/
int32_t StrMpl_excludeMarks(char **str, uint8_t modify, char delim, char *excludeDelim, char **last)
{
    char *s;
    char *openMark;
    char *closeMark;
    uint8_t stage = 0;

    /* check exclude delimiters were defined */
    if (excludeDelim == NULL)
    {
        return -1;
    }
    /* go over the given string */
    s = *str;
    while (*s != 0)
    {
        /* search for open delimiter */
        if (stage == 0)
        {
            if (*s == excludeDelim[0]) 
            {
                /* open delimiter was found */
                stage = 1;
                openMark = s;
                s++;
            }
            else if (*s != ' ')
            {
                break;
            }
        }
        /* search for close delimiter */
        if (stage == 1)
        {
            if (*s == excludeDelim[1])
            {
                /* close delimiter was found */
                stage = 2;
                closeMark = s;
                s++;
            }          
        }
        /* search for the parameter delimiter */
        if (stage == 2)
        {
            if ((*s == delim) || (*s == 0))
            {
                /* parameter delimiter was found */
                stage = 3;
                if (last != NULL)
                {
                    *last = s;
                }
                break;
            }
            else if (*s != ' ') 
            {
                /* revert to stage 1 in order to find the close delimiter */
                stage = 1;
            }
        }
        s++;
    }
    /* case open and close exclude delimiters were found and parameter delimiter was found */
    if (stage == 3)
    {
        /* case we want to modify the source string */
        if (modify)
        {
            /* update source string pointer to begining of exclude string */
            *str = openMark + 1;
            /* set NULL instead of the close exclude delimiter */
            *closeMark = 0;
        }
        else
        {
            /* update source string pointer after the exclude string */
            *str = closeMark + 1;
        }
        return 0;
    }
    else
    {
        /* no exclude delimiters were found */
        return -1;
    }
}

/*!
    \brief          String manipulation - prepare string.

    This routine search for given delimiter and set it to NULL (0).

    \param          src             -   points to source string
    \param          delim           -   hold the expected delimiter between 
                                        current and next parameter  
    \param          excludeDelim    -   points to array of 2 chars which hold the 
                                        expected open and close exclude delimiters
    \param          last            -   points to next parameter.

    \return         Upon successful, the function shall return 0.
                    In case of missing delimiter, this function would return STRMPL_ERROR_DELIM_MISSING;
                    In case of missing parameter, this function would return STRMPL_ERROR_PARAM_MISSING;

*/
int32_t StrMpl_prepare(char **src, char delim, char *excludeDelim, char **last)
{
    *last = NULL;

    /* check that the first char is not the delimiter */
    if (**src != delim)
    {
        /* Search for exclude marks - in case not exist, search for delimiter*/
        if (StrMpl_excludeMarks(src, 1, delim,excludeDelim, last) == -1)
        {
            /* Search for delimiter in case did not find exclude marks */
            *last = strchr(*src, delim);
        }
        /* case no delimiter found */
        if (*last == NULL)
        {
            return STRMPL_ERROR_DELIM_MISSING;
        }
        /* replace the given delimiter to NULL */
        **last = 0;
        /* */
        *last = *last + 1;
    }
    else
    {        
        /* case no parameters found */
        if (**src != 0)
        {
            *last = *src + 1;
        } 
        return STRMPL_ERROR_PARAM_MISSING;
    }

    return 0;
}


//*****************************************************************************
//String manipulation public routines 
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
uint32_t StrMpl_getNumVals(char *src, char intraDelim, char termDelim, char *excludeDelim)
{    
    uint32_t i = 0;
    char *token = src;
    char *last;
    uint8_t exclude = 0;

    /* case of terminate delimiter or no terminate delimiter */
    if ((*src == termDelim) || ((last = strchr(src,termDelim)) == NULL))
    {
        return 0;
    }
    i++;

    while (token < last)
    {   
        if (excludeDelim != NULL)
        {
            if ((*token == excludeDelim[0]) && (exclude == 0))
            {
                exclude = 1;
            }
            else if ((*token == excludeDelim[1]) && (exclude == 1))
            {
                exclude = 0;
            }
        }
        if ((*token == intraDelim) && (exclude == 0))
        {
            i++;
        }
        token++;
    }

    return i;
}

/*!
    \brief          String manipulation - converts a given string to lowercase.

    \param          str             -   points to source string

    \return         None
*/
void StrMpl_lwr(char *str)
{
    char *s;

    for(s = str; *s; s++)
    {
      *s = tolower((unsigned char)*s);
    }
}

/*!
    \brief          String manipulation - remove spaces from a given string.
    
    This routine remove spaces from a given string unless it enclosed with exclude delimiters.

    \param          str             -   points to source string
    \param          delim           -   hold the expected delimiter
    \param          excludeDelim    -   points to array of 2 chars which hold the
                                        expected open and close exclude delimiters
    \return         None
*/
void StrMpl_rmspc(char *str, char delim, char *excludeDelim)
{
    char *s;
    char *t;
    uint8_t rm = 1;
    uint8_t count = 0;
    
    for(s = str; *s; )
    {
        /* case current char is space */
        if (*s == ' ') 
        {
            count++;
        }
        /* case exclude delimiters were defined and current char is open exclude delimiter */
        if ((excludeDelim != NULL) && (*s == excludeDelim[0]))
        {
            if (StrMpl_excludeMarks(&s, 0, delim,excludeDelim, NULL) == 0)
            {
                continue;
            }
        }
        /* case current char is the expected delimiter */
        if (*s == delim)
        {
            rm = 1;
        }
        
        /* case next char is not space */
        if (*(s + 1) != ' ')
        {
            if ((count > 0) && (rm || (*(s + 1) == delim) || (*(s + 1) == 0)))
            {
                s = (s + 1 - count);
                for(t = s; *t; t++)
                {
                    *t = *(t+count);
                }
            }
            else
            {
                s++;
            }
            count = 0;
            rm = 0;
        }
        else
        {
            s++;
        }

    }  
}

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
int32_t StrMpl_getListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des, char delim, uint32_t flags)
{
    uint32_t i = 0;
    uint8_t found = 0;
    char *last;
    int32_t status;

    /* check size was defined */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }
    status = StrMpl_prepare(src, delim, NULL, &last);
    
    if (status == 0)
    {
        StrMpl_lwr(*src); 
        /* search string in list */
        while ((!found) && (i < listSize))
        {
            if(!strcmp(list->str, *src))
            {
                /* case string was found */
                StrMpl_setBySize(&list->value,des,flags & 0x0F);
                found = 1;
            }
            else
            {
                list++;
                i++;
            }
        }
        if (!found)
        {
            status = STRMPL_ERROR_WRONG_PARAM;
        }
    }
    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }

    return status;
}

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
int32_t StrMpl_setListStr(StrMpl_List_t *list, uint32_t listSize, void *src, char **des, char delim, uint32_t flags)
{
    uint32_t i;
    
    for (i = 0; i < listSize ; list++,i++)
    {
        if (((flags & STRMPL_FLAG_PARAM_SIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_8) && (list->value == *(int8_t *)(src)))) ||
            ((flags & STRMPL_FLAG_PARAM_SIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_16) && (list->value == *(int16_t *)(src)))) ||
            ((flags & STRMPL_FLAG_PARAM_SIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_32) && (list->value == *(int32_t *)(src)))) ||
            ((flags & STRMPL_FLAG_PARAM_UNSIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_8) && (list->value == *(uint8_t *)(src)))) ||
            ((flags & STRMPL_FLAG_PARAM_UNSIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_16) && (list->value == *(uint16_t *)(src)))) ||
            ((flags & STRMPL_FLAG_PARAM_UNSIGNED) && ((flags & STRMPL_FLAG_PARAM_SIZE_32) && (list->value == *(uint32_t *)(src)))))
        {
            sprintf(*des,"%s%c",list->str,delim);
            *des += (strlen(*des));
            return 0;
        }
    }
    return STRMPL_ERROR_WRONG_PARAM;
}

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
int32_t StrMpl_getVal(char **src, void *des, char delim, uint32_t flags)
{
    char *last;
    int32_t status;
    int val;

    /* check size was defined */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }    
    status = StrMpl_prepare(src, delim, NULL, &last);

    if (status == 0)
    {
        /* convert to hexadecimal or decimal number  base 0 = auto detect */
        val = strtoul(*src, NULL, 0);
        StrMpl_setBySize(&val,des,flags & 0x0F);
    }

    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }
    return status;
}


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
int32_t StrMpl_setVal(void *src, char **des, char delim, uint32_t flags)
{
    /* case source is NULL - print only the delimiter */
    if (src == NULL)
    {
        sprintf(*des,"%c",delim);
    }
    else
    {
        /* case of decimal number */
        if (flags & STRMPL_FLAG_PARAM_DEC)
        {
            /* case of signed number */
            if (flags & STRMPL_FLAG_PARAM_SIGNED)
            {
                switch (flags & 0x0F)
                {
                    case STRMPL_FLAG_PARAM_SIZE_8:
                        sprintf(*des,"%d%c",*(int8_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_16:
                        sprintf(*des,"%d%c",*(int16_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_32:
                        sprintf(*des,"%d%c",*(int32_t *)(src),delim);
                        break;
                    default:
                        break;
                }
            }
            /* case of unsigned number */
            else if (flags & STRMPL_FLAG_PARAM_UNSIGNED)
            {
                switch (flags & 0x0F)
                {
                    case STRMPL_FLAG_PARAM_SIZE_8:
                        sprintf(*des,"%d%c",*(uint8_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_16:
                        sprintf(*des,"%d%c",*(uint16_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_32:
                        sprintf(*des,"%d%c",*(uint32_t *)(src),delim);
                        break;
                    default:
                        break;
                }
            }
            
        }
        /* case of hexadecimal number */
        else if (flags & STRMPL_FLAG_PARAM_HEX)
        {
            /* case of signed number */
            if (flags & STRMPL_FLAG_PARAM_SIGNED)
            {
                switch (flags & 0x0F)
                {
                    case STRMPL_FLAG_PARAM_SIZE_8:
                        sprintf(*des,"0x%x%c",*(int8_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_16:
                        sprintf(*des,"0x%x%c",*(int16_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_32:
                        sprintf(*des,"0x%x%c",*(int32_t *)(src),delim);
                        break;
                    default:
                        break;
                }
            }
            /* case of unsigned number */
            else if (flags & STRMPL_FLAG_PARAM_UNSIGNED)
            {
                switch (flags & 0x0F)
                {
                    case STRMPL_FLAG_PARAM_SIZE_8:
                        sprintf(*des,"0x%x%c",*(uint8_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_16:
                        sprintf(*des,"0x%x%c",*(uint16_t *)(src),delim);
                        break;
                    case STRMPL_FLAG_PARAM_SIZE_32:
                        sprintf(*des,"0x%x%c",*(uint32_t *)(src),delim);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    /* update destination pointer */
    *des += (strlen(*des));
    return 0;
}

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
int32_t StrMpl_getArrayVal(char **src, void *des,uint32_t arrayLen, char delim, char intraDelim, uint32_t flags, char *excludeDelim)
{
    char *last;
    int32_t status;
    uint8_t i;
    char delimiter = intraDelim;
    void *tmp;

    /* check size was defined */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }
    
    status = StrMpl_prepare(src, delim, excludeDelim, &last);

    if (status == 0)
    {
        for(i = 0; i < arrayLen ; i++)
        {
            if (i == (arrayLen - 1))
            {
                /* the delim was placed with 0 in function StrMpl_prepare */ 
                delimiter = 0;
            }

            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    tmp = (void *)(((int8_t *)des) + i);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    tmp = (void *)(((int16_t *)des) + i);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    tmp = (void *)(((int32_t *)des) + i);
                    break;
                default:
                    break;
                
            }
            /* get the numeric value */
            status = StrMpl_getVal(src ,tmp,delimiter,flags);
            if (status != 0)
            {
                break;
            }
        }
    }
    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }
    return status;
}


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
int32_t StrMpl_setArrayVal(void *src, char **des, uint32_t arrayLen, char delim, char intraDelim, uint32_t flags)
{
    uint8_t i;
    char delimiter = intraDelim;
    
    if (src != NULL)
    {
        for(i = 0; i < arrayLen ; i++)
        {
            /* last array member - print the terminate delimiter */
            if (i == (arrayLen - 1))
            {
                delimiter = delim;
            }
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    StrMpl_setVal(((int8_t *)(src))+ i,des,delimiter,flags);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    StrMpl_setVal(((int16_t *)(src))+ i,des,delimiter,flags);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    StrMpl_setVal(((int32_t *)(src))+ i,des,delimiter,flags);
                    break;
                default:
                    break;
            }           
        }
    }
    else
    {
        /* print only the delimiter */
        StrMpl_setStr(NULL, des, delim);
    }
    return 0;
}

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
int32_t StrMpl_getArrayListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des,uint32_t arrayLen, char delim, char intraDelim, char *excludeDelim, uint32_t flags)
{
    char *last;
    int32_t status;
    uint8_t i;
    char delimiter = intraDelim;
    void *tmp;
    
    /* check size was defined */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }
 
    status = StrMpl_prepare(src, delim, excludeDelim, &last);
    
    if (status == 0)
    {   
        for(i = 0; i < arrayLen ; i++)
        {
            if (i == (arrayLen - 1))
            {
                /* the delim was placed with 0 in function StrMpl_prepare */ 
                delimiter = 0;
            }

            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    tmp = (void *)(((int8_t *)des) + i);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    tmp = (void *)(((int16_t *)des) + i);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    tmp = (void *)(((int32_t *)des) + i);
                    break;
            }

            /* get the list value */
            status = StrMpl_getListVal(list,listSize,src ,tmp,delimiter,flags);
            if (status != 0)
            {
                break;
            }
        }
    }

    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }
    return status;
}


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
int32_t StrMpl_setArrayListStr(StrMpl_List_t *list, uint32_t listSize,void *src, char **des, uint32_t arrayLen, char delim, char intraDelim, uint32_t flags)
{
    uint8_t i;
    char delimiter = intraDelim;
    
    if (src != NULL)
    {
        for(i = 0; i < arrayLen ; i++)
        {
            /* last array member - print the terminate delimiter */
            if (i == (arrayLen - 1))
            {
                delimiter = delim;
            }
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    StrMpl_setListStr(list,listSize,((int8_t *)(src))+ i,des,delimiter,flags);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    StrMpl_setListStr(list,listSize,((int16_t *)(src))+ i,des,delimiter,flags);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    StrMpl_setListStr(list,listSize,((int32_t *)(src))+ i,des,delimiter,flags);
                    break;
                default:
                    break;
            }           
        }
    }
    else
    {
        /* print only the delimiter */
        StrMpl_setStr(NULL, des, delim);
    }
    return 0;
}

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
int32_t StrMpl_getArrayBitVal(char **src, void *des, char delim, char intraDelim, char *excludeDelim, uint32_t flags)
{
    int32_t status;
    uint32_t *val = NULL;  
    uint8_t i;
    uint8_t numParams = StrMpl_getNumVals(*src,intraDelim,delim,NULL);
    uint32_t missingParamVal = 0;
    
    /* check for parameter size */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }
    /* check for no parameters */
    if (numParams == 0)
    {
        *src = *src + 1;
        return STRMPL_ERROR_PARAM_MISSING;
    }

    /* allocate the middle buffer */
    val = calloc(numParams , sizeof(uint32_t));
    if (val == NULL)
    {
        return STRMPL_ERROR_MEM_ALLOCATION;      
    }

    /* get the parameters in to the middle buffer */
    status = StrMpl_getArrayVal(src, (void *)val, numParams, delim,intraDelim, STRMPL_FLAG_PARAM_SIZE_32, excludeDelim);

    /* copy middle buffer into the destination */
    if (status == 0)
    { 
        StrMpl_setBySize(&missingParamVal, des, flags & 0x0F);
        for (i = 0; i < numParams; i++)
        {
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    *(uint8_t *)des |= (1 << val[i]);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    *(uint16_t *)des |= (1 << val[i]);
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    *(uint32_t *)des |= (1 << val[i]);
                    break;
            }
        }
    }
    /* free the middle buffer */
    free(val);
    return status;
}


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
int32_t StrMpl_setArrayBitVal(void *src, char **des, char delim, char intraDelim, uint32_t flags)
{
    uint32_t i, bit;
    char delimiter = intraDelim;
    uint32_t val;
    uint8_t num1s = 0;

    if (src != NULL)
    {
        val = *(uint32_t *)src;

        /* find the number of 1's */
        for(i = 0; i < (flags & 0x0F) * 8 ; i++)
        {
            num1s += ((val>>i)&1);
        }
        val = 0;
        for(i = 0; i < (flags & 0x0F) * 8 ; i++)
        {
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    bit = ((*(uint8_t *)src) & (1 << i));
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    bit = ((*(uint16_t *)src) & (1 << i));
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    bit = ((*(uint32_t *)src) & (1 << i));
                    break;
                default:
                    break;
            }
            if (bit != 0)
            {
                /* count the values */
                val++;
                /* case of the last value - set close delimiter */
                if (val == num1s)
                {
                    delimiter = delim;
                }
                /* set the value */
                StrMpl_setVal(&i,des,delimiter,flags | STRMPL_FLAG_PARAM_UNSIGNED);
            }          
        }
    }
    else
    {
        /* print only the delimiter */
        StrMpl_setStr(NULL, des, delim);
    }
   
    return 0;
}

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
int32_t StrMpl_getBitmaskListVal(StrMpl_List_t *list, uint32_t listSize, char **src, void *des, char delim, char intraDelim, char *excludeDelim, uint32_t flags)
{
    int32_t status;
    uint32_t *val = NULL;  
    uint8_t i;
    uint8_t numParams = StrMpl_getNumVals(*src,intraDelim,delim,NULL);
    uint32_t missingParamVal = 0;
    
    /* check for parameter size */
    if ((flags & 0x0F) == 0)
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }
    /* check for no parameters */
    if (numParams == 0)
    {
        *src = *src + 1;
        return STRMPL_ERROR_PARAM_MISSING;
    }

    /* allocate the middle buffer */
    val = calloc(numParams , sizeof(uint32_t));
    if (val == NULL)
    {
        return STRMPL_ERROR_MEM_ALLOCATION;      
    }

    /* get the parameters in to the middle buffer */
    status = StrMpl_getArrayListVal(list, listSize, src, (void *)val, numParams, delim,intraDelim, excludeDelim, STRMPL_FLAG_PARAM_SIZE_32);

    /* copy middle buffer into the destination */
    if (status == 0)
    { 
        StrMpl_setBySize(&missingParamVal, des, flags & 0x0F);
        for (i = 0; i < numParams; i++)
        {
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    *(uint8_t *)des |= val[i];
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    *(uint16_t *)des |= val[i];
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    *(uint32_t *)des |= val[i];
                    break;
            }
        }
    }
    /* free the middle buffer */
    free(val);
    return status;
}


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
int32_t StrMpl_setBitmaskListStr(StrMpl_List_t *list, uint32_t listSize,void *src, char **des, char delim, char intraDelim, uint32_t flags)
{
    uint32_t i, bit;
    char delimiter = intraDelim;
    uint32_t val;
    uint8_t num1s = 0;

    if (src != NULL)
    {
        val = *(uint32_t *)src;

        /* find the number of 1's */
        for(i = 0; i < (flags & 0x0F) * 8 ; i++)
        {
            num1s += ((val>>i)&1);
        }
        val = 0;
        for(i = 0; i < (flags & 0x0F) * 8 ; i++)
        {
            switch (flags & 0x0F)
            {
                case STRMPL_FLAG_PARAM_SIZE_8:
                    bit = ((*(uint8_t *)src) & (1 << i));
                    break;
                case STRMPL_FLAG_PARAM_SIZE_16:
                    bit = ((*(uint16_t *)src) & (1 << i));
                    break;
                case STRMPL_FLAG_PARAM_SIZE_32:
                    bit = ((*(uint32_t *)src) & (1 << i));
                    break;
                default:
                    break;
            }
            if (bit != 0)
            {
                /* count the values */
                val++;
                /* case of the last value - set close delimiter */
                if (val == num1s)
                {
                    delimiter = delim;
                }
                /* set the value */
                StrMpl_setListStr(list,listSize,&bit,des,delimiter,flags | STRMPL_FLAG_PARAM_UNSIGNED);
            }          
        }
    }
    else
    {
        /* print only the delimiter */
        StrMpl_setStr(NULL, des, delim);
    }
   
    return 0;
}

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
int32_t StrMpl_getStr(char **src, char *des, char delim, uint32_t maxSize, char *excludeDelim)
{
    char *last;
    int32_t status;
    uint32_t length;

    status = StrMpl_prepare(src, delim, excludeDelim, &last);

    if (status == 0)
    {
        /* get current string length */
        length = strlen(*src);

        /* check current string longer then buffer size */  
        if (length > maxSize)
        {
            status = STRMPL_ERROR_WRONG_SIZE;
        }
        else
        {
            /* copy sub string to destibation buffer */
            strcpy(des,*src);
        }
    }

    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }

    return status;
}

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
int32_t StrMpl_getAllocStr(char **src, char **des, char delim, uint32_t maxSize, char *excludeDelim)
{
    char *last;
    int32_t status;
    uint32_t length;

    status = StrMpl_prepare(src, delim, excludeDelim, &last);

    if (status == 0)
    {
        /* get current string length + null terminated */
        length = strlen(*src) + 1;

        /* check current string length and max size + null terminated */  
        if (length > (maxSize + 1))
        {
            status = STRMPL_ERROR_WRONG_SIZE;
        }
        else
        {
            /* allocate the destination */
            *des = calloc(length , sizeof(uint8_t));
            if (*des == NULL)
            {
                return STRMPL_ERROR_MEM_ALLOCATION;
            }
                
            strcpy(*des,*src);
        }
    }

    /* update source string pointer */
    if (last != NULL)
    {
        *src = last;
    }

    return status;
}


/*!
    \brief          String manipulation - Set sub string into string buffer.
    
    This routine print a given sub string and a given delimiter in destination string 
    It also updates the destination string pointer to point the end of string. 

    \param          src             -   points to given sub string 
    \param          des             -   points to destination string 
    \param          delim           -   hold the expected delimiter after source sub string

    \return         function return 0
*/
int32_t StrMpl_setStr(char *src, char **des, char delim)
{
    if (src != NULL)
    {
        sprintf(*des,"%s%c",src,delim);
    }
    else
    {
        /* print only the delimiter */
        sprintf(*des,"%c",delim);
    }
    /* update destination pointer */
    *des += (strlen(*des));
    return 0;
}

/*!
    \brief          Get Base64 decoded buffer size
    
    This routine calculate the expected raw data buffer size 
    by given base64 buffer and buffer size    

    \param          inputData       -  source buffer which hold the base64 data 
    \param          inputLength     -  length of base64 data 

    \return         function shall return expected size.
*/
uint32_t StrMpl_getBase64DecBufSize(uint8_t *inputData, uint32_t inputLength)
{
    uint32_t outputLength = inputLength / 4 * 3;

    if (outputLength == 0)
    {
        return 0;
    }
    if (inputData[inputLength - 1] == '=') outputLength--;
    if (inputData[inputLength - 2] == '=') outputLength--;

    return outputLength;
}

/*!
    \brief          Get Base64 encoded buffer size
    
    This routine calculate the expected Base64 buffer size 
    by given raw data size    

    \param          inputLength     -  raw buffer size

    \return         function shall return expected size.
*/
uint32_t StrMpl_getBase64EncBufSize(uint32_t inputLength)
{
    return (4 * ((inputLength + 2) / 3));
}

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
int32_t StrMpl_decodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength)
{
    uint32_t decode_value;
    uint32_t nibble6_1, nibble6_2, nibble6_3, nibble6_4;
    uint32_t i, j;

    if (inputLength % 4 != 0) 
    {
        return STRMPL_ERROR_WRONG_SIZE;
    }

    if (outputData == NULL) 
    {
        return STRMPL_ERROR_MEM_ALLOCATION;
    }
    
    *outputLength = StrMpl_getBase64DecBufSize(inputData, inputLength);

    for (i = 0, j = 0; i < inputLength;) 
    {
        nibble6_1 = inputData[i] == '=' ? 0 & i++ : StrMpl_base64DecTable[inputData[i++]];
        nibble6_2 = inputData[i] == '=' ? 0 & i++ : StrMpl_base64DecTable[inputData[i++]];
        nibble6_3 = inputData[i] == '=' ? 0 & i++ : StrMpl_base64DecTable[inputData[i++]];
        nibble6_4 = inputData[i] == '=' ? 0 & i++ : StrMpl_base64DecTable[inputData[i++]];

        decode_value = (nibble6_1 << 3 * 6) + (nibble6_2 << 2 * 6)  + (nibble6_3 << 1 * 6) + (nibble6_4 << 0 * 6);

        if (j < *outputLength) outputData[j++] = (decode_value >> 2 * 8) & 0xFF;
        if (j < *outputLength) outputData[j++] = (decode_value >> 1 * 8) & 0xFF;
        if (j < *outputLength) outputData[j++] = (decode_value >> 0 * 8) & 0xFF;
    }
    outputData[j] = 0;

    return 0;
}


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
int32_t StrMpl_encodeBase64(uint8_t *inputData, uint32_t inputLength, uint8_t *outputData, uint32_t *outputLength)
{
    uint32_t encodeValue;
    uint32_t nibble6_1, nibble6_2, nibble6_3;
    uint32_t i, j;
 
    *outputLength = StrMpl_getBase64EncBufSize(inputLength);
 
    if (outputData == NULL)
    {
        return STRMPL_ERROR_MEM_ALLOCATION;
    }
 
    for (i = 0, j = 0; i < inputLength;)
    {
 
        nibble6_1 = i < inputLength ? inputData[i++] : 0;
        nibble6_2 = i < inputLength ? inputData[i++] : 0;
        nibble6_3 = i < inputLength ? inputData[i++] : 0;
 
        encodeValue = (nibble6_1 << 0x10) + (nibble6_2 << 0x08) + nibble6_3;
 
        outputData[j++] = StrMpl_base64EncTable[(encodeValue >> 3 * 6) & 0x3F];
        outputData[j++] = StrMpl_base64EncTable[(encodeValue >> 2 * 6) & 0x3F];
        outputData[j++] = StrMpl_base64EncTable[(encodeValue >> 1 * 6) & 0x3F];
        outputData[j++] = StrMpl_base64EncTable[(encodeValue >> 0 * 6) & 0x3F];
    }

    if (inputLength % 3 >= 1) outputData[*outputLength - 1] = '=';
    if (inputLength % 3 == 1) outputData[*outputLength - 2] = '=';

    outputData[*outputLength] = 0;

    return 0;
}

