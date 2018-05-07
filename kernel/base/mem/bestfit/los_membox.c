/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2017>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include <string.h>
#include "los_hwi.h"
#include "los_typedef.h"
#include "los_membox.h"
#include "los_memory.h"

#ifdef LOS_MEMBOX_CHECK
LITE_OS_SEC_BSS UINT8 g_aucMemMang[MEM_INFO_SIZE];
#endif

/*--------------------------- LOS_InitBox -------------------------------------*/
LITE_OS_SEC_TEXT_INIT UINT32 LOS_MemboxInit (VOID *pBoxMem, UINT32 uwBoxSize, UINT32 uwBlkSize)
{
    UINTPTR uvIntSave;
#ifdef LOS_MEMBOX_CHECK
    UINT32 *puwMemCount = (UINT32 *)g_aucMemMang;
    MEM_INFO *pstMemInfo = (MEM_INFO *)(g_aucMemMang + sizeof(UINT32));
    UINT8 ucLoop;
#endif

    if (pBoxMem == NULL)
    {
        return OS_ERROR;
    }

    /* Initialize memory block system, returns 0 if OK, 1 if fails. */
    if (uwBlkSize == 0)
    {
        return OS_ERROR;
    }

#ifdef LOS_MEMBOX_CHECK
    uvIntSave = LOS_IntLock();
    for (ucLoop = 0; ucLoop < *puwMemCount; ucLoop++)
    {
        if (pstMemInfo->uwStartAddr == (UINT32)pBoxMem)
        {
            (*puwMemCount)--;
            break;
        }
        pstMemInfo++;
    }
    if(*puwMemCount < OS_SYS_MEM_NUM)
    {
        pstMemInfo->uwType = MEM_MANG_MEMBOX;
        pstMemInfo->uwStartAddr = (UINT32)pBoxMem;
        pstMemInfo->uwSize = uwBoxSize;
        (*puwMemCount)++;
    }
    LOS_IntRestore(uvIntSave);
#endif

    /* Create a Memory structure. */
    uvIntSave = LOS_IntLock();//PRIMASK_DISABLE_IRQ();
    ((OS_MEMBOX_S_P) pBoxMem)->uwMaxBlk = uwBoxSize/uwBlkSize;
    ((OS_MEMBOX_S_P) pBoxMem)->uwBlkSize = uwBlkSize;
    ((OS_MEMBOX_S_P) pBoxMem)->uwBlkCnt = 0;
    (VOID)LOS_IntRestore(uvIntSave);//PRIMASK_ENABLE_IRQ();

    return LOS_OK;
}

/*--------------------------- LOS_AllocBox ----------------------------------*/

LITE_OS_SEC_TEXT VOID *LOS_MemboxAlloc (VOID *pBoxMem)
{
    VOID *pRet = NULL;
    UINTPTR uvIntSave;

    uvIntSave = LOS_IntLock();//PRIMASK_DISABLE_IRQ();
    if (((OS_MEMBOX_S_P) pBoxMem)->uwBlkCnt < ((OS_MEMBOX_S_P) pBoxMem)->uwMaxBlk)
    {
        pRet = LOS_MemAlloc(m_aucSysMem0, ((OS_MEMBOX_S_P) pBoxMem)->uwBlkSize);
        if (pRet)
        {
            ((OS_MEMBOX_S_P) pBoxMem)->uwBlkCnt ++;
        }
        else
        {
            PRINT_ERR("LOS_AllocMem return fail!\n");
        }
    }
    (VOID)LOS_IntRestore(uvIntSave);//PRIMASK_ENABLE_IRQ();
    return pRet;
}


/*--------------------------- LOS_FreeBox ----------------------------------*/
LITE_OS_SEC_TEXT UINT32 LOS_MemboxFree (VOID *pBoxMem, VOID *pBox)
{
    INT32 swFreeRes;
    UINTPTR uvIntSave;

    swFreeRes = LOS_MemFree(m_aucSysMem0, pBox);
    if (LOS_OK == swFreeRes)
    {
        uvIntSave = LOS_IntLock();//PRIMASK_DISABLE_IRQ();
        if (((OS_MEMBOX_S_P) pBoxMem)->uwBlkCnt)
            ((OS_MEMBOX_S_P) pBoxMem)->uwBlkCnt --;
        (VOID)LOS_IntRestore(uvIntSave);//PRIMASK_ENABLE_IRQ();
    }
    return (UINT32)swFreeRes;
}

/*--------------------------- LOS_ClrBox ----------------------------------*/
LITE_OS_SEC_TEXT_MINOR VOID LOS_MemboxClr (VOID *pBoxMem, VOID *pBox)
{
    memset(pBox, 0, ((OS_MEMBOX_S_P) pBoxMem)->uwBlkSize);
}

LITE_OS_SEC_TEXT_MINOR UINT32 LOS_MemboxStatisticsGet(VOID *pBoxMem, UINT32 *puwMaxBlk, UINT32 *puwBlkCnt, UINT32 *puwBlkSize)
{
    if ((NULL == pBoxMem) || (NULL == puwMaxBlk) || (NULL == puwBlkCnt) || (NULL ==
    puwBlkSize))
    {
        return LOS_NOK;
    }

    *puwMaxBlk = ((OS_MEMBOX_S_P)pBoxMem)->uwMaxBlk;
    *puwBlkCnt = ((OS_MEMBOX_S_P)pBoxMem)->uwBlkCnt;;
    *puwBlkSize = ((OS_MEMBOX_S_P)pBoxMem)->uwBlkSize;

    return LOS_OK;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
