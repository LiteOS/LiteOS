

#ifndef __OS_AL_H
#define __OS_AL_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <link_misc.h>
#include <osal_types.h>

//we need nothing here except the standard struct here
//kernel port here
//create a task:return the task handle here while -1 means create failed

/**
 * @brief:use this function to create a task here
 *
 * @param[in]:name, task name
 * @param[in]:task_entry, the task entry function
 * @param[in]:stack_size,the task stack size
 * @param[in]:stack, the stack memory, if NULL, which needed malloc by the system
 * @param[in]:args,the parameter pass to the entry function
 * @param[in]:prior,the prior if supported for the task;the bigger value, the lower prior
 *
 * @return:the task handle, while NULL failed
 *
 * */
void* osal_task_create(const char *name,int (*task_entry)(void *args),\
                      void *args,int stack_size,void *stack,int prior);

/**
 * @brief:use this funciton to kill the task
 *
 * @param[in]:task, the object get from task_create
 *
 * @return:0 success while false failed
 * */
int osal_task_kill(void *task);

/**
 * @brief:use this function  to exit from the current task
 *
 **/
void osal_task_exit();

/**
 * @brief:use this function to make the task to sleep
 *
 * @param[in]:ms, how much time to sleep, unit:ms
 *
 * @return:0 success while -1 failed
 **/
void osal_task_sleep(int ms);

/**
 *@brief: the mutex that the os must supplied for the link
 *
 **/

bool_t  osal_mutex_create(osal_mutex_t *mutex);
bool_t  osal_mutex_lock(osal_mutex_t mutex);
bool_t  osal_mutex_unlock(osal_mutex_t mutex);
bool_t  osal_mutex_del(osal_mutex_t mutex);

/**
 *@brief: the semp method that the os must supplied for the link
 *
 **/
bool_t  osal_semp_create(osal_semp_t *semp,int limit,int initvalue);
bool_t  osal_semp_pend(osal_semp_t semp,int timeout);
bool_t  osal_semp_post(osal_semp_t semp);
bool_t  osal_semp_del(osal_semp_t  semp);


/**
 *@brief: the memory method that the os must supplied for the link
 *
 **/
void *osal_malloc(size_t size);
void  osal_free(void *addr);
void *osal_zalloc(size_t size);
void *osal_realloc(void *ptr,size_t newsize);
void *osal_calloc(size_t n, size_t size);


/**
 * @brief: use this function to get the system time
 *
 * @return:system time (unit ms)
 * */
unsigned long long osal_sys_time();

/**
 *
 * @brief: the loop soft timer
 * @param[in] : timer, the loop timer to be initialized
 *
 * */
void osal_loop_timer_init(osal_loop_timer_t *timer);

/**
 *
 * @brief: the loop soft timer
 * @param[in] : timer, the loop timer
 *
 * @return: true expired while false not expired
 *
 * */
char osal_loop_timer_expired(osal_loop_timer_t *timer);

/**
 *
 * @brief: the loop soft timer
 * @param[in]: timer, the loop timer
 * @param[in]: timeout, how many time to be expired,unit:second
 *
 * */
void osal_loop_timer_count_down(osal_loop_timer_t *timer, unsigned int timeout);
/**
 *
 * @brief: the loop soft timer
 * @param[in]: timer, the loop timer
 * @param[in]: timeout, how many time to be expired,unit:ms
 *
 * */
void osal_loop_timer_count_downms(osal_loop_timer_t *timer, unsigned int timeout);

/**
 *
 * @brief: the loop soft timer
 * @param[in]: timer, the loop timer
 * @return: how many ms to be expired
 * */
int osal_loop_timer_left(osal_loop_timer_t *timer);


/**
 * @brief: you could use this as the main printf for your own log
 *
 *       :the input parameters is just as the printf does
 * */
int osal_printf(const char *format, ...);

/**
 * @brief:  you could use this function to do the system reboot
 *
 *
 * */
int osal_reboot(void);



int osal_int_connect(int intnum, int prio, int mode, fn_interrupt_handle callback, void *arg);

/**
 * @brief:use this function to initialize the os abstract layer
 *
 * @return:0 success while -1 failed
 * */
int osal_init(void);


#endif /* __OS_AL_H */
