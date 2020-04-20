/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: General interrupt controller version 2.0 (GICv2).
 * Notes: Reference from arm documents:
 *        https://static.docs.arm.com/ihi0048/bb/IHI0048B_b_gic_architecture_specification.pdf
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
#include "los_hwi_pri.h"
#include "los_mp.h"

STATIC_ASSERT(OS_USER_HWI_MAX <= 1020, "hwi max is too large!");

#ifdef LOSCFG_PLATFORM_BSP_GIC_V2

STATIC UINT32 g_curIrqNum = 0;

UINT32 HalCurIrqGet(VOID)
{
    return g_curIrqNum;
}

VOID HalIrqMask(UINT32 vector)
{
    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) {
        return;
    }

    GIC_REG_32(GICD_ICENABLER(vector / 32)) = 1U << (vector % 32);
}

VOID HalIrqUnmask(UINT32 vector)
{
    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) {
        return;
    }

    GIC_REG_32(GICD_ISENABLER(vector >> 5)) = 1U << (vector % 32);
}

VOID HalIrqPending(UINT32 vector)
{
    if ((vector > OS_USER_HWI_MAX) || (vector < OS_USER_HWI_MIN)) {
        return;
    }

    GIC_REG_32(GICD_ISPENDR(vector >> 5)) = 1U << (vector % 32);
}

VOID HalIrqClear(UINT32 vector)
{
    GIC_REG_32(GICC_EOIR) = vector;
}

VOID HalIrqInitPercpu(VOID)
{
    /* unmask interrupts */
    GIC_REG_32(GICC_PMR) = 0xFF;

    /* enable gic cpu interface */
    GIC_REG_32(GICC_CTLR) = 1;
}

VOID HalIrqInit(VOID)
{
    UINT32 i;

    /* set externel interrupts to be level triggered, active low. */
    for (i = 32; i < OS_HWI_MAX_NUM; i += 16) {
        GIC_REG_32(GICD_ICFGR(i / 16)) = 0;
    }

    /* set externel interrupts to CPU 0 */
    for (i = 32; i < OS_HWI_MAX_NUM; i += 4) {
        GIC_REG_32(GICD_ITARGETSR(i / 4)) = 0x01010101;
    }

    /* set priority on all interrupts */
    for (i = 0; i < OS_HWI_MAX_NUM; i += 4) {
        GIC_REG_32(GICD_IPRIORITYR(i / 4)) = GICD_INT_DEF_PRI_X4;
    }

    /* disable all interrupts. */
    for (i = 0; i < OS_HWI_MAX_NUM; i += 32) {
        GIC_REG_32(GICD_ICENABLER(i / 32)) = ~0;
    }

    HalIrqInitPercpu();

    /* enable gic distributor control */
    GIC_REG_32(GICD_CTLR) = 1;
}

VOID HalIrqHandler(VOID)
{
    UINT32 iar = GIC_REG_32(GICC_IAR);
    UINT32 vector = iar & 0x3FFU;

    /*
     * invalid irq number, mainly the spurious interrupts 0x3ff,
     * gicv2 valid irq ranges from 0~1019, we use OS_HWI_MAX_NUM
     * to do the checking.
     */
    if (vector >= OS_HWI_MAX_NUM) {
        return;
    }
    g_curIrqNum = vector;

    OsInterrupt(vector);

    /* use orignal iar to do the EOI */
    GIC_REG_32(GICC_EOIR) = iar;
}

CHAR *HalIrqVersion(VOID)
{
    UINT32 pidr = GIC_REG_32(GICD_PIDR2V2);
    CHAR *irqVerString = NULL;

    switch (pidr >> GIC_REV_OFFSET) {
        case GICV1:
            irqVerString = "GICv1";
            break;
        case GICV2:
            irqVerString = "GICv2";
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
