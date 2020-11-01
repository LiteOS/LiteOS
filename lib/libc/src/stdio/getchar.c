#include <stdio.h>
#include "getc.h"
#include "getchar_uart.h"

int getchar(void)
{
#if defined(LOSCFG_FS_VFS)
	return do_getc(stdin);
#elif defined(LOSCFG_DRIVERS_UART)
	return do_uart_getc();
#else
	return EOF;
#endif
}
