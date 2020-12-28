/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Lvgl Demo
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

#define LV_USE_DEMO_WIDGETS

#include "los_task.h"
#include "lvgl_demo.h"

#include "lvgl.h"
#include "stm32f7xx.h"
#include "stm32f769i_discovery.h"
#include "tft/tft.h"
#include "touchpad/touchpad.h"

#ifdef LV_USE_DEMO_WIDGETS
#include "widgets/lv_demo_widgets.h"
#endif

#define LVGL_TASK_STACK_SIZE  0x2000
#define LVGL_TASK_POLL_PERIOD 5


STATIC VOID EnableCache(VOID)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

STATIC VOID LvglTaskEntry(VOID)
{
    /* Enable the CPU Cache */
    EnableCache();

    lv_init();

    tft_init();
    touchpad_init();

#ifdef LV_USE_DEMO_WIDGETS
    lv_demo_widgets();
#endif

    while (1) {
        lv_task_handler();
        LOS_Msleep(LVGL_TASK_POLL_PERIOD);
    }
}

UINT32 LvglDemo(VOID)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S lvglTask;

    (VOID)memset_s(&lvglTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    lvglTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LvglTaskEntry;
    lvglTask.uwStackSize = LVGL_TASK_STACK_SIZE;
    lvglTask.pcName = "Lvgl_Task";
    lvglTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    lvglTask.uwResved = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&taskId, &lvglTask);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}
