/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Record Implementation
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
#include "los_memrecord_pri.h"
#include "stdio.h"
#include "los_memory.h"
#include "los_binarytree_pri.h"
#include "los_event.h"
#include "los_exc.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

enum MemActType {
    OS_MEM_VALIDFREE,
    OS_MEM_INVALIDFREE,
    OS_MEM_ALLOC
};

STATIC MemRecordInfo g_memRecord1[RECORD_LEN] = {0};
STATIC MemRecordInfo g_memRecord2[RECORD_LEN] = {0};
STATIC MemRecordInfo *g_saveMemRecord = g_memRecord1;
STATIC MemRecordInfo *g_printMemRecord = NULL;
STATIC MemRecordInfo *g_curPtr = NULL;

STATIC volatile UINT32 g_memRecordIndex = 0;
STATIC volatile UINT32 g_memLastEndIndex = 0;
STATIC EVENT_CB_S g_memShowEvent;

UINT32 g_memRecordShowEnable = 1;
STATIC UINT32 g_lastAddrNodeIndex = 0;
STATIC UINT32 g_lastReqSizeNodeIndex = 0;
STATIC UINT32 g_lastlinkRegNodeIndex = 0;

#define INDEX_LENGTH           2
#define ADDR_ID_LENGTH         3
#define REQSIZE_ID_LENGTH      2
#define ACTTYPE_LENGTH         4
#define TASK_ID_LENGTH         2
#define SYS_TICK_LENGTH        6
#define LINK_REG_ID_LENGTH     2
#define INFO_STR_LENGTH        20
#define PRINT_STR_LENGTH       32
#define NODE_VALUE_LENGTH      7
#define READ_EVENT_MASK        0xFFF
#define WRITE_EVENT            0x112

STATIC VOID OsMemRecordCompressInfo(VOID)
{
    UINT32 count;
    CHAR infoStr[INFO_STR_LENGTH];

    UINT32 currentIndex = g_addrNodeIndex;
    for (count = g_lastAddrNodeIndex; count < currentIndex; count++) {
        OsDecTo64F(g_addrNode[count].leaf.nodeID, infoStr, ADDR_ID_LENGTH);
        printf("~^%s%x^~\n", infoStr, g_addrNode[count].addr);
    }
    g_lastAddrNodeIndex = currentIndex;

    currentIndex = g_reqSizeNodeIndex;
    for (count = g_lastReqSizeNodeIndex; count < currentIndex; count++) {
        OsDecTo64F(g_reqSizeNode[count].leaf.nodeID, infoStr, REQSIZE_ID_LENGTH);
        printf("*^%s%u^*\n", infoStr, g_reqSizeNode[count].reqSize);
    }
    g_lastReqSizeNodeIndex = currentIndex;

    currentIndex = g_linkRegNodeIndex;
    for (count = g_lastlinkRegNodeIndex; count < currentIndex; count++) {
        OsDecTo64F(g_linkRegNode[count].leaf.nodeID, infoStr, LINK_REG_ID_LENGTH);
        printf("$^%s%x%x%x^$\n", infoStr, g_linkRegNode[count].linkReg1, g_linkRegNode[count].linkReg2,
               g_linkRegNode[count].linkReg3);
    }
    g_lastlinkRegNodeIndex = currentIndex;
}

