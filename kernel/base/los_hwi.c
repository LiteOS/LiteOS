/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Interrupt Abstraction Layer And API Implementation
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

#include "los_hwi_pri.h"
#include "los_hwi.h"
#include "los_memory.h"
#include "los_spinlock.h"
#include "los_trace.h"
#ifdef LOSCFG_KERNEL_LOWPOWER
#include "los_lowpower_pri.h"
#endif
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
#include "los_sched_debug_pri.h"
#endif
#include "los_err_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* spinlock for hwi module, only available on SMP mode */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_hwiSpin);

#define HWI_LOCK(state)       LOS_SpinLockSave(&g_hwiSpin, &(state))
#define HWI_UNLOCK(state)     LOS_SpinUnlockRestore(&g_hwiSpin, (state))

/* The specification of interrupt number is configured by the user through Menuconfig, Configuration
 * macro is LOSCFG_PLATFORM_HWI_LIMIT which Represents total count supported by the system */
#define HWI_NUM_VALID(num)    (((num) >= OS_USER_HWI_MIN) && ((num) <= OS_USER_HWI_MAX))

/* The lower priority number, the higher priority, so OS_HWI_PRIO_LOWEST big
 * than OS_HWI_PRIO_HIGHEST */
#define HWI_PRI_VALID(pri)    (((pri) >= OS_HWI_PRIO_HIGHEST) && ((pri) <= OS_HWI_PRIO_LOWEST))

size_t g_intCount[LOSCFG_KERNEL_CORE_NUM] = {0};

#ifdef LOSCFG_KERNEL_LOWPOWER
IntWakeupHookFn g_intWakeupHook = NULL;
#endif

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

STATIC HwiHandleInfo *DefaultGetHandleForm(HWI_HANDLE_T hwiNum)
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
    .sendIpi = DefaultSendIpi,
    .setIrqCpuAffinity = DefaultSetIrqCpuAffinity,
#endif
};

STATIC const HwiControllerOps *g_hwiOps = &g_defaultOps;

UINT32 OsGetHwiFormCnt(UINT32 hwiNum)
{
    HwiHandleInfo *hwiForm = g_hwiOps->getHandleForm(hwiNum);

    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    return hwiForm->respCount;
}

