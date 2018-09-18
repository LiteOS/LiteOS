/*
 * Copyright (c) 2017-2018 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== errno.h ========
 */

#ifndef ti_posix_ccs_errno__include
#define ti_posix_ccs_errno__include

/* compiler vendor check */
#ifndef __TI_COMPILER_VERSION__
#error Incompatible compiler: use this include path (.../ti/posix/ccs) only \
with a Texas Instruments compiler. You appear to be using a different compiler.
#endif

/*  Determine which TI toolchain is in use (ARM, C6x, ...) because
 *  each one has different errno support.
 *
 *  TI ARM Compiler
 */
#if defined(__TMS470__)

#if __TI_COMPILER_VERSION__ < 18001000
#ifdef __cplusplus
extern "C" {
#endif

/*  To enable thread-safe errno, set _AEABI_PORTABILITY_LEVEL to
 *  a none-zero value. When using ti.posix (i.e. this file), the
 *  default is to enable thread-safe errno. However, the TI compiler
 *  does not do this. It uses one global errno symbol by default.
 *  Must define this symbol before including errno.h.
 */
#define _AEABI_PORTABILITY_LEVEL 1

#ifdef __cplusplus
}
#endif
#endif /* TI ARM version < 18.1.0.LTS */

/* include toolchain's header file */
#include <../include/errno.h>

#ifdef __cplusplus
extern "C" {
#endif
#if __TI_COMPILER_VERSION__ < 18001000

/*  When thread-safe errno is enabled (as above) the TI Compiler
 *  does not define any error codes. Define the error codes here.
 */
#if defined(__TMS470__) && defined(__TI_EABI_SUPPORT__) && \
    defined(_AEABI_PORTABILITY_LEVEL) && _AEABI_PORTABILITY_LEVEL != 0

#ifndef EACCES
#define EACCES          13
#endif

#ifndef EAGAIN
#define EAGAIN          11
#endif

#ifndef EBADF
#define EBADF           9
#endif

#ifndef EBUSY
#define EBUSY           16
#endif

#ifndef EDEADLK
#define EDEADLK         45
#endif

#ifndef EEXIST
#define EEXIST          17
#endif

#ifndef EFAULT
#define EFAULT          14
#endif

#ifndef EINVAL
#define EINVAL          22
#endif

#ifndef EMSGSIZE
#define EMSGSIZE        122
#endif

#ifndef ENFILE
#define ENFILE          23
#endif

#ifndef ENOMEM
#define ENOMEM          12
#endif

#ifndef ENOENT
#define ENOENT          2
#endif

#ifndef ENOSPC
#define ENOSPC          28
#endif

#ifndef ENOSYS
#define ENOSYS          89
#endif

#ifndef ENOTSUP
#define ENOTSUP         48
#endif

#ifndef EPERM
#define EPERM           1
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT       145
#endif

#endif
#endif /* TI ARM version < 18.1.0.LTS */

/* These are in errno.h but commented out. Values match GNU ARM compiler. */

#ifndef EADDRINUSE
#define EADDRINUSE              112
#endif

#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL           125
#endif

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT            106
#endif

#ifndef ECONNREFUSED
#define ECONNREFUSED            111
#endif

#ifndef EDESTADDRREQ
#define EDESTADDRREQ            121
#endif

#ifndef EISCONN
#define EISCONN                 127
#endif

#ifndef ENETUNREACH
#define ENETUNREACH             114
#endif

#ifndef ENOBUFS
#define ENOBUFS                 105
#endif

#ifndef ENOPROTOOPT
#define ENOPROTOOPT             109
#endif

#ifndef ENOTCONN
#define ENOTCONN                128
#endif

#ifndef EOPNOTSUPP
#define EOPNOTSUPP              95
#endif

#ifndef EOVERFLOW
#define EOVERFLOW               139
#endif

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT         123
#endif

#ifndef EPROTOTYPE
#define EPROTOTYPE              107
#endif

/* custom error codes */
#define EFREERTOS    2001       /* FreeRTOS function failure */

#ifdef __cplusplus
}
#endif


/*  TI toolchain check
 *
 *  TI DSP Compiler
 */
#elif defined(__TMS320C6X__)

/* include toolchain's header file */
#include <../include/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  Must not define __C6X_MIGRATION__ because this disables
 *  thread-safe errno support.
 */
#if defined(__C6X_MIGRATION__)
#error must not define __C6X_MIGRATION__
#endif

/* make sure __TI_EABI__ is defined to enable thread-safe errno support */
#if !defined(__TI_EABI__)
#error __TI_EABI__ must be defined
#endif

