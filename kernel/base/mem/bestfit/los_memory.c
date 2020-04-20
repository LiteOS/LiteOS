/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Mem Module Implementation
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

#include "los_memory_pri.h"
#ifdef LOSCFG_LIB_LIBC
#include "stdio.h"
#endif
#include "los_multipledlinkhead_pri.h"
#include "los_memstat_pri.h"
#ifdef LOSCFG_MEM_RECORDINFO
#include "los_memrecord_pri.h"
#endif
#include "los_task_pri.h"
#include "los_exc.h"
#include "los_spinlock.h"

#ifdef LOSCFG_SHELL_EXCINFO
#include "los_excinfo_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define NODEDUMPSIZE  64  /* the dump size of current broken node when memcheck error */
#define COLUMN_NUM    8   /* column num of the output info of mem node */

/* Memory linked list control node structure */
typedef struct {
    LOS_DL_LIST freeNodeInfo;         /* Free memory node */
    struct tagLosMemDynNode *preNode; /* Pointer to the previous memory node */

#ifdef LOSCFG_MEM_HEAD_BACKUP
    UINT32 gapSize;
    UINTPTR checksum; /* magic = xor checksum */
#endif

#ifdef LOSCFG_MEM_RECORDINFO
    UINT32 originSize;
#ifdef LOSCFG_AARCH64
    UINT32 reserve1; /* 64-bit alignment */
#endif
#endif

#ifdef LOSCFG_MEM_LEAKCHECK
    UINTPTR linkReg[LOS_RECORD_LR_CNT];
#endif

#ifdef LOSCFG_AARCH64
    UINT32 reserve2; /* 64-bit alignment */
#endif
    /* Size and flag of the current node (the high two bits represent a flag,and the rest bits specify the size) */
    UINT32 sizeAndFlag;
} LosMemCtlNode;

/* Memory linked list node structure */
typedef struct tagLosMemDynNode {
#ifdef LOSCFG_MEM_HEAD_BACKUP
    LosMemCtlNode backupNode;
#endif
    LosMemCtlNode selfNode;
} LosMemDynNode;

#ifdef LOSCFG_MEM_MUL_POOL
VOID *g_poolHead = NULL;
#endif

/* spinlock for mem module */
LITE_OS_SEC_BSS  SPIN_LOCK_INIT(g_memSpin);

#ifdef LOSCFG_MEM_HEAD_BACKUP
/*
 * 0xffff0000U, 0xffffU
 * the taskID and moduleID multiplex the node->selfNode.freeNodeInfo.pstNext
 * the low 16 bits is the taskID and the high 16bits is the moduleID
 */
