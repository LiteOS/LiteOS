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

#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"
#include "dtls_interface.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/errno.h"

#include "MQTTliteos.h"

//#define get_time_ms() LOS_Tick2MS(LOS_TickCountGet())
#define get_time_ms atiny_gettime_ms

void TimerInit(Timer* timer)
{
    timer->end_time = get_time_ms();
}

char TimerIsExpired(Timer* timer)
{
    unsigned long long now = get_time_ms();
    return now >= timer->end_time;
}

void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout;
}

void TimerCountdown(Timer* timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout * 1000;
}

int TimerLeftMS(Timer* timer)
{
    UINT64 now = get_time_ms();
    return timer->end_time <= now ? 0 : timer->end_time - now;
}

static int los_mqtt_read(void *ctx, unsigned char* buffer, int len, int timeout_ms)
{
    fd_set read_fds;
    int fd, ret, rc;
    int bytes;
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};;

    if(NULL == ctx || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }
    fd = ((mqtt_context_t *)ctx)->fd;
    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
    {
        interval.tv_sec = 0;
        interval.tv_usec = 100;
    }

    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    ret = select(fd + 1, &read_fds, NULL, NULL, &interval);

    if(ret > 0 && FD_ISSET(fd, &read_fds))
    {
        bytes = 0;
        while (bytes < len)
        {
            rc = recv(fd, &buffer[bytes], (size_t)(len - bytes), 0);
            if (rc == -1)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                  bytes = -1;
                break;
            }
            else if (rc == 0)
            {
                bytes = 0;
                break;
            }
            else
                bytes += rc;
        }
        return bytes;
    }
    else
    {
        return ret;
    }
}

static int los_mqtt_tls_read(mbedtls_ssl_context* ssl, unsigned char* buffer, int len, int timeout_ms)
{
    int ret;

    if(NULL == ssl || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    mbedtls_ssl_conf_read_timeout(ssl->conf, timeout_ms);

    ret = mbedtls_ssl_read(ssl, buffer, len);
    if(ret == MBEDTLS_ERR_SSL_WANT_READ
        || ret == MBEDTLS_ERR_SSL_WANT_WRITE
        || ret == MBEDTLS_ERR_SSL_TIMEOUT)
        ret = 0;

    return ret;
}

/*****************************************************************************
 函 数 名  : los_read
 功能描述  : 注册在mqttread的回调函数，依据是否基于TLS调用各自的read函数进行
             处理
 输入参数  : Network* n             
             unsigned char* buffer  
             int len                
             int timeout_ms         
 输出参数  : 无
 返 回 值  : static
 
 修改历史      :
  1.日    期   : 2018年4月20日
    作    者   : l00438842
    修改内容   : 新生成函数

*****************************************************************************/
static int los_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    int ret = -1;

    if(NULL == n || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    switch(n->proto)
    {
        case MQTT_PROTO_NONE :
            ret = los_mqtt_read(n->ctx, buffer, len, timeout_ms);
            break;
        case MQTT_PROTO_TLS_PSK:
            ret = los_mqtt_tls_read(n->ctx, buffer, len, timeout_ms);
            break;
        default :
            ATINY_LOG(LOG_WARNING, "unknow proto : %d", n->proto);
            break;
    }

    return ret;
}

static int los_mqtt_write(void *ctx, unsigned char* buffer, int len, int timeout_ms)
{
    int fd, rc;
    struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};;

    if(NULL == ctx || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }
    fd = ((mqtt_context_t *)ctx)->fd;

    if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
    {
        interval.tv_sec = 0;
        interval.tv_usec = 100;
    }

    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&interval, sizeof(struct timeval));
    rc = write(fd, buffer, len);
    return rc;
}

/*****************************************************************************
 函 数 名  : los_write
 功能描述  : 注册在mqttwrite的回调函数，依据是否基于SSL/TLS调用各自的write函
             数进行处理
 输入参数  : Network* n             
             unsigned char* buffer  
             int len                
             int timeout_ms         
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月20日
    作    者   : l00438842
    修改内容   : 新生成函数

*****************************************************************************/
int los_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    int ret = -1;

    if(NULL == n || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    switch(n->proto)
    {
        case MQTT_PROTO_NONE :
            ret = los_mqtt_write(n->ctx, buffer, len, timeout_ms);
            break;
        case MQTT_PROTO_TLS_PSK:
            ret = dtls_write(n->ctx, buffer, len);
            break;
        default :
            ATINY_LOG(LOG_WARNING, "unknow proto : %d", n->proto);
            break;
    }

    return ret;
}

void NetworkInit(Network* n)
{
    if(NULL == n)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return;
    }
    memset(n, 0x0, sizeof(Network));
    n->proto = MQTT_PROTO_MAX;
    n->mqttread = los_read;
    n->mqttwrite = los_write;

    return;
}

