/**
  ******************************************************************************
  * @file    gd32f20x_hash.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   HAU functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_hash.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup HAU 
  * @brief HAU driver modules
  * @{
  */ 

/* Private functions ---------------------------------------------------------*/ 

/** @defgroup HAU_Private_Functions
  * @{
  */ 
  
/**
  * @brief  Reset the HAU peripheral.
  * @param  None
  * @retval None
  */
void HAU_DeInit(void)
{
    /* Enable HAU reset state */
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_HAURST, ENABLE);
    /* Release HAU from reset state */
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_HAURST, DISABLE);
}

/**
  * @brief  Initialize the HAU peripheral parameters.
  * @param  HAU_InitParaStruct: the HAU_InitPara structure pointer.
  * @retval None
  */
void HAU_Init(HAU_InitPara* HAU_InitParaStruct)
{
    /* Configure the Algorithm used, algorithm mode and the datatype */
    HAU->CTLR &= ~ (HAU_CTLR_ALGS | HAU_CTLR_DATAS | HAU_CTLR_HMS);
    HAU->CTLR |= (HAU_InitParaStruct->HAU_AlgoSelection | \
                  HAU_InitParaStruct->HAU_DataType | \
                  HAU_InitParaStruct->HAU_AlgoMode);
  
    /* when algorithm mode is HMAC, set the Key */  
    if(HAU_InitParaStruct->HAU_AlgoMode == HAU_ALGOMODE_HMAC) 
    {
        HAU->CTLR &= ~HAU_CTLR_LKS;
        HAU->CTLR |= HAU_InitParaStruct->HAU_HMACKeyType;
    }

    /* Reset the HAU processor core */
    HAU->CTLR |= HAU_CTLR_START;  
}

/**
  * @brief  Initial the sturct HAU_InitPara.
  * @param  HAU_InitParaStruct : the sturct HAU_InitPara pointer. 
  * @retval None
  */
void HAU_ParaInit(HAU_InitPara* HAU_InitParaStruct)
{
    /* Initialize the HAU_AlgoSelection member */
    HAU_InitParaStruct->HAU_AlgoSelection = HAU_ALGOSELECTION_SHA1;

    /* Initialize the HAU_AlgoMode member */
    HAU_InitParaStruct->HAU_AlgoMode = HAU_ALGOMODE_HASH;

    /* Initialize the HAU_DataType member */
    HAU_InitParaStruct->HAU_DataType = HAU_DATAS_32BIT;

    /* Initialize the HAU_HMACKeyType member */
    HAU_InitParaStruct->HAU_HMACKeyType = HAU_HMACKEYTYPE_SHORTKEY;
}

/**
  * @brief  Reset the HAU processor core
  * @param  None
  * @retval None
  */
void HAU_Reset(void)
{
    /* Reset the HAU processor core */
    HAU->CTLR |= HAU_CTLR_START;
}

/**
  * @brief  Configure the Number of valid bits in last word of the message
  * @param  ValidNumber: Number of valid bits in last word of the message.
  *   This parameter must be a number between 0 and 0x1F.
  *     - 0x00: All 32 bits of the last data written are valid
  *     - 0x01: Only bit [0] of the last data written is valid
  *     - 0x02: Only bits[1:0] of the last data written are valid
  *     - 0x03: Only bits[2:0] of the last data written are valid
  *     - ...
  *     - 0x1F: Only bits[30:0] of the last data written are valid      
  * @retval None
  */
void HAU_SetLastWordValidBitsNbr(uint16_t ValidNumber)
{  
    /* Configure the Number of valid bits in last word of the message */
    HAU->STR &= ~(HAU_STR_NOB);
    HAU->STR |= ValidNumber;
}

/**
  * @brief  Write data to the Input FIFO
  * @param  Data: new data of the message to be processed.
  * @retval None
  */
void HAU_PutData(uint32_t Data)
{
    /* Write in the DI register a new data */
    HAU->DI = Data;
}

/**
  * @brief  Return the number of words already Writed into the IN FIFO.
  * @param  None
  * @retval The value of words already Writed into the IN FIFO.
  */
uint8_t HAU_GetInFIFOWordsNbr(void)
{   
    /* Return the value of NOW bits */
    return ((HAU->CTLR & HAU_CTLR_NOW) >> 8);
}

/**
  * @brief  Get the message digest result.
  * @param  HAU_MessageDigest: the HAU_MsgDigest structure pointer
  * @retval None
  */
void HAU_GetDigest(HAU_MsgDigest* HAU_MessageDigest)
{
    /* Get the data field */
    HAU_MessageDigest->Data[0] = HAU->DOR[0];
    HAU_MessageDigest->Data[1] = HAU->DOR[1];
    HAU_MessageDigest->Data[2] = HAU->DOR[2];
    HAU_MessageDigest->Data[3] = HAU->DOR[3];
    HAU_MessageDigest->Data[4] = HAU->DOR[4];
    HAU_MessageDigest->Data[5] = HAU_DIGEST->DOR[5];
    HAU_MessageDigest->Data[6] = HAU_DIGEST->DOR[6];
    HAU_MessageDigest->Data[7] = HAU_DIGEST->DOR[7];
}

