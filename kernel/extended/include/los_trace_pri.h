/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: LiteOS Trace Module Private HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-08-30
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

#ifndef _LOS_TRACE_PRI_H
#define _LOS_TRACE_PRI_H

#include "los_trace.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_TRACE_CONTROL_AGENT
#define TRACE_CMD_END_CHAR                  0xD
#endif

#define TRACE_ERROR                         PRINT_ERR
#define TRACE_MODE_OFFLINE                  0
#define TRACE_MODE_ONLINE                   1

/* just task and hwi were traced */
#define TRACE_DEFAULT_MASK                  (TRACE_HWI_FLAG | TRACE_TASK_FLAG)
#define TRACE_CTL_MAGIC_NUM                 0xDEADBEEF
#define TRACE_BIGLITTLE_WORD                0x12345678
#define TRACE_VERSION(MODE)                 (0xFFFFFFFF & (MODE))
#define TRACE_MASK_COMBINE(c1, c2, c3, c4)  (((c1) << 24) | ((c2) << 16) | ((c3) << 8) | (c4))

#define TRACE_GET_MODE_FLAG(type)           ((type) & 0xFFFFFFF0)

extern SPIN_LOCK_S g_traceSpin;
#define TRACE_LOCK(state)                   LOS_SpinLockSave(&g_traceSpin, &(state))
#define TRACE_UNLOCK(state)                 LOS_SpinUnlockRestore(&g_traceSpin, (state))

enum TraceCmd {
    TRACE_CMD_START = 1,
    TRACE_CMD_STOP,
    TRACE_CMD_SET_EVENT_MASK,
    TRACE_CMD_RECODE_DUMP,
    TRACE_CMD_MAX_CODE,
};

/**
 * @ingroup los_trace
 * struct to store the trace cmd from traceClient.
 */
typedef struct {
    UINT8 cmd;
    UINT8 param1;
    UINT8 param2;
    UINT8 param3;
    UINT8 param4;
    UINT8 param5;
    UINT8 end;
} TraceClientCmd;

/**
 * @ingroup los_trace
 * struct to store the event infomation
 */
typedef struct {
    UINT32 cmd;     /* trace start or stop cmd */
    UINT32 param;   /* magic numb stand for notify msg */
} TraceNotifyFrame;

/**
 * @ingroup los_trace
 * struct to store the trace config information.
 */
typedef struct {
    struct WriteCtrl {
        UINT16 curIndex;            /* The current record index */
        UINT16 maxRecordCount;      /* The max num of track items */
        UINT16 curObjIndex;         /* The current obj index */
        UINT16 maxObjCount;         /* The max num of obj index */
        ObjData *objBuf;            /* Pointer to obj info data */
        TraceEventFrame *frameBuf;  /* Pointer to the track items */
    } ctrl;
    OfflineHead *head;
} TraceOfflineHeaderInfo;

extern UINT32 OsTraceGetMaskTid(UINT32 taskId);
extern VOID OsTraceSetObj(ObjData *obj, const LosTaskCB *tcb);
extern VOID OsTraceWriteOrSendEvent(const TraceEventFrame *frame);
extern UINT32 OsTraceBufInit(VOID *buf, UINT32 size);
extern VOID OsTraceObjAdd(UINT32 eventType, UINT32 taskId);
extern BOOL OsTraceIsEnable(VOID);
extern OfflineHead *OsTraceRecordGet(VOID);

#ifdef LOSCFG_RECORDER_MODE_ONLINE
extern VOID OsTraceSendHead(VOID);
extern VOID OsTraceSendObjTable(VOID);
extern VOID OsTraceSendNotify(UINT32 type, UINT32 value);

#define OsTraceNotifyStart() do {                                \
        OsTraceSendNotify(SYS_START, TRACE_CTL_MAGIC_NUM);       \
        OsTraceSendHead();                                       \
        OsTraceSendObjTable();                                   \
    } while (0)

#define OsTraceNotifyStop() do {                                 \
        OsTraceSendNotify(SYS_STOP, TRACE_CTL_MAGIC_NUM);        \
    } while (0)

#define OsTraceReset()
#define OsTraceRecordDump(toClient)
#else
extern VOID OsTraceReset(VOID);
extern VOID OsTraceRecordDump(BOOL toClient);
#define OsTraceNotifyStart()
#define OsTraceNotifyStop()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TRACE_PRI_H */
