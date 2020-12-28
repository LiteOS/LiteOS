/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Module Implementation
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

#include "los_memory_pri.h"
#include "los_memory_internal.h"

#include "string.h"
#include "securec.h"
#include "los_hwi.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_task_pri.h"
#include "los_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HEAP_CAST(t, exp)   ((t)(exp))
#define HEAP_ALIGN          4
#define MALLOC_MAXSIZE      (0xFFFFFFFF - HEAP_ALIGN + 1)

/*
 * Description : look up the next memory node according to one memory node in the memory block list.
 * Input       : struct LosHeapManager *heapMan --- Pointer to the manager,to distinguish heap
 *               struct LosHeapNode *node --- Size of memory in bytes to allocate
 * Return      : Pointer to next memory node
 */
struct LosHeapNode* OsHeapPrvGetNext(struct LosHeapManager *heapMan, struct LosHeapNode *node)
{
    return (heapMan->tail == node) ? NULL : (struct LosHeapNode *)(UINTPTR)(node->data + node->size);
}

#ifdef LOSCFG_MEM_TASK_STAT

VOID OsHeapStatInit(struct LosHeapManager *heapMan, UINT32 size)
{
    (VOID)memset_s(&heapMan->stat, sizeof(Memstat), 0, sizeof(Memstat));

    heapMan->stat.memTotalUsed = sizeof(struct LosHeapNode) + sizeof(struct LosHeapManager);
    heapMan->stat.memTotalPeak = heapMan->stat.memTotalUsed;
}

VOID OsHeapStatAddUsed(struct LosHeapManager *heapMan, struct LosHeapNode *node)
{
    UINT32 taskId;
    UINT32 blockSize = sizeof(struct LosHeapNode) + node->size;

    if ((OsCurrTaskGet() != NULL) && OS_INT_INACTIVE) {
        /*
         * after OsTaskInit, OsCurrTaskGet() is not null, but id is the same
         * as (LOSCFG_BASE_CORE_TSK_LIMIT + 1), so it will be recorded into
         * the last one of the array.
         */
        taskId = LOS_CurTaskIDGet();
    } else {
        taskId = TASK_NUM - 1;
    }

    node->taskId = taskId;
    OS_MEM_ADD_USED(&heapMan->stat, blockSize, taskId);
}

VOID OsHeapStatDecUsed(struct LosHeapManager *heapMan, struct LosHeapNode *node)
{
    UINT32 taskId = node->taskId;
    UINT32 blockSize = sizeof(struct LosHeapNode) + node->size;

    OS_MEM_REDUCE_USED(&heapMan->stat, blockSize, taskId);
}

#else /* LOSCFG_MEM_TASK_STAT */

VOID OsHeapStatInit(struct LosHeapManager *heapMan, UINT32 size) { }

VOID OsHeapStatAddUsed(struct LosHeapManager *heapMan, struct LosHeapNode *node) { }

VOID OsHeapStatDecUsed(struct LosHeapManager *heapMan, struct LosHeapNode *node) { }

#endif /* LOSCFG_MEM_TASK_STAT */

#ifdef LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK

UINT32 OsHeapIntegrityCheck(struct LosHeapManager *heap)
{
    struct LosHeapNode *node = (struct LosHeapNode *)(heap + 1);
    UINTPTR heapStart = (UINTPTR)heap;
    UINTPTR heapEnd = (UINTPTR)node + heap->size;

    while (node != NULL) {
        if ((UINTPTR)node < heapStart || (UINTPTR)node > heapEnd) {
            LOS_Panic("node %p has been corrupted.\n", node);
            return LOS_NOK;
        }

        node = OsHeapPrvGetNext(heap, node);
    }

    return LOS_OK;
}

#else /* LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK */

UINT32 OsHeapIntegrityCheck(struct LosHeapManager *heap)
{
    return LOS_OK;
}

#endif /* LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK */

#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION

VOID *OsMemAlloc(VOID *pool, UINT32 size)
{
    return OsHeapAlloc(pool, size);
}

UINT32 OsMemFree(VOID *pool, VOID *mem)
{
    if (OsHeapFree(pool, mem) == TRUE) {
        return LOS_OK;
    } else {
        return LOS_NOK;
    }
}

#endif /* LOSCFG_KERNEL_MEM_SLAB_EXTENTION */

/*
 * Description : To initialize the heap memory and get the begin address and size of heap memory,
 *               then initialize LosHeapManager.
 * Input       : VOID *pool  --- begin address of the heap memory pool
 *               UITN32 size --- size of the heap memory pool
 * Return      : 1:success 0:error
 */
BOOL OsHeapInit(VOID *pool, UINT32 size)
{
    struct LosHeapNode *node = NULL;
    struct LosHeapManager *heapMan = HEAP_CAST(struct LosHeapManager *, pool);

    if ((heapMan == NULL) || (size <= (sizeof(struct LosHeapNode) + sizeof(struct LosHeapManager)))) {
        return FALSE;
    }

    (VOID)memset_s(pool, size, 0, size);

    heapMan->size = size - sizeof(struct LosHeapManager);

    node = heapMan->head = (struct LosHeapNode *)((UINT8*)pool + sizeof(struct LosHeapManager));

    heapMan->tail = node;

    node->used = 0;
    node->prev = NULL;
    node->size = heapMan->size - sizeof(struct LosHeapNode);

    OsHeapStatInit(heapMan, size);

    return TRUE;
}

/*
 * Description : To alloc memory block from the heap memory poll
 * Input       : VOID *pool --- Pointer to the manager,to distinguish heap
 *               UINT32 size --- size of the heap memory pool
 * Return      : NULL:error, other value:the address of the memory we alloced
 */
VOID *OsHeapAlloc(VOID *pool, UINT32 size)
{
    struct LosHeapNode *node = NULL;
    struct LosHeapNode *next = NULL;
    struct LosHeapNode *best = NULL;
    VOID *ptr = NULL;
    UINT32 alignSize = ALIGNE(size);

    struct LosHeapManager *heapMan = HEAP_CAST(struct LosHeapManager *, pool);
    if ((heapMan == NULL) || (size > MALLOC_MAXSIZE)) {
        return NULL;
    }

    if (OsHeapIntegrityCheck(heapMan) != LOS_OK) {
        return NULL;
    }

    node = heapMan->tail;
    while (node != NULL) {
        if ((node->used == 0) && (node->size >= alignSize) &&
            ((best == NULL) || (best->size > node->size))) {
            best = node;
            if (best->size == alignSize) {
                goto SIZE_MATCH;
            }
        }
        node = node->prev;
    }

    /* alloc failed */
    if (best == NULL) {
        PRINT_ERR("there's not enough whole to alloc 0x%x Bytes!\n", alignSize);
        goto OUT;
    }

    if ((best->size - alignSize) > sizeof(struct LosHeapNode)) {
        /* hole divide into 2 */
        node = (struct LosHeapNode*)(UINTPTR)(best->data + alignSize);

        node->used = 0;
        node->size = best->size - alignSize - sizeof(struct LosHeapNode);
        node->prev = best;

        if (best != heapMan->tail) {
            next = OsHeapPrvGetNext(heapMan, node);
            if (next != NULL) {
                next->prev = node;
            }
        } else {
            heapMan->tail = node;
        }

        best->size = alignSize;
    }

SIZE_MATCH:
    best->align = 0;
    best->used = 1;
    ptr = best->data;

    OsHeapStatAddUsed(heapMan, best);
OUT:
    return ptr;
}

/*
 * Description : To alloc memory block from the heap memory poll with
 * Input       : VOID *pool   --- Pointer to the manager,to distinguish heap
 *               UINT32 size   --- size of the heap memory pool
 *               UINT32 boundary --- boundary the heap needs align
 * Return      : NULL:error, other value:the address of the memory we alloced
 */
VOID* OsHeapAllocAlign(VOID *pool, UINT32 size, UINT32 boundary)
{
    UINT32 useSize;
    UINT32 gapSize;
    VOID *ptr = NULL;
    VOID *alignedPtr = NULL;

    if ((pool == NULL) || (size == 0) || (boundary < sizeof(VOID *)) || !IS_ALIGNED(boundary, boundary)) {
        return NULL;
    }

    /* worst case is that the node happen to be 4 bytes ahead of the boundary */
    useSize = (size + boundary) - sizeof(VOID*);
    if (useSize < size) {
        return NULL;
    }

    ptr = OsHeapAlloc(pool, useSize);
    if (ptr != NULL) {
        alignedPtr = (VOID *)(UINTPTR)OS_MEM_ALIGN(ptr, boundary);
        if (alignedPtr == ptr) {
            goto OUT;
        }

        gapSize = (UINTPTR)alignedPtr - (UINTPTR)ptr;
        OS_MEM_SET_ALIGN_FLAG(gapSize);
        *((UINT32 *)((UINTPTR)alignedPtr - sizeof(UINTPTR))) = gapSize;

        ptr = alignedPtr;
    }
OUT:
    return ptr;
}

STATIC VOID OsHeapDoFree(struct LosHeapManager *heapMan, struct LosHeapNode *curNode)
{
    struct LosHeapNode *node = curNode;
    struct LosHeapNode *next = NULL;
    /* set to unused status */
    node->used = 0;

    /* unused region before and after combination */
    while ((node->prev) && (!node->prev->used)) {
        node = node->prev;
    }

    next = OsHeapPrvGetNext(heapMan, node);
    while (next != NULL) {
        if (next->used) {
            next->prev = node;
            break;
        }
        node->size += (sizeof(struct LosHeapNode) + next->size);
        if (heapMan->tail == next) {
            heapMan->tail = node;
        }
        next = OsHeapPrvGetNext(heapMan, node);
    }
}

