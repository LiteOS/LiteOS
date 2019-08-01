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
 *  2019-05-09 16:17  zhangqianfu  The first version  
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <sal.h>

__attribute__((weak)) in_addr_t inet_addr(const char *addr)
{
    int para;
    int tmp[4];
    in_addr_t result = INADDR_NONE;
    unsigned char  ip[4];

    para = sscanf(addr,"%d.%d.%d.%d",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    if(para == 4)
    {
        ip[0] = (unsigned char)tmp[0];
        ip[1] = (unsigned char)tmp[1];
        ip[2] = (unsigned char)tmp[2];
        ip[3] = (unsigned char)tmp[3];
        memcpy((void *)&result,(void *)ip,sizeof(result));
    }
    return result;
}

__attribute__((weak)) int inet_aton(const char *string,struct in_addr *addr)
{
    int para;
    int tmp[4];
    unsigned char  ip[4];
    int result = 0;

    para = sscanf(string,"%d.%d.%d.%d",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    if(para == 4)
    {
        ip[0] = (unsigned char)tmp[0];
        ip[1] = (unsigned char)tmp[1];
        ip[2] = (unsigned char)tmp[2];
        ip[3] = (unsigned char)tmp[3];
        memcpy((void *)addr,(void *)ip,sizeof(ip));
        result = 1;
    }

    return result;
}


#define cn_inet_addrlen_max     16
static  char s_inet_addr_buf[cn_inet_addrlen_max];

__attribute__((weak)) char *inet_ntoa(struct in_addr addr)
{
    unsigned char ip[4];
    int tmp[4];
    memcpy((void *)ip,(void *)&addr,sizeof(ip));
    memset((void *)s_inet_addr_buf,0,cn_inet_addrlen_max);

    tmp[0] = (int)ip[0];
    tmp[1] = (int)ip[1];
    tmp[2] = (int)ip[2];
    tmp[3] = (int)ip[3];
    sprintf(s_inet_addr_buf,"%d.%d.%d.%d",tmp[0],tmp[1],tmp[2],tmp[3]);

    return (char *)s_inet_addr_buf;
}

__attribute__((weak)) const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt)
{
    char *str;
    char          *result = NULL;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        return result;
    }

    if(af==AF_INET)
    {

        memcpy((void *)&addr,src,sizeof(addr));
        str = inet_ntoa(addr);

        if((NULL != str)&&(cnt > (strlen(str)+1)))
        {
            memcpy((void*)dst,(void*)str,strlen(str)+1);
            result = dst;
        }
    }

    return result;
}

__attribute__((weak)) int inet_pton(int af, const char *src, void *dst)
{

    int result = -1;
    struct in_addr addr;

    if((NULL == src)||(NULL == dst))
    {
        result = 0;
        return result;
    }

    if(af==AF_INET)
    {
        if(inet_aton(src,&addr))
        {
            memcpy((void *)dst,&addr,sizeof(addr));
            result = 1;
        }
    }

    return result;
}
