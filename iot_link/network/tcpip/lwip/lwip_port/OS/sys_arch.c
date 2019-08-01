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
 *  2019-05-10 17:26  zhangqianfu  The first version  
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <lwip/sys.h>
#include <lwip/stats.h>

#include <sys_arch.h>

#include <osal.h>

extern void LOS_TaskLock();
extern void LOS_TaskUnlock();

/*lint -e**/
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/

err_t sys_mbox_new(struct sys_mbox **mb, int size)
{
    struct sys_mbox *mbox;

    if (size <= 0)
    {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_new: mbox size must bigger than 0\n"));
        return ERR_MEM;
    }

    mbox = (struct sys_mbox *)osal_malloc(sizeof(struct sys_mbox));

    if (mbox == NULL)
    {
        goto err_handler;
    }

    (void)memset(mbox, 0, sizeof(struct sys_mbox)); //CSEC_FIX_2302

    mbox->msgs = (void **)osal_malloc(sizeof(void *) * size);

    if (mbox->msgs == NULL)
    {
        goto err_handler;
    }

    (void)memset(mbox->msgs, 0, (sizeof(void *) * size)); //CSEC_FIX_2302

    mbox->mbox_size = size;

    mbox->first = 0;
    mbox->last = 0;
    mbox->isFull = 0;
    mbox->isEmpty = 1;

    mbox->mutex = cn_mutex_invalid;
    mbox->not_empty = cn_semp_invalid;
    mbox->not_full = cn_semp_invalid;

    if(false == osal_mutex_create(&mbox->mutex))
    {
        mbox->mutex = cn_mutex_invalid;
        goto err_handler;
    }

    if(false == osal_semp_create(&(mbox->not_empty),size,0)) //which means could be read
    {
        mbox->not_empty = cn_semp_invalid;
        goto err_handler;
    }

    if(false == osal_semp_create(&(mbox->not_full),size,size))  ///which means could be write
    {
        mbox->not_full = cn_semp_invalid;
        goto err_handler;
    }

    SYS_STATS_INC_USED(mbox);
    *mb = mbox;
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_new: mbox created successfully 0x%p\n", (void *)mbox));
    return ERR_OK;

err_handler:

    if (mbox != NULL)
    {
        if (mbox->msgs != NULL)
        {
            osal_free(mbox->msgs);
        }

        osal_free(mbox);
    }

    if (mbox->mutex != cn_mutex_invalid)
    {
        (void)osal_mutex_del(mbox->mutex);
    }

    if (mbox->not_empty != cn_semp_invalid)
    {
        (void)osal_semp_del(mbox->not_empty);
    }

    if (mbox->not_full != cn_semp_invalid)
    {
        osal_semp_del(mbox->not_full);
        (void)osal_semp_del(mbox->not_full);
    }

    return ERR_MEM;
}

