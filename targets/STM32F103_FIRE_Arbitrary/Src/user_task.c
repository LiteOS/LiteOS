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
#include "gpio.h"

#define LED_TASK_PRIORITY   5
static UINT32 g_led_tskHandle;

STATIC UINT32 LedTask(VOID)
{
   UINT32 ret = LOS_OK;
    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        LOS_TaskDelay(1000);
   }
   return ret;
}

STATIC UINT32 LedTaskCreate(VOID)
{
    uint32_t ret = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    memset(&task_init_param, 0, sizeof(TSK_INIT_PARAM_S));
    task_init_param.usTaskPrio = LED_TASK_PRIORITY;
    task_init_param.pcName = "LedTask";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)LedTask;
    task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ret = LOS_TaskCreate(&g_led_tskHandle, &task_init_param);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}

VOID app_init(VOID)
{
    printf("Hello, welcome to liteos!\n");
    LedTaskCreate();
    (VOID)DemoEntry();
}
