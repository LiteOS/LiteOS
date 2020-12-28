/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Lite Memory Sanitizer HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-09-01
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

#ifndef _LOS_LMS_H
#define _LOS_LMS_H

#include "stdint.h"
#include "stddef.h"
#include "los_typedef.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_LMS
// Start: Kernel Address Sanitizer(KASAN) for LiteOS
// Redefine memcpy and other memory opration functions to enable KASAN check ability for LiteOS
// This is a debug feature and only available when LOSCFG_KERNEL_LMS is configured to Y
#define memcpy                            KasanMemcpy
#define memmove                           KasanMemmove
#define strcat                            KasanStrcat
#define strcpy                            KasanStrcpy

#define memcpy_s                          KasanMemcpySec
#define memmove_s                         KasanMemmoveSec
#define strcat_s                          KasanStrcatSec
#define strcpy_s                          KasanStrcpySec
// End： Kernel Address Sanitizer(KASAN) for LiteOS

VOID *KasanMemcpy(VOID *__restrict dest, const VOID *__restrict src, size_t copyAmount);
VOID *KasanMemmove(VOID *dest, const VOID *src, size_t len);
CHAR *KasanStrcat(CHAR *s, const CHAR *append);
CHAR *KasanStrcpy(CHAR *dest, const CHAR *src);

errno_t KasanMemcpySec(VOID *dest, size_t destMax, const VOID *src, size_t copyAmount);
errno_t KasanMemmoveSec(VOID *dest, size_t destMax, const VOID *src, size_t len);
errno_t KasanStrcatSec(CHAR *s, size_t destMax, const CHAR *append);
errno_t KasanStrcpySec(CHAR *dest, size_t destMax, const CHAR *src);

#endif /* LOSCFG_KERNEL_LMS */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* _LOS_LMS_H */
