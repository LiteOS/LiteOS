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

/*
 *  Simple DTLS client demonstration program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "dtls_interface.h"
#include "atiny_adapter.h"
#include "mbedtls/net_sockets.h"

#define MBEDTLS_DEBUG

#ifdef MBEDTLS_DEBUG
#define MBEDTLS_LOG(fmt, ...) \
    do \
    { \
        (void)atiny_printf("[MBEDTLS][%s:%d] " fmt "\r\n", \
        __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define MBEDTLS_LOG(fmt, ...) ((void)0)
#endif


static void* atiny_calloc(size_t n, size_t size)
{
    return atiny_malloc(n * size);
}

mbedtls_ssl_context* dtls_ssl_new_with_psk(char* psk, unsigned psk_len, char* psk_identity)
{
    int ret;
    mbedtls_ssl_context* ssl;
    mbedtls_ssl_config* conf;
    mbedtls_entropy_context* entropy;
    mbedtls_ctr_drbg_context* ctr_drbg;

    const char* pers = "dtls_client";

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
        goto exit_fail;
    }

    mbedtls_ssl_init(ssl);
    mbedtls_ssl_config_init(conf);
    mbedtls_ctr_drbg_init(ctr_drbg);
    mbedtls_entropy_init(entropy);

    if ((ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
                                     (const unsigned char*) pers,
                                     strlen(pers))) != 0)
    {
        MBEDTLS_LOG("mbedtls_ctr_drbg_seed failed: -0x%x", -ret);
        goto exit_fail;
    }

    MBEDTLS_LOG("setting up the DTLS structure");

    if ((ret = mbedtls_ssl_config_defaults(conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        MBEDTLS_LOG("mbedtls_ssl_config_defaults failed: -0x%x", -ret);
        goto exit_fail;
    }

    mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_rng(conf, mbedtls_ctr_drbg_random, ctr_drbg);

#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)

    if ((ret = mbedtls_ssl_conf_psk(conf, (const unsigned char*)psk, psk_len,
                                    (const unsigned char*) psk_identity,
                                    strlen(psk_identity))) != 0)
    {
        MBEDTLS_LOG("mbedtls_ssl_conf_psk failed: -0x%x", -ret);
        goto exit_fail;
    }

#endif

    if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0)
    {
        MBEDTLS_LOG("mbedtls_ssl_setup failed: -0x%x", -ret);
        goto exit_fail;
    }

#if defined(MBEDTLS_X509_CRT_PARSE_C)

    if ((ret = mbedtls_ssl_set_hostname(ssl, SERVER_NAME)) != 0)
    {
        MBEDTLS_LOG("mbedtls_ssl_set_hostname failed: -0x%x", -ret);
        goto exit_fail;
    }

#endif
    MBEDTLS_LOG("set DTLS structure succeed");

    return ssl;

exit_fail:
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
    return NULL;
}

int dtls_shakehand(mbedtls_ssl_context* ssl, const char* host, const char* port)
{
    int ret;
    int j = 0;
    mbedtls_net_context* server_fd = NULL;
    mbedtls_timing_delay_context* timer = NULL;

    timer = mbedtls_calloc(1, sizeof(mbedtls_timing_delay_context));


    if (NULL == timer)
    {
        ret = MBEDTLS_ERR_SSL_ALLOC_FAILED;
        goto exit_fail;
    }

    MBEDTLS_LOG("connecting to udp/%s:%s", host, port);

    if (( server_fd = mbedtls_net_connect(host,
                                          port, MBEDTLS_NET_PROTO_UDP )) == NULL)
    {
        MBEDTLS_LOG("mbedtls_net_connect failed");
        ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
        goto exit_fail;
    }

    mbedtls_ssl_set_bio(ssl, server_fd,
                        mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

    mbedtls_ssl_set_timer_cb(ssl, timer, mbedtls_timing_set_delay,
                             mbedtls_timing_get_delay);

    MBEDTLS_LOG("Performing the SSL/TLS handshake");

    do
    {
        ret = mbedtls_ssl_handshake(ssl);       
        //LOS_TaskDelay(1);
        j++;

    }
    while ((ret == MBEDTLS_ERR_SSL_WANT_READ ||
           ret == MBEDTLS_ERR_SSL_WANT_WRITE) && j < 10 );

    if (ret != 0)
    {
        MBEDTLS_LOG("mbedtls_ssl_handshake failed: -0x%x", -ret);
        goto exit_fail;
    }

    MBEDTLS_LOG("Handshake succeed");
    return 0;

exit_fail:

    if (server_fd)
    {
        mbedtls_net_free(server_fd);
        ssl->p_bio = NULL;
    }

    if (timer)
    {
        mbedtls_free(timer);
        ssl->p_timer = NULL;
    }

    return ret;

}
void dtls_ssl_destroy(mbedtls_ssl_context* ssl)
{
    mbedtls_ssl_config*           conf = NULL;
    mbedtls_ctr_drbg_context*     ctr_drbg = NULL;
    mbedtls_entropy_context*      entropy = NULL;
    mbedtls_net_context*          server_fd = NULL;
    mbedtls_timing_delay_context* timer = NULL;

    if (ssl == NULL)
    {
        return;
    }

    conf       = ssl->conf;
    server_fd  = (mbedtls_net_context*)ssl->p_bio;
    timer      = (mbedtls_timing_delay_context*)ssl->p_timer;

    if (conf)
    {
        ctr_drbg   = conf->p_rng;

        if (ctr_drbg)
        {
            entropy =  ctr_drbg->p_entropy;
        }
    }

    if (server_fd)
    {
        mbedtls_net_free(server_fd);
    }

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

    if (timer)
    {
        mbedtls_free(timer);
    }

    mbedtls_ssl_free(ssl);
    mbedtls_free(ssl);
}

int dtls_write(mbedtls_ssl_context* ssl, const unsigned char* buf, size_t len)
{
    int ret = mbedtls_ssl_write(ssl, (unsigned char*) buf, len);

    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE)
    {
        return 0;
    }
    else if (ret < 0)
    {
        return -1;
    }

    return ret;
}

int dtls_read(mbedtls_ssl_context* ssl, unsigned char* buf, size_t len, uint32_t timeout)
{
    int ret;

    mbedtls_ssl_conf_read_timeout(ssl->conf, timeout);

    ret = mbedtls_ssl_read(ssl, buf, len);

    if (ret == MBEDTLS_ERR_SSL_WANT_READ)
    {
        return 0;
    }
    else if (ret == MBEDTLS_ERR_SSL_TIMEOUT)
    {
        return -2;
    }
    else if (ret < 0)
    {
        return -1;
    }

    return ret;
}

