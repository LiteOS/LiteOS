/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch64 Exc Implementation
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

#include "arch/exception.h"
#include "arch/regs.h"

#include "los_memory_pri.h"
#include "los_printf_pri.h"
#include "los_task_pri.h"
#include "los_exc_pri.h"
#include "los_stackinfo_pri.h"
#include "los_mp_pri.h"

#ifdef LOSCFG_KERNEL_TRACE
#include "los_trace_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC EXC_PROC_FUNC g_excHook = NULL;
UINT32 g_curNestCount = 0;

#define OS_MAX_BACKTRACE 15
#define DUMPSIZE         128U
#define DUMPREGS         30
#define FP_NUM           29

#define NBIT(val, high, low) (((val) >> (low)) & ((1U << (((high) - (low)) + 1)) - 1))
#define GET_IL(esr) ((esr) & (1U << 25))
#define IS_VALID_ADDR(ptr) (((ptr) >= SYS_MEM_BASE) &&       \
                            ((ptr) <= g_sys_mem_addr_end) && \
                            IS_ALIGNED((ptr), sizeof(CHAR *)))

VOID OsDecodeDataAbortISS(UINT32 bitsISS)
{
    UINT32 bitFnV = bitsISS & (1U << 10);   /* FnV bit[10] */
    UINT32 bitWnR = bitsISS & (1U << 6);    /* WnR bit[6] */
    UINT32 bitsDFSC = NBIT(bitsISS, 5, 0);  /* DFSC bits[5:0] */

    if (!bitFnV) {
        if (bitWnR) {
            PRINTK("Abort caused by a write instruction.");
        } else {
            PRINTK("Abort caused by a read instruction.");
        }
        switch (bitsDFSC) {
            case 0x21:  /* 0b100001 */
                PRINTK("Alignment fault.\n");
                break;
            case 0x0:   /* 0b000000 */
            case 0x01:  /* 0b000001 */
            case 0x03:  /* 0b000011 */
            case 0x04:  /* 0b000100 */
                PRINTK("Address size fault.\n");
                break;
            default:
                PRINTK("\nOMG!UNKNOWN fault, "
                       "check ISS encoding for an exception from a Data Abort in AARM for armv8-a.\n");
                break;
        }
    } else {
        PRINTK("FAR is not valid, and holds an UNKNOWN value.\n");
    }
}

STATIC VOID OsExcType(const ExcContext *frame, UINT32 regESR, UINT32 bitsEC)
{
    UINT32 bitIL = GET_IL(regESR);
    UINT32 bitsISS = NBIT(regESR, 24, 0); /* Instruction Specific Syndrome in ESR_ELx[24:0] */
    UINT64 regFAR;

    switch (bitsEC) {
        case 0x24:  /* 0b100100 */
        case 0x25:  /* 0b100101 */
            PrintExcInfo("Data Abort.\n");
            /* read the FAR register */
            regFAR = AARCH64_SYSREG_READ(FAR_ELx);
            /* decode the iss */
            if (bitIL) {
                OsDecodeDataAbortISS(bitsISS);
            }
            PrintExcInfo("PC at 0x%llx,FAR 0x%llx,ISS 0x%x\n", frame->regELR, regFAR, bitsISS);
            break;
        case 0x20:  /* 0b100000 */
        case 0x21:  /* 0b100001 */
            PrintExcInfo("Instruction Abort.\n");
            break;
        case 0x22:  /* 0b100010 */
            PrintExcInfo("PC alignment fault.\n");
            break;
        case 0x26:  /* 0b100110 */
            PrintExcInfo("SP alignment fault.\n");
            break;
        default:
            PrintExcInfo("OMG!Unknown synchronous exception, "
                         "check Exception Syndrome Register in AARM for armv8-a.\n");
            break;
    }
    PrintExcInfo("ESR 0x%x: ec 0x%x, il 0x%x, iss 0x%x\n", regESR, bitsEC, bitIL, bitsISS);
}

