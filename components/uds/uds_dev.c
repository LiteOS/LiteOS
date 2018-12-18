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
//this file implement the device driver 
#include <stdio.h>
#include <stdlib.h>
#include "uds/uds_dev.h"

#if LOSCFG_ENABLE_DRIVER

//#include <sys/fcntl.h>

#define cn_driv_status_initialized     (1<<0)
#define cn_driv_status_opend           (1<<1)


//define the error type has happedn in the driver
typedef enum
{
    en_dev_err_none = 0,   //no err happened
    en_dev_err_init,       //call bsp open function failed
    en_dev_err_open,       //call bsp init function failed
    
}en_dev_error_type;


struct driv_cb
{
    void                  *nxt;              //add the deice to device list
    void                  *pri;              //bsp developed para
    const char            *name;             //the device name
    s32_t                  flagmask;         //copy from the register             
    const los_driv_op_t   *op;               //operation method
    u32_t                  drivstatus;       //show the state here like init or something like this
    los_dev_t              devlst;           //the open list,support the multi open
    //following member used for the debug
    u32_t            total_write;        //how many data has been sent
    u32_t            total_read;        //how many data has received
    u32_t            opencounter;      //reference counter
    u32_t            errno;            //the last errno has happend
};
typedef struct
{
    mutex_t                         lock;      //used to lock the devlst
    struct driv_cb                 *drivlst;   //all the dev will be added to the list
    u32_t                           drivnum;
}los_driv_module;
static  los_driv_module   s_los_driv_module ;  //manage all the driver here

//used this function to find a dev
static struct driv_cb *__driv_match(const char *name)
{   
    struct driv_cb *ret = NULL;
    
    ret = s_los_driv_module.drivlst;
    while(NULL != ret)
    {
        if(0 == strcmp(name,ret->name))
        {
            break;
        }
        ret = ret->nxt;
    }

    return ret;
}

/*******************************************************************************
function     :bsp developer use this function to add a device to the system
parameters   :
instruction  :NULL if failed else return the device handle
*******************************************************************************/
los_driv_t los_driv_register(const char *name, const los_driv_op_t *op,\
                             void *pri,u32_t flagmask)
{
    struct driv_cb  *driv = NULL;

    if((NULL == name)||(NULL == op))
    {
        goto EXIT_PARAS;
    }

    driv = malloc(sizeof(struct driv_cb));
    if(NULL == driv)
    {
        goto EXIT_MALLOC;
    }
    memset(driv,0,sizeof(struct driv_cb));

    //do the member initialize
    driv->name = name;
    driv->op = op;
    driv->pri = pri;
    driv->flagmask = flagmask;
    
    //add it to the device list if no device with the same name exsit
    if(false == mutex_lock(s_los_driv_module.lock))
    {
        goto EXIT_MUTEX;
    }
    
    if(NULL != __driv_match(name))
    {
        goto EXIT_EXISTED;
    }
    
    driv->nxt = s_los_driv_module.drivlst;
    s_los_driv_module.drivlst = driv;
    mutex_unlock(s_los_driv_module.lock);
    
    s_los_driv_module.drivnum++;
    
    return driv;


EXIT_EXISTED:
    mutex_unlock(s_los_driv_module.lock);
EXIT_MUTEX:
    free(driv);
    driv = NULL;
EXIT_MALLOC:
EXIT_PARAS:
    return driv;
}

/*******************************************************************************
function     :bsp developer use this function to remove a device from the system
parameters   :
instruction  :if the device has been refered,then will fail here
*******************************************************************************/
s32_t los_driv_unregister(const char *name) 
{
    struct driv_cb *tmp = NULL;
    struct driv_cb *pre = NULL;
    s32_t           ret = -UDS_NOT_FOUND;
    
    if(NULL == name)
    {
        return -UDS_NAME_INVALID;
    }

    if(mutex_lock(s_los_driv_module.lock))
    {
        tmp = s_los_driv_module.drivlst;
        pre = NULL;
        while(NULL != ret)
        {
            if( 0 == strcmp(name,tmp->name))
            {
                break;
            }
            pre = tmp;
            tmp = tmp->nxt;
        }
        if(NULL != tmp)  //find the device here
        {
            if(pre == NULL)
            {
                s_los_driv_module.drivlst = tmp->nxt;
            }
            else
            {
                pre->nxt = tmp->nxt;
            }

            free(tmp);
            ret = UDS_OK;
            
            s_los_driv_module.drivnum--;
        }

        mutex_unlock(s_los_driv_module.lock);   
    }

    return ret;
}

