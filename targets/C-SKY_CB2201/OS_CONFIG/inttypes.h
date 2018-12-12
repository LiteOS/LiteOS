/* Copyright (C) ARM Ltd., 1999,2014 */
/* All rights reserved */

/*
 * RCS $Revision$
 * Checkin $Date$
 * Revising $Author: agrant $
 */

/* Based on WG14/N843 (C9X) Committee Draft August 3, 1998 */

#ifndef __inttypes_h
#define __inttypes_h
#define __ARMCLIB_VERSION 5060037

#define _ARMABI 
#define _ARMABI_PURE 

#include <stdint.h>
#include <stddef.h>

#if !defined(__cplusplus) || defined(__STDC_FORMAT_MACROS) || 201103L <= __cplusplus

#ifdef __LP64__
  #define __PRISCN64 "l" /* 'int64_t' is 'long' */
#else
  #define __PRISCN64 "ll" /* 'int64_t' is 'long long' */
#endif
#if __sizeof_ptr == 8
  #define __PRISCNPTR __PRISCN64 /* 'intptr_t' is 64-bits */
#else
  #define __PRISCNPTR /*nothing*/ /* 'intptr_t' is 'int' */
#endif


    /* 7.8.1 */

#define PRId8         "d"
#define PRId16        "d"
#define PRId32        "d"
#define PRId64      __PRISCN64 "d"
#define PRIdLEAST8    "d"
#define PRIdLEAST16   "d"
#define PRIdLEAST32   "d"
#define PRIdLEAST64 __PRISCN64 "d"
#define PRIdFAST8     "d"
#define PRIdFAST16    "d"
#define PRIdFAST32    "d"
#define PRIdFAST64  __PRISCN64 "d"
#define PRIdMAX      "jd"
#define PRIdPTR     __PRISCNPTR "d"

#define PRIi8         "i"
#define PRIi16        "i"
#define PRIi32        "i"
#define PRIi64      __PRISCN64 "i"
#define PRIiLEAST8    "i"
#define PRIiLEAST16   "i"
#define PRIiLEAST32   "i"
#define PRIiLEAST64 __PRISCN64 "i"
#define PRIiFAST8     "i"
#define PRIiFAST16    "i"
#define PRIiFAST32    "i"
#define PRIiFAST64  __PRISCN64 "i"
#define PRIiMAX      "ji"
#define PRIiPTR     __PRISCNPTR "i"

#define PRIo8         "o"
#define PRIo16        "o"
#define PRIo32        "o"
#define PRIo64      __PRISCN64 "o"
#define PRIoLEAST8    "o"
#define PRIoLEAST16   "o"
#define PRIoLEAST32   "o"
#define PRIoLEAST64 __PRISCN64 "o"
#define PRIoFAST8     "o"
#define PRIoFAST16    "o"
#define PRIoFAST32    "o"
#define PRIoFAST64  __PRISCN64 "o"
#define PRIoMAX      "jo"
#define PRIoPTR     __PRISCNPTR "o"

#define PRIu8         "d"
#define PRIu16        "d"
#define PRIu32        "u"
#define PRIu64      __PRISCN64 "u"
#define PRIuLEAST8    "d"
#define PRIuLEAST16   "d"
#define PRIuLEAST32   "u"
#define PRIuLEAST64 __PRISCN64 "u"
#define PRIuFAST8     "u"
#define PRIuFAST16    "u"
#define PRIuFAST32    "u"
#define PRIuFAST64  __PRISCN64 "u"
#define PRIuMAX      "ju"
#define PRIuPTR     __PRISCNPTR "u"

#define PRIx8         "x"
#define PRIx16        "x"
#define PRIx32        "x"
#define PRIx64      __PRISCN64 "x"
#define PRIxLEAST8    "x"
#define PRIxLEAST16   "x"
#define PRIxLEAST32   "x"
#define PRIxLEAST64 __PRISCN64 "x"
#define PRIxFAST8     "x"
#define PRIxFAST16    "x"
#define PRIxFAST32    "x"
#define PRIxFAST64  __PRISCN64 "x"
#define PRIxMAX      "jx"
#define PRIxPTR     __PRISCNPTR "x"

#define PRIX8         "X"
#define PRIX16        "X"
#define PRIX32        "X"
#define PRIX64      __PRISCN64 "X"
#define PRIXLEAST8    "X"
#define PRIXLEAST16   "X"
#define PRIXLEAST32   "X"
#define PRIXLEAST64 __PRISCN64 "X"
#define PRIXFAST8     "X"
#define PRIXFAST16    "X"
#define PRIXFAST32    "X"
#define PRIXFAST64  __PRISCN64 "X"
#define PRIXMAX      "jX"
#define PRIXPTR     __PRISCNPTR "X"

