/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Semaphore
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

#include "los_sem_pri.h"
#include "los_sem_debug_pri.h"
#include "los_err_pri.h"
#include "los_task_pri.h"
#include "los_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if (LOSCFG_BASE_IPC_SEM == YES)

LITE_OS_SEC_DATA_INIT STATIC LOS_DL_LIST g_unusedSemList;

#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
LITE_OS_SEC_BSS LosSemCB *g_allSem = NULL;
#else
#if (LOSCFG_BASE_IPC_SEM_LIMIT <= 0)
#error "sem maxnum cannot be zero"
#endif /* LOSCFG_BASE_IPC_SEM_LIMIT <= 0 */
LITE_OS_SEC_BSS LosSemCB g_allSem[LOSCFG_BASE_IPC_SEM_LIMIT];
#endif

/*
 * Description  : Initialize the semaphore doubly linked list
 * Return       : LOS_OK on success, or error code on failure
 */
LITE_OS_SEC_TEXT_INIT UINT32 OsSemInit(VOID)
{
    LosSemCB *semNode = NULL;
    UINT32 index;

#if (LOSCFG_BASE_IPC_DYNAMIC_MEM == YES)
    UINT32 size;

    size = g_semLimit * sizeof(LosSemCB);
    /* system resident memory, don't free */
    g_allSem = (LosSemCB *)LOS_MemAlloc(m_aucSysMem0, size);
    if (g_allSem == NULL) {
        return LOS_ERRNO_SEM_NO_MEMORY;
    }
    (VOID)memset_s(g_allSem, size, 0, size);
#endif

    LOS_ListInit(&g_unusedSemList);

    for (index = 0; index < LOSCFG_BASE_IPC_SEM_LIMIT; index++) {
        semNode = ((LosSemCB *)g_allSem) + index;
        semNode->semID = SET_SEM_ID(0, index);
        semNode->semStat = OS_SEM_UNUSED;
        LOS_ListTailInsert(&g_unusedSemList, &semNode->semList);
    }

    if (OsSemDbgInitHook() != LOS_OK) {
        return LOS_ERRNO_SEM_NO_MEMORY;
    }
    return LOS_OK;
}

/*
 * Description  : Create a semaphore,
 * Input        : count     --- semaphore count,
 *                maxCount  --- Max number of available semaphores,
 *                semHandle --- Index of semaphore,
 * Return       : LOS_OK on success ,or error code on failure
 */
