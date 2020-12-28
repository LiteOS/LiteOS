/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Interrupt Demo
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_hwi.h"
#include "los_typedef.h"
#include "los_api_interrupt.h"
#include "los_inspect_entry.h"
#ifdef LOSCFG_PLATFORM_STM32F429IGTX
#include "stm32f4xx_hal.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SOFT_IRQ_NUM 39

#ifdef LOSCFG_PLATFORM_STM32F429IGTX
static VOID Example_Exti0_Init(VOID)
{
    /* add your IRQ init code here */
    GPIO_InitTypeDef GPIO_InitStructure;

    __GPIOA_CLK_ENABLE();

    GPIO_InitStructure.Pin  = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    return;
}

static VOID Button_IRQHandler(VOID)
{
    printf("Button IRQ test ok.\n");
    LOS_HwiClear(EXTI0_IRQn + 16);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    LOS_InspectStatusSetById(LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_SUCCESS);
    return;
}
#endif

static VOID User_IRQHandler(VOID)
{
    printf("User IRQ test ok.\n");
    LOS_HwiClear(SOFT_IRQ_NUM);
    LOS_InspectStatusSetById(LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_SUCCESS);
    return;
}

UINT32 Example_Interrupt(VOID)
{
    UINTPTR intSave;
    intSave = LOS_IntLock();

    printf("Kernel interrupt demo begin.\n");
    /* software interrupt test */
    LOS_HwiCreate(SOFT_IRQ_NUM, 0, 0, User_IRQHandler, 0);
    LOS_HwiEnable(SOFT_IRQ_NUM);
    LOS_IntRestore(intSave);
    LOS_HwiTrigger(SOFT_IRQ_NUM);

    /* hardware interrupt test */
#ifdef LOSCFG_PLATFORM_STM32F429IGTX
    Example_Exti0_Init();
    LOS_HwiCreate(EXTI0_IRQn + 16, 0, 0, Button_IRQHandler, 0); // 16: cortex-m irq shift
    LOS_HwiEnable(EXTI0_IRQn + 16);
    LOS_IntRestore(intSave);
#endif

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