STATIC VOID PrintPtrAssign(CHAR *printStr, UINT32 strLen, UINT32 startIndex, UINT32 index)
{
    CHAR nodeValue[NODE_VALUE_LENGTH];
    UINT32 tmpIndex = 0;

    /* 3 bytes for ending "!~" and '\0'. */
    if (strLen < (INDEX_LENGTH + ADDR_ID_LENGTH + REQSIZE_ID_LENGTH + ACTTYPE_LENGTH +
                  TASK_ID_LENGTH + SYS_TICK_LENGTH + LINK_REG_ID_LENGTH + index + 3)) {
        PRINT_ERR("printStr is not big enough\n");
        return;
    }

    OsDecTo64F(startIndex, nodeValue, INDEX_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].addrID, nodeValue, ADDR_ID_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].reqSizeID, nodeValue, REQSIZE_ID_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].actType, nodeValue, ACTTYPE_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].taskID, nodeValue, TASK_ID_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].sysTick, nodeValue, SYS_TICK_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    tmpIndex = 0;
    OsDecTo64F(g_curPtr[startIndex].linkRegID, nodeValue, LINK_REG_ID_LENGTH);
    printStr[index++] = nodeValue[tmpIndex++];
    printStr[index++] = nodeValue[tmpIndex];

    printStr[index++] = '!';
    printStr[index++] = '~';
    printStr[index++] = '\0';

    printf("%s\n", printStr);
}

STATIC VOID OsMemRecordShow(VOID)
{
    UINT32 index = 0;
    UINT32 startIndex = g_memLastEndIndex;
    UINT32 endIndex;
    MemRecordInfo *printMemRecord = g_printMemRecord;
    CHAR printStr[PRINT_STR_LENGTH];

    if (g_curPtr == NULL) {
        g_curPtr = g_printMemRecord;
    }

    OsMemRecordCompressInfo();

    if (g_curPtr != NULL) {
        printStr[index++] = '~';
        printStr[index++] = '!';
        if (g_curPtr == printMemRecord) {
            while (startIndex < RECORD_LEN) {
                PrintPtrAssign(printStr, PRINT_STR_LENGTH, startIndex, index);
                startIndex++;
            }
            g_memLastEndIndex = 0;
        }
    }

    startIndex = g_memLastEndIndex;
    endIndex = g_memRecordIndex;
    if ((g_curPtr == g_saveMemRecord) && (g_printMemRecord != NULL) && (startIndex >= endIndex)) {
        printf("Rec:error:the printf speed is low,Rnd\n");
        startIndex = 0;
    }

    g_curPtr = g_saveMemRecord;
    index = 0;
    printStr[index++] = '~';
    printStr[index++] = '!';
    while (startIndex < endIndex) {
        PrintPtrAssign(printStr, PRINT_STR_LENGTH, startIndex, index);
        startIndex++;
    }

    g_memLastEndIndex = endIndex;
}

STATIC VOID OsMemRecordExchange(VOID)
{
    if (g_saveMemRecord == g_memRecord1) {
        g_saveMemRecord = g_memRecord2;
        g_printMemRecord = g_memRecord1;
    } else {
        g_saveMemRecord = g_memRecord1;
        g_printMemRecord = g_memRecord2;
    }
    g_memRecordIndex = 0;
    (VOID)LOS_EventWrite(&g_memShowEvent, WRITE_EVENT);
}

#define LINK_REG1_INDEX   1
#define LINK_REG2_INDEX   2
#define LINK_REG3_INDEX   3
#define LINK_REG_NUM      3

STATIC INLINE VOID OsMemRecordLR(LinkRegNode *linkRegNode)
{
    UINTPTR framePtr, framePtrTmp;
    UINT32 index = 0;

    linkRegNode->linkReg1 = 0;
    linkRegNode->linkReg2 = 0;
    linkRegNode->linkReg3 = 0;

    framePtr = Get_Fp();
    while ((framePtr > OS_SYS_FUNC_ADDR_START) &&
           (framePtr < OS_SYS_FUNC_ADDR_END) &&
           ((framePtr % sizeof(CHAR *)) == 0)) {
        framePtrTmp = framePtr;
        if (index == LINK_REG1_INDEX) {
            linkRegNode->linkReg1 = *((UINTPTR *)(framePtrTmp));
        } else if (index == LINK_REG2_INDEX) {
            linkRegNode->linkReg2 = *((UINTPTR *)(framePtrTmp));
        } else if (index == LINK_REG3_INDEX) {
            linkRegNode->linkReg3 = *((UINTPTR *)(framePtrTmp));
        }
        framePtr = *((UINTPTR *)(framePtrTmp - sizeof(UINTPTR *)));
        index++;
        if (index == (LINK_REG_NUM + 1)) {
            break;
        }
    }
}