STATIC VOID OsMemNodeSave(LosMemDynNode *node);
#define OS_MEM_TASKID_SET(node, ID) do {                                                  \
    UINTPTR tmp_ = (UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext);   \
    tmp_ &= 0xffff0000U;                                                                  \
    tmp_ |= (ID);                                                                         \
    ((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext = (LOS_DL_LIST *)tmp_;       \
    OsMemNodeSave((LosMemDynNode *)(node));                                               \
} while (0)
#else
#define OS_MEM_TASKID_SET(node, ID) do {                                                  \
    UINTPTR tmp_ = (UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext);   \
    tmp_ &= 0xffff0000U;                                                                  \
    tmp_ |= (ID);                                                                         \
    ((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext = (LOS_DL_LIST *)tmp_;       \
} while (0)
#endif
#define OS_MEM_TASKID_GET(node) ((UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext) & 0xffffU)

#ifdef LOSCFG_MEM_MUL_MODULE
#define BITS_NUM_OF_TYPE_SHORT    16
#ifdef LOSCFG_MEM_HEAD_BACKUP
#define OS_MEM_MODID_SET(node, ID) do {                                                   \
    UINTPTR tmp_ = (UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext);   \
    tmp_ &= 0xffffU;                                                                      \
    tmp_ |= (ID) << BITS_NUM_OF_TYPE_SHORT;                                               \
    ((LosMemDynNode *)node)->selfNode.freeNodeInfo.pstNext = (LOS_DL_LIST *)tmp_;         \
    OsMemNodeSave((LosMemDynNode *)(node));                                               \
} while (0)
#else
#define OS_MEM_MODID_SET(node, ID) do {                                                   \
    UINTPTR tmp_ = (UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext);   \
    tmp_ &= 0xffffU;                                                                      \
    tmp_ |= (ID) << BITS_NUM_OF_TYPE_SHORT;                                               \
    ((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext = (LOS_DL_LIST *)tmp_;       \
} while (0)
#endif
#define OS_MEM_MODID_GET(node) \
    (((UINTPTR)(((LosMemDynNode *)(node))->selfNode.freeNodeInfo.pstNext) >> BITS_NUM_OF_TYPE_SHORT) & 0xffffU)
#endif

#define OS_MEM_ALIGN(p, alignSize) (((UINTPTR)(p) + (alignSize) - 1) & ~((UINTPTR)((alignSize) - 1)))
#define OS_MEM_NODE_HEAD_SIZE      sizeof(LosMemDynNode)
#define OS_MEM_MIN_POOL_SIZE       (OS_DLNK_HEAD_SIZE + (2 * OS_MEM_NODE_HEAD_SIZE) + sizeof(LosMemPoolInfo))
#define IS_POW_TWO(value)          ((((UINTPTR)(value)) & ((UINTPTR)(value) - 1)) == 0)
#define POOL_ADDR_ALIGNSIZE        64
#ifdef LOSCFG_AARCH64
#define OS_MEM_ALIGN_SIZE 8
#else
#define OS_MEM_ALIGN_SIZE 4
#endif
#define OS_MEM_NODE_USED_FLAG             0x80000000U
#define OS_MEM_NODE_ALIGNED_FLAG          0x40000000U
#define OS_MEM_NODE_ALIGNED_AND_USED_FLAG (OS_MEM_NODE_USED_FLAG | OS_MEM_NODE_ALIGNED_FLAG)

#define OS_MEM_NODE_GET_ALIGNED_FLAG(sizeAndFlag) \
    ((sizeAndFlag) & OS_MEM_NODE_ALIGNED_FLAG)
#define OS_MEM_NODE_SET_ALIGNED_FLAG(sizeAndFlag) \
    ((sizeAndFlag) = ((sizeAndFlag) | OS_MEM_NODE_ALIGNED_FLAG))
#define OS_MEM_NODE_GET_ALIGNED_GAPSIZE(sizeAndFlag) \
    ((sizeAndFlag) & ~OS_MEM_NODE_ALIGNED_FLAG)
#define OS_MEM_NODE_GET_USED_FLAG(sizeAndFlag) \
    ((sizeAndFlag) & OS_MEM_NODE_USED_FLAG)
#define OS_MEM_NODE_SET_USED_FLAG(sizeAndFlag) \
    ((sizeAndFlag) = ((sizeAndFlag) | OS_MEM_NODE_USED_FLAG))
#define OS_MEM_NODE_GET_SIZE(sizeAndFlag) \
    ((sizeAndFlag) & ~OS_MEM_NODE_ALIGNED_AND_USED_FLAG)
#define OS_MEM_HEAD(pool, size) \
    OsDLnkMultiHead(OS_MEM_HEAD_ADDR(pool), size)
#define OS_MEM_HEAD_ADDR(pool) \
    ((VOID *)((UINTPTR)(pool) + sizeof(LosMemPoolInfo)))
#define OS_MEM_NEXT_NODE(node) \
    ((LosMemDynNode *)(VOID *)((UINT8 *)(node) + OS_MEM_NODE_GET_SIZE((node)->selfNode.sizeAndFlag)))
#define OS_MEM_FIRST_NODE(pool) \
    ((LosMemDynNode *)(VOID *)((UINT8 *)OS_MEM_HEAD_ADDR(pool) + OS_DLNK_HEAD_SIZE))
#define OS_MEM_END_NODE(pool, size) \
    ((LosMemDynNode *)(VOID *)(((UINT8 *)(pool) + (size)) - OS_MEM_NODE_HEAD_SIZE))
#define OS_MEM_MIDDLE_ADDR_OPEN_END(startAddr, middleAddr, endAddr) \
    (((UINT8 *)(startAddr) <= (UINT8 *)(middleAddr)) && ((UINT8 *)(middleAddr) < (UINT8 *)(endAddr)))
#define OS_MEM_MIDDLE_ADDR(startAddr, middleAddr, endAddr) \
    (((UINT8 *)(startAddr) <= (UINT8 *)(middleAddr)) && ((UINT8 *)(middleAddr) <= (UINT8 *)(endAddr)))
#define OS_MEM_SET_MAGIC(value) \
    (value) = (LOS_DL_LIST *)((UINTPTR)&(value) ^ (UINTPTR)(-1))
#define OS_MEM_MAGIC_VALID(value) \
    (((UINTPTR)(value) ^ (UINTPTR)&(value)) == (UINTPTR)(-1))

UINT8 *m_aucSysMem0 = NULL;
UINT8 *m_aucSysMem1 = NULL;

__attribute__((section(".data.init"))) UINTPTR g_sys_mem_addr_end;
__attribute__((section(".data.init"))) UINTPTR g_excInteractMemSize = 0;
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
STATIC UINT8 g_memCheckLevel = LOS_MEM_CHECK_LEVEL_DEFAULT;
#endif

#ifdef LOSCFG_MEM_MUL_MODULE
UINT32 g_moduleMemUsedSize[MEM_MODULE_MAX + 1] = { 0 };
#endif

MALLOC_HOOK g_MALLOC_HOOK = NULL;

VOID OsMemInfoPrint(VOID *pool);
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
const VOID *OsMemFindNodeCtrl(const VOID *pool, const VOID *ptr);
#endif
#ifdef LOSCFG_MEM_HEAD_BACKUP
#define CHECKSUM_MAGICNUM    0xDEADBEEF
#define OS_MEM_NODE_CHECKSUN_CALCULATE(ctlNode)    \
    (((UINTPTR)(ctlNode)->freeNodeInfo.pstPrev) ^  \
    ((UINTPTR)(ctlNode)->freeNodeInfo.pstNext) ^   \
    ((UINTPTR)(ctlNode)->preNode) ^                \
    (ctlNode)->gapSize ^                           \
    (ctlNode)->sizeAndFlag ^                       \
    CHECKSUM_MAGICNUM)

STATIC INLINE VOID OsMemDispCtlNode(const LosMemCtlNode *ctlNode)
{
    UINTPTR checksum;

    checksum = OS_MEM_NODE_CHECKSUN_CALCULATE(ctlNode);

    PRINTK("node:%p checksum=%p[%p] freeNodeInfo.pstPrev=%p "
           "freeNodeInfo.pstNext=%p preNode=%p gapSize=0x%x sizeAndFlag=0x%x\n",
           ctlNode,
           ctlNode->checksum,
           checksum,
           ctlNode->freeNodeInfo.pstPrev,
           ctlNode->freeNodeInfo.pstNext,
           ctlNode->preNode,
           ctlNode->gapSize,
           ctlNode->sizeAndFlag);
}

STATIC INLINE VOID OsMemDispMoreDetails(const LosMemDynNode *node)
{
    UINT32 taskID;
    LosTaskCB *taskCB = NULL;

    PRINT_ERR("************************************************\n");
    OsMemDispCtlNode(&node->selfNode);
    PRINT_ERR("the address of node :%p\n", node);

    if (!OS_MEM_NODE_GET_USED_FLAG(node->selfNode.sizeAndFlag)) {
        PRINT_ERR("this is a FREE node\n");
        PRINT_ERR("************************************************\n\n");
        return;
    }

    taskID = OS_MEM_TASKID_GET(node);
    if (taskID >= g_taskMaxNum) {
        PRINT_ERR("The task [ID: 0x%x] is ILLEGAL\n", taskID);
        if (taskID == g_taskMaxNum) {
            PRINT_ERR("PROBABLY alloc by SYSTEM INIT, NOT IN ANY TASK\n");
        }
        PRINT_ERR("************************************************\n\n");
        return;
    }

    taskCB = OS_TCB_FROM_TID(taskID);
    if ((taskCB->taskStatus & OS_TASK_STATUS_UNUSED) ||
        (taskCB->taskEntry == NULL) ||
        (taskCB->taskName == NULL)) {
        PRINT_ERR("The task [ID: 0x%x] is NOT CREATED(ILLEGAL)\n", taskID);
        PRINT_ERR("************************************************\n\n");
        return;
    }

    PRINT_ERR("allocated by task: %s [ID = 0x%x]\n", taskCB->taskName, taskID);
#ifdef LOSCFG_MEM_MUL_MODULE
    PRINT_ERR("allocated by moduleID: %lu\n", OS_MEM_MODID_GET(node));
#endif

    PRINT_ERR("************************************************\n\n");
}

STATIC INLINE VOID OsMemDispWildPointerMsg(const LosMemDynNode *node, const VOID *ptr)
{
    PRINT_ERR("*****************************************************\n");
    PRINT_ERR("find an control block at: %p, gap size: 0x%x, sizeof(LosMemDynNode): 0x%x\n", node,
              node->selfNode.gapSize, sizeof(LosMemDynNode));
    PRINT_ERR("the pointer should be: %p\n",
              ((UINTPTR)node + node->selfNode.gapSize + sizeof(LosMemDynNode)));
    PRINT_ERR("the pointer given is: %p\n", ptr);
    PRINT_ERR("PROBABLY A WILD POINTER\n");
    OsBackTrace();
    PRINT_ERR("*****************************************************\n\n");
}

STATIC INLINE VOID OsMemChecksumSet(LosMemCtlNode *ctlNode)
{
    ctlNode->checksum = OS_MEM_NODE_CHECKSUN_CALCULATE(ctlNode);
}

STATIC INLINE BOOL OsMemChecksumVerify(const LosMemCtlNode *ctlNode)
{
    return ctlNode->checksum == OS_MEM_NODE_CHECKSUN_CALCULATE(ctlNode);
}

STATIC INLINE VOID OsMemBackupSetup(const LosMemDynNode *node)
{
    LosMemDynNode *nodePre = node->selfNode.preNode;
    if (nodePre != NULL) {
        nodePre->backupNode.freeNodeInfo.pstNext = node->selfNode.freeNodeInfo.pstNext;
        nodePre->backupNode.freeNodeInfo.pstPrev = node->selfNode.freeNodeInfo.pstPrev;
        nodePre->backupNode.preNode = node->selfNode.preNode;
        nodePre->backupNode.checksum = node->selfNode.checksum;
        nodePre->backupNode.gapSize = node->selfNode.gapSize;
#ifdef LOSCFG_MEM_RECORDINFO
        nodePre->backupNode.originSize = node->selfNode.originSize;
#endif
        nodePre->backupNode.sizeAndFlag = node->selfNode.sizeAndFlag;
    }
}

LosMemDynNode *OsMemNodeNextGet(const VOID *pool, const LosMemDynNode *node)
{
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;

    if (node == OS_MEM_END_NODE(pool, poolInfo->poolSize)) {
        return OS_MEM_FIRST_NODE(pool);
    } else {
        return OS_MEM_NEXT_NODE(node);
    }
}

STATIC INLINE UINT32 OsMemBackupSetup4Next(const VOID *pool, LosMemDynNode *node)
{
    LosMemDynNode *nodeNext = OsMemNodeNextGet(pool, node);

    if (!OsMemChecksumVerify(&nodeNext->selfNode)) {
        PRINT_ERR("[%s]the next node is broken!!\n", __FUNCTION__);
        OsMemDispCtlNode(&(nodeNext->selfNode));
        PRINT_ERR("Current node details:\n");
        OsMemDispMoreDetails(node);

        return LOS_NOK;
    }

    if (!OsMemChecksumVerify(&node->backupNode)) {
        node->backupNode.freeNodeInfo.pstNext = nodeNext->selfNode.freeNodeInfo.pstNext;
        node->backupNode.freeNodeInfo.pstPrev = nodeNext->selfNode.freeNodeInfo.pstPrev;
        node->backupNode.preNode = nodeNext->selfNode.preNode;
        node->backupNode.checksum = nodeNext->selfNode.checksum;
        node->backupNode.gapSize = nodeNext->selfNode.gapSize;
#ifdef LOSCFG_MEM_RECORDINFO
        node->backupNode.originSize = nodeNext->selfNode.originSize;
#endif
        node->backupNode.sizeAndFlag = nodeNext->selfNode.sizeAndFlag;
    }
    return LOS_OK;
}

UINT32 OsMemBackupDoRestore(VOID *pool, const LosMemDynNode *nodePre, LosMemDynNode *node)
{
    if (node == NULL) {
        PRINT_ERR("the node is NULL.\n");
        return LOS_NOK;
    }
    PRINT_ERR("the backup node information of current node in previous node:\n");
    OsMemDispCtlNode(&nodePre->backupNode);
    PRINT_ERR("the detailed information of previous node:\n");
    OsMemDispMoreDetails(nodePre);

    node->selfNode.freeNodeInfo.pstNext = nodePre->backupNode.freeNodeInfo.pstNext;
    node->selfNode.freeNodeInfo.pstPrev = nodePre->backupNode.freeNodeInfo.pstPrev;
    node->selfNode.preNode = nodePre->backupNode.preNode;
    node->selfNode.checksum = nodePre->backupNode.checksum;
    node->selfNode.gapSize = nodePre->backupNode.gapSize;
#ifdef LOSCFG_MEM_RECORDINFO
    node->selfNode.originSize = nodePre->backupNode.originSize;
#endif
    node->selfNode.sizeAndFlag = nodePre->backupNode.sizeAndFlag;

    /* we should re-setup next node's backup on current node */
    return OsMemBackupSetup4Next(pool, node);
}

STATIC LosMemDynNode *OsMemFirstNodePrevGet(const LosMemPoolInfo *poolInfo)
{
    LosMemDynNode *nodePre = NULL;

    nodePre = OS_MEM_END_NODE(poolInfo, poolInfo->poolSize);
    if (!OsMemChecksumVerify(&(nodePre->selfNode))) {
        PRINT_ERR("the current node is THE FIRST NODE !\n");
        PRINT_ERR("[%s]: the node information of previous node is bad !!\n", __FUNCTION__);
        OsMemDispCtlNode(&(nodePre->selfNode));
        return nodePre;
    }
    if (!OsMemChecksumVerify(&(nodePre->backupNode))) {
        PRINT_ERR("the current node is THE FIRST NODE !\n");
        PRINT_ERR("[%s]: the backup node information of current node in previous Node is bad !!\n", __FUNCTION__);
        OsMemDispCtlNode(&(nodePre->backupNode));
        return nodePre;
    }

    return NULL;
}

LosMemDynNode *OsMemNodePrevGet(VOID *pool, const LosMemDynNode *node)
{
    LosMemDynNode *nodeCur = NULL;
    LosMemDynNode *nodePre = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;

    if (node == OS_MEM_FIRST_NODE(pool)) {
        return OsMemFirstNodePrevGet(poolInfo);
    }

    for (nodeCur = OS_MEM_FIRST_NODE(pool);
         nodeCur < OS_MEM_END_NODE(pool, poolInfo->poolSize);
         nodeCur = OS_MEM_NEXT_NODE(nodeCur)) {
        if (!OsMemChecksumVerify(&(nodeCur->selfNode))) {
            PRINT_ERR("[%s]: the node information of current node is bad !!\n", __FUNCTION__);
            OsMemDispCtlNode(&(nodeCur->selfNode));

            if (nodePre == NULL) {
                return NULL;
            }

            PRINT_ERR("the detailed information of previous node:\n");
            OsMemDispMoreDetails(nodePre);

            /* due to the every step's checksum verify, nodePre is trustful */
            if (OsMemBackupDoRestore(pool, nodePre, nodeCur) != LOS_OK) {
                return NULL;
            }
        }

        if (!OsMemChecksumVerify(&(nodeCur->backupNode))) {
            PRINT_ERR("[%s]: the backup node information in current node is bad !!\n", __FUNCTION__);
            OsMemDispCtlNode(&(nodeCur->backupNode));

            if (nodePre != NULL) {
                PRINT_ERR("the detailed information of previous node:\n");
                OsMemDispMoreDetails(nodePre);
            }

            if (OsMemBackupSetup4Next(pool, nodeCur) != LOS_OK) {
                return NULL;
            }
        }

        if (OS_MEM_NEXT_NODE(nodeCur) == node) {
            return nodeCur;
        }

        if (OS_MEM_NEXT_NODE(nodeCur) > node) {
            break;
        }

        nodePre = nodeCur;
    }

    return NULL;
}

LosMemDynNode *OsMemNodePrevTryGet(VOID *pool, LosMemDynNode **node, const VOID *ptr)
{
    UINTPTR nodeShoudBe = 0;
    LosMemDynNode *nodeCur = NULL;
    LosMemDynNode *nodePre = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;

    if (ptr == OS_MEM_FIRST_NODE(pool)) {
        return OsMemFirstNodePrevGet(poolInfo);
    }

    for (nodeCur = OS_MEM_FIRST_NODE(pool);
         nodeCur < OS_MEM_END_NODE(pool, poolInfo->poolSize);
         nodeCur = OS_MEM_NEXT_NODE(nodeCur)) {
        if (!OsMemChecksumVerify(&(nodeCur->selfNode))) {
            PRINT_ERR("[%s]: the node information of current node is bad !!\n", __FUNCTION__);
            OsMemDispCtlNode(&(nodeCur->selfNode));

            if (nodePre == NULL) {
                return NULL;
            }

            PRINT_ERR("the detailed information of previous node:\n");
            OsMemDispMoreDetails(nodePre);

            /* due to the every step's checksum verify, nodePre is trustful */
            if (OsMemBackupDoRestore(pool, nodePre, nodeCur) != LOS_OK) {
                return NULL;
            }
        }

        if (!OsMemChecksumVerify(&(nodeCur->backupNode))) {
            PRINT_ERR("[%s]: the backup node information in current node is bad !!\n", __FUNCTION__);
            OsMemDispCtlNode(&(nodeCur->backupNode));

            if (nodePre != NULL) {
                PRINT_ERR("the detailed information of previous node:\n");
                OsMemDispMoreDetails(nodePre);
            }

            if (OsMemBackupSetup4Next(pool, nodeCur) != LOS_OK) {
                return NULL;
            }
        }

        nodeShoudBe = (UINTPTR)nodeCur + nodeCur->selfNode.gapSize + sizeof(LosMemDynNode);
        if (nodeShoudBe == (UINTPTR)ptr) {
            *node = nodeCur;
            return nodePre;
        }

        if (OS_MEM_NEXT_NODE(nodeCur) > (LosMemDynNode *)ptr) {
            OsMemDispWildPointerMsg(nodeCur, ptr);
            break;
        }

        nodePre = nodeCur;
    }

    return NULL;
}

STATIC INLINE UINT32 OsMemBackupTryRestore(VOID *pool, LosMemDynNode **node, const VOID *ptr)
{
    LosMemDynNode *nodeHead = NULL;
    LosMemDynNode *nodePre = OsMemNodePrevTryGet(pool, &nodeHead, ptr);
    if (nodePre == NULL) {
        return LOS_NOK;
    }

    *node = nodeHead;
    return OsMemBackupDoRestore(pool, nodePre, *node);
}

STATIC INLINE UINT32 OsMemBackupRestore(VOID *pool, LosMemDynNode *node)
{
    LosMemDynNode *nodePre = OsMemNodePrevGet(pool, node);
    if (nodePre == NULL) {
        return LOS_NOK;
    }

    return OsMemBackupDoRestore(pool, nodePre, node);
}

STATIC INLINE VOID OsMemSetGapSize(LosMemCtlNode *ctlNode, UINT32 gapSize)
{
    ctlNode->gapSize = gapSize;
}

STATIC VOID OsMemNodeSave(LosMemDynNode *node)
{
    OsMemSetGapSize(&node->selfNode, 0);
    OsMemChecksumSet(&node->selfNode);
    OsMemBackupSetup(node);
}

STATIC VOID OsMemNodeSaveWithGapSize(LosMemDynNode *node, UINT32 gapSize)
{
    OsMemSetGapSize(&node->selfNode, gapSize);
    OsMemChecksumSet(&node->selfNode);
    OsMemBackupSetup(node);
}

STATIC VOID OsMemListDelete(LOS_DL_LIST *node, const VOID *firstNode)
{
    LosMemDynNode *dynNode = NULL;

    node->pstNext->pstPrev = node->pstPrev;
    node->pstPrev->pstNext = node->pstNext;

    if ((VOID *)(node->pstNext) >= firstNode) {
        dynNode = LOS_DL_LIST_ENTRY(node->pstNext, LosMemDynNode, selfNode.freeNodeInfo);
        OsMemNodeSave(dynNode);
    }

    if ((VOID *)(node->pstPrev) >= firstNode) {
        dynNode = LOS_DL_LIST_ENTRY(node->pstPrev, LosMemDynNode, selfNode.freeNodeInfo);
        OsMemNodeSave(dynNode);
    }

    node->pstNext = NULL;
    node->pstPrev = NULL;

    dynNode = LOS_DL_LIST_ENTRY(node, LosMemDynNode, selfNode.freeNodeInfo);
    OsMemNodeSave(dynNode);
}

STATIC VOID OsMemListAdd(LOS_DL_LIST *listNode, LOS_DL_LIST *node, const VOID *firstNode)
{
    LosMemDynNode *dynNode = NULL;

    node->pstNext = listNode->pstNext;
    node->pstPrev = listNode;

    dynNode = LOS_DL_LIST_ENTRY(node, LosMemDynNode, selfNode.freeNodeInfo);
    OsMemNodeSave(dynNode);

    listNode->pstNext->pstPrev = node;
    if ((VOID *)(listNode->pstNext) >= firstNode) {
        dynNode = LOS_DL_LIST_ENTRY(listNode->pstNext, LosMemDynNode, selfNode.freeNodeInfo);
        OsMemNodeSave(dynNode);
    }

    listNode->pstNext = node;
}

VOID LOS_MemBadNodeShow(VOID *pool)
{
    LosMemDynNode *nodePre = NULL;
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 intSave;

    if (pool == NULL) {
        return;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode <= OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        OsMemDispCtlNode(&tmpNode->selfNode);

        if (OsMemChecksumVerify(&tmpNode->selfNode)) {
            continue;
        }

        nodePre = OsMemNodePrevGet(pool, tmpNode);
        if (nodePre == NULL) {
            PRINT_ERR("the current node is invalid, but cannot find its previous Node\n");
            continue;
        }

        PRINT_ERR("the detailed information of previous node:\n");
        OsMemDispMoreDetails(nodePre);
    }

    MEM_UNLOCK(intSave);
    PRINTK("check finish\n");
}

#else  /* without LOSCFG_MEM_HEAD_BACKUP */

STATIC VOID OsMemListDelete(LOS_DL_LIST *node, const VOID *firstNode)
{
    (VOID)firstNode;
    node->pstNext->pstPrev = node->pstPrev;
    node->pstPrev->pstNext = node->pstNext;
    node->pstNext = NULL;
    node->pstPrev = NULL;
}

STATIC VOID OsMemListAdd(LOS_DL_LIST *listNode, LOS_DL_LIST *node, const VOID *firstNode)
{
    (VOID)firstNode;
    node->pstNext = listNode->pstNext;
    node->pstPrev = listNode;
    listNode->pstNext->pstPrev = node;
    listNode->pstNext = node;
}

#endif

#ifdef LOSCFG_EXC_INTERACTION
LITE_OS_SEC_TEXT_INIT UINT32 OsMemExcInteractionInit(UINTPTR memStart)
{
    UINT32 ret;
    UINT32 poolSize;
    m_aucSysMem0 = (UINT8 *)((memStart + (POOL_ADDR_ALIGNSIZE - 1)) & ~((UINTPTR)(POOL_ADDR_ALIGNSIZE - 1)));
    poolSize = OS_EXC_INTERACTMEM_SIZE;
    ret = LOS_MemInit(m_aucSysMem0, poolSize);
    PRINT_INFO("LiteOS kernel exc interaction memory address:%p,size:0x%x\n", m_aucSysMem0, poolSize);
    return ret;
}
#endif

LITE_OS_SEC_TEXT_INIT UINT32 OsMemSystemInit(UINTPTR memStart)
{
    UINT32 ret;
    UINT32 poolSize;

    m_aucSysMem1 = (UINT8 *)((memStart + (POOL_ADDR_ALIGNSIZE - 1)) & ~((UINTPTR)(POOL_ADDR_ALIGNSIZE - 1)));
    poolSize = OS_SYS_MEM_SIZE;
    ret = LOS_MemInit(m_aucSysMem1, poolSize);
    PRINT_INFO("LiteOS system heap memory address:%p,size:0x%x\n", m_aucSysMem1, poolSize);
#ifndef LOSCFG_EXC_INTERACTION
    m_aucSysMem0 = m_aucSysMem1;
#endif
    return ret;
}

#ifdef LOSCFG_MEM_LEAKCHECK
STATIC INLINE VOID OsMemLinkRegisterRecord(LosMemDynNode *node)
{
    UINT32 count = 0;
    UINT32 index = 0;
    UINTPTR framePtr, tmpFramePtr, linkReg;

    (VOID)memset_s(node->selfNode.linkReg, (LOS_RECORD_LR_CNT * sizeof(UINTPTR)), 0,
        (LOS_RECORD_LR_CNT * sizeof(UINTPTR)));
    framePtr = Get_Fp();
    while ((framePtr > OS_SYS_FUNC_ADDR_START) && (framePtr < OS_SYS_FUNC_ADDR_END)) {
        tmpFramePtr = framePtr;
#ifdef __LP64__
        framePtr = *(UINTPTR *)framePtr;
        linkReg = *(UINTPTR *)(tmpFramePtr + sizeof(UINTPTR));
#else
        linkReg = *(UINTPTR *)framePtr;
        framePtr = *(UINTPTR *)(tmpFramePtr - sizeof(UINTPTR));
#endif
        if (index >= LOS_OMIT_LR_CNT) {
            node->selfNode.linkReg[count++] = linkReg;
            if (count == LOS_RECORD_LR_CNT) {
                break;
            }
        }
        index++;
    }
    return;
}
#endif

#define OS_CHECK_NULL_RETURN(param) do {              \
    if ((param) == NULL) {                            \
        PRINT_ERR("%s %d\n", __FUNCTION__, __LINE__); \
        return;                                       \
    }                                                 \
} while (0);

/*
 * Description : find suitable free block use "best fit" algorithm
 * Input       : pool      --- Pointer to memory pool
 *               allocSize --- Size of memory in bytes which note need allocate
 * Return      : NULL      --- no suitable block found
 *               tmpNode   --- pointer a suitable free block
 */
STATIC INLINE LosMemDynNode *OsMemFindSuitableFreeBlock(VOID *pool, UINT32 allocSize)
{
    LOS_DL_LIST *listNodeHead = NULL;
    LosMemDynNode *tmpNode = NULL;
    UINT32 maxCount = (((LosMemPoolInfo *)pool)->poolSize / allocSize) << 1;
    UINT32 count;
#ifdef LOSCFG_MEM_HEAD_BACKUP
    UINT32 ret = LOS_OK;
#endif
    for (listNodeHead = OS_MEM_HEAD(pool, allocSize); listNodeHead != NULL;
         listNodeHead = OsDLnkNextMultiHead(OS_MEM_HEAD_ADDR(pool), listNodeHead)) {
        count = 0;
        LOS_DL_LIST_FOR_EACH_ENTRY(tmpNode, listNodeHead, LosMemDynNode, selfNode.freeNodeInfo) {
            if (count++ >= maxCount) {
                PRINT_ERR("[%s:%d]node: %p execute too much time\n", __FUNCTION__, __LINE__, tmpNode);
                break;
            }

#ifdef LOSCFG_MEM_HEAD_BACKUP
            if (!OsMemChecksumVerify(&tmpNode->selfNode)) {
                PRINT_ERR("[%s]: the node information of current node is bad !!\n", __FUNCTION__);
                OsMemDispCtlNode(&tmpNode->selfNode);
                ret = OsMemBackupRestore(pool, tmpNode);
            }
            if (ret != LOS_OK) {
                break;
            }
#endif

            if (((UINTPTR)tmpNode < (UINTPTR)pool) ||
                ((UINTPTR)tmpNode > ((UINTPTR)pool + ((LosMemPoolInfo *)pool)->poolSize)) ||
                (((UINTPTR)tmpNode & (OS_MEM_ALIGN_SIZE - 1)) != 0)) {
                LOS_Panic("[%s:%d]Mem node data error:OS_MEM_HEAD_ADDR(pool)=%p, listNodeHead:%p,"
                          "allocSize=%u, tmpNode=%p\n",
                          __FUNCTION__, __LINE__, OS_MEM_HEAD_ADDR(pool), listNodeHead, allocSize, tmpNode);
                break;
            }
            if (tmpNode->selfNode.sizeAndFlag >= allocSize) {
                return tmpNode;
            }
        }
    }

    return NULL;
}

/*
 * Description : clear a mem node, set every member to NULL
 * Input       : node    --- Pointer to the mem node which will be cleared up
 */
STATIC INLINE VOID OsMemClearNode(LosMemDynNode *node)
{
    (VOID)memset_s((VOID *)node, sizeof(LosMemDynNode), 0, sizeof(LosMemDynNode));
}

/*
 * Description : merge this node and pre node, then clear this node info
 * Input       : node    --- Pointer to node which will be merged
 */
STATIC INLINE VOID OsMemMergeNode(LosMemDynNode *node)
{
    LosMemDynNode *nextNode = NULL;

    node->selfNode.preNode->selfNode.sizeAndFlag += node->selfNode.sizeAndFlag;
    nextNode = (LosMemDynNode *)((UINTPTR)node + node->selfNode.sizeAndFlag);
    nextNode->selfNode.preNode = node->selfNode.preNode;
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(node->selfNode.preNode);
    OsMemNodeSave(nextNode);
#endif
    OsMemClearNode(node);
}

/*
 * Description : split new node from allocNode, and merge remainder mem if necessary
 * Input       : pool      -- Pointer to memory pool
 *               allocNode -- the source node which new node be spit from to.
 *                            After pick up it's node info, change to point the new node
 *               allocSize -- the size of new node
 * Output      : allocNode -- save new node addr
 */
STATIC INLINE VOID OsMemSplitNode(VOID *pool,
                                  LosMemDynNode *allocNode, UINT32 allocSize)
{
    LosMemDynNode *newFreeNode = NULL;
    LosMemDynNode *nextNode = NULL;
    LOS_DL_LIST *listNodeHead = NULL;
    const VOID *firstNode = (const VOID *)((UINT8 *)OS_MEM_HEAD_ADDR(pool) + OS_DLNK_HEAD_SIZE);

    newFreeNode = (LosMemDynNode *)(VOID *)((UINT8 *)allocNode + allocSize);
    newFreeNode->selfNode.preNode = allocNode;
    newFreeNode->selfNode.sizeAndFlag = allocNode->selfNode.sizeAndFlag - allocSize;
    allocNode->selfNode.sizeAndFlag = allocSize;
    nextNode = OS_MEM_NEXT_NODE(newFreeNode);
    nextNode->selfNode.preNode = newFreeNode;
    if (!OS_MEM_NODE_GET_USED_FLAG(nextNode->selfNode.sizeAndFlag)) {
        OsMemListDelete(&nextNode->selfNode.freeNodeInfo, firstNode);
        OsMemMergeNode(nextNode);
    }
#ifdef LOSCFG_MEM_HEAD_BACKUP
    else {
        OsMemNodeSave(nextNode);
    }
#endif
    listNodeHead = OS_MEM_HEAD(pool, newFreeNode->selfNode.sizeAndFlag);
    OS_CHECK_NULL_RETURN(listNodeHead);

    OsMemListAdd(listNodeHead, &newFreeNode->selfNode.freeNodeInfo, firstNode);
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(newFreeNode);
#endif
}

/*
 * Description : free the node from memory & if there are free node beside, merger them.
 *               at last update "listNodeHead' which saved all free node control head
 * Input       : node -- the node which need be freed
 *               pool -- Pointer to memory pool
 */
STATIC INLINE VOID OsMemFreeNode(LosMemDynNode *node, VOID *pool)
{
    LosMemDynNode *nextNode = NULL;
    LOS_DL_LIST *listNodeHead = NULL;
    const VOID *firstNode = (const VOID *)((UINT8 *)OS_MEM_HEAD_ADDR(pool) + OS_DLNK_HEAD_SIZE);
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    poolInfo->poolCurUsedSize -= OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag);
#endif
    OS_MEM_REDUCE_USED(poolInfo->memStats, OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag), OS_MEM_TASKID_GET(node));
    node->selfNode.sizeAndFlag = OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag);
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(node);
#endif
#ifdef LOSCFG_MEM_LEAKCHECK
    OsMemLinkRegisterRecord(node);
#endif
    if ((node->selfNode.preNode != NULL) &&
        !OS_MEM_NODE_GET_USED_FLAG(node->selfNode.preNode->selfNode.sizeAndFlag)) {
        LosMemDynNode *preNode = node->selfNode.preNode;
        OsMemMergeNode(node);
        nextNode = OS_MEM_NEXT_NODE(preNode);
        if (!OS_MEM_NODE_GET_USED_FLAG(nextNode->selfNode.sizeAndFlag)) {
            OsMemListDelete(&nextNode->selfNode.freeNodeInfo, firstNode);
            OsMemMergeNode(nextNode);
        }

        OsMemListDelete(&(preNode->selfNode.freeNodeInfo), firstNode);
        listNodeHead = OS_MEM_HEAD(pool, preNode->selfNode.sizeAndFlag);
        OS_CHECK_NULL_RETURN(listNodeHead);

        OsMemListAdd(listNodeHead, &preNode->selfNode.freeNodeInfo, firstNode);
    } else {
        nextNode = OS_MEM_NEXT_NODE(node);
        if (!OS_MEM_NODE_GET_USED_FLAG(nextNode->selfNode.sizeAndFlag)) {
            OsMemListDelete(&nextNode->selfNode.freeNodeInfo, firstNode);
            OsMemMergeNode(nextNode);
        }

        listNodeHead = OS_MEM_HEAD(pool, node->selfNode.sizeAndFlag);
        OS_CHECK_NULL_RETURN(listNodeHead);

        OsMemListAdd(listNodeHead, &node->selfNode.freeNodeInfo, firstNode);
    }
}

/*
 * Description : check the result if pointer memory node belongs to pointer memory pool
 * Input       : pool -- Pointer to memory pool
 *               node -- the node which need be checked
 * Return      : LOS_OK or LOS_NOK
 */
#ifdef LOS_DLNK_SAFE_CHECK
STATIC INLINE UINT32 OsMemCheckUsedNode(const VOID *pool, const LosMemDynNode *node)
{
    LosMemDynNode *tmpNode = NULL;
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;
    const LosMemDynNode *endNode = (const LosMemDynNode *)OS_MEM_END_NODE(pool, poolInfo->poolSize);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode < endNode; tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if ((tmpNode == node) &&
            OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
            return LOS_OK;
        } else if (tmpNode > node) {
            return LOS_NOK;
        }
    }

    return LOS_NOK;
}

