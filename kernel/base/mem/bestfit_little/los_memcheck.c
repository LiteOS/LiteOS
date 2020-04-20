/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 * Description: LiteOS memory Module Implementation
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

#include "los_memcheck_pri.h"
#include "securec.h"
#include "los_memory_pri.h"
#include "los_membox_pri.h"
#include "los_heap_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if (LOSCFG_PLATFORM_EXC == YES)
UINT8 g_memInfoMgr[MEM_INFO_SIZE];

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemExcInfoGet(UINT32 memNum, MEM_INFO_S *memExcInfo)
{
    UINT32 maxBlk = 0;
    UINT32 blkCnt = 0;
    UINT32 blkSize = 0;
    LOS_MEM_POOL_STATUS status;
    MEM_INFO *memInfo = NULL;

    if ((memNum >= OS_SYS_MEM_NUM) || (memExcInfo == NULL)) {
        return LOS_NOK;
    }

    (VOID)memset_s(memExcInfo, sizeof(MEM_INFO_S), 0, sizeof(MEM_INFO_S));
    memInfo = (MEM_INFO *)(VOID *)g_memInfoMgr + memNum;
    memExcInfo->uwType = memInfo->uwType;
    memExcInfo->uwStartAddr = memInfo->uwStartAddr;
    memExcInfo->uwSize = memInfo->uwSize;

    if (memInfo->uwType == MEM_MANG_MEMBOX) {
        (VOID)LOS_MemboxStatisticsGet((VOID *)(UINTPTR)(memInfo->uwStartAddr), &maxBlk, &blkCnt, &blkSize);
        memExcInfo->uwBlockSize = blkSize;
        memExcInfo->uwSize = maxBlk; /* Block num */
        memExcInfo->uwFree = maxBlk - blkCnt;
    } else if (memInfo->uwType == MEM_MANG_MEMORY) {
        (VOID)LOS_MemInfoGet((VOID *)(UINTPTR)(memInfo->uwStartAddr), &status);

        memExcInfo->uwSize = status.uwTotalUsedSize;
        memExcInfo->uwFree = status.uwTotalFreeSize;
    } else {
        PRINT_ERR("%s:the type of 0x%x is MEM_MANG_EMPTY !\n", __func__, memInfo->uwStartAddr);
    }

    return LOS_OK;
}

VOID OsMemInfoUpdate(VOID *pool, UINT32 size, UINT32 type)
{
    MEM_INFO *memInfo = (MEM_INFO *)(VOID *)g_memInfoMgr;
    UINTPTR intSave;
    UINT8 loop;

    intSave = LOS_IntLock();
    for (loop = 0; loop < OS_SYS_MEM_NUM; loop++) {
        if (type == MEM_MANG_EMPTY) {
            if (memInfo->uwStartAddr == (UINTPTR)pool) {
                memInfo->uwType = MEM_MANG_EMPTY;
                LOS_IntRestore(intSave);
                return;
            }
        } else if (memInfo->uwStartAddr == (UINTPTR)pool) {
            break;
        } else if (memInfo->uwType == MEM_MANG_EMPTY) {
            break;
        }
        memInfo++;
    }
    if ((loop < OS_SYS_MEM_NUM) && (type != MEM_MANG_EMPTY)) {
        memInfo->uwType = type;
        memInfo->uwStartAddr = (UINTPTR)pool;
        memInfo->uwSize = size;
    } else {
        PRINT_ERR("MemInfo recorded up to max num or the input pool to empty not recorded before\n");
    }

    LOS_IntRestore(intSave);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */