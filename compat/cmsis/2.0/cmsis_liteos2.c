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
#include "cmsis_os.h"
#include "los_config.h"
#include "string.h"
#include "securec.h"
#include "los_typedef.h"
#include "los_printf.h"
#include "los_event.h"
#include "los_membox.h"
#include "los_hwi.h"

#include "los_mux_pri.h"
#include "los_queue_pri.h"
#include "los_sem_pri.h"
#include "los_swtmr_pri.h"
#include "los_sys_pri.h"
#include "los_task_pri.h"
#include "los_tick_pri.h"
#include "los_sched_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#if (LOSCFG_CMSIS_VERSION == 2)

/* Kernel initialization state */
static osKernelState_t g_kernelState;

#define LOS_PRIORITY_WIN 8

static const osVersion_t g_losVersion = {001, 001};

#define LITEOS_VERSION_MAJOR    1
#define LITEOS_VERSION_MINOR    0
#define LITEOS_VERSION_BUILD    0

/* Kernel version and identification string definition */
#define KERNEL_VERSION            (((UINT32)LITEOS_VERSION_MAJOR * 10000000UL) | \
                                   ((UINT32)LITEOS_VERSION_MINOR *    10000UL) | \
                                   ((UINT32)LITEOS_VERSION_BUILD *        1UL))

#define KERNEL_ID   "HUAWEI-LiteOS"
#define UNUSED(var) do { \
    (void)var;           \
} while (0)

#define TASK_UNLOCK 0
#define TASK_LOCK 1

/* Kernel Management Functions */
uint32_t osTaskStackWaterMarkGet(uint32_t taskID);


osStatus_t osKernelInitialize (void)
{
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (g_kernelState != osKernelInactive) {
        return osError;
    }

    if (LOS_OK == OsMain()) {
        g_kernelState = osKernelReady;
        return osOK;
    } else {
        return osError;
    }
}


osStatus_t osKernelGetInfo(osVersion_t *version, char *idBuf, uint32_t idSize)
{
    uint32_t ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (version != NULL) {
        version->api = g_losVersion.api;
        version->kernel = g_losVersion.kernel;
    }

    if ((idBuf != NULL) && (idSize != 0U)) {
        if (idSize > (strlen(KERNEL_ID) + 1)) {
            idSize = strlen(KERNEL_ID) + 1;
        }
        ret = memcpy_s(idBuf, idSize, KERNEL_ID, idSize);
        if (ret != EOK) {
            PRINT_ERR("%s[%d] memcpy failed, error type = %u\n", __FUNCTION__, __LINE__, ret);
            return osError;
        }
    }

    return osOK;
}


osKernelState_t osKernelGetState (void)
{
    uint32_t losTaskLock = OsPercpuGet()->taskLockCnt;;

    if (OS_INT_ACTIVE) {
        return osKernelError;
    }

    if (!g_taskScheduled) {
        if (g_kernelState == osKernelReady) {
            return osKernelReady;
        } else {
            return osKernelInactive;
        }
    } else if (losTaskLock > 0) {
        return osKernelLocked;
    } else {
        return osKernelRunning;
    }
}

osStatus_t osKernelStart (void)
{
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (g_kernelState == osKernelReady) {
        if (LOS_Start() == LOS_OK) {
            g_kernelState = osKernelRunning;
            return osOK;
        } else {
            return osError;
        }
    } else {
        return osError;
    }
}


int32_t osKernelLock(void)
{
    int32_t lock;
    uint32_t losTaskLock = OsPercpuGet()->taskLockCnt;;
    uint32_t cpuID = ArchCurrCpuid();

    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }

    if (!(g_taskScheduled & (1 << cpuID))) {
        return (int32_t)osError;
    }

    if (losTaskLock > 0) {
        lock = 1;
    } else {
        LOS_TaskLock();
        lock = 0;
    }

    return lock;
}


