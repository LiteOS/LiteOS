/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Type define
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

/**
 * @defgroup los_typedef Type define
 * @ingroup kernel
 */

#ifndef _LOS_TYPEDEF_H
#define _LOS_TYPEDEF_H
#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"
#include "los_builddef.h"
#include "los_toolchain.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_STRING(x)  #x
#define X_STRING(x) OS_STRING(x)

/* type definitions */
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef signed char        INT8;
typedef signed short       INT16;
typedef signed int         INT32;
typedef float              FLOAT;
typedef double             DOUBLE;
typedef char               CHAR;

#ifdef __LP64__
typedef long unsigned int  UINT64;
typedef long signed int    INT64;
typedef unsigned long      UINTPTR;
typedef signed long        INTPTR;
#else
typedef unsigned long long UINT64;
typedef signed long long   INT64;
typedef unsigned int       UINTPTR;
typedef signed int         INTPTR;
#endif

#ifdef __LP64__
typedef __uint128_t        UINT128;
typedef INT64              ssize_t;
typedef UINT64             size_t;
#define LOSCFG_AARCH64
#else
typedef INT32              ssize_t;
typedef UINT32             size_t;
#endif

typedef UINTPTR            AARCHPTR;
typedef size_t             BOOL;

#define VOID               void
#define STATIC             static

#ifndef FALSE
#define FALSE              0U
#endif

#ifndef TRUE
#define TRUE               1U
#endif

#ifndef NULL
#define NULL               ((VOID *)0)
#endif

#ifdef YES
#undef YES
#endif
#define YES                1

#ifdef NO
#undef NO
#endif
#define NO                 0

#define OS_NULL_BYTE       ((UINT8)0xFF)
#define OS_NULL_SHORT      ((UINT16)0xFFFF)
#define OS_NULL_INT        ((UINT32)0xFFFFFFFF)

#ifndef LOS_OK
#define LOS_OK             0
#endif

#ifndef LOS_NOK
#define LOS_NOK            1
#endif

#define OS_FAIL            1
#define OS_ERROR           (UINT32)(-1)
#define OS_INVALID         (UINT32)(-1)

#define asm __asm
#ifdef typeof
#undef typeof
#endif
#define typeof __typeof__

#ifndef LOS_LABEL_DEFN
#define LOS_LABEL_DEFN(label) label
#endif

#ifndef LOSARC_ALIGNMENT
#define LOSARC_ALIGNMENT 8
#endif
/* And corresponding power of two alignment */
#ifndef LOSARC_P2ALIGNMENT
#ifdef LOSCFG_AARCH64
#define LOSARC_P2ALIGNMENT 3
#else
#define LOSARC_P2ALIGNMENT 2
#endif
#endif

/* Give a type or object explicit minimum alignment */
#if !defined(LOSBLD_ATTRIB_ALIGN)
#define LOSBLD_ATTRIB_ALIGN(__align__) __attribute__((aligned(__align__)))
#endif

/* Assign a defined variable to a specific section */
#if !defined(LOSBLD_ATTRIB_SECTION)
#define LOSBLD_ATTRIB_SECTION(__sect__) __attribute__((section(__sect__)))
#endif

/*
 * Tell the compiler not to throw away a variable or function. Only known
 * available on 3.3.2 or above. Old version's didn't throw them away,
 * but using the unused attribute should stop warnings.
 */
#define LOSBLD_ATTRIB_USED __attribute__((used))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_TYPEDEF_H */
