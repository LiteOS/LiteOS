/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ARM Hwi Implementation
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

#if (LOSCFG_PLATFORM_HWI == YES)
#include "los_memory.h"
#include "los_spinlock.h"

#if (LOSCFG_KERNEL_TRACE == YES)
#include "los_trace.h"
#endif

#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#if (LOSCFG_KERNEL_TICKLESS == YES)
#include "los_tickless_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* spinlock for hwi module */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_hwiSpin);
#define HWI_LOCK(state)       LOS_SpinLockSave(&g_hwiSpin, &(state))
#define HWI_UNLOCK(state)     LOS_SpinUnlockRestore(&g_hwiSpin, (state))

size_t g_intCount[LOSCFG_KERNEL_CORE_NUM] = {0};
HWI_HANDLE_FORM_S g_hwiForm[OS_HWI_MAX_NUM + OS_SYS_VECTOR_CNT];
STATIC CHAR *g_hwiFormName[OS_HWI_MAX_NUM] = {0};
STATIC UINT32 g_hwiFormCnt[OS_HWI_MAX_NUM] = {0};

VOID OsIncHwiFormCnt(UINT32 index)
{
    g_hwiFormCnt[index]++;
}

UINT32 OsGetHwiFormCnt(UINT32 index)
{
    return g_hwiFormCnt[index];
}

CHAR *OsGetHwiFormName(UINT32 index)
{
    return g_hwiFormName[index];
}

typedef VOID (*HWI_PROC_FUNC0)(VOID);
typedef VOID (*HWI_PROC_FUNC2)(INT32, VOID *);

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
STATIC INLINE VOID OsIrqNestingActive(UINT32 intNum)
{
    /* preemption not allowed when handling tick interrupt */
    if (intNum != OS_TICK_INT_NUM) {
        (VOID)LOS_IntUnLock();
    }
}

STATIC INLINE VOID OsIrqNestingInactive(UINT32 intNum)
{
    if (intNum != OS_TICK_INT_NUM) {
        (VOID)LOS_IntLock();
    }
}

size_t OsIrqNestingCntGet(VOID)
{
    return g_intCount[ArchCurrCpuid()];
}
#endif

STATIC INLINE VOID OsIrqError(UINT32 intNum)
{
    PRINT_ERR("%s irqnum:%d\n", __FUNCTION__, intNum);
    for (; ;) { }
}

