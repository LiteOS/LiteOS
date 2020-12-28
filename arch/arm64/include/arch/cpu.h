/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: CPU Operations HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2018-08-21
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

#ifndef _ARCH_CPU_H
#define _ARCH_CPU_H

#include "los_typedef.h"
#include "arch/regs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    const UINT32 partNo;
    const CHAR *cpuName;
} CpuVendor;

extern UINT64 g_cpuMap[];

#define CPU_MAP_GET(cpuid)          g_cpuMap[(cpuid)]
#define CPU_MAP_SET(cpuid, hwid)    (g_cpuMap[(cpuid)] = (hwid))

extern const CHAR *ArchCpuInfo(VOID);

STATIC INLINE UINT32 ArchCurrCpuid(VOID)
{
#if (LOSCFG_KERNEL_SMP == YES)
    UINT32 cpuid;
    UINT64 mpidr = AARCH64_SYSREG_READ(mpidr_el1);
    if (!(mpidr & MPIDR_MT_MASK)) {
        /* single-thread type */
        cpuid = MPIDR_AFF_LEVEL(mpidr, 0);
    } else {
        /* muti-thread type, aff1 is cpuid */
        cpuid = MPIDR_AFF_LEVEL(mpidr, 1);
    }
    return cpuid;
#else
    return 0;
#endif
}

STATIC INLINE UINT32 ArchSPGet(VOID)
{
    UINT32 val;
    __asm__ __volatile__("mov %0, sp" : "=r"(val));
    return val;
}

STATIC INLINE UINT64 OsHwIDGet(VOID)
{
    return AARCH64_SYSREG_READ(mpidr_el1);
}

STATIC INLINE UINT32 OsMainIDGet(VOID)
{
    return (UINT32)AARCH64_SYSREG_READ(midr_el1);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_CPU_H */
