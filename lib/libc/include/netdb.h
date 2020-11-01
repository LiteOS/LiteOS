#ifndef	_NETDB_H
#define	_NETDB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>
#include <netinet/in.h>

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_size_t
#include <bits/alltypes.h>
#endif

struct addrinfo {
	int ai_flags;
	int ai_family;
	int ai_socktype;
	int ai_protocol;
	socklen_t ai_addrlen;
	struct sockaddr *ai_addr;
	char *ai_canonname;
	struct addrinfo *ai_next;
};

#define AI_PASSIVE      0x01
#define AI_CANONNAME    0x02
#define AI_NUMERICHOST  0x04
#define AI_V4MAPPED     0x08
#define AI_ALL          0x10
#define AI_ADDRCONFIG   0x20
#define AI_NUMERICSERV  0x400


#define NI_NUMERICHOST  0x01
#define NI_NUMERICSERV  0x02
#define NI_NOFQDN       0x04
#define NI_NAMEREQD     0x08
#define NI_DGRAM        0x10
#define NI_NUMERICSCOPE 0x100

#define EAI_BADFLAGS   -1
#define EAI_NONAME     -2
#define EAI_AGAIN      -3
#define EAI_FAIL       -4
#define EAI_FAMILY     -6
#define EAI_SOCKTYPE   -7
#define EAI_SERVICE    -8
#define EAI_MEMORY     -10
#define EAI_SYSTEM     -11
#define EAI_OVERFLOW   -12

/**
 * @ingroup  netdb
 *
 * @par Description:
 *  This API is used to resolve a hostname (string) or a service name and into an IP_add.
 *  And this addresses associated information to be used in creating a socket with which to address the
 *  specified service.
 *
 * @param    nodename     [IN]   The descriptive name or address string of the host.
 * @param    servname     [IN]   The servname points to string of port number or NULL.
 * @param    hints        [IN]   The structure containing input values that set socktype and protocol.
 * @param    res          [OUT]  The pointer to a pointer where to store the result (set to NULL on failure).
 *
 * @attention
 * <ul>
 * <li>Service names are not supported (only port numbers)!</li>
 * </ul>
 *
 * @retval  #0            On success.
 * @retval  #EAI_FAIL     The name server returned a permanent failure indication.
 * @retval  #EAI_NONAME   The nodename and servname are NULL.
 * @retval  #EAI_SERVICE  The requested servname is not available. Only port number is supported.
 * @retval  #EAI_MEMORY   Out of memory.
 *
 * @par Dependency:
 * <ul><li>netdb.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
int getaddrinfo (const char *__restrict, const char *__restrict, const struct addrinfo *__restrict, struct addrinfo **__restrict);

/**
 * @ingroup  netdb
 *
 * @par Description:
 * This API is used to free one or more addrinfo structures. If the ai_next field
 * of the structure is not null, the entire list of structures is freed.
 *
 * @param    ai     [IN]   The struct addrinfo to free.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval   #void     None.
 *
 * @par Errors
 * <ul>
 * <li>None</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>netdb.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
void freeaddrinfo (struct addrinfo *);

/* Android ABI error: POSIX getnameinfo(3) uses socklen_t rather than size_t. */
/**
 * @ingroup  netdb
 *
 * @par Description:
 * Converts a socket address to a corresponding host and service, in a
 * protocol-independent manner. It is reentrant and allows programs to eliminate
 * IPv4-versus-IPv6 dependencies.
 * Memory for the result is allocated by the caller.
 *
 * @param    sa        [IN]   Indicates a pointer to a generic socket address structure (of type sockaddr_in or sockaddr_in6) that holds the input IP address and port number.
 * @param    salen     [IN]   Indicates the size of the generic socket address structure "sa".
 * @param    host      [OUT]  Indicates a pointer to caller-allocated buffer which will holds the null terminated hostname string.
 * @param    hostlen   [IN]   Indicates the size of "host" buffer.
 * @param    serv      [OUT]  Indicates a pointer to caller-allocated buffer which will holds the null terminated service-name string.
 * @param    servlen   [IN]   Indicates the size of "serv" buffer.
 * @param    flags     [IN]   Used to modify the behavior of getnameinfo() and can have the following values:
 *           NI_NAMEREQD : If set, then an error is returned if the hostname cannot be determined.
 *           NI_DGRAM : If set, then the service is datagram (UDP) based rather than stream (TCP) based and causes getservbyport()
 *                      to be called with a second  argument of "udp" instead of its default of "tcp".  This is required for the few ports
 *                      (512-514) that have different services for UDP and TCP.
 *           NI_NOFQDN : If set, return only the hostname part of the fully qualified domain name for local hosts.
 *           NI_NUMERICHOST : If set, then the numeric form of the hostname is returned. (When not set, this will still happen in case the node's name
 *                            cannot be determined.)
 *           NI_NUMERICSERV : If set, then the numeric form of the service address is returned.  (When not set, this will still happen in case the
 *                            service's name cannot be determined.)
 *
 *
 * @attention
 * <ul>
 * <li>This API is only supported since lwip 2.0.</li>
 * <li>No support for translation of service names.</li>
 * <li>Since there is no support for Service names, the flags - NI_DGRAM, NI_NUMERICSERV is not supported.</li>
 * <li>NI_NOFQDN is not currently implemented.</li>
 * <li>Reverse DNS Query for the IP Address, if needed, will be sent out immediately.</li>
 * <li>If the time gap between the transmission of first Reverse DNS Query and the periodic
 *     retransmission of the same query is less than 100ms, then the periodic retransmission
 *     will be postponed by another 1100ms.</li>
 * </ul>
 *
 * @retval #0             On success.
 * @retval #EAI_FAIL      A nonrecoverable error occurred.
 * @retval #EAI_FAMILY    The requested address family is not supported.
 * @retval #EAI_NONAME    The name does not resolve for the supplied arguments. NI_NAMEREQD is set and the host's name cannot be located, or
 *                        neither hostname nor service name were requested.
 * @retval #EAI_OVERFLOW  The buffer pointed to by host or serv was too small.
 * @retval #EAI_BADFLAGS  The "flags" argument has an invalid value.
 *
 * @par Dependency:
 * <ul><li>netdb.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
int getnameinfo (const struct sockaddr *__restrict, socklen_t, char *__restrict, socklen_t, char *__restrict, socklen_t, int);
const char *gai_strerror(int);


/* Legacy functions follow (marked OBsolete in SUS) */

