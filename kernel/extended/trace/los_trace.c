/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: LiteOS Trace Implementation
 * Author: Huawei LiteOS Team
 * Create: 2019-08-31
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

#ifdef LOSCFG_KERNEL_SMP
#include "los_mp_pri.h"
#endif

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_TRACE
LITE_OS_SEC_BSS STATIC UINT32 g_traceEventCount;
LITE_OS_SEC_BSS STATIC volatile enum TraceState g_traceState = TRACE_UNINIT;
LITE_OS_SEC_DATA_INIT STATIC volatile BOOL g_enableTrace = FALSE;
LITE_OS_SEC_BSS STATIC UINT32 g_traceMask = TRACE_DEFAULT_MASK;

#ifdef LOSCFG_TRACE_CONTROL_AGENT
LITE_OS_SEC_BSS STATIC UINT32 g_traceTaskId;
#endif

#define MIN(x, y)             ((x) < (y) ? (x) : (y))

LITE_OS_SEC_BSS STATIC TRACE_HWI_FILTER_HOOK g_traceHwiFliterHook = NULL;

LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_traceSpin);

STATIC_INLINE BOOL OsTraceHwiFilter(UINT32 hwiNum)
{
    BOOL ret = ((hwiNum == NUM_HAL_INTERRUPT_UART) || (hwiNum == OS_TICK_INT_NUM));
#ifdef LOSCFG_KERNEL_SMP
    ret |= (hwiNum == LOS_MP_IPI_SCHEDULE);
#endif
    if (g_traceHwiFliterHook != NULL) {
        ret |= g_traceHwiFliterHook(hwiNum);
    }
    return ret;
}

STATIC VOID OsTraceSetFrame(TraceEventFrame *frame, UINT32 eventType, UINTPTR identity, const UINTPTR *params,
    UINT16 paramCount)
{
    INT32 i;
    UINT32 intSave;

    (VOID)memset_s(frame, sizeof(TraceEventFrame), 0, sizeof(TraceEventFrame));

    if (paramCount > LOSCFG_TRACE_FRAME_MAX_PARAMS) {
        paramCount = LOSCFG_TRACE_FRAME_MAX_PARAMS;
    }

    TRACE_LOCK(intSave);
    frame->curTask   = OsTraceGetMaskTid(OsCurrTaskGet()->taskId);
    frame->identity  = identity;
    frame->curTime   = HalClockGetCycles();
    frame->eventType = eventType;

#if (LOSCFG_TRACE_FRAME_CORE_MSG == YES)
    frame->core.cpuId      = ArchCurrCpuid();
    frame->core.hwiActive  = OS_INT_ACTIVE ? TRUE : FALSE;
    frame->core.taskLockCnt = MIN(OsPercpuGet()->taskLockCnt, 0xF); /* taskLockCnt is 4 bits, max vaule = 0xF */
    frame->core.paramCount = paramCount;
#endif

#if (LOSCFG_TRACE_FRAME_EVENT_COUNT == YES)
    frame->eventCount = g_traceEventCount;
    g_traceEventCount++;
#endif
    TRACE_UNLOCK(intSave);

    for (i = 0; i < paramCount; i++) {
        frame->params[i] = params[i];
    }
}

VOID OsTraceSetObj(ObjData *obj, const LosTaskCB *tcb)
{
    errno_t ret;
    (VOID)memset_s(obj, sizeof(ObjData), 0, sizeof(ObjData));

    obj->id   = OsTraceGetMaskTid(tcb->taskId);
    obj->prio = tcb->priority;

    ret = strncpy_s(obj->name, LOSCFG_TRACE_OBJ_MAX_NAME_SIZE, tcb->taskName, LOSCFG_TRACE_OBJ_MAX_NAME_SIZE - 1);
    if (ret != EOK) {
        TRACE_ERROR("Task name copy failed!\n");
    }
}

