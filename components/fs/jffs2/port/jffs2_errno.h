/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#ifndef __JFFS2_ERRNO_H__
#define __JFFS2_ERRNO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* CONSTANT DEFINITIONS */

/* If adding to this list, you must also update strerror() with its text
 * If there is a common error of the same purpose on Unix, try and use its
 * name and number. If not, use one above 200 to prevent future conflicts
 *
 * Do not use negative numbers, so that functions can return positive on
 * success and -ESOMETHING on error, and it all works consistently.
*/

#define ENOERR           0     /* No error */
#define EPERM            1     /* Not permitted */
#define ENOENT           2     /* No such entity */
#define ESRCH            3     /* No such process */
#define EINTR            4     /* Operation interrupted */
#define EIO              5     /* I/O error */
#define EBADF            9     /* Bad file handle */
#define EAGAIN           11    /* Try again later */
#define EWOULDBLOCK      EAGAIN
#define ENOMEM           12    /* Out of memory */
#define EBUSY            16    /* Resource busy */
#define EXDEV            18    /* Cross-device link */
#define ENODEV           19    /* No such device */
#define ENOTDIR          20    /* Not a directory */
#define EISDIR           21    /* Is a directory */
#define EINVAL           22    /* Invalid argument */
#define ENFILE           23    /* Too many open files in system */
#define EMFILE           24    /* Too many open files */
#define EFBIG            27    /* File too large */
#define ENOSPC           28    /* No space left on device */
#define ESPIPE           29    /* Illegal seek */
#define EROFS            30    /* Read-only file system */
#define EDOM             33    /* Argument to math function outside valid */
                               /* domain */
#define ERANGE           34    /* Math result cannot be represented */
#define EDEADLK          35    /* Resource deadlock would occur */
#define EDEADLOCK        EDEADLK
#define ENOSYS           38    /* Function not implemented */
#define ENAMETOOLONG     60    /* File name too long */
#define ENOTEMPTY        66    /* Directory not empty */
#define ENOTSUP          95    /* Not supported error */
#define EEOF             200   /* End of file reached */
#define ENOSUPP          201   /* Operation not supported */
#define EDEVNOSUPP       202   /* Device does not support this operation */

/* Additional errors used by networking */
#define ENXIO            300   /* Device not configured */
#define EACCES           301   /* Permission denied */
#define EEXIST           302   /* File exists */
#define ENOTTY           303   /* Inappropriate ioctl for device */
#define EPIPE            304   /* Broken pipe */

/* non-blocking and interrupt i/o */
#define EINPROGRESS      310   /* Operation now in progress */
#define EALREADY         311   /* Operation already in progress */

/* ipc/network software -- argument errors */
#define ENOTSOCK         320   /* Socket operation on non-socket */
#define EDESTADDRREQ     321   /* Destination address required */
#define EMSGSIZE         322   /* Message too long */
#define EPROTOTYPE       323   /* Protocol wrong type for socket */
#define ENOPROTOOPT      324   /* Protocol not available */
#define EPROTONOSUPPORT  325   /* Protocol not supported */
#define ESOCKTNOSUPPORT  326   /* Socket type not supported */
#define EOPNOTSUPP       327   /* Operation not supported */
#define EPFNOSUPPORT     328   /* Protocol family not supported */
#define EAFNOSUPPORT     329   /* Address family not supported by */
                               /* protocol family */
#define EADDRINUSE       330   /* Address already in use */
#define EADDRNOTAVAIL    331   /* Can't assign requested address */

/* ipc/network software -- operational errors */
#define ENETDOWN         350   /* Network is down */
#define ENETUNREACH      351   /* Network is unreachable */
#define ENETRESET        352   /* Network dropped connection on reset */
#define ECONNABORTED     353   /* Software caused connection abort */
#define ECONNRESET       354   /* Connection reset by peer */
#define ENOBUFS          355   /* No buffer space available */
#define EISCONN          356   /* Socket is already connected */
#define ENOTCONN         357   /* Socket is not connected */
#define ESHUTDOWN        358   /* Can't send after socket shutdown */
#define ETOOMANYREFS     359   /* Too many references: can't splice */
#define ETIMEDOUT        360   /* Operation timed out */
#define ECONNREFUSED     361   /* Connection refused */

#define EHOSTDOWN        364   /* Host is down */
#define EHOSTUNREACH     365   /* No route to host */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __JFFS2_ERRNO_H__ */