int32_t osKernelUnlock(void)
{
    int32_t lock;
    uint32_t losTaskLock = OsPercpuGet()->taskLockCnt;
    uint32_t cpuID = ArchCurrCpuid();

    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }

    if (!(g_taskScheduled & (1 << cpuID))) {
        return (int32_t)osError;
    }

    if (losTaskLock > 0) {
        LOS_TaskUnlock();
        if (losTaskLock != 0) {
            return (int32_t)osError;
        }
        lock = 1;
    } else {
        lock = 0;
    }

    return lock;
}


int32_t osKernelRestoreLock(int32_t lock)
{
    uint32_t losTaskLock = OsPercpuGet()->taskLockCnt;
    uint32_t cpuID = ArchCurrCpuid();

    if (OS_INT_ACTIVE) {
        return (int32_t)osErrorISR;
    }
    if (!(g_taskScheduled & (1 << cpuID))) {
        return (int32_t)osError;
    }

    switch (lock) {
        case TASK_UNLOCK:
            LOS_TaskUnlock();
            if (losTaskLock != 0) {
                break;
            }
            return 0;
        case TASK_LOCK:
            LOS_TaskLock();
            return 1;
        default:
            break;
    }

    return (int32_t)osError;
}


UINT64 osKernelGetTickCount(void)
{
    UINT64 ticks;
    UINTPTR intSave;

    if (OS_INT_ACTIVE) {
        ticks = 0U;
    } else {
        intSave = LOS_IntLock();
        ticks = g_tickCount[0];
        LOS_IntRestore(intSave);
    }

    return ticks;
}


UINT64 osKernelGetTick2ms(void)
{
    return osKernelGetTickCount() * (OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND);
}


UINT64 osMs2Tick(UINT64 ms)
{
    return ms * (LOSCFG_BASE_CORE_TICK_PER_SECOND / OS_SYS_MS_PER_SECOND);
}


uint32_t osKernelGetTickFreq(void)
{
    uint32_t freq;

    if (OS_INT_ACTIVE) {
        freq = 0U;
    } else {
        freq = LOSCFG_BASE_CORE_TICK_PER_SECOND;
    }

    return (freq);
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_GetSystickCycle(uint32_t *highCnt, uint32_t *lowCnt);
uint32_t osKernelGetSysTimerCount (void)
{
    uint32_t countHigh = 0;
    uint32_t countLow = 0;

    if (OS_INT_ACTIVE) {
        countLow = 0U;
    } else {
        LOS_GetSystickCycle((uint32_t *)&countHigh, (uint32_t *)&countLow);
    }
    return countLow;
}


uint32_t osKernelGetSysTimerFreq(void)
{
    return OS_SYS_CLOCK;
}

/* Thread Management Functions */
osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    UNUSED(argument);

    uint32_t tid;
    uint32_t ret;
    LosTaskCB *taskCB = NULL;
    TSK_INIT_PARAM_S tskInitParam;

    if (OS_INT_ACTIVE) {
        return NULL;
    }

    if ((attr == NULL) || (func == NULL)) {
        return (osThreadId_t)NULL;
    }
    if ((attr->priority < osPriorityLow1) || (attr->priority > osPriorityAboveNormal6)) {
        return (osThreadId_t)NULL;
    }

    /* Ignore the return code when matching CSEC rule 6.6(4). */
    (void)memset_s(&tskInitParam, sizeof(tskInitParam), 0, sizeof(tskInitParam));
    tskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)func;
    tskInitParam.uwStackSize = attr->stack_size * sizeof(uint32_t);
    tskInitParam.pcName = (CHAR *)attr->name;
    /* task priority: 0~31 */
    tskInitParam.usTaskPrio = OS_TASK_PRIORITY_LOWEST - ((UINT16)(attr->priority) - LOS_PRIORITY_WIN);

    ret = LOS_TaskCreate(&tid, &tskInitParam);
    if (ret != LOS_OK) {
        return (osThreadId_t)NULL;
    }
    taskCB = OS_TCB_FROM_TID(tid);

    return (osThreadId_t)taskCB;
}

