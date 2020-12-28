/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Task Low Level Impelmentations Headfile
 * Author: Huawei LiteOS Team
 * Create: 2020-01-14
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

#ifndef _ARCH_TASK_H
#define _ARCH_TASK_H

#include "los_typedef.h"
#include "arch/cpu.h"
#include "arch/regs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOSCFG_STACK_POINT_ALIGN_SIZE (sizeof(UINTPTR) * 2)

#if defined(LOSCFG_ARCH_FPU_VFP_D16)
#define FP_REGS_NUM     16
#elif defined (LOSCFG_ARCH_FPU_VFP_D32)
#define FP_REGS_NUM     32
#endif
#define GEN_REGS_NUM    13

/* The size of this structure must be smaller than or equal to the size specified by OS_TSK_STACK_ALIGN (16 bytes). */
typedef struct {
#if !defined(LOSCFG_ARCH_FPU_DISABLE)
    UINT64 D[FP_REGS_NUM];  /* D0-D31 */
    UINT32 regFPSCR;        /* FPSCR */
    UINT32 regFPEXC;        /* FPEXC */
#endif
    UINT32 R[GEN_REGS_NUM]; /* R0-R12 */
    UINT32 LR;              /* R14 */
    UINT32 PC;              /* R15 */
    UINT32 regPSR;
} TaskContext;

STATIC INLINE VOID *ArchCurrTaskGet(VOID)
{
    return (VOID *)(UINTPTR)ARM_SYSREG_READ(TPIDRPRW);
}

STATIC INLINE VOID ArchCurrTaskSet(VOID *val)
{
    ARM_SYSREG_WRITE(TPIDRPRW, (UINT32)(UINTPTR)val);
}

STATIC INLINE UINTPTR ArchGetTaskFp(const VOID *stackPointer)
{
    return ((TaskContext *)(stackPointer))->R[11]; /* R11: FP */
}

/*
 * Description : task stack initialization
 * Input       : taskId    -- task ID
 *               stackSize -- task stack size
 *               topStack  -- stack top of task (low address)
 * Return      : pointer to the task context
 */
extern VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_TASK_H */