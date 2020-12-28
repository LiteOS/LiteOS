/**
 * @defgroup stdlib Stdlib
 * @ingroup libc
 */

#ifndef _STDLIB_H
#define _STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __LITEOS__
#include <malloc.h>
#endif

#include <features.h>

#ifdef __LITEOS__
#undef NULL
#endif
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#define __NEED_wchar_t

#include <bits/alltypes.h>

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to an integer.
 *    The call is equivalent to strtol(str, (char **)NULL, 10).
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  int     converted value if the value can be represented.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtol
 *@since Huawei LiteOS V100R001C00
 */
int atoi (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a long integer.
 *    The call is equivalent to strtol(str, (char **)NULL, 10).
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  long     converted value if the value can be represented.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtol
 *@since Huawei LiteOS V100R001C00
 */
long atol (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a long long integer.
 *    The call is equivalent to strtol(str, (char **)NULL, 10).
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  long-long     converted value if the value can be represented.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtol
 *@since Huawei LiteOS V100R001C00
 */
long long atoll (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a double-precision number.
 *    The call is equivalent to strtod(str, (char **)NULL).
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  double  converted value if the value can be represented.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The value to be returned would cause overflow  or underflow.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtod
 *@since Huawei LiteOS V100R001C00
 */
double atof (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a float-precision number.
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  float     converted value if the value can be represented.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The value to be returned would cause overflow  or underflow.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtod | strtold
 *@since Huawei LiteOS V100R001C00
 */
float strtof (const char *__restrict, char **__restrict);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a double-precision number.
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  double     converted value if the value can be represented.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The value to be returned would cause overflow  or underflow.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtof | strtold
 *@since Huawei LiteOS V100R001C00
 */
double strtod (const char *__restrict, char **__restrict);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a long-double-precision number.
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  long-double     converted value if the value can be represented.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The value to be returned would cause overflow  or underflow.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtod | strtof
 *@since Huawei LiteOS V100R001C00
 */
long double strtold (const char *__restrict, char **__restrict);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a long integer.
 *    Ensure that "base" is between 2 and 36 inclusive, or the special value of 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  non-zero  The converted value, successful completion.
 *@retval  0    Fails to convert, with any of the following error codes in errno.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The converted value is out of range {LONG_MIN, LONG_MAX}.</li>
 *<li><b>EINVAL</b>: The value of "base" is not supported.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtoll | strtoul | strtoull
 *@since Huawei LiteOS V100R001C00
 */
long strtol (const char *__restrict, char **__restrict, int);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a unsigned long integer.
 *    Ensure that "base" is between 2 and 36 inclusive, or the special value of 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  non-zero  The converted value, successful completion.
 *@retval  0    Fails to convert, with any of the following error codes in errno.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The converted value is out of range ULONG_MAX.</li>
 *<li><b>EINVAL</b>: The value of "base" is not supported.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtoll | strtoul | strtoull
 *@since Huawei LiteOS V100R001C00
 */
unsigned long strtoul (const char *__restrict, char **__restrict, int);


/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a long long integer.
 *    Ensure that "base" is between 2 and 36 inclusive, or the special value of 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  non-zero  The converted value, successful completion.
 *@retval  0    Fails to convert, with any of the following error codes in errno.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The converted value is out of range {LLONG_MIN, LLONG_MAX}.</li>
 *<li><b>EINVAL</b>: The value of "base" is not supported.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtoll | strtoul | strtoull
 *@since Huawei LiteOS V100R001C00
 */
long long strtoll (const char *__restrict, char **__restrict, int);


/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to convert a string to a unsigned long long integer.
 *    Ensure that "base" is between 2 and 36 inclusive, or the special value of 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  non-zero  The converted value, successful completion.
 *@retval  0    Fails to convert, with any of the following error codes in errno.
 *
 *@par Errors
 *<ul>
 *<li><b>ERANGE</b>: The converted value is out of range ULLONG_MAX.</li>
 *<li><b>EINVAL</b>: The value of "base" is not supported.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see strtoll | strtoul | strtoull
 *@since Huawei LiteOS V100R001C00
 */
unsigned long long strtoull (const char *__restrict, char **__restrict, int);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to compute a sequence of pseudo-random integers in the range [0, RAND_MAX].
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see random
 *@since Huawei LiteOS V100R001C00
 */
int rand (void);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to set  the argument as a seed for a new sequence of pseudo-random
 *    numbers to be returned by subsequent calls to rand().
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see srandom
 *@since Huawei LiteOS V100R001C00
 */
void srand (unsigned);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *aligned_alloc(size_t, size_t);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to abort the system, the system enters swi exception, print backtrace and then
 *    Entering an infinite loop.
 *@attention
 *<ul>
 *<li> Normal interrupts will not be responded, only NMI exception can be triggered.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see exit
 *@since Huawei LiteOS V100R001C00
 */
_Noreturn void abort (void);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see abort | exit
 *@since Huawei LiteOS V100R001C00
 */
int atexit (void (*) (void));

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to terminate a thread, the current thread enters an infinite loop.
 *@attention
 *<ul>
 *<li> The current thread enters an infinite loop, the thread with a lower priority than
      the current thread can not obtain cpu resources.</li>
 *<li> CPU occupancy rate will up to 100%.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see abort
 *@since Huawei LiteOS V100R001C00
 */
_Noreturn void exit (int);
_Noreturn void _Exit (int);
int at_quick_exit (void (*) (void));
_Noreturn void quick_exit (int);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return NULL.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see putenv | setenv | unsetenv
 *@since Huawei LiteOS V100R001C00
 */
char *getenv (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return 0.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
int system (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to search an array of "nmemb" objects, the initial element of which
 *    is pointed to by base0", for an element that matches the object pointed to by "key".
 *    The size of each element in the array is specified by "size".
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  NULL    No match is found.
 *@retval  void*   A pointer to a matching member of the array.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to sort an array of "nmemb", the initial element of which
 *    is pointed to by base", The size of each object, in bytes, is specified by the "size".
 *    If the "nmemb" has the value zero, the comparison function pointed to by "compar"
 *    would not be called and no rearrangement will take place.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see None.
 *@since Huawei LiteOS V100R001C00
 */
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to compute the absolute value of its integer operand "j".
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  int      The absolute value of its integer operand.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see labs | llabs
 *@since Huawei LiteOS V100R001C00
 */
int abs (int);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to compute the absolute value of the long integer operand "j".
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  long      The absolute value of its integer operand.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see abs | llabs
 *@since Huawei LiteOS V100R001C00
 */
long labs (long);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to compute the absolute value of the long long integer operand "j".
 *    If the value cannot be represented, the behavior is undefined.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  long-long      The absolute value of its integer operand.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see labs | abs
 *@since Huawei LiteOS V100R001C00
 */
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);

int mblen (const char *, size_t);
int mbtowc (wchar_t *__restrict, const char *__restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs (char *__restrict, const wchar_t *__restrict, size_t);

/**
 * @ingroup stdlib
 * Define unsuccessful termination to 1.
 */
#define EXIT_FAILURE 1

/**
 * @ingroup stdlib
 * Define successful termination to 0.
 */
#define EXIT_SUCCESS 0

size_t __ctype_get_mb_cur_max(void);
#define MB_CUR_MAX (__ctype_get_mb_cur_max())

/**
 * @ingroup stdlib
 * Define Maximum value returned by rand() to 0x7fffffff.
 */
#define RAND_MAX (0x7fffffff)


#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

#define WNOHANG    1
#define WUNTRACED  2

#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#define WTERMSIG(s) ((s) & 0x7f)
#define WSTOPSIG(s) WEXITSTATUS(s)
#define WIFEXITED(s) (!WTERMSIG(s))
#define WIFSTOPPED(s) ((short)((((s)&0xffff)*0x10001)>>8) > 0x7f00)
#define WIFSIGNALED(s) (((s)&0xffff)-1U < 0xffu)

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to allocate "size" bytes  memory aligned on a boundary specified by "alignment".
 *The value of "alignment" shall be a power of two and  multiple of sizeof(void *).
 *The value of errno would not be modified.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@retval  0  Success to allocate memory.
 *@retval  EINVAL    The value of "alignment" shall be a power of two and  multiple of sizeof(void *).
 *@retval  ENOMEM    There is no enough memory.
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see malloc | free
 *@since Huawei LiteOS V100R001C00
 */
int posix_memalign (void **, size_t, size_t);
int setenv (const char *, const char *, int);
int unsetenv (const char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return -1.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see mktemp | mkstemps
 *@since Huawei LiteOS V100R001C00
 */
int mkstemp (char *);
int mkostemp (char *, int);
char *mkdtemp (char *);
int getsubopt (char **, char *const *, char **);
int rand_r (unsigned *);

#endif


#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
char *realpath (const char *__restrict, char *__restrict);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to generate a pseudo-random integer in the range [0, RAND_MAX].
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see srandom
 *@since Huawei LiteOS V100R001C00
 */
long int random (void);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is used to set  the argument as a seed for a new sequence of pseudo-random
 *    numbers to be returned by subsequent calls to random().
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see srandom
 *@since Huawei LiteOS V100R001C00
 */
void srandom (unsigned int);
char *initstate (unsigned int, char *, size_t);
char *setstate (char *);
int putenv (char *);
int posix_openpt (int);
int grantpt (int);
int unlockpt (int);
char *ptsname (int);
char *l64a (long);
long a64l (const char *);
void setkey (const char *);
double drand48 (void);
double erand48 (unsigned short [3]);
long int lrand48 (void);
long int nrand48 (unsigned short [3]);
long mrand48 (void);
long jrand48 (unsigned short [3]);
void srand48 (long);
unsigned short *seed48 (unsigned short [3]);
void lcong48 (unsigned short [7]);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#include <alloca.h>

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return NULL.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see mkstemp
 *@since Huawei LiteOS V100R001C00
 */
char *mktemp (char *);

/**
 *@ingroup stdlib
 *
 *@par Description:
 *This API is defined, but just return -1.
 *@attention
 *<ul>
 *<li> None.</li>
 *</ul>
 *
 *@par Dependency:
 *<ul><li>stdlib.h</li></ul>
 *@see mkstemp | mkstemps
 *@since Huawei LiteOS V100R001C00
 */
int mkstemps (char *, int);
int mkostemps (char *, int, int);
void *valloc (size_t);
void *memalign(size_t, size_t);
int getloadavg(double *, int);
int clearenv(void);
#define WCOREDUMP(s) ((s) & 0x80)
#define WIFCONTINUED(s) ((s) == 0xffff)
#endif

#ifdef _GNU_SOURCE
int ptsname_r(int, char *, size_t);
char *ecvt(double, int, int *, int *);
char *fcvt(double, int, int *, int *);
char *gcvt(double, int, char *);
char *secure_getenv(const char *);
struct __locale_struct;
#ifndef __LITEOS__
float strtof_l(const char *__restrict, char **__restrict, struct __locale_struct *);
double strtod_l(const char *__restrict, char **__restrict, struct __locale_struct *);
long double strtold_l(const char *__restrict, char **__restrict, struct __locale_struct *);
#endif
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define mkstemp64 mkstemp
#define mkostemp64 mkostemp
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define mkstemps64 mkstemps
#define mkostemps64 mkostemps
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
