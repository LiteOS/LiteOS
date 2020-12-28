# Ipv6_client使用教程

## 使用场景
Ipv6_client用于演示Ipv6客户端与服务端的连接。下面以开发板Cloud_STM32F429IGTx_FRIE为例，讲解如何在Cloud_STM32F429IGTx_FIRE开发板上使能Ipv6_client，实现与服务端之间的连接与通信。

## 相关配置

### 1.在路由器(测试 荣耀WS851)中设置Ipv6局域网前缀

在支持Ipv6的路由器中设置地址前缀(测试中使用的是 荣耀WS851路由器，前缀设为2000::，前缀长度设为64)。

### 2.设置开发板Ipv6地址和网关地址

在<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c" target="_blank">sys_init.c</a>文件的`void net_init(void)`函数中设置开发板的IP地址及网关地址，网关地址与路由器中设置的前缀保持一致。

### 3.设置服务端的地址和监听端口

在服务端的PC机上获取到Ipv6地址后，在开发板的<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/ipv6_client/client_demo.c" target="_blank">client_demo.c</a>文件中通过`SERVER_ADDR`、`SERVER_PORT`宏设置服务端的Ipv6地址和端口。

## 开启客户端 Client并选择连接方式

Ipv6_client需要Lwip组件的支持，可通过make munuconfig使能该组件。

```
Components --> Network --> Enable Network -> Enable Lwip -> Enable Ipv4 Or Ipv6 -> Enable Ipv6
```

Ipv6_client支持Tcp和Udp协议，下面通过make munuconfig使能该Demo并设置为Tcp协议。
```
Demos --> Ipv6 Client Demo --> Enable Ipv6 Client Demo -> Enable Tcp Or Udp --> Enable Tcp Client
```
完成以上操作后编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，Ipv6_client即启动。

## 编写服务端程序

开发板上运行的是客户端程序，用户需要自行编写服务端程序与客户端进行交互。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_ADDR   "2000::4639:c4ff:fe94:5d44"
#define SERVER_PORT   1883
#define PROTOCOL_TYPE 1


int Ipv6TcpServer()
{
    int ret;
    int count;
    int recvLen;
    int serverFd;
    char msg[128];
    socklen_t addrLen;
    struct sockaddr_in6 serverAddr;
    struct sockaddr_in6 clientAddr;

    serverFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (serverFd < 0) {
        printf("socket failed\n");
        return -1;
    }

    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &serverAddr.sin6_addr);
    addrLen = sizeof(serverAddr);

    if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("bind failed");
        return -1;
    }

    if (listen(serverFd, 5) < 0) {
        printf("listen failed\n");
        return -1;
    } else {
        printf("listen ...\n");
    }

    while (1) {
        if ((ret = accept(serverFd, (struct sockaddr *)&clientAddr, &addrLen)) < 0) {
            printf("accept failed\n");
            return -1;
        }

        memset(msg, 0, sizeof(msg));
        strcpy(msg, "Hi ipv6 client");
        addrLen = send(ret, msg, strlen(msg), 0);
        if (addrLen < 0) {
            printf("send failed\n");
            break;
        } else {
            printf("send: %s\n", msg);
            count++;
        }

        memset(msg, 0, sizeof(msg));
        recvLen = recv(ret, msg, sizeof(msg), 0);
        if (recvLen > 0) {
            printf("recv: %s\n", msg);
        }
    }

    close(serverFd);
    return 0;
}


int Ipv6UdpServer()
{
    int recvLen;
    int serverFd;
    char msg[128];
    socklen_t addrLen;
    struct sockaddr_in6 serverAddr;

    serverFd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (serverFd < 0) {
        printf("socket failed\n");
        return -1;
    }

    addrLen = sizeof(struct sockaddr_in6);
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET6, SERVER_ADDR, &serverAddr.sin6_addr);
    if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("bind failed\n");
        return -1;
    }

    printf("recv form client: ");
    fflush(stdout);
    while (1) {
        memset(msg, 0, sizeof(msg));
        recvLen = recvfrom(serverFd, msg, sizeof(msg), 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (recvLen > 0) {
            printf("%d bytes \n>%s\n", recvLen, msg);
            continue;
        }
    }

    close(serverFd);
    return 0;
}

int main(void)
{
    if (PROTOCOL_TYPE) {
        Ipv6TcpServer();
    } else {
        Ipv6UdpServer();
    }

    return 0;
}
```

**参数说明**

```
PROTOCOL_TYPE  传输协议 0-Udp 1-Tcp
SERVER_ADDR    服务端Ipv6地址
SERVER_PORT    服务端监听端口
```

#### Tcp
在文件路径下执行`gcc ipv6_server.c -o server`命令编译服务端程序，编译后生成可执行程序server，执行`./server`运行该服务端程序。传输协议为`Tcp`时的服务端执行结果如下：

```
listen ...
send: Hi ipv6 client
recv: Hi ipv6 server
```

客户端输出如下：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 27 2020 02:02:31

**********************************
osAppInit
cpu 0 entering scheduler
lwip test init ok.
read from server: 14 bytes read
> Hi ipv6 client
write to server: 14 bytes written
> Hi ipv6 server
```

#### Udp

修改服务端`ipv6_server.c`及开发板上Ipv6_client的传输协议为Udp。在文件路径下执行`gcc ipv6_server.c -o server`命令编译服务端程序，编译后生成可执行程序server，执行`./server`运行该服务端程序。传输协议为`Udp`时的服务端执行结果如下：

```
recv from client: 14 bytes
>Hi ipv6 server
```

客户端输出如下：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0-rc1
build data : Nov 27 2020 02:14:43

**********************************
osAppInit
cpu 0 entering scheduler
lwip test init ok.
write to server: 14 bytes written
> Hi ipv6 server
```
