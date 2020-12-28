#include <features.h>
#include "libc.h"

#define START "_start"

#include "crt_arch.h"

#ifdef __LITEOS__
#define weak
#endif
int main();
weak void _init();
weak void _fini();
int __libc_start_main(int (*)(), int, char **,
	void (*)(), void(*)(), void(*)());

void _start_c(long *p)
{
#ifndef __LITEOS__
	int argc = p[0];
	char **argv = (void *)(p+1);
	__libc_start_main(main, argc, argv, _init, _fini, 0);
#endif
}