#elif defined(LOS_DLNK_SIMPLE_CHECK)
STATIC INLINE UINT32 OsMemCheckUsedNode(const VOID *pool, const LosMemDynNode *node)
{
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;
    const LosMemDynNode *startNode = (const LosMemDynNode *)OS_MEM_FIRST_NODE(pool);
    const LosMemDynNode *endNode = (const LosMemDynNode *)OS_MEM_END_NODE(pool, poolInfo->poolSize);
    if (!OS_MEM_MIDDLE_ADDR_OPEN_END(startNode, node, endNode)) {
        return LOS_NOK;
    }

    if (!OS_MEM_NODE_GET_USED_FLAG(node->selfNode.sizeAndFlag)) {
        return LOS_NOK;
    }

    if (!OS_MEM_MAGIC_VALID(node->selfNode.freeNodeInfo.pstPrev)) {
        return LOS_NOK;
    }

    return LOS_OK;
}

#else
STATIC INLINE BOOL OsMemIsNodeValid(const LosMemDynNode *node, const LosMemDynNode *startNode,
                                    const LosMemDynNode *endNode,
                                    const UINT8 *startPool, const UINT8 *endPool)
{
    if (!OS_MEM_MIDDLE_ADDR(startNode, node, endNode)) {
        return FALSE;
    }

    if (OS_MEM_NODE_GET_USED_FLAG(node->selfNode.sizeAndFlag)) {
        if (!OS_MEM_MAGIC_VALID(node->selfNode.freeNodeInfo.pstPrev)) {
            return FALSE;
        }
        return TRUE;
    }

    if (!OS_MEM_MIDDLE_ADDR_OPEN_END(startPool, node->selfNode.freeNodeInfo.pstPrev, endPool)) {
        return FALSE;
    }

    return TRUE;
}

STATIC INLINE UINT32 OsMemCheckUsedNode(const VOID *pool, const LosMemDynNode *node)
{
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;
    const LosMemDynNode *startNode = (const LosMemDynNode *)OS_MEM_FIRST_NODE(pool);
    const LosMemDynNode *endNode = (const LosMemDynNode *)OS_MEM_END_NODE(pool, poolInfo->poolSize);
    const UINT8 *endPool = (const UINT8 *)pool + poolInfo->poolSize;
    const LosMemDynNode *nextNode = NULL;
    if (!OsMemIsNodeValid(node, startNode, endNode, (UINT8 *)pool, endPool)) {
        return LOS_NOK;
    }

    if (!OS_MEM_NODE_GET_USED_FLAG(node->selfNode.sizeAndFlag)) {
        return LOS_NOK;
    }

    nextNode = OS_MEM_NEXT_NODE(node);
    if (!OsMemIsNodeValid(nextNode, startNode, endNode, (UINT8 *)pool, endPool)) {
        return LOS_NOK;
    }

    if (nextNode->selfNode.preNode != node) {
        return LOS_NOK;
    }

    if (node != startNode) {
        if (!OsMemIsNodeValid(node->selfNode.preNode, startNode, endNode, (UINT8 *)pool, endPool)) {
            return LOS_NOK;
        }

        if (OS_MEM_NEXT_NODE(node->selfNode.preNode) != node) {
            return LOS_NOK;
        }
    }

    return LOS_OK;
}

#endif

/*
 * Description : set magic & taskid
 * Input       : node -- the node which will be set magic & taskid
 */
STATIC INLINE VOID OsMemSetMagicNumAndTaskID(LosMemDynNode *node)
{
    LosTaskCB *runTask = OsCurrTaskGet();

    OS_MEM_SET_MAGIC(node->selfNode.freeNodeInfo.pstPrev);

    /*
     * If the operation occured before task initialization(runTask was not assigned)
     * or in interrupt,make the value of taskid of node to 0xffffffff
     */
    if ((runTask != NULL) && OS_INT_INACTIVE) {
        OS_MEM_TASKID_SET(node, runTask->taskID);
    } else {
        /* If the task mode does not initialize, the field is the 0xffffffff */
        node->selfNode.freeNodeInfo.pstNext = (LOS_DL_LIST *)OS_NULL_INT;
    }
}

LITE_OS_SEC_TEXT_MINOR STATIC INLINE UINT32 OsMemPoolDlinkcheck(const LosMemPoolInfo *pool, LOS_DL_LIST listHead)
{
    if (((UINTPTR)listHead.pstPrev < (UINTPTR)(pool + 1)) ||
        ((UINTPTR)listHead.pstPrev >= ((UINTPTR)pool + pool->poolSize)) ||
        ((UINTPTR)listHead.pstNext < (UINTPTR)(pool + 1)) ||
        ((UINTPTR)listHead.pstNext >= ((UINTPTR)pool + pool->poolSize)) ||
        !IS_ALIGNED(listHead.pstPrev, sizeof(VOID *)) ||
        !IS_ALIGNED(listHead.pstNext, sizeof(VOID *))) {
        return LOS_NOK;
    }

    return LOS_OK;
}

/*
 * Description : show mem pool header info
 * Input       : pool --Pointer to memory pool
 */
LITE_OS_SEC_TEXT_MINOR VOID OsMemPoolHeadInfoPrint(const VOID *pool)
{
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;
    UINT32 dlinkNum;
    UINT32 flag = 0;
    const LosMultipleDlinkHead *dlinkHead = NULL;

    if (!IS_ALIGNED(poolInfo, sizeof(VOID *))) {
        PRINT_ERR("wrong mem pool addr: %p, func:%s,line:%d\n", poolInfo, __FUNCTION__, __LINE__);
#ifdef LOSCFG_SHELL_EXCINFO
        WriteExcInfoToBuf("wrong mem pool addr: %p, func:%s,line:%d\n", poolInfo, __FUNCTION__, __LINE__);
#endif
        return;
    }

    dlinkHead = (const LosMultipleDlinkHead *)(VOID *)(poolInfo + 1);
    for (dlinkNum = 0; dlinkNum < OS_MULTI_DLNK_NUM; dlinkNum++) {
        if (OsMemPoolDlinkcheck(pool, dlinkHead->listHead[dlinkNum])) {
            flag = 1;
            PRINT_ERR("DlinkHead[%u]: pstPrev:%p, pstNext:%p\n",
                      dlinkNum, dlinkHead->listHead[dlinkNum].pstPrev, dlinkHead->listHead[dlinkNum].pstNext);
#ifdef LOSCFG_SHELL_EXCINFO
            WriteExcInfoToBuf("DlinkHead[%u]: pstPrev:%p, pstNext:%p\n",
                              dlinkNum, dlinkHead->listHead[dlinkNum].pstPrev, dlinkHead->listHead[dlinkNum].pstNext);
#endif
        }
    }
    if (flag) {
        PRINTK("mem pool info: poolAddr:%p, poolSize:0x%x\n", poolInfo->pool, poolInfo->poolSize);
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
        PRINTK("mem pool info: poolWaterLine:0x%x, poolCurUsedSize:0x%x\n", poolInfo->poolWaterLine,
               poolInfo->poolCurUsedSize);
#endif

#ifdef LOSCFG_SHELL_EXCINFO
        WriteExcInfoToBuf("mem pool info: poolAddr:%p, poolSize:0x%x\n", poolInfo->pool, poolInfo->poolSize);
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
        WriteExcInfoToBuf("mem pool info: poolWaterLine:0x%x, poolCurUsedSize:0x%x\n",
                          poolInfo->poolWaterLine, poolInfo->poolCurUsedSize);
#endif
#endif
    }
}

STATIC UINT32 OsMemIntegrityCheck(const VOID *pool,
                                  LosMemDynNode **tmpNode,
                                  LosMemDynNode **preNode)
{
    const LosMemPoolInfo *poolInfo = (const LosMemPoolInfo *)pool;
    const UINT8 *endPool = (const UINT8 *)pool + poolInfo->poolSize;

    OsMemPoolHeadInfoPrint(pool);

    *preNode = OS_MEM_FIRST_NODE(pool);
    for (*tmpNode = OS_MEM_FIRST_NODE(pool); *tmpNode < OS_MEM_END_NODE(pool, poolInfo->poolSize);
         *tmpNode = OS_MEM_NEXT_NODE(*tmpNode)) {
        if (OS_MEM_NODE_GET_USED_FLAG((*tmpNode)->selfNode.sizeAndFlag)) {
            if (!OS_MEM_MAGIC_VALID((*tmpNode)->selfNode.freeNodeInfo.pstPrev)) {
                PRINT_ERR("[%s], %d, memory check error!\n"
                          "memory used but magic num wrong, freeNodeInfo.pstPrev(magic num):%p \n",
                          __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstPrev);
#ifdef LOSCFG_SHELL_EXCINFO
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                                  "memory used but magic num wrong, freeNodeInfo.pstPrev(magic num):%p \n",
                                  __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstPrev);
#endif
                return LOS_NOK;
            }
        } else { /* is free node, check free node range */
            if (!OS_MEM_MIDDLE_ADDR_OPEN_END(pool, (*tmpNode)->selfNode.freeNodeInfo.pstPrev, endPool)) {
                PRINT_ERR("[%s], %d, memory check error!\n"
                          " freeNodeInfo.pstPrev:%p is out of legal mem range[%p, %p]\n",
                          __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstPrev, pool, endPool);
#ifdef LOSCFG_SHELL_EXCINFO
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                                  " freeNodeInfo.pstPrev:%p is out of legal mem range[%p, %p]\n",
                                  __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstPrev, pool, endPool);
#endif
                return LOS_NOK;
            }
            if (!OS_MEM_MIDDLE_ADDR_OPEN_END(pool, (*tmpNode)->selfNode.freeNodeInfo.pstNext, endPool)) {
                PRINT_ERR("[%s], %d, memory check error!\n"
                          " freeNodeInfo.pstNext:%p is out of legal mem range[%p, %p]\n",
                          __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstNext, pool, endPool);
#ifdef LOSCFG_SHELL_EXCINFO
                WriteExcInfoToBuf("[%s], %d, memory check error!\n"
                                  " freeNodeInfo.pstNext:%p is out of legal mem range[%p, %p]\n",
                                  __FUNCTION__, __LINE__, (*tmpNode)->selfNode.freeNodeInfo.pstNext, pool, endPool);
#endif
                return LOS_NOK;
            }
        }

        *preNode = *tmpNode;
    }
    return LOS_OK;
}

#ifdef LOSCFG_MEM_LEAKCHECK
STATIC VOID OsMemNodeBacktraceInfo(const LosMemDynNode *tmpNode,
                                   const LosMemDynNode *preNode)
{
    int i;
    PRINTK("\n broken node head LR info: \n");
    for (i = 0; i < LOS_RECORD_LR_CNT; i++) {
        PRINTK(" LR[%d]:%p\n", i, tmpNode->selfNode.linkReg[i]);
    }
    PRINTK("\n pre node head LR info: \n");
    for (i = 0; i < LOS_RECORD_LR_CNT; i++) {
        PRINTK(" LR[%d]:%p\n", i, preNode->selfNode.linkReg[i]);
    }

#ifdef LOSCFG_SHELL_EXCINFO
    WriteExcInfoToBuf("\n broken node head LR info: \n");
    for (i = 0; i < LOS_RECORD_LR_CNT; i++) {
        WriteExcInfoToBuf("LR[%d]:%p\n", tmpNode->selfNode.linkReg[i]);
    }
    WriteExcInfoToBuf("\n pre node head LR info: \n");
    for (i = 0; i < LOS_RECORD_LR_CNT; i++) {
        WriteExcInfoToBuf("LR[%d]:%p\n", preNode->selfNode.linkReg[i]);
    }
#endif
}
#endif

STATIC VOID OsMemNodeInfo(const LosMemDynNode *tmpNode,
                          const LosMemDynNode *preNode)
{
    if (tmpNode == preNode) {
        PRINTK("\n the broken node is the first node\n");
#ifdef LOSCFG_SHELL_EXCINFO
        WriteExcInfoToBuf("\n the broken node is the first node\n");
#endif
    }
    PRINTK("\n broken node head: %p  %p  %p  0x%x, pre node head: %p  %p  %p  0x%x\n",
           tmpNode->selfNode.freeNodeInfo.pstPrev, tmpNode->selfNode.freeNodeInfo.pstNext,
           tmpNode->selfNode.preNode, tmpNode->selfNode.sizeAndFlag,
           preNode->selfNode.freeNodeInfo.pstPrev, preNode->selfNode.freeNodeInfo.pstNext,
           preNode->selfNode.preNode, preNode->selfNode.sizeAndFlag);

#ifdef LOSCFG_SHELL_EXCINFO
    WriteExcInfoToBuf("\n broken node head: %p  %p  %p  0x%x, pre node head: %p  %p  %p  0x%x\n",
                      tmpNode->selfNode.freeNodeInfo.pstPrev, tmpNode->selfNode.freeNodeInfo.pstNext,
                      tmpNode->selfNode.preNode, tmpNode->selfNode.sizeAndFlag,
                      preNode->selfNode.freeNodeInfo.pstPrev, preNode->selfNode.freeNodeInfo.pstNext,
                      preNode->selfNode.preNode, preNode->selfNode.sizeAndFlag);
#endif
#ifdef LOSCFG_MEM_LEAKCHECK
    OsMemNodeBacktraceInfo(tmpNode, preNode);
#endif

    PRINTK("\n---------------------------------------------\n");
    PRINTK(" dump mem tmpNode:0x%x ~ 0x%x\n", tmpNode, ((UINTPTR)tmpNode + NODEDUMPSIZE));
    OsDumpMemByte(NODEDUMPSIZE, (UINTPTR)tmpNode);
    PRINTK("\n---------------------------------------------\n");
    if (preNode != tmpNode) {
        PRINTK(" dump mem :0x%x ~ tmpNode:0x%x\n", ((UINTPTR)tmpNode - NODEDUMPSIZE), tmpNode);
        OsDumpMemByte(NODEDUMPSIZE, ((UINTPTR)tmpNode - NODEDUMPSIZE));
        PRINTK("\n---------------------------------------------\n");
    }
}

