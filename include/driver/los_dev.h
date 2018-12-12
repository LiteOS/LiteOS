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

#ifndef __DEVICE_H
#define __DEVICE_H

#include <los_config.h>

#include <osdepends/osport.h>
#include <fs/sys/fcntl.h>

#define  LOSCFG_ENABLE_DRIVER 1


typedef void* los_driv_t ;//returned by the driver register
typedef bool_t (*fn_open)  (void *pri,s32_t flag);                                              
typedef s32_t  (*fn_read)  (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef s32_t  (*fn_write) (void *pri,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
typedef void   (*fn_close) (void *pri); 
typedef bool_t (*fn_ioctl) (void *pri,u32_t cmd, void *para,s32_t len);
typedef bool_t (*fn_init)  (void *pri);
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
}los_driv_op_t;
//the member could be NULL,depend on the device property
//attention that whether the device support multi read and write depend on the device itself

typedef void* los_dev_t ;                   //this type is returned by the dev open



#if LOSCFG_ENABLE_DRIVER
//device module init entry
bool_t  los_driv_init(void);
//these interface called by the bsp develop
los_driv_t los_driv_register(const char *name, const los_driv_op_t *op,void *pri,u32_t flagmask);
bool_t     los_driv_unregister(const char *name) ;
bool_t     los_driv_event(los_driv_t driv,u32_t event,void *para);  
//attention that the device dirver could use this function to send some event to the application,
//something like the device could be read or write or something like this for async call

//these interface by the application
los_dev_t  los_dev_open  (const char *name,u32_t flag);
s32_t      los_dev_read (los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
s32_t      los_dev_write (los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout);
bool_t     los_dev_close (los_dev_t dev);
bool_t     los_dev_ioctl (los_dev_t dev,u32_t cmd,void *para,s32_t paralen);
//attention that the cmd and para used in ioctl must compatible with the driver development

//we also supply a macro to add a driver staticly ,which means 
typedef struct 
{
    const char            *name;      //device driver name 
    los_driv_op_t         *op;        //device operate functions
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
  
#define   los_driv_init()                              false
#define   los_driv_register(name,op,pri,flagmask)      NULL
#define   los_driv_unregister(name)                    false
#define   los_driv_event(drive,event,para)             false
#define   los_dev_open(name,flag)                      NULL
#define   los_dev_read (dev,offset,buf,len,timeout)    0
#define   los_dev_write(dev,offset,buf,len,timeout)    0
#define   los_dev_ioctl (dev,cmd,para,paralen)         false
#define   los_dev_close (dev)                          false
   
#define OSDRIV_EXPORT(varname,drivname,operate,pridata,flagmask)
#endif   //end for LOSCFG_ENABLE_DRIVER
    
#if LOSCFG_ENABLE_DEVFS
bool_t devfs_install(void);       
#else 
                                               
#define   devfs_install()      
#endif  //end for the LOSCFG_ENABLE_DRIVER
   
#endif  //end for the file




