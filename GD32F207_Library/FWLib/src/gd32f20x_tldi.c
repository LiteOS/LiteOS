/**
  ******************************************************************************
  * @file    gd32f20x_tldi.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   TLDI functions of the firmware library..
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_tldi.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup TLDI 
  * @brief TLDI driver modules
  * @{
  */
  
/** @defgroup TLDI_Private_Defines
  * @{
  */

#define CTLR_MASK                     ((uint32_t)0x0FFE888F)  /* TLDI GCR Mask */

/**
  * @}
  */
/** @defgroup TLDI_Private_Functions
  * @{
  */

/**
  * @brief  Reset the TLDI peripheral.
  *         values.
  * @param  None
  * @retval None
  */
void TLDI_DeInit(void)
{
    /* Enable TLDI reset state */
    RCC_APB2Periph2Reset_Enable(RCC_APB2PERIPH2_TLDI, ENABLE);

    /* Release TLDI from reset state */
    RCC_APB2Periph2Reset_Enable(RCC_APB2PERIPH2_TLDI, DISABLE);
}

/**
  * @brief  Initialize the TLDI interface parameters..
  * @note   This function can be used in case that TLDI is disabled.
  * @param  TLDI_Para: pointer to a TLDI_InitPara structure .
  * @retval None
  */
void TLDI_Init(TLDI_InitPara* TLDI_InitParaStruct)
{
    /* Configue Synchronization pluse size */
    TLDI->PSCR &= ~(TLDI_PSCR_VPS  | TLDI_PSCR_HPS);
    TLDI->PSCR |= TLDI_InitParaStruct->TLDI_HSPulseSize<< 16 | 
                      TLDI_InitParaStruct->TLDI_VSPulseSize;

    /* Configue Back porch size */
    TLDI->BPSCR &= ~(TLDI_BPSCR_VBPS  | TLDI_BPSCR_HBPS);
    TLDI->BPSCR |= (TLDI_InitParaStruct->TLDI_HBackPorchSize<< 16 | TLDI_InitParaStruct->TLDI_VBackPorchSize);

    /* Configue Active size */
    TLDI->ASCR &= ~(TLDI_ASCR_VAS| TLDI_ASCR_HAS );
    TLDI->ASCR |= (TLDI_InitParaStruct->TLDI_HActiveSize<< 16 | TLDI_InitParaStruct->TLDI_VActiveSize);

    /* Configue Total size */
    TLDI->TSCR &= ~(TLDI_TSCR_VTS | TLDI_TSCR_HTS );
    TLDI->TSCR |= (TLDI_InitParaStruct->TLDI_HTotalSize<< 16 | TLDI_InitParaStruct->TLDI_VTotalSize);

    TLDI->CTLR &= (uint32_t)CTLR_MASK;
    TLDI->CTLR |=  (uint32_t)(TLDI_InitParaStruct->TLDI_VSPolaritySelect | TLDI_InitParaStruct->TLDI_HSPolaritySelect | \
                           TLDI_InitParaStruct->TLDI_DEPolaritySelect | TLDI_InitParaStruct->TLDI_CLKPolaritySelect);

    /* Configue the background color value */
    TLDI->BVCR &= ~(TLDI_BVCR_BVB  | TLDI_BVCR_BVG| TLDI_BVCR_BVR );
    TLDI->BVCR |= (TLDI_InitParaStruct->TLDI_BackgroundValuegreen << 8 | TLDI_InitParaStruct->TLDI_BackgroundValuered<< 16 |\
                                                                  TLDI_InitParaStruct->TLDI_BackgroundValueBlue);
}

/**
  * @brief  Initialize the sturct TLDI_InitPara.
  * @param  TLDI_InitParaStruct: the sturct TLDI_InitPara pointer.
  * @retval None.
  */
void TLDI_ParaInit(TLDI_InitPara* TLDI_InitParaStruct)
{
    TLDI_InitParaStruct->TLDI_HSPolaritySelect = TLDI_HSPOLARITYSELECT_LOW;      
    TLDI_InitParaStruct->TLDI_VSPolaritySelect = TLDI_VSPOLARITYSELECT_LOW;      
    TLDI_InitParaStruct->TLDI_DEPolaritySelect = TLDI_DEPOLARITYSELECT_LOW;     
    TLDI_InitParaStruct->TLDI_CLKPolaritySelect = TLDI_CLKPOLARITYSELECT_TLDICLOCK;     
    TLDI_InitParaStruct->TLDI_HSPulseSize = 0x00;                
    TLDI_InitParaStruct->TLDI_VSPulseSize = 0x00;                 
    TLDI_InitParaStruct->TLDI_HBackPorchSize = 0x00;               
    TLDI_InitParaStruct->TLDI_VBackPorchSize = 0x00;               
    TLDI_InitParaStruct->TLDI_HActiveSize = 0x00;            
    TLDI_InitParaStruct->TLDI_VActiveSize = 0x00;           
    TLDI_InitParaStruct->TLDI_HTotalSize = 0x00;                    
    TLDI_InitParaStruct->TLDI_VTotalSize = 0x00;                    
    TLDI_InitParaStruct->TLDI_BackgroundValuered = 0x00;            
    TLDI_InitParaStruct->TLDI_BackgroundValuegreen = 0x00;          
    TLDI_InitParaStruct->TLDI_BackgroundValueBlue = 0x00;          
}

/**
  * @brief  Enable or disable the TLDI Controller.
  * @param  NewValue: new state of the TLDI peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TLDI_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable TLDI by setting TLDIEN bit */
        TLDI->CTLR |= (uint32_t)TLDI_CTLR_TLDIEN;
    }
    else
    {
        /* Disable TLDI by clearing TLDIEN bit */
        TLDI->CTLR &= ~(uint32_t)TLDI_CTLR_TLDIEN;
    }
}

/**
  * @brief  Enable or disable Dither.
  * @param  NewValue: new state of the Dither.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

void TLDI_DitherEnable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        /* Enable Dither by setting DEN bit */
        TLDI->CTLR |= (uint32_t)TLDI_CTLR_DEN;
    }
    else
    {
        /* Disable Dither by clearing DEN bit */
        TLDI->CTLR &= ~(uint32_t)TLDI_CTLR_DEN;
    }
}

/**
  * @brief  Get the dither color bit value.
  * @param  None.
  * @retval None
  */
TLDI_Color_InitPara TLDI_GetDitherColorBits(void)
{
    TLDI_Color_InitPara DitherColor_BitsParaStruct;

    TLDI->CTLR &= (uint32_t)CTLR_MASK;

    DitherColor_BitsParaStruct.BlueValue = (uint32_t)((TLDI->CTLR >> 4) & 0x7);
    DitherColor_BitsParaStruct.GreenValue = (uint32_t)((TLDI->CTLR >> 8) & 0x7);
    DitherColor_BitsParaStruct.RedValue = (uint32_t)((TLDI->CTLR >> 12) & 0x7);

    return DitherColor_BitsParaStruct;
}

/**
  * @brief  Initialize the sturct TLDI_Color_InitPara.
  * @param  Color_InitParaStruct: the sturct TLDI_Color_InitPara pointer.
  * @retval None.
  */
void TLDI_ColorParaInit(TLDI_Color_InitPara* Color_InitParaStruct)
{
    Color_InitParaStruct->BlueValue = 0x02;
    Color_InitParaStruct->GreenValue = 0x02;
    Color_InitParaStruct->RedValue = 0x02;
}

/**
  * @brief  Define the position of the line reach .
  * @param  TLDI_LRPositionConfig: Line reach position.
  * @retval None
  */
void TLDI_LineReachPosConfig(uint32_t TLDI_LRPositionConfig)
{
  /* Set the Line Interrupt position */
  TLDI->LRFR = (uint32_t)TLDI_LRPositionConfig;
}

/**
  * @brief  reload layers registers with new parameters 
  * @param  TLDI_ReloadLayerType: specifies the type of reload.
  *   This parameter can be one of the following values:
  *     @arg TLDI_RELOADLAYERTYPE_FBR: Vertical blanking reload.
  *     @arg TLDI_RELOADLAYERTYPE_RQR: Immediate reload.  
  * @retval None
  */
void TLDI_ReloadConfig(uint32_t TLDI_ReloadLayerType)
{
    /* Set the Reload type */
    TLDI->RLCR = (uint32_t)TLDI_ReloadLayerType;
}


/**
  * @brief  Initialize the TLDI Layer on the base of  the specified parameters
  *         in the TLDI_Layer_InitParaStruct.
  * @note   This function can be used in case that the TLDI is disabled.
  * @param  TLDI_layerx: configures the layer to be initialized, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2    
  * @param   pointer to a Layer_InitPara structure .
  * @retval None
  */
