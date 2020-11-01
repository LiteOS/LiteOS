/**
 * @defgroup socket Socket
 * @ingroup libc
 */

#ifndef	_SYS_SOCKET_H
#define	_SYS_SOCKET_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_socklen_t
#define __NEED_sa_family_t
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_uid_t
#define __NEED_pid_t
#define __NEED_gid_t
#define __NEED_struct_iovec

#include <bits/alltypes.h>

#include <bits/socket.h>

struct msghdr {
	void *msg_name;
	socklen_t msg_namelen;
	struct iovec *msg_iov;
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __BIG_ENDIAN
	int __pad1;
#endif
	int msg_iovlen;
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __LITTLE_ENDIAN
	int __pad1;
#endif
	void *msg_control;
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __BIG_ENDIAN
	int __pad2;
#endif
	socklen_t msg_controllen;
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __LITTLE_ENDIAN
	int __pad2;
#endif
	int msg_flags;
};

struct cmsghdr {
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __BIG_ENDIAN
	int __pad1;
#endif
	socklen_t cmsg_len;
#if __LONG_MAX > 0x7fffffff && __BYTE_ORDER == __LITTLE_ENDIAN
	int __pad1;
#endif
	int cmsg_level;
	int cmsg_type;
};

#ifdef _GNU_SOURCE
struct ucred {
	pid_t pid;
	uid_t uid;
	gid_t gid;
};

struct mmsghdr {
	struct msghdr msg_hdr;
	unsigned int  msg_len;
};

struct timespec;

int sendmmsg (int, struct mmsghdr *, unsigned int, unsigned int);
int recvmmsg (int, struct mmsghdr *, unsigned int, unsigned int, struct timespec *);
#endif

struct linger {
	int l_onoff;
	int l_linger;
};

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2

#ifndef SOCK_STREAM
#define SOCK_STREAM    1
#define SOCK_DGRAM     2
#endif

#define SOCK_RAW       3
#define SOCK_RDM       4
#define SOCK_SEQPACKET 5
#define SOCK_DCCP      6
#define SOCK_PACKET    10

#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC   02000000
#define SOCK_NONBLOCK  04000
#endif

#define PF_UNSPEC       0
#define PF_LOCAL        1
#define PF_UNIX         PF_LOCAL
#define PF_FILE         PF_LOCAL
#define PF_INET         2
#define PF_AX25         3
#define PF_IPX          4
#define PF_APPLETALK    5
#define PF_NETROM       6
#define PF_BRIDGE       7
#define PF_ATMPVC       8
#define PF_X25          9
#define PF_INET6        10
#define PF_ROSE         11
#define PF_DECnet       12
#define PF_NETBEUI      13
#define PF_SECURITY     14
#define PF_KEY          15
#define PF_NETLINK      16
#define PF_ROUTE        PF_NETLINK
#define PF_PACKET       17
#define PF_ASH          18
#define PF_ECONET       19
#define PF_ATMSVC       20
#define PF_RDS          21
#define PF_SNA          22
#define PF_IRDA         23
#define PF_PPPOX        24
#define PF_WANPIPE      25
#define PF_LLC          26
#define PF_IB           27
#define PF_MPLS         28
#define PF_CAN          29
#define PF_TIPC         30
#define PF_BLUETOOTH    31
#define PF_IUCV         32
#define PF_RXRPC        33
#define PF_ISDN         34
#define PF_PHONET       35
#define PF_IEEE802154   36
#define PF_CAIF         37
#define PF_ALG          38
#define PF_NFC          39
#define PF_VSOCK        40
#define PF_KCM          41
#define PF_QIPCRTR      42
#define PF_SMC          43
#define PF_XDP          44
#define PF_MAX          45

#define AF_UNSPEC       PF_UNSPEC
#define AF_LOCAL        PF_LOCAL
#define AF_UNIX         AF_LOCAL
#define AF_FILE         AF_LOCAL
#define AF_INET         PF_INET
#define AF_AX25         PF_AX25
#define AF_IPX          PF_IPX
#define AF_APPLETALK    PF_APPLETALK
#define AF_NETROM       PF_NETROM
#define AF_BRIDGE       PF_BRIDGE
#define AF_ATMPVC       PF_ATMPVC
#define AF_X25          PF_X25
#define AF_INET6        PF_INET6
#define AF_ROSE         PF_ROSE
#define AF_DECnet       PF_DECnet
#define AF_NETBEUI      PF_NETBEUI
#define AF_SECURITY     PF_SECURITY
#define AF_KEY          PF_KEY
#define AF_NETLINK      PF_NETLINK
#define AF_ROUTE        PF_ROUTE
#define AF_PACKET       PF_PACKET
#define AF_ASH          PF_ASH
#define AF_ECONET       PF_ECONET
#define AF_ATMSVC       PF_ATMSVC
#define AF_RDS          PF_RDS
#define AF_SNA          PF_SNA
#define AF_IRDA         PF_IRDA
#define AF_PPPOX        PF_PPPOX
#define AF_WANPIPE      PF_WANPIPE
#define AF_LLC          PF_LLC
#define AF_IB           PF_IB
#define AF_MPLS         PF_MPLS
#define AF_CAN          PF_CAN
#define AF_TIPC         PF_TIPC
#define AF_BLUETOOTH    PF_BLUETOOTH
#define AF_IUCV         PF_IUCV
#define AF_RXRPC        PF_RXRPC
#define AF_ISDN         PF_ISDN
#define AF_PHONET       PF_PHONET
#define AF_IEEE802154   PF_IEEE802154
#define AF_CAIF         PF_CAIF
#define AF_ALG          PF_ALG
#define AF_NFC          PF_NFC
#define AF_VSOCK        PF_VSOCK
#define AF_KCM          PF_KCM
#define AF_QIPCRTR      PF_QIPCRTR
#define AF_SMC          PF_SMC
#define AF_XDP          PF_XDP
#define AF_MAX          PF_MAX

