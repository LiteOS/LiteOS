/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Aarch32 Exception HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-10-10
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

#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#include "arch/regs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Define exception type ID */
#define OS_EXCEPT_RESET          0x00
#define OS_EXCEPT_UNDEF_INSTR    0x01
#define OS_EXCEPT_SWI            0x02
#define OS_EXCEPT_PREFETCH_ABORT 0x03
#define OS_EXCEPT_DATA_ABORT     0x04
#define OS_EXCEPT_FIQ            0x05
#define OS_EXCEPT_ADDR_ABORT     0x06
#define OS_EXCEPT_IRQ            0x07

/* Define core num */
#ifdef LOSCFG_KERNEL_SMP
#define CORE_NUM                 LOSCFG_KERNEL_SMP_CORE_NUM
#else
#define CORE_NUM                 1
#endif

/* Initial bit32 stack value. */
#define OS_STACK_INIT            0xCACACACA
/* Bit32 stack top magic number. */
#define OS_STACK_MAGIC_WORD      0xCCCCCCCC

#ifdef LOSCFG_GDB
#define OS_EXC_UNDEF_STACK_SIZE  512
#define OS_EXC_ABT_STACK_SIZE    512
#else
#define OS_EXC_UNDEF_STACK_SIZE  40
#define OS_EXC_ABT_STACK_SIZE    40
#endif
#define OS_EXC_FIQ_STACK_SIZE    64
#define OS_EXC_IRQ_STACK_SIZE    64
#define OS_EXC_SVC_STACK_SIZE    0x2000
#define OS_EXC_STACK_SIZE        0x1000

#ifndef __ASSEMBLER__

extern UINTPTR __fiq_stack_top;
extern UINTPTR __svc_stack_top;
extern UINTPTR __abt_stack_top;
extern UINTPTR __undef_stack_top;
extern UINTPTR __exc_stack_top;
extern UINTPTR __irq_stack_top;
extern UINTPTR __fiq_stack;
extern UINTPTR __svc_stack;
extern UINTPTR __abt_stack;
extern UINTPTR __undef_stack;
extern UINTPTR __exc_stack;
extern UINTPTR __irq_stack;

typedef struct {
    UINT32 regCPSR; /**< Current program status register (CPSR) */
    UINT32 R0;      /**< Register R0 */
    UINT32 R1;      /**< Register R1 */
    UINT32 R2;      /**< Register R2 */
    UINT32 R3;      /**< Register R3 */
    UINT32 R4;      /**< Register R4 */
    UINT32 R5;      /**< Register R5 */
    UINT32 R6;      /**< Register R6 */
    UINT32 R7;      /**< Register R7 */
    UINT32 R8;      /**< Register R8 */
    UINT32 R9;      /**< Register R9 */
    UINT32 R10;     /**< Register R10 */
    UINT32 R11;     /**< Register R11 */
    UINT32 R12;     /**< Register R12 */
    UINT32 SP;      /**< Stack pointer */
    UINT32 LR;      /**< Program returning address. */
    UINT32 PC;      /**< PC pointer of the exceptional function */
} ExcContext;

typedef struct {
    UINT16 phase;        /**< Phase in which an exception occurs */
    UINT16 type;         /**< Exception type */
    UINT16 nestCnt;      /**< Count of nested exception */
    UINT16 reserved;     /**< Reserved for alignment */
    ExcContext *context; /**< Hardware context when an exception occurs */
} ExcInfo;

#define ArchGetFp() ({ \
    UINTPTR _regFp; \
    __asm__ __volatile__("mov %0, fp" : "=r"(_regFp)); \
    _regFp; \
})

typedef VOID (*EXC_PROC_FUNC)(UINT32, ExcContext*);

UINT32 ArchSetExcHook(EXC_PROC_FUNC excHook);
#define LOS_ExcRegHook ArchSetExcHook

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("swi 0");
}

VOID ArchBackTraceWithSp(const VOID *stackPointer);
VOID ArchBackTrace(VOID);
VOID ArchExcInit(VOID);
UINT32 ArchBackTraceGet(UINTPTR fp, UINTPTR *callChain, UINT32 maxDepth);

STATIC INLINE UINT32 OsGetDFSR(VOID)
{
    UINT32 regDFSR;
    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 0"
                         : "=r"(regDFSR));
    return regDFSR;
}

STATIC INLINE UINT32 OsGetIFSR(VOID)
{
    UINT32 regIFSR;
    __asm__ __volatile__("mrc p15, 0, %0, c5, c0, 1"
                         : "=r"(regIFSR));
    return regIFSR;
}

STATIC INLINE UINT32 OsGetDFAR(VOID)
{
    UINT32 regDFAR;
    __asm__ __volatile__("mrc p15, 0, %0, c6, c0, 0"
                         : "=r"(regDFAR));
    return regDFAR;
}

STATIC INLINE UINT32 OsGetIFAR(VOID)
{
    UINT32 regIFAR;
    __asm__ __volatile__("mrc p15, 0, %0, c6, c0, 2"
                         : "=r"(regIFAR));
    return regIFAR;
}

#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */