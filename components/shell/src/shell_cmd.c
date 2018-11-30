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

/**********************************README***************************************
 * 1, we will load the command table in shell_cmd_init
 * 2, uptil now, we support the keil,gcc,you could add more compiler here
 *    make sure you get the method to define a section and know its address and size
*******************************************************************************/


/**************************************FILE INCLIUDES**************************/
#include "target_config.h"
#if CN_OS_SHELL

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
extern int string2arg(int *argc, const char *argv[],char *string);
/**************************************FILE DEFINES****************************/
#define CN_OSSHELL_ARGS    16     //at most could support 16 parameters for command


/**************************************FILE DATA STRUCTURE*********************/
struct shell_cb_t{
	struct shell_item_t *s;      //static tabs
	s32_t                s_num;  //static number
	struct shell_item_t *d;      //dynamic list
	s32_t                d_num;  //dynamic number;
};


/**************************************FILE VARS*******************************/
static struct shell_cb_t gs_shell_cb; //shell controller here

static const char *gs_os_shell_type[EN_OSSHELL_LAST]={
	"CMD",\
	"VAR",
};//this will be used in show the shell type

/**************************************FILE FUNCTIONS**************************/
//export functions
s32_t shell_cmd_execute(char *param);            //execute the command
s32_t shell_cmd_init(void);                      //do the command table load
const char *shell_cmd_index(const char *index);  //find the most like command


/**************************************FILE FUNCIMPLEMENT**********************/

/*******************************************************************************
function     :system help
parameters   :
instruction  :take care of help specified command
*******************************************************************************/
static s32_t shell_cmd_help(int argc, const char *argv[]){
	s32_t i ;
	struct shell_item_t *item;

	printf("%-4s%-16s%-5s%-4s%-10s%-20s\n\r",\
			"No","Name","Type","Len","RunAddr","Description");
	for(i = 0;i <gs_shell_cb.s_num;i++){
		item = &(gs_shell_cb.s[i]);
		printf("%-2x  %-16s%-5s%-4x%08x  %-30s\n\r",\
				i,item->name,gs_os_shell_type[item->type%EN_OSSHELL_LAST],\
				item->len,(unsigned int)item->addr,item->help);
	}
	for(i = 0;i <gs_shell_cb.d_num;i++){
		item = &(gs_shell_cb.d[i]);
		printf("%-2x  %-16s%-5s%-4x%08x  %-30s\n\r",\
				i,item->name,gs_os_shell_type[item->type%EN_OSSHELL_LAST],\
				item->len,(unsigned int)item->addr,item->help);
	}
	return 0;
}

/*******************************************************************************
function     :used to find the command
parameters   :
instruction  :
*******************************************************************************/
static struct shell_item_t *shell_cmd_match(const char *name)
{
	struct shell_item_t *item,*ret;
	s32_t i;
	//search the static tab first,if not found then find the dynamic tab
	//首先搜索静态选项卡，如果没有找到，则查找动态选项卡
	ret = NULL;
	item=NULL;//（新增）消除警告：warning: 'item' may be used uninitialized in this function 
	
	//static
	for(i = 0;i <gs_shell_cb.s_num;i++){
		item = &(gs_shell_cb.s[i]);
		if(0 == strcmp(name,item->name)){
			ret = item;
			break;
		}
	}
	//dynamic
	if(NULL == item)
	{
		for(i = 0;i <gs_shell_cb.d_num;i++){
			item = &(gs_shell_cb.d[i]);
			if(0 == strcmp(name,item->name)){
				ret = item;
				break;
			}
		}
	}
	return ret;
}

/*******************************************************************************
function     :used to find the most like command
parameters   :
instruction  :
*******************************************************************************/
const char *shell_cmd_index(const char *index){
    struct shell_item_t *item;
    const char *ret = NULL;
	item=NULL;//消除警告：warning: 'item' may be used uninitialized in this function [-Wmaybe-uninitialized]
	s32_t i;
	//search the static tab first,if not found then find the dynamic tab
	for(i = 0;i <gs_shell_cb.s_num;i++){
		item = &(gs_shell_cb.s[i]);
		if(NULL != strstr(item->name,index)){
			ret = item->name;
			break;
		}
	}
	if(NULL == item){
		for(i = 0;i <gs_shell_cb.d_num;i++){
			item = &(gs_shell_cb.d[i]);
			if(NULL!= strstr(item->name,index)){
				ret = item->name;
				break;
			}
		}
	}
    return ret;
}

