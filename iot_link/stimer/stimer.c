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
 *  2019-07-26 18:04  zhangqianfu  The first version
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#if CONFIG_STIMER_ENABLE


#include <osal.h>
#include <stimer.h>

///< we use the normal list to manage the timer, next time you could use the RB
///< tree or something else to do some optimization,thanks for your supporting

typedef struct timer_item
{
    const char         *name;        ///< verify the timer, will be used in the debug
    uint32_t            cycle;       ///< timer cycle
    uint32_t            flag;        ///< timer flag
    fn_stimer_handler   handler;     ///< timer handler
    void               *args;        ///< timer handler parameter
    int64_t             dead_time;   ///< it is the dead time
    struct timer_item  *nxt;         ///< used for the timer list
}stimer_item_t;  ///< each timer will have this structure

typedef struct
{
    osal_semp_t    semp;       ///< used for time wait sync
    osal_mutex_t   mutex;      ///< used for protect the timer list
    stimer_item_t *lst;        ///< this is the soft timer list，
    void          *task;       ///< this is the task engine
}stimer_cb_t;

static stimer_cb_t s_stimer_cb =  \
{
    .semp  = cn_semp_invalid,
    .mutex = cn_mutex_invalid,
    .lst   = NULL,
    .task  = NULL,
};


#define cn_stimer_wait_max  (cn_osal_timeout_forever)    ///< almost the max time
#define cn_stimer_dead_max  (0x7fffffffffffffff)         ///< maybe the int64_t max time
static void timer_remove(stimer_item_t *item)
{
    stimer_item_t  *tmp;
    if(item == s_stimer_cb.lst)
    {
        s_stimer_cb.lst = item->nxt ;
    }
    else
    {
        tmp = s_stimer_cb.lst;
        while(NULL != tmp)
        {
            if(tmp->nxt == item)
            {
                tmp->nxt = item->nxt;
                break;
            }
            tmp = tmp->nxt;
        }
    }
    item->nxt = NULL;
}
///< add the timer to the queue,by dead time
static void timer_add(stimer_item_t *timer)
{
    int64_t        dead_time;
    stimer_item_t  *tmp;  ///< find a timer,who's dead time is less and the following one is equal or bigger

    tmp = s_stimer_cb.lst;

    dead_time = timer->dead_time;

    while(NULL != tmp)
    {
        if(tmp->dead_time >= dead_time)   ///< all dead time is bigger
        {
            tmp = NULL;
            break;
        }
        else if((NULL ==tmp->nxt) ||(tmp->nxt->dead_time >= dead_time))  ///< find one
        {
            break;
        }
        else ///< continue to find
        {
            tmp = tmp->nxt;
        }
    }
    if(NULL == tmp)
    {
        if(NULL != s_stimer_cb.lst)
        {
            timer->nxt = s_stimer_cb.lst;
        }
        s_stimer_cb.lst = timer;
    }
    else
    {
        timer->nxt = tmp->nxt;
        tmp->nxt = timer;
    }

    return;
}

///< scan the all the list, take it easy
static void  timer_scan(void)
{
    stimer_item_t *item;
    int64_t       cur_time;

    while(NULL != s_stimer_cb.lst)
    {
        item = s_stimer_cb.lst;
        s_stimer_cb.lst = item->nxt;
        item->nxt = NULL;

        cur_time = osal_sys_time();
        ///< check if is timeout or not
        if(item->dead_time <= cur_time)
        {
            if(NULL != item->handler)
            {
                item->handler(item->args);
            }
            if(item->flag & cn_stimer_flag_once)
            {
                item->flag &= (~cn_stimer_flag_start);
                item->dead_time = cn_stimer_dead_max;
            }
            else
            {
                item->dead_time = cur_time + item->cycle;
            }

            timer_add(item);
        }
        else
        {
            timer_add(item);
            break;
        }
    }

}

///< this is the soft timer main engine
static int __timer_entry(void *args)
{
    uint32_t wait_time;
    int64_t cur_time;

    wait_time= cn_stimer_wait_max;
    while(1)
    {
        osal_semp_pend(s_stimer_cb.semp,wait_time);  ///< if any timer exist before the task, then the semphore is active
        ///< check all the list and do it
        if(true == osal_mutex_lock(s_stimer_cb.mutex))
        {
            timer_scan();
            osal_mutex_unlock(s_stimer_cb.mutex);
        }

        ///<compute all the time to wait;
        cur_time = osal_sys_time();
        if(true == osal_mutex_lock(s_stimer_cb.mutex))
        {
            if(( NULL==s_stimer_cb.lst) ||(cn_stimer_dead_max == s_stimer_cb.lst->dead_time))
            {
                wait_time = cn_stimer_wait_max;
            }
            else
            {
                wait_time = s_stimer_cb.lst->dead_time > cur_time?(s_stimer_cb.lst->dead_time - cur_time):0;
                if(wait_time > cn_stimer_wait_max)
                {
                    wait_time = cn_stimer_wait_max;
                }
            }

            osal_mutex_unlock(s_stimer_cb.mutex);
        }
    }
    return 0;
}

/**
 * @brief: this function used for initialize the timer component, should be called after the os
 *
 * */
int32_t stimer_init()
{
    int32_t ret =-1;

    if(false == osal_semp_create(&s_stimer_cb.semp,1,0))
    {
        goto EXIT_SEMPERR;
    }
    if(false == osal_mutex_create(&s_stimer_cb.mutex))
    {
        goto EXIT_MUTEXERR;
    }

    s_stimer_cb.task = osal_task_create("soft timer",__timer_entry,NULL,0x800,NULL,10);

    if(NULL == s_stimer_cb.task)
    {
        goto EXIT_TASKERR;
    }

    ret = 0;
    return ret;

EXIT_TASKERR:
    osal_mutex_del(s_stimer_cb.mutex);
    s_stimer_cb.mutex = cn_mutex_invalid;
EXIT_MUTEXERR:
    osal_semp_del(s_stimer_cb.semp);
    s_stimer_cb.semp = cn_semp_invalid;
EXIT_SEMPERR:
    return ret;
}

stimer_t stimer_create(const char *name,fn_stimer_handler handler, void *arg,uint32_t cycle,uint32_t flag)
{
    stimer_item_t *item;

    item  = osal_malloc(sizeof(stimer_item_t));
    if(NULL == item)
    {
        return item;
    }
    memset(item,0,sizeof(stimer_item_t));
    item->name = name;
    item->cycle = cycle;
    item->flag = flag;
    item->handler = handler;
    item->args = arg;

    if(item->flag & cn_stimer_flag_start)
    {
        item->dead_time  = osal_sys_time() + item->cycle;
    }
    else
    {
        item->dead_time = cn_stimer_dead_max;
    }

    if(true == osal_mutex_lock(s_stimer_cb.mutex))
    {
        timer_add(item);
        osal_mutex_unlock(s_stimer_cb.mutex);
        osal_semp_post(s_stimer_cb.semp);
    }
    else
    {
        osal_free(item);
        item = NULL;
    }

    return item;
}

int32_t stimer_delete(stimer_t timer)
{
    int32_t ret = -1;

    if(true == osal_mutex_lock(s_stimer_cb.mutex))
    {
        timer_remove(timer);
        osal_mutex_unlock(s_stimer_cb.mutex);
        osal_free(timer);
        osal_semp_post(s_stimer_cb.semp);
        ret = 0;
    }

    return ret;
}

int32_t stimer_ioctl(stimer_t timer,en_stimer_opt_t opt, void *arg)
{
    int32_t ret = -1;

    stimer_item_t   *item;
    if(NULL == timer)
    {
        return ret;
    }

    item = timer;
    if(true == osal_mutex_lock(s_stimer_cb.mutex))
    {
        switch(opt)
        {
            case en_stimer_opt_gettime:
                if(NULL != arg)
                {
                    *(uint32_t *)arg = item->dead_time- osal_sys_time();
                    ret = 0;
                }
                break;
            case en_stimer_opt_stop:
                timer_remove(item);
                item->nxt = NULL;
                item->flag &= (~cn_stimer_flag_start);
                item->dead_time = cn_stimer_dead_max;
                timer_add(item);
                osal_semp_post(s_stimer_cb.semp);
                ret = 0;
                break;
            case en_stimer_opt_start:
                timer_remove(item);
                item->nxt = NULL;
                item->flag |= cn_stimer_flag_start;
                item->dead_time = item->cycle + osal_sys_time();
                timer_add(item);
                osal_semp_post(s_stimer_cb.semp);
                ret = 0;
                break;
            case en_stimer_opt_recycle:
                timer_remove(item);
                item->nxt = NULL;
                item->cycle = *(uint32_t *)arg;
                if(item->flag & cn_stimer_flag_start)
                {
                    item->dead_time = item->cycle + osal_sys_time();
                }
                timer_add(item);
                osal_semp_post(s_stimer_cb.semp);
                ret = 0;
                break;
            default:
                ret = -1;

        }
        osal_mutex_unlock(s_stimer_cb.mutex);
    }

    return ret;

}


#include <shell.h>
static int32_t stimer_print(int32_t argc, const char *argv[])
{
    int       timer_number = 0;
    stimer_item_t  *item = NULL;

    if(true == osal_mutex_lock(s_stimer_cb.mutex))
    {
        printf("%-8s %-8s %-8s %-8s %-5s %-5s %s\n\r",\
                "TimerNo","Cycle","Handler","Arg","Start","Once","DeadTime");

        item = s_stimer_cb.lst;
        while(NULL != item)
        {
            printf("%-8d %08x %08x %08x %-5s %-5s %x\n\r",\
                    timer_number++,(unsigned int)item->cycle,(unsigned int)item->handler,(unsigned int)item->args,\
                    item->flag&cn_stimer_flag_start?"Yes":"No",item->flag&cn_stimer_flag_once?"Yes":"No",\
                    (unsigned int)item->dead_time);
            item = item->nxt;
        }
        printf("Total:%d Soft timers\n\r",timer_number);

        osal_mutex_unlock(s_stimer_cb.mutex);
    }

    return 0;
}
OSSHELL_EXPORT_CMD(stimer_print,"stimer","stimer");

#endif

