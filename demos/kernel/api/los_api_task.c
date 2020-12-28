/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Task Demo
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "math.h"
#include "time.h"
#include "los_task.h"
#include "los_api_task.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 g_demoTaskHiId;
static UINT32 g_demoTaskLoId;

#define TSK_PRIOR_HI 4
#define TSK_PRIOR_LO 5

static UINT32 Example_TaskHi(VOID)
{
    UINT32 ret;

    printf("Enter TaskHi Handler.\n");

    /* task delay 5 ticks, task will be suspend */
    ret = LOS_TaskDelay(5);
    if (ret != LOS_OK) {
        printf("Delay Task Failed.\n");
        return LOS_NOK;
    }

    /* task resumed */
    printf("TaskHi LOS_TaskDelay Done.\n");

    /* suspend self */
    ret = LOS_TaskSuspend(g_demoTaskHiId);
    if (ret != LOS_OK) {
        printf("Suspend TaskHi Failed.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskResume ok.\n");

    ret = LOS_InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_SUCCESS);
    if (ret != LOS_OK) {
        printf("Set Inspect Status Err.\n");
    }

    /* delete self */
    if (LOS_TaskDelete(g_demoTaskHiId) != LOS_OK) {
        printf("Delete TaskHi failed.\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

static UINT32 Example_TaskLo(VOID)
{
    UINT32 ret;

    printf("Enter TaskLo Handler.\n");

    /* task delay 10 ticks, task will be suspend */
    ret = LOS_TaskDelay(10);
    if (ret != LOS_OK) {
        printf("Delay TaskLo Failed.\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskSuspend ok.\n");

    /* resumed task g_demoTaskHiId */
    ret = LOS_TaskResume(g_demoTaskHiId);
    if (ret != LOS_OK) {
        printf("Resume TaskHi Failed.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_ERROR);
        if (LOS_OK != ret) {
            printf("Set Inspect Status Err.\n");
        }
        return LOS_NOK;
    }

    /* delete self */
    if (LOS_TaskDelete(g_demoTaskLoId) != LOS_OK) {
        printf("Delete taskLo failed.\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

UINT32 Example_TskCaseEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stInitParam;

    /* lock task shcedue */
    printf("\nKernel task demo begin.\n");
    LOS_TaskLock();

    printf("LOS_TaskLock() ok.\n");

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
    stInitParam.usTaskPrio = TSK_PRIOR_HI;
    stInitParam.pcName = "HIGH_NAME";
    stInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    /* create high prio task */
    ret = LOS_TaskCreate(&g_demoTaskHiId, &stInitParam);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();

        printf("Example_TaskHi failed.\n");
        return LOS_NOK;
    }

    printf("Create Example_TaskHi ok.\n");

    stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo;
    stInitParam.usTaskPrio = TSK_PRIOR_LO;
    stInitParam.pcName = "LOW_NAME";
    stInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    /* create low prio task */
    ret = LOS_TaskCreate(&g_demoTaskLoId, &stInitParam);
    if (ret != LOS_OK) {
        /* delete high prio task */
        if (LOS_OK != LOS_TaskDelete(g_demoTaskHiId)) {
            printf("Delete TaskHi failed.\n");
        }

        LOS_TaskUnlock();

        printf("Create Example_TaskLo failed.\n");

        return LOS_NOK;
    }

    printf("Create Example_TaskLo ok.\n");

    /* unlock task schedue */
    LOS_TaskUnlock();
    LOS_TaskDelay(40);
    
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