/* These are missing from errno.h Values match GNU ARM compiler. */

#ifndef EACCES
#define EACCES                  13
#endif

#ifndef EAGAIN
#define EAGAIN                  11
#endif

#ifndef EBADF
#define EBADF                   9
#endif

#ifndef EBUSY
#define EBUSY                   16
#endif

#ifndef EDEADLK
#define EDEADLK                 45
#endif

#ifndef EEXIST
#define EEXIST                  17
#endif

#ifndef EFAULT
#define EFAULT                  14
#endif

#ifndef EINVAL
#define EINVAL                  22
#endif

#ifndef EMSGSIZE
#define EMSGSIZE                122
#endif

#ifndef ENFILE
#define ENFILE                  23
#endif

#ifndef ENOMEM
#define ENOMEM                  12
#endif

#ifndef ENOENT
#define ENOENT                  2
#endif

#ifndef ENOSPC
#define ENOSPC                  28
#endif

#ifndef ENOSYS
#define ENOSYS                  89
#endif

#ifndef ENOTSUP
#define ENOTSUP                 48
#endif

#ifndef EPERM
#define EPERM                   1
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT               145
#endif

#ifndef EADDRINUSE
#define EADDRINUSE              112
#endif

#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL           125
#endif

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT            106
#endif

#ifndef ECONNREFUSED
#define ECONNREFUSED            111
#endif

#ifndef EDESTADDRREQ
#define EDESTADDRREQ            121
#endif

#ifndef EISCONN
#define EISCONN                 127
#endif

#ifndef ENETUNREACH
#define ENETUNREACH             114
#endif

#ifndef ENOBUFS
#define ENOBUFS                 105
#endif

#ifndef ENOPROTOOPT
#define ENOPROTOOPT             109
#endif

#ifndef ENOTCONN
#define ENOTCONN                128
#endif

#ifndef EOPNOTSUPP
#define EOPNOTSUPP              95
#endif

#ifndef EOVERFLOW
#define EOVERFLOW               139
#endif

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT         123
#endif

#ifndef EPROTOTYPE
#define EPROTOTYPE              107
#endif

#ifdef __cplusplus
}
#endif


/*  TI toolchain check
 *
 *  Error: Unsupported compiler
 */
#else

/*  #error Unsupported TI Compiler
 *
 *  Temporary: Define error codes and include compiler header file
 *  without warning. This will be fixed in a future release.
 */

/* include toolchain's header file */
#include <../include/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EACCES
#define EACCES                  13
#endif

#ifndef EAGAIN
#define EAGAIN                  11
#endif

#ifndef EBADF
#define EBADF                   9
#endif

#ifndef EBUSY
#define EBUSY                   16
#endif

#ifndef EDEADLK
#define EDEADLK                 45
#endif

#ifndef EEXIST
#define EEXIST                  17
#endif

#ifndef EFAULT
#define EFAULT                  14
#endif

#ifndef EINVAL
#define EINVAL                  22
#endif

#ifndef EMSGSIZE
#define EMSGSIZE                122
#endif

#ifndef ENFILE
#define ENFILE                  23
#endif

#ifndef ENOMEM
#define ENOMEM                  12
#endif

#ifndef ENOENT
#define ENOENT                  2
#endif

#ifndef ENOSPC
#define ENOSPC                  28
#endif

#ifndef ENOSYS
#define ENOSYS                  89
#endif

#ifndef ENOTSUP
#define ENOTSUP                 48
#endif

#ifndef EPERM
#define EPERM                   1
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT               145
#endif

#ifndef EADDRINUSE
#define EADDRINUSE              112
#endif

#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL           125
#endif

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT            106
#endif

#ifndef ECONNREFUSED
#define ECONNREFUSED            111
#endif

#ifndef EDESTADDRREQ
#define EDESTADDRREQ            121
#endif

#ifndef EISCONN
#define EISCONN                 127
#endif

#ifndef ENETUNREACH
#define ENETUNREACH             114
#endif

#ifndef ENOBUFS
#define ENOBUFS                 105
#endif

#ifndef ENOPROTOOPT
#define ENOPROTOOPT             109
#endif

#ifndef ENOTCONN
#define ENOTCONN                128
#endif

#ifndef EOPNOTSUPP
#define EOPNOTSUPP              95
#endif

#ifndef EOVERFLOW
#define EOVERFLOW               139
#endif

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT         123
#endif

#ifndef EPROTOTYPE
#define EPROTOTYPE              107
#endif

#ifdef __cplusplus
}
#endif

#endif /* TI toolchain check */

#endif /* ti_posix_ccs_errno__include */
