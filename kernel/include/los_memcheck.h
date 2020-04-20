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

#ifndef _LOS_MEMCHECK_H
#define _LOS_MEMCHECK_H

#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

enum _MEM_MANG_TYPE {
    MEM_MANG_MEMBOX,
    MEM_MANG_MEMORY,
    MEM_MANG_EMPTY,
};

enum _MEM_MANG_SOUCE {
    MEM_MANG_UNUSED,
    MEM_MANG_INIT,
    MEM_MANG_INT,
    MEM_MANG_TASK,
};

typedef struct _MEM_INFO {
    UINT32 uwType;
    UINT32 uwStartAddr;
    UINT32 uwSize;
    VOID *blkAddrArray;
} MEM_INFO;

typedef struct _SLAB_INFO {
    UINT32 item_sz;
    UINT32 item_cnt;
    UINT32 cur_usage;
} SLAB_INFO;

#define SLAB_CLASS_NUM                   4U
typedef struct _MEM_INFO_S {
    UINT32 uwType;
    UINT32 uwStartAddr;
    UINT32 uwSize;
    UINT32 uwFree;
    UINT32 uwBlockSize;
    UINT32 uwErrorAddr;
    UINT32 uwErrorLen;
    UINT32 uwErrorOwner;
    SLAB_INFO stSlabInfo[SLAB_CLASS_NUM];
} MEM_INFO_S;

/**
 * @ingroup los_memboxcheck
 * @brief Get the information of the exc memory.
 *
 * @par Description:
 * <ul>
 * <li>This API is used to get the information of the exc memory.</li>
 * </ul>
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @param memNum       [IN]     Type #UINT32  Memory pool number.
 * @param memExcInfo   [IN/OUT] Type #MEM_INFO_S *  information of the exc memory.
 *
 * @retval UINT32 Get information result.
 * @par Dependency:
 * <ul>
 * <li>los_memboxcheck.h: the header file that contains the API declaration.</li>
 * </ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
UINT32 LOS_MemExcInfoGet(UINT32 memNum, MEM_INFO_S *memExcInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MEMCHECK_H */
