/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Offline Mode Implementation
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

#ifdef LOSCFG_RECORDER_MODE_OFFLINE
#define BITS_NUM_FOR_TASK_ID 16

LITE_OS_SEC_BSS STATIC TraceOfflineHeaderInfo g_traceRecoder;
LITE_OS_SEC_BSS STATIC UINT32 g_tidMask[LOSCFG_BASE_CORE_TSK_LIMIT] = {0};

UINT32 OsTraceGetMaskTid(UINT32 tid)
{
    return tid | ((tid < LOSCFG_BASE_CORE_TSK_LIMIT) ? g_tidMask[tid] << BITS_NUM_FOR_TASK_ID : 0); /* tid < 65535 */
}

UINT32 OsTraceBufInit(VOID *buf, UINT32 size)
{
    UINT32 headSize;

    headSize = sizeof(OfflineHead) + sizeof(ObjData) * LOSCFG_TRACE_OBJ_MAX_NUM;
    if (size <= headSize) {
        TRACE_ERROR("trace buf size not enough than 0x%x\n", headSize);
        return LOS_ERRNO_TRACE_BUF_TOO_SMALL;
    }

    if (buf == NULL) {
        buf = LOS_MemAlloc(m_aucSysMem1, size);
        if (buf == NULL) {
            return LOS_ERRNO_TRACE_NO_MEMORY;
        }
    }

    (VOID)memset_s(buf, size, 0, size);
    g_traceRecoder.head = (OfflineHead *)buf;
    g_traceRecoder.head->baseInfo.bigLittleEndian = TRACE_BIGLITTLE_WORD;
    g_traceRecoder.head->baseInfo.version         = TRACE_VERSION(TRACE_MODE_OFFLINE);
    g_traceRecoder.head->baseInfo.clockFreq       = GET_SYS_CLOCK();
    g_traceRecoder.head->objSize                  = sizeof(ObjData);
    g_traceRecoder.head->frameSize                = sizeof(TraceEventFrame);
    g_traceRecoder.head->objOffset                = sizeof(OfflineHead);
    g_traceRecoder.head->frameOffset              = headSize;
    g_traceRecoder.head->totalLen                 = size;

    g_traceRecoder.ctrl.curIndex       = 0;
    g_traceRecoder.ctrl.curObjIndex    = 0;
    g_traceRecoder.ctrl.maxObjCount    = LOSCFG_TRACE_OBJ_MAX_NUM;
    g_traceRecoder.ctrl.maxRecordCount = (size - headSize) / sizeof(TraceEventFrame);
    g_traceRecoder.ctrl.objBuf         = (ObjData *)((UINTPTR)buf + g_traceRecoder.head->objOffset);
    g_traceRecoder.ctrl.frameBuf       = (TraceEventFrame *)((UINTPTR)buf + g_traceRecoder.head->frameOffset);

    return LOS_OK;
}

VOID OsTraceObjAdd(UINT32 eventType, UINT32 taskId)
{
    UINT32 intSave;
    UINT32 index;
    ObjData *obj = NULL;

    TRACE_LOCK(intSave);
    /* add obj begin */
    index = g_traceRecoder.ctrl.curObjIndex;
    if (index >= LOSCFG_TRACE_OBJ_MAX_NUM) { /* do nothing when config LOSCFG_TRACE_OBJ_MAX_NUM = 0 */
        TRACE_UNLOCK(intSave);
        return;
    }
    obj = &g_traceRecoder.ctrl.objBuf[index];

    if (taskId < LOSCFG_BASE_CORE_TSK_LIMIT) {
        g_tidMask[taskId]++;
    }

    OsTraceSetObj(obj, OS_TCB_FROM_TID(taskId));

    g_traceRecoder.ctrl.curObjIndex++;
    if (g_traceRecoder.ctrl.curObjIndex >= g_traceRecoder.ctrl.maxObjCount) {
        g_traceRecoder.ctrl.curObjIndex = 0; /* turn around */
    }
    /* add obj end */
    TRACE_UNLOCK(intSave);
}

VOID OsTraceWriteOrSendEvent(const TraceEventFrame *frame)
{
    UINT16 index;
    UINT32 intSave;

    TRACE_LOCK(intSave);
    index = g_traceRecoder.ctrl.curIndex;
    (VOID)memcpy_s(&g_traceRecoder.ctrl.frameBuf[index], sizeof(TraceEventFrame), frame, sizeof(TraceEventFrame));

    g_traceRecoder.ctrl.curIndex++;
    if (g_traceRecoder.ctrl.curIndex >= g_traceRecoder.ctrl.maxRecordCount) {
        g_traceRecoder.ctrl.curIndex = 0;
    }
    TRACE_UNLOCK(intSave);
}

