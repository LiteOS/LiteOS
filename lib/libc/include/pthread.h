/**
 * @defgroup posix POSIX
 * @defgroup pthread Thread
 * @ingroup posix
 */

#ifndef _PTHREAD_H
#define _PTHREAD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "los_list.h"
#include "los_task_pri.h"
#include "bits/pthread_types.h"
#include "los_mux_pri.h"
#include <features.h>

#define __NEED_time_t
#define __NEED_clockid_t
#define __NEED_struct_timespec
#define __NEED_sigset_t
#define __NEED_pthread_t
#define __NEED_pthread_attr_t
#define __NEED_pthread_mutexattr_t
#define __NEED_pthread_condattr_t
#define __NEED_pthread_rwlockattr_t
#define __NEED_pthread_barrierattr_t
#define __NEED_pthread_mutex_t
#define __NEED_pthread_cond_t
#define __NEED_pthread_rwlock_t
#define __NEED_pthread_barrier_t
#define __NEED_pthread_spinlock_t
#define __NEED_pthread_key_t
#define __NEED_pthread_once_t
#define __NEED_size_t

#include <bits/alltypes.h>

#include <sched.h>
#include <time.h>

/**
 * @ingroup pthread
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_PROTECT.
 */

/**
 * @ingroup pthread
 * Mutex attributes object
 */
typedef struct pthread_mutexattr {
	UINT8 protocol;
	UINT8 prioceiling;
	UINT8 type;
	UINT8 reserved;
} pthread_mutexattr_t;

/**
 * @ingroup pthread
 * Mutex object
 */
typedef struct pthread_mutex {
	pthread_mutexattr_t stAttr;
	MuxBaseCB stLock;
} pthread_mutex_t;

typedef struct {
	clockid_t clock;
} pthread_condattr_t;

/**
 * @ingroup pthread
 * Condition variable structure.
 */
typedef struct pthread_cond {
	pthread_condattr_t stCondAttr;
	volatile int count;
	EVENT_CB_S event;
	pthread_mutex_t* mutex;
	volatile int value;
} pthread_cond_t;

#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_CREATE_DETACHED 1

#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_DEFAULT 0
#define PTHREAD_MUTEX_RECURSIVE 1
#define PTHREAD_MUTEX_ERRORCHECK 2
#ifdef __LITEOS__
#define PTHREAD_MUTEX_ERRORCHECK_NP 2
#define PTHREAD_MUTEX_RECURSIVE_NP 1
#endif

#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_MUTEX_ROBUST 1

#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_INHERIT 1
#define PTHREAD_PRIO_PROTECT 2

#define PTHREAD_INHERIT_SCHED 0
#define PTHREAD_EXPLICIT_SCHED 1

#define PTHREAD_SCOPE_SYSTEM 0
#define PTHREAD_SCOPE_PROCESS 1

/**
 * @ingroup pthread
 * Define a condition variable to be shared between threads within the same process.
 */
#define PTHREAD_PROCESS_PRIVATE 0

/**
 * @ingroup pthread
 * Define a condition variable to be shared among multiple processes.
 */
#define PTHREAD_PROCESS_SHARED 1

#ifndef __LITEOS__
#define PTHREAD_MUTEX_INITIALIZER {{{0}}}
#endif
#define PTHREAD_RWLOCK_INITIALIZER {{{0}}}
#ifdef __LITEOS__
/**
 * @ingroup pthread
 * Statically initialize a condition variable.
 */
#define PTHREAD_COND_INITIALIZER { { 0 }, -1, { 0, { NULL, NULL } } , NULL, -1 }
#else
#define PTHREAD_COND_INITIALIZER {{{0}}}
#endif

/* Initializer for pthread_once_t instances */
/**
 * @ingroup pthread
 * Define the initial value of pthread_once_t.
 */
#define PTHREAD_ONCE_INIT 0


#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DISABLE 1
#define PTHREAD_CANCEL_MASKED 2

#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

#define PTHREAD_CANCELED ((void *)-1)


#define PTHREAD_BARRIER_SERIAL_THREAD (-1)

#ifdef __LITEOS__
/**
 * @ingroup pthread
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_INHERIT.
 */
#ifndef POSIX_MUTEX_DEFAULT_INHERIT
#define POSIX_MUTEX_DEFAULT_INHERIT
#endif

