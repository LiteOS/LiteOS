/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: CMSIS Interface V1.0
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "cmsis_os.h"
#include "securec.h"
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
#if (LOSCFG_CMSIS_VERSION == 1)

#define PRIORITY_WIN 4

INT32 osKernelRunning(void)
{
    UINT32 cpuID = ArchCurrCpuid();
    return (INT32)(g_taskScheduled & (1U << cpuID));
}

/* Start the RTOS Kernel with executing the specified thread */
osStatus osKernelStart(void)
{
    OsStart();
    return osOK;
}

UINT32 osKernelSysTick(void)
{
    return (UINT32)g_tickCount[0];
}

osStatus osKernelInitialize(void)
{
    UINT32 ret;

    ret = LOS_KernelInit();
    if (ret != LOS_OK) {
        return osErrorOS;
    }

    return osOK;
}

/* Create a thread and add it to Active Threads and set it to state READY */
osThreadId osThreadCreate(const osThreadDef_t *threadDef, void *argument)
{
    osThreadId taskCb;
    TSK_INIT_PARAM_S taskInitParam;
    UINT32 taskHandle;
    UINT32 ret;
    if ((threadDef == NULL) ||
        (threadDef->pthread == NULL) ||
        (threadDef->tpriority < osPriorityIdle) ||
        (threadDef->tpriority > osPriorityRealtime)) {
        return (osThreadId)NULL;
    }

    (VOID)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)threadDef->pthread;
    taskInitParam.uwStackSize  = threadDef->stacksize;
    taskInitParam.pcName       = threadDef->name;
    taskInitParam.auwArgs[0]   = (UINT32)(UINTPTR)argument;
    taskInitParam.usTaskPrio   = (UINT16)(PRIORITY_WIN - threadDef->tpriority); /* 1~7 */
    taskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED; /* the cmsis task is detached, the task can deleteself */

    ret = LOS_TaskCreate(&taskHandle, &taskInitParam);
    if (ret != LOS_OK) {
        return (osThreadId)NULL;
    }

    taskCb = (osThreadId)&g_taskCBArray[taskHandle];

    return taskCb;
}

#if (LOSCFG_KERNEL_USERSPACE == YES)
osThreadId osUsrThreadCreate(const osThreadDef_t *thread_def, VOID *stack_pointer, UINT32 stack_size, VOID *argument)
{
    TSK_INIT_PARAM_S taskInitParam;
    UINT32 taskHandle;
    UINT32 ret;
    if ((threadDef == NULL) ||
        (threadDef->pthread == NULL) ||
        (threadDef->tpriority < osPriorityIdle) ||
        (threadDef->tpriority > osPriorityRealtime)) {
        return (osThreadId)NULL;
    }

    (VOID)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry     = (TSK_ENTRY_FUNC)thread_def->pthread;
    taskInitParam.uwStackSize      = thread_def->stacksize;
    taskInitParam.pcName           = thread_def->name;
    taskInitParam.uwArg            = (UINT32)argument;
    taskInitParam.usTaskPrio       = (UINT16)(PRIORITY_WIN - thread_def->tpriority);  /* 1~7 */
    taskInitParam.uwResved         = OS_TASK_STATUS_USERSPACE;
    taskInitParam.pUserSP          = stack_pointer;
    taskInitParam.uwUserStackSize  = stack_size;

    ret = LOS_TaskCreate(&taskHandle, &taskInitParam);
    if (ret != LOS_OK) {
        return (osThreadId)NULL;
    }

    return (osThreadId)&g_taskCBArray[taskHandle];
}
#endif

/* Return the thread ID of the current running thread */
osThreadId osThreadGetId(void)
{
    return (osThreadId)g_runTask;
}

UINT32 osThreadGetPId(osThreadId threadId)
{
    return ((LosTaskCB *)threadId)->taskID;
}

