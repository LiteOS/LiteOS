/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: CMSIS Interface V1.0
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

#include "cmsis_os.h"
#include "securec.h"
#include "los_printf.h"
#include "los_membox.h"
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

#if (CMSIS_OS_VER == 1)

#define PRIORITY_WIN 4

//  ==== Kernel Control Functions ====
osStatus osKernelInitialize(void)
{
    UINT32 ret;

    ret = OsMain();
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

osStatus osKernelStart(void)
{
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    OsStart();
    return osOK;
}

int32_t osKernelRunning(void)
{
    return (int32_t)g_taskScheduled;
}

uint32_t osKernelSysTick(void)
{
    return (uint32_t)LOS_TickCountGet();
}


//  ==== Thread Management ====
osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argument)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam;

    if ((thread_def == NULL) ||
        (thread_def->pthread == NULL) ||
        (thread_def->tpriority < osPriorityIdle) ||
        (thread_def->tpriority > osPriorityRealtime)) {
        return NULL;
    }

    (VOID)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)thread_def->pthread;
    taskInitParam.uwStackSize  = thread_def->stacksize;
    taskInitParam.pcName       = thread_def->name;
    taskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    taskInitParam.usTaskPrio   = (UINT16)(PRIORITY_WIN - thread_def->tpriority);  /* 1~7 */
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, argument);

    ret = LOS_TaskCreate(&taskId, &taskInitParam);
    if (ret == LOS_OK) {
        return (osThreadId)OS_TCB_FROM_TID(taskId);
    } else {
        return NULL;
    }
}

#if (LOSCFG_KERNEL_USERSPACE == YES)
osThreadId osUsrThreadCreate(const osThreadDef_t *thread_def, void *stackPointer, UINT32 stackSize, void *argument)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam;

    if ((thread_def == NULL) ||
        (thread_def->pthread == NULL) ||
        (thread_def->tpriority < osPriorityIdle) ||
        (thread_def->tpriority > osPriorityRealtime)) {
        return NULL;
    }

    (VOID)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry     = (TSK_ENTRY_FUNC)thread_def->pthread;
    taskInitParam.uwStackSize      = thread_def->stacksize;
    taskInitParam.pcName           = thread_def->name;
    taskInitParam.usTaskPrio       = (UINT16)(PRIORITY_WIN - thread_def->tpriority);  /* 1~7 */
    taskInitParam.uwResved         = OS_TASK_STATUS_USERSPACE;
    taskInitParam.pUserSP          = stackPointer;
    taskInitParam.uwUserStackSize  = stackSize;
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, argument);

    ret = LOS_TaskCreate(&taskId, &taskInitParam);
    if (ret == LOS_OK) {
        return (osThreadId)OS_TCB_FROM_TID(taskId);
    } else {
        return NULL;
    }
}
#endif

osThreadId osThreadGetId(void)
{
    return (osThreadId)OsCurrTaskGet();
}

UINT32 osThreadGetPId(osThreadId thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;

    if (taskCB == NULL) {
        return (UINT32)-1;
    }

    return taskCB->taskId;
}

osStatus osThreadTerminate(osThreadId thread_id)
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
    } else if ((ret == LOS_ERRNO_TSK_ID_INVALID) || (ret == LOS_ERRNO_TSK_NOT_CREATED)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

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

osStatus osThreadSetPriority(osThreadId thread_id, osPriority priority)
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
    if ((priority < osPriorityIdle) || (priority > osPriorityRealtime)) {
        return osErrorValue;
    }

    losPrio = (UINT16)(PRIORITY_WIN - priority);
    ret = LOS_TaskPriSet(taskCB->taskId, losPrio);
    if (ret == LOS_OK) {
        return osOK;
    } else if ((ret == LOS_ERRNO_TSK_ID_INVALID) || (ret == LOS_ERRNO_TSK_NOT_CREATED)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osPriority osThreadGetPriority(osThreadId thread_id)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT16 losPrio;
    INT16 priorityRet;

    if ((taskCB == NULL) || OS_INT_ACTIVE) {
        return osPriorityError;
    }

    losPrio = LOS_TaskPriGet(taskCB->taskId);
    priorityRet = (INT16)(PRIORITY_WIN - (INT16)losPrio);
    if (priorityRet < osPriorityIdle || priorityRet > osPriorityRealtime) {
        return osPriorityError;
    }

    return (osPriority)priorityRet;
}

#if (LOSCFG_KERNEL_USERSPACE == YES)
osStatus osThreadSelfSuspend(void)
{
    LosTaskCB *runTask = OsCurrTaskGet();
    if (LOS_TaskSuspend(runTask->taskId) == LOS_OK) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

osStatus osThreadResume(osThreadId thread_id)
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
    } else if ((ret == LOS_ERRNO_TSK_ID_INVALID) || (ret == LOS_ERRNO_TSK_NOT_CREATED)) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}
