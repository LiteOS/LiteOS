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

#ifndef __OSSHELL_H
#define __OSSHELL_H

#include <target_config.h>


/*************************************************************************************************************/
/*------------------------------------------ 邪恶的分割线 ---------------------------------------------------*/
/*************************************************************************************************************/
#if CN_OS_SHELL			//在target_config.h中被定义为1
#include "osport.h"

//this is a shell module designed for the os
//这是一个为操作系统设计的shell模块

//this is a shell type,maybe a command or the data variables
//这里是shell类型，包括命令，数字变量，Last，
enum en_os_shell_type{
	EN_OSSHELL_CMD = 0,
	EN_OSSHELL_VAR,       //up till now, we only support 4 bytes（对于变量类型，截止目前，我们只支持4字节）
	EN_OSSHELL_LAST,
};

//"##" 是将两个参数连为一个整体。__oshell_##函数名
#define BUILD_VAR_NAME(A,B)    A##B

//this is the shell function module.the register function must have the same type
//这里是shell功能模块。寄存器功能必须有同样的类型。

//uptils now, we don't care the return value
//截至目前，我们不关心返回值
typedef int (*fn_shell_cmdentry)(int argc, const char *argv[]); 

//shell的cmd命令表中成员的数据结构
struct shell_item_t{
	const char     *name;   //point to the shell name string
	const char     *help;   //point to the shell description string
	void           *addr;   //point to the shell function or the shell data
	u16_t           type;   //used to  point the shell type:command or a data
	u16_t           len;    //used to  point the shell command or data length
};


//this define will create  a shell command with the specified cmdname
//这里的宏定义将会创建（带有具体名字的）cmd命令
#define OSSHELL_EXPORT_CMD(cmdentry,cmdname,cmdhelp)      \
    static const struct shell_item_t BUILD_VAR_NAME(__oshell_,cmdentry) __attribute__((used,section("oshell")))= \
    {                           \
        .name=cmdname,    		\
        .help=cmdhelp,    		\
        .addr=(void *)&cmdentry,\
		.type=EN_OSSHELL_CMD,   \
		.len = sizeof(void *),  \
    }
	
//this define will create  a create a shell data with the specified name
//这里的宏定义将会创建（带有具体名字的）shell数据
#define OSSHELL_EXPORT_VAR(var,varname,varhelp)      \
    static const struct shell_item_t BUILD_VAR_NAME(__oshell_,var) __attribute__((used,section("oshell")))= \
    {                           \
        .name=varname,    		\
        .help=varhelp,    		\
        .addr=(void *)&var,     \
		.type=EN_OSSHELL_VAR,   \
		.len =sizeof(var),      \
    }
    
//shell模块的安装，初始化
void shell_install(void);

/*************************************************************************************************************/
/*------------------------------------------ 邪恶的分割线 ---------------------------------------------------*/
/*************************************************************************************************************/
#else			//在target_config.h中被定义为1,所以不会编译下面的内容

	//定义出命令
	#define OSSHELL_EXPORT_CMD(cmdname,cmdentry,cmdhelp)
	//定义出变量
	#define OSSHELL_EXPORT_VAR(varname,var,varhelp)
	#define shell_install()

/*************************************************************************************************************/
/*------------------------------------------ 邪恶的分割线 ---------------------------------------------------*/
/*************************************************************************************************************/
#endif   		//end for the shell_config


#endif /* __OSSHELL_H */