STATIC VOID OsMemIntegrityCheckError(const LosMemDynNode *tmpNode,
                                     const LosMemDynNode *preNode,
                                     UINT32 intSave)
{
    LosTaskCB *taskCB = NULL;
    UINT32 taskID;

    OsMemNodeInfo(tmpNode, preNode);

    taskID = OS_MEM_TASKID_GET(preNode);
    if (taskID >= g_taskMaxNum) {
#ifdef LOSCFG_SHELL_EXCINFO
        WriteExcInfoToBuf("Task ID %u in pre node is invalid!\n", taskID);
#endif
        MEM_UNLOCK(intSave);
        LOS_Panic("Task ID %u in pre node is invalid!\n", taskID);
        return;
    }

    taskCB = OS_TCB_FROM_TID(taskID);
    if ((taskCB->taskStatus & OS_TASK_STATUS_UNUSED) ||
        (taskCB->taskEntry == NULL) || (taskCB->taskName == NULL)) {
#ifdef LOSCFG_SHELL_EXCINFO
        WriteExcInfoToBuf("\r\nTask ID %u in pre node is not created or deleted!\n", taskID);
#endif
        MEM_UNLOCK(intSave);
        LOS_Panic("\r\nTask ID %u in pre node is not created!\n", taskID);
        return;
    }
#ifdef LOSCFG_SHELL_EXCINFO
    WriteExcInfoToBuf("cur node: %p\npre node: %p\npre node was allocated by task:%s\n",
                      tmpNode, preNode, taskCB->taskName);
#endif
    MEM_UNLOCK(intSave);
    LOS_Panic("cur node: %p\npre node: %p\npre node was allocated by task:%s\n",
              tmpNode, preNode, taskCB->taskName);
    return;
}

/*
 * Description : memory pool integrity checking
 * Input       : pool --Pointer to memory pool
 * Return      : LOS_OK --memory pool integrate or LOS_NOK--memory pool impaired
 */
LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemIntegrityCheck(VOID *pool)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemDynNode *preNode = NULL;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);
    if (OsMemIntegrityCheck(pool, &tmpNode, &preNode)) {
        goto ERROR_OUT;
    }
    MEM_UNLOCK(intSave);
    return LOS_OK;

ERROR_OUT:
    OsMemIntegrityCheckError(tmpNode, preNode, intSave);
    return LOS_NOK;
}

STATIC INLINE VOID OsMemNodeDebugOperate(VOID *pool, LosMemDynNode *allocNode, UINT32 size)
{
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    poolInfo->poolCurUsedSize += OS_MEM_NODE_GET_SIZE(allocNode->selfNode.sizeAndFlag);
    if (poolInfo->poolCurUsedSize > poolInfo->poolWaterLine) {
        poolInfo->poolWaterLine = poolInfo->poolCurUsedSize;
    }
#endif

#ifdef LOSCFG_MEM_RECORDINFO
    allocNode->selfNode.originSize = size;
#endif

#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(allocNode);
#endif

#ifdef LOSCFG_MEM_LEAKCHECK
    OsMemLinkRegisterRecord(allocNode);
#endif
}

/*
 * Description : Allocate node from Memory pool
 * Input       : pool  --- Pointer to memory pool
 *               size  --- Size of memory in bytes to allocate
 * Return      : Pointer to allocated memory
 */
STATIC INLINE VOID *OsMemAllocWithCheck(VOID *pool, UINT32 size, UINT32 intSave)
{
    LosMemDynNode *allocNode = NULL;
    UINT32 allocSize;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
#ifdef LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK
    LosMemDynNode *tmpNode = NULL;
    LosMemDynNode *preNode = NULL;
#endif
    const VOID *firstNode = (const VOID *)((UINT8 *)OS_MEM_HEAD_ADDR(pool) + OS_DLNK_HEAD_SIZE);

    if (g_MALLOC_HOOK != NULL) {
        g_MALLOC_HOOK();
    }

#ifdef LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK
    if (OsMemIntegrityCheck(pool, &tmpNode, &preNode)) {
        OsMemIntegrityCheckError(tmpNode, preNode, intSave);
        return NULL;
    }
#endif

    allocSize = OS_MEM_ALIGN(size + OS_MEM_NODE_HEAD_SIZE, OS_MEM_ALIGN_SIZE);
    allocNode = OsMemFindSuitableFreeBlock(pool, allocSize);
    if (allocNode == NULL) {
        PRINT_ERR("---------------------------------------------------"
                  "--------------------------------------------------------\n");
        MEM_UNLOCK(intSave);
        OsMemInfoPrint(pool);
        MEM_LOCK(intSave);
        PRINT_ERR("[%s] No suitable free block, require free node size: 0x%x\n", __FUNCTION__, allocSize);
        PRINT_ERR("----------------------------------------------------"
                  "-------------------------------------------------------\n");
        return NULL;
    }
    if ((allocSize + OS_MEM_NODE_HEAD_SIZE + OS_MEM_ALIGN_SIZE) <= allocNode->selfNode.sizeAndFlag) {
        OsMemSplitNode(pool, allocNode, allocSize);
    }
    OsMemListDelete(&allocNode->selfNode.freeNodeInfo, firstNode);
    OsMemSetMagicNumAndTaskID(allocNode);
    OS_MEM_NODE_SET_USED_FLAG(allocNode->selfNode.sizeAndFlag);
    OS_MEM_ADD_USED(poolInfo->memStats, OS_MEM_NODE_GET_SIZE(allocNode->selfNode.sizeAndFlag),
                    OS_MEM_TASKID_GET(allocNode));
    OsMemNodeDebugOperate(pool, allocNode, size);
    return (allocNode + 1);
}

/*
 * Description : reAlloc a smaller memory node
 * Input       : pool      --- Pointer to memory pool
 *               allocSize --- the size of new node which will be alloced
 *               node      --- the node which wille be realloced
 *               nodeSize  --- the size of old node
 * Output      : node      --- pointer to the new node after realloc
 */
STATIC INLINE VOID OsMemReAllocSmaller(VOID *pool, UINT32 allocSize, LosMemDynNode *node, UINT32 nodeSize)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    if ((allocSize + OS_MEM_NODE_HEAD_SIZE + OS_MEM_ALIGN_SIZE) <= nodeSize) {
        node->selfNode.sizeAndFlag = nodeSize;
        OsMemSplitNode(pool, node, allocSize);
        OS_MEM_NODE_SET_USED_FLAG(node->selfNode.sizeAndFlag);
#ifdef LOSCFG_MEM_HEAD_BACKUP
        OsMemNodeSave(node);
#endif
        OS_MEM_REDUCE_USED(poolInfo->memStats, nodeSize - allocSize, OS_MEM_TASKID_GET(node));
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
        poolInfo->poolCurUsedSize -= nodeSize - allocSize;
#endif
    }
#ifdef LOSCFG_MEM_LEAKCHECK
    OsMemLinkRegisterRecord(node);
#endif
}

/*
 * Description : reAlloc a Bigger memory node after merge node and nextNode
 * Input       : pool      --- Pointer to memory pool
 *               allocSize --- the size of new node which will be alloced
 *               node      --- the node which wille be realloced
 *               nodeSize  --- the size of old node
 *               nextNode  --- pointer next node which will be merged
 * Output      : node      --- pointer to the new node after realloc
 */
STATIC INLINE VOID OsMemMergeNodeForReAllocBigger(VOID *pool, UINT32 allocSize, LosMemDynNode *node,
                                                  UINT32 nodeSize, LosMemDynNode *nextNode)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    const VOID *firstNode = (const VOID *)((UINT8 *)OS_MEM_HEAD_ADDR(pool) + OS_DLNK_HEAD_SIZE);

    node->selfNode.sizeAndFlag = nodeSize;
    OsMemListDelete(&nextNode->selfNode.freeNodeInfo, firstNode);
    OsMemMergeNode(nextNode);
    if ((allocSize + OS_MEM_NODE_HEAD_SIZE + OS_MEM_ALIGN_SIZE) <= node->selfNode.sizeAndFlag) {
        OsMemSplitNode(pool, node, allocSize);
    }
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    poolInfo->poolCurUsedSize += (node->selfNode.sizeAndFlag - nodeSize);
    if (poolInfo->poolCurUsedSize > poolInfo->poolWaterLine) {
        poolInfo->poolWaterLine = poolInfo->poolCurUsedSize;
    }
#endif
    OS_MEM_ADD_USED(poolInfo->memStats, node->selfNode.sizeAndFlag - nodeSize, OS_MEM_TASKID_GET(node));
    OS_MEM_NODE_SET_USED_FLAG(node->selfNode.sizeAndFlag);
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(node);
#endif
#ifdef LOSCFG_MEM_LEAKCHECK
    OsMemLinkRegisterRecord(node);
#endif
}

#ifdef LOSCFG_MEM_MUL_POOL
STATIC UINT32 OsMemPoolAdd(VOID *pool, UINT32 size)
{
    VOID *nextPool = g_poolHead;
    VOID *curPool = g_poolHead;
    UINTPTR poolEnd;
    while (nextPool != NULL) {
        poolEnd = (UINTPTR)nextPool + LOS_MemPoolSizeGet(nextPool);
        if (((pool <= nextPool) && (((UINTPTR)pool + size) > (UINTPTR)nextPool)) ||
            (((UINTPTR)pool < poolEnd) && (((UINTPTR)pool + size) >= poolEnd))) {
            PRINT_ERR("pool [%p, %p) conflict with pool [%p, %p)\n",
                      pool, (UINTPTR)pool + size,
                      nextPool, (UINTPTR)nextPool + LOS_MemPoolSizeGet(nextPool));
            return LOS_NOK;
        }
        curPool = nextPool;
        nextPool = ((LosMemPoolInfo *)nextPool)->nextPool;
    }

    if (g_poolHead == NULL) {
        g_poolHead = pool;
    } else {
        ((LosMemPoolInfo *)curPool)->nextPool = pool;
    }

    ((LosMemPoolInfo *)pool)->nextPool = NULL;
    return LOS_OK;
}
#endif

STATIC UINT32 OsMemInit(VOID *pool, UINT32 size)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    LosMemDynNode *newNode = NULL;
    LosMemDynNode *endNode = NULL;
    LOS_DL_LIST *listNodeHead = NULL;
#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
    UINT32 statSize;
#endif
    poolInfo->pool = pool;
    poolInfo->poolSize = size;
#if (LOSCFG_KERNEL_MEM_STATISTICS == YES)
    statSize = sizeof(TaskMemUsedInfo) * TASK_BLOCK_NUM;
    (VOID)memset_s(poolInfo->memStats, statSize, 0, statSize);
#endif
    OsDLnkInitMultiHead(OS_MEM_HEAD_ADDR(pool));
    newNode = OS_MEM_FIRST_NODE(pool);
    newNode->selfNode.sizeAndFlag = (size - ((UINTPTR)newNode - (UINTPTR)pool) - OS_MEM_NODE_HEAD_SIZE);
    newNode->selfNode.preNode = (LosMemDynNode *)OS_MEM_END_NODE(pool, size);
    listNodeHead = OS_MEM_HEAD(pool, newNode->selfNode.sizeAndFlag);
    if (listNodeHead == NULL) {
        return LOS_NOK;
    }

    LOS_ListTailInsert(listNodeHead, &(newNode->selfNode.freeNodeInfo));
    endNode = (LosMemDynNode *)OS_MEM_END_NODE(pool, size);
    (VOID)memset_s(endNode, sizeof(*endNode), 0, sizeof(*endNode));
    endNode->selfNode.preNode = newNode;
    endNode->selfNode.sizeAndFlag = OS_MEM_NODE_HEAD_SIZE;
    OS_MEM_NODE_SET_USED_FLAG(endNode->selfNode.sizeAndFlag);
    OsMemSetMagicNumAndTaskID(endNode);
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    poolInfo->poolCurUsedSize = sizeof(LosMemPoolInfo) + OS_MULTI_DLNK_HEAD_SIZE +
                                OS_MEM_NODE_GET_SIZE(endNode->selfNode.sizeAndFlag);
    poolInfo->poolWaterLine = poolInfo->poolCurUsedSize;
#endif
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(newNode);
    OsMemNodeSave(endNode);