struct netent {
	char *n_name;
	char **n_aliases;
	int n_addrtype;
	uint32_t n_net;
};

struct hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
};
#define h_addr h_addr_list[0]

struct servent {
	char *s_name;
	char **s_aliases;
	int s_port;
	char *s_proto;
};

struct protoent {
	char *p_name;
	char **p_aliases;
	int p_proto;
};

void sethostent (int);
void endhostent (void);
struct hostent *gethostent (void);

void setnetent (int);
void endnetent (void);
struct netent *getnetent (void);
struct netent *getnetbyaddr (uint32_t, int);
struct netent *getnetbyname (const char *);

void setservent (int);
void endservent (void);
struct servent *getservent (void);
struct servent *getservbyname (const char *, const char *);
struct servent *getservbyport (int, const char *);

void setprotoent (int);
void endprotoent (void);
struct protoent *getprotoent (void);
struct protoent *getprotobyname (const char *);
struct protoent *getprotobynumber (int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_POSIX_SOURCE) \
 || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE+0 < 200809L) \
 || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE+0 < 700)

/**
 * @ingroup  netdb
 *
 * @par Description:
 * This API is used to resolve a hostname (string) into an IP_add.
 *
 * @param    name     [IN]  The hostname that is to be resolved.
 *
 * @attention
 * <ul>
 * <li>This API can give more than one IP_add.</li>
 * <li>While parsing the multiple answer rcrds in DNS response message, if
 * it encounters any malformed answer rcrd then it stops parsing and returns
 * success if it has any successfully parsed rcrd or else it returns failure.</li>
 * </ul>
 *
 * @retval  #hostent*  On success, pointer to the hostent structure is returned. On failure, NULL shall be returned.
 *
 * @par Dependency:
 * <ul><li>netdb.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
struct hostent *gethostbyname (const char *);
struct hostent *gethostbyaddr (const void *, socklen_t, int);
#ifdef __GNUC__
__attribute__((const))
#endif
int *__h_errno_location(void);
#define h_errno (*__h_errno_location())
#define HOST_NOT_FOUND 1
#define TRY_AGAIN      2
#define NO_RECOVERY    3
#define NO_DATA        4
#define NO_ADDRESS     NO_DATA
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void herror(const char *);
const char *hstrerror(int);

/**
 * @ingroup  netdb
 *
 * @par Description:
 * This API is used to resolve a hostname (string) into an IP_add. This function is Thread-safe variant of gethostbyname().
 *
 * @param    name     [IN]   The hostname that is to be resolved.
 * @param    ret      [OUT]  The pre-allocated struct where the result is stored.
 * @param    buf      [IN]   The pre-allocated buffer where additional data is stored.
 * @param    buflen   [IN]   The size of the buf.
 * @param    result   [OUT]  This is a double pointer to hostent which is set to ret on success and set to zero on error scenario.
 * @param    h_errnop [OUT]  It stores the errors.
 *
 * @attention
 * <ul>
 * <li>Additional error information is stored in *h_errnop instead of h_errno to be thread-safe.</li>
 * <li>This API can give more than one IP_add also.</li>
 * <li>While parsing the multiple answer rcrds in DNS response message, if
 * it encounters any malformed answer rcrd then it stops parsing and returns
 * success if it has any successfully parsed rcrd or else it returns failure.</li>
 * </ul>
 *
 * @retval  #0     On success, pointer to the hostent structure is returned.
 * @retval  #-1    On failure, the h_errnop variable will contain one among the following error codes in case of failure.\n
 *                 HOST_NOT_FOUND : Hostname not found in DNS Server.\n
 *                 EINVAL : Invalid arguments.\n
 *                 ERANGE : Result not representable due to storage size limitations.
 *
 * @par Dependency:
 * <ul><li>netdb.h</li></ul>
 *
 * @see  None
 *
 * @since Huawei LiteOS V100R001C00
 */
int gethostbyname_r(const char *, struct hostent *, char *, size_t, struct hostent **, int *);
int gethostbyname2_r(const char *, int, struct hostent *, char *, size_t, struct hostent **, int *);
struct hostent *gethostbyname2(const char *, int);
int gethostbyaddr_r(const void *, socklen_t, int, struct hostent *, char *, size_t, struct hostent **, int *);
int getservbyport_r(int, const char *, struct servent *, char *, size_t, struct servent **);
int getservbyname_r(const char *, const char *, struct servent *, char *, size_t, struct servent **);
#define EAI_NODATA     -5
#define EAI_ADDRFAMILY -9
#define EAI_INPROGRESS -100
#define EAI_CANCELED   -101
#define EAI_NOTCANCELED -102
#define EAI_ALLDONE    -103
#define EAI_INTR       -104
#define EAI_IDN_ENCODE -105
#define NI_MAXHOST 255
#define NI_MAXSERV 32
#endif


#ifdef __cplusplus
}
#endif

#endif
