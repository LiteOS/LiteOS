/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Pipeline Implementation
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

#include "trace_pipeline.h"
#include "trace_tlv.h"
#include "los_trace_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_pipeSpin);
#define PIPE_LOCK(state)                   LOS_SpinLockSave(&g_pipeSpin, &(state))
#define PIPE_UNLOCK(state)                 LOS_SpinUnlockRestore(&g_pipeSpin, (state))

static TlvTable g_traceTlvTblNotify[] = {
    { CMD,    LOS_OFF_SET_OF(TraceNotifyFrame, cmd),   sizeof(UINT32) },
    { PARAMS, LOS_OFF_SET_OF(TraceNotifyFrame, param), sizeof(UINT32) },
    { TRACE_TLV_TYPE_NULL, 0, 0 },
};

static TlvTable g_traceTlvTblHead[] = {
    { ENDIAN,     LOS_OFF_SET_OF(TraceBaseHeaderInfo, bigLittleEndian), sizeof(UINT32) },
    { VERSION,    LOS_OFF_SET_OF(TraceBaseHeaderInfo, version),         sizeof(UINT32) },
    { CLOCK_FREQ, LOS_OFF_SET_OF(TraceBaseHeaderInfo, clockFreq),       sizeof(UINT32) },
    { TRACE_TLV_TYPE_NULL, 0, 0 },
};

static TlvTable g_traceTlvTblObj[] = {
    { ADDR, LOS_OFF_SET_OF(ObjData, id),   sizeof(UINT32) },
    { PRIO, LOS_OFF_SET_OF(ObjData, prio), sizeof(UINT32) },
    { NAME, LOS_OFF_SET_OF(ObjData, name), sizeof(CHAR) * LOSCFG_TRACE_OBJ_MAX_NAME_SIZE },
    { TRACE_TLV_TYPE_NULL, 0, 0 },
};

static TlvTable g_traceTlvTblEvent[] = {
#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
    { CORE,         LOS_OFF_SET_OF(TraceEventFrame, core),       sizeof(UINT32) },
#endif
    { EVENT_CODE,   LOS_OFF_SET_OF(TraceEventFrame, eventType),  sizeof(UINT32) },
    { CUR_TIME,     LOS_OFF_SET_OF(TraceEventFrame, curTime),    sizeof(UINT64) },

#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
    { EVENT_COUNT,  LOS_OFF_SET_OF(TraceEventFrame, eventCount), sizeof(UINT32) },
#endif
    { CUR_TASK,     LOS_OFF_SET_OF(TraceEventFrame, curTask),    sizeof(UINT32) },
    { IDENTITY,     LOS_OFF_SET_OF(TraceEventFrame, identity),   sizeof(UINTPTR) },
    { EVENT_PARAMS, LOS_OFF_SET_OF(TraceEventFrame, params),     sizeof(UINTPTR) * LOSCFG_TRACE_FRAME_MAX_PARAMS },
    { TRACE_TLV_TYPE_NULL, 0, 0 },
};

static TlvTable *g_traceTlvTbl[] = {
    g_traceTlvTblNotify,
    g_traceTlvTblHead,
    g_traceTlvTblObj,
    g_traceTlvTblEvent
};

STATIC UINT32 DefaultPipelineInit(VOID)
{
    return LOS_OK;
}

STATIC VOID DefaultDataSend(UINT16 len, UINT8 *data)
{
    (VOID)len;
    (VOID)data;
}

STATIC UINT32 DefaultDataReceive(UINT8 *data, UINT32 size, UINT32 timeout)
{
    (VOID)data;
    (VOID)size;
    (VOID)timeout;
    return LOS_OK;
}

STATIC UINT32 DefaultWait(VOID)
{
    return LOS_OK;
}

STATIC TracePipelineOps g_defaultOps = {
    .init = DefaultPipelineInit,
    .dataSend = DefaultDataSend,
    .dataRecv = DefaultDataReceive,
    .wait = DefaultWait,
};

STATIC const TracePipelineOps *g_tracePipelineOps = &g_defaultOps;

VOID OsTracePipelineReg(const TracePipelineOps *ops)
{
    g_tracePipelineOps = ops;
}

VOID OsTraceDataSend(UINT8 type, UINT16 len, UINT8 *data)
{
    UINT32 intSave;
    UINT8 outBuf[LOSCFG_TRACE_TLV_BUF_SIZE] = {0};

    if ((type > TRACE_MSG_MAX) || (len > LOSCFG_TRACE_TLV_BUF_SIZE)) {
        return;
    }

    len = OsTraceDataEncode(type, g_traceTlvTbl[type], data, &outBuf[0], sizeof(outBuf));

    PIPE_LOCK(intSave);
    g_tracePipelineOps->dataSend(len, &outBuf[0]);
    PIPE_UNLOCK(intSave);
}

UINT32 OsTraceDataRecv(UINT8 *data, UINT32 size, UINT32 timeout)
{
    return g_tracePipelineOps->dataRecv(data, size, timeout);
}

UINT32 OsTraceDataWait(VOID)
{
    return g_tracePipelineOps->wait();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