/* Terminate execution of a thread and remove it from ActiveThreads */
osStatus osThreadTerminate(osThreadId threadId)
{
    UINT32 ret;

    if (threadId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskDelete(((LosTaskCB *)threadId)->taskID);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

/* Pass control to next thread that is in state READY */
osStatus osThreadYield(void)
{
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_TaskYield();
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

osStatus osThreadSetPriority(osThreadId threadId, osPriority priority)
{
    UINT32 ret;
    UINT16 priorityTemp;

    if (threadId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if ((priority < osPriorityIdle) || (priority > osPriorityRealtime)) {
        return osErrorPriority;
    }

    priorityTemp = (UINT16)(PRIORITY_WIN - priority);

    ret = LOS_TaskPriSet(((LosTaskCB *)threadId)->taskID, priorityTemp);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

osPriority osThreadGetPriority(osThreadId threadId)
{
    UINT16 priorityTemp;
    INT16 priorityRet;

    if (threadId == NULL) {
        return osPriorityError;
    }

    priorityTemp = LOS_TaskPriGet(((LosTaskCB *)threadId)->taskID);

    priorityRet = (INT16)(PRIORITY_WIN - (INT32)priorityTemp);
    if ((priorityRet < osPriorityIdle) || (priorityRet > osPriorityRealtime)) {
        return osPriorityError;
    }

    return (osPriority)priorityRet;
}

#if (LOSCFG_KERNEL_USERSPACE == YES)
osStatus osThreadSelfSuspend(void)
{
    UINT32 taskHandle = ((LosTaskCB *)g_runTask)->taskID;
    if (LOS_TaskSuspend(taskHandle) == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

osStatus osThreadResume(osThreadId threadId)
{
    UINT32 ret;
    UINT32 taskHandle = osThreadGetPId(threadId);
    ret = LOS_TaskResume(taskHandle);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}
#endif

osSemaphoreId osBinarySemaphoreCreate(const osSemaphoreDef_t *semaphoreDef, INT32 count)
{
#if (LOSCFG_BASE_IPC_SEM == YES)
    UINT32 ret;
    UINT32 *semHandle = NULL;
    LosSemCB *semCreated = NULL;

    if ((semaphoreDef == NULL) || (count > OS_NULL_SHORT) || (count < 0)) {
        return (osSemaphoreId)NULL;
    }

    semHandle = (UINT32 *)(UINTPTR)(semaphoreDef->puwSemHandle);
    if (semHandle == NULL) {
        return (osSemaphoreId)NULL;
    }
    ret = LOS_BinarySemCreate((UINT16)count, semHandle);
    if (ret == LOS_OK) {
        semCreated = GET_SEM(*semHandle);
        return semCreated;
    } else {
        return (osSemaphoreId)NULL;
    }
#endif
}

osSemaphoreId osSemaphoreCreate(const osSemaphoreDef_t *semaphoreDef, INT32 count)
{
#if (LOSCFG_BASE_IPC_SEM == YES)
    UINT32 ret;
    UINT32 *semHandle = NULL;

    if ((semaphoreDef == NULL) || (count > OS_NULL_SHORT) || (count < 0)) {
        return (osSemaphoreId)NULL;
    }

    semHandle = (UINT32 *)(UINTPTR)(semaphoreDef->puwSemHandle);
    ret = LOS_SemCreate((UINT16)count, semHandle);
    if (ret == LOS_OK) {
        return GET_SEM(*semHandle);
    } else {
        return NULL;
    }
#endif
}

/*
 * Wait until a Semaphore becomes available
 * return numbers of available tokens, or -1 in case of incorrect parameters.
 */
INT32 osSemaphoreWait(osSemaphoreId semaphoreId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_SEM == YES)
    UINT32 ret;
    UINT32 semId;

    if (semaphoreId == NULL) {
        return -1;
    }

    if (OS_INT_ACTIVE) {
        return -1;
    }

    semId = ((LosSemCB *)semaphoreId)->semID;

    ret = LOS_SemPend(semId, LOS_MS2Tick(millisec));
    if (ret == LOS_OK) {
        return ((LosSemCB *)semaphoreId)->semCount;
    } else {
        return -1;
    }
#else
    (VOID)semaphoreId;
    (VOID)millisec;
    return -1;
#endif
}

/*
 * Release a Semaphore
 * osOK: the semaphore has been released.
 * osErrorResource: all tokens have already been released.
 * osErrorParameter: the parameter semaphore_id is incorrect.
 */
osStatus osSemaphoreRelease(osSemaphoreId semaphoreId)
{
#if (LOSCFG_BASE_IPC_SEM == YES)
    UINT32 ret;
    UINT32 semId;

    if (semaphoreId == NULL) {
        return osErrorParameter;
    }

    semId = ((LosSemCB *)semaphoreId)->semID;
    ret = LOS_SemPost(semId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
#else
    (VOID)semaphoreId;
    return osErrorParameter;
#endif
}

/*
 * osOK: the semaphore object has been deleted.
 * osErrorISR: osSemaphoreDelete cannot be called from interrupt service routines.
 * osErrorResource: the semaphore object could not be deleted.
 * osErrorParameter: the parameter semaphore_id is incorrect.
 */
osStatus osSemaphoreDelete(osSemaphoreId semaphoreId)
{
#if (LOSCFG_BASE_IPC_SEM == YES)
    UINT32 ret;
    UINT32 semId;

    if (semaphoreId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    semId = ((LosSemCB *)semaphoreId)->semID;
    ret = LOS_SemDelete(semId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SEM_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
#else
    (VOID)semaphoreId;
    return osErrorParameter;
#endif
}

/*
 * Create and Initialize a Mutex object.
 * param[in]     mutex_def     mutex definition referenced with \ref osMutex.
 * return mutex ID for reference by other functions or NULL in case of error.
 * note MUST REMAIN UNCHANGED: \b osMutexCreate shall be consistent in every CMSIS-RTOS.
 */
osMutexId osMutexCreate(const osMutexDef_t *mutexDef)
{
#if (LOSCFG_BASE_IPC_MUX == YES)
    UINT32 ret;
    UINT32 *muxHandle = NULL;

    if (mutexDef == NULL) {
        return NULL;
    }

    muxHandle = (UINT32 *)(UINTPTR)(mutexDef->puwMuxHandle);
    if (muxHandle == NULL) {
        return NULL;
    }
    ret = LOS_MuxCreate(muxHandle);
    if (ret == LOS_OK) {
        return GET_MUX(*muxHandle);
    } else {
        return NULL;
    }
#else
    (VOID)mutexDef;
    return NULL;
#endif
}

/*
 * Wait until a Mutex becomes available.
 * param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
 * param[in]     millisec      timeout value or 0 in case of no time-out.
 * return status code that indicates the execution status of the function.
 * note MUST REMAIN UNCHANGED: \b osMutexWait shall be consistent in every CMSIS-RTOS.
 * osOK: the mutex has been obtain.
 * osErrorTimeoutResource: the mutex could not be obtained in the given time.
 * osErrorResource: the mutex could not be obtained when no timeout was specified.
 * osErrorParameter: the parameter mutex_id is incorrect.
 * osErrorISR: osMutexWait cannot be called from interrupt service routines.
 */
osStatus osMutexWait(osMutexId mutexId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_MUX == YES)
    UINT32 ret;
    UINT32 mutId;

    if (mutexId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    mutId = ((LosMuxCB*)mutexId)->muxID;

    ret = LOS_MuxPend(mutId, LOS_MS2Tick(millisec));
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        return osErrorTimeoutResource;
    } else if (ret == LOS_ERRNO_MUX_UNAVAILABLE) {
        return osErrorResource;
    } else if (ret == LOS_ERRNO_MUX_PEND_INTERR) {
        return osErrorISR;
    } else {
        return osErrorParameter;
    }
#else
    (VOID)mutexId;
    (VOID)millisec;
    return osErrorParameter;
#endif
}

/*
 * Release a Mutex that was obtained by \ref osMutexWait.
 * param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
 * return status code that indicates the execution status of the function.
 * note MUST REMAIN UNCHANGED: \b osMutexRelease shall be consistent in every CMSIS-RTOS.
 * osOK: the mutex has been correctly released.
 * osErrorResource: the mutex was not obtained before.
 * osErrorParameter: the parameter mutex_id is incorrect.
 * osErrorISR: osMutexRelease cannot be called from interrupt service routines.
 */
osStatus osMutexRelease(osMutexId mutexId)
{
#if (LOSCFG_BASE_IPC_MUX == YES)
    UINT32 ret;
    UINT32 mutId;

    if (mutexId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    mutId = ((LosMuxCB*)mutexId)->muxID;
    ret = LOS_MuxPost(mutId);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorResource;
    }
#else
    (VOID)mutexId;
    return osErrorParameter;
#endif
}

/*
 * Delete a Mutex that was created by \ref osMutexCreate.
 * param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
 * return status code that indicates the execution status of the function.
 * note MUST REMAIN UNCHANGED: \b osMutexDelete shall be consistent in every CMSIS-RTOS.
 * osOK: the mutex object has been deleted.
 * osErrorISR: osMutexDelete cannot be called from interrupt service routines.
 * osErrorResource: all tokens have already been released.
 * osErrorParameter: the parameter mutex_id is incorrect.
 */
osStatus osMutexDelete(osMutexId mutexId)
{
#if (LOSCFG_BASE_IPC_MUX == YES)
    UINT32 ret;
    UINT32 mutId;

    if (mutexId == NULL) {
        return osErrorParameter;
    }

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    mutId = ((LosMuxCB*)mutexId)->muxID;
    ret = LOS_MuxDelete(mutId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorResource;
    } else {
        return osErrorParameter;
    }
#else
    (VOID)mutexId;
    return osErrorParameter;
#endif
}

osPoolId osPoolCreate(const osPoolDef_t *poolDef)
{
    UINT32 blkSize;
    UINT32 boxSize;
    UINT32 ret;

    if ((poolDef == NULL) ||
        (poolDef->pool_sz == 0) ||
        (poolDef->item_sz == 0) ||
        (poolDef->pool == NULL)) {
        return (osPoolId)NULL;
    }

    blkSize = (poolDef->item_sz + 3) & (~3); /* 3:the number 3 is for align 4 bytes */
    boxSize = LOS_MEMBOX_SIZE(blkSize, poolDef->pool_sz);

    ret = LOS_MemboxInit(poolDef->pool, boxSize, blkSize);
    if (ret != LOS_OK) {
        return (osPoolId)NULL;
    }

    return (osPoolId)(poolDef->pool);
}

void *osPoolAlloc(osPoolId poolId)
{
    void *ptr = NULL;

    if (poolId == NULL) {
        return NULL;
    }

    ptr = LOS_MemboxAlloc(poolId);

    return ptr;
}

void *osPoolCAlloc(osPoolId poolId)
{
    void *ptr = NULL;

    if (poolId == NULL) {
        return NULL;
    }

    ptr = LOS_MemboxAlloc(poolId);
    if (ptr) {
        LOS_MemboxClr(poolId, ptr);
    }

    return ptr;
}

osStatus osPoolFree(osPoolId poolId, void *block)
{
    UINT32 res;

    if (poolId == NULL) {
        return osErrorParameter;
    }

    res = LOS_MemboxFree(poolId, block);
    if (res != 0) {
        return osErrorValue;
    }

    return osOK;
}

STATIC_INLINE UINT32 osMessageCheckRet(UINT32 ret)
{
    if (ret == LOS_OK) {
        ret = osOK;
    } else if (ret == LOS_ERRNO_QUEUE_INVALID || ret == LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT) {
        ret = osErrorParameter;
    } else if (ret == LOS_ERRNO_QUEUE_TIMEOUT || ret == LOS_ERRNO_QUEUE_ISFULL) {
        ret = osEventTimeout;
    } else {
        ret = osErrorOS;
    }
    return ret;
}

osMessageQId osMessageCreate(osMessageQDef_t *queueDef, osThreadId threadId)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 queueId;
    UINT32 ret;

    (void)(threadId);
    if (queueDef == NULL) {
        return (osMessageQId)NULL;
    }
    ret = LOS_QueueCreate((char *)NULL, (UINT16)(queueDef->queue_sz), &queueId, 0, (UINT16)(queueDef->item_sz));
    if (ret == LOS_OK) {
        return (osMessageQId)GET_QUEUE_HANDLE(queueId);
    } else {
        return (osMessageQId)NULL;
    }
#else
    (VOID)queueDef;
    (VOID)threadId;
    return (osMessageQId)NULL;
#endif
}

osStatus osMessagePutHead(const osMessageQId queueId, UINT32 info, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 ret;
    ret = LOS_QueueWriteHead(MESSAGEQID_TO_QUEUEID(queueId),
        (void *)(UINTPTR)info, sizeof(UINT32), LOS_MS2Tick(millisec));
    ret = osMessageCheckRet(ret);
    return (osStatus)ret;
#else
    (VOID)queueId;
    (VOID)info;
    (VOID)millisec;
    return (osStatus)osErrorParameter;
#endif
}

static UINT32 osMessagePutCheckRet(UINT32 ret)
{
    UINT32 result;
    if (ret == LOS_OK) {
        result = osOK;
    } else if ((ret == LOS_ERRNO_QUEUE_INVALID) || (ret == LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT)) {
        result = osErrorParameter;
    } else if ((ret == LOS_ERRNO_QUEUE_TIMEOUT) || (ret == LOS_ERRNO_QUEUE_ISFULL)) {
        result = osEventTimeout;
    } else {
        result = osErrorOS;
    }

    return result;
}

osStatus osMessagePut(const osMessageQId queueId, UINT32 info, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 ret;
    if (queueId == NULL) {
        return osErrorParameter;
    }

#if (LOSCFG_BASE_CORE_SWTMR == YES)
    if (MESSAGEQID_TO_QUEUEID(queueId) == 0) {
        return osErrorParameter;
    }
#endif

    ret = LOS_QueueWrite(MESSAGEQID_TO_QUEUEID(queueId), 
        (void*)(UINTPTR)info, sizeof(UINT32), LOS_MS2Tick(millisec));
    ret = osMessagePutCheckRet(ret);
    return (osStatus)ret;
#else
    (VOID)queue_id;
    (VOID)info;
    (VOID)millisec;
    return (osStatus)osErrorParameter;
#endif
}

static UINT32 osMessageGetCheckRet(UINT32 ret)
{
    UINT32 result;
    if (ret == LOS_OK) {
        result = osEventMessage;
    } else if ((ret == LOS_ERRNO_QUEUE_INVALID) || (ret == LOS_ERRNO_QUEUE_READ_IN_INTERRUPT)) {
        result = osErrorParameter;
    } else if ((ret == LOS_ERRNO_QUEUE_TIMEOUT) || (ret == LOS_ERRNO_QUEUE_ISEMPTY)) {
        result = osEventTimeout;
    } else {
        result = osErrorOS;
    }

    return result;
}

/* Get a Message or Wait for a Message from a Queue */
osEvent osMessageGet(osMessageQId queueId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 ret;
    osEvent retEvent;

    if (queueId == NULL) {
        retEvent.status = osErrorParameter;
        return retEvent;
    }
    (VOID)memset_s((VOID *)(&retEvent), sizeof(osEvent), 0, sizeof(osEvent));
    ret = LOS_QueueRead(MESSAGEQID_TO_QUEUEID(queueId), &(retEvent.value.v), sizeof(UINT32), LOS_MS2Tick(millisec));
    retEvent.status = osMessageGetCheckRet(ret);

    return retEvent;
#else
    (VOID)queue_id;
    (VOID)millisec;
    osEvent retEvent;
    retEvent.status = osErrorParameter;
    return retEvent;
#endif
}

/* Create and Initialize mail queue */
osMailQId osMailCreate(osMailQDef_t *queueDef, osThreadId threadId)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 ret;
    UINT32 queueId;
    UINT32 blkSize;
    UINT32 boxSize;

    (void)(threadId);
    if (queueDef == NULL) {
        return (osMailQId)NULL;
    }

    if (!queueDef->pool) {
        return (osMailQId)NULL;
    }

    ret = LOS_QueueCreate((char *)NULL, (UINT16)(queueDef->queue_sz), &queueId, 0, sizeof(UINT32));
    if (ret == LOS_OK) {
        *(UINT32 *)(UINTPTR)((void **)(UINTPTR)queueDef->pool) = (UINT32)(UINTPTR)(GET_QUEUE_HANDLE(queueId));
        blkSize = (queueDef->item_sz + 3) & (~3); /* 3:the number 3 is for align 4 bytes */
        boxSize = LOS_MEMBOX_SIZE(blkSize, queueDef->queue_sz);

        (void)LOS_MemboxInit(*(((void **)queueDef->pool) + 1), boxSize, blkSize);
        return (osMailQId)queueDef->pool;
    }
    return (osMailQId)NULL;
#else
    (VOID)queue_def;
    (VOID)thread_id;
    return (osMailQId)NULL;
#endif
}

/* Allocate a memory block from a mail */
void *osMailAlloc(osMailQId queueId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    void *pool = NULL;
    UINT32 id;

    if (queueId == NULL) {
        return NULL;
    }

    id = *((UINT32 *)(UINTPTR)((void **)(UINTPTR)queueId));
    pool = *((((void **)(UINTPTR)queueId) + 1));

    return (void *)OsQueueMailAlloc(MESSAGEQID_TO_QUEUEID((UINTPTR)id), pool, LOS_MS2Tick(millisec));
#else
    (VOID)queueId;
    (VOID)millisec;
    return NULL;
#endif
}

/* Allocate a memory block from a mail and set memory block to zero */
void *osMailCAlloc(osMailQId queueId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    void *mem = NULL;
    OS_MEMBOX_S *pool = NULL;

    mem = osMailAlloc(queueId, millisec);
    if (mem != NULL) {
        pool = (OS_MEMBOX_S *)(*(((void **)(UINTPTR)queueId) + 1));
        (VOID)memset_s(mem, pool->uwBlkSize, 0, pool->uwBlkSize);
    }

    return mem;
#else
    (VOID)queue_id;
    (VOID)millisec;
    return NULL;
#endif
}

/* Free a memory block from a mail */
osStatus osMailFree(osMailQId queueId, void *mail)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    void *pool = NULL;
    UINT32 id;
    UINT32 ret;

    if (queueId == NULL) {
        return osErrorParameter;
    }

    id = *((UINT32 *)(UINTPTR)((void **)(UINTPTR)queueId));
    pool = *((((void **)(UINTPTR)queueId) + 1));

    ret = OsQueueMailFree(MESSAGEQID_TO_QUEUEID((UINTPTR)id), pool, mail);
    if (ret == LOS_ERRNO_QUEUE_MAIL_HANDLE_INVALID || ret == LOS_ERRNO_QUEUE_MAIL_PTR_INVALID) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_QUEUE_MAIL_FREE_ERROR) {
        return osErrorOS;
    }
    return osOK;
#else
    (VOID)queue_id;
    (VOID)mail;
    return osErrorParameter;
#endif
}

/* Put a mail to a queue Header */
osStatus osMailPutHead(osMailQId queueId, const void *mail)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 id;

    if (queueId == NULL) {
        return osErrorParameter;
    }

    if (mail == NULL) {
        return osErrorValue;
    }

    id = *((UINT32 *)(UINTPTR)((void **)(UINTPTR)queueId));

    return osMessagePutHead ((osMessageQId)(UINTPTR)id, (UINT32)(UINTPTR)mail, 0);
#else
    (VOID)queue_id;
    (VOID)mail;
    return osErrorParameter;
#endif
}

/* Put a mail to a queue */
osStatus osMailPut(osMailQId queueId, void *mail)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 id;

    if (queueId == NULL) {
        return osErrorParameter;
    }

    if (mail == NULL) {
        return osErrorValue;
    }

    id = *((UINT32 *)(UINTPTR)((void **)(UINTPTR)queueId));

    return osMessagePut ((osMessageQId)(UINTPTR)id, (UINT32)(UINTPTR)mail, 0);
#else
    (VOID)queue_id;
    (VOID)mail;
    return osErrorParameter;
#endif
}

/* Get a mail from a queue */
osEvent osMailGet(osMailQId queueId, UINT32 millisec)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    UINT32 id;
    osEvent ret;

    if (queueId == NULL) {
        ret.status = osErrorParameter;
        return ret;
    }

    id = *((UINT32 *)(UINTPTR)((void **)(UINTPTR)queueId));
    ret = osMessageGet((osMessageQId)(UINTPTR)id, millisec);
    if (ret.status == osEventMessage) {
        ret.status = osEventMail;
    }
    return ret;
#else
    (VOID)queue_id;
    (VOID)millisec;
    osEvent ret;
    ret.status = osErrorParameter;
    return ret;
#endif
}

osStatus osMailClear(osMailQId queueId)
{
#if (LOSCFG_BASE_IPC_QUEUE == YES)
    osEvent evt;
    UINTPTR intSave;
    intSave = LOS_IntLock();
    for (; ;) {
        evt = osMailGet(queueId, 0);
        if (evt.status == osEventMail) {
            (VOID)osMailFree(queueId, evt.value.p);
        } else if (evt.status == osEventTimeout) {
            LOS_IntRestore(intSave);
            return osOK;
        } else {
            LOS_IntRestore(intSave);
            return evt.status;
        }
    }
#else
    (VOID)queueId;
    return osErrorParameter;
#endif
}

INT32 osSignalSet(osThreadId threadId, INT32 signals)
{
    EVENT_CB_S sig;
    UINT32 oldSig;
    UINT32 ret;
    UINTPTR intSave;

    if (threadId == NULL) {
        return (INT32)0x80000000;
    }

    if (((UINT32)signals) & (~((0x1 << osFeature_Signals) - 1))) {
        return osErrorValue;
    }

    intSave = LOS_IntLock();
    sig = ((LosTaskCB *)threadId)->event;
    oldSig = sig.uwEventID;
    if (sig.uwEventID == 0xFFFFFFFF) {
        ret = LOS_EventInit(&(((LosTaskCB *)threadId)->event));
        if (ret != LOS_OK) {
            LOS_IntRestore(intSave);
            return osErrorOS;
        }
    }
    LOS_IntRestore(intSave);

    ret = LOS_EventWrite(&(((LosTaskCB *)threadId)->event), (UINT32)signals);
    if (ret != LOS_OK) {
        return osErrorOS;
    }

    return (INT32)oldSig;
}

INT32 osSignalClear(osThreadId threadId, INT32 signals)
{
    EVENT_CB_S sig;
    UINT32 oldSig;
    UINT32 ret;

    if (threadId == NULL) {
        return (INT32)0x80000000;
    }

    if (((UINT32)signals) & (~((0x1 << osFeature_Signals) - 1))) {
        return osErrorValue;
    }

    sig = ((LosTaskCB *)threadId)->event;
    oldSig = sig.uwEventID;
    ret = LOS_EventClear(&(((LosTaskCB *)threadId)->event), ~(UINT32)signals);
    if (ret != LOS_OK) {
        return osErrorValue;
    }

    return (INT32)oldSig;
}

STATIC_INLINE VOID osSignalWaitRetCheck(UINT32 ret, osEvent *evt)
{
    evt->value.signals = 0;
    if (ret == LOS_ERRNO_EVENT_READ_TIMEOUT) {
        evt->status = osEventTimeout;
    } else if (ret == 0) {
        evt->status = osOK;
    } else if ((ret == LOS_ERRNO_EVENT_PTR_NULL) || (ret == LOS_ERRNO_EVENT_EVENTMASK_INVALID) ||
        (ret == LOS_ERRNO_EVENT_READ_IN_LOCK) || (ret == LOS_ERRNO_EVENT_READ_IN_INTERRUPT)) {
        evt->status = osErrorOS;
    } else {
        evt->status = osEventSignal;
        evt->value.signals = (INT32)ret;
    }
}

osEvent osSignalWait(INT32 signals, UINT32 millisec)
{
    UINT32 ret;
    UINTPTR intSave;
    osEvent evt;
    UINT32 flags = 0;
    UINT32 timeOut;
    EVENT_CB_S sig;
    LosTaskCB *runTsk = NULL;

    if (OS_INT_ACTIVE) {
        /* Not allowed in ISR */
        evt.status = osErrorISR;
        return evt;
    }

    if (((UINT32)signals) & (~((0x1 << osFeature_Signals) - 1))) {
        evt.status = osErrorValue;
        return evt;
    }

    if (signals != 0) {
        flags |= LOS_WAITMODE_AND;
    } else {
        signals = (INT32)(0xFFFFFFFF & ((0x1 << osFeature_Signals) - 1));
        flags |= LOS_WAITMODE_OR;
    }

    timeOut = LOS_MS2Tick(millisec);

    intSave = LOS_IntLock();
    runTsk = (LosTaskCB *)g_runTask;
    sig = (runTsk)->event;
    if (sig.uwEventID == 0xFFFFFFFF) {
        ret = LOS_EventInit(&(((LosTaskCB *)g_runTask)->event));
        if (ret != LOS_OK) {
            evt.status = osErrorOS;
            LOS_IntRestore(intSave);
            return evt;
        }
    }
    LOS_IntRestore(intSave);
    ret = LOS_EventRead(&(((LosTaskCB *)g_runTask)->event),
        (UINT32)signals, flags | LOS_WAITMODE_CLR, timeOut);

    osSignalWaitRetCheck(ret, &evt);

    return evt;
}

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
osTimerId osTimerExtCreate(const osTimerDef_t *timerDef, os_timer_type type,
    void *argument, os_timer_rouses_type rouses, os_timer_align_type sensitive)
{
    SWTMR_CTRL_S *swTmr = NULL;
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    UINT32 ret;
    UINT16 swTmrId;

    if ((timerDef == NULL) ||
        (timerDef->ptimer == NULL) ||
        (timerDef->default_interval == 0) ||
        ((type != osTimerOnce) && (type != osTimerPeriodic))) {
        return NULL;
    }

    ret = LOS_SwtmrCreate(timerDef->default_interval, type,
                          (SWTMR_PROC_FUNC)(timerDef->ptimer),
                          &swTmrId, (UINT32)(UINTPTR)argument, rouses, sensitive);
    if (ret != LOS_OK) {
        return NULL;
    }

    swTmr = OS_SWT_FROM_SID(swTmrId);
#else
    (VOID)timerDef;
    (VOID)type;
    (VOID)argument;
    (VOID)rouses;
    (VOID)sensitive;
#endif
    return swTmr;
}
#endif

osTimerId osTimerCreate(const osTimerDef_t *timerDef, os_timer_type type, void *argument)
{
    SWTMR_CTRL_S *swTmr = NULL;
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    UINT32 ret;
    UINT16 swTmrId;

    if ((timerDef == NULL) ||
        (timerDef->ptimer == NULL) ||
        (timerDef->default_interval == 0) ||
        ((type != osTimerOnce) && (type != osTimerPeriodic) && (type != osTimerDelay))) {
        return (osTimerId)NULL;
    }

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    ret = LOS_SwtmrCreate(timerDef->default_interval, type,
                          (SWTMR_PROC_FUNC)(timerDef->ptimer),
                          &swTmrId, (UINT32)(UINTPTR)argument,
                          osTimerRousesAllow, osTimerAlignIgnore);
#else
    ret = LOS_SwtmrCreate(timerDef->default_interval, type,
                          (SWTMR_PROC_FUNC)(timerDef->ptimer),
                          &swTmrId, (UINT32)(UINTPTR)argument);
#endif
    if (ret != LOS_OK) {
        return (osTimerId)NULL;
    }

    swTmr = OS_SWT_FROM_SID(swTmrId);
#else
    (VOID)timerDef;
    (VOID)type;
    (VOID)argument;
#endif
    return swTmr;
}

osStatus osTimerStart(osTimerId timerId, UINT32 millisec)
{
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    SWTMR_CTRL_S *swTmr = NULL;
    UINT32 interval;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    swTmr = (SWTMR_CTRL_S *)timerId;
    if (swTmr == NULL) {
        return osErrorParameter;
    }

    interval = LOS_MS2Tick(millisec);
    if (interval == 0) {
        return osErrorValue;
    }

    swTmr->uwInterval = interval;
    swTmr->uwExpiry = interval;
    if (LOS_SwtmrStart(swTmr->usTimerID)) {
        return osErrorResource;
    }
#else
    (VOID)timerId;
    (VOID)millisec;
#endif
    return osOK;
}

osStatus osTimerStop(osTimerId timerId)
{
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    SWTMR_CTRL_S *swTmr = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    swTmr = (SWTMR_CTRL_S *)timerId;
    if (swTmr == NULL) {
        return osErrorParameter;
    }

    if (LOS_SwtmrStop(swTmr->usTimerID)) {
        return osErrorResource;
    }
#else
    (VOID)timerId;
#endif
    return osOK;
}

osStatus osTimerRestart(osTimerId timerId, UINT32 millisec, UINT8 strict)
{
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    osStatus status;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    status = osTimerStop(timerId);
    if (strict && (status != osOK)) {
        return status;
    }

    status = osTimerStart(timerId, millisec);
    if (status != osOK) {
        return status;
    }
#else
    (VOID)timerId;
    (VOID)millisec;
    (VOID)strict;
#endif
    return osOK;
}

osStatus osTimerDelete(osTimerId timerId)
{
#if (LOSCFG_BASE_CORE_SWTMR == YES)
    SWTMR_CTRL_S *swTmr = NULL;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    swTmr = (SWTMR_CTRL_S *)timerId;
    if (swTmr == NULL) {
        return osErrorParameter;
    }

    if (LOS_SwtmrDelete(swTmr->usTimerID)) {
        return osErrorResource;
    }
#else
    (VOID)timerId;
#endif
    return osOK;
}

osStatus osDelay(UINT32 millisec)
{
    UINT32   interval;
    UINT32 ret;
    Percpu *perCpu = OsPercpuGet();
    UINT32 idleTaskID = perCpu->idleTaskID;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (millisec == 0) {
        return osOK;
    }

    /* check if in idle we use udelay instead */
    if (idleTaskID == LOS_CurTaskIDGet()) {
        PRINT_ERR("Idle Task Do Not Support Delay!\n");
        return osOK;
    }

    interval = LOS_MS2Tick(millisec);
    ret = LOS_TaskDelay(interval);
    if (ret == LOS_OK) {
        return osEventTimeout;
    } else {
        return osErrorResource;
    }
}

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))
osEvent osWait(UINT32 millisec)
{
    osEvent evt;
    UINT32 interval;
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        evt.status = osErrorISR;
        return evt;
    }

    if (millisec == 0) {
        evt.status = osOK;
        return evt;
    }

    interval = LOS_MS2Tick(millisec);
    ret = LOS_TaskDelay(interval);
    if (ret == LOS_OK) {
        evt.status = osEventTimeout;
    } else {
        evt.status = osErrorResource;
    }

    return evt;
}
#endif

