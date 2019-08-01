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
 *  2019-05-14 17:22  zhangqianfu  The first version
 *
 */

#ifndef __OSSHELL_H
#define __OSSHELL_H

#if CONFIG_SHELL_ENABLE

#include <stdint.h>
#include <stddef.h>
//this is a shell module designed for the os
//this is a shell type,maybe a command or the data variables
enum en_os_shell_type
{
	EN_OSSHELL_CMD = 0,
	EN_OSSHELL_VAR,       //up till now, we only support 4 bytes
	EN_OSSHELL_LAST,
};

#define BUILD_VAR_NAME(A,B)         A##B
#define  MAX_TAB_MATCHES			16
struct shell_tab_matches
{
	const char *matches[MAX_TAB_MATCHES];
	unsigned short len;
};

//this is the shell function module.the register function must have the same type
//uptils now, we don't care the return value
struct shell_item_t
{
	const char     *name;   //point to the shell name string
	const char     *help;   //point to the shell description string
	void           *addr;   //point to the shell function or the shell data
	unsigned short  type;   //used to  point the shell type:command or a data
	unsigned short  len;    //used to  point the shell command or data length
};
//this define will create  a shell command with the specified cmdname
#define OSSHELL_EXPORT_CMD(cmdentry,cmdname,cmdhelp)      \
    static const struct shell_item_t BUILD_VAR_NAME(__oshell_,cmdentry) __attribute__((used,section("oshell")))= \
    {                           \
        .name=cmdname,    \
        .help=cmdhelp,    \
        .addr=(void *)&cmdentry,              \
		.type=EN_OSSHELL_CMD,           \
		.len = sizeof(void *),             \
    }
//this define will create  a create a shell data with the specified name
#define OSSHELL_EXPORT_VAR(var,varname,varhelp)      \
    static const struct shell_item_t BUILD_VAR_NAME(__oshell_,var) __attribute__((used,section("oshell")))= \
    {                           \
        .name=varname,    \
        .help=varhelp,    \
        .addr=(void *)&var,              \
		.type=EN_OSSHELL_VAR,                   \
		.len =sizeof(var),               \
    }
    
void shell_init(void);
#else

#define OSSHELL_EXPORT_CMD(cmdentry,cmdname,cmdhelp)
#define OSSHELL_EXPORT_VAR(var,varname,varhelp)
#define shell_init()

#endif   //end for the shell_config


#endif /* __OSSHELL_H */