LITE_OS_SEC_TEXT_INIT UINT32 OsSemCreate (UINT16 count, UINT16 maxCount, UINT32 *semHandle)
{
    UINT32 intSave;
    LosSemCB *semCreated = NULL;
    LOS_DL_LIST *unusedSem = NULL;
    UINT32 errNo;
    UINT32 errLine;

    if (semHandle == NULL) {
        return LOS_ERRNO_SEM_PTR_NULL;
    }

    if (count > maxCount) {
        OS_GOTO_ERR_HANDLER(LOS_ERRNO_SEM_OVERFLOW);
    }

    SCHEDULER_LOCK(intSave);

    if (LOS_ListEmpty(&g_unusedSemList)) {
        SCHEDULER_UNLOCK(intSave);
        OsSemInfoGetFullDataHook();
        OS_GOTO_ERR_HANDLER(LOS_ERRNO_SEM_ALL_BUSY);
    }

    unusedSem = LOS_DL_LIST_FIRST(&g_unusedSemList);
    LOS_ListDelete(unusedSem);
    semCreated = GET_SEM_LIST(unusedSem);
    semCreated->semCount = count;
    semCreated->semStat = OS_SEM_USED;
    semCreated->maxSemCount = maxCount;
    LOS_ListInit(&semCreated->semList);
    *semHandle = semCreated->semID;

    OsSemDbgUpdateHook(semCreated->semID, OsCurrTaskGet()->taskEntry, count);

    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;

ERR_HANDLER:
    OS_RETURN_ERROR_P2(errLine, errNo);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_SemCreate(UINT16 count, UINT32 *semHandle)
{
    return OsSemCreate(count, OS_SEM_COUNT_MAX, semHandle);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_BinarySemCreate(UINT16 count, UINT32 *semHandle)
{
    return OsSemCreate(count, OS_SEM_BINARY_COUNT_MAX, semHandle);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_SemDelete(UINT32 semHandle)
{
    UINT32 intSave;
    LosSemCB *semDeleted = NULL;
    UINT32 errNo;
    UINT32 errLine;

    if (GET_SEM_INDEX(semHandle) >= (UINT32)LOSCFG_BASE_IPC_SEM_LIMIT) {
        OS_GOTO_ERR_HANDLER(LOS_ERRNO_SEM_INVALID);
    }

    semDeleted = GET_SEM(semHandle);

    SCHEDULER_LOCK(intSave);

    if ((semDeleted->semStat == OS_SEM_UNUSED) || (semDeleted->semID != semHandle)) {
        SCHEDULER_UNLOCK(intSave);
        OS_GOTO_ERR_HANDLER(LOS_ERRNO_SEM_INVALID);
    }

    if (!LOS_ListEmpty(&semDeleted->semList)) {
        SCHEDULER_UNLOCK(intSave);
        OS_GOTO_ERR_HANDLER(LOS_ERRNO_SEM_PENDED);
    }

    LOS_ListTailInsert(&g_unusedSemList, &semDeleted->semList);
    semDeleted->semStat = OS_SEM_UNUSED;
    semDeleted->semID = SET_SEM_ID(GET_SEM_COUNT(semDeleted->semID) + 1, GET_SEM_INDEX(semDeleted->semID));

    OsSemDbgUpdateHook(semDeleted->semID, NULL, 0);

    SCHEDULER_UNLOCK(intSave);
    return LOS_OK;

ERR_HANDLER:
    OS_RETURN_ERROR_P2(errLine, errNo);
}

LITE_OS_SEC_TEXT UINT32 LOS_SemPend(UINT32 semHandle, UINT32 timeout)
{
    UINT32 intSave;
    LosSemCB *semPended = GET_SEM(semHandle);
    UINT32 retErr = LOS_OK;
    LosTaskCB *runTask = NULL;

    if (GET_SEM_INDEX(semHandle) >= (UINT32)LOSCFG_BASE_IPC_SEM_LIMIT) {
        OS_RETURN_ERROR(LOS_ERRNO_SEM_INVALID);
    }

    if (OS_INT_ACTIVE) {
        return LOS_ERRNO_SEM_PEND_INTERR;
    }

    runTask = OsCurrTaskGet();
    if (runTask->taskFlags & OS_TASK_FLAG_SYSTEM) {
        return LOS_ERRNO_SEM_PEND_IN_SYSTEM_TASK;
    }

    SCHEDULER_LOCK(intSave);

    if ((semPended->semStat == OS_SEM_UNUSED) || (semPended->semID != semHandle)) {
        retErr = LOS_ERRNO_SEM_INVALID;
        goto OUT;
    }

    /* Update the operate time, no matter the actual Pend success or not */
    OsSemDbgTimeUpdateHook(semHandle);

    if (semPended->semCount > 0) {
        semPended->semCount--;
        goto OUT;
    } else if (!timeout) {
        retErr = LOS_ERRNO_SEM_UNAVAILABLE;
        goto OUT;
    }

    if (!OsPreemptableInSched()) {
        retErr =  LOS_ERRNO_SEM_PEND_IN_LOCK;
        goto OUT;
    }

    runTask->taskSem = (VOID *)semPended;
    OsTaskWait(&semPended->semList, OS_TASK_STATUS_PEND, timeout);

    /*
     * it will immediately do the scheduling, so there's no need to release the
     * task spinlock. when this task's been rescheduled, it will be holding the spinlock.
     */
    OsSchedResched();

    SCHEDULER_UNLOCK(intSave);
    SCHEDULER_LOCK(intSave);
    if (runTask->taskStatus & OS_TASK_STATUS_TIMEOUT) {
        runTask->taskStatus &= ~OS_TASK_STATUS_TIMEOUT;
        retErr = LOS_ERRNO_SEM_TIMEOUT;
        goto OUT;
    }

OUT:
    SCHEDULER_UNLOCK(intSave);
    return retErr;
}

LITE_OS_SEC_TEXT UINT32 LOS_SemPost(UINT32 semHandle)
{
    UINT32 intSave;
    LosSemCB *semPosted = GET_SEM(semHandle);
    LosTaskCB *resumedTask = NULL;

    if (GET_SEM_INDEX(semHandle) >= LOSCFG_BASE_IPC_SEM_LIMIT) {
        return LOS_ERRNO_SEM_INVALID;
    }

    SCHEDULER_LOCK(intSave);

    if ((semPosted->semID != semHandle) || (semPosted->semStat == OS_SEM_UNUSED)) {
        SCHEDULER_UNLOCK(intSave);
        OS_RETURN_ERROR(LOS_ERRNO_SEM_INVALID);
    }

    /* Update the operate time, no matter the actual Post success or not */
    OsSemDbgTimeUpdateHook(semHandle);

    if (semPosted->semCount == OS_SEM_COUNT_MAX) {
        SCHEDULER_UNLOCK(intSave);
        OS_RETURN_ERROR(LOS_ERRNO_SEM_OVERFLOW);
    }
    if (!LOS_ListEmpty(&semPosted->semList)) {
        resumedTask = OS_TCB_FROM_PENDLIST(LOS_DL_LIST_FIRST(&(semPosted->semList)));
        resumedTask->taskSem = NULL;
        OsTaskWake(resumedTask, OS_TASK_STATUS_PEND);

        SCHEDULER_UNLOCK(intSave);
        LOS_Schedule();
    } else {
        semPosted->semCount++;
        SCHEDULER_UNLOCK(intSave);
    }

    return LOS_OK;
}

#endif /* (LOSCFG_BASE_IPC_SEM == YES) */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
