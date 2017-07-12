/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * 
 * Copyright (c) 2017, Synopsys, Inc.
 * Modified for port to ARC processor by Wayne Ren wei.ren@synopsys.com 
 *
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
#include "inc/arc/arc_exception.h"
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


#define INITIAL_STATUS32 (0x80000000 | STATUS32_RESET_VALUE | (((INT_PRI_MAX - INT_PRI_MIN) << 1) & 0x1e)) 

extern TSKSWITCHHOOK g_pfnTskSwitchHook;
extern uint32_t _f_sdata;

volatile UINT32 context_switch_reqflg;    /* task context switch request flag in exceptions and interrupts handling */
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
            if (g_pfnTskSwitchHook != NULL)
            {
                g_pfnTskSwitchHook();
            }

            if (OS_INT_ACTIVE)
            {
                context_switch_reqflg = 1; // trigger a task dispath in interrupt/exception context
            } else {
                osTaskSchedule();  // trigger a task dispatch in task context
            }
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

#ifndef ARC_FEATURE_RF16
    pstContext->uwR25 = 0x25252525L;
    pstContext->uwR24 = 0x24242424L;
    pstContext->uwR23 = 0x23232323L;
    pstContext->uwR22 = 0x22222222L;
    pstContext->uwR21 = 0x21212121L;
    pstContext->uwR20 = 0x20202020L;
    pstContext->uwR19 = 0x19191919L;
    pstContext->uwR18 = 0x18181818L;
    pstContext->uwR17 = 0x17171717L;
    pstContext->uwR16 = 0x16161616L;
#endif
    pstContext->uwR15 = 0x15151515L;
    pstContext->uwR14 = 0x14141414L;
    pstContext->uwR13 = 0x13131313L;
    pstContext->uwBTA = 0x0;
    pstContext->uwR30 = 0x30303030L;
    pstContext->uwILINK = 0x29292929L;
    pstContext->uwFP = 0x0;
    pstContext->uwGP = (UINT32)&_f_sdata;
    pstContext->uwR12 = 0x12121212L;
    pstContext->uwR0 = uwTaskID;
    pstContext->uwR1 = 0x01010101L;
    pstContext->uwR2 = 0x02020202L;
    pstContext->uwR3 = 0x03030303L;
#ifndef ARC_FEATURE_RF16
    pstContext->uwR4 = 0x04040404L;
    pstContext->uwR5 = 0x05050505L;
    pstContext->uwR6 = 0x06060606L;
    pstContext->uwR7 = 0x07070707L;
    pstContext->uwR8 = 0x08080808L;
    pstContext->uwR9 = 0x09090909L;
#endif
    pstContext->uwR10 = 0x10101010L;
    pstContext->uwR11 = 0x11111111L;
    pstContext->uwR31 = (UINT32)osTaskExit;

    pstContext->uwLP_END = 0x0;
    pstContext->uwLP_START = 0x0; 
    pstContext->uwLP_COUNT = 0x0;
#ifdef ARC_FEATURE_CODE_DENSITY
    pstContext->uwEI = 0x0;
    pstContext->uwLDI = 0x0; 
    pstContext->uwJLI = 0x0;
#endif
    pstContext->uwPC = (UINT32)osTaskEntry;
    pstContext->uwSTATUS32 = INITIAL_STATUS32;
    return (VOID *)pstContext;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
