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
 *  2019-05-14 09:36  zhangqianfu  The first version  
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <driver.h>

#if CONFIG_DRIVER_ENABLE

#include <sys/fcntl.h>
#include <sys/types.h>

#define cn_driv_status_initialized     (1<<0)
#define cn_driv_status_opend           (1<<1)


//define the error type has happened in the driver
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
    int                    flagmask;         //copy from the register
    const los_driv_op_t   *op;               //operation method
    unsigned int           drivstatus;       //show the state here like init or something like this
    los_dev_t              devlst;           //the open list,support the multi open
    //following member used for the debug
    size_t            total_write;            //how many data has been sent
    size_t            total_read;             //how many data has received
    size_t            opencounter;            //reference counter
    unsigned int      errno;                  //the last errno has happend
};
typedef struct
{
    osal_mutex_t                    lock;      //used to lock the devlst
    struct driv_cb                 *drivlst;   //all the dev will be added to the list
    unsigned int                    drivnum;
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

los_driv_t los_driv_register(os_driv_para_t *para)
{
    struct driv_cb  *driv = NULL;

    if((NULL == para->name)||(NULL == para->op))
    {
        goto EXIT_PARAS;
    }

    driv = osal_malloc(sizeof(struct driv_cb));
    if(NULL == driv)
    {
        goto EXIT_MALLOC;
    }
    memset(driv,0,sizeof(struct driv_cb));

    //do the member initialize
    driv->name = para->name;
    driv->op = para->op;
    driv->pri = para->pri;
    driv->flagmask = para->flag;

    //add it to the device list if no device with the same name exsit
    if(false == osal_mutex_lock(s_los_driv_module.lock))
    {
        goto EXIT_MUTEX;
    }

    if(NULL != __driv_match(para->name))
    {
        goto EXIT_EXISTED;
    }

    driv->nxt = s_los_driv_module.drivlst;
    s_los_driv_module.drivlst = driv;
    osal_mutex_unlock(s_los_driv_module.lock);

    s_los_driv_module.drivnum++;

    return driv;


EXIT_EXISTED:
    osal_mutex_unlock(s_los_driv_module.lock);
EXIT_MUTEX:
    osal_free(driv);
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
bool_t los_driv_unregister(const char *name)
{
    struct driv_cb *tmp = NULL;
    struct driv_cb *pre = NULL;
    bool_t         ret = false;
    if(NULL == name)
    {
        return ret;
    }

    if(osal_mutex_lock(s_los_driv_module.lock))
    {
        tmp = s_los_driv_module.drivlst;
        pre = NULL;
        while(NULL != tmp)
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

            osal_free(tmp);
            ret = true;

            s_los_driv_module.drivnum--;
        }

        osal_mutex_unlock(s_los_driv_module.lock);
    }

    return ret;
}

/*******************************************************************************
function     :bsp developer use this function to notify the application any event has happened
parameters   :
instruction  :not implement yet,we will do it as the asynchronize call
*******************************************************************************/
bool_t los_driv_event(los_driv_t driv,unsigned int event,void *para)
{
    return false;
}


#ifdef __CC_ARM /* ARM C Compiler ,like keil,options for linker:--keep *.o(osdriv)*/
extern unsigned int osdriv$$Base;
extern unsigned int osdriv$$Limit;
//#pragma section("osdriv", read)
#endif


static void osdriv_load_static(void){

    os_driv_para_t *para;
    unsigned int num = 0;
    int i = 0;
#if defined (__CC_ARM)    //you could add other compiler like this
    num = ((unsigned int)&osdriv$$Limit-(unsigned int)&osdriv$$Base)/sizeof(os_driv_para_t);
    para = (os_driv_para_t *) &osdriv$$Base;
#elif defined(__GNUC__)
    extern unsigned int __osdriv_start;
    extern unsigned int __osdriv_end;
    para = (os_driv_para_t *)&__osdriv_start;
    num = ((unsigned int )&__osdriv_end - (unsigned int)&__osdriv_start)/sizeof(os_driv_para_t);
#else
    #error("unknown compiler here");
#endif
    for(i =0;i<num;i++)
    {
        los_driv_register(para);
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
bool_t  los_driv_init()
{
    bool_t ret = false;

    ret = osal_mutex_create(&s_los_driv_module.lock);
    if(false == ret)
    {
        goto EXIT_MUTEX;
    }

    //load all the static device init
    osdriv_load_static();

EXIT_MUTEX:
    return ret;
}

//here we define the data structure which used by the driv application apis
struct dev_cb
{
    void *nxt;            //used by the open lst
    void *driv;           //which attached dri here
    int openflag;       //here means the open state here
    unsigned int offset;
    //the following for the debug
    unsigned int readbytes;
    unsigned int writebytes;
};

/*******************************************************************************
function     :open the device with the specified name
parameters   :
instruction  :if first open the device,then will call the init function if exsited
*******************************************************************************/
los_dev_t  los_dev_open  (const char *name,unsigned int flag)
{
    bool_t opret = true;           //operate result
    struct driv_cb *driv = NULL;   //the driver attached
    struct dev_cb  *dev = NULL;    //the device opened
    if ( NULL == name )
    {
        goto EXIT_PARAERR;
    }

    dev = osal_malloc(sizeof(struct dev_cb));
    if (NULL == dev)
    {
        goto EXIT_MEMERR;
    }
    memset(dev,0,sizeof(struct dev_cb));

    opret = osal_mutex_lock(s_los_driv_module.lock);
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
        opret = driv->op->init(driv->pri);
        if(false == opret)
        {
            driv->errno = en_dev_err_init;
            goto EXIT_INITERR;
        }
        driv->drivstatus |= cn_driv_status_initialized;
    }

    if(NULL != driv->op->open)
    {
        opret = driv->op->open(driv->pri,flag);
        if(false == opret)
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

    osal_mutex_unlock(s_los_driv_module.lock);
    return dev;

EXIT_OPENERR:
EXIT_INITERR:
EXIT_EXCLERR:
EXIT_DRIVERR:
    osal_mutex_unlock(s_los_driv_module.lock);
EXIT_MUTEXERR:
    osal_free(dev);
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
bool_t  los_dev_close  (los_dev_t dev)
{
    bool_t ret = false;
    struct dev_cb *devcb = NULL;
    struct dev_cb *tmp = NULL;
    struct driv_cb *driv = NULL;

    if ( NULL == dev )
    {
        goto EXIT_PARAERR;
    }
    devcb = dev;

    if(false == osal_mutex_lock(s_los_driv_module.lock))
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

    osal_free(dev);
    driv->opencounter--;

    osal_mutex_unlock(s_los_driv_module.lock);

    ret = true;

EXIT_DETACHERR:
EXIT_DRIVERR:
    osal_mutex_unlock(s_los_driv_module.lock);
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
ssize_t   los_dev_read  (los_dev_t dev,size_t offset, void *buf,size_t len,uint32_t timeout)
{
    ssize_t ret = 0;
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
ssize_t los_dev_write (los_dev_t dev,size_t offset,const void *buf,size_t len, uint32_t timeout)
{
    ssize_t ret = 0;
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
bool_t los_dev_ioctl (los_dev_t dev,unsigned int cmd,void *para,int paralen)
{
    bool_t ret = false;
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

/*******************************************************************************
function     :use this function to set the buffer current position,like lseek
parameters   :dev,returned by the los_dev_open function
              offset, refer to the lseek
              fromwhere, refer to the lseek
instruction  :return the current write/read position if success, else -1
*******************************************************************************/
off_t  los_dev_seek (los_dev_t dev,off_t offset,int fromwhere)
{
    off_t ret = -1;
    struct dev_cb  *devcb;
    struct driv_cb *drivcb;

    if(NULL != dev)
    {
        devcb = dev;
        drivcb = devcb->driv;
        if((NULL != drivcb->op)&&(NULL != drivcb->op->seek))
        {
            ret = drivcb->op->seek( drivcb->pri,offset,fromwhere);
        }
    }

    return ret;
}


//the following function should be implement by the vfs,
//this is very important for the multi io operation, like the select epoll and so on
//bool_t     los_dev_upara_set (los_dev_t dev,void *para);
//void*      los_dev_upara_get (los_dev_t dev);

//export some shell for the driver debug
#include <shell.h>
//use this function to show all the driver infomation
static int  __driv_show_shell(int argc,const char *argv[])
{
    struct driv_cb  *driv;

    if(osal_mutex_lock(s_los_driv_module.lock))
    {
        printf("%s:total %d drivers\n\r",__FUNCTION__,s_los_driv_module.drivnum);
        if(s_los_driv_module.drivnum != 0) //print all the driver
        {

            printf("%-16s %-8s %-8s %-8s %-8s %-8s %-8s\r\n",\
                "drivername","flagmask","status","writbyte","readbyte","open","errno");

            driv = s_los_driv_module.drivlst;
            while(NULL != driv)
            {
                printf("%-16s %08x %08x %08x %08x %08x %08x\r\n",driv->name,driv->flagmask,\
                driv->drivstatus,driv->total_write,driv->total_read,driv->opencounter,driv->errno);
                driv=driv->nxt;
            }
        }

        osal_mutex_unlock(s_los_driv_module.lock);
    }

    return 0;
}

OSSHELL_EXPORT_CMD(__driv_show_shell,"devlst","devlst");

#endif
