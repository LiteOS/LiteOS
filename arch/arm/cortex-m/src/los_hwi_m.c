/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2018. All rights reserved.
 * Description: hwi_v7m
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
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "los_task_pri.h"
#include "los_hwi_pri.h"
#include "los_armcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_PLATFORM_HWI == YES)
LITE_OS_SEC_TEXT_MINOR VOID OsHwiDefaultHandler(VOID);
#endif

#if (LOSCFG_PLATFORM_HWI == NO)
BOOL OsIntActive(VOID)
{
    UINT32 ic;
    __asm__ __volatile__("MRS %0, xpsr" : "=r"(ic));
    return ((ic & 0x0000003f) != 0); /* 0x0000003f: mask of interrupt context */
}
#endif

/*****************************************************************************
 Function    : OsIntNumGet
 Description : Get a interrupt number
 Input       : None
 Output      : None
 Return      : Interrupt Indexes number
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR UINT32 OsIntNumGet(VOID)
{
    return __get_IPSR();
}

/*****************************************************************************
 Function    : OsHwiDefaultHandler
 Description : default handler of the hardware interrupt
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR VOID OsHwiDefaultHandler(VOID)
{
    UINT32 irqNum = OsIntNumGet();
    PRINT_ERR("%s irqnum:%d\n", __FUNCTION__, irqNum);
    while (1) { };
}

#if (LOSCFG_PLATFORM_HWI == YES)

#ifdef __ICCARM__
#pragma  location = ".data.vector"
#elif defined (__CC_ARM) || defined (__GNUC__)
LITE_OS_SEC_DATA_VEC
#endif

HWI_PROC_FUNC g_hwiVec[OS_VECTOR_CNT] = {
    (HWI_PROC_FUNC)0,                    // [0] Top of Stack
    (HWI_PROC_FUNC)Reset_Handler,        // [1] reset
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [2] NMI Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [3] Hard Fault Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [4] MPU Fault Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [5] Bus Fault Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [6] Usage Fault Handler
    (HWI_PROC_FUNC)0,                    // [7] Reserved
    (HWI_PROC_FUNC)0,                    // [8] Reserved
    (HWI_PROC_FUNC)0,                    // [9] Reserved
    (HWI_PROC_FUNC)0,                    // [10] Reserved
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [11] SVCall Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [12] Debug Monitor Handler
    (HWI_PROC_FUNC)0,                    // [13] Reserved
    (HWI_PROC_FUNC)osPendSV,             // [14] PendSV Handler
    (HWI_PROC_FUNC)OsHwiDefaultHandler,  // [15] SysTick Handler
};

/*****************************************************************************
 Function    : IrqEntryV7M
 Description : irq entry for cortex-m
 Input       : irq number
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR VOID IrqEntryV7M(VOID)
{
    UINT32 hwiIndex;

#if (LOSCFG_KERNEL_RUNSTOP == YES)
    SCB->SCR &= (UINT32)~((UINT32)SCB_SCR_SLEEPDEEP_Msk);
#endif

    hwiIndex = OsIntNumGet();
    OsInterrupt(hwiIndex);
#if (LOSCFG_KERNEL_RUNSTOP == YES)
    if (g_srStateFlag == SENSORHUB_SLEEP) {
        DisableWakeUpTimer();
        LOS_SystemWakeup(hwiIndex);
        g_srStateFlag = SENSORHUB_CLODBOOT;
        OsSRRestoreRegister();
    }
#endif
    if (OsTaskProcSignal() != 0) {
        OsSchedPreempt();
    }
}

/*****************************************************************************
 Function    : HalIrqInit
 Description : initialization of the hardware interrupt
 Input       : None
 Output      : None
 Return      : NONE
 *****************************************************************************/
VOID HalIrqInit(VOID)
{
    UINT32 index;
    for (index = OS_SYS_VECTOR_CNT; index < OS_VECTOR_CNT; index++) {
            g_hwiVec[index] = (HWI_PROC_FUNC)IrqEntryV7M;
    }

    /* Interrupt vector table location */
    SCB->VTOR = (UINT32)g_hwiVec;

#if (__CORTEX_M >= 0x03U)  /* only for Cortex-M3 and above */
    NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
#endif

    return;
}

/*****************************************************************************
 Function    : HalIrqCreate
 Description : create hardware interrupt
 Input       : irq      --- hwi num to create
               priority --- priority of the hwi
 Output      : None
 Return      : OS_SUCCESS on success or error code on failure
 *****************************************************************************/
UINT32 HalIrqCreate(UINT32 irq, UINT8 priority)
{
    UINT32 intSave;

    if ((priority > OS_HWI_PRIO_LOWEST) || (priority < OS_HWI_PRIO_HIGHEST)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    intSave = LOS_IntLock();

    NVIC_EnableIRQ((IRQn_Type)irq);
    NVIC_SetPriority((IRQn_Type)irq, priority);

    LOS_IntRestore(intSave);

    return LOS_OK;
}

/*****************************************************************************
 Function    : LOS_HwiDelete
 Description : Delete hardware interrupt
 Input       : irq   --- hwi num to delete
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
UINT32 HalIrqDelete(HWI_HANDLE_T irq)
{
    NVIC_DisableIRQ((IRQn_Type)irq);

    return LOS_OK;
}

VOID HalIrqMask(UINT32 vector)
{
    NVIC_DisableIRQ(vector);
}

VOID HalIrqUnmask(UINT32 vector)
{
    NVIC_EnableIRQ(vector);
}

VOID HalIrqPending(UINT32 vector)
{
    NVIC_SetPendingIRQ(vector);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
