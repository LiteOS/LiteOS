/**
  ******************************************************************************
  * @file    gd32f20x_tldi.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   TLDI header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_TLDI_H
#define __GD32F20x_TLDI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup TLDI
  * @{
  */ 

/** @defgroup TLDI_Exported_Types
  * @{
  */
 
/** 
  * @brief TLDI Init structure definition 
  */
typedef struct
{
    uint32_t TLDI_VSPolaritySelect;                 /*!< selects the  vertical synchronization polarity.*/
                                             
    uint32_t TLDI_HSPolaritySelect;                 /*!< selects the  horizontal synchronization polarity.*/
                                               
    uint32_t TLDI_DEPolaritySelect;                 /*!< selects the data enable polarity.*/

    uint32_t TLDI_CLKPolaritySelect;                /*!< selects  clock polarity.*/

    uint32_t TLDI_BackgroundValuered;               /*!< configures the red background value, ranges from 0x00 to 0xFF.*/

    uint32_t TLDI_BackgroundValuegreen;             /*!< configures the green background value, ranges from 0x00 to 0xFF.*/ 

    uint32_t TLDI_BackgroundValueBlue;              /*!< configures the blue background value, ranges from 0x00 to 0xFF.*/

    uint32_t TLDI_HSPulseSize;                      /*!< configures the size of horizontal pulse, ranges from 0x00 to 0xFFF.*/
                                                 
    uint32_t TLDI_VSPulseSize;                      /*!< configures the size of vertical pulse, ranges from 0x00 to 0x7FF.*/

    uint32_t TLDI_HBackPorchSize;                   /*!< configures the size of value horizontal back porch adding up,
                                                         ranges from TLDI_HSPulseSize to 0xFFF.*/

    uint32_t TLDI_VBackPorchSize;                   /*!< configures the size of value vertical back porch adding up, 
                                                         ranges from TLDI_VSPulseSize to 0x7FF.*/
            
    uint32_t TLDI_HActiveSize;                      /*!< configures the  active horizontal size adding up,  
                                                         ranges from TLDI_HBackPorchSize to 0xFFF.*/

    uint32_t TLDI_VActiveSize;                      /*!< configures the  active verical size adding up,  
                                                         ranges from TLDI_VBackPorchSize to 0x7FF.*/

    uint32_t TLDI_HTotalSize;                       /*!< configures the total horizontal size, 
                                                         ranges from TLDI_HActiveSize to 0xFFF.*/

    uint32_t TLDI_VTotalSize;                       /*!< configures the total verical size, 
                                                         ranges from TLDI_VActiveSize to 0x7FF.*/
            
} TLDI_InitPara;


typedef struct
{
    uint32_t Current_HorPosition;
    uint32_t Current_VerPosition;

} TLDI_Pixel_CurrentPosition;

/** 
  * @brief  TLDI color Parameters 
  */
typedef struct
{
    uint32_t BlueValue;                             /*!< Configures the blue value, ranges from 0x00 to 0xFF.*/
                                                 
    uint32_t GreenValue;                            /*!< Configures the green value, ranges from 0x00 to 0xFF.*/

    uint32_t RedValue;                              /*!< Configures the red value, ranges from 0x00 to 0xFF.*/
} TLDI_Color_InitPara;

/** 
  * @brief  TLDI Layer structure definition  
  */
typedef struct
{

    uint32_t Layer_WindowLeftPos;                   /*!< Configures the window left position, 
                                                         ranges from 0x000 to 0xFFF.*/            
    uint32_t Layer_WindowRightPos;                  /*!< Configures the window right position,
                                                         ranges from 0x0000 to 0xFFFF.*/
  
    uint32_t Layer_WindowTopPos;                    /*!< Configures the window top position.
                                                         ranges from 0x000 to 0xFFF.*/

    uint32_t Layer_WindowBottomPos;                 /*!< Configures the Window bottom position.
                                                         ranges from 0x0000 to 0xFFFF.*/
  
    uint32_t Layer_PixelFormat;                     /*!< Configures the pixel format.*/

    uint32_t Layer_SpecifiedAlpha;                  /*!< Configures  alpha, ranges from 0x00 to 0xFF.*/

    uint32_t Layer_DefaultColorBlue;                /*!< Configures the blue default value, ranges from 0x00 to 0xFF.*/

    uint32_t Layer_DefaultColorGreen;               /*!< Configures the green default value, ranges from 0x00 to 0xFF.*/
            
    uint32_t Layer_DefaultColorRed;                 /*!< Configures the red default value, ranges from 0x00 to 0xFF.*/
    
    uint32_t Layer_DefaultColorAlpha;               /*!< Configures the default alpha value, ranges from 0x00 to 0xFF.*/

    uint32_t Layer_ACM_BlendingFactor_1;            /*!< Configures ACM blending factor 1.*/

    uint32_t Layer_ACM_BlendingFactor_2;            /*!< Configures ACM blending factor 2.*/
            
    uint32_t Layer_FrameBufferStartAddress;         /*!< Configures the color frame buffer start address.*/

    uint32_t Layer_FrameBufferOffset;               /*!< Configures the color frame buffer offset, ranges from 0x0000 to 0x1FFF.*/

    uint32_t Layer_FrameBufferLength;               /*!< Configures the color frame buffer length, ranges from 0x0000 to 0x1FFF.*/
                                                 
    uint32_t Layer_FrameBufferLineNumber;           /*!< Configures line number in frame buffer, ranges from 0x000 to 0x7FF.*/
} TLDI_Layer_InitPara;