#ifndef SO_DEBUG
#define SO_DEBUG        1
#define SO_REUSEADDR    2
#define SO_TYPE         3
#define SO_ERROR        4
#define SO_DONTROUTE    5
#define SO_BROADCAST    6
#define SO_SNDBUF       7
#define SO_RCVBUF       8
#define SO_KEEPALIVE    9
#define SO_OOBINLINE    10
#define SO_NO_CHECK     11
#define SO_PRIORITY     12
#define SO_LINGER       13
#define SO_BSDCOMPAT    14
#define SO_REUSEPORT    15
#define SO_PASSCRED     16
#define SO_PEERCRED     17
#define SO_RCVLOWAT     18
#define SO_SNDLOWAT     19
#define SO_ACCEPTCONN   30
#define SO_PEERSEC      31
#define SO_SNDBUFFORCE  32
#define SO_RCVBUFFORCE  33
#define SO_PROTOCOL     38
#define SO_DOMAIN       39
#endif

#ifndef SO_RCVTIMEO
#if __LONG_MAX == 0x7fffffff
#define SO_RCVTIMEO     66
#define SO_SNDTIMEO     67
#else
#define SO_RCVTIMEO     20
#define SO_SNDTIMEO     21
#endif
#endif

#ifndef SO_TIMESTAMP
#if __LONG_MAX == 0x7fffffff
#define SO_TIMESTAMP    63
#define SO_TIMESTAMPNS  64
#define SO_TIMESTAMPING 65
#else
#define SO_TIMESTAMP    29
#define SO_TIMESTAMPNS  35
#define SO_TIMESTAMPING 37
#endif
#endif

#define SO_SECURITY_AUTHENTICATION              22
#define SO_SECURITY_ENCRYPTION_TRANSPORT        23
#define SO_SECURITY_ENCRYPTION_NETWORK          24

#define SO_BINDTODEVICE 25

#define SO_ATTACH_FILTER        26
#define SO_DETACH_FILTER        27
#define SO_GET_FILTER           SO_ATTACH_FILTER

#define SO_PEERNAME             28
#define SCM_TIMESTAMP           SO_TIMESTAMP
#define SO_PASSSEC              34
#define SCM_TIMESTAMPNS         SO_TIMESTAMPNS
#define SO_MARK                 36
#define SCM_TIMESTAMPING        SO_TIMESTAMPING
#define SO_RXQ_OVFL             40
#define SO_WIFI_STATUS          41
#define SCM_WIFI_STATUS         SO_WIFI_STATUS
#define SO_PEEK_OFF             42
#define SO_NOFCS                43
#define SO_LOCK_FILTER          44
#define SO_SELECT_ERR_QUEUE     45
#define SO_BUSY_POLL            46
#define SO_MAX_PACING_RATE      47
#define SO_BPF_EXTENSIONS       48
#define SO_INCOMING_CPU         49
#define SO_ATTACH_BPF           50
#define SO_DETACH_BPF           SO_DETACH_FILTER
#define SO_ATTACH_REUSEPORT_CBPF 51
#define SO_ATTACH_REUSEPORT_EBPF 52
#define SO_CNX_ADVICE           53
#define SCM_TIMESTAMPING_OPT_STATS 54
#define SO_MEMINFO              55
#define SO_INCOMING_NAPI_ID     56
#define SO_COOKIE               57
#define SCM_TIMESTAMPING_PKTINFO 58
#define SO_PEERGROUPS           59
#define SO_ZEROCOPY             60
#define SO_TXTIME               61
#define SCM_TXTIME              SO_TXTIME
#define SO_BINDTOIFINDEX        62
#define SO_DETACH_REUSEPORT_BPF 68

#ifndef SOL_SOCKET
#define SOL_SOCKET      1
#endif

#define SOL_IP          0
#define SOL_IPV6        41
#define SOL_ICMPV6      58

#define SOL_RAW         255
#define SOL_DECNET      261
#define SOL_X25         262
#define SOL_PACKET      263
#define SOL_ATM         264
#define SOL_AAL         265
#define SOL_IRDA        266
#define SOL_NETBEUI     267
#define SOL_LLC         268
#define SOL_DCCP        269
#define SOL_NETLINK     270
#define SOL_TIPC        271
#define SOL_RXRPC       272
#define SOL_PPPOL2TP    273
#define SOL_BLUETOOTH   274
#define SOL_PNPIPE      275
#define SOL_RDS         276
#define SOL_IUCV        277
#define SOL_CAIF        278
#define SOL_ALG         279
#define SOL_NFC         280
#define SOL_KCM         281
#define SOL_TLS         282
#define SOL_XDP         283

#define SOMAXCONN       128

#define MSG_OOB       0x0001
#define MSG_PEEK      0x0002
#define MSG_DONTROUTE 0x0004
#define MSG_CTRUNC    0x0008
#define MSG_PROXY     0x0010
#define MSG_TRUNC     0x0020
#define MSG_DONTWAIT  0x0040
#define MSG_EOR       0x0080
#define MSG_WAITALL   0x0100
#define MSG_FIN       0x0200
#define MSG_SYN       0x0400
#define MSG_CONFIRM   0x0800
#define MSG_RST       0x1000
#define MSG_ERRQUEUE  0x2000
#define MSG_NOSIGNAL  0x4000
#define MSG_MORE      0x8000
#define MSG_WAITFORONE 0x10000
#define MSG_BATCH     0x40000
#define MSG_ZEROCOPY  0x4000000
#define MSG_FASTOPEN  0x20000000
#define MSG_CMSG_CLOEXEC 0x40000000

#define __CMSG_LEN(cmsg) (((cmsg)->cmsg_len + sizeof(long) - 1) & ~(long)(sizeof(long) - 1))
#define __CMSG_NEXT(cmsg) ((unsigned char *)(cmsg) + __CMSG_LEN(cmsg))
#define __MHDR_END(mhdr) ((unsigned char *)(mhdr)->msg_control + (mhdr)->msg_controllen)

#define CMSG_DATA(cmsg) ((unsigned char *) (((struct cmsghdr *)(cmsg)) + 1))
#define CMSG_NXTHDR(mhdr, cmsg) ((cmsg)->cmsg_len < sizeof (struct cmsghdr) || \
	__CMSG_LEN(cmsg) + sizeof(struct cmsghdr) >= __MHDR_END(mhdr) - (unsigned char *)(cmsg) \
	? 0 : (struct cmsghdr *)__CMSG_NEXT(cmsg))
