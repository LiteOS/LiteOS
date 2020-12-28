/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Module HeadFile
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
 * @defgroup los_memory  Dynamic memory
 * @ingroup kernel
 */

#ifndef _LOS_MEMORY_H
#define _LOS_MEMORY_H

#include "los_config.h"
#include "los_base.h"
#include "los_toolchain.h"
#include "los_membox.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_MEM_LEAKCHECK

/**
 * @ingroup los_memory
 * The omit layers of function call from kernel memory interfaces such as
 * LOS_MemAlloc/LOS_MemAllocAlign/LOS_MemRealloc/LOS_MemFree.
 * Note that this macro is defined only when LOSCFG_MEM_LEAKCHECK is defined.
 */
#define LOS_OMIT_LR_CNT 2

/**
 * @ingroup los_memory
 * The recorded layers of function call.
 * Note that this macro is defined only when LOSCFG_MEM_LEAKCHECK is defined.
 */
#define LOS_RECORD_LR_CNT 3
#endif

/**
 * @ingroup los_memory
 * @brief Define the type of the customized tuning function when calling the API LOS_MemAlloc to allocate
 * memory.
 *
 * @par Description:
 * This definition is used to declare the customized tuning function when calling the API LOS_MemAlloc to
 * allocate memory.
 * @attention
 * None.
 *
 * @param None.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
typedef VOID (*MALLOC_HOOK)(VOID);

/**
 * @ingroup los_memory
 * Customized tuning function when calling the API LOS_MemAlloc to allocate memory.
 */
extern MALLOC_HOOK g_MALLOC_HOOK;

/**
 * @ingroup los_memory
 * The start address of exception interaction dynamic memory pool address, when the exception
 * interaction feature not support, m_aucSysMem0 equals to m_aucSysMem1.
 */
extern UINT8 *m_aucSysMem0;

/**
 * @ingroup los_memory
 * The start address of system dynamic memory pool address.
 */
extern UINT8 *m_aucSysMem1;

/**
 * @ingroup los_memory
 * The end address of system memory.
 */
extern UINTPTR g_sys_mem_addr_end;

/**
 * @ingroup los_memory
 * The size of exception interaction memory.
 */
extern UINTPTR g_excInteractMemSize;

/**
 * @ingroup los_memory
 * The memory Maximum memory usage statistics.
 * Note that this macro is defined only when LOSCFG_MEM_WATERLINE is defined.
 * @attention
 * <ul> <li>If running as debug mode, it will affect the performance of memory malloc and free.</li>
 * <li> OS_MEM_WATERLINE=YES: open the function for Maximum memory usage statistics </li>
 * <li> OS_MEM_WATERLINE=NO: close the function for Maximum memory usage statistics, it set to NO as usual </li>
 * </ul>
 */
#ifdef LOSCFG_MEM_WATERLINE
#define OS_MEM_WATERLINE NO
#endif

#ifdef LOSCFG_MEM_MUL_MODULE
/**
 * @ingroup los_memory
 * The memory usage statistics depend on module, this is the max module number 0x20.
 * Note that this macro is defined only when LOSCFG_MEM_MUL_MODULE is defined.
 */
#define MEM_MODULE_MAX 0x20