/** 
  * @brief TLDI color key value Initial Parameters 
  */
typedef struct
{
    uint32_t ColorKey_Blue;                         /*!< Configures the color key blue value,  ranges from 0x00 to 0xFF.*/

    uint32_t ColorKey_Green;                        /*!< Configures the color key green value, ranges from 0x00 to 0xFF.*/
            
    uint32_t ColorKey_Red;                          /*!< Configures the color key red value,  ranges from 0x00 to 0xFF.*/
} TLDI_ColorKey_InitPara;

/** 
  * @brief  TLDI color look_up table Initial Parameters 
  */
typedef struct
{
    uint32_t LUT_Adress;                            /*!< Configures the LUT address,ranges from 0x00 to 0xFF.*/

    uint32_t LUT_BlueValue;                         /*!< Configures the LUT blue value, ranges from 0x00 to 0xFF.*/
                                                
    uint32_t LUT_GreenValue;                        /*!< Configures the LUT green value, range from 0x00 to 0xFF.*/

    uint32_t LUT_RedValue;                          /*!< Configures the LUT red value, range from 0x00 to 0xFF.*/
} TLDI_LUT_InitPara;
/**
  * @}
  */

/** @defgroup TLDI_Exported_Constants
  * @{
  */

/** @defgroup TLDI_VSPolaritySelect 
  * @{
  */
#define TLDI_VSPOLARITYSELECT_LOW                   ((uint32_t)0x00000000)               
#define TLDI_VSPOLARITYSELECT_HIGH                  TLDI_CTLR_VSPS     

/**
  * @}
  */

/** @defgroup TLDI_HSPolaritySelect 
  * @{
  */
#define TLDI_HSPOLARITYSELECT_LOW                   ((uint32_t)0x00000000)               
#define TLDI_HSPOLARITYSELECT_HIGH                  TLDI_CTLR_HSPS 

/**
  * @}
  */

/** @defgroup TLDI_DEPolaritySelect 
  * @{
  */
#define TLDI_DEPOLARITYSELECT_LOW                   ((uint32_t)0x00000000)                
#define TLDI_DEPOLARITYSELECT_HIGH                  TLDI_CTLR_DEPS 

/**
  * @}
  */
  
/** @defgroup TLDI_CLKPolaritySelect 
  * @{
  */
#define TLDI_CLKPOLARITYSELECT_TLDICLOCK            ((uint32_t)0x00000000)                
#define TLDI_CLKPOLARITYSELECT_ITLDICLOCK           TLDI_CTLR_CLKPS 

/**
  * @}
  */
  
/** @defgroup Layer_Pixelformat 
  * @{
  */
#define PF_ARGB8888                                 ((uint32_t)0x00000000)
#define PF_RGB888                                   ((uint32_t)0x00000001)
#define PF_RGB565                                   ((uint32_t)0x00000002)
#define PF_ARGB1555                                 ((uint32_t)0x00000003)
#define PF_ARGB4444                                 ((uint32_t)0x00000004)
#define PF_L8                                       ((uint32_t)0x00000005)
#define PF_AL44                                     ((uint32_t)0x00000006)
#define PF_AL88                                     ((uint32_t)0x00000007)

/**
  * @}
  */

/** @defgroup Layer_ACM_BlendingFactor_1 
  * @{
  */
#define LAYER_ACM_BLENDINGFACTOR_1_SA               ((uint32_t)0x00000400)
#define LAYER_ACM_BLENDINGFACTOR_1_SAPA             ((uint32_t)0x00000600)