/**
  * @brief  Start the message padding and calculation message     
  * @param  None
  * @retval None
  */
void HAU_StartDigest(void)
{
    /* Start the Digest calculation */
    HAU->STR |= HAU_STR_SCAL;
}

/**
  * @brief  Enable or disable auto-start message padding and
  *         calculation message digest at the end of DMA transfer.
  * @param  NewValue: new value of the selected HAU DMA transfer.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void HAU_AutoStartDigest(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the auto start of the final message digest at the end of DMA transfer */
        HAU->CTLR &= ~HAU_CTLR_MDS;
    }
    else
    {
        /* Disable the auto start of the final message digest at the end of DMA transfer */
        HAU->CTLR |= HAU_CTLR_MDS;
    }
}
  
/**
  * @brief  Enable or disable the HAU DMA interface.
  * @param  NewValue: new value of the HAU DMA transfer.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void HAU_DMA_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the HAU DMA request */
        HAU->CTLR |= HAU_CTLR_DMAE;
    }
    else
    {
        /* Disable the HAU DMA request */
        HAU->CTLR &= ~HAU_CTLR_DMAE;
    }
}
  
/**
  * @brief  Enable or disable the HAU interrupts.
  * @param  HAU_INT: the HAU interrupt sources
  *   This parameter can be any combination of the following values:
  *     @arg HAU_INT_DIIE: Data Input interrupt
  *     @arg HAU_INT_CCIE: Digest Calculation Completion Interrupt
  * @param  NewValue: new value of the specified HAU interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void HAU_INTConfig(uint32_t HAU_INT, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected HAU interrupt */
        HAU->IER |= HAU_INT;
    }
    else
    {
        /* Disable the selected HAU interrupt */
        HAU->IER &= (uint32_t)(~HAU_INT);
    }
}

/**
  * @brief  Get the bit flag of IFR registers.
  * @param  HAU_FLAG: the flag of IFR registers.
  *   This parameter can be one of the following values:
  *     @arg HAU_FLAG_DIIF: Data input interrupt status flag
  *     @arg HAU_FLAG_CCIF: Digest calculation completion interrupt status flag
  *     @arg HAU_FLAG_BSY: Busy flag
  *     @arg HAU_FLAG_DMAF: DMAS Status flag
  *     @arg HAU_FLAG_DINE: Data Input register (DI) not empty status flag
  * @retval The new value of HAU_FLAG (SET or RESET)
  */
TypeState HAU_GetBitState(uint32_t HAU_FLAG)
{
    uint32_t RegValue = 0;

    /* check if the FLAG is in CTLR register */
    if ((HAU_FLAG & HAU_FLAG_DINE) != (uint32_t)RESET ) 
    {
        RegValue = HAU->CTLR;
    }
    else /* The FLAG is in IFR register */
    {
        RegValue = HAU->IFR;
    }

    /* Check the status of the specified HAU flag */
    if ((RegValue & HAU_FLAG) != (uint32_t)RESET)
    {
        /* HAU is set */
        return SET;
    }
    else
    {
        /* HAU_FLAG is reset */
        return RESET;
    }
}

/**
  * @brief  Clear the bit flag of IFR register.
  * @param  HAU_FLAG: the flag of IFR registers.
  *   This parameter can be any combination of the following values:
  *     @arg HAU_FLAG_DIIF: Data Input Flag
  *     @arg HAU_FLAG_CCIF: Digest Calculation Completion Flag                       
  * @retval None
  */
void HAU_ClearBitState(uint32_t HAU_FLAG)
{
    /* Clear the selected HAU flags */
    HAU->STR = ~(uint32_t)HAU_FLAG;
}

/**
  * @brief  Get the interrupt bit flag.
  * @param  HAU_INT: the interrupt bit flag.
  *   This parameter can be one of the following values:
  *     @arg HAU_INT_DIIE: Data Input interrupt
  *     @arg HAU_INT_CCIE: Digest Calculation Completion Interrupt
  * @retval The new value of HAU_INT (SET or RESET).
  */
TypeState HAU_GetIntBitState(uint32_t HAU_INT)
{
    uint32_t RegValue = 0;

    /* Check the status of the specified HAU interrupt */
    RegValue =  HAU->STR;

    if (((HAU->IER & RegValue) & HAU_INT) != RESET)
    {
        /* HAU_INT is set */
        return SET;
    }
    else
    {
        /* HAU_INT is reset */
        return RESET;
    }
}

/**
  * @brief  Clear the interrupt bit flag.
  * @param  HAU_INT: the interrupt bit flag.
  *   This parameter can be any combination of the following values:
  *     @arg HAU_INT_DIIE: Data Input interrupt
  *     @arg HAU_INT_CCIE: Digest Calculation Completion Interrupt
  * @retval None
  */
void HAU_ClearIntBitState(uint32_t HAU_INT)
{
    /* Clear the selected HAU interrupt pending bit */
    HAU->STR = (uint32_t)(~HAU_INT);
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
