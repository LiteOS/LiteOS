/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Demo Inspect Entry
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include <string.h>
#include "los_inspect_entry.h"
#include "los_task.h"

/* task schedule */
#include "los_api_task.h"
/* dynamic memory */
#include "los_api_dynamic_mem.h"
/* static memory */
#include "los_api_static_mem.h"
/* interrupt */
#include "los_api_interrupt.h"
/* message queue */
#include "los_api_msgqueue.h"
/* event  */
#include "los_api_event.h"
/* mutex */
#include "los_api_mutex.h"
/* semphore */
#include "los_api_sem.h"
/* sw timer */
#include "los_api_timer.h"
/* system tick */
#include "los_api_systick.h"
/* dlist */
#include "los_api_list.h"

/*****************************************************************************
    global var
 *****************************************************************************/
static UINT32 g_demoInspectErrCnt = 0;
static UINT32 g_demoTaskId;

static osInspect_Def g_inspect[LOS_INSPECT_BUFF] = {

    {LOS_INSPECT_TASK, LOS_INSPECT_STU_START, Example_TskCaseEntry, "task"},

    {LOS_INSPECT_DMEM, LOS_INSPECT_STU_START, Example_Dyn_Mem, "dynamic memory"},

    {LOS_INSPECT_SMEM, LOS_INSPECT_STU_START, Example_StaticMem, "static memory"},

    {LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_START, Example_Interrupt, "interrupt"},

    {LOS_INSPECT_MSG, LOS_INSPECT_STU_START, Example_MsgQueue, "message queue"},

    {LOS_INSPECT_EVENT, LOS_INSPECT_STU_START, Example_SndRcvEvent, "event"},

    {LOS_INSPECT_MUTEX, LOS_INSPECT_STU_START, Example_MutexLock, "mutex"},

    {LOS_INSPECT_SEM, LOS_INSPECT_STU_START, Example_Semphore, "semaphore"},

    {LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_START, Example_GetTick, "systick"},

    {LOS_INSPECT_TIMER, LOS_INSPECT_STU_START, Example_SwTimer, "timer"},

    {LOS_INSPECT_LIST, LOS_INSPECT_STU_START, Example_List, "list"},

};

/*****************************************************************************
 Function    : LOS_InspectStatusSetById
 Description : Set InspectStatus by id
 Input       : enInspectId  InspectId
               enInspectStu InspectStu
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 LOS_InspectStatusSetById(enInspectId InspectId, enInspectStu InspectStu)
{
    UINT32 index;

    if (InspectId >= LOS_INSPECT_BUFF) {
        printf("\nInspectId = [%d] Err.\n", InspectId);
        return LOS_NOK;
    }

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        if (InspectId == g_inspect[index].InspectId) {
            g_inspect[index].Status = InspectStu;
            break;
        }
    }

    if (index == LOS_INSPECT_BUFF) {
        printf("\nInspectId = [%d] not find.\n", InspectId);
        return LOS_NOK;
    } else {
        return LOS_OK;
    }
}

/*****************************************************************************
 Function    : LOS_InspectByID
 Description : Inspect function By ID
 Input       : enInspectId  InspectId
 Output      : None
 Return      : LOS_NOK/LOS_OK
 *****************************************************************************/
UINT32 LOS_InspectByID(enInspectId InspectId)
{
    UINT32 index;
    UINT32 ret;
    enInspectStu enCurStatus = LOS_INSPECT_STU_START;

    if (InspectId >= LOS_INSPECT_BUFF) {
        printf("\nInspectId = [%d] Err.\n", InspectId);
        return LOS_NOK;
    }

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        if (InspectId == g_inspect[index].InspectId) {
            if (g_inspect[index].Inspectfunc == NULL) {
                printf("InspectId = [%d] Err, Inspectfunc is NULL.\n\n", InspectId);
                return LOS_NOK;
            }

            ret = g_inspect[index].Inspectfunc();

            do {
                if ((g_inspect[index].Status == LOS_INSPECT_STU_SUCCESS) && (ret == LOS_OK)) {
                    printf("Kernel %s demo ok.\n\n", g_inspect[index].name);
                    enCurStatus = LOS_INSPECT_STU_SUCCESS;
                } else if (g_inspect[index].Status == LOS_INSPECT_STU_ERROR) {
                    enCurStatus = LOS_INSPECT_STU_ERROR;
                    printf("Inspect %s error, g_demoInspectErrCnt = [%d]\n\n", g_inspect[index].name, g_demoInspectErrCnt);
                }

                (VOID)LOS_TaskDelay(1000);

            } while ((g_inspect[index].Status == LOS_INSPECT_STU_START) || (enCurStatus != g_inspect[index].Status));
            break;
        }
    }
    if (index == LOS_INSPECT_BUFF) {
        return LOS_NOK;
    } else {
        return enCurStatus ? LOS_NOK : LOS_OK;
    }
}

/*****************************************************************************
 Function    : LOS_Inspect_TskDeal
 Description : Inspect Task Deal
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
static VOID LOS_Inspect_TskDeal(VOID)
{
    UINT32 ret;
    UINT32 index;
    g_demoInspectErrCnt = 0;

    /* output a message on hyperterminal using printf function */
    printf("\nLos Inspect start.\n");

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        ret = LOS_InspectByID((enInspectId)index);
        if (ret != LOS_OK) {
            g_demoInspectErrCnt++;
        }
    }

    printf("Inspect completed, g_demoInspectErrCnt = %d.\n\n", g_demoInspectErrCnt);

    while (1);
}

/*****************************************************************************
 Function    : KernelDemoInspectEntry
 Description : Create Inspect task
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 KernelDemoInspectEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((VOID *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_Inspect_TskDeal;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTaskInitParam.pcName = "InspectTask";
    stTaskInitParam.usTaskPrio = 10;
    ret = LOS_TaskCreate(&g_demoTaskId, &stTaskInitParam);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