const char *osThreadGetName(osThreadId_t threadID)
{
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE || (threadID == NULL)) {
        return NULL;
    }

    taskCB = (LosTaskCB *)threadID;

    return taskCB->taskName;
}


osThreadId_t osThreadGetId (void)
{
    if (OS_INT_ACTIVE) {
        return NULL;
    }

    return (osThreadId_t)(g_runTask);
}


osThreadState_t osThreadGetState(osThreadId_t threadID)
{
    UINT16 taskStatus;
    osThreadState_t state;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE || (threadID == NULL)) {
        return osThreadError;
    }

    taskCB = (LosTaskCB *)threadID;
    taskStatus = taskCB->taskStatus;
    if (taskStatus & OS_TASK_STATUS_RUNNING) {
        state = osThreadRunning;
    } else if (taskStatus & OS_TASK_STATUS_READY) {
        state = osThreadReady;
    } else if (taskStatus &
        (OS_TASK_STATUS_DELAY | OS_TASK_STATUS_PEND |
         OS_TASK_STATUS_SUSPEND)) {
        state = osThreadBlocked;
    } else if (taskStatus & OS_TASK_STATUS_UNUSED) {
        state = osThreadInactive;
    } else {
        state = osThreadError;
    }

    return state;
}


uint32_t osThreadGetStackSize(osThreadId_t threadID)
{
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE || (threadID == NULL)) {
        return 0U;
    }

    taskCB = (LosTaskCB *)threadID;

    return taskCB->stackSize;
}


uint32_t osTaskStackWaterMarkGet(uint32_t taskID)
{
    uint32_t count = 0;
    uint32_t *topOfStack = NULL;
    UINTPTR intSave;
    LosTaskCB *taskCB = NULL;

    if (taskID > LOSCFG_BASE_CORE_TSK_LIMIT) {
        return 0;
    }

    intSave = LOS_IntLock();

    taskCB = OS_TCB_FROM_TID(taskID);
    if ((taskCB->taskStatus) & OS_TASK_STATUS_UNUSED) {
        LOS_IntRestore(intSave);
        return 0;
    }

    /* first 4 bytes is OS_TASK_MAGIC_WORD, skip */
    topOfStack = (uint32_t *)(UINTPTR)taskCB->topOfStack + 1;
    while (*topOfStack == (uint32_t)OS_STACK_INIT) {
        ++topOfStack;
        ++count;
    }

    count *= sizeof(uint32_t);
    LOS_IntRestore(intSave);

    return count;
}


uint32_t osThreadGetStackSpace(osThreadId_t threadID)
{
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE || (threadID == NULL)) {
        return 0U;
    }

    taskCB = (LosTaskCB *)threadID;

    return osTaskStackWaterMarkGet(taskCB->taskID);
}


osStatus_t osThreadSetPriority(osThreadId_t threadID, osPriority_t priority)
{
    uint32_t ret;
    UINT16 priorityTemp;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (threadID == NULL) {
        return osErrorParameter;
    }

    if ((priority < osPriorityLow1) || (priority > osPriorityAboveNormal6)) {
        return osErrorParameter;
    }

    taskCB = (LosTaskCB *)threadID;
    priorityTemp = OS_TASK_PRIORITY_LOWEST - ((UINT16)priority - LOS_PRIORITY_WIN);
    ret = LOS_TaskPriSet(taskCB->taskID, priorityTemp);
    switch (ret) {
        case LOS_ERRNO_TSK_PRIOR_ERROR:
            /* fall-through */
        case LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK:
            /* fall-through */
        case LOS_ERRNO_TSK_ID_INVALID:
            return osErrorParameter;
        case LOS_ERRNO_TSK_NOT_CREATED:
            return osErrorResource;
        default:
            return osOK;
    }
}


