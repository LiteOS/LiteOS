/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch32 Hw Inner HeadFile
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

#ifndef _LOS_HW_PRI_H
#define _LOS_HW_PRI_H

#include "los_base.h"
#include "los_hw.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(LOSCFG_ARCH_FPU_VFP_D16)
#define FP_REGS_NUM     16
#elif defined (LOSCFG_ARCH_FPU_VFP_D32)
#define FP_REGS_NUM     32
#endif
#define GEN_REGS_NUM    13

/* The size of this structure must be smaller than or equal to the size specified by OS_TSK_STACK_ALIGN (16 bytes). */
typedef struct {
#if !defined(LOSCFG_ARCH_FPU_DISABLE)
    UINT64 D[FP_REGS_NUM]; /* D0-D31 */
    UINT32 regFPSCR;       /* FPSCR */
    UINT32 regFPEXC;       /* FPEXC */
#endif
    UINT32 R[GEN_REGS_NUM]; /* R0-R12 */
    UINT32 LR;              /* R14 */
    UINT32 PC;              /* R15 */
    UINT32 regPSR;
} TaskContext;

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

/*
 * Description : task stack initialization
 * Input       : taskID    -- task ID
 *               stackSize -- task stack size
 *               topStack  -- stack top of task (low address)
 * Return      : pointer to the task context
 */
extern VOID *OsTaskStackInit(UINT32 taskID, UINT32 stackSize, VOID *topStack);
extern void arm_clean_cache_range(UINTPTR start, UINTPTR end);
extern void arm_inv_cache_range(UINTPTR start, UINTPTR end);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_HW_PRI_H */
