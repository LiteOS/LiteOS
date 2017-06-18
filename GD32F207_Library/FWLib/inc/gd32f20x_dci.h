/**
  ******************************************************************************
  * @file    gd32f20x_dci.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   DCI header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_DCI_H
#define __GD32F20x_DCI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup DCI
  * @{
  */ 
/** @defgroup DCI_Exported_Types
  * @{
  */
     
/** 
  * @brief   DCI Initial Parameters  
  */ 
typedef struct
{
    uint16_t DCI_SnapshotMode;                    /*!< Snapshot Mode: detailed in @ref DCI_Snapshot_Mode */

    uint16_t DCI_SyncMode;                        /*!< Embedded Synchronous Mode: detailed in @ref DCI_Embeddedsync_Mode */

    uint16_t DCI_CKPolarity;                      /*!< Clock Polarity: detailed in @ref DCI_Clock_Polarity */

    uint16_t DCI_VPolarity;                       /*!< Vsync Polarity: detailed in @ref DCI_Vsync_Polarity */

    uint16_t DCI_HPolarity;                       /*!< Hsync Polarity: detailed in @ref DCI_Hsync_Polarity */

    uint16_t DCI_FrameRate;                       /*!< Frame Rate: detailed in @ref DCI_Frame_Rate */

    uint16_t DCI_InterfaceFormat;                 /*!< Digital Camera Interface Format: detailed in @ref DCI_Interface_Mode */
} DCI_InitPara;

/** 
  * @brief   DCI window timing module Initial Parameters  
  */ 
typedef struct
{
    uint16_t DCI_WindowVerStartPosition;          /*!< Window Vertical Start Position */

    uint16_t DCI_WindowHorStartPosition;          /*!< Window Horizontal Start Position*/

    uint16_t DCI_WindowVerSize;                   /*!< Window Vertical Size */

    uint16_t DCI_WindowHorSize;                   /*!< Window Horizontal Size */
} DCI_WindowInitPara;

/** 
  * @brief   DCI Embedded Sync CODE Init Initial Parameters   
  */ 
typedef struct
{
    uint8_t DCI_FSCode;                           /*!< Frame Start Code in Embedded Synchronous Mode */
    uint8_t DCI_LSCode;                           /*!< Line Start Code in Embedded Synchronous Mode */
    uint8_t DCI_LECode;                           /*!< Line End Code in Embedded Synchronous Mode */
    uint8_t DCI_FECode;                           /*!< Frame End Code in Embedded Synchronous Mode */
} DCI_CodeInitPara;

/**
  * @}
  */

/** @defgroup DCI_Exported_Constants
  * @{
  */

/** @defgroup DCI_Snapshot_Mode 
  * @{
  */ 
#define DCI_CAPMODE_CONTINUOUS                    ((uint16_t)0x0000) /*!< Continuous capture mode */
#define DCI_CAPMODE_SNAPSHOT                      ((uint16_t)0x0002) /*!< Snapshot capture mode */

/**
  * @}
  */ 


/** @defgroup DCI_Embeddedsync_Mode
  * @{
  */ 
#define DCI_SYNCMODE_HARDWARE                     ((uint16_t)0x0000) /*!< Embedded synchronous mode is disabled,Hardware synchronization  */
#define DCI_SYNCMODE_EMBEDDED                     ((uint16_t)0x0010) /*!< Embedded synchronous mode is enabled */

/**
  * @}
  */ 


/** @defgroup DCI_Clock_Polarity 
  * @{
  */ 
#define DCI_CKPOL_FALLING                         ((uint16_t)0x0000) /*!< Capture at falling edge */
#define DCI_CKPOL_RISING                          ((uint16_t)0x0020) /*!< Capture at rising edge */

/**
  * @}
  */ 


/** @defgroup DCI_Vsync_Polarity 
  * @{
  */ 
#define DCI_VPOL_LOW                              ((uint16_t)0x0000) /*!< Low active */
#define DCI_VPOL_HIGH                             ((uint16_t)0x0080) /*!< High active */

/**
  * @}
  */ 


/** @defgroup DCI_Hsync_Polarity 
  * @{
  */ 
#define DCI_HPOL_LOW                              ((uint16_t)0x0000) /*!< Low active */
#define DCI_HPOL_HIGH                             ((uint16_t)0x0040) /*!< High active */