#endif


//  ==== Semaphore Management Functions ====
#ifdef LOSCFG_BASE_IPC_SEM

osSemaphoreId osBinarySemaphoreCreate(const osSemaphoreDef_t *semaphore_def, int32_t count)
{
    UINT32 ret;
    UINT32 semId;

    if ((semaphore_def == NULL) || (count > OS_SEM_BINARY_COUNT_MAX) || (count < 0) || OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_BinarySemCreate((UINT16)count, &semId);
    if (ret == LOS_OK) {
        return (osSemaphoreId)GET_SEM(semId);
    } else {
        return NULL;
    }
}

osSemaphoreId osSemaphoreCreate(const osSemaphoreDef_t *semaphore_def, int32_t count)
{
    UINT32 ret;
    UINT32 semId;

    if ((semaphore_def == NULL) || (count > OS_SEM_COUNT_MAX) || (count < 0) || OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_SemCreate((UINT16)count, &semId);
    if (ret == LOS_OK) {
        return (osSemaphoreId)GET_SEM(semId);
    } else {
        return NULL;
    }
}

int32_t osSemaphoreWait(osSemaphoreId semaphore_id, uint32_t millisec)
{
    LosSemCB *semCB = (LosSemCB *)semaphore_id;
    UINT32 ret;

    if ((semCB == NULL) || OS_INT_ACTIVE) {
        return -1;
    }

    ret = LOS_SemPend(semCB->semId, LOS_MS2Tick(millisec));
    if (ret == LOS_OK) {
        return semCB->semCount;
    } else {
        return -1;
    }
}

osStatus osSemaphoreRelease(osSemaphoreId semaphore_id)
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

osStatus osSemaphoreDelete(osSemaphoreId semaphore_id)
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


//  ==== Mutex Management ====
#ifdef LOSCFG_BASE_IPC_MUX

osMutexId osMutexCreate(const osMutexDef_t *mutexDef)
{
    UINT32 ret;
    UINT32 muxId;

    if ((mutexDef == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_MuxCreate(&muxId);
    if (ret == LOS_OK) {
        return (osMutexId)GET_MUX(muxId);
    } else {
        return NULL;
    }
}

osStatus osMutexWait(osMutexId mutex_id, uint32_t millisec)
{
    LosMuxCB *muxCB = (LosMuxCB*)mutex_id;
    UINT32 ret;

    if (muxCB == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret = LOS_MuxPend(muxCB->muxId, LOS_MS2Tick(millisec));
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_MUX_INVALID) {
        return osErrorParameter;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        return osErrorTimeoutResource;
    } else {
        return osErrorResource;
    }
}

osStatus osMutexRelease(osMutexId mutex_id)
{
    LosMuxCB *muxCB = (LosMuxCB*)mutex_id;
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

osStatus osMutexDelete(osMutexId mutex_id)
{
    LosMuxCB *muxCB = (LosMuxCB*)mutex_id;
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


/* internal function for osPool and osMail */
STATIC VOID *CreateAndInitMemBox(UINT32 blkNum, UINT32 blkSize)
{
    VOID *memBox = NULL;
    UINT32 poolSize;
    UINT32 ret;

    if ((blkNum == 0) || (blkSize == 0)) {
        return NULL;
    }

#ifdef LOSCFG_KERNEL_MEMBOX_STATIC
    poolSize = LOS_MEMBOX_SIZE(blkSize, blkNum);
    memBox = LOS_MemAlloc(m_aucSysMem0, poolSize);
#else
    blkSize = LOS_MEMBOX_ALLIGNED(blkSize);
    poolSize = blkSize * blkNum;
    memBox = LOS_MemAlloc(m_aucSysMem0, sizeof(LOS_MEMBOX_INFO));
#endif
    if (memBox == NULL) {
        return NULL;
    }

    ret = LOS_MemboxInit(memBox, poolSize, blkSize);
    if (ret != LOS_OK) {
        LOS_MemFree(m_aucSysMem0, memBox);
        return NULL;
    }

    return memBox;
}


//  ==== Memory Pool Management Functions ====
osPoolId osPoolCreate(const osPoolDef_t *pool_def)
{
    if ((pool_def == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }

    return (osPoolId)CreateAndInitMemBox(pool_def->pool_sz, pool_def->item_sz);
}

void *osPoolAlloc(osPoolId pool_id)
{
    if (pool_id == NULL) {
        return NULL;
    }

    return LOS_MemboxAlloc(pool_id);
}

void *osPoolCAlloc(osPoolId pool_id)
{
    void *ptr = NULL;

    if (pool_id == NULL) {
        return NULL;
    }

    ptr = LOS_MemboxAlloc(pool_id);
    if (ptr != NULL) {
        LOS_MemboxClr(pool_id, ptr);
    }

    return ptr;
}

osStatus osPoolFree(osPoolId pool_id, void *block)
{
    UINT32 ret;

    if (pool_id == NULL) {
        return osErrorParameter;
    }

    ret = LOS_MemboxFree(pool_id, block);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorValue;
    }
}

osStatus osPoolDelete(osPoolId pool_id)
{
    LOS_MEMBOX_INFO *memBox = (LOS_MEMBOX_INFO *)pool_id;
    UINT32 ret;

    if (memBox == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }
    if (memBox->uwBlkCnt != 0) {
        return osErrorResource;
    }

    ret = LOS_MemFree(m_aucSysMem0, memBox);
    if (ret == LOS_OK) {
        return osOK;
    } else {
        return osErrorValue;
    }
}


#ifdef LOSCFG_BASE_IPC_QUEUE

/* internal function for osMessage and osMail */
STATIC_INLINE osStatus MappingQueueWriteRet(UINT32 ret)
{
    switch (ret) {
        case LOS_OK:
            return osOK;
        case LOS_ERRNO_QUEUE_TIMEOUT:
        case LOS_ERRNO_QUEUE_ISFULL:
            return osErrorTimeoutResource;
        case LOS_ERRNO_QUEUE_INVALID:
        case LOS_ERRNO_QUEUE_NOT_CREATE:
        case LOS_ERRNO_QUEUE_WRITE_PTR_NULL:
        case LOS_ERRNO_QUEUE_WRITESIZE_ISZERO:
        case LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG:
            return osErrorParameter;
        default:
            return osErrorResource;
    }
}

STATIC_INLINE osStatus MappingQueueReadRet(UINT32 ret)
{
    switch (ret) {
        case LOS_OK:
            return osEventMessage;
        case LOS_ERRNO_QUEUE_TIMEOUT:
        case LOS_ERRNO_QUEUE_ISEMPTY:
            return osEventTimeout;
        case LOS_ERRNO_QUEUE_INVALID:
        case LOS_ERRNO_QUEUE_NOT_CREATE:
        case LOS_ERRNO_QUEUE_READ_PTR_NULL:
        case LOS_ERRNO_QUEUE_READSIZE_IS_INVALID:
        case LOS_ERRNO_QUEUE_READ_SIZE_TOO_SMALL:
            return osErrorParameter;
        default:
            return osErrorResource;
    }
}


//  ==== Message Queue Management Functions ====
osMessageQId osMessageCreate(const osMessageQDef_t *queue_def, osThreadId thread_id)
{
    (VOID)thread_id;
    UINT32 queueId;
    UINT32 ret;

    if ((queue_def == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }
    // unused queue_def->item_sz, const 4 bytes to save pointer or 32-bit value. /ref <struct osEvent>->value.
    ret = LOS_QueueCreate(NULL, (UINT16)(queue_def->queue_sz), &queueId, 0, (UINT16)sizeof(UINT32));
    if (ret == LOS_OK) {
        return (osMessageQId)GET_QUEUE_HANDLE(queueId);
    } else {
        return NULL;
    }
}

osStatus osMessagePutHead(const osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
    LosQueueCB *queueCB = (LosQueueCB *)queue_id;
    UINT32 ret;

    if ((queueCB == NULL) || (OS_INT_ACTIVE && (millisec != 0))) {
        return osErrorParameter;
    }

    ret = LOS_QueueWriteHead(queueCB->queueId, (VOID *)(UINTPTR)info, sizeof(UINT32), LOS_MS2Tick(millisec));
    return MappingQueueWriteRet(ret);
}

osStatus osMessagePut(const osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
    LosQueueCB *queueCB = (LosQueueCB *)queue_id;
    UINT32 ret;

    if ((queueCB == NULL) || (OS_INT_ACTIVE && (millisec != 0))) {
        return osErrorParameter;
    }

    ret = LOS_QueueWrite(queueCB->queueId, (VOID *)(UINTPTR)info, sizeof(UINT32), LOS_MS2Tick(millisec));
    return MappingQueueWriteRet(ret);
}

osEvent osMessageGet(osMessageQId queue_id, uint32_t millisec)
{
    LosQueueCB *queueCB = (LosQueueCB *)queue_id;
    osEvent event = {0};
    UINT32 ret;

    if ((queueCB == NULL) || (OS_INT_ACTIVE && (millisec != 0))) {
        event.status = osErrorParameter;
        return event;
    }

    ret = LOS_QueueRead(queueCB->queueId, &(event.value.p), sizeof(UINT32), LOS_MS2Tick(millisec));
    event.status = MappingQueueReadRet(ret);
    return event;
}

osStatus osMessageDelete(const osMessageQId queue_id)
{
    LosQueueCB *queueCB = (LosQueueCB *)queue_id;
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
    } else {
        return osErrorResource;
    }
}


//  ==== Mail Queue Management Functions ====
osMailQId osMailCreate(const osMailQDef_t *queue_def, osThreadId thread_id)
{
    (VOID)thread_id;
    UINT32 ret;
    UINT32 queueId;
    struct osMailQ *mailQ = NULL;

    if ((queue_def == NULL) || (queue_def->pool == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }

    ret = LOS_QueueCreate(NULL, (UINT16)(queue_def->queue_sz), &queueId, 0, sizeof(UINT32));
    if (ret != LOS_OK) {
        return NULL;
    }

    mailQ = (struct osMailQ *)(queue_def->pool);
    mailQ->pool = CreateAndInitMemBox(queue_def->queue_sz, queue_def->item_sz);
    if (mailQ->pool == NULL) {
        LOS_QueueDelete(queueId);
        return NULL;
    }

    mailQ->queue = queueId;
    return (osMailQId)mailQ;
}

void *osMailAlloc(osMailQId queue_id, uint32_t millisec)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    if (mailQ == NULL) {
        return NULL;
    }

    return OsQueueMailAlloc(mailQ->queue, mailQ->pool, LOS_MS2Tick(millisec));
}

void *osMailCAlloc(osMailQId queue_id, uint32_t millisec)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    void *mem = NULL;

    mem = osMailAlloc(queue_id, millisec);
    if (mem != NULL) {
        LOS_MemboxClr(mailQ->pool, mem);
    }

    return mem;
}

osStatus osMailFree(osMailQId queue_id, void *mail)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    UINT32 ret;

    if (mailQ == NULL) {
        return osErrorParameter;
    }

    ret = OsQueueMailFree(mailQ->queue, mailQ->pool, mail);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_QUEUE_MAIL_FREE_ERROR) {
        return osErrorValue;
    } else {
        return osErrorParameter;
    }
}

osStatus osMailPutHead(osMailQId queue_id, void *mail)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    UINT32 ret;

    if (mailQ == NULL) {
        return osErrorParameter;
    }
    if (mail == NULL) {
        return osErrorValue;
    }

    ret = LOS_QueueWriteHead(mailQ->queue, mail, sizeof(UINT32), 0);
    return MappingQueueWriteRet(ret);
}

osStatus osMailPut(osMailQId queue_id, void *mail)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    UINT32 ret;

    if (mailQ == NULL) {
        return osErrorParameter;
    }
    if (mail == NULL) {
        return osErrorValue;
    }

    ret = LOS_QueueWrite(mailQ->queue, mail, sizeof(UINT32), 0);
    return MappingQueueWriteRet(ret);
}

osEvent osMailGet(osMailQId queue_id, uint32_t millisec)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    osEvent event = {0};
    osStatus status;
    UINT32 ret;

    if ((mailQ == NULL) || (OS_INT_ACTIVE && (millisec != 0))) {
        event.status = osErrorParameter;
        return event;
    }

    ret = LOS_QueueRead(mailQ->queue, &(event.value.p), sizeof(UINT32), LOS_MS2Tick(millisec));
    status = MappingQueueReadRet(ret);
    event.status = (status == osEventMessage) ? osEventMail : status;
    return event;
}

