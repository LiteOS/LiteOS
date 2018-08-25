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

#ifndef ti_posix_iar_errno__include
#define ti_posix_iar_errno__include

/* compiler vendor check */
#ifndef __IAR_SYSTEMS_ICC__
#error Incompatible compiler: use this include path (.../ti/posix/iar) only with an IAR compiler. You appear to be using a different compiler.
#endif

/* include toolchain's header file */
#if defined(__430_CORE__) || defined(__430X_CORE__)
#include <../inc/dlib/c/errno.h>
#else
#include <../inc/c/errno.h>
#endif

#ifdef __cplusplus
extern "C" {
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
/* custom error codes */
#define EFREERTOS    2001       /* FreeRTOS function failure */

#ifdef __cplusplus
}
#endif

#endif /* ti_posix_iar_errno__include */
