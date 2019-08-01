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

/**********************************README***************************************
 * 1, we will load the command table in shell_cmd_init
 * 2, uptil now, we support the keil,gcc,you could add more compiler here
 *    make sure you get the method to define a section and know its address and size
*******************************************************************************/


/**************************************FILE INCLIUDES**************************/

#if CONFIG_SHELL_ENABLE

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include  <link_misc.h>


typedef int (*fn_shell_cmdentry)(int argc, const char *argv[]);


/**************************************FILE DEFINES****************************/
#define cn_shell_args       16     //at most could support 16 parameters for command
#define cn_shell_ver_major  1
#define cn_shell_ver_minor  0

/**************************************FILE DATA STRUCTURE*********************/
struct shell_cb_t
{
    struct shell_item_t *s;      //static tabs
    int                  s_num;  //static number
    struct shell_item_t *d;      //dynamic list
    int                  d_num;  //dynamic number;
};


/**************************************FILE VARS*******************************/
static struct shell_cb_t gs_shell_cb; //shell controller here

static const char *gs_os_shell_type[EN_OSSHELL_LAST]={
    "CMD",\
    "VAR",
};//this will be used in show the shell type
static struct shell_tab_matches tab_matches;
/**************************************FILE FUNCTIONS**************************/
//export functions
int   shell_cmd_execute(char *param);            //execute the command
int   shell_cmd_init(void);                      //do the command table load
const struct shell_tab_matches *shell_cmd_index(const char *index);  //find the most like command


/**************************************FILE FUNCIMPLEMENT**********************/

/*******************************************************************************
function     :system help
parameters   :
instruction  :take care of help specified command
*******************************************************************************/
static int   shell_cmd_help(int argc, const char *argv[]){
    int   i ;
    struct shell_item_t *item;

    printf("%-16s%-5s%-4s%-10s%-20s\n\r",\
            "Name","Type","Len","RunAddr","Description");
    for(i = 0;i <gs_shell_cb.s_num;i++){
        item = &(gs_shell_cb.s[i]);
        printf("%-16s%-5s%-4x%08x  %-30s\n\r",\
                item->name,gs_os_shell_type[item->type%EN_OSSHELL_LAST],\
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
    struct shell_item_t *item = NULL;
    struct shell_item_t  *ret = NULL;
    int   i;
    //search the static tab first,if not found then find the dynamic tab
    ret = NULL;
    for(i = 0;i <gs_shell_cb.s_num;i++){
        item = &(gs_shell_cb.s[i]);
        if(0 == strcmp(name,item->name)){
            ret = item;
            break;
        }
    }
    if(NULL == item){
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
const struct shell_tab_matches *shell_cmd_index(const char *index){
    struct shell_item_t *item;
    int   i;
    //search the static tab first,if not found then find the dynamic tab
    tab_matches.len = 0;
    for(i = 0;i <gs_shell_cb.s_num && tab_matches.len < MAX_TAB_MATCHES; i++){
        item = &(gs_shell_cb.s[i]);
        if (strncmp(item->name, index, strlen(index)) == 0) {
            tab_matches.matches[tab_matches.len++] = item->name;
        }
    }
    
    for(i = 0;i <gs_shell_cb.d_num && tab_matches.len < MAX_TAB_MATCHES; i++){
        item = &(gs_shell_cb.d[i]);
        if(strncmp(item->name, index, strlen(index)) == 0){
            tab_matches.matches[tab_matches.len++] = item->name;
        }
    }
    
    return &tab_matches;
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
int   shell_cmd_execute(char *param){
    int   ret= -1;
    int    argc = cn_shell_args;           //split the params to argc and argv format
    unsigned int     value;
    unsigned char   *bytes;
    int     i;
    const  char *argv[cn_shell_args];
    struct shell_item_t *item;            //match the command item
    fn_shell_cmdentry shell_cmd_entry;    //command function entry
    string_to_arg(&argc,argv,param);      //format the parameters
    if(argc == 0){
        shell_cmd_help(0,NULL);               //if no args we show system help
        ret = 0;
    }
    else{
        item =shell_cmd_match(argv[0]);//find the item
        if(NULL == item){
            printf("SHELL COMMAND NOT FIND:%s\n\r",argv[0]);
            ret = -1;
        }
        else{
            if(item->type == EN_OSSHELL_CMD){
                shell_cmd_entry = (fn_shell_cmdentry)item->addr;
                shell_cmd_entry(argc,argv);
            }
            else{
                if((argc == 3)&&(0 == strcmp(argv[1],"set"))){
                    //deal it simple and easy here
                    value = strtol(argv[2],NULL,0);
                    memcpy(item->addr,&value,item->len);
                }
                else{
                    bytes = item->addr;
                    printf("(HEX):ADDR:0X%08X:",(unsigned int)bytes);
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
extern unsigned int oshell$$Base;
extern unsigned int oshell$$Limit;
//#pragma section("oshell", read) 
#elif defined(__GNUC__)
    extern unsigned int __oshell_start;
    extern unsigned int __oshell_end;
#else
    #error("unknown compiler here");
#endif        


int   shell_cmd_init(void){
    void  *cmd_start = NULL;
    unsigned int len = 0;
#if defined (__CC_ARM)    //you could add other compiler like this
    len = (unsigned int)&oshell$$Limit-(unsigned int)&oshell$$Base;
    cmd_start = &oshell$$Base;
#elif defined(__GNUC__)
    cmd_start = &__oshell_start;
    len = (unsigned int )&__oshell_end - (unsigned int)&__oshell_start;
#else 
    #error("unknown compiler here");
#endif    
    if(len > 0){
        len = len/sizeof(struct shell_item_t);
        gs_shell_cb.s = (struct shell_item_t *)cmd_start;
        gs_shell_cb.s_num = len;
    }
    return 0;
}

static int   os_shell_version(int argc, char *argv){
    printf("os_shell_version:%d.%d\n\r",cn_shell_ver_major,cn_shell_ver_minor);
    return 0;
};
//add the shell cmd
OSSHELL_EXPORT_CMD(os_shell_version,"shellversion","shellversion");
OSSHELL_EXPORT_CMD(shell_cmd_help,"help","help");

#endif



