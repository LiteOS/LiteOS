/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Sortlink Private HeadFile
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 * --------------------------------------------------------------------------- */

#ifndef _LOS_SORTLINK_PRI_H
#define _LOS_SORTLINK_PRI_H

#include "los_typedef.h"
#include "los_list.h"
#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Sortlink Rollnum Structure:
 *   ------------------------------------------
 *  | 31 | 30 | 29 |.......| 4 | 3 | 2 | 1 | 0 |
 *   ------------------------------------------
 *  |<-High Bits->|<---------Low Bits--------->|
 *
 *  Low Bits  : circles
 *  High Bits : sortlink index
 */
#define OS_TSK_HIGH_BITS       3U
#define OS_TSK_LOW_BITS        (32U - OS_TSK_HIGH_BITS)
#define OS_TSK_SORTLINK_LOGLEN OS_TSK_HIGH_BITS
#define OS_TSK_SORTLINK_MASK   (OS_TSK_SORTLINK_LEN - 1U)
#define OS_TSK_MAX_ROLLNUM     (0xFFFFFFFFU - OS_TSK_SORTLINK_LEN)
#define OS_TSK_HIGH_BITS_MASK  (OS_TSK_SORTLINK_MASK << OS_TSK_LOW_BITS)
#define OS_TSK_LOW_BITS_MASK   (~OS_TSK_HIGH_BITS_MASK)

#define EVALUATE_L(NUM, VALUE) NUM = (((NUM) & OS_TSK_HIGH_BITS_MASK) | (VALUE))

#define EVALUATE_H(NUM, VALUE) NUM = (((NUM) & OS_TSK_LOW_BITS_MASK) | ((VALUE) << OS_TSK_LOW_BITS))

#define ROLLNUM_SUB(NUM1, NUM2)                 \
    NUM1 = (((NUM1) & OS_TSK_HIGH_BITS_MASK) |  \
    (ROLLNUM(NUM1) - ROLLNUM(NUM2)))

#define ROLLNUM_ADD(NUM1, NUM2)                 \
    NUM1 = (((NUM1) & OS_TSK_HIGH_BITS_MASK) |  \
    (ROLLNUM(NUM1) + ROLLNUM(NUM2)))

#define ROLLNUM_DEC(NUM) NUM = ((NUM) - 1)

#define ROLLNUM(NUM) ((NUM) & OS_TSK_LOW_BITS_MASK)

#define SORT_INDEX(NUM) ((NUM) >> OS_TSK_LOW_BITS)

#define SET_SORTLIST_VALUE(sortList, value) (((SortLinkList *)(sortList))->idxRollNum = (value))

typedef struct {
    LOS_DL_LIST sortLinkNode;
    UINT32 idxRollNum;
} SortLinkList;

typedef struct {
    LOS_DL_LIST sortLink[OS_TSK_SORTLINK_LEN];
    UINT16 cursor;
    UINT16 reserved;
} SortLinkAttribute;

extern UINT32 OsSortLinkInit(SortLinkAttribute *sortLinkHeader);
extern VOID OsAdd2SortLink(const SortLinkAttribute *sortLinkHeader, SortLinkList *sortList);
extern VOID OsDeleteSortLink(const SortLinkAttribute *sortLinkHeader, SortLinkList *sortList);
extern UINT32 OsSortLinkGetNextExpireTime(const SortLinkAttribute *sortLinkHeader);
extern UINT32 OsSortLinkGetTargetExpireTime(const SortLinkAttribute *sortLinkHeader,
                                            const SortLinkList *targetSortList);
extern VOID OsSortLinkUpdateExpireTime(UINT32 sleepTicks, SortLinkAttribute *sortLinkHeader);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_SORTLINK_PRI_H */