HwiHandleInfo *OsGetHwiForm(UINT32 hwiNum)
{
    return g_hwiOps->getHandleForm(hwiNum);
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

VOID OsIrqNestingCntSet(size_t val)
{
    g_intCount[ArchCurrCpuid()] = val;
}

STATIC INLINE VOID InterruptHandle(HwiHandleInfo *hwiForm)
{
    hwiForm->respCount++;
#ifndef LOSCFG_NO_SHARED_IRQ
    while (hwiForm->next != NULL) {
        hwiForm = hwiForm->next;
#endif
        if (hwiForm->registerInfo) {
            HWI_PROC_FUNC2 func = (HWI_PROC_FUNC2)hwiForm->hook;
            if (func != NULL) {
                UINTPTR *param = (UINTPTR *)(hwiForm->registerInfo);
                func((INT32)(*param), (VOID *)(*(param + 1)));
            }
        } else {
            HWI_PROC_FUNC0 func = (HWI_PROC_FUNC0)hwiForm->hook;
            if (func != NULL) {
                func();
            }
        }
#ifndef LOSCFG_NO_SHARED_IRQ
    }
#endif
}

VOID OsIntHandle(UINT32 hwiNum, HwiHandleInfo *hwiForm)
{
    size_t *intCnt = NULL;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqStart();
#endif
    intCnt = &g_intCount[ArchCurrCpuid()];
    *intCnt = *intCnt + 1;

#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    OsHwiStatistics(hwiNum);
#endif

#ifdef LOSCFG_KERNEL_LOWPOWER
    if (g_intWakeupHook != NULL) {
        g_intWakeupHook(hwiNum);
    }
#endif
    LOS_TRACE(HWI_RESPONSE_IN, hwiNum);

    OsIrqNestingActive(hwiNum);
    InterruptHandle(hwiForm);
    OsIrqNestingInactive(hwiNum);

    LOS_TRACE(HWI_RESPONSE_OUT, hwiNum);

    *intCnt = *intCnt - 1;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqEnd(hwiNum);
#endif
}

STATIC HWI_ARG_T OsHwiCpIrqParam(const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_IRQ_PARAM_S *paramByAlloc = NULL;

    paramByAlloc = (HWI_IRQ_PARAM_S *)LOS_MemAlloc(m_aucSysMem0, sizeof(HWI_IRQ_PARAM_S));
    if (paramByAlloc != NULL) {
        (VOID)memcpy_s(paramByAlloc, sizeof(HWI_IRQ_PARAM_S), irqParam, sizeof(HWI_IRQ_PARAM_S));
    }

    return (HWI_ARG_T)paramByAlloc;
}
#ifdef LOSCFG_NO_SHARED_IRQ
STATIC UINT32 OsHwiDelNoShared(HwiHandleInfo *hwiForm)
{
    UINT32 intSave;

    HWI_LOCK(intSave);
    hwiForm->hook = NULL;
    if (hwiForm->registerInfo) {
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->registerInfo);
    }
    hwiForm->registerInfo = 0;
    hwiForm->respCount = 0;

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateNoShared(HwiHandleInfo *hwiForm, HWI_MODE_T hwiMode, HWI_PROC_FUNC hwiHandler,
                                  const HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 intSave;

    if (hwiMode & IRQF_SHARED) {
        return OS_ERRNO_HWI_SHARED_ERROR;
    }
    HWI_LOCK(intSave);
    if (hwiForm->hook == NULL) {
        hwiForm->hook = hwiHandler;

        if (irqParam != NULL) {
            hwiForm->registerInfo = OsHwiCpIrqParam(irqParam);
            if (hwiForm->registerInfo == (HWI_ARG_T)NULL) {
                HWI_UNLOCK(intSave);
                return OS_ERRNO_HWI_NO_MEMORY;
            }
        }
    } else {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_ALREADY_CREATED;
    }
    HWI_UNLOCK(intSave);
    return LOS_OK;
}
#else
STATIC UINT32 OsHwiDelShared(HwiHandleInfo *head, const HWI_IRQ_PARAM_S *irqParam)
{
    HwiHandleInfo *hwiFormtmp = NULL;
    HwiHandleInfo *hwiForm = NULL;
    UINT32 find = FALSE;
    UINT32 intSave;

    HWI_LOCK(intSave);

    if ((head->shareMode & IRQF_SHARED) && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    if ((head->next != NULL) && !(head->shareMode & IRQF_SHARED)) {
        hwiForm = head->next;
        if (hwiForm->registerInfo) {
            (VOID) LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->registerInfo);
        }
        (VOID) LOS_MemFree(m_aucSysMem0, hwiForm);
        head->next = NULL;
        head->respCount = 0;

        HWI_UNLOCK(intSave);
        return LOS_OK;
    }

    hwiFormtmp = head;
    hwiForm = head->next;
    while (hwiForm != NULL) {
        if (((HWI_IRQ_PARAM_S *)(hwiForm->registerInfo))->pDevId != irqParam->pDevId) {
            hwiFormtmp = hwiForm;
            hwiForm = hwiForm->next;
        } else {
            hwiFormtmp->next = hwiForm->next;
            (VOID) LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->registerInfo);
            (VOID) LOS_MemFree(m_aucSysMem0, hwiForm);

            find = TRUE;
            break;
        }
    }

    if (!find) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_HWINUM_UNCREATE;
    }

    if (head->next == NULL) {
        head->shareMode = 0;
    }

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateShared(HwiHandleInfo *head, HWI_MODE_T hwiMode, HWI_PROC_FUNC hwiHandler,
                                const HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 intSave;
    HwiHandleInfo *hwiFormNode = NULL;
    HWI_IRQ_PARAM_S *hwiParam = NULL;
    HWI_MODE_T modeResult = hwiMode & IRQF_SHARED;
    HwiHandleInfo *hwiForm = NULL;

    if (modeResult && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    HWI_LOCK(intSave);

    if ((head->next != NULL) && ((modeResult == 0) || (!(head->shareMode & IRQF_SHARED)))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    hwiForm = head;
    while (hwiForm->next != NULL) {
        hwiForm = hwiForm->next;
        hwiParam = (HWI_IRQ_PARAM_S *)(hwiForm->registerInfo);
        if (hwiParam->pDevId == irqParam->pDevId) {
            HWI_UNLOCK(intSave);
            return OS_ERRNO_HWI_ALREADY_CREATED;
        }
    }

    hwiFormNode = (HwiHandleInfo *)LOS_MemAlloc(m_aucSysMem0, sizeof(HwiHandleInfo));
    if (hwiFormNode == NULL) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_NO_MEMORY;
    }
    hwiForm->respCount = 0;

    if (irqParam != NULL) {
        hwiFormNode->registerInfo = OsHwiCpIrqParam(irqParam);
        if (hwiFormNode->registerInfo == (HWI_ARG_T)NULL) {
            HWI_UNLOCK(intSave);
            (VOID) LOS_MemFree(m_aucSysMem0, hwiFormNode);
            return OS_ERRNO_HWI_NO_MEMORY;
        }
    } else {
        hwiFormNode->registerInfo = 0;
    }

    hwiFormNode->hook = hwiHandler;
    hwiFormNode->next = (struct tagHwiHandleForm *)NULL;
    hwiForm->next = hwiFormNode;

    head->shareMode = modeResult;

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
    HwiHandleInfo *hwiForm = NULL;

    if (hwiHandler == NULL) {
        return OS_ERRNO_HWI_PROC_FUNC_NULL;
    }

    hwiForm = g_hwiOps->getHandleForm(hwiNum);
    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    LOS_TRACE(HWI_CREATE, hwiNum, hwiPrio, hwiMode, (UINTPTR)hwiHandler);

#ifdef LOSCFG_NO_SHARED_IRQ
    ret = OsHwiCreateNoShared(hwiForm, hwiMode, hwiHandler, irqParam);
#else
    ret = OsHwiCreateShared(hwiForm, hwiMode, hwiHandler, irqParam);
    LOS_TRACE(HWI_CREATE_SHARE, hwiNum, (UINTPTR)(irqParam != NULL ? irqParam->pDevId : NULL), ret);
#endif
    /* priority will be changed if setIrqPriority implemented,
     * but interrupt preemption only allowed when LOSCFG_ARCH_INTERRUPT_PREEMPTION enable */
    if ((ret == LOS_OK) && (g_hwiOps->setIrqPriority != NULL)) {
        if (!HWI_PRI_VALID(hwiPrio)) {
            return OS_ERRNO_HWI_PRIO_INVALID;
        }
        ret = g_hwiOps->setIrqPriority(hwiNum, hwiPrio);
    }
    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDelete(HWI_HANDLE_T hwiNum, HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 ret;
    HwiHandleInfo *hwiForm = g_hwiOps->getHandleForm(hwiNum);

    if (hwiForm == NULL) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    LOS_TRACE(HWI_DELETE, hwiNum);

#ifdef LOSCFG_NO_SHARED_IRQ
    (VOID)irqParam;
    ret = OsHwiDelNoShared(hwiForm);
#else
    ret = OsHwiDelShared(hwiForm, irqParam);
    LOS_TRACE(HWI_DELETE_SHARE, hwiNum, (UINTPTR)(irqParam != NULL ? irqParam->pDevId : NULL), ret);
#endif
    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiTrigger(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->triggerIrq, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_TRIGGER, hwiNum);
    g_hwiOps->triggerIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiEnable(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->enableIrq, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_ENABLE, hwiNum);
    g_hwiOps->enableIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDisable(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->disableIrq, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_DISABLE, hwiNum);
    g_hwiOps->disableIrq(hwiNum);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiClear(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->clearIrq, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_CLEAR, hwiNum);
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
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->setIrqPriority, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_SETPRI, hwiNum, priority);
    g_hwiOps->setIrqPriority(hwiNum, priority);
    return LOS_OK;
}
#ifdef LOSCFG_KERNEL_SMP
UINT32 LOS_HwiSetAffinity(HWI_HANDLE_T hwiNum, UINT32 cpuMask)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->setIrqCpuAffinity, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_SETAFFINITY, hwiNum, cpuMask);
    g_hwiOps->setIrqCpuAffinity(hwiNum, cpuMask);
    return LOS_OK;
}