VOID OsInterrupt(UINT32 intNum)
{
    HWI_HANDLE_FORM_S *hwiForm = NULL;
    size_t *intCnt = NULL;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqStart();
#endif

    intCnt = &g_intCount[ArchCurrCpuid()];
    *intCnt = *intCnt + 1;

#if (LOSCFG_KERNEL_TRACE == YES)
    LOS_Trace(LOS_TRACE_INTERRUPT, intNum, IRQ_DIRECT_IN);
#endif

#if (LOSCFG_KERNEL_TICKLESS == YES)
    OsTicklessUpdate(intNum);
#endif

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    OsIrqNestingActive(intNum);
#endif

    hwiForm = (&g_hwiForm[intNum]);
#ifndef LOSCFG_NO_SHARED_IRQ
    while (hwiForm->pstNext != NULL) {
        hwiForm = hwiForm->pstNext;
#endif

        if (hwiForm->uwParam) {
            HWI_PROC_FUNC2 func = (HWI_PROC_FUNC2)hwiForm->pfnHook;
            if (func != NULL) {
                UINTPTR *param = (UINTPTR *)(hwiForm->uwParam);
                func((INT32)(*param), (VOID *)(*(param + 1)));
            } else {
                OsIrqError(intNum);
            }
        } else {
            HWI_PROC_FUNC0 func = (HWI_PROC_FUNC0)hwiForm->pfnHook;
            if (func != NULL) {
                func();
            } else {
                OsIrqError(intNum);
            }
        }
#ifndef LOSCFG_NO_SHARED_IRQ
    }
#endif

    ++g_hwiFormCnt[intNum];

#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    OsIrqNestingInactive(intNum);
#endif

#if (LOSCFG_KERNEL_TRACE == YES)
    LOS_Trace(LOS_TRACE_INTERRUPT, intNum, IRQ_DIRECT_OUT);
#endif

    *intCnt = *intCnt - 1;

#ifdef LOSCFG_CPUP_INCLUDE_IRQ
    OsCpupIrqEnd(intNum);
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
STATIC UINT32 OsHwiDelNoShared(HWI_HANDLE_T hwiNum)
{
    UINT32 intSave;

    HWI_LOCK(intSave);
    g_hwiForm[hwiNum].pfnHook = NULL;
    if (g_hwiForm[hwiNum].uwParam) {
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)g_hwiForm[hwiNum].uwParam);
    }
    g_hwiForm[hwiNum].uwParam = 0;

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateNoShared(HWI_HANDLE_T hwiNum, HWI_MODE_T hwiMode,
                                  HWI_PROC_FUNC hwiHandler, const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_ARG_T retParam;
    UINT32 intSave;

    (VOID)hwiMode;
    HWI_LOCK(intSave);
    if (g_hwiForm[hwiNum].pfnHook == NULL) {
        g_hwiForm[hwiNum].pfnHook = hwiHandler;

        retParam = OsHwiCpIrqParam(irqParam);
        if (retParam == LOS_NOK) {
            HWI_UNLOCK(intSave);
            return OS_ERRNO_HWI_NO_MEMORY;
        }
        g_hwiForm[hwiNum].uwParam = retParam;
    } else {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_ALREADY_CREATED;
    }
    HWI_UNLOCK(intSave);
    return LOS_OK;
}
#else
STATIC UINT32 OsHwiDelShared(HWI_HANDLE_T hwiNum, const HWI_IRQ_PARAM_S *irqParam)
{
    HWI_HANDLE_FORM_S *hwiForm = NULL;
    HWI_HANDLE_FORM_S *hwiFormtmp = NULL;
    UINT32 hwiValid = FALSE;
    UINT32 intSave;

    HWI_LOCK(intSave);
    hwiForm = &g_hwiForm[hwiNum];
    hwiFormtmp = hwiForm;

    if ((hwiForm->uwParam & IRQF_SHARED) && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    if ((hwiForm->pstNext != NULL) && !(hwiForm->uwParam & IRQF_SHARED)) {
        hwiForm = hwiForm->pstNext;
        if (hwiForm->uwParam) {
            (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->uwParam);
        }
        (VOID)LOS_MemFree(m_aucSysMem0, hwiForm);
        hwiFormtmp->pstNext = NULL;

        g_hwiFormName[hwiNum] = NULL;

        HWI_UNLOCK(intSave);
        return LOS_OK;
    }
    hwiForm = hwiForm->pstNext;
    while (hwiForm != NULL) {
        if (((HWI_IRQ_PARAM_S *)(hwiForm->uwParam))->pDevId != irqParam->pDevId) {
            hwiFormtmp = hwiForm;
            hwiForm = hwiForm->pstNext;
        } else {
            hwiFormtmp->pstNext = hwiForm->pstNext;
            (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)hwiForm->uwParam);
            (VOID)LOS_MemFree(m_aucSysMem0, hwiForm);

            hwiValid = TRUE;
            break;
        }
    }

    if (hwiValid != TRUE) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_HWINUM_UNCREATE;
    }

    if (g_hwiForm[hwiNum].pstNext == NULL) {
        g_hwiForm[hwiNum].uwParam = 0;
        g_hwiFormName[hwiNum] = NULL;
    }

    HWI_UNLOCK(intSave);
    return LOS_OK;
}

