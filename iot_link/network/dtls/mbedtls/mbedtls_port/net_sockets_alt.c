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
#include <stdlib.h>
#include <osal.h>
#include <sal.h>

#include "mbedtls/net_sockets.h"

///< attention that we only port the ip dot mode and only think about the ipv4

void mbedtls_net_init(mbedtls_net_context *ctx)
{
    ctx->fd = -1;
}

///< suppose it to be ipv4 and ip dot host
void *mbedtls_net_connect(const char *host, const char *port, int proto)
{
    mbedtls_net_context *ret;
    struct sockaddr_in addr;

    ret = osal_zalloc(sizeof(mbedtls_net_context));
    if(NULL == ret)
    {
        return ret;
    }

    ret->fd = -1;

    if(proto == MBEDTLS_NET_PROTO_TCP)
    {
        ret->fd = sal_socket(AF_INET,SOCK_STREAM,0);
    }
    else if(proto == MBEDTLS_NET_PROTO_UDP)
    {
        ret->fd = sal_socket(AF_INET,SOCK_DGRAM,0);
    }
    else
    {
        ret->fd = -1;
    }

    if(ret->fd == -1)
    {
        osal_free(ret);
        ret = NULL;
        return ret;
    }

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(atoi(port));

    if(-1 == sal_connect(ret->fd,(struct sockaddr *)&addr,sizeof(addr)))
    {
        sal_closesocket(ret->fd);
        osal_free(ret);
        ret = NULL;
    }

    return ret;
}

void mbedtls_net_usleep(unsigned long usec)
{
    osal_task_sleep(usec/1000);///--TODO, we only support the ms delay for the system
}

int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
    int fd;
    int ret = MBEDTLS_ERR_NET_RECV_FAILED;
    int rcvlen = -1;

    if(NULL == ctx)
    {
        return ret;
    }

    fd = ((mbedtls_net_context *)ctx)->fd;
    rcvlen = sal_recv(fd,buf,len,0);
    if(rcvlen == 0)
    {
        ret = MBEDTLS_ERR_NET_RECV_FAILED;
    }
    else if(rcvlen < 0)
    {
        ret = MBEDTLS_ERR_SSL_WANT_READ;
    }
    else
    {
        ret = rcvlen;
    }
    return ret;
}

#include <time.h>

int mbedtls_net_recv_timeout(void *ctx, unsigned char *buf, size_t len,
                             uint32_t timeout)
{
    int fd;
    int ret = MBEDTLS_ERR_NET_RECV_FAILED;
    int rcvlen = -1;

    struct timeval timedelay;


    if(NULL == ctx)
    {
        return ret;
    }

    fd = ((mbedtls_net_context *)ctx)->fd;

    timedelay.tv_sec = timeout/1000;
    timedelay.tv_usec = (timeout%1000)*1000;

    ///< set the recv timeout
    if(0 != sal_setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,&timedelay,sizeof(timedelay)))
    {
        return ret;  //could not support the rcv timeout
    }

    rcvlen = sal_recv(fd,buf,len,0);
    if(rcvlen == 0)
    {
        ret = MBEDTLS_ERR_NET_RECV_FAILED;
    }
    else if(rcvlen < 0)
    {
        ret = MBEDTLS_ERR_SSL_WANT_READ;
    }
    else
    {
        ret = rcvlen;
    }
    return ret;
}

int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len)
{
    int fd;
    int ret = MBEDTLS_ERR_NET_SEND_FAILED;
    int senlen = -1;

    if(NULL == ctx)
    {
        return ret;
    }

    fd = ((mbedtls_net_context *)ctx)->fd;

    senlen = sal_send(fd,buf,len,0);
    if(senlen == 0)
    {
        ret = MBEDTLS_ERR_NET_SEND_FAILED;
    }
    else if(senlen < 0)
    {
        ret = MBEDTLS_ERR_SSL_WANT_WRITE;
    }
    else
    {
        ret = senlen;
    }

    return ret;
}

void mbedtls_net_free(mbedtls_net_context *ctx)
{
    if(NULL != ctx)
    {
        sal_closesocket(((mbedtls_net_context *)ctx)->fd);
        osal_free(ctx);
    }
}

int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{
    int ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
    struct sockaddr_in  client_addr;
    int addrlen;

    if((NULL == bind_ctx) && (NULL == client_ctx) )
    {
        return ret;
    }

    sal_listen(((mbedtls_net_context *)bind_ctx)->fd,MBEDTLS_NET_LISTEN_BACKLOG);

    addrlen = sizeof(client_addr);
    ((mbedtls_net_context *)client_ctx)->fd = sal_accept(((mbedtls_net_context *)bind_ctx)->fd,\
                                                          (struct sockaddr *)&client_addr,&addrlen);
    if(-1 ==  ((mbedtls_net_context *)client_ctx)->fd)
    {
        ret = MBEDTLS_ERR_NET_ACCEPT_FAILED;
    }

    if( NULL != client_ip)
    {
        *ip_len = sizeof(client_addr.sin_addr)>buf_size?buf_size:sizeof(client_addr.sin_addr);

        memcpy(client_ip,&client_addr.sin_addr,*ip_len);
    }

    ret = 0;

    return ret;
}

