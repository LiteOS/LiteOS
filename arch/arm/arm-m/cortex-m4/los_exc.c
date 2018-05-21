/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
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


#include "los_exc.inc"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#if (LOSCFG_PLATFORM_EXC == YES)

VOID *m_puwExcContent;
UINT32 g_uwCurNestCount = 0;
EXC_INFO_S m_stExcInfo;
UINT8 g_uwExcTbl[32] = {
                        0, 0, 0, 0, 0, 0, OS_EXC_UF_DIVBYZERO, OS_EXC_UF_UNALIGNED,
                        0, 0, 0, 0, OS_EXC_UF_NOCP, OS_EXC_UF_INVPC, OS_EXC_UF_INVSTATE, OS_EXC_UF_UNDEFINSTR,
                        0, 0, 0, OS_EXC_BF_STKERR, OS_EXC_BF_UNSTKERR, OS_EXC_BF_IMPRECISERR, OS_EXC_BF_PRECISERR, OS_EXC_BF_IBUSERR,
                        0, 0, 0, OS_EXC_MF_MSTKERR, OS_EXC_MF_MUNSTKERR, 0, OS_EXC_MF_DACCVIOL, OS_EXC_MF_IACCVIOL
                       };
UINT32 g_uwArraySize = 0;
EXC_INFO_ARRAY_S m_stExcArray[OS_EXC_TYPE_MAX - 1];


 LITE_OS_SEC_TEXT_INIT VOID osExcInfoDisplay(EXC_INFO_S *pstExc)
{

    printf("Phase      = 0x%x\n", pstExc->usPhase);
    printf("Type       = 0x%x\n", pstExc->usType);
    printf("FaultAddr  = 0x%x\n", pstExc->uwFaultAddr );
    printf("ThrdPid    = 0x%x\n", pstExc->uwThrdPid );
    printf("R0         = 0x%x\n", pstExc->pstContext->uwR0);
    printf("R1         = 0x%x\n", pstExc->pstContext->uwR1);
    printf("R2         = 0x%x\n", pstExc->pstContext->uwR2);
    printf("R3         = 0x%x\n", pstExc->pstContext->uwR3);
    printf("R4         = 0x%x\n", pstExc->pstContext->uwR4);
    printf("R5         = 0x%x\n", pstExc->pstContext->uwR5);
    printf("R6         = 0x%x\n", pstExc->pstContext->uwR6);
    printf("R7         = 0x%x\n", pstExc->pstContext->uwR7);
    printf("R8         = 0x%x\n", pstExc->pstContext->uwR8);
    printf("R9         = 0x%x\n", pstExc->pstContext->uwR9);
    printf("R10        = 0x%x\n", pstExc->pstContext->uwR10);
    printf("R11        = 0x%x\n", pstExc->pstContext->uwR11);
    printf("R12        = 0x%x\n", pstExc->pstContext->uwR12);
    printf("PriMask    = 0x%x\n", pstExc->pstContext->uwPriMask);
    printf("SP         = 0x%x\n", pstExc->pstContext->uwSP);
    printf("LR         = 0x%x\n", pstExc->pstContext->uwLR);
    printf("PC         = 0x%x\n", pstExc->pstContext->uwPC);
    printf("xPSR       = 0x%x\n", pstExc->pstContext->uwxPSR);

    return;
}


/*****************************************************************************
 Function    : osExcHandleEntry
 Description : EXC模块的处理分发函数
 Input       : puwExcBufAddr --- EXC寄存器信息的首地址
 Output      : None
 Return      : None
 *****************************************************************************/