#define CMSG_FIRSTHDR(mhdr) ((size_t) (mhdr)->msg_controllen >= sizeof (struct cmsghdr) ? (struct cmsghdr *) (mhdr)->msg_control : (struct cmsghdr *) 0)

#define CMSG_ALIGN(len) (((len) + sizeof (size_t) - 1) & (size_t) ~(sizeof (size_t) - 1))
#define CMSG_SPACE(len) (CMSG_ALIGN (len) + CMSG_ALIGN (sizeof (struct cmsghdr)))
#define CMSG_LEN(len)   (CMSG_ALIGN (sizeof (struct cmsghdr)) + (len))

#define SCM_RIGHTS      0x01
#define SCM_CREDENTIALS 0x02

struct sockaddr {
	sa_family_t sa_family;
	char sa_data[14];
};

struct sockaddr_storage {
	sa_family_t ss_family;
	char __ss_padding[128-sizeof(long)-sizeof(sa_family_t)];
	unsigned long __ss_align;
};

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API is used to create an endpoint for communication and returns a file descriptor.
 *
 * @param    domain    [IN]  Specifies a protocol family.
 * @param    type      [IN]  Specifies the socket type.  [SOCK_RAW|SOCK_DGRAM|SOCK_STREAM]
 * @param    protocol  [IN]  Specifies the protocol to be used with the socket.
 *
 * @attention
 * <ul>
 * <li>Only domain AF_INET and AF_PACKET are supported. For AF_INET, type SOCK_RAW|SOCK_DGRAM|SOCK_STREAM is supported.
 * For AF_PACKET, only type SOCK_RAW is supported.</li>
 * <li>The type argument can only be used to specify the socket type, no flags is supported, such as SOCK_NONBLOCK and SOCK_CLOEXEC.</li>
 * </ul>
 *
 * @retval    #int    On success, socket() shall return a non-negative integer, the socket file descriptor. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EAFNOSUPPORT</b>: The implementation does not support the specified address family.</li>
 * <li><b>EINVAL</b>: Invalid type or invalid flags in type.</li>
 * <li><b>ENFILE</b>: The maximum number of sockets in the system are already open.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>EPROTONOSUPPORT</b>: The specified protocol is not supported within this domain & type.</li>
 * <li><b>ESOCKTNOSUPPORT</b>: The socket type is not supported within this domain.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
int socket (int, int, int);
int socketpair (int, int, int, int [2]);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API is used to shut down socket send and receive operations.
 *
 * @param  s    [IN]  Specifies a file descriptor referring to the socket.
 * @param  how  [IN]  Specifies the type of shutdown. The values are as follows: \n SHUT_RD Disables further receive operations. SHUT_WR: Disables further send operations. SHUT_RDWR: Disables further send and receive operations.
 *
 * @attention
 * <ul>
 * <li>Only "SHUT_RDWR" is supported for the "how" parameter in this API. lwIP does not support closing one end of the full-duplex connection.</li>
 * <li>This API does not support the PF_PACKET option.</li>
 * <li>The listen socket does not support half shutdown.</li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EINVAL</b>: The how argument is invalid.</li>
 * <li><b>ENOTCONN</b>: lwIP does not support half close, ENOTCONN is returned.</li>
 * <li><b>EINPROGRESS</b>: If WRITE/CONNECTING/CLOSE is in progress, lwip may fail.</li>
 * <li><b>EINVAL</b>: If socket is not connected, lwIP shall fail with EINVAL.</li>
 * <li><b>ENOMEM</b>: There was insufficient memory available to complete the operation.</li>
 * <li><b>EOPNOTSUPP</b>: Socket is not TCP, lwIP shall fail.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
