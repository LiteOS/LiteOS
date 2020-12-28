/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Semaphore Demo
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

#include "los_sem.h"
#include "los_base.h"
#include "los_hwi.h"
#include "los_api_sem.h"
#include "los_inspect_entry.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* task prio */
#define TASK_PRIO_TEST  5

/* task pid */
static UINT32 g_demoTaskId01, g_demoTaskId02;
/* sem id */
static UINT32 g_demoSemId;

static VOID Example_SemTask1(VOID)
{
    UINT32 ret;

    printf("Example_SemTask1 try get sem g_demoSemId, timeout 10 ticks.\n");
    /* get sem, timeout is 10 ticks */
    ret = LOS_SemPend(g_demoSemId, 10);
    /* get sem ok */
    if (ret == LOS_OK) {
        LOS_SemPost(g_demoSemId);
        return;
    }
    /* timeout, get sem fail */
    if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        printf("Example_SemTask1 timeout and try get sem g_demoSemId wait forever.\n");
        /* get sem wait forever, LOS_SemPend return until has been get mux */
        ret = LOS_SemPend(g_demoSemId, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            printf("Example_SemTask1 wait_forever and got sem g_demoSemId ok.\n");
            LOS_SemPost(g_demoSemId);
            ret = LOS_InspectStatusSetById(LOS_INSPECT_SEM, LOS_INSPECT_STU_SUCCESS);
            if (ret != LOS_OK) {
                printf("Set Inspect Status Err.\n");
            }
            return;
        }
    }
    return;
}

static VOID Example_SemTask2(VOID)
{
    UINT32 ret;
    printf("Example_SemTask2 try get sem g_demoSemId wait forever.\n");
    /* wait forever get sem */
    ret = LOS_SemPend(g_demoSemId, LOS_WAIT_FOREVER);
    if (ret == LOS_OK) {
        printf("Example_SemTask2 get sem g_demoSemId and then delay 20ticks.\n");
    }

    /* task delay 20 ticks */
    LOS_TaskDelay(20);

    printf("Example_SemTask2 post sem g_demoSemId.\n");
    /* release sem */
    LOS_SemPost(g_demoSemId);

    return;
}

UINT32 Example_Semphore(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

   /* create sem */
    printf("Kernel semaphore demo begin.\n");
    LOS_SemCreate(0, &g_demoSemId);

    /* lock task schedue */
    LOS_TaskLock();

    /* create task1 */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = TASK_PRIO_TEST;
    ret = LOS_TaskCreate(&g_demoTaskId01, &stTask1);
    if (ret != LOS_OK) {
        printf("Create task1 failed.\n");
        return LOS_NOK;
    }

    /* create task2 */
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = (TASK_PRIO_TEST - 1);
    ret = LOS_TaskCreate(&g_demoTaskId02, &stTask2);
    if (ret != LOS_OK) {
        printf("Create task2 failed.\n");

        /* delete task 1 */
        if (LOS_OK != LOS_TaskDelete(g_demoTaskId01)) {
            printf("Delete task1 failed.\n");
        }

        return LOS_NOK;
    }

    /* unlock task schedue */
    LOS_TaskUnlock();

    ret = LOS_SemPost(g_demoSemId);

    /* task delay 40 ticks */
    LOS_TaskDelay(40);

    /* delete sem */
    LOS_SemDelete(g_demoSemId);

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