/**
  * @}
  */
  
/** @defgroup Layer_ACM_BlendingFactor_2 
  * @{
  */
#define LAYER_ACM_BLENDINGFACTOR_2_SA               ((uint32_t)0x00000005)
#define LAYER_ACM_BLENDINGFACTOR_2_SAPA             ((uint32_t)0x00000007)

/**
  * @}
  */

/** @defgroup TLDI_ReloadLayerType
  * @{
  */                           
#define TLDI_RELOADLAYERTYPE_FBR                    TLDI_RLCR_FBR 
#define TLDI_RELOADLAYERTYPE_RQR                    TLDI_RLCR_RQR 


/**
  * @}
  */
 

/** @defgroup TLDI_Interrupts 
  * @{
  */                           
#define TLDI_INT_CRIM                               TLDI_IMR_CRIM 
#define TLDI_INT_FEIM                               TLDI_IMR_FEIM 
#define TLDI_INT_TEIM                               TLDI_IMR_TEIM
#define TLDI_INT_LRIM                               TLDI_IMR_LRIM

/**
  * @}
  */
      
/** @defgroup TLDI_Flag 
  * @{
  */
#define TLDI_FLAG_CLR                               TLDI_STR_CLR
#define TLDI_FLAG_FE                                TLDI_STR_FE
#define TLDI_FLAG_TE                                TLDI_STR_TE
#define TLDI_FLAG_LR                                TLDI_STR_LR 

/**
  * @}
  */

/**
  * @}
  */ 

/** @defgroup TLDI_Exported_Functions
  * @{
  */
void TLDI_DeInit(void);

/* Initialization and Configuration functions */
void TLDI_Init(TLDI_InitPara* TLDI_InitParaStruct);
void TLDI_ParaInit(TLDI_InitPara* TLDI_InitParaStruct);
void TLDI_Enable(TypeState NewState);
void TLDI_DitherEnable(TypeState NewState);
TLDI_Color_InitPara TLDI_GetDitherColorBits(void);
void TLDI_ColorParaInit(TLDI_Color_InitPara* Color_InitParaStruct);
void TLDI_LineReachPosConfig(uint32_t TLDI_LRPositionConfig);
void TLDI_ReloadConfig(uint32_t TLDI_ReloadLayerType);
void TLDI_LayerInit(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_Layer_InitPara* Layer_InitParaStruct);
void TLDI_LayerParaInit(TLDI_Layer_InitPara * Layer_InitParaStruct);
void TLDI_Layer_Enable(TLDI_Layer_TypeDef* TLDI_Layerx, TypeState NewState);
TLDI_Pixel_CurrentPosition TLDI_GetCurrentPos(void);
void TLDI_PosParaInit(TLDI_Pixel_CurrentPosition* Pixel_CurrentPositionStruct);
TypeState TLDI_GetCurInterfaceState(uint32_t TLDI_CIS);
void TLDI_ColorKeyingConfig(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_ColorKey_InitPara* ColorKey_InitParaStruct, TypeState NewState);
void TLDI_ColorKeyingParaInit(TLDI_ColorKey_InitPara* ColorKey_InitParaStruct);
void TLDI_LUT_Enable(TLDI_Layer_TypeDef* TLDI_Layerx, TypeState NewState);
void TLDI_LUTInit(TLDI_Layer_TypeDef* TLDI_Layerx, TLDI_LUT_InitPara* LUT_InitParaStruct);
void TLDI_LUTParaInit(TLDI_LUT_InitPara* LUT_InitParaStruct);
void TLDI_LayerPosition(TLDI_Layer_TypeDef* TLDI_Layerx, uint16_t OffsetX, uint16_t OffsetY);
void TLDI_LayerAlpha(TLDI_Layer_TypeDef* TLDI_Layerx, uint8_t Alpha);
void TLDI_LayerAddress(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t Address);
void TLDI_LayerSize(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t Width, uint32_t Height);
void TLDI_LayerPixelFormat(TLDI_Layer_TypeDef* TLDI_Layerx, uint32_t PixelFormat);

/* Interrupts and flags management functions */
void TLDI_INTConfig(uint32_t TLDI_INT, TypeState NewState);
TypeState TLDI_GetBitState(uint32_t TLDI_FLAG);
void TLDI_ClearBitState(uint32_t TLDI_FLAG);
TypeState TLDI_GetIntState(uint32_t TLDI_INT);
void TLDI_ClearIntBitState(uint32_t TLDI_INT);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20x_TLDI_H */

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
