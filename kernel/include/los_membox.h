/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS memory Module HeadFile
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

/**
 * @defgroup los_membox Static memory
 * @ingroup kernel
 */

#ifndef _LOS_MEMBOX_H
#define _LOS_MEMBOX_H

#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MEMBOX_NEXT(addr, blkSize) (LOS_MEMBOX_NODE *)(VOID *)((UINT8 *)(addr) + (blkSize))

#define OS_MEMBOX_NODE_HEAD_SIZE sizeof(LOS_MEMBOX_NODE)

/**
 * @ingroup los_membox
 * Structure of a free node in a memory pool
 */
typedef struct tagMEMBOX_NODE {
    struct tagMEMBOX_NODE *pstNext; /**< Free node's pointer to the next node in a memory pool */
} LOS_MEMBOX_NODE;

/**
 * @ingroup los_membox
 * Memory pool information structure
 */
typedef struct {
    UINT32 uwBlkSize;           /**< Block size */
    UINT32 uwBlkNum;            /**< Block number */
    UINT32 uwBlkCnt;            /**< The number of allocated blocks */
    LOS_MEMBOX_NODE stFreeList; /**< Free list */
} LOS_MEMBOX_INFO;

typedef LOS_MEMBOX_INFO OS_MEMBOX_S;

/**
 * @ingroup los_membox
 * Memory pool alignment
 */
#define LOS_MEMBOX_ALLIGNED(memAddr) (((UINTPTR)(memAddr) + sizeof(UINTPTR) - 1) & (~(sizeof(UINTPTR) - 1)))

/**
 * @ingroup los_membox
 * Memory pool size
 */
#define LOS_MEMBOX_SIZE(blkSize, blkNum) \
    (sizeof(LOS_MEMBOX_INFO) + (LOS_MEMBOX_ALLIGNED((blkSize) + OS_MEMBOX_NODE_HEAD_SIZE) * (blkNum)))

/**
 * @ingroup los_membox
 * @brief Initialize a memory pool.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to initialize a memory pool.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The poolSize parameter value should match the following two conditions :
 * 1) Be less than or equal to the Memory pool size;
 * 2) Be greater than the size of LOS_MEMBOX_INFO.</li>
 * <li>The pool parameter must be pointer size aligned.</li>
 * </ul>
 *
 * @param pool     [IN] Memory pool address.
 * @param poolSize [IN] Memory pool size.
 * @param blkSize  [IN] Memory block size.
 *
 * @retval #LOS_NOK   The memory pool fails to be initialized.
 * @retval #LOS_OK    The memory pool is successfully initialized.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemboxInit(VOID *pool, UINT32 poolSize, UINT32 blkSize);

/**
 * @ingroup los_membox
 * @brief Request a memory block.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to request a memory block.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemboxInit.</li>
 * </ul>
 *
 * @param pool    [IN] Memory pool address.
 *
 * @retval #VOID*      The request is accepted, and return a memory block address.
 * @retval #NULL       The request fails.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see LOS_MemboxFree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemboxAlloc(VOID *pool);

/**
 * @ingroup los_membox
 * @brief Free a memory block.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to free a memory block.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemboxInit.</li>
 * <li>The input box parameter must be allocated by LOS_MemboxAlloc.</li>
 * </ul>
 *
 * @param pool     [IN] Memory pool address.
 * @param box      [IN] Memory block address.
 *
 * @retval #LOS_NOK   This memory block fails to be freed.
 * @retval #LOS_OK    This memory block is successfully freed.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see LOS_MemboxAlloc
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemboxFree(VOID *pool, VOID *box);

/**
 * @ingroup los_membox
 * @brief Clear a memory block.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to set the memory block value to be 0.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemboxInit.</li>
 * <li>The input box parameter must be allocated by LOS_MemboxAlloc.</li>
 * </ul>
 *
 * @param pool    [IN] Memory pool address.
 * @param box     [IN] Memory block address.
 *
 * @retval VOID
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_MemboxClr(VOID *pool, VOID *box);

/**
 * @ingroup los_membox
 * @brief show membox info.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to show the memory pool info.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemboxInit.</li>
 * </ul>
 *
 * @param pool    [IN] Memory pool address.
 *
 * @retval VOID
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_ShowBox(VOID *pool);

/**
 * @ingroup los_membox
 * @brief calculate membox information.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to calculate membox information.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>One parameter of this interface is a pointer, it should be a correct value, otherwise, the system may
 * be abnormal.</li>
 * </ul>
 *
 * @param  boxMem        [IN]  Type  #VOID*   Pointer to the calculate membox.
 * @param  maxBlk       [OUT] Type  #UINT32* Record membox max block.
 * @param  blkCnt       [OUT] Type  #UINT32* Record membox block count alreay allocated.
 * @param  blkSize      [OUT] Type  #UINT32* Record membox block size.
 *
 * @retval #LOS_OK        The heap status calculate success.
 * @retval #LOS_NOK       The membox  status calculate with some error.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemAlloc | LOS_MemRealloc | LOS_MemFree
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemboxStatisticsGet(const VOID *boxMem, UINT32 *maxBlk, UINT32 *blkCnt, UINT32 *blkSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMBOX_H */
