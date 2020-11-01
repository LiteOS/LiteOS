#include <stdio.h>
#include <stdarg.h>
#include "los_printf_pri.h"

int printf(const char *restrict fmt, ...)
{
	int ret = 0;
	va_list ap;
	va_start(ap, fmt);
#if defined(LOSCFG_FS_VFS)
	ret = vfprintf(stdout, fmt, ap);
#elif defined(LOSCFG_PLATFORM_UART_WITHOUT_VFS)
	UartVprintf(fmt, ap);
#else
	ret = -1;
#endif
	va_end(ap);
	return ret;
}
