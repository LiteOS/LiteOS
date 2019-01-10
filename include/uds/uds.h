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

#ifndef __UDS_H
#define __UDS_H

#include <stdio.h>
#include <fcntl.h>


#define  LOSCFG_ENABLE_DRIVER 1

enum uds_err_code
{
    UDS_OK,
    UDS_ERROR,
    UDS_NAME_INVALID,
    UDS_NOT_FOUND,
    UDS_TIMEOUT,
};



#ifndef u8_t
#define u8_t   unsigned char    
#endif
#ifndef u16_t
#define u16_t  unsigned short    
#endif
#ifndef u32_t
#define u32_t  unsigned  int   
#endif
#ifndef s8_t
#define s8_t   signed char    
#endif
#ifndef s16_t
#define s16_t  signed short    
#endif
#ifndef s32_t
#define s32_t  signed int   
#endif


typedef void* uds_dev_t;
typedef void* uds_driv_t ;//returned by the driver register


#if LOSCFG_ENABLE_DRIVER


typedef s32_t  (*fn_open)  (void *pri,s32_t flag);                                              
typedef s32_t  (*fn_read)  (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef s32_t  (*fn_write) (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef void   (*fn_close) (void *pri); 
typedef s32_t  (*fn_ioctl) (void *pri,u32_t cmd, void *para,s32_t len);
typedef s32_t  (*fn_init)  (void *pri);
typedef void   (*fn_deinit)(void *pri);
//all the member function of pri is inherited by the register function
typedef struct
{
    fn_open    open;   //triggered by the application
    fn_read    read;   //triggered by the application
    fn_write   write;  //triggered by the application
    fn_close   close;  //triggered by the application
    fn_ioctl   ioctl;  //triggered by the application
    fn_init    init;   //if first open,then will be called
    fn_deinit  deinit; //if the last close, then will be called
}uds_driv_op_t;

s32_t       uds_driv_init(void);
uds_driv_t  uds_driv_register(const char *name, const uds_driv_op_t *op, void *pri, u32_t flagmask);
s32_t       uds_driv_unregister(const char *name);
s32_t       uds_driv_event(uds_driv_t driv,u32_t event,void *para);
uds_dev_t   uds_dev_open  (const char *name,u32_t flag);
s32_t       uds_dev_read (uds_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
s32_t       uds_dev_write (uds_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
s32_t       uds_dev_close (uds_dev_t dev);
s32_t       uds_dev_ioctl (uds_dev_t dev,u32_t cmd,void *para,s32_t paralen);

typedef struct 
{
    const char            *name;      //device driver name 
    uds_driv_op_t         *op;        //device operate functions
    void                  *pri;       //private data,will be passed to op functions
    u32_t                  flag;      //flags, like O_RDONLY O_WRONLY O_RDWR
}os_driv_para_t;

#define OSDRIV_EXPORT(varname,drivname,operate,pridata,flagmask)      \
    static const os_driv_para_t varname __attribute__((used,section("osdriv")))= \
    {                           \
        .name   = drivname,    \
        .op     = operate,      \
        .pri    = pridata,  \
        .flag   = flagmask,    \
    }

   


#else   
  
#define   uds_driv_init()                              
#define   uds_driv_register(name,op,pri,flagmask)      
#define   uds_driv_unregister(name)                    
#define   uds_driv_event(drive,event,para)             
#define   uds_dev_open(name,flag)                      
#define   uds_dev_read (dev,offset,buf,len,timeout)    
#define   uds_dev_write(dev,offset,buf,len,timeout)    
#define   uds_dev_ioctl (dev,cmd,para,paralen)         
#define   uds_dev_close (dev)                          
   
#define   OSDRIV_EXPORT(varname,drivname,operate,pridata,flagmask)
#endif

#endif