/**
 * @ingroup los_memory
 * @brief Allocate dynamic memory.
 *
 * @par Description:
 * This API is used to allocate a memory block of which the size is specified and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The size of the input parameter size must be four byte-aligned.</li>
 * <li>This function is defined only when LOSCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the memory block to be allocated.
 * @param  size     [IN] Size of the memory block to be allocated (unit: byte).
 * @param  moduleId [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL       The memory fails to be allocated.
 * @retval #VOID*      The memory is successfully allocated, and the API returns the pointer to
 *                     the allocated memory block.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemMrealloc | LOS_MemMallocAlign | LOS_MemMfree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemMalloc(VOID *pool, UINT32 size, UINT32 moduleId);

/**
 * @ingroup los_memory
 * @brief Allocate aligned memory.
 *
 * @par Description:
 * This API is used to allocate memory blocks of specified size and of which the starting addresses are aligned on
 * a specified boundary and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The alignment parameter value must be a power of 2 with the minimum value being 4.</li>
 * <li>This function is defined only when LOSCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the memory blocks to be allocated.
 * @param  size      [IN] Size of the memory to be allocated.
 * @param  boundary  [IN] Boundary on which the memory is aligned.
 * @param  moduleId  [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL          The memory fails to be allocated.
 * @retval #VOID*         The memory is successfully allocated, and the API returns the pointer to the allocated memory.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemMalloc | LOS_MemRealloc | LOS_MemMfree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemMallocAlign(VOID *pool, UINT32 size, UINT32 boundary, UINT32 moduleId);

/**
 * @ingroup los_memory
 * @brief Free dynamic memory.
 *
 * @par Description:
 * This API is used to free specified dynamic memory that has been allocated and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The input ptr parameter must be allocated by LOS_MemMalloc or LOS_MemMallocAlign or LOS_MemMrealloc.</li>
 * <li>This function is defined only when LOSCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the dynamic memory block to be freed.
 * @param  ptr      [IN] Starting address of the memory block to be freed.
 * @param  moduleId [IN] module ID (0~MODULE_MAX).
 *
 * @retval #LOS_NOK          The memory block fails to be freed because the starting address of the memory block is
 * invalid, or the memory overwriting occurs.
 * @retval #LOS_OK           The memory block is freed successfully.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemMalloc | LOS_MemMrealloc | LOS_MemMallocAlign
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemMfree(VOID *pool, VOID *ptr, UINT32 moduleId);

/**
 * @ingroup los_memory
 * @brief Re-allocate a memory block.
 *
 * @par Description:
 * This API is used to allocate a new memory block of which the size is specified by size if the original memory
 * block size is insufficient. The new memory block will copy the data in the original memory block of which the
 * address is specified by ptr.The size of the new memory block determines the maximum size of data to be copied.
 * After the new memory block is created, the original one is freed. And update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The input ptr parameter must be allocated by LOS_MemMalloc or LOS_MemMallocAlign.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The size of the input parameter size must be aligned as follows: 1) if the ptr is allocated by LOS_MemAlloc,
 * it must be four byte-aligned; 2) if the ptr is allocated by LOS_MemMallocAlign, it must be aligned with the size of
 * the input parameter boundary of LOS_MemMallocAlign.</li>
 * <li>This function is defined only when LOSCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the original and new memory blocks.
 * @param  ptr       [IN] Address of the original memory block.
 * @param  size      [IN] Size of the new memory block.
 * @param  moduleId  [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL          The memory fails to be re-allocated.
 * @retval #VOID*         The memory is successfully re-allocated, and the API returns the pointer to
 *                        the new memory block.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemMalloc | LOS_MemMallocAlign | LOS_MemMfree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemMrealloc(VOID *pool, VOID *ptr, UINT32 size, UINT32 moduleId);

/**
 * @ingroup los_memory
 * @brief get the uesed memory size of the specified module.
 *
 * @par Description:
 * This API is used to get the specified module's memory consume size.
 * @attention This function is defined only when LOSCFG_MEM_MUL_MODULE is defined.
 *
 * @param  moduleId   [IN] module ID (0~MODULE_MAX).
 *
 * @retval #UINT32         The size of the specified module's consumed memory.
 * @retval #OS_NULL_INT    The input module id is illegal.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemMusedGet(UINT32 moduleId);
#endif

#ifdef LOSCFG_MEM_MUL_POOL
/**
 * @ingroup los_memory
 * @brief Deinitialize dynamic memory.
 *
 * @par Description:
 * This API is used to deinitialize the dynamic memory of a doubly linked list.
 * @attention This function is defined only when LOSCFG_MEM_MUL_POOL is defined.
 *
 * @param pool          [IN] Starting address of memory.
 *
 * @retval #LOS_NOK    The dynamic memory fails to be deinitialized.
 * @retval #LOS_OK     The dynamic memory is successfully deinitialized.
 * @par Dependency:
 * <ul>
 * <li>los_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemDeInit(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Print information about all pools.
 *
 * @par Description:
 * This API is used to print information about all pools.
 *
 * @attention This function is defined only when LOSCFG_MEM_MUL_POOL is defined.
 *
 * @param None.
 *
 * @retval #UINT32  The pool number.
 * @par Dependency:
 * <ul>
 * <li>los_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemPoolList(VOID);
#endif

/**
 * @ingroup los_memory
 * Memory pool extern information structure
 */