/*******************************************************************************
function     :used to execute the command specified by the string
parameters   :
instruction  :we will split the string with the blank space, and the first one
             will be used as the shell name to search the shell.If a shell cmd
             found then will execute the command,and then all the args will be
             passed to the function;if a data shell found, then will only do the
             "get" or set "action";attention that the first arg is the shell name
             itself!
*******************************************************************************/
s32_t shell_cmd_execute(char *param)
{
	s32_t ret= -1;
	s32_t  argc = CN_OSSHELL_ARGS;           //split the params to argc and argv format
	u32_t  value;
	u8_t   *bytes;
	s32_t   i;
	const  char *argv[CN_OSSHELL_ARGS];
	struct shell_item_t *item;          //match the command item
	fn_shell_cmdentry shell_cmd_entry;  //command function entry //函数指针，指向入口函数
	
	//分割命令行字符数组，变成argc和argv
	string2arg(&argc,argv,param);       //format the parameters
	
	
	if(argc == 0)						//如果没有输入字符
	{
		shell_cmd_help(0,NULL);         //if no args we show system help
		ret = 0;
	}
	else								//有输入字符
	{
		item =shell_cmd_match(argv[0]);//find the item（进行字符匹配）
		
		if(NULL == item)				//如果没有匹配到
		{
			printf("SHELL COMMAND NOT FIND:%s\n\r",argv[0]);
			ret = -1;
		}
		else							//匹配到了shell字符
		{
			if(item->type == EN_OSSHELL_CMD)	//shell字符为CMD类型，命令
			{
				shell_cmd_entry = (fn_shell_cmdentry)item->addr;//函数入口地址
				shell_cmd_entry(argc,argv);						//传入参数
			}
			else								//shell字符为其他类型
			{
				if((argc == 3)&&(0 == strcmp(argv[1],"set")))
				{
					//deal it simple and easy here
					//这里简化处理
					value = strtol(argv[2],NULL,0);
					memcpy(item->addr,&value,item->len);
				}
				else
				{
					bytes = item->addr;
                    printf("variable:   ADDR:0X%08X   VALUE(HEX):",(u32_t)bytes);
					for(i = 0;i<item->len;i++){
						printf("%02x ",*bytes++);
					}
				}
			}
		}
	}
	return ret;
}

/*******************************************************************************
function     :used to load the command table
parameters   :
instruction  :different compiler has different method to ge the section("oshell")
              so take care of your compiler
*******************************************************************************/
#ifdef __CC_ARM /* ARM C Compiler ,like keil,options for linker:--keep *.o(oshell)*/
extern u32_t oshell$$Base;
extern u32_t oshell$$Limit;         
#pragma section("oshell", read)

#eldef 



#endif        


s32_t shell_cmd_init(void)
{
    void  *cmd_start = NULL;
	u32_t len = 0;
#if defined (__CC_ARM)    //you could add other compiler like this
	len = (u32_t)&oshell$$Limit-(u32_t)&oshell$$Base;
    cmd_start = &oshell$$Base;
#elif defined(__GNUC_LIKE)
	extern unsigned int __oshell_start;
	extern unsigned int __oshell_end;
	cmd_start = &__oshell_start;
	len = (unsigned int )&__oshell_end - (unsigned int)&__oshell_start;
#else 
	#error("unknown compiler here");
#endif    
	if(len > 0)
	{
		len = len/sizeof(struct shell_item_t);
		gs_shell_cb.s = (struct shell_item_t *)cmd_start;
		gs_shell_cb.s_num = len;
	}
	return 0;
}

//版本号：os_shell_version:1.0
#define CN_OSSHELL_MAJOR  1
#define CN_OSSHELL_MINOR  1	//20181130做出修改
static s32_t os_shell_version(int argc, char *argv)
{
	printf("os_shell_version:%d.%d\n\r",CN_OSSHELL_MAJOR,CN_OSSHELL_MINOR);
	return 0;
};

//add the shell cmd
OSSHELL_EXPORT_CMD(os_shell_version,"ver","show the shell version");
OSSHELL_EXPORT_CMD(shell_cmd_help,"help","show all the shell item");

#endif



