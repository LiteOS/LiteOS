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

#ifndef ti_net_bsd_sys_socket__include
#define ti_net_bsd_sys_socket__include

#include <stddef.h>
#include <sys/types.h>

#include <ti/net/bsd/errnoutil.h>
#include <ti/net/slnetsock.h>

#ifdef    __cplusplus
extern "C" {
#endif

/* structs */
#define socklen_t                           SlNetSocklen_t
#define sockaddr                            SlNetSock_Addr_t

/* must match type of _SlNetSock_Addr_t's sa_family field */
typedef uint16_t sa_family_t;

#define sockaddr_storage SlNetSock_SockAddrStorage_t

/* socket opts */
#define SOCK_STREAM                         SLNETSOCK_SOCK_STREAM
#define SOCK_DGRAM                          SLNETSOCK_SOCK_DGRAM
#define SOCK_RAW                            SLNETSOCK_SOCK_RAW
#define AF_UNSPEC                           SLNETSOCK_AF_UNSPEC
#define AF_INET                             SLNETSOCK_AF_INET
#define AF_INET6                            SLNETSOCK_AF_INET6
#define AF_RF                               SLNETSOCK_AF_RF
#define AF_PACKET                           SLNETSOCK_AF_PACKET
#define PF_UNSPEC                           SLNETSOCK_PF_UNSPEC
#define PF_INET                             SLNETSOCK_PF_INET
#define PF_INET6                            SLNETSOCK_PF_INET6
#define SOL_SOCKET                          SLNETSOCK_LVL_SOCKET
#define SO_KEEPALIVE                        SLNETSOCK_OPSOCK_KEEPALIVE
#define SO_KEEPALIVETIME                    SLNETSOCK_OPSOCK_KEEPALIVE_TIME
#define SO_LINGER                           SLNETSOCK_OPSOCK_LINGER
#define SO_RX_NO_IP_BOUNDARY                SLNETSOCK_OPSOCK_NON_IP_BOUNDARY
#define SO_RCVTIMEO                         SLNETSOCK_OPSOCK_RCV_TIMEO
#define SO_RCVBUF                           SLNETSOCK_OPSOCK_RCV_BUF
#define SO_NONBLOCKING                      SLNETSOCK_OPSOCK_NON_BLOCKING
#define MSG_OOB                             SLNETSOCK_MSG_OOB
#define MSG_PEEK                            SLNETSOCK_MSG_PEEK
#define MSG_WAITALL                         SLNETSOCK_MSG_WAITALL
#define MSG_DONTWAIT                        SLNETSOCK_MSG_DONTWAIT
#define MSG_DONTROUTE                       SLNETSOCK_MSG_DONTROUTE
#define MSG_NOSIGNAL                        SLNETSOCK_MSG_NOSIGNAL
#define SO_BROADCAST                        (200)
#define SO_REUSEADDR                        (201)
#define SO_SNDBUF                           (202)
#define TCP_NODELAY                         (203)


/* socket shutdown opts */
#define SHUT_RD                             SLNETSOCK_SHUT_RD
#define SHUT_WR                             SLNETSOCK_SHUT_WR
#define SHUT_RDWR                           SLNETSOCK_SHUT_RDWR

/* BSD/POSIX socket functions.  Consult POSIX documentation
 * for more details.
 */
int socket(int Domain, int Type, int Protocol);
int shutdown(int fd, int how);
int accept(int fd, struct sockaddr *addr, socklen_t *addrlen);
int bind(int fd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int fd, int backlog);
int connect(int fd, const struct sockaddr *addr, socklen_t addrlen);
int getpeername(int fd, struct sockaddr *addr, socklen_t *addrlen);
int getsockname(int fd, struct sockaddr *addr, socklen_t *addrlen);
int setsockopt(int fd, int level, int optname, const void *optval,
        socklen_t optlen);
int getsockopt(int fd, int level, int optname, void *optval,
        socklen_t *optlen);
ssize_t recv(int fd, void *pBuf, size_t Len, int flags);
ssize_t recvfrom(int fd, void *buf, size_t Len, int flags,
        struct sockaddr *from, socklen_t *fromlen);
ssize_t send(int fd, const void *pBuf, size_t Len, int flags);
ssize_t sendto(int fd, const void *pBuf, size_t Len, int flags,
        const struct sockaddr *to, socklen_t tolen);

/* close is a static inline to avoid conflicting with the "C" file close */
static inline int close(int fd)
{
    int RetVal = (int16_t)SlNetSock_close((int16_t)fd);
    return ErrnoUtil_set(RetVal);
}

/*!
    \brief errno - returns error code for last failure of BSD API calling.

     if SL_INC_INTERNAL_ERRNO is enabled, when an error occurs the BSD API returns -1, in order to detect the specific error value,
     user should invoke this function.

    \return  Return number of last error

    \sa      bind  connect  recvfrom  recv  accept  sendto  send
    \warning
    \par        Example

    - Querying errno:
    \code
        Status = recvfrom(SockID, Buf, 1472, 0, (struct sockaddr *)&Addr, (socklen_t*)&AddrSize);
        while(Status < 0)
        {
            if(errno == EAGAIN)
            {
               Status = recvfrom(SockID, Buf, 1472, 0, (struct sockaddr *)&Addr, (socklen_t*)&AddrSize);
            }
            else if(errno)
            {
                printf("A socket error occurred..");
                return (close(SockID));
            }
        }
    \endcode
*/
#ifdef SL_INC_INTERNAL_ERRNO
#ifdef errno
#undef errno
#endif
#define errno       *(__errno())
#endif

#ifdef  __cplusplus
}
#endif
#endif /* ti_net_bsd_sys_socket__include */