osPriority_t osThreadGetPriority(osThreadId_t threadID)
{
    UINT16 ret;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE || (threadID == NULL)) {
        return osPriorityError;
    }

    taskCB = (LosTaskCB *)threadID;
    ret = LOS_TaskPriGet(taskCB->taskID);
    if (ret == (UINT16)OS_INVALID) {
        return osPriorityError;
    }

    return (osPriority_t)(OS_TASK_PRIORITY_LOWEST - (ret - LOS_PRIORITY_WIN));
}


osStatus_t osThreadYield (void)
{
    uint32_t ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskYield();
    if (ret == LOS_OK) {
        return osOK;
    }

    return osError;
}


osStatus_t osThreadSuspend(osThreadId_t threadID)
{
    uint32_t ret;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (threadID == NULL) {
        return osErrorParameter;
    }

    taskCB = (LosTaskCB *)threadID;
    ret = LOS_TaskSuspend(taskCB->taskID);
    switch (ret) {
        case LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK:
            /* fall-through */
        case LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED:
            /* fall-through */
        case LOS_ERRNO_TSK_ID_INVALID:
            return osErrorParameter;

        case LOS_ERRNO_TSK_NOT_CREATED:
            /* fall-through */
        case LOS_ERRNO_TSK_ALREADY_SUSPENDED:
            /* fall-through */
        case LOS_ERRNO_TSK_SUSPEND_LOCKED:
            return osErrorResource;
        default:
            return osOK;
    }
}

osStatus_t osThreadResume(osThreadId_t threadID)
{
    uint32_t ret;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (threadID == NULL) {
        return osErrorParameter;
    }

    taskCB = (LosTaskCB *)threadID;
    ret = LOS_TaskResume(taskCB->taskID);
    switch (ret) {
        case LOS_ERRNO_TSK_ID_INVALID:
            return osErrorParameter;
        case LOS_ERRNO_TSK_NOT_CREATED:
            /* fall-through */
        case LOS_ERRNO_TSK_NOT_SUSPENDED:
            return osErrorResource;
        default:
            return osOK;
    }
}

osStatus_t osThreadTerminate(osThreadId_t threadID)
{
    uint32_t ret;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (threadID == NULL) {
        return osErrorParameter;
    }

    taskCB = (LosTaskCB *)threadID;
    ret = LOS_TaskDelete(taskCB->taskID);
    switch (ret) {
        case LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK:
            /* fall-through */
        case LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED:
            /* fall-through */
        case LOS_ERRNO_TSK_ID_INVALID:
            return osErrorParameter;
        case LOS_ERRNO_TSK_NOT_CREATED:
            return osErrorResource;
        default:
            return osOK;
    }
}


uint32_t osThreadGetCount(void)
{
    uint32_t count = 0;
    int index;

    if (OS_INT_ACTIVE) {
        return 0U;
    }

    for (index = 0; index <= LOSCFG_BASE_CORE_TSK_LIMIT; index++) {
        if (!((g_taskCBArray + index)->taskStatus & OS_TASK_STATUS_UNUSED)) {
            count++;
        }
    }

    return count;
}

/* Generic Wait Functions */
osStatus_t osDelay(uint32_t ticks)
{
    uint32_t ret;

    ret = LOS_TaskDelay(ticks);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osError;
    }
}

osStatus_t osDelayUntil(UINT64 ticks)
{
    uint32_t ret;
    uint32_t ticksTemp;
    UINT64 tickCount = osKernelGetTickCount();
    if (ticks < tickCount) {
        return osError;
    }

    ticksTemp = (uint32_t)(ticks - tickCount);
    ret = LOS_TaskDelay(ticksTemp);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osError;
    }
}