typedef struct {
    UINT32 uwTotalUsedSize;
    UINT32 uwTotalFreeSize;
    UINT32 uwMaxFreeNodeSize;
    UINT32 uwUsedNodeNum;
    UINT32 uwFreeNodeNum;
#ifdef LOSCFG_MEM_TASK_STAT
    UINT32 uwUsageWaterLine;  /**< this structure member is defined only when LOSCFG_MEM_TASK_STAT is defined. */
#endif
} LOS_MEM_POOL_STATUS;

/**
 * @ingroup los_memory
 * @brief Initialize dynamic memory.
 *
 * @par Description:
 * This API is used to initialize the dynamic memory of a doubly linked list.
 * @attention
 * <ul>
 * <li>The size parameter value should match the following two conditions :
 * 1) Be less than or equal to the Memory pool size;
 * 2) Be greater than the size of OS_MEM_MIN_POOL_SIZE.</li>
 * <li>Call this API when dynamic memory needs to be initialized during the startup of Huawei LiteOS.</li>
 * <li>The parameter input must be four byte-aligned.</li>
 * <li>The init area [pool, pool + size] should not conflict with other pools.</li>
 * </ul>
 *
 * @param pool         [IN] Starting address of memory.
 * @param size         [IN] Memory size.
 *
 * @retval #LOS_NOK    The dynamic memory fails to be initialized.
 * @retval #LOS_OK     The dynamic memory is successfully initialized.
 * @par Dependency:
 * <ul>
 * <li>los_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemInit(VOID *pool, UINT32 size);

/**
 * @ingroup los_memory
 * @brief Allocate dynamic memory.
 *
 * @par Description:
 * This API is used to allocate a memory block of which the size is specified.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The size of the input parameter size must be four byte-aligned.</li>
 * </ul>
 *
 * @param  pool    [IN] Pointer to the memory pool that contains the memory block to be allocated.
 * @param  size    [IN] Size of the memory block to be allocated (unit: byte).
 *
 * @retval #NULL          The memory fails to be allocated.
 * @retval #VOID*         The memory is successfully allocated, and the API returns the pointer to
 *                        the allocated memory block.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemRealloc | LOS_MemAllocAlign | LOS_MemFree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemAlloc(VOID *pool, UINT32 size);

/**
 * @ingroup los_memory
 * @brief Free dynamic memory.
 *
 * @par Description:
 * This API is used to free specified dynamic memory that has been allocated.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The input ptr parameter must be allocated by LOS_MemAlloc or LOS_MemAllocAlign or LOS_MemRealloc.</li>
 * </ul>
 *
 * @param  pool  [IN] Pointer to the memory pool that contains the dynamic memory block to be freed.
 * @param  ptr   [IN] Starting address of the memory block to be freed.
 *
 * @retval #LOS_NOK      The memory block fails to be freed because the starting address of the memory block is
 *                       invalid, or the memory overwriting occurs.
 * @retval #LOS_OK       The memory block is successfully freed.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemAlloc | LOS_MemRealloc | LOS_MemAllocAlign
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemFree(VOID *pool, VOID *ptr);

/**
 * @ingroup los_memory
 * @brief Re-allocate a memory block.
 *
 * @par Description:
 * This API is used to allocate a new memory block of which the size is specified by size if the original memory
 * block size is insufficient. The new memory block will copy the data in the original memory block of which the
 * address is specified by ptr. The size of the new memory block determines the maximum size of data to be copied.
 * After the new memory block is created, the original one is freed.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The input ptr parameter must be allocated by LOS_MemAlloc; if the ptr is allocated by LOS_MemAllocAlign,
 * the alignment of the newly allocated memory address cannot be guaranteed to be consistent with the original
 * memory address.</li>
 * <li>The input size parameter can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The input size parameter must be aligned as follows: 1) if the ptr is allocated by LOS_MemAlloc,
 * it must be four byte-aligned; 2) if the ptr is allocated by LOS_MemAllocAlign, it must be aligned with the size of
 * the input parameter boundary of LOS_MemAllocAlign.</li>
 * <li> If the user has special requirements for address alignment, it is not recommended to use this realloc function
 *  for address allocated by LOS_MemAllocAlign. For example, on a 32-bit system, LiteOS's default address alignment
 *  is 4 bytes, and if the boundary specified in LOS_MemAllocAlign is 8 bytes, it is cannot be ensured that the
 *  reallocated address is 8 bytes aligned.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the original and new memory blocks.
 * @param  ptr      [IN] Address of the original memory block.
 * @param  size     [IN] Size of the new memory block.
 *
 * @retval #NULL     The memory fails to be re-allocated.
 * @retval #VOID*    The memory is successfully re-allocated, and the API returns the pointer to the new memory block.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemAlloc | LOS_MemAllocAlign | LOS_MemFree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemRealloc(VOID *pool, VOID *ptr, UINT32 size);

/**
 * @ingroup los_memory
 * @brief Allocate aligned memory.
 *
 * @par Description:
 * This API is used to allocate memory blocks of specified size and of which the starting addresses are aligned on
 * a specified boundary.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of LOS_MemInit.</li>
 * <li>The alignment parameter value must be a power of 2 with the minimum value being 4.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the memory blocks to be allocated.
 * @param  size      [IN] Size of the memory to be allocated.
 * @param  boundary  [IN] Boundary on which the memory is aligned.
 *
 * @retval #NULL    The memory fails to be allocated.
 * @retval #VOID*   The memory is successfully allocated, and the API returns the pointer to the allocated memory.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemAlloc | LOS_MemRealloc | LOS_MemFree
 * @since Huawei LiteOS V100R001C00
 */
