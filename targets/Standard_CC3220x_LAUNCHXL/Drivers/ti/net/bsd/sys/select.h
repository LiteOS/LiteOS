/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
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

#ifndef ti_net_bsd_sys_select__include
#define ti_net_bsd_sys_select__include

/* include compiler sys/select.h (added in TI ARM 18.1.0.LTS) */
#if defined(__TMS470__) && (__TI_COMPILER_VERSION__ >= 18001000)
#define _SELECT_DECLARED
#include <../include/sys/select.h>
#endif

#include <ti/net/bsd/errnoutil.h>
#include <ti/net/slnetsock.h>

#ifdef    __cplusplus
extern "C" {
#endif

/* structs */
#define timeval                             SlNetSock_Timeval_t
#undef fd_set
#define fd_set                              SlNetSock_SdSet_t

/* FD_ functions */
#undef  FD_SETSIZE
#define FD_SETSIZE                          SLNETSOCK_MAX_CONCURRENT_SOCKETS
#undef FD_SET
#define FD_SET                              SlNetSock_sdsSet
#undef FD_CLR
#define FD_CLR                              SlNetSock_sdsClr
#undef FD_ISSET
#define FD_ISSET                            SlNetSock_sdsIsSet
#undef FD_ZERO
#define FD_ZERO                             SlNetSock_sdsClrAll

/* select */
static inline int select(int nfds, fd_set *readsds, fd_set *writesds, fd_set *exceptsds, struct timeval *timeout)
{
    int RetVal = (int)SlNetSock_select(nfds, readsds, writesds, exceptsds, timeout);
    return ErrnoUtil_set(RetVal);
}


#ifdef  __cplusplus
}
#endif

#endif /* ti_net_bsd_sys_select__include */
