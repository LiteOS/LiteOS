/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: General interrupt controller version 2.0 (GICv2).
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "gic_common.h"
#include "los_hwi_pri.h"
#include "los_mp_pri.h"

STATIC_ASSERT(OS_USER_HWI_MAX <= 1020, "hwi max is too large!");

#ifdef LOSCFG_PLATFORM_BSP_GIC_V2

STATIC UINT32 g_curIrqNum = 0;
STATIC HwiHandleInfo g_hwiForm[OS_HWI_MAX_NUM] = { 0 };

#ifdef LOSCFG_KERNEL_SMP
/*
 * filter description
 *   0b00: forward to the cpu interfaces specified in cpu_mask
 *   0b01: forward to all cpu interfaces
 *   0b10: forward only to the cpu interface that request the irq
 */
STATIC VOID GicWriteSgi(UINT32 vector, UINT32 cpuMask, UINT32 filter)
{
    UINT32 val = ((filter & 0x3) << 24) | ((cpuMask & 0xFF) << 16) |
                 (vector & 0xF);

    GIC_REG_32(GICD_SGIR) = val;
}

VOID HalIrqSendIpi(UINT32 target, UINT32 ipi)
{
    GicWriteSgi(ipi, target, 0);
}

VOID HalIrqSetAffinity(UINT32 vector, UINT32 cpuMask)
{
    UINT32 offset = vector / 4;
    UINT32 index = vector & 0x3;

    GIC_REG_8(GICD_ITARGETSR(offset) + index) = cpuMask;
}
#endif

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

VOID HalIrqHandler(VOID)
{
    UINT32 iar = GIC_REG_32(GICC_IAR);
    UINT32 vector = iar & 0x3FFU;

    if (vector >= OS_HWI_MAX_NUM) {
        return;
    }

    g_curIrqNum = vector;
    OsIntHandle(vector, &g_hwiForm[vector]);

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

STATIC UINT32 HwiNumToIndex(HWI_HANDLE_T hwiNum)
{
    return hwiNum;
}

HwiHandleInfo *HalIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    UINT32 index;

    if ((hwiNum > OS_USER_HWI_MAX) || (hwiNum < OS_USER_HWI_MIN)) {
        return NULL;
    }
    index = HwiNumToIndex(hwiNum);
    return &g_hwiForm[index];
}

VOID HalIrqInitPercpu(VOID)
{
    /* unmask interrupts */
    GIC_REG_32(GICC_PMR) = 0xFF;

    /* enable gic cpu interface */
    GIC_REG_32(GICC_CTLR) = 1;
}

UINT32 StubSetIrqPriority(HWI_HANDLE_T hwiNum, UINT8 priority)
{
    (VOID) hwiNum;
    (VOID) priority;
    return LOS_OK;
}

STATIC const HwiControllerOps g_gicv2Ops = {
    .triggerIrq         = HalIrqPending,
    .clearIrq           = HalIrqClear,
    .enableIrq          = HalIrqUnmask,
    .disableIrq         = HalIrqMask,
    .setIrqPriority     = StubSetIrqPriority,
    .getCurIrqNum       = HalCurIrqGet,
    .getIrqVersion      = HalIrqVersion,
    .getHandleForm      = HalIrqGetHandleForm,
    .handleIrq          = HalIrqHandler,
#ifdef LOSCFG_KERNEL_SMP
    .sendIpi            = HalIrqSendIpi,
    .setIrqCpuAffinity  = HalIrqSetAffinity,
#endif
};

VOID HalIrqInit(VOID)
{
    UINT32 i;

    /* set external interrupts to be level triggered, active low. */
    for (i = 32; i < OS_HWI_MAX_NUM; i += 16) {
        GIC_REG_32(GICD_ICFGR(i / 16)) = 0;
    }

    /* set external interrupts to CPU 0 */
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

    /* register interrupt controller's operations */
    OsHwiControllerReg(&g_gicv2Ops);
#if (LOSCFG_KERNEL_SMP == YES)
    /* register inter-processor interrupt */
    (VOID) LOS_HwiCreate(LOS_MP_IPI_WAKEUP, 0xa0, 0, OsMpWakeHandler, 0);
    (VOID) LOS_HwiCreate(LOS_MP_IPI_SCHEDULE, 0xa0, 0, OsMpScheduleHandler, 0);
    (VOID) LOS_HwiCreate(LOS_MP_IPI_HALT, 0xa0, 0, OsMpHaltHandler, 0);
#ifdef LOSCFG_KERNEL_SMP_CALL
    (VOID) LOS_HwiCreate(LOS_MP_IPI_FUNC_CALL, 0xa0, 0, OsMpFuncCallHandler, 0);
#endif
#endif
}

#endif
