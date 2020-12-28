# dtls_server使用教程

## 使用场景
dtls_server用于演示`mbedtls`加密方式中的`psk`加密。下面以开发板Cloud_STM32F429IGTx_FIRE为例，讲解如何在Cloud_STM32F429IGTx_FIRE开发板上使能dtls_server，实现与客户端之间的psk加密通信。

**注意：** mbedtls还提供一种基于证书的加密方式，完整的服务端代码请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/components/security/mbedtls/mbedtls-2.16.8/programs/ssl/dtls_server.c" target="_blank">dtls_server.c</a>，客户端代码请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/components/security/mbedtls/mbedtls-2.16.8/programs/ssl/dtls_client.c" target="_blank">dtls_client.c</a>。


## 相关配置

### 1. 设置开发板Ipv4地址、子网掩码和网关地址
通过<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c" target="_blank">sys_init.c</a>文件中的`void net_init(void)`函数设置开发板的Ipv4地址、子网掩码及网关地址。

### 2. 设置dtls_server的端口号
通过<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/dtls_server/test_dtls_server.c" target="_blank">test_dtls_server.c</a>文件中的宏`SERVER_PORT`设置dtls_server的端口号，端口范围为1-65535，需要设置为一个空闲的端口。

### 2. 设置dtls_server的psk和psk_id
通过<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/dtls_server/test_dtls_server.c" target="_blank">test_dtls_server.c</a>文件中的宏`SERVER_PSK`、`SERVER_IDENTITY`设置服务端的psk和psk_id。


## 开启dtls_server

dtls_server需要lwip、sal、mbedtls组件的支持，可通过make menuconfig使能以上组件和该Demo。
```
Components --> Network --> Enable Network -> Enable Lwip
Components --> Network --> Enable Network -> Enable Sal
Components --> Security --> Enable Security -> Enable Mbed TLS Component
Demos --> Dtls Server Demo --> Enable Dtls Server Demo
```
完成以上操作后编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，dtls_server即启动。


## 编写客户端

开发板上运行的是服务端程序，用户需要自行编写客户端程序与服务端进行交互。客户端需要用到mbedtls提供的接口，可拷贝<a href="https://gitee.com/LiteOS/LiteOS/blob/master/components/security/mbedtls/mbedtls-2.16.8" target="_blank">mbedtls-2.16.8软件包</a>到linux服务器上（windows上搭建环境较繁琐），客户端示列代码如下（该代码参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/components/security/mbedtls/mbedtls-2.16.8/programs/ssl/dtls_client.c" target="_blank">dtls_client.c</a>编写）：
````c
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include <string.h>

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/timing.h"

#define SERVER_PORT "5658"
#define SERVER_ADDR "192.168.3.200"
#define MESSAGE "Hi Server"

#define CLIENT_PSK  "11223344556677881122334455667788"
#define CLIENT_IDENTITY "testserver1"

