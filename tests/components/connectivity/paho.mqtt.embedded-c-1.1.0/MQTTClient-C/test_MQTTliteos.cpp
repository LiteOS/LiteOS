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

#include "test_MQTTliteos.h"
#include "ssl.h"
#include "ctr_drbg.h"
#include "net_sockets.h"
#include "sys/socket.h"
#include "unistd.h"

extern "C"
{   
    #include "MQTTliteos.h"
    
    static Timer t;
    extern int mbedtls_mqtt_connect( mbedtls_net_context *ctx, const char *host,
                         const char *port, int proto );
    static uint64_t stub_atiny_gettime_ms(void)
    {
        static int timer = 1;
        return timer++;
    }
    static int stub_mbedtls_ssl_read( mbedtls_ssl_context *ssl, unsigned char *buf, size_t len )
    {
        return 1;
    }
    static int stub_mbedtls_ssl_write( mbedtls_ssl_context *ssl, const unsigned char *buf, size_t len )
    {
        return 1;
    }
    static int stub_mbedtls_ctr_drbg_seed( mbedtls_ctr_drbg_context *ctx,
                       int (*f_entropy)(void *, unsigned char *, size_t),
                       void *p_entropy,
                       const unsigned char *custom,
                       size_t len )
    {
        ctx->p_entropy = p_entropy;
        return 0;
    }
    static int stub_mbedtls_mqtt_connect( mbedtls_net_context *ctx, const char *host,
                            const char *port, int proto )
    {
        return 0;
    }
                            
    static int stub_mbedtls_net_set_block( mbedtls_net_context *ctx )
                                {
                                    return 0;
                                }
    
    static int stub_mbedtls_ssl_handshake( mbedtls_ssl_context *ssl )
    {
        return 0;
    }
    static int stub_mbedtls_ssl_handshake_step( mbedtls_ssl_context *ssl )
    {
        return MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    }
    
    int stub_connect(int s, void *name, int namelen)
    {
        return 1;
    }
    int stub_recv(int s, void *mem, size_t len, int flags)
    {
        return 0;
    }
    int stub_recv_err(int s, void *mem, size_t len, int flags)
    {
        return -1;
    }
    int stub_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout)
    {
        return 1;
    }
    int stub_write(int s, const void *data, size_t size, int flags)
    {
        return sizeof(data);
    }

}

