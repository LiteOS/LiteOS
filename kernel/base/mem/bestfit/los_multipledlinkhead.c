/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Module Implementation
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

#include "los_multipledlinkhead_pri.h"
#include "los_bitmap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC INLINE UINT32 OsLog2(UINT32 size)
{
    return (size > 0) ? (UINT32)LOS_HighBitGet(size) : 0;
}

LITE_OS_SEC_TEXT_INIT VOID OsDLnkInitMultiHead(VOID *headAddr)
{
    LosMultipleDlinkHead *dlinkHead = (LosMultipleDlinkHead *)headAddr;
    LOS_DL_LIST *listNodeHead = dlinkHead->listHead;
    UINT32 index;

    for (index = 0; index < OS_MULTI_DLNK_NUM; ++index, ++listNodeHead) {
        LOS_ListInit(listNodeHead);
    }
}

LITE_OS_SEC_TEXT_MINOR LOS_DL_LIST *OsDLnkMultiHead(VOID *headAddr, UINT32 size)
{
    LosMultipleDlinkHead *dlinkHead = (LosMultipleDlinkHead *)headAddr;
    UINT32 index = OsLog2(size);
    if (index > OS_MAX_MULTI_DLNK_LOG2) {
        return NULL;
    } else if (index <= OS_MIN_MULTI_DLNK_LOG2) {
        index = OS_MIN_MULTI_DLNK_LOG2;
    }

    return dlinkHead->listHead + (index - OS_MIN_MULTI_DLNK_LOG2);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
