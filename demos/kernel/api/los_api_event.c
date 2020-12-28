/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Event Demo
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

#include "los_event.h"
#include "los_task.h"
#include "los_api_event.h"
#include "los_inspect_entry.h"

#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* task pid */
static UINT32 g_demoTaskId;
/* event control struct */
static EVENT_CB_S  g_exampleEvent;

/* wait event type */
#define EVENT_WAIT 0x00000001

/* example task entry function */
VOID Example_Event(VOID)
{
    UINT32 event;
    UINT32 ret;

    /*
     * timeout, WAITMODE to read event, timeout is 100 ticks,
     * if timeout, wake task directly
     */
    printf("Example_Event wait event 0x%x.\n", EVENT_WAIT);

    event = LOS_EventRead(&g_exampleEvent, EVENT_WAIT, LOS_WAITMODE_AND, 100);
    if (event == EVENT_WAIT) {
        printf("Example_Event, read event : 0x%x.\n", event);
        ret = LOS_InspectStatusSetById(LOS_INSPECT_EVENT, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    } else {
        printf("Example_Event, read event timeout.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_EVENT, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    }
    return;
}

UINT32 Example_SndRcvEvent(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTask1;

    printf("Kernel event demo begin.\n");
    /* event init */
    ret = LOS_EventInit(&g_exampleEvent);
    if (ret != LOS_OK) {
        printf("Init event failed.\n");
        return LOS_NOK;
    }

    /* create task */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    stTask1.pcName       = "EventTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    ret = LOS_TaskCreate(&g_demoTaskId, &stTask1);
    if (ret != LOS_OK) {
        printf("Create task failed.\n");
        return LOS_NOK;
    }

    /* write event */
    printf("Example_SndRcvEvent write event.\n");
    ret = LOS_EventWrite(&g_exampleEvent, EVENT_WAIT);
    if (ret != LOS_OK) {
        printf("Write Event failed.\n");
        return LOS_NOK;
    }

    /* clear event flag */
    printf("EventMask : %d\n", g_exampleEvent.uwEventID);
    LOS_EventClear(&g_exampleEvent, ~g_exampleEvent.uwEventID);
    printf("EventMask : %d\n", g_exampleEvent.uwEventID);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
