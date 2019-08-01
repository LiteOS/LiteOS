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
 *  2019-04-28 11:04  zhangqianfu  The first version
 *  2019-05-23 09:53  huerjia      The second version
 */
#include  <osal_imp.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>


///< the macos has not implement the sem_init and sem_timedwait, so we must do
///< something instead of it


typedef void *(*pthread_entry) (void *);

static void __task_sleep(int ms)
{
    usleep(ms*1000);
}

static void *__task_create(const char *name,int (*task_entry)(void *args),\
        void *args,int stack_size,void *stack,int prior)
{
    void *ret = NULL;
    pthread_t pid;
    printf("task create name:%s\r\n", name);
    if(pthread_create(&pid, NULL, (pthread_entry)task_entry, args))
        return ret;

    ret = (void *)pid;
    return ret;
}

static int __task_kill(void *task)
{
    pthread_t pid;

    pid = (pthread_t)task;
    return pthread_cancel(pid);
}

static void __task_exit()
{
    pthread_exit(NULL);
}

///< this is implement for the mutex
//creat a mutex for the os
static bool_t  __mutex_create(osal_mutex_t *mutex)
{
    pthread_mutex_t *m;
    m = malloc(sizeof(pthread_mutex_t));
    *mutex = m;
    if(!pthread_mutex_init((pthread_mutex_t *)m,NULL))
    {
        return true;
    }

    return false;
}

//lock the mutex
static bool_t  __mutex_lock(osal_mutex_t mutex)
{
    int ret ;
    ret = pthread_mutex_lock((pthread_mutex_t *)mutex);
    if(!ret)
    {
        return true;
    }

    return false;
}

//unlock the mutex
static bool_t  __mutex_unlock(osal_mutex_t mutex)
{
    if(!pthread_mutex_unlock((pthread_mutex_t *)mutex))
    {
        usleep(1000);
        return true;
    }

    return false;
}
//delete the mutex
static bool_t  __mutex_del(osal_mutex_t mutex)
{
    if(!pthread_mutex_destroy((pthread_mutex_t *)mutex))
    {
        free(mutex);
        return true;
    }

    return false;
}


///< this is implement for the semp
//semp of the os
#include <errno.h>

//static bool_t  __semp_create(osal_semp_t *semp,int limit,int initvalue)
//{
//    int ret = -1;
//
//    sem_t *s;
//    s = malloc(sizeof(sem_t));
//
//    ret = sem_init(s, 1, initvalue);
//    if(ret == 0)
//    {
//        *semp = s;
//        return true;
//    }
//    else
//    {
//        free(s);
//        if(errno == EINVAL)
//        {
//            printf("%s:invalid value \n\r",__FUNCTION__);
//        }
//        else if(errno == ENOSYS)
//        {
//            printf("%s:no share semp yet \n\r",__FUNCTION__);
//        }
//        else
//        {
//            printf("%s:unknown reason:%d\n\r",__FUNCTION__,errno);
//        }
//
//        return false;
//    }
//}
//
//static bool_t  __semp_del(osal_semp_t semp)
//{
//    if(sem_destroy((sem_t *)semp))
//        return false;
//    else
//        return true;
////}
//
//static bool_t  __semp_pend(osal_semp_t semp,int timeout)
//{
//    bool_t ret = false;
//
//    if(timeout == cn_osal_timeout_forever)
//    {
//        if(0 == sem_wait((sem_t *)semp))
//        {
//            ret = true;
//        }
//    }
//    else
//    {
//        while(timeout >0)
//        {
//            timeout--;
//            usleep(1000);
//            if(0 == sem_trywait((sem_t *)semp))
//            {
//                ret = true;
//                break;
//            }
//        }
//    }
//
//    return ret;
//}
//
//static bool_t  __semp_post(osal_semp_t semp)
//{
//    if(sem_post((sem_t *)semp))
//    {
//        return false;
//    }
//    else
//    {
//        return true;
//    }
//}
///< for the macos could not use the unnamed sem,so we use time as the name for semaphore

typedef struct
{
    sem_t *sem;
    char  *time;
}macos_sem_t;

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static bool_t  __semp_create(osal_semp_t *semp,int limit,int initvalue)
{
    time_t       timenow;
    macos_sem_t *macos_sem;

    sleep(2);
    macos_sem = malloc(sizeof(macos_sem_t));
    if(NULL != macos_sem)
    {
        time(&timenow);
        macos_sem->time = ctime(&timenow);
        printf("%s:%s\n\r",__FUNCTION__,macos_sem->time);
        macos_sem->sem = sem_open(macos_sem->time,O_CREAT, S_IRUSR | S_IWUSR, initvalue);

        printf("%s:address:0x%08x\n\r",__FUNCTION__,(unsigned int)macos_sem->sem);

        if(NULL == macos_sem->sem)
        {
            free(macos_sem->time);
            free(macos_sem);
            return false;
        }
        else
        {
            *semp = macos_sem;
            return true;
        }
    }
    else
    {
        return false;
    }
}

static bool_t  __semp_del(osal_semp_t semp)
{
    macos_sem_t *macos_sem = semp;

    if(NULL != macos_sem)
    {
        sem_close(macos_sem->sem);
        free(macos_sem->time);
        free(macos_sem);
        return true;
    }
    else
    {
        return false;
    }
}


static bool_t  __semp_pend(osal_semp_t semp,int timeout)
{
    bool_t ret = false;
    macos_sem_t *macos_sem = semp;
    if(NULL == macos_sem)
    {
        return ret;
    }

    if(timeout == cn_osal_timeout_forever)
    {
        if(0 == sem_wait(macos_sem->sem))
        {
            ret = true;
        }
    }
    else
    {
        while(timeout >0)
        {
            timeout--;
            usleep(1000);
            if(0 == sem_trywait(macos_sem->sem))
            {
                ret = true;
                break;
            }
        }
    }

    return ret;
}

static bool_t  __semp_post(osal_semp_t semp)
{
    macos_sem_t *macos_sem = semp;
    if(NULL == macos_sem)
    {
        return false;
    }


    if(sem_post(macos_sem->sem))
    {
        return false;
    }
    else
    {
        return true;
    }
}



static void *__mem_malloc(int size)
{
    void *ret = NULL;

    if(size > 0)
    {
         ret = malloc(size);
    }

    return ret;
}

static void __mem_free(void *addr)
{
    free(addr);
}

///< sys time
#include <sys/time.h>
static unsigned long long __get_sys_time()
{
    struct    timeval    tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


static const tag_os_ops s_macos_ops =
{
    .task_sleep = __task_sleep,
    .task_create = __task_create,
    .task_kill = __task_kill,
    .task_exit = __task_exit,

    .mutex_create = __mutex_create,
    .mutex_lock = __mutex_lock,
    .mutex_unlock = __mutex_unlock,
    .mutex_del = __mutex_del,

    .semp_create = __semp_create,
    .semp_pend = __semp_pend,
    .semp_post = __semp_post,
    .semp_del = __semp_del,

    .malloc = __mem_malloc,
    .free = __mem_free,

    .get_sys_time = __get_sys_time,
};

static const tag_os s_link_macos =
{
    .name = "Linux",
    .ops = &s_macos_ops,
};

int osal_install_macos(void)
{
    int ret = -1;

    ret = osal_install(&s_link_macos);

    return ret;
}
