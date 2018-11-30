/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
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

#include "los_base.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_memory.h"
#include "los_membox.h"
#include "los_priqueue.ph"
#include "csi_config.h"
void LOS_Schedule (void)
{
    uintptr_t flags;
    BOOL      need_schedule = FALSE;

    flags = LOS_IntLock ();

    g_stLosTask.pstNewTask = LOS_DL_LIST_ENTRY (osPriqueueTop (),
                                                LOS_TASK_CB, stPendList);/*lint !e413*/

    need_schedule = (g_stLosTask.pstRunTask != g_stLosTask.pstNewTask) &&
                    (!g_usLosTaskLock) && (!OS_INT_ACTIVE);

    (void) LOS_IntRestore (flags);

    if (need_schedule)
    {
        osSchedule ();
    }
}

static void osTaskExit (void)
{
    LOS_IntUnLock();

    while (1)
    {
        LOS_TaskDelay (0xffffffff);
    };
}

/*****************************************************************************
 Function    : osTskStackInit
 Description : task stack initialization
 Input       : tid        -- task id
               stack_size -- task stack size
               stack      -- task stack
 Output      : None
 Return      : OS_SUCCESS on success or error code on failure
 *****************************************************************************/
void * osTskStackInit (UINT32 tid, UINT32 stack_size, VOID * stack)
{
    TSK_CONTEXT_S * context;

#if (LOSCFG_STATIC_TASK == NO)

    UINT32 i;

    /*initialize the task stack, write magic num to stack top*/

    for (i = 0; i < (stack_size / sizeof (UINT32)); i++)
    {
        ((UINT32 *) stack) [i] = OS_TASK_STACK_INIT;
    }

    *((UINT32 *) stack) = OS_TASK_MAGIC_WORD;

#endif

    stack = (void *) ((char *) stack + stack_size);

    /* make extra 4 bytes after the context to fake return address in stack */

    context = (TSK_CONTEXT_S *) (((char *) stack) - sizeof (TSK_CONTEXT_S));

    /*initialize the task context*/

    context->pc  = (unsigned int) &osTaskEntry;
#ifndef CONFIG_SYSTEM_SECURE
	context->sr  = 0x80000140;
#else
	context->sr  = 0xE0000140;
#endif
    context->r0  = tid;
    context->r15 = (unsigned int)osTaskExit;//warning: assignment makes integer from pointer without a cast
#if 1
    context->r1   = 1;
    context->r2   = 2;
    context->r3   = 3;
    context->r4   = 4;
    context->r5   = 5;
    context->r6   = 6;
    context->r7   = 7;
    context->r8   = 8;
    context->r9   = 9;
    context->r10  = 10;
    context->r11  = 11;
    context->r12  = 12;
    context->r13  = 13;
#endif

    return (void *) context;
}

#if (LOSCFG_KERNEL_RUNSTOP == YES)
void osEnterSleep (void)
{
    //_BIS_SR (LPM0_bits + GIE);  /* Enter LPM0 w/ interrupt */
}
#endif

