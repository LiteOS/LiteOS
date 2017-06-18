/**
  ******************************************************************************
  * @file    gd32f20x_dci.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   DCI functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_dci.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup DCI 
  * @brief DCI driver modules
  * @{
  */ 


/** @defgroup DCI_Private_Functions
  * @{
  */ 

/**
  * @brief  Reset the DCI registers..
  * @param  None
  * @retval None
  */
void DCI_DeInit(void)
{
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_DCIRST, ENABLE);
    RCC_AHB2PeriphReset_Enable(RCC_AHB2PERIPH_DCIRST, DISABLE);
}

/**
  * @brief  Initialize the DCI parameters .
  * @param  DCI_InitParaStruct: the struct DCI_InitPara pointer
  * @retval None
  */
void DCI_Init(DCI_InitPara* DCI_InitParaStruct)
{
    uint32_t temp = 0x0;


    /* Reset DCI CTLR register value */
    temp = DCI->CTLR;

    temp &= ~((uint32_t)DCI_CTLR_DCIEN | DCI_CTLR_CAP | DCI_CTLR_SNAP | DCI_CTLR_ESM | DCI_CTLR_CKPOL |
                      DCI_CTLR_HPOL | DCI_CTLR_VPOL | DCI_CTLR_FR_0 | 
                      DCI_CTLR_FR_1 | DCI_CTLR_DCIF_0 | DCI_CTLR_DCIF_1); 
                  
    /* Initialize DCI peripheral according to DCI_InitParaStruct */
    temp |= ((uint32_t)DCI_InitParaStruct->DCI_SnapshotMode |
                     DCI_InitParaStruct->DCI_SyncMode |
                     DCI_InitParaStruct->DCI_CKPolarity |
                     DCI_InitParaStruct->DCI_VPolarity |
                     DCI_InitParaStruct->DCI_HPolarity |
                     DCI_InitParaStruct->DCI_FrameRate |
                     DCI_InitParaStruct->DCI_InterfaceFormat);

    DCI->CTLR = temp;                              
}

/**
  * @brief  Initial the struct DCI_InitPara.
  * @param  DCI_InitParaStruct: the struct DCI_InitPara pointer
  * @retval None
  */
void DCI_ParaInit(DCI_InitPara* DCI_InitParaStruct)
{
    /* DCI_InitPara members default value */
    DCI_InitParaStruct->DCI_SnapshotMode = DCI_CAPMODE_CONTINUOUS;
    DCI_InitParaStruct->DCI_SyncMode = DCI_SYNCMODE_HARDWARE;
    DCI_InitParaStruct->DCI_CKPolarity = DCI_CKPOL_FALLING;
    DCI_InitParaStruct->DCI_VPolarity = DCI_VPOL_LOW;
    DCI_InitParaStruct->DCI_HPolarity = DCI_HPOL_LOW;
    DCI_InitParaStruct->DCI_FrameRate = DCI_FRATE_ALL_FRAME;
    DCI_InitParaStruct->DCI_InterfaceFormat = DCI_INTERFACEFORMAT_8B;
}

/**
  * @brief  Initialize the  window timing module of DCI peripheral .   
  * @param  DCI_WindowInitParaStruct:  the struct DCI_WindowInitPara pointer
  * @retval None
  * @note   This function should be called before to enable and start the DCI interface.
  */
void DCI_WindowFunctionConfig(DCI_WindowInitPara* DCI_WindowInitParaStruct)
{  
    /* Set Window Start Position */
    DCI->WSPR = (uint32_t)((uint32_t)DCI_WindowInitParaStruct->DCI_WindowHorStartPosition |
                  ((uint32_t)DCI_WindowInitParaStruct->DCI_WindowVerStartPosition << 16));

    /* Set window size */
    DCI->WSZR = (uint32_t)(DCI_WindowInitParaStruct->DCI_WindowHorSize |
                  ((uint32_t)DCI_WindowInitParaStruct->DCI_WindowVerSize << 16));
}

/**
  * @brief  Enable or disable the DCI Window Function.
  * @param  NewValue: new value of the DCI Window Function. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  * @note   This function should be called before to enable and start the DCI interface.
  */
void DCI_WindowFunction_Enable(TypeState NewValue)
{    
    if (NewValue != DISABLE)
    {
        /* Enable the DCI Window Function */
        DCI->CTLR |= (uint32_t)DCI_CTLR_WDEN;
    }
    else
    {
        /* Disable the DCI Window Function */
        DCI->CTLR &= ~(uint32_t)DCI_CTLR_WDEN;
    }
}

/**
  * @brief  Initializes Embedded sync detect module
  * @param  DCI_CodeInitPara: the struct DCI_CodeInitPara pointer
  * @retval None
  */
void DCI_EmbeddedSyncDetectConfig(DCI_CodeInitPara* DCI_CodeInitParaStruct)
{
    DCI->ECR = (uint32_t)(DCI_CodeInitParaStruct->DCI_FSCode |
                          ((uint32_t)DCI_CodeInitParaStruct->DCI_LSCode << 8)|
                          ((uint32_t)DCI_CodeInitParaStruct->DCI_LECode << 16)|
                          ((uint32_t)DCI_CodeInitParaStruct->DCI_FECode << 24));
}

/**
  * @brief  Enable or disable the DCI JPEG format. 
  * @param  NewValue: new value of the DCI JPEG format. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  * @note   The Crop and Embedded Synchronization features cannot be used in this mode. 
  */
void DCI_JPEGMode_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable the DCI JPEG format */
        DCI->CTLR |= (uint32_t)DCI_CTLR_JM;
    }
    else
    {
        /* Disable the DCI JPEG format */
        DCI->CTLR &= ~(uint32_t)DCI_CTLR_JM;
    }
}
  
/**
  * @brief  Enable or disable the DCI interface.
  * @param  NewValue: new value of the DCI interface. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DCI_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Set DCIEN bit,enable the DCI */
        DCI->CTLR |= (uint32_t)DCI_CTLR_DCIEN;
    }
    else
    {
        /* Reset DCIEN bit,disable the DCI */
        DCI->CTLR &= ~(uint32_t)DCI_CTLR_DCIEN;
    }
}

/**
  * @brief  Enable or disable the DCI Capture.
  * @param  NewValue: new value of the DCI capture. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DCI_CAP_Enable(TypeState NewValue)
{    
    if (NewValue != DISABLE)
    {
        /* Enable the DCI Capture */
        DCI->CTLR |= (uint32_t)DCI_CTLR_CAP;
    }
    else
    {
        /* Disable the DCI Capture */
        DCI->CTLR &= ~(uint32_t)DCI_CTLR_CAP;
    }
}

/**
  * @brief  Get the data stored in Pixel Buffer.
  * @param  None 
  * @retval Pixel Buffer value
  */
uint32_t DCI_GetPixel(void)
{
    return DCI->PBR;
}

/**
  * @brief  Enable or disable the DCI interface interrupts.
  * @param  DCI_INT: the I2C interrupt sources. 
  *   This parameter can be any combination of the following values:
  *     @arg DCI_INT_ESEM: Embedded Synchronous Error Interrupt Mask
  *     @arg DCI_INT_VSM: Vsync Interrupt Mask
  *     @arg DCI_INT_OVRM: FIFO Overrun Interrupt Mask
  *     @arg DCI_INT_EFM: End of Frame Interrupt Mask
  *     @arg DCI_INT_ELM: End of Line Interrupt Mask
  * @param  NewValue: new value of the specified DCI interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DCI_INTConfig(uint16_t DCI_INT, TypeState NewValue)
{

    if (NewValue != DISABLE)
    {
        /* Enable the Interrupt sources */
        DCI->IMR |= DCI_INT;
    }
    else
    {
        /* Disable the Interrupt sources */
        DCI->IMR &= (uint16_t)(~DCI_INT);
    }  
}

/**
  * @brief  Get the bit flag of DCI interface.
  * @param  DCI_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg DCI_FLAG_HS: HS line status
  *     @arg DCI_FLAG_VS: VS line status
  *     @arg DCI_FLAG_FV: FIFO Valid
  *     @arg DCI_FLAG_ESEF: Embedded Synchronous Error Flag
  *     @arg DCI_FLAG_VSF: Vsync Flag
  *     @arg DCI_FLAG_OVRF: FIFO Overrun Flag
  *     @arg DCI_FLAG_EFF: End of Frame Flag
  *     @arg DCI_FLAG_ELF: End of Line Flag
  *     @arg DCI_FLAG_ESEMF: Masked Embedded Synchronous Error Flag
  *     @arg DCI_FLAG_VSMF: Masked Vsync Flag
  *     @arg DCI_FLAG_OVRMF: Masked FIFO Overrun Flag
  *     @arg DCI_FLAG_EFMF: Masked End of Frame Flag
  *     @arg DCI_FLAG_ELMF: Masked End of Line Flag
  * @retval The new state of DCI_FLAG (SET or RESET).
  */
TypeState DCI_GetBitState(uint16_t DCI_FLAG)
{
    uint32_t   dciregindex=0;
    uint32_t  dcireg = 0;

    /* Get the DCI register index */
    dcireg = (((uint16_t)DCI_FLAG) >> 12);

    if (dciregindex == 0x00) 
    {
        /* The FLAG is in FIR register */
        dcireg= DCI->FIR;
    }
    else if (dciregindex == 0x02) 
    {
        /* The FLAG is in STR register */
        dcireg = DCI->STR;
    }
    else 
    {
        /* The FLAG is in MFR register */
        dcireg = DCI->MFR;
    }

    if ((dcireg & DCI_FLAG) != (uint16_t)RESET )
    {
        return  SET;
    }
    else
    {
        return  RESET;
    }
}

/**
  * @brief  Clear the bit flag of DCI flags.
  * @param  DCI_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg DCI_FLAG_ESEF: Embedded Synchronous Error Flag
  *     @arg DCI_FLAG_VSF: Vsync Flag
  *     @arg DCI_FLAG_OVRF: FIFO Overrun Flag
  *     @arg DCI_FLAG_EFF: End of Frame Flag
  *     @arg DCI_FLAG_ELF: End of Line Flag
  * @retval None
  */
void DCI_ClearBitState(uint16_t DCI_FLAG)
{
    /* Clear the selected DCI flag */ 
    DCI->CFR = DCI_FLAG;
}

/**
  * @brief  Get the interrupt bit flag.
  * @param  DCI_INT: the DCI interrupt source to get.
  *   This parameter can be one of the following values:
  *     @arg DCI_INT_ESEM: Embedded Synchronous Error Interrupt Mask
  *     @arg DCI_INT_VSM: Vsync Interrupt Mask
  *     @arg DCI_INT_OVRM: FIFO Overrun Interrupt Mask
  *     @arg DCI_INT_EFM: End of Frame Interrupt Mask
  *     @arg DCI_INT_ELM: End of Line Interrupt Mask
  * @retval The new state of DCI_INT (SET or RESET).
  */
TypeState DCI_GetIntBitState(uint16_t DCI_INT)
{

  uint32_t enablestatus = 0;
 
  enablestatus = DCI->MFR & DCI_INT; 
  
  if ((enablestatus != (uint16_t)RESET))
  {
    return SET;
  }
  else
  {
    return RESET;
  }

}

/**
  * @brief  Clear the interrupt bit flag.
  * @param  DCI_IT: the DCI interrupt source to clear.
  *   This parameter can be any combination of the following values:
  *     @arg DCI_INT_ESEM: Embedded Synchronous Error Interrupt Mask
  *     @arg DCI_INT_VSM: Vsync Interrupt Mask
  *     @arg DCI_INT_OVRM: FIFO Overrun Interrupt Mask
  *     @arg DCI_INT_EFM: End of Frame Interrupt Mask
  *     @arg DCI_INT_ELM: End of Line Interrupt Mask
  * @retval None
  */
void DCI_ClearIntBitState(uint16_t DCI_INT)
{
  /* Clear the selected I2C flag */  
  DCI->CFR = DCI_INT;
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
