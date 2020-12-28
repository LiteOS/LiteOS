/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Mutex Debug
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

#include "los_mux_debug_pri.h"
#include "los_typedef.h"
#include "los_task.h"
#include "los_misc_pri.h"
#include "arch/exception.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef LOSCFG_DEBUG_MUTEX
typedef struct {
    TSK_ENTRY_FUNC creator; /* The task entry who created this mutex */
    UINT64  lastAccessTime; /* The last access time */
} MuxDebugCB;
STATIC MuxDebugCB *g_muxDebugArray = NULL;

STATIC BOOL MuxCompareValue(const SortParam *sortParam, UINT32 left, UINT32 right)
{
    return (*((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, left)) >
            *((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, right)));
}

UINT32 OsMuxDbgInit(VOID)
{
    UINT32 size = LOSCFG_BASE_IPC_MUX_LIMIT * sizeof(MuxDebugCB);
    /* system resident memory, don't free */
    g_muxDebugArray = (MuxDebugCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_muxDebugArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }
    (VOID)memset_s(g_muxDebugArray, size, 0, size);
    return LOS_OK;
}

VOID OsMuxDbgTimeUpdate(UINT32 muxId)
{
    MuxDebugCB *muxDebug = &g_muxDebugArray[GET_MUX_INDEX(muxId)];
    muxDebug->lastAccessTime = LOS_TickCountGet();
}

VOID OsMuxDbgUpdate(UINT32 muxId, TSK_ENTRY_FUNC creator)
{
    MuxDebugCB *muxDebug = &g_muxDebugArray[GET_MUX_INDEX(muxId)];
    muxDebug->creator = creator;
    muxDebug->lastAccessTime = LOS_TickCountGet();
}

STATIC VOID SortMuxIndexArray(UINT32 *indexArray, UINT32 count)
{
    LosMuxCB muxNode = {0};
    MuxDebugCB muxDebugNode = {0};
    UINT32 index, intSave;
    SortParam muxSortParam;
    muxSortParam.buf = (CHAR *)g_muxDebugArray;
    muxSortParam.ctrlBlockSize = sizeof(MuxDebugCB);
    muxSortParam.ctrlBlockCnt = LOSCFG_BASE_IPC_MUX_LIMIT;
    muxSortParam.sortElemOff = OFFSET_OF_FIELD(MuxDebugCB, lastAccessTime);

    if (count > 0) {
        SCHEDULER_LOCK(intSave);
        OsArraySort(indexArray, 0, count - 1, &muxSortParam, MuxCompareValue);
        SCHEDULER_UNLOCK(intSave);
        for (index = 0; index < count; index++) {
            SCHEDULER_LOCK(intSave);
            (VOID)memcpy_s(&muxNode, sizeof(LosMuxCB),
                           GET_MUX(indexArray[index]), sizeof(LosMuxCB));
            (VOID)memcpy_s(&muxDebugNode, sizeof(MuxDebugCB),
                           &g_muxDebugArray[indexArray[index]], sizeof(MuxDebugCB));
            SCHEDULER_UNLOCK(intSave);
            /*
             * muxStat may be altered after the g_taskSpin is unlocked in OsMutexCheck.
             * We should recheck the muxStat before the print.
             */
            if ((muxNode.muxStat != OS_MUX_USED) ||
                ((muxNode.muxStat == OS_MUX_USED) && ((muxDebugNode.creator == NULL) || (muxNode.owner == NULL)))) {
                continue;
            }
            PRINTK("Mutex ID <0x%x> may leak, TaskID of owner:0x%x, TaskEntry of owner: %p,"
                   "TaskEntry of creator: %p,Latest operation time: 0x%llx\n",
                   muxNode.muxId, muxNode.owner->taskId, muxNode.owner->taskEntry, muxDebugNode.creator,
                   muxDebugNode.lastAccessTime);
        }
    }
    (VOID)LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, indexArray);
}

VOID OsMutexCheck(VOID)
{
    LosMuxCB muxNode = {0};
    MuxDebugCB muxDebugNode = {0};
    UINT32 index, intSave;
    UINT32 count = 0;

    /*
     * This return value does not need to be judged immediately,
     * and the following code logic has already distinguished the return value from null and non-empty,
     * and there is no case of accessing the null pointer.
     */
    UINT32 *indexArray = (UINT32 *)LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, LOSCFG_BASE_IPC_MUX_LIMIT * sizeof(UINT32));

    for (index = 0; index < LOSCFG_BASE_IPC_MUX_LIMIT; index++) {
        SCHEDULER_LOCK(intSave);
        (VOID)memcpy_s(&muxNode, sizeof(LosMuxCB), GET_MUX(index), sizeof(LosMuxCB));
        (VOID)memcpy_s(&muxDebugNode, sizeof(MuxDebugCB), &g_muxDebugArray[index], sizeof(MuxDebugCB));
        SCHEDULER_UNLOCK(intSave);

        if ((muxNode.muxStat != OS_MUX_USED) ||
            ((muxNode.muxStat == OS_MUX_USED) && (muxDebugNode.creator == NULL))) {
            continue;
        } else if ((muxNode.muxStat == OS_MUX_USED) && (muxNode.owner == NULL)) {
            PRINTK("Mutex ID <0x%x> may leak, Owner is null, TaskEntry of creator: %p,"
                   "Latest operation time: 0x%llx\n",
                   muxNode.muxId, muxDebugNode.creator, muxDebugNode.lastAccessTime);
        } else {
            if (indexArray != NULL) {
                *(indexArray + count) = index;
                count++;
            } else {
                PRINTK("Mutex ID <0x%x> may leak, TaskID of owner:0x%x, TaskEntry of owner: %p,"
                       "TaskEntry of creator: %p,Latest operation time: 0x%llx\n",
                       muxNode.muxId, muxNode.owner->taskId, muxNode.owner->taskEntry, muxDebugNode.creator,
                       muxDebugNode.lastAccessTime);
            }
        }
    }

    if (indexArray != NULL) {
        SortMuxIndexArray(indexArray, count);
    }
}

#ifdef LOSCFG_SHELL
LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdMuxInfoGet(UINT32 argc, const CHAR **argv)
{
    if (argc > 0) {
        PRINTK("\nUsage: mutex\n");
        return OS_ERROR;
    }
    PRINTK("used mutexs information: \n");
    OsMutexCheck();
    return LOS_OK;
}
SHELLCMD_ENTRY(mutex_shellcmd, CMD_TYPE_EX, "mutex", 0, (CmdCallBackFunc)OsShellCmdMuxInfoGet);
#endif /* LOSCFG_SHELL */
#endif /* LOSCFG_DEBUG_MUTEX */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