extern VOID *LOS_MemAllocAlign(VOID *pool, UINT32 size, UINT32 boundary);

/**
 * @ingroup los_memory
 * @brief Get the size of memory pool's size.
 *
 * @par Description:
 * This API is used to get the size of memory pool' total size.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK        The input parameter pool is NULL.
 * @retval #UINT32         The size of the memory pool.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemPoolSizeGet(const VOID *pool);

/**
 * @ingroup los_memory
 * @brief Get the size of memory totally used.
 *
 * @par Description:
 * This API is used to get the size of memory totally used in memory pool.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK        The input parameter pool is NULL.
 * @retval #UINT32         The size of the used memory pool.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemTotalUsedGet(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Get the number of free memory nodes.
 *
 * @par Description:
 * This API is used to get the number of free memory nodes in memory pool.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK        The input parameter pool is NULL.
 * @retval #UINT32         The number of free memory nodes.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemFreeBlksGet(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Get the number of used memory nodes.
 *
 * @par Description:
 * This API is used to get the number of used memory nodes in memory pool.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK        The input parameter pool is NULL.
 * @retval #UINT32         The number of used memory nodes.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemUsedBlksGet(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Get the task ID of a used memory node.
 *
 * @par Description:
 * This API is used to get the task ID of a used memory node.
 * @attention
 * <ul>
 * <li>The input ptr parameter must be allocated by LOS_MemAlloc or LOS_MemAllocAlign.</li>
 * <li>This interface only support obtain the task ID of a used memory node which is allocated from the system memory
 * pool (OS_SYS_MEM_ADDR) at present.</li>
 * </ul>
 *
 * @param  ptr               [IN] A used memory node.
 *
 * @retval #OS_INVALID        The input parameter ptr is illegal.
 * @retval #UINT32            The task ID of used memory node ptr.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemTaskIdGet(const VOID *ptr);

/**
 * @ingroup los_memory
 * @brief Get the address of last node.
 *
 * @par Description:
 * This API is used to get the address of last node.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The last node of memory pool is not the end node.</li>
 * </ul>
 *
 * @param  pool               [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK           The input parameter pool is NULL.
 * @retval #UINTPTR           The pointer to the last used node.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINTPTR LOS_MemLastUsedGet(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Get the information of memory pool.
 *
 * @par Description:
 * This API is used to get the information of memory pool.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool                 [IN] A pointer pointed to the memory pool.
 * @param  poolStatus           [IN] A pointer for storage the pool status
 *
 * @retval #LOS_NOK           The input parameter pool is NULL or invalid.
 * @retval #LOS_OK            Get memory information successfully.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemInfoGet(VOID *pool, LOS_MEM_POOL_STATUS *poolStatus);

/**
 * @ingroup los_memory
 * @brief Get the number of free node in every size.
 *
 * @par Description:
 * This API is used to print the number of free node in every size.
 * @attention
 * The input pool parameter must be initialized via func LOS_MemInit.
 *
 * @param  pool               [IN] A pointer pointed to the memory pool.
 *
 * @retval #LOS_NOK           The input parameter pool is NULL.
 * @retval #LOS_OK            Print the number of free node in every size successfully.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemFreeNodeShow(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Check the memory pool integrity.
 *
 * @par Description:
 * This API is used to check the memory pool integrity.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>LOS_MemIntegrityCheck will be called by malloc function when the macro of LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK
 * is defined in LiteOS.</li>
 * <li>LOS_MemIntegrityCheck function can be called by user anytime.</li>
 * </ul>
 *
 * @param  pool              [IN] A pointer to the memory pool.
 *
 * @retval #LOS_NOK           The memory pool (pool) is impaired.
 * @retval #LOS_OK            The memory pool (pool) is integrated.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemIntegrityCheck(VOID *pool);

/**
 * @ingroup los_memory
 * @brief Check the size of the specified memory node.
 *
 * @par Description:
 * This API is used to check the size of memory node.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func LOS_MemInit.</li>
 * <li>The input ptr parameter must be allocated by LOS_MemAlloc or LOS_MemAllocAlign.</li>
 * <li>The function will be called by function specified, such as memset or memcpy.</li>
 * <li>The feature can be enabled when you set the macro value of LOSCFG_BASE_MEM_NODE_SIZE_CHECK as YES.</li>
 * <li>You had better set memory check level as LOS_MEM_CHECK_LEVEL_DISABLE when copy bin file.</li>
 * </ul>
 *
 * @param  pool            [IN]  A pointer pointed to the memory pool.
 * @param  ptr             [IN]  A pointer pointed to the source node.
 * @param  totalSize       [OUT] A pointer to save total size, must point to valid memory.
 * @param  availSize       [OUT] A pointer to save available size, must point to valid memory.
 *
 * @retval #LOS_ERRNO_MEMCHECK_DISABLED         Memcheck function does not open.
 * @retval #LOS_ERRNO_MEMCHECK_PARA_NULL        The pool or ptr is NULL.
 * @retval #LOS_ERRNO_MEMCHECK_OUTSIDE          The ptr address is not in the reasonable range.
 * @retval #LOS_ERRNO_MEMCHECK_NO_HEAD          Can't find the control head node from ptr.
 * @retval #LOS_ERRNO_MEMCHECK_WRONG_LEVEL      The memory check level is illegal.
 * @retval #LOS_OK                              Success to get total size and available size of the memory node (ptr).
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemCheckLevelSet | LOS_MemCheckLevelGet
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemNodeSizeCheck(VOID *pool, VOID *ptr, UINT32 *totalSize, UINT32 *availSize);

/**
 * @ingroup los_memory
 * @brief Set the memory check level.
 *
 * @par Description:
 * This API is used to set the memory check level.
 * @attention
 * <ul>
 * <li>There are three level you can set.</li>
 * <li>The legal level are LOS_MEM_CHECK_LEVEL_LOW, LOS_MEM_CHECK_LEVEL_HIGH, LOS_MEM_CHECK_LEVEL_DISABLE.</li>
 * </ul>
 *
 * @param  checkLevel                               [IN] The level what you want to set.
 *
 * @retval #LOS_ERRNO_MEMCHECK_WRONG_LEVEL          The input memory check level is illegal.
 * @retval #LOS_OK                                  Set the memory check level successfully.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemNodeSizeCheck | LOS_MemCheckLevelGet
 * @since Huawei LiteOS V100R001C00
 */
