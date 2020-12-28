/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Multipledlinkhead Private HeadFile
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

#ifndef _LOS_MULTIPLE_DLINK_HEAD_PRI_H
#define _LOS_MULTIPLE_DLINK_HEAD_PRI_H

#include "los_base.h"
#include "los_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MAX_MULTI_DLNK_LOG2  29
#define OS_MIN_MULTI_DLNK_LOG2  4
#define OS_MULTI_DLNK_NUM       ((OS_MAX_MULTI_DLNK_LOG2 - OS_MIN_MULTI_DLNK_LOG2) + 1)
#define OS_DLNK_HEAD_SIZE       OS_MULTI_DLNK_HEAD_SIZE
#define OS_MULTI_DLNK_HEAD_SIZE sizeof(LosMultipleDlinkHead)

typedef struct {
    LOS_DL_LIST listHead[OS_MULTI_DLNK_NUM];
} LosMultipleDlinkHead;

STATIC INLINE LOS_DL_LIST *OsDLnkNextMultiHead(VOID *headAddr, LOS_DL_LIST *listNodeHead)
{
    LosMultipleDlinkHead *head = (LosMultipleDlinkHead *)headAddr;

    return (&head->listHead[OS_MULTI_DLNK_NUM - 1] == listNodeHead) ? NULL : (listNodeHead + 1);
}

extern VOID OsDLnkInitMultiHead(VOID *headAddr);
extern LOS_DL_LIST *OsDLnkMultiHead(VOID *headAddr, UINT32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_MULTIPLE_DLINK_HEAD_PRI_H */
