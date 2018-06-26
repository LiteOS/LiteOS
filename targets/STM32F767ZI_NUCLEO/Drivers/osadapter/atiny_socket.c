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

#include "atiny_socket.h"
#include "atiny_adapter.h"

#ifdef USE_AT_FRAMEWORK
//#include "at_api_interface.h"
	#include "M8266_WIFI_SPI_IF.h"
#endif

#if defined(WITH_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

#elif defined(WITH_LWIP)
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/errno.h"
#endif

#if defined(WITH_LINUX) || defined(WITH_LWIP)

#define SOCKET_DEBUG

#if defined(SOCKET_DEBUG)
#define SOCKET_LOG(fmt, ...) \
    do \
    { \
        (void)atiny_printf("[SOCKET][%s:%d] " fmt "\r\n", \
        __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while(0)
#else
#define SOCKET_LOG(fmt, ...) ((void)0)
#endif


typedef struct
{
    int fd;
} atiny_net_context;

int errno;

void* atiny_net_connect(const char* host, const char* port, int proto)
{
    atiny_net_context* ctx;
#ifndef USE_AT_FRAMEWORK
    int flags;
    int ret;
    struct addrinfo hints;
    struct addrinfo* addr_list;
    struct addrinfo* cur;

    if (NULL == host || NULL == port ||
        (proto != ATINY_PROTO_UDP && proto != ATINY_PROTO_TCP))
    {
        SOCKET_LOG("ilegal incoming parameters");
        return NULL;
    }

    /* Do name resolution with both IPv6 and IPv4 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == ATINY_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == ATINY_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;

    SOCKET_LOG("try to do name resolution now...");

    if ((ret = getaddrinfo(host, port, &hints, &addr_list)) != 0)
    {
        SOCKET_LOG("getaddrinfo failed: 0x%x", ret);
        return NULL;
    }

    SOCKET_LOG("do name resolution succeed");

    ctx = atiny_malloc(sizeof(atiny_net_context));

    if (NULL == ctx)
    {
        SOCKET_LOG("malloc failed for socket context");
        freeaddrinfo(addr_list);
        return NULL;
    }

    ctx->fd = -1;

    /* Try the sockaddrs until a connection succeeds */
    for (cur = addr_list; cur != NULL; cur = cur->ai_next)
    {
        ctx->fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);

        if (ctx->fd < 0)
        {
            continue;
        }

        flags = fcntl(ctx->fd, F_GETFL, 0);

        if (flags < 0 || fcntl(ctx->fd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            close(ctx->fd);
            ctx->fd = -1;
            continue;
        }

        if (connect(ctx->fd, cur->ai_addr, cur->ai_addrlen) == 0)
        {
            break;
        }

        close(ctx->fd);
        ctx->fd = -1;
    }

    freeaddrinfo(addr_list);

    if (ctx->fd < 0)
    {
        SOCKET_LOG("unkown host(%s) or port(%s)", host, port);
        atiny_free(ctx);
        return NULL;
    }

    if (proto == ATINY_PROTO_UDP)
    {
        SOCKET_LOG("UDP create socket and bind to server(%s:%s) finished", host, port);
    }
    else /* proto == ATINY_PROTO_TCP */
    {
        SOCKET_LOG("TCP connect to server(%s:%s) succeed", host, port);
    }
#else
		
    ctx = atiny_malloc(sizeof(atiny_net_context));
    if (NULL == ctx)
    {
        SOCKET_LOG("malloc failed for socket context");
        return NULL;
    }    
		
		#ifdef USE_ESP8266_SPI
    ctx->fd = wifi_spi_if_connect(host, port, proto);
		#endif
		
    if (ctx->fd < 0)
    {
        SOCKET_LOG("unkown host(%s) or port(%s)", host, port);
        atiny_free(ctx);
        ctx = NULL;
    }
#endif  
    return ctx;
}

int atiny_net_recv(void* ctx, unsigned char* buf, size_t len)
{
    int ret;
    int fd = ((atiny_net_context*)ctx)->fd;
		
	#ifndef USE_AT_FRAMEWORK
    ret = recv(fd, buf, len, 0);

    if (ret < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        {
            SOCKET_LOG("no data available for now");
            return 0;
        }
        else
        {
            SOCKET_LOG("error accured when recv: 0x%x", errno);
            return -1;
        }
    }

    else if (ret == 0)
    {
        SOCKET_LOG("socket was closed by peer");
        return -1;
    }

    return ret;
	#else
		
		#ifdef USE_ESP8266_SPI
		return wifi_spi_if_recv(fd , buf , len);
		#endif
		
	#endif
}

int atiny_net_recv_timeout(void* ctx, unsigned char* buf, size_t len,
                           uint32_t timeout)
{
    int ret;
    struct timeval tv;
    fd_set read_fds;
    int fd = ((atiny_net_context*)ctx)->fd;

    if (fd < 0)
    {
        SOCKET_LOG("ilegal socket(%d)", fd);
        return -1;
    }
#ifndef USE_AT_FRAMEWORK

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret = select(fd + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);

    if (ret == 0)
    {
        SOCKET_LOG("recv timeout");
        return -2;
    }

    return atiny_net_recv(ctx, buf, len);
#else 
		
		#ifdef USE_ESP8266_SPI
    return wifi_spi_if_recv_timeout(fd, buf, len, timeout);
		#endif
    
#endif
}

int atiny_net_send(void* ctx, const unsigned char* buf, size_t len)
{
    int ret;
    int fd = ((atiny_net_context*)ctx)->fd;

    if (fd < 0)
    {
        SOCKET_LOG("ilegal socket(%d)", fd);
        return -1;
    }

    #ifndef USE_AT_FRAMEWORK
    ret = send(fd, buf, len, 0);
    #else
		
		#ifdef USE_ESP8266_SPI
    ret = wifi_spi_if_send(fd , buf , len);
		#endif
		
    #endif

    if (ret < 0)
    {
        /* no data available for now */
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        {
            return 0;
        }
        else
        {
            SOCKET_LOG("error accured when send: 0x%x", errno);
            return -1;
        }
    }

    return ret;
}

void atiny_net_close(void* ctx)
{
    int fd = ((atiny_net_context*)ctx)->fd;

    if (fd >= 0)
    {
        #ifndef USE_AT_FRAMEWORK
        close(fd);
        #else
				
				#ifdef USE_ESP8266_SPI
        wifi_spi_if_close(fd);
				#endif
			
        #endif
    }

    atiny_free(ctx);
}

#endif /* WITH_LINUX || WITH_LWIP */

