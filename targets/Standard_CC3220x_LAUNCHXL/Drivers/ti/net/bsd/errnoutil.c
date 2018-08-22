/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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

#include <errno.h>

#include <ti/net/bsd/errnoutil.h>
#include <ti/net/slneterr.h>

/********************************************************************
  ErrnoUtil_Set
  Go over the BSD error codes, and sets errno using the internal
  errno management mechanism, or using an external errno infrastructure.
*********************************************************************/

int ErrnoUtil_set(int32_t Errno)
{
    if(Errno >= 0)
    {
        return Errno;
    }
    /*  This switch case block is necessary for translating the SlNetSock error codes
     *  to BSD ones. The #ifdef in each case are made in order to reduce code footprint:
     *  This cases are compiled if and only if there's a discrepancy between the BSD error
     *  number and the error code returned by the SlNetSock layer.
     */
    switch(Errno)
    {
#if EBADF != SLNETERR_BSD_EBADF
    case SLNETERR_BSD_EBADF:
        Errno = EBADF;
        break;
#endif
#if ENFILE != SLNETERR_BSD_ENSOCK
    case SLNETERR_BSD_ENSOCK:
        Errno = ENFILE;
        break;
#endif
#if EAGAIN != SLNETERR_BSD_EAGAIN
    case SLNETERR_BSD_EAGAIN:
        Errno = EAGAIN;
        break;
#endif
#if ENOMEM != SLNETERR_BSD_ENOMEM
    case SLNETERR_BSD_ENOMEM:
        Errno = ENOMEM;
        break;
#endif
#if EACCES != SLNETERR_BSD_EACCES
    case SLNETERR_BSD_EACCES:
        Errno = EACCES;
        break;
#endif
#if EFAULT != SLNETERR_BSD_EFAULT
    case SLNETERR_BSD_EFAULT:
        Errno = EFAULT;
        break;
#endif
#if EINVAL != SLNETERR_BSD_EINVAL
    case SLNETERR_BSD_EINVAL:
        Errno = EINVAL;
        break;
#endif
#if EDESTADDRREQ != SLNETERR_BSD_EDESTADDRREQ
    case SLNETERR_BSD_EDESTADDRREQ:
        Errno = EDESTADDRREQ;
        break;
#endif
#if EPROTOTYPE != SLNETERR_BSD_EPROTOTYPE
    case SLNETERR_BSD_EPROTOTYPE:
        Errno = EPROTOTYPE;
        break;
#endif
#if ENOPROTOOPT != SLNETERR_BSD_ENOPROTOOPT
    case SLNETERR_BSD_ENOPROTOOPT:
        Errno = ENOPROTOOPT;
        break;
#endif
#if EPROTONOSUPPORT != SLNETERR_BSD_EPROTONOSUPPORT
    case SLNETERR_BSD_EPROTONOSUPPORT:
        Errno = EPROTONOSUPPORT;
        break;
#endif
#if EOPNOTSUPP != SLNETERR_BSD_EOPNOTSUPP
    case SLNETERR_BSD_EOPNOTSUPP:
        Errno = EOPNOTSUPP;
        break;
#endif
#if EAFNOSUPPORT != SLNETERR_BSD_EAFNOSUPPORT
    case SLNETERR_BSD_EAFNOSUPPORT:
        Errno = EAFNOSUPPORT;
        break;
#endif
#if EADDRINUSE != SLNETERR_BSD_EADDRINUSE
    case SLNETERR_BSD_EADDRINUSE:
        Errno = EADDRINUSE;
        break;
#endif
#if EADDRNOTAVAIL != SLNETERR_BSD_EADDRNOTAVAIL
    case SLNETERR_BSD_EADDRNOTAVAIL:
        Errno = EADDRNOTAVAIL;
        break;
#endif
#if ENETUNREACH != SLNETERR_BSD_ENETUNREACH
    case SLNETERR_BSD_ENETUNREACH:
        Errno = ENETUNREACH;
        break;
#endif
#if ENOBUFS != SLNETERR_BSD_ENOBUFS
    case SLNETERR_BSD_ENOBUFS:
        Errno = ENOBUFS;
        break;
#endif
#if EISCONN != SLNETERR_BSD_EISCONN
    case SLNETERR_BSD_EISCONN:
        Errno = EISCONN;
        break;
#endif
#if ENOTCONN != SLNETERR_BSD_ENOTCONN
    case SLNETERR_BSD_ENOTCONN:
        Errno = ENOTCONN;
        break;
#endif
#if ETIMEDOUT != SLNETERR_BSD_ETIMEDOUT
    case SLNETERR_BSD_ETIMEDOUT:
        Errno = ETIMEDOUT;
        break;
#endif
#if ECONNREFUSED != SLNETERR_BSD_ECONNREFUSED
    case SLNETERR_BSD_ECONNREFUSED:
        Errno = ECONNREFUSED;
        break;
#endif
    /* The cases below are propriety driver errors, which can
     * be returned by the SimpleLink Driver, in various cases of failure.
     * Each is mapped to the corresponding BSD error.
     */
    case SLNETERR_POOL_IS_EMPTY:
    case SLNETERR_RET_CODE_NO_FREE_SPACE:
    case SLNETERR_RET_CODE_MUTEX_CREATION_FAILED:
    case SLNETERR_RET_CODE_MALLOC_ERROR:
        Errno = ENOMEM;
        break;
    case SLNETERR_RET_CODE_INVALID_INPUT:
    case SLNETERR_EZEROLEN:
    case SLNETERR_ESMALLBUF:
    case SLNETERR_INVALPARAM:
        Errno = EINVAL;
        break;
    case SLNETERR_RET_CODE_DOESNT_SUPPORT_NON_MANDATORY_FXN:
        Errno = EOPNOTSUPP;
        break;
    case SLNETERR_RET_CODE_COULDNT_FIND_RESOURCE:
        Errno = EBADF;
        break;
    default:
     /* Do nothing ..
      * If no case is true, that means that the BSD error
      * code and the code returned by the NWP are either identical,
      * or no propriety error has occurred.
      */
        break;
    }
        errno = Errno;

    return SLNETERR_BSD_SOC_ERROR;
}