osStatus osMailClear(osMailQId queue_id)
{
    osEvent evt;
    UINTPTR intSave;
    intSave = LOS_IntLock();
    while (1) {
        evt = osMailGet(queue_id, 0);
        if (evt.status == osEventMail) {
            (VOID)osMailFree(queue_id, evt.value.p);
        } else if (evt.status == osEventTimeout) {
            LOS_IntRestore(intSave);
            return osOK;
        } else {
            LOS_IntRestore(intSave);
            return evt.status;
        }
    }
}

osStatus osMailDelete(osMailQId queue_id)
{
    struct osMailQ *mailQ = (struct osMailQ *)queue_id;
    osStatus ret1;
    UINT32 ret2;

    if (mailQ == NULL) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    ret1 = osPoolDelete(mailQ->pool);
    ret2 = LOS_QueueDelete(mailQ->queue);
    if ((ret1 == osOK) && (ret2 == LOS_OK)) {
        return osOK;
    } else {
        return osErrorOS;
    }
}

#endif /* LOSCFG_BASE_IPC_QUEUE */


//  ==== Signal Management ====
#ifdef LOSCFG_BASE_IPC_EVENT

// can only use 31 bits at most
#if (osFeature_Signals > 0x1F)
#error exceed max event bit count
#endif

#define VALID_EVENT_MASK   ((UINT32)((1U << osFeature_Signals) - 1))
#define INVALID_EVENT_MASK ((UINT32)(~VALID_EVENT_MASK))

