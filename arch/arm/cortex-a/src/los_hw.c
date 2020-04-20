/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch32 Hw Task Implementation
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

#include "los_hw_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* support cpu vendors */
CpuVendor g_cpuTable[] = {
    /* armv7-a */
    { 0xc07, "Cortex-A7" },
    { 0xc09, "Cortex-A9" },
    { 0, NULL }
};

/* logical cpu mapping */
UINT64 g_cpuMap[LOSCFG_KERNEL_CORE_NUM] = {
    [0 ... LOSCFG_KERNEL_CORE_NUM - 1] = (UINT64)(-1)
};

/* bit[30] is enable FPU */
#define FP_EN (1U << 30)
LITE_OS_SEC_TEXT_INIT VOID OsTaskExit(VOID)
{
    __asm__ __volatile__("swi  0");
}

#ifdef LOSCFG_GDB
STATIC VOID OsTaskEntrySetupLoopFrame(UINT32) __attribute__((noinline, naked));
VOID OsTaskEntrySetupLoopFrame(UINT32 arg0)
{
    asm volatile("\tsub fp, sp, #0x4\n"
                 "\tpush {fp, lr}\n"
                 "\tadd fp, sp, #0x4\n"
                 "\tpush {fp, lr}\n"

                 "\tadd fp, sp, #0x4\n"
                 "\tbl OsTaskEntry\n"

                 "\tpop {fp, lr}\n"
                 "\tpop {fp, pc}\n");
}
#endif

LITE_OS_SEC_TEXT_INIT VOID *OsTaskStackInit(UINT32 taskID, UINT32 stackSize, VOID *topStack)
{
    UINT32 index = 1;
    TaskContext *taskContext = NULL;

    OsStackInit(topStack, stackSize);
    taskContext = (TaskContext *)(((UINTPTR)topStack + stackSize) - sizeof(TaskContext));

    /* initialize the task context */
#ifdef LOSCFG_GDB
    taskContext->PC = (UINTPTR)OsTaskEntrySetupLoopFrame;
#else
    taskContext->PC = (UINTPTR)OsTaskEntry;
#endif
    taskContext->LR = (UINTPTR)OsTaskExit;  /* LR should be kept, to distinguish it's THUMB or ARM instruction */
    taskContext->R[0] = taskID;             /* R0 */
    taskContext->R[index++] = 0x01010101;   /* R1, 0x01010101 : reg initialed magic word */
    for (; index < GEN_REGS_NUM; index++) {
        taskContext->R[index] = taskContext->R[index - 1] + taskContext->R[1]; /* R2 - R12 */
    }

#ifdef LOSCFG_INTERWORK_THUMB
    taskContext->regPSR = PSR_MODE_SVC_THUMB; /* CPSR (Enable IRQ and FIQ interrupts, THUMNB-mode) */
#else
    taskContext->regPSR = PSR_MODE_SVC_ARM;   /* CPSR (Enable IRQ and FIQ interrupts, ARM-mode) */
#endif

#if !defined(LOSCFG_ARCH_FPU_DISABLE)
    /* 0xAAA0000000000000LL : float reg initialed magic word */
    for (index = 0; index < FP_REGS_NUM; index++) {
        taskContext->D[index] = 0xAAA0000000000000LL + index; /* D0 - D31 */
    }
    taskContext->regFPSCR = 0;
    taskContext->regFPEXC = FP_EN;
#endif

    return (VOID *)taskContext;
}

VOID sev(VOID)
{
    __asm__ __volatile__ ("sev" : : : "memory");
}

VOID wfe(VOID)
{
    __asm__ __volatile__ ("wfe" : : : "memory");
}

VOID wfi(VOID)
{
    __asm__ __volatile__ ("wfi" : : : "memory");
}

VOID dmb(VOID)
{
    __asm__ __volatile__ ("dmb" : : : "memory");
}

VOID dsb(VOID)
{
    __asm__ __volatile__("dsb" : : : "memory");
}

VOID isb(VOID)
{
    __asm__ __volatile__("isb" : : : "memory");
}

VOID flush_icache(VOID)
{
    /*
     * Use ICIALLUIS instead of ICIALLU. ICIALLUIS operates on all processors in the Inner
     * shareable domain of the processor that performs the operation.
     */
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c1, 0" : : "r" (0) : "memory");
}

VOID flush_dcache(UINT32 start, UINT32 end)
{
    arm_clean_cache_range(start, end);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
