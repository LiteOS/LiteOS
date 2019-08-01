/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "los_hwi.h"
#if (LOSCFG_KERNEL_TICKLESS == YES)
#include "los_tickless.ph"
#endif
#if (LOSCFG_PLATFORM_HWI == NO)
#include "los_tick.ph"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*****************************************************************************
 Function    : osIntNumGet
 Description : Get a interrupt number
 Input       : None
 Output      : None
 Return      : Interrupt Indexes number
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR UINT32 osIntNumGet(VOID)
{
    return __get_IPSR();
}

#if (LOSCFG_PLATFORM_HWI == YES)
/*****************************************************************************
 Function    : osHwiDefaultHandler
 Description : default handler of the hardware interrupt
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
/*lint -e529*/
LITE_OS_SEC_TEXT_MINOR VOID  osHwiDefaultHandler(VOID)
{
    PRINT_ERR("%s irqnum:%d\n", __FUNCTION__, osIntNumGet());

    while(1);
}

/*****************************************************************************
 Function    : NMI_Handler
 Description : default handler of nmi
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID NMI_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : HardFault_Handler
 Description : default handler of hard fault
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID HardFault_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : MemManage_Handler
 Description : default handler of MPU fault
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID MemManage_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : BusFault_Handler
 Description : default handler of bus fault
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID BusFault_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : UsageFault_Handler
 Description : default handler of usage fault
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID UsageFault_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : SVC_Handler
 Description : default handler of svc
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID SVC_Handler (VOID)
{
    osHwiDefaultHandler();
}

/*****************************************************************************
 Function    : Debug_Handler
 Description : default handler of debug
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
WEAK VOID Debug_Handler (VOID)
{
    osHwiDefaultHandler();
}

extern void Reset_Handler(void);
extern void PendSV_Handler(void);

VOID osInterrupt(VOID);

/*lint -save -e40 -e522 -e533*/

LITE_OS_SEC_DATA_INIT UINT32 g_vuwIntCount = 0;

/*lint -restore*/
#if    defined (__ICCARM__)
#pragma location = ".intvec"
#elif  defined (__CC_ARM)
__attribute__ ((section("RESET")))
#elif  defined (__GNUC__)
__attribute__ ((section(".isr_vector")))
#endif
VOID (* const g_pstHwiForm[])(VOID)  =
{
    NULL,                // [0] Top of Stack
    Reset_Handler,       // [1] reset
    NMI_Handler,         // [2] NMI Handler
    HardFault_Handler,   // [3] Hard Fault Handler
    MemManage_Handler,   // [4] MPU Fault Handler
    BusFault_Handler,    // [5] Bus Fault Handler
    UsageFault_Handler,  // [6] Usage Fault Handler
    NULL,                // [7] Reserved
    NULL,                // [8] Reserved
    NULL,                // [9] Reserved
    NULL,                // [10] Reserved
    SVC_Handler,         // [11] SVCall Handler
    Debug_Handler,       // [12] Debug Monitor Handler
    NULL,                // [13] Reserved
    PendSV_Handler,      // [14] PendSV Handler
    osInterrupt,         // [15] SysTick Handler, can be connected with LOS_HwiCreate

    /* so SysTick Handler will be index == 0, following IRQs starts from 1 */

#include "__vectors.h"
};

/* SysTick_IRQn == -1, and it have the index of 0, IRQs starts from 1 */

#define VECTOR_IDX_OFFSET       1

HWI_HANDLER_T m_pstHwiSlaveForm[OS_HWI_MAX_NUM + VECTOR_IDX_OFFSET];