#endif

    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemInit(VOID *pool, UINT32 size)
{
    UINT32 intSave;

    if ((pool == NULL) || (size < OS_MEM_MIN_POOL_SIZE)) {
        return OS_ERROR;
    }

    if (!IS_ALIGNED(size, OS_MEM_ALIGN_SIZE)) {
        PRINT_WARN("pool [%p, %p) size 0x%x sholud be aligned with OS_MEM_ALIGN_SIZE\n",
                   pool, (UINTPTR)pool + size, size);
        size = OS_MEM_ALIGN(size, OS_MEM_ALIGN_SIZE) - OS_MEM_ALIGN_SIZE;
    }

    MEM_LOCK(intSave);
#ifdef LOSCFG_MEM_MUL_POOL
    if (OsMemPoolAdd(pool, size)) {
        MEM_UNLOCK(intSave);
        return OS_ERROR;
    }
#endif

    if (OsMemInit(pool, size)) {
#ifdef LOSCFG_MEM_MUL_POOL
        (VOID)LOS_MemDeInit(pool);
#endif
        MEM_UNLOCK(intSave);
        return OS_ERROR;
    }

    MEM_UNLOCK(intSave);
    return LOS_OK;
}

#ifdef LOSCFG_MEM_MUL_POOL
LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemDeInit(VOID *pool)
{
    UINT32 intSave;
    UINT32 ret = LOS_NOK;
    VOID *nextPool = NULL;
    VOID *curPool = NULL;

    MEM_LOCK(intSave);
    do {
        if (pool == NULL) {
            break;
        }

        if (pool == g_poolHead) {
            g_poolHead = ((LosMemPoolInfo *)g_poolHead)->nextPool;
            ret = LOS_OK;
            break;
        }

        curPool = g_poolHead;
        nextPool = g_poolHead;
        while (nextPool != NULL) {
            if (pool == nextPool) {
                ((LosMemPoolInfo *)curPool)->nextPool = ((LosMemPoolInfo *)nextPool)->nextPool;
                ret = LOS_OK;
                break;
            }
            curPool = nextPool;
            nextPool = ((LosMemPoolInfo *)nextPool)->nextPool;
        }
    } while (0);

    MEM_UNLOCK(intSave);
    return ret;
}

LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemPoolList(VOID)
{
    VOID *nextPool = g_poolHead;
    UINT32 index = 0;
    while (nextPool != NULL) {
        PRINTK("pool%u :\n", index);
        index++;
        OsMemInfoPrint(nextPool);
        nextPool = ((LosMemPoolInfo *)nextPool)->nextPool;
    }
    return index;
}
#endif

LITE_OS_SEC_TEXT VOID *LOS_MemAlloc(VOID *pool, UINT32 size)
{
    VOID *ptr = NULL;
    UINT32 intSave;

    if ((pool == NULL) || (size == 0)) {
        return NULL;
    }

    MEM_LOCK(intSave);
    do {
        if (OS_MEM_NODE_GET_USED_FLAG(size) || OS_MEM_NODE_GET_ALIGNED_FLAG(size)) {
            break;
        }

        ptr = OsMemAllocWithCheck(pool, size, intSave);
    } while (0);
#ifdef LOSCFG_MEM_RECORDINFO
    OsMemRecordMalloc(ptr, size);
#endif
    MEM_UNLOCK(intSave);
    return ptr;
}

LITE_OS_SEC_TEXT VOID *LOS_MemAllocAlign(VOID *pool, UINT32 size, UINT32 boundary)
{
    UINT32 useSize;
    UINT32 gapSize;
    VOID *ptr = NULL;
    VOID *alignedPtr = NULL;
    LosMemDynNode *allocNode = NULL;
    UINT32 intSave;

    if ((pool == NULL) || (size == 0) || (boundary == 0) || !IS_POW_TWO(boundary) ||
        !IS_ALIGNED(boundary, sizeof(VOID *))) {
        return NULL;
    }

    MEM_LOCK(intSave);
    do {
        /*
         * sizeof(gapSize) bytes stores offset between alignedPtr and ptr,
         * the ptr has been OS_MEM_ALIGN_SIZE(4 or 8) aligned, so maximum
         * offset between alignedPtr and ptr is boundary - OS_MEM_ALIGN_SIZE
         */
        if ((boundary - sizeof(gapSize)) > ((UINT32)(-1) - size)) {
            break;
        }

        useSize = (size + boundary) - sizeof(gapSize);
        if (OS_MEM_NODE_GET_USED_FLAG(useSize) || OS_MEM_NODE_GET_ALIGNED_FLAG(useSize)) {
            break;
        }

        ptr = OsMemAllocWithCheck(pool, useSize, intSave);

        alignedPtr = (VOID *)OS_MEM_ALIGN(ptr, boundary);
        if (ptr == alignedPtr) {
            break;
        }

        /* store gapSize in address (ptr -4), it will be checked while free */
        gapSize = (UINT32)((UINTPTR)alignedPtr - (UINTPTR)ptr);
        allocNode = (LosMemDynNode *)ptr - 1;
        OS_MEM_NODE_SET_ALIGNED_FLAG(allocNode->selfNode.sizeAndFlag);
#ifdef LOSCFG_MEM_RECORDINFO
        allocNode->selfNode.originSize = size;
#endif
#ifdef LOSCFG_MEM_HEAD_BACKUP
        OsMemNodeSaveWithGapSize(allocNode, gapSize);
#endif

        OS_MEM_NODE_SET_ALIGNED_FLAG(gapSize);
        *(UINT32 *)((UINTPTR)alignedPtr - sizeof(gapSize)) = gapSize;

        ptr = alignedPtr;
    } while (0);
#ifdef LOSCFG_MEM_RECORDINFO
    OsMemRecordMalloc(ptr, size);
#endif
    MEM_UNLOCK(intSave);

    return ptr;
}

LITE_OS_SEC_TEXT STATIC INLINE UINT32 OsDoMemFree(VOID *pool, const VOID *ptr, LosMemDynNode *node)
{
    UINT32 ret = OsMemCheckUsedNode(pool, node);
    if (ret == LOS_OK) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(ptr, node->selfNode.originSize);
#endif
        OsMemFreeNode(node, pool);
    }
    return ret;
}

#ifdef LOSCFG_MEM_HEAD_BACKUP
LITE_OS_SEC_TEXT STATIC INLINE UINT32 OsMemBackupCheckAndRetore(VOID *pool, VOID *ptr, LosMemDynNode *node)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    LosMemDynNode *startNode = OS_MEM_FIRST_NODE(pool);
    LosMemDynNode *endNode   = OS_MEM_END_NODE(pool, poolInfo->poolSize);

    if (OS_MEM_MIDDLE_ADDR(startNode, node, endNode)) {
        /* GapSize is bad or node is broken, we need to verify & try to restore */
        if (!OsMemChecksumVerify(&(node->selfNode))) {
            node = (LosMemDynNode *)((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE);
            return OsMemBackupTryRestore(pool, &node, ptr);
        }
    }
    return LOS_OK;
}
#endif

LITE_OS_SEC_TEXT UINT32 LOS_MemFree(VOID *pool, VOID *ptr)
{
    UINT32 ret = LOS_NOK;
    UINT32 gapSize;
    UINT32 intSave;
    LosMemDynNode *node = NULL;

    if ((pool == NULL) || (ptr == NULL) ||
        !IS_ALIGNED(pool, sizeof(VOID *)) || !IS_ALIGNED(ptr, sizeof(VOID *))) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);
    do {
        gapSize = *(UINT32 *)((UINTPTR)ptr - sizeof(UINT32));
        if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize) && OS_MEM_NODE_GET_USED_FLAG(gapSize)) {
            PRINT_ERR("[%s:%d]gapSize:0x%x error\n", __FUNCTION__, __LINE__, gapSize);
            goto OUT;
        }

        node = (LosMemDynNode *)((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE);

        if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize)) {
            gapSize = OS_MEM_NODE_GET_ALIGNED_GAPSIZE(gapSize);
            if ((gapSize & (OS_MEM_ALIGN_SIZE - 1)) || (gapSize > ((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE))) {
                PRINT_ERR("illegal gapSize: 0x%x\n", gapSize);
                break;
            }
            node = (LosMemDynNode *)((UINTPTR)ptr - gapSize - OS_MEM_NODE_HEAD_SIZE);
        }
#ifndef LOSCFG_MEM_HEAD_BACKUP
        ret = OsDoMemFree(pool, ptr, node);
#endif
    } while (0);
#ifdef LOSCFG_MEM_HEAD_BACKUP
    ret = OsMemBackupCheckAndRetore(pool, ptr, node);
    if (!ret) {
        ret = OsDoMemFree(pool, ptr, node);
    }
#endif
OUT:
#ifdef LOSCFG_MEM_RECORDINFO
    if (ret == LOS_NOK) {
        OsMemRecordFree(ptr, 0);
    }
#endif
    MEM_UNLOCK(intSave);
    return ret;
}

STATIC VOID *OsGetRealPtr(const VOID *pool, VOID *ptr)
{
    VOID *realPtr = ptr;
    UINT32 gapSize = *((UINT32 *)((UINTPTR)ptr - sizeof(UINT32)));

    if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize) && OS_MEM_NODE_GET_USED_FLAG(gapSize)) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(ptr, 0);
#endif
        PRINT_ERR("[%s:%d]gapSize:0x%x error\n", __FUNCTION__, __LINE__, gapSize);
        return NULL;
    }
    if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize)) {
        gapSize = OS_MEM_NODE_GET_ALIGNED_GAPSIZE(gapSize);
        if ((gapSize & (OS_MEM_ALIGN_SIZE - 1)) ||
            (gapSize > ((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE - (UINTPTR)pool))) {
            PRINT_ERR("[%s:%d]gapSize:0x%x error\n", __FUNCTION__, __LINE__, gapSize);
#ifdef LOSCFG_MEM_RECORDINFO
            OsMemRecordFree(ptr, 0);
#endif
            return NULL;
        }
        realPtr = (VOID *)((UINTPTR)ptr - (UINTPTR)gapSize);
    }
    return realPtr;
}

#ifdef LOSCFG_MEM_RECORDINFO
STATIC INLINE VOID OsMemReallocNodeRecord(LosMemDynNode *node, UINT32 size, const VOID *ptr)
{
    node->selfNode.originSize = size;
#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(node);
#endif
    OsMemRecordMalloc(ptr, size);
}
#endif

STATIC VOID *OsMemRealloc(VOID *pool, const VOID *ptr, LosMemDynNode *node, UINT32 size, UINT32 intSave)
{
    LosMemDynNode *nextNode = NULL;
    UINT32 allocSize = OS_MEM_ALIGN(size + OS_MEM_NODE_HEAD_SIZE, OS_MEM_ALIGN_SIZE);
    UINT32 nodeSize = OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag);
    VOID *tmpPtr = NULL;
#ifdef LOSCFG_MEM_RECORDINFO
    const VOID *originPtr = ptr;
#endif

    if (nodeSize >= allocSize) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(originPtr, node->selfNode.originSize);
#endif
        OsMemReAllocSmaller(pool, allocSize, node, nodeSize);
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemReallocNodeRecord(node, size, ptr);
#endif
        return (VOID *)ptr;
    }

    nextNode = OS_MEM_NEXT_NODE(node);
    if (!OS_MEM_NODE_GET_USED_FLAG(nextNode->selfNode.sizeAndFlag) &&
        ((nextNode->selfNode.sizeAndFlag + nodeSize) >= allocSize)) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(originPtr, node->selfNode.originSize);
#endif
        OsMemMergeNodeForReAllocBigger(pool, allocSize, node, nodeSize, nextNode);
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemReallocNodeRecord(node, size, ptr);
#endif
        return (VOID *)ptr;
    }

    tmpPtr = OsMemAllocWithCheck(pool, size, intSave);
    if (tmpPtr != NULL) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordMalloc(tmpPtr, size);
#endif
        if (memcpy_s(tmpPtr, size, ptr, (nodeSize - OS_MEM_NODE_HEAD_SIZE)) != EOK) {
            MEM_UNLOCK(intSave);
            (VOID)LOS_MemFree((VOID *)pool, (VOID *)tmpPtr);
            MEM_LOCK(intSave);
            return NULL;
        }
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(originPtr, node->selfNode.originSize);
#endif
        OsMemFreeNode(node, pool);
    }
    return tmpPtr;
}

