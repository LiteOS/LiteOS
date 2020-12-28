/* ---------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Software Timer Manager
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

#include "los_swtmr_pri.h"
#include "los_sortlink_pri.h"
#include "los_queue_pri.h"
#include "los_task_pri.h"
#include "los_trace.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_BASE_CORE_SWTMR
LITE_OS_SEC_BSS LosSwtmrCB      *g_swtmrCBArray = NULL;     /* First address in Timer memory space */
LITE_OS_SEC_BSS UINT8           *g_swtmrHandlerPool = NULL; /* Pool of Swtmr Handler */
LITE_OS_SEC_BSS LOS_DL_LIST     g_swtmrFreeList;            /* Free list of Software Timer */

/* spinlock for swtmr module, only available on SMP mode */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_swtmrSpin);
#define SWTMR_LOCK(state)       LOS_SpinLockSave(&g_swtmrSpin, &(state))
#define SWTMR_UNLOCK(state)     LOS_SpinUnlockRestore(&g_swtmrSpin, (state))

/*
 * Description: Start Software Timer
 * Input      : swtmr --- Need to start software timer
 */
LITE_OS_SEC_TEXT VOID OsSwtmrStart(LosSwtmrCB *swtmr)
{
    if ((swtmr->overrun == 0) && ((swtmr->mode == LOS_SWTMR_MODE_ONCE) ||
        (swtmr->mode == LOS_SWTMR_MODE_OPP) ||
        (swtmr->mode == LOS_SWTMR_MODE_NO_SELFDELETE))) {
        SET_SORTLIST_VALUE(&(swtmr->sortList), swtmr->expiry);
    } else {
        SET_SORTLIST_VALUE(&(swtmr->sortList), swtmr->interval);
    }

    OsAdd2SortLink(&OsPercpuGet()->swtmrSortLink, &swtmr->sortList);

    swtmr->state = OS_SWTMR_STATUS_TICKING;

#ifdef LOSCFG_KERNEL_SMP
    swtmr->cpuid = ArchCurrCpuid();
#endif
}

/*
 * Description: Delete Software Timer
 * Input      : swtmr --- Need to delete software timer, When using, Ensure that it can't be NULL.
 */
STATIC INLINE VOID OsSwtmrDelete(LosSwtmrCB *swtmr)
{
    /* insert to free list */
    LOS_ListTailInsert(&g_swtmrFreeList, &swtmr->sortList.sortLinkNode);
    swtmr->state = OS_SWTMR_STATUS_UNUSED;
}

STATIC INLINE VOID OsSwtmrUpdate(LosSwtmrCB *swtmr)
{
    if (swtmr->mode == LOS_SWTMR_MODE_ONCE) {
        OsSwtmrDelete(swtmr);

        if (swtmr->timerId < (OS_SWTMR_MAX_TIMERID - LOSCFG_BASE_CORE_SWTMR_LIMIT)) {
            swtmr->timerId += LOSCFG_BASE_CORE_SWTMR_LIMIT;
        } else {
            swtmr->timerId %= LOSCFG_BASE_CORE_SWTMR_LIMIT;
        }
    } else if (swtmr->mode == LOS_SWTMR_MODE_NO_SELFDELETE) {
        swtmr->state = OS_SWTMR_STATUS_CREATED;
    } else {
        swtmr->overrun++;
        OsSwtmrStart(swtmr);
    }
}

#ifndef LOSCFG_BASE_CORE_SWTMR_IN_ISR
LITE_OS_SEC_TEXT VOID OsSwtmrTask(VOID)
{
    UINT32 ret, swtmrHandlerQueue;
    SwtmrHandlerItemPtr swtmrHandler = NULL;
    UINT32 readSize;
    readSize = sizeof(CHAR *);

    swtmrHandlerQueue = OsPercpuGet()->swtmrHandlerQueue;
    for (;;) {
        ret = LOS_QueueReadCopy(swtmrHandlerQueue, &swtmrHandler, &readSize, LOS_WAIT_FOREVER);
        if ((ret == LOS_OK) && (readSize == sizeof(CHAR *))) {
            SWTMR_PROC_FUNC handler = swtmrHandler->handler;
            UINTPTR arg = swtmrHandler->arg;
            (VOID)LOS_MemFree(m_aucSysMem0, swtmrHandler);
            if (handler != NULL) {
                handler(arg);
            }
        }
    }
}

