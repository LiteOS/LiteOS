/*
 * os-api.h
 *
 */

#ifndef OS_API_H_
#define OS_API_H_


#include "los_sys.h"
#include "los_sem.h"
#include "los_task.h"
#include "los_memory.h"
#include "cmsis_os.h"

#define  OSA_WAIT_FOREVER  0xFFFFFFFFU

typedef osSemaphoreId sem_t;
typedef osMutexId     pthread_mutex_t;
typedef osThreadId    pthread_t;


#define OS_PLATFORM_ASSERT(__x) do { printf("Assertion \"%s\" failed at  \
    line %d in %s\n",#__x, __LINE__, __FILE__); while (1);} while(0)

#define OS_ASSERT(assertion) do { if (!(assertion)) { \
  OS_PLATFORM_ASSERT(assertion); }} while(0)

int osMemInit(void);
void *osMalloc(int size);
void osFree(void *buffer);
void osDelayMS (UINT32 ms);

	
void *os_mutex_create(void);
void os_mutex_destroy(void *mutex);
void os_mutex_lock(void *mutex, uint32_t millisec);
void os_mutex_unlock(void *mutex);
	

int sem_init(sem_t *pSem, int shared, UINT32 initValue);
int sem_destroy(sem_t *pSem);
int sem_post(sem_t *pSem);
int sem_timedwait(sem_t *pSem, UINT32 timeout);


pthread_t os_thread_new(const char *name, os_pthread thread , void *arg, int stacksize, int prio);


#endif /* OS_API_H_ */
