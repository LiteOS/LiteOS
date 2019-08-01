
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


#include <string.h>

#include <osal_imp.h>
#include <osal.h>

static const tag_os *s_os_cb = NULL;

int osal_install(const tag_os *os)
{
    int ret = -1;

    if(NULL == s_os_cb)
    {
        s_os_cb = os;
        ret = 0;
    }

    return ret;
}


void* osal_task_create(const char *name,int (*task_entry)(void *args),\
                      void *args,int stack_size,void *stack,int prior)
{
    void *ret = NULL;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->task_create))
    {
        ret = s_os_cb->ops->task_create(name, task_entry,args,stack_size,stack,prior);
    }

    return ret;
}


int osal_task_kill(void *task)
{
    int ret = -1;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->task_kill))
    {
        ret = s_os_cb->ops->task_kill(task);
    }

    return ret;

}


void osal_task_exit()
{

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->task_exit))
    {
        s_os_cb->ops->task_exit();
    }
    return ;

}


void osal_task_sleep(int ms)
{
    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->task_sleep))
    {
        s_os_cb->ops->task_sleep(ms);
    }

    return ;

}


bool_t  osal_mutex_create(osal_mutex_t *mutex)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->mutex_create))
    {
        ret = s_os_cb->ops->mutex_create(mutex);
    }

    return ret;

}


bool_t  osal_mutex_lock(osal_mutex_t mutex)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->mutex_lock))
    {
        ret = s_os_cb->ops->mutex_lock(mutex);
    }

    return ret;

}


bool_t  osal_mutex_unlock(osal_mutex_t mutex)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->mutex_unlock))
    {
        ret = s_os_cb->ops->mutex_unlock(mutex);
    }

    return ret;

}

bool_t  osal_mutex_del(osal_mutex_t mutex)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->mutex_del))
    {
        ret = s_os_cb->ops->mutex_del(mutex);
    }

    return ret;

}

bool_t  osal_semp_create(osal_semp_t *semp,int limit,int initvalue)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->semp_create))
    {
        ret = s_os_cb->ops->semp_create(semp,limit,initvalue);
    }

    return ret;

}

bool_t  osal_semp_pend(osal_semp_t semp,int timeout)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->semp_pend))
    {
        ret = s_os_cb->ops->semp_pend(semp,timeout);
    }

    return ret;

}


bool_t  osal_semp_post(osal_semp_t semp)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->semp_post))
    {
        ret = s_os_cb->ops->semp_post(semp);
    }

    return ret;

}

bool_t  osal_semp_del(osal_semp_t semp)
{
    bool_t ret = false;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->semp_del))
    {
        ret = s_os_cb->ops->semp_del(semp);
    }

    return ret;

}


void *osal_malloc(size_t size)
{
    void *ret = NULL;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->malloc))
    {
        ret = s_os_cb->ops->malloc(size);
    }

    return ret;

}

void  osal_free(void *addr)
{

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->free))
    {
        s_os_cb->ops->free(addr);
    }

    return;
}

void *osal_zalloc(size_t size)
{
    void *ret = NULL;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->malloc))
    {
        ret = s_os_cb->ops->malloc(size);
        if(NULL != ret)
        {
            memset(ret,0,size);
        }
    }

    return ret;

}

void *osal_realloc(void *ptr,size_t newsize)
{
    void *ret = NULL;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->realloc))
    {
        ret = s_os_cb->ops->realloc(ptr,newsize);
    }

    return ret;
}

void *osal_calloc(size_t n, size_t size)
{
    void *p = osal_malloc(n * size);
    if(NULL != p)
    {
        memset(p, 0, n * size);
    }

    return p;
}



unsigned long long osal_sys_time()
{

    unsigned long long  ret = 0;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->get_sys_time))
    {
        ret = s_os_cb->ops->get_sys_time();
    }

    return ret;
}

void osal_loop_timer_init(osal_loop_timer_t *timer)
{
    timer->dead_time = osal_sys_time();
}

char osal_loop_timer_expired(osal_loop_timer_t *timer)
{
    unsigned long long now = osal_sys_time();
    return now >= timer->dead_time;
}

void osal_loop_timer_count_downms(osal_loop_timer_t *timer, unsigned int timeout)
{
    unsigned long long now = osal_sys_time();
    timer->dead_time = now + timeout;
}


void osal_loop_timer_count_down(osal_loop_timer_t *timer, unsigned int timeout)
{
    osal_loop_timer_count_downms(timer,timeout*1000);
}

int osal_loop_timer_left(osal_loop_timer_t *timer)
{
    unsigned long long now = osal_sys_time();
    return timer->dead_time <= now ? 0 : timer->dead_time - now;
}


#include <stdarg.h>
#define LOG_BUF_SIZE  256  ///< you could modify it

int osal_printf(const char *format, ...)
{
    int ret;
    char str_buf[LOG_BUF_SIZE] = {0};
    va_list list;

    memset(str_buf, 0, LOG_BUF_SIZE);
    va_start(list, format);
    ret = vsnprintf(str_buf, LOG_BUF_SIZE, format, list);
    va_end(list);

    printf("%s", str_buf);

    return ret;

}

int osal_reboot()  ///< maybe we should never come back
{
    int ret = -1;

    if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->reboot))
    {
        ret = s_os_cb->ops->reboot();
    }

    return ret;
}



int osal_int_connect(int intnum, int prio, int mode, fn_interrupt_handle callback, void *arg)
{
	int ret = -1;
	if((NULL != s_os_cb) &&(NULL != s_os_cb->ops) &&(NULL != s_os_cb->ops->int_connect))
    {
        ret = s_os_cb->ops->int_connect(intnum, prio, mode, callback, arg);
    }

	return ret;
}

int osal_init(void)
{
    return 0;
}

