#include "pthread_impl.h"

volatile size_t __pthread_tsd_size = sizeof(void *) * PTHREAD_KEYS_MAX;
void *__pthread_tsd_main[PTHREAD_KEYS_MAX] = { 0 };

static void (*keys[PTHREAD_KEYS_MAX])(void *);

static pthread_mutex_t key_lock = PTHREAD_MUTEX_INITIALIZER;

static pthread_key_t next_key;

static void nodtor(void *dummy)
{
}

static void dummy_0(void)
{
}

weak_alias(dummy_0, __tl_lock);
weak_alias(dummy_0, __tl_unlock);

int __pthread_key_create(pthread_key_t *k, void (*dtor)(void *))
{

	/* Purely a sentinel value since null means slot is free. */
	if (!dtor) dtor = nodtor;

	(void)pthread_mutex_lock(&key_lock);
	pthread_key_t j = next_key;
	do {
		if (!keys[j]) {
			keys[next_key = *k = j] = dtor;
			(void)pthread_mutex_unlock(&key_lock);
			return 0;
		}
	} while ((j=(j+1)%PTHREAD_KEYS_MAX) != next_key);

	(void)pthread_mutex_unlock(&key_lock);
	return EAGAIN;
}

int __pthread_key_delete(pthread_key_t k)
{
	_pthread_data *self = __pthread_self(), *td=self;

	(void)pthread_mutex_lock(&key_lock);

	__tl_lock();
	td->tsd[k] = 0;
	__tl_unlock();

	keys[k] = 0;

	(void)pthread_mutex_unlock(&key_lock);

	return 0;
}

void __pthread_tsd_run_dtors(void)
{
	_pthread_data *self = __pthread_self();
	int i, j;
	for (j=0; self->tsd_used && j<PTHREAD_DESTRUCTOR_ITERATIONS; j++) {
		(void)pthread_mutex_lock(&key_lock);
		self->tsd_used = 0;
		for (i=0; i<PTHREAD_KEYS_MAX; i++) {
			void *val = self->tsd[i];
			void (*dtor)(void *) = keys[i];
			self->tsd[i] = 0;
			if (val && dtor && dtor != nodtor) {
				(void)pthread_mutex_unlock(&key_lock);
				dtor(val);
				(void)pthread_mutex_lock(&key_lock);
			}
		}
		(void)pthread_mutex_unlock(&key_lock);
	}
}

weak_alias(__pthread_key_create, pthread_key_create);
weak_alias(__pthread_key_delete, pthread_key_delete);