#if (LOSCFG_COMPAT_CMSIS_FW == YES)
fwMailQId g_fwMailQList = (fwMailQId)NULL;
UINT32 g_maxEventTime = 0x400;
#endif

fwMailQId fwMailCreate(fwMailQDef_t *queueDef, osThreadId threadId)
{
    if (queueDef == NULL) {
        return (fwMailQId)NULL;
    }
#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    /* add mailQ to list */
    if (queueDef->queue_id == NULL) {
        return (fwMailQId)NULL;
    }
    (VOID)osMailCreate(queueDef->queue_id, threadId);
    queueDef->next = (struct fw_MailQ_def *)g_fwMailQList;
    g_fwMailQList = (fwMailQId)queueDef;
    return (fwMailQId)queueDef;
#else
    return osMailCreate(queueDef, threadId);
#endif
}

VOID *fwMailAlloc(fwMailQId queueId, UINT32 millisec, UINT8 tag, UINT8 cmd)
{
    VOID *mem = NULL;

    if (queueId == NULL) {
        return NULL;
    }

#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    osMailQDef_t *pQueueId = ((fwMailQDef_t *)queueId)->queue_id;
    if (pQueueId == NULL) {
        return NULL;
    }

    if (pQueueId->pool == NULL) {
        return NULL;
    }
    mem = osMailAlloc((osMailQId)(pQueueId)->pool, millisec);
#else
    mem = osMailAlloc(queueId, millisec);
#endif
    if (mem != NULL) {
        ((fw_event_t*)mem)->cmd = cmd;
        ((fw_event_t*)mem)->tag = tag;
    }

    return mem;
}

