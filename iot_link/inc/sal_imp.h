/*
 * sal_imp.h
 *
 *  Created on: 2019Äê4ÔÂ26ÈÕ
 *      Author: zhangqf
 */

#ifndef __SAL_IMP_H
#define __SAL_IMP_H

/**
 * @brief: all the function model,please refer to the
 *       : you could declare and add more functions here
 *
 * */

typedef int (*fn_sal_socket)(int domain, int type, int protocol);

typedef int (*fn_sal_bind)(int sock,const void *addr,int addrlen);

typedef int (*fn_sal_listen)(int sock,  int backlog);

typedef int (*fn_sal_accept)(int sock,void *addr,int *addrlen);

typedef int (*fn_sal_connect)(int sock, const void *addr,int addrlen);

typedef int (*fn_sal_getsockname)(int sock, void *addr,int *addrlen);

typedef int (*fn_sal_getpeername)(int sock, void *addr,int *addrlen);

typedef int (*fn_sal_getsockopt)(int sock, int level, int optname,\
                                 void *optval, int *optlen);
typedef int (*fn_sal_setsockopt)(int sock, int level, int optname,\
                                 const void *optval, int optlen);

typedef int (*fn_sal_recv)(int sock, void *mem, int len, int flags);

typedef int (*fn_sal_recvfrom)(int sock, void *mem, int len, int flags,
                              void *from, int *fromlen);

typedef int (*fn_sal_send)(int sock,const void *buf,int len,int flags);

typedef int (*fn_sal_sendto)(int sock, const void *dataptr, int size, int flags,
                             const void *to, int tolen);

typedef int (*fn_sal_shutdown)(int sock, int how);

typedef int (*fn_sal_closesocket)(int sock);

typedef void * (*fn_sal_gethostbyname)(const char *name);


typedef struct
{
    fn_sal_socket           socket;
    fn_sal_bind             bind;
    fn_sal_listen           listen;
    fn_sal_accept           accept;
    fn_sal_connect          connect;
    fn_sal_getsockname      getsockname;
    fn_sal_getpeername      getpeername;
    fn_sal_setsockopt       setsockopt;
    fn_sal_getsockopt       getsockopt;
    fn_sal_send             send;
    fn_sal_sendto           sendto;
    fn_sal_recv             recv;
    fn_sal_recvfrom         recvfrom;
    fn_sal_shutdown         shutdown;
    fn_sal_closesocket      closesocket;
    fn_sal_gethostbyname    gethostbyname;
}tag_tcpip_ops;


typedef struct
{
    const char           *name;    ///< this member reserved now
    int                   domain;  ///< this member to match the tcpip ops,likes AF_INET and so on
    const tag_tcpip_ops  *ops;     ///< this member used to implement the user's operation
}tag_tcpip_domain;


/**
 * @brief: the tcpip stack should use this function to install it to the socket
 *         abstract layer, and the after the installation, the user could use
 *         socket abstract api or the BSD api as your need
 * @param[in]: domain, refer to the structure tag_tcpip_domain,remember that it must
 *             not changed after the installation
 * @return: 0 success while -1 failed
 * */
int tcpip_sal_install(const tag_tcpip_domain *domain);


#endif /* __SAL_IMP_H */
