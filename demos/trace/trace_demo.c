/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Demo Code
 * Author: Huawei LiteOS Team
 * Create: 2020-08-31
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

#include "los_trace.h"
#include "los_swtmr.h"
#include "los_mux.h"
#include "los_sem.h"
#include "los_queue.h"
#include "los_exc.h"
#include "trace_demo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TRACE_DEMO_HWI_NUM    56
#define TRACE_DEMO_TASK_PRIO  25
#define TRACE_DEMO_EVENT_WAIT 0x00000001

static HWI_IRQ_PARAM_S g_demoHwiDev1;
static HWI_IRQ_PARAM_S g_demoHwiDev2;
static EVENT_CB_S g_demoEvent;
static UINT32 g_demoTaskId;
static UINT32 g_demoQueue;
static UINT32 g_demoMux;
static UINT32 g_demoSem;

static VOID Example_TraceEasyMarco(VOID)
{
    LOS_TraceEventMaskSet(0);
    LOS_TRACE_EASY(0, 1, 2, 3);
    LOS_TRACE_EASY(0, 1);

    LOS_TraceEventMaskSet(TRACE_SWTMR_FLAG);
    LOS_TRACE_EASY(1, 2, 2, 3);
    LOS_TRACE_EASY(1, 2);
    LOS_TRACE_EASY(1, 2, 1);

    LOS_TraceEventMaskSet(TRACE_TASK_FLAG);
    LOS_TRACE_EASY(2, 3, 2, 3);
    LOS_TRACE_EASY(2, 3);
}

static VOID Timer1_Callback(UINT32 arg)
{
    printf("%s .\r\n", __FUNCTION__);
}

static VOID Timer2_Callback(UINT32 arg)
{
    printf("%s .\r\n", __FUNCTION__);
}

static VOID Example_SwTimer(VOID)
{
    UINT32 ret;
    UINT16 id1;
    UINT16 id2;

    LOS_TraceEventMaskSet(TRACE_SWTMR_FLAG);

    ret = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_Callback, &id1, 1);
    if (ret != LOS_OK) {
        printf("timer1 creat failed\n");
        return;
    }

    ret = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_Callback, &id2, 1);
    if (ret != LOS_OK) {
        printf("timer2 creat failed\n");
        return;
    }

    LOS_SwtmrStart(id1);
    (VOID)LOS_TaskDelay(200);
    LOS_SwtmrStop(id1);

    LOS_SwtmrStart(id2);
    (VOID)LOS_TaskDelay(200);
    LOS_SwtmrStop(id2);
    LOS_SwtmrDelete(id2);
}

static VOID User_IrqHandler(VOID)
{
    printf("%s .\r\n", __FUNCTION__);
}

static VOID Example_Hwi(VOID)
{
    LOS_TraceEventMaskSet(TRACE_HWI_FLAG);
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 0, 0, User_IrqHandler, 0);
    LOS_HwiTrigger(TRACE_DEMO_HWI_NUM);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, 0);

    g_demoHwiDev1.pDevId = (void *)1;
    g_demoHwiDev1.swIrq = TRACE_DEMO_HWI_NUM;
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 3, IRQF_SHARED, (HWI_PROC_FUNC)User_IrqHandler, &g_demoHwiDev1);

    g_demoHwiDev2.pDevId = (void *)2;
    g_demoHwiDev2.swIrq = TRACE_DEMO_HWI_NUM;
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 3, IRQF_SHARED, (HWI_PROC_FUNC)User_IrqHandler, &g_demoHwiDev2);

    LOS_HwiEnable(TRACE_DEMO_HWI_NUM);
    LOS_HwiTrigger(TRACE_DEMO_HWI_NUM);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, &g_demoHwiDev1);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, &g_demoHwiDev2);
}

static VOID Example_Mem(VOID)
{
    VOID *p = NULL;
    VOID *pool = NULL;

    LOS_TraceEventMaskSet(TRACE_MEM_FLAG);
    p = LOS_MemAlloc(m_aucSysMem0, 0x100);
    LOS_MemFree(m_aucSysMem0, p);

    p = LOS_MemAllocAlign(m_aucSysMem0, 0x102, 4);
    LOS_MemFree(m_aucSysMem0, p);

    LOS_MemFree(m_aucSysMem0, (VOID *)0xffffffff);

    pool = LOS_MemAlloc(m_aucSysMem0, 0x2000);
    if (pool == NULL) {
        printf("alloc failed\n");
        return;
    }

    LOS_MemInit(pool, 0x2000);
    p = LOS_MemAlloc(pool, 0x200);
    LOS_MemFree(pool, p);

    LOS_MemFree(m_aucSysMem0, pool);
}

