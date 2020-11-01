#include <stdlib.h>
#include "syscall.h"

_Noreturn void _Exit(int ec)
{
#ifndef __LITEOS__
	__syscall(SYS_exit_group, ec);
	for (;;) __syscall(SYS_exit, ec);
#else
	exit(ec);
#endif
}