#if defined POSIX_MUTEX_DEFAULT_INHERIT
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies the priority inheritance protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies the priority inheritance protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_INHERIT, OS_TASK_PRIORITY_LOWEST, 0, 0 }, \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
#elif defined POSIX_MUTEX_DEFAULT_PROTECT
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies the priority protection protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies the priority protection protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_PROTECT, OS_TASK_PRIORITY_LOWEST, 0, 0 },  \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
#else
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies no priority protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies no priority protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_NONE, OS_TASK_PRIORITY_LOWEST, 0, 0 }, \
    { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, \
    (LosTaskCB *)NULL, 0 } }
#endif
#endif

/* General thread operations */
/* Thread creation and management. */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to create a thread according to input parameters.
 *
 * @param pthread         [OUT] ID of the created thread.
 * @param attr            [IN]  Pointer to a thread attributes object. If the parameter value is NULL, the default thread attributes are applied.
 * @param start_routine   [IN]  Function entry of the thread.
 * @param arg             [IN]  Parameter of the thread entrance function.
 *
 * @attention
 * <ul>
 * <li>The parameter value and return value of the thread entrance function must be void *.</li>
 * <li>A smaller priority value indicates a higher priority.</li>
 * <li>POSIX issue7 defined pthread has the signal status in pthread, Huawei LiteOS pthread don't have signal status.</li>
 * </ul>
 *
 * @retval #int If successful, the pthread_create() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL   The thread fails to be created due to incorrect parameters.
 * @retval  #ENOSPC   The thread fails to be created because No free task control block is available ,or the SEM control block is not enough.
 * @retval  #ENOMEM   The thread fails to be created because the maximum number of threads is exceeded, or memory allocation fails, or an interrupt in underway.
 * @retval  #ENOERR   The thread is successfully created.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_exit | pthread_join
 * @since Huawei LiteOS V100R001C00
 */
int pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict, void *(*)(void *), void *__restrict);

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to mark a thread as detached. When a detached thread terminates, its resources are automatically released back to Huawei LiteOS without the need for another thread to join with the terminated thread.
 *
 * @param pthread  [IN]  ID of the thread to be marked as detached.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval    #int   If the call succeeds, pthread_detach() shall return 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #ESRCH   The target thread cannot be marked (the thread does not exist or has exited).
 * @retval  #EINVAL  The target thread is already marked as detached.
 * @retval  #ENOERR  The thread is successfully marked.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_join
 * @since Huawei LiteOS V100R001C00
 */
int pthread_detach(pthread_t);
#ifndef __LITEOS__
_Noreturn void pthread_exit(void *);
#else
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to terminate the current thread, and the return value contains only one parameter.
 *
 * @param   retval  [OUT] Pointer to the thread return value.
 *
 * @attention
 * <ul>
 * <li>Thread termination does not release any application visible process resources, including, but not limited to,
 * mutexes and file descriptors.</li>
 * </ul>
 *
 * @retval  #void None.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_create | pthread_join
 * @since Huawei LiteOS V100R001C00
 */
void pthread_exit(void *);
#endif

/* Wait for the thread to terminate. If thread_return is not NULL then
 * the retval from the thread's call to pthread_exit() is stored at thread_return.
 */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to wait for a thread to terminate and reclaim its resources.
 *
 * @param   pthread          [IN]  ID of the target thread (the waited thread).
 * @param   thread_return    [OUT] Return value sent to the waiting thread.
 *
 * @attention
 * <ul>
 * <li>A thread cannot be waited for by multiple threads. If a thread is waited for by multiple threads, ESRCH will be returned.</li>
 * </ul>
 *
 * @retval  #int  If successful, the pthread_join() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #ESRCH     The target thread is not joinable (the thread does not exist, or has exited, or is waited for by another thread).
 * @retval  #EDEADLK   A deadlock results because a thread is waiting for itself to terminate.
 * @retval  #EINVAL    The target thread is not joinable.
 * @retval  #ENOERR    The target thread is successfully joined with.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_detach
 * @since Huawei LiteOS V100R001C00
 */
int pthread_join(pthread_t, void **);

#ifdef __GNUC__
__attribute__((const))
#endif

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the thread identifier of the calling thread.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval pthread_t Thread ID [0,LOSCFG_BASE_CORE_TSK_LIMIT].
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
pthread_t pthread_self(void);

/* Compare two thread identifiers. */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to compare whether two thread IDs belong to the same thread.
 *
 * @param thread1  [IN]  ID of the thread 1 being compared.
 * @param thread2  [IN]  ID of the thread 2 being compared.
 *
 * @attention
 * <ul>
 * <li>Just compare the thread ID, not judge the validity of the thread ID.</li>
 * </ul>
 *
 * @retval  #int Any value other than 0  mean that two threads are equal.
 * @retval  #0  The two threads are unequal.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int pthread_equal(pthread_t, pthread_t);
