/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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
/**
 *  DATE                AUTHOR      INSTRUCTION
 *  2019-04-28 14:25  zhangqianfu  The first version  
 *
 */
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/errno.h"
#include "lwip/tcpip.h"

#include <string.h>
#include <sal_imp.h>   ///< register the lwip to sal
#include <lwip_imp.h>

///< struct sockaddr and struct sockaddr_in is quit different from the normal defines, so must translate it here
///< something which seems very foolish i think
static int __lwip_bind(int fd, struct sockaddr *addr, int addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = lwip_bind(fd,addr,addrlen);

    return ret;
}

static int __lwip_connect(int fd, struct sockaddr *addr, int addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = lwip_connect(fd,addr,addrlen);

    return ret;
}

static int __lwip_accept(int fd, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    ret = lwip_accept(fd, addr, addrlen);

    memcpy(buf,addr,2);
    family = buf[2];
    memcpy(addr,&family,2);

    return ret;
}


static int __lwip_sendto(int fd, void *msg, int len, int flag, struct sockaddr *addr, int addrlen)
{

    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = lwip_sendto(fd,msg,len,flag,addr,addrlen);

    return ret;
}

static int __lwip_recvfrom(int fd, void *msg, int len, int flag, struct sockaddr *addr, socklen_t *addrlen)
{

    int ret;
    unsigned char buf[2];
    unsigned short family;

    ret = lwip_recvfrom(fd, msg, len,flag,addr,addrlen);

    memcpy(buf,addr,2);
    family = buf[2];
    memcpy(addr,&family,2);

    return ret;
}


static int __lwip_setsockopt(int fd, int level, int option, const void *option_value,int option_len)
{
    if(level == 0xffff)  ///< the lwip make some level and option map
    {
        level = SOL_SOCKET;
    }

    return lwip_setsockopt(fd,level,option,option_value,option_len);
}



static const tag_tcpip_ops s_tcpip_lwip_ops =
{
   .socket = (fn_sal_socket)lwip_socket,
   .bind = (fn_sal_bind)__lwip_bind,
   .listen = (fn_sal_listen)lwip_listen,
   .connect = (fn_sal_connect)__lwip_connect,
   .accept = (fn_sal_accept)__lwip_accept,
   .send = (fn_sal_send)lwip_send,
   .sendto = (fn_sal_sendto)__lwip_sendto,
   .recv = (fn_sal_recv)lwip_recv,
   .recvfrom = (fn_sal_recvfrom)__lwip_recvfrom,
   .setsockopt = (fn_sal_setsockopt)__lwip_setsockopt,
   .getsockopt = (fn_sal_getsockopt)lwip_getsockopt,
   .shutdown =(fn_sal_shutdown) lwip_shutdown,
   .closesocket =(fn_sal_closesocket) lwip_close,
   .getpeername =(fn_sal_getpeername)lwip_getpeername,
   .getsockname = (fn_sal_getsockname)lwip_getsockname,
   .gethostbyname =(fn_sal_gethostbyname)lwip_gethostbyname,
};

static const tag_tcpip_domain s_tcpip_lwip =
{
    .name = "lwip",
    .domain = AF_INET,
    .ops = &s_tcpip_lwip_ops,
};


int tcpipstack_install_lwip(fn_lwip_netdriver driver)
{
    int ret = -1;
    /* Initilialize the LwIP stack with RTOS */
    tcpip_init(NULL, NULL);

    if(NULL != driver)
    {
        ret = driver();
        if(ret != 0)
        {
            return ret;
        }
    }

    ret = tcpip_sal_install(&s_tcpip_lwip);

    if(0 == ret)
    {
        printf("sal:install lwip success\r\n");
    }
    else
    {
        printf("sal:install lwip failed\r\n");
    }

    return 0;
}






