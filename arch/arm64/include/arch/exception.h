/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Aarch64 Exception HeadFile
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

#include "menuconfig.h"
#ifndef __ASSEMBLER__
#include "los_typedef.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_SMP
#define CORE_NUM                LOSCFG_KERNEL_SMP_CORE_NUM
#else
#define CORE_NUM                1
#endif

/* Initial bit64 stack value. */
#define OS_STACK_INIT           0xCACACACACACACACA
/* Bit64 stack top magic number. */
#define OS_STACK_MAGIC_WORD     0xCCCCCCCCCCCCCCCC

#define OS_EXC_START_STACK_SIZE 0x1000
#ifdef LOSCFG_ARCH_INTERRUPT_PREEMPTION
#define OS_EXC_IRQ_STACK_SIZE   0x6000
#else /* !LOSCFG_ARCH_INTERRUPT_PREEMPTION */
#define OS_EXC_IRQ_STACK_SIZE   0x1000
#endif /* LOSCFG_ARCH_INTERRUPT_PREEMPTION */

#ifndef __ASSEMBLER__

#define EXC_GEN_REGS_NUM     30
typedef struct {
    UINT64 X[EXC_GEN_REGS_NUM]; /**< Register X0-X29 */
    UINT64 LR;                  /**< Program returning address. X30 */
    UINT64 SP;
    UINT64 regELR;
    UINT64 SPSR;
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
    __asm__ __volatile__("mov %0, X29" : "=r"(_regFp)); \
    _regFp; \
})

typedef VOID (*EXC_PROC_FUNC)(UINT32, ExcContext *);

UINT32 ArchSetExcHook(EXC_PROC_FUNC excHook);
#define LOS_ExcRegHook ArchSetExcHook

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("svc #0");
}

VOID ArchBackTraceWithSp(const VOID *stackPointer);
VOID ArchBackTrace(VOID);
UINT32 ArchBackTraceGet(UINTPTR fp, UINTPTR *callChain, UINT32 maxDepth);
VOID ArchExcInit(VOID);

/* Stack pointers for different modes. */
extern UINTPTR __stack_startup;
extern UINTPTR __stack_startup_top;
extern UINTPTR __irq_stack_top;
extern UINTPTR __irq_stack;

#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */