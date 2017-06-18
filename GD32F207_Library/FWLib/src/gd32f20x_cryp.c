/**
  ******************************************************************************
  * @file    gd32f20x_cryp.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   CAU functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_cryp.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup CAU 
  * @brief CAU driver modules
  * @{
  */ 

/* Private define ------------------------------------------------------------*/
#define FLAG_MASK     ((uint8_t)0x20)
#define MAX_TIMEOUT   ((uint16_t)0xFFFF)

/* Private functions ---------------------------------------------------------*/

/** @defgroup CAU_Private_Functions
  * @{
  */ 

/**
  * @brief  Reset the CAU peripheral.
  * @param  None
  * @retval None
  */
void CAU_DeInit(void)
{
    /* Enable CAU reset state */
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_CAURST, ENABLE);

    /* Release CAU from reset state */
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_CAURST, DISABLE);
}

/**
  * @brief  Initialize the CAU peripheral parameters.
  * @param  CAU_InitParaStruct: the sturct CAU_InitPara pointer.
  * @retval None
  */
void CAU_Init(CAU_InitPara* CAU_InitParaStruct)
{
    /* Select Algorithm mode */  
    CAU->CTLR &= ~CAU_CTLR_ALGS;
    CAU->CTLR |= CAU_InitParaStruct->CAU_AlgoMode;
    
    /* Select dataType */ 
    CAU->CTLR &= ~CAU_CTLR_DATAS;
    CAU->CTLR |= CAU_InitParaStruct->CAU_DataType;

    /* Select Key size if used AES algorithm) */
    if ((CAU_InitParaStruct->CAU_AlgoMode != CAU_CTLR_ALGS_TDES_ECB) &&
        (CAU_InitParaStruct->CAU_AlgoMode != CAU_CTLR_ALGS_TDES_CBC) &&
        (CAU_InitParaStruct->CAU_AlgoMode != CAU_CTLR_ALGS_DES_ECB) &&
        (CAU_InitParaStruct->CAU_AlgoMode != CAU_CTLR_ALGS_DES_CBC))
    {
        CAU->CTLR &= ~CAU_CTLR_KEYLEN;
        CAU->CTLR |= CAU_InitParaStruct->CAU_KeySize; 
    }

    /* Select data Direction */ 
    CAU->CTLR &= ~CAU_CTLR_ALGDIR;
    CAU->CTLR |= CAU_InitParaStruct->CAU_AlgoDir;
}

/**
  * @brief  Initial the sturct CAU_InitPara.
  * @param  CAU_InitParaStruct: the sturct CAU_InitPara pointer.
  * @retval None
  */
void CAU_ParaInit(CAU_InitPara* CAU_InitParaStruct)
{
    CAU_InitParaStruct->CAU_AlgoDir = CAU_ALGODIR_ENCRYPT;
    CAU_InitParaStruct->CAU_AlgoMode = CAU_ALGOMODE_TDES_ECB;
    CAU_InitParaStruct->CAU_DataType = CAU_DATAS_32BIT;
    CAU_InitParaStruct->CAU_KeySize = CAU_KEYSIZE_128BIT;
}

/**
  * @brief  Initial the key parameters.
  * @param  CAU_KeyInitParaStruct: the sturct CAU_KeyInitParaStruct pointer.
  * @retval None
  */
void CAU_KeyInit(CAU_KeyInitPara* CAU_KeyInitParaStruct)
{
    /* Key Initialisation */
    CAU->K0HR = CAU_KeyInitParaStruct->CAU_Key0High;
    CAU->K0LR = CAU_KeyInitParaStruct->CAU_Key0Low;
    CAU->K1HR = CAU_KeyInitParaStruct->CAU_Key1High;
    CAU->K1LR = CAU_KeyInitParaStruct->CAU_Key1Low;
    CAU->K2HR = CAU_KeyInitParaStruct->CAU_Key2High;
    CAU->K2LR = CAU_KeyInitParaStruct->CAU_Key2Low;
    CAU->K3HR = CAU_KeyInitParaStruct->CAU_Key3High;
    CAU->K3LR = CAU_KeyInitParaStruct->CAU_Key3Low;
}

/**
  * @brief  Initial the sturct CAU_KeyInitPara.
  * @param  CAU_KeyInitParaStruct: the sturct CAU_KeyInitParaStruct pointer.
  * @retval None
  */
void CAU_KeyStructInit(CAU_KeyInitPara* CAU_KeyInitParaStruct)
{
    CAU_KeyInitParaStruct->CAU_Key0High  = 0;
    CAU_KeyInitParaStruct->CAU_Key0Low = 0;
    CAU_KeyInitParaStruct->CAU_Key1High  = 0;
    CAU_KeyInitParaStruct->CAU_Key1Low = 0;
    CAU_KeyInitParaStruct->CAU_Key2High  = 0;
    CAU_KeyInitParaStruct->CAU_Key2Low = 0;
    CAU_KeyInitParaStruct->CAU_Key3High  = 0;
    CAU_KeyInitParaStruct->CAU_Key3Low = 0;
}

/**
  * @brief  Initial the CAU Initialization Vectors(IV) parameters.
  * @param  CAU_IVInitParaStruct: the sturct CAU_IVInitParaStruct pointer.
  * @retval None
  */
void CAU_IVInit(CAU_IVInitPara* CAU_IVInitParaStruct)
{
    CAU->IV0HR = CAU_IVInitParaStruct->CAU_IV0High;
    CAU->IV0LR = CAU_IVInitParaStruct->CAU_IV0Low;
    CAU->IV1HR = CAU_IVInitParaStruct->CAU_IV1High;
    CAU->IV1LR = CAU_IVInitParaStruct->CAU_IV1Low;
}

/**
  * @brief  Initial the sturct CAU_IVInitPara.
  * @param  CAU_IVInitParaStruct: the sturct CAU_IVInitParaStruct pointer.
  * @retval None
  */
void CAU_IVStructInit(CAU_IVInitPara* CAU_IVInitParaStruct)
{
    CAU_IVInitParaStruct->CAU_IV0High  = 0;
    CAU_IVInitParaStruct->CAU_IV0Low = 0;
    CAU_IVInitParaStruct->CAU_IV1High  = 0;
    CAU_IVInitParaStruct->CAU_IV1Low = 0;
}

/**
  * @brief  Flush the IN and OUT FIFOs
  * @param  None
  * @retval None
  */
void CAU_FIFOFlush(void)
{
    /* Reset the read and write pointers of the FIFOs */
    CAU->CTLR |= CAU_CTLR_FLUSH;
}

/**
  * @brief  Enable or disable the CAU peripheral.
  * @param  NewValue: new value of the CAU peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAU_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the CAU processor */
        CAU->CTLR |= CAU_CTLR_CAUEN;
    }
    else
    {
        /* Disable the CAU processor */
        CAU->CTLR &= ~CAU_CTLR_CAUEN;
    }
}  

/**
  * @brief  Write data to the IN FIFO.
  * @param  Data: data to write in Data Input register
  * @retval None
  */
void CAU_PutData(uint32_t Data)
{
    CAU->DI = Data;
}

/**
  * @brief  Return the last data entered into the output FIFO.
  * @param  None
  * @retval Last data entered into the output FIFO.
  */
uint32_t CAU_GetData(void)
{
    return CAU->DO;
}

/**
  * @brief  Enable or disable the CAU DMA interface.
  * @param  CAU_DMAReq: specifies the CAU DMA transfer request to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg CAU_DMAREQ_DATAOUT: DMA for outgoing(Tx) data transfer
  *     @arg CAU_DMAREQ_DATAIN: DMA for incoming(Rx) data transfer
  * @param  NewValue: new value of the selected CAU DMA transfer request.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAU_DMA_Enable(uint8_t CAU_DMAReq, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected CAU DMA request */
        CAU->DCTLR |= CAU_DMAReq;
    }
    else
    {
        /* Disable the selected CAU DMA request */
        CAU->DCTLR &= (uint8_t)~CAU_DMAReq;
    }
}

/**
  * @brief  Enable or disable the CAU interrupts.
  * @param  CAU_INT: specifies the CAU interrupt source to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg CAU_INT_INIE: Input FIFO interrupt
  *     @arg CAU_INT_OUTIE: Output FIFO interrupt
  * @param  NewValue: new value of the specified CAU interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAU_INTConfig(uint8_t CAU_INT, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the selected CAU interrupt */
        CAU->IER |= CAU_INT;
    }
    else
    {
        /* Disable the selected CAU interrupt */
        CAU->IER &= (uint8_t)~CAU_INT;
    }
}

/**
  * @brief  Get the interrupt bit flag..     
  * @param  CAU_INT: the interrupt bit flag.
  *   This parameter can be one of the following values:
  *     @arg CAU_INT_INIE: Input FIFO interrupt
  *     @arg CAU_INT_OUTIE: Output FIFO interrupt
  * @retval The new value of CAU_INT (SET or RESET).
  */
TypeState CAU_GetIntBitState(uint8_t CAU_INT)
{
    /* Check the status of the specified CAU interrupt */
    if ((CAU->EIFR &  CAU_INT) != (uint8_t)RESET)
    {
        /* CAU_INT is set */
        return SET;
    }
    else
    {
        /* CAU_INT is reset */
        return RESET;
    }
}

/**
  * @brief  Return whether CAU peripheral is enabled or disabled.
  * @param  none.
  * @retval Current value of the CAU peripheral (ENABLE or DISABLE).
  */
TypeState CAU_GetEnableState(void)
{
    if ((CAU->CTLR & CAU_CTLR_CAUEN) != 0)
    {
        /* CAUEN bit is set */
        return ENABLE;
    }
    else
    {
        /* CAUEN bit is reset */
        return DISABLE;
    }
}

/**
  * @brief  Get the bit flag of CAU state.
  * @param  CAU_FLAG: the flag of CAU state.
  *   This parameter can be one of the following values:
  *     @arg CAU_FLAG_IFEF: Input FIFO Empty flag.
  *     @arg CAU_FLAG_IFNFF: Input FIFO Not Full flag.
  *     @arg CAU_FLAG_OFNEF: Output FIFO Not Empty flag.
  *     @arg CAU_FLAG_OFFF: Output FIFO Full flag.
  *     @arg CAU_FLAG_BSY: Busy flag.
  *     @arg CAU_FLAG_OUTIF: Output FIFO raw interrupt flag.
  *     @arg CAU_FLAG_INIF: Input FIFO raw interrupt flag.
  * @retval The new value of CAU_FLAG (SET or RESET).
  */
TypeState CAU_GetBitState(uint8_t CAU_FLAG)
{
    uint32_t RegValue = 0;

    /* check if the FLAG is in RISR register */
    if ((CAU_FLAG & FLAG_MASK) != 0x00) 
    {
        RegValue = CAU->IFR;
    }
    else  /* The FLAG is in SFR register */
    {
        RegValue = CAU->SFR;
    }

    /* Check the status of the specified CAU flag */
    if ((RegValue & CAU_FLAG ) != (uint8_t)RESET)
    {
        /* CAU_FLAG is set */
        return SET;
    }
    else
    {
        /* CAU_FLAG is reset */
        return RESET;
    }
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
