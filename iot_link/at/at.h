/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
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

#ifndef __AT_H
#define __AT_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <osal.h>


typedef int (*fn_at_oob)(void *args,void *data,size_t datalen);

#if  CONFIG_AT_ENABLE

/**
 * @brief: use this function to do the at client framwork initialized
 *
 * @return:0 success while -1 failed
 * */
int at_init(const char *devname);

/**
 * @brief:use this function to register a function that monitor the URC message
 * @param[in]:name, which used for the at framework debug
 * @param[in]:inxdex, used for match the out of band data
 * @param[in]:length, index length, this is match length
 * @param[in]:func, supply the function that will execute when the index is matched
 * @paarm[in]:args, supply for the registered function
 *
 * @return:0 success while -1 failed
 * */
int at_oobregister(const char *name,const void *index,size_t len,fn_at_oob func,void *args);

/**
 * @brief:use this function to register a function that monitor the URC message
 * @param[in]:cmd, the command to send
 * @param[in]:cmdlen, the command length
 * @param[in]:index, the command index, if you don't need the response, set it to NULL; this must be a string
 * @param[in]:respbuf, if you need the response, you should supply the buffer
 * @param[in]:respbuflen,the respbuf length
 * @param[in]:timeout, the time you may wait for the response;and the unit is ms
 *
 * @return:0 success while -1 failed
 * */

int at_command(const void *cmd, size_t cmdlen,const char *index,\
                void *respbuf,size_t respbuflen,uint32_t timeout);

#else

#define at_init(devname)                                               -1
#define at_oobregister(name,index,lenc,func,args)                      -1
#define at_command(cmd,cmdlen,index,respbuf,respbuflen,timeout)        -1

#endif


#endif
