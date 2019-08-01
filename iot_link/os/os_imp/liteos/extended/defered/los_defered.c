/*----------------------------------------------------------------------------
 * Copyright (c) <2019>, <Huawei Technologies Co., Ltd>
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

#include <los_config.h>
#include <los_typedef.h>

#include <los_task.h>
#include <los_hw.h>
#include <los_sem.h>

#include <los_defered.h>

#if (LOSCFG_DEFERED_JOB == YES)

static LOS_DEFERED_JOB s_astDeferedJobs[LOSCFG_NR_DEFERED_JOBS];

static UINT32 s_uwDeferedSemID;
static BOOL   s_bInitilized = FALSE;
static UINT32 s_uwDeferedTaskID;
static UINT16 s_usReadIdx  = 0;
static UINT16 s_usWriteIdx = 0;

static VOID osDeferedTask(VOID)
{
    LOS_DEFERED_JOB * pstJob;
    UINTPTR           uvKey;

    while (1)
    {
        pstJob = NULL;

        LOS_SemPend (s_uwDeferedSemID, LOS_WAIT_FOREVER);

        uvKey = LOS_IntLock ();

        if (s_usReadIdx != s_usWriteIdx)        /* if queue not empty */
        {
            pstJob = &s_astDeferedJobs[s_usReadIdx & (LOSCFG_NR_DEFERED_JOBS - 1)];
            s_usReadIdx++;
        }

        LOS_IntRestore (uvKey);

        if (pstJob != NULL)
        {
            pstJob->pfnDeferedJob (pstJob->uvArg);
        }
    }
}

UINT32 LOS_doDefered(DEFERED_FUNC pfnFunc, UINTPTR uvArg)
{
    LOS_DEFERED_JOB * pstJob;
    UINTPTR           uvKey;

    if (!s_bInitilized)
        {
        return LOS_NOK;
        }

    uvKey = LOS_IntLock ();

    if ((s_usWriteIdx - s_usReadIdx) >= LOSCFG_NR_DEFERED_JOBS)
    {
        /* full */
        LOS_IntRestore (uvKey);
        return LOS_NOK;
    }

    pstJob = &s_astDeferedJobs[s_usWriteIdx & (LOSCFG_NR_DEFERED_JOBS - 1)];

    s_usWriteIdx++;

    /*
     * these assignments must be done with IRQ locked
     * because this routine maybe invoked in task context, to prevent these
     * assignment be interrupted and shwitch to another task, they may invoke
     * this routine, and take next item, and then give the semphore, and then
     * the defered task may take this item which have not done the assignment
     */

    pstJob->pfnDeferedJob = pfnFunc;
    pstJob->uvArg         = uvArg;

    LOS_IntRestore (uvKey);

    LOS_SemPost (s_uwDeferedSemID);

    return LOS_OK;
}

#if (LOSCFG_STATIC_TASK == YES)
LOS_TASK_DEF(deferedTask, "defered", osDeferedTask,
    OS_TASK_PRIORITY_LOWEST, 0, LOSCFG_BASE_CORE_TSK_DEFERED_STACK_SIZE, 0, NULL);
#endif

/*****************************************************************************
 Function : osDeferedTaskInit
 Description : Initialize the defered job queue.
 Input       : None
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
UINT32 osDeferedTaskInit(VOID)
{
    UINT32 uwRet;

    if (LOS_SemCreate (0, &s_uwDeferedSemID) != LOS_OK)
    {
        return LOS_NOK;
    }

#if (LOSCFG_STATIC_TASK == NO)
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)osDeferedTask;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFERED_STACK_SIZE;
    stTaskInitParam.pcName = "defered";
    stTaskInitParam.usTaskPrio = 1;
    uwRet = LOS_TaskCreate(&s_uwDeferedTaskID, &stTaskInitParam);
#else
    uwRet = LOS_TASK_INIT (deferedTask, &s_uwDeferedTaskID);
#endif

    if (uwRet != LOS_OK)
    {
        LOS_SemDelete (s_uwDeferedSemID);
        return uwRet;
    }

    s_bInitilized = TRUE;

    return LOS_OK;
}

#endif

