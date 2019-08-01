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

#include "los_config.h"
#include "los_base.h"
#include "los_task.ph"
#include "los_hw.h"
#include "los_priqueue.ph"
#include "los_mpu.h"
#include "los_memory.h"
#include "los_printf.h"

#if (LOSCFG_ENABLE_MPU == YES)
#include "heap.h"
#include "los_svc.h"
#endif

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
LITE_OS_SEC_TEXT VOID osSchedule(VOID)
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
LITE_OS_SEC_TEXT VOID LOS_Schedule(VOID)
{
    UINTPTR uvIntSave;

    uvIntSave = LOS_IntLock();

    /* Find the highest task */
    g_stLosTask.pstNewTask = LOS_DL_LIST_ENTRY(osPriqueueTop(), LOS_TASK_CB, stPendList);

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
    __disable_irq();
    while(1);
}

#if (LOSCFG_ENABLE_MPU == YES)
LITE_OS_SEC_TEXT_INIT VOID osUsrTaskEntry(UINT32 uwTaskID)
{
    LOS_TASK_CB    *pstTaskCB;
    TSK_ENTRY_FUNC entry;
    UINT32         arg;

    pstTaskCB = OS_TCB_FROM_TID(uwTaskID);

    entry = pstTaskCB->pfnTaskEntry;
    arg   = pstTaskCB->uwArg;

    /* enter unprivileged */

    __set_CONTROL(__get_CONTROL() | CONTROL_nPRIV_Msk);

    (VOID)entry(arg);

    (VOID)LOS_UsrTaskDelete(uwTaskID);
}
#endif

/*****************************************************************************
 Function    : osTskStackInit
 Description : Task stack initialization function
 Input       : pstTaskCB    --- TCB
               pstInitParam --- Task init parameters
 Output      : None
 Return      :
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT VOID osTskStackInit(LOS_TASK_CB *pstTaskCB, TSK_INIT_PARAM_S *pstInitParam)
{
    TSK_CONTEXT_S  *pstContext;
    char *pTopStack = (char *)pstTaskCB->uwTopOfStack;
    char *pStack = pTopStack + pstTaskCB->uwStackSize;

#if (LOSCFG_ENABLE_MPU == YES)
    int i;   //this var only be used when mpu enabled --zhangqf
    static LOS_MPU_ENTRY astNullMpuSetting [MPU_NR_USR_ENTRIES + 1] = {0};
    LOS_MPU_ENTRY * pstMpuSetting = NULL;
#if (LOSCFG_STATIC_TASK == YES)
    LOS_MPU_PARA  * pstMpuPara = (LOS_MPU_PARA *)pstTaskCB->pMpuSettings;
#else
    LOS_MPU_PARA  * pstMpuPara = (LOS_MPU_PARA *)pstInitParam->pRegions;
#endif
#endif

    /*initialize the task stack, write magic num to stack top*/
    memset(pTopStack, OS_TASK_STACK_INIT, pstTaskCB->uwStackSize);
    *((UINT32 *)(pTopStack)) = OS_TASK_MAGIC_WORD;

#if (LOSCFG_ENABLE_MPU == YES)

    if (pstMpuPara != NULL)
    {
        UINT32 allRegionBase, allRegionSize;

        /* per-task heap is only used for unprivileged task */

#if (LOSCFG_STATIC_TASK == NO)
        pstTaskCB->uwHeapSize = pstInitParam->uwHeapSize;
#endif

        if (pstTaskCB->uwHeapSize)
        {

            /* the pool address is just the begin of stack, see osTskStackAlloc */

            if (LOS_MemInit (pStack, pstTaskCB->uwHeapSize))
            {
                PRINT_ERR("init per task heap fail!\n");
                pstTaskCB->pPool = NULL;
            }
            else
            {
                pstTaskCB->pPool = pStack;
            }
        }

        /* plus 1 for stack */

        pStack -= sizeof(LOS_MPU_ENTRY) * (MPU_NR_USR_ENTRIES + 1);
        pstMpuSetting = (LOS_MPU_ENTRY *)pStack;

        for (i = 0; i < MPU_NR_USR_ENTRIES; i++)
        {
            pstMpuSetting [i].uwRegionAddr = pstMpuPara[i].uwRegionAddr |
                MPU_RBAR_REGION (i + MPU_FIRST_USR_REGION) | MPU_RBAR_VALID;
            pstMpuSetting [i].uwRegionAttr = pstMpuPara[i].uwRegionAttr |
                MPU_ATTR_SZ (pstMpuPara[i].uwRegionSize);
        }

        allRegionBase = pstTaskCB->uwTopOfStack;
        allRegionSize = pstTaskCB->uwStackSize + pstTaskCB->uwHeapSize;

        pstMpuSetting [MPU_NR_USR_ENTRIES].uwRegionAddr =
            allRegionBase | MPU_RBAR_VALID |
            MPU_RBAR_REGION (MPU_NR_USR_ENTRIES + MPU_FIRST_USR_REGION);
        pstMpuSetting [MPU_NR_USR_ENTRIES].uwRegionAttr =
            MPU_ATTR_RW_RW | MPU_ATTR_SZ (allRegionSize) | MPU_ATTR_WB |
            MPU_ATTR_EN | MPU_ATTR_XN;
    }
    else
    {
        pstTaskCB->pPool = (void *)OS_SYS_MEM_ADDR;

        pstMpuSetting = astNullMpuSetting;

        for (i = 0; i < MPU_NR_USR_ENTRIES + 1; i++)
        {
            pstMpuSetting [i].uwRegionAddr = 0 |
                MPU_RBAR_REGION (i + MPU_FIRST_USR_REGION) | MPU_RBAR_VALID;
            pstMpuSetting [i].uwRegionAttr = 0;
        }
    }

    pstTaskCB->pMpuSettings = (VOID *)pstMpuSetting;
