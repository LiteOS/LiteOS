/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ARM Hwi Implementation
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */
#include "los_hwi_pri.h"
#include "los_hwi.h"
#include "los_memory.h"
#include "los_spinlock.h"
#include "los_trace.h"
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif
#ifdef LOSCFG_KERNEL_TICKLESS
#include "los_tickless_pri.h"
#endif

#ifdef LOSCFG_KERNEL_SCHED_STATISTICS
#include "los_sched_debug_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* spinlock for hwi module, only available on SMP mode */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_hwiSpin);
#define HWI_LOCK(state)       LOS_SpinLockSave(&g_hwiSpin, &(state))
#define HWI_UNLOCK(state)     LOS_SpinUnlockRestore(&g_hwiSpin, (state))
#define HWI_NUM_VALID(num)    (((num) >= OS_USER_HWI_MIN) && ((num) <= OS_USER_HWI_MAX))
/* The lower priority number, the higher priority, so OS_HWI_PRIO_LOWEST big
 * than OS_HWI_PRIO_HIGHEST */
#define HWI_PRI_VALID(pri)    (((pri) >= OS_HWI_PRIO_HIGHEST) && ((pri) <= OS_HWI_PRIO_LOWEST))

size_t g_intCount[LOSCFG_KERNEL_CORE_NUM] = {0};
STATIC VOID DefaultTriggerIrq(HWI_HANDLE_T hwiNum)
{
    (VOID) hwiNum;
    return;
}
STATIC VOID DefaultClearIrq(HWI_HANDLE_T hwiNum)
{
    (VOID) hwiNum;
    return;
}
STATIC VOID DefaultEnableIrq(HWI_HANDLE_T hwiNum)
{
    (VOID) hwiNum;
    return;
}
STATIC VOID DefaultDisableIrq(HWI_HANDLE_T hwiNum)
{
    (VOID) hwiNum;
    return;
}
STATIC UINT32 DefaultSetIrqPriority(HWI_HANDLE_T hwiNum, UINT8 priority)
{
    (VOID) hwiNum;
    (VOID) priority;
    return 0;
}
STATIC UINT32 DefaultGetCurIrqNum(VOID)
{
    return 0;
}
STATIC CHAR *DefaultGetIrqVersion(VOID)
{
    return NULL;
}
STATIC HWI_HANDLE_FORM_S *DefaultGetHandleForm(HWI_HANDLE_T hwiNum)
{
    (VOID) hwiNum;
    return NULL;
}
STATIC VOID DefaultHandleIrq(VOID)
{
    return;
}
#ifdef LOSCFG_KERNEL_SMP
STATIC VOID DefaultSetIrqCpuAffinity(HWI_HANDLE_T hwiNum, UINT32 cpuMask)
{
    (VOID) hwiNum;
    (VOID) cpuMask;
    return;
}
STATIC VOID DefaultSendIpi(UINT32 target, UINT32 ipi)
{
    (VOID) target;
    (VOID) ipi;
    return;
}
#endif

/* The implementation of the default interface stub, in the actual operation process, through the interrupt controller
 * implementation layer configuration, this table realizes interface registration */
STATIC const HwiControllerOps g_defaultOps = {
    .triggerIrq = DefaultTriggerIrq,
    .clearIrq = DefaultClearIrq,
    .enableIrq = DefaultEnableIrq,
    .disableIrq = DefaultDisableIrq,
    .setIrqPriority = DefaultSetIrqPriority,
    .getCurIrqNum = DefaultGetCurIrqNum,
    .getIrqVersion = DefaultGetIrqVersion,
    .getHandleForm = DefaultGetHandleForm,
    .handleIrq = DefaultHandleIrq,
#ifdef LOSCFG_KERNEL_SMP
    .sendIpi = DefaultSetIrqCpuAffinity,
    .setIrqCpuAffinity = DefaultSendIpi,
#endif
};

STATIC const HwiControllerOps *g_hwiOps = &g_defaultOps;

VOID OsIncHwiFormCnt(UINT32 hwiNum)
{
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return;
    }
    hwiForm->count++;
}