UINT32 ArchSetExcHook(EXC_PROC_FUNC excHook)
{
    UINT32 intSave;

    intSave = ArchIntLock();
    g_excHook = excHook;
    ArchIntRestore(intSave);
    return 0;
}

UINT32 ArchBackTraceGet(UINTPTR fp, UINTPTR *callChain, UINT32 maxDepth)
{
    UINTPTR tmpFp;
    UINTPTR backLr;
    UINTPTR backFp = fp;
    UINT32 count = 0;

    while ((backFp > OS_SYS_FUNC_ADDR_START) && (backFp < OS_SYS_FUNC_ADDR_END)) {
        tmpFp = backFp;
        backFp = *((UINTPTR *)(tmpFp));
        backLr = *((UINTPTR *)(tmpFp + sizeof(CHAR *)));

        if (callChain == NULL) {
            PrintExcInfo("traceback %u -- lr = 0x%llx    fp = 0x%llx\n", count, backLr, backFp);
        } else {
            callChain[count] = backLr;
        }

        count++;
        if (count == maxDepth) {
            break;
        }
    }
    return count;
}

STATIC VOID BackTraceSub(UINTPTR fp)
{
    (VOID)ArchBackTraceGet(fp, NULL, OS_MAX_BACKTRACE);
}

STATIC VOID BackTraceWithFp(UINTPTR fp)
{
    UINTPTR backFp;

    if ((VOID *)fp == NULL) {
        backFp = ArchGetFp();
    } else {
        backFp = fp;
    }

    PrintExcInfo("*******backtrace begin*******\n");
    BackTraceSub(backFp);
}

VOID ArchBackTrace(VOID)
{
    BackTraceWithFp((UINTPTR)NULL);
}

VOID ArchBackTraceWithSp(const VOID *stackPointer)
{
    UINTPTR fp = ArchGetTaskFp(stackPointer);
    BackTraceSub(fp);
}

VOID OsCallStackInfo(VOID)
{
    UINT32 count = 0;
    LosTaskCB *runTask = OsCurrTaskGet();

    UINTPTR *stackBottom = (UINTPTR *)(runTask->topOfStack + runTask->stackSize);
    UINTPTR *stackPointer = stackBottom;

    PrintExcInfo("runTask->stackPointer = 0x%llx\n"
                 "runTask->topOfStack = 0x%llx\n"
                 "text_start = 0x%llx,text_end = 0x%llx\n",
                 stackPointer, runTask->topOfStack, &__text_start, &__text_end);

    while ((stackPointer > (UINTPTR *)runTask->topOfStack) && (count < OS_MAX_BACKTRACE)) {
        if ((*stackPointer > (UINTPTR)(&__text_start)) &&
            (*stackPointer < (UINTPTR)(&__text_end)) &&
            IS_ALIGNED(*stackPointer, sizeof(CHAR *))) {
            if ((*(stackPointer - 1) > (UINTPTR)runTask->topOfStack) &&
                (*(stackPointer - 1) < (UINTPTR)stackBottom) &&
                IS_ALIGNED(*(stackPointer - 1), sizeof(CHAR *))) {
                count++;
                PrintExcInfo("traceback %u -- lr = 0x%llx\n", count, *stackPointer);
            }
        }
        stackPointer--;
    }
    PRINTK("\n");
}

VOID OsDumpContextMem(const ExcContext *excBufAddr)
{
    UINT32 count = 0;
    const UINT64 *excReg = NULL;

    for (excReg = &(excBufAddr->X[0]); count <= (DUMPREGS - 1); excReg++, count++) {
        if (IS_VALID_ADDR(*excReg)) {
            PrintExcInfo("\ndump mem around X%u:%p", count, (*excReg));
            OsDumpMemByte(DUMPSIZE, ((*excReg) - (DUMPSIZE >> 1)));
        }
    }

    if (IS_VALID_ADDR(excBufAddr->SP)) {
        PrintExcInfo("\ndump mem around SP:%p", excBufAddr->SP);
        OsDumpMemByte(DUMPSIZE, (excBufAddr->SP - (DUMPSIZE >> 1)));
    }
}

