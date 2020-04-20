/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2018. All rights reserved.
 * Description: Exception Handler
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

#include "los_exc_pri.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif
#include "los_hwi_pri.h"
#include "los_memcheck_pri.h"
#include "los_tick_pri.h"
#include "securec.h"
#include "core_cm7.h"
#include "los_printf_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define USGFAULT (1U << 18)
#define BUSFAULT (1U << 17)
#define MEMFAULT (1U << 16)
#define DIV0FAULT (1U << 4)
#define HARD_FAULT_IRQN (-13)
#define MASK_16_BIT 16

UINT32 g_curNestCount = 0;
EXC_INFO_S g_excInfo;
UINT8 g_excTbl[FAULT_STATUS_REG_BIT] = {
    0, 0, 0, 0, 0, 0, OS_EXC_UF_DIVBYZERO, OS_EXC_UF_UNALIGNED,
    0, 0, 0, 0, OS_EXC_UF_NOCP, OS_EXC_UF_INVPC, OS_EXC_UF_INVSTATE, OS_EXC_UF_UNDEFINSTR,
    0, 0, 0, OS_EXC_BF_STKERR, OS_EXC_BF_UNSTKERR, OS_EXC_BF_IMPRECISERR, OS_EXC_BF_PRECISERR, OS_EXC_BF_IBUSERR,
    0, 0, 0, OS_EXC_MF_MSTKERR, OS_EXC_MF_MUNSTKERR, 0, OS_EXC_MF_DACCVIOL, OS_EXC_MF_IACCVIOL
};
ExcInfoArray g_excArray[OS_EXC_TYPE_MAX - 1];
STATIC const CHAR *g_phaseName[] = {
    "fault in init",
    "fault in task",
    "fault in interrupt",
};

__attribute__((noinline)) VOID LOS_Panic(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    PRINT_ERR(fmt, ap);
    va_end(ap);
    asm volatile("swi 0");
}

LITE_OS_SEC_TEXT_INIT VOID OsExcInfoDisplay(const EXC_INFO_S *exc)
{
    PrintExcInfo("Phase      = %s\n"
                 "Type       = 0x%x\n"
                 "FaultAddr  = 0x%x\n"
                 "ThrdPid    = 0x%x\n"
                 "R0         = 0x%x\n"
                 "R1         = 0x%x\n"
                 "R2         = 0x%x\n"
                 "R3         = 0x%x\n"
                 "R4         = 0x%x\n"
                 "R5         = 0x%x\n"
                 "R6         = 0x%x\n"
                 "R7         = 0x%x\n"
                 "R8         = 0x%x\n"
                 "R9         = 0x%x\n"
                 "R10        = 0x%x\n"
                 "R11        = 0x%x\n"
                 "R12        = 0x%x\n"
                 "PriMask    = 0x%x\n"
                 "SP         = 0x%x\n"
                 "LR         = 0x%x\n"
                 "PC         = 0x%x\n"
                 "xPSR       = 0x%x\n",
                 g_phaseName[exc->phase], exc->type, exc->faultAddr, exc->thrdPid, exc->context->uwR0,
                 exc->context->uwR1, exc->context->uwR2, exc->context->uwR3, exc->context->uwR4, exc->context->uwR5,
                 exc->context->uwR6, exc->context->uwR7, exc->context->uwR8, exc->context->uwR9,
                 exc->context->uwR10, exc->context->uwR11, exc->context->uwR12, exc->context->uwPriMask,
                 exc->context->uwSP, exc->context->uwLR, exc->context->uwPC, exc->context->uwxPSR);
    return;
}

LITE_OS_SEC_TEXT_INIT VOID OsExcHandleEntry(UINT32 excType, UINT32 faultAddr, UINT32 pid,
                                            const EXC_CONTEXT_S *excBufAddr)
{
    UINT16 tmpFlag = (excType >> MASK_16_BIT) & OS_NULL_SHORT; /* 2:in intrrupt,1:faul addr valid */
    g_curNestCount++;
    g_tickCount[ArchCurrCpuid()]++;
    g_excInfo.nestCnt = g_curNestCount;

    g_excInfo.type = excType & OS_NULL_SHORT;

    if (tmpFlag & OS_EXC_FLAG_FAULTADDR_VALID) {
        g_excInfo.faultAddr = faultAddr;
    } else {
        g_excInfo.faultAddr = OS_EXC_IMPRECISE_ACCESS_ADDR;
    }

    if (ArchCurrTaskGet() != NULL) {
        if (tmpFlag & OS_EXC_FLAG_IN_HWI) {
            g_excInfo.phase = OS_EXC_IN_HWI;
            g_excInfo.thrdPid = pid;
        } else {
            g_excInfo.phase = OS_EXC_IN_TASK;
            g_excInfo.thrdPid = ((LosTaskCB *)ArchCurrTaskGet())->taskID;
        }
    } else {
        g_excInfo.phase = OS_EXC_IN_INIT;
        g_excInfo.thrdPid = OS_NULL_INT;
    }

    if (excType & OS_EXC_FLAG_NO_FLOAT) {
        g_excInfo.context = (EXC_CONTEXT_S *)((CHAR *)excBufAddr - LOS_OFF_SET_OF(EXC_CONTEXT_S, uwR4));
    } else {
        g_excInfo.context = (EXC_CONTEXT_S *)excBufAddr;
    }

    OsExcInfoDisplay((const EXC_INFO_S *)&g_excInfo);

    while (1) { }
}

LITE_OS_SEC_TEXT_INIT VOID OsExcInit(VOID)
{
    g_hwiVec[HARD_FAULT_IRQN + OS_SYS_VECTOR_CNT] = OsExcHardFault;
    g_hwiVec[NonMaskableInt_IRQn + OS_SYS_VECTOR_CNT] = OsExcNMI;
    g_hwiVec[MemoryManagement_IRQn + OS_SYS_VECTOR_CNT] = OsExcMemFault;
    g_hwiVec[BusFault_IRQn + OS_SYS_VECTOR_CNT] = OsExcBusFault;
    g_hwiVec[UsageFault_IRQn + OS_SYS_VECTOR_CNT] = OsExcUsageFault;
    g_hwiVec[SVCall_IRQn + OS_SYS_VECTOR_CNT] = OsExcSvcCall;
    /* Enable USGFAULT, BUSFAULT, MEMFAULT */
    *(volatile UINT32 *)OS_NVIC_SHCSR |= (USGFAULT | BUSFAULT | MEMFAULT);
    /* Enable DIV 0 and unaligned exception */
    *(volatile UINT32 *)OS_NVIC_CCR |= DIV0FAULT;
}

/* stack protector */
UINT32 __stack_chk_guard = 0xd00a0dff;

VOID __stack_chk_fail(VOID)
{
    /* __builtin_return_address is a builtin function, building in gcc */
    LOS_Panic("stack-protector: Kernel stack is corrupted in: %p\n", __builtin_return_address(0));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
