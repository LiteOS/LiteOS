/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Aarch64 Atomic HeadFile
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

#ifndef _ARCH_ATOMIC_H
#define _ARCH_ATOMIC_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC INLINE INT32 ArchAtomicRead(const Atomic *v)
{
    return *(volatile INT32 *)v;
}

STATIC INLINE VOID ArchAtomicSet(Atomic *v, INT32 setVal)
{
    *(volatile INT32 *)v = setVal;
}

STATIC INLINE INT32 ArchAtomicAdd(Atomic *v, INT32 addVal)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w1, %2\n"
                             "add   %w1, %w1, %w3\n"
                             "stxr   %w0, %w1, %2"
                             : "=&r"(status), "=&r"(val), "+Q"(*v)
                             : "r"(addVal)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE INT32 ArchAtomicSub(Atomic *v, INT32 subVal)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w1, %2\n"
                             "sub   %w1, %w1, %w3\n"
                             "stxr   %w0, %w1, %2"
                             : "=&r"(status), "=&r"(val), "+Q"(*v)
                             : "r"(subVal)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE VOID ArchAtomicInc(Atomic *v)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w0, %2\n"
                             "add   %w0, %w0, #1\n"
                             "stxr   %w1, %w0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));
}

STATIC INLINE INT32 ArchAtomicIncRet(Atomic *v)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w0, %2\n"
                             "add   %w0, %w0, #1\n"
                             "stxr   %w1, %w0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE VOID ArchAtomicDec(Atomic *v)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w0, %2\n"
                             "sub   %w0, %w0, #1\n"
                             "stxr   %w1, %w0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));
}

STATIC INLINE INT32 ArchAtomicDecRet(Atomic *v)
{
    INT32 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w0, %2\n"
                             "sub   %w0, %w0, #1\n"
                             "stxr   %w1, %w0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE INT64 ArchAtomic64Read(const Atomic64 *v)
{
    return *(volatile INT64 *)v;
}

STATIC INLINE VOID ArchAtomic64Set(Atomic64 *v, INT64 setVal)
{
    *(volatile INT64 *)v = setVal;
}

STATIC INLINE INT64 ArchAtomic64Add(Atomic64 *v, INT64 addVal)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %1, %2\n"
                             "add   %1, %1, %3\n"
                             "stxr   %w0, %1, %2"
                             : "=&r"(status), "=&r"(val), "+Q"(*v)
                             : "r"(addVal)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE INT64 ArchAtomic64Sub(Atomic64 *v, INT64 subVal)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %1, %2\n"
                             "sub   %1, %1, %3\n"
                             "stxr   %w0, %1, %2"
                             : "=&r"(status), "=&r"(val), "+Q"(*v)
                             : "r"(subVal)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE VOID ArchAtomic64Inc(Atomic64 *v)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %0, %2\n"
                             "add   %0, %0, #1\n"
                             "stxr   %w1, %0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));
}

STATIC INLINE INT64 ArchAtomic64IncRet(Atomic64 *v)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %0, %2\n"
                             "add   %0, %0, #1\n"
                             "stxr   %w1, %0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE VOID ArchAtomic64Dec(Atomic64 *v)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %0, %2\n"
                             "sub   %0, %0, #1\n"
                             "stxr   %w1, %0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));
}

STATIC INLINE INT64 ArchAtomic64DecRet(Atomic64 *v)
{
    INT64 val;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %0, %2\n"
                             "sub   %0, %0, #1\n"
                             "stxr   %w1, %0, %2"
                             : "=&r"(val), "=&r"(status), "+Q"(*v));
    } while (__builtin_expect(status != 0, 0));

    return val;
}

STATIC INLINE INT32 ArchAtomicXchg32bits(Atomic *v, INT32 val)
{
    INT32 prevVal;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %w0, %w2\n"
                             "stxr   %w1, %w3, %w2"
                             : "=&r"(prevVal), "=&r"(status), "+Q"(*v)
                             : "r"(val)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return prevVal;
}

STATIC INLINE INT64 ArchAtomicXchg64bits(Atomic64 *v, INT64 val)
{
    INT64 prevVal;
    UINT32 status;

    do {
        __asm__ __volatile__("ldxr   %0, %2\n"
                             "stxr   %w1, %3, %2"
                             : "=&r"(prevVal), "=&r"(status), "+Q"(*v)
                             : "r"(val)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return prevVal;
}

STATIC INLINE BOOL ArchAtomicCmpXchg32bits(Atomic *v, INT32 val, INT32 oldVal)
{
    INT32 prevVal;
    UINT32 status;

    do {
        __asm__ __volatile__("1: ldxr %w0, %w2\n"
                             "    mov %w1, #0\n"
                             "    cmp %w0, %w3\n"
                             "    b.ne 2f\n"
                             "    stxr %w1, %w4, %w2\n"
                             "2:"
                             : "=&r"(prevVal), "=&r"(status), "+Q"(*v)
                             : "r"(oldVal), "r"(val)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return prevVal != oldVal;
}

STATIC INLINE BOOL ArchAtomicCmpXchg64bits(Atomic64 *v, INT64 val, INT64 oldVal)
{
    INT64 prevVal;
    UINT32 status;

    do {
        __asm__ __volatile__("1: ldxr %0, %2\n"
                             "    mov %w1, #0\n"
                             "    cmp %0, %3\n"
                             "    b.ne 2f\n"
                             "    stxr %w1, %4, %2\n"
                             "2:"
                             : "=&r"(prevVal), "=&r"(status), "+Q"(*v)
                             : "r"(oldVal), "r"(val)
                             : "cc");
    } while (__builtin_expect(status != 0, 0));

    return prevVal != oldVal;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_ATOMIC_H */