#define SCNd8       "hhd"
#define SCNd16       "hd"
#define SCNd32        "d"
#define SCNd64      __PRISCN64 "d"
#define SCNdLEAST8  "hhd"
#define SCNdLEAST16  "hd"
#define SCNdLEAST32   "d"
#define SCNdLEAST64 __PRISCN64 "d"
#define SCNdFAST8     "d"
#define SCNdFAST16    "d"
#define SCNdFAST32    "d"
#define SCNdFAST64  __PRISCN64 "d"
#define SCNdMAX      "jd"
#define SCNdPTR     __PRISCNPTR "d"

#define SCNi8       "hhd"
#define SCNi16       "hi"
#define SCNi32        "i"
#define SCNi64      __PRISCN64 "i"
#define SCNiLEAST8  "hhi"
#define SCNiLEAST16  "hi"
#define SCNiLEAST32   "i"
#define SCNiLEAST64 __PRISCN64 "i"
#define SCNiFAST8     "i"
#define SCNiFAST16    "i"
#define SCNiFAST32    "i"
#define SCNiFAST64  __PRISCN64 "i"
#define SCNiMAX      "ji"
#define SCNiPTR     __PRISCNPTR "i"

#define SCNo8       "hho"
#define SCNo16       "ho"
#define SCNo32        "o"
#define SCNo64      __PRISCN64 "o"
#define SCNoLEAST8  "hho"
#define SCNoLEAST16  "ho"
#define SCNoLEAST32   "o"
#define SCNoLEAST64 __PRISCN64 "o"
#define SCNoFAST8     "o"
#define SCNoFAST16    "o"
#define SCNoFAST32    "o"
#define SCNoFAST64  __PRISCN64 "o"
#define SCNoMAX      "jo"
#define SCNoPTR     __PRISCNPTR "o"

#define SCNu8       "hhu"
#define SCNu16       "hu"
#define SCNu32        "u"
#define SCNu64      __PRISCN64 "u"
#define SCNuLEAST8  "hhu"
#define SCNuLEAST16  "hu"
#define SCNuLEAST32   "u"
#define SCNuLEAST64 __PRISCN64 "u"
#define SCNuFAST8     "u"
#define SCNuFAST16    "u"
#define SCNuFAST32    "u"
#define SCNuFAST64  __PRISCN64 "u"
#define SCNuMAX      "ju"
#define SCNuPTR     __PRISCNPTR "u"

#define SCNx8       "hhx"
#define SCNx16       "hx"
#define SCNx32        "x"
#define SCNx64      __PRISCN64 "x"
#define SCNxLEAST8  "hhx"
#define SCNxLEAST16  "hx"
#define SCNxLEAST32   "x"
#define SCNxLEAST64 __PRISCN64 "x"
#define SCNxFAST8     "x"
#define SCNxFAST16    "x"
#define SCNxFAST32    "x"
#define SCNxFAST64  __PRISCN64 "x"
#define SCNxMAX      "jx"
#define SCNxPTR     __PRISCNPTR "x"

#endif /* __STDC_FORMAT_MACROS */



typedef struct imaxdiv_t { intmax_t quot, rem; } imaxdiv_t;
   /* type of the value returned by the imaxdiv function. */

#ifdef __cplusplus
extern "C" {
#endif

_ARMABI intmax_t strtoimax(const char * __restrict /*nptr*/,
                   char ** __restrict /*endptr*/, int /*base*/) __attribute__((__nonnull__(1)));
    /* as for strtol */
_ARMABI uintmax_t strtoumax(const char * __restrict /*nptr*/,
                    char ** __restrict /*endptr*/, int /*base*/) __attribute__((__nonnull__(1)));
    /* as for strtoul */

_ARMABI intmax_t wcstoimax(const wchar_t * __restrict /*nptr*/,
                   wchar_t ** __restrict /*endptr*/, int /*base*/) __attribute__((__nonnull__(1)));
_ARMABI uintmax_t wcstoumax(const wchar_t * __restrict /*nptr*/,
                    wchar_t ** __restrict /*endptr*/, int /*base*/) __attribute__((__nonnull__(1)));

extern _ARMABI_PURE intmax_t imaxabs(intmax_t /*j*/);
   /*
    * computes the absolute value of an intmax_t j. If the result cannot be
    * represented, the behaviour is undefined.
    * Returns: the absolute value.
    */
extern _ARMABI_PURE imaxdiv_t imaxdiv(intmax_t /*numer*/, intmax_t /*denom*/);
   /*
    * computes the quotient and remainder of the division of the numerator
    * numer by the denominator denom. If the division is inexact, the sign of
    * the resulting quotient is that of the algebraic quotient, and the
    * magnitude of the resulting quotient is the largest integer less than the
    * magnitude of the algebraic quotient. If the result cannot be represented,
    * the behaviour is undefined; otherwise, quot * denom + rem shall equal
    * numer.
    * Returns: a structure of type imaxdiv_t, comprising both the quotient and
    *          the remainder. the structure shall contain the following
    *          members, in either order.
    *          intmax_t quot; intmax_t rem;
    */

#ifdef __cplusplus
}
#endif

#endif /* __inttypes_h */

/* end of inttypes.h */

