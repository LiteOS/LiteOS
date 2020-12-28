/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: CMSIS Interface V2.0
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
 *---------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "securec.h"
#include "los_typedef.h"
#include "los_printf.h"
#include "los_hwi.h"
#include "los_sys.h"
#include "los_task_pri.h"
#include "los_tick_pri.h"
#ifdef LOSCFG_BASE_IPC_EVENT
#include "los_event.h"
#endif
#ifdef LOSCFG_BASE_CORE_SWTMR
#include "los_swtmr_pri.h"
#endif
#ifdef LOSCFG_BASE_IPC_MUX
#include "los_mux_pri.h"
#endif
#ifdef LOSCFG_BASE_IPC_SEM
#include "los_sem_pri.h"
#endif
#ifdef LOSCFG_BASE_IPC_QUEUE
#include "los_queue_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (CMSIS_OS_VER == 2)

#define KERNEL_UNLOCKED 0
#define KERNEL_LOCKED   1

#define LOS_PRIORITY_WIN   10

#define KERNEL_ID   "HUAWEI-LiteOS"
static const osVersion_t g_losVersion = {20010016, 20010016};

/* Kernel initialization state */
static osKernelState_t g_kernelState = osKernelInactive;

//  ==== Kernel Management Functions ====
osStatus_t osKernelInitialize(void)
{
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (g_kernelState != osKernelInactive) {
        return osError;
    }

    if (OsMain() == LOS_OK) {
        g_kernelState = osKernelReady;
        return osOK;
    } else {
        return osError;
    }
}

osStatus_t osKernelGetInfo(osVersion_t *version, char *id_buf, uint32_t id_size)
{
    errno_t ret;

    if ((version == NULL) || (id_buf == NULL) || (id_size == 0)) {
        return osError;
    }

    ret = memcpy_s(id_buf, id_size, KERNEL_ID, sizeof(KERNEL_ID));
    if (ret == EOK) {
        version->api = g_losVersion.api;
        version->kernel = g_losVersion.kernel;
        return osOK;
    } else {
        PRINT_ERR("[%s] memcpy_s failed, error type = %u\n", __func__, ret);
        return osError;
    }
}

osKernelState_t osKernelGetState(void)
{
    if (!g_taskScheduled) {
        if (g_kernelState == osKernelReady) {
            return osKernelReady;
        } else {
            return osKernelInactive;
        }
    } else if (OsPercpuGet()->taskLockCnt > 0) {
        return osKernelLocked;
    } else {
        return osKernelRunning;
    }
}

osStatus_t osKernelStart(void)
{
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }
    if (g_kernelState != osKernelReady) {
        return osError;
    }

    g_kernelState = osKernelRunning;
    OsStart();
    return osOK;
}

int32_t osKernelLock(void)
{
    int32_t lock;

    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }

    if (!g_taskScheduled) {
        return (int32_t)osError;
    }

    if (OsPercpuGet()->taskLockCnt > 0) {
        lock = KERNEL_LOCKED;
    } else {
        LOS_TaskLock();
        lock = KERNEL_UNLOCKED;
    }

    return lock;
}

int32_t osKernelUnlock(void)
{
    int32_t lock;

    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }

    if (!g_taskScheduled) {
        return (int32_t)osError;
    }

    if (OsPercpuGet()->taskLockCnt > 0) {
        LOS_TaskUnlock();
        if (OsPercpuGet()->taskLockCnt != 0) {
            return (int32_t)osError;
        }
        lock = KERNEL_LOCKED;
    } else {
        lock = KERNEL_UNLOCKED;
    }

    return lock;
}

int32_t osKernelRestoreLock(int32_t lock)
{
    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }

    if (!g_taskScheduled) {
        return (int32_t)osError;
    }

    switch (lock) {
        case KERNEL_UNLOCKED:
            LOS_TaskUnlock();
            if (OsPercpuGet()->taskLockCnt != 0) {
                break;
            }
            return KERNEL_UNLOCKED;
        case KERNEL_LOCKED:
            LOS_TaskLock();
            return KERNEL_LOCKED;
        default:
            break;
    }

    return (int32_t)osError;
}

uint64_t osKernelGetTickCount(void)
{
    return (uint64_t)LOS_TickCountGet();
}

uint64_t osKernelGetTick2ms(void)
{
    return osKernelGetTickCount() * OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND;
}