void TLDI_LayerInit(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_Layer_InitPara* Layer_InitParaStruct)
{
    /* Configure the horizontal start and stop position */
    TLDI_Layerx->HWCR &= ~(TLDI_LxHWCR_WRP | TLDI_LxHWCR_WLP);
    TLDI_Layerx->HWCR = (Layer_InitParaStruct->Layer_WindowRightPos | \
                       Layer_InitParaStruct->Layer_WindowLeftPos << 16);

    /* Configure the vertical start and stop position */
    TLDI_Layerx->VWCR &= ~(TLDI_LxVWCR_WBP | TLDI_LxVWCR_WTP );
    TLDI_Layerx->VWCR  = (Layer_InitParaStruct->Layer_WindowBottomPos | \
                         Layer_InitParaStruct->Layer_WindowTopPos << 16);

    /* Specify the pixel format */
    TLDI_Layerx->PFR &= ~(TLDI_LxPFR_PF );
    TLDI_Layerx->PFR = (Layer_InitParaStruct->Layer_PixelFormat);

    /* Configure the default color values */
    TLDI_Layerx->DCR &=  ~(TLDI_LxDCR_DCB | TLDI_LxDCR_DCG | TLDI_LxDCR_DCR | TLDI_LxDCR_DCA);
    TLDI_Layerx->DCR = (Layer_InitParaStruct->Layer_DefaultColorBlue | \
                      Layer_InitParaStruct->Layer_DefaultColorGreen<< 8  | \
                      Layer_InitParaStruct->Layer_DefaultColorRed<< 16  | \
                      Layer_InitParaStruct->Layer_DefaultColorAlpha<< 24  );

    /* Specified alpha value */      
    TLDI_Layerx->SAR &= ~(TLDI_LxSAR_SALPHA );
    TLDI_Layerx->SAR = (Layer_InitParaStruct->Layer_SpecifiedAlpha);

    /* Specify the blending factors */
    TLDI_Layerx->BMR &= ~(TLDI_LxBMR_ACM2 | TLDI_LxBMR_ACM1 );
    TLDI_Layerx->BMR = (Layer_InitParaStruct->Layer_ACM_BlendingFactor_1 | Layer_InitParaStruct->Layer_ACM_BlendingFactor_2);

    /* Configure the color frame buffer start address */
    TLDI_Layerx->FBSAR &= ~(TLDI_LxFBSAR_FBSA);
    TLDI_Layerx->FBSAR = (Layer_InitParaStruct->Layer_FrameBufferStartAddress);

    /* Configure the color frame buffer pitch in byte */
    TLDI_Layerx->FBLR  &= ~(TLDI_LxFBLR_FBL | TLDI_LxFBLR_FBOFFSET);
    TLDI_Layerx->FBLR  = (Layer_InitParaStruct->Layer_FrameBufferLength |\
                          Layer_InitParaStruct->Layer_FrameBufferOffset<< 16);

    /* Configure the frame buffer line number */
    TLDI_Layerx->FBLNR  &= ~(TLDI_LxFBLNR_FBLN );
    TLDI_Layerx->FBLNR  = (Layer_InitParaStruct->Layer_FrameBufferLineNumber);

}

/**
  * @brief  Initialize the sturct TLDI_Layer_InitPara.
  * @param  Layer_InitParaStruct: the sturct TLDI_Layer_InitPara pointer.
  * @retval None.
  */
void TLDI_LayerParaInit(TLDI_Layer_InitPara * Layer_InitParaStruct)
{

    /*!< Initialize the WindowrightPos and WindowLeftPos member */
    Layer_InitParaStruct->Layer_WindowRightPos = 0x00;
    Layer_InitParaStruct->Layer_WindowLeftPos = 0x00;

    /*!< Initialize the WindowBottomPos and WindowTopPos member */
    Layer_InitParaStruct->Layer_WindowBottomPos = 0x00;
    Layer_InitParaStruct->Layer_WindowTopPos = 0x00;

    /*!< Initialize PixelFormat member */
    Layer_InitParaStruct->Layer_PixelFormat = PF_ARGB8888;

    /*!< Initialize SpecifiedAlpha member */
    Layer_InitParaStruct->Layer_SpecifiedAlpha = 0xFF;

    /*!< Initialize the default color values */
    Layer_InitParaStruct->Layer_DefaultColorBlue = 0x00;
    Layer_InitParaStruct->Layer_DefaultColorGreen = 0x00;
    Layer_InitParaStruct->Layer_DefaultColorRed = 0x00;
    Layer_InitParaStruct->Layer_DefaultColorAlpha = 0x00;

    /*!< Initialize the ACM blending factors */
    Layer_InitParaStruct->Layer_ACM_BlendingFactor_1 = LAYER_ACM_BLENDINGFACTOR_1_SAPA;
    Layer_InitParaStruct->Layer_ACM_BlendingFactor_2 = LAYER_ACM_BLENDINGFACTOR_2_SAPA;

    /*!< Initialize the frame buffer start address */
    Layer_InitParaStruct->Layer_FrameBufferStartAddress = 0x00;

    /*!< Initialize the frame buffer length and offset */
    Layer_InitParaStruct->Layer_FrameBufferLength = 0x00;
    Layer_InitParaStruct->Layer_FrameBufferOffset = 0x00;

    /*!< Initialize the frame buffer line number */
    Layer_InitParaStruct->Layer_FrameBufferLineNumber = 0x00;
}


/**
  * @brief  Enable or disable the TLDI_Layer Controller.
  * @param  TLDI_layerx: configures layer to be enabled, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2
  * @param  NewValue: new state of the TLDI_Layer peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TLDI_Layer_Enable(TLDI_Layer_TypeDef* TLDI_Layerx, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
    /* Enable TLDI_Layer by setting LEN bit */
    TLDI_Layerx->CTLR |= (uint32_t)TLDI_LxCTLR_LEN;
    }
    else
    {
    /* Disable TLDI_Layer by clearing LEN bit */
    TLDI_Layerx->CTLR &= ~(uint32_t)TLDI_LxCTLR_LEN;
    }
}


/**
  * @brief  Attain the current position.
  * @param  None.
  * @retval None
  */
TLDI_Pixel_CurrentPosition TLDI_GetCurrentPos(void)
{
    TLDI_Pixel_CurrentPosition Pixel_CurrentPositionStruct;

    TLDI->CPPR &= ~(TLDI_CPPR_VPOS | TLDI_CPPR_HPOS);

    Pixel_CurrentPositionStruct.Current_HorPosition = (uint32_t)(TLDI->CPPR >> 16);
    Pixel_CurrentPositionStruct.Current_VerPosition = (uint32_t)(TLDI->CPPR & 0xFFFF);

    return Pixel_CurrentPositionStruct;
}

/**
  * @brief  Initialize the sturct Pixel_CurrentPosition.
  * @param  Pixel_CurrentPositionStruct: the sturct Pixel_CurrentPosition pointer.
  * @retval None.
  */
void TLDI_PosParaInit(TLDI_Pixel_CurrentPosition* Pixel_CurrentPositionStruct)
{
    Pixel_CurrentPositionStruct->Current_HorPosition = 0x00;
    Pixel_CurrentPositionStruct->Current_VerPosition = 0x00;
}

/**
  * @brief  Check whether the specified flag of TLDI is set or not.
  * @param  TLDI_CIS: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg TLDI_CISR_VDE: vertical data enable current status.
  *     @arg TLDI_CISR_HDE: horizontal data enable current status.
  *     @arg TLDI_CISR_VS:  Vertical Synchronization current status.
  *     @arg TLDI_CISR_HS:  Horizontal Synchronization current status.
  * @retval The new state of TLDI_CIS (SET or RESET).
  */
TypeState TLDI_GetCurInterfaceState(uint32_t TLDI_CIS)
{
    if ((TLDI->CISR & TLDI_CIS) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }

}

/**
  * @brief  Configure the color keying.
  * @param  ColorKey_InitParaStruct: pointer to TLDI_ColorKey_InitPara structure. 
  * @param  TLDI_layerx: Select the layer to be configured.
       This parameter can be one of the following values: TLDI_Layer1, TLDI_Layer2   
  * @param  NewValue:This parameter can be ENABLE or DISABLE
  * @retval None
  */

void TLDI_ColorKeyingConfig(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_ColorKey_InitPara* ColorKey_InitParaStruct, TypeState NewState)
{   
    if (NewState != DISABLE)
    {
        /* Enable TLDI color keying by setting COKYEN bit */
        TLDI_Layerx->CTLR |= (uint32_t)TLDI_LxCTLR_COKYEN;

        /* Set the color keying values */
        TLDI_Layerx->CKVR  &= ~(TLDI_LxCKVR_CKB | TLDI_LxCKVR_CKG | TLDI_LxCKVR_CKR);
        TLDI_Layerx->CKVR |= (ColorKey_InitParaStruct->ColorKey_Blue | ColorKey_InitParaStruct->ColorKey_Green << 8 | \
                              ColorKey_InitParaStruct->ColorKey_Red << 16);
    }
    else
    {
        /* Disable TLDI color keying by clearing COKYEN bit */
        TLDI_Layerx->CTLR &= ~(uint32_t)TLDI_LxCTLR_COKYEN;
    }
    /* Reload shadow register */
    TLDI->RLCR = TLDI_RELOADLAYERTYPE_RQR;
}

/**
  * @brief  Attach each TLDI_ColorKey_InitPara member to its default value.
  * @param  ColorKey_InitParaStruct:pointer to a TLDI_ColorKey_InitPara structure .
  * @retval None
  */
void TLDI_ColorKeyingParaInit(TLDI_ColorKey_InitPara* ColorKey_InitParaStruct)
{
    /*!< Initialize the color keying values */
    ColorKey_InitParaStruct->ColorKey_Blue = 0x00;
    ColorKey_InitParaStruct->ColorKey_Green = 0x00;
    ColorKey_InitParaStruct->ColorKey_Red = 0x00;
}


/**
  * @brief  Enable or disables LUT.
  * @param  NewState: new state of LUT.
  * @param  TLDI_layerx: Selects the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2  
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TLDI_LUT_Enable(TLDI_Layer_TypeDef* TLDI_Layerx, TypeState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable CLUT by setting LUTEN bit */
        TLDI_Layerx->CTLR |= (uint32_t)TLDI_LxCTLR_LUTEN;
    }
    else
    {
        /* Disable CLUT by clearing LUTEN bit */
        TLDI_Layerx->CTLR &= ~(uint32_t)TLDI_LxCTLR_LUTEN;
    }
    /* Reload shadow register */
    TLDI->RLCR = TLDI_RELOADLAYERTYPE_RQR;
}

/**
  * @brief  Initializes the LUT.
  * @param  LUT_InitParaStruct:pointer to a TLDI_LUT_InitPara structure .
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2   
  * @retval None
  */
void TLDI_LUTInit(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_LUT_InitPara* LUT_InitParaStruct)
{     
    /* Specifies the CLUT address and RGB value */
    TLDI_Layerx->LUTWR  = (LUT_InitParaStruct->LUT_BlueValue | \
                         LUT_InitParaStruct->LUT_GreenValue << 8 | \
                         LUT_InitParaStruct->LUT_RedValue << 16| \
                         LUT_InitParaStruct->LUT_Adress << 24 );
}

/**
  * @brief  Attaches each TLDI_LUT_InitPara member to its default value.
  * @param  LUT_InitParaStruct:pointer to a TLDI_LUT_InitPara structure .
  * @retval None
  */
void TLDI_LUTParaInit(TLDI_LUT_InitPara* LUT_InitParaStruct)
{
    /*!< Initialize the LUT address and RGB values */
    LUT_InitParaStruct->LUT_Adress = 0x00;
    LUT_InitParaStruct->LUT_BlueValue = 0x00;
    LUT_InitParaStruct->LUT_GreenValue = 0x00;
    LUT_InitParaStruct->LUT_RedValue = 0x00;
}


/**
  * @brief  Reconfigure the layer position.
  * @param  OffsetX: horizontal offset .
  * @param  OffsetY: vertical offset .   
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2   
  * @retval Reload of the shadow registers values must be applied in case that layer 
  *         position reconfiguration.
  */
void TLDI_LayerPosition(TLDI_Layer_TypeDef* TLDI_Layerx, uint16_t OffsetX, uint16_t OffsetY)
{  
    uint32_t tempreg, temp;
    uint32_t horizontal_start;
    uint32_t horizontal_stop;
    uint32_t vertical_start;
    uint32_t vertical_stop;

    TLDI_Layerx->HWCR &= ~(TLDI_LxHWCR_WRP | TLDI_LxHWCR_WLP);
    TLDI_Layerx->VWCR &= ~(TLDI_LxVWCR_WBP | TLDI_LxVWCR_WTP);

    /* Reconfigures the horizontal and vertical start position */
    tempreg = TLDI->BPSCR;
    horizontal_start = (tempreg >> 16) + 1 + OffsetX;
    vertical_start = (tempreg & 0xFFFF) + 1 + OffsetY;

    /* Reconfigures the horizontal and vertical stop position */
    /* Get the number of byte per pixel */

    tempreg = TLDI_Layerx->PFR;

    if (tempreg == PF_ARGB8888)
    {
        temp = 4;
    }
    else if (tempreg == PF_RGB888)
    {
        temp = 3;
    }
    else if ((tempreg == PF_ARGB4444) || 
          (tempreg == PF_RGB565)    ||  
          (tempreg == PF_ARGB1555)  ||
          (tempreg == PF_AL88))
    {
        temp = 2;  
    }
    else
    {
        temp = 1;
    }  

    tempreg = TLDI_Layerx->FBLR;
    horizontal_stop = (((tempreg & 0x1FFF) - 3)/temp) + horizontal_start - 1;

    tempreg = TLDI_Layerx->FBLNR;
    vertical_stop = (tempreg & 0x7FF) + vertical_start - 1;  

    TLDI_Layerx->HWCR = horizontal_start | (horizontal_stop << 16);
    TLDI_Layerx->VWCR = vertical_start | (vertical_stop << 16);  
}
  
/**
  * @brief  Reconfigure alpha.
  * @param  ConstantAlpha:constant alpha value.
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2    
  * @retval Reload of the shadow registers values must be applied depending on constant 
  *         alpha reconfiguration.         
  */
void TLDI_LayerAlpha(TLDI_Layer_TypeDef* TLDI_Layerx, uint8_t Alpha)
{  
    /* reconfigure the constant alpha value */      
    TLDI_Layerx->SAR = Alpha;
}

/**
  * @brief  Reconfigure layer address.
  * @param  Address:The color frame buffer start address.
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2     
  * @retval Reload of the shadow registers values must be applied depending on layer 
  *         address reconfiguration.
  */
void TLDI_LayerAddress(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t Address)
{
    /* Reconfigures the color frame buffer start address */
    TLDI_Layerx->FBSAR = Address;
}
  
/**
  * @brief  Reconfigure layer size.
  * @param  Width: layer window width.
  * @param  Height: layer window height.   
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2   
  * @retval Reload of the shadow registers values must be applied depending on layer 
  *         size reconfiguration.
  */
void TLDI_LayerSize(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t Width, uint32_t Height)
{
    uint8_t temp;
    uint32_t tempreg;
    uint32_t horizontal_start;
    uint32_t horizontal_stop;
    uint32_t vertical_start;
    uint32_t vertical_stop;  

    tempreg = TLDI_Layerx->PFR;

    if (tempreg == PF_ARGB8888)
    {
    temp = 4;
    }
    else if (tempreg == PF_RGB888)
    {
    temp = 3;
    }
    else if ((tempreg == PF_ARGB4444) || \
          (tempreg == PF_RGB565)    || \
          (tempreg == PF_ARGB1555)  || \
          (tempreg == PF_AL88))
    {
    temp = 2;  
    }
    else
    {
    temp = 1;
    }

    /* update horizontal and vertical stop */
    tempreg = TLDI_Layerx->HWCR;
    horizontal_start = (tempreg & 0x1FFF);
    horizontal_stop = Width + horizontal_start - 1;  

    tempreg = TLDI_Layerx->VWCR;
    vertical_start = (tempreg & 0x1FFF);
    vertical_stop = Height + vertical_start - 1;  

    TLDI_Layerx->HWCR = horizontal_start | (horizontal_stop << 16);
    TLDI_Layerx->VWCR = vertical_start | (vertical_stop << 16);  

    /* Reconfigures the color frame buffer pitch in byte */
    TLDI_Layerx->FBLR  = ((Width * temp) << 16) | ((Width * temp) + 3);  

    /* Reconfigures the frame buffer line number */
    TLDI_Layerx->FBLNR  = Height;  
}

/**
  * @brief  Reconfigure layer pixel format.
  * @param  PixelFormat: reconfigures the pixel format.   
  * @param  TLDI_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: TLDI_Layer1, TLDI_Layer2   
  * @retval Reloads of the shadow registers values must be applied depending on layer 
  *         pixel format reconfiguration.
  */