/* Timer Management Functions */
#if (LOSCFG_BASE_CORE_SWTMR == YES)
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
osTimerId_t osTimerExtNew(osTimerFunc_t func,
                          osTimerType_t type,
                          void *argument,
                          const osTimerAttr_t *attr,
                          os_timer_rouses_type rouses,
                          os_timer_align_type sensitive)
{
    UNUSED(attr);
    UINT16 swtmrID;
    UINT8 mode;

    if (OS_INT_ACTIVE || (func == NULL) ||
        ((type != osTimerOnce) && (type != osTimerPeriodic))) {
        return (osTimerId_t)NULL;
    }

    if (type == osTimerOnce) {
        mode = LOS_SWTMR_MODE_NO_SELFDELETE;
    } else {
        mode = LOS_SWTMR_MODE_PERIOD;
    }
    if (LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)func,
                        &swtmrID, (uint32_t)(UINTPTR)argument,
                        rouses, sensitive) != LOS_OK) {
        return (osTimerId_t)NULL;
    }

    return (osTimerId_t)OS_SWT_FROM_SID(swtmrID);
}
#endif
osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    UNUSED(attr);
    UINT16 swtmrID;
    UINT8 mode;

    if (OS_INT_ACTIVE || (func == NULL) ||
        ((type != osTimerOnce) && (type != osTimerPeriodic))) {
        return (osTimerId_t)NULL;
    }

    if (type == osTimerOnce) {
        mode = LOS_SWTMR_MODE_NO_SELFDELETE;
    } else {
        mode = LOS_SWTMR_MODE_PERIOD;
    }
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    if (LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)func,
                        &swtmrID, (uint32_t)(UINTPTR)argument,
                        OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE) != LOS_OK) {
        return (osTimerId_t)NULL;
    }
#else
    if (LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)func,
                        &swtmrID, (uint32_t)(UINTPTR)argument) != LOS_OK) {
        return (osTimerId_t)NULL;
    }
#endif

    return (osTimerId_t)OS_SWT_FROM_SID(swtmrID);
}


osStatus_t osTimerStart(osTimerId_t timerID, uint32_t ticks)
{
    uint32_t ret;
    SWTMR_CTRL_S *swtmr = NULL;

    if ((ticks == 0) || (timerID == NULL)) {
        return osErrorParameter;
    }

    swtmr = (SWTMR_CTRL_S *)timerID;
    swtmr->uwInterval = LOS_Tick2MS(ticks);
    ret = LOS_SwtmrStart(swtmr->usTimerID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}


const char *osTimerGetName(osTimerId_t timerID)
{
    UNUSED(timerID);
    return (const char *)NULL;
}


osStatus_t osTimerStop(osTimerId_t timerID)
{
    uint32_t ret;
    SWTMR_CTRL_S *swtmr = (SWTMR_CTRL_S *)timerID;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (swtmr == NULL) {
        return osErrorParameter;
    }

    ret = LOS_SwtmrStop(swtmr->usTimerID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}


uint32_t osTimerIsRunning(osTimerId_t timerID)
{
    if (OS_INT_ACTIVE || (timerID == NULL)) {
        return 0;
    }

    return (((SWTMR_CTRL_S *)timerID)->ucState == OS_SWTMR_STATUS_TICKING);
}

osStatus_t osTimerDelete(osTimerId_t timerID)
{
    uint32_t ret;
    SWTMR_CTRL_S *swtmr = (SWTMR_CTRL_S *)timerID;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (swtmr == NULL) {
        return osErrorParameter;
    }

    ret = LOS_SwtmrDelete(swtmr->usTimerID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}
#endif

osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr)
{
    PEVENT_CB_S eventCB;
    uint32_t ret;

    UNUSED(attr);

    if (OS_INT_ACTIVE) {
        return (osEventFlagsId_t)NULL;
    }

    eventCB = (PEVENT_CB_S)LOS_MemAlloc(m_aucSysMem0, sizeof(EVENT_CB_S));
    if (eventCB == NULL) {
        return (osEventFlagsId_t)NULL;
    }

    ret = LOS_EventInit(eventCB);
    if (ret == LOS_ERRNO_EVENT_PTR_NULL) {
        return (osEventFlagsId_t)NULL;
    } else {
        return (osEventFlagsId_t)eventCB;
    }
}


const char *osEventFlagsGetName(osEventFlagsId_t efID)
{
    UNUSED(efID);

    if (OS_INT_ACTIVE) {
        return (const char *)NULL;
    }

    return (const char *)NULL;
}


uint32_t osEventFlagsSet(osEventFlagsId_t efID, uint32_t flags)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)efID;
    uint32_t ret;
    uint32_t rflags;

    ret = LOS_EventWrite(eventCB, (uint32_t)flags);
    if (ret != LOS_OK) {
        return (uint32_t)osFlagsErrorParameter;
    } else {
        rflags = eventCB->uwEventID;
        return rflags;
    }
}


