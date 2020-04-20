/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: IPC Debug
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

#include "los_ipcdebug_pri.h"
#include "sys_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if defined(LOSCFG_DEBUG_SEMAPHORE) || defined(LOSCFG_DEBUG_MUTEX) || defined(LOSCFG_DEBUG_QUEUE)

VOID OsArraySortByTime(UINT32 *sortArray, UINT32 start, UINT32 end, const IpcSortParam *sortParam,
                       OsCompareFunc compareFunc)
{
    UINT32 left = start;
    UINT32 right = end;
    UINT32 idx = start;
    UINT32 pivot = sortArray[start];

    while (left < right) {
        while ((left < right) && (sortArray[right] < sortParam->ipcDebugCBCnt) && (pivot < sortParam->ipcDebugCBCnt) &&
               compareFunc(sortParam, sortArray[right], pivot)) {
            right--;
        }

        if (left < right) {
            sortArray[left] = sortArray[right];
            idx = right;
            left++;
        }

        while ((left < right) && (sortArray[left] < sortParam->ipcDebugCBCnt) && (pivot < sortParam->ipcDebugCBCnt) &&
               compareFunc(sortParam, pivot, sortArray[left])) {
            left++;
        }

        if (left < right) {
            sortArray[right] = sortArray[left];
            idx = left;
            right--;
        }
    }

    sortArray[idx] = pivot;

    if (start < idx) {
        OsArraySortByTime(sortArray, start, idx - 1, sortParam, compareFunc);
    }
    if (idx < end) {
        OsArraySortByTime(sortArray, idx + 1, end, sortParam, compareFunc);
    }
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