int32_t osSignalSet(osThreadId thread_id, int32_t signals)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 events = (UINT32)signals;
    UINT32 intSave;
    UINT32 ret;
    EVENT_CB_S *eventCB = NULL;
    UINT32 eventSave;

    if ((taskCB == NULL) || (events & INVALID_EVENT_MASK)) {
        return (int32_t)0x80000000;
    }
    if (OS_TASK_ID_CHECK_INVALID(taskCB->taskId) || (taskCB->taskStatus & OS_TASK_STATUS_UNUSED)) {
        return (int32_t)0x80000000;
    }

    eventCB = &(taskCB->event);
    intSave = LOS_IntLock();
    eventSave = eventCB->uwEventID;
    LOS_IntRestore(intSave);

    ret = LOS_EventWrite(eventCB, events);
    if (ret == LOS_OK) {
        return (int32_t)eventSave;
    } else {
        return (int32_t)0x80000000;
    }
}

int32_t osSignalClear(osThreadId thread_id, int32_t signals)
{
    LosTaskCB *taskCB = (LosTaskCB *)thread_id;
    UINT32 events = (UINT32)signals;
    UINT32 intSave;
    UINT32 ret;
    EVENT_CB_S *eventCB = NULL;
    UINT32 eventSave;

    if ((taskCB == NULL) || (events & INVALID_EVENT_MASK) || OS_INT_ACTIVE) {
        return (int32_t)0x80000000;
    }
    if (OS_TASK_ID_CHECK_INVALID(taskCB->taskId) || (taskCB->taskStatus & OS_TASK_STATUS_UNUSED)) {
        return (int32_t)0x80000000;
    }

    eventCB = &(taskCB->event);
    intSave = LOS_IntLock();
    eventSave = eventCB->uwEventID;
    LOS_IntRestore(intSave);

    ret = LOS_EventClear(eventCB, ~events);
    if (ret == LOS_OK) {
        return (int32_t)eventSave;
    } else {
        return (int32_t)0x80000000;
    }
}

