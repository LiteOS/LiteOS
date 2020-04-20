/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Task Mem Implementation
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "los_memstat_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MIN_TASK_ID(x, y)               ((x) > (y) ? (y) : (x))
#define MAX_MEM_USE(x, y)               ((x) > (y) ? (x) : (y))

#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
LITE_OS_SEC_TEXT_MINOR VOID OsTaskMemUsedInc(TaskMemUsedInfo *memStats, UINT32 usedSize, UINT32 taskID)
{
    UINT32 record = MIN_TASK_ID(taskID, TASK_BLOCK_NUM - 1);
    memStats[record].memUsed += usedSize;
    memStats[record].memPeak = MAX_MEM_USE(memStats[record].memPeak, memStats[record].memUsed);
}

LITE_OS_SEC_TEXT_MINOR VOID OsTaskMemUsedDec(TaskMemUsedInfo *memStats, UINT32 usedSize, UINT32 taskID)
{
    UINT32 record = MIN_TASK_ID(taskID, TASK_BLOCK_NUM - 1);

    if (memStats[record].memUsed < usedSize) {
        PRINT_INFO("mem used of current task '%s':0x%x, decrease size:0x%x\n",
                   OsCurrTaskGet()->taskName, memStats[record].memUsed, usedSize);
        return;
    }

    memStats[record].memUsed -= usedSize;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsTaskMemUsage(const TaskMemUsedInfo *memStats, UINT32 taskID)
{
    UINT32 record = MIN_TASK_ID(taskID, TASK_BLOCK_NUM - 1);
    return memStats[record].memUsed;
}

LITE_OS_SEC_TEXT_MINOR VOID OsTaskMemClear(UINT32 taskID)
{
    UINT32 record = MIN_TASK_ID(taskID, TASK_BLOCK_NUM - 1);
#if (LOSCFG_MEM_MUL_POOL == YES)
    TaskMemUsedInfo *memStats = NULL;
    LosMemPoolInfo *pool = (LosMemPoolInfo *)g_poolHead;
    while (pool != NULL) {
        memStats = (TaskMemUsedInfo *)(((LosMemPoolInfo *)pool)->memStats);
        if (memStats[taskID].memUsed != 0) {
            if (OS_TSK_GET_INDEX(taskID) < g_taskMaxNum) {
                PRINT_INFO("mem used of task '%s' is:0x%x, not zero when task being deleted\n",
                    OS_TCB_FROM_TID(taskID)->taskName, memStats[record].memUsed);
            }
        }
        memStats[record].memUsed = 0;
        memStats[record].memPeak = 0;
        pool = pool->nextPool;
    }
#else
    TaskMemUsedInfo *memStats = (TaskMemUsedInfo *)(((LosMemPoolInfo *)m_aucSysMem1)->memStats);
    if (memStats[taskID].memUsed != 0) {
        if (OS_TSK_GET_INDEX(taskID) < g_taskMaxNum) {
            PRINT_INFO("mem used of task '%s' is:0x%x, not zero when task being deleted\n",
                OS_TCB_FROM_TID(taskID)->taskName, memStats[record].memUsed);
        }
    }
    memStats[record].memUsed = 0;
    memStats[record].memPeak = 0;
#endif
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
