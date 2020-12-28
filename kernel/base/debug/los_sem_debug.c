/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Sem Debug
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

#include "los_sem_debug_pri.h"
#include "stdlib.h"
#include "los_typedef.h"
#include "los_task_pri.h"
#include "los_misc_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef LOSCFG_DEBUG_SEMAPHORE
#define OS_ALL_SEM_MASK 0xffffffff
STATIC VOID OsSemPendedTaskNamePrint(LosSemCB *semNode)
{
    LosTaskCB *tskCB = NULL;
    CHAR *nameArr[LOSCFG_BASE_CORE_TSK_LIMIT] = {0};
    UINT32 i, intSave;
    UINT32 num = 0;

    SCHEDULER_LOCK(intSave);
    if ((semNode->semStat == OS_SEM_UNUSED) || (LOS_ListEmpty(&semNode->semList))) {
        SCHEDULER_UNLOCK(intSave);
        return;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(tskCB, &semNode->semList, LosTaskCB, pendList) {
        nameArr[num++] = tskCB->taskName;
        if (num == LOSCFG_BASE_CORE_TSK_LIMIT) {
            break;
        }
    }
    SCHEDULER_UNLOCK(intSave);

    PRINTK("Pended task list : ");
    for (i = 0; i < num; i++) {
        if (i == 0) {
            PRINTK("%s\n", nameArr[i]);
        } else {
            PRINTK(", %s", nameArr[i]);
        }
    }
    PRINTK("\n");
}

typedef struct {
    UINT16  origSemCount;   /* Number of original available semaphores */
    UINT64  lastAccessTime; /* The last operation time */
    TSK_ENTRY_FUNC creator; /* The task entry who created this sem */
} SemDebugCB;
STATIC SemDebugCB *g_semDebugArray = NULL;

STATIC BOOL SemCompareValue(const SortParam *sortParam, UINT32 left, UINT32 right)
{
    return (*((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, left)) >
            *((UINT64 *)(VOID *)SORT_ELEM_ADDR(sortParam, right)));
}

UINT32 OsSemDbgInit(VOID)
{
    UINT32 size = LOSCFG_BASE_IPC_SEM_LIMIT * sizeof(SemDebugCB);
    /* system resident memory, don't free */
    g_semDebugArray = (SemDebugCB *)LOS_MemAlloc(m_aucSysMem1, size);
    if (g_semDebugArray == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return LOS_NOK;
    }
    (VOID)memset_s(g_semDebugArray, size, 0, size);
    return LOS_OK;
}

VOID OsSemDbgTimeUpdate(UINT32 semId)
{
    SemDebugCB *semDebug = &g_semDebugArray[GET_SEM_INDEX(semId)];
    semDebug->lastAccessTime = LOS_TickCountGet();
    return;
}

VOID OsSemDbgUpdate(UINT32 semId, TSK_ENTRY_FUNC creator, UINT16 count)
{
    SemDebugCB *semDebug = &g_semDebugArray[GET_SEM_INDEX(semId)];
    semDebug->creator = creator;
    semDebug->lastAccessTime = LOS_TickCountGet();
    semDebug->origSemCount = count;
    return;
}

STATIC VOID OsSemSort(UINT32 *semIndexArray, UINT32 usedCount)
{
    UINT32 i, intSave;
    LosSemCB *semCB = NULL;
    LosSemCB semNode = {0};
    SemDebugCB semDebug = {0};
    SortParam semSortParam;
    semSortParam.buf = (CHAR *)g_semDebugArray;
    semSortParam.ctrlBlockSize = sizeof(SemDebugCB);
    semSortParam.ctrlBlockCnt = LOSCFG_BASE_IPC_SEM_LIMIT;
    semSortParam.sortElemOff = OFFSET_OF_FIELD(SemDebugCB, lastAccessTime);

    /* It will Print out ALL the Used Semaphore List. */
    PRINTK("Used Semaphore List: \n");
    PRINTK("\r\n   SemID    Count    OriginalCount   Creater(TaskEntry)    LastAccessTime\n");
    PRINTK("   ------   ------   -------------   ------------------    --------------   \n");

    SCHEDULER_LOCK(intSave);
    OsArraySort(semIndexArray, 0, usedCount - 1, &semSortParam, SemCompareValue);
    SCHEDULER_UNLOCK(intSave);
    for (i = 0; i < usedCount; i++) {
        semCB = GET_SEM(semIndexArray[i]);
        SCHEDULER_LOCK(intSave);
        (VOID)memcpy_s(&semNode, sizeof(LosSemCB), semCB, sizeof(LosSemCB));
        (VOID)memcpy_s(&semDebug, sizeof(SemDebugCB), &g_semDebugArray[semIndexArray[i]], sizeof(SemDebugCB));
        SCHEDULER_UNLOCK(intSave);
        if ((semNode.semStat != OS_SEM_USED) || (semDebug.creator == NULL)) {
            continue;
        }
        PRINTK("   0x%-07x0x%-07u0x%-14u%-22p0x%llx\n", semNode.semId, semDebug.origSemCount,
               semNode.semCount, semDebug.creator, semDebug.lastAccessTime);
        if (!LOS_ListEmpty(&semNode.semList)) {
            OsSemPendedTaskNamePrint(semCB);
        }
    }
}

UINT32 OsSemInfoGetFullData(VOID)
{
    UINT32 usedSemCnt = 0;
    LosSemCB *semNode = NULL;
    SemDebugCB *semDebug = NULL;
    UINT32 i;
    UINT32 *semIndexArray = NULL;
    UINT32 count, intSave;

    SCHEDULER_LOCK(intSave);
    /* Get the used semaphore count. */
    for (i = 0; i < LOSCFG_BASE_IPC_SEM_LIMIT; i++) {
        semNode = GET_SEM(i);
        semDebug = &g_semDebugArray[i];
        if ((semNode->semStat == OS_SEM_USED) && (semDebug->creator != NULL)) {
            usedSemCnt++;
        }
    }
    SCHEDULER_UNLOCK(intSave);

    if (usedSemCnt > 0) {
        semIndexArray = (UINT32 *)LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, usedSemCnt * sizeof(UINT32));
        if (semIndexArray == NULL) {
            PRINTK("LOS_MemAlloc failed in %s \n", __func__);
            return LOS_NOK;
        }

        /* Fill the semIndexArray with the real index. */
        count = 0;

        SCHEDULER_LOCK(intSave);
        for (i = 0; i < LOSCFG_BASE_IPC_SEM_LIMIT; i++) {
            semNode = GET_SEM(i);
            semDebug = &g_semDebugArray[i];
            if ((semNode->semStat != OS_SEM_USED) || (semDebug->creator == NULL)) {
                continue;
            }
            *(semIndexArray + count) = i;
            count++;
            /* if the count is touched usedSemCnt break. */
            if (count >= usedSemCnt) {
                break;
            }
        }
        SCHEDULER_UNLOCK(intSave);
        OsSemSort(semIndexArray, count);

        /* free the index array. */
        (VOID)LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, semIndexArray);
    }
    return LOS_OK;
}