LITE_OS_SEC_TEXT_MINOR VOID *LOS_MemRealloc(VOID *pool, VOID *ptr, UINT32 size)
{
    UINT32 intSave;
    VOID *newPtr = NULL;
    LosMemDynNode *node = NULL;
#ifdef LOSCFG_MEM_RECORDINFO
    VOID *originPtr = ptr;
#endif

    if (OS_MEM_NODE_GET_USED_FLAG(size) || OS_MEM_NODE_GET_ALIGNED_FLAG(size) || (pool == NULL)) {
        return NULL;
    }

    if (ptr == NULL) {
        newPtr = LOS_MemAlloc(pool, size);
        goto OUT;
    }

    if (size == 0) {
        (VOID)LOS_MemFree(pool, ptr);
        goto OUT;
    }

    MEM_LOCK(intSave);

    ptr = OsGetRealPtr(pool, ptr);
    if (ptr == NULL) {
        goto OUT_UNLOCK;
    }

    node = (LosMemDynNode *)((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE);
    if (OsMemCheckUsedNode(pool, node) != LOS_OK) {
#ifdef LOSCFG_MEM_RECORDINFO
        OsMemRecordFree(originPtr, 0);
#endif
        goto OUT_UNLOCK;
    }

    newPtr = OsMemRealloc(pool, ptr, node, size, intSave);

OUT_UNLOCK:
    MEM_UNLOCK(intSave);
OUT:
    return newPtr;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemTotalUsedGet(VOID *pool)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 memUsed = 0;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode <= OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if (OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
            memUsed += OS_MEM_NODE_GET_SIZE(tmpNode->selfNode.sizeAndFlag);
        }
    }

    MEM_UNLOCK(intSave);

    return memUsed;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemUsedBlksGet(VOID *pool)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 blkNums = 0;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode <= OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if (OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
            blkNums++;
        }
    }

    MEM_UNLOCK(intSave);

    return blkNums;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemTaskIdGet(VOID *ptr)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)(VOID *)m_aucSysMem1;
    UINT32 intSave;
#ifdef LOSCFG_EXC_INTERACTION
    if (ptr < (VOID *)m_aucSysMem1) {
        poolInfo = (LosMemPoolInfo *)(VOID *)m_aucSysMem0;
    }
#endif
    if ((ptr == NULL) ||
        (ptr < (VOID *)OS_MEM_FIRST_NODE(poolInfo)) ||
        (ptr > (VOID *)OS_MEM_END_NODE(poolInfo, poolInfo->poolSize))) {
        PRINT_ERR("input ptr %p is out of system memory range[%p, %p]\n", ptr, OS_MEM_FIRST_NODE(poolInfo),
                  OS_MEM_END_NODE(poolInfo, poolInfo->poolSize));
        return OS_INVALID;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(poolInfo); tmpNode <= OS_MEM_END_NODE(poolInfo, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if ((UINTPTR)ptr < (UINTPTR)tmpNode) {
            if (OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.preNode->selfNode.sizeAndFlag)) {
                MEM_UNLOCK(intSave);
                return (UINT32)((UINTPTR)(tmpNode->selfNode.preNode->selfNode.freeNodeInfo.pstNext));
            } else {
                MEM_UNLOCK(intSave);
                PRINT_ERR("input ptr %p is belong to a free mem node\n", ptr);
                return OS_INVALID;
            }
        }
    }

    MEM_UNLOCK(intSave);
    return OS_INVALID;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemFreeBlksGet(VOID *pool)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 blkNums = 0;
    UINT32 intSave;

    if (pool == NULL) {
        return LOS_NOK;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode <= OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if (!OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
            blkNums++;
        }
    }

    MEM_UNLOCK(intSave);

    return blkNums;
}

LITE_OS_SEC_TEXT_MINOR UINTPTR LOS_MemLastUsedGet(VOID *pool)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    LosMemDynNode *node = NULL;

    if (pool == NULL) {
        return LOS_NOK;
    }

    node = OS_MEM_END_NODE(pool, poolInfo->poolSize)->selfNode.preNode;
    if (OS_MEM_NODE_GET_USED_FLAG(node->selfNode.sizeAndFlag)) {
        return (UINTPTR)((CHAR *)node + OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag) + sizeof(LosMemDynNode));
    } else {
        return (UINTPTR)((CHAR *)node + sizeof(LosMemDynNode));
    }
}

/*
 * Description : reset "end node"
 * Input       : pool    --- Pointer to memory pool
 *               preAddr --- Pointer to the pre Pointer of end node
 * Output      : endNode --- pointer to "end node"
 * Return      : the number of free node
 */
LITE_OS_SEC_TEXT_MINOR VOID OsMemResetEndNode(VOID *pool, UINTPTR preAddr)
{
    LosMemDynNode *endNode = (LosMemDynNode *)OS_MEM_END_NODE(pool, ((LosMemPoolInfo *)pool)->poolSize);
    endNode->selfNode.sizeAndFlag = OS_MEM_NODE_HEAD_SIZE;
    if (preAddr != 0) {
        endNode->selfNode.preNode = (LosMemDynNode *)(preAddr - sizeof(LosMemDynNode));
    }
    OS_MEM_NODE_SET_USED_FLAG(endNode->selfNode.sizeAndFlag);
    OsMemSetMagicNumAndTaskID(endNode);

#ifdef LOSCFG_MEM_HEAD_BACKUP
    OsMemNodeSave(endNode);
#endif
}

UINT32 LOS_MemPoolSizeGet(const VOID *pool)
{
    if (pool == NULL) {
        return LOS_NOK;
    }
    return ((LosMemPoolInfo *)pool)->poolSize;
}

LITE_OS_SEC_TEXT_MINOR VOID OsMemInfoPrint(VOID *pool)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    LOS_MEM_POOL_STATUS status = {0};

    if (LOS_MemInfoGet(pool, &status) == LOS_NOK) {
        return;
    }

#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    PRINTK("pool addr          pool size    used size     free size    "
           "max free node size   used node num     free node num      UsageWaterLine\n");
    PRINTK("---------------    --------     -------       --------     "
           "--------------       -------------      ------------      ------------\n");
    PRINTK("%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x    0x%-13x\n",
           poolInfo->pool, poolInfo->poolSize, status.uwTotalUsedSize,
           status.uwTotalFreeSize, status.uwMaxFreeNodeSize, status.uwUsedNodeNum,
           status.uwFreeNodeNum, status.uwUsageWaterLine);

#else
    PRINTK("pool addr          pool size    used size     free size    "
           "max free node size   used node num     free node num\n");
    PRINTK("---------------    --------     -------       --------     "
           "--------------       -------------      ------------\n");
    PRINTK("%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x\n",
           poolInfo->pool, poolInfo->poolSize, status.uwTotalUsedSize,
           status.uwTotalFreeSize, status.uwMaxFreeNodeSize, status.uwUsedNodeNum,
           status.uwFreeNodeNum);
#endif
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemInfoGet(VOID *pool, LOS_MEM_POOL_STATUS *poolStatus)
{
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    LosMemDynNode *tmpNode = NULL;
    UINT32 totalUsedSize = 0;
    UINT32 totalFreeSize = 0;
    UINT32 maxFreeNodeSize = 0;
    UINT32 usedNodeNum = 0;
    UINT32 freeNodeNum = 0;
    UINT32 intSave;

    if (poolStatus == NULL) {
        PRINT_ERR("can't use NULL addr to save info\n");
        return LOS_NOK;
    }

    if ((poolInfo == NULL) || ((UINTPTR)pool != (UINTPTR)poolInfo->pool)) {
        PRINT_ERR("wrong mem pool addr: %p, line:%d\n", poolInfo, __LINE__);
        return LOS_NOK;
    }

    tmpNode = (LosMemDynNode *)OS_MEM_END_NODE(pool, poolInfo->poolSize);
    tmpNode = (LosMemDynNode *)OS_MEM_ALIGN(tmpNode, OS_MEM_ALIGN_SIZE);

    if (!OS_MEM_MAGIC_VALID(tmpNode->selfNode.freeNodeInfo.pstPrev)) {
        PRINT_ERR("wrong mem pool addr: %p\n, line:%d", poolInfo, __LINE__);
        return LOS_NOK;
    }

    MEM_LOCK(intSave);

    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode <= OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if (!OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
            ++freeNodeNum;
            totalFreeSize += OS_MEM_NODE_GET_SIZE(tmpNode->selfNode.sizeAndFlag);
            if (maxFreeNodeSize < OS_MEM_NODE_GET_SIZE(tmpNode->selfNode.sizeAndFlag)) {
                maxFreeNodeSize = OS_MEM_NODE_GET_SIZE(tmpNode->selfNode.sizeAndFlag);
            }
        } else {
            ++usedNodeNum;
            totalUsedSize += OS_MEM_NODE_GET_SIZE(tmpNode->selfNode.sizeAndFlag);
        }
    }

    MEM_UNLOCK(intSave);

    poolStatus->uwTotalUsedSize = totalUsedSize;
    poolStatus->uwTotalFreeSize = totalFreeSize;
    poolStatus->uwMaxFreeNodeSize = maxFreeNodeSize;
    poolStatus->uwUsedNodeNum = usedNodeNum;
    poolStatus->uwFreeNodeNum = freeNodeNum;
#if defined(OS_MEM_WATERLINE) && (OS_MEM_WATERLINE == YES)
    poolStatus->uwUsageWaterLine = poolInfo->poolWaterLine;
#endif
    return LOS_OK;
}