#ifndef __LITEOS__
#ifndef __cplusplus
#define pthread_equal(x,y) ((x)==(y))
#endif
#endif

int pthread_setcancelstate(int, int *);
int pthread_setcanceltype(int, int *);
void pthread_testcancel(void);
int pthread_cancel(pthread_t);

/* Get scheduling policy and parameters for the thread */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the scheduling policy and priority of a thread.
 *
 * @param  pthread  [IN]  ID of the thread whose scheduling policy is to be read.
 * @param  policy   [OUT] Acquired scheduling policy.
 * @param  param    [OUT] Acquired scheduling priority.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int If successful, the pthread_getschedparam() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL   invalid parameter.
 * @retval  #ENOERR   The scheduling policy and priority of the thread is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_setschedparam
 * @since Huawei LiteOS V100R001C00
 */
int pthread_getschedparam(pthread_t, int *__restrict, struct sched_param *__restrict);

/* Set scheduling policy and parameters for the thread */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the scheduling policy and priority of a thread.
 *
 * @param  pthread  [IN] ID of the thread whose scheduling policy is to be set.
 * @param  policy   [IN] Specified scheduling policy.
 * @param  param    [IN] Pointer to the scheduling priority.
 *
 * @attention
 * <ul>
 * <li>The scheduling policy must be SCHED_OTHER, SCHED_FIFO, or SCHED_RR.</li>
 * <li>Only SCHED_RR is supported now. An error code will be returned if the value is set to the other two scheduling policies.</li>
 * <li>The content priority specified by the param parameter must fall within the OS priority range[0,31].</li>
 * <li>A smaller priority value indicates a higher priority.</li>
 * </ul>
 *
 * @retval  #int  If successful, the pthread_setschedparam() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL   invalid parameter.
 * @retval  #ENOERR   The scheduling policy and priority of the thread is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_getschedparam
 * @since Huawei LiteOS V100R001C00
 */
int pthread_setschedparam(pthread_t, int, const struct sched_param *);
int pthread_setschedprio(pthread_t, int);