/*
 * Description : To free the  memory block from  heap memory poll
 * Input       : VOID* pool --- Pointer to the manager,to distinguish heap
 *               VOID* ptr --- the pointer of heap memory we want to free
 * Return      : 1:success 0:error
 */
BOOL OsHeapFree(VOID *pool, VOID *ptr)
{
    struct LosHeapNode *node = NULL;

    UINT32 gapSize;
    BOOL ret = TRUE;

    struct LosHeapManager *heapMan = HEAP_CAST(struct LosHeapManager *, pool);
    if ((heapMan == NULL) || (ptr == NULL)) {
        return LOS_NOK;
    }

    /* find the real ptr through gap size */
    gapSize = *((UINT32 *)((UINTPTR)ptr - sizeof(UINTPTR)));
    if (OS_MEM_GET_ALIGN_FLAG(gapSize)) {
        gapSize = OS_MEM_GET_ALIGN_GAPSIZE(gapSize);
        ptr = (VOID *)((UINTPTR)ptr - gapSize);
    }

    if (((UINTPTR)ptr < (UINTPTR)heapMan->head) ||
        ((UINTPTR)ptr > ((UINTPTR)heapMan->tail + sizeof(struct LosHeapNode)))) {
        PRINT_ERR("0x%lx out of range!\n", (UINTPTR)ptr);
        return FALSE;
    }

    node = ((struct LosHeapNode *)ptr) - 1;
    /* check if the address is a node of the heap memory list */
    if ((node->used == 0) || (!((UINTPTR)node == (UINTPTR)heapMan->head) &&
        (((UINTPTR)node->prev < (UINTPTR)heapMan->head) ||
        ((UINTPTR)node->prev > ((UINTPTR)heapMan->tail + sizeof(struct LosHeapNode))) ||
        ((UINTPTR)OsHeapPrvGetNext(heapMan, node->prev) != (UINTPTR)node)))) {
        ret = FALSE;
        goto OUT;
    }

    OsHeapStatDecUsed(heapMan, node);
    OsHeapDoFree(heapMan, node);

OUT:
    return ret;
}

/*
 * Description : collect heap statistics
 * Input       : pool --- Pointer to the manager, to distinguish heap
 * Output      : status --- heap statistics
 * Return      : LOS_OK on success or error code on failure
 */
UINT32 OsHeapStatisticsGet(VOID *pool, LosHeapStatus *status)
{
    UINT32 heapUsed = 0;
    UINT32 maxFreeNodeSize = 0;
    UINT32 freeNodeNum = 0;
    UINT32 usedNodeNum = 0;

    struct LosHeapNode *node = NULL;
    struct LosHeapManager *ramHeap = HEAP_CAST(struct LosHeapManager *, pool);

    if (ramHeap == NULL) {
        return LOS_NOK;
    }

    if (status == NULL) {
        return LOS_NOK;
    }

    /* heap manager header use heap space */
    heapUsed += sizeof(struct LosHeapManager);

    node = ramHeap->tail;
    while (node != NULL) {
        if (node->used) {
            heapUsed += (node->size + sizeof(struct LosHeapNode));
            usedNodeNum++;
        } else {
            if (node->size > maxFreeNodeSize) {
                maxFreeNodeSize = node->size;
            }
            freeNodeNum++;
        }
        node = node->prev;
    }

    if (ramHeap->size < heapUsed) {
        return LOS_NOK;
    }

    status->totalUsedSize = heapUsed;
    status->maxFreeNodeSize = maxFreeNodeSize;
    status->totalFreeSize = ramHeap->size - status->totalUsedSize;
    status->usedNodeNum = usedNodeNum;
    status->freeNodeNum = freeNodeNum;

#ifdef LOSCFG_MEM_TASK_STAT
    status->usageWaterLine = ramHeap->stat.memTotalPeak;
#endif

    return LOS_OK;
}

/*
 * Description : get max free block size
 * Input       : pool --- Pointer to the manager, to distinguish heap
 * Return      : max free block size
 */
UINT32 OsHeapGetMaxFreeBlkSize(VOID *pool)
{
    UINT32 size = 0;
    UINT32 temp;
    struct LosHeapNode *node = NULL;
    struct LosHeapManager *ramHeap = HEAP_CAST(struct LosHeapManager *, pool);

    if (ramHeap == NULL) {
        return LOS_NOK;
    }

    node = ramHeap->tail;
    while (node != NULL) {
        if (!(node->used)) {
            temp = node->size;
            if (temp > size) {
                size = temp;
            }
        }
        node = node->prev;
    }
    return size;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
