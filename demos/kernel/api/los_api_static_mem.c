/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel Static Memory Demo
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

#include <stdio.h>
#include "los_membox.h"
#include "los_api_static_mem.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 g_demoBoxMem[144];
UINT32 Example_StaticMem(VOID)
{
    UINT32 *mem = NULL;
    UINT32 blksize = 3;
    UINT32 boxsize = 48;
    UINT32 ret;

    printf("Kernel static memory demo begin.\n");
    ret = LOS_MemboxInit(&g_demoBoxMem[0], boxsize, blksize);
    if (ret != LOS_OK) {
        printf("Mem box init failed.\n");
        return LOS_NOK;
    } else {
        printf("Mem box init ok.\n");
    }

    /* membox alloc */
    mem = (UINT32*)LOS_MemboxAlloc(g_demoBoxMem);
    if (mem == NULL) {
        printf("Mem box alloc failed.\n");
        return LOS_NOK;
    }
    printf("Mem box alloc ok.\n");
    /* assignment */
    *mem = 828;
    printf("*mem = %d.\n", *mem);
    /* clear mem context */
    LOS_MemboxClr(g_demoBoxMem, mem);
    printf("Clear data ok, *mem = %d.\n", *mem);
    /* membox free */
    ret = LOS_MemboxFree(g_demoBoxMem, mem);
    if (ret == LOS_OK) {
        printf("Mem box free ok.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_SMEM, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set InspectStatus Err.\n");
        }
    } else {
        printf("Mem box free failed.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_SMEM, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set InspectStatus Err.\n");
        }
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
