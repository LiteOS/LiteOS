#ifndef LOCK_H
#define LOCK_H
#ifdef __LITEOS__
#include "pthread.h"
#endif

hidden void __lock(volatile int *);
hidden void __unlock(volatile int *);
#ifdef __LITEOS__
#define LOCK(x) pthread_mutex_lock((pthread_mutex_t *)(&(x)))
#define UNLOCK(x) pthread_mutex_unlock((pthread_mutex_t *)(&(x)))
#else
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)
#endif

#endif