VOID *fwMailCAlloc(fwMailQId queueId, UINT32 millisec, UINT8 tag, UINT8 cmd)
{
    VOID *mem = NULL;

    if (queueId == NULL) {
        return NULL;
    }
#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    osMailQDef_t *pQueueId = ((fwMailQDef_t *)queueId)->queue_id;
    if (pQueueId == NULL) {
        return NULL;
    }
    if (pQueueId->pool == NULL) {
        return NULL;
    }
    mem = osMailCAlloc((osMailQId)(pQueueId)->pool, millisec);
#else
    mem = osMailCAlloc(queue_id, millisec);
#endif
    if (mem != NULL) {
        ((fw_event_t*)mem)->cmd = cmd;
        ((fw_event_t*)mem)->tag = tag;
    }

    return mem;
}

osStatus fwMailFree(fwMailQId queueId, VOID *mail)
{
    if (queueId == NULL) {
        return osErrorParameter;
    }

#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    osMailQDef_t *pQueueId = ((fwMailQDef_t *)queueId)->queue_id;
    if (pQueueId == NULL) {
        return osErrorParameter;
    }

    if (pQueueId->pool == NULL) {
        return osErrorParameter;
    }

    return osMailFree((osMailQId)(pQueueId)->pool, mail);
#else
    return osMailFree(queueId, mail);
#endif
}

