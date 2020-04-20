/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Aarch32 Hw CPU HeadFile
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
 * @ingroup kernel
 */

#ifndef __LOS_HW_CPU_H__
#define __LOS_HW_CPU_H__

#include "los_typedef.h"
#include "los_toolchain.h"
#include "los_hw_arch.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* ARM System Registers */
#define DSB     __asm__ volatile("dsb" ::: "memory")
#define DMB     __asm__ volatile("dmb" ::: "memory")
#define ISB     __asm__ volatile("isb" ::: "memory")
#define BARRIER __asm__ volatile("":::"memory")

#define ARM_SYSREG_READ(REG)                    \
({                                              \
    UINT32 _val;                                \
    __asm__ volatile("mrc " REG : "=r" (_val)); \
    _val;                                       \
})

#define ARM_SYSREG_WRITE(REG, val)              \
({                                              \
    __asm__ volatile("mcr " REG :: "r" (val));  \
    ISB;                                        \
})

#define ARM_SYSREG64_READ(REG)                   \
({                                               \
    UINT64 _val;                                 \
    __asm__ volatile("mrrc " REG : "=r" (_val)); \
    _val;                                        \
})

#define ARM_SYSREG64_WRITE(REG, val)             \
({                                               \
    __asm__ volatile("mcrr " REG :: "r" (val));  \
    ISB;                                         \
})

#define CP14_REG(CRn, Op1, CRm, Op2)    "p14, "#Op1", %0, "#CRn","#CRm","#Op2
#define CP15_REG(CRn, Op1, CRm, Op2)    "p15, "#Op1", %0, "#CRn","#CRm","#Op2
#define CP15_REG64(CRn, Op1)            "p15, "#Op1", %0,    %H0,"#CRn

/*
 * Identification registers (c0)
 */
#define MIDR                CP15_REG(c0, 0, c0, 0)    /* Main ID Register */
#define MPIDR               CP15_REG(c0, 0, c0, 5)    /* Multiprocessor Affinity Register */
#define CCSIDR              CP15_REG(c0, 1, c0, 0)    /* Cache Size ID Registers */
#define CLIDR               CP15_REG(c0, 1, c0, 1)    /* Cache Level ID Register */
#define VPIDR               CP15_REG(c0, 4, c0, 0)    /* Virtualization Processor ID Register */
#define VMPIDR              CP15_REG(c0, 4, c0, 5)    /* Virtualization Multiprocessor ID Register */

/*
 * System control registers (c1)
 */
#define SCTLR               CP15_REG(c1, 0, c0, 0)    /* System Control Register */
#define ACTLR               CP15_REG(c1, 0, c0, 1)    /* Auxiliary Control Register */
#define CPACR               CP15_REG(c1, 0, c0, 2)    /* Coprocessor Access Control Register */

/*
 * Memory protection and control registers (c2 & c3)
 */
#define TTBR0               CP15_REG(c2, 0, c0, 0)    /* Translation Table Base Register 0 */
#define TTBR1               CP15_REG(c2, 0, c0, 1)    /* Translation Table Base Register 1 */
#define TTBCR               CP15_REG(c2, 0, c0, 2)    /* Translation Table Base Control Register */
#define DACR                CP15_REG(c3, 0, c0, 0)    /* Domain Access Control Register */

/*
 * Memory system fault registers (c5 & c6)
 */
#define DFSR                CP15_REG(c5, 0, c0, 0)    /* Data Fault Status Register */
#define IFSR                CP15_REG(c5, 0, c0, 1)    /* Instruction Fault Status Register */
#define DFAR                CP15_REG(c6, 0, c0, 0)    /* Data Fault Address Register */
#define IFAR                CP15_REG(c6, 0, c0, 2)    /* Instruction Fault Address Register */

/*
 * Process, context and thread ID registers (c13)
 */
#define FCSEIDR             CP15_REG(c13, 0, c0, 0)    /* FCSE Process ID Register */
#define CONTEXTIDR          CP15_REG(c13, 0, c0, 1)    /* Context ID Register */
#define TPIDRURW            CP15_REG(c13, 0, c0, 2)    /* User Read/Write Thread ID Register */
#define TPIDRURO            CP15_REG(c13, 0, c0, 3)    /* User Read-Only Thread ID Register */
#define TPIDRPRW            CP15_REG(c13, 0, c0, 4)    /* PL1 only Thread ID Register */

#define MPIDR_CPUID_MASK    (0xffU)

STATIC INLINE VOID *ArchCurrTaskGet(VOID)
{
    return (VOID *)ARM_SYSREG_READ(TPIDRPRW);
}

STATIC INLINE VOID ArchCurrTaskSet(VOID *val)
{
    ARM_SYSREG_WRITE(TPIDRPRW, (UINT32)val);
}

STATIC INLINE UINT32 ArchCurrCpuid(VOID)
{
    return 0;
}

STATIC INLINE UINT64 OsHwIDGet(VOID)
{
    return ARM_SYSREG_READ(MPIDR);
}

STATIC INLINE UINT32 OsMainIDGet(VOID)
{
    return ARM_SYSREG_READ(MIDR);
}

/* CPU interrupt mask handle implementation */
#if LOSCFG_ARM_ARCH >= 6

STATIC INLINE UINT32 ArchIntLock(VOID)
{
    UINT32 intSave;
    __asm__ __volatile__(
        "mrs    %0, cpsr      \n"
        "cpsid  if              "
        : "=r"(intSave)
        :
        : "memory");
    return intSave;
}

STATIC INLINE UINT32 ArchIntUnlock(VOID)
{
    UINT32 intSave;
    __asm__ __volatile__(
        "mrs    %0, cpsr      \n"
        "cpsie  if              "
        : "=r"(intSave)
        :
        : "memory");
    return intSave;
}

#else

STATIC INLINE UINT32 ArchIntLock(VOID)
{
    UINT32 intSave, temp;
    __asm__ __volatile__(
        "mrs    %0, cpsr      \n"
        "orr    %1, %0, #0xc0 \n"
        "msr    cpsr_c, %1      "
        :"=r"(intSave),  "=r"(temp)
        : :"memory");
    return intSave;
}

STATIC INLINE UINT32 ArchIntUnlock(VOID)
{
    UINT32 intSave;
    __asm__ __volatile__(
        "mrs    %0, cpsr      \n"
        "bic    %0, %0, #0xc0 \n"
        "msr    cpsr_c, %0      "
        : "=r"(intSave)
        : : "memory");
    return intSave;
}

#endif

STATIC INLINE VOID ArchIntRestore(UINT32 intSave)
{
    __asm__ __volatile__(
        "msr    cpsr_c, %0      "
        :
        : "r"(intSave)
        : "memory");
}

#define PSR_I_BIT   0x00000080U

STATIC INLINE UINT32 OsIntLocked(VOID)
{
    UINT32 intSave;

    asm volatile(
        "mrs    %0, cpsr        "
        : "=r" (intSave)
        :
        : "memory", "cc");

    return intSave & PSR_I_BIT;
}

STATIC INLINE UINT32 ArchSPGet(VOID)
{
    UINT32 val;
    __asm__ __volatile__("mov %0, sp" : "=r"(val));
    return val;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LOS_HW_CPU_H__ */
