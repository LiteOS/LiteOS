/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * 
 * Copyright (c) 2017, Synopsys, Inc.
 * Modified for port to ARC processor by Wayne Ren wei.ren@synopsys.com 
 * 
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


extern void LosAdapIrpEnable(unsigned int irqnum, unsigned short prior);
extern void LosAdapIrqDisable(unsigned int irqnum);


/*****************************************************************************
 Function    : LOS_IntUnLock
 Description : Unlock all interrupts
 Input       : None
 Output      : None
 Return      : 0
 *****************************************************************************/
UINTPTR LOS_IntUnLock(VOID) 
{
    arc_unlock();
    return 0;
}

/*****************************************************************************
 Function    : LOS_IntLock
 Description : Lock all interrupts
 Input       : None
 Output      : None
 Return      : Interrupt priority mask and interrupt enable bit 
 *****************************************************************************/
UINTPTR LOS_IntLock(VOID)
{
    return (UINTPTR) arc_lock_save();
}

/*****************************************************************************
 Function    : LOS_IntRestore
 Description : Restore interrupt related status
 Input       : None
 Output      : None
 Return      : None 
 *****************************************************************************/
VOID LOS_IntRestore(UINTPTR uvIntSave)
{
    arc_unlock_restore(uvIntSave);
}


/*****************************************************************************
 Function    : osIntNumGet
 Description : Get a interrupt number
 Input       : None
 Output      : None
 Return      : Interrupt Indexes number
 *****************************************************************************/
LITE_OS_SEC_TEXT_MINOR UINT32 osIntNumGet(VOID)
{
    return _arc_lr_reg(AUX_IRQ_CAUSE);
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
LITE_OS_SEC_TEXT_INIT UINT32 LOS_HwiCreate( HWI_HANDLE_T  uwHwiNum,
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
    if (uwHwiNum >= NUM_EXC_ALL || uwHwiNum < NUM_EXC_CPU)
    {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    if (usHwiPrio > OS_HWI_PRIO_LOWEST)
    {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    uvIntSave = LOS_IntLock();

    int_handler_install(uwHwiNum, (INT_HANDLER)pfnHandler);

    LosAdapIrpEnable(uwHwiNum, usHwiPrio);

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
    if (uwHwiNum >= NUM_EXC_ALL || uwHwiNum < NUM_EXC_CPU)
    {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    LosAdapIrqDisable(uwHwiNum);

    return LOS_OK;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