osStatus fwMailPut(fwMailQId queueId, VOID *mail)
{
    if (queueId == NULL) {
        return osErrorParameter;
    }

#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    osMailQDef_t *pQueueId = ((fwMailQDef_t *)queueId)->queue_id;
    if (pQueueId == NULL) {
        return osErrorParameter;
    }

    if (pQueueId->pool == NULL) {
        return osErrorParameter;
    }
    return osMailPut((osMailQId)(pQueueId)->pool, mail);
#else
    return osMailPut(queueId, mail);
#endif
}

osEvent fwMailGet(fwMailQId queueId, UINT32 millisec)
{
    osEvent evt;
#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    UINT32 maxTime;
    VOID *pool = NULL;
    UINT32 id;
#endif

    evt.status = osErrorParameter;
    if (queueId == NULL) {
        return evt;
    }
#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    osMailQDef_t *pQueueId = ((fwMailQDef_t *)queueId)->queue_id;
    if (pQueueId == NULL) {
        return evt;
    }

    if (pQueueId->pool == NULL) {
        return evt;
    }

    id = *((UINT32*)((VOID **)pool));
    maxTime = (GET_EVENT_MAXTIME(queueId) != 0) ? GET_EVENT_MAXTIME(queueId) : g_maxEventTime;
    if ((((fwMailQDef_t *)queueId)->event_begin_time != 0) &&
        ((osKernelSysTick() - ((fwMailQDef_t *)queueId)->event_begin_time) > maxTime)) {
        ((fwMailQDef_t *)queueId)->timeout_cnt++;
        PRINT_ERR("[ERR] Get QID %d TIMEOUTCNT %d\n", id, ((fwMailQDef_t *)queueId)->timeout_cnt);
    }

    ((fwMailQDef_t *)queueId)->event_begin_time = 0;
    evt = osMailGet((osMailQId)((((fwMailQDef_t *)queueId)->queue_id)->pool), millisec);
    if (evt.status == osEventMail) {
        ((fwMailQDef_t *)queueId)->last_event = *(fw_event_t*)(evt.value.p);
        ((fwMailQDef_t *)queueId)->event_begin_time = osKernelSysTick();
    }
#else
    evt = osMailGet(queueId, millisec);
#endif
    return evt;
}

