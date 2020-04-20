/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: ARMv7 Exc Implementation
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

#include "los_exc.h"
#include "los_memory_pri.h"
#include "los_printf_pri.h"
#include "los_task_pri.h"
#include "los_hw_pri.h"
#ifdef LOSCFG_SHELL_EXCINFO
#include "los_excinfo_pri.h"
#endif
#ifdef LOSCFG_EXC_INTERACTION
#include "los_exc_interaction_pri.h"
#endif
#include "los_sys_stack_pri.h"
#include "los_stackinfo_pri.h"
#ifdef LOSCFG_COREDUMP
#include "los_coredump.h"
#endif
#ifdef LOSCFG_GDB
#include "gdb_int.h"
#endif
#include "los_mp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID OsExcHook(UINT32 excType, ExcContext *excBufAddr);
UINT32 g_curNestCount = 0;
STATIC EXC_PROC_FUNC g_excHook = (EXC_PROC_FUNC)OsExcHook;

#define OS_MAX_BACKTRACE 15U
#define DUMPSIZE         128U
#define DUMPREGS         12U
#define INSTR_SET_MASK   0x01000020U
#define THUMB_INSTR_LEN  2U
#define ARM_INSTR_LEN    4U
#define POINTER_SIZE     4U

#define GET_FS(fsr) (((fsr) & 0xFU) | (((fsr) & (1U << 10)) >> 6))
#define GET_WNR(dfsr) ((dfsr) & (1U << 11))

#define IS_VALID_ADDR(ptr) (((ptr) >= SYS_MEM_BASE) &&       \
                            ((ptr) <= g_sys_mem_addr_end) && \
                            IS_ALIGNED((ptr), sizeof(CHAR *)))

STATIC const StackInfo g_excStack[] = {
    { &__undef_stack, OS_EXC_UNDEF_STACK_SIZE, "udf_stack" },
    { &__abt_stack,   OS_EXC_ABT_STACK_SIZE,   "abt_stack" },
    { &__fiq_stack,   OS_EXC_FIQ_STACK_SIZE,   "fiq_stack" },
    { &__svc_stack,   OS_EXC_SVC_STACK_SIZE,   "svc_stack" },
    { &__irq_stack,   OS_EXC_IRQ_STACK_SIZE,   "irq_stack" },
    { &__exc_stack,   OS_EXC_STACK_SIZE,       "exc_stack" }
};

STATIC INT32 OsDecodeFS(UINT32 bitsFS)
{
    switch (bitsFS) {
        case 0x05:  /* 0b00101 */
        case 0x07:  /* 0b00111 */
            PrintExcInfo("Translation fault, %s\n", (bitsFS & 0x2) ? "page" : "section");
            break;
        case 0x09:  /* 0b01001 */
        case 0x0b:  /* 0b01011 */
            PrintExcInfo("Domain fault, %s\n", (bitsFS & 0x2) ? "page" : "section");
            break;
        case 0x0d:  /* 0b01101 */
        case 0x0f:  /* 0b01111 */
            PrintExcInfo("Permission fault, %s\n", (bitsFS & 0x2) ? "page" : "section");
            break;
        default:
            PrintExcInfo("Unknown fault! FS:0x%x. "
                         "Check IFSR and DFSR in ARM Architecture Reference Manual.\n",
                         bitsFS);
            break;
    }

    return LOS_OK;
}

STATIC INT32 OsDecodeInstructionFSR(UINT32 regIFSR)
{
    INT32 ret;
    UINT32 bitsFS = GET_FS(regIFSR); /* FS bits[4]+[3:0] */

    ret = OsDecodeFS(bitsFS);
    return ret;
}

STATIC INT32 OsDecodeDataFSR(UINT32 regDFSR)
{
    INT32 ret = 0;
    UINT32 bitWnR = GET_WNR(regDFSR); /* WnR bit[11] */
    UINT32 bitsFS = GET_FS(regDFSR);  /* FS bits[4]+[3:0] */

    if (bitWnR) {
        PrintExcInfo("Abort caused by a write instruction. ");
    } else {
        PrintExcInfo("Abort caused by a read instruction. ");
    }

    if (bitsFS == 0x01) { /* 0b00001 */
        PrintExcInfo("Alignment fault.\n");
        return ret;
    }
    ret = OsDecodeFS(bitsFS);
    return ret;
}

