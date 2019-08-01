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
 *  2019-05-23 15:09    huerjia     The first version  
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sal_imp.h>   ///< register the macos socket to sal
#include <macos_socket_imp.h>


///< struct sockaddr and struct sockaddr_in is quit different from the normal defines, so must translate it here
///< something which seems very foolish i think
static int __macos_bind(int fd, struct sockaddr *addr, int addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = bind(fd,addr,addrlen);

    return ret;
}

static int __macos_connect(int fd, struct sockaddr *addr, int addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = connect(fd,addr,addrlen);

    return ret;
}

static int __macos_accept(int fd, struct sockaddr *addr, socklen_t *addrlen)
{
    int ret;
    unsigned char buf[2];
    unsigned short family;

    ret = accept(fd, addr, addrlen);

    memcpy(buf,addr,2);
    family = buf[1];
    memcpy(addr,&family,2);

    return ret;
}


static int __macos_sendto(int fd, void *msg, int len, int flag, struct sockaddr *addr, int addrlen)
{

    int ret;
    unsigned char buf[2];
    unsigned short family;

    memcpy(&family,addr,2);
    buf[0] = addrlen;
    buf[1] = family;
    memcpy(addr,buf,2);

    ret = sendto(fd,msg,len,flag,addr,addrlen);

    return ret;
}

static int __macos_recvfrom(int fd, void *msg, int len, int flag, struct sockaddr *addr, socklen_t *addrlen)
{

    int ret;
    unsigned char buf[2];
    unsigned short family;

    ret = recvfrom(fd, msg, len,flag,addr,addrlen);

    memcpy(buf,addr,2);
    family = buf[1];
    memcpy(addr,&family,2);

    return ret;
}

///< map the level and option
#include <sys/time.h>
static int __macos_setsockopt(int fd, int level, int option, const void *option_value, int option_len)
{
    struct timeval *timedelay;

    if((level == SOL_SOCKET))
    {
        if(option == SO_RCVTIMEO)
        {
            timedelay =option_value;
            if((timedelay->tv_sec == 0) && (timedelay->tv_usec == 0))
            {
                printf("%s:log:::::::timeout should be mapped:::::modified it 1000 us\n\r",__FUNCTION__);
                timedelay->tv_usec =1000;
            }
        }
    }

    return setsockopt(fd, level, option, option_value, option_len);
}



static const tag_tcpip_ops s_tcpip_socket_ops =
{
   .socket = (fn_sal_socket)socket,
   .bind = (fn_sal_bind)__macos_bind,
   .listen = (fn_sal_listen)listen,
   .connect = (fn_sal_connect)__macos_connect,
   .accept = (fn_sal_accept)__macos_accept,
   .send = (fn_sal_send)send,
   .sendto = (fn_sal_sendto)__macos_sendto,
   .recv = (fn_sal_recv)recv,
   .recvfrom = (fn_sal_recvfrom)__macos_recvfrom,
   .setsockopt = (fn_sal_setsockopt)__macos_setsockopt,
   .getsockopt = (fn_sal_getsockopt)getsockopt,
   .shutdown =(fn_sal_shutdown)shutdown,
   .closesocket =(fn_sal_closesocket)close,
   .getpeername =(fn_sal_getpeername)getpeername,
   .getsockname = (fn_sal_getsockname)getsockname,
   .gethostbyname = (fn_sal_gethostbyname)gethostbyname,
};

static const tag_tcpip_domain s_tcpip_socket =
{
    .name = "macos socket",
    .domain = AF_INET,
    .ops = &s_tcpip_socket_ops,
};


int tcpipstack_install_macos_socket(void)
{
    int ret = -1;

    ret = tcpip_sal_install(&s_tcpip_socket);

    if(0 == ret)
    {
        printf("sal:install socket success\r\n");
    }
    else
    {
        printf("sal:install socket failed\r\n");
    }

    return 0;
}






