/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Pipeline Implementation HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-03-16
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

#ifndef _TRACE_PIPELINE_H
#define _TRACE_PIPELINE_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    UINT32 (*init)(VOID);
    VOID (*dataSend)(UINT16 len, UINT8 *data);
    UINT32 (*dataRecv)(UINT8 *data, UINT32 size, UINT32 timeout);
    UINT32 (*wait)(VOID);
} TracePipelineOps;

/* used as tlv's tag */
enum TraceMsgType {
    NOTIFY,
    HEAD,
    OBJ,
    EVENT,
    TRACE_MSG_MAX,
};

enum TraceNotifySubType {
    CMD = 0x1,
    PARAMS,
};

enum TraceHeadSubType {
    ENDIAN = 0x1,
    VERSION,
    OBJ_SIZE,
    OBJ_COUNT,
    CUR_INDEX,
    MAX_RECODE,
    CUR_OBJ_INDEX,
    CLOCK_FREQ,
};

enum TraceObjSubType {
    ADDR = 0x1,
    PRIO,
    NAME,
};

enum TraceEvtSubType {
    CORE = 0x1,
    EVENT_CODE,
    CUR_TIME,
    EVENT_COUNT,
    CUR_TASK,
    IDENTITY,
    EVENT_PARAMS,
};

extern VOID OsTracePipelineReg(const TracePipelineOps *ops);
extern UINT32 OsTracePipelineInit(VOID);

extern VOID OsTraceDataSend(UINT8 type, UINT16 len, UINT8 *data);
extern UINT32 OsTraceDataRecv(UINT8 *data, UINT32 size, UINT32 timeout);
extern UINT32 OsTraceDataWait(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _TRACE_PIPELINE_H */
