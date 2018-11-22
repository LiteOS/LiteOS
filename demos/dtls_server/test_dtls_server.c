#include "test_dtls_server.h"
#include "dtls_interface.h"
#include "osdepends/atiny_osdep.h"
#include "sal/atiny_socket.h"

#if defined(WITH_DTLS) && defined(SUPPORT_DTLS_SRV)

#define SERVER_PSK  "11223344556677881122334455667788"
#define SERVER_IDENTITY "testserver1"

#ifdef DTLS_SERVER_DEBUG
#define LOG(fmt, arg...) do {printf("%s:%d --- "fmt"\r\n", __func__, __LINE__, ##arg);}while(0)
#else
#define LOG(fmt, arg...)
#endif



void dtls_server_task(void)
{
	mbedtls_net_context * bind_ctx = atiny_malloc(sizeof(mbedtls_net_context));
    mbedtls_net_context *  cli_ctx = atiny_malloc(sizeof(mbedtls_net_context));
    int ret ;

    mbedtls_ssl_context *ssl = NULL;

    bind_ctx = (mbedtls_net_context*)atiny_net_bind(NULL, "5685", 1);
    if (bind_ctx == NULL)
    {
		LOG("bind failed!");
        return ;
    }

    ssl = dtls_ssl_new_with_psk(SERVER_PSK, strlen(SERVER_PSK), SERVER_IDENTITY, MBEDTLS_SSL_IS_SERVER);
    do {
        unsigned char buf[64] = {0};

        ret = dtls_accept(bind_ctx, cli_ctx, NULL, 0, 0);
		LOG("dtls_accept return ret = %d, bindfd = %d, cli_fd = %d", ret, bind_ctx->fd, cli_ctx->fd);
        mbedtls_ssl_set_bio(ssl, cli_ctx, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

        //new psk and handshake should been done for each client, now only for frist connection.
        do{
            ret = mbedtls_ssl_handshake(ssl);
        }
        while ((ret == MBEDTLS_ERR_SSL_WANT_READ ||
                ret == MBEDTLS_ERR_SSL_WANT_WRITE));
		LOG("mbedtls_ssl_handshake return ret = %d", ret);

        ret = dtls_read(ssl, buf, sizeof(buf), 5000);
		LOG("dtls_read return ret = %d", ret);

        printf("%s:%d --- dtls read buf = %s\r\n", __func__, __LINE__, buf);

        unsigned char hello[] = "hello this\r\n";
        ret = dtls_write(ssl, hello, sizeof(hello));

        LOS_TaskDelay(2000);
    }while(0);

    if (ssl)dtls_ssl_destroy(ssl);
    mbedtls_net_free(bind_ctx);

}


#endif