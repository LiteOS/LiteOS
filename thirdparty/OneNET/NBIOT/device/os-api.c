/*
 * os-api.c
 *
 */

#include <assert.h>
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