/* Call init_routine just the once per control variable. */
/**
 * @ingroup pthread
 *
 * @par  Decription:
 * This API is used to call the initialization routine at most once.
 *
 * @param  once_control  [IN/OUT]  Once-control parameter.
 * @param  init_routine  [IN]      Initialization routine function.
 *
 * @attention
 * <ul>
 * <li>The control variable once_control must be statically initialized using PTHREAD_ONCE_INIT. Otherwise, this API will not work as expected.</li>
 * </ul>
 *
 * @retval  #int     Upon successful completion, pthread_once() shall return zero; otherwise, an error number shall be returned to indicate the error.
 * @retval  #EINVAL  One or more parameters are invalid.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
int pthread_once(pthread_once_t *, void (*)(void));

int pthread_mutex_init(pthread_mutex_t *__restrict, const pthread_mutexattr_t *__restrict);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_timedlock(pthread_mutex_t *__restrict, const struct timespec *__restrict);
int pthread_mutex_destroy(pthread_mutex_t *);
int pthread_mutex_consistent(pthread_mutex_t *);

int pthread_mutex_getprioceiling(const pthread_mutex_t *__restrict, int *__restrict);
int pthread_mutex_setprioceiling(pthread_mutex_t *__restrict, int, int *__restrict);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to initialize a condition variable.
 *
 * @param cond  [OUT]  Condition variable object.
 * @param attr  [IN]   Condition variable attribute. The passed value and the default value of this parameter can be only set to PTHREAD_PROCESS_PRIVATE.
 *
 * @attention
 * <ul>
 * <li>A condition variable cannot be initialized by multiple threads. When a condition variable needs to be reinitialized, it must not have been used.</li>
 * </ul>
 *
 * @retval  #EINVAL  One or more parameters are invalid.
 * @retval  #ENOERR  The condition variable is successfully initialized.
 * @retval  #ENOMEM  Failed to allocate in-memory resources for the operation.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_destroy
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_init(pthread_cond_t *__restrict, const pthread_condattr_t *__restrict);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to destroy a condition variable.
 *
 * @param cond  [IN]  Condition variable object.
 *
 * @attention
 * <ul>
 * <li>The condition variable is using memory and not destroyed.</li>
 * </ul>
 *
 * @retval  #ENVAIL  The parameter is invalid.
 * @retval  #EBUSY   The condition variable is being in use.
 * @retval  #ENOERR  The condition variable is successfully destroyed.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_init
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_destroy(pthread_cond_t *);

/**
 * @ingroup pthread
 * @par Description:
 * A thread has been being blocked on a condition variable and waits to be awoken by the condition variable.
 *
 * @param cond   [IN]  Condition variable object.
 * @param mutex  [IN]  Mutex object.
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #ENVAIL  One or more parameters are invalid.
 * @retval  #ENOERR  The thread is successfully awoken.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_signal | pthread_cond_broadcast
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_wait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict);

/**
 * @ingroup pthread
 * @par Description:
 * A thread has been being blocked on a condition variable and is awoken until the set relative time has passed or the thread obtains a condition variable.
 *
 * @param cond     [IN]    Condition variable object.
 * @param mutex    [IN]     Mutex object.
 * @param abstime  [IN]    Time object.
 *
 * @attention
 * <ul>
 * <li>The waiting time is a relative time.</li>
 * <li> Setting the timeout interval to a past time period is not supported.</li>
 * </ul>
 *
 * @retval  #ENOERR     The thread receives a signal and is successfully awoken.
 * @retval  #ENVAIL     One or more parameters are invalid.
 * @retval  #ETIMEDOUT  The waiting time has passed.
 * @retval  #ENOMEM     Failed to allocate in-memory resources for the operation.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_wait
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_timedwait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict, const struct timespec *__restrict);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to unblock all threads blocked on a condition variable and wake all these threads.
 *
 * @param cond  [IN]  Condition variable object.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #ENVAIL  The parameter is invalid.
 * @retval  #ENOERR  All threads blocked on this condition variable are successfully unblocked.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_wait
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_broadcast(pthread_cond_t *);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to unblock a thread blocked on a condition variable and wakes this thread.
 *
 * @param cond  [IN]  Condition variable object.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #ENVAIL  The parameter is invalid.
 * @retval  #ENOERR  The thread is successfully unlocked.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_cond_wait
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_cond_signal(pthread_cond_t *);

int pthread_rwlock_init(pthread_rwlock_t *__restrict, const pthread_rwlockattr_t *__restrict);
int pthread_rwlock_destroy(pthread_rwlock_t *);
int pthread_rwlock_rdlock(pthread_rwlock_t *);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict);
int pthread_rwlock_wrlock(pthread_rwlock_t *);
int pthread_rwlock_trywrlock(pthread_rwlock_t *);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict);
int pthread_rwlock_unlock(pthread_rwlock_t *);

int pthread_spin_init(pthread_spinlock_t *, int);
int pthread_spin_destroy(pthread_spinlock_t *);
int pthread_spin_lock(pthread_spinlock_t *);
int pthread_spin_trylock(pthread_spinlock_t *);
int pthread_spin_unlock(pthread_spinlock_t *);

int pthread_barrier_init(pthread_barrier_t *__restrict, const pthread_barrierattr_t *__restrict, unsigned);
int pthread_barrier_destroy(pthread_barrier_t *);
int pthread_barrier_wait(pthread_barrier_t *);
int pthread_key_create(pthread_key_t *, void (*)(void *));
int pthread_key_delete(pthread_key_t);
void *pthread_getspecific(pthread_key_t);
int pthread_setspecific(pthread_key_t, const void *);

/* Thread attribute handling. */

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to initialize a thread attributes object into default attributes.
 *
 * @param attr  [IN] Pointer to the thread attributes object to be initialized.
 *
 * @attention
 * <ul>
 * <li>Default thread attributes</li>
 * <li>detachstate                 = PTHREAD_CREATE_JOINABLE</li>
 * <li>schedpolicy                 = SCHED_RR</li>
 * <li>schedparam.sched_priority   = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO</li>
 * <li>inheritsched                = PTHREAD_INHERIT_SCHED</li>
 * <li>scope                       = PTHREAD_SCOPE_SYSTEM</li>
 * <li>stackaddr_set               = 0</li>
 * <li>stackaddr                   = NULL</li>
 * <li>stacksize_set               = 1</li>
 * <li>stacksize                   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE</li>
 * </ul>
 *
 * @retval  #int  Upon successful completion, pthread_attr_init() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  The input parameter is NULL.
 * @retval  #ENOERR  The thread attributes object is successfully initialized.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_destroy
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_init(pthread_attr_t *);