uint32_t osEventFlagsClear(osEventFlagsId_t efID, uint32_t flags)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)efID;
    UINTPTR intSave;
    uint32_t rflags;
    uint32_t ret;

    if (eventCB == NULL) {
        return (uint32_t)osFlagsErrorParameter;
    }

    intSave = LOS_IntLock();
    rflags = eventCB->uwEventID;

    ret = LOS_EventClear(eventCB, ~flags);
    LOS_IntRestore(intSave);
    if (ret != LOS_OK) {
        return (uint32_t)osFlagsErrorParameter;
    } else {
        return rflags;
    }
}

uint32_t osEventFlagsGet(osEventFlagsId_t efID)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)efID;
    UINTPTR intSave;
    uint32_t rflags;

    if (eventCB == NULL) {
        return (uint32_t)osFlagsErrorParameter;
    }

    intSave = LOS_IntLock();
    rflags = eventCB->uwEventID;
    LOS_IntRestore(intSave);

    return rflags;
}

uint32_t osEventFlagsWait(osEventFlagsId_t efID, uint32_t flags, uint32_t options, uint32_t timeout)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)efID;
    uint32_t mode = 0;
    uint32_t ret;
    uint32_t rflags;

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

    ret = LOS_EventRead(eventCB, (uint32_t)flags, mode, (uint32_t)timeout);
    switch (ret) {
        case LOS_ERRNO_EVENT_PTR_NULL:
            /* fall-through */
        case LOS_ERRNO_EVENT_EVENTMASK_INVALID:
            /* fall-through */
        case LOS_ERRNO_EVENT_SETBIT_INVALID:
            return (uint32_t)osFlagsErrorParameter;
        case LOS_ERRNO_EVENT_READ_IN_INTERRUPT:
            /* fall-through */
        case LOS_ERRNO_EVENT_FLAGS_INVALID:
            /* fall-through */
        case LOS_ERRNO_EVENT_READ_IN_LOCK:
            return (uint32_t)osFlagsErrorResource;
        case LOS_ERRNO_EVENT_READ_TIMEOUT:
            return (uint32_t)osFlagsErrorTimeout;
        default:
            rflags = (uint32_t)ret;
            return rflags;
    }
}

osStatus_t osEventFlagsDelete(osEventFlagsId_t efID)
{
    PEVENT_CB_S eventCB = (PEVENT_CB_S)efID;
    UINTPTR intSave;
    osStatus_t ret;

    intSave = LOS_IntLock();
    if (LOS_EventDestroy(eventCB) == LOS_OK) {
        ret = osOK;
    } else {
        ret = osErrorParameter;
    }
    LOS_IntRestore(intSave);

    if (LOS_MemFree(m_aucSysMem0, (void *)eventCB) == LOS_OK) {
        ret = osOK;
    } else {
        ret = osErrorParameter;
    }

    return ret;
}