UINT32 LOS_HwiSendIpi(HWI_HANDLE_T hwiNum, UINT32 cpuMask)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->sendIpi, OS_ERRNO_HWI_PROC_FUNC_NULL);
    LOS_TRACE(HWI_SENDIPI, hwiNum, cpuMask);
    g_hwiOps->sendIpi(cpuMask, hwiNum);
    return LOS_OK;
}
#endif

CHAR *OsIntVersionGet(VOID)
{
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->getIrqVersion, NULL);
    return g_hwiOps->getIrqVersion();
}

#ifdef LOSCFG_KERNEL_LOWPOWER
LITE_OS_SEC_TEXT_MINOR VOID LOS_IntWakeupHookReg(IntWakeupHookFn hook)
{
    g_intWakeupHook = hook;
}
#endif

UINT32 OsIntNumGet(VOID)
{
    OS_RETURN_ERR_FUNCPTR_IS_NULL(g_hwiOps->getCurIrqNum, 0);
    return g_hwiOps->getCurIrqNum();
}

BOOL OsIntIsRegisted(UINT32 num)
{
    HwiHandleInfo *hwiForm = g_hwiOps->getHandleForm(num);

    if (hwiForm == NULL) {
        return false;
    }
#ifdef LOSCFG_NO_SHARED_IRQ
    return (hwiForm->hook != NULL);
#else
    return (hwiForm->next != NULL);
#endif
}

HWI_ARG_T OsIntGetPara(UINT32 num)
{
    HwiHandleInfo *hwiForm = g_hwiOps->getHandleForm(num);

    if (hwiForm == NULL) {
        return 0;
    }
    return hwiForm->registerInfo;
}

VOID OsHwiControllerReg(const HwiControllerOps *ops)
{
    g_hwiOps = ops;
}

VOID OsIntEntry(VOID)
{
    OS_RETURN_FUNCPTR_IS_NULL(g_hwiOps->handleIrq);
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