VOID OsTraceHook(UINT32 eventType, UINTPTR identity, const UINTPTR *params, UINT16 paramCount)
{
    if ((eventType == TASK_CREATE) || (eventType == TASK_PRIOSET)) {
        OsTraceObjAdd(eventType, identity); /* handle important obj info, these can not be filtered */
    }

    if ((g_enableTrace == TRUE) && (eventType & g_traceMask)) {
        UINTPTR id = identity;
        if (TRACE_GET_MODE_FLAG(eventType) == TRACE_HWI_FLAG) {
            if (OsTraceHwiFilter(identity)) {
                return;
            }
        } else if (TRACE_GET_MODE_FLAG(eventType) == TRACE_TASK_FLAG) {
            id = OsTraceGetMaskTid(identity);
        } else if (eventType == MEM_INFO_REQ) {
            LOS_MEM_POOL_STATUS status;
            LOS_MemInfoGet((VOID *)identity, &status);
            LOS_TRACE(MEM_INFO, identity, status.uwTotalUsedSize, status.uwTotalFreeSize);
            return;
        }

        TraceEventFrame frame;
        OsTraceSetFrame(&frame, eventType, id, params, paramCount);

        OsTraceWriteOrSendEvent(&frame);
    }
}

BOOL OsTraceIsEnable(VOID)
{
    return g_enableTrace == TRUE;
}

#ifdef LOSCFG_TRACE_CONTROL_AGENT
STATIC BOOL OsTraceCmdIsValid(const TraceClientCmd *msg)
{
    return ((msg->end == TRACE_CMD_END_CHAR) && (msg->cmd < TRACE_CMD_MAX_CODE));
}

STATIC VOID OsTraceCmdHandle(const TraceClientCmd *msg)
{
    if (!OsTraceCmdIsValid(msg)) {
        return;
    }

    switch (msg->cmd) {
        case TRACE_CMD_START:
            LOS_TraceStart();
            break;
        case TRACE_CMD_STOP:
            LOS_TraceStop();
            break;
        case TRACE_CMD_SET_EVENT_MASK:
            /* 4 params(UINT8) composition the mask(UINT32) */
            LOS_TraceEventMaskSet(TRACE_MASK_COMBINE(msg->param1, msg->param2, msg->param3, msg->param4));
            break;
        case TRACE_CMD_RECODE_DUMP:
            LOS_TraceRecordDump(TRUE);
            break;
        default:
            break;
    }
}

VOID TraceAgent(VOID)
{
    UINT32 ret;
    TraceClientCmd msg;

    while (1) {
        (VOID)memset_s(&msg, sizeof(TraceClientCmd), 0, sizeof(TraceClientCmd));
        ret = OsTraceDataWait();
        if (ret == LOS_OK) {
            OsTraceDataRecv((UINT8 *)&msg, sizeof(TraceClientCmd), 0);
            OsTraceCmdHandle(&msg);
        }
    }
}

STATIC UINT32 OsCreateTraceAgentTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    (VOID)memset_s((VOID *)(&taskInitParam), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TraceAgent;
    taskInitParam.usTaskPrio = LOSCFG_TRACE_TASK_PRIORITY;
    taskInitParam.pcName = "TraceAgent";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_traceTaskId, &taskInitParam);
    return ret;
}
#endif

UINT32 LOS_TraceInit(VOID *buf, UINT32 size)
{
    UINT32 intSave;
    UINT32 ret;

    TRACE_LOCK(intSave);
    if (g_traceState != TRACE_UNINIT) {
        TRACE_ERROR("trace has been initialized already, the current state is :%d\n", g_traceState);
        ret = LOS_ERRNO_TRACE_ERROR_STATUS;
        goto LOS_ERREND;
    }

#ifdef LOSCFG_TRACE_CLIENT_INTERACT
    ret = OsTracePipelineInit();
    if (ret != LOS_OK) {
        goto LOS_ERREND;
    }
#endif

#ifdef LOSCFG_TRACE_CONTROL_AGENT
    ret = OsCreateTraceAgentTask();
    if (ret != LOS_OK) {
        TRACE_ERROR("trace init create agentTask error :0x%x\n", ret);
        goto LOS_ERREND;
    }
#endif

    ret = OsTraceBufInit(buf, size);
    if (ret != LOS_OK) {
        goto LOS_RELEASE;
    }

    g_traceEventCount = 0;

#ifdef LOSCFG_RECORDER_MODE_ONLINE  /* Wait trace client to start trace */
    g_enableTrace = FALSE;
    g_traceState = TRACE_INITED;
#else
    g_enableTrace = TRUE;
    g_traceState = TRACE_STARTED;
#endif
    TRACE_UNLOCK(intSave);
    return LOS_OK;
LOS_RELEASE:
#ifdef LOSCFG_TRACE_CONTROL_AGENT
    LOS_TaskDelete(g_traceTaskId);
#endif
LOS_ERREND:
    TRACE_UNLOCK(intSave);
    return ret;
}

