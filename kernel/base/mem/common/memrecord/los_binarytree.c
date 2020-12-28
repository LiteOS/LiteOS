/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS BinTree Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
#include "los_config.h"

#ifdef LOSCFG_MEM_RECORDINFO
#include "los_binarytree_pri.h"
#include "los_typedef.h"
#include "los_memory.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 OsBinTreeInsert(const VOID *node, UINT32 nodeLen, BinNode **leaf,
                       BinNode *(*GetMyBinNode)(UINT32 *nodeId),
                       INT32 (*CompareNode)(const VOID *node1, const VOID *node2))
{
    UINT32 nodeId;
    INT32 result;
    BinNode **currentNode = leaf;

    if (leaf == NULL) {
        return OS_INVALID;
    }

    do {
        if (*currentNode != NULL) {
            result = CompareNode(node, (const VOID *)(*currentNode));
            if (result == 0) {
                return (*currentNode)->nodeId;
            } else if (result < 0) {
                currentNode = (BinNode **)(&((*currentNode)->left));
            } else {
                currentNode = (BinNode **)(&((*currentNode)->right));
            }
        } else {
            (*currentNode) = GetMyBinNode(&nodeId);
            if (*currentNode == NULL) {
                return OS_INVALID;
            }

            (VOID)memcpy_s((*currentNode)->keyValue, (nodeLen - sizeof(BinNode)),
                ((BinNode *)node)->keyValue, (nodeLen - sizeof(BinNode)));
            (*currentNode)->nodeId = nodeId;
            /* initialize the children to NULL */
            (*currentNode)->left = NULL;
            (*currentNode)->right = NULL;

            return (*currentNode)->nodeId;
        }
    } while (1);
}

/* LRNODE */
LinkRegNode g_linkRegNode[LR_COUNT];
UINT32 g_linkRegNodeIndex = 0;
LinkRegNode *g_linkRegRoot = NULL;
INT32 OsCompareLRNode(const VOID *node1, const VOID *node2)
{
    LinkRegNode *linkRegNode1 = (LinkRegNode *)node1;
    LinkRegNode *linkRegNode2 = (LinkRegNode *)node2;
    if (linkRegNode1->linkReg1 < linkRegNode2->linkReg1) {
        return -1;
    } else if ((linkRegNode1->linkReg1 == linkRegNode2->linkReg1) &&
               (linkRegNode1->linkReg2 < linkRegNode2->linkReg2)) {
        return -1;
    } else if ((linkRegNode1->linkReg1 == linkRegNode2->linkReg1) &&
               (linkRegNode1->linkReg2 == linkRegNode2->linkReg2) &&
               (linkRegNode1->linkReg3 < linkRegNode2->linkReg3)) {
        return -1;
    } else if ((linkRegNode1->linkReg1 == linkRegNode2->linkReg1) &&
               (linkRegNode1->linkReg2 == linkRegNode2->linkReg2) &&
               (linkRegNode1->linkReg3 == linkRegNode2->linkReg3)) {
        return 0;
    } else {
        return 1;
    }
}

BinNode *OsGetLRBinNode(UINT32 *nodeId)
{
    if (g_linkRegNodeIndex < LR_COUNT) {
        *nodeId = g_linkRegNodeIndex;
        return (BinNode *)(&g_linkRegNode[g_linkRegNodeIndex++]);
    } else {
        *nodeId = (UINT32)-1;
        return NULL;
    }
}

/* ADDRNODE */
AddrNode g_addrNode[ADDR_COUNT];
UINT32 g_addrNodeIndex = 0;
AddrNode *g_addrRoot = NULL;
INT32 OsCompareAddrNode(const VOID *node1, const VOID *node2)
{
    AddrNode *addrNode1 = (AddrNode *)node1;
    AddrNode *addrNode2 = (AddrNode *)node2;
    if (addrNode1->addr < addrNode2->addr) {
        return -1;
    } else if (addrNode1->addr == addrNode2->addr) {
        return 0;
    } else {
        return 1;
    }
}

BinNode *OsGetAddrBinNode(UINT32 *nodeId)
{
    if (g_addrNodeIndex < ADDR_COUNT) {
        *nodeId = g_addrNodeIndex;
        return (BinNode *)(&g_addrNode[g_addrNodeIndex++]);
    } else {
        *nodeId = (UINT32)-1;
        return NULL;
    }
}

/* REQSIZENODE */
ReqSizeNode g_reqSizeNode[REQ_SIZE_COUNT];
UINT32 g_reqSizeNodeIndex = 0;
ReqSizeNode *g_reqSizeRoot = NULL;