/*****************************************************************************
 Function    : osInterrupt
 Description : Hardware interrupt entry function
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
LITE_OS_SEC_TEXT VOID osInterrupt(VOID)
{
    UINT32 uwHwiIndex;
    UINT32 uwIntSave;

#if(LOSCFG_KERNEL_RUNSTOP == YES)
    SCB->SCR &= (UINT32)~((UINT32)SCB_SCR_SLEEPDEEP_Msk);
#endif

    uwIntSave = LOS_IntLock();
    g_vuwIntCount++;
    LOS_IntRestore(uwIntSave);

    uwHwiIndex = osIntNumGet();

#if (LOSCFG_KERNEL_TICKLESS == YES)
    osUpdateKernelTickCount(uwHwiIndex);
#endif

    uwHwiIndex = uwHwiIndex - OS_SYS_VECTOR_CNT + VECTOR_IDX_OFFSET;

    if (m_pstHwiSlaveForm[uwHwiIndex].pfnHandler != NULL)
    {
#if (OS_HWI_WITH_ARG == YES)
        m_pstHwiSlaveForm[uwHwiIndex].pfnHandler(m_pstHwiSlaveForm[uwHwiIndex].pParm);
#else
        m_pstHwiSlaveForm[uwHwiIndex].pfnHandler();
#endif
    }
    else
    {
        osHwiDefaultHandler();
    }

    uwIntSave = LOS_IntLock();
    g_vuwIntCount--;
    LOS_IntRestore(uwIntSave);
}
/*****************************************************************************
 Function    : osHwiInit
 Description : initialization of the hardware interrupt
 Input       : None
 Output      : None
 Return      : OS_SUCCESS
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT VOID osHwiInit()
{
#if ((__CORTEX_M == 0U) || (__CORTEX_M == 23U)) && \
    defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (UINT32)g_pstHwiForm;
#endif

#if (__CORTEX_M >= 0x03U)  /* only for Cortex-M3 and above */
    SCB->VTOR = (UINT32)g_pstHwiForm;
    NVIC_SetPriorityGrouping(OS_NVIC_AIRCR_PRIGROUP);
#endif

    return;
}
/*****************************************************************************
 Function    : LOS_HwiCreate
 Description : create hardware interrupt
 Input       : uwHwiNum   --- hwi num to create
               usHwiPrio  --- priority of the hwi
               usMode     --- unused
               pfnHandler --- hwi handler
               uwArg      --- param of the hwi handler
 Output      : None
 Return      : OS_SUCCESS on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiCreate(HWI_HANDLE_T  uwHwiNum,
                                           HWI_PRIOR_T   usHwiPrio,
                                           HWI_MODE_T    usMode,
                                           HWI_PROC_FUNC pfnHandler,
                                           HWI_ARG_T     uwArg )
{
    UINTPTR uvIntSave;

    if (NULL == pfnHandler)
    {
        return OS_ERRNO_HWI_PROC_FUNC_NULL;
    }

    /* SysTick_IRQn == (UINT32) -1 */

    if ((uwHwiNum >= OS_HWI_MAX_NUM) && (uwHwiNum != (UINT32)SysTick_IRQn))
    {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    //because unsigned interger could be less than zero,so if OS_HWI_PRIO_HIGHEST is zero,there
    //will be warning using usHwiPrio < OS_HWI_PRIO_HIGHEST
    //  if ((usHwiPrio > OS_HWI_PRIO_LOWEST) || (usHwiPrio < OS_HWI_PRIO_HIGHEST))  --modified by  zhangqf
    if (usHwiPrio > OS_HWI_PRIO_LOWEST)
    {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    uvIntSave = LOS_IntLock();

    m_pstHwiSlaveForm[uwHwiNum + VECTOR_IDX_OFFSET].pfnHandler = pfnHandler;

#if (OS_HWI_WITH_ARG == YES)
    m_pstHwiSlaveForm[uwHwiNum + VECTOR_IDX_OFFSET].pParm = (VOID*)uwArg;
#endif

    NVIC_EnableIRQ((IRQn_Type)uwHwiNum);
    NVIC_SetPriority((IRQn_Type)uwHwiNum, usHwiPrio);

    LOS_IntRestore(uvIntSave);

    return LOS_OK;
}

/*****************************************************************************
 Function    : LOS_HwiDelete
 Description : Delete hardware interrupt
 Input       : uwHwiNum   --- hwi num to delete
 Output      : None
 Return      : LOS_OK on success or error code on failure
 *****************************************************************************/
LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiDelete(HWI_HANDLE_T uwHwiNum)
{
    UINT32 uwIntSave;

    if (uwHwiNum >= OS_HWI_MAX_NUM)
    {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    NVIC_DisableIRQ((IRQn_Type)uwHwiNum);

    uwIntSave = LOS_IntLock();
    m_pstHwiSlaveForm[uwHwiNum + VECTOR_IDX_OFFSET].pfnHandler = NULL;

    LOS_IntRestore(uwIntSave);

    return LOS_OK;
}

#endif /*(LOSCFG_PLATFORM_HWI == YES)*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


