/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: cortex-m Hw CPU HeadFile
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

/**
 * @defgroup los_hw Hardware
 *  @ingroup kernel
 */

#ifndef __LOS_HW_CPU_H__
#define __LOS_HW_CPU_H__

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CPUID_BASE              0xE000ED00    /* Main ID Register */
#define ARM_SYSREG_READ(addr)   (*(volatile unsigned *)(uintptr_t)(addr))

extern VOID *g_runTask;
extern VOID *g_oldTask;
STATIC INLINE VOID *ArchCurrTaskGet(VOID)
{
    return g_runTask;
}

STATIC INLINE VOID ArchCurrTaskSet(VOID* val)
{
    g_runTask = val;
}

STATIC INLINE UINT32 ArchCurrCpuid(void)
{
    return 0;
}


STATIC INLINE UINT32 OsMainIDGet(VOID)
{
    return ARM_SYSREG_READ(CPUID_BASE);
}

STATIC INLINE UINT32 ArchSPGet(VOID)
{
    UINT32 val;
    asm volatile("mov %0, sp" : "=r"(val));
    return val;
}

STATIC INLINE UINT32 OsIntLocked(VOID)
{
    UINT32 intSave;

    asm volatile(
        "mrs    %0, primask        "
        : "=r" (intSave)
        :
        : "memory");

    return intSave;
}

extern UINT32 OsIntUnLock(VOID);
extern UINT32 OsIntLock(VOID);
extern VOID OsIntRestore(UINT32 uvIntSave);

#define ArchIntLock()             OsIntLock()
#define ArchIntUnlock()           OsIntUnLock()
#define ArchIntRestore(intSave)   OsIntRestore(intSave)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LOS_HW_CPU_H__ */
