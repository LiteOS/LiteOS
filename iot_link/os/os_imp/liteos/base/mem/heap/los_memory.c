/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include <los_config.h>

#if (LOSCFG_HEAP_IMPROVED == YES)

#include <string.h>

#include <mem.h>
#include <heap.h>

#ifdef LOSCFG_ENABLE_MPU
#include <los_task.ph>
#endif

#include <stdbool.h>

/*****************************************************************************
 Function : LOS_MemInit
 Description : Initialize Dynamic Memory pool
 Input       : pPool  --- Pointer to memory pool
               uwSize --- Size of memory in bytes to allocate
 Output      : None
 Return      : LOS_OK - Ok, LOS_NOK - Error
*****************************************************************************/
UINT32 LOS_MemInit (VOID *pPool, UINT32 uwSize)
{
    heap_t *                heap;

    uwSize -= (UINT32) pPool & ALLOC_ALIGN_MASK;
    uwSize &= ~ALLOC_ALIGN_MASK;
    pPool  = (char *) ((UINT32) pPool & ~ALLOC_ALIGN_MASK);

    if (pPool == NULL || uwSize <= sizeof(heap_t))
    {
        return LOS_NOK;
    }

    heap = (heap_t *) pPool;

    heap_init (heap);

    if (heap_add (heap, (char *) (heap + 1), uwSize - sizeof (heap_t)) != 0)
    {
        return LOS_NOK;
    }

    return LOS_OK;
}

UINT32 osMemSystemInit(VOID)
{
    UINT32                  uwRet;
    const struct phys_mem * spm = system_phys_mem;

    uwRet = LOS_MemInit((VOID *)spm->start, (UINT32)(spm->end - spm->start));

    if (uwRet != LOS_OK)
    {
        return uwRet;
    }

    m_aucSysMem0 = (UINT8*) spm->start;

    spm++;

    while (spm->end)
    {
        heap_add ((heap_t *) m_aucSysMem0, (char *) spm->start,
                  (size_t) spm->end - spm->start);

        spm++;
    }

    return LOS_OK;
}

/*****************************************************************************
 Function : osHeapAllocAlign
 Description : To alloc memory block from the heap memory poll with
 Input       : VOID *pPool       --- Pointer to the manager,to distinguish heap
               UINT32 uwSize     --- size of the heap memory pool
               UINT32 uwBoundary --- boundary the heap needs align
 Output      : None
 Return      : NULL:error    other value:the address of the memory we alloced
*****************************************************************************/
LITE_OS_SEC_TEXT VOID* osHeapAllocAlign(VOID *pPool, UINT32 uwSize, UINT32 uwBoundary)
{
    return heap_alloc_align ((heap_t *) pPool, uwBoundary, uwSize);
}

/*****************************************************************************
 Function : osHeapAlloc
 Description : To alloc memory block from the heap memory poll
 Input       : VOID *pPool   --- Pointer to the manager,to distinguish heap
               UINT32 uwSize --- size of the heap memory pool
 Output      : None
 Return      : NULL:error    other value:the address of the memory we alloced
*****************************************************************************/
VOID * osHeapAlloc (VOID * pPool, UINT32 uwSize)
{
    return heap_alloc ((heap_t *) pPool, uwSize);
}

