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
#if CONFIG_SHELL_ENABLE

#include <shell.h>
#include <los_memory.h>
#include <osal.h>
/*
//here we export to shell command,you could add your own shell like this
//test fucntion:print the params passed in
static int shell_test_func(int argc, const char *argv[]){
    int i = 0;
    printf("argc:%d \n\r",argc);
    for(i=0;i<argc;i++){
        printf("args:%d %s\n\r",i,argv[i]);
    }
    return 0;
}
OSSHELL_EXPORT_CMD(shell_test_func,"shell_test_func","shell test func"); //export a functuon demo

static u32_t gs_shell_test_var =123;
OSSHELL_EXPORT_VAR(gs_shell_test_var,"shell_test_var","shell test var");  //export a var demo here
*/
//show all the task status here
extern UINT32 osGetAllTskInfo(VOID);
static int shell_taskinfo(int argc,const char *argv[]){
    osGetAllTskInfo();
    return 0;
}
OSSHELL_EXPORT_CMD(shell_taskinfo,"taskinfo","taskinfo");

//show the memstatus here
static int shell_heapinfo(int argc,const char *argv[])
{
#if (LOSCFG_HEAP_IMPROVED == YES)
    LOS_MemInfo(NULL, 1);
#else
    LOS_MemPoolList();
#endif
    return 0;
}
OSSHELL_EXPORT_CMD(shell_heapinfo,"heapinfo","heapinfo");


static int shell_reboot(int argc,const char *argv[])
{
    osal_reboot();

    printf("reboot failed\n\r");

    return -1;
}

OSSHELL_EXPORT_CMD(shell_reboot,"reboot","reboot");


#endif

