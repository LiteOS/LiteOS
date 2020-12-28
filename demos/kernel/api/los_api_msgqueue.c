/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Message Queue Demo
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

#include "los_base.h"
#include "los_task.h"
#include "los_swtmr.h"
#include "los_hwi.h"
#include "los_queue.h"
#include "los_event.h"
#include "los_typedef.h"
#include "los_api_msgqueue.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 g_demoQueue;

static CHAR g_demoBuf[] = "test is message x";

/* task1 send data */
static VOID *send_Entry(UINT32 param)
{
    UINT32 i = 0;
    UINT32 ret;
    UINT32 len = sizeof(g_demoBuf);

    while (i < API_MSG_NUM) {
        g_demoBuf[len - 2] = '0' + i;
        i++;

        /* write g_demoBuf data to queue */
        ret = LOS_QueueWrite(g_demoQueue, g_demoBuf, len, 0);
        if (ret != LOS_OK) {
            printf("Send message failed, error: %x.\n", ret);
        }

        LOS_TaskDelay(50);
    }

    return NULL;
}

/* task2 recv data */
static VOID *recv_Entry(UINT32 param)
{
    UINT32 readbuf;
    UINT32 ret;
    UINT32 msgCount = 0;

    while (1) {
        /* read data from queue to readbuf */
        ret = LOS_QueueRead(g_demoQueue, &readbuf, 24, 0);
        if (ret != LOS_OK) {
            printf("Recv message failed, error: %x.\n", ret);
            break;
        } else {
            printf("Recv message : %s.\n", (CHAR *)readbuf);
            msgCount++;
        }

        (VOID)LOS_TaskDelay(50);
    }

    /* delete queue */
    while (LOS_QueueDelete(g_demoQueue) != LOS_OK) {
        (VOID)LOS_TaskDelay(1);
    }

    printf("Delete the queue ok.\n");

    if (API_MSG_NUM == msgCount) {
        ret = LOS_InspectStatusSetById(LOS_INSPECT_MSG, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    } else {
        ret = LOS_InspectStatusSetById(LOS_INSPECT_MSG, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    }

    return NULL;
}

UINT32 Example_MsgQueue(VOID)
{
    UINT32 ret;
    UINT32 task1, task2;
    TSK_INIT_PARAM_S stInitParam1;

    /* create task1 */
    printf("Kernel message queue demo begin.\n");
    stInitParam1.pfnTaskEntry = (TSK_ENTRY_FUNC)send_Entry;
    stInitParam1.usTaskPrio = 9;
    stInitParam1.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stInitParam1.pcName = "sendQueue";
    LOS_TaskLock(); // lock task schedue
    ret = LOS_TaskCreate(&task1, &stInitParam1);
    if (ret != LOS_OK) {
        printf("Create task1 failed, error : %x.\n", ret);
        return ret;
    }

    /* create task2 */
    stInitParam1.pfnTaskEntry = (TSK_ENTRY_FUNC)recv_Entry;
    ret = LOS_TaskCreate(&task2, &stInitParam1);
    if (ret != LOS_OK) {
        printf("Create task2 failed, error : %x.\n", ret);
        return ret;
    }

    /* create queue */
    ret = LOS_QueueCreate("queue", 5, &g_demoQueue, 0, 24);
    if (ret != LOS_OK) {
        printf("Create queue failed, error : %x.\n", ret);
    }

    printf("Create the queue ok.\n");
    LOS_TaskUnlock(); // unlock task schedue

    LOS_TaskDelay(500);

    LOS_TaskDelete(task1);
    LOS_TaskDelete(task2);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