UINT32 OsGetHwiFormCnt(UINT32 hwiNum)
{
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    return hwiForm->count;
}

CHAR *OsGetHwiFormName(UINT32 hwiNum)
{
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return NULL;
    }
    return hwiForm->name;
}

typedef VOID (*HWI_PROC_FUNC0)(VOID);
typedef VOID (*HWI_PROC_FUNC2)(INT32, VOID *);

STATIC INLINE VOID OsIrqNestingActive(UINT32 hwiNum)
{
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    /* preemption not allowed when handling tick interrupt */
    if (hwiNum != OS_TICK_INT_NUM) {
        (VOID)LOS_IntUnLock();
    }
#endif
}

STATIC INLINE VOID OsIrqNestingInactive(UINT32 hwiNum)
{
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    if (hwiNum != OS_TICK_INT_NUM) {
        (VOID)LOS_IntLock();
    }
#endif
}

size_t OsIrqNestingCntGet(VOID)
{
    return g_intCount[ArchCurrCpuid()];
}

STATIC INLINE VOID InterruptHandle(HWI_HANDLE_FORM_S *hwiForm)
{
    hwiForm->count++;
#ifndef LOSCFG_NO_SHARED_IRQ
    while (hwiForm->pstNext != NULL) {
        hwiForm = hwiForm->pstNext;
#endif
        if (hwiForm->uwParam) {
            HWI_PROC_FUNC2 func = (HWI_PROC_FUNC2)hwiForm->pfnHook;
            if (func != NULL) {
                UINTPTR *param = (UINTPTR *)(hwiForm->uwParam);
                func((INT32)(*param), (VOID *)(*(param + 1)));
            }
        } else {
            HWI_PROC_FUNC0 func = (HWI_PROC_FUNC0)hwiForm->pfnHook;
            if (func != NULL) {
                func();
            }
        }
#ifndef LOSCFG_NO_SHARED_IRQ
    }
#endif
}

VOID OsIntHandle(UINT32 hwiNum, HWI_HANDLE_FORM_S *hwiForm)
{
    size_t *intCnt = NULL;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqStart();
#endif
    intCnt = &g_intCount[ArchCurrCpuid()];
    *intCnt = *intCnt + 1;

#ifdef LOSCFG_KERNEL_SCHED_STATISTICS
    OsHwiStatistics(hwiNum);
#endif

#ifdef LOSCFG_KERNEL_TICKLESS
    OsTicklessUpdate(hwiNum);
#endif

    LOS_TRACE(HWI, RESPONSE_IN, hwiNum);

    OsIrqNestingActive(hwiNum);
    InterruptHandle(hwiForm);
    OsIrqNestingInactive(hwiNum);

    LOS_TRACE(HWI, RESPONSE_OUT, hwiNum);

    *intCnt = *intCnt - 1;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqEnd(hwiNum);
#endif
}

