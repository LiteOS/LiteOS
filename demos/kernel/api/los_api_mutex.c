/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Mutex Demo
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

#include "los_mux.h"
#include "los_task.h"
#include "los_api_mutex.h"
#include "los_inspect_entry.h"

#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* mux handle id */
static UINT32 g_demoMux01;

/* task pid */
static UINT32 g_demoTaskId01;
static UINT32 g_demoTaskId02;

static VOID Example_MutexTask1(VOID)
{
    UINT32 ret;

    printf("Task1 try to get mutex, wait 10 ticks.\n");
    /* get mux */
    ret = LOS_MuxPend(g_demoMux01, 10);
    if (ret == LOS_OK) {
        printf("Task1 get mutex g_demoMux01.\n");
        /* release mux */
        LOS_MuxPost(g_demoMux01);
        return;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        printf("Task1 timeout and try to get mutex, wait forever.\n");
        /* LOS_WAIT_FOREVER type get mux, LOS_MuxPend return until has been get mux */
        ret = LOS_MuxPend(g_demoMux01, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            printf("Task1 wait forever, got mutex g_demoMux01 ok.\n");
            /* release mux */
            LOS_MuxPost(g_demoMux01);
            ret = LOS_InspectStatusSetById(LOS_INSPECT_MUTEX, LOS_INSPECT_STU_SUCCESS);
            if (LOS_OK != ret) {
                printf("Set Inspect Status Err \n");
            }
            return;
        }
    }
    return;
}

static VOID Example_MutexTask2(VOID)
{
    UINT32 ret;

    printf("Task2 try to get mutex, wait forever.\n");
    /* get mux */
    ret = LOS_MuxPend(g_demoMux01, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        printf("Task2 LOS_MuxPend failed.\n");
        return;
    }

    printf("Task2 get mutex g_demoMux01 and suspend 100 ticks.\n");

    /* task delay 100 ticks */
    LOS_TaskDelay(100);

    printf("Task2 resumed and post the g_demoMux01.\n");
    /* release mux */
    LOS_MuxPost(g_demoMux01);
    return;
}

UINT32 Example_MutexLock(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

    /* create mux */
    printf("Kernel mutex demo begin.\n");
    LOS_MuxCreate(&g_demoMux01);

    /* lock task schedue */
    LOS_TaskLock();

    /* create task1 */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    ret = LOS_TaskCreate(&g_demoTaskId01, &stTask1);
    if (ret != LOS_OK) {
        printf("Create task1 failed.\n");
        return LOS_NOK;
    }

    /* create task2 */
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = 4;
    ret = LOS_TaskCreate(&g_demoTaskId02, &stTask2);
    if (ret != LOS_OK) {
        printf("Create task2 failed.\n");
        return LOS_NOK;
    }

    /* unlock task schedue */
    LOS_TaskUnlock();
    /* task delay 300 ticks */
    LOS_TaskDelay(300);

    /* delete mux */
    LOS_MuxDelete(g_demoMux01);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
