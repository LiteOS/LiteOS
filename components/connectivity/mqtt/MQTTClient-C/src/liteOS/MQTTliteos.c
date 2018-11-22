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
//#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/errno.h"
#endif

#include "MQTTliteos.h"

#define get_time_ms atiny_gettime_ms

void TimerInit(Timer *timer)
{
    timer->end_time = get_time_ms();
}

char TimerIsExpired(Timer *timer)
{
    unsigned long long now = get_time_ms();
    return now >= timer->end_time;
}

void TimerCountdownMS(Timer *timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout;
}

void TimerCountdown(Timer *timer, unsigned int timeout)
{
    unsigned long long now = get_time_ms();
    timer->end_time = now + timeout * 1000;
}

int TimerLeftMS(Timer *timer)
{
    UINT64 now = get_time_ms();
    return timer->end_time <= now ? 0 : timer->end_time - now;
}

static int los_mqtt_read(void *ctx, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = atiny_net_recv_timeout(ctx, buffer, len, timeout_ms);
    /* 0 is timeout for mqtt for normal select */
    if (ret == ATINY_NET_TIMEOUT)
    {
        ret = 0;
    }
    return ret;
}

static int los_mqtt_tls_read(mbedtls_ssl_context *ssl, unsigned char *buffer, int len, int timeout_ms)
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
static int los_read(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;
    mqtt_security_info_s *info;

    if(NULL == n || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    info = n->get_security_info();

    switch(info->security_type)
    {
    case MQTT_SECURITY_TYPE_NONE :
        ret = los_mqtt_read(n->ctx, buffer, len, timeout_ms);
        break;
    case MQTT_SECURITY_TYPE_PSK:
        ret = los_mqtt_tls_read(n->ctx, buffer, len, timeout_ms);
        break;
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d", info->security_type);
        break;
    }

    return ret;
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
static int los_write(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    int ret = -1;
    mqtt_security_info_s *info;

    if(NULL == n || NULL == buffer)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return -1;
    }

    info = n->get_security_info();

    switch(info->security_type)
    {
    case MQTT_SECURITY_TYPE_NONE :
        ret = atiny_net_send_timeout(n->ctx, buffer, len, timeout_ms);
        break;
    case MQTT_SECURITY_TYPE_PSK:
        ret = dtls_write(n->ctx, buffer, len);
        break;
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d", info->security_type);
        break;
    }

    return ret;
}

void NetworkInit(Network *n, mqtt_security_info_s *(*get_security_info)(void))
{
    if((NULL == n) ||
       (NULL == get_security_info))
    {
        ATINY_LOG(LOG_FATAL, "invalid params.");
        return;
    }
    memset(n, 0x0, sizeof(Network));
    n->mqttread = los_read;
    n->mqttwrite = los_write;
    n->get_security_info = get_security_info;

    return;
}

static int los_mqtt_connect(Network *n, char *addr, int port)
{
    char port_str[16];

    (void)snprintf(port_str, sizeof(port_str), "%u", port);
    port_str[sizeof(port_str) - 1] = '\0';
    n->ctx = atiny_net_connect(addr, port_str, ATINY_PROTO_TCP);
    if (n->ctx == NULL)
    {
        ATINY_LOG(LOG_FATAL, "atiny_net_connect fail");
        return ATINY_NET_ERR;
    }

    return ATINY_OK;
}

static void *atiny_calloc(size_t n, size_t size)
{
    void *p = atiny_malloc(n * size);
    if(p)
    {
        memset(p, 0, n * size);
    }

    return p;
}

#if defined(_MSC_VER)
#define MSVC_INT_CAST   (int)
#else
#define MSVC_INT_CAST
#endif
#if 0

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
#endif

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

static int los_mqtt_tls_connect(Network *n, char *addr, int port)
{
    int ret;
    void *ctx = NULL;
    mbedtls_ssl_context *ssl;
    mbedtls_ssl_config *conf;
    mbedtls_entropy_context *entropy;
    mbedtls_ctr_drbg_context *ctr_drbg;
    char port_str[16] = {0};
    const char *pers = "myhint";
    mqtt_security_info_s *info;

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

    if (NULL == ssl || NULL == conf || entropy == NULL ||
            NULL == ctr_drbg)
    {
        goto exit;
    }

    mbedtls_ssl_init(ssl);
    mbedtls_ssl_config_init(conf);
    mbedtls_ctr_drbg_init(ctr_drbg);
    mbedtls_entropy_init(entropy);

    if( ( ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
                                      (const unsigned char *) pers,
                                      strlen( pers ) ) ) != 0 )
    {
        goto exit;
    }

    (void)atiny_snprintf(port_str, sizeof(port_str) - 1, "%d", port);
    ctx = atiny_net_connect(addr, port_str, ATINY_PROTO_TCP);
    if( ctx == NULL )
    {
        ATINY_LOG(LOG_ERR, "atiny_net_connect failed.");
        goto exit;
    }
    ATINY_LOG(LOG_DEBUG, "atiny_net_connect success");
    n->ctx = (void *)ssl;

    //TODO: use select to timeout to send, the same in atiny_socket
/*
    ret = mbedtls_net_set_block( ctx );
    if( ret != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! net_set_(non)block() returned -0x%x", -ret );
        goto exit;
    }*/
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

    info = n->get_security_info();

    if( ( ret = mbedtls_ssl_conf_psk( conf, (const unsigned char *)(info->u.psk.psk), info->u.psk.psk_len,
                                      (const unsigned char *)(info->u.psk.psk_id),
                                      info->u.psk.psk_id_len ) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_conf_psk returned %d", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ssl_setup( ssl, conf ) ) != 0 )
    {
        ATINY_LOG(LOG_ERR, " failed\n  ! mbedtls_ssl_setup returned -0x%x", -ret );
        goto exit;
    }

    mbedtls_ssl_set_bio( ssl, ctx, mbedtls_net_send, mbedtls_net_recv,
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
    if(ctx)
    {
        atiny_net_close(ctx);
    }
    n->ctx = (void *)NULL;
    return -1;
}

int NetworkConnect(Network *n, char *addr, int port)
{
    int ret = -1;
    mqtt_security_info_s *info;

    if(NULL == n || NULL == addr)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return -1;
    }

    info = n->get_security_info();
    switch(info->security_type)
    {
    case MQTT_SECURITY_TYPE_NONE :
        ret = los_mqtt_connect(n, addr, port);
        break;
    case MQTT_SECURITY_TYPE_PSK:
        ret = los_mqtt_tls_connect(n, addr, port);
        break;
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d\n", info->security_type);
        break;
    }

    return ret;
}

static void los_mqtt_disconnect(void *ctx)
{
    if(NULL == ctx)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return;
    }
    atiny_net_close(ctx);

    return;
}

void NetworkDisconnect(Network *n)
{
    mqtt_security_info_s *info;
    if(NULL == n)
    {
        ATINY_LOG(LOG_FATAL, "invalid params.\n");
        return;
    }

    info = n->get_security_info();
    switch(info->security_type)
    {
    case MQTT_SECURITY_TYPE_NONE :
        los_mqtt_disconnect(n->ctx);
        break;
    case MQTT_SECURITY_TYPE_PSK:
        dtls_ssl_destroy(n->ctx);
        break;
    default :
        ATINY_LOG(LOG_WARNING, "unknow proto : %d\n", info->security_type);
        break;
    }

    n->ctx = NULL;

    return;
}