/*******************************************************************************
function     :bsp developer use this function to notify the application any event has happened
parameters   :
instruction  :not implement yet,we will do it as the asynchronize call
*******************************************************************************/
s32_t los_driv_event(los_driv_t driv,u32_t event,void *para)
{
    return -UDS_ERROR;
}


#ifdef __CC_ARM /* ARM C Compiler ,like keil,options for linker:--keep *.o(osdriv)*/
extern u32_t osdriv$$Base;
extern u32_t osdriv$$Limit;         
//#pragma section("osdriv", read) 
#endif        


static void osdriv_load_static(void){
    
    os_driv_para_t *para;
    u32_t num = 0;
    s32_t i = 0;
#if defined (__CC_ARM)    //you could add other compiler like this
    num = ((u32_t)&osdriv$$Limit-(u32_t)&osdriv$$Base)/sizeof(os_driv_para_t);
    para = (os_driv_para_t *) &osdriv$$Base;
#elif defined(__GNUC_LIKE)
    extern unsigned int __osdriv_start;
    extern unsigned int __osdriv_end;
    para = (os_driv_para_t *)&__osdriv_start;
    i = ((unsigned int )&__osdriv_end - (unsigned int)&__osdriv_start)/sizeof(os_driv_para_t);
#else 
    error("unknown compiler here");
#endif    
    for(i =0;i<num;i++)
    {
        los_driv_register(para->name,para->op,para->pri,para->flag);
        para++;
    }

    return;
}

/*******************************************************************************
function     :the device module entry
parameters   :
instruction  :call this function to initialize the device module here
              load the static init from section os_device
*******************************************************************************/
s32_t  los_driv_init(void)
{
    bool_t ret = false;

    ret = mutex_create(&s_los_driv_module.lock);
    if(false == ret)
    {
        return -UDS_ERROR;
    }

    //load all the static device init
    osdriv_load_static();
    return UDS_OK;
}

//here we define the data structure which used by the driv application apis
struct dev_cb
{
    void *nxt;            //used by the open lst
    void *driv;           //which attached dri here
    s32_t openflag;       //here means the open state here
    u32_t offset;
    //the following for the debug
    u32_t readbytes;
    u32_t writebytes; 
};

/*******************************************************************************
function     :open the device with the specified name
parameters   :
instruction  :if first open the device,then will call the init function if exsited
*******************************************************************************/
los_dev_t  los_dev_open  (const char *name,u32_t flag)
{
    bool_t opret = true;           //operate result
    struct driv_cb *driv = NULL;   //the driver attached
    struct dev_cb  *dev = NULL;    //the device opened
    if ( NULL == name )
    {
        goto EXIT_PARAERR;
    }

    dev = malloc(sizeof(struct dev_cb));
    if (NULL == dev)
    {
        goto EXIT_MEMERR;
    }
    memset(dev,0,sizeof(struct dev_cb));

    opret = mutex_lock(s_los_driv_module.lock);
    if(false == opret)
    {
        goto EXIT_MUTEXERR;
    }

    driv = __driv_match(name);
    if(NULL == driv)
    {
        goto EXIT_DRIVERR;
    }
    //WE DON'T CARE TOO MUCH ABOUT THE RD AND WR FLAG,MAY BE TODO IT IN THE NEXT VERSION HERE
    if((O_EXCL & driv->flagmask) && (NULL != driv->devlst))
    {
        goto EXIT_EXCLERR;
    }

    if((0 == (driv->drivstatus & cn_driv_status_initialized)) && \
        (NULL != driv->op->init))
    {
        ;
        if(UDS_OK != driv->op->init(driv->pri))
        {
            driv->errno = en_dev_err_init;
            goto EXIT_INITERR;
        }
        driv->drivstatus |= cn_driv_status_initialized;
    }

    if(NULL != driv->op->open)
    {
        if(UDS_OK != driv->op->open(driv->pri,flag))
        {
            driv->errno = en_dev_err_open;
            goto EXIT_OPENERR;
        }
    }

    //reach here means all the initialize is ok
    //add the dev to the list of the drive and  attach the driv to the device
    driv->opencounter++;
    dev->nxt = driv->devlst;
    driv->devlst = dev;

    dev->driv = driv;
    dev->openflag =  flag;

    mutex_unlock(s_los_driv_module.lock);
    return dev;

EXIT_OPENERR:
EXIT_INITERR:
EXIT_EXCLERR:
EXIT_DRIVERR:
    mutex_unlock(s_los_driv_module.lock);
EXIT_MUTEXERR:
    free(dev);
    dev = NULL;
EXIT_MEMERR:
EXIT_PARAERR:
    return dev;    
}

