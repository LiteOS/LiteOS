/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: General interrupt controller version 3.0 (GICv3).
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

#ifndef _GIC_V3_H_
#define _GIC_V3_H_

#include "stdint.h"
#include "asm/platform.h"
#include "los_hw_cpu.h"

#define BIT_32(bit) (1u << bit)
#define BIT_64(bit) (1ul << bit)

#define ICC_CTLR_EL1    "S3_0_C12_C12_4"
#define ICC_PMR_EL1     "S3_0_C4_C6_0"
#define ICC_IAR1_EL1    "S3_0_C12_C12_0"
#define ICC_SRE_EL1     "S3_0_C12_C12_5"
#define ICC_BPR0_EL1    "S3_0_C12_C8_3"
#define ICC_BPR1_EL1    "S3_0_C12_C12_3"
#define ICC_IGRPEN0_EL1 "S3_0_C12_C12_6"
#define ICC_IGRPEN1_EL1 "S3_0_C12_C12_7"
#define ICC_EOIR1_EL1   "S3_0_C12_C12_1"
#define ICC_SGI1R_EL1   "S3_0_C12_C11_5"
#define ICC_EOIR0_EL1   "S3_0_c12_c8_1"
#define ICC_IAR0_EL1    "S3_0_C12_C8_0"

#define ICC_CTLR_EL3    "S3_6_C12_C12_4"
#define ICC_SRE_EL3     "S3_6_C12_C12_5"
#define ICC_IGRPEN1_EL3 "S3_6_C12_C12_7"

/* GICD_CTLR bit definitions */
#define CTLR_ENALBE_G0 BIT_32(0)
#define CTLR_ENABLE_G1NS BIT_32(1)
#define CTLR_ENABLE_G1S BIT_32(2)
#define CTLR_RES0 BIT_32(3)
#define CTLR_ARE_S BIT_32(4)
#define CTLR_ARE_NS BIT_32(5)
#define CTLR_DS BIT_32(6)
#define CTLR_E1NWF BIT_32(7)
#define GICD_CTLR_RWP BIT_32(31)

/* peripheral identification registers */
#define GICD_CIDR0 (GICD_OFFSET + 0xfff0)
#define GICD_CIDR1 (GICD_OFFSET + 0xfff4)
#define GICD_CIDR2 (GICD_OFFSET + 0xfff8)
#define GICD_CIDR3 (GICD_OFFSET + 0xfffc)
#define GICD_PIDR0 (GICD_OFFSET + 0xffe0)
#define GICD_PIDR1 (GICD_OFFSET + 0xffe4)
#define GICD_PIDR2 (GICD_OFFSET + 0xffe8)
#define GICD_PIDR3 (GICD_OFFSET + 0xffec)

/* GICD_PIDR bit definitions and masks */
#define GICD_PIDR2_ARCHREV_SHIFT 4
#define GICD_PIDR2_ARCHREV_MASK 0xf

/* redistributor registers */
#define GICR_SGI_OFFSET (GICR_OFFSET + 0x10000)

#define GICR_CTLR(i)        (GICR_OFFSET + GICR_STRIDE * (i) + 0x0000)
#define GICR_IIDR(i)        (GICR_OFFSET + GICR_STRIDE * (i) + 0x0004)
#define GICR_TYPER(i, n)    (GICR_OFFSET + GICR_STRIDE * (i) + 0x0008 + (n)*4)
#define GICR_STATUSR(i)     (GICR_OFFSET + GICR_STRIDE * (i) + 0x0010)
#define GICR_WAKER(i)       (GICR_OFFSET + GICR_STRIDE * (i) + 0x0014)
#define GICR_IGROUPR0(i)    (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0080)
#define GICR_IGRPMOD0(i)    (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0d00)
#define GICR_ISENABLER0(i)  (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0100)
#define GICR_ICENABLER0(i)  (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0180)
#define GICR_ISPENDR0(i)    (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0200)
#define GICR_ICPENDR0(i)    (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0280)
#define GICR_ISACTIVER0(i)  (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0300)
#define GICR_ICACTIVER0(i)  (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0380)
#define GICR_IPRIORITYR0(i) (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0400)
#define GICR_ICFGR0(i)      (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0c00)
#define GICR_ICFGR1(i)      (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0c04)
#define GICR_NSACR(i)       (GICR_SGI_OFFSET + GICR_STRIDE * (i) + 0x0e00)

