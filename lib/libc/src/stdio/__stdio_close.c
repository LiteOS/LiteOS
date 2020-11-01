#include "unistd.h"
#include "stdio_impl.h"

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int __stdio_close(FILE *f)
{
	return close(__aio_close(f->fd));
}
