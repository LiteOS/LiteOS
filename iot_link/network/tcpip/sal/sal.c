/*
 * sal.c
 *
 *  Created on: 2019Äê4ÔÂ26ÈÕ
 *      Author: zhangqf
 */

///< attention that this file is the main implement
#include <osal.h>
#include <sal.h>
#include <sal_imp.h>

#if CONFIG_TCPIP_ENABLE

typedef struct
{
    const tag_tcpip_ops    *ops;
    int                     fd;
    int                     sock;          ///< returned by the implement functions
}tag_sock_cb;

typedef struct
{
    const tag_tcpip_domain *domain;         ///< tcpip stack operations
    int                     sock_cb_num;    ///< how many socket control block could be used
    osal_mutex_t            sock_cb_mutex;  ///< used to protect the sock control block
    void                  **sock_cb_tab;    ///< which used to
}tag_sal_cb;

static tag_sal_cb   s_sal_cb;

int tcpipstack_init( int socknum)
{
    int ret = -1;

    if(socknum <= 0)
    {
        goto EXIT_PARA_ERR;
    }

    if(NULL != s_sal_cb.sock_cb_tab)
    {
        goto EXIT_INIT_ERR;
    }

    if(false == osal_mutex_create(&s_sal_cb.sock_cb_mutex))
    {
        goto EXIT_MUTEX_ERR;
    }

    s_sal_cb.sock_cb_tab = osal_malloc(socknum*sizeof(void *));
    if(NULL == s_sal_cb.sock_cb_tab)
    {
        goto EXIT_MEM_ERR;
    }

    memset(s_sal_cb.sock_cb_tab,0,socknum*sizeof(void *));
    s_sal_cb.sock_cb_num = socknum;
    s_sal_cb.domain = NULL;

    ret = 0;

    return ret;


EXIT_MEM_ERR:
    osal_mutex_del(s_sal_cb.sock_cb_mutex);
    s_sal_cb.sock_cb_mutex = cn_mutex_invalid;

EXIT_MUTEX_ERR:
EXIT_INIT_ERR:
EXIT_PARA_ERR:
    return ret;

}

int tcpip_sal_install(const tag_tcpip_domain *domain)
{
    int ret = -1;

    if(NULL == s_sal_cb.domain)
    {
        s_sal_cb.domain = domain;

        ret = 0;
    }

    return ret;

}

///< malloc a sock fd and memory
static void * __sal_sockcb_malloc()
{
    int i =  0;

    tag_sock_cb *sockcb;

    sockcb = osal_malloc(sizeof(tag_sock_cb));

    if(NULL == sockcb)
    {
        return sockcb;
    }

    if(osal_mutex_lock(s_sal_cb.sock_cb_mutex))
    {
        for(i = 0;i<s_sal_cb.sock_cb_num;i++ )
        {
            if(s_sal_cb.sock_cb_tab[i] == NULL)
            {
                sockcb->ops = s_sal_cb.domain->ops;
                sockcb->fd = i;
                sockcb->sock = -1;

                s_sal_cb.sock_cb_tab[i] = sockcb;

                break;
            }
        }

        osal_mutex_unlock(s_sal_cb.sock_cb_mutex);

        if(i == s_sal_cb.sock_cb_num)
        {
            osal_free(sockcb);

            sockcb = NULL;
        }

    }

    return sockcb;
}

///< free the sockfd and the memory in it
static void __sal_sockcb_free(int sockfd)
{
    tag_sock_cb *sockcb = NULL;

    if((sockfd <0 ) || (sockfd >= s_sal_cb.sock_cb_num))
    {
        return;
    }

    if(osal_mutex_lock(s_sal_cb.sock_cb_mutex))
    {

        sockcb = s_sal_cb.sock_cb_tab[sockfd];
        s_sal_cb.sock_cb_tab[sockfd]= NULL;

        osal_mutex_unlock(s_sal_cb.sock_cb_mutex);

        osal_free(sockcb);
    }

    return;
}


///< use this function to get the sockcb
static tag_sock_cb * __sal_sockcb_getcb(int sockfd)
{
    tag_sock_cb *sockcb = NULL;

    if((sockfd >=0 ) && (sockfd < s_sal_cb.sock_cb_num))
    {
        sockcb = s_sal_cb.sock_cb_tab[sockfd];
    }

    return sockcb;
}

static int   __sal_sockcb_getfd(tag_sock_cb *sockcb)
{
    int sockfd = -1;

    if(NULL != sockcb)
    {
        sockfd = sockcb->fd;
    }

    return sockfd;
}


