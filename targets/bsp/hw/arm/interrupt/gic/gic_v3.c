/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: General interrupt controller version 3.0 (GICv3).
 * Notes: Reference from arm documents:
 *        https://static.docs.arm.com/ihi0069/d/IHI0069D_gic_architecture_specification.pdf
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

#include "gic_common.h"
#include "gic_v3.h"
#include "los_typedef.h"
#include "los_hwi_pri.h"
#include "los_mp.h"

#ifdef LOSCFG_PLATFORM_BSP_GIC_V3

STATIC UINT32 g_curIrqNum = 0;

STATIC INLINE UINT64 MpidrToAffinity(UINT64 mpidr)
{
    return ((MPIDR_AFF_LEVEL(mpidr, 3) << 32) |
            (MPIDR_AFF_LEVEL(mpidr, 2) << 16) |
            (MPIDR_AFF_LEVEL(mpidr, 1) << 8)  |
            (MPIDR_AFF_LEVEL(mpidr, 0)));
}

STATIC VOID GicWaitForRwp(UINT64 reg)
{
    INT32 count = 1000000;

    while (GIC_REG_32(reg) & (1U << 31)) {
        count -= 1;
        if (!count) {
            PRINTK("gic_v3: rwp timeout 0x%x\n", GIC_REG_32(reg));
            return;
        }
    }
}

STATIC INLINE VOID GicdSetGroup(UINT32 irq)
{
    /* configure spi as group 0 on secure mode and group 1 on unsecure mode */
#if LOSCFG_ARCH_SECURE_MONITOR_MODE
    GIC_REG_32(GICD_IGROUPR(irq / 32)) = 0;
#else
    GIC_REG_32(GICD_IGROUPR(irq / 32)) = 0xffffffff;
#endif
}

STATIC INLINE VOID GicrSetWaker(UINT32 cpu)
{
    GIC_REG_32(GICR_WAKER(cpu)) &= ~GICR_WAKER_PROCESSORSLEEP;
    DSB;
    ISB;
    while ((GIC_REG_32(GICR_WAKER(cpu)) & 0x4) == GICR_WAKER_CHILDRENASLEEP);
}

STATIC INLINE VOID GicrSetGroup(UINT32 cpu)
{
    /* configure sgi/ppi as group 0 on secure mode and group 1 on unsecure mode */
#if LOSCFG_ARCH_SECURE_MONITOR_MODE
    GIC_REG_32(GICR_IGROUPR0(cpu)) = 0;
    GIC_REG_32(GICR_IGRPMOD0(cpu)) = 0;
#else
    GIC_REG_32(GICR_IGROUPR0(cpu)) = 0xffffffff;
#endif
}

STATIC VOID GicdSetPmr(UINT32 irq, UINT8 priority)
{
    UINT32 pos = irq >> 2; /* one irq have the 8-bit interrupt priority field */
    UINT32 newPri = GIC_REG_32(GICD_IPRIORITYR(pos));

    /* Shift and mask the correct bits for the priority */
    newPri &= ~(GIC_PRIORITY_MASK << ((irq % 4) * GIC_PRIORITY_OFFSET));
    newPri |= priority << ((irq % 4) * GIC_PRIORITY_OFFSET);

    GIC_REG_32(GICD_IPRIORITYR(pos)) = newPri;
}

STATIC VOID GicrSetPmr(UINT32 irq, UINT8 priority)
{
    UINT32 cpu = ArchCurrCpuid();
    UINT32 pos = irq >> 2; /* one irq have the 8-bit interrupt priority field */
    UINT32 newPri = GIC_REG_32(GICR_IPRIORITYR0(cpu) + pos * 4);

    /* Clear priority offset bits and set new priority */
    newPri &= ~(GIC_PRIORITY_MASK << ((irq % 4) * GIC_PRIORITY_OFFSET));
    newPri |= priority << ((irq % 4) * GIC_PRIORITY_OFFSET);

    GIC_REG_32(GICR_IPRIORITYR0(cpu) + pos * 4) = newPri;
}

STATIC VOID GiccInitPercpu(VOID)
{
    /* enable system register interface */
    UINT32 sre = GiccGetSre();
    if (!(sre & 0x1)) {
        GiccSetSre(sre | 0x1);

        /*
         * Need to check that the SRE bit has actually been set. If
         * not, it means that SRE is disabled at up EL level. We're going to
         * die painfully, and there is nothing we can do about it.
         */
        sre = GiccGetSre();
        LOS_ASSERT(sre & 0x1);
    }

#if LOSCFG_ARCH_SECURE_MONITOR_MODE
    /* Enable group 0 and disable grp1ns grp1s interrupts */
    GiccSetIgrpen0(1);
    GiccSetIgrpen1(0);

    /*
     * For priority grouping.
     * The value of this field control show the 8-bit interrupt priority field
     * is split into a group priority field, that determines interrupt preemption,
     * and a subpriority field.
     */
    GiccSetBpr0(MAX_BINARY_POINT_VALUE);
#else
    /* enable group 1 interrupts */
    GiccSetIgrpen1(1);
#endif

    /* set priority threshold to max */
    GiccSetPmr(0xff);

    /* EOI deactivates interrupt too (mode 0) */
    GiccSetCtlr(0);
}

UINT32 HalCurIrqGet(VOID)
{
    return g_curIrqNum;
}

VOID HalIrqMask(UINT32 vector)
{
    INT32 i;
    const UINT32 mask = 1U << (vector % 32);

    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) {
        return;
    }

    if (vector < 32) {
        for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
            GIC_REG_32(GICR_ICENABLER0(i)) = mask;
            GicWaitForRwp(GICR_CTLR(i));
        }
    } else {
        GIC_REG_32(GICD_ICENABLER(vector >> 5)) = mask;
        GicWaitForRwp(GICD_CTLR);
    }
}