/*-----------------------------------------------------------------------------------*/
void
sys_mbox_free(struct sys_mbox **mb)
{
    if ((mb != NULL) && (*mb != SYS_MBOX_NULL))
    {
        struct sys_mbox *mbox = *mb;
        SYS_STATS_DEC(mbox);

        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_free: going to free mbox 0x%p\n", (void *)mbox));

        (void)osal_mutex_lock(mbox->mutex);

        (void)osal_semp_del(mbox->not_empty);
        (void)osal_semp_del(mbox->not_full);

        (void)osal_mutex_unlock(mbox->mutex);
        (void)osal_mutex_del(mbox->mutex);

        osal_free(mbox->msgs);
        osal_free(mbox);
        *mb = NULL;

        LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_free: freed mbox\n"));
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/

void  sys_mbox_post(struct sys_mbox **mb, void *msg)   ///< check if it is empty or not
{
    struct sys_mbox *mbox;
    mbox = *mb;
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox 0x%p msg 0x%p\n", (void *)mbox, (void *)msg));


    if(osal_semp_pend(mbox->not_full,cn_osal_timeout_forever))
    {
        if(osal_mutex_lock(mbox->mutex))
        {
            mbox->msgs[mbox->last] = msg;

            mbox->last = (mbox->last + 1) % mbox->mbox_size;

            if (mbox->first == mbox->last)
            {
                mbox->isFull = 1;
            }

            osal_mutex_unlock(mbox->mutex);
            osal_semp_post(mbox->not_empty);
        }
    }

    return;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(struct sys_mbox **mb, void *msg)
{
    err_t ret = ERR_MEM;
    struct sys_mbox *mbox;
    mbox = *mb;
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox 0x%p msg 0x%p\n", (void *)mbox, (void *)msg));


    if(osal_semp_pend(mbox->not_full,0))
    {
        if(osal_mutex_lock(mbox->mutex))
        {
            mbox->msgs[mbox->last] = msg;

            mbox->last = (mbox->last + 1) % mbox->mbox_size;

            if (mbox->first == mbox->last)
            {
                mbox->isFull = 1;
            }

            osal_mutex_unlock(mbox->mutex);
            osal_semp_post(mbox->not_empty);
            ret = ERR_OK;
        }
    }

    return ret;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t
sys_arch_mbox_fetch(struct sys_mbox **mb, void **msg, u32_t timeout)
{
    u32_t time_needed = 0;
    struct sys_mbox *mbox;
    unsigned long long time_start;
    unsigned long long time_end;
    int pend_time ;

    mbox = *mb;
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch: mbox 0x%p msg 0x%p\n", (void *)mbox, (void *)msg));

    if((NULL == mb) ||(NULL == *mb)||(NULL == msg))
    {
        return time_needed;
    }

    if(timeout == 0)
    {
        pend_time = cn_osal_timeout_forever;
    }
    else
    {
        pend_time = timeout;
    }
    time_start = osal_sys_time();

    time_needed = SYS_ARCH_TIMEOUT;
    if(osal_semp_pend(mbox->not_empty,pend_time))
    {
        if(osal_mutex_lock(mbox->mutex))
        {

            *msg = mbox->msgs[mbox->first];
            mbox->first = (mbox->first + 1) % mbox->mbox_size;

            if (mbox->first == mbox->last)
            {
                mbox->isEmpty = 1;
            }

            if (mbox->isFull)
            {
                mbox->isFull = 0;
            }
            osal_mutex_unlock(mbox->mutex);
            osal_semp_post(mbox->not_full);
            time_end = osal_sys_time();

            time_needed = time_end - time_start;
            if(time_needed == 0)
            {
                time_needed =1; ///< could not be zero, else the caller will think it failed
            }
        }

    }

    return time_needed;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
void sys_init(void)
{

}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      OS.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect(void)
{
    LOS_TaskLock();
    return 0;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an OS.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void
sys_arch_unprotect(sys_prot_t pval)
{
    LWIP_UNUSED_ARG(pval);
    LOS_TaskUnlock();
}

u32_t sys_now(void)
{
    /* Lwip docs mentioned like wraparound is not a problem in this funtion */
    return (u32_t)(osal_sys_time());
}
sys_thread_t  sys_thread_new(char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{

    void *handle;

    handle = osal_task_create(name,(int (*)(void *args))function,arg,stacksize,NULL,prio);
    return (sys_thread_t)handle;
}

#ifdef LWIP_DEBUG

/** \brief  Displays an error message on assertion

    This function will display an error message on an assertion
    to the dbg output.

    \param[in]    msg   Error message to display
    \param[in]    line  Line number in file with error
    \param[in]    file  Filename with error
 */
void assert_printf(char *msg, int line, char *file)
{
    if (msg)
    {
        LWIP_DEBUGF(LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
                    ("%s:%d in file %s", msg, line, file));
        return;
    }
    else
    {
        LWIP_DEBUGF( LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
                     ("LWIP ASSERT"));
        return;
    }
}
#endif /* LWIP_DEBUG */


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem,  u8_t count)
{

    err_t ret = ERR_MEM;
    if (NULL == sem)
    {
        return ERR_ARG;
    }

    LWIP_ASSERT("in sys_sem_new count exceeds the limit", (count < 0xFF));

    if(osal_semp_create(sem,1,count))
    {
        ret = ERR_OK;
    }
    return ret;

}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    uint32_t ret = 0;
    uint64_t time_start;
    uint64_t time_end;

    int pend_time;

    if (!sem)
    {
        return ret;
    }

    if(0 == timeout)
    {
        pend_time = cn_osal_timeout_forever;
    }
    else
    {
        pend_time = timeout;
    }

    time_start = osal_sys_time();
    if(osal_semp_pend(*sem,pend_time))
    {
        time_end = osal_sys_time();
        ret = time_end - time_start;
    }
    else
    {
        ret = SYS_ARCH_TIMEOUT;
    }

    if(ret == 0)
    {
        ret = 1; ///< could not be zeror, if not, the caller think it failed
    }

    return ret;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *sem)
{
    if(NULL != sem)
    {
        osal_semp_post(*sem);
    }
    return;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/

void sys_sem_free(sys_sem_t *sem)
{
    if(NULL != sem)
    {
        osal_semp_del(*sem);
    }

    return;
}


/**
 * @ingroup sys_mutex
 * Create a new mutex.
 * Note that mutexes are expected to not be taken recursively by the lwIP code,
 * so both implementation types (recursive or non-recursive) should work.
 * @param mutex pointer to the mutex to create
 * @return ERR_OK if successful, another err_t otherwise
 */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    err_t ret = ERR_ARG;
    if(NULL == mutex)
    {
        return ret;
    }

    if(osal_mutex_create(mutex))
    {
        ret = ERR_OK;
    }
    else
    {
        ret = ERR_MEM;
    }
    return ret;
}
/**
 * @ingroup sys_mutex
 * Lock a mutex
 * @param mutex the mutex to lock
 */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    if(NULL != mutex)
    {
        osal_mutex_lock(*mutex);
    }
    return;
}
/**
 * @ingroup sys_mutex
 * Unlock a mutex
 * @param mutex the mutex to unlock
 */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    if(NULL != mutex)
    {
        osal_mutex_unlock(*mutex);
    }
    return;
}
/**
 * @ingroup sys_mutex
 * Delete a semaphore
 * @param mutex the mutex to delete
 */
void sys_mutex_free(sys_mutex_t *mutex)
{
    if(NULL != mutex)
    {
        osal_mutex_del(*mutex);
    }
    return;
}
