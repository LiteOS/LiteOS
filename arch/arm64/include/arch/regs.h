/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: CPU Register Defines Headfile
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

#ifndef _ARCH_REGS_H
#define _ARCH_REGS_H

#ifndef __ASSEMBLER__
#include "arch/barrier.h"
#include "los_typedef.h"
#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
#define RUNLVL     12
#define SPSR_ELx   spsr_el3
#define ELR_ELx    elr_el3
#define TPIDR_ELx  tpidr_el3
#define ESR_ELx    esr_el3
#define FAR_ELx    far_el3
#else
#define RUNLVL     4
#define SPSR_ELx   spsr_el1
#define ELR_ELx    elr_el1
#define TPIDR_ELx  tpidr_el1
#define ESR_ELx    esr_el1
#define FAR_ELx    far_el1
#endif

#ifndef __ASSEMBLER__

/* AARCH64 System Registers */
#define STRINGIFY_1(x) #x
#define STRINGIFY(x) STRINGIFY_1(x)

#define AARCH64_SYSREG_READ(reg)                                 \
    ({                                                           \
        UINT64 _val;                                             \
        __asm__ volatile("mrs %0," STRINGIFY(reg) : "=r"(_val)); \
        _val;                                                    \
    })

#define AARCH64_SYSREG_WRITE(reg, val)                             \
    ({                                                             \
        __asm__ volatile("msr " STRINGIFY(reg) ", %0" ::"r"(val)); \
        __asm__ volatile("isb" ::: "memory");                      \
    })

/*
 *  mpidr register (64bit)
 *       res0  aff3  res1  u  res0  mt  aff2  aff1  aff0
 *  bit 63-40 39-32   31  30 29~25  24 23-16  15-8   7~0
 */
#define MPIDR_U_MASK        (0x1ULL << 30)
#define MPIDR_MT_MASK       (0x1ULL << 24)

#define MPIDR_AFF_LEVEL_MASK 0xFFULL
#define MPIDR_AFF_SHIFT(level) (((1 << (level)) >> 1) << 3)
#define MPIDR_AFF_LEVEL(mpidr, level) (((mpidr) >> MPIDR_AFF_SHIFT(level)) & MPIDR_AFF_LEVEL_MASK)

/*
 *  midr register (32bit)
 *       implementer variant  arch partnum  rev
 *  bit     31-24    23-20   19~16  15-4    3-0
 */
#define MIDR_REV_MASK       0xFF
#define MIDR_REV(midr)      ((midr)&MIDR_REV_MASK)
#define MIDR_PARTNO_SHIFT   0x4
#define MIDR_PARTNO_MASK    (0xFFF << MIDR_PARTNO_SHIFT)
#define MIDR_PARTNO(midr)   (((midr)&MIDR_PARTNO_MASK) >> MIDR_PARTNO_SHIFT)

#define PSR_T_ARM           0x00000000u
#define PSR_T_THUMB         0x00000020u
#define PSR_MODE_SVC        0x00000013u
#define PSR_MODE_SYS        0x0000001Fu
#define PSR_FIQ_DIS         0x00000040u
#define PSR_IRQ_DIS         0x00000080u

#define PSR_MODE_SVC_THUMB  (PSR_MODE_SVC | PSR_T_THUMB | PSR_FIQ_DIS | PSR_IRQ_DIS)
#define PSR_MODE_SVC_ARM    (PSR_MODE_SVC | PSR_T_ARM | PSR_FIQ_DIS | PSR_IRQ_DIS)

#define PSR_MODE_SYS_THUMB  (PSR_MODE_SYS | PSR_T_THUMB)
#define PSR_MODE_SYS_ARM    (PSR_MODE_SYS | PSR_T_ARM)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_REGS_H */