STATIC VOID OsExcType(UINT32 excType, ExcContext *excBufAddr)
{
    /* undefinited exception handling or software interrupt */
    if ((excType == OS_EXCEPT_UNDEF_INSTR) || (excType == OS_EXCEPT_SWI)) {
        if ((excBufAddr->regCPSR & INSTR_SET_MASK) == 0) { /* work status: ARM */
            excBufAddr->PC = excBufAddr->PC - ARM_INSTR_LEN;
        } else if ((excBufAddr->regCPSR & INSTR_SET_MASK) == 0x20) { /* work status: Thumb */
            excBufAddr->PC = excBufAddr->PC - THUMB_INSTR_LEN;
        }
    }

    if (excType == OS_EXCEPT_PREFETCH_ABORT) {
        PrintExcInfo("prefetch_abort fault fsr:0x%x, far:0x%0+8x\n", OsGetIFSR(), OsGetIFAR());
        (VOID)OsDecodeInstructionFSR(OsGetIFSR());
    } else if (excType == OS_EXCEPT_DATA_ABORT) {
        PrintExcInfo("data_abort fsr:0x%x, far:0x%0+8x\n", OsGetDFSR(), OsGetDFAR());
        (VOID)OsDecodeDataFSR(OsGetDFSR());
    }
}

STATIC const CHAR *g_excTypeString[] = {
    "reset",
    "undefined instruction",
    "software interrupt",
    "prefetch abort",
    "data abort",
    "fiq",
    "address abort",
    "irq"
};

STATIC VOID OsExcSysInfo(UINT32 excType, const ExcContext *excBufAddr)
{
    LosTaskCB *runTask = OsCurrTaskGet();

    PrintExcInfo("excType:%s\n"
                 "taskName = %s\n"
                 "taskID = %u\n"
                 "task stackSize = %u\n"
                 "system mem addr = 0x%x\n"
                 "excBuffAddr pc = 0x%x\n"
                 "excBuffAddr lr = 0x%x\n"
                 "excBuffAddr sp = 0x%x\n"
                 "excBuffAddr fp = 0x%x\n",
                 g_excTypeString[excType],
                 runTask->taskName,
                 runTask->taskID,
                 runTask->stackSize,
                 m_aucSysMem0,
                 excBufAddr->PC,
                 excBufAddr->LR,
                 excBufAddr->SP,
                 excBufAddr->R11);
}

