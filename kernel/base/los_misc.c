/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Misc Functions
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
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#include "los_task_pri.h"
#include "los_misc_pri.h"
#include "los_memory_pri.h"
#ifdef LOSCFG_SHELL_EXCINFO
#include "los_excinfo_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

LITE_OS_SEC_TEXT UINTPTR LOS_Align(UINTPTR addr, UINT32 boundary)
{
    return (addr + boundary - 1) & ~((UINTPTR)(boundary - 1));
}

LITE_OS_SEC_TEXT_MINOR VOID LOS_Msleep(UINT32 msecs)
{
    UINT32 interval;

    if (msecs == 0) {
        interval = 0;
    } else {
        interval = LOS_MS2Tick(msecs);
        if (interval == 0) {
            interval = 1;
        }
    }

    (VOID)LOS_TaskDelay(interval);
}

VOID OsDumpMemByte(size_t length, UINTPTR addr)
{
    size_t dataLen;
    UINTPTR *alignAddr = NULL;
    UINT32 count = 0;

    dataLen = ALIGN(length, sizeof(UINTPTR));
    alignAddr = (UINTPTR *)TRUNCATE(addr, sizeof(UINTPTR));
    if ((dataLen == 0) || (alignAddr == NULL)) {
        return;
    }
    while (dataLen) {
        if (IS_ALIGNED(count, sizeof(CHAR *))) {
            PRINTK("\n 0x%lx :", alignAddr);
#ifdef LOSCFG_SHELL_EXCINFO
            WriteExcInfoToBuf("\n 0x%lx :", alignAddr);
#endif
        }
#ifdef __LP64__
        PRINTK("%0+16lx ", *alignAddr);
#else
        PRINTK("%0+8lx ", *alignAddr);
#endif
#ifdef LOSCFG_SHELL_EXCINFO
#ifdef __LP64__
        WriteExcInfoToBuf("0x%0+16x ", *alignAddr);
#else
        WriteExcInfoToBuf("0x%0+8x ", *alignAddr);
#endif
#endif
        alignAddr++;
        dataLen -= sizeof(CHAR *);
        count++;
    }
    PRINTK("\n");
#ifdef LOSCFG_SHELL_EXCINFO
    WriteExcInfoToBuf("\n");
#endif

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