#endif

    pStack -= sizeof(TSK_CONTEXT_S);

    pstContext = (TSK_CONTEXT_S *)pStack;

#if (LOSCFG_ENABLE_MPU == YES)

    /*
     * 1. CONTROL.SPSEL is reserved in handler mode, and context load is always
     *    happend in handler mode expected the first one, so do not set this bit
     * 2. task is created at priveledged mode, for usr task, it is switched to
     *    usr mode in osUsrTaskEntry
     */

    pstContext->uwControl = 0;
#endif

    pstContext->uwR4  = 0x04040404L;
    pstContext->uwR5  = 0x05050505L;
    pstContext->uwR6  = 0x06060606L;
    pstContext->uwR7  = 0x07070707L;
    pstContext->uwR8  = 0x08080808L;
    pstContext->uwR9  = 0x09090909L;
    pstContext->uwR10 = 0x10101010L;
    pstContext->uwR11 = 0x11111111L;
    /* The initial interruption state(PRIMASK value: 0 --- enable) of the task */
    pstContext->uwPriMask = 0;
#if FPU_EXIST
    /**
     * The initial EXC_RETURN value(use 8 word stack frame, return to thread mode and use PSP).
     * Please do not modify it.
     */
    pstContext->uwExcReturn = 0xFFFFFFFD;
#endif
    pstContext->uwR0  = pstTaskCB->uwTaskID;
    pstContext->uwR1  = 0x01010101L;
    pstContext->uwR2  = 0x02020202L;
    pstContext->uwR3  = 0x03030303L;
    pstContext->uwR12 = 0x12121212L;
    pstContext->uwLR  = (UINT32)osTaskExit;
#if (LOSCFG_ENABLE_MPU == YES)
    if (pstMpuPara != NULL)
    {
        pstContext->uwPC  = (UINT32)osUsrTaskEntry;
    }
    else
#endif
    pstContext->uwPC  = (UINT32)osTaskEntry;
    pstContext->uwxPSR = 0x01000000L;

    pstTaskCB->pStackPointer = (VOID *)pStack;

    return;
}

#if (LOSCFG_STATIC_TASK == NO)
LITE_OS_SEC_TEXT_INIT VOID *osTskStackAlloc (TSK_INIT_PARAM_S *pstInitParam)
{
    UINT32 align = LOSCFG_STACK_POINT_ALIGN_SIZE;
    UINT32 alloc;

    /* TSK_CONTEXT_S will take space in stack, reserve space for it */

    pstInitParam->uwStackSize += sizeof (TSK_CONTEXT_S);

#if (LOSCFG_ENABLE_MPU == YES)
    if (pstInitParam->pRegions != NULL)
    {

        /* LOS_MPU_ENTRY will be allocated from stack, reserve space for it,
           +1 for the stack entry */

        pstInitParam->uwStackSize += sizeof (LOS_MPU_ENTRY) * (MPU_NR_USR_ENTRIES + 1);

        /* make sure the heap size is correctly aligned */

        if (pstInitParam->uwHeapSize != 0)
            {
            pstInitParam->uwHeapSize += sizeof (heap_t);
            }

        pstInitParam->uwHeapSize = ALIGN(pstInitParam->uwHeapSize, 8);

        alloc = pstInitParam->uwStackSize + pstInitParam->uwHeapSize;

        if (alloc < 256)        /* the minimal supported region size is 256 */
        {
            alloc = 256;
        }
        else
        {
            /* make sure the size is power of 2 */

            if (alloc & (alloc - 1))
            {
                alloc = 1 << (32 - CLZ (alloc));
            }
        }

        pstInitParam->uwStackSize = alloc - pstInitParam->uwHeapSize;

        align = alloc;
    }
    else
#endif
    {
        alloc = pstInitParam->uwStackSize;
    }

    return LOS_MemAllocAlign(OS_TASK_STACK_ADDR, alloc, align);
}
#endif


LITE_OS_SEC_TEXT_INIT VOID osEnterSleep(VOID)
{
    __DSB();
    __WFI();
    __ISB();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