/* Mutex Management Functions */
#if (LOSCFG_BASE_IPC_MUX == YES)
osMutexId_t osMutexNew (const osMutexAttr_t *attr)
{
    uint32_t ret;
    uint32_t muxID;

    UNUSED(attr);

    if (OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_MuxCreate(&muxID);
    if (ret == LOS_OK) {
        return (osMutexId_t)(GET_MUX(muxID));
    } else {
        return (osMutexId_t)NULL;
    }
}

osStatus_t osMutexAcquire(osMutexId_t mutexID, uint32_t timeout)
{
    uint32_t ret;

    if (mutexID == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE && (timeout != LOS_NO_WAIT)) {
        timeout = 0;
    }

    ret = LOS_MuxPend(((LosMuxCB *)mutexID)->muxID, timeout);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        return osErrorTimeout;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osStatus_t osMutexRelease(osMutexId_t mutexID)
{
    uint32_t ret;

    if (mutexID == NULL) {
        return osErrorParameter;
    }

    ret = LOS_MuxPost(((LosMuxCB *)mutexID)->muxID);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorResource;
    }
}

osThreadId_t osMutexGetOwner(osMutexId_t mutexID)
{
    uint32_t intSave;
    LosTaskCB *taskCB = NULL;

    if (OS_INT_ACTIVE) {
        return NULL;
    }

    if (mutexID == NULL) {
        return NULL;
    }

    intSave = LOS_IntLock();
    taskCB = ((LosMuxCB *)mutexID)->owner;
    LOS_IntRestore(intSave);

    return (osThreadId_t)taskCB;
}

osStatus_t osMutexDelete(osMutexId_t mutexID)
{
    uint32_t ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (mutexID == NULL) {
        return osErrorParameter;
    }

    ret = LOS_MuxDelete(((LosMuxCB *)mutexID)->muxID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}
#endif

/* Semaphore Management Functions */
#if (LOSCFG_BASE_IPC_SEM == YES)

osSemaphoreId_t osSemaphoreNew(uint32_t maxCount, uint32_t initialCount, const osSemaphoreAttr_t *attr)
{
    uint32_t ret;
    uint32_t semID;

    UNUSED(attr);

    if (OS_INT_ACTIVE) {
        return (osSemaphoreId_t)NULL;
    }

    if (maxCount == 1) {
        ret = LOS_BinarySemCreate((UINT16)initialCount, &semID);
    } else {
        ret = LOS_SemCreate((UINT16)initialCount, &semID);
    }
    if (ret == LOS_OK) {
        return (osSemaphoreId_t)(GET_SEM(semID));
    } else {
        return (osSemaphoreId_t)NULL;
    }
}

osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphoreID, uint32_t timeout)
{
    uint32_t ret;

    if (semaphoreID == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE && (timeout != LOS_NO_WAIT)) {
        return osErrorISR;
    }

    ret = LOS_SemPend(((LosSemCB *)semaphoreID)->semID, timeout);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        return osErrorTimeout;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_SEM_PEND_INTERR) {
        return osErrorISR;
    } else {
        return osErrorResource;
    }
}

osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphoreID)
{
    uint32_t ret;

    if (semaphoreID == NULL) {
        return osErrorParameter;
    }

    ret = LOS_SemPost(((LosSemCB *)semaphoreID)->semID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphoreID)
{
    uint32_t intSave;
    uint32_t count;

    if (OS_INT_ACTIVE) {
        return 0;
    }

    if (semaphoreID == NULL) {
        return 0;
    }

    intSave = LOS_IntLock();
    count = ((LosSemCB *)semaphoreID)->semCount;
    LOS_IntRestore(intSave);

    return count;
}


osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphoreID)
{
    uint32_t ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (semaphoreID == NULL) {
        return osErrorParameter;
    }

    ret = LOS_SemDelete(((LosSemCB *)semaphoreID)->semID);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}
#endif

