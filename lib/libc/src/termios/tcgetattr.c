#include <termios.h>
#include <sys/ioctl.h>
#ifdef __LITEOS__
#include <errno.h>
#include "los_printf.h"
#endif

int tcgetattr(int fd, struct termios *tio)
{
#ifndef __LITEOS__
	if (ioctl(fd, TCGETS, tio))
		return -1;
	return 0;
#else
	(void)fd;
	(void)tio;
	PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
	errno = ENOSYS;
	return -1;
#endif
}