#define GICR_WAKER_PROCESSORSLEEP_LEN           1U
#define GICR_WAKER_PROCESSORSLEEP_OFFSET        1
#define GICR_WAKER_CHILDRENASLEEP_LEN           1U
#define GICR_WAKER_CHILDRENASLEEP_OFFSET        2
#define GICR_WAKER_PROCESSORSLEEP               (GICR_WAKER_PROCESSORSLEEP_LEN << GICR_WAKER_PROCESSORSLEEP_OFFSET)
#define GICR_WAKER_CHILDRENASLEEP               (GICR_WAKER_CHILDRENASLEEP_LEN << GICR_WAKER_CHILDRENASLEEP_OFFSET)

STATIC INLINE VOID GiccSetCtlr(UINT32 val)
{
#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("msr " ICC_CTLR_EL3 ", %0" ::"r"(val));
#else
    __asm__ volatile("msr " ICC_CTLR_EL1 ", %0" ::"r"(val));
#endif
    ISB;
}

STATIC INLINE VOID GiccSetPmr(UINT32 val)
{
    __asm__ volatile("msr " ICC_PMR_EL1 ", %0" ::"r"(val));
    ISB;
    DSB;
}

STATIC INLINE VOID GiccSetIgrpen0(UINT32 val)
{
    __asm__ volatile("msr " ICC_IGRPEN0_EL1 ", %0" ::"r"(val));
    ISB;
}

STATIC INLINE VOID GiccSetIgrpen1(UINT32 val)
{
#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("msr " ICC_IGRPEN1_EL3 ", %0" ::"r"(val));
#else
    __asm__ volatile("msr " ICC_IGRPEN1_EL1 ", %0" ::"r"(val));
#endif
    ISB;
}

STATIC INLINE UINT32 GiccGetSre(VOID)
{
    UINT32 temp;
#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("mrs %0, " ICC_SRE_EL3 : "=r"(temp));
#else
    __asm__ volatile("mrs %0, " ICC_SRE_EL1 : "=r"(temp));
#endif
    return temp;
}

STATIC INLINE VOID GiccSetSre(UINT32 val)
{
#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("msr " ICC_SRE_EL3 ", %0" ::"r"(val));
#else
    __asm__ volatile("msr " ICC_SRE_EL1 ", %0" ::"r"(val));
#endif
    ISB;
}

STATIC INLINE VOID GiccSetEoir(UINT32 val)
{
#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("msr " ICC_EOIR0_EL1 ", %0" ::"r"(val));
#else
    __asm__ volatile("msr " ICC_EOIR1_EL1 ", %0" ::"r"(val));
#endif
    ISB;
}

STATIC INLINE UINT32 GiccGetIar(VOID)
{
    UINT32 temp;

#ifdef LOSCFG_ARCH_SECURE_MONITOR_MODE
    __asm__ volatile("mrs %0, " ICC_IAR0_EL1 : "=r"(temp));
#else
    __asm__ volatile("mrs %0, " ICC_IAR1_EL1 : "=r"(temp));
#endif
    DSB;

    return temp;
}

STATIC INLINE VOID GiccSetSgi1r(UINT64 val)
{
    __asm__ volatile("msr " ICC_SGI1R_EL1 ", %0" ::"r"(val));
    ISB;
    DSB;
}

STATIC INLINE VOID GiccSetBpr0(UINT32 val)
{
    __asm__ volatile("msr " ICC_BPR0_EL1 ", %0" ::"r"(val));
    ISB;
    DSB;
}
#endif
