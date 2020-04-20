/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: BinaryTree Private HeadFile
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

#ifndef _LOS_BINARYTREE_PRI_H
#define _LOS_BINARYTREE_PRI_H

#include "los_typedef.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct tagBinNode {
    struct tagBinNode *left;
    struct tagBinNode *right;
    UINT32 nodeID;
    CHAR keyValue[0];
} BinNode;

typedef struct {
    BinNode leaf;
    UINTPTR linkReg1;
    UINTPTR linkReg2;
    UINTPTR linkReg3;
} LinkRegNode;

#define LR_COUNT 4096
extern LinkRegNode g_linkRegNode[LR_COUNT];
extern UINT32 g_linkRegNodeIndex;
extern LinkRegNode *g_linkRegRoot;

typedef struct {
    BinNode leaf;
    UINTPTR addr;
} AddrNode;

#define ADDR_COUNT 40960
extern AddrNode g_addrNode[ADDR_COUNT];
extern UINT32 g_addrNodeIndex;
extern AddrNode *g_addrRoot;

typedef struct {
    BinNode leaf;
    UINT32 reqSize;
} ReqSizeNode;

#define REQ_SIZE_COUNT 4096
extern ReqSizeNode g_reqSizeNode[REQ_SIZE_COUNT];
extern UINT32 g_reqSizeNodeIndex;
extern ReqSizeNode *g_reqSizeRoot;

typedef struct {
    BinNode leaf;
    UINT32 taskID;
} TaskIDNode;

#define TASK_ID_COUNT 1024

extern UINT32 OsBinTreeInsert(const VOID *node, UINT32 nodeLen, BinNode **leaf,
                              BinNode *(*GetMyBinNode)(UINT32 *nodeID),
                              INT32 (*CompareNode)(const VOID *node1, const VOID *node2));

extern INT32 OsCompareLRNode(const VOID *node1, const VOID *node2);
extern BinNode *OsGetLRBinNode(UINT32 *nodeID);

extern INT32 OsCompareAddrNode(const VOID *node1, const VOID *node2);
extern BinNode *OsGetAddrBinNode(UINT32 *nodeID);

extern INT32 OsCompareReqSizeNode(const VOID *node1, const VOID *node2);
extern BinNode *OsGetReqSizeBinNode(UINT32 *nodeID);

extern INT32 OsCompareTaskIDNode(const VOID *node1, const VOID *node2);
extern BinNode *OsGetTaskIDBinNode(UINT32 *nodeID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_BINARYTREE_PRI_H */
