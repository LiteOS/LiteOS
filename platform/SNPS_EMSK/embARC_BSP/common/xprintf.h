/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	1	/* 1: Use input function */
#define	_LINE_ECHO		1	/* 1: Echo back input chars in xgets function */

#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
extern void (*xfunc_out)(unsigned char);
extern void xputc (char c);
extern void xputs (const char* str);
extern void xfputs (void (*func)(unsigned char), const char* str);
extern void xvprintf (const char* fmt, va_list arp);
extern void xprintf (const char* fmt, ...);
extern void xsprintf (char* buff, const char* fmt, ...);
extern void xfprintf (void (*func)(unsigned char), const char*	fmt, ...);
extern void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (unsigned char(*)(void))(func)
extern unsigned char (*xfunc_in)(void);
extern int xgets (char* buff, int len);
extern int xfgets (unsigned char (*func)(void), char* buff, int len);
extern int xatoi (char** str, long* res);
#endif

#ifdef __cplusplus
}
#endif

#endif
