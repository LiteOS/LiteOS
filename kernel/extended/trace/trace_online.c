/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Online Mode Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-03-31
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

#include "los_trace_pri.h"
#include "trace_pipeline.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_RECORDER_MODE_ONLINE
UINT32 OsTraceGetMaskTid(UINT32 taskId)
{
    return taskId;
}

UINT32 OsTraceBufInit(VOID *buf, UINT32 size)
{
    (VOID)buf;
    (VOID)size;
    return LOS_OK;
}

VOID OsTraceSendHead(VOID)
{
    TraceBaseHeaderInfo head = {
        .bigLittleEndian = TRACE_BIGLITTLE_WORD,
        .version         = TRACE_VERSION(TRACE_MODE_ONLINE),
        .clockFreq       = GET_SYS_CLOCK(),
    };

    OsTraceDataSend(HEAD, sizeof(TraceBaseHeaderInfo), (UINT8 *)&head);
}

VOID OsTraceSendNotify(UINT32 type, UINT32 value)
{
    TraceNotifyFrame frame = {
        .cmd   = type,
        .param = value,
    };

    OsTraceDataSend(NOTIFY, sizeof(TraceNotifyFrame), (UINT8 *)&frame);
}

STATIC VOID OsTraceSendObj(const LosTaskCB *tcb)
{
    ObjData obj;

    OsTraceSetObj(&obj, tcb);
    OsTraceDataSend(OBJ, sizeof(ObjData), (UINT8 *)&obj);
}

VOID OsTraceSendObjTable(VOID)
{
    UINT32 loop;
    LosTaskCB *tcb = NULL;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        tcb = g_taskCBArray + loop;
        if (tcb->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
        OsTraceSendObj(tcb);
    }
}

VOID OsTraceObjAdd(UINT32 eventType, UINT32 taskId)
{
    if (OsTraceIsEnable()) {
        OsTraceSendObj(OS_TCB_FROM_TID(taskId));
    }
}

VOID OsTraceWriteOrSendEvent(const TraceEventFrame *frame)
{
    OsTraceDataSend(EVENT, sizeof(TraceEventFrame), (UINT8 *)frame);
}

OfflineHead *OsTraceRecordGet(VOID)
{
    return NULL;
}

#endif /* LOSCFG_RECORDER_MODE_ONLINE */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