UINT32 fwMailQGetStatus(VOID)
{
#if (LOSCFG_COMPAT_CMSIS_FW == YES)
    fwMailQDef_t *mailQueue = (fwMailQDef_t *)NULL;
    VOID *pool = NULL;
    UINT32 id;
    UINT32 maxTime;
    UINT32 currTick = osKernelSysTick();
    UINT32 ret = 0;

    mailQueue = (fwMailQDef_t *)g_fwMailQList;
    while (mailQueue != NULL) {
        maxTime = (GET_EVENT_MAXTIME(mailQueue) != 0) ? GET_EVENT_MAXTIME(mailQueue) : g_maxEventTime;
        if (mailQueue->queue_id == NULL) {
            return 0;
        }

        pool = (mailQueue->queue_id)->pool;
        if (pool == NULL) {
            return 0;
        }

        id = *((UINT32*)((VOID **)pool));
        if ((mailQueue->event_begin_time != 0) && ((currTick - mailQueue->event_begin_time) > maxTime)) {
            mailQueue->timeout_cnt++;
            PRINT_ERR("[ERR] QID %d OUTQUE %d Phase %d\n", id, mailQueue->event_begin_time,
                      GET_EVENT_PHASE(mailQueue));
            PRINT_ERR("TAG %d CMD %d\n", mailQueue->last_event.tag, mailQueue->last_event.cmd);
            ret++;
        }
        if (mailQueue->timeout_cnt != 0) {
            PRINT_ERR("QID %d TIMEOUTCNT %d\n", id, mailQueue->timeout_cnt);
            ret += mailQueue->timeout_cnt;
            mailQueue->timeout_cnt = 0;
        }
        mailQueue = mailQueue->next;
    }

    return (ret > 0) ? 1 : 0;
#else
    return 0;
#endif
}
#endif // (LOSCFG_CMSIS_VERSION == 1)
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