#ifdef LOSCFG_SHELL
STATIC UINT32 OsSemInfoOutput(size_t semId)
{
    UINT32 loop, semCnt, intSave;
    LosSemCB *semCB = NULL;
    LosSemCB semNode = {0};

    if (semId == OS_ALL_SEM_MASK) {
        for (loop = 0, semCnt = 0; loop < LOSCFG_BASE_IPC_SEM_LIMIT; loop++) {
            semCB = GET_SEM(loop);
            SCHEDULER_LOCK(intSave);
            if (semCB->semStat == OS_SEM_USED) {
                (VOID)memcpy_s(&semNode, sizeof(LosSemCB), semCB, sizeof(LosSemCB));
                SCHEDULER_UNLOCK(intSave);
                semCnt++;
                PRINTK("\r\n   SemID       Count\n   ----------  -----\n");
                PRINTK("   0x%08x  %u\n", semNode.semId, semNode.semCount);
                continue;
            }
            SCHEDULER_UNLOCK(intSave);
        }
        PRINTK("   SemUsingNum    :  %u\n\n", semCnt);
        return LOS_OK;
    } else {
        if (GET_SEM_INDEX(semId) >= LOSCFG_BASE_IPC_SEM_LIMIT) {
            PRINTK("\nInvalid semaphore id!\n");
            return OS_ERROR;
        }

        semCB = GET_SEM(semId);
        SCHEDULER_LOCK(intSave);
        (VOID)memcpy_s(&semNode, sizeof(LosSemCB), semCB, sizeof(LosSemCB));
        SCHEDULER_UNLOCK(intSave);
        if ((semNode.semId != semId) || (semNode.semStat != OS_SEM_USED)) {
            PRINTK("\nThe semaphore is not in use!\n");
            return LOS_OK;
        }

        PRINTK("\r\n   SemID       Count\n   ----------  -----\n");
        PRINTK("   0x%08x      0x%u\n", semNode.semId, semNode.semCount);

        if (LOS_ListEmpty(&semNode.semList)) {
            PRINTK("No task is pended on this semaphore!\n");
            return LOS_OK;
        } else {
            OsSemPendedTaskNamePrint(semCB);
        }
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsShellCmdSemInfoGet(UINT32 argc, const CHAR **argv)
{
    size_t semId;
    CHAR *endPtr = NULL;
    UINT32 ret;

    if (argc > 1) {
        PRINTK("\nUsage: sem [fulldata|ID]\n");
        return OS_ERROR;
    }

    if (argc == 0) {
        semId = OS_ALL_SEM_MASK;
    } else {
        if (strcmp(argv[0], "fulldata") == 0) {
            ret = OsSemInfoGetFullData();
            return ret;
        }

        semId = strtoul(argv[0], &endPtr, 0);
        if ((endPtr == NULL) || (*endPtr != 0)) {
            PRINTK("\nsem ID can't access %s.\n", argv[0]);
            return OS_ERROR;
        }
    }

    ret = OsSemInfoOutput(semId);
    return ret;
}

SHELLCMD_ENTRY(sem_shellcmd, CMD_TYPE_EX, "sem", 1, (CmdCallBackFunc)OsShellCmdSemInfoGet);
#endif /* LOSCFG_SHELL */
#endif /* LOSCFG_DEBUG_SEMAPHORE */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