STATIC VOID OsExcRegsInfo(const ExcContext *excBufAddr)
{
    PrintExcInfo("R0         = 0x%x\n"
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
                 "CPSR       = 0x%x\n",
                 excBufAddr->R0, excBufAddr->R1, excBufAddr->R2,
                 excBufAddr->R3, excBufAddr->R4, excBufAddr->R5,
                 excBufAddr->R6, excBufAddr->R7, excBufAddr->R8,
                 excBufAddr->R9, excBufAddr->R10, excBufAddr->R11,
                 excBufAddr->R12, excBufAddr->regCPSR);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_ExcRegHook(EXC_PROC_FUNC excHook)
{
    UINT32 intSave;

    intSave = LOS_IntLock();
    g_excHook = excHook;
    LOS_IntRestore(intSave);

    return LOS_OK;
}

EXC_PROC_FUNC OsExcRegHookGet(VOID)
{
    return g_excHook;
}

VOID OsDumpContextMem(const ExcContext *excBufAddr)
{
    UINT32 count = 0;
    const UINT32 *excReg = NULL;

    for (excReg = &(excBufAddr->R0); count <= DUMPREGS; excReg++, count++) {
        if (IS_VALID_ADDR(*excReg)) {
            PrintExcInfo("\ndump mem around R%u:%p", count, (*excReg));
            OsDumpMemByte(DUMPSIZE, ((*excReg) - (DUMPSIZE >> 1)));
        }
    }

    if (IS_VALID_ADDR(excBufAddr->SP)) {
        PrintExcInfo("\ndump mem around SP:%p", excBufAddr->SP);
        OsDumpMemByte(DUMPSIZE, (excBufAddr->SP - (DUMPSIZE >> 1)));
    }
}

/* this function is used to validate fp or validate the checking range start and end. */
STATIC INLINE BOOL IsValidFP(UINTPTR regFP, UINTPTR start, UINTPTR end)
{
    return (regFP > start) && (regFP < end);
}

STATIC INLINE BOOL FindSuitableStack(UINTPTR regFP, UINTPTR *start, UINTPTR *end)
{
    UINT32 index, stackStart, stackEnd;
    BOOL found = FALSE;
    LosTaskCB *taskCB = NULL;
    const StackInfo *stack = NULL;

    /* Search in the task stacks */
    for (index = 0; index < g_taskMaxNum; index++) {
        taskCB = &g_taskCBArray[index];
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        stackStart = taskCB->topOfStack;
        stackEnd = taskCB->topOfStack + taskCB->stackSize;
        if (IsValidFP(regFP, stackStart, stackEnd)) {
            found = TRUE;
            goto FOUND;
        }
    }

    /* Search in the exc stacks */
    for (index = 0; index < sizeof(g_excStack) / sizeof(StackInfo); index++) {
        stack = &g_excStack[index];
        stackStart = (UINTPTR)stack->stackTop;
        stackEnd = stackStart + LOSCFG_KERNEL_CORE_NUM * stack->stackSize;
        if (IsValidFP(regFP, stackStart, stackEnd)) {
            found = TRUE;
            goto FOUND;
        }
    }

FOUND:
    if (found == TRUE) {
        *start = stackStart;
        *end = stackEnd;
    }

    return found;
}

VOID BackTraceSub(UINTPTR regFP)
{
    UINTPTR tmpFP;
    UINTPTR backLR;
    UINTPTR backFP = regFP;
    UINTPTR stackStart, stackEnd;
    UINT32 count = 0;

    if (FindSuitableStack(regFP, &stackStart, &stackEnd) == FALSE) {
        return;
    }

    /*
     * Check whether it is the leaf function.
     * Generally, the frame pointer points to the address of link register, while in the leaf function,
     * there's no function call, and compiler will not store the link register, but the frame pointer
     * will still be stored and updated. In that case we needs to find the right position of frame pointer.
     */
    tmpFP = *((UINTPTR *)(regFP));
    if (IsValidFP(tmpFP, stackStart, stackEnd)) {
        backFP = tmpFP;
        PrintExcInfo("traceback fp fixed, trace using   fp = 0x%x\n", backFP);
    }

    while (IsValidFP(backFP, stackStart, stackEnd)) {
        tmpFP = backFP;
        backLR = *((UINTPTR *)(tmpFP));
        backFP = *((UINTPTR *)(tmpFP - POINTER_SIZE));
        PrintExcInfo("traceback %u -- lr = 0x%x    fp = 0x%x\n", count, backLR, backFP);

        count++;
        if ((count == OS_MAX_BACKTRACE) || (backFP == tmpFP)) {
            break;
        }
    }
}

VOID BackTrace(UINT32 regFP)
{
    PrintExcInfo("*******backtrace begin*******\n");

    BackTraceSub(regFP);
}

VOID OsExcInit(VOID)
{
    OsExcStackInfoReg(g_excStack, sizeof(g_excStack) / sizeof(g_excStack[0]));
}

VOID OsExcHook(UINT32 excType, ExcContext *excBufAddr)
{
    OsExcType(excType, excBufAddr);
    OsExcSysInfo(excType, excBufAddr);
    OsExcRegsInfo(excBufAddr);

    BackTrace(excBufAddr->R11);
    (VOID)OsShellCmdTskInfoGet(OS_ALL_TASK_MASK);

    OsExcStackInfo();

    OsDumpContextMem(excBufAddr);

    (VOID)OsShellCmdMemCheck(0, NULL);

#ifdef LOSCFG_COREDUMP
    LOS_CoreDumpV2(excType, excBufAddr);
#endif
}

VOID OsCallStackInfo(VOID)
{
    UINT32 count = 0;
    LosTaskCB *runTask = OsCurrTaskGet();
    UINTPTR stackBottom = runTask->topOfStack + runTask->stackSize;
    UINT32 *stackPointer = (UINT32 *)stackBottom;

    PrintExcInfo("runTask->stackPointer = 0x%x\n"
                 "runTask->topOfStack = 0x%x\n"
                 "text_start:0x%x,text_end:0x%x\n",
                 stackPointer, runTask->topOfStack, &__text_start, &__text_end);

    while ((stackPointer > (UINT32 *)runTask->topOfStack) && (count < OS_MAX_BACKTRACE)) {
        if ((*stackPointer > (UINTPTR)(&__text_start)) &&
            (*stackPointer < (UINTPTR)(&__text_end)) &&
            IS_ALIGNED((*stackPointer), POINTER_SIZE)) {
            if ((*(stackPointer - 1) > (UINT32)runTask->topOfStack) &&
                (*(stackPointer - 1) < stackBottom) &&
                IS_ALIGNED((*(stackPointer - 1)), POINTER_SIZE)) {
                count++;
                PrintExcInfo("traceback %u -- lr = 0x%x\n", count, *stackPointer);
            }
        }
        stackPointer--;
    }
    PRINTK("\n");
}

VOID OsTaskBackTrace(UINT32 taskID)
{
    LosTaskCB *taskCB = NULL;

    if (taskID >= g_taskMaxNum) {
        PRINT_ERR("\r\nTask PID is invalid!\n");
        return;
    }
    taskCB = OS_TCB_FROM_TID(taskID);
    if ((taskCB->taskStatus & OS_TASK_STATUS_UNUSED) ||
        (taskCB->taskEntry == NULL) ||
        (taskCB->taskName == NULL)) {
        PRINT_ERR("\r\nThe task is not created!\n");
        return;
    }
    PRINTK("TaskName = %s\n", taskCB->taskName);
    PRINTK("TaskID = 0x%x\n", taskCB->taskID);
    BackTrace(((TaskContext *)(taskCB->stackPointer))->R[11]); /* R11 : FP */
}

VOID OsBackTrace(VOID)
{
    UINT32 regFP = Get_Fp();
    LosTaskCB *runTask = OsCurrTaskGet();
    PRINTK("OsBackTrace fp = 0x%x\n", regFP);
    PRINTK("runTask->taskName = %s\n", runTask->taskName);
    PRINTK("runTask->taskID = %u\n", runTask->taskID);
    BackTrace(regFP);
}

#ifdef LOSCFG_GDB
VOID OsUndefIncExcHandleEntry(ExcContext *excBufAddr)
{
    excBufAddr->PC -= 4;  /* lr in undef is pc + 4 */

    if (gdb_undef_hook(excBufAddr, OS_EXCEPT_UNDEF_INSTR)) {
        return;
    }

    if (g_excHook != NULL) {
        g_excHook(OS_EXCEPT_UNDEF_INSTR, excBufAddr);
    }
    while (1) {
        ;
    }
}

#if __LINUX_ARM_ARCH__ >= 7
VOID OsPrefetchAbortExcHandleEntry(ExcContext *excBufAddr)
{
    excBufAddr->PC -= 4;  /* lr in prefetch abort is pc + 4 */

    if (gdbhw_hook(excBufAddr, OS_EXCEPT_PREFETCH_ABORT)) {
        return;
    }

    if (g_excHook != NULL) {
        g_excHook(OS_EXCEPT_PREFETCH_ABORT, excBufAddr);
    }
    while (1) {
        ;
    }
}

VOID OsDataAbortExcHandleEntry(ExcContext *excBufAddr)
{
    excBufAddr->PC -= 8;  /* lr in data abort is pc + 8 */

    if (gdbhw_hook(excBufAddr, OS_EXCEPT_DATA_ABORT)) {
        return;
    }

    if (g_excHook != NULL) {
        g_excHook(OS_EXCEPT_DATA_ABORT, excBufAddr);
    }
    while (1) {
        ;
    }
}
#endif /* __LINUX_ARM_ARCH__ */
#endif /* LOSCFG_GDB */

/*
 * Description : EXC handler entry
 * Input       : excType    --- exc type
 *               excBufAddr --- address of EXC buf
 */
LITE_OS_SEC_TEXT_INIT VOID OsExcHandleEntry(UINT32 excType, ExcContext *excBufAddr)
{
#ifdef LOSCFG_SHELL_EXCINFO
    log_read_write_fn func = GetExcInfoRW();
#endif

    g_curNestCount++;

    if (g_excHook != NULL) {
        if (g_curNestCount == 1) {
#ifdef LOSCFG_SHELL_EXCINFO
            if (func != NULL) {
                SetExcInfoIndex(0);
                g_intCount[ArchCurrCpuid()] = 0;
                OsRecordExcInfoTime();
                g_intCount[ArchCurrCpuid()] = 1;
            }
#endif
            g_excHook(excType, excBufAddr);
        } else {
            OsCallStackInfo();
        }

#ifdef LOSCFG_SHELL_EXCINFO
        if (func != NULL) {
            PrintExcInfo("Be sure flash space bigger than GetExcInfoIndex():0x%x\n", GetExcInfoIndex());
            g_intCount[ArchCurrCpuid()] = 0;
            func(GetRecordAddr(), GetRecordSpace(), 0, GetExcInfoBuf());
            g_intCount[ArchCurrCpuid()] = 1;
        }
#endif
    }
#ifdef LOSCFG_EXC_INTERACTION
    OsExcInteractionTaskKeep();
#endif

    while (1) {
        ;
    }
}

__attribute__((noinline)) VOID LOS_Panic(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    UartVprintf(fmt, ap);
    va_end(ap);
    __asm__ __volatile__("swi 0");
}

/* stack protector */
UINT32 __stack_chk_guard = 0xd00a0dff;

VOID __stack_chk_fail(VOID)
{
    /* __builtin_return_address is a builtin function, building in gcc */
    LOS_Panic("stack-protector: Kernel stack is corrupted in: %p\n",
              __builtin_return_address(0));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
