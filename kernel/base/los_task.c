/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Task Module Implementation
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

#include "los_task_pri.h"
#include "los_err_pri.h"
#include "los_priqueue_pri.h"
#include "los_sem_pri.h"
#include "los_mux_debug_pri.h"
#include "los_exc.h"
#include "los_memstat_pri.h"
#include "los_mp_pri.h"
#include "los_spinlock.h"
#include "los_percpu_pri.h"
#include "los_trace.h"

#ifdef LOSCFG_KERNEL_LOWPOWER
#include "los_lowpower_pri.h"
#endif
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif
#if (LOSCFG_BASE_CORE_SWTMR == YES)
#include "los_swtmr_pri.h"
#endif
#ifdef LOSCFG_EXC_INTERACTION
#include "los_exc_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_BSS LosTaskCB                       *g_taskCBArray;
LITE_OS_SEC_BSS LOS_DL_LIST                     g_losFreeTask;
LITE_OS_SEC_BSS LOS_DL_LIST                     g_taskRecycleList;
LITE_OS_SEC_BSS UINT32                          g_taskMaxNum;
LITE_OS_SEC_BSS UINT32                          g_taskScheduled; /* one bit for each cores */
#ifdef LOSCFG_LAZY_STACK
LITE_OS_SEC_BSS UINT16                          g_stackFrameOffLenInTcb;
#endif

/* spinlock for task module, only available on SMP mode */
LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_taskSpin);

#ifdef LOSCFG_BASE_CORE_TSK_MONITOR
TSKSWITCHHOOK g_pfnUsrTskSwitchHook = NULL;
#endif /* LOSCFG_BASE_CORE_TSK_MONITOR == YES */

#ifdef LOSCFG_KERNEL_LOWPOWER
LowPowerHookFn g_lowPowerHook = NULL;
#endif

STATIC VOID OsConsoleIDSetHook(UINT32 param1,
                               UINT32 param2) __attribute__((weakref("OsSetConsoleID")));
STATIC VOID OsExcStackCheckHook(VOID) __attribute__((weakref("OsExcStackCheck")));

#define OS_CHECK_TASK_BLOCK (OS_TASK_STATUS_DELAY |    \
                             OS_TASK_STATUS_PEND |     \
                             OS_TASK_STATUS_SUSPEND)

#define OS_INVALID_VALUE  0xFFFFFFFF

/* temp task blocks for booting procedure */
LITE_OS_SEC_BSS STATIC LosTaskCB                g_mainTask[LOSCFG_KERNEL_CORE_NUM];

VOID *OsGetMainTask()
{
    return (g_mainTask + ArchCurrCpuid());
}

VOID OsSetMainTask()
{
    UINT32 i;
    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        g_mainTask[i].taskStatus = OS_TASK_STATUS_UNUSED;
        g_mainTask[i].taskId = LOSCFG_BASE_CORE_TSK_LIMIT;
        g_mainTask[i].priority = OS_TASK_PRIORITY_LOWEST + 1;
        g_mainTask[i].taskName = "osMain";
#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
        g_mainTask[i].lockDep.lockDepth = 0;
        g_mainTask[i].lockDep.waitLock = NULL;
#endif
    }
}

LITE_OS_SEC_TEXT_INIT STATIC VOID OsTaskCBRecycleToFree(VOID)
{
    LosTaskCB *taskCB = NULL;
    VOID *poolTmp = NULL;
#ifdef LOSCFG_TASK_STACK_PROTECT
    UINTPTR MMUProtectAddr;
#endif
    while (!LOS_ListEmpty(&g_taskRecycleList)) {
        poolTmp = (VOID *)m_aucSysMem1;
        taskCB = OS_TCB_FROM_PENDLIST(LOS_DL_LIST_FIRST(&g_taskRecycleList));
        LOS_ListDelete(LOS_DL_LIST_FIRST(&g_taskRecycleList));
        LOS_ListAdd(&g_losFreeTask, &taskCB->pendList);
#ifdef LOSCFG_TASK_STACK_PROTECT
        MMUProtectAddr = taskCB->topOfStack - MMU_4K;
        OsTaskStackProtect(MMUProtectAddr, MMU_4K, ACCESS_PERM_RW_RW);
#ifdef LOSCFG_EXC_INTERACTION
        if (MMUProtectAddr < (UINTPTR)m_aucSysMem1) {
            poolTmp = (VOID *)m_aucSysMem0;
        }
#endif
        (VOID)LOS_MemFree(poolTmp, (VOID *)MMUProtectAddr);
#else
#ifdef LOSCFG_EXC_INTERACTION
        if (taskCB->topOfStack < (UINTPTR)m_aucSysMem1) {
            poolTmp = (VOID *)m_aucSysMem0;
        }
#endif
        (VOID)LOS_MemFree(poolTmp, (VOID *)taskCB->topOfStack);
#endif
        taskCB->topOfStack = 0;
    }
}

VOID LOS_TaskResRecycle(VOID)
{
    UINT32 intSave;

    SCHEDULER_LOCK(intSave);
    OsTaskCBRecycleToFree();
    SCHEDULER_UNLOCK(intSave);
}

