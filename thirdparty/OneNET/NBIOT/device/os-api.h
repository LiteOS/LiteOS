/*
 Copyright (c) 2018 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

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