/* Destroy thread attributes object */
/**
 * @ingroup pthread
 *
 * @par Dependency:
 * This API is used to destroy a thread attributes object.
 *
 * @param attr  [IN] Pointer to the thread attributes object to be destroyed.
 *
 * @attention
 * <ul>
 * <li>This API does not take effect on Huawei LiteOS. In fact, nothing has been done in this API.</li>
 * </ul>
 *
 * @retval  #int  Upon successful completion, pthread_attr_destroy() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  The input parameter is NULL.
 * @retval  #ENOERR  The thread attributes object is successfully destroyed.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_init
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_destroy(pthread_attr_t *);

int pthread_attr_getguardsize(const pthread_attr_t *__restrict, size_t *__restrict);
int pthread_attr_setguardsize(pthread_attr_t *, size_t);

/* Get current minimal stack size. */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the stack size attribute of a thread attribute object.
 *
 * @param attr       [IN]  Pointer to the thread attributes object to be read.
 * @param stacksize  [OUT] Pointer to the acquired stack size.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval   #int   Upon successful completion, pthread_attr_getstacksize() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 * The pthread_attr_getstacksize() function stores the stacksize attribute value in stacksize if successful.
 *
 * @retval  #EINVAL  invalid parameter, or the stack size left unspecified.
 * @retval  #ENOERR  The stack size attribute is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setstacksize
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getstacksize(const pthread_attr_t *__restrict, size_t *__restrict);

/* Set minimum creation stack size. */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the stack size attribute of a thread attribute object.
 *
 * @param attr       [OUT] Pointer to the thread attributes object to be set.
 * @param stacksize  [IN]  Specified stack size.
 *
 * @attention
 * <ul>
 * <li>The stack size must fall within an appropriate range and be greater than PTHREAD_STACK_MIN.
 * If the stack size attribute is left unspecified, the default stack size will be used.</li>
 * </ul>
 *
 * @retval  #int Upon successful completion, pthread_attr_setstacksize() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The stack size attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getstacksize
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setstacksize(pthread_attr_t *, size_t);

/* Get the detachstate attribute */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the detach state attribute of a thread attributes object.
 *
 * @param  attr         [IN]  Pointer to the thread attributes object to be read.
 * @param  detachstate  [OUT] Pointer to the acquired detach state attribute.
 *
 * @attention
 * <ul>
 * <li>Detachstate shall be set to either PTHREAD_CREATE_DETACHED or PTHREAD_CREATE_JOINABLE.</li>
 * </ul>
 *
 * @retval   int   Upon successful completion, pthread_attr_getdetachstate() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 * The pthread_attr_getdetachstate() function stores the value of the detachstate attribute in detachstate if successful.
 *
 * @retval  #EINVAL    invalid parameter.
 * @retval  #ENOERR    The detach state attribute is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setdetachstate
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getdetachstate(const pthread_attr_t *, int *);

/* Set the detachstate attribute */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the detach state attribute of a thread attributes object.
 *
 * @param attr         [OUT] Pointer to the thread attributes object to be set.
 * @param detachstate  [IN]  Detach state attribute.
 *
 * @attention
 * <ul>
 * <li>The detach state attribute must be either PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHED. Otherwise, the attempt to set the detach state attribute will fail.</li>
 * </ul>
 *
 * @retval   #int   Upon successful completion, pthread_attr_setdetachstate() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The detach state attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getdetachstate
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_getstack(const pthread_attr_t *__restrict, void **__restrict, size_t *__restrict);
int pthread_attr_setstack(pthread_attr_t *, void *, size_t);

#ifdef __LITEOS__
/* Set starting address of stack. Whether this is at the start or end of
 * the memory block allocated for the stack depends on whether the stack
 * grows up or down.
 */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the stack address attribute of a thread attributes object. This attribute specifies the start address of a stack.
 *
 * @param attr       [OUT] Pointer to the thread attributes object to be set.
 * @param stackaddr  [IN]  Specified stack address.
 *
 * @attention
 * <ul>
 * <li>This API does not take effect on Huawei LiteOS Kernel.</li>
 * </ul>
 *
 * @retval  #int  Upon successful completion, pthread_attr_setstackaddr() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The stack address attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getstackaddr
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setstackaddr(pthread_attr_t *, void *);

