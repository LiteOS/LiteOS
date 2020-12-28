/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description : LiteOS arm-m flash patch module implemention.
 * Author : Huawei LiteOS Team
 * Create : 2018-03-07
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
 * ---------------------------------------------------------------------------- */
#include "stdint.h"
#include "los_errno.h"

#ifndef _ARCH_FPB_H
#define _ARCH_FPB_H

#define FPB_SUCCESS              LOS_OK
#define FPB_COMP_REPEAT_ERR      LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x00)
#define FPB_NO_COMP_ERR          LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x01)
#define FPB_TYPE_ERR             LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x02)
#define FPB_NO_FREE_COMP_ERR     LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x03)
#define FPB_ADDR_NOT_ALIGN_ERR   LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x04)
#define FPB_TARGET_ADDR_ERR      LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x05)
#define FPB_BUSY_ERR             LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x06)
#define FPB_ERROR_INPUT_ERR      LOS_ERRNO_OS_ERROR(LOS_MOD_FPB, 0x07)

typedef enum {
    FPB_TYPE_INSTR        = 0,
    FPB_TYPE_LITERAL      = 1,
    FPB_TYPE_MAX
} FpbCompTypeEnum;

void FpbInit(void);

uint32_t FpbAddPatch(uint32_t oldAddr, uint32_t patchValue, FpbCompTypeEnum fpbType);

uint32_t FpbDeletePatch(uint32_t oldAddr, FpbCompTypeEnum fpbType);

void FpbDisable(void);

void FpbLock(void);

#endif /* _ARCH_FPB_H */