/*****************************************************************************
 Function : osHeapFree
 Description : To free the  memory block from  heap memory poll
 Input       : VOID* pPool --- Pointer to the manager,to distinguish heap
               VOID* pPtr: --- the pointer of heap memory we want to free
 Output      : None
 Return      : 1:success 0:error
*****************************************************************************/
BOOL osHeapFree (VOID * pPool, VOID * pMem)
{
    if (heap_free ((heap_t *) pPool, pMem) != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************
 Function : osHeapRealloc
 Description : realloc memory from heap memory pool
 Input       : VOID* pPool --- Pointer to the manager,to distinguish heap
               VOID* pPtr: --- the pointer of heap memory we want to free
 Output      : None
 Return      : Pointer to allocated memory node
*****************************************************************************/
VOID *osHeapRealloc(VOID * pPool, VOID *pPtr, UINT32 uwSize)
{
    return heap_realloc ((heap_t *) pPool, pPtr, uwSize);
}

/*****************************************************************************
 Function : LOS_MemAlloc
 Description : Allocate Memory from Memory pool
 Input       : pPool  --- Pointer to memory pool
               uwSize --- Size of memory in bytes to allocate
 Output      : None
 Return      : Pointer to allocated memory
*****************************************************************************/
LITE_OS_SEC_TEXT VOID *LOS_MemAlloc (VOID *pPool, UINT32 uwSize)
{
    VOID *pRet = NULL;

    if (0 == uwSize)
    {
        return pRet;
    }

    if (NULL == pPool)
    {
#if (LOSCFG_ENABLE_MPU == YES)
        pPool = g_stLosTask.pstRunTask->pPool;
#else
        return pRet;
#endif
    }

#if (LOSCFG_KERNEL_MEM_SLAB == YES)
    pRet = osSlabMemAlloc(pPool, uwSize);
    if(pRet == NULL)
#endif
        pRet = osHeapAlloc((heap_t *) pPool, uwSize);

    return pRet;
}

/*****************************************************************************
 Function : LOS_MemAllocAlign
 Description : align size then allocate node from Memory pool
 Input       : pPool      --- Pointer to memory pool
               uwSize     --- Size of memory in bytes to allocate
               uwBoundary --- align form
 Output      : None
 Return      : Pointer to allocated memory node
*****************************************************************************/
LITE_OS_SEC_TEXT VOID *LOS_MemAllocAlign(VOID *pPool, UINT32 uwSize, UINT32 uwBoundary)
{
    return osHeapAllocAlign((heap_t *) pPool, uwSize, uwBoundary);
}

/*****************************************************************************
 Function : LOS_MemRealloc
 Description : realloc memory from Memory pool
 Input       : pPool  --- Pointer to memory pool
               pPtr   --- Pointer to memory
               uwSize --- new size
 Output      : None
 Return      : Pointer to allocated memory node
*****************************************************************************/
LITE_OS_SEC_TEXT_MINOR VOID *LOS_MemRealloc(VOID *pPool, VOID *pPtr, UINT32 uwSize)
{
    return osHeapRealloc((heap_t *) pPool, pPtr, uwSize);
}

/*****************************************************************************
 Function : LOS_MemFree
 Description : Free Memory and return it to Memory pool
 Input       : pPool --- Pointer to memory pool
               pMem  --- Pointer to memory to free
 Output      : None
 Return      : LOS_OK - OK, LOS_NOK - Error
*****************************************************************************/
LITE_OS_SEC_TEXT UINT32 LOS_MemFree (VOID *pPool, VOID *pMem)
{
    BOOL bRet = FALSE;

    if ((NULL == pPool) || (NULL == pMem))
    {
        return LOS_NOK;
    }

#if (LOSCFG_KERNEL_MEM_SLAB == YES)
    bRet = osSlabMemFree(pPool, pMem);
    if(bRet != TRUE)
#endif
        bRet = osHeapFree((heap_t *) pPool, pMem);

    return (bRet == TRUE ? LOS_OK : LOS_NOK);
}

VOID * osSlabCtrlHdrGet (VOID * pPool)
{
#if (LOSCFG_KERNEL_MEM_SLAB == YES)
    return &((heap_t *)pPool)->stSlabCtrlHdr;
#else
    return NULL;
#endif
}

VOID LOS_MemInfo (VOID * pPool, BOOL bShowDetail)
    {
    heap_t * pHeap = pPool == NULL ? (heap_t *) m_aucSysMem0 : (heap_t *) pPool;

    __dump_heap (pHeap, (bool)bShowDetail);
    }

#endif