STATIC VOID OsMemRecordTaskID(VOID)
{
    LosTaskCB *runTask = OsCurrTaskGet();
    if (runTask != NULL) {
        g_saveMemRecord[g_memRecordIndex].taskID = LOS_CurTaskIDGet();
    } else {
        g_saveMemRecord[g_memRecordIndex].taskID = 0;
    }
}

STATIC INLINE VOID OsMemRecord(const VOID *ptr, UINT32 size)
{
    UINT64 tickCount = LOS_TickCountGet();
    UINT32 nodeID;
    LinkRegNode linkRegNode;
    AddrNode node;
    ReqSizeNode reqNode;

    OsMemRecordLR(&linkRegNode);
    nodeID = OsBinTreeInsert(&linkRegNode, sizeof(linkRegNode), (BinNode **)&g_linkRegRoot, OsGetLRBinNode,
                             OsCompareLRNode);
    if (nodeID == OS_INVALID) {
        PRINT_WARN("LIST g_linkRegRoot insert linkRegNode failed!\n");
    }
    g_saveMemRecord[g_memRecordIndex].linkRegID = nodeID;

    node.addr = (UINTPTR)ptr;
    nodeID = OsBinTreeInsert(&node, sizeof(AddrNode), (BinNode **)&g_addrRoot, OsGetAddrBinNode,
                             OsCompareAddrNode);
    if (nodeID == OS_INVALID) {
        PRINT_WARN("LIST g_addrRoot insert addrNode failed!\n");
    }
    g_saveMemRecord[g_memRecordIndex].addrID = nodeID;
    g_saveMemRecord[g_memRecordIndex].sysTick = tickCount;

    OsMemRecordTaskID();

    reqNode.reqSize = size;
    nodeID = OsBinTreeInsert(&reqNode, sizeof(ReqSizeNode), (BinNode **)&g_reqSizeRoot, OsGetReqSizeBinNode,
                             OsCompareReqSizeNode);
    if (nodeID == OS_INVALID) {
        PRINT_WARN("LIST g_reqSizeRoot insert reqSizeNode failed!\n");
    }
    g_saveMemRecord[g_memRecordIndex].reqSizeID = nodeID;

    g_memRecordIndex++;
    if (g_memRecordIndex == RECORD_LEN) {
        OsMemRecordExchange();
    }
}

VOID OsMemRecordMalloc(const VOID *ptr, UINT32 size)
{
    if (g_memRecordShowEnable == 0) {
        return;
    }

    OsMemRecord(ptr, size);
    g_saveMemRecord[g_memRecordIndex].actType = OS_MEM_ALLOC;
}

VOID OsMemRecordFree(const VOID *ptr, UINT32 size)
{
    UINT32 actType;
    if (g_memRecordShowEnable == 0) {
        return;
    }
    actType = (size == 0) ? OS_MEM_INVALIDFREE : OS_MEM_VALIDFREE;
    OsMemRecord(ptr, size);
    g_saveMemRecord[g_memRecordIndex].actType = actType;
}

VOID OsMemRecordShowTask(VOID)
{
    (VOID)LOS_EventInit(&g_memShowEvent);
    while (1) {
        (VOID)LOS_EventRead(&g_memShowEvent, READ_EVENT_MASK,
            LOS_WAITMODE_OR | LOS_WAITMODE_CLR, MEM_RECORDSHOW_TIMEOUT);
        if (g_memRecordShowEnable) {
            OsMemRecordShow();
        }
    }
}

VOID OsMemRecordShowSet(UINT32 value)
{
    g_memRecordShowEnable = value;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