INT32 OsCompareReqSizeNode(const VOID *node1, const VOID *node2)
{
    ReqSizeNode *reqSizeNode1 = (ReqSizeNode *)node1;
    ReqSizeNode *reqSizeNode2 = (ReqSizeNode *)node2;
    if (reqSizeNode1->reqSize < reqSizeNode2->reqSize) {
        return -1;
    } else if (reqSizeNode1->reqSize == reqSizeNode2->reqSize) {
        return 0;
    } else {
        return 1;
    }
}

BinNode *OsGetReqSizeBinNode(UINT32 *nodeId)
{
    if (g_reqSizeNodeIndex < REQ_SIZE_COUNT) {
        *nodeId = g_reqSizeNodeIndex;
        return (BinNode *)(&g_reqSizeNode[g_reqSizeNodeIndex++]);
    } else {
        *nodeId = (UINT32)-1;
        return NULL;
    }
}

/* TASKIDNODE */
STATIC TaskIDNode g_taskIdNode[TASK_ID_COUNT];
STATIC UINT32 g_taskIdNodeIndex = 0;
STATIC TaskIDNode *g_taskIdRoot = NULL;
INT32 OsCompareTaskIDNode(const VOID *node1, const VOID *node2)
{
    TaskIDNode *taskIdNode1 = (TaskIDNode *)node1;
    TaskIDNode *taskIdNode2 = (TaskIDNode *)node2;
    if (taskIdNode1->taskId < taskIdNode2->taskId) {
        return -1;
    } else if (taskIdNode1->taskId == taskIdNode2->taskId) {
        return 0;
    } else {
        return 1;
    }
}

BinNode *OsGetTaskIDBinNode(UINT32 *nodeId)
{
    if (g_taskIdNodeIndex < TASK_ID_COUNT) {
        *nodeId = g_taskIdNodeIndex;
        return (BinNode *)(&g_taskIdNode[g_taskIdNodeIndex++]);
    } else {
        *nodeId = (UINT32)-1;
        return NULL;
    }
}
#define BINARYTREE_TASKID_COUNT  11
#define BINARYTREE_REQSIZE_COUNT 4
STATIC const UINT32 g_binaryTreeTaskID[BINARYTREE_TASKID_COUNT] = { 33, 10, 20, 9, 42, 34, 45, 47, 46, 50, 49 };
STATIC const UINT32 g_binaryTreeReqSize[BINARYTREE_REQSIZE_COUNT] = { 616, 136, 1708, 1580 };

VOID OsBinaryTreeInit(VOID)
{
    INT32 index;
    LinkRegNode linkRegNode;
    AddrNode node;
    TaskIDNode taskNode;
    ReqSizeNode reqNode;

    /* equal to the middle address of __text_start and __text_end */
    linkRegNode.linkReg1 = (UINTPTR)(((&__text_end - &__text_start) / 2) + &__text_start);
    linkRegNode.linkReg2 = linkRegNode.linkReg1;
    linkRegNode.linkReg3 = linkRegNode.linkReg1;
    (VOID)OsBinTreeInsert(&linkRegNode, sizeof(LinkRegNode), (BinNode **)&g_linkRegRoot,
                          OsGetLRBinNode, OsCompareLRNode);

    /* equal to the middle address of __bss_end and g_sys_mem_addr_end */
    node.addr = ((g_sys_mem_addr_end - (UINTPTR)(&__bss_end)) / 2) + (UINTPTR)(&__bss_end);
    (VOID)OsBinTreeInsert(&node, sizeof(AddrNode), (BinNode **)&g_addrRoot,
                          OsGetAddrBinNode, OsCompareAddrNode);

    for (index = 0; index < BINARYTREE_TASKID_COUNT; index++) {
        taskNode.taskId = g_binaryTreeTaskID[index];
        (VOID)OsBinTreeInsert(&taskNode, sizeof(TaskIDNode), (BinNode **)&g_taskIdRoot,
                              OsGetTaskIDBinNode, OsCompareTaskIDNode);
    }

    for (index = 0; index < BINARYTREE_REQSIZE_COUNT; index++) {
        reqNode.reqSize = g_binaryTreeReqSize[index];
        (VOID)OsBinTreeInsert(&reqNode, sizeof(ReqSizeNode), (BinNode **)&g_reqSizeRoot,
                              OsGetReqSizeBinNode, OsCompareReqSizeNode);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
