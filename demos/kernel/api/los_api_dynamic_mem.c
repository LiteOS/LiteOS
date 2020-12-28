/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Dynamic Memory Demo
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

#include <stdlib.h>
#include "los_config.h"
#include "los_memory.h"
#include "los_api_dynamic_mem.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_ARCH_ARM_CORTEX_A
#define MEM_DYN_SIZE  (256 * 4)
#else
#define MEM_DYN_SIZE  256
#endif

static UINT32 g_demoDynMem[MEM_DYN_SIZE / 4];

UINT32 Example_Dyn_Mem(VOID)
{
    UINT32 *mem = NULL;
    UINT32 ret;
    printf("Kernel dynamic memory demo begin.\n");
    ret = LOS_MemInit(g_demoDynMem, MEM_DYN_SIZE);
    if (ret == LOS_OK) {
        printf("Mempool init ok.\n");
    } else {
        printf("Mempool init failed.\n");
        return LOS_NOK;
    }

    /* mem alloc */
    mem = (UINT32 *)LOS_MemAlloc(g_demoDynMem, 4);
    if (mem == NULL) {
        printf("Mem alloc failed.\n");
        return LOS_NOK;
    }
    printf("Mem alloc ok.\n");

    /* assignment */
    *mem = 828;
    printf("*mem = %d.\n", *mem);

    /* mem free */
    ret = LOS_MemFree(g_demoDynMem, mem);
    if (ret == LOS_OK) {
        printf("Mem free ok.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_DMEM, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    } else {
        printf("Mem free failed.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_DMEM, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
        return LOS_NOK;
    }
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
