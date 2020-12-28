/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Timer Demo
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

#include <stdio.h>
#include "los_swtmr.h"
#include "time.h"
#include "los_sys.h"
#include "los_api_timer.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 g_demoTimerCount1 = 0;
static UINT32 g_demoTimerCount2 = 0;

static VOID Timer1_Callback(UINT32 arg)
{
    UINT32 tickLast1;

    g_demoTimerCount1++;
    tickLast1 = (UINT32)LOS_TickCountGet();
    printf("LOS_TickCountGet g_demoTimerCount1 = %d.\n", g_demoTimerCount1);
    printf("LOS_TickCountGet tickLast1 = %d.\n", tickLast1);
}

static VOID Timer2_Callback(UINT32 arg)
{
    UINT32 ret;
    UINT32 tickLast2;

    tickLast2 = (UINT32)LOS_TickCountGet();
    g_demoTimerCount2++;
    printf("LOS_TickCountGet g_demoTimerCount2 = %d.\n", g_demoTimerCount2);
    printf("LOS_TickCountGet tickLast2 = %d.\n", tickLast2);
    ret = LOS_InspectStatusSetById(LOS_INSPECT_TIMER, LOS_INSPECT_STU_SUCCESS);
    if (ret != LOS_OK) {
        printf("Set Inspect Status Err.\n");
    }
}

UINT32 Example_SwTimer(VOID)
{
    UINT16 id1;
    UINT16 id2; // timer id
    UINT32 ret;

    printf("Kernel swtimer demo begin.\n");
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    ret = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_Callback, &id1, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    ret = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_Callback, &id1, 1);
#endif
    if (ret != LOS_OK) {
        printf("Create Timer1 failed.\n");
    } else {
        printf("Create Timer1 ok.\n");
    }

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    ret = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_Callback, &id2, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    ret = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_Callback, &id2, 1);
#endif
    if (ret != LOS_OK) {
        printf("Create Timer2 failed.\n");
    } else {
        printf("Create Timer2 ok.\n");
    }

    ret = LOS_SwtmrStart(id1);
    if (ret != LOS_OK) {
        printf("Start Timer1 failed.\n");
    } else {
        printf("Start Timer1 ok.\n");
    }

    (VOID)LOS_TaskDelay(200);

    ret = LOS_SwtmrStop(id1);
    if (ret != LOS_OK) {
        printf("Stop Timer1 failed.\n");
    } else {
        printf("Stop Timer1 ok.\n");
    }

    ret = LOS_SwtmrStart(id1);
    if (ret != LOS_OK) {
        printf("Start Timer1 failed.\n");
    }

    (VOID)LOS_TaskDelay(1000);

    /* the timer that mode is once, kernel will delete it automatically when timer is timeout */
    ret = LOS_SwtmrDelete(id1);
    if (ret != LOS_OK) {
        printf("Delete Timer1 failed.\n");
    } else {
        printf("Delete Timer1 ok.\n");
    }

    ret = LOS_SwtmrStart(id2);
    if (ret != LOS_OK) {
        printf("Start Timer2 failed.\n");
    } else {
        printf("Start Timer2 ok.\n");
    }

    (VOID)LOS_TaskDelay(1000);

    ret = LOS_SwtmrStop(id2);
    if (ret != LOS_OK) {
        printf("Stop Timer2 failed.\n");
    }

    ret = LOS_SwtmrDelete(id2);
    if (ret != LOS_OK) {
        printf("Delete Timer2 failed.\n");
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
