

#ifndef __SAL_H
#define __SAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <sal_define.h>
#include <sal_types.h>
#include <link_endian.h>

/**
 *
 *@brief: list all the api need for the application development,,supported depends on the implement of
 *       the tcpip stack
 *  Thanks to BSD && linux, which must be compatible
 */

int sal_socket(int domain, int type, int protocol);

int sal_bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen);

int sal_listen(int sockfd,  int backlog);

int sal_accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);

int sal_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

int sal_getsockname(int sockfd, struct sockaddr *addr,socklen_t *addrlen);

int sal_getpeername(int sockfd, struct sockaddr *addr,socklen_t *addrlen);

int sal_getsockopt (int socket, int level, int optname, void *optval, int *optlen);

int sal_setsockopt (int socket, int level, int optname, const void *optval, int optlen);

int sal_recv(int sockfd,void *buf,size_t len,int flags);

int sal_recvfrom(int sockfd, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen);

int sal_send(int sockfd,const void *buf,size_t len,int flags);

int sal_sendto(int sockfd, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen);

int sal_shutdown(int sockfd, int how);

int sal_closesocket(int sockfd);

struct hostent * sal_gethostbyname(const char *name);


/**
 * @brief: if you want to use the tcpip abstract layer,like install the tcpip stack,
 *         or use the api as the application user, you must call this function first
 * @param[in]:socknum, how many socket the application could use mostly
 *
 * @return:0 success while -1 failed
 * */
int tcpipstack_init( int socknum);

//define the normal addres function
#define swaps(value) ((((value)&((uint16_t)0xff00))>>8)|(((value)&((uint16_t)0x00ff))<<8))

#define swapl(value)  ((((value)&((uint32_t)0xff000000))>>24)|(((value)&((uint32_t)0xff0000))>>8)|\
                      (((value)&((uint32_t)0xff00))<<8)|(((value)&((uint32_t)0xff))<<24))

#if cfg_endian_big

#define htoles(value)  swaps(value)
#define htolel(value)  swapl(value)
#define htobes(value)  (value)
#define htobel(value)  (value)

#else
#define htoles(value)  (value)
#define htolel(value)  (value)
#define htobes(value)  swaps(value)
#define htobel(value)  swapl(value)
#endif

#ifndef htons
#define htons      htobes      //translate the host endian to network endian (2 Bytes)
#endif

#ifndef ntohs
#define htonl      htobel      //translate the host endian to network endian (4 Bytes)
#endif

#ifndef ntohs
#define ntohs      htobes      //translate the network endian to host endian (2 Bytes)
#endif

#ifndef ntohl
#define ntohl      htobel      //translate the network endian to host endian (4 Bytes)
#endif

in_addr_t inet_addr(const char *addr);
char *inet_ntoa(struct in_addr addr);
int inet_aton(const char *string,struct in_addr *addr);
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);



#endif /* __SAL_H */

