/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Low-power Framework.
 * Author: Huawei LiteOS Team
 * Create: 2020-09-19
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

#ifndef _LOS_LOWPOWER_PRI_H
#define _LOS_LOWPOWER_PRI_H

#include "los_lowpower.h"
#include "los_task.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifdef LOSCFG_KERNEL_LOWPOWER

#define CALL_FUNC_NO_RETURN_FROM_PTR(ops, func, ...) do { \
        if (((ops) != NULL) && ((ops)->func != NULL)) {   \
            (ops)->func(__VA_ARGS__);                     \
        }                                                 \
    } while (0)

#define CALL_FUNC_NO_RETURN(ops, func, ...) do { \
        if (((ops).func != NULL)) {              \
            (ops).func(__VA_ARGS__);             \
        }                                        \
    } while (0)

#define CALL_FUNC_WITH_RETURN_FROM_PTR(ops, ret, func, ...) do { \
        if (((ops) != NULL) && ((ops)->func != NULL)) {          \
            (ret) = (ops)->func(__VA_ARGS__);                    \
        }                                                        \
    } while (0)

#define CALL_FUNC_WITH_RETURN(ops, ret, func, ...) do { \
        if (((ops).func != NULL)) {                     \
            (ret) = (ops).func(__VA_ARGS__);            \
        }                                               \
    } while (0)

typedef VOID (*VoidFuntionType)(VOID);
typedef UINT32 (*Uint32FuntionType)(VOID);

STATIC INLINE VOID CallVoidFunction(VOID *obj, UINTPTR offset)
{
    if (obj != NULL) {
        VoidFuntionType func = (VoidFuntionType)((UINTPTR)obj + offset);
        if (func != NULL) {
            func();
        }
    }
}

STATIC INLINE VOID CallFunctionReturnUint32(VOID *obj, UINT32 *ret, UINTPTR offset)
{
    if (obj != NULL) {
        Uint32FuntionType func = (Uint32FuntionType)((UINTPTR)obj + offset);
        if (func != NULL) {
            *ret = func();
        }
    }
}

#else

#define CALL_FUNC_NO_RETURN_FROM_PTR(ops, func, ...)
#define CALL_FUNC_NO_RETURN(ops, func, ...)
#define CALL_FUNC_WITH_RETURN_FROM_PTR(ops, ret, func, ...)
#define CALL_FUNC_WITH_RETURN(ops, ret, func, ...)

typedef VOID (*VoidFuntionType)(VOID);
typedef UINT32 (*Uint32FuntionType)(VOID);

STATIC INLINE VOID CallVoidFunction(VOID *obj, UINTPTR offset)
{
    (void)obj;
    (void)offset;
}

STATIC INLINE VOID CallFunctionReturnUint32(VOID *obj, UINT32 *ret, UINTPTR offset)
{
    (void)obj;
    (void)ret;
    (void)offset;
}
#endif

extern VOID OsPowerMgrProcess(VOID);

extern VOID OsPowerMgrWakeUpFromInterrupt(UINT32 intNum);

extern VOID OsPowerMgrWakeupFromReset(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif // _LOS_LOWPOWER_PRI_H