STATIC const StackInfo g_excStack[] = {
    {&__stack_startup, OS_EXC_START_STACK_SIZE, "start_stack"},
#ifdef LOSCFG_IRQ_USE_STANDALONE_STACK
    {&__irq_stack, OS_EXC_IRQ_STACK_SIZE, "irq_stack"}
#endif
};

VOID ArchExcInit(VOID)
{
    OsExcStackInfoReg(g_excStack, sizeof(g_excStack) / sizeof(g_excStack[0]));
}

VOID OsExcDumpContext(const ExcContext *excBufAddr)
{
    UINT32 i;
    LosTaskCB *runTask = OsCurrTaskGet();

    PrintExcInfo("taskName = %s\nTaskId = %u\nTask StackSize = %u\n"
                 "system mem addr = %p\nPC = 0x%llx\nLR = 0x%llx\n",
                 runTask->taskName, runTask->taskId, runTask->stackSize,
                 m_aucSysMem0, excBufAddr->regELR, excBufAddr->LR);

    for (i = 0; i < DUMPREGS; i++) {
        PrintExcInfo("X%-02d = 0x%llx\n", i, excBufAddr->X[i]);
    }

    PrintExcInfo("LR = 0x%llx\nELR = 0x%llx\nSPSR = 0x%llx\n",
                 excBufAddr->LR, excBufAddr->regELR, excBufAddr->SPSR);

    BackTraceSub(excBufAddr->X[FP_NUM]);
#ifdef LOSCFG_SHELL
    (VOID)OsShellCmdTskInfoGet(OS_ALL_TASK_MASK);
#endif
    OsExcStackInfo();
    OsDumpContextMem(excBufAddr);
#ifdef LOSCFG_KERNEL_MEM_BESTFIT
    OsMemIntegrityMultiCheck();
#endif
#ifdef LOSCFG_KERNEL_TRACE
    OsTraceRecordDump(FALSE);
#endif
}

VOID OsCurrentELGet(UINT32 reg)
{
    PRINT_ERR("The current exception level is EL%d\n", NBIT(reg, 3, 2));
}

VOID OsExceptSyncExcHdl(ExcContext *frame)
{
    UINT32 regESR = AARCH64_SYSREG_READ(ESR_ELx);
    UINT32 bitsEC = NBIT(regESR, 31, 26);       /* get the 26-31bit for EC */

#if (LOSCFG_KERNEL_SMP == YES)
    /* use halt ipi to stop other active cores */
    UINT32 target = (UINT32)(OS_MP_CPU_ALL & ~CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
    HalIrqSendIpi(target, LOS_MP_IPI_HALT);
#endif

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
    LogReadWriteFunc func = OsGetExcInfoRW();
#endif

    g_curNestCount++;

    if (g_curNestCount == 1) {
        if (g_excHook != NULL) {
            g_excHook(0, frame);
        }

#ifdef LOSCFG_SHELL_EXCINFO_DUMP
        if (func != NULL) {
            SetExcInfoIndex(0);
        }
        OsRecordExcInfoTime();
#endif
        PrintExcInfo("/*******************Exception Information*******************/"
                     "\nExcClass:0x%x => ", bitsEC);

        OsExcType(frame, regESR, bitsEC);
        OsExcDumpContext(frame);

        PrintExcInfo("/*******************Exception Information END*****************/\n");
    } else {
        OsCallStackInfo();
    }
#ifdef LOSCFG_SHELL_EXCINFO_DUMP
    if (func != NULL) {
        PrintExcInfo("Be sure your space bigger than OsOsGetExcInfoOffset():0x%x\n", OsGetExcInfoOffset());
        func(OsGetExcInfoDumpAddr(), OsGetExcInfoLen(), 0, OsGetExcInfoBuf());
    }
#endif
#ifdef LOSCFG_EXC_INTERACTION
    OsKeepExcInteractionTask();
#endif
    for (;;) {
        ;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
