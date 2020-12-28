/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS memory Module HeadFile
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

/**
 * @ingroup los_membox
 * @brief Align the input parameter.
 *
 * @par Description:
 * The macro is used to align memAddr based on UINTPTR. memAddr is the input parameter.
 * @attention
 * None.
 *
 * @param memAddr  [IN] The variable that need to be aligned. Usually memAddr is an unsigned integer
 *                      or an unsigned long integer on 64-bit platporm.
 *
 * @retval The memAddr value after alignment.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_MEMBOX_ALLIGNED(memAddr) (((UINTPTR)(memAddr) + sizeof(UINTPTR) - 1) & (~(sizeof(UINTPTR) - 1)))

#ifdef LOSCFG_KERNEL_MEMBOX_STATIC
/**
 * @ingroup los_membox
 * Get next node in static memory pool
 */
#define OS_MEMBOX_NEXT(addr, blkSize) (LOS_MEMBOX_NODE *)(VOID *)((UINT8 *)(addr) + (blkSize))

/**
 * @ingroup los_membox
 * Head size of each node in staic memory pool
 */
#define OS_MEMBOX_NODE_HEAD_SIZE sizeof(LOS_MEMBOX_NODE)

/**
 * @ingroup los_membox
 * @brief Obtain the size of the static memory pool.
 *
 * @par Description:
 * The macro is used to obtain the size of the static memory pool according to the memory
 * block size and number.
 * @attention
 * None.
 *
 * @param blkSize  [IN] Type #UINT32  The memory block size of the static memory pool.
 * @param blkNum   [IN] Type #UINT32  The total memory block number of the static memory pool.
 *
 * @retval The size of the static memory pool.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_MEMBOX_SIZE(blkSize, blkNum) \
    (sizeof(LOS_MEMBOX_INFO) + (LOS_MEMBOX_ALLIGNED((blkSize) + OS_MEMBOX_NODE_HEAD_SIZE) * (blkNum)))

/**
 * @ingroup los_membox
 * Structure of a free node in a static memory pool
 */
typedef struct tagMEMBOX_NODE {
    struct tagMEMBOX_NODE *pstNext; /**< Free node's pointer to the next node in a static memory pool. */
} LOS_MEMBOX_NODE;
#endif

/**
 * @ingroup los_membox
 * Static memory pool information structure
 */
typedef struct {
    UINT32 uwBlkSize;           /**< The memory block size of the static memory pool */
    UINT32 uwBlkNum;            /**< The total memory block number of the static memory pool */
    UINT32 uwBlkCnt;            /**< The number of allocated memory blocks in the static memory pool */
#ifdef LOSCFG_KERNEL_MEMBOX_STATIC
    LOS_MEMBOX_NODE stFreeList; /**< The list of free memory block node in the static memory pool. This
                                     structure member is available only LOSCFG_KERNEL_MEMBOX_STATIC is
                                     defined. */
#endif
} LOS_MEMBOX_INFO;

typedef LOS_MEMBOX_INFO OS_MEMBOX_S;

/**
 * @ingroup los_membox
 * @brief Initialize a static memory pool.
 *
 * @par Description:
 * This API is used to initialize a static memory pool. The start address of the memory pool is specified
 * by the first parameter. In the API, it will set the memory block size, total block number, allocated
 * block number and the list of free memory block nodes of the static memory pool.
 * @attention
 * The poolSize parameter value should match the following two conditions:
 * <ul>
 * <li>Be less than or equal to the size of the memory pool specified by the pool parameter.</li>
 * <li>Be greater than the size of LOS_MEMBOX_INFO.</li>
 * </ul>
 *
 * @param pool     [IN] Memory pool address.
 * @param poolSize [IN] The total size of the static memory pool.
 * @param blkSize  [IN] The memory block size of the static memory pool.
 *
 * @retval #LOS_NOK   The memory pool fails to be initialized.
 * @retval #LOS_OK    The memory pool is successfully initialized.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemboxInit(VOID *pool, UINT32 poolSize, UINT32 blkSize);

/**
 * @ingroup los_membox
 * @brief Request a static memory block.
 *
 * @par Description:
 * This API is used to request a static memory block from the static memory pool which has been initialized.
 * @attention
 * The input pool parameter must be initialized via func #LOS_MemboxInit.
 *
 * @param pool    [IN] Memory pool address.
 *
 * @retval #VOID*    This API will return a memory block address, if the request is accepted successfully.
 * @retval #NULL     The request fails.
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
 * @brief Free a static memory block.
 *
 * @par Description:
 * This API is used to free a static memory block to the static memory pool.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func #LOS_MemboxInit.</li>
 * <li>The input box parameter must be allocated by #LOS_MemboxAlloc.</li>
 * </ul>
 *
 * @param pool     [IN] Memory pool address. The memory block need to release is requested
 *                      from this memory pool.
 * @param box      [IN] The pointer to the memory block to be released.
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
 * @brief Clear a static memory block.
 *
 * @par Description:
 * This API is used to set the memory block value to 0.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func #LOS_MemboxInit.</li>
 * <li>The input box parameter must be allocated by #LOS_MemboxAlloc.</li>
 * </ul>
 *
 * @param pool     [IN] Memory pool address. The memory block need to clear is requested
 *                      from this memory pool.
 * @param box      [IN] The pointer to the memory block to clear.
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_MemboxClr(VOID *pool, VOID *box);

/**
 * @ingroup los_membox
 * @brief show static memory pool information.
 *
 * @par Description:
 * This API is used to print static memory pool information. It can print the memory pool address,
 * the memory block size, the total block number, the list of free memory block node and the total block
 * node list of the static memory pool.
 * @attention
 * The input pool parameter must be initialized via func #LOS_MemboxInit.
 *
 * @param pool    [IN] Memory pool address.
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_membox.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see LOS_MemboxStatisticsGet
 * @since Huawei LiteOS V100R001C00
 */
extern VOID LOS_ShowBox(VOID *pool);

/**
 * @ingroup los_membox
 * @brief Obtain the static memory pool information.
 *
 * @par Description:
 * This API is used to obtain the static memory pool information. The information includes
 * the total memory block number, block size and the allocated block number of the static
 * memory pool. The obtained information will be outputted in the last three parameters.
 *
 * @attention
 * The first parameter boxMem of this interface is a pointer, it should be a correct value.
 * Otherwise the system may be abnormal.
 *
 * @param  boxMem       [IN]  Type  #VOID*   Pointer to the static memory pool.
 * @param  maxBlk       [OUT] Type  #UINT32* The total memory block number is outputted to this parameter.
 * @param  blkCnt       [OUT] Type  #UINT32* The allocated memory block number is outputted to this parameter.
 * @param  blkSize      [OUT] Type  #UINT32* The memory block size is outputted to this parameter.
 *
 * @retval #LOS_OK        Obtain the static memory pool information successfully.
 * @retval #LOS_NOK       Failed to obtain the static memory pool information, check whether the
 *                        parameters is NULL or not.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ShowBox
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemboxStatisticsGet(const VOID *boxMem, UINT32 *maxBlk, UINT32 *blkCnt, UINT32 *blkSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMBOX_H */