VOID HalIrqUnmask(UINT32 vector)
{
    INT32 i;
    const UINT32 mask = 1U << (vector % 32);

    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) {
        return;
    }

    if (vector < 32) {
        for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
            GIC_REG_32(GICR_ISENABLER0(i)) = mask;
            GicWaitForRwp(GICR_CTLR(i));
        }
    } else {
        GIC_REG_32(GICD_ISENABLER(vector >> 5)) = mask;
        GicWaitForRwp(GICD_CTLR);
    }
}

VOID HalIrqPending(UINT32 vector)
{
    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) { /*lint !e685 !e568*/
        return;
    }

    GIC_REG_32(GICD_ISPENDR(vector >> 5)) = 1U << (vector % 32);
}

VOID HalIrqClear(UINT32 vector)
{
    GiccSetEoir(vector);
    ISB;
}

UINT32 HalIrqSetPrio(UINT32 vector, UINT8 priority)
{
    UINT8 prio = priority;

    if (vector > OS_HWI_MAX_NUM) {
        PRINT_ERR("Invalid irq value %u, max irq is %u\n", vector, OS_HWI_MAX_NUM);
        return LOS_NOK;
    }

    prio = prio & (UINT8)GIC_INTR_PRIO_MASK;

    if (vector >= GIC_MIN_SPI_NUM) {
        GicdSetPmr(vector, prio);
    } else {
        GicrSetPmr(vector, prio);
    }

    return LOS_OK;
}

VOID HalIrqInitPercpu(VOID)
{
    INT32 idx;
    UINT32 cpu = ArchCurrCpuid();

    /* GICR init */
    GicrSetWaker(cpu);
    GicrSetGroup(cpu);
    GicWaitForRwp(GICR_CTLR(cpu));

    /* GICR: clear and mask sgi/ppi */
    GIC_REG_32(GICR_ICENABLER0(cpu)) = 0xffffffff;
    GIC_REG_32(GICR_ICPENDR0(cpu)) = 0xffffffff;

    GIC_REG_32(GICR_ISENABLER0(cpu)) = 0xffffffff;

    for (idx = 0; idx < GIC_MIN_SPI_NUM; idx += 1) {
        GicrSetPmr(idx, MIN_INTERRUPT_PRIORITY);
    }

    GicWaitForRwp(GICR_CTLR(cpu));

    /* GICC init */
    GiccInitPercpu();
}

VOID HalIrqInit(VOID)
{
    UINT32 i;
    UINT64 affinity;

    /* disable distributor */
    GIC_REG_32(GICD_CTLR) = 0;
    GicWaitForRwp(GICD_CTLR);
    ISB;

    /* set externel interrupts to be level triggered, active low. */
    for (i = 32; i < OS_HWI_MAX_NUM; i += 16) {
        GIC_REG_32(GICD_ICFGR(i / 16)) = 0;
    }

    /* config distributer, mask and clear all spis, set group x */
    for (i = 32; i < OS_HWI_MAX_NUM; i += 32) {
        GIC_REG_32(GICD_ICENABLER(i / 32)) = 0xffffffff;
        GIC_REG_32(GICD_ICPENDR(i / 32)) = 0xffffffff;
        GIC_REG_32(GICD_IGRPMODR(i / 32)) = 0;

        GicdSetGroup(i);
    }

    /* set spi priority as default */
    for (i = 32; i < OS_HWI_MAX_NUM; i++) {
        GicdSetPmr(i, MIN_INTERRUPT_PRIORITY);
    }

    GicWaitForRwp(GICD_CTLR);

    /* disable all interrupts. */
    for (i = 0; i < OS_HWI_MAX_NUM; i += 32) {
        GIC_REG_32(GICD_ICENABLER(i / 32)) = 0xffffffff;
    }

    /* enable distributor with ARE, group 1 enabled */
    GIC_REG_32(GICD_CTLR) = CTLR_ENALBE_G0 | CTLR_ENABLE_G1NS | CTLR_ARE_S;

    /* set spi to boot cpu only. ARE must be enabled */
    affinity = MpidrToAffinity(AARCH64_SYSREG_READ(mpidr_el1));
    for (i = 32; i < OS_HWI_MAX_NUM; i++) {
        GIC_REG_64(GICD_IROUTER(i)) = affinity;
    }

    HalIrqInitPercpu();
}

VOID HalIrqHandler(VOID)
{
    UINT32 iar = GiccGetIar();
    UINT32 vector = iar & 0x3FFU;

    /*
     * invalid irq number, mainly the spurious interrupts 0x3ff,
     * valid irq ranges from 0~1019, we use OS_HWI_MAX_NUM to do
     * the checking.
     */
    if (vector >= OS_HWI_MAX_NUM) {
        return;
    }
    g_curIrqNum = vector;

    OsInterrupt(vector);
    GiccSetEoir(vector);
}

CHAR *HalIrqVersion(VOID)
{
    UINT32 pidr = GIC_REG_32(GICD_PIDR2V3);
    CHAR *irqVerString = NULL;

    switch (pidr >> GIC_REV_OFFSET) {
        case GICV3:
            irqVerString = "GICv3";
            break;
        case GICV4:
            irqVerString = "GICv4";
            break;
        default:
            irqVerString = "unknown";
    }
    return irqVerString;
}

UINT32 HalIrqCreate(UINT32 irq, UINT8 priority)
{
    (VOID)irq;
    (VOID)priority;
    return LOS_OK;
}
UINT32 HalIrqDelete(UINT32 irq)
{
    (VOID)irq;
    return LOS_OK;
}
#endif
