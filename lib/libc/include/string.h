/**
 * @defgroup libc Libc
 * @defgroup string String
 * @ingroup libc
 */

#ifndef	_STRING_H
#define	_STRING_H

#ifdef __cplusplus
extern "C" {
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
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
#define __NEED_locale_t
#endif

#include <bits/alltypes.h>

/**
 * @ingroup  string
 *
 * @par Description:
 * The memcpy() function copies n bytes from memory area src to memory area dest.
 * The memory areas must not overlap.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void* The memcpy() function returns a pointer to dest.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memmove | strcpy | strncpy | wmemcpy
 * @since Huawei LiteOS V100R001C00
 */
void *memcpy (void *__restrict, const void *__restrict, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The memmove() function copies n bytes from memory area src to memory area dest.
 * The memory areas may overlap: copying takes place as though the bytes in src are
 * first copied into a temporary array that does not overlap src or dest, and
 * the bytes are then copied from the temporary array to dest.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void* The memmove() function returns a pointer to dest.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strcpy | strncpy | wmemcpy
 * @since Huawei LiteOS V100R001C00
 */
void *memmove (void *, const void *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The memset() function fills the first n bytes of the memory area
 * pointed to by s with the constant byte c.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void* The memset() function returns a pointer to the memory area s.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  wmemset
 * @since Huawei LiteOS V100R001C00
 */
void *memset (void *, int, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The memcmp() function compares the first n bytes (each interpreted as unsigned char)
 * of the memory areas s1 and s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The memcmp() function returns 0 if the parameters s1 and s2 are identical.
 * If s1 is greater than s2, a value greater than 0 is returned.
 * If s1 is less than s2, a value less than 0 is returned.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strcasecmp | strcmp | strcoll | strncasecmp | strncmp | wmemcmp
 * @since Huawei LiteOS V100R001C00
 */
int memcmp (const void *, const void *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The memchr() function scans the initial n bytes of the memory area
 * pointed to by src for the first instance of c. Both c and the bytes of
 * the memory area pointed to by src are interpreted as unsigned char.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void* The memchr() function returns a pointer to the matching
 *         byte or NULL if the character does not occur in the given memory
 *         area.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see strchr | strpbrk | strrchr | strsep | strspn | strstr | wmemchr
 * @since Huawei LiteOS V100R001C00
 */
void *memchr (const void *, int, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcpy() function copies the string pointed to by from, including
 * the terminating null byte ("\0"), to the buffer pointed to by to.
 * The strings may not overlap, and the destination string to must be
 * large enough to receive the copy. Beware of buffer overruns!
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The function returns a pointer to the resulting string to.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcpy | stpcpy | strdup
 * @since Huawei LiteOS V100R001C00
 */
char *strcpy (char *__restrict, const char *__restrict);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strncpy() function shall copy not more than n
 * bytes (bytes that follow a NUL character are not copied) from the
 * array pointed to by src to the array pointed to by dst.
 * If the array pointed to by src is a string that is shorter than n
 * bytes, NUL characters shall be appended to the copy in the array
 * pointed to by dst, until n bytes in all are written.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The strncpy() function shall return dst.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strcpy | wcsncpy
 * @since Huawei LiteOS V100R001C00
 */
char *strncpy (char *__restrict, const char *__restrict, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcat() function appends the src string to the dst string,
 * overwriting the terminating null byte ("\0") at the end of dst, and
 * then adds a terminating null byte. The strings may not overlap, and
 * the dst string must have enough space for the result. If dst is
 * not large enough, program behavior is unpredictable.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The function returns a pointer to the resulting string dst.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcpy | strcpy | strncpy
 * @since Huawei LiteOS V100R001C00
 */
char *strcat (char *__restrict, const char *__restrict);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strncat() function will use at most n bytes from src and src does not
 * need to be null-terminated if it contains n or more bytes. As with strcat(),
 * the resulting string in dst is always null-terminated. If src contains n or more bytes,
 * strncat() writes n+1 bytes to dst(n from src plus the terminating null byte).
 * Therefore, the size of dst must be at least strlen(dst)+n+1.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The strncat() function returns pointer to the
 *       resulting string dst.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcpy | strcpy | strncpy
 * @since Huawei LiteOS V100R001C00
 */
char *strncat (char *__restrict, const char *__restrict, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcmp() function shall compare the string pointed to by s1 to the
 * string pointed to by s2. The sign of a non-zero return value shall be
 * determined by the sign of the difference between the values of the first
 * pair of bytes (both interpreted as type unsigned char) that differ in the
 * strings being compared.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon completion, strcmp() shall return an integer greater than,
 * equal to, or less than 0, if the string pointed to by s1 is greater than, equal to,
 * or less than the string pointed to by s2, respectively.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strncmp
 * @since Huawei LiteOS V100R001C00
 */
int strcmp (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strncmp() function compares only the first (at most) n bytes of s1 and s2.
 * It returns an integer less than, equal to, or greater than zero if s1 is found,
 * respectively, to be less than, to match, or be greater than s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The strncmp() function returns an integer less than,
 *       equal to, or greater than zero if s1 (or the first n bytes thereof)
 *       is found, respectively, to be less than, to match, or be greater than
 *       s2.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcmp | strcasecmp | strcoll | strncasecmp | wcscmp | wcsncmp
 * @since Huawei LiteOS V100R001C00
 */
int strncmp (const char *, const char *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcoll() function compares the two strings s1 and s2. It
 * returns an integer less than, equal to, or greater than zero if s1 is
 * found, respectively, to be less than, to match, or be greater than s2.
 *
 * @attention
 * <ul>
 * <li>Liteos does not compare strings according to the order in which
 * text is specified by LC_COLLATE.</li>
 * </ul>
 *
 * @retval #int The strcoll() function returns an integer less than, equal to, or
 *       greater than zero if s1 is found, respectively, to be less than, to
 *       match, or be greater than s2, when both are interpreted as
 *       appropriate for the current locale.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcmp | setlocale | strcasecmp | strcmp | strxfrm
 * @since Huawei LiteOS V100R001C00
 */
int strcoll (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 *
 * The strxfrm() function Transforms the string src and places the result into dst. It copies at most
 * n characters into dst including the null terminating character. The
 * transformation occurs such that strcmp applied to two separate converted
 * strings returns the same value as strcoll applied to the same two strings. If
 * overlapping occurs, the result is undefined.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The strxfrm() function returns the number of bytes required to store
 * the transformed string in dst excluding the terminating null byte
 * ("\0"). If the value returned is n or more, the contents of dst are
 * indeterminate.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcmp | setlocale | strcasecmp | strcmp | strcoll
 * @since Huawei LiteOS V100R001C00
 */
size_t strxfrm (char *__restrict, const char *__restrict, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strchr() function returns a pointer to the first occurrence of the character c in the string s.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* Upon completion, strchr() shall return a pointer to the byte, or a null pointer
 * if the byte was not found.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcpy | memmove | stpcpy | strdup | wcsncpy
 * @since Huawei LiteOS V100R001C00
 */
char *strchr (const char *, int);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strrchr() function returns a pointer to the last occurrence of
 * the character c in the string s.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The strrchr() function returns a pointer to the matched
 *       character or NULL if the character is not found. The terminating
 *       null byte is considered part of the string, so that if c is specified
 *       as "\0", the function returns a pointer to the terminator.
 *
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strlen | strpbrk | strsep | strspn | strstr | strtok | wcschr
 * @since Huawei LiteOS V100R001C00
 */
char *strrchr (const char *, int);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcspn() function calculates the length of the initial segment
 * of s1 which consists entirely of bytes not in s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The strcspn() function returns the number of bytes in the initial
 *       segment of s1 which are not in the string s2.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strchr | strpbrk | strsep | strstr | strtok
 * @since Huawei LiteOS V100R001C00
 */
size_t strcspn (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strspn() function calculates the length (in bytes) of the initial
 *       segment of s1 which consists entirely of bytes in s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The strspn() function returns the number of bytes in the initial
 *      segment of s1 which consist only of bytes from s2.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strchr | strpbrk | strsep | strstr | strtok
 * @since Huawei LiteOS V100R001C00
 */
size_t strspn (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strpbrk() function shall locate the first occurrence in the
 *       string pointed to by s1 of any byte from the string pointed to by s2.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* Upon successful completion, strpbrk() shall return a pointer to the
 *      byte or a null pointer if no byte from s2 occurs in s1.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strchr | strrchr
 * @since Huawei LiteOS V100R001C00
 */
char *strpbrk (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strstr() function finds the first occurrence of the substring
 * find in the string s. The terminating null bytes ("\0") are
 * not compared.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The function returns a pointer to the beginning of the located
 *               substring, or NULL if the substring is not found.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strcasecmp | strchr | strpbrk | strsep | strspn | strtok
 * @since Huawei LiteOS V100R001C00
 */
char *strstr (const char *, const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strtok() function breaks a string into a sequence of zero or more
 *       nonempty tokens. On the first call to strtok(), the string to be
 *       parsed should be specified in str. In each subsequent call that
 *       should parse the same string, str must be NULL.
 *       The delim argument specifies a set of bytes that delimit the tokens
 *       in the parsed string. The caller may specify different strings in
 *       delim in successive calls that parse the same string.
 *       Each call to strtok() returns a pointer to a null-terminated string
 *       containing the next token. This string does not include the
 *       delimiting byte. If no more tokens are found, strtok() returns NULL.
 *       A sequence of calls to strtok() that operate on the same string
 *       maintains a pointer that determines the point from which to start
 *       searching for the next token. The first call to strtok() sets this
 *       pointer to point to the first byte of the string. The start of the
 *       next token is determined by scanning forward for the next
 *       nondelimiter byte in str. If such a byte is found, it is taken as
 *       the start of the next token. If no such byte is found, then there
 *       are no more tokens, and strtok() returns NULL. (A string that is
 *       empty or that contains only delimiters will thus cause strtok() to
 *       return NULL on the first call.)
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The strtok() function returns a pointer to the next
 *       token, or NULL if there are no more tokens.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strchr | strpbrk | strsep | strspn | strstr
 * @since Huawei LiteOS V100R001C00
 */
char *strtok (char *__restrict, const char *__restrict);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strlen() function calculates the length of the string s,
 * excluding the terminating null byte ("\0").
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The strlen() function returns the number of bytes in the string s.
 *
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see strnlen | wcslen
 *
 * @since Huawei LiteOS V100R001C00
 */
size_t strlen (const char *);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strerror() function returns a pointer to a string that describes
 * the error code passed in the argument error_number. (For example, if error_number is EINVAL,
 * the returned description will be "Invalid argument".) This string must not be
 * modified by the application, but may be modified by a subsequent call
 * to strerror(). No other library function, including
 * perror(), will modify this string.
 *
 * @attention
 * <ul>
 * <li>The errno is not to be set in Liteos.</li>
 * </ul>
 *
 * @retval #char* The strerror() function returns the appropriate error description string,
 * or return NULL if can not find error description string.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see   perror
 * @since Huawei LiteOS V100R001C00
 */
char *strerror (int);

#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
#include <strings.h>
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

/**
 * @ingroup  string
 *
 * @par Description:
 * The strtok_r() function is a reentrant version strtok(). The saveptr
 *   argument is a pointer to a char* variable that is used internally by
 *   strtok_r() in order to maintain context between successive calls that
 *   parse the same string.
 *   On the first call to strtok_r(), str should point to the string to be
 *   parsed, and the value of saveptr is ignored. In subsequent calls,
 *   str should be NULL, and saveptr should be unchanged since the
 *   previous call.
 *   Different strings may be parsed concurrently using sequences of calls
 *   to strtok_r() that specify different saveptr arguments.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* The strtok_r() function returns a pointer to the next
 *       token, or NULL if there are no more tokens.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strchr | strpbrk | strsep | strspn | strstr
 * @since Huawei LiteOS V100R001C00
 */
char *strtok_r (char *__restrict, const char *__restrict, char **__restrict);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strerror_r() function shall map the error number in error_number
 * to a locale-dependent error message string and shall return the string
 * in the buffer pointed to by buf, with length buf_len. If the value of
 * error_number is a valid error number, the message string shall indicate
 * what error occurred; if the value of error_number is zero, the message
 * string shall either be an empty string or indicate that no error occurred;
 * otherwise, if these functions complete successfully, the message string
 * shall indicate that an unknown error occurred.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, strerror_l() shall return a pointer
 * to the generated message string. If error_number is not a valid error number,
 * errno may be set to [EINVAL], but a pointer to a message string shall still
 * be returned. If any other error occurs, errno shall be set to indicate the
 * error and a null pointer shall be returned.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  perror
 * @since Huawei LiteOS V100R001C00
 */
int strerror_r (int, char *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The stpcpy() function copies the string pointed to by src
 * (including the terminating null byte ("\0")) to the array pointed to by dest.
 * The strings may not overlap, and the destination string dest must be large
 * enough to receive the copy.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* stpcpy() returns a pointer to the end of the string dest
 * (that is, the address of the terminating null byte) rather than the beginning.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memcpy | memmove | strcpy
 * @since Huawei LiteOS V100R001C00
 */
char *stpcpy(char *__restrict, const char *__restrict);
char *stpncpy(char *__restrict, const char *__restrict, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strnlen() function returns the number of characters in the string
 * pointed to by str, excluding the terminating null byte ("\0"), but at
 * most maxlen. In doing this, strnlen() looks only at the first maxlen
 * characters in the string pointed to by str and never beyond str+maxlen.
 *
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The strnlen() function returns the number of characters in the string
 * pointed to by str, if that is less than maxlen, or maxlen if there is no null
 * terminating ("\0") among the first maxlen characters pointed to by str.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  strlen
 * @since Huawei LiteOS V100R001C00
 */
size_t strnlen (const char *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strdup() function returns a pointer to a new string which is a
 * duplicate of the string str. Memory for the new string is obtained
 * with malloc(), and can be freed with free().
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* On success, the strdup() function returns a pointer to the duplicated
 * string. It returns NULL if insufficient memory was available.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  calloc | free | malloc | realloc
 * @since Huawei LiteOS V100R001C00
 */
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *__restrict, const char *__restrict, size_t, locale_t);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
void *memccpy (void *__restrict, const void *__restrict, int, size_t);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

/**
 * @ingroup  string
 *
 * @par Description:
 * Decompose a string into a set of strings. Scan backwards from the position pointed
 * to by stringp. After encountering the character in the string pointed to by delim,
 * replace this character with NULL and return the address pointed to by stringp.
 *
 * @attention
 * <ul>
 * <li>
 * The strsep() function is not a standard C Library Function.
 * </li>
 * </ul>
 *
 * @retval #char* Returns a substring starting at the beginning of stringp, and returns
 * NULL when there is no substring that is split.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strchr | strspn | strstr | strtok
 * @since Huawei LiteOS V100R001C00
 */
char *strsep(char **, const char *);
size_t strlcat (char *, const char *, size_t);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strlcpy() function copies and concatenate strings. It is
 * designed to be safer, more consistent, and less error prone replacements
 * for the easily misused functions strncpy() and.
 * strlcpy() takes the full size	of the destination buffer and
 * guarantee NUL-termination if there is room. Note that room for the NUL
 * should be included in siz. strlcpy() copies up to siz - 1 characters from
 * the string src to dst, NUL-terminating the result if siz is not 0.
 *
 * @attention
 * <ul>
 * <li>
 * The strlcpy() function is not a standard C Library Function.
 * </li>
 * </ul>
 *
 * @retval #size_t the strlcpy() function returns the	total
 * length of the string they tried to create. For strlcpy() that means the
 * length of src. If the return value is >= siz, the output string has been truncated.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  snprintf | strncat | strncpy | wcslcpy
 * @since Huawei LiteOS V100R001C00
 */
size_t strlcpy (char *, const char *, size_t);
void explicit_bzero (void *, size_t);
#endif

#ifdef _GNU_SOURCE
#define	strdupa(x)	strcpy(alloca(strlen(x)+1),x)
int strverscmp (const char *, const char *);
char *strchrnul(const char *, int);

/**
 * @ingroup  string
 *
 * @par Description:
 * The strcasestr() function finds the first occurrence of the substring
 * find in the string s. The terminating null bytes ("\0") are not compared.
 *
 * @attention
 * <ul>
 * <li>
 * The strcasestr() function is not a standard C Library Function.
 * </li>
 * </ul>
 *
 * @retval #char* The function returns a pointer to the beginning of the substring,
 * or NULL if the substring is not found.
 *
 * @par Dependency:
 * <ul><li>string.h</li></ul>
 * @see  memchr | strcasecmp | strchr | strpbrk | strsep | strspn | strtok
 * @since Huawei LiteOS V100R001C00
 */
char *strcasestr(const char *, const char *);
void *memmem(const void *, size_t, const void *, size_t);
void *memrchr(const void *, int, size_t);
void *mempcpy(void *, const void *, size_t);
#ifndef __cplusplus
#ifndef __LITEOS__
char *basename();
#else
char *basename(char *);
#endif
#endif

#ifdef __LITEOS__
char *strnchr(const char *, size_t, int);
char* strnstr(const char *, const char *, size_t);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