STATIC INLINE VOID OsShowFreeNode(UINT32 index, UINT32 length, const UINT32 *countNum)
{
    UINT32 count = 0;
    PRINTK("\n    block size:  ");
    while (count < length) {
        PRINTK("2^%-5u", (index + OS_MIN_MULTI_DLNK_LOG2 + count));
        count++;
    }
    PRINTK("\n    node number: ");
    count = 0;
    while (count < length) {
        PRINTK("  %-5u", countNum[count + index]);
        count++;
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemFreeNodeShow(VOID *pool)
{
    LOS_DL_LIST *listNodeHead = NULL;
    LosMultipleDlinkHead *headAddr = (LosMultipleDlinkHead *)((UINTPTR)pool + sizeof(LosMemPoolInfo));
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 linkHeadIndex;
    UINT32 countNum[OS_MULTI_DLNK_NUM] = { 0 };
    UINT32 intSave;

    if ((poolInfo == NULL) || ((UINTPTR)pool != (UINTPTR)poolInfo->pool)) {
        PRINT_ERR("wrong mem pool addr: %p, line:%d\n", poolInfo, __LINE__);
        return LOS_NOK;
    }

    PRINTK("\n   ************************ left free node number**********************");
    MEM_LOCK(intSave);

    for (linkHeadIndex = 0; linkHeadIndex <= (OS_MULTI_DLNK_NUM - 1);
         linkHeadIndex++) {
        listNodeHead = headAddr->listHead[linkHeadIndex].pstNext;
        while (listNodeHead != &(headAddr->listHead[linkHeadIndex])) {
            listNodeHead = listNodeHead->pstNext;
            countNum[linkHeadIndex]++;
        }
    }

    linkHeadIndex = 0;
    while (linkHeadIndex < OS_MULTI_DLNK_NUM) {
        if (linkHeadIndex + COLUMN_NUM < OS_MULTI_DLNK_NUM) {
            OsShowFreeNode(linkHeadIndex, COLUMN_NUM, countNum);
            linkHeadIndex += COLUMN_NUM;
        } else {
            OsShowFreeNode(linkHeadIndex, (OS_MULTI_DLNK_NUM - 1 - linkHeadIndex), countNum);
            break;
        }
    }

    MEM_UNLOCK(intSave);
    PRINTK("\n   ********************************************************************\n\n");

    return LOS_OK;
}
#ifdef LOSCFG_MEM_LEAKCHECK
LITE_OS_SEC_TEXT_MINOR VOID OsMemUsedNodeShow(VOID *pool)
{
    LosMemDynNode *tmpNode = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT32 intSave;
    UINT32 count;

    if (pool == NULL) {
        PRINTK("input param is NULL\n");
        return;
    }
    if (LOS_MemIntegrityCheck(pool)) {
        PRINTK("LOS_MemIntegrityCheck error\n");
        return;
    }
    MEM_LOCK(intSave);
#ifdef __LP64__
    PRINTK("node                ");
#else
    PRINTK("node        ");
#endif
    for (count = 0; count < LOS_RECORD_LR_CNT; count++) {
#ifdef __LP64__
        PRINTK("        LR[%u]       ", count);
#else
        PRINTK("    LR[%u]   ", count);
#endif
    }
    PRINTK("\n");
    for (tmpNode = OS_MEM_FIRST_NODE(pool); tmpNode < OS_MEM_END_NODE(pool, poolInfo->poolSize);
         tmpNode = OS_MEM_NEXT_NODE(tmpNode)) {
        if (OS_MEM_NODE_GET_USED_FLAG(tmpNode->selfNode.sizeAndFlag)) {
#ifdef __LP64__
            PRINTK("%018p: ", tmpNode);
#else
            PRINTK("%010p: ", tmpNode);
#endif
            for (count = 0; count < LOS_RECORD_LR_CNT; count++) {
#ifdef __LP64__
                PRINTK(" %018p ", tmpNode->selfNode.linkReg[count]);
#else
                PRINTK(" %010p ", tmpNode->selfNode.linkReg[count]);
#endif
            }
            PRINTK("\n");
        }
    }
    MEM_UNLOCK(intSave);
    return;
}
#endif

#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemNodeSizeCheck(VOID *pool, VOID *ptr, UINT32 *totalSize, UINT32 *availSize)
{
    const VOID *head = NULL;
    LosMemPoolInfo *poolInfo = (LosMemPoolInfo *)pool;
    UINT8 *endPool = NULL;

    if (g_memCheckLevel == LOS_MEM_CHECK_LEVEL_DISABLE) {
        return LOS_ERRNO_MEMCHECK_DISABLED;
    }

    if ((pool == NULL) || (ptr == NULL) || (totalSize == NULL) || (availSize == NULL)) {
        return LOS_ERRNO_MEMCHECK_PARA_NULL;
    }

    endPool = (UINT8 *)pool + poolInfo->poolSize;
    if (!(OS_MEM_MIDDLE_ADDR_OPEN_END(pool, ptr, endPool))) {
        return LOS_ERRNO_MEMCHECK_OUTSIDE;
    }

    if (g_memCheckLevel == LOS_MEM_CHECK_LEVEL_HIGH) {
        head = OsMemFindNodeCtrl(pool, ptr);
        if ((head == NULL) ||
            (OS_MEM_NODE_GET_SIZE(((LosMemDynNode *)head)->selfNode.sizeAndFlag) < ((UINTPTR)ptr - (UINTPTR)head))) {
            return LOS_ERRNO_MEMCHECK_NO_HEAD;
        }
        *totalSize = OS_MEM_NODE_GET_SIZE(((LosMemDynNode *)head)->selfNode.sizeAndFlag - sizeof(LosMemDynNode));
        *availSize = OS_MEM_NODE_GET_SIZE(((LosMemDynNode *)head)->selfNode.sizeAndFlag - ((UINTPTR)ptr -
                                          (UINTPTR)head));
        return LOS_OK;
    }
    if (g_memCheckLevel == LOS_MEM_CHECK_LEVEL_LOW) {
        if (ptr != (VOID *)OS_MEM_ALIGN(ptr, OS_MEM_ALIGN_SIZE)) {
            return LOS_ERRNO_MEMCHECK_NO_HEAD;
        }
        head = (const VOID *)((UINTPTR)ptr - sizeof(LosMemDynNode));
        if (OS_MEM_MAGIC_VALID(((LosMemDynNode *)head)->selfNode.freeNodeInfo.pstPrev)) {
            *totalSize = OS_MEM_NODE_GET_SIZE(((LosMemDynNode *)head)->selfNode.sizeAndFlag - sizeof(LosMemDynNode));
            *availSize = OS_MEM_NODE_GET_SIZE(((LosMemDynNode *)head)->selfNode.sizeAndFlag - sizeof(LosMemDynNode));
            return LOS_OK;
        } else {
            return LOS_ERRNO_MEMCHECK_NO_HEAD;
        }
    }

    return LOS_ERRNO_MEMCHECK_WRONG_LEVEL;
}

/*
 * Description : get a pool's memCtrl
 * Input       : ptr -- point to source ptr
 * Return      : search forward for ptr's memCtrl or "NULL"
 * attention : this func couldn't ensure the return memCtrl belongs to ptr it just find forward the most nearly one
 */
LITE_OS_SEC_TEXT_MINOR const VOID *OsMemFindNodeCtrl(const VOID *pool, const VOID *ptr)
{
    const VOID *head = ptr;

    if (ptr == NULL) {
        return NULL;
    }

    head = (const VOID *)OS_MEM_ALIGN(head, OS_MEM_ALIGN_SIZE);
    while (!OS_MEM_MAGIC_VALID(((LosMemDynNode *)head)->selfNode.freeNodeInfo.pstPrev)) {
        head = (const VOID *)((UINT8 *)head - sizeof(CHAR *));
        if (head <= pool) {
            return NULL;
        }
    }
    return head;
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemCheckLevelSet(UINT8 checkLevel)
{
    if (checkLevel == LOS_MEM_CHECK_LEVEL_LOW) {
        PRINTK("%s: LOS_MEM_CHECK_LEVEL_LOW \n", __FUNCTION__);
    } else if (checkLevel == LOS_MEM_CHECK_LEVEL_HIGH) {
        PRINTK("%s: LOS_MEM_CHECK_LEVEL_HIGH \n", __FUNCTION__);
    } else if (checkLevel == LOS_MEM_CHECK_LEVEL_DISABLE) {
        PRINTK("%s: LOS_MEM_CHECK_LEVEL_DISABLE \n", __FUNCTION__);
    } else {
        PRINTK("%s: wrong param, setting failed !! \n", __FUNCTION__);
        return LOS_ERRNO_MEMCHECK_WRONG_LEVEL;
    }
    g_memCheckLevel = checkLevel;
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT8 LOS_MemCheckLevelGet(VOID)
{
    return g_memCheckLevel;
}


UINT32 OsMemSysNodeCheck(VOID *dstAddr, VOID *srcAddr, UINT32 nodeLength, UINT8 pos)
{
    UINT32 ret;
    UINT32 totalSize = 0;
    UINT32 availSize = 0;
    UINT8 *pool = m_aucSysMem1;
#ifdef LOSCFG_EXC_INTERACTION
    if ((UINTPTR)dstAddr < ((UINTPTR)m_aucSysMem0 + g_excInteractMemSize)) {
        pool = m_aucSysMem0;
    }
#endif
    if (pos == 0) { /* if this func was called by memset */
        ret = LOS_MemNodeSizeCheck(pool, dstAddr, &totalSize, &availSize);
        if ((ret == LOS_OK) && (nodeLength > availSize)) {
            PRINT_ERR("---------------------------------------------\n");
            PRINT_ERR("memset: dst inode availSize is not enough"
                      " availSize = 0x%x, memset length = 0x%x\n", availSize, nodeLength);
            OsBackTrace();
            PRINT_ERR("---------------------------------------------\n");
            return LOS_NOK;
        }
    } else if (pos == 1) { /* if this func was called by memcpy */
        ret = LOS_MemNodeSizeCheck(pool, dstAddr, &totalSize, &availSize);
        if ((ret == LOS_OK) && (nodeLength > availSize)) {
            PRINT_ERR("---------------------------------------------\n");
            PRINT_ERR("memcpy: dst inode availSize is not enough"
                      " availSize = 0x%x, memcpy length = 0x%x\n", availSize, nodeLength);
            OsBackTrace();
            PRINT_ERR("---------------------------------------------\n");
            return LOS_NOK;
        }
#ifdef LOSCFG_EXC_INTERACTION
        if ((UINTPTR)srcAddr < ((UINTPTR)m_aucSysMem0 + g_excInteractMemSize)) {
            pool = m_aucSysMem0;
        } else {
            pool = m_aucSysMem1;
        }
#endif
        ret = LOS_MemNodeSizeCheck(pool, srcAddr, &totalSize, &availSize);
        if ((ret == LOS_OK) && (nodeLength > availSize)) {
            PRINT_ERR("---------------------------------------------\n");
            PRINT_ERR("memcpy: src inode availSize is not enough"
                      " availSize = 0x%x, memcpy length = 0x%x\n",
                      availSize, nodeLength);
            OsBackTrace();
            PRINT_ERR("---------------------------------------------\n");
            return LOS_NOK;
        }
    }
    return LOS_OK;
}
#endif /* LOSCFG_BASE_MEM_NODE_SIZE_CHECK */

#ifdef LOSCFG_MEM_MUL_MODULE
STATIC INLINE UINT32 OsMemModCheck(UINT32 moduleID)
{
    if (moduleID > MEM_MODULE_MAX) {
        PRINT_ERR("error module ID input!\n");
        return LOS_NOK;
    }
    return LOS_OK;
}

STATIC INLINE VOID *OsMemPtrToNode(VOID *ptr)
{
    UINT32 gapSize;

    if ((UINTPTR)ptr & (OS_MEM_ALIGN_SIZE - 1)) {
        PRINT_ERR("[%s:%d]ptr:%p not align by 4byte\n", __FUNCTION__, __LINE__, ptr);
        return NULL;
    }

    gapSize = *((UINT32 *)((UINTPTR)ptr - sizeof(UINT32)));
    if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize) && OS_MEM_NODE_GET_USED_FLAG(gapSize)) {
        PRINT_ERR("[%s:%d]gapSize:0x%x error\n", __FUNCTION__, __LINE__, gapSize);
        return NULL;
    }
    if (OS_MEM_NODE_GET_ALIGNED_FLAG(gapSize)) {
        gapSize = OS_MEM_NODE_GET_ALIGNED_GAPSIZE(gapSize);
        if ((gapSize & (OS_MEM_ALIGN_SIZE - 1)) || (gapSize > ((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE))) {
            PRINT_ERR("[%s:%d]gapSize:0x%x error\n", __FUNCTION__, __LINE__, gapSize);
            return NULL;
        }

        ptr = (VOID *)((UINTPTR)ptr - gapSize);
    }

    return (VOID *)((UINTPTR)ptr - OS_MEM_NODE_HEAD_SIZE);
}

STATIC INLINE UINT32 OsMemNodeSizeGet(VOID *ptr)
{
    LosMemDynNode *node = (LosMemDynNode *)OsMemPtrToNode(ptr);
    if (node == NULL) {
        return 0;
    }

    return OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag);
}

VOID *LOS_MemMalloc(VOID *pool, UINT32 size, UINT32 moduleID)
{
    UINT32 intSave;
    VOID *ptr = NULL;
    VOID *node = NULL;
    if (OsMemModCheck(moduleID) == LOS_NOK) {
        return NULL;
    }
    ptr = LOS_MemAlloc(pool, size);
    if (ptr != NULL) {
        MEM_LOCK(intSave);
        g_moduleMemUsedSize[moduleID] += OsMemNodeSizeGet(ptr);
        node = OsMemPtrToNode(ptr);
        if (node != NULL) {
            OS_MEM_MODID_SET(node, moduleID);
        }
        MEM_UNLOCK(intSave);
    }
    return ptr;
}

VOID *LOS_MemMallocAlign(VOID *pool, UINT32 size, UINT32 boundary, UINT32 moduleID)
{
    UINT32 intSave;
    VOID *ptr = NULL;
    VOID *node = NULL;
    if (OsMemModCheck(moduleID) == LOS_NOK) {
        return NULL;
    }
    ptr = LOS_MemAllocAlign(pool, size, boundary);
    if (ptr != NULL) {
        MEM_LOCK(intSave);
        g_moduleMemUsedSize[moduleID] += OsMemNodeSizeGet(ptr);
        node = OsMemPtrToNode(ptr);
        if (node != NULL) {
            OS_MEM_MODID_SET(node, moduleID);
        }
        MEM_UNLOCK(intSave);
    }
    return ptr;
}

UINT32 LOS_MemMfree(VOID *pool, VOID *ptr, UINT32 moduleID)
{
    UINT32 intSave;
    UINT32 ret;
    UINT32 size;
    LosMemDynNode *node = NULL;

    if ((OsMemModCheck(moduleID) == LOS_NOK) || (ptr == NULL) || (pool == NULL)) {
        return LOS_NOK;
    }

    node = (LosMemDynNode *)OsMemPtrToNode(ptr);
    if (node == NULL) {
        return LOS_NOK;
    }

    size = OS_MEM_NODE_GET_SIZE(node->selfNode.sizeAndFlag);

    if (moduleID != OS_MEM_MODID_GET(node)) {
        PRINT_ERR("node[%p] alloced in module %lu, but free in module %u\n node's taskID: 0x%x\n",
                  ptr, OS_MEM_MODID_GET(node), moduleID, OS_MEM_TASKID_GET(node));
        moduleID = OS_MEM_MODID_GET(node);
    }

    ret = LOS_MemFree(pool, ptr);
    if (ret == LOS_OK) {
        MEM_LOCK(intSave);
        g_moduleMemUsedSize[moduleID] -= size;
        MEM_UNLOCK(intSave);
    }
    return ret;
}

VOID *LOS_MemMrealloc(VOID *pool, VOID *ptr, UINT32 size, UINT32 moduleID)
{
    VOID *newPtr = NULL;
    UINT32 oldNodeSize;
    UINT32 intSave;
    LosMemDynNode *node = NULL;
    UINT32 oldModuleID = moduleID;

    if ((OsMemModCheck(moduleID) == LOS_NOK) || (pool == NULL)) {
        return NULL;
    }

    if (ptr == NULL) {
        return LOS_MemMalloc(pool, size, moduleID);
    }

    node = (LosMemDynNode *)OsMemPtrToNode(ptr);
    if (node == NULL) {
        return NULL;
    }

    if (moduleID != OS_MEM_MODID_GET(node)) {
        PRINT_ERR("a node[%p] alloced in module %lu, but realloc in module %u\n node's taskID: %lu\n",
                  ptr, OS_MEM_MODID_GET(node), moduleID, OS_MEM_TASKID_GET(node));
        oldModuleID = OS_MEM_MODID_GET(node);
    }

    if (size == 0) {
        (VOID)LOS_MemMfree(pool, ptr, oldModuleID);
        return NULL;
    }

    oldNodeSize = OsMemNodeSizeGet(ptr);
    newPtr = LOS_MemRealloc(pool, ptr, size);
    if (newPtr != NULL) {
        MEM_LOCK(intSave);
        g_moduleMemUsedSize[moduleID] += OsMemNodeSizeGet(newPtr);
        g_moduleMemUsedSize[oldModuleID] -= oldNodeSize;
        node = (LosMemDynNode *)OsMemPtrToNode(newPtr);
        OS_MEM_MODID_SET(node, moduleID);
        MEM_UNLOCK(intSave);
    }
    return newPtr;
}

UINT32 LOS_MemMusedGet(UINT32 moduleID)
{
    if (OsMemModCheck(moduleID) == LOS_NOK) {
        return OS_NULL_INT;
    }
    return g_moduleMemUsedSize[moduleID];
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