/* Get any previously set stack address. */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the stack address attribute of a thread attributes object. This attribute specifies the starting address of a stack.
 *
 * @param attr       [IN]  Pointer to the thread attributes object to be read.
 * @param stackaddr  [OUT] Pointer to the acquired starting address of stack.
 *
 * @attention
 * <ul>
 * <li>This API does not take effect on Huawei LiteOS Kernel.</li>
 * </ul>
 *
 * @retval  #int  Upon successful completion, pthread_attr_getstackaddr() shall return a value of 0; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The stack address attribute is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setstackaddr
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getstackaddr(const pthread_attr_t *, void **);
#endif

/* Get scheduling contention scope */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the contention scope attribute of a thread attributes object.
 *
 * @param attr   [IN]  Pointer to the thread attributes object to be read.
 * @param scope  [OUT] Pointer to the acquired contention scope attribute.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #int  If successful, the pthread_attr_getscope() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The contention scope attribute is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setscope
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getscope(const pthread_attr_t *__restrict, int *__restrict);

/* Set scheduling contention scope */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the contention scope attribute of a thread attributes object.
 *
 * @param  attr   [OUT] Pointer to the thread attributes object to be set.
 * @param  scope  [IN]  Contention scope attribute.
 *
 * @attention
 * <ul>
 * <li>The contention scope attribute can only be PTHREAD_SCOPE_SYSTEM. PTHREAD_SCOPE_PROCES is not supported.</li>
 * </ul>
 *
 * @retval   #int     If successful, the pthread_attr_setscope() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL   invalid parameter.
 * @retval  #ENOTSUP  The specified contention scope attribute value is not supported.
 * @retval  #ENOERR   The contention scope attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getscope
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setscope(pthread_attr_t *, int);

/* Get scheduling policy */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the scheduling policy attribute of a thread attributes object.
 *
 * @param attr    [IN]  Pointer to the thread attributes object to be read.
 * @param policy  [OUT] Pointer to the acquired scheduling policy attribute.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int  If successful, the pthread_attr_getschedpolicy() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The scheduling policy attribute is successfully obtained.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setschedpolicy
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getschedpolicy(const pthread_attr_t *__restrict, int *__restrict);

/* Set scheduling policy */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the scheduling policy attribute of a thread attributes object.
 *
 * @param attr    [OUT] Pointer to the thread attributes object to be set.
 * @param policy  [IN]  Scheduling policy attribute.
 *
 * @attention
 * <ul>
 * <li>The scheduling policy attribute is SCHED_OTHER, SCHED_FIFO, or SCHED_RR.</li>
 * <li>Only SCHED_RR is supported now. An error code will be returned if the value is set to the other two scheduling policies.</li>
 * </ul>
 *
 * @retval   #int  If successful, the pthread_attr_setschedpolicy() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL    invalid parameter.
 * @retval  #ENOERR    The scheduling policy attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getschedpolicy
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setschedpolicy(pthread_attr_t *, int);

/* Get scheduling parameters */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the scheduling parameter attributes of a thread attributes object. The scheduling parameter attributes indicate the thread priorities.
 *
 * @param  attr   [IN]  Pointer to the thread attributes object to be read.
 * @param  param  [OUT] Pointer to the acquired scheduling parameter attributes object.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #int  If successful, the pthread_attr_getschedparam() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL  invalid parameter.
 * @retval  #ENOERR  The scheduling parameter attributes are successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setschedparam
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getschedparam(const pthread_attr_t *__restrict, struct sched_param *__restrict);

/* Set scheduling parameters */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the scheduling parameter attributes of a thread attributes object. The scheduling parameter attributes indicate thread priorities.
 *
 * @param  attr   [OUT] Pointer to the thread attributes object to be set.
 * @param  param  [IN]  Pointer to the scheduling parameter attributes object.
 *
 *@attention
 * <ul>
 * <li>The priority of the scheduling parameter attributes must be in the range of [0, 31].</li>
 * </ul>
 *
 * @retval   #int   If successful, the pthread_attr_setschedparam() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL    invalid parameter.
 * @retval  #ENOTSUP   made to set the attribute to an unsupported value.
 * @retval  #ENOERR    The scheduling parameter attributes are successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getschedparam
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setschedparam(pthread_attr_t *__restrict, const struct sched_param *__restrict);

/* Get scheduling inheritance attribute */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to acquire the inherit scheduler attribute of a thread attributes object.
 *
 * @param  attr     [IN]  Pointer to the thread attributes object to be read.
 * @param  inherit  [OUT] Pointer to the acquired inherit scheduler attribute.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #int  If successful, the pthread_attr_getinheritsched() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 *
 * @retval  #EINVAL   invalid parameter.
 * @retval  #ENOERR   The inherit scheduler attribute is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_setinheritsched
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_getinheritsched(const pthread_attr_t *__restrict, int *__restrict);

