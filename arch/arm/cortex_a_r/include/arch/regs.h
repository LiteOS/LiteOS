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

/**
 * @defgroup los_hw Hardware
 * @ingroup kernel
 */

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

#ifndef __ASSEMBLER__

#define ARM_SYSREG_READ(REG)                    \
({                                              \
    UINT32 _val;                                \
    __asm__ volatile("mrc " REG : "=r" (_val)); \
    _val;                                       \
})

#define ARM_SYSREG_WRITE(REG, val)              \
({                                              \
    __asm__ volatile("mcr " REG :: "r" (val));  \
    ISB();                                        \
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
    ISB();                                         \
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
 * Cache maintenance, address translation, and other functions
 */
#define ICIALLUIS           CP15_REG(c7, 0, c1, 0)      /* Instruction Cache Invalidate All to PoU,
                                                           Inner Shareable */
#define BPIALLIS            CP15_REG(c7, 0, c1, 6)      /* Branch Predictor Invalidate All,
                                                           Inner Shareable */
#define ICIALLU             CP15_REG(c7, 0, c5, 0)      /* Instruction Cache Invalidate All to PoU */
#define ICIMVAU             CP15_REG(c7, 0, c5, 1)      /* Instruction Cache Invalidate by MVA to PoU */
#define BPIALL              CP15_REG(c7, 0, c5, 6)      /* Branch Predictor Invalidate All */
#define BPIMVA              CP15_REG(c7, 0, c5, 7)      /* Branch Predictor Invalidate by MVA */
#define DCIMVAC             CP15_REG(c7, 0, c6, 1)      /* Data Cache Invalidate by MVA to PoC */
#define DCISW               CP15_REG(c7, 0, c6, 2)      /* Data Cache Invalidate by Set/Way */
#define DCCMVAC             CP15_REG(c7, 0, c10, 1)     /* Data Cache Clean by MVA to PoC */
#define DCCSW               CP15_REG(c7, 0, c10, 2)     /* Data Cache Clean by Set/Way */
#define DCCMVAU             CP15_REG(c7, 0, c11, 1)     /* Data Cache Clean by MVA to PoU */
#define DCCIMVAC            CP15_REG(c7, 0, c14, 1)     /* Data Cache Clean and Invalidate by MVA to PoC */
#define DCCISW              CP15_REG(c7, 0, c14, 2)     /* Data Cache Clean and Invalidate by Set/Way */

/*
 * Process, context and thread ID registers (c13)
 */
#define FCSEIDR             CP15_REG(c13, 0, c0, 0)    /* FCSE Process ID Register */
#define CONTEXTIDR          CP15_REG(c13, 0, c0, 1)    /* Context ID Register */
#define TPIDRURW            CP15_REG(c13, 0, c0, 2)    /* User Read/Write Thread ID Register */
#define TPIDRURO            CP15_REG(c13, 0, c0, 3)    /* User Read-Only Thread ID Register */
#define TPIDRPRW            CP15_REG(c13, 0, c0, 4)    /* PL1 only Thread ID Register */

#endif /* __ASSEMBLER__ */

#define PSR_F_BIT               0x00000040u
#define PSR_I_BIT               0x00000080u
#define PSR_A_BIT               0x00000100u

#define PSR_T_ARM               0x00000000u
#define PSR_T_THUMB             0x00000020u

#define CPSR_USR_MODE           0x00000010u
#define CPSR_FIQ_MODE           0x00000011u
#define CPSR_IRQ_MODE           0x00000012u
#define CPSR_SVC_MODE           0x00000013u
#define CPSR_ABT_MODE           0x00000017u
#define CPSR_UNDEF_MODE         0x0000001Bu

#define CPSR_IRQ_DISABLE        PSR_I_BIT   /* IRQ disabled when =1 */
#define CPSR_FIQ_DISABLE        PSR_F_BIT   /* FIQ disabled when =1 */
#define CPSR_INT_DISABLE        (CPSR_IRQ_DISABLE | CPSR_FIQ_DISABLE)

#define PSR_MODE_SVC_ARM        (CPSR_SVC_MODE | PSR_T_ARM   | CPSR_INT_DISABLE)
#define PSR_MODE_SVC_THUMB      (CPSR_SVC_MODE | PSR_T_THUMB | CPSR_INT_DISABLE)

/* MPIDR field defines */
#define MPIDR_CPUID_MASK        0x000000FFu

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_REGS_H */
