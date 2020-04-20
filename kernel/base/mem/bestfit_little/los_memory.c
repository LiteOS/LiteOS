/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Module Implementation
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
#include "los_memory_pri.h"
#include "securec.h"
#include "los_typedef.h"
#include "los_heap_pri.h"
#include "los_hwi.h"
#include "los_spinlock.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define POOL_ADDR_ALIGNSIZE 64

#if (LOSCFG_MEM_MUL_POOL == YES)
VOID *g_poolHead = NULL;
#endif

LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_memSpin);

UINT8 *m_aucSysMem0 = (UINT8 *)NULL;
UINT8 *m_aucSysMem1 = (UINT8 *)NULL;
__attribute__((section(".data.init"))) UINTPTR g_sys_mem_addr_end;
__attribute__((section(".data.init"))) UINTPTR g_excInteractMemSize = 0;

LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemInit(VOID *pool, UINT32 size)
{
    BOOL ret = TRUE;
    UINT32 intSave;
#ifdef LOSCFG_MEM_MUL_POOL
    VOID *next = g_poolHead;
    VOID *cur = g_poolHead;
    UINT32 poolEnd;
#endif

    if ((pool == NULL) || (size <= sizeof(LosMemPoolInfo))) {
        return OS_ERROR;
    }

    MEM_LOCK(intSave);

#ifdef LOSCFG_MEM_MUL_POOL
    while (next != NULL) {
        poolEnd = (UINT32)(UINTPTR)next + ((LosMemPoolInfo *)next)->size;
        if (((pool <= next) && (((UINT32)(UINTPTR)pool + size) > (UINT32)(UINTPTR)next)) ||
            (((UINT32)(UINTPTR)pool < poolEnd) && (((UINT32)(UINTPTR)pool + size) >= poolEnd))) {
            PRINT_ERR("pool [%p, 0x%x) conflict with pool [%p, 0x%x)\n",
                      pool, (UINT32)(UINTPTR)pool + size,
                      next, (UINT32)(UINTPTR)next + ((LosMemPoolInfo *)next)->size);

            MEM_UNLOCK(intSave);
            return OS_ERROR;
        }
        cur = next;
        next = ((LosMemPoolInfo *)next)->nextPool;
    }
#endif

    ret = OsHeapInit(pool, size);
    if (!ret) {
        MEM_UNLOCK(intSave);
        return OS_ERROR;
    }

#ifdef LOSCFG_MEM_MUL_POOL
    if (g_poolHead == NULL) {
        g_poolHead = pool;
    } else {
        ((LosMemPoolInfo *)cur)->nextPool = pool;
    }

    ((LosMemPoolInfo *)pool)->nextPool = NULL;
#endif

    MEM_UNLOCK(intSave);
    return LOS_OK;
}

/*
 * Description : Initialize Dynamic Memory pool
 * Return      : LOS_OK on success or error code on failure
 */
LITE_OS_SEC_TEXT_INIT UINT32 OsMemSystemInit(UINTPTR memStart)
{
    UINT32 ret;
    UINT32 memSize;

    m_aucSysMem1 = (UINT8 *)((memStart + (POOL_ADDR_ALIGNSIZE - 1)) & ~((UINTPTR)(POOL_ADDR_ALIGNSIZE - 1)));
    memSize = OS_SYS_MEM_SIZE;
    ret = LOS_MemInit((VOID *)m_aucSysMem1, memSize);
#ifndef LOSCFG_EXC_INTERACTION
    m_aucSysMem0 = m_aucSysMem1;
#endif
    return ret;
}