void TestMQTTLiteos::test_TimerInit()
{
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerInit(&t);
    TEST_ASSERT(t.end_time > 0);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerIsExpired()
{
    char result = -1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    result = TimerIsExpired(&t);
    TEST_ASSERT(result = TRUE);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerCountdownMS()
{
    unsigned int timeout = 1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerCountdownMS(&t, timeout);
    TEST_ASSERT(t.end_time > timeout);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerCountdown()
{
    unsigned int timeout = 1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    TimerCountdown(&t, timeout);
    TEST_ASSERT(t.end_time > timeout * 1000);
    cleanStub(&si);
}
void TestMQTTLiteos::test_TimerLeftMS()
{
    int result = -1;
    
    stubInfo si;
    setStub((void *)atiny_gettime_ms, (void *)stub_atiny_gettime_ms, &si);
    result = TimerLeftMS(&t);
    TEST_ASSERT(result > 0);
    cleanStub(&si);
}
void TestMQTTLiteos::test_NetworkInit()
{
    Network n;
    
    memset(&n, sizeof(Network), 0);
    NetworkInit(NULL);
    
    NetworkInit(&n);
    TEST_ASSERT(&n != NULL);
}
void TestMQTTLiteos::test_los_read()
{
    Network n;
    unsigned char buffer[256];
    int timeout_ms = 100;
    int ret = 0;
    mqtt_context_t ctx;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    memset(&n, sizeof(Network), 0);
    memset(&ctx, sizeof(mqtt_context_t), 0);
    memset(&ssl, sizeof(mbedtls_ssl_context), 0);
    memset(&conf, sizeof(mbedtls_ssl_config), 0);

    NetworkInit(&n);
    ret = n.mqttread(NULL, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);
    
    n.proto = MQTT_PROTO_NONE;
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);

    n.proto = MQTT_PROTO_TLS_PSK;
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);

    n.proto = MQTT_PROTO_NONE;
    ctx.fd = 3;
    n.ctx = &ctx;
    stubInfo si1;
    stubInfo si2;
    stubInfo si3;
    setStub((void *)select, (void *)stub_select, &si1);
    setStub((void *)recv, (void *)stub_recv_err, &si3);
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);
    cleanStub(&si3);
    setStub((void *)recv, (void *)stub_recv, &si2);
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == 0);

    timeout_ms = -100;
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == 0);
    cleanStub(&si2);
    cleanStub(&si1);

    n.proto = MQTT_PROTO_TLS_PSK;
    timeout_ms = 100;
    n.ctx = &ssl;
    ssl.conf = &conf;
    stubInfo si;
    setStub((void *)mbedtls_ssl_read, (void *)stub_mbedtls_ssl_read, &si);
    ret = n.mqttread(&n, buffer, sizeof(buffer), timeout_ms);
    cleanStub(&si);
    TEST_ASSERT(ret == 1);
    
    
}
void TestMQTTLiteos::test_los_write()
{
    Network n;
    unsigned char* buffer = (unsigned char *)"hgkllhfkghki";
    int timeout_ms = 100;
    int ret = 0;
    mqtt_context_t ctx;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    memset(&n, sizeof(Network), 0);
    memset(&ctx, sizeof(mqtt_context_t), 0);
    memset(&ssl, sizeof(mbedtls_ssl_context), 0);
    memset(&conf, sizeof(mbedtls_ssl_config), 0);
    NetworkInit(&n);
    ret = n.mqttwrite(NULL, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);
    ret = n.mqttwrite(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);
    
    n.proto = MQTT_PROTO_NONE;
    ret = n.mqttwrite(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);

    n.proto = MQTT_PROTO_TLS_PSK;
    ret = n.mqttwrite(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == -1);

    n.proto = MQTT_PROTO_NONE;
    ctx.fd = 3;
    n.ctx = &ctx;
    stubInfo si;
    setStub((void *)write, (void *)stub_write, &si);
    ret = n.mqttwrite(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == sizeof(buffer));
    cleanStub(&si);

    timeout_ms = -100;
    ret = n.mqttwrite(&n, buffer, sizeof(buffer), timeout_ms);
    TEST_ASSERT(ret == sizeof(buffer));

}
void TestMQTTLiteos::test_NetworkConnect()
{
    Network n;
    char* addr = "192.168.1.189";
    int port = 8000;
    int ret = 0;
    mqtt_context_t *ctx;
    
    memset(&n, sizeof(Network), 0);
    ctx = (mqtt_context_t *)malloc(sizeof(mqtt_context_t));
    NetworkInit(&n);
    ret = NetworkConnect(NULL, addr, port);
    TEST_ASSERT(ret == -1);

    stubInfo si1;
    setStub((void *)connect, (void *)stub_connect, &si1);
    n.proto = MQTT_PROTO_NONE;
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == 1);
    NetworkDisconnect(&n);
    
    ctx->fd = 2;
    n.ctx = ctx;
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == 1);
    NetworkDisconnect(&n);

    n.proto = MQTT_PROTO_TLS_PSK;
    n.psk.psk = (unsigned char *)"jkkdhglkdfgh";
    n.psk.psk_len = strlen((char *)n.psk.psk);
    n.psk.psk_id = (unsigned char *)"123455";
    n.psk.psk_id_len = strlen((char *)n.psk.psk_id);
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == -1);
    
    stubInfo si2;
    setStub((void *)mbedtls_ctr_drbg_seed, (void *)stub_mbedtls_ctr_drbg_seed, &si2);
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == -1);
    
    stubInfo si3;
    stubInfo si4;
    stubInfo si5;
    stubInfo si6;
    setStub((void *)mbedtls_mqtt_connect, (void *)stub_mbedtls_mqtt_connect, &si3);
    setStub((void *)mbedtls_net_set_block, (void *)stub_mbedtls_net_set_block, &si4);
    setStub((void *)mbedtls_ssl_handshake_step, (void *)stub_mbedtls_ssl_handshake_step, &si5);
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == -1);
    cleanStub(&si5);
    setStub((void *)mbedtls_ssl_handshake, (void *)stub_mbedtls_ssl_handshake, &si6);
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == 0);
    
    FILE *stream;
    stream = fopen("fprintf.out","w");
    ((mbedtls_ssl_context *)n.ctx)->conf->f_dbg(stream, 2, "/home/protocols/wsy/LiteOS/tests/components/connectivity/paho.mqtt.embedded-c-1.1./MQTTClient-C/src/liteOS/test_MQTTliteos.cpp", 288, "err"); 

    cleanStub(&si6);
    cleanStub(&si4);
    cleanStub(&si3);
    cleanStub(&si2);
    NetworkDisconnect(&n);

    n.proto = MQTT_PROTO_MAX;
    ret = NetworkConnect(&n, addr, port);
    TEST_ASSERT(ret == -1);
    cleanStub(&si1);

}
void TestMQTTLiteos::test_mbedtls_net_set_block()
{
    mbedtls_net_context ctx;
    int ret = 0;

    memset(&ctx, sizeof(mbedtls_net_context), 0);
    ctx.fd = 3;
    ret = mbedtls_net_set_block(&ctx);
    TEST_ASSERT(ret == 0);
}
void TestMQTTLiteos:: test_mbedtls_net_set_nonblock()
{
    mbedtls_net_context ctx;
    int ret = 0;

    memset(&ctx, sizeof(mbedtls_net_context), 0);
    ctx.fd = 3;
    ret = mbedtls_net_set_nonblock(&ctx);
    TEST_ASSERT(ret == 0);
}
void TestMQTTLiteos::test_NetworkDisconnect()
{
    Network n;
    int ret = 0;
    
    memset(&n, sizeof(Network), 0);
    NetworkDisconnect(NULL);

    n.proto = MQTT_PROTO_MAX;
    n.ctx = NULL;
    NetworkDisconnect(&n);

}

TestMQTTLiteos::TestMQTTLiteos()
{
    TEST_ADD(TestMQTTLiteos::test_TimerInit);
    TEST_ADD(TestMQTTLiteos::test_TimerIsExpired);
    TEST_ADD(TestMQTTLiteos::test_TimerCountdownMS);
    TEST_ADD(TestMQTTLiteos::test_TimerCountdown);
    TEST_ADD(TestMQTTLiteos::test_TimerLeftMS);
    TEST_ADD(TestMQTTLiteos::test_NetworkInit);
    TEST_ADD(TestMQTTLiteos::test_los_read);
    TEST_ADD(TestMQTTLiteos::test_los_write);
    TEST_ADD(TestMQTTLiteos::test_NetworkConnect);
    TEST_ADD(TestMQTTLiteos::test_mbedtls_net_set_block);
    TEST_ADD(TestMQTTLiteos::test_mbedtls_net_set_nonblock);
    TEST_ADD(TestMQTTLiteos::test_NetworkDisconnect);
}

void TestMQTTLiteos::setup()
{
    return;
}
void TestMQTTLiteos::tear_down()
{
    return ;
}