int sal_socket(int domain, int type, int protocol)
{
    int sockfd = -1;
    int sock = -1;

    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_malloc();

    if(NULL != sockcb)
    {
        if((NULL != s_sal_cb.domain)&&(NULL != s_sal_cb.domain->ops) &&\
           (NULL != s_sal_cb.domain->ops->socket))
        {
            sock = s_sal_cb.domain->ops->socket(domain,type,protocol);
        }
        if(-1 != sock)
        {
            sockcb->sock = sock;
            sockfd = __sal_sockcb_getfd(sockcb);
        }
        else
        {
            __sal_sockcb_free(sockcb->fd);
        }
    }


    return sockfd;
}

int sal_bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen)
{
    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops) && (NULL != sockcb->ops->bind))
        {
            ret = sockcb->ops->bind(sockcb->sock,addr,addrlen);
        }
    }

    return ret;
}


int sal_listen(int sockfd,  int backlog)
{
    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->listen))
        {
            ret = sockcb->ops->listen(sockcb->sock,backlog);
        }
    }

    return ret;
}

int sal_accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int sockfd_client = -1;
    int sock_client = -1;
    tag_sock_cb *sockcb_client;

    tag_sock_cb *sockcb_server;

    sockcb_server = __sal_sockcb_getcb(sockfd);
    if(NULL == sockcb_server)
    {
        return sockfd_client;
    }

    sockcb_client = __sal_sockcb_malloc();

    if(NULL != sockcb_client)
    {
        if((NULL != sockcb_server->ops)&&(NULL != sockcb_server->ops->accept))
        {
            sock_client = sockcb_server->ops->accept(sockcb_server->sock,addr,addrlen);
        }
        if(-1 != sock_client)
        {
            sockcb_client->sock = sock_client;
            sockfd_client = __sal_sockcb_getfd(sockcb_client);
        }
        else
        {
            __sal_sockcb_free(sockcb_client->fd);
        }
    }


    return sockfd_client;
}


int sal_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->connect))
        {
            ret = sockcb->ops->connect(sockcb->sock,addr,addrlen);
        }
    }

    return ret;
}

int sal_getsockname(int sockfd, struct sockaddr *addr,socklen_t *addrlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->getsockname))
        {
            ret = sockcb->ops->getsockname(sockcb->sock,addr,addrlen);
        }
    }

    return ret;
}


int sal_getpeername(int sockfd, struct sockaddr *addr,socklen_t *addrlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->getpeername))
        {
            ret = sockcb->ops->getpeername(sockcb->sock,addr,addrlen);
        }
    }

    return ret;
}


int sal_getsockopt (int sockfd, int level, int optname, void *optval, int *optlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->getsockopt))
        {
            ret = sockcb->ops->getsockopt(sockcb->sock,level,optname,optval,optlen);
        }
    }

    return ret;
}

int sal_setsockopt (int sockfd, int level, int optname, const void *optval, int optlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->setsockopt))
        {
            ret = sockcb->ops->setsockopt(sockcb->sock,level,optname,optval,optlen);
        }
    }

    return ret;
}

int sal_recv(int sockfd,void *buf,size_t len,int flags)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->recv))
        {
            ret = sockcb->ops->recv(sockcb->sock,buf,len,flags);
        }
    }

    return ret;
}

int sal_recvfrom(int sockfd, void *buf, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->recvfrom))
        {
            ret = sockcb->ops->recvfrom(sockcb->sock,buf,len,flags,from,fromlen);
        }
    }

    return ret;
}

int sal_send(int sockfd,const void *buf,size_t len,int flags)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->send))
        {
            ret = sockcb->ops->send(sockcb->sock,buf,len,flags);
        }
    }

    return ret;
}

int sal_sendto(int sockfd, const void *buf, size_t len, int flags,
    const struct sockaddr *to, socklen_t tolen)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->sendto))
        {
            ret = sockcb->ops->sendto(sockcb->sock,buf,len,flags,to,tolen);
        }
    }

    return ret;
}


int sal_shutdown(int sockfd, int how)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->shutdown))
        {
            ret = sockcb->ops->shutdown(sockcb->sock,how);
        }
    }

    return ret;
}

int sal_closesocket(int sockfd)
{

    int ret = -1;
    tag_sock_cb *sockcb;

    sockcb = __sal_sockcb_getcb(sockfd);
    if(NULL != sockcb)
    {
        if((NULL != sockcb->ops)&&(NULL != sockcb->ops->closesocket))
        {
            ret = sockcb->ops->closesocket(sockcb->sock);
        }

        __sal_sockcb_free(sockcb->fd);
    }

    return ret;
}

struct hostent * sal_gethostbyname(const char *name)
{
    if((NULL != s_sal_cb.domain)&&(NULL != s_sal_cb.domain->ops) &&\
       (NULL != s_sal_cb.domain->ops->gethostbyname))
    {
        return (struct hostent *)s_sal_cb.domain->ops->gethostbyname(name);
    }
    return NULL;
}


#endif


