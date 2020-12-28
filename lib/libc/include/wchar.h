/**
 * @defgroup wchar Wchar
 * @ingroup libc
 */

#ifndef _WCHAR_H
#define _WCHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_FILE
#define __NEED___isoc_va_list
#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_wint_t
#define __NEED_mbstate_t

#if __STDC_VERSION__ < 201112L
#define __NEED_struct__IO_FILE
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_locale_t
#define __NEED_va_list
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_wctype_t
#endif

#include <bits/alltypes.h>

#if L'\0'-1 > 0
#define WCHAR_MAX (0xffffffffu+L'\0')
#define WCHAR_MIN (0+L'\0')
#else
#define WCHAR_MAX (0x7fffffff+L'\0')
#define WCHAR_MIN (-1-0x7fffffff+L'\0')
#endif

#ifdef __LITEOS__
#undef NULL
#endif
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#undef WEOF
#define WEOF 0xffffffffU

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcscpy() function is the wide-character equivalent of the strcpy() function.
 * It copies the wide-character string pointed to by src,
 * including the terminating null wide character (L'\0'), to the array pointed to by dest.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* wcscpy() returns dest.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strcpy | wmemcpy
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wcscpy (wchar_t *__restrict, const wchar_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsncpy() function copies at most n wide characters from the wide-character
 * string pointed to by src, including the terminating null wide character (L"\0"),
 * to the array pointed to by dest. Exactly n wide characters are written at dst.
 * If the length wcslen(src) is smaller than n, the remaining wide characters in the
 * array pointed to by dst are filled with null wide characters. If the length
 * wcslen(src) is greater than or equal to n, the string pointed to by dest will
 * not be terminated by a null wide character. The strings may not overlap.The programmer
 * must ensure that there is room for at least n wide characters at dst.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* wcsncpy() returns the address of dst.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strncpy
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wcsncpy (wchar_t *__restrict, const wchar_t *__restrict, size_t);

wchar_t *wcscat (wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcsncat (wchar_t *__restrict, const wchar_t *__restrict, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcscmp() function compares the wide-character string pointed to by s1
 * and the wide-character string pointed to by s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The wcscmp() function returns zero if the wide-character strings
 * at s1 and s2 are equal. It returns an integer greater than zero if at the first
 * differing position i, the corresponding wide-character s1[i] is greater than s2[i].
 * It returns an integer less than zero if at the first differing position i,
 * the corresponding wide-character s1[i] is less than s2[i].
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strcmp | wmemcmp
 * @since Huawei LiteOS V100R001C00
 */
int wcscmp (const wchar_t *, const wchar_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsncmp() function compares the wide-character string pointed to by s1
 * and the wide-character string pointed to by s2, but at most n wide characters
 * from each string. In each string, the comparison extends only up to the first
 * occurrence of a null wide character (L"\0"), if any.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The wcsncmp() function returns zero if the wide-character
 * strings at s1 and s2, truncated to at most length n,
 * are equal. It returns an integer greater than zero if at the first differing
 * position i (i < n), the corresponding wide-character
 * s1[i] is greater than s2[i]. It returns an integer less than zero if at the
 * first differing position i (i < n), the corresponding
 * wide-character s1[i] is less than s2[i].
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strncmp
 * @since Huawei LiteOS V100R001C00
 */
int wcsncmp (const wchar_t *, const wchar_t *, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcscoll() function shall compare the wide-character string pointed to by
 * s1 to the wide-character string pointed to by s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, wcscoll() shall return an integer
 * greater than, equal to, or less than 0, according to whether the wide-character
 * string pointed to by s1 is greater than, equal to, or less than the wide-character
 * string pointed to by s2.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see wcscmp | wcsxfrm
 * @since Huawei LiteOS V100R001C00
 */
int wcscoll(const wchar_t *, const wchar_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsxfrm() function transforms wide-character string.
 *
 * @attention
 * <ul>
 * <li>On error, it don't set errno in Liteos.</li>
 * </ul>
 *
 * @retval #size_t The wcsxfrm() shall return the length of the transformed
 * wide-character string (not including the terminating null wide-character code).
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see wcscmp | wcscoll
 * @since Huawei LiteOS V100R001C00
 */
size_t wcsxfrm (wchar_t *__restrict, const wchar_t *__restrict, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcschr() function searches the first occurrence of c in the
 * wide-character string pointed to by s.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* The wcschr() function returns a pointer to the first occurrence of c in
 * the wide-character string pointed to by s, or NULL if wc does not  occur in the string.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strchr | wmemchr
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wcschr (const wchar_t *, wchar_t);
wchar_t *wcsrchr (const wchar_t *, wchar_t);

size_t wcscspn (const wchar_t *, const wchar_t *);
size_t wcsspn (const wchar_t *, const wchar_t *);
wchar_t *wcspbrk (const wchar_t *, const wchar_t *);

wchar_t *wcstok (wchar_t *__restrict, const wchar_t *__restrict, wchar_t **__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcslen() function shall compute the number of wide-character
 * codes in the wide-character string to which s points,
 * not including the terminating null wide-character code.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The wcslen() function shall return the length of s.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strlen
 * @since Huawei LiteOS V100R001C00
 */
size_t wcslen (const wchar_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The  wcsstr() function is the wide-character equivalent of the strstr() function. It searches for the first occurrence of
 * the wide-character string __needle (without its terminating null wide character (L'\0')) as  a  substring  in
 * the wide-character string __haystack.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* The wcsstr() function returns a pointer to the first occurrence of __needle in __haystack.
 *                            It returns NULL if needle does not occur as a substring in __haystack.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbsinit | wcrtomb | wcsnrtombs | wcstombs
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wcsstr (const wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcswcs (const wchar_t *, const wchar_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wmemchr() searches the n wide characters starting at s for the
 * first occurrence of the wide character c.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* The wmemchr() function returns a pointer to the first occurrence of c among
 * the n wide characters starting at s, or NULL if c does not occur among these.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see memchr | wcschr
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wmemchr (const wchar_t *, wchar_t, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wmemcmp() function compares the n wide-characters starting at s1
 * and the n wide-characters starting at s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The wmemcmp() function returns zero if the wide-character arrays
 * of size n at s1 and s2 are equal. It returns an integer greater than zero if
 * at the first  differing position  i (i < n), the corresponding wide-character
 * s1[i] is greater than s2[i]. It returns an integer less than zero if at the first
 * differing position i (i < n), the corresponding wide-character s1[i] is less than s2[i].
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see memcmp | wcscmp
 * @since Huawei LiteOS V100R001C00
 */
int wmemcmp (const wchar_t *, const wchar_t *, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wmemcpy() function copies n wide characters from the array starting at
 * s to the array starting at d. The programmer must ensure that there is room
 * for at least n wide characters at d.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* wmemcpy() returns the address of d.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see memcpy | wmemmove
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wmemcpy (wchar_t *__restrict, const wchar_t *__restrict, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wmemmove() function is the wide-character equivalent of the memmove() function.
 * It copies n wide characters from the array starting at src to the array starting at dest.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* wmemmove() returns dest.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see memmove | wmemcpy
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wmemmove (wchar_t *, const wchar_t *, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wmemset() function fills the array of n wide-characters
 * starting at s with n copies of the wide character c.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wchar_t* wmemset() returns the address of s.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see memset
 * @since Huawei LiteOS V100R001C00
 */
wchar_t *wmemset (wchar_t *, wchar_t, size_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The btowc() function converts single byte to wide character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wint_t The btowc() function shall return WEOF if c has the value EOF or if c
 * does not constitute a valid character in the initial shift state. Otherwise, it shall
 * return the wide-character representation of that character.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see wctob
 * @since Huawei LiteOS V100R001C00
 */
wint_t btowc (int);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wctob() function tests whether the multibyte representation of
 * the wide character wc, starting in the initial state, consists of a
 * single byte. If so, it is returned as an unsigned char.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The wctob() function returns the single-byte representation of wc,
 * if it exists, of EOF otherwise.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see btowc | wcrtomb | wctomb
 * @since Huawei LiteOS V100R001C00
 */
int wctob (wint_t);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The mbsinit() function determines conversion object status.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The mbsinit() returns nonzero if *ps is an initial state, or if ps is NULL.
 * Otherwise, it returns 0.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbrlen | mbrtowc | wcrtomb | mbsrtowcs | wcsrtombs
 * @since Huawei LiteOS V100R001C00
 */
int mbsinit (const mbstate_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The mbrtowc() function converts a character to a wide-character code.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The mbrtowc() function returns the number of bytes parsed
 * from the multibyte sequence starting at s, if a non-L'\0' wide character
 * was recognized. It returns 0, if a L'\0' wide character was recognized.
 * It returns (size_t) -1 and sets errno to EILSEQ, if an invalid multibyte
 * sequence was encountered. It returns (size_t) -2 if it couldn't parse a
 * complete multibyte character, meaning that n should be increased.
 *
 * @par Errors
 * <ul>
 * <li><b>EILSEQ</b>: An invalid character sequence is detected.</li>
 * <li><b>EINVAL</b>: ps points to an object that contains an invalid conversion state.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbsinit | mbsrtowcs
 * @since Huawei LiteOS V100R001C00
 */
size_t mbrtowc (wchar_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcrtomb() function converts a wide-character code to a character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The wcrtomb() function returns the number of bytes that have been or
 * would have been written to the byte array at s. If wc can not be represented as a multibyte
 * sequence (according to the current locale), (size_t) -1 is returned, and errno set to EILSEQ.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbsinit | wcsrtombs
 * @since Huawei LiteOS V100R001C00
 */
size_t wcrtomb (char *__restrict, wchar_t, mbstate_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * If s is not a null pointer, mbrlen() shall determine the number of bytes constituting
 * the character pointed to by s. It shall be equivalent:\n
 *           mbstate_t internal;\n
 *           mbrtowc(NULL, s, n, ps != NULL ? ps : &internal);\n
 * If ps is a null pointer, the mbrlen() function shall use its own
 * internal mbstate_t object, which is initialized at program start-up to the initial
 * conversion state. Otherwise, the mbstate_t object pointed to by ps shall be used to
 * completely describe the current conversion state of the associated character sequence.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The mbrlen() function returns the number of bytes parsed
 * from the multibyte sequence starting at s, if a non-null wide character was
 * recognized. It returns 0, if a null wide character was recognized. It returns (size_t) -1
 * and sets errno to EILSEQ, if an invalid multibyte sequence was encountered.
 * It returns (size_t) -2 if it couldn't parse a complete multibyte character,
 * meaning that n should be increased.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbrtowc
 * @since Huawei LiteOS V100R001C00
 */
size_t mbrlen (const char *__restrict, size_t, mbstate_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The mbsrtowcs() function converts a character string to a wide-character string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The mbsrtowcs() function returns the number of wide characters that
 * make up the converted part of the wide-character string, not including the terminating null
 * wide character. If an invalid multibyte sequence was encountered, (size_t)-1 is returned,
 * and errno set to EILSEQ.
 *
 * @par Errors
 * <ul>
 * <li><b>EILSEQ</b>: An invalid character sequence is detected.</li>
 * <li><b>EINVAL</b>: ps points to an object that contains an invalid conversion state.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbrtowc | mbsinit | mbsnrtowcs
 * @since Huawei LiteOS V100R001C00
 */
size_t mbsrtowcs (wchar_t *__restrict, const char **__restrict, size_t, mbstate_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsrtombs() function convert a wide-character string to a multibyte string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The wcsrtombs() function returns the number of bytes that make up
 * the converted part of multibyte sequence, not including the terminating null byte.
 * If a wide character was encountered which could not be converted, (size_t) -1
 * is returned, and errno set to EILSEQ.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbsinit | wcrtomb | wcsnrtombs | wcstombs
 * @since Huawei LiteOS V100R001C00
 */
size_t wcsrtombs (char *__restrict, const wchar_t **__restrict, size_t, mbstate_t *__restrict);

float wcstof (const wchar_t *__restrict, wchar_t **__restrict);
double wcstod (const wchar_t *__restrict, wchar_t **__restrict);
long double wcstold (const wchar_t *__restrict, wchar_t **__restrict);

long wcstol (const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long wcstoul (const wchar_t *__restrict, wchar_t **__restrict, int);

long long wcstoll (const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long long wcstoull (const wchar_t *__restrict, wchar_t **__restrict, int);



int fwide (FILE *, int);


int wprintf (const wchar_t *__restrict, ...);
int fwprintf (FILE *__restrict, const wchar_t *__restrict, ...);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The swprintf() functione prints formatted wide-character output.
 *
 * @attention
 * <ul>
 * <li>Liteos do not support the function.</li>
 * </ul>
 *
 * @retval #int The swprintf() function returns the number of wide characters written,
 * excluding the terminating null wide character.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see printf
 * @since Huawei LiteOS V100R001C00
 */
int swprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);

int vwprintf (const wchar_t *__restrict, __isoc_va_list);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The vfwprintf() function performs wide-character output to stream.
 * stream must not be byte oriented.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vfwprintf() function returns the number of wide characters written,
 * excluding the terminating null wide character. It returns -1 when an error occurs.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fprintf | fputwc | printf | snprintf
 * @since Huawei LiteOS V100R001C00
 */
int vfwprintf (FILE *__restrict, const wchar_t *__restrict, __isoc_va_list);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The vswprintf() function performs wide-character output to an array of wide characters.
 * The programmer must ensure that there is room for at least n wide characters at s.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vswprintf() function returns the number of wide characters written,
 * excluding the terminating null wide character. It returns -1 when an error occurs.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fprintf | fputwc | | printf | snprintf
 * @since Huawei LiteOS V100R001C00
 */
int vswprintf (wchar_t *__restrict, size_t, const wchar_t *__restrict, __isoc_va_list);

int wscanf (const wchar_t *__restrict, ...);
int fwscanf (FILE *__restrict, const wchar_t *__restrict, ...);
int swscanf (const wchar_t *__restrict, const wchar_t *__restrict, ...);

int vwscanf (const wchar_t *__restrict, __isoc_va_list);
int vfwscanf (FILE *__restrict, const wchar_t *__restrict, __isoc_va_list);
int vswscanf (const wchar_t *__restrict, const wchar_t *__restrict, __isoc_va_list);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The fgetwc() function reads a wide character from stream and returns it.
 * If the end of stream is reached, it returns WEOF. If a wide-character
 * conversion error occurs, it sets errno to EILSEQ and returns WEOF.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wint_t The fgetwc() function returns the next wide-character from the stream,
 * or WEOF. In the event of an error, errno is set to indicate the cause.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * <li><b>EILSEQ</b>: The data obtained from the input stream does not form a valid character.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fputwc | ungetwc
 * @since Huawei LiteOS V100R001C00
 */
wint_t fgetwc (FILE *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The getwc() function identically to fgetwc().
 *
 * @attention
 * <ul>
 * <li>Refer to fgetwc().</li>
 * </ul>
 *
 * @retval #wint_t The getwc() function returns the next wide-character from the
 * stream, or WEOF. In the event of an error, errno is set to indicate the cause.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * <li><b>EILSEQ</b>: The data obtained from the input stream does not form a valid character.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fputwc | ungetwc
 * @since Huawei LiteOS V100R001C00
 */
wint_t getwc (FILE *);
wint_t getwchar (void);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The fputwc() function writes the wide character wc to stream. If a wide-character
 * conversion error occurs, it sets errno to EILSEQ and returns WEOF.
 * Otherwise, it returns wc.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wint_t The fputwc() function returns wc if no error occurred, or WEOF
 * to indicate an error. In the event of an error, errno is set to indicate the cause.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * <li><b>EILSEQ</b>: Conversion of wc to the stream's encoding fails.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fgetwc
 * @since Huawei LiteOS V100R001C00
 */
wint_t fputwc (wchar_t, FILE *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The putwc() function identically to fputwc().
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wint_t The putwc() function returns wc if no error occurred,
 * or WEOF to indicate an error. In the event of an error, errno is set
 * to indicate the cause.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * <li><b>EILSEQ</b>: Conversion of wc to the stream's encoding fails.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fgetwc
 * @since Huawei LiteOS V100R001C00
 */
wint_t putwc (wchar_t, FILE *);
wint_t putwchar (wchar_t);

wchar_t *fgetws (wchar_t *__restrict, int, FILE *__restrict);
int fputws (const wchar_t *__restrict, FILE *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The ungetwc() function is the wide-character equivalent of the ungetc() function.
 * It pushes back a wide character onto stream and returns it. If wc is WEOF, it returns WEOF.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #wint_t The ungetwc() function returns wc when successful, or WEOF upon failure.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see fgetwc
 * @since Huawei LiteOS V100R001C00
 */
wint_t ungetwc (wint_t, FILE *);

struct tm;

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsftime() function converts date and time to a wide-character string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t If the total number of resulting wide-character codes
 * including the terminating null wide-character code is no more than maxsize,
 * wcsftime() shall return the number of wide-character codes placed into the
 * array pointed to by s, not including the terminating null wide-character code.
 * Otherwise, zero is returned and the contents of the array are unspecified.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strftime
 * @since Huawei LiteOS V100R001C00
 */
size_t wcsftime (wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict);

#undef iswdigit

#if defined(_GNU_SOURCE)
wint_t fgetwc_unlocked (FILE *);
wint_t getwc_unlocked (FILE *);
wint_t getwchar_unlocked (void);
wint_t fputwc_unlocked (wchar_t, FILE *);
wint_t putwc_unlocked (wchar_t, FILE *);
wint_t putwchar_unlocked (wchar_t);
wchar_t *fgetws_unlocked (wchar_t *__restrict, int, FILE *__restrict);
int fputws_unlocked (const wchar_t *__restrict, FILE *__restrict);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
size_t wcsftime_l (wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict, locale_t);
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)  || defined(_BSD_SOURCE)
FILE *open_wmemstream(wchar_t **, size_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The mbsnrtowcs() function converts a character string to a wide-character string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The mbsnrtowcs() function returns the number of wide characters that
 * make up the converted part of the wide-character string, not including the terminating null
 * wide character. If an invalid multibyte sequence was encountered, (size_t)-1 is returned,
 * and errno set to EILSEQ.
 *
 * @par Errors
 * <ul>
 * <li><b>EILSEQ</b>: An invalid character sequence is detected.</li>
 * <li><b>EINVAL</b>: ps points to an object that contains an invalid conversion state.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbrtowc | mbsinit | mbsrtowcs
 * @since Huawei LiteOS V100R001C00
 */
size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t, mbstate_t *__restrict);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsnrtombs() function converts a wide-character string to a multibyte string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The wcsnrtombs() function returns the number of bytes that make up
 * the converted part of multibyte sequence, not including the terminating null byte.
 * If a wide character was encountered which could not be converted, (size_t) -1
 * is returned, and errno set to EILSEQ.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see mbsinit | wcsrtombs
 * @since Huawei LiteOS V100R001C00
 */
size_t wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t, mbstate_t *__restrict);
wchar_t *wcsdup(const wchar_t *);

/**
 * @ingroup  wchar
 *
 * @par Description:
 * The wcsnlen() function is the wide-character equivalent of the strnlen() function. It returns the number of wide-
 * characters in the string pointed to by s, not including the terminating null wide character  (L'\0'),  but  at  most
 * maxlen wide characters (note: this parameter is not a byte count). In doing this, wcsnlen() looks at only the first
 * maxlen wide characters at s and never beyond s+maxlen.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The wcsnlen() function returns wcslen(s), if that is less than maxlen, or maxlen if there is no null wide  character
 *                        among the first maxlen wide characters pointed to by s.
 *
 * @par Dependency:
 * <ul><li>wchar.h</li></ul>
 * @see strnlen | wcslen
 * @since Huawei LiteOS V100R001C00
 */
size_t wcsnlen (const wchar_t *, size_t);
wchar_t *wcpcpy (wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcpncpy (wchar_t *__restrict, const wchar_t *__restrict, size_t);
int wcscasecmp(const wchar_t *, const wchar_t *);
int wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t);
int wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
int wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t);
int wcscoll_l(const wchar_t *, const wchar_t *, locale_t);
size_t wcsxfrm_l(wchar_t *__restrict, const wchar_t *__restrict, size_t, locale_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int wcwidth (wchar_t);
int wcswidth (const wchar_t *, size_t);
int       iswalnum(wint_t);
int       iswalpha(wint_t);
int       iswblank(wint_t);
int       iswcntrl(wint_t);
int       iswdigit(wint_t);
int       iswgraph(wint_t);
int       iswlower(wint_t);
int       iswprint(wint_t);
int       iswpunct(wint_t);
int       iswspace(wint_t);
int       iswupper(wint_t);
int       iswxdigit(wint_t);
int       iswctype(wint_t, wctype_t);
wint_t    towlower(wint_t);
wint_t    towupper(wint_t);
wctype_t  wctype(const char *);

#ifndef __cplusplus
#undef iswdigit
#define iswdigit(a) (0 ? iswdigit(a) : ((unsigned)(a)-'0') < 10)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
