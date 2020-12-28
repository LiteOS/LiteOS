/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS LMS Module Private HeadFile
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

#ifndef _LOS_LMS_PRI_H
#define _LOS_LMS_PRI_H

#include "los_lms.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_LMS

#define POINTER_SIZE                      4

#define STORE_ERRMODE                     1
#define LOAD_ERRMODE                      0
#define SANITIZER_INTERFACE_ATTRIBUTE
#define ATTRIBUTE_NO_SANITIZE_ADDRESS     __attribute__((no_sanitize_address))
#define SHADOW_BYTE_LOW_ZERO              0xfc
#define SHADOW_BYTE_MIDLOW_ZERO           0xf3
#define SHADOW_BYTE_MIDHIGH_ZERO          0xcf
#define SHADOW_BYTE_HIGH_ZERO             0x3f
#define SHADOW_BYTE_LOW_ONE               0x01
#define SHADOW_BYTE_MIDLOW_ONE            0x04
#define SHADOW_BYTE_MIDHIGH_ONE           0x10
#define SHADOW_BYTE_HIGH_ONE              0x40
#define SHADOW_BYTE_LOW_THREE             0x03
#define SHADOW_BYTE_MIDLOW_THREE          0x0c
#define SHADOW_BYTE_MIDHIGH_THREE         0x30
#define SHADOW_BYTE_HIGH_THREE            0xc0

#define LMS_SD_MEMORY_RATIO               16
#define LMS_ALIGN_BYTE                    4

VOID OsLmsSetAfterMalloc(const VOID *ptr);
VOID OsLmsSetAfterFree(const VOID *ptr);
UINT32 OsLmsMemInit(VOID *pool, UINT32 size);

UINT8 OsLmsAddrIsPoisoned(UINTPTR p);
VOID OsLmsReportError(UINTPTR p, UINT8 sd, UINT32 errMod);

VOID CheckValid(const CHAR *dest, const CHAR *src);

extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_store1_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_store4_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_load4_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_load1_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_loadN_noabort(UINTPTR p, UINT32 size);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_storeN_noabort(UINTPTR p, UINT32 size);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_store2_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_load2_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_store8_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_load8_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_load16_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_store16_noabort(UINTPTR p);
extern SANITIZER_INTERFACE_ATTRIBUTE VOID __asan_handle_no_return(VOID);

#else /* ! LOSCFG_KERNEL_LMS */
STATIC INLINE VOID OsLmsSetAfterMalloc(const VOID *ptr)
{
}

STATIC INLINE VOID OsLmsSetAfterFree(const VOID *ptr)
{
}

STATIC INLINE UINT32 OsLmsMemInit(VOID *pool, UINT32 size)
{
    return 0;
}
#endif /* LOSCFG_KERNEL_LMS */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_LMS_PRI_H */
