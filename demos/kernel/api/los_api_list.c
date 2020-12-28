/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS Kernel List Demo
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

#include "los_list.h"
#include <stdio.h>
#include <stdlib.h>
#include "los_config.h"
#include "los_memory.h"
#include "los_api_list.h"
#include "los_inspect_entry.h"

#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 Example_List(VOID)
{
    UINT32 ret;

    /* init */
    printf("Kernel list demo begin.\n");
    printf("Init list......\n");
    LOS_DL_LIST *head;
    head = (LOS_DL_LIST *)LOS_MemAlloc(m_aucSysMem0, sizeof(LOS_DL_LIST));
    if (head == NULL) {
        printf("Malloc failed.\n");
        return LOS_NOK;
    }

    LOS_ListInit(head);
    if (!LOS_ListEmpty(head)) {
        printf("Init list failed.\n");
        return LOS_NOK;
    }

    /* tail insert node */
    printf("Node add and tail add......\n");

    LOS_DL_LIST *node1 = (LOS_DL_LIST *)LOS_MemAlloc(m_aucSysMem0, sizeof(LOS_DL_LIST));
    if (node1 == NULL) {
        printf("Malloc failed\n");
        return LOS_NOK;
    }

    LOS_DL_LIST *node2 = (LOS_DL_LIST *)LOS_MemAlloc(m_aucSysMem0, sizeof(LOS_DL_LIST));
    if (node2 == NULL) {
        printf("Malloc failed.\n");
        LOS_MemFree(m_aucSysMem0, node1);
        return LOS_NOK;
    }

    LOS_DL_LIST *tail = (LOS_DL_LIST *)LOS_MemAlloc(m_aucSysMem0, sizeof(LOS_DL_LIST));
    if (tail == NULL) {
        printf("Malloc failed.\n");
        LOS_MemFree(m_aucSysMem0, node1);
        LOS_MemFree(m_aucSysMem0, node2);
        return LOS_NOK;
    }

    LOS_ListAdd(head, node1);
    LOS_ListAdd(node1, node2);
    if ((node1->pstPrev == head) || (node2->pstPrev == node1)) {
        printf("Add node ok.\n");
    }

    LOS_ListTailInsert(head, tail);
    if (tail->pstPrev == node2) {
        printf("Add tail ok.\n");
    }

    /* delete node */
    printf("Delete node......\n");
    LOS_ListDelete(node1);
    LOS_MemFree(m_aucSysMem0, node1);
    if (head->pstNext == node2) {
        printf("Delete node ok.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_LIST, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    } else {
        printf("Delete node failed.\n");
        ret = LOS_InspectStatusSetById(LOS_INSPECT_LIST, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set Inspect Status Err.\n");
        }
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