/* Set scheduling inheritance attribute */
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the inherit scheduler attribute of a thread attributes object.
 *
 * @param  attr     [OUT] Pointer to the thread attributes object to be set.
 * @param  inherit  [IN]  Inherit scheduler attribute.
 *
 * @attention
 * <ul>
 * <li>The inherit scheduler attribute must be either PTHREAD_INHERIT_SCHED or PTHREAD_EXPLICIT_SCHED.</li>
 * </ul>
 *
 * @retval   #int     If successful, the pthread_attr_setinheritsched() function shall return zero; otherwise, an error number shall be returned to indicate the error.
 * @retval  #EINVAL    invalid parameter.
 * @retval  #ENOERR    The inherit scheduler attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 * @see pthread_attr_getinheritsched
 * @since Huawei LiteOS V100R001C00
 */
int pthread_attr_setinheritsched(pthread_attr_t *, int);

int pthread_mutexattr_destroy(pthread_mutexattr_t *);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to initialize the condition variable attribute.
 * This API does not task effect on Huawei LiteOS Kernel.
 *
 * @param attr  [OUT]  Condition variable attribute.
 *
 * @attention
 * <ul>
 * <li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 * </ul>
 *
 * @retval  #ENVAIL  The parameter is invalid.
 * @retval  #ENOERR  The condition variable attribute is successfully initialized.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_condattr_setpshared | pthread_condattr_getpshared
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_condattr_init(pthread_condattr_t *);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to destroy the condition variable attribute.
 *
 * @param attr [IN]  Condition variable attribute.
 *
 * @attention
 * <ul>
 * <li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 * <li>This API does not task effect on Huawei LiteOS Kernel.</li>
 * </ul>
 *
 * @retval  #ENVAIL  The parameter is invalid.
 * @retval  #ENOERR  The condition variable attribute is successfully destroied.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_condattr_init
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_condattr_destroy(pthread_condattr_t *);
int pthread_condattr_setclock(pthread_condattr_t *, clockid_t);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to set the condition variable attribute.
 *
 * @param attr     [IN]   Condition variable attribute.
 * @param pshared  [OUT]  Condition variable attribute, which is always PTHREAD_PROCESS_PRIVATE.
 *
 * @attention
 * <ul>
 * <li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 * <li>This API does not task effect on Huawei LiteOS Kernel.</li>
 * </ul>
 *
 * @retval  #ENVAIL  One or more parameters are invalid.
 * @retval  #ENOERR  The condition variable attribute is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_condattr_getpshared
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_condattr_setpshared(pthread_condattr_t *, int);
int pthread_condattr_getclock(const pthread_condattr_t *__restrict, clockid_t *__restrict);

/**
 * @ingroup pthread
 * @par Description:
 * This API is used to obtain the condition variable attribute.
 *
 * @param attr     [IN]   Condition variable attribute.
 * @param pshared  [OUT]  Obtained condition variable attribute, which is always PTHREAD_PROCESS_PRIVATE.
 *
 * @attention
 * <ul>
 * <li>The condition variable attribute can be only set to PTHREAD_PROCESS_PRIVATE.</li>
 * <li>This API does not task effect on Huawei LiteOS Kernel.</li>
 * </ul>
 *
 * @retval  #ENVAIL  One or more parameters are invalid.
 * @retval  #ENOERR  The condition variable attribute is successfully obtained.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_condattr_setpshared
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_condattr_getpshared(const pthread_condattr_t *__restrict, int *__restrict);

int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__restrict, int *__restrict);

int pthread_barrierattr_destroy(pthread_barrierattr_t *);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *__restrict, int *__restrict);
int pthread_barrierattr_init(pthread_barrierattr_t *);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int);

/**
 * @ingroup pthread
 * @par Description:
 * The function is not supported.
 *
 * @attention
 * <ul>
 * <li>The function is not supported.</li>
 * </ul>
 *
 * @retval #int  zero.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see None
 *
 * @since Huawei LiteOS V100R001C00
 */
int pthread_atfork(void (*)(void), void (*)(void), void (*)(void));

int pthread_getconcurrency(void);
int pthread_setconcurrency(int);

int pthread_getcpuclockid(pthread_t, clockid_t *);

#ifndef __LITEOS__
struct __ptcb {
	void (*__f)(void *);
	void *__x;
	struct __ptcb *__next;
};