#if (LOSCFG_PLATFORM_HWI == YES)
extern UINT32 g_vuwIntCount;
#endif
LITE_OS_SEC_TEXT_INIT VOID osExcHandleEntry(UINT32 uwExcType, UINT32 uwFaultAddr, UINT32 uwPid, EXC_CONTEXT_S *  pstExcBufAddr)
{
    UINT16 usTmpFlag = (uwExcType >> 16) & 0xFFFF; // 为2在中断中，为1 uwFaultAddr有效
    g_uwCurNestCount++;
#if (LOSCFG_PLATFORM_HWI == YES)
    g_vuwIntCount++;
#endif
    m_stExcInfo.usNestCnt = g_uwCurNestCount;

    m_stExcInfo.usType = uwExcType & 0xFFFF;
    m_puwExcContent = (UINT32*)m_aucTaskArray;

    if (usTmpFlag & OS_EXC_FLAG_FAULTADDR_VALID)
    {
        m_stExcInfo.uwFaultAddr = uwFaultAddr;
    }
    else
    {
        m_stExcInfo.uwFaultAddr = OS_EXC_IMPRECISE_ACCESS_ADDR;
    }

    if (NULL !=  g_stLosTask.pstRunTask)
    {
        if (usTmpFlag & OS_EXC_FLAG_IN_HWI)
        {
            m_stExcInfo.usPhase = OS_EXC_IN_HWI;
            m_stExcInfo.uwThrdPid = uwPid;
        }
        else
        {
            m_stExcInfo.usPhase = OS_EXC_IN_TASK;
            m_stExcInfo.uwThrdPid = g_stLosTask.pstRunTask->uwTaskID;
        }
    }
    else
    {
        m_stExcInfo.usPhase = OS_EXC_IN_INIT;
        m_stExcInfo.uwThrdPid = 0xffffffff;
    }

    if(uwExcType & OS_EXC_FLAG_NO_FLOAT)
    {
        m_stExcInfo.pstContext = (EXC_CONTEXT_S*)((char*)pstExcBufAddr-64);
    }
    else
    {
        m_stExcInfo.pstContext = pstExcBufAddr;
    }

    osExcSave2DDR();

    osExcInfoDisplay(&m_stExcInfo);

    LOS_Reboot();
}
static VOID osExcSaveIntStatus()
{

    *((UINT32*)m_puwExcContent)  = OS_EXC_TYPE_NVIC;
     m_puwExcContent = (UINT8*)m_puwExcContent + 4;
    *((UINT32*)m_puwExcContent)= 0x164; // = OS_NVIC_INT_ENABLE_SIZE + OS_NVIC_INT_PEND_SIZE + OS_NVIC_INT_ACT_SIZE + OS_NVIC_INT_PRI_SIZE + 12 + 4 + 4
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    /* 保存中断ENABLE寄存器组 */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_SETENA_BASE, OS_NVIC_INT_ENABLE_SIZE);
    m_puwExcContent =(UINT8*)m_puwExcContent + OS_NVIC_INT_ENABLE_SIZE;

    /* 保存中断PEND寄存器组 */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_SETPEND_BASE, OS_NVIC_INT_PEND_SIZE);
    m_puwExcContent = (UINT8*)m_puwExcContent + OS_NVIC_INT_PEND_SIZE;

    /* 保存中断ACTIVE寄存器组 */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_INT_ACT_BASE, OS_NVIC_INT_ACT_SIZE);
    m_puwExcContent = (UINT8*)m_puwExcContent + OS_NVIC_INT_ACT_SIZE;

    /* 保存中断优先级寄存器组 */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_PRI_BASE, OS_NVIC_INT_PRI_SIZE);
    m_puwExcContent = (UINT8*)m_puwExcContent + OS_NVIC_INT_PRI_SIZE;

    /* 	系统异常优先级寄存器组 */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_EXCPRI_BASE, 12);
    m_puwExcContent = (UINT8*)m_puwExcContent + 12;

    /* 保存系统Handler控制及状态寄存器SHCSR */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_SHCSR, 4);
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    /* 保存中断控制及状态寄存器ICSR */
    memcpy(m_puwExcContent, (const void *)OS_NVIC_INT_CTRL, 4);
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    return;
}

VOID osExcRegister(EXC_INFO_TYPE uwType, EXC_INFO_SAVE_CALLBACK pFunc, VOID* pArg)
{
    EXC_INFO_ARRAY_S* pstExcInfo;
    if (uwType == 0 || uwType >= OS_EXC_TYPE_MAX || pFunc == NULL )
    {
        PRINT_ERR("osExcRegister ERROR!\n");
        return;
    }
    pstExcInfo = &(m_stExcArray[uwType - 1]);
    pstExcInfo->uwType       = uwType;
    pstExcInfo->pFnExcInfoCb = pFunc;
    pstExcInfo->pArg         = pArg;
    pstExcInfo->uwValid      = TRUE;
}

VOID osExcSaveSysInfo(EXC_INFO_TYPE uwType, EXC_INFO_SAVE_CALLBACK pFunc, UINT32 uwLoop, UINT32 uwLen, UINT32 uwIdx)
{
    UINT32 uwRet;
    UINT32 uwBuffer[OS_EXC_MAX_BUF_LEN];

    *((UINT32*)m_puwExcContent) = uwType;
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;
    *((UINT32*)m_puwExcContent)  = uwLen * (uwLoop - uwIdx);
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    for (; uwIdx < uwLoop; uwIdx++)
    {
        memset((void *)uwBuffer, 0, sizeof(UINT32) * OS_EXC_MAX_BUF_LEN);
        uwRet = pFunc(uwIdx, (VOID*)uwBuffer);
        if (LOS_OK == uwRet)
        {
            memcpy(m_puwExcContent, (VOID*)uwBuffer, uwLen);
            m_puwExcContent =(UINT8*)m_puwExcContent + uwLen;
        }
        else
        {
            m_puwExcContent =(UINT8*)m_puwExcContent + uwLen;
        }
    }
}

