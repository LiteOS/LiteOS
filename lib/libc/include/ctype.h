/**
 * @defgroup ctype Ctype
 * @ingroup libc
 */

#ifndef	_CTYPE_H
#define	_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isalnum() function tests whether c is an alphanumeric character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isalnum() function shall return non-zero if c is an alphanumeric character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalpha | isblank | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isalnum(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isalpha() function tests whether c is an alphabetic character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isalpha() function shall return non-zero if c is an alphabetic character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isalpha(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isblank() function tests whether c is a character of class blank.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The value returned is nonzero if the character c falls
 * into the tested class, and zero if not.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalnum | isalpha | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isspace | isupper | isxdigit
 * @since Huawei LiteOS V100R001C00
 */
int   isblank(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The iscntrl() function tests whether c is a control character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The iscntrl() function shall return non-zero if c is a control character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | isdigit | isgraph | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   iscntrl(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isdigit() function tests whether c is a decimal digit.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isdigit() function shall return non-zero if c is a decimal digit;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | iscntrl | isgraph | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isdigit(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isgraph() function tests whether c is a visible character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isgraph() function shall return non-zero if c is a visible character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | iscntrl | isdigit | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isgraph(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The islower() function tests whether c is a lowercase letter.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The islower() function shall return non-zero if c is a lowercase letter;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | iscntrl | isdigit | isgraph | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   islower(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isprint() function tests whether c is a printable character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isprint() function shall return non-zero if c is a printable character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | iscntrl | isdigit | isgraph | islower | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isprint(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The ispunct() function tests whether c is a punctuation character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The ispunct() function shall return non-zero if c is a punctuation character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isspace | isalnum | isblank | isalpha | iscntrl | isdigit | isgraph | islower | isprint | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   ispunct(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isspace() function tests whether c is a white-space character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isspace() function shall return non-zero if c is a white-space character;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalnum | isalpha | isblank | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isspace(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isupper() function tests whether c is an uppercase letter.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isupper() function shall return non-zero if c is an uppercase letter;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalnum | isalpha | isblank | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isspace
 * @since Huawei LiteOS V100R001C00
 */
int   isupper(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isxdigit() function tests whether c is a hexadecimal digit.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isxdigit() function shall return non-zero if c is a hexadecimal digit;
 * otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalnum | isalpha | isblank | iscntrl | isdigit | isgraph | islower | isprint | ispunct | isspace | isupper
 * @since Huawei LiteOS V100R001C00
 */
int   isxdigit(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The tolower() function converts uppercase letters to lowercase.
 * If c is an uppercase letter, tolower() returns its lowercase
 * equivalent. Otherwise, it returns c.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The value returned is that of the converted letter, or c if the
 * conversion was not possible.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalpha | towlower | towupper
 * @since Huawei LiteOS V100R001C00
 */
int   tolower(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The toupper() function converts lowercase letters to uppercase.
 * If c is a lowercase letter, toupper() returns its uppercase
 * equivalent. Otherwise, it returns c.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The value returned is that of the converted letter, or c if the
 * conversion was not possible.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isalpha | towlower | towupper
 * @since Huawei LiteOS V100R001C00
 */
int   toupper(int);

#ifndef __cplusplus
static __inline int __isspace(int _c)
{
	return _c == ' ' || (unsigned)_c-'\t' < 5;
}

#define isalpha(a) (0 ? isalpha(a) : (((unsigned)(a)|32)-'a') < 26)
#define isdigit(a) (0 ? isdigit(a) : ((unsigned)(a)-'0') < 10)
#define islower(a) (0 ? islower(a) : ((unsigned)(a)-'a') < 26)
#define isupper(a) (0 ? isupper(a) : ((unsigned)(a)-'A') < 26)
#define isprint(a) (0 ? isprint(a) : ((unsigned)(a)-0x20) < 0x5f)
#define isgraph(a) (0 ? isgraph(a) : ((unsigned)(a)-0x21) < 0x5e)
#define isspace(a) __isspace(a)
#endif


#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

#define __NEED_locale_t
#include <bits/alltypes.h>

int   isalnum_l(int, locale_t);
int   isalpha_l(int, locale_t);
int   isblank_l(int, locale_t);
int   iscntrl_l(int, locale_t);
int   isdigit_l(int, locale_t);
int   isgraph_l(int, locale_t);
int   islower_l(int, locale_t);
int   isprint_l(int, locale_t);
int   ispunct_l(int, locale_t);
int   isspace_l(int, locale_t);
int   isupper_l(int, locale_t);
int   isxdigit_l(int, locale_t);
int   tolower_l(int, locale_t);
int   toupper_l(int, locale_t);


/**
 * @ingroup  ctype
 *
 * @par Description:
 * The isascii() function tests whether c is a 7-bit US-ASCII character code.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The isascii() function shall return non-zero if c is a 7-bit US-ASCII
 * character code between 0 and octal 0177 inclusive; otherwise, it shall return 0.
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  toascii
 * @since Huawei LiteOS V100R001C00
 */
int   isascii(int);

/**
 * @ingroup  ctype
 *
 * @par Description:
 * The toascii() function converts its argument into a 7-bit ASCII character.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The toascii() function shall return the value (c &0177).
 *
 * @par Dependency:
 * <ul><li>ctype.h</li></ul>
 * @see  isascii
 * @since Huawei LiteOS V100R001C00
 */
int   toascii(int);
#define _tolower(a) ((a)|0x20)
#define _toupper(a) ((a)&0x5f)
#define isascii(a) (0 ? isascii(a) : (unsigned)(a) < 128)

#endif

#ifdef __cplusplus
}
#endif

#endif
