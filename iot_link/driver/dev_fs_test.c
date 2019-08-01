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
 *  2019-05-14 09:44  zhangqianfu  The first version  
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <shell.h>
#include <driver.h>
#include "fs/inc/los_vfs.h"
#include "sys/fcntl.h"

//this file implement some demo to test the device module
#define cn_testdriv_buf_len 256

typedef struct
{
    int refers;
}testdriv_cb_test_t;
static testdriv_cb_test_t s_testdriv_cb_test;
//cached only one frame here

static bool_t testdriv_open(void *pri,int flag)
{
    printf("TESTDRIV:PRI:0x%08X OPEN\n\r",(unsigned int)pri);

    s_testdriv_cb_test.refers++;
    return true;
}
static void testdriv_close(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X CLOSE\n\r",(unsigned int)pri);
    s_testdriv_cb_test.refers--;
    return ;
}

static bool_t testdriv_write(void *pri,unsigned int offset,unsigned char *buf,int len,unsigned int timeout)
{
    printf("TESTDRIV:PRI:0x%08X WTRITE: buf:0x%08x len:%d timeout:%d\n\r",(unsigned int)pri,(unsigned int)buf,len,timeout);
    return len;
}

static bool_t testdriv_read(void *pri,unsigned int offset,unsigned char *buf,int len,unsigned int timeout)
{
    printf("TESTDRIV:PRI:0x%08X READ: buf:0x%08x len:%d timeout:%d\n\r",(unsigned int)pri,(unsigned int)buf,len,timeout);
    return len;
}


static bool_t testdriv_init(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X INIT\n\r",(unsigned int)pri);
    return true;
}

static void testdriv_deinit(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X DEINIT\n\r",(unsigned int)pri);
    return ;
}

static bool_t testdriv_ioctl(void *pri,unsigned int cmd, void *para,int paralen)
{
    printf("TESTDRIV:PRI:0x%08X IOCTL:cmd:%d para:0x%08x paralen:%d \n\r",(unsigned int)pri,cmd,(unsigned int)para,paralen);
    return  true;
}

static const  los_driv_op_t  s_testdriv =
{
    .open = testdriv_open,
    .close = testdriv_close,
    .ioctl= testdriv_ioctl,
    .read = testdriv_read,
    .write = testdriv_write,
    .init = testdriv_init,
    .deinit = testdriv_deinit,
};

OSDRIV_EXPORT(drivpara1,"dev1",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);
OSDRIV_EXPORT(drivpara2,"dev1",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);
OSDRIV_EXPORT(drivpara3,"dev2",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);
OSDRIV_EXPORT(drivpara4,"dev3",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);
OSDRIV_EXPORT(drivpara5,"dev2",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);
OSDRIV_EXPORT(drivpara6,"dev1",(los_driv_op_t *)&s_testdriv,NULL,O_RDWR);

static int s_shell_opendev = NULL;
static int __driv_open(int argc,const char *argv[]) //dirvopen drivname flag
{
    int dev;
    const char *drivname = 0;
    unsigned int flag = 0;
    if(argc != 3 )
    {
        printf("paraerr");
        return 0;
    }
    drivname = argv[1];
    if( 0 == strcmp(argv[2],"O_RDWR"))
    {
        flag = O_RDWR;
    }
    else if( 0 == strcmp(argv[2],"O_RDONLY"))
    {
        flag = O_RDONLY;
    }
    else if(0 == strcmp(argv[2],"O_WRONLY"))
    {
        flag = O_WRONLY;
    }
    else
    {
        printf("open flag err\r\n");
        return 0;
    }

    dev = open(drivname,flag);
    if(dev < 0)
    {
        printf("open err\r\n");
        return 0;
    }
    else
    {
        printf("open OK\r\n");
    }

    s_shell_opendev = dev;
    return 0;
}

OSSHELL_EXPORT_CMD(__driv_open,"open","open name flag");


static int __driv_write(int argc,const char *argv[]) //drivewrite string timeout
{
    int ret;
    unsigned int timeout = 0;

    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }

    timeout = strtoul(argv[2],NULL,0);
    ret = write(s_shell_opendev,(unsigned char *)argv[1],strlen(argv[1]));
    printf("write:%d bytes\n\r",ret);

    return 0;
}

OSSHELL_EXPORT_CMD(__driv_write,"write","write string timeout");


static int __driv_read(int argc,const char *argv[]) //driveread len timeout
{
    int ret;
    unsigned int timeout = 0;
    int len ;
    unsigned char *buf;

    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }

    len = strtoul(argv[1],NULL,0);
    timeout = strtoul(argv[2],NULL,0);

    buf = malloc(len);
    ret = read(s_shell_opendev,buf,len);
    printf("read:%d bytes\n\r",ret);
    free(buf);
    return 0;
}

OSSHELL_EXPORT_CMD(__driv_read,"read","read len timeout");

static int __driv_ioctl(int argc,const char *argv[]) //drivioctl cmd cmdpara
{
    bool_t ret;
    unsigned int cmd = 0;

    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }

    cmd = strtoul(argv[1],NULL,0);

    ret = ioctl(s_shell_opendev,cmd,(unsigned int)argv[2]);
    if(ret)
    {
        printf("IOCTL OK\r\n");
    }
    else
    {
        printf("IOCTL ERR\r\n");
    }


    return 0;
}

OSSHELL_EXPORT_CMD(__driv_ioctl,"ioctl","ioctl cmd cmdpara");

static int __driv_close(int argc,const char *argv[]) //drivclose
{
    int ret;

    ret = close(s_shell_opendev);
    if(ret)
    {
        printf("CLOSE OK\r\n");
    }
    else
    {
        printf("CLOSE ERR\r\n");

    }

    return 0;
}

OSSHELL_EXPORT_CMD(__driv_close,"close","close");