osEvent osSignalWait(int32_t signals, uint32_t millisec)
{
    UINT32 events;
    UINT32 ret;
    osEvent evt = {0};
    UINT32 flags = 0;
    LosTaskCB *runTask = NULL;

    if (OS_INT_ACTIVE) {
        evt.status = osErrorISR;
        return evt;
    }
    if ((UINT32)signals & INVALID_EVENT_MASK) {
        evt.status = osErrorValue;
        return evt;
    }

    if (signals != 0) {
        events = (UINT32)signals;
        flags |= LOS_WAITMODE_AND;
    } else {
        events = VALID_EVENT_MASK;
        flags |= LOS_WAITMODE_OR;
    }

    runTask = OsCurrTaskGet();
    ret = LOS_EventRead(&(runTask->event), events, flags | LOS_WAITMODE_CLR, LOS_MS2Tick(millisec));
    if (ret & LOS_ERRTYPE_ERROR) {
        if (ret == LOS_ERRNO_EVENT_READ_TIMEOUT) {
            evt.status = osEventTimeout;
        } else {
            evt.status = osErrorResource;
        }
    } else {
        if (ret == 0) {
            evt.status = osOK;
        } else {
            evt.status = osEventSignal;
            evt.value.signals = (int32_t)ret;
        }
    }

    return evt;
}