int shutdown (int, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API assigns the address specified by name to the socket referred to
 * by the file descriptor 's'. namelen specifies the size, in bytes, of the address
 * structure pointed to by name.
 *
 * @param  s        [IN]  Specifies the file descriptor of the socket to be bound.
 * @param  name     [IN]  Points to a sockaddr structure containing the address to be bound to the socket. The length and format of the address depend on the address family of the socket.
 * @param  namelen  [IN]  Specifies the length of the sockaddr structure pointed to by the address argument.
 *
 * @attention
 * <ul>
 * <li>Bind must not be called multiple times.</li>
 * </ul>
 *
 * @retval     #0    On success.
 * @retval     #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EADDRINUSE</b>: The specified address is already in use.</li>
 * <li><b>EADDRINUSE</b>: For AF_INET socket, the port number was specified as zero in the socket address structure, but, upon attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.</li>
 * <li><b>EADDRNOTAVAIL</b>: The specified address is not available from the local machine.</li>
 * <li><b>EAFNOSUPPORT</b>: The specified address is not a valid address for the address family of the specified socket.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EINVAL</b>: The socket is already bound to an address, and the protocol does not support binding to a new address.</li>
 * <li><b>EINVAL</b>: The namelen argument is not a valid length for the address family.</li>
 * <li><b>EINVAL</b>: For AF_PACKET socket, the ifindex is out of system netif index range.</li>
 * <li><b>EISCONN</b>: The specified socket is already connected.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available to complete the call.</li>
 * <li><b>ENODEV</b>: For AF_PACKET socket, the netif can not be determined by the ifindex.</li>
 * <li><b>ENETDOWN</b>: For AF_PACKET socket, the netif determined by the ifindex was down.</li>
 * <li><b>EOPNOTSUPP</b>: The socket type of the specified socket does not support binding to an address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  connect | listen | socket
 *
 * @since Huawei LiteOS V100R001C00
 */
int bind (int, const struct sockaddr *, socklen_t);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API connects the socket referred to by the file descriptor 's' to the address specified by name.
 *
 * @param  s          [IN]  Specifies a socket file descriptor.
 * @param  name       [IN]  Specifies a pointer to the sockaddr structure which identifies the connection.
 * @param  namelen    [IN]  Specifies size of name.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EACCES</b>: The user tried to connect to a broadcast address without having the socket broadcast flag enabled.</li>
 * <li><b>EADDRNOTAVAIL</b>: For AF_IENT socket, the socket had not previously been bound to an address and, upon attempting to bind it to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.</li>
 * <li><b>EAFNOSUPPORT</b>: The specified address is not a valid address for the address family of the specified socket.</li>
 * <li><b>EALREADY</b>: The socket is nonblocking and a previous connection attempt has not yet been completed.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNREFUSED</b>: The target address was not listening for connections or refused the connection request.</li>
 * <li><b>ECONNRESET</b>: Remote host reset the connection request.</li>
 * <li><b>EINPROGRESS</b>: O_NONBLOCK is set for the file descriptor for the socket and the connection cannot be immediately established; the connection shall be established asynchronously.</li>
 * <li><b>EINVAL</b>: The address_len argument is not a valid length for the address family.</li>
 * <li><b>EISCONN</b>: The specified socket is connection-mode and is already connected.</li>
 * <li><b>ENETUNREACH</b>: No route to the network is present.</li>
 * <li><b>ENOBUFS</b>: No buffer space is available.</li>
 * <li><b>EOPNOTSUPP</b>: The referenced socket is not of domain AF_INET.</li>
 * <li><b>EOPNOTSUPP</b>: The socket is listening and cannot be connected.</li>
 * <li><b>ETIMEDOUT</b>: The attempt to connect timed out before a connection was made.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  bind | listen | socket
 *
 * @since Huawei LiteOS V100R001C00
 */
int connect (int, const struct sockaddr *, socklen_t);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API marks the socket referred to by 's' as a passive socket, that is, as a socket that will be used
 * to accept incoming connection requests using accept().
 *
 * @param  s          [IN]  Specifies a file descriptor that refers to a socket of type SOCK_STREAM.
 * @param  backlog    [IN]  Defines the maximum length to which the queue of pending connections for 's' may grow. If a connection request arrives when the queue is full, the client may receive an error with
 *      an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.
 *
 * @attention
 * <ul>
 * <li>Max value of backlog is 0xFF, and min value of backlog is 0.</li>
 * <li>If backlog value is 0 or less than zero, backlog value of 1 is used.</li>
 * <li>This API does not support the PF_PACKET socket.</li>
 * <li>lwIP do not support auto binding in listen operation, so bind must be called before listen.</li>
 * <li>Listen can only be called once, and the backlog value can not be changed by later calling.</li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EDESTADDRREQ</b>: The socket is not bound to a local address, and the protocol does not support listening on an unbound socket.</li>
 * <li><b>EINVAL</b>: The socket is in state which is not acceptable for listen.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources are available in the system to complete the call.</li>
 * <li><b>EOPNOTSUPP</b>: The socket protocol does not support listen().</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  bind | connect | socket
 *
 * @since Huawei LiteOS V100R001C00
 */
int listen (int, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API extracts the first connection request on the queue of pending connections
 * for the listening socket 's', creates a new connected socket, and returns a new
 * file descriptor  referring to that  socket. The newly created socket is not in the
 * the listening state. The original socket 's' is unaffected by this call.
 *
 * @param  s          [IN]     Specifies a socket that was created with socket(), has been bound to an address with bind(), and has issued a successful call to listen().
 * @param  addr       [OUT]    Either a null pointer, or a pointer to a sockaddr structure where the address of the connecting socket shall be returned.
 * @param  addrlen    [IN/OUT] Either a null pointer, if address is a null pointer, or a pointer to a socklen_t object which on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.
 *
 * @attention
 * <ul>
 * <li>This API does not support the PF_PACKET option.</li>
 * </ul>
 *
 * @retval    #int     On success, accept() shall return the non-negative file descriptor of the accepted socket. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EWOULDBLOCK</b>: O_NONBLOCK is set for the socket file descriptor and no connections are present to be accepted.</li>
 * <li><b>EWOULDBLOCK</b>: O_NONBLOCK is not set but SO_RCVTIMEO is set, and no connections are present within acception timeout.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNABORTED</b>: A connection has been aborted.</li>
 * <li><b>EINVAL</b>: The socket is not accepting connections.</li>
 * <li><b>ENFILE</b>: The maximum number of sockets in the system are already open.</li>
 * <li><b>ENOBUFS</b>: No buffer space is available.</li>
 * <li><b>ENOMEM</b>: There was insufficient memory available to complete the operation.</li>
 * <li><b>EOPNOTSUPP</b>: The referenced socket is not of type SOCK_STREAM.</li>
 * <li><b>EIO</b>: Internal errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  bind | connect | listen | socket
 *
 * @since Huawei LiteOS V100R001C00
 */
int accept (int, struct sockaddr *__restrict, socklen_t *__restrict);
int accept4(int, struct sockaddr *__restrict, socklen_t *__restrict, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API returns the current address to which the socket 's'  is bound, in the buffer pointed to by name.
 * The namelen argument should be initialized to indicate the amount of space(in bytes) pointed to by
 * name.The returned address is truncated if the buffer provided is too small; in this case, namelen will
 * return a value greater than was supplied to the call.
 *
 * @param  s       [IN]      Specifies the file descriptor referring to connected socket.
 * @param  name    [OUT]     Pointer to sockaddr structure of local peer.
 * @param  namelen [IN/OUT]  Size of name structure.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EOPNOTSUPP</b>: The operation is not supported for this socket's protocol.For PF_RAW sockets, this error is returned.</li>
 * <li><b>EINVAL</b>: The Name or Namelen are NULL, then EINVAL is returned.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  getpeername
 *
 * @since Huawei LiteOS V100R001C00
 */
int getsockname (int, struct sockaddr *__restrict, socklen_t *__restrict);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API returns the address of the peer connected to the socket 's', in the buffer pointed to by name.
 * The namelen argument should be initialized  to  indicate the amount of space pointed to by name.
 * On return it contains the actual size of the name returned (in bytes). The name is truncated if the
 * buffer provided is too small.
 *
 * @param  s        [IN]      Specifies the file descriptor referring to the connected socket.
 * @param  name     [OUT]     Pointer to sockaddr structure of remote peer
 * @param  namelen  [IN/OUT]  Size of name structure.
 *
 * @attention
 * <ul>
 * <li>This API does not support the PF_PACKET option.</li>
 * </ul>
 *
 * @retval     #0    On success.
 * @retval     #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ENOTCONN</b>: The socket is not connected or otherwise has not had the peer pre-specified.</li>
 * <li><b>EOPNOTSUPP</b>: The operation is not supported for the socket protocol.</li>
 * <li><b>EINVAL</b>: The Name or Namelen are NULL, then EINVAL is returned.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  getsockname
 *
 * @since Huawei LiteOS V100R001C00
 */
int getpeername (int, struct sockaddr *__restrict, socklen_t *__restrict);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API initiate transmission of a message from the specified socket to its peer.
 * This API will send a message only when the socket is connected.
 *
 * @param    s       [IN]   Specifies the socket file descriptor.
 * @param    dataptr [IN]   Specifies a buffer containing message to send.
 * @param    size    [IN]   Specifies the length of the message to send.
 * @param    flags   [IN]   Indicates the flags of message transmission.
 *
 * @attention
 * <ul>
 * <li>UDP & RAW connection can send only a maximum data of length 65000. Sending more data would
 * return -1 and errno set to EMSGSIZE.</li>
 * <li>Only flag MSG_MORE, MSG_DONTWAIT and MSG_NOSIGNAL are supported, other flags, such as MSG_OOB/MSG_EOR is not supported.</li>
 * </ul>
 *
 * @retval    #ssize_t      On success, send() shall return the number of bytes sent. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EWOULDBLOCK</b>: The socket's file descriptor is marked O_NONBLOCK or MSG_DONWAIT flag is set and the requested operation would block.</li>
 * <li><b>EWOULDBLOCK</b>: The socket was not marked with O_NONBLOCK, but set with option SO_SNDTIMEO, and elapsed time is more than timeout value.</li>
 * <li><b>EADDINUSE</b>: For PF_INET/SOCK_DGRAM socket without local port bond, when attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNRESET</b>: A connection was forcibly closed by a peer.</li>
 * <li><b>EDESTADDRREQ</b>: The socket is not connection-mode and does not have its peer address set.</li>
 * <li><b>EINPROGRESS</b>: For PF_INET/SOCK_STREAM, there was one send operation in progress, and concurrent sending was not supported.</li>
 * <li><b>EINVAL</b>: Invalid argument passed, e.g. dataptr is NULL, size is zero.</li>
 * <li><b>EMSGSIZE</b>: The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible. For PF_PACKET/SOCK_RAW socket, it means the packet is larger than the MTU of out network interface. For PF_INET/(SOCK_RAW,SOCK_DGRAM) socket, it means the packet is larger than 65000 bytes.</li>
 * <li><b>ENETDOWN</b>: For PF_PACKET/SOCK_RAW socket, the binding network interface was down.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>ENOMEM</b>: Insufficient memory was available to fulfill the request.</li>
 * <li><b>ENODEVADDR</b>: For PF_PACKET/SOCK_RAW socket, the binding network interface does not exist.</li>
 * <li><b>ENOTCONN</b>: The socket is not connected.</li>
 * <li><b>EOPNOTSUPP</b>: Some bit in the flags argument is unsupported for the socket type.</li>
 * <li><b>EIO</b>: Internal errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  sendto
 *
 * @since Huawei LiteOS V100R001C00
 */
ssize_t send (int, const void *, size_t, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API can be used to receive messages from a connection-oriented sockets only
 * because it doesnot permits the application to retrieve the source address of received data.
 *
 * @param  s          [IN]  Specifies the socket file descriptor.
 * @param  mem        [IN]  Points to a buffer where the message should be stored.
 * @param  len        [IN]  Specifies the length in bytes of the buffer pointed to by the 'mem' argument.
 * @param  flags      [IN]  Specifies the type of message reception. Values of this argument are formed by logically OR'ing zero or more of the following values: MSG_PEEK
 *                          Peeks at an incoming message. The data is treated as unread and the next recv() or similar function shall still return this data.
 *
 * @attention
 * <ul>
 * <li>LwIP updates this receive buffer list, once it gets the next expected segment.
 * If there is any out of order segment which is next to the received segment, means
 * it merges and puts that as one segment into receive buffer list.</li>
 * <li>If the apps don't read the packet from the socket and the recv buffered
 * packets up to MAX_MBOX_SIZE, the incoming packet may be discard by lwIP and cause
 * retransmission for tcp connection.</li>
 * <li>Only flag MSG_PEEK and MSG_DONTWAIT are supported, other flags, such as MSG_TRUNC/MSG_WAITALL/MSG_OOB is not supported.</li>
 * <li>When RST is received after FIN, this API will return 0 to indicate FIN, then
 * return -1 and set errno to ECONNRESET for RST if called again.</li>
 * <li>This API is not thread-safe.</li>
 * </ul>
 *
 * @retval    #ssize_t    On success, recv() shall return the length of the message in bytes. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EWOULDBLOCK</b>: The socket's file descriptor is marked O_NONBLOCK or MSG_DONTWAIT flag is set and no data is waiting to be received.</li>
 * <li><b>EWOULDBLOCK</b>: The socket was not marked with O_NONBLOCK, but set with option SO_RCVTIMEO, and elapsed time is more than timeout value.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNRESET</b>: A connection was forcibly closed by a peer.</li>
 * <li><b>EINVAL</b>: Invalid input parameters. If mem, len is null or flags is less than zero, lwip return failure.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>ENOMEM</b>: Insufficient memory was available to fulfill the request.</li>
 * <li><b>ENOTCONN</b>: A receive is attempted on a connection-mode socket that is not connected.</li>
 * <li><b>EOPNOTSUPP</b>: Some bit in the flags argument is unsupported for the socket type.</li>
 * <li><b>EIO</b>: Internal errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  recvfrom
 *
 * @since Huawei LiteOS V100R001C00
 */
ssize_t recv (int, void *, size_t, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API can be used to send messages to a connection-oriented or connectionless socket.
 * If the socket is connectionless-mode, the message shall be sent to the address specified by 'to'.
 * If the socket is connection-mode, destination address in 'to' shall be ignored.
 *
 * @param    s        [IN]  Specifies a socket file descriptor.
 * @param    dataptr  [IN]  Specifies a buffer containing the message to send.
 * @param    size     [IN]  Specifies the length of the message to send.
 * @param    flags    [IN]  Indicates the flags of message transmission.
 * @param    to       [IN]  Specifies a pointer to the sockaddr structure that contains the destination address.
 * @param    tolen    [IN]  Specifies the size of the 'to' structure.
 *
 * @attention
 * <ul>
 * <li>UDP & RAW connection can send only a maximum data of length 65000. Sending more data would
 * return -1 and errno set to EMSGSIZE.</li>
 * <li>Only flag MSG_MORE, MSG_DONTWAIT and MSG_NOSIGNAL are supported, other flags, such as MSG_OOB/MSG_EOR is not supported.</li>
 * </ul>
 *
 * @retval    #ssize_t       On success, sendto() shall return the number of bytes sent. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EWOULDBLOCK</b>: The socket's file descriptor is marked O_NONBLOCK or MSG_DONTWAIT flag is set and the requested operation would block.</li>
 * <li><b>EWOULDBLOCK</b>: The socket was not marked with O_NONBLOCK, but set with option SO_SNDTIMEO, and elapsed time is more than timeout value.</li>
 * <li><b>EACCES</b>: For PF_INET/(SOCK_RAW,SOCK_DGRAM) socket, the destination address is broadcast but SO_BROADCAST option was not enabled.</li>
 * <li><b>EADDINUSE</b>: For PF_INET/SOCK_DGRAM socket without local port bond, when attempting to bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.</li>
 * <li><b>EAFNOSUPPORT</b>: Addresses in the specified address family cannot be used with this socket.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNRESET</b>: A connection was forcibly closed by a peer.</li>
 * <li><b>EDESTADDRREQ</b>: The socket is not connection-mode and does not have its peer address set, and no destination address was specified.</li>
 * <li><b>EINPROGRESS</b>: For PF_INET/SOCK_STREAM, there was one send operation in progress, and concurrent sending was not supported.</li>
 * <li><b>EINVAL</b>: Invalid argument passed, e.g. dataptr is NULL, size is zero.</li>
 * <li><b>EMSGSIZE</b>: The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible. For PF_PACKET/SOCK_RAW socket, it means the packet is larger than the MTU of out network interface. For PF_INET/(SOCK_RAW,SOCK_DGRAM) socket, it means the packet is larger than 65000 bytes.</li>
 * <li><b>ENETDOWN</b>: For PF_PACKET/SOCK_RAW socket, the out network interface was down.</li>
 * <li><b>ENETUNREACH</b>: No route to the destination is present.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>ENXIO</b>: For PF_PACKET/SOCK_RAW socket, the given network interface does not exist.</li>
 * <li><b>ENOMEM</b>: Insufficient memory was available to fulfill the request.</li>
 * <li><b>ENOTCONN</b>: The socket is not connected.</li>
 * <li><b>EOPNOTSUPP</b>: Some bit in the flags argument is unsupported for the socket type.</li>
 * <li><b>EIO</b>: Internal error.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  send
 *
 * @since Huawei LiteOS V100R001C00
 */
ssize_t sendto (int, const void *, size_t, int, const struct sockaddr *, socklen_t);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API can be used to receive messages from a connection-oriented and connectionless sockets
 * because it permits the application to retrieve the source address of received data.
 *
 * @param    s         [IN]   Specifies the socket file descriptor.
 * @param    mem       [IN]   Points to the buffer where the message should be stored.
 * @param    len       [IN]   Specifies the length in bytes of the buffer pointed to by the buffer argument.
 * @param    flags     [IN]   Specifies the type of message reception. Values of this argument are formed by logically OR'ing zero or more of the following values: MSG_PEEK Peeks at an incoming message. The data is treated as unread and the next recvfrom() or similar function shall still return this data.
 * @param    from      [OUT]  A null pointer, or points to a sockaddr structure in which the sending address is to be stored. The length and format of the address depend on the address family of the socket.
 * @param    fromlen   [OUT]  Either a null pointer, if address is a null pointer, or a pointer to a socklen_t object which on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.
 *
 * @attention
 * <ul>
 * <li>If the apps don't read the packet from the socket and the recv buffered
 * packets up to MAX_MBOX_SIZE, the incoming packet may be discard by lwIP.</li>
 * <li>For TCP connection, when RST is received after FIN, this API will return
 * 0 to indicate FIN, then return -1 and set errno to ECONNRESET for RST if
 * called again.</li>
 * <li>Only flag MSG_PEEK and MSG_DONTWAIT are supported, other flags, such as MSG_TRUNC/MSG_WAITALL/MSG_OOB is not supported.</li>
 * <li>This API is not thread-safe.</li>
 * </ul>
 *
 * @retval     #ssize_t      On success, recvfrom() shall return the length of the message in bytes. On failure, the function shall return -1.
 *
 * @par Errors
 * <ul>
 * <li><b>EWOULDBLOCK</b>: The socket's file descriptor is marked O_NONBLOCK or MSG_DONTWAIT flag is set and no data is waiting to be received.</li>
 * <li><b>EWOULDBLOCK</b>: The socket was not marked with O_NONBLOCK, but set with option SO_RCVTIMEO, and elapsed time is more than timeout value.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNRESET</b>: A connection was forcibly closed by a peer.</li>
 * <li><b>EINVAL</b>: Invalid input parameters. If mem, len is null or flags is less than zero, lwip return failure.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>ENOMEM</b>: Insufficient memory was available to fulfill the request.</li>
 * <li><b>ENOTCONN</b>: A receive is attempted on a connection-mode socket that is not connected.</li>
 * <li><b>EOPNOTSUPP</b>: Some bit in the flags argument is unsupported for the socket type.</li>
 * <li><b>EIO</b>: Internal errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  recv
 *
 * @since Huawei LiteOS V100R001C00
 */
ssize_t recvfrom (int, void *__restrict, size_t, int, struct sockaddr *__restrict, socklen_t *__restrict);

/**
 * @ingroup  socket
 *
 * @par Description:
 *  This API initiates transmission of a message from the specified socket to its peer.
 *  It sends a message only when the socket is connected.  The sendmsg() call also allows
 * sending ancillary data (also known as control information).
 *
 * @param    s       [IN]   Specifies the socket file descriptor.
 * @param    message [IN]   Specifies a buffer containing message to send.
 * @param    flags   [IN]   Indicates the flags of message transmission.
 *
 * @attention
 * <ul>
 * <li>This API is only supported since lwip 2.0.</li>
 * <li>UDP and RAW connection can send only data of maximum length 65332. Sending more than data
 * will return -1 and errno set to EMSGSIZE</li>
 * <li>Only flag MSG_MORE and MSG_DONTWAIT is supported. Other flags, such as
 * MSG_OOB/MSG_NOSIGNAL/MSG_EOR are not supported.</li>
 * </ul>
 *
 * @retval    #ssize_t      On success, sendmsg() shall return the number of bytes sent. On failure, -1 shall be returned.
 *
 * @par Errors
 * <ul>
 * <li><b>EADDINUSE</b>: For PF_INET/SOCK_DGRAM socket without local port bond, when attempting to
 * bind to an ephemeral port, it was determined that all port numbers in the ephemeral port range are currently in use.</li>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>ECONNRESET</b>: A connection was forcibly closed by a peer.</li>
 * <li><b>EDESTADDRREQ</b>: The socket is not connection-mode and does not have its peer address set.</li>
 * <li><b>EINPROGRESS</b>: For PF_INET/SOCK_STREAM, there was one send operation in progress,
 * and concurrent sending was not supported.</li>
 * <li><b>EINVAL</b>: Invalid argument passed, e.g. dataptr is NULL, size is zero.</li>
 * <li><b>EMSGSIZE</b>: The socket type requires that message be sent atomically, and the size of the message
 * to be sent made this impossible. For PF_PACKET/SOCK_RAW socket, it means the packet is larger than the MTU of out network interface. For PF_INET/(SOCK_RAW,SOCK_DGRAM) socket, it means the packet is larger than 65000 bytes.</li>
 * <li><b>ENETDOWN</b>: For PF_PACKET/SOCK_RAW socket, the binding network interface was down.</li>
 * <li><b>ENOBUFS</b>: The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible. For PF_PACKET/SOCK_RAW socket, it means the packet is larger than the MTU of out network interface. For PF_INET/(SOCK_RAW,SOCK_DGRAM) socket, it means the packet is larger than 65000 bytes.</li>
 * <li><b>ENETDOWN</b>: For PF_PACKET/SOCK_RAW socket, the binding network interface was down.</li>
 * <li><b>ENOBUFS</b>: Insufficient resources were available in the system to perform the operation.</li>
 * <li><b>ENODEVADDR</b>: For PF_PACKET/SOCK_RAW socket, the binding network interface does not exist.</li>
 * <li><b>ENOMEM</b>: Insufficient memory was available to fulfill the request.</li>
 * <li><b>ENOTCONN</b>: The socket is not connected.</li>
 * <li><b>EOPNOTSUPP</b>: Some bit in the flags argument is unsupported for the socket type.</li>
 * <li><b>EPIPE</b>: The socket is shut down for writing, or the socket is connection-mode
 * and is no longer connected. In the latter case, and if the socket is of type SOCK_STREAM.</li>
 * <li><b>EWOULDBLOCK</b>: The socket's file descriptor is marked O_NONBLOCK or MSG_DONWAIT flag is set and the requested operation would block.</li>
 * <li><b>EWOULDBLOCK</b>: The socket was not marked with O_NONBLOCK, but set with option SO_SNDTIMEO, and elapsed time is more than timeout value.</li>
 * <li><b>EIO</b>: Internal errors.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  sendto
 *
 * @since Huawei LiteOS V100R001C00
 */
ssize_t sendmsg (int, const struct msghdr *, int);
ssize_t recvmsg (int, struct msghdr *, int);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API retrieves the value for the option specified by the optname argument for the socket
 * specified by 's'. If the size of the optval is greater than optlen, the value stored in the object
 * pointed to by the optval argument shall be silently truncated.
 *
 * @param  s        [IN]      Specifies a socket file descriptor.
 * @param  level    [IN]      Specifies the protocol level at which the option resides.
 * @param  optname  [IN]      Specifies a single option to be retrieved.
 * @param  optval   [OUT]     Indicates an address to store option value.
 * @param  optlen   [IN/OUT]  Specifies the size of the option value.
 *
 * @attention
 * <ul>
 * <li>Supported protocol levels are: SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP,IPPROTO_UDPLITE.</li>
 * <li>Under SOL_SOCKET the options supported are:
 * SO_ACCEPTCONN
 * SO_BROADCAST
 * SO_ERROR
 * SO_KEEPALIVE
 * SO_SNDTIMEO
 * SO_RCVTIMEO
 * SO_RCVBUF
 * SO_SNDBUF
 * SO_REUSEADDR
 * SO_TYPE
 * SO_NO_CHECK
 * SO_DONTROUTE
 * SO_BINDTODEVICE
 * For SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF, the macros LWIP_SO_SNDTIMEO, LWIP_SO_RCVTIMEO and LWIP_SO_RCVBUF should have been defined at compile time.
 * For SO_REUSEADDR, the macro SO_REUSE should have been defined at compile time.
 * For SO_BINDTODEVICE, the macro LWIP_SO_BINDTODEVICE should have been defined at compile time.
 * For SO_SNDBUF, the macro LWIP_SO_SNDBUF should have been defined at compile time.
 * </li>
 * <li>Under IPPROTO_IP the options supported are:
 * IP_MULTICAST_TTL
 * IP_MULTICAST_LOOP
 * IP_MULTICAST_IF
 * IP_HDRINCL
 * IP_TTL
 * IP_TOS
 * Only UDP socket supports IP_MULTICAST_TTL, IP_MULTICAST_LOOP, IP_MULTICAST_IF.
 * </li>
 * <li>Under IPPROTO_TCP the options supported are:
 * TCP_NODELAY
 * TCP_KEEPIDLE
 * TCP_KEEPINTVL
 * TCP_KEEPCNT
 * TCP_INFO
 * TCP_QUEUE_SEQ.
 * For TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT, the macro LWIP_TCP_KEEPALIVE should have been defined at compile time.\n
 * For TCP_INFO, Only tcpi_state, tcpi_retransmits, tcpi_probes, tcpi_backoff, tcpi_options, tcpi_rto, tcpi_snd_mss, tcpi_rcv_mss, tcpi_unacked,
 * tcpi_rtt, tcpi_rttvar, tcpi_snd_ssthresh, tcpi_snd_cwnd, tcpi_reordering in
 * struct tcp_info are supported, other fields are keep 0.
 * </li>
 * <li>Under IPPROTO_UDPLITE the options supported are:
 * UDPLITE_SEND_CSCOV
 * UDPLITE_RECV_CSCOV
 * </li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EINVAL</b>: The specified option is invalid at the specified socket level.</li>
 * <li><b>EINVAL</b>: If opt or optlen are NULL,option or option length is incorrect.</li>
 * <li><b>ENOPROTOOPT</b>: The option is not supported by the protocol.</li>
 * <li><b>ENOMEM</b>: There was insufficient memory available to complete the operation.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  setsockopt
 *
 * @since Huawei LiteOS V100R001C00
 */
int getsockopt (int, int, int, void *__restrict, socklen_t *__restrict);

/**
 * @ingroup  socket
 *
 * @par Description:
 * This API sets the option specified by the optname, at the protocol level specified by the level,
 * to the value pointed to by the optval for the socket associated with the file descriptor specified by 's'.
 *
 * @param  s        [IN]      Specifies a socket file descriptor.
 * @param  level    [IN]      Specifies the protocol level at which the option resides.
 * @param  optname  [IN]      Specifies a single option to set.
 * @param  optval   [IN]      Indicates the address to store the option value.
 * @param  optlen   [IN]      Specifies the size of option value.
 *
 * @attention
 * <ul>
 * <li>Supported protocol levels are: SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP,IPPROTO_UDPLITE.</li>
 * <li>Under SOL_SOCKET the options supported are:
 * SO_BROADCAST
 * SO_KEEPALIVE
 * SO_SNDTIMEO
 * SO_RCVTIMEO
 * SO_RCVBUF
 * SO_SNDBUF
 * SO_REUSEADDR
 * SO_REUSEPORT
 * SO_NO_CHECK.
 * SO_ATTACH_FILTER
 * SO_DETACH_FILTER
 * SO_DONTROUTE
 * SO_BINDTODEVICE
 * For SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF, the macros LWIP_SO_SNDTIMEO, LWIP_SO_RCVTIMEO and LWIP_SO_RCVBUF should have been defined at compile time.
 * For SO_REUSEADDR, the macro SO_REUSE should have been defined at compile time.
 * For SO_BINDTODEVICE, the macro LWIP_SO_BINDTODEVICE should have been defined at compile time.
 * For SO_ATTACH_FILTER, SO_DETACH_FILTER, the macro LWIP_SOCKET_FILTER should have been defined at compile time.
 * Only PF_PACKET RAW socket supports SO_ATTACH_FILTER and SO_DETACH_FILTER now.
 * For SO_SNDBUF, the macro LWIP_SO_SNDBUF should have been defined at compile time.
 * Only TCP socket in listen or closed stated supports SO_SNDBUF.
 * </li>
 * <li>Under IPPROTO_IP the options supported are:
 * IP_ADD_MEMBERSHIP
 * IP_DROP_MEMBERSHIP
 * IP_MULTICAST_TTL
 * IP_MULTICAST_LOOP
 * IP_MULTICAST_IF
 * IP_HDRINCL
 * IP_TTL
 * IP_TOS
 * Only UDP socket supports IP_ADD_MEMBERSHIP, IP_DROP_MEMBERSHIP, IP_MULTICAST_TTL, IP_MULTICAST_LOOP, IP_MULTICAST_IF.
 * </li>
 * <li>Under IPPROTO_TCP the options supported are:
 * TCP_NODELAY
 * TCP_KEEPIDLE
 * TCP_KEEPINTVL
 * TCP_KEEPCNT.
 * For TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT, the macro LWIP_TCP_KEEPALIVE should have been defined at compile time.
 * </li>
 * <li>Under IPPROTO_UDPLITE the options supported are:
 * UDPLITE_SEND_CSCOV
 * UDPLITE_RECV_CSCOV
 * </li>
 * </ul>
 *
 * @retval    #0    On success.
 * @retval    #-1   On failure.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The socket argument is not a valid file descriptor.</li>
 * <li><b>EDOM</b>: The send and receive timeout values are too big to fit into the timeout fields in the socket structure.</li>
 * <li><b>ENOENT</b>: the option is SO_DETACH_FILTER while no previous socket filter was attached.</li>
 * <li><b>ENOMEM</b>: There was insufficient memory available for the operation to complete.</li>
 * <li><b>ENOPROTOOPT</b>: The option is not supported by the protocol.</li>
 * <li><b>EINVAL</b>: If optval is NULL pointer, lwIP shall fail.</li>
 * <li><b>EINVAL</b>: If for PF_PACKET socket type, level is neither SOL_PACKET nor SOL_SOCKET, lwIP shall fail.</li>
 * <li><b>EINVAL</b>: If optlen does not match optval for corresponding optname, lwIP shall fail.</li>
 * <li><b>EINVAL</b>: If the send and receive timeout values are smaller than 10000 microseconds, lwIP shall fail.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>socket.h</li></ul>
 *
 * @see  getsockopt
 *
 * @since Huawei LiteOS V100R001C00
 */
int setsockopt (int, int, int, const void *, socklen_t);

int sockatmark (int);

#if _REDIR_TIME64
#ifdef _GNU_SOURCE
__REDIR(recvmmsg, __recvmmsg_time64);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
