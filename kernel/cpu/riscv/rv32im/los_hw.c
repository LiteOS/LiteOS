/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
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
#include "los_task.ph"
#include "los_hw.h"
#include "los_sys.ph"
#include "los_priqueue.ph"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*****************************************************************************
 Function    : osSchedule
 Description : task scheduling
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
VOID osSchedule(VOID)
{
    osTaskSchedule();
}

/*****************************************************************************
 Function    : LOS_Schedule
 Description : Function to determine whether task scheduling is required
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
VOID LOS_Schedule(VOID)
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();

    /* Find the highest task */
    g_stLosTask.pstNewTask = LOS_DL_LIST_ENTRY(LOS_PriqueueTop(), LOS_TASK_CB, stPendList);

    /* In case that running is not highest then reschedule */
    if (g_stLosTask.pstRunTask != g_stLosTask.pstNewTask)
    {
        if ((!g_usLosTaskLock))
        {
            (VOID)LOS_IntRestore(uvIntSave);

            osTaskSchedule();

            return;
        }
    }

    (VOID)LOS_IntRestore(uvIntSave);
}

/*****************************************************************************
 Function    : osTaskExit
 Description : Task exit function
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR VOID osTaskExit(VOID)
{
    osDisableIRQ();
    while(1);
}

/*****************************************************************************
 Function    : osTskStackInit
 Description : Task stack initialization function
 Input       : uwTaskID     --- TaskID
               uwStackSize  --- Total size of the stack
               pTopStack    --- Top of task's stack
 Output      : None
 Return      : Context pointer
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT VOID *osTskStackInit(UINT32 uwTaskID, UINT32 uwStackSize, VOID *pTopStack)
{
    UINT32 uwIdx;
    TSK_CONTEXT_S  *pstContext;

    /*initialize the task stack, write magic num to stack top*/
    for (uwIdx = 1; uwIdx < (uwStackSize/sizeof(UINT32)); uwIdx++)
    {
        *((UINT32 *)pTopStack + uwIdx) = OS_TASK_STACK_INIT;
    }
    *((UINT32 *)(pTopStack)) = OS_TASK_MAGIC_WORD;

    pstContext    = (TSK_CONTEXT_S *)(((UINT32)pTopStack + uwStackSize) - sizeof(TSK_CONTEXT_S));

    pstContext->ra = (UINT32)osTaskExit;
    pstContext->sp = 0x02020202L;
    pstContext->gp = 0x03030303L;
    pstContext->tp = 0x04040404L;
    pstContext->t0 = 0505050505;
    pstContext->t1 = 0x06060606L;
    pstContext->t2 = 0x07070707L;
    pstContext->s0 = 0x08080808L;
    pstContext->s1 = 0x09090909L;
    pstContext->a0 = uwTaskID;		//a0 first argument
    pstContext->a1 = 0x11111111L;
    pstContext->a2 = 0x12121212L;
    pstContext->a3 = 0x13131313L;
    pstContext->a4 = 0x14141414L;
    pstContext->a5 = 0x15151515L;
    pstContext->a6 = 0x16161616L;
    pstContext->a7 = 0x17171717L;
    pstContext->s2 = 0x18181818L;
    pstContext->s3 = 0x19191919L;
    pstContext->s4 = 0x20202020L;
    pstContext->s5 = 0x21212121L;
    pstContext->s6 = 0x22222222L;
    pstContext->s7 = 0x23232323L;
    pstContext->s8 = 0x24242424L;
    pstContext->s9 = 0x25252525L;
    pstContext->s10 = 0x26262626L;
    pstContext->s11 = 0x27272727L;
    pstContext->t3 = 0x28282828L;
    pstContext->t4 = 0x29292929L;
    pstContext->t5 = 0x30303030L;
    pstContext->t6 = 0x31313131L;
    pstContext->mepc =(UINT32)osTaskEntry;
    return (VOID *)pstContext;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