/*******************************************************************************
function     :close the device of opened
parameters   :handle returned by open
instruction  :
*******************************************************************************/
s32_t  los_dev_close  (los_dev_t dev)
{
    s32_t ret = -UDS_ERROR;
    struct dev_cb *devcb = NULL;
    struct dev_cb *tmp = NULL;
    struct driv_cb *driv = NULL;

    if ( NULL == dev )
    {
        goto EXIT_PARAERR;
    }
    devcb = dev;

    if(false == mutex_lock(s_los_driv_module.lock))
    {
        goto EXIT_MUTEXERR;
    }

    //deattach the dev from the driv
    driv = devcb->driv;
    if(NULL == driv)
    {
        goto EXIT_DRIVERR;
    }

    if(devcb == driv->devlst)
    {
        driv->devlst = devcb->nxt;
    }
    else
    {
        tmp = driv->devlst;
        while(NULL != tmp)
        {
            if(tmp->nxt == devcb)
            {
                tmp->nxt = devcb->nxt;
                break;
            }
            tmp = tmp->nxt;
        }
        if(NULL == tmp)
        {
            goto EXIT_DETACHERR;
        }
    }
    if(NULL != driv->op->close)
    {
        driv->op->close(driv->pri);
    }

    if((NULL == driv->devlst) && (NULL != driv->op->deinit))
    {
        driv->op->deinit(driv->pri);
        driv->drivstatus &= (~cn_driv_status_initialized);
    }

    free(dev);
    driv->opencounter--;

    mutex_unlock(s_los_driv_module.lock);

    ret = UDS_OK;

EXIT_DETACHERR:
EXIT_DRIVERR:
    mutex_unlock(s_los_driv_module.lock);
EXIT_MUTEXERR:
EXIT_PARAERR:
    return ret;
}

/*******************************************************************************
function     :use this function to read data from the device
parameters   :dev,returned by the los_dev_open function
              offet:from where to read,only used for storage device
              buf:used to storage the data
              len:the length of the buf
              timeout:the waittime if no data current
instruction  :how many data has been read to the buf
*******************************************************************************/
s32_t   los_dev_read(los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t ret = 0;
    struct dev_cb  *devcb;
    struct driv_cb *drivcb;

    if((NULL != dev)&&(NULL != buf)&&(0 != len))
    {
        devcb = dev;
        if((0 == (devcb->openflag & O_WRONLY)))
        {
            drivcb = devcb->driv;
            if((NULL != drivcb->op)&&(NULL != drivcb->op->read))
            {
                ret = drivcb->op->read( drivcb->pri,offset,buf,len,timeout);
                if(ret > 0)
                {
                    drivcb->total_read += ret;
                }
            }
        }
    }

    return ret;
}

/*******************************************************************************
function     :use this function to write data to the device
parameters   :dev,returned by the los_dev_open function
              offset: from where to write,only used for storage device
              buf:the data to be written
              len:the length of the buf
              timeout:the waittime if no data current
instruction  :how many data has been written to the device
*******************************************************************************/
s32_t los_dev_write (los_dev_t dev,u32_t offset,u8_t *buf,s32_t len,u32_t timeout)
{
    s32_t ret = 0;
    struct dev_cb  *devcb;
    struct driv_cb *drivcb;

    if((NULL != dev) && (NULL != buf) && (len != 0))
    {
        devcb = dev;
        if((devcb->openflag & O_WRONLY) || (devcb->openflag & O_RDWR))
        {
            drivcb = devcb->driv;
            if((NULL != drivcb->op)&&(NULL != drivcb->op->write))
            {
                ret = drivcb->op->write( drivcb->pri,offset,buf,len,timeout);
                if(ret > 0)
                {
                    drivcb->total_write += ret;
                }
            }
        }
    }

    return ret;
}

/*******************************************************************************
function     :use this function to control the device
parameters   :dev,returned by the los_dev_open function
              cmd, must compatible with bsp develop
              para,used with cmd, its length and format depend on the bsp develop
instruction  :ctrol true or false
*******************************************************************************/
s32_t los_dev_ioctl (los_dev_t dev,u32_t cmd,void *para,s32_t paralen)
{
    s32_t ret = -UDS_ERROR;
    
    struct dev_cb  *devcb;
    struct driv_cb *drivcb;

    if(NULL != dev)
    {
        devcb = dev;
        drivcb = devcb->driv;
        if((NULL != drivcb->op)&&(NULL != drivcb->op->ioctl))
        {
            ret = drivcb->op->ioctl( drivcb->pri,cmd,para,paralen);
        }
    }

    return ret;
}



#endif