#ifdef LOSCFG_EXC_INTERACTION
BOOL IsSwtmrTask(UINT32 taskId)
{
    UINT32 i;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        if (taskId == g_percpu[i].swtmrTaskId) {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

LITE_OS_SEC_TEXT_INIT UINT32 OsSwtmrTaskCreate(VOID)
{
    UINT32 ret, swtmrTaskId;
    TSK_INIT_PARAM_S swtmrTask;
    UINT32 cpuid = ArchCurrCpuid();

    (VOID)memset_s(&swtmrTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    swtmrTask.pfnTaskEntry = (TSK_ENTRY_FUNC)OsSwtmrTask;
    swtmrTask.uwStackSize = LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE;
    swtmrTask.pcName = "Swt_Task";
    swtmrTask.usTaskPrio = 0;
    swtmrTask.uwResved = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    swtmrTask.usCpuAffiMask = CPUID_TO_AFFI_MASK(cpuid);
#endif
    ret = LOS_TaskCreate(&swtmrTaskId, &swtmrTask);
    if (ret == LOS_OK) {
        g_percpu[cpuid].swtmrTaskId = swtmrTaskId;
        OS_TCB_FROM_TID(swtmrTaskId)->taskFlags |= OS_TASK_FLAG_SYSTEM;
    }

    return ret;
}
#endif

LITE_OS_SEC_TEXT_INIT UINT32 OsSwtmrInit(VOID)
{
    UINT32 size;
    UINT16 index;
    UINT32 ret;
    LosSwtmrCB *swtmr = NULL;
    UINT32 cpuid = ArchCurrCpuid();
    if (cpuid == 0) {
        size = sizeof(LosSwtmrCB) * LOSCFG_BASE_CORE_SWTMR_LIMIT;
        swtmr = (LosSwtmrCB *)LOS_MemAlloc(m_aucSysMem0, size); /* system resident resource */
        if (swtmr == NULL) {
            return LOS_ERRNO_SWTMR_NO_MEMORY;
        }

        (VOID)memset_s(swtmr, size, 0, size);
        g_swtmrCBArray = swtmr;
        LOS_ListInit(&g_swtmrFreeList);
        for (index = 0; index < LOSCFG_BASE_CORE_SWTMR_LIMIT; index++, swtmr++) {
            swtmr->timerId = index;
            LOS_ListTailInsert(&g_swtmrFreeList, &swtmr->sortList.sortLinkNode);
        }
    }

#ifndef LOSCFG_BASE_CORE_SWTMR_IN_ISR
    ret = LOS_QueueCreate(NULL, OS_SWTMR_HANDLE_QUEUE_SIZE, &g_percpu[cpuid].swtmrHandlerQueue, 0, sizeof(CHAR *));
    if (ret != LOS_OK) {
        return LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED;
    }

    ret = OsSwtmrTaskCreate();
    if (ret != LOS_OK) {
        return LOS_ERRNO_SWTMR_TASK_CREATE_FAILED;
    }
#endif

    ret = OsSortLinkInit(&g_percpu[cpuid].swtmrSortLink);
    if (ret != LOS_OK) {
        return LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED;
    }

    return LOS_OK;
}

/*
 * Description: Tick interrupt interface module of software timer
 * Return     : LOS_OK on success or error code on failure
 */
LITE_OS_SEC_TEXT VOID OsSwtmrScan(VOID)
{
    SortLinkList *sortList = NULL;
    LosSwtmrCB *swtmr = NULL;
    LOS_DL_LIST *listObject = NULL;
    SortLinkAttribute* swtmrSortLink = &OsPercpuGet()->swtmrSortLink;

    SORTLINK_CURSOR_UPDATE(swtmrSortLink->cursor);
    SORTLINK_LISTOBJ_GET(listObject, swtmrSortLink);

    /*
     * it needs to be carefully coped with, since the swtmr is in specific sortlink
     * while other cores still has the chance to process it, like stop the timer.
     */
    LOS_SpinLock(&g_swtmrSpin);

    if (LOS_ListEmpty(listObject)) {
        LOS_SpinUnlock(&g_swtmrSpin);
        return;
    }
    sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
    ROLLNUM_DEC(sortList->idxRollNum);

    while (ROLLNUM(sortList->idxRollNum) == 0) {
        sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
        LOS_ListDelete(&sortList->sortLinkNode);
        swtmr = LOS_DL_LIST_ENTRY(sortList, LosSwtmrCB, sortList);

#ifndef LOSCFG_BASE_CORE_SWTMR_IN_ISR
        LOS_TRACE(SWTMR_EXPIRED, swtmr->timerId);
        SwtmrHandlerItemPtr swtmrHandler = (SwtmrHandlerItemPtr)LOS_MemAlloc(m_aucSysMem0, sizeof(SwtmrHandlerItem));
        if (swtmrHandler != NULL) {
            swtmrHandler->handler = swtmr->handler;
            swtmrHandler->arg = swtmr->arg;
            if (LOS_QueueWriteCopy(OsPercpuGet()->swtmrHandlerQueue, &swtmrHandler, sizeof(CHAR *), LOS_NO_WAIT)) {
                (VOID)LOS_MemFree(m_aucSysMem0, swtmrHandler);
            }
        }
        OsSwtmrUpdate(swtmr);
#else
        SWTMR_PROC_FUNC handler = swtmr->handler;
        UINTPTR arg = swtmr->arg;
        OsSwtmrUpdate(swtmr);
        if (handler != NULL) {
            LOS_SpinUnlock(&g_swtmrSpin);

            LOS_TRACE(SWTMR_EXPIRED, swtmr->timerId);
            handler(arg); /* do swtmr callback */

            LOS_SpinLock(&g_swtmrSpin);
        }
#endif

        if (LOS_ListEmpty(listObject)) {
            break;
        }

        sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
    }

    LOS_SpinUnlock(&g_swtmrSpin);
}

/*
 * Description: Get next timeout
 * Return     : Count of the Timer list
 */
LITE_OS_SEC_TEXT UINT32 OsSwtmrGetNextTimeout(VOID)
{
    return OsSortLinkGetNextExpireTime(&OsPercpuGet()->swtmrSortLink);
}

/*
 * Description: Stop of Software Timer interface
 * Input      : swtmr --- the software timer control handler
 */
LITE_OS_SEC_TEXT STATIC VOID OsSwtmrStop(LosSwtmrCB *swtmr)
{
    SortLinkAttribute *sortLinkHeader = NULL;

#ifdef LOSCFG_KERNEL_SMP
    /*
     * the timer is running on the specific processor,
     * we need delete the timer from that processor's sortlink.
     */
    sortLinkHeader = &g_percpu[swtmr->cpuid].swtmrSortLink;
#else
    sortLinkHeader = &g_percpu[0].swtmrSortLink;
#endif
    OsDeleteSortLink(sortLinkHeader, &swtmr->sortList);

    swtmr->state = OS_SWTMR_STATUS_CREATED;
    swtmr->overrun = 0;
}

/*
 * Description: Get next software timer expiretime
 * Input      : swtmr --- the software timer control handler
 */
LITE_OS_SEC_TEXT STATIC UINT32 OsSwtmrTimeGet(const LosSwtmrCB *swtmr)
{
    SortLinkAttribute *sortLinkHeader = NULL;

#ifdef LOSCFG_KERNEL_SMP
    /*
     * the timer is running on the specific processor,
     * we need search the timer from that processor's sortlink.
     */
    sortLinkHeader = &g_percpu[swtmr->cpuid].swtmrSortLink;
#else
    sortLinkHeader = &g_percpu[0].swtmrSortLink;
#endif

    return OsSortLinkGetTargetExpireTime(sortLinkHeader, &swtmr->sortList);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_SwtmrCreate(UINT32 interval,
                                             UINT8 mode,
                                             SWTMR_PROC_FUNC handler,
                                             UINT16 *swtmrId,
                                             UINTPTR arg)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 intSave;
    SortLinkList *sortList = NULL;

    if (interval == 0) {
        return LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED;
    }

    if ((mode != LOS_SWTMR_MODE_ONCE) && (mode != LOS_SWTMR_MODE_PERIOD) &&
        (mode != LOS_SWTMR_MODE_NO_SELFDELETE)) {
        return LOS_ERRNO_SWTMR_MODE_INVALID;
    }

    if (handler == NULL) {
        return LOS_ERRNO_SWTMR_PTR_NULL;
    }

    if (swtmrId == NULL) {
        return LOS_ERRNO_SWTMR_RET_PTR_NULL;
    }

    SWTMR_LOCK(intSave);
    if (LOS_ListEmpty(&g_swtmrFreeList)) {
        SWTMR_UNLOCK(intSave);
        return LOS_ERRNO_SWTMR_MAXSIZE;
    }

    sortList = LOS_DL_LIST_ENTRY(g_swtmrFreeList.pstNext, SortLinkList, sortLinkNode);
    swtmr = LOS_DL_LIST_ENTRY(sortList, LosSwtmrCB, sortList);
    LOS_ListDelete(LOS_DL_LIST_FIRST(&g_swtmrFreeList));
    SWTMR_UNLOCK(intSave);

    swtmr->handler = handler;
    swtmr->mode = mode;
    swtmr->overrun = 0;
    swtmr->interval = interval;
    swtmr->expiry = interval;
    swtmr->arg = arg;
    swtmr->state = OS_SWTMR_STATUS_CREATED;
    SET_SORTLIST_VALUE(&(swtmr->sortList), 0);
    *swtmrId = swtmr->timerId;
    LOS_TRACE(SWTMR_CREATE, swtmr->timerId);

    return LOS_OK;
}

LITE_OS_SEC_TEXT UINT32 LOS_SwtmrStart(UINT16 swtmrId)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 intSave;
    UINT32 ret = LOS_OK;
    UINT16 swtmrCBId;

    if (swtmrId >= OS_SWTMR_MAX_TIMERID) {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    SWTMR_LOCK(intSave);
    swtmrCBId = swtmrId % LOSCFG_BASE_CORE_SWTMR_LIMIT;
    swtmr = g_swtmrCBArray + swtmrCBId;

    if (swtmr->timerId != swtmrId) {
        SWTMR_UNLOCK(intSave);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    switch (swtmr->state) {
        case OS_SWTMR_STATUS_UNUSED:
            ret = LOS_ERRNO_SWTMR_NOT_CREATED;
            break;
        /*
         * If the status of swtmr is timing, it should stop the swtmr first,
         * then start the swtmr again.
         */
        case OS_SWTMR_STATUS_TICKING:
            OsSwtmrStop(swtmr);
            /* fall-through */
        case OS_SWTMR_STATUS_CREATED:
            OsSwtmrStart(swtmr);
            break;
        default:
            ret = LOS_ERRNO_SWTMR_STATUS_INVALID;
            break;
    }

    SWTMR_UNLOCK(intSave);
    LOS_TRACE(SWTMR_START, swtmr->timerId, swtmr->mode, swtmr->overrun, swtmr->interval, swtmr->expiry);
    return ret;
}

LITE_OS_SEC_TEXT UINT32 LOS_SwtmrStop(UINT16 swtmrId)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 intSave;
    UINT32 ret = LOS_OK;
    UINT16 swtmrCBId;

    if (swtmrId >= OS_SWTMR_MAX_TIMERID) {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    SWTMR_LOCK(intSave);
    swtmrCBId = swtmrId % LOSCFG_BASE_CORE_SWTMR_LIMIT;
    swtmr = g_swtmrCBArray + swtmrCBId;

    if (swtmr->timerId != swtmrId) {
        SWTMR_UNLOCK(intSave);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    switch (swtmr->state) {
        case OS_SWTMR_STATUS_UNUSED:
            ret = LOS_ERRNO_SWTMR_NOT_CREATED;
            break;
        case OS_SWTMR_STATUS_CREATED:
            ret = LOS_ERRNO_SWTMR_NOT_STARTED;
            break;
        case OS_SWTMR_STATUS_TICKING:
            OsSwtmrStop(swtmr);
            break;
        default:
            ret = LOS_ERRNO_SWTMR_STATUS_INVALID;
            break;
    }

    SWTMR_UNLOCK(intSave);
    LOS_TRACE(SWTMR_STOP, swtmr->timerId);
    return ret;
}

LITE_OS_SEC_TEXT UINT32 LOS_SwtmrTimeGet(UINT16 swtmrId, UINT32 *tick)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 intSave;
    UINT32 ret = LOS_OK;
    UINT16 swtmrCBId;

    if (swtmrId >= OS_SWTMR_MAX_TIMERID) {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    if (tick == NULL) {
        return LOS_ERRNO_SWTMR_TICK_PTR_NULL;
    }

    SWTMR_LOCK(intSave);
    swtmrCBId = swtmrId % LOSCFG_BASE_CORE_SWTMR_LIMIT;
    swtmr = g_swtmrCBArray + swtmrCBId;

    if (swtmr->timerId != swtmrId) {
        SWTMR_UNLOCK(intSave);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }
    switch (swtmr->state) {
        case OS_SWTMR_STATUS_UNUSED:
            ret = LOS_ERRNO_SWTMR_NOT_CREATED;
            break;
        case OS_SWTMR_STATUS_CREATED:
            ret = LOS_ERRNO_SWTMR_NOT_STARTED;
            break;
        case OS_SWTMR_STATUS_TICKING:
            *tick = OsSwtmrTimeGet(swtmr);
            break;
        default:
            ret = LOS_ERRNO_SWTMR_STATUS_INVALID;
            break;
    }
    SWTMR_UNLOCK(intSave);
    return ret;
}

LITE_OS_SEC_TEXT UINT32 LOS_SwtmrDelete(UINT16 swtmrId)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 intSave;
    UINT32 ret = LOS_OK;
    UINT16 swtmrCBId;

    if (swtmrId >= OS_SWTMR_MAX_TIMERID) {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    SWTMR_LOCK(intSave);
    swtmrCBId = swtmrId % LOSCFG_BASE_CORE_SWTMR_LIMIT;
    swtmr = g_swtmrCBArray + swtmrCBId;

    if (swtmr->timerId != swtmrId) {
        SWTMR_UNLOCK(intSave);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    switch (swtmr->state) {
        case OS_SWTMR_STATUS_UNUSED:
            ret = LOS_ERRNO_SWTMR_NOT_CREATED;
            break;
        case OS_SWTMR_STATUS_TICKING:
            OsSwtmrStop(swtmr);
            /* fall-through */
        case OS_SWTMR_STATUS_CREATED:
            OsSwtmrDelete(swtmr);
            break;
        default:
            ret = LOS_ERRNO_SWTMR_STATUS_INVALID;
            break;
    }

    SWTMR_UNLOCK(intSave);
    LOS_TRACE(SWTMR_DELETE, swtmr->timerId);
    return ret;
}

#endif /* LOSCFG_BASE_CORE_SWTMR */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
