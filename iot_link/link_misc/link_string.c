/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-04-29 09:50  zhangqianfu  The first version  
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <osal.h>
#include <link_misc.h>

//this function is used to format the char string to the argc mode
//this function will changed the original string, used it carefully
//return how many arguments has been
int string_to_arg(int *argc, const char *argv[],char *string)
{
    int argvlen = 0;
    int paramnum = 0;
    char *tmp = NULL;
    char bak;
    int len;

    argvlen = *argc;
    *argc = paramnum;
    if(NULL == string)
    {
        return paramnum;
    }

    //use the '\0' to replace the ' '
    len = strlen(string);
    tmp = string;
    while(tmp < (string + len))
    {
        if(*tmp == ' ')
        {
            *tmp = '\0';
        }
        tmp++;
    }
    bak = '\0';
    tmp = string;
    while(tmp < (string + len))
    {
        if((*tmp != '\0')&&(bak =='\0'))
        {
            if(paramnum < argvlen)
            {
                argv[paramnum] = tmp;
                paramnum++;
            }
        }
        bak = *tmp;
        tmp++;
    }
    *argc = paramnum;

    return paramnum;
}


char *osal_strdup(const char *ch)
{
    char *copy;
    size_t length;

    if(NULL == ch)
        return NULL;

    length = strlen(ch);
    copy = (char *)osal_malloc(length + 1);
    if(NULL == copy)
        return NULL;
    strncpy(copy, ch, length);
    copy[length] = '\0';

    return copy;
}