uint64_t osMs2Tick(uint64_t ms)
{
    return ms * LOSCFG_BASE_CORE_TICK_PER_SECOND / OS_SYS_MS_PER_SECOND;
}

uint32_t osKernelGetTickFreq(void)
{
    return (uint32_t)LOSCFG_BASE_CORE_TICK_PER_SECOND;
}

uint32_t osKernelGetSysTimerCount(void)
{
    UINT32 countHigh = 0;
    UINT32 countLow = 0;

    LOS_GetCpuCycle((UINT32 *)&countHigh, (UINT32 *)&countLow);
    return countLow;
}

uint32_t osKernelGetSysTimerFreq(void)
{
    return (uint32_t)OS_SYS_CLOCK;
}


//  ==== Thread Management Functions ====
osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam;

    if ((func == NULL) || (attr == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }
    if ((attr->priority < osPriorityLow3) || (attr->priority > osPriorityHigh)) {
        PRINT_ERR("[%s] Fail, NOT in adapt priority range: [osPriorityLow3 : osPriorityHigh].\n", __func__);
        return NULL;
    }

    /* Ignore the return code when matching CSEC rule 6.6(4). */
    (VOID)memset_s(&taskInitParam, sizeof(taskInitParam), 0, sizeof(taskInitParam));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)func;
    taskInitParam.uwStackSize = attr->stack_size;
    taskInitParam.pcName = (CHAR *)attr->name;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    /* task priority: 0~31 */
    taskInitParam.usTaskPrio = (UINT16)(OS_TASK_PRIORITY_LOWEST - (attr->priority - LOS_PRIORITY_WIN));
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, argument);

    ret = LOS_TaskCreate(&taskId, &taskInitParam);
    if (ret == LOS_OK) {
        return (osThreadId_t)OS_TCB_FROM_TID(taskId);
    } else {
        return NULL;
    }
}

const char *osThreadGetName(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;

    if (taskCB == NULL) {
        return NULL;
    }

    return taskCB->taskName;
}

osThreadId_t osThreadGetId(void)
{
    return (osThreadId_t)OsCurrTaskGet();
}

osThreadState_t osThreadGetState(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    osThreadState_t state;
    UINT16 taskStatus;

    if ((taskCB == NULL) || OS_INT_ACTIVE) {
        return osThreadError;
    }

    taskStatus = taskCB->taskStatus;
    if (taskStatus & OS_TASK_STATUS_RUNNING) {
        state = osThreadRunning;
    } else if (taskStatus & OS_TASK_STATUS_READY) {
        state = osThreadReady;
    } else if (taskStatus &
        (OS_TASK_STATUS_DELAY | OS_TASK_STATUS_PEND |
         OS_TASK_STATUS_SUSPEND | OS_TASK_STATUS_PEND_TIME)) {
        state = osThreadBlocked;
    } else if (taskStatus & OS_TASK_STATUS_UNUSED) {
        state = osThreadInactive;
    } else {
        state = osThreadError;
    }

    return state;
}

osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 ret;
    UINT16 losPrio;

    if (taskCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }
    if ((priority < osPriorityLow3) || (priority > osPriorityHigh)) {
        PRINT_ERR("[%s] Fail, NOT in adapt priority range: [osPriorityLow3 : osPriorityHigh].\n", __func__);
        return osErrorParameter;
    }

    losPrio = (UINT16)(OS_TASK_PRIORITY_LOWEST - (priority - LOS_PRIORITY_WIN));
    ret = LOS_TaskPriSet(taskCB->taskId, losPrio);
    if (ret == LOS_OK) {
        return osOK;
    } else if ((ret == LOS_ERRNO_TSK_ID_INVALID) || (ret == LOS_ERRNO_TSK_PRIOR_ERROR)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osPriority_t osThreadGetPriority(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT16 losPrio;
    UINT16 priorityRet;

    if ((taskCB == NULL) || OS_INT_ACTIVE) {
        return osPriorityError;
    }

    losPrio = LOS_TaskPriGet(taskCB->taskId);
    if (losPrio > OS_TASK_PRIORITY_LOWEST) {
        return osPriorityError;
    }

    priorityRet = (UINT16)((OS_TASK_PRIORITY_LOWEST - losPrio) + LOS_PRIORITY_WIN);
    return (osPriority_t)priorityRet;
}

osStatus_t osThreadYield(void)
{
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskYield();
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osError;
    }
}