static VOID osExcSaveInfo(EXC_INFO_TYPE uwType, EXC_INFO_SAVE_CALLBACK pFunc, VOID* pArg)
{
    UINT32 uwLen;
    UINT32 uwIdx ;
    UINT32 uwLoop;
    UINT32 uwTaskSwitchCount = 0;
    OS_TASK_SWITCH_INFO *pstTaskSwitchInfo;

    switch(uwType)
    {
        case OS_EXC_TYPE_TSK: /*save task info*/
            uwLen = sizeof(TSK_INFO_S);
            uwLoop = *(UINT32 *)pArg;
            uwIdx = 0;
            break;

        case OS_EXC_TYPE_QUE: /*save queue info*/
            uwLen = sizeof(QUEUE_INFO_S);
            uwLoop = *(UINT32 *)pArg + 1;
            uwIdx = 1;
            break;

        case OS_EXC_TYPE_NVIC:
            (VOID)pFunc(0, 0);
            goto END;

        case OS_EXC_TYPE_TSK_SWITCH: /*save task switch info*/
            //not necessary, just for macro int library
            pstTaskSwitchInfo = pArg;
            uwTaskSwitchCount = pstTaskSwitchInfo->ucIsFull & 0x7F;
            uwLen = sizeof(UINT32) + sizeof(CHAR) * LOS_TASK_NAMELEN;
            if (pstTaskSwitchInfo->ucIsFull & 0x80)
            {
                uwIdx = pstTaskSwitchInfo->ucIdx;
                uwLoop = uwIdx + uwTaskSwitchCount;
            }
            else
            {
                uwIdx = 0;
                uwLoop = pstTaskSwitchInfo->ucIdx;
            }
            break;

        case OS_EXC_TYPE_MEM: /*save mem info*/
            uwLen = sizeof(MEM_INFO_S);
            uwLoop = *(UINT32 *)pArg;
            uwIdx = 0;
            break;

        default:
            goto END;
    }
    osExcSaveSysInfo(uwType, (EXC_INFO_SAVE_CALLBACK)pFunc,uwLoop, uwLen, uwIdx);
END:
    return;
}

static VOID osExcSave2DDR(VOID)
{
    UINT32 uwIdx = 0;

    memset((void *)m_aucTaskArray, 0xff, g_uwArraySize);

    /*define core type*/
    *((UINT32*)m_puwExcContent) = 2; // 1 is cortex-M3, 2 is cortex-M4
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    /* save exception info*/
    *((UINT32*)m_puwExcContent) = OS_EXC_TYPE_CONTEXT;
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    *((UINT32*)m_puwExcContent) = sizeof(EXC_INFO_S) - 4 + sizeof(EXC_CONTEXT_S);
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;

    memcpy((VOID*)m_puwExcContent, (VOID*)&m_stExcInfo, sizeof(EXC_INFO_S) - 4);
    m_puwExcContent = (UINT8*)m_puwExcContent + sizeof(EXC_INFO_S) - 4;

    memcpy((VOID*)m_puwExcContent, m_stExcInfo.pstContext, sizeof(EXC_CONTEXT_S));
    m_puwExcContent = (UINT8*)m_puwExcContent + sizeof(EXC_CONTEXT_S);

    for (uwIdx = 0; uwIdx < OS_EXC_TYPE_MAX - 1; uwIdx++)
    {
        if (m_stExcArray[uwIdx].uwValid == FALSE)
        {
            continue;
        }
        osExcSaveInfo(m_stExcArray[uwIdx].uwType, m_stExcArray[uwIdx].pFnExcInfoCb, m_stExcArray[uwIdx].pArg);
    }
    *((UINT32*)m_puwExcContent) = OS_EXC_TYPE_MAX;
    m_puwExcContent = (UINT8*)m_puwExcContent + 4;
    return;
}

LITE_OS_SEC_TEXT_INIT VOID osExcInit(UINT32 uwArraySize)
{
#if (LOSCFG_PLATFORM_HWI == YES)
    m_pstHwiForm[NonMaskableInt_IRQn + OS_SYS_VECTOR_CNT] = osExcNMI;

    //m_pstHwiForm[HardFault_IRQn + OS_SYS_VECTOR_CNT] = osExcHardFault;
    m_pstHwiForm[MemoryManagement_IRQn + OS_SYS_VECTOR_CNT] = osExcMemFault;
    m_pstHwiForm[BusFault_IRQn + OS_SYS_VECTOR_CNT] = osExcBusFault;
    m_pstHwiForm[UsageFault_IRQn + OS_SYS_VECTOR_CNT] = osExcUsageFault;
    m_pstHwiForm[SVCall_IRQn + OS_SYS_VECTOR_CNT] = osExcSvcCall;
#endif
    /* Enable USGFAULT, BUSFAULT, MEMFAULT */
    *(volatile UINT32 *)OS_NVIC_SHCSR |= 0x70000;
    /* Enable DIV 0 and unaligned exception */  //因为文件系统存在非对齐操作，故此异常暂不接管

    *(volatile UINT32 *)OS_NVIC_CCR |= 0x10;
    g_uwArraySize = uwArraySize;

    osExcRegister((EXC_INFO_TYPE)OS_EXC_TYPE_NVIC, (EXC_INFO_SAVE_CALLBACK)osExcSaveIntStatus, NULL);
}

#if (LOSCFG_PLATFORM_HWI == NO)
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    osExcNMI();
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    osExcHardFault();
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    osExcBusFault();
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    osExcUsageFault();
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
    osExcSvcCall();
}
#endif

#endif /*(LOSCFG_PLATFORM_EXC == YES)*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