STATIC UINT32 OsHwiCreateShared(HWI_HANDLE_T hwiNum, HWI_MODE_T hwiMode,
                                HWI_PROC_FUNC hwiHandler, const HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 intSave;
    HWI_HANDLE_FORM_S *hwiFormNode = NULL;
    HWI_HANDLE_FORM_S *hwiForm = NULL;
    HWI_IRQ_PARAM_S *hwiParam = NULL;
    HWI_MODE_T modeResult = hwiMode & IRQF_SHARED;

    if (modeResult && ((irqParam == NULL) || (irqParam->pDevId == NULL))) {
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

    HWI_LOCK(intSave);

    hwiForm = &g_hwiForm[hwiNum];
    if ((hwiForm->pstNext != NULL) && ((modeResult == 0) || (!(hwiForm->uwParam & IRQF_SHARED)))) {
        HWI_UNLOCK(intSave);
        return OS_ERRNO_HWI_SHARED_ERROR;
    }

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
        (VOID)LOS_MemFree(m_aucSysMem0, hwiFormNode);
        return OS_ERRNO_HWI_NO_MEMORY;
    }

    hwiFormNode->pfnHook = hwiHandler;
    hwiFormNode->pstNext = (struct tagHwiHandleForm *)NULL;
    hwiForm->pstNext = hwiFormNode;

    if ((irqParam != NULL) && (irqParam->pName != NULL)) {
        g_hwiFormName[hwiNum] = (CHAR *)irqParam->pName;
    }

    g_hwiForm[hwiNum].uwParam = modeResult;

    HWI_UNLOCK(intSave);
    return LOS_OK;
}
#endif

/*
 * Description : initialization of the hardware interrupt
 */
LITE_OS_SEC_TEXT_INIT VOID OsHwiInit(VOID)
{
    UINT32 hwiNum;

    for (hwiNum = 0; hwiNum < OS_HWI_MAX_NUM; hwiNum++) {
        g_hwiForm[hwiNum].pfnHook = NULL;
        g_hwiForm[hwiNum].uwParam = 0;
        g_hwiForm[hwiNum].pstNext = NULL;
    }

    (VOID)memset_s(g_hwiFormName, (sizeof(CHAR *) * OS_HWI_MAX_NUM), 0, (sizeof(CHAR *) * OS_HWI_MAX_NUM));

    HalIrqInit();

    return;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiCreate(HWI_HANDLE_T hwiNum,
                                           HWI_PRIOR_T hwiPrio,
                                           HWI_MODE_T hwiMode,
                                           HWI_PROC_FUNC hwiHandler,
                                           HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 ret;

    if (hwiHandler == NULL) {
        return OS_ERRNO_HWI_PROC_FUNC_NULL;
    }
    if ((hwiNum > OS_USER_HWI_MAX) || ((INT32)hwiNum < OS_USER_HWI_MIN)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

#ifdef LOSCFG_NO_SHARED_IRQ
    ret = OsHwiCreateNoShared(hwiNum + OS_SYS_VECTOR_CNT, hwiMode, hwiHandler, irqParam);
#else
    ret = OsHwiCreateShared(hwiNum + OS_SYS_VECTOR_CNT, hwiMode, hwiHandler, irqParam);
#endif
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
    if (ret == LOS_OK) {
        ret = HalIrqSetPrio(hwiNum, (UINT8)hwiPrio);
    }
#endif
    if (ret == LOS_OK) {
        (VOID)HalIrqCreate(hwiNum, (UINT8)hwiPrio);
    }

    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDelete(HWI_HANDLE_T hwiNum, HWI_IRQ_PARAM_S *irqParam)
{
    UINT32 ret;

    if ((hwiNum > OS_USER_HWI_MAX) || ((INT32)hwiNum < OS_USER_HWI_MIN)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

#ifdef LOSCFG_NO_SHARED_IRQ
    (VOID)irqParam;
    ret = OsHwiDelNoShared(hwiNum + OS_SYS_VECTOR_CNT);
#else
    ret = OsHwiDelShared(hwiNum + OS_SYS_VECTOR_CNT, irqParam);
#endif
    if (ret == LOS_OK) {
        (VOID)HalIrqDelete(hwiNum);
    }
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