#ifdef LOSCFG_EXC_INTERACTION
BOOL IsIdleTask(UINT32 taskId)
{
    UINT32 i;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        if (taskId == g_percpu[i].idleTaskId) {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

LITE_OS_SEC_TEXT WEAK VOID OsIdleTask(VOID)
{
    while (1) {
        LOS_TaskResRecycle();

#ifdef LOSCFG_KERNEL_LOWPOWER
        if (g_lowPowerHook != NULL) {
            g_lowPowerHook();
        }
#else
        wfi();
#endif
    }
}

/*
 * Description : Change task priority.
 * Input       : taskCB    --- task control block
 *               priority  --- priority
 */
LITE_OS_SEC_TEXT_MINOR VOID OsTaskPriModify(LosTaskCB *taskCB, UINT16 priority)
{
    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    LOS_TRACE(TASK_PRIOSET, taskCB->taskId, taskCB->taskStatus, taskCB->priority, priority);

    if (taskCB->taskStatus & OS_TASK_STATUS_READY) {
        OsPriQueueDequeue(&taskCB->pendList);
        taskCB->priority = priority;
        OsPriQueueEnqueue(&taskCB->pendList, taskCB->priority);
    } else {
        taskCB->priority = priority;
    }
}

/*
 * Description : Add task to sorted delay list.
 * Input       : taskCB  --- task control block
 *               timeout --- wait time, ticks
 */
LITE_OS_SEC_TEXT VOID OsTaskAdd2TimerList(LosTaskCB *taskCB, UINT32 timeout)
{
    SET_SORTLIST_VALUE(&(taskCB->sortList), timeout);
    OsAdd2SortLink(&OsPercpuGet()->taskSortLink, &taskCB->sortList);
#ifdef LOSCFG_KERNEL_SMP
    taskCB->timerCpu = ArchCurrCpuid();
#endif
}

LITE_OS_SEC_TEXT VOID OsTimerListDelete(LosTaskCB *taskCB)
{
    SortLinkAttribute *sortLinkHeader = NULL;

#ifdef LOSCFG_KERNEL_SMP
    /*
     * the task delay timer is on the specific processor,
     * we need delete the timer from that processor's sortlink.
     */
    sortLinkHeader = &g_percpu[taskCB->timerCpu].taskSortLink;
#else
    sortLinkHeader = &g_percpu[0].taskSortLink;
#endif
    OsDeleteSortLink(sortLinkHeader, &taskCB->sortList);
}

LITE_OS_SEC_TEXT VOID OsTaskScan(VOID)
{
    SortLinkList *sortList = NULL;
    LosTaskCB *taskCB = NULL;
    BOOL needSchedule = FALSE;
    UINT16 tempStatus;
    LOS_DL_LIST *listObject = NULL;
    SortLinkAttribute *taskSortLink = NULL;

    taskSortLink = &OsPercpuGet()->taskSortLink;
    SORTLINK_CURSOR_UPDATE(taskSortLink->cursor);
    SORTLINK_LISTOBJ_GET(listObject, taskSortLink);

    /*
     * When task is pended with timeout, the task block is on the timeout sortlink
     * (per cpu) and ipc(mutex,sem and etc.)'s block at the same time, it can be waken
     * up by either timeout or corresponding ipc it's waiting.
     *
     * Now synchronize sortlink procedure is used, therefore the whole task scan needs
     * to be protected, preventing another core from doing sortlink deletion at same time.
     */
    LOS_SpinLock(&g_taskSpin);

    if (LOS_ListEmpty(listObject)) {
        LOS_SpinUnlock(&g_taskSpin);
        return;
    }
    sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
    ROLLNUM_DEC(sortList->idxRollNum);

    while (ROLLNUM(sortList->idxRollNum) == 0) {
        LOS_ListDelete(&sortList->sortLinkNode);
        taskCB = LOS_DL_LIST_ENTRY(sortList, LosTaskCB, sortList);
        taskCB->taskStatus &= ~OS_TASK_STATUS_PEND_TIME;
        tempStatus = taskCB->taskStatus;
        if (tempStatus & OS_TASK_STATUS_PEND) {
            taskCB->taskStatus &= ~OS_TASK_STATUS_PEND;
            taskCB->taskStatus |= OS_TASK_STATUS_TIMEOUT;
            LOS_ListDelete(&taskCB->pendList);
            taskCB->taskSem = NULL;
            taskCB->taskMux = NULL;
        } else {
            taskCB->taskStatus &= ~OS_TASK_STATUS_DELAY;
        }

        if (!(tempStatus & OS_TASK_STATUS_SUSPEND)) {
            taskCB->taskStatus |= OS_TASK_STATUS_READY;
            OsPriQueueEnqueue(&taskCB->pendList, taskCB->priority);
            needSchedule = TRUE;
        }

        if (LOS_ListEmpty(listObject)) {
            break;
        }

        sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
    }

    LOS_SpinUnlock(&g_taskSpin);

    if (needSchedule != FALSE) {
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
    }
}

LITE_OS_SEC_TEXT_INIT UINT32 OsTaskInit(VOID)
{
    UINT32 index;
    UINT32 ret;
    UINT32 size;

    g_taskMaxNum = LOSCFG_BASE_CORE_TSK_LIMIT;
    size = (g_taskMaxNum + 1) * sizeof(LosTaskCB);
    /*
     * This memory is resident memory and is used to save the system resources
     * of task control block and will not be freed.
     */
    g_taskCBArray = (LosTaskCB *)LOS_MemAlloc(m_aucSysMem0, size);
    if (g_taskCBArray == NULL) {
        return LOS_ERRNO_TSK_NO_MEMORY;
    }
    (VOID)memset_s(g_taskCBArray, size, 0, size);

    LOS_ListInit(&g_losFreeTask);
    LOS_ListInit(&g_taskRecycleList);
    for (index = 0; index < g_taskMaxNum; index++) {
        g_taskCBArray[index].taskStatus = OS_TASK_STATUS_UNUSED;
        g_taskCBArray[index].taskId = index;
        LOS_ListTailInsert(&g_losFreeTask, &g_taskCBArray[index].pendList);
    }

    ret = OsPriQueueInit();
    if (ret != LOS_OK) {
        return LOS_ERRNO_TSK_NO_MEMORY;
    }

    ret = OsMuxDlockCheckInitHook();
    if (ret != LOS_OK) {
        return LOS_ERRNO_TSK_NO_MEMORY;
    }

    /* init sortlink for each core */
    for (index = 0; index < LOSCFG_KERNEL_CORE_NUM; index++) {
        ret = OsSortLinkInit(&g_percpu[index].taskSortLink);
        if (ret != LOS_OK) {
            return LOS_ERRNO_TSK_NO_MEMORY;
        }
    }

#ifdef LOSCFG_LAZY_STACK
    g_stackFrameOffLenInTcb = (UINT16)LOS_OFF_SET_OF(LosTaskCB, stackFrame);
#endif

    return LOS_OK;
}

UINT32 OsGetIdleTaskId(VOID)
{
    Percpu *perCpu = OsPercpuGet();
    return perCpu->idleTaskId;
}

LITE_OS_SEC_TEXT_INIT UINT32 OsIdleTaskCreate(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    Percpu *perCpu = OsPercpuGet();
    UINT32 *idleTaskId = &perCpu->idleTaskId;

    (VOID)memset_s((VOID *)(&taskInitParam), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)OsIdleTask;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    taskInitParam.pcName = "IdleCore000";
    taskInitParam.usTaskPrio = OS_TASK_PRIORITY_LOWEST;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(idleTaskId, &taskInitParam);
    if (ret == LOS_OK) {
        OS_TCB_FROM_TID(*idleTaskId)->taskFlags |= OS_TASK_FLAG_SYSTEM;
    }

    return ret;
}

/*
 * Description : get id of current running task.
 * Return      : task id
 */
LITE_OS_SEC_TEXT UINT32 LOS_CurTaskIDGet(VOID)
{
    LosTaskCB *runTask = OsCurrTaskGet();

    if (runTask == NULL) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }
    return runTask->taskId;
}

LITE_OS_SEC_TEXT CHAR *OsCurTaskNameGet(VOID)
{
    LosTaskCB *runTask = OsCurrTaskGet();

    if (runTask != NULL) {
        return runTask->taskName;
    }

    return NULL;
}

#ifdef LOSCFG_BASE_CORE_TSK_MONITOR
LITE_OS_SEC_TEXT STATIC VOID OsTaskStackCheck(const LosTaskCB *oldTask, const LosTaskCB *newTask)
{
    if (!OS_STACK_MAGIC_CHECK(oldTask->topOfStack)) {
        LOS_Panic("CURRENT task ID: %s:%u stack overflow!\n", oldTask->taskName, oldTask->taskId);
    }

    if (((UINTPTR)(newTask->stackPointer) <= newTask->topOfStack) ||
        ((UINTPTR)(newTask->stackPointer) > (newTask->topOfStack + newTask->stackSize))) {
        LOS_Panic("HIGHEST task ID: %s:%u SP error! StackPointer: %p TopOfStack: %p\n",
                  newTask->taskName, newTask->taskId, newTask->stackPointer, newTask->topOfStack);
    }

    if (OsExcStackCheckHook != NULL) {
        OsExcStackCheckHook();
    }
}

LITE_OS_SEC_TEXT_MINOR VOID OsTaskMonInit(VOID)
{
    g_pfnUsrTskSwitchHook = NULL;
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_TaskSwitchHookReg(TSKSWITCHHOOK hook)
{
    g_pfnUsrTskSwitchHook = hook;
}

LITE_OS_SEC_TEXT_MINOR VOID OsTaskSwitchCheck(const LosTaskCB *oldTask, const LosTaskCB *newTask)
{
    OsTaskStackCheck(oldTask, newTask);

    if (g_pfnUsrTskSwitchHook != NULL) {
        g_pfnUsrTskSwitchHook();
    }
}
#endif /* LOSCFG_BASE_CORE_TSK_MONITOR */

#ifdef LOSCFG_KERNEL_LOWPOWER
LITE_OS_SEC_TEXT_MINOR VOID LOS_LowpowerHookReg(LowPowerHookFn hook)
{
    g_lowPowerHook = hook;
}
#endif

STATIC BOOL OsTaskDeleteCheckDetached(const LosTaskCB *taskCB)
{
#if LOSCFG_COMPAT_POSIX
    return ((taskCB->taskFlags & OS_TASK_FLAG_DETACHED) != 0);
#else
    return TRUE;
#endif
}

STATIC VOID OsTaskDeleteDetached(const LosTaskCB *taskCB)
{
    UINT32 intSave;
    intSave = LOS_IntLock();
    OsPercpuGet()->taskLockCnt = 0;
    LOS_IntRestore(intSave);
    (VOID)LOS_TaskDelete(taskCB->taskId);
}

STATIC VOID OsTaskDeleteJoined(LosTaskCB *taskCB, VOID *ret)
{
#ifdef LOSCFG_COMPAT_POSIX
    UINT32 intSave;
    taskCB->threadJoinRetval = ret;

    intSave = LOS_IntLock();
    OsPercpuGet()->taskLockCnt = 1;

    if (taskCB->threadJoin != NULL) {
        if (LOS_SemPost((UINT32)(((LosSemCB *)taskCB->threadJoin)->semId)) != LOS_OK) {
            PRINT_ERR("OsTaskEntry LOS_SemPost fail!\n");
        }
        taskCB->threadJoin = NULL;
    }
    OsPercpuGet()->taskLockCnt = 0;

    LOS_SpinLock(&g_taskSpin);
    OsSchedResched();

    SCHEDULER_UNLOCK(intSave);
#endif
}

/*
 * Description : All task entry
 * Input       : taskId     --- The ID of the task to be run
 */
LITE_OS_SEC_TEXT_INIT VOID OsTaskEntry(UINT32 taskId)
{
    LosTaskCB *taskCB = NULL;
    VOID *ret = NULL;

    LOS_ASSERT(OS_TSK_GET_INDEX(taskId) < g_taskMaxNum);

    /*
     * task scheduler needs to be protected throughout the whole process
     * from interrupt and other cores. release task spinlock and enable
     * interrupt in sequence at the task entry.
     */
    LOS_SpinUnlock(&g_taskSpin);
    (VOID)LOS_IntUnLock();

    taskCB = OS_TCB_FROM_TID(taskId);

#ifdef LOSCFG_OBSOLETE_API
    ret = taskCB->taskEntry(taskCB->args[0], taskCB->args[1], taskCB->args[2],
        taskCB->args[3]); /* 0~3: just for args array index */
#else
    ret = taskCB->taskEntry(taskCB->args);
#endif

    if (OsTaskDeleteCheckDetached(taskCB)) {
        OsTaskDeleteDetached(taskCB);
    } else {
        OsTaskDeleteJoined(taskCB, ret);
    }
}

STATIC UINT32 OsTaskInitParamCheck(const TSK_INIT_PARAM_S *initParam)
{
    if (initParam == NULL) {
        return LOS_ERRNO_TSK_PTR_NULL;
    }

    if (initParam->pcName == NULL) {
        return LOS_ERRNO_TSK_NAME_EMPTY;
    }

    if (initParam->pfnTaskEntry == NULL) {
        return LOS_ERRNO_TSK_ENTRY_NULL;
    }

    if (initParam->usTaskPrio > OS_TASK_PRIORITY_LOWEST) {
        return LOS_ERRNO_TSK_PRIOR_ERROR;
    }

    return LOS_OK;
}

#ifdef LOSCFG_TASK_STATIC_ALLOCATION
STATIC UINT32 OsTaskCreateParamCheckStatic(const UINT32 *taskId,
    const TSK_INIT_PARAM_S *initParam, const VOID *topStack)
{
    UINT32 ret;

    if (taskId == NULL) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    if (topStack == NULL) {
        return LOS_ERRNO_TSK_PTR_NULL;
    }

    ret = OsTaskInitParamCheck(initParam);
    if (ret != LOS_OK) {
        return ret;
    }

    if ((UINTPTR)topStack & (OS_TASK_STACK_ADDR_ALIGN - 1)) {
        return LOS_ERRNO_TSK_STKSZ_NOT_ALIGN;
    }

    if (initParam->uwStackSize & (OS_TASK_STACK_ADDR_ALIGN - 1)) {
        return LOS_ERRNO_TSK_STKSZ_NOT_ALIGN;
    }

    if (initParam->uwStackSize < LOS_TASK_MIN_STACK_SIZE) {
        return LOS_ERRNO_TSK_STKSZ_TOO_SMALL;
    }
    return LOS_OK;
}
#endif

LITE_OS_SEC_TEXT_INIT STATIC UINT32 OsTaskCreateParamCheck(const UINT32 *taskId,
    TSK_INIT_PARAM_S *initParam, VOID **pool)
{
    UINT32 ret;
    UINT32 poolSize = OS_SYS_MEM_SIZE;
    *pool = (VOID *)m_aucSysMem1;

    if (taskId == NULL) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    ret = OsTaskInitParamCheck(initParam);
    if (ret != LOS_OK) {
        return ret;
    }

#ifdef LOSCFG_EXC_INTERACTION
    if (!OsCheckExcInteractionTask(initParam)) {
        *pool = m_aucSysMem0;
        poolSize = g_excInteractMemSize;
    }
#endif
#ifdef LOSCFG_TASK_STACK_PROTECT
    poolSize = (poolSize > (MMU_4K << 1)) ? (poolSize - (MMU_4K << 1)) : 0;
#endif
    if (initParam->uwStackSize > poolSize) {
        return LOS_ERRNO_TSK_STKSZ_TOO_LARGE;
    }

    if (initParam->uwStackSize == 0) {
        initParam->uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    }
    initParam->uwStackSize = (UINT32)ALIGN(initParam->uwStackSize, LOSCFG_STACK_POINT_ALIGN_SIZE);

    if (initParam->uwStackSize < LOS_TASK_MIN_STACK_SIZE) {
        return LOS_ERRNO_TSK_STKSZ_TOO_SMALL;
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT STATIC VOID OsTaskStackAlloc(VOID **topStack, UINT32 stackSize, VOID *pool)
{
#ifdef LOSCFG_TASK_STACK_PROTECT
    UINTPTR MMUProtectAddr;
    UINT32 alignStackSize;

    alignStackSize = ALIGN(stackSize, MMU_4K);
    MMUProtectAddr = (UINTPTR)LOS_MemAllocAlign(pool, (alignStackSize + MMU_4K), MMU_4K);
    if (MMUProtectAddr == 0) {
        *topStack = NULL;
    } else {
        *topStack = (VOID *)(MMUProtectAddr + MMU_4K);
        OsTaskStackProtect(MMUProtectAddr, MMU_4K, ACCESS_PERM_RO_RO);
    }
#else
    *topStack = (VOID *)LOS_MemAllocAlign(pool, stackSize, LOSCFG_STACK_POINT_ALIGN_SIZE);
#endif
}

STATIC INLINE UINT32 OsTaskSyncCreate(LosTaskCB *taskCB)
{
#ifdef LOSCFG_KERNEL_SMP_TASK_SYNC
    UINT32 ret = LOS_SemCreate(0, &taskCB->syncSignal);
    if (ret != LOS_OK) {
        return LOS_ERRNO_TSK_MP_SYNC_RESOURCE;
    }
#else
    (VOID)taskCB;
#endif
    return LOS_OK;
}

STATIC INLINE VOID OsTaskSyncDestroy(const LosTaskCB *taskCB)
{
#ifdef LOSCFG_KERNEL_SMP_TASK_SYNC
    (VOID)LOS_SemDelete(taskCB->syncSignal);
#else
    (VOID)taskCB;
#endif
}

STATIC INLINE UINT32 OsTaskSyncWait(const LosTaskCB *taskCB)
{
#ifdef LOSCFG_KERNEL_SMP_TASK_SYNC
    UINT32 ret = LOS_OK;

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));
    LOS_SpinUnlock(&g_taskSpin);
    /*
     * gc soft timer works every OS_MP_GC_PERIOD period, to prevent this timer
     * triggered right at the timeout has reached, we set the timeout as double
     * of the gc peroid.
     */
    if (LOS_SemPend(taskCB->syncSignal, OS_MP_GC_PERIOD * 2) != LOS_OK) {
        ret = LOS_ERRNO_TSK_MP_SYNC_FAILED;
    }

    LOS_SpinLock(&g_taskSpin);

    return ret;
#else
    (VOID)taskCB;
    return LOS_OK;
#endif
}

STATIC INLINE VOID OsTaskSyncWake(const LosTaskCB *taskCB)
{
#ifdef LOSCFG_KERNEL_SMP_TASK_SYNC
    UINT32 syncSignal = taskCB->syncSignal;
    /*
     * unlock to let synchronization works.
     * this operation needs task status set to UNUSED, and yet not put
     * back to the recycle or free tcb list.
     */
    LOS_ASSERT(taskCB->taskStatus & OS_TASK_STATUS_UNUSED);
    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    LOS_SpinUnlock(&g_taskSpin);

    /*
     * do the sync, because botton half of LOS_SemPend has no operation on the sem,
     * we can delete this sem after the post.
     */
    (VOID)LOS_SemPost(syncSignal);
    (VOID)LOS_SemDelete(syncSignal);

    LOS_SpinLock(&g_taskSpin);
#else
    (VOID)taskCB;
#endif
}

STATIC VOID OsTaskDelActionOnRun(LosTaskCB *taskCB)
{
    LosTaskCB *runTask = NULL;

    runTask = &g_taskCBArray[g_taskMaxNum];
    runTask->taskId = taskCB->taskId;
    runTask->taskStatus = taskCB->taskStatus;
    runTask->topOfStack = taskCB->topOfStack;
    runTask->taskName = taskCB->taskName;
    taskCB->taskStatus = OS_TASK_STATUS_UNUSED;
}

LITE_OS_SEC_TEXT_INIT STATIC BOOL OsTaskDelAction(LosTaskCB *taskCB, BOOL useUsrStack)
{
    VOID *pool = (VOID *)m_aucSysMem1;
    UINTPTR taskStack;

    LOS_TRACE(TASK_DELETE, taskCB->taskId, taskCB->taskStatus, taskCB->usrStack);

    if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
#ifdef LOSCFG_TASK_STATIC_ALLOCATION
        if (useUsrStack) {
            LOS_ListAdd(&g_losFreeTask, &taskCB->pendList);
        } else {
#endif
            LOS_ListTailInsert(&g_taskRecycleList, &taskCB->pendList);
#ifdef LOSCFG_TASK_STATIC_ALLOCATION
        }
#endif
        OsTaskDelActionOnRun(taskCB);
        return TRUE;
    }

    taskCB->taskStatus = OS_TASK_STATUS_UNUSED;
    LOS_ListAdd(&g_losFreeTask, &taskCB->pendList);
    if (useUsrStack == FALSE) {
#ifdef LOSCFG_TASK_STACK_PROTECT
        taskStack = taskCB->topOfStack - MMU_4K;
        OsTaskStackProtect(taskStack, MMU_4K, ACCESS_PERM_RW_RW);
#else
        taskStack = taskCB->topOfStack;
#endif

#ifdef LOSCFG_EXC_INTERACTION
        if (taskStack < (UINTPTR)m_aucSysMem1) {
            pool = (VOID *)m_aucSysMem0;
        }
#endif
        (VOID)LOS_MemFree(pool, (VOID *)taskStack);
    }
    taskCB->topOfStack = 0;
    return FALSE;
}

/*
 * Check if needs to do the delete operation on the running task.
 * Return TRUE, if needs to do the deletion.
 * Return FALSE, if meets following circumstances:
 * 1. Do the deletion across cores, if SMP is enabled
 * 2. Do the deletion when preemption is disabled
 * 3. Do the deletion in hard-irq
 * then LOS_TaskDelete will directly return with 'ret' value.
 */
LITE_OS_SEC_TEXT_INIT STATIC BOOL OsTaskDeleteCheckOnRun(LosTaskCB *taskCB, UINT32 *ret)
{
    /* init default out return value */
    *ret = LOS_OK;

#ifdef LOSCFG_KERNEL_SMP
    /* ASYNCHRONIZED. No need to do task lock checking */
    if (taskCB->currCpu != ArchCurrCpuid()) {
        /*
         * the task is running on another cpu.
         * mask the target task with "kill" signal, and trigger mp schedule
         * which might not be essential but the deletion could more in time.
         */
        taskCB->signal = SIGNAL_KILL;
        LOS_MpSchedule(taskCB->currCpu);
        *ret = OsTaskSyncWait(taskCB);
        return FALSE;
    }
#endif

    if (!OsPreemptableInSched()) {
        /* If the task is running and scheduler is locked then you can not delete it */
        *ret = LOS_ERRNO_TSK_DELETE_LOCKED;
        return FALSE;
    }

    if (OS_INT_ACTIVE) {
        /*
         * delete running task in interrupt.
         * mask "kill" signal and later deletion will be handled.
         */
        taskCB->signal = SIGNAL_KILL;
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_INIT STATIC VOID OsTaskCBInit(LosTaskCB *taskCB, const TSK_INIT_PARAM_S *initParam,
                                               VOID *stackPtr, const VOID *topStack, BOOL useUsrStack)
{
    taskCB->stackPointer = stackPtr;
#ifdef LOSCFG_OBSOLETE_API
    taskCB->args[0]      = initParam->auwArgs[0]; /* 0~3: just for args array index */
    taskCB->args[1]      = initParam->auwArgs[1];
    taskCB->args[2]      = initParam->auwArgs[2];
    taskCB->args[3]      = initParam->auwArgs[3];
#else
    taskCB->args         = initParam->pArgs;
#endif
    taskCB->topOfStack   = (UINTPTR)topStack;
    taskCB->stackSize    = initParam->uwStackSize;
    taskCB->taskSem      = NULL;
#ifdef LOSCFG_COMPAT_POSIX
    taskCB->threadJoin   = NULL;
#endif
    taskCB->taskMux      = NULL;
    taskCB->taskStatus   = OS_TASK_STATUS_SUSPEND;
    taskCB->priority     = initParam->usTaskPrio;
    taskCB->priBitMap    = 0;
    taskCB->taskEntry    = initParam->pfnTaskEntry;
#ifdef LOSCFG_BASE_IPC_EVENT
    LOS_ListInit(&taskCB->event.stEventList);
    taskCB->event.uwEventID = 0;
    taskCB->eventMask    = 0;
#endif

    taskCB->taskName     = initParam->pcName;
    taskCB->msg          = NULL;

    taskCB->taskFlags    = ((initParam->uwResved == LOS_TASK_STATUS_DETACHED) ?
                            OS_TASK_FLAG_DETACHED : 0); /* set the task is detached or joinable */
    taskCB->usrStack     = useUsrStack ? 1 : 0; /* 0: dynamicly alloc stack space;1: user inputs stack space */
    taskCB->signal       = SIGNAL_NONE;

#ifdef LOSCFG_KERNEL_SMP
    taskCB->currCpu      = OS_TASK_INVALID_CPUID;
#if (LOSCFG_SCHED_MQ == YES)
    taskCB->lastCpu      = OS_TASK_INVALID_CPUID;
#endif
    taskCB->cpuAffiMask  = (initParam->usCpuAffiMask) ? initParam->usCpuAffiMask : LOSCFG_KERNEL_CPU_MASK;
#endif
#ifdef LOSCFG_BASE_CORE_TIMESLICE
    taskCB->timeSlice    = 0;
#endif
#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
    taskCB->lockDep.waitLock  = NULL;
    taskCB->lockDep.lockDepth = 0;
#endif
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    (VOID)memset_s(&taskCB->schedStat, sizeof(SchedStat), 0, sizeof(SchedStat));
#endif
}

STATIC UINT32 OsTaskGetFreeTaskCB(LosTaskCB **taskCB)
{
    if (LOS_ListEmpty(&g_losFreeTask)) {
        return LOS_ERRNO_TSK_TCB_UNAVAILABLE;
    }

    *taskCB = OS_TCB_FROM_PENDLIST(LOS_DL_LIST_FIRST(&g_losFreeTask));
    if (*taskCB == NULL) {
        return LOS_ERRNO_TSK_PTR_NULL;
    }
    LOS_ListDelete(LOS_DL_LIST_FIRST(&g_losFreeTask));
    return LOS_OK;
}

STATIC UINT32 OsTaskCreateOnly(UINT32 *taskId, TSK_INIT_PARAM_S *initParam, VOID *topStack, BOOL useUsrStack)
{
    UINT32 intSave, errRet;
    VOID *stackPtr = NULL;
    LosTaskCB *taskCB = NULL;
    VOID *pool = NULL;

#ifdef LOSCFG_TASK_STATIC_ALLOCATION
    if (useUsrStack) {
        errRet = OsTaskCreateParamCheckStatic(taskId, initParam, topStack);
    } else {
#endif
        errRet = OsTaskCreateParamCheck(taskId, initParam, &pool);
#ifdef LOSCFG_TASK_STATIC_ALLOCATION
    }
#endif
    if (errRet != LOS_OK) {
        return errRet;
    }

    SCHEDULER_LOCK(intSave);
    errRet = OsTaskGetFreeTaskCB(&taskCB);
    if (errRet != LOS_OK) {
        OS_GOTO_ERREND();
    }
    SCHEDULER_UNLOCK(intSave);

    errRet = OsTaskSyncCreate(taskCB);
    if (errRet != LOS_OK) {
        goto LOS_ERREND_REWIND_TCB;
    }

    if (useUsrStack == FALSE) {
        OsTaskStackAlloc(&topStack, initParam->uwStackSize, pool);
        if (topStack == NULL) {
            errRet = LOS_ERRNO_TSK_NO_MEMORY;
            goto LOS_ERREND_REWIND_SYNC;
        }
    }
    stackPtr = OsTaskStackInit(taskCB->taskId, initParam->uwStackSize, topStack);
    OsTaskCBInit(taskCB, initParam, stackPtr, topStack, useUsrStack);

    if (OsConsoleIDSetHook != NULL) {
        OsConsoleIDSetHook(taskCB->taskId, OsCurrTaskGet()->taskId);
    }

#ifdef LOSCFG_KERNEL_CPUP
    OsCpupCB *cpup = OsCpupCBGet(taskCB->taskId);
    cpup->id = taskCB->taskId;
    cpup->status = taskCB->taskStatus;
#endif

    *taskId = taskCB->taskId;
    return LOS_OK;

LOS_ERREND_REWIND_SYNC:
    OsTaskSyncDestroy(taskCB);
LOS_ERREND_REWIND_TCB:
    SCHEDULER_LOCK(intSave);
    LOS_ListAdd(&g_losFreeTask, &taskCB->pendList);
LOS_ERREND:
    SCHEDULER_UNLOCK(intSave);
    return errRet;
}

STATIC VOID OsTaskResume(const UINT32 *taskId)
{
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;

    taskCB = OS_TCB_FROM_TID(*taskId);

    SCHEDULER_LOCK(intSave);

    taskCB->taskStatus &= ~OS_TASK_STATUS_SUSPEND;
    taskCB->taskStatus |= OS_TASK_STATUS_READY;
    OsPriQueueEnqueue(&taskCB->pendList, taskCB->priority);

    SCHEDULER_UNLOCK(intSave);

    LOS_TRACE(TASK_CREATE, taskCB->taskId, taskCB->taskStatus, taskCB->priority);

    /* in case created task not running on this core,
       schedule or not depends on other schedulers status. */
    LOS_MpSchedule(OS_MP_CPU_ALL);
    if (OS_SCHEDULER_ACTIVE) {
        LOS_Schedule();
    }
}

#ifdef LOSCFG_TASK_STATIC_ALLOCATION
UINT32 LOS_TaskCreateOnlyStatic(UINT32 *taskId, TSK_INIT_PARAM_S *initParam, VOID *topStack)
{
    return OsTaskCreateOnly(taskId, initParam, topStack, TRUE);
}

UINT32 LOS_TaskCreateStatic(UINT32 *taskId, TSK_INIT_PARAM_S *initParam, VOID *topStack)
{
    UINT32 ret;

    ret = LOS_TaskCreateOnlyStatic(taskId, initParam, topStack);
    if (ret != LOS_OK) {
        return ret;
    }

    OsTaskResume(taskId);

    return LOS_OK;
}
#endif

LITE_OS_SEC_TEXT_INIT UINT32 LOS_TaskCreateOnly(UINT32 *taskId, TSK_INIT_PARAM_S *initParam)
{
    return OsTaskCreateOnly(taskId, initParam, NULL, FALSE);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_TaskCreate(UINT32 *taskId, TSK_INIT_PARAM_S *initParam)
{
    UINT32 ret;

    ret = LOS_TaskCreateOnly(taskId, initParam);
    if (ret != LOS_OK) {
        return ret;
    }

    OsTaskResume(taskId);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_TaskDelete(UINT32 taskId)
{
    LosTaskCB *taskCB = NULL;
    UINT32 intSave, errRet;
    UINT16 tempStatus;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    if (taskCB->taskFlags & OS_TASK_FLAG_SYSTEM) {
        return LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
    }

    SCHEDULER_LOCK(intSave);

    tempStatus = taskCB->taskStatus;
    if (tempStatus & OS_TASK_STATUS_UNUSED) {
        errRet = LOS_ERRNO_TSK_NOT_CREATED;
        OS_GOTO_ERREND();
    }
    if ((tempStatus & OS_TASK_STATUS_RUNNING) &&
        !OsTaskDeleteCheckOnRun(taskCB, &errRet)) {
        OS_GOTO_ERREND();
    }

    if (tempStatus & OS_TASK_STATUS_READY) {
        OsPriQueueDequeue(&taskCB->pendList);
        taskCB->taskStatus &= ~OS_TASK_STATUS_READY;
    } else if (tempStatus & OS_TASK_STATUS_PEND) {
        LOS_ListDelete(&taskCB->pendList);
    }

    if (tempStatus & (OS_TASK_STATUS_DELAY | OS_TASK_STATUS_PEND_TIME)) {
        OsTimerListDelete(taskCB);
    }

    taskCB->taskStatus &= ~OS_TASK_STATUS_SUSPEND;
    taskCB->taskStatus |= OS_TASK_STATUS_UNUSED;
#ifdef LOSCFG_BASE_IPC_EVENT
    taskCB->event.uwEventID = OS_INVALID_VALUE;
    taskCB->eventMask = 0;
#endif
#ifdef LOSCFG_LAZY_STACK
    taskCB->stackFrame = 0;
#endif
#ifdef LOSCFG_KERNEL_CPUP
    (VOID)memset_s((VOID *)OsCpupCBGet(taskCB->taskId), sizeof(OsCpupCB), 0, sizeof(OsCpupCB));
#endif
    OS_MEM_CLEAR(taskId);

    OsTaskSyncWake(taskCB);
    if (OsTaskDelAction(taskCB, taskCB->usrStack)) {
        OsSchedResched();
    }

    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;

LOS_ERREND:
    SCHEDULER_UNLOCK(intSave);
    return errRet;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_TaskResume(UINT32 taskId)
{
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;
    UINT16 tempStatus;
    UINT32 errRet;
    BOOL needSched = FALSE;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);

    SCHEDULER_LOCK(intSave);

    /* clear pending signal */
    taskCB->signal &= ~SIGNAL_SUSPEND;

    tempStatus = taskCB->taskStatus;

    if (tempStatus & OS_TASK_STATUS_UNUSED) {
        errRet = LOS_ERRNO_TSK_NOT_CREATED;
        OS_GOTO_ERREND();
    } else if (!(tempStatus & OS_TASK_STATUS_SUSPEND)) {
        errRet = LOS_ERRNO_TSK_NOT_SUSPENDED;
        OS_GOTO_ERREND();
    }

    taskCB->taskStatus &= ~OS_TASK_STATUS_SUSPEND;
    if (!(taskCB->taskStatus & OS_CHECK_TASK_BLOCK)) {
        taskCB->taskStatus |= OS_TASK_STATUS_READY;
        OsPriQueueEnqueue(&taskCB->pendList, taskCB->priority);
        if (OS_SCHEDULER_ACTIVE) {
            needSched = TRUE;
        }
    }

    SCHEDULER_UNLOCK(intSave);

    LOS_TRACE(TASK_RESUME, taskCB->taskId, taskCB->taskStatus, taskCB->priority);

    if (needSched) {
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
    }

    return LOS_OK;

LOS_ERREND:
    SCHEDULER_UNLOCK(intSave);
    return errRet;
}

/*
 * Check if needs to do the suspend operation on the running task.
 * Return TRUE, if needs to do the suspension.
 * Return FALSE, if meets following circumstances:
 * 1. Do the suspension across cores, if SMP is enabled
 * 2. Do the suspension when preemption is disabled
 * 3. Do the suspension in hard-irq
 * then LOS_TaskSuspend will directly return with 'ret' value.
 */
LITE_OS_SEC_TEXT_INIT STATIC BOOL OsTaskSuspendCheckOnRun(LosTaskCB *taskCB, UINT32 *ret)
{
    /* init default out return value */
    *ret = LOS_OK;

#ifdef LOSCFG_KERNEL_SMP
    /* ASYNCHRONIZED. No need to do task lock checking */
    if (taskCB->currCpu != ArchCurrCpuid()) {
        taskCB->signal = SIGNAL_SUSPEND;
        LOS_MpSchedule(taskCB->currCpu);
        return FALSE;
    }
#endif

    if (!OsPreemptableInSched()) {
        /* Suspending the current core's running task */
        *ret = LOS_ERRNO_TSK_SUSPEND_LOCKED;
        return FALSE;
    }

    if (OS_INT_ACTIVE) {
        /* suspend running task in interrupt */
        taskCB->signal = SIGNAL_SUSPEND;
        return FALSE;
    }

    return TRUE;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_TaskSuspend(UINT32 taskId)
{
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;
    LosTaskCB *runTask = NULL;
    UINT16 tempStatus;
    UINT32 errRet;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    if (taskCB->taskFlags & OS_TASK_FLAG_SYSTEM) {
        return LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
    }

    SCHEDULER_LOCK(intSave);
    tempStatus = taskCB->taskStatus;
    if (tempStatus & OS_TASK_STATUS_UNUSED) {
        errRet = LOS_ERRNO_TSK_NOT_CREATED;
        OS_GOTO_ERREND();
    }

    if (tempStatus & OS_TASK_STATUS_SUSPEND) {
        errRet = LOS_ERRNO_TSK_ALREADY_SUSPENDED;
        OS_GOTO_ERREND();
    }

    if ((tempStatus & OS_TASK_STATUS_RUNNING) &&
        !OsTaskSuspendCheckOnRun(taskCB, &errRet)) {
        OS_GOTO_ERREND();
    }

    if (tempStatus & OS_TASK_STATUS_READY) {
        OsPriQueueDequeue(&taskCB->pendList);
        taskCB->taskStatus &= ~OS_TASK_STATUS_READY;
    }

    taskCB->taskStatus |= OS_TASK_STATUS_SUSPEND;

    runTask = OsCurrTaskGet();

    LOS_TRACE(TASK_SUSPEND, taskCB->taskId, taskCB->taskStatus, runTask->taskId);

    if (taskId == runTask->taskId) {
        OsSchedResched();
    }

    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;

LOS_ERREND:
    SCHEDULER_UNLOCK(intSave);
    return errRet;
}

LITE_OS_SEC_TEXT UINT32 LOS_TaskDelay(UINT32 tick)
{
    UINT32 intSave;
    LosTaskCB *runTask = NULL;

    if (OS_INT_ACTIVE) {
        return LOS_ERRNO_TSK_DELAY_IN_INT;
    }

    runTask = OsCurrTaskGet();
    if (runTask->taskFlags & OS_TASK_FLAG_SYSTEM) {
        OsBackTrace();
        return LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
    }

    if (!OsPreemptable()) {
        return LOS_ERRNO_TSK_DELAY_IN_LOCK;
    }

    if (tick == 0) {
        return LOS_TaskYield();
    } else {
        SCHEDULER_LOCK(intSave);
        OsTaskAdd2TimerList(runTask, tick);
        runTask->taskStatus |= OS_TASK_STATUS_DELAY;
        OsSchedResched();
        SCHEDULER_UNLOCK(intSave);
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT16 LOS_TaskPriGet(UINT32 taskId)
{
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;
    UINT16 priority;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return (UINT16)OS_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);

    SCHEDULER_LOCK(intSave);
    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        SCHEDULER_UNLOCK(intSave);
        return (UINT16)OS_INVALID;
    }

    priority = taskCB->priority;
    SCHEDULER_UNLOCK(intSave);
    return priority;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_TaskPriSet(UINT32 taskId, UINT16 taskPrio)
{
    BOOL isReady = FALSE;
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;
    UINT16 tempStatus;

    if (taskPrio > OS_TASK_PRIORITY_LOWEST) {
        return LOS_ERRNO_TSK_PRIOR_ERROR;
    }

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    if (taskCB->taskFlags & OS_TASK_FLAG_SYSTEM) {
        return LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
    }

    SCHEDULER_LOCK(intSave);

    tempStatus = taskCB->taskStatus;
    if (tempStatus & OS_TASK_STATUS_UNUSED) {
        SCHEDULER_UNLOCK(intSave);
        return LOS_ERRNO_TSK_NOT_CREATED;
    }
    /* delete the task and insert with right priority into ready queue */
    isReady = tempStatus & OS_TASK_STATUS_READY;
    if (isReady) {
        OsPriQueueDequeue(&taskCB->pendList);
        taskCB->priority = taskPrio;
        OsPriQueueEnqueue(&taskCB->pendList, taskCB->priority);
    } else {
        taskCB->priority = taskPrio;
        if (tempStatus & OS_TASK_STATUS_RUNNING) {
            isReady = TRUE;
        }
    }

    SCHEDULER_UNLOCK(intSave);

    LOS_TRACE(TASK_PRIOSET, taskCB->taskId, taskCB->taskStatus, taskCB->priority, taskPrio);

    /* delete the task and insert with right priority into ready queue */
    if (isReady) {
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_CurTaskPriSet(UINT16 taskPrio)
{
    return LOS_TaskPriSet(OsCurrTaskGet()->taskId, taskPrio);
}

/*
 * Description : pend a task in list
 * Input       : list       --- wait task list
 *               taskStatus --- task status
 *               timeOut    ---  Expiry time
 * Return      : LOS_OK on success or LOS_NOK on failure
 */
VOID OsTaskWait(LOS_DL_LIST *list, UINT16 taskStatus, UINT32 timeout)
{
    LosTaskCB *runTask = NULL;
    LOS_DL_LIST *pendObj = NULL;

    runTask = OsCurrTaskGet();
    runTask->taskStatus &= ~OS_TASK_STATUS_READY;
    pendObj = &runTask->pendList;
    runTask->taskStatus |= taskStatus;
    LOS_ListTailInsert(list, pendObj);
    if (timeout != LOS_WAIT_FOREVER) {
        runTask->taskStatus |= OS_TASK_STATUS_PEND_TIME;
        OsTaskAdd2TimerList((LosTaskCB *)runTask, timeout);
    }
}

/*
 * Description : delete the task from pendlist and also add to the priqueue
 * Input       : resumedTask --- resumed task
 *               taskStatus  --- task status
 */
VOID OsTaskWake(LosTaskCB *resumedTask, UINT16 taskStatus)
{
    LOS_ListDelete(&resumedTask->pendList);
    resumedTask->taskStatus &= ~taskStatus;

    if (resumedTask->taskStatus & OS_TASK_STATUS_PEND_TIME) {
        OsTimerListDelete(resumedTask);
        resumedTask->taskStatus &= ~OS_TASK_STATUS_PEND_TIME;
    }
    if (!(resumedTask->taskStatus & OS_TASK_STATUS_SUSPEND)) {
        resumedTask->taskStatus |= OS_TASK_STATUS_READY;
        OsPriQueueEnqueue(&resumedTask->pendList, resumedTask->priority);
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_TaskYield(VOID)
{
    UINT32 tskCount;
    UINT32 intSave;
    LosTaskCB *runTask = NULL;

    if (OS_INT_ACTIVE) {
        return LOS_ERRNO_TSK_YIELD_IN_INT;
    }

    if (!OsPreemptable()) {
        return LOS_ERRNO_TSK_YIELD_IN_LOCK;
    }

    runTask = OsCurrTaskGet();
    if (runTask->taskId >= g_taskMaxNum) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    SCHEDULER_LOCK(intSave);

#ifdef LOSCFG_BASE_CORE_TIMESLICE
    /* reset timeslice of yielded task */
    runTask->timeSlice = 0;
#endif

    tskCount = OsPriQueueSize(runTask->priority);
    if (tskCount > 0) {
        runTask->taskStatus |= OS_TASK_STATUS_READY;
        OsPriQueueEnqueue(&(runTask->pendList), runTask->priority);
    } else {
        SCHEDULER_UNLOCK(intSave);
        return LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK;
    }
    OsSchedResched();
    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_TaskLock(VOID)
{
    UINT32 intSave;
    UINT32 *losTaskLock = NULL;

    intSave = LOS_IntLock();
    losTaskLock = &OsPercpuGet()->taskLockCnt;
    (*losTaskLock)++;
    LOS_IntRestore(intSave);
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_TaskUnlock(VOID)
{
    UINT32 intSave;
    UINT32 *losTaskLock = NULL;
    Percpu *percpu = NULL;

    intSave = LOS_IntLock();

    percpu = OsPercpuGet();
    losTaskLock = &percpu->taskLockCnt;
    if (*losTaskLock > 0) {
        (*losTaskLock)--;
        if ((*losTaskLock == 0) && (percpu->schedFlag == INT_PEND_RESCH) &&
            OS_SCHEDULER_ACTIVE) {
            percpu->schedFlag = INT_NO_RESCH;
            LOS_IntRestore(intSave);
            LOS_Schedule();
            return;
        }
    }

    LOS_IntRestore(intSave);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_TaskInfoGet(UINT32 taskId, TSK_INFO_S *taskInfo)
{
    UINT32 intSave;
    LosTaskCB *taskCB = NULL;

    if (taskInfo == NULL) {
        return LOS_ERRNO_TSK_PTR_NULL;
    }

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    SCHEDULER_LOCK(intSave);

    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        SCHEDULER_UNLOCK(intSave);
        return LOS_ERRNO_TSK_NOT_CREATED;
    }

    if (!(taskCB->taskStatus & OS_TASK_STATUS_RUNNING) || OS_INT_ACTIVE) {
        taskInfo->uwSP = (UINTPTR)taskCB->stackPointer;
    } else {
        taskInfo->uwSP = ArchSPGet();
    }

    taskInfo->usTaskStatus = taskCB->taskStatus;
    taskInfo->usTaskPrio = taskCB->priority;
    taskInfo->uwStackSize = taskCB->stackSize;
    taskInfo->uwTopOfStack = taskCB->topOfStack;
#ifdef LOSCFG_BASE_IPC_EVENT
    taskInfo->uwEvent = taskCB->event;
    taskInfo->uwEventMask = taskCB->eventMask;
#endif
    taskInfo->pTaskSem = taskCB->taskSem;
    taskInfo->pTaskMux = taskCB->taskMux;
    taskInfo->uwTaskID = taskId;

    if (strncpy_s(taskInfo->acName, LOS_TASK_NAMELEN, taskCB->taskName, LOS_TASK_NAMELEN - 1) != EOK) {
        PRINT_ERR("Task name copy failed!\n");
    }
    taskInfo->uwBottomOfStack = TRUNCATE(((UINTPTR)taskCB->topOfStack + taskCB->stackSize),
                                         OS_TASK_STACK_ADDR_ALIGN);
    taskInfo->uwCurrUsed = (UINT32)(taskInfo->uwBottomOfStack - taskInfo->uwSP);

    taskInfo->bOvf = OsStackWaterLineGet((const UINTPTR *)taskInfo->uwBottomOfStack,
                                         (const UINTPTR *)taskInfo->uwTopOfStack, &taskInfo->uwPeakUsed);
    SCHEDULER_UNLOCK(intSave);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_TaskCpuAffiSet(UINT32 taskId, UINT16 cpuAffiMask)
{
#ifdef LOSCFG_KERNEL_SMP
    LosTaskCB *taskCB = NULL;
    UINT32 intSave;
    BOOL needSched = FALSE;
    UINT16 currCpuMask;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return LOS_ERRNO_TSK_ID_INVALID;
    }

    if (!(cpuAffiMask & LOSCFG_KERNEL_CPU_MASK)) {
        return LOS_ERRNO_TSK_CPU_AFFINITY_MASK_ERR;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    if (taskCB->taskFlags & OS_TASK_FLAG_SYSTEM) {
        return LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK;
    }

    SCHEDULER_LOCK(intSave);
    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        SCHEDULER_UNLOCK(intSave);
        return LOS_ERRNO_TSK_NOT_CREATED;
    }

    taskCB->cpuAffiMask = cpuAffiMask;
    currCpuMask = CPUID_TO_AFFI_MASK(taskCB->currCpu);
    if (!(currCpuMask & cpuAffiMask)) {
        needSched = TRUE;
        taskCB->signal = SIGNAL_AFFI;
    }
    SCHEDULER_UNLOCK(intSave);

    if (needSched && OS_SCHEDULER_ACTIVE) {
        LOS_MpSchedule(currCpuMask);
        LOS_Schedule();
    }
#endif
    (VOID)taskId;
    (VOID)cpuAffiMask;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT16 LOS_TaskCpuAffiGet(UINT32 taskId)
{
#ifdef LOSCFG_KERNEL_SMP
#define INVALID_CPU_AFFI_MASK   0
    LosTaskCB *taskCB = NULL;
    UINT16 cpuAffiMask;
    UINT32 intSave;

    if (OS_TASK_ID_CHECK_INVALID(taskId)) {
        return INVALID_CPU_AFFI_MASK;
    }

    taskCB = OS_TCB_FROM_TID(taskId);
    SCHEDULER_LOCK(intSave);
    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        SCHEDULER_UNLOCK(intSave);
        return INVALID_CPU_AFFI_MASK;
    }

    cpuAffiMask = taskCB->cpuAffiMask;
    SCHEDULER_UNLOCK(intSave);

    return cpuAffiMask;
#else
    (VOID)taskId;
    return 1; /* 1: mask of current cpu */
#endif
}

/*
 * Description : Process pending signals tagged by others cores
 */
LITE_OS_SEC_TEXT_MINOR UINT32 OsTaskProcSignal(VOID)
{
    Percpu    *percpu = NULL;
    LosTaskCB *runTask = NULL;
    UINT32    ret;

    /*
     * private and uninterruptable, no protection needed.
     * while this task is always running when others cores see it,
     * so it keeps recieving signals while follow code excuting.
     */
    runTask = OsCurrTaskGet();
    if (runTask->signal == SIGNAL_NONE) {
        goto EXIT;
    }

    if (runTask->signal & SIGNAL_KILL) {
        /*
         * clear the signal, and do the task deletion. if the signaled task has been
         * scheduled out, then this deletion will wait until next run.
         */
        runTask->signal = SIGNAL_NONE;
        ret = LOS_TaskDelete(runTask->taskId);
        if (ret) {
            PRINT_ERR("%s: tsk del fail err:0x%x\n", __FUNCTION__, ret);
        }
    } else if (runTask->signal & SIGNAL_SUSPEND) {
        runTask->signal &= ~SIGNAL_SUSPEND;

        /* suspend killed task may fail, ignore the result */
        (VOID)LOS_TaskSuspend(runTask->taskId);
#ifdef LOSCFG_KERNEL_SMP
    } else if (runTask->signal & SIGNAL_AFFI) {
        runTask->signal &= ~SIGNAL_AFFI;

        /* pri-queue has updated, notify the target cpu */
        LOS_MpSchedule((UINT32)runTask->cpuAffiMask);
#endif
    }

EXIT:
    /* check if needs to schedule */
    percpu = OsPercpuGet();

    LOS_TRACE(TASK_SIGNAL, runTask->taskId, runTask->signal, percpu->schedFlag);

    if (OsPreemptable() && (percpu->schedFlag == INT_PEND_RESCH)) {
        percpu->schedFlag = INT_NO_RESCH;
        return INT_PEND_RESCH;
    }

    return INT_NO_RESCH;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