void _pthread_cleanup_push(struct __ptcb *, void (*)(void *), void *);
void _pthread_cleanup_pop(struct __ptcb *, int);
#endif

struct pthread_cleanup_buffer {
	void (*routine)(void *);
	void *arg;
	struct pthread_cleanup_buffer *next;
};
#define pthread_cleanup_push(routine, arg) do { struct pthread_cleanup_buffer cb; pthread_cleanup_push_inner(&cb, routine, arg);
#define pthread_cleanup_pop(arg) pthread_cleanup_pop_inner(&cb, (arg)); } while(0)

#ifdef _GNU_SOURCE
struct cpu_set_t;

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to get the cpu affinity mask from the thread.
 *
 * @param   pthread      [IN]  ID of the thread whose affinity mask is to be read.
 * @param   cpusetsize   [IN]  Size of cpuset in bytes.
 * @param   cpuset       [OUT] Pointer to the cpu affinity mask.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #EINVAL    Invalid parameter.
 * @retval  #ENOERR    The cpu affinity mask of the thread is successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_setaffinity_np
 *
 * @since Huawei LiteOS V200R003C00
 */
int pthread_getaffinity_np(pthread_t, size_t, struct cpu_set_t *);

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the cpu affinity mask for the thread.
 *
 * @param   pthread      [IN] ID of the thread whose affinity mask is to be set.
 * @param   cpusetsize   [IN] Size of cpuset in bytes.
 * @param   cpuset       [IN] Pointer to the cpu affinity mask.
 *
 * @attention
 * <ul>
 * <li>The cpu affinity mask attributes must be in the range of [1, LOSCFG_KERNEL_CPU_MASK].</li>
 * </ul>
 *
 * @retval  #EINVAL    Invalid parameter.
 * @retval  #ENOERR    The cpu affinity mask of the thread is successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_getaffinity_np
 *
 * @since Huawei LiteOS V200R003C00
 */
int pthread_setaffinity_np(pthread_t, size_t, const struct cpu_set_t *);
#ifdef __LITEOS__
/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to set the cpu affinity mask attributes of a thread attributes object.
 *
 * @param   attr         [OUT] Pointer to the thread attributes object to be set.
 * @param   cpusetsize   [IN]  Size of cpuset in bytes.
 * @param   cpuset       [IN]  Pointer to the cpu affinity mask.
 *
 * @attention
 * <ul>
 * <li>The cpu affinity mask attributes must be in the range of [1, LOSCFG_KERNEL_CPU_MASK].</li>
 * </ul>
 *
 * @retval  #EINVAL    Invalid parameter.
 * @retval  #ENOERR    The cpu affinity mask attributes are successfully set.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_attr_getaffinity_np
 *
 * @since Huawei LiteOS V200R003C00
 */
int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset);

/**
 * @ingroup pthread
 *
 * @par Description:
 * This API is used to get the cpu affinity mask attributes of a thread attributes object.
 *
 * @param   attr         [IN]  Pointer to the thread attributes object to be read.
 * @param   cpusetsize   [IN]  Size of cpuset in bytes.
 * @param   cpuset       [OUT] Pointer to the cpu affinity mask.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval  #EINVAL    Invalid parameter.
 * @retval  #ENOERR    The cpu affinity mask attributes are successfully acquired.
 *
 * @par Dependency:
 * <ul><li>pthread.h</li></ul>
 *
 * @see pthread_attr_setaffinity_np
 *
 * @since Huawei LiteOS V200R003C00
 */
int pthread_attr_getaffinity_np(const pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset);
#endif
int pthread_getattr_np(pthread_t, pthread_attr_t *);
int pthread_setname_np(pthread_t, const char *);
int pthread_getattr_default_np(pthread_attr_t *);
int pthread_setattr_default_np(const pthread_attr_t *);
int pthread_tryjoin_np(pthread_t, void **);
int pthread_timedjoin_np(pthread_t, void **, const struct timespec *);
#endif

#if _REDIR_TIME64
__REDIR(pthread_mutex_timedlock, __pthread_mutex_timedlock_time64);
__REDIR(pthread_cond_timedwait, __pthread_cond_timedwait_time64);
__REDIR(pthread_rwlock_timedrdlock, __pthread_rwlock_timedrdlock_time64);
__REDIR(pthread_rwlock_timedwrlock, __pthread_rwlock_timedwrlock_time64);
#ifdef _GNU_SOURCE
__REDIR(pthread_timedjoin_np, __pthread_timedjoin_np_time64);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