/* Message Queue Management Functions */
#if (LOSCFG_BASE_IPC_QUEUE == YES)
osMessageQueueId_t osMessageQueueNew(uint32_t msgCount, uint32_t msgSize, const osMessageQueueAttr_t *attr)
{
    uint32_t queueID;
    uint32_t ret;
    UNUSED(attr);
    osMessageQueueId_t handle;

    if ((msgCount == 0) || (msgSize == 0) || OS_INT_ACTIVE) {
        return (osMessageQueueId_t)NULL;
    }

    ret = LOS_QueueCreate((char *)NULL, (UINT16)msgCount, &queueID, 0, (UINT16)msgSize);
    if (ret == LOS_OK) {
        handle = (osMessageQueueId_t)(GET_QUEUE_HANDLE(queueID));
    } else {
        handle = (osMessageQueueId_t)NULL;
    }

    return handle;
}

osStatus_t osMessageQueuePut(osMessageQueueId_t mqID, const void *msgPtr, uint8_t msgPrio, uint32_t timeout)
{
    UNUSED(msgPrio);
    uint32_t ret;
    uint32_t bufferSize;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if ((queueCB == NULL) || (msgPtr == NULL) || (OS_INT_ACTIVE && (timeout != 0))) {
        return osErrorParameter;
    }

    bufferSize = (uint32_t)(queueCB->queueSize - sizeof(uint32_t));
    ret = LOS_QueueWriteCopy((uint32_t)queueCB->queueID, (void *)msgPtr, bufferSize, timeout);
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

osStatus_t osMessageQueueGet(osMessageQueueId_t mqID, void *msgPtr, uint8_t *msgPrio, uint32_t timeout)
{
    UNUSED(msgPrio);
    uint32_t ret;
    uint32_t bufferSize;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if ((queueCB == NULL) || (msgPtr == NULL) || (OS_INT_ACTIVE && (timeout != 0))) {
        return osErrorParameter;
    }

    bufferSize = (uint32_t)(queueCB->queueSize - sizeof(uint32_t));
    ret = LOS_QueueReadCopy((uint32_t)queueCB->queueID, msgPtr, &bufferSize, timeout);
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

uint32_t osMessageQueueGetCapacity(osMessageQueueId_t mqID)
{
    uint32_t capacity;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if (queueCB == NULL) {
        capacity = 0U;
    } else {
        capacity = queueCB->queueLen;
    }

    return (capacity);
}

uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mqID)
{
    uint32_t size;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if (queueCB == NULL) {
        size = 0U;
    } else {
        size = queueCB->queueSize - sizeof(uint32_t);
    }

    return size;
}

uint32_t osMessageQueueGetCount(osMessageQueueId_t mqID)
{
    uint32_t count;
    UINTPTR intSave;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if (queueCB == NULL) {
        count = 0U;
    } else {
        intSave = LOS_IntLock();
        count = (uint32_t)(queueCB->readWriteableCnt[OS_QUEUE_READ]);
        LOS_IntRestore(intSave);
    }
    return count;
}


uint32_t osMessageQueueGetSpace(osMessageQueueId_t mqID)
{
    uint32_t space;
    UINTPTR intSave;
    LosQueueCB *queueCB = (LosQueueCB *)mqID;

    if (queueCB == NULL) {
        space = 0U;
    } else {
        intSave = LOS_IntLock();
        space = (uint32_t)queueCB->readWriteableCnt[OS_QUEUE_WRITE];
        LOS_IntRestore(intSave);
    }
    return space;
}

osStatus_t osMessageQueueDelete(osMessageQueueId_t mqID)
{
    LosQueueCB *queueCB = (LosQueueCB *)mqID;
    uint32_t ret;

    if (queueCB == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_QueueDelete((uint32_t)queueCB->queueID);
    if (ret == LOS_OK) {
        return osOK;
    } else if ((ret == LOS_ERRNO_QUEUE_NOT_FOUND) || (ret == LOS_ERRNO_QUEUE_NOT_CREATE)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}
#endif

#endif // (LOSCFG_CMSIS_VERSION == 2)
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
