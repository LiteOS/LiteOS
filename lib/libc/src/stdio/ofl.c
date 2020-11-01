#include "stdio_impl.h"
#include "lock.h"

static FILE *ofl_head;
#ifndef __LITEOS__
static volatile int ofl_lock[1];
#else
pthread_mutex_t ofl_lock_value = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t *ofl_lock = &ofl_lock_value;
#endif

FILE **__ofl_lock()
{
	LOCK(ofl_lock);
	return &ofl_head;
}

void __ofl_unlock()
{
	UNLOCK(ofl_lock);
}
