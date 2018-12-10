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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <shell.h>
#include <los_dev.h> 
#include <fs/los_vfs.h>


#if LOSCFG_ENABLE_DRIVER

//this file implement some demo to test the device module
#define cn_testdriv_buf_len 256

typedef struct
{    
    s32_t refers;
}testdriv_cb_test_t;
static testdriv_cb_test_t s_testdriv_cb_test;
//cached only one frame here

static bool_t testdriv_open(void *pri,s32_t flag)
{
    printf("TESTDRIV:PRI:0x%08X OPEN\n\r",(u32_t)pri);

    s_testdriv_cb_test.refers++;
    return true;
}
static void testdriv_close(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X CLOSE\n\r",(u32_t)pri);
    s_testdriv_cb_test.refers--;
    return ;
} 

static bool_t testdriv_write(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    printf("TESTDRIV:PRI:0x%08X WTRITE: buf:0x%08x len:%d timeout:%d\n\r",(u32_t)pri,(u32_t)buf,len,timeout);
    return len;
}

static bool_t testdriv_read(void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    printf("TESTDRIV:PRI:0x%08X READ: buf:0x%08x len:%d timeout:%d\n\r",(u32_t)pri,(u32_t)buf,len,timeout);
    return len;
} 


static bool_t testdriv_init(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X INIT\n\r",(u32_t)pri);
    return true;
}

static void testdriv_deinit(void *pri)
{
    printf("TESTDRIV:PRI:0x%08X DEINIT\n\r",(u32_t)pri);
    return ;
}

static bool_t testdriv_ioctl(void *pri,u32_t cmd, void *para,s32_t paralen)
{
    printf("TESTDRIV:PRI:0x%08X IOCTL:cmd:%d para:0x%08x paralen:%d \n\r",(u32_t)pri,cmd,(u32_t)para,paralen);
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

//all the following command for the interface supplied debug

//#define CN_DEV_TEST_DRIMODE   //dri mode or filesystem mode

#if defined CN_DEV_TEST_DRIMODE
static los_dev_t s_shell_opendev = NULL;
static s32_t __driv_open(s32_t argc,const char *argv[]) //dirvopen drivname flag
{
    los_dev_t dev;
    const char *drivname = 0;
    u32_t flag = 0;
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
    
    dev = los_dev_open(drivname,flag);
    if(NULL == dev)
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

OSSHELL_EXPORT_CMD(__driv_open,"drivopen","drivopen name flag");


static s32_t __driv_write(s32_t argc,const char *argv[]) //drivewrite string timeout
{
    s32_t ret;
    u32_t timeout = 0;
    
    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }
    
    timeout = strtoul(argv[2],NULL,0);
    ret = los_dev_write(s_shell_opendev,0,(u8_t *)argv[1],strlen(argv[1]),timeout);
    printf("write:%d bytes\n\r",ret);

    return 0;
}

OSSHELL_EXPORT_CMD(__driv_write,"drivwrite","drivwrite string timeout");


static s32_t __driv_read(s32_t argc,const char *argv[]) //driveread len timeout
{
    s32_t ret;
    u32_t timeout = 0;
    s32_t len ;
    u8_t *buf;
    
    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }
    
    len = strtoul(argv[1],NULL,0);
    timeout = strtoul(argv[2],NULL,0);
    
    buf = malloc(len);
    ret = los_dev_read(s_shell_opendev,0,buf,len,timeout);
    printf("read:%d bytes\n\r",ret);
    free(buf);
    return 0;
}

OSSHELL_EXPORT_CMD(__driv_read,"drivread","drivread len timeout");

static s32_t __driv_ioctl(s32_t argc,const char *argv[]) //drivioctl cmd cmdpara
{
    bool_t ret;
    u32_t cmd = 0;
    
    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }
    
    cmd = strtoul(argv[1],NULL,0);
    
    ret = los_dev_ioctl(s_shell_opendev,cmd,(void *)argv[2],strlen(argv[2]));
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

OSSHELL_EXPORT_CMD(__driv_ioctl,"drivioctl","drivioctl cmd cmdpara");

static s32_t __driv_close(s32_t argc,const char *argv[]) //drivclose
{
    s32_t ret;
    
    ret = los_dev_close(s_shell_opendev);
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

OSSHELL_EXPORT_CMD(__driv_close,"drivclose","driviclose");

#else  
 
static s32_t s_shell_opendev = NULL;
static s32_t __driv_open(s32_t argc,const char *argv[]) //dirvopen drivname flag
{
    int dev;
    const char *drivname = 0;
    u32_t flag = 0;
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


static s32_t __driv_write(s32_t argc,const char *argv[]) //drivewrite string timeout
{
    s32_t ret;
    u32_t timeout = 0;
    
    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }
    
    timeout = strtoul(argv[2],NULL,0);
    ret = write(s_shell_opendev,(u8_t *)argv[1],strlen(argv[1]));
    printf("write:%d bytes\n\r",ret);

    return 0;
}

OSSHELL_EXPORT_CMD(__driv_write,"write","write string timeout");


static s32_t __driv_read(s32_t argc,const char *argv[]) //driveread len timeout
{
    s32_t ret;
    u32_t timeout = 0;
    s32_t len ;
    u8_t *buf;
    
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

static s32_t __driv_ioctl(s32_t argc,const char *argv[]) //drivioctl cmd cmdpara
{
    bool_t ret;
    u32_t cmd = 0;
    
    if(argc != 3)
    {
        printf("paraerr");
        return 0;
    }
    
    cmd = strtoul(argv[1],NULL,0);
    
    ret = ioctl(s_shell_opendev,cmd,(u32_t)argv[2]);
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

static s32_t __driv_close(s32_t argc,const char *argv[]) //drivclose
{
    s32_t ret;
    
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
#endif 


#endif  //end for LOSCFG_ENABLE_DRIVER