#ifdef LOSCFG_MEM_MUL_POOL
LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemDeInit(VOID *pool)
{
    UINT32 intSave;
    UINT32 ret = LOS_NOK;
    VOID *next = NULL;
    VOID *cur = NULL;

    if (pool == NULL) {
        return ret;
    }
    MEM_LOCK(intSave);
    do {
        if (pool == g_poolHead) {
            g_poolHead = ((LosMemPoolInfo *)g_poolHead)->nextPool;
            ret = LOS_OK;
            break;
        }

        cur = g_poolHead;
        next = g_poolHead;
        while (next != NULL) {
            if (pool == next) {
                ((LosMemPoolInfo *)cur)->nextPool = ((LosMemPoolInfo *)next)->nextPool;
                ret = LOS_OK;
                break;
            }
            cur = next;
            next = ((LosMemPoolInfo *)next)->nextPool;
        }
    } while (0);

    MEM_UNLOCK(intSave);
    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemPoolList(VOID)
{
    VOID *next = g_poolHead;
    UINT32 index = 0;
    UINT32 intSave;

    while (next != NULL) {
        index++;
        MEM_LOCK(intSave);
        OsAlarmHeapInfo(next);
        MEM_UNLOCK(intSave);
        next = ((LosMemPoolInfo *)next)->nextPool;
    }
    return index;
}
#endif

LITE_OS_SEC_TEXT VOID *LOS_MemAlloc(VOID *pool, UINT32 size)
{
    VOID *ptr = NULL;
    UINT32 intSave;

    if ((pool == NULL) || (size == 0)) {
        return NULL;
    }

    MEM_LOCK(intSave);
    ptr = OsHeapAlloc(pool, size);
    MEM_UNLOCK(intSave);

    return ptr;
}

LITE_OS_SEC_TEXT VOID *LOS_MemAllocAlign(VOID *pool, UINT32 size, UINT32 boundary)
{
    VOID *ptr = NULL;
    UINT32 intSave;

    MEM_LOCK(intSave);
    ptr = OsHeapAllocAlign(pool, size, boundary);
    MEM_UNLOCK(intSave);

    return ptr;
}

VOID *LOS_MemRealloc(VOID *pool, VOID *ptr, UINT32 size)
{
    VOID *retPtr = NULL;
    VOID *freePtr = NULL;
    struct LosHeapNode *node = NULL;
    UINT32 cpySize = 0;
    UINT32 gapSize = 0;
    errno_t rc;

    /* Zero-size requests are treated as free. */
    if ((ptr != NULL) && (size == 0)) {
        if (LOS_MemFree(pool, ptr) != LOS_OK) {
            PRINT_ERR("LOS_MemFree error, pool[%p], ptr[%p]\n", pool, ptr);
        }
    } else if (ptr == NULL) { /* Requests with NULL pointers are treated as malloc */
        retPtr = LOS_MemAlloc(pool, size);
    } else {
        /* find the real ptr through gap size */
        gapSize = *((UINTPTR *)((UINTPTR)ptr - sizeof(UINTPTR) / sizeof(UINT8)));
        if (OS_MEM_GET_ALIGN_FLAG(gapSize)) {
            return NULL;
        }

        node = ((struct LosHeapNode *)ptr) - 1;
        cpySize = (size > node->size) ? node->size : size;

        retPtr = LOS_MemAlloc(pool, size);
        if (retPtr != NULL) {
            rc = memcpy_s(retPtr, size, ptr, cpySize);
            if (rc == EOK) {
                freePtr = ptr;
            } else {
                freePtr = retPtr;
                retPtr = NULL;
            }

            if (LOS_MemFree(pool, freePtr) != LOS_OK) {
                PRINT_ERR("LOS_MemFree error, pool[%p], ptr[%p]\n", pool, freePtr);
            }
        }
    }

    return retPtr;
}

LITE_OS_SEC_TEXT UINT32 LOS_MemFree(VOID *pool, VOID *mem)
{
    BOOL ret = FALSE;
    UINT32 intSave;

    if ((pool == NULL) || (mem == NULL)) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);
    ret = OsHeapFree(pool, mem);
    MEM_UNLOCK(intSave);

    return ((ret == TRUE) ? LOS_OK : LOS_NOK);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemInfoGet(VOID *pool, LOS_MEM_POOL_STATUS *status)
{
    LosHeapStatus heapStatus;
    UINT32 err;
    UINT32 intSave;

    if ((pool == NULL) || (status == NULL)) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);

    err = OsHeapStatisticsGet(pool, &heapStatus);
    if (err != LOS_OK) {
        MEM_UNLOCK(intSave);
        return LOS_NOK;
    }

    status->uwTotalUsedSize   = heapStatus.totalUsedSize;
    status->uwTotalFreeSize   = heapStatus.totalFreeSize;
    status->uwMaxFreeNodeSize = heapStatus.maxFreeNodeSize;
    status->uwUsedNodeNum  = heapStatus.usedNodeNum;
    status->uwFreeNodeNum  = heapStatus.freeNodeNum;

    MEM_UNLOCK(intSave);
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemTotalUsedGet(VOID *pool)
{
    LosHeapStatus heapStatus;
    UINT32 err;
    UINT32 intSave;

    if (pool == NULL) {
        return OS_NULL_INT;
    }

    MEM_LOCK(intSave);
    err = OsHeapStatisticsGet(pool, &heapStatus);
    MEM_UNLOCK(intSave);
    if (err != LOS_OK) {
        return OS_NULL_INT;
    }

    return heapStatus.totalUsedSize;
}

UINT32 LOS_MemGetMaxFreeBlkSize(VOID *pool)
{
    UINT32 maxFreeBlkSize;
    UINT32 intSave;
    if (pool == NULL) {
        return 0;
    }
    MEM_LOCK(intSave);
    maxFreeBlkSize = OsHeapGetMaxFreeBlkSize(pool);
    MEM_UNLOCK(intSave);
    return maxFreeBlkSize;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemPoolSizeGet(const VOID *pool)
{
    LosMemPoolInfo *heapManager = (LosMemPoolInfo *)pool;
    if (heapManager == NULL) {
        return 0;
    }
    return heapManager->size;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemIntegrityCheck(VOID *pool)
{
    (VOID)pool;
    PRINT_ERR("[%s:%d] not Implement!!\n", __FUNCTION__, __LINE__);
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
