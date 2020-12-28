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

/**
 * @defgroup los_slab Slab
 * @ingroup kernel
 */

#ifndef _LOS_SLAB_H
#define _LOS_SLAB_H

#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

// max size of each class
#define SLAB_MEM_ALLOCATOR_SIZE     512U

#define SLAB_BASIC_NEED_SIZE        0x1000

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
enum SlabBucketSizeType {
    SLAB_BUCKET_SIZE_HALF_OF_HEAP    = 0x1UL,   /* a half of heap size slab mem can use */
    SLAB_BUCKET_SIZE_QUARTER_OF_HEAP,           /* one quarter of heap size slab mem can use */
    SLAB_BUCKET_SIZE_TYPE_MAX
};

#define SLAB_MEM_BUCKET_SIZE_TYPE   SLAB_BUCKET_SIZE_HALF_OF_HEAP
#endif

/**
 * @ingroup los_slab
 * @brief configure slab size.
 *
 * @par Description:
 * This API is used to configure slab size.
 * @attention
 * <ul>
 * <li>The function should be called before function LOS_MemInit if necessary.</li>
 * <li>The function takes effect when LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE is NO.</li>
 * </ul>
 *
 * @param cfg     [IN] the pointer to slab size config table. Note that the pointer cannot be null.
 * @param cnt     [IN] slab class size, must be equal to SLAB_MEM_COUNT(the number of slab class).
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>los_slab.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R003C00
 */
extern VOID LOS_SlabSizeCfg(UINT32 *cfg, UINT32 cnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SLAB_H */