extern UINT32 LOS_MemCheckLevelSet(UINT8 checkLevel);

/**
 * @ingroup los_memory
 * @brief Get the memory check level.
 *
 * @par Description:
 * This API is used to get the current memory check level.
 * @attention None.
 *
 * @param  None.
 *
 * @retval #UINT8           The current memory check level.
 * @par Dependency:
 * <ul><li>los_memory.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_MemNodeSizeCheck | LOS_MemCheckLevelSet
 * @since Huawei LiteOS V100R001C00
 */
extern UINT8 LOS_MemCheckLevelGet(VOID);

/**
 * @ingroup los_memory
 * Define a mem size check intensity
 *
 * Lowest mem check.
 */
#define LOS_MEM_CHECK_LEVEL_LOW 0

/**
 * @ingroup los_memory
 * Define a mem size check intensity
 *
 * Highest mem check.
 */
#define LOS_MEM_CHECK_LEVEL_HIGH 1

/**
 * @ingroup los_memory
 * Define a mem size check intensity
 *
 * disable mem check.
 */
#define LOS_MEM_CHECK_LEVEL_DISABLE 0xff

/**
 * @ingroup los_memory
 * Define a mem size check intensity.
 *
 * default intensity set mem check.
 */
#define LOS_MEM_CHECK_LEVEL_DEFAULT LOS_MEM_CHECK_LEVEL_DISABLE

