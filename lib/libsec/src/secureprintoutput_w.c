/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2020. All rights reserved.
 * Licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: By defining corresponding macro for UNICODE string and including "output.inl",
 *              this file generates real underlying function used by printf family API.
 * Author: lishunda
 * Create: 2014-02-25
 */

/* If some platforms don't have wchar.h, dont't include it */
#if !(defined(SECUREC_VXWORKS_PLATFORM))
/* If there is no macro above, it will cause compiling alarm */
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#ifndef _CRTIMP_ALTERNATIVE
#define _CRTIMP_ALTERNATIVE     /* Comment microsoft *_s function */
#endif
#ifndef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 0
#endif
#endif
#include <wchar.h>
#endif

/* Disable wchar func to clear vs warning */
#define SECUREC_ENABLE_WCHAR_FUNC   0
#define SECUREC_FORMAT_OUTPUT_INPUT 1

#ifndef SECUREC_FOR_WCHAR
#define SECUREC_FOR_WCHAR
#endif

#if defined(SECUREC_WARP_OUTPUT) && SECUREC_WARP_OUTPUT
#undef SECUREC_WARP_OUTPUT
#define SECUREC_WARP_OUTPUT 0
#endif

#include "secureprintoutput.h"

SECUREC_INLINE void SecWriteCharW(wchar_t ch, SecPrintfStream *f, int *pnumwritten);
SECUREC_INLINE int SecPutWcharStrEndingZero(SecPrintfStream *str, int zeroCount);

#include "output.inl"

/*
 * Wide character formatted output implementation
 */
int SecVswprintfImpl(wchar_t *string, size_t sizeInWchar, const wchar_t *format, va_list argList)
{
    SecPrintfStream str;
    int retVal; /* If initialization causes  e838 */

    str.cur = (char *)string;
    /* This count include \0 character, Must be greater than zero */
    str.count = (int)(sizeInWchar * sizeof(wchar_t));

    retVal = SecOutputSW(&str, format, argList);
    if (retVal >= 0) {
        if (SecPutWcharStrEndingZero(&str, (int)sizeof(wchar_t)) == 0) {
            return retVal;
        }
    }
    if (str.count < 0) {
        /* The buffer was too small, then truncate */
        string[sizeInWchar - 1] = L'\0';
        return SECUREC_PRINTF_TRUNCATE;
    }
    string[0] = L'\0'; /* Empty the dest string */
    return -1;
}

/*
 * Output a wide character zero end into the SecPrintfStream structure
 */
SECUREC_INLINE int SecPutWcharStrEndingZero(SecPrintfStream *str, int zeroCount)
{
    int count;
    for (count = zeroCount; count > 0; --count) {
        if (SecPutZeroChar(str) != 0) {
            return -1;
        }
    }
    return 0;
}

/*
 * Output a wide character into the SecPrintfStream structure
 */
SECUREC_INLINE int SecPutCharW(wchar_t ch, SecPrintfStream *f)
{
    f->count -= (int)sizeof(wchar_t); /* f -> count may be negative,indicating insufficient space */
    if (f->count >= 0) {
        *(wchar_t *)(void *)(f->cur) = ch;
        f->cur += sizeof(wchar_t);
        return 0;
    }
    return -1;
}

/*
 * Output a wide character into the SecPrintfStream structure, returns the number of characters written
 */
SECUREC_INLINE void SecWriteCharW(wchar_t ch, SecPrintfStream *f, int *pnumwritten)
{
    if (SecPutCharW(ch, f) == 0) {
        *pnumwritten = *pnumwritten + 1;
    } else {
        *pnumwritten = -1;
    }
}

/*
 * Output multiple wide character into the SecPrintfStream structure,  returns the number of characters written
 */
SECUREC_INLINE void SecWriteMultiCharW(wchar_t ch, int num, SecPrintfStream *f, int *pnumwritten)
{
    int count;
    for (count = num; count > 0; --count) {
        SecWriteCharW(ch, f, pnumwritten);
        if (*pnumwritten == -1) {
            break;
        }
    }
}

/*
 * Output a wide string into the SecPrintfStream structure,  returns the number of characters written
 */
SECUREC_INLINE void SecWriteStringW(const wchar_t *string, int len, SecPrintfStream *f, int *pnumwritten)
{
    const wchar_t *str = string;
    int count;
    for (count = len; count > 0; --count) {
        SecWriteCharW(*str, f, pnumwritten);
        ++str;
        if (*pnumwritten == -1) {
            break;
        }
    }
}