static VOID Pend_Entry(VOID)
{
#define QUEUE_BUFFER_SIZE 32
    CHAR buf[QUEUE_BUFFER_SIZE] = {0};

    LOS_EventRead(&g_demoEvent, TRACE_DEMO_EVENT_WAIT, LOS_WAITMODE_AND, 100);
    LOS_EventClear(&g_demoEvent, ~g_demoEvent.uwEventID);

    LOS_SemPend(g_demoSem, 10);
    LOS_SemDelete(g_demoSem);

    LOS_QueueRead(g_demoQueue, &buf, QUEUE_BUFFER_SIZE, LOS_WAIT_FOREVER);
    LOS_QueueDelete(g_demoQueue);

    LOS_MuxPend(g_demoMux, LOS_WAIT_FOREVER);
    LOS_MuxPost(g_demoMux);
    LOS_MuxDelete(g_demoMux);
}

static VOID Example_Ipc(VOID)
{
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam;

    LOS_TraceEventMaskSet(TRACE_EVENT_FLAG | TRACE_MUX_FLAG | TRACE_SEM_FLAG | TRACE_QUE_FLAG);

    LOS_EventInit(&g_demoEvent);
    LOS_SemCreate(0, &g_demoSem);
    LOS_MuxCreate(&g_demoMux);
    LOS_MuxPend(g_demoMux, LOS_WAIT_FOREVER);

    LOS_QueueCreate("queue", 5, &g_demoQueue, 0, 24);

    memset(&taskInitParam, 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.uwStackSize   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio    = TRACE_DEMO_TASK_PRIO -1;
    taskInitParam.pfnTaskEntry  = (TSK_ENTRY_FUNC)Pend_Entry;
    taskInitParam.pcName        = "taskPendEntry";
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    LOS_TaskCreate(&taskId, &taskInitParam);

    LOS_EventWrite(&g_demoEvent, TRACE_DEMO_EVENT_WAIT);
    LOS_SemPost(g_demoSem);

    LOS_QueueWrite(g_demoQueue, "buff", 8, 0);
    LOS_MuxPost(g_demoMux);
}

static VOID Example_Trace(VOID)
{
    UINT32 ret;
    printf("trace sample run\n");
    LOS_TaskDelay(20);

    ret = LOS_TraceStart();
    if (ret != LOS_OK) {
        printf("trace start error\n");
        return;
    }

    Example_TraceEasyMarco();

    /* trigger task switch event */
    LOS_TaskDelay(1);
    LOS_TaskDelay(1);
    LOS_TaskDelay(1);

    /* trigger hwi event */
    Example_Hwi();

    /* trigger swtmr event */
    Example_SwTimer();

    /* trigger memory event */
    Example_Mem();

    /* trigger sem\queue\event\mux event */
    Example_Ipc();

    LOS_TraceStop();
    LOS_TraceRecordDump(FALSE);

    printf("trace sample run end\n");
}

UINT32 create_trace_task(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S traceDemoTask;

    memset(&traceDemoTask, 0, sizeof(TSK_INIT_PARAM_S));
    traceDemoTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Trace;
    traceDemoTask.pcName       = "TraceDemo";
    traceDemoTask.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    traceDemoTask.usTaskPrio   = TRACE_DEMO_TASK_PRIO;
    traceDemoTask.uwResved     = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &traceDemoTask);
    if (ret != LOS_OK) {
        printf("traceDemoTask create failed .\n");
        return LOS_NOK;
    }

    /* trace is already started in offline mode, user can stop and reset it before run demo */
#ifdef LOSCFG_RECORDER_MODE_OFFLINE
    LOS_TraceStop();
    // LOS_TraceReset();
#endif
    LOS_TraceEventMaskSet(TRACE_TASK_FLAG);
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