UINT32 LOS_TraceStart(VOID)
{
    UINT32 intSave;
    UINT32 ret = LOS_OK;

    TRACE_LOCK(intSave);
    if (g_traceState == TRACE_STARTED) {
        goto START_END;
    }

    if (g_traceState == TRACE_UNINIT) {
        TRACE_ERROR("trace not inited, be sure LOS_TraceInit excute success\n");
        ret = LOS_ERRNO_TRACE_ERROR_STATUS;
        goto START_END;
    }

    OsTraceNotifyStart();

    g_enableTrace = TRUE;
    g_traceState = TRACE_STARTED;

    TRACE_UNLOCK(intSave);
    LOS_TRACE(MEM_INFO_REQ, m_aucSysMem0);
    return ret;
START_END:
    TRACE_UNLOCK(intSave);
    return ret;
}

VOID LOS_TraceStop(VOID)
{
    UINT32 intSave;

    TRACE_LOCK(intSave);
    if (g_traceState != TRACE_STARTED) {
        goto STOP_END;
    }

    g_enableTrace = FALSE;
    g_traceState = TRACE_STOPED;
    OsTraceNotifyStop();
STOP_END:
    TRACE_UNLOCK(intSave);
}

VOID LOS_TraceEventMaskSet(UINT32 mask)
{
    g_traceMask = mask;
}

VOID LOS_TraceRecordDump(BOOL toClient)
{
    if (g_traceState != TRACE_STOPED) {
        TRACE_ERROR("trace dump must after trace stopped , the current state is : %d\n", g_traceState);
        return;
    }
    OsTraceRecordDump(toClient);
}

OfflineHead *LOS_TraceRecordGet(VOID)
{
    return OsTraceRecordGet();
}

VOID LOS_TraceReset(VOID)
{
    if (g_traceState == TRACE_UNINIT) {
        TRACE_ERROR("trace not inited, be sure LOS_TraceInit excute success\n");
        return;
    }

    OsTraceReset();
}

VOID LOS_TraceHwiFilterHookReg(TRACE_HWI_FILTER_HOOK hook)
{
    UINT32 intSave;

    TRACE_LOCK(intSave);
    g_traceHwiFliterHook = hook;
    TRACE_UNLOCK(intSave);
}

#ifdef LOSCFG_SHELL
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdTraceSetMask(INT32 argc, const CHAR **argv)
{
    size_t mask;
    CHAR *endPtr = NULL;

    if (argc >= 2) { /* 2:Just as number of parameters */
        PRINTK("\nUsage: trace_mask or trace_mask ID\n");
        return OS_ERROR;
    }

    if (argc == 0) {
        mask = TRACE_DEFAULT_MASK;
    } else {
        mask = strtoul(argv[0], &endPtr, 0);
    }
    LOS_TraceEventMaskSet((UINT32)mask);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdTraceDump(INT32 argc, const CHAR **argv)
{
    BOOL toClient;
    CHAR *endPtr = NULL;

    if (argc >= 2) { /* 2:Just as number of parameters */
        PRINTK("\nUsage: trace_dump or trace_dump [1/0]\n");
        return OS_ERROR;
    }

    if (argc == 0) {
        toClient = FALSE;
    } else {
        toClient = strtoul(argv[0], &endPtr, 0) != 0 ? TRUE : FALSE;
    }
    LOS_TraceRecordDump(toClient);
    return LOS_OK;
}

SHELLCMD_ENTRY(tracestart_shellcmd,   CMD_TYPE_EX, "trace_start", 0, (CmdCallBackFunc)LOS_TraceStart);
SHELLCMD_ENTRY(tracestop_shellcmd,    CMD_TYPE_EX, "trace_stop",  0, (CmdCallBackFunc)LOS_TraceStop);
SHELLCMD_ENTRY(tracesetmask_shellcmd, CMD_TYPE_EX, "trace_mask",  1, (CmdCallBackFunc)OsShellCmdTraceSetMask);
SHELLCMD_ENTRY(tracereset_shellcmd,   CMD_TYPE_EX, "trace_reset", 0, (CmdCallBackFunc)LOS_TraceReset);
SHELLCMD_ENTRY(tracedump_shellcmd,    CMD_TYPE_EX, "trace_dump", 1, (CmdCallBackFunc)OsShellCmdTraceDump);
#endif

#endif /* LOSCFG_KERNEL_TRACE */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