/**
  * @}
  */ 


/** @defgroup DCI_Frame_Rate 
  * @{
  */ 
#define DCI_FRATE_ALL_FRAME                       ((uint16_t)0x0000) /*!< Capture All frames */
#define DCI_FRATE_1IN2_FRAME                      ((uint16_t)0x0100) /*!< Capture one in 2 frames */
#define DCI_FRate_1IN4_Frame                      ((uint16_t)0x0200) /*!< Capture one in 4 frames */

/**
  * @}
  */ 


/** @defgroup DCI_Interface_Mode 
  * @{
  */ 
#define DCI_INTERFACEFORMAT_8B                    ((uint16_t)0x0000) /*!< 8-bit data on every pixel clock */
#define DCI_INTERFACEFORMAT_10B                   ((uint16_t)0x0400) /*!< 10-bit data on every pixel clock */
#define DCI_INTERFACEFORMAT_12B                   ((uint16_t)0x0800) /*!< 12-bit data on every pixel clock */
#define DCI_INTERFACEFORMAT_14B                   ((uint16_t)0x0C00) /*!< 14-bit data on every pixel clock */

/**
  * @}
  */ 


/** @defgroup DCI_interrupt_sources 
  * @{
  */ 
#define DCI_INT_ESEM                              ((uint16_t)0x0001)
#define DCI_INT_VSM                               ((uint16_t)0x0002)
#define DCI_INT_OVRM                              ((uint16_t)0x0004)
#define DCI_INT_EFM                               ((uint16_t)0x0008)
#define DCI_INT_ELM                               ((uint16_t)0x0010)

/**
  * @}
  */ 


/** @defgroup DCI_Flags 
  * @{
  */ 
  
/** @defgroup DCI_Flags 
  * @brief   DCI STR register 
  * @{
  */ 

#define DCI_FLAG_HS                               ((uint16_t)0x2001)
#define DCI_FLAG_VS                               ((uint16_t)0x2002)
#define DCI_FLAG_FV                               ((uint16_t)0x2004)

/**
  * @}
  */ 

/** @defgroup DCI_Flags 
  * @brief   DCI FIR register  
  * @{
  */ 

#define DCI_FLAG_ESEF                             ((uint16_t)0x0001)
#define DCI_FLAG_VSF                              ((uint16_t)0x0002)
#define DCI_FLAG_OVRF                             ((uint16_t)0x0004)
#define DCI_FLAG_EFF                              ((uint16_t)0x0008)
#define DCI_FLAG_ELF                              ((uint16_t)0x0010)

/**
  * @}
  */ 

/** @defgroup DCI_Flags 
  * @brief   DCI MFR register  
  * @{
  */ 

#define DCI_FLAG_ESEMF                            ((uint16_t)0x1001)
#define DCI_FLAG_VSMF                             ((uint16_t)0x1002)
#define DCI_FLAG_OVRMF                            ((uint16_t)0x1004)
#define DCI_FLAG_EFMF                             ((uint16_t)0x1008)
#define DCI_FLAG_ELMF                             ((uint16_t)0x1010)

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup DCI_Exported_Functions
  * @{
  */

void DCI_DeInit(void);
void DCI_Init(DCI_InitPara* DCI_InitParatStruct);
void DCI_ParaInit(DCI_InitPara* DCI_InitParaStruct);
void DCI_WindowFunctionConfig(DCI_WindowInitPara* DCI_WindowInitParaStruct);
void DCI_WindowFunction_Enable(TypeState NewState);
void DCI_EmbeddedSyncDetectConfig(DCI_CodeInitPara* DCI_CodeInitParaStruct);
void DCI_JPEGMode_Enable(TypeState NewState);
void DCI_Enable(TypeState NewState);
void DCI_CAP_Enable(TypeState NewState);
uint32_t DCI_GetPixel(void);
void DCI_INTConfig(uint16_t DCI_IT, TypeState NewState);
TypeState DCI_GetBitState(uint16_t DCI_FLAG);
void DCI_ClearBitState(uint16_t DCI_FLAG);
TypeState DCI_GetIntBitState(uint16_t DCI_INT);
void DCI_ClearIntBitState(uint16_t DCI_INT);

#ifdef __cplusplus
}
#endif

#endif /*__GD32F20x_DCI_H */

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