STATIC HWI_ARG_T OsHwiCpIrqParam(const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_IRQ_PARAM_S *paramByAlloc = NULL;

    if (irqParam != NULL) {
        paramByAlloc = (HWI_IRQ_PARAM_S *)LOS_MemAlloc(m_aucSysMem0, sizeof(HWI_IRQ_PARAM_S));
        if (paramByAlloc == NULL) {
            return LOS_NOK;
        }
        (VOID)memcpy_s(paramByAlloc, sizeof(HWI_IRQ_PARAM_S), irqParam, sizeof(HWI_IRQ_PARAM_S));
    }
    return (HWI_ARG_T)paramByAlloc;
}
#ifdef LOSCFG_NO_SHARED_IRQ
STATIC UINT32 OsHwiDelNoShared(HWI_HANDLE_FORM_S *hwiForm)
{
    UINT32 intSave;

    HWI_LOCK(intSave);
    hwiForm->pfnHook = NULL;
    if (hwiForm->uwParam) {
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->uwParam);
    }
    hwiForm->uwParam = 0;
    hwiForm->name = NULL;
    hwiForm->count = 0;

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateNoShared(HWI_HANDLE_FORM_S *hwiForm, HWI_MODE_T hwiMode, HWI_PROC_FUNC hwiHandler,
                                  const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_ARG_T retParam;
    UINT32 intSave;

    (VOID)hwiMode;
    HWI_LOCK(intSave);
    if (hwiForm->pfnHook == NULL) {
        hwiForm->pfnHook = hwiHandler;

        retParam = OsHwiCpIrqParam(irqParam);
        if (retParam == LOS_NOK) {
            HWI_UNLOCK(intSave);
            return OS_ERRNO_HWI_NO_MEMORY;
        }
        hwiForm->uwParam = retParam;
    } else {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_ALREADY_CREATED;
    }
    HWI_UNLOCK(intSave);
    return LOS_OK;
}
#else
STATIC UINT32 OsHwiDelShared(HWI_HANDLE_FORM_S *head, const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_HANDLE_FORM_S *hwiFormtmp = NULL;
    HWI_HANDLE_FORM_S *hwiForm = NULL;
    UINT32 find = FALSE;
    UINT32 intSave;

    HWI_LOCK(intSave);

    if ((head->uwParam & IRQF_SHARED) && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    if ((head->pstNext != NULL) && !(head->uwParam & IRQF_SHARED)) {
        hwiForm = head->pstNext;
        if (hwiForm->uwParam) {
            (VOID) LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->uwParam);
        }
        (VOID) LOS_MemFree(m_aucSysMem0, hwiForm);
        head->pstNext = NULL;
        head->name = NULL;
        head->count = 0;

        HWI_UNLOCK(intSave);
        return LOS_OK;
    }

    hwiFormtmp = head;
    hwiForm = head->pstNext;
    while (hwiForm != NULL) {
        if (((HWI_IRQ_PARAM_S *)(hwiForm->uwParam))->pDevId != irqParam->pDevId) {
            hwiFormtmp = hwiForm;
            hwiForm = hwiForm->pstNext;
        } else {
            hwiFormtmp->pstNext = hwiForm->pstNext;
            (VOID) LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->uwParam);
            (VOID) LOS_MemFree(m_aucSysMem0, hwiForm);

            find = TRUE;
            break;
        }
    }

    if (!find) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_HWINUM_UNCREATE;
    }

    if (head->pstNext == NULL) {
        head->uwParam = 0;
        head->name = NULL;
    }

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateShared(HWI_HANDLE_FORM_S *head, HWI_MODE_T hwiMode, HWI_PROC_FUNC hwiHandler,
                                const HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 intSave;
    HWI_HANDLE_FORM_S *hwiFormNode = NULL;
    HWI_IRQ_PARAM_S *hwiParam = NULL;
    HWI_MODE_T modeResult = hwiMode & IRQF_SHARED;
    HWI_HANDLE_FORM_S *hwiForm = NULL;

    if (modeResult && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    HWI_LOCK(intSave);

    if ((head->pstNext != NULL) && ((modeResult == 0) || (!(head->uwParam & IRQF_SHARED)))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    hwiForm = head;
    while (hwiForm->pstNext != NULL) {
        hwiForm = hwiForm->pstNext;
        hwiParam = (HWI_IRQ_PARAM_S *)(hwiForm->uwParam);
        if (hwiParam->pDevId == irqParam->pDevId) {
            HWI_UNLOCK(intSave);
            return OS_ERRNO_HWI_ALREADY_CREATED;
        }
    }

    hwiFormNode = (HWI_HANDLE_FORM_S *)LOS_MemAlloc(m_aucSysMem0, sizeof(HWI_HANDLE_FORM_S));
    if (hwiFormNode == NULL) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_NO_MEMORY;
    }

    hwiFormNode->uwParam = OsHwiCpIrqParam(irqParam);
    if (hwiFormNode->uwParam == LOS_NOK) {
        HWI_UNLOCK(intSave);
        (VOID) LOS_MemFree(m_aucSysMem0, hwiFormNode);
        return OS_ERRNO_HWI_NO_MEMORY;
    }

    hwiFormNode->pfnHook = hwiHandler;
    hwiFormNode->pstNext = (struct tagHwiHandleForm *)NULL;
    hwiForm->pstNext = hwiFormNode;

    /* Update the interrupt sharing mode and name */
    if ((irqParam != NULL) && (irqParam->pName != NULL)) {
        head->name = (CHAR *)irqParam->pName;
    }
    head->uwParam = modeResult;

    HWI_UNLOCK(intSave);
    return LOS_OK;
}
#endif

size_t IntActive()
{
    size_t intCount;
    UINT32 intSave = LOS_IntLock();
    intCount = g_intCount[ArchCurrCpuid()];
    LOS_IntRestore(intSave);
    return intCount;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiCreate(HWI_HANDLE_T hwiNum,
                                           HWI_PRIOR_T hwiPrio,
                                           HWI_MODE_T hwiMode,
                                           HWI_PROC_FUNC hwiHandler,
                                           HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 ret;
    HWI_HANDLE_FORM_S *hwiForm = NULL;

    if (hwiHandler == NULL) {
        return OS_ERRNO_HWI_PROC_FUNC_NULL;
    }

    hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

#ifdef LOSCFG_NO_SHARED_IRQ
    ret = OsHwiCreateNoShared(hwiForm, hwiMode, hwiHandler, irqParam);
#else
    ret = OsHwiCreateShared(hwiForm, hwiMode, hwiHandler, irqParam);
#endif
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    if (ret == LOS_OK) {
        ret = g_hwiOps->setIrqPriority(hwiNum, hwiPrio);
    }
#endif

    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDelete(HWI_HANDLE_T hwiNum, HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 ret;
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

#ifdef LOSCFG_NO_SHARED_IRQ
    (VOID)irqParam;
    ret = OsHwiDelNoShared(hwiForm);
#else
    ret = OsHwiDelShared(hwiForm, irqParam);
#endif

    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiTrigger(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    g_hwiOps->triggerIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiEnable(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    g_hwiOps->enableIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDisable(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    g_hwiOps->disableIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiClear(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    g_hwiOps->clearIrq(hwiNum);
    return LOS_OK;
}

UINT32 LOS_HwiSetPriority(HWI_HANDLE_T hwiNum, UINT32 priority)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    if (!HWI_PRI_VALID(priority)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }
    g_hwiOps->setIrqPriority(hwiNum, priority);
    return LOS_OK;
}
#ifdef LOSCFG_KERNEL_SMP
UINT32 LOS_HwiSetAffinity(HWI_HANDLE_T hwiNum, UINT32 cpuMask)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    g_hwiOps->setIrqCpuAffinity(hwiNum, cpuMask);
    return LOS_OK;
}

UINT32 LOS_HwiSendIpi(HWI_HANDLE_T hwiNum, UINT32 cpu)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    g_hwiOps->sendIpi(hwiNum, cpu);
    return LOS_OK;
}
#endif

CHAR *LOS_HwiVersion(VOID)
{
    return g_hwiOps->getIrqVersion();
}

UINT32 OsIntNumGet(VOID)
{
    return g_hwiOps->getCurIrqNum();
}

CHAR *OsIntVersionGet(VOID)
{
    return g_hwiOps->getIrqVersion();
}

BOOL OsIntIsRegisted(UINT32 num)
{
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(num);
    if (hwiForm == NULL) {
        return false;
    }
#ifdef LOSCFG_NO_SHARED_IRQ
    return (hwiForm->pfnHook != NULL);
#else
    return (hwiForm->pstNext != NULL);
#endif
}

HWI_ARG_T OsIntGetPara(UINT32 num)
{
    HWI_HANDLE_FORM_S *hwiForm = g_hwiOps->getHandleForm(num);
    if (hwiForm == NULL) {
        return 0;
    }
    return hwiForm->uwParam;
}

VOID OsHwiControllerReg(const HwiControllerOps *ops)
{
    g_hwiOps = ops;
}

VOID OsIntEntry(VOID)
{
    g_hwiOps->handleIrq();
    return;
}

/* Initialization of the hardware interrupt */
LITE_OS_SEC_TEXT_INIT VOID OsHwiInit(VOID)
{
    HalIrqInit();
    return;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