VOID OsTraceReset(VOID)
{
    UINT32 intSave;
    UINT32 bufLen;

    TRACE_LOCK(intSave);
    bufLen = sizeof(TraceEventFrame) * g_traceRecoder.ctrl.maxRecordCount;
    (VOID)memset_s(g_traceRecoder.ctrl.frameBuf, bufLen, 0, bufLen);
    g_traceRecoder.ctrl.curIndex = 0;
    TRACE_UNLOCK(intSave);
}

STATIC VOID OsTraceInfoObj(VOID)
{
    UINT32 i;
    ObjData *obj = &g_traceRecoder.ctrl.objBuf[0];

    if (g_traceRecoder.ctrl.maxObjCount > 0) {
        PRINTK("CurObjIndex = %u\n", g_traceRecoder.ctrl.curObjIndex);
        PRINTK("Index   TaskID   TaskPrio   TaskName \n");
        for (i = 0; i < g_traceRecoder.ctrl.maxObjCount; i++, obj++) {
            PRINTK("%-7u 0x%-6x %-10u %s\n", i, obj->id, obj->prio, obj->name);
        }
        PRINTK("\n");
    }
}

STATIC VOID OsTraceInfoEventTitle(VOID)
{
    PRINTK("CurEvtIndex = %u\n", g_traceRecoder.ctrl.curIndex);

    PRINTK("Index   Time(cycles)      EventType      CurTask   Identity      ");
#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
    PRINTK("cpuId    hwiActive    taskLockCnt    ");
#endif
#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
    PRINTK("eventCount    ");
#endif
    if (LOSCFG_TRACE_FRAME_MAX_PARAMS > 0) {
        PRINTK("params    ");
    }
    PRINTK("\n");
}

STATIC VOID OsTraceInfoEventData(VOID)
{
    UINT32 i, j;
    TraceEventFrame *frame = &g_traceRecoder.ctrl.frameBuf[0];

    for (i = 0; i < g_traceRecoder.ctrl.maxRecordCount; i++, frame++) {
        PRINTK("%-7u 0x%-15llx 0x%-12x 0x%-7x 0x%-11x ", i, frame->curTime, frame->eventType,
            frame->curTask, frame->identity);
#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
        UINT32 taskLockCnt = frame->core.taskLockCnt;
#ifdef LOSCFG_KERNEL_SMP
        /*
         * For smp systems, TRACE_LOCK will requst taskLock, and this counter
         * will increase by 1 in that case.
         */
        taskLockCnt -= 1;
#endif
        PRINTK("%-11u %-11u %-11u", frame->core.cpuId, frame->core.hwiActive, taskLockCnt);
#endif
#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
        PRINTK("%-11u", frame->eventCount);
#endif
        for (j = 0; j < LOSCFG_TRACE_FRAME_MAX_PARAMS; j++) {
            PRINTK("0x%-11x", frame->params[j]);
        }
        PRINTK("\n");
    }
}

STATIC VOID OsTraceInfoDisplay(VOID)
{
    OfflineHead *head = g_traceRecoder.head;

    PRINTK("*******TraceInfo begin*******\n");
    PRINTK("clockFreq = %u\n", head->baseInfo.clockFreq);

    OsTraceInfoObj();

    OsTraceInfoEventTitle();
    OsTraceInfoEventData();

    PRINTK("*******TraceInfo end*******\n");
}

#ifdef LOSCFG_TRACE_CLIENT_INTERACT
STATIC VOID OsTraceSendInfo(VOID)
{
    UINT32 i;
    ObjData *obj = NULL;
    TraceEventFrame *frame = NULL;

    OsTraceDataSend(HEAD, sizeof(OfflineHead), (UINT8 *)g_traceRecoder.head);

    obj = &g_traceRecoder.ctrl.objBuf[0];
    for (i = 0; i < g_traceRecoder.ctrl.maxObjCount; i++) {
        OsTraceDataSend(OBJ, sizeof(ObjData), (UINT8 *)(obj + i));
    }

    frame = &g_traceRecoder.ctrl.frameBuf[0];
    for (i = 0; i < g_traceRecoder.ctrl.maxRecordCount; i++) {
        OsTraceDataSend(EVENT, sizeof(TraceEventFrame), (UINT8 *)(frame + i));
    }
}
#endif

VOID OsTraceRecordDump(BOOL toClient)
{
    if (!toClient) {
        OsTraceInfoDisplay();
        return;
    }

#ifdef LOSCFG_TRACE_CLIENT_INTERACT
    OsTraceSendInfo();
#endif
}

OfflineHead *OsTraceRecordGet(VOID)
{
    return g_traceRecoder.head;
}

#endif /* LOSCFG_RECORDER_MODE_OFFLINE */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
