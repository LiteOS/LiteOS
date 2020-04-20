/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
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

#include "los_membox.h"
#include "los_hwi.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_AARCH64
#define OS_MEMBOX_MAGIC 0xa55a5aa5a55a5aa5
#else
#define OS_MEMBOX_MAGIC 0xa55a5aa5
#endif
#define OS_MEMBOX_SET_MAGIC(addr) \
    ((LOS_MEMBOX_NODE *)(addr))->pstNext = (LOS_MEMBOX_NODE *)OS_MEMBOX_MAGIC
#define OS_MEMBOX_CHECK_MAGIC(addr) \
    ((((LOS_MEMBOX_NODE *)(addr))->pstNext == (LOS_MEMBOX_NODE *)OS_MEMBOX_MAGIC) ? LOS_OK : LOS_NOK)

#define OS_MEMBOX_USER_ADDR(addr) \
    ((VOID *)((UINT8 *)(addr) + OS_MEMBOX_NODE_HEAD_SIZE))
#define OS_MEMBOX_NODE_ADDR(addr) \
    ((LOS_MEMBOX_NODE *)(VOID *)((UINT8 *)(addr) - OS_MEMBOX_NODE_HEAD_SIZE))
/* spinlock for mem module */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_memboxSpin);
#define MEMBOX_LOCK(state)       LOS_SpinLockSave(&g_memboxSpin, &(state))
#define MEMBOX_UNLOCK(state)     LOS_SpinUnlockRestore(&g_memboxSpin, (state))

STATIC INLINE UINT32 OsCheckBoxMem(const LOS_MEMBOX_INFO *boxInfo, const VOID *node)
{
    UINT32 offset;

    if (boxInfo->uwBlkSize == 0) {
        return LOS_NOK;
    }

    offset = (UINTPTR)node - (UINTPTR)(boxInfo + 1);
    if ((offset % boxInfo->uwBlkSize) != 0) {
        return LOS_NOK;
    }

    if ((offset / boxInfo->uwBlkSize) >= boxInfo->uwBlkNum) {
        return LOS_NOK;
    }

    return OS_MEMBOX_CHECK_MAGIC(node);
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemboxInit(VOID *pool, UINT32 poolSize, UINT32 blkSize)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    LOS_MEMBOX_NODE *node = NULL;
    UINT32 index;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    if (blkSize == 0) {
        return LOS_NOK;
    }

    if (poolSize < sizeof(LOS_MEMBOX_INFO)) {
        return LOS_NOK;
    }

    MEMBOX_LOCK(intSave);
    boxInfo->uwBlkSize = LOS_MEMBOX_ALLIGNED(blkSize + OS_MEMBOX_NODE_HEAD_SIZE);
    boxInfo->uwBlkNum = (poolSize - sizeof(LOS_MEMBOX_INFO)) / boxInfo->uwBlkSize;
    boxInfo->uwBlkCnt = 0;
    if (boxInfo->uwBlkNum == 0) {
        MEMBOX_UNLOCK(intSave);
        return LOS_NOK;
    }

    node = (LOS_MEMBOX_NODE *)(boxInfo + 1);

    boxInfo->stFreeList.pstNext = node;

    for (index = 0; index < boxInfo->uwBlkNum - 1; ++index) {
        node->pstNext = OS_MEMBOX_NEXT(node, boxInfo->uwBlkSize);
        node = node->pstNext;
    }

    node->pstNext = NULL;

    MEMBOX_UNLOCK(intSave);

    return LOS_OK;
}

LITE_OS_SEC_TEXT VOID *LOS_MemboxAlloc(VOID *pool)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    LOS_MEMBOX_NODE *node = NULL;
    LOS_MEMBOX_NODE *nodeTmp = NULL;
    UINT32 intSave;

    if (pool == NULL) {
        return NULL;
    }

    MEMBOX_LOCK(intSave);
    node = &(boxInfo->stFreeList);
    if (node->pstNext != NULL) {
        nodeTmp = node->pstNext;
        node->pstNext = nodeTmp->pstNext;
        OS_MEMBOX_SET_MAGIC(nodeTmp);
        boxInfo->uwBlkCnt++;
    }
    MEMBOX_UNLOCK(intSave);

    return (nodeTmp == NULL) ? NULL : OS_MEMBOX_USER_ADDR(nodeTmp);
}

LITE_OS_SEC_TEXT UINT32 LOS_MemboxFree(VOID *pool, VOID *box)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    UINT32 ret = LOS_NOK;
    UINT32 intSave;

    if ((pool == NULL) || (box == NULL)) {
        return LOS_NOK;
    }

    MEMBOX_LOCK(intSave);
    do {
        LOS_MEMBOX_NODE *node = OS_MEMBOX_NODE_ADDR(box);
        if (OsCheckBoxMem(boxInfo, node) != LOS_OK) {
            break;
        }

        node->pstNext = boxInfo->stFreeList.pstNext;
        boxInfo->stFreeList.pstNext = node;
        boxInfo->uwBlkCnt--;
        ret = LOS_OK;
    } while (0);
    MEMBOX_UNLOCK(intSave);

    return ret;
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_MemboxClr(VOID *pool, VOID *box)
{
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;

    if ((pool == NULL) || (box == NULL)) {
        return;
    }

    (VOID)memset_s(box, (boxInfo->uwBlkSize - OS_MEMBOX_NODE_HEAD_SIZE), 0,
        (boxInfo->uwBlkSize - OS_MEMBOX_NODE_HEAD_SIZE));
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_ShowBox(VOID *pool)
{
    UINT32 index;
    UINT32 intSave;
    LOS_MEMBOX_INFO *boxInfo = (LOS_MEMBOX_INFO *)pool;
    LOS_MEMBOX_NODE *node = NULL;

    if (pool == NULL) {
        return;
    }
    MEMBOX_LOCK(intSave);
    PRINT_INFO("membox(%p,0x%x,0x%x):\r\n", pool, boxInfo->uwBlkSize, boxInfo->uwBlkNum);
    PRINT_INFO("free node list:\r\n");

    for (node = boxInfo->stFreeList.pstNext, index = 0; node != NULL;
         node = node->pstNext, ++index) {
        PRINT_INFO("(%u,%p)\r\n", index, node);
    }

    PRINT_INFO("all node list:\r\n");
    node = (LOS_MEMBOX_NODE *)(boxInfo + 1);
    for (index = 0; index < boxInfo->uwBlkNum; ++index, node = OS_MEMBOX_NEXT(node, boxInfo->uwBlkSize)) {
        PRINT_INFO("(%u,%p,%p)\r\n", index, node, node->pstNext);
    }
    MEMBOX_UNLOCK(intSave);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemboxStatisticsGet(const VOID *boxMem, UINT32 *maxBlk,
                                                      UINT32 *blkCnt, UINT32 *blkSize)
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