static int los_mqtt_connect(Network* n, char* addr, int port)
{
    int type = SOCK_STREAM;
    struct sockaddr_in address;
    int rc = -1;
    sa_family_t family = AF_INET;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};
    mqtt_context_t *ctx = NULL;
    struct addrinfo* res;

    if(NULL == n || NULL == addr)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    if ((rc = getaddrinfo(addr, NULL, &hints, &result)) == 0)
    {
        res = result;

        /* prefer ip4 addresses */
        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

        if (result->ai_family == AF_INET)
        {
            address.sin_port = htons(port);
            address.sin_family = family = AF_INET;
            address.sin_addr = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
        }
        else
            rc = -1;

        freeaddrinfo(result);
    }

    if (rc == 0)
    {
        ctx = (mqtt_context_t *)atiny_malloc(sizeof(mqtt_context_t));
        if (NULL == ctx)
        {
            rc = -1;
        }
        else
        {
            memset(ctx, 0x0, sizeof(mqtt_context_t));
            ctx->fd = socket(family, type, 0);
            if (ctx->fd != -1)
            {
                rc = connect(ctx->fd, (struct sockaddr*)&address, sizeof(address));
                if(0 != rc)
                {
                    close(ctx->fd);
                    ctx->fd = -1;
                    atiny_free(ctx);
                    ctx = NULL;
                }
                else
                {
                    ATINY_LOG(LOG_DEBUG, "connect success.");
                    n->ctx = (void *)ctx;
                }
            }
            else
            {
                rc = -1;
                atiny_free(ctx);
                ctx = NULL;
            }
        }
    }
    return rc;
}

static void* atiny_calloc(size_t n, size_t size)
{
    return atiny_malloc(n * size);
}

#if defined(_MSC_VER)
#define MSVC_INT_CAST   (int)
#else
#define MSVC_INT_CAST
#endif

int mbedtls_mqtt_connect( mbedtls_net_context *ctx, const char *host,
                         const char *port, int proto )
{
    int ret;
    struct addrinfo hints, *addr_list, *cur;

    /* Do name resolution with both IPv6 and IPv4 */
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;

    if( getaddrinfo( host, port, &hints, &addr_list ) != 0 )
        return( MBEDTLS_ERR_NET_UNKNOWN_HOST );

    /* Try the sockaddrs until a connection succeeds */
    ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;
    for( cur = addr_list; cur != NULL; cur = cur->ai_next )
    {
        ctx->fd = (int) socket( cur->ai_family, cur->ai_socktype,
                            cur->ai_protocol );
        if( ctx->fd < 0 )
        {
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        if( connect( ctx->fd, cur->ai_addr, MSVC_INT_CAST cur->ai_addrlen ) == 0 )
        {
            ret = 0;
            break;
        }

        close( ctx->fd );
        ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
    }

    freeaddrinfo( addr_list );

    return( ret );
}

int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 0;
    return( ioctlsocket( ctx->fd, FIONBIO, &n ) );
#else
    return( fcntl( ctx->fd, F_SETFL, fcntl( ctx->fd, F_GETFL, 0) & ~O_NONBLOCK ) );
#endif
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
#if ( defined(_WIN32) || defined(_WIN32_WCE) ) && !defined(EFIX64) && \
    !defined(EFI32)
    u_long n = 1;
    return( ioctlsocket( ctx->fd, FIONBIO, &n ) );
#else
    return( fcntl( ctx->fd, F_SETFL, fcntl( ctx->fd, F_GETFL, 0) | O_NONBLOCK ) );
#endif
}

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    const char *p, *basename;

    /* Extract basename from file */
    for( p = basename = file; *p != '\0'; p++ )
        if( *p == '/' || *p == '\\' )
            basename = p + 1;

    mbedtls_fprintf( (FILE *) ctx, "%s:%04d: |%d| %s", basename, line, level, str );
    fflush(  (FILE *) ctx  );
}

static int los_mqtt_tls_connect(Network* n, char* addr, int port)
{
    int ret;
    mbedtls_net_context *server_fd;
    mbedtls_ssl_context* ssl;
    mbedtls_ssl_config* conf;
    mbedtls_entropy_context* entropy;
    mbedtls_ctr_drbg_context* ctr_drbg;
    char port_str[16] = {0};
    const char* pers = "myhint";

    if(NULL == n || NULL == addr)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    (void)mbedtls_platform_set_calloc_free(atiny_calloc, atiny_free);
    (void)mbedtls_platform_set_snprintf(atiny_snprintf);
    (void)mbedtls_platform_set_printf(atiny_printf);

    ssl       = mbedtls_calloc(1, sizeof(mbedtls_ssl_context));
    conf      = mbedtls_calloc(1, sizeof(mbedtls_ssl_config));
    entropy   = mbedtls_calloc(1, sizeof(mbedtls_entropy_context));
    ctr_drbg  = mbedtls_calloc(1, sizeof(mbedtls_ctr_drbg_context));
    server_fd = mbedtls_calloc(1, sizeof(mbedtls_net_context));

    if (NULL == ssl || NULL == conf || entropy == NULL ||
        NULL == ctr_drbg || NULL == server_fd)
    {
        goto exit;
    }

    mbedtls_ssl_init(ssl);
    mbedtls_ssl_config_init(conf);
    mbedtls_ctr_drbg_init(ctr_drbg);
    mbedtls_entropy_init(entropy);
    server_fd->fd = -1;

    if( ( ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func,entropy,
                               (const unsigned char *) pers,
                               strlen( pers ) ) ) != 0 )
    {
        goto exit;
    }

    (void)atiny_snprintf(port_str, sizeof(port_str)-1, "%d", port);
    if( ( ret = mbedtls_mqtt_connect(server_fd, addr, port_str, MBEDTLS_NET_PROTO_TCP) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, "mbedtls_net_connect failed.");
        goto exit;
    }
    ATINY_LOG(LOG_DEBUG, "mbedtls_net_connect success");
    n->ctx = (void *)ssl;

    ret = mbedtls_net_set_block( server_fd );
    if( ret != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! net_set_(non)block() returned -0x%x", -ret );
        goto exit;
    }
    if( ( ret = mbedtls_ssl_config_defaults(conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_config_defaults returned -0x%x", -ret );
        goto exit;
    }

    mbedtls_ssl_conf_rng( conf, mbedtls_ctr_drbg_random, ctr_drbg );
    mbedtls_ssl_conf_dbg( conf, my_debug, stdout );

    mbedtls_ssl_conf_read_timeout( conf, 1 );

    if( ( ret = mbedtls_ssl_conf_psk( conf, (const unsigned char *)(n->psk.psk), n->psk.psk_len,
                             (const unsigned char *)(n->psk.psk_id),
                             n->psk.psk_id_len ) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_conf_psk returned %d", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ssl_setup( ssl, conf ) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_setup returned -0x%x", -ret );
        goto exit;
    }

    mbedtls_ssl_set_bio( ssl, server_fd, mbedtls_net_send, mbedtls_net_recv,
                             mbedtls_net_recv_timeout);

    while( ( ret = mbedtls_ssl_handshake( ssl ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE 
            && ret != MBEDTLS_ERR_SSL_TIMEOUT)
        {
            ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_handshake returned -0x%x", -ret );
            goto exit;
        }
    }

    if( ( ret = mbedtls_ssl_get_record_expansion( ssl ) ) >= 0 )
        ATINY_LOG(LOG_DEBUG, "    [ Record expansion is %d ]", ret );
    else
        ATINY_LOG(LOG_DEBUG, "    [ Record expansion is unknown (compression) ]" );
    ATINY_LOG(LOG_DEBUG, "success");
    return 0;
exit:
    if (conf)
    {
        mbedtls_ssl_config_free(conf);
        mbedtls_free(conf);
    }

    if (ctr_drbg)
    {
        mbedtls_ctr_drbg_free(ctr_drbg);
        mbedtls_free(ctr_drbg);
    }

    if (entropy)
    {
        mbedtls_entropy_free(entropy);
        mbedtls_free(entropy);
    }

    if (ssl)
    {
        mbedtls_ssl_free(ssl);
        mbedtls_free(ssl);
    }
    if(server_fd)
    {
        mbedtls_free(server_fd);
    }
    n->ctx = (void *)NULL;
    return -1;
}

int NetworkConnect(Network* n, char* addr, int port)
{
    int ret = -1;

    if(NULL == n || NULL == addr)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return -1;
    }

    switch(n->proto)
    {
        case MQTT_PROTO_NONE :
            ret = los_mqtt_connect(n, addr, port);
            break;
        case MQTT_PROTO_TLS_PSK:
            ret = los_mqtt_tls_connect(n, addr, port);
            break;
        default :
            ATINY_LOG(LOG_WARNING, "unknow proto : %d\n", n->proto);
            break;
    }

    return ret;
}

static void los_mqtt_disconnect(void* ctx)
{
    int fd;

    if(NULL == ctx)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return;
    }
    fd = ((mqtt_context_t *)ctx)->fd;
    if(fd > 0)
    {
        close(fd);
        ((mqtt_context_t *)ctx)->fd = -1;
    }
    atiny_free(ctx);

    return;
}

void NetworkDisconnect(Network* n)
{
    if(NULL == n)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return;
    }

    switch(n->proto)
    {
        case MQTT_PROTO_NONE :
            los_mqtt_disconnect(n->ctx);
            break;
        case MQTT_PROTO_TLS_PSK:
            dtls_ssl_destroy(n->ctx);
            break;
        default :
            ATINY_LOG(LOG_WARNING, "unknow proto : %d\n", n->proto);
            break;
    }

    n->ctx = NULL;

    return;
}

