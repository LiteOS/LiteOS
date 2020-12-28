/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 * Description: LiteOS memory Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-05-12
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

#include "string.h"
#include "securec.h"

#include "los_hwi.h"
#include "los_typedef.h"
#include "los_memory_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* spinlock for mem module, only available on SMP mode */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_memboxSpin);
#define MEMBOX_LOCK(state)       LOS_SpinLockSave(&g_memboxSpin, &(state))
#define MEMBOX_UNLOCK(state)     LOS_SpinUnlockRestore(&g_memboxSpin, (state))

UINT32 LOS_MemboxInit(VOID *pool, UINT32 poolSize, UINT32 blkSize)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    /* Initialize memory block system, returns 0 if OK, 1 if fails. */
    if (blkSize == 0) {
        return LOS_NOK;
    }

    /* Create a Memory structure. */
    MEMBOX_LOCK(intSave);
    boxInfo->uwBlkSize = blkSize;
    boxInfo->uwBlkNum = poolSize / blkSize;
    boxInfo->uwBlkCnt = 0;
    if (boxInfo->uwBlkNum == 0) {
        MEMBOX_UNLOCK(intSave);
        return LOS_NOK;
    }
    MEMBOX_UNLOCK(intSave);

    return LOS_OK;
}

VOID *LOS_MemboxAlloc(VOID *pool)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    VOID *ret = NULL;
    UINT32 intSave;

    if (pool == NULL) {
        return NULL;
    }

    MEMBOX_LOCK(intSave);
    if (boxInfo->uwBlkCnt < boxInfo->uwBlkNum) {
        ret = LOS_MemAlloc(OS_SYS_MEM_ADDR, boxInfo->uwBlkSize);
        if (ret != NULL) {
            boxInfo->uwBlkCnt++;
        }
    }
    MEMBOX_UNLOCK(intSave);
    return ret;
}

UINT32 LOS_MemboxFree(VOID *pool, VOID *box)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    UINT32 freeRes;
    UINT32 intSave;

    if ((pool == NULL) || (box == NULL)) {
        return LOS_NOK;
    }

    freeRes = LOS_MemFree(OS_SYS_MEM_ADDR, box);
    if (freeRes == LOS_OK) {
        MEMBOX_LOCK(intSave);
        if (boxInfo->uwBlkCnt) {
            boxInfo->uwBlkCnt--;
        }
        MEMBOX_UNLOCK(intSave);
    }
    return freeRes;
}

VOID LOS_MemboxClr(VOID *pool, VOID *box)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    UINT32 intSave;

    if ((pool == NULL) || (box == NULL)) {
        PRINT_WARN("LOS_MemboxClr : invalid parameter!\n");
        return;
    }

    MEMBOX_LOCK(intSave);
    (VOID)memset_s(box, boxInfo->uwBlkSize, 0, boxInfo->uwBlkSize);
    MEMBOX_UNLOCK(intSave);
}

VOID LOS_ShowBox(VOID *pool)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;

    if (pool == NULL) {
        return;
    }

    PRINTK("membox(%p,0x%x,0x%x):\r\n", pool, boxInfo->uwBlkSize, boxInfo->uwBlkNum);
}

UINT32 LOS_MemboxStatisticsGet(const VOID *boxMem, UINT32 *maxBlk, UINT32 *blkCnt, UINT32 *blkSize)
{
    if ((boxMem == NULL) || (maxBlk == NULL) || (blkCnt == NULL) || (blkSize == NULL)) {
        return LOS_NOK;
    }

    *maxBlk = ((OS_MEMBOX_S *)boxMem)->uwBlkNum;
    *blkCnt = ((OS_MEMBOX_S *)boxMem)->uwBlkCnt;
    *blkSize = ((OS_MEMBOX_S *)boxMem)->uwBlkSize;

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