/**
 * @ingroup los_memory
 * memcheck error code: the pointer or pool is NULL.
 *
 * Value: 0x02000101.
 *
 * Solution: don't give a NULL parameter.
 */
#define LOS_ERRNO_MEMCHECK_PARA_NULL LOS_ERRNO_OS_ERROR(LOS_MOD_MEM, 0x1)

/**
 * @ingroup los_memory
 * memcheck error code: the pointer address is not in the suitable range.
 *
 * Value: 0x02000102.
 *
 * Solution: check pointer and comfirm it is in stack.
 */
#define LOS_ERRNO_MEMCHECK_OUTSIDE LOS_ERRNO_OS_ERROR(LOS_MOD_MEM, 0x2)

/**
 * @ingroup los_memory
 * memcheck error code: can't find the control node.
 *
 * Value: 0x02000103.
 *
 * Solution: check if the node which the pointer points to has been freed or not been allocated.
 */
#define LOS_ERRNO_MEMCHECK_NO_HEAD LOS_ERRNO_OS_ERROR(LOS_MOD_MEM, 0x3)

/**
 * @ingroup los_memory
 * memcheck error code: the memcheck level is wrong.
 *
 * Value: 0x02000104.
 *
 * Solution: check the memcheck level by the function "LOS_MemCheckLevelGet".
 */
#define LOS_ERRNO_MEMCHECK_WRONG_LEVEL LOS_ERRNO_OS_ERROR(LOS_MOD_MEM, 0x4)

/**
 * @ingroup los_memory
 * memcheck error code: memcheck function is not enable.
 *
 * Value: 0x02000105.
 *
 * Solution: enable memcheck by the function "LOS_MemCheckLevelSet".
 */
#define LOS_ERRNO_MEMCHECK_DISABLED LOS_ERRNO_OS_ERROR(LOS_MOD_MEM, 0x5)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMORY_H */
