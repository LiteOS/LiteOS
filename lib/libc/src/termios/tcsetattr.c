#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#ifdef __LITEOS__
#include "los_printf.h"
#endif

int tcsetattr(int fd, int act, const struct termios *tio)
{
#ifndef __LITEOS__
	if (act < 0 || act > 2) {
		errno = EINVAL;
		return -1;
	}
	return ioctl(fd, TCSETS+act, tio);
#else
	(void)fd;
	(void)act;
	(void)tio;
	PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
	errno = ENOSYS;
	return -1;
#endif
}