int main(int argc, char *argv[])
{
    int ret, len;
    unsigned char buf[512];
    const char *pers = "dtls_client";

    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_timing_delay_context timer;

    ((void) argc);
    ((void) argv);

    mbedtls_net_init(&server_fd);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_printf("\n . Seeding the random number generator...");
    fflush(stdout);

    mbedtls_entropy_init(&entropy);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
    if (ret  != 0) {
        mbedtls_printf("failed\n ! mbedtls_ctr_drbg_seed returned %d\n", ret);
        goto exit;
    }

    mbedtls_printf("ok\n . Setting up the SSL structure...");
    fflush(stdout);
    ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0 ) {
        mbedtls_printf("failed\n ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    mbedtls_printf("ok\n . Setting psk...");
    fflush(stdout);
    ret = mbedtls_ssl_conf_psk(&conf,
                            (unsigned char *)CLIENT_PSK,
                            strlen(CLIENT_PSK),
                            (unsigned char *)CLIENT_IDENTITY,
                            strlen((const char *)CLIENT_IDENTITY));
    if (ret != 0) {
        mbedtls_printf("failed\n !  mbedtls_ssl_conf_psk returned -0x%x\n\n", -ret);
        goto exit;
    }

    ret = mbedtls_ssl_setup(&ssl, &conf);
    if(ret != 0) {
        mbedtls_printf("failed\n ! mbedtls_ssl_setup returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_printf("ok\n . Connecting to udp/%s/%s...", SERVER_ADDR, SERVER_PORT);
    ret = mbedtls_net_connect(&server_fd, SERVER_ADDR, SERVER_PORT, MBEDTLS_NET_PROTO_UDP);
    if (ret != 0) {
        mbedtls_printf("failed\n ! mbedtls_net_connect returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);
    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);
    mbedtls_printf("ok\n");

    mbedtls_printf(" . Performing the SSL/TLS handshake...");
    fflush(stdout);
    do {
        ret = mbedtls_ssl_handshake( &ssl );
    } while ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE));
    if (ret != 0) {
        mbedtls_printf(" failed\n ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
        goto exit;
    }
    mbedtls_printf("ok\n");

    mbedtls_printf(" > Write to server:");
    fflush(stdout);
    do {
        ret = mbedtls_ssl_write(&ssl, (unsigned char *)MESSAGE, strlen(MESSAGE));
    } while ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE));
    if (ret < 0) {
        mbedtls_printf("failed\n ! mbedtls_ssl_write returned %d\n\n", ret);
        goto exit;
    }
    len = ret;
    mbedtls_printf(" %d bytes written\n %s\n\n", len, MESSAGE);

    mbedtls_printf(" > Read from server:");
    fflush(stdout);
    len = sizeof(buf) - 1;
    memset(buf, 0, sizeof(buf));
    do {
        ret = mbedtls_ssl_read(&ssl, buf, len);
    } while ((ret == MBEDTLS_ERR_SSL_WANT_READ) || (ret == MBEDTLS_ERR_SSL_WANT_WRITE));
    if (ret <= 0) {
        mbedtls_printf("failed\n ! mbedtls_ssl_read returned -0x%x\n\n", -ret);
        goto exit;
    }
    len = ret;
    mbedtls_printf(" %d bytes read\n %s\n", len, buf);

    mbedtls_printf(" . Closing the connection ...");
    fflush(stdout);
    do {
        ret = mbedtls_ssl_close_notify(&ssl);
    } while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);
    ret = 0;
    mbedtls_printf("done\n");

exit:
    mbedtls_net_free(&server_fd);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    return ret;
}
````
**参数说明：**
```
SERVER_PORT        dtls_server中绑定的端口
SERVER_ADDR        开发板的Ipv4地址
CLIENT_PSK         psk，与dtls_server中设置的psk保持一致
CLIENT_IDENTITY    psk_id，与dtls_server中设置的psk保持一致
```

用以上示列代码替换<a href="https://gitee.com/LiteOS/LiteOS/blob/master/components/security/mbedtls/mbedtls-2.16.8/programs/ssl/dtls_client.c" target="_blank">dtls_client.c</a>中的内容，根据自己的实际情况修改`参数说明`的四个参数，其余部分无需修改。然后在`mbedtls-2.16.8`目录下执行`make`进行编译，会在`mbedtls-2.16.8/programs/ssl`中生成可执行文件`dtls_client`，执行`./dtls_client`运行客户端，执行结果如下：
```
 . Seeding the random number generator...ok
 . Setting up the SSL structure...ok
 . Setting psk...ok
 . Connecting to udp/192.168.3.200/5658...ok
 . Performing the SSL/TLS handshake...ok
 > Write to server: 9 bytes written
 Hi Server

 > Read from server: 12 bytes read
 Hi Client

 . Closing the connection ...done
```

## dtls_server端日志如下
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 13 2020 02:40:11

**********************************
osAppInit
cpu 0 entering scheduler

Huawei LiteOS # lwip test init ok.
netif: netmask of interface set to 255. 255. 255.0
netif: GW address of interface set to 192. 168. 3. 1
netif_set_ipaddr: netif address being changed
netif: added interface st IP addr 192. 168. 3. 200 netmask 255. 255. 255. 0 gw 192. 168. 3. 1
netif: setting default interface st
[MBEDTLS][dtls_ssl_new:142] setting up the SSL structure
[MBEDTLS][dtls_ssl_new:209] set SSL structure succeed
dtls_server_task:83 dtls_accept return ret = 0, bindfd = 1, cli_fd = 0
dtls_server_task:99 Read from client: 9 bytes read
Hi Server

dtls_server_task:107 Write to client: 12 bytes written
Hi Client
```