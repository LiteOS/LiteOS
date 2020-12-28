/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stack Info Implementation
 * Author: Huawei LiteOS Team
 * Create: 2019-09-01
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

#include "securec.h"
#include "los_stackinfo_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

const StackInfo *g_stackInfo = NULL;
UINT32 g_stackNum;

UINT32 OsStackWaterLineGet(const UINTPTR *stackBottom, const UINTPTR *stackTop, UINT32 *peakUsed)
{
    UINT32 size;
    const UINTPTR *tmp = NULL;
    if (*stackTop == OS_STACK_MAGIC_WORD) {
        tmp = stackTop + 1;
        while ((tmp < stackBottom) && (*tmp == OS_STACK_INIT)) {
            tmp++;
        }
        size = (UINT32)((UINTPTR)stackBottom - (UINTPTR)tmp);
        *peakUsed = (size == 0) ? size : (size + sizeof(CHAR *));
        return LOS_OK;
    } else {
        *peakUsed = OS_INVALID_WATERLINE;
        return LOS_NOK;
    }
}

VOID OsExcStackInfoReg(const StackInfo *stackInfo, UINT32 stackNum)
{
    g_stackInfo = stackInfo;
    g_stackNum = stackNum;
}

VOID OsStackInit(VOID *stacktop, UINT32 stacksize)
{
    /* initialize the task stack, write magic num to stack top */
    (VOID)memset_s(stacktop, stacksize, (INT32)OS_STACK_INIT, stacksize);
    *((UINTPTR *)stacktop) = OS_STACK_MAGIC_WORD;
}

VOID OsGetStackInfo(const StackInfo **stackInfo, UINT32 *stackNum)
{
    *stackInfo = g_stackInfo;
    *stackNum = g_stackNum;
}
