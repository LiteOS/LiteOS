/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: User Task Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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

#include "sys_init.h"
#include "demo_entry.h"
#include "stdio.h"
#include "core_cm0.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
#include "los_hwi.h"

#define OS_SYS_VECTOR_CNT_M0 16
#define OS_EXC_NMI_M0 2

VOID UserTaskFunc1(VOID)
{
    while (1) {
        printf("task1.\r\n");
        LOS_TaskDelay(1000);
    }
}

VOID UserTaskFunc2(VOID)
{
    while (1) {
        printf("task2.\r\n");
        LOS_TaskDelay(2000);
    }
}

STATIC UINT32 UserTask1(VOID)
{
    UINT32 taskId = 0;
    TSK_INIT_PARAM_S taskP;

    memset(&taskP, 0, sizeof(TSK_INIT_PARAM_S));
    taskP.pfnTaskEntry = (TSK_ENTRY_FUNC)UserTaskFunc1;
    taskP.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskP.pcName = "Task1";
    taskP.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    return LOS_TaskCreate(&taskId, &taskP);
}

STATIC UINT32 UserTask2(VOID)
{
    UINT32 taskId = 0;
    TSK_INIT_PARAM_S taskP;

    memset(&taskP, 0, sizeof(TSK_INIT_PARAM_S));
    taskP.pfnTaskEntry = (TSK_ENTRY_FUNC)UserTaskFunc2;
    taskP.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskP.pcName = "Task2";
    taskP.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    return LOS_TaskCreate(&taskId, &taskP);
}

VOID UserTask(VOID)
{
    UserTask1();
    UserTask2();
}

STATIC VOID ButtonIrqHandle(VOID)
{
    BSP_LED_Toggle(LED2);
    /* clear extern irq */
    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_13);
}

STATIC VOID UserButtonIrqConfig(VOID)
{
    UINT32 ret;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, OS_EXC_NMI_M0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
    /* EXTI4_15_IRQn + 16 */
    ret = LOS_HwiCreate(EXTI4_15_IRQn + OS_SYS_VECTOR_CNT_M0, OS_EXC_NMI_M0, 0, ButtonIrqHandle, NULL);
    if (ret != LOS_OK) {
        printf("%s, %d Create Hwi Failed\n", __FILE__, __LINE__);
    }
}

VOID app_init(VOID)
{
    printf("app init!\r\n");
    BSP_LED_Init(LED2);
    UserTask();
    UserButtonIrqConfig();
    (VOID)DemoEntry();
}
