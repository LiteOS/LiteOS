#include "stdio_impl.h"
#include <errno.h>

int fileno(FILE *f)
{
	FLOCK(f);
	int fd;
#ifdef __LITEOS__
	if (f->flags & F_CLS) {
		fd = -1;
	} else
#endif
	{
		fd = f->fd;
	}
	FUNLOCK(f);
	if (fd < 0) {
		errno = EBADF;
		return -1;
	}
	return fd;
}

weak_alias(fileno, fileno_unlocked);