#endif /* LOSCFG_BASE_IPC_EVENT */


//  ==== Timer Management Functions ====
#ifdef LOSCFG_BASE_CORE_SWTMR

osTimerId osTimerCreate(const osTimerDef_t *timer_def, os_timer_type type, void *argument)
{
    UINT32 ret;
    UINT16 swtmrId;

    if ((timer_def == NULL) || (timer_def->ptimer == NULL) || OS_INT_ACTIVE) {
        return NULL;
    }
    if ((type != osTimerOnce) && (type != osTimerPeriodic) && (type != osTimerDelay)) {
        return NULL;
    }

    ret = LOS_SwtmrCreate(1, type, (SWTMR_PROC_FUNC)(timer_def->ptimer), &swtmrId, (UINT32)(UINTPTR)argument);
    if (ret == LOS_OK) {
        return (osTimerId)OS_SWT_FROM_SID(swtmrId);
    } else {
        return NULL;
    }
}

osStatus osTimerStart(osTimerId timer_id, uint32_t millisec)
{
    LosSwtmrCB *swtmrCB = (LosSwtmrCB *)timer_id;
    UINT32 interval = LOS_MS2Tick(millisec);
    UINT32 ret;

    if ((swtmrCB == NULL) || (interval == 0)) {
        return osErrorParameter;
    }
    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    swtmrCB->expiry = interval;
    swtmrCB->interval = interval;
    ret = LOS_SwtmrStart(swtmrCB->timerId);
    if (ret == LOS_OK) {
        return osOK;
    } else if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        return osErrorParameter;
    } else {
        return osErrorResource;
    }
}

osStatus osTimerStop(osTimerId timer_id)
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

osStatus osTimerRestart(osTimerId timer_id, uint32_t millisec, uint8_t strict)
{
    osStatus status;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    status = osTimerStop(timer_id);
    if (strict && (status != osOK)) {
        return status;
    }

    status = osTimerStart(timer_id, millisec);
    if (status != osOK) {
        return status;
    }

    return osOK;
}

osStatus osTimerDelete(osTimerId timer_id)
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


osStatus osDelay(uint32_t millisec)
{
    UINT32 interval;
    UINT32 ret;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (millisec == 0) {
        return osOK;
    }

    /* check if in idle we use udelay instead */
    if (OsGetIdleTaskId()  == LOS_CurTaskIDGet()) {
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

osEvent osWait(uint32_t millisec)
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

    /* osEventSignal, osEventMessage, osEventMail */
    interval = LOS_MS2Tick(millisec);

    ret = LOS_TaskDelay(interval);
    if (ret == LOS_OK) {
        evt.status = osEventTimeout;
    } else {
        evt.status = osErrorResource;
    }
    return evt;
}

#endif  // (CMSIS_OS_VER == 1)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