osStatus_t osThreadSuspend(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 ret;

    if (taskCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskSuspend(taskCB->taskId);
    if ((ret == LOS_OK) || (ret == LOS_ERRNO_TSK_ALREADY_SUSPENDED)) {
        return osOK;
    } else if (ret == LOS_ERRNO_TSK_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osStatus_t osThreadResume(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 ret;

    if (taskCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskResume(taskCB->taskId);
    if ((ret == LOS_OK) || (ret == LOS_ERRNO_TSK_NOT_SUSPENDED)) {
        return osOK;
    } else if (ret == LOS_ERRNO_TSK_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 ret;

    if (taskCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskDelete(taskCB->taskId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_TSK_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

uint32_t osThreadGetStackSize(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;

    if ((taskCB == NULL) || OS_INT_ACTIVE) {
        return 0;
    }

    return (uint32_t)taskCB->stackSize;
}

uint32_t osThreadGetStackSpace(osThreadId_t thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 intSave;
    UINT32 count = 0;
    UINT32 *topOfStack = NULL;

    if ((taskCB == NULL) || OS_INT_ACTIVE) {
        return 0;
    }

    intSave = LOS_IntLock();
    if ((taskCB->taskStatus) & OS_TASK_STATUS_UNUSED) {
        LOS_IntRestore(intSave);
        return 0;
    }

    /* first 4 bytes is OS_TASK_MAGIC_WORD, skip */
    topOfStack = (UINT32 *)(UINTPTR)taskCB->topOfStack + 1;
    while (*topOfStack == (UINT32)OS_STACK_INIT) {
        ++topOfStack;
        ++count;
    }
    LOS_IntRestore(intSave);
    count *= sizeof(UINT32);

    return (uint32_t)count;
}

uint32_t osThreadGetCount(void)
{
    UINT32 count = 0;
    UINT32 taskId;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return 0;
    }

    for (taskId = 0; taskId <= LOSCFG_BASE_CORE_TSK_LIMIT; taskId++) {
        taskCB = OS_TCB_FROM_TID(taskId);
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
        count++;
    }

    return (uint32_t)count;
}


//  ==== Thread Flags Functions ====
#ifdef LOSCFG_BASE_IPC_EVENT

uint32_t osThreadFlagsSet(osThreadId_t thread_id, uint32_t flags)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 ret;
    EVENT_CB_S *eventCB = NULL;
    UINT32 eventSave;

    if (taskCB == NULL) {
        return (uint32_t)osFlagsErrorParameter;
    }

    eventCB = &(taskCB->event);
    eventSave = eventCB->uwEventID;
    ret = LOS_EventWrite(eventCB, (UINT32)flags);
    if (ret == LOS_OK) {
        return ((uint32_t)eventSave | flags);
    } else if (ret == LOS_ERRNO_EVENT_SETBIT_INVALID) {
        return (uint32_t)osFlagsErrorParameter;
    } else {
        return (uint32_t)osFlagsErrorResource;
    }
}

uint32_t osThreadFlagsClear(uint32_t flags)
{
    UINT32 intSave;
    UINT32 ret;
    LosTaskCB *runTask = NULL;
    EVENT_CB_S *eventCB = NULL;
    UINT32 rflags;

    if (OS_INT_ACTIVE) {
        return (uint32_t)osFlagsErrorUnknown;
    }

    intSave = LOS_IntLock();
    runTask = OsCurrTaskGet();
    eventCB = &(runTask->event);
    rflags = eventCB->uwEventID;
    ret = LOS_EventClear(eventCB, ~(UINT32)flags);
    LOS_IntRestore(intSave);
    if (ret == LOS_OK) {
        return (uint32_t)rflags;
    } else {
        return (uint32_t)osFlagsErrorResource;
    }
}

uint32_t osThreadFlagsGet(void)
{
    LosTaskCB *runTask = NULL;
    EVENT_CB_S *eventCB = NULL;

    if (OS_INT_ACTIVE) {
        return (uint32_t)osFlagsErrorUnknown;
    }

    runTask = OsCurrTaskGet();
    eventCB = &(runTask->event);
    return (uint32_t)(eventCB->uwEventID);
}

uint32_t osThreadFlagsWait(uint32_t flags, uint32_t options, uint32_t timeout)
{
    UINT32 ret;
    LosTaskCB *runTask = NULL;
    EVENT_CB_S *eventCB = NULL;
    UINT32 mode = 0;

    if (OS_INT_ACTIVE) {
        return (uint32_t)osFlagsErrorUnknown;
    }

    if (options > (osFlagsWaitAny | osFlagsWaitAll | osFlagsNoClear)) {
        return (uint32_t)osFlagsErrorParameter;
    }

    if ((options & osFlagsWaitAll) == osFlagsWaitAll) {
        mode |= LOS_WAITMODE_AND;
    } else {
        mode |= LOS_WAITMODE_OR;
    }

    if ((options & osFlagsNoClear)  == osFlagsNoClear) {
        mode &= ~LOS_WAITMODE_CLR;
    } else {
        mode |= LOS_WAITMODE_CLR;
    }

    runTask = OsCurrTaskGet();
    eventCB = &(runTask->event);
    ret = LOS_EventRead(eventCB, (UINT32)flags, mode, (UINT32)timeout);
    if (!(ret & LOS_ERRTYPE_ERROR)) {
        return (uint32_t)ret;
    }

    switch (ret) {
        case LOS_ERRNO_EVENT_PTR_NULL:
        case LOS_ERRNO_EVENT_SETBIT_INVALID:
        case LOS_ERRNO_EVENT_EVENTMASK_INVALID:
        case LOS_ERRNO_EVENT_FLAGS_INVALID:
            return (uint32_t)osFlagsErrorParameter;
        case LOS_ERRNO_EVENT_READ_TIMEOUT:
            return (uint32_t)osFlagsErrorTimeout;
        default:
            return (uint32_t)osFlagsErrorResource;
    }
}

#endif /* LOSCFG_BASE_IPC_EVENT */


//  ==== Event Flags Management Functions ====
#ifdef LOSCFG_BASE_IPC_EVENT

osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
{
    (VOID)attr;
    PEVENT_CB_S eventCB = NULL;
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return NULL;
    }

    eventCB = (PEVENT_CB_S)LOS_MemAlloc(m_aucSysMem0, sizeof(EVENT_CB_S));
    if (eventCB == NULL) {
        return NULL;
    }

    ret = LOS_EventInit(eventCB);
    if (ret == LOS_OK) {
        return (osEventFlagsId_t)eventCB;
    } else {
        if (LOS_MemFree(m_aucSysMem0, eventCB) != LOS_OK) {
            PRINT_ERR("[%s] memory free fail!\n", __func__);
        }
        return NULL;
    }
}

uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)ef_id;
    UINT32 ret;
    UINT32 rflags;

    ret = LOS_EventWrite(eventCB, (UINT32)flags);
    if (ret == LOS_OK) {
        rflags = eventCB->uwEventID;
        return rflags;
    } else if ((ret == LOS_ERRNO_EVENT_PTR_NULL) || (ret == LOS_ERRNO_EVENT_SETBIT_INVALID)) {
        return (uint32_t)osFlagsErrorParameter;
    } else {
        return (uint32_t)osFlagsErrorResource;
    }
}

uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)ef_id;
    UINT32 intSave;
    UINT32 ret;
    UINT32 rflags;

    if (eventCB == NULL) {
        return (UINT32)osFlagsErrorParameter;
    }

    intSave = LOS_IntLock();
    rflags = eventCB->uwEventID;

    ret = LOS_EventClear(eventCB, ~flags);
    LOS_IntRestore(intSave);
    if (ret == LOS_OK) {
        return (uint32_t)rflags;
    } else if (ret == LOS_ERRNO_EVENT_PTR_NULL) {
        return (uint32_t)osFlagsErrorParameter;
    } else {
        return (uint32_t)osFlagsErrorResource;
    }
}

uint32_t osEventFlagsGet(osEventFlagsId_t ef_id)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)ef_id;
    UINT32 intSave;
    UINT32 rflags;

    if (eventCB == NULL) {
        return (uint32_t)osFlagsErrorParameter;
    }

    intSave = LOS_IntLock();
    rflags = eventCB->uwEventID;
    LOS_IntRestore(intSave);

    return (uint32_t)rflags;
}

uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)ef_id;
    UINT32 ret;
    UINT32 mode = 0;

    if (OS_INT_ACTIVE && (timeout != 0)) {
        return (uint32_t)osFlagsErrorParameter;
    }
    if (options > (osFlagsWaitAny | osFlagsWaitAll | osFlagsNoClear)) {
        return (uint32_t)osFlagsErrorParameter;
    }

    if ((options & osFlagsWaitAll) == osFlagsWaitAll) {
        mode |= LOS_WAITMODE_AND;
    } else {
        mode |= LOS_WAITMODE_OR;
    }

    if ((options & osFlagsNoClear) == osFlagsNoClear) {
        mode &= ~LOS_WAITMODE_CLR;
    } else {
        mode |= LOS_WAITMODE_CLR;
    }

    ret = LOS_EventRead(eventCB, (UINT32)flags, mode, (UINT32)timeout);
    if (!(ret & LOS_ERRTYPE_ERROR)) {
        return (uint32_t)ret;
    }

    switch (ret) {
        case LOS_ERRNO_EVENT_PTR_NULL:
        case LOS_ERRNO_EVENT_SETBIT_INVALID:
        case LOS_ERRNO_EVENT_EVENTMASK_INVALID:
        case LOS_ERRNO_EVENT_FLAGS_INVALID:
            return (uint32_t)osFlagsErrorParameter;
        case LOS_ERRNO_EVENT_READ_TIMEOUT:
            return (uint32_t)osFlagsErrorTimeout;
        default:
            return (uint32_t)osFlagsErrorResource;
    }
}

osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)ef_id;
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_EventDestroy(eventCB);
    if (ret == LOS_OK) {
        if (LOS_MemFree(m_aucSysMem0, (VOID *)eventCB) == LOS_OK) {
            return osOK;
        } else {
            PRINT_ERR("[%s] memory free fail!\n", __func__);
            return osErrorResource;
        }
    } else if (ret == LOS_ERRNO_EVENT_PTR_NULL) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

const char *osEventFlagsGetName(osEventFlagsId_t ef_id)
{
    (VOID)ef_id;
    return NULL;
}

#endif /* LOSCFG_BASE_IPC_EVENT */


//  ==== Generic Wait Functions ====
osStatus_t osDelay(uint32_t ticks)
{
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskDelay(ticks);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osError;
    }
}

osStatus_t osDelayUntil(uint64_t ticks)
{
    UINT32 ret;
    UINT64 ticksGap;
    UINT64 tickCount;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    tickCount = LOS_TickCountGet();
    if ((ticks < tickCount)) {
        return osError;
    }

    ticksGap = ticks - tickCount;
    /* get the high 32 bits */
    if ((ticksGap >> 32) > 0) {
        return osError;
    }

    ret = LOS_TaskDelay((UINT32)ticksGap);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osError;
    }
}


//  ==== Timer Management Functions ====
#ifdef LOSCFG_BASE_CORE_SWTMR

osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    (VOID)attr;
    UINT32 ret;
    UINT16 swtmrId;
    UINT8 mode;

    if ((func == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }

    if (type == osTimerOnce) {
        mode = LOS_SWTMR_MODE_NO_SELFDELETE;
    } else if (type == osTimerPeriodic) {
        mode = LOS_SWTMR_MODE_PERIOD;
    } else {
        return NULL;
    }

    ret = LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)func, &swtmrId, (UINTPTR)argument);
    if (ret == LOS_OK) {
        return (osTimerId_t)OS_SWT_FROM_SID(swtmrId);
    } else {
        return NULL;
    }
}

const char *osTimerGetName(osTimerId_t timer_id)
{
    (VOID)timer_id;
    return NULL;
}

osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
    LosSwtmrCB *swtmrCB = (LosSwtmrCB *)timer_id;
    UINT32 ret;

    if ((swtmrCB == NULL) || (ticks == 0)) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    swtmrCB->expiry = ticks;
    swtmrCB->interval = ticks;
    ret = LOS_SwtmrStart(swtmrCB->timerId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osStatus_t osTimerStop(osTimerId_t timer_id)
{
    LosSwtmrCB *swtmrCB = (LosSwtmrCB *)timer_id;
    UINT32 ret;

    if (swtmrCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_SwtmrStop(swtmrCB->timerId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

uint32_t osTimerIsRunning(osTimerId_t timer_id)
{
    LosSwtmrCB *swtmrCB = (LosSwtmrCB *)timer_id;

    if ((swtmrCB == NULL) || OS_INT_ACTIVE) {
        return 0;
    }

    return (swtmrCB->state == OS_SWTMR_STATUS_TICKING);
}

osStatus_t osTimerDelete(osTimerId_t timer_id)
{
    LosSwtmrCB *swtmrCB = (LosSwtmrCB *)timer_id;
    UINT32 ret;

    if (swtmrCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_SwtmrDelete(swtmrCB->timerId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

#endif /* LOSCFG_BASE_CORE_SWTMR */


//  ==== Mutex Management Functions ====
#ifdef LOSCFG_BASE_IPC_MUX

osMutexId_t osMutexNew(const osMutexAttr_t *attr)
{
    (VOID)attr;
    UINT32 ret;
    UINT32 muxId;

    if (OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_MuxCreate(&muxId);
    if (ret == LOS_OK) {
        return (osMutexId_t)GET_MUX(muxId);
    } else {
        return NULL;
    }
}

const char *osMutexGetName(osMutexId_t mutex_id)
{
    (VOID)mutex_id;
    return NULL;
}

osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout)
{
    LosMuxCB *muxCB = (LosMuxCB *)mutex_id;
    UINT32 ret;

    if (muxCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_MuxPend(muxCB->muxId, timeout);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        return osErrorTimeout;
    } else {
        return osErrorResource;
    }
}

osStatus_t osMutexRelease(osMutexId_t mutex_id)
{
    LosMuxCB *muxCB = (LosMuxCB *)mutex_id;
    UINT32 ret;

    if (muxCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_MuxPost(muxCB->muxId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osThreadId_t osMutexGetOwner(osMutexId_t mutex_id)
{
    LosMuxCB *muxCB = (LosMuxCB *)mutex_id;
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;

    if ((muxCB == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }

    intSave = LOS_IntLock();
    taskCB = muxCB->owner;
    LOS_IntRestore(intSave);

    return (osThreadId_t)taskCB;
}

osStatus_t osMutexDelete(osMutexId_t mutex_id)
{
    LosMuxCB *muxCB = (LosMuxCB *)mutex_id;
    UINT32 ret;

    if (muxCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_MuxDelete(muxCB->muxId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

#endif /* LOSCFG_BASE_IPC_MUX */


//  ==== Semaphore Management Functions ====
#ifdef LOSCFG_BASE_IPC_SEM

osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
    (VOID)attr;
    UINT32 ret;
    UINT32 semId;

    if ((initial_count > max_count) || (max_count > OS_SEM_COUNT_MAX) || OS_INT_ACTIVE) {
        return NULL;
    }

    if (max_count == 1) {
        ret = LOS_BinarySemCreate((UINT16)initial_count, &semId);
    } else {
        ret = LOS_SemCreate((UINT16)initial_count, &semId);
    }
    if (ret == LOS_OK) {
        return (osSemaphoreId_t)GET_SEM(semId);
    } else {
        return NULL;
    }
}

const char *osSemaphoreGetName(osSemaphoreId_t semaphore_id)
{
    (VOID)semaphore_id;
    return NULL;
}

osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    LosSemCB *semCB = (LosSemCB *)semaphore_id;
    UINT32 ret;

    if ((semCB == NULL) || (OS_INT_ACTIVE && (timeout != 0))) {
        return osErrorParameter;
    }

    ret = LOS_SemPend(semCB->semId, timeout);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        return osErrorTimeout;
    } else {
        return osErrorResource;
    }
}

osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
{
    LosSemCB *semCB = (LosSemCB *)semaphore_id;
    UINT32 ret;

    if (semCB == NULL) {
        return osErrorParameter;
    }

    ret = LOS_SemPost(semCB->semId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id)
{
    LosSemCB *semCB = (LosSemCB *)semaphore_id;
    UINT32 intSave;
    UINT16 count;

    if (semCB == NULL) {
        return 0;
    }

    intSave = LOS_IntLock();
    if (semCB->semStat == OS_SEM_UNUSED) {
        LOS_IntRestore(intSave);
        return 0;
    }

    count = semCB->semCount;
    LOS_IntRestore(intSave);

    return (uint32_t)count;
}

osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphore_id)
{
    LosSemCB *semCB = (LosSemCB *)semaphore_id;
    UINT32 ret;

    if (semCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_SemDelete(semCB->semId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

#endif /* LOSCFG_BASE_IPC_SEM */


//  ==== Message Queue Management Functions ====
#ifdef LOSCFG_BASE_IPC_QUEUE

typedef enum {
    ATTR_CAPACITY = 0,
    ATTR_MSGSIZE = 1,
    ATTR_COUNT = 2,
    ATTR_SPACE = 3
} QueueAttribute;

osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{
    (VOID)attr;
    UINT32 ret;
    UINT32 queueId;

    if ((msg_count == 0) || (msg_size == 0) || OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_QueueCreate(NULL, (UINT16)msg_count, &queueId, 0, (UINT16)msg_size);
    if (ret == LOS_OK) {
        return (osMessageQueueId_t)GET_QUEUE_HANDLE(queueId);
    } else {
        return NULL;
    }
}

const char *osMessageQueueGetName(osMessageQueueId_t mq_id)
{
    (VOID)mq_id;
    return NULL;
}

STATIC osStatus_t osMessageQueueOp(osMessageQueueId_t mq_id, VOID *msg_ptr, UINT32 timeout, QueueReadWrite rw)
{
    LosQueueCB *queueCB = (LosQueueCB *)mq_id;
    UINT32 ret;
    UINT32 bufferSize;

    if ((queueCB == NULL) || (msg_ptr == NULL) || (OS_INT_ACTIVE && (timeout != 0))) {
        return osErrorParameter;
    }

    bufferSize = (UINT32)(queueCB->queueSize - sizeof(UINT32));
    if (rw == OS_QUEUE_WRITE) {
        ret = LOS_QueueWriteCopy(queueCB->queueId, msg_ptr, bufferSize, timeout);
    } else {
        ret = LOS_QueueReadCopy(queueCB->queueId, msg_ptr, &bufferSize, timeout);
    }

    if (ret == LOS_OK) {
        return osOK;
    } else if ((ret == LOS_ERRNO_QUEUE_INVALID) || (ret == LOS_ERRNO_QUEUE_NOT_CREATE)) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_QUEUE_TIMEOUT) {
        return osErrorTimeout;
    } else {
        return osErrorResource;
    }
}

osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    (VOID)msg_prio;
    return osMessageQueueOp(mq_id, (VOID *)msg_ptr, (UINT32)timeout, OS_QUEUE_WRITE);
}

osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    (VOID)msg_prio;
    return osMessageQueueOp(mq_id, (VOID *)msg_ptr, (UINT32)timeout, OS_QUEUE_READ);
}

STATIC UINT16 osMessageQueueGetAttr(osMessageQueueId_t mq_id, QueueAttribute attr)
{
    LosQueueCB *queueCB = (LosQueueCB *)mq_id;
    UINT32 intSave;
    UINT16 attrVal = 0;

    if (queueCB == NULL) {
        return 0;
    }

    intSave = LOS_IntLock();
    if (queueCB->queueState == OS_QUEUE_UNUSED) {
        LOS_IntRestore(intSave);
        return 0;
    }

    switch (attr) {
        case ATTR_CAPACITY:
            attrVal = queueCB->queueLen;
            break;
        case ATTR_MSGSIZE:
            attrVal = queueCB->queueSize - sizeof(UINT32);
            break;
        case ATTR_COUNT:
            attrVal = queueCB->readWriteableCnt[OS_QUEUE_READ];
            break;
        case ATTR_SPACE:
            attrVal = queueCB->readWriteableCnt[OS_QUEUE_WRITE];
            break;
        default:
            break;
    }
    LOS_IntRestore(intSave);

    return attrVal;
}

uint32_t osMessageQueueGetCapacity(osMessageQueueId_t mq_id)
{
    return (uint32_t)osMessageQueueGetAttr(mq_id, ATTR_CAPACITY);
}

uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mq_id)
{
    return (uint32_t)osMessageQueueGetAttr(mq_id, ATTR_MSGSIZE);
}

uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id)
{
    return (uint32_t)osMessageQueueGetAttr(mq_id, ATTR_COUNT);
}

uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id)
{
    return (uint32_t)osMessageQueueGetAttr(mq_id, ATTR_SPACE);
}

osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id)
{
    LosQueueCB *queueCB = (LosQueueCB *)mq_id;
    UINT32 ret;

    if (queueCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_QueueDelete(queueCB->queueId);
    if (ret == LOS_OK) {
        return osOK;
    } else if ((ret == LOS_ERRNO_QUEUE_NOT_FOUND) || (ret == LOS_ERRNO_QUEUE_NOT_CREATE)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

#endif /* LOSCFG_BASE_IPC_QUEUE */


#endif // (CMSIS_OS_VER == 2)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