void TLDI_LayerPixelFormat(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t PixelFormat)
{
    uint8_t temp;
    uint32_t tempreg;

    tempreg = TLDI_Layerx->PFR;

    if (tempreg == PF_ARGB8888)
    {
        temp = 4;
    }
    else if (tempreg == PF_RGB888)
    {
        temp = 3;
    }
    else if ((tempreg == PF_ARGB4444) || \
          (tempreg == PF_RGB565)    || \
          (tempreg == PF_ARGB1555)  || \
          (tempreg == PF_AL88))  
    {
        temp = 2;  
    }
    else
    {
        temp = 1;
    }

    tempreg = (TLDI_Layerx->FBLR >> 16);
    tempreg = (tempreg / temp); 

    if (PixelFormat == PF_ARGB8888)
    {
        temp = 4;
    }
    else if (PixelFormat == PF_RGB888)
    {
        temp = 3;
    }
    else if ((PixelFormat == PF_ARGB4444) || \
          (PixelFormat == PF_RGB565)    || \
          (PixelFormat == PF_ARGB1555)  || \
          (PixelFormat == PF_AL88))
    {
        temp = 2;  
    }
    else
    {
        temp = 1;
    }

    /* Reconfigures the color frame buffer pitch in byte */
    TLDI_Layerx->FBLR  = ((tempreg * temp) << 16) | ((tempreg * temp) + 3);  

    /* Reconfigures the color frame buffer start address */
    TLDI_Layerx->PFR = PixelFormat;

}
    

/**
  * @brief  Enable or disable TLDI nterrupts.
  * @param  TLDI_INT: specifies the TLDI interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg TLDI_INT_CRIM
  *     @arg TLDI_INT_FEIM
  *     @arg TLDI_INT_TEIM
  *     @arg TLDI_INT_LRIM  
  * @param NewState: This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TLDI_INTConfig(uint32_t TLDI_INT, TypeState NewState)
{
    if (NewState != DISABLE)
    {
        TLDI->IMR |= TLDI_INT;
    }
    else
    {
        TLDI->IMR &= (uint32_t)~TLDI_INT;
    }
}

/**
  * @brief  Detect the interrupt bit flag.
  * @param  TLDI_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg TLDI_FLAG_LR:    Line Interrupt flag.
  *     @arg TLDI_FLAG_FE:   FIFO Underrun Interrupt flag.
  *     @arg TLDI_FLAG_TE: Transfer Error Interrupt flag.
  *     @arg TLDI_FLAG_CLR:   Register Reload interrupt flag.
  * @retval The new state of TLDI_FLAG (SET or RESET).
  */
TypeState TLDI_GetBitState(uint32_t TLDI_FLAG)
{
    if ((TLDI->STR & TLDI_FLAG) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }

}

/**
  * @brief  Clear the TLDI flags.
  * @param  TLDI_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg TLDI_FLAG_LR:    Line Interrupt flag.
  *     @arg TLDI_FLAG_FE:   FIFO Underrun Interrupt flag.
  *     @arg TLDI_FLAG_TE: Transfer Error Interrupt flag.
  *     @arg TLDI_FLAG_CLR:   Register Reload interrupt flag.
  * @retval None
  */
void TLDI_ClearBitState(uint32_t TLDI_FLAG)
{
    /* Clear the corresponding TLDI flag */
    TLDI->CFR = (uint32_t)TLDI_FLAG;
}

/**
  * @brief  Check whether the specified TLDI's interrupt has occurred or not.
  * @param  TLDI_INT: specifies the TLDI interrupts sources to check.
  *   This parameter can be one of the following values:
  *     @arg TLDI_INT_CRIM
  *     @arg TLDI_INT_FEIM
  *     @arg TLDI_INT_TEIM
  *     @arg TLDI_INT_LRIM  
  * @retval The new state of the TLDI_IT (SET or RESET).
  */
TypeState TLDI_GetIntState(uint32_t TLDI_INT)
{
    TypeState bitstatus = RESET;

    if ((TLDI->STR & TLDI_INT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    if (((TLDI->IMR & TLDI_INT) != (uint32_t)RESET) && (bitstatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}


/**
  * @brief  Clear the TLDI interrupt bit.
  * @param  TLDI_INT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg TLDI_INT_CRIM
  *     @arg TLDI_INT_FEIM
  *     @arg TLDI_INT_TEIM
  *     @arg TLDI_INT_LRIM  
  * @retval None
  */
void TLDI_ClearIntBitState(uint32_t TLDI_INT)
{
    /* Clear the corresponding TLDI Interrupt */
    TLDI->CFR = (uint32_t)TLDI_INT;
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
