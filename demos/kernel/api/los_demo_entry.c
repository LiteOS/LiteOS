/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Demo Entry
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "los_demo_entry.h"
#include "los_task.h"
#include <string.h>

static UINT32 g_DemoTaskId;

static LITE_OS_SEC_TEXT VOID LOS_Demo_Tskfunc(VOID)
{
#ifdef LOS_KERNEL_DEMO_ALL
#else /* LOS_KERNEL_DEMO_ALL */

/* only test some function */
#ifdef LOS_KERNEL_DEMO_TASK
    Example_TskCaseEntry();
    printf("Kernel task demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MEM_DYNAMIC
    Example_Dyn_Mem();
    printf("Kernel dynamic memory demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MEM_STATIC
    Example_StaticMem();
    printf("Kernel static memory demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_INTERRUPT
    Example_Interrupt();
    printf("Kernel interrupt demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_QUEUE
    Example_MsgQueue();
    printf("Kernel message queue demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_EVENT
    Example_SndRcvEvent();
    printf("Kernel event demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MUTEX
    Example_MutexLock();
    printf("Kernel mutex demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SEMPHORE
    Example_Semphore();
    printf("Kernel semaphore demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SYSTICK
    Example_GetTick();
    printf("Kernel systick demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SWTIMER
    Example_SwTimer();
    printf("Kernel swtimer demo ok.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_LIST
    Example_List();
    printf("Kernel list demo ok.\n\n");
#endif
#endif /* LOS_KERNEL_DEMO_ALL */

    while (1) {
        (VOID)LOS_TaskDelay(100);
    }
}

UINT32 KernelDemoEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Demo_Tskfunc;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTaskInitParam.pcName = "KernelApiDemo";
    stTaskInitParam.usTaskPrio = 30;
    ret = LOS_TaskCreate(&g_DemoTaskId, &stTaskInitParam);
    if (ret != LOS_OK) {
        printf("Api demo test task create failed.\n");
        return ret;
    }
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
