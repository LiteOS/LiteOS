/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
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
#include "main.h"
#include "sys_init.h"
#include "los_base.h"
#include "los_task_pri.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "uart.h"
#include "gpio.h"


VOID HardwareInit(VOID)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    dwt_delay_init(SystemCoreClock);
}


UINT32 LED1_init(VOID)
{
    while(1) {
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); // 需要和“创建裸机工程”中配置的LED灯引脚对应
        LOS_TaskDelay(500000);
    }
    return 0;
}

UINT32 LED2_init(VOID)
{
    while(1) {
        HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10); // 需要和“创建裸机工程”中配置的LED灯引脚对应
        LOS_TaskDelay(1000000);
    }
    return 0;
}

STATIC UINT32 LED1TaskCreate(VOID)
{
    UINT32 taskId;
    TSK_INIT_PARAM_S LEDTask;

    (VOID)memset_s(&LEDTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    LEDTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LED1_init;
    LEDTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    LEDTask.pcName = "LED1_Task";
    LEDTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    LEDTask.uwResved = LOS_TASK_STATUS_DETACHED;
    return LOS_TaskCreate(&taskId, &LEDTask);
}

STATIC UINT32 LED2TaskCreate(VOID)
{
    UINT32 taskId;
    TSK_INIT_PARAM_S LEDTask;

    (VOID)memset_s(&LEDTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    LEDTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LED2_init;
    LEDTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    LEDTask.pcName = "LED2_Task";
    LEDTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    LEDTask.uwResved = LOS_TASK_STATUS_DETACHED;
    return LOS_TaskCreate(&taskId, &LEDTask);
}

UINT32 app_init(VOID)
{
    LED1TaskCreate();
    LED2TaskCreate();

    return 0;
}


INT32 main(VOID)
{
    HardwareInit();

    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n"
                  "\nLiteOS Kernel Version : %s\n"
                  "build data : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING, __DATE__, __TIME__);

    ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    OsStart();

    return 0;
}
