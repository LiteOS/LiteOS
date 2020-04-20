/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Sort Link
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

#include "los_sortlink_pri.h"
#include "los_memory.h"
#include "los_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define OS_INVALID_VALUE    0xFFFFFFFF

LITE_OS_SEC_TEXT_INIT UINT32 OsSortLinkInit(SortLinkAttribute *sortLinkHeader)
{
    LOS_DL_LIST *listObject = NULL;
    UINT32 index;

    listObject = &sortLinkHeader->sortLink[0];

    sortLinkHeader->cursor = 0;
    for (index = 0; index < OS_TSK_SORTLINK_LEN; index++, listObject++) {
        LOS_ListInit(listObject);
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT VOID OsAdd2SortLink(const SortLinkAttribute *sortLinkHeader, SortLinkList *sortList)
{
    SortLinkList *listSorted = NULL;
    LOS_DL_LIST *listObject = NULL;
    UINT32 sortIndex;
    UINT32 rollNum;
    UINT32 timeout;

    /*
     * huge rollnum could cause carry to invalid high bit
     * and eventually affect the calculation of sort index.
     */
    if (sortList->idxRollNum > OS_TSK_MAX_ROLLNUM) {
        SET_SORTLIST_VALUE(sortList, OS_TSK_MAX_ROLLNUM);
    }
    timeout = sortList->idxRollNum;
    sortIndex = timeout & OS_TSK_SORTLINK_MASK;
    rollNum = (timeout >> OS_TSK_SORTLINK_LOGLEN) + 1;
    if (sortIndex == 0) {
        rollNum--;
    }
    EVALUATE_L(sortList->idxRollNum, rollNum);
    sortIndex = sortIndex + sortLinkHeader->cursor;
    sortIndex = sortIndex & OS_TSK_SORTLINK_MASK;
    EVALUATE_H(sortList->idxRollNum, sortIndex);

    listObject = (LOS_DL_LIST *)sortLinkHeader->sortLink + sortIndex;
    if (listObject->pstNext == listObject) {
        LOS_ListTailInsert(listObject, &sortList->sortLinkNode);
    } else {
        listSorted = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
        do {
            if (ROLLNUM(listSorted->idxRollNum) <= ROLLNUM(sortList->idxRollNum)) {
                ROLLNUM_SUB(sortList->idxRollNum, listSorted->idxRollNum);
            } else {
                ROLLNUM_SUB(listSorted->idxRollNum, sortList->idxRollNum);
                break;
            }

            listSorted = LOS_DL_LIST_ENTRY(listSorted->sortLinkNode.pstNext, SortLinkList, sortLinkNode);
        } while (&listSorted->sortLinkNode != listObject);

        LOS_ListTailInsert(&listSorted->sortLinkNode, &sortList->sortLinkNode);
    }
}

LITE_OS_SEC_TEXT STATIC VOID OsCheckSortLink(const LOS_DL_LIST *listHead, const LOS_DL_LIST *listNode)
{
    LOS_DL_LIST *tmp = listNode->pstPrev;

    /* recursive check until double link round to itself */
    while (tmp != listNode) {
        if (tmp == listHead) {
            goto FOUND;
        }
        tmp = tmp->pstPrev;
    }

    /* delete invalid sortlink node */
    PRINT_ERR("the node is not on this sortlink!\n");
    OsBackTrace();

FOUND:
    return;
}

LITE_OS_SEC_TEXT VOID OsDeleteSortLink(const SortLinkAttribute *sortLinkHeader, SortLinkList *sortList)
{
    LOS_DL_LIST *listObject = NULL;
    SortLinkList *nextSortList = NULL;
    UINT32 sortIndex;

    sortIndex = SORT_INDEX(sortList->idxRollNum);
    listObject = (LOS_DL_LIST *)sortLinkHeader->sortLink + sortIndex;

    /* check if pstSortList node is on the right sortlink */
    OsCheckSortLink(listObject, &sortList->sortLinkNode);

    if (listObject != sortList->sortLinkNode.pstNext) {
        nextSortList = LOS_DL_LIST_ENTRY(sortList->sortLinkNode.pstNext, SortLinkList, sortLinkNode);
        ROLLNUM_ADD(nextSortList->idxRollNum, sortList->idxRollNum);
    }
    LOS_ListDelete(&sortList->sortLinkNode);
}

LITE_OS_SEC_TEXT STATIC UINT32 OsCalcExpierTime(UINT32 rollNum, UINT32 sortIndex, UINT16 curSortIndex)
{
    UINT32 expireTime;

    if (sortIndex > curSortIndex) {
        sortIndex = sortIndex - curSortIndex;
    } else {
        sortIndex = OS_TSK_SORTLINK_LEN - curSortIndex + sortIndex;
    }
    expireTime = ((rollNum - 1) << OS_TSK_SORTLINK_LOGLEN) + sortIndex;
    return expireTime;
}

LITE_OS_SEC_TEXT UINT32 OsSortLinkGetNextExpireTime(const SortLinkAttribute *sortLinkHeader)
{
    UINT16 cursor;
    UINT32 minSortIndex = OS_INVALID_VALUE;
    UINT32 minRollNum = OS_TSK_LOW_BITS_MASK;
    UINT32 expireTime = OS_INVALID_VALUE;
    LOS_DL_LIST *listObject = NULL;
    SortLinkList *listSorted = NULL;
    UINT32 i;

    cursor = (sortLinkHeader->cursor + 1) & OS_TSK_SORTLINK_MASK;

    for (i = 0; i < OS_TSK_SORTLINK_LEN; i++) {
        listObject = (LOS_DL_LIST *)sortLinkHeader->sortLink + ((cursor + i) & OS_TSK_SORTLINK_MASK);
        if (!LOS_ListEmpty(listObject)) {
            listSorted = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
            if (minRollNum > ROLLNUM(listSorted->idxRollNum)) {
                minRollNum = ROLLNUM(listSorted->idxRollNum);
                minSortIndex = (cursor + i) & OS_TSK_SORTLINK_MASK;
            }
        }
    }

    if (minRollNum != OS_TSK_LOW_BITS_MASK) {
        expireTime = OsCalcExpierTime(minRollNum, minSortIndex, sortLinkHeader->cursor);
    }

    return expireTime;
}

LITE_OS_SEC_TEXT VOID OsSortLinkUpdateExpireTime(UINT32 sleepTicks, SortLinkAttribute *sortLinkHeader)
{
    SortLinkList *sortList = NULL;
    LOS_DL_LIST *listObject = NULL;
    UINT32 i;
    UINT32 sortIndex;
    UINT32 rollNum;

    if (sleepTicks == 0) {
        return;
    }
    sortIndex = sleepTicks & OS_TSK_SORTLINK_MASK;
    rollNum = (sleepTicks >> OS_TSK_SORTLINK_LOGLEN) + 1;
    if (sortIndex == 0) {
        rollNum--;
        sortIndex = OS_TSK_SORTLINK_LEN;
    }

    for (i = 0; i < OS_TSK_SORTLINK_LEN; i++) {
        listObject = (LOS_DL_LIST *)sortLinkHeader->sortLink + ((sortLinkHeader->cursor + i) & OS_TSK_SORTLINK_MASK);
        if (listObject->pstNext != listObject) {
            sortList = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
            ROLLNUM_SUB(sortList->idxRollNum, rollNum - 1);
            if ((i > 0) && (i < sortIndex)) {
                ROLLNUM_DEC(sortList->idxRollNum);
            }
        }
    }
    sortLinkHeader->cursor = (sortLinkHeader->cursor + sleepTicks - 1) % OS_TSK_SORTLINK_LEN;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsSortLinkGetTargetExpireTime(const SortLinkAttribute *sortLinkHeader,
                                                            const SortLinkList *targetSortList)
{
    SortLinkList *listSorted = NULL;
    LOS_DL_LIST *listObject = NULL;
    UINT32 sortIndex = SORT_INDEX(targetSortList->idxRollNum);
    UINT32 rollNum = ROLLNUM(targetSortList->idxRollNum);

    listObject = (LOS_DL_LIST *)sortLinkHeader->sortLink + sortIndex;

    listSorted = LOS_DL_LIST_ENTRY(listObject->pstNext, SortLinkList, sortLinkNode);
    while (listSorted != targetSortList) {
        rollNum += ROLLNUM(listSorted->idxRollNum);
        listSorted = LOS_DL_LIST_ENTRY((listSorted->sortLinkNode).pstNext, SortLinkList, sortLinkNode);
    }
    return OsCalcExpierTime(rollNum, sortIndex, sortLinkHeader->cursor);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
