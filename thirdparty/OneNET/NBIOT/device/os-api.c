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

#include <stdint.h>
#include "os-api.h"
#include "los_mux.ph"

int osMemInit(void)
{
	return 0;
}
void *osMalloc(int size)
{
	void *buffer = LOS_MemAlloc(m_aucSysMem0, size);

	OS_ASSERT(buffer);
	return buffer;
}
void osFree(void *buffer)
{
	LOS_MemFree(m_aucSysMem0, buffer);
}


void osDelayMS (UINT32 ms)
{
	osDelay(ms);
}


void *os_mutex_create(void)
{
  uint32_t uwRet;
  uint32_t uwSemId;
  
  if (OS_INT_ACTIVE)
  {
    return NULL;
  }

  uwRet = LOS_MuxCreate(&uwSemId);

  if (uwRet == LOS_OK)
  {
    return (void*)(GET_MUX(uwSemId));
  }
  else
  {
    return NULL;
  }
}

void os_mutex_destroy(void *mutex)
{
  if (OS_INT_ACTIVE)
  {
    return;
  }

  if (mutex == NULL)
  {
    return;
  }

  LOS_MuxDelete(((MUX_CB_S *)mutex)->ucMuxID);
}

void os_mutex_lock(void *mutex, uint32_t millisec)
{
	uint32_t ticks;
	
	if (mutex == NULL) return;
	if (OS_INT_ACTIVE) return;
	
  if (millisec == OSA_WAIT_FOREVER) {
    ticks = OSA_WAIT_FOREVER;
  } else {
    ticks = LOS_MS2Tick(millisec);
    if (ticks == 0) 
     ticks = 1;
 }
 
 LOS_MuxPend(((MUX_CB_S *)mutex)->ucMuxID, ticks);
}

void os_mutex_unlock(void *mutex)
{
  if (mutex == NULL)
  {
    return;
  }

  LOS_MuxPost(((MUX_CB_S *)mutex)->ucMuxID);
}


int sem_init(sem_t *sem, int shared, uint32_t count)
{
	(void) shared;

  osSemaphoreDef(SEM);
  
  *sem = osBinarySemaphoreCreate (osSemaphore(SEM), 1);
  
  if(*sem == NULL)
  {
    return -1;
  }
  
  if(count == 0)	// Means it can't be taken
  {
    osSemaphoreWait(*sem,0);
  }

  return 0;
}

int sem_destroy(sem_t *sem)
{
	if (osSemaphoreDelete(*sem) == LOS_OK)
		return 0;
	return -1;
}

int sem_post(sem_t *sem)
{
	if (osSemaphoreRelease(*sem) == LOS_OK)
		return 0;
	return -1;
}

int sem_timedwait(sem_t *sem, uint32_t timeout)
{
  if(osSemaphoreWait (*sem, timeout) != -1)
  {
    return 0;
  }
  return -1;
}

pthread_t os_thread_new(const char *name, os_pthread thread , void *arg, int stacksize, int prio)
{
  const osThreadDef_t os_thread_def = { (char *)name, (os_pthread)thread, (osPriority)prio, 0, stacksize};
  pthread_t pid = osThreadCreate(&os_thread_def, arg);
  
  OS_ASSERT(pid);
  return pid;
}
