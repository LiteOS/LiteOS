/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Ipv6 Client Demo
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifdef LOSCFG_COMPONENTS_NET_LWIP
#include "lwip/sockets.h"
#include "los_task.h"

void ipv6_tcp_test(void)
{
    struct sockaddr_in6 server_addr;
    char msg[128];
    int rbytes = -1;

    int client_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (client_fd < 0) {
        printf("client_fd is %d\n", client_fd);
        return;
    }

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(1883);
    if (inet_pton(AF_INET6, "fe80::70d7:3d63:ca2d:ee52", &server_addr.sin6_addr) <= 0) {
        printf("inet_pton error!!!\n");
        return;
    }
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("socket connect error=%d(%s)!!!\n", errno, strerror(errno));

        return;
    }
    printf("connect to server ok!\n");

    close(client_fd);
}

void ipv6_udp_test(void)
{
    struct sockaddr_in6 server_addr;
    char msg[128];
    int rbytes = -1;

    int client_fd = socket(AF_INET6, SOCK_DGRAM, 17);
    if (client_fd < 0) {
        printf("client_fd is %d\n", client_fd);
        return;
    }

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(3500);
    if (inet_pton(AF_INET6, "fe80::70d7:3d63:ca2d:ee52", &server_addr.sin6_addr) <= 0) {
        printf("inet_pton error!!!\n");
        return;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("socket connect error=%d(%s)!!!\n", errno, strerror(errno));

        return;
    }
    printf("connect to server ok!\n");

    int aa = write(client_fd, "receive ok", strlen("receive ok"));
    printf("aa is %d\n", aa);
    close(client_fd);

}

#endif
