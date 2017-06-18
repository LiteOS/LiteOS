/**
  ******************************************************************************
  * @file    gd32f20x_usart.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   USART header file of the firmware library.
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_USART_H
#define __GD32F20X_USART_H

#ifdef __cplusplus
 extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup USART
  * @{
  */

/** @defgroup USART_Exported_Types
  * @{
  */

/** 
  * @brief USART Initial Parameters
  */
typedef struct
{
    uint32_t USART_BRR;                   /*!< the USART communication baud rate configuration.
                                             The baud-rate divider (USARTDIV) has the following relationship to the system clock: 
                                             In case of oversampling by 16, the equation is:
                                             USARTDIV=  UCLK/(16¡ÁBaud Rate)
                                             In case of oversampling by 8, the equation is:
                                             USARTDIV=  UCLK/(8¡ÁBaud Rate) */
    uint32_t USART_WL;                    /*!< Set by USART_CTLR1_WL   Word length 0: 8 Data bits,
                                                                                 1: 9 Data bits */
    uint32_t USART_STBits;                /*!< Stop bits configuration */                         

    uint32_t USART_Parity;                /*!< Set by USART_CTLR1_PCEN */

    uint32_t USART_RxorTx;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled. */

    uint32_t USART_HardwareFlowControl;  /*!< Specifies wether the hardware flow control mode is enabled
                                            or disabled.This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitPara;

/** 
  * @brief  USART Clock Init Structure definition
  */ 

typedef struct
{
    uint32_t USART_CKEN;                  /*!< USART clock enabled this parameter can be a value of @ref USART_CKEN */     
                                         
    uint32_t USART_CPL;                   /*!< Clock polarity of Steady state this parameter can be a value of @ref USART_USART_CKEN */
                                        
    uint32_t USART_CPH;                   /*!< Clock phase this parameter can be a value of @ref USART_Clock_Phase */
                                   
    uint32_t USART_LBCP;                  /*!< Last bit clock pulse this parameter can be a value of @ref USART_Last_Bit */                                                                     
} USART_ClockInitPara;

/**
  * @}
  */
  
/** @defgroup USART_Exported_Constants
  * @{
  */ 

/** @defgroup USART_WL 
  * @{
  */ 
#define USART_WL_8B                                                    ((uint32_t)0x00000000)
#define USART_WL_9B                                                    USART_CTLR1_WL

/**
  * @}
  */ 

/** @defgroup USART_STBits 
  * @{
  */ 
#define USART_STBITS_1                                                 ((uint32_t)0x00000000)
#define USART_STBITS_0_5                                               ((uint32_t)0x00001000)
#define USART_STBITS_2                                                  USART_CTLR2_STB_1
#define USART_STBITS_1_5                                               (USART_CTLR2_STB_0 | USART_CTLR2_STB_1)

/**
  * @}
  */ 

/** @defgroup USART_Parity 
  * @{
  */
#define USART_PARITY_RESET                                             ((uint32_t)0x00000000)
#define USART_PARITY_SETEVEN                                           USART_CTLR1_PCEN
#define USART_PARITY_SETODD                                            (USART_CTLR1_PCEN | USART_CTLR1_PM) 

/**
  * @}
  */ 

/** @defgroup USART_RxorTx 
  * @{
  */ 
#define USART_RXORTX_RX                                                USART_CTLR1_REN
#define USART_RXORTX_TX                                                USART_CTLR1_TEN

/**
  * @}
  */ 

/** @defgroup USART_Hardware_Flow_Control 
  * @{
  */ 
#define USART_HARDWAREFLOWCONTROL_NONE                                 ((uint32_t)0x00000000)
#define USART_HARDWAREFLOWCONTROL_RTS                                  USART_CTLR3_RTSEN
#define USART_HARDWAREFLOWCONTROL_CTS                                  USART_CTLR3_CTSEN
#define USART_HARDWAREFLOWCONTROL_RTS_CTS                              (USART_CTLR3_RTSEN | USART_CTLR3_CTSEN)

/**
  * @}
  */ 

/** @defgroup USART_USART_CKEN 
  * @{
  */ 
#define USART_CKEN_RESET                                               ((uint32_t)0x00000000)
#define USART_CKEN_SET                                                 USART_CTLR2_CKEN

/**
  * @}
  */ 

/** @defgroup USART_Clock_Polarity 
  * @{
  */
#define USART_CPL_LOW                                                  ((uint32_t)0x00000000)
#define USART_CPL_HIGH                                                 USART_CTLR2_CPL

/**
  * @}
  */ 

/** @defgroup USART_Clock_Phase
  * @{
  */
#define USART_CPH_1EDGE                                                ((uint32_t)0x00000000)
#define USART_CPH_2EDGE                                                USART_CTLR2_CPH

/**
  * @}
  */

/** @defgroup USART_Last_Bit
  * @{
  */
#define USART_LBCP_DISABLE                                             ((uint32_t)0x00000000)
#define USART_LBCP_ENABLE                                              USART_CTLR2_LBCP

/**
  * @}
  */
  
/** @defgroup USART_DMA_Requests 
  * @{
  */
#define USART_DMAREQ_TX                                                USART_CTLR3_DENT
#define USART_DMAREQ_RX                                                USART_CTLR3_DENR

/**
  * @}
  */ 



/** @defgroup USART_MuteMode_WakeUp_methods
  * @{
  */
#define USART_WAKEUP_IDLELINE                                          ((uint32_t)0x00000000)
#define USART_WAKEUP_ADDRESSMARK                                       USART_CTLR1_WM

/**
  * @}
  */



/** @defgroup USART_LIN_Break_Detection_Length 
  * @{
  */
#define USART_LINBREAKDETECTLENGTH_10B                                 ((uint32_t)0x00000000)
#define USART_LINBREAKDETECTLENGTH_11B                                 USART_CTLR2_LBDL

/**
  * @}
  */

/** @defgroup USART_IrDA_Low_Power 
  * @{
  */
#define USART_IRDAMODE_LOWPOWER                                        USART_CTLR3_IRLP
#define USART_IRDAMODE_NORMAL                                          ((uint32_t)0x00000000)

/**
  * @}
  */ 


/** @defgroup USART_Inversion_Pins 
  * @{
  */
#define USART_INVPIN_TX                                                USART_CTLR4_TINV
#define USART_INVPIN_RX                                                USART_CTLR4_RINV

/**
  * @}
  */ 


   

/** @defgroup USART_Flags 
  * @{
  */
 
/** @defgroup USART_Flags 
  * @brief STR register flags
  * @{
  */ 
  
#define USART_FLAG_CTSF                                                ((uint32_t)0x10000200)
#define USART_FLAG_LBDF                                                ((uint32_t)0x10000100)
#define USART_FLAG_TBE                                                 ((uint32_t)0x10000080)
#define USART_FLAG_TC                                                  ((uint32_t)0x10000040)
#define USART_FLAG_RBNE                                                ((uint32_t)0x10000020)
#define USART_FLAG_IDLEF                                               ((uint32_t)0x10000010)
#define USART_FLAG_ORE                                                 ((uint32_t)0x10000008)
#define USART_FLAG_NE                                                  ((uint32_t)0x10000004)
#define USART_FLAG_FE                                                  ((uint32_t)0x10000002)
#define USART_FLAG_PE                                                  ((uint32_t)0x10000001)

/**
  * @}
  */ 
/** @defgroup USART_Flags 
  * @brief  STR2 register flags
  * @{
  */ 
  
#define USART_FLAG_RTF                                                 ((uint32_t)0x00000800)
#define USART_FLAG_EBF                                                 ((uint32_t)0x00001000)
#define USART_FLAG_BSY                                                 ((uint32_t)0x00010000)

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup USART_Interrupt_definition 
  * @brief USART Interrupt definition
  * @{
  */
#define USART_INT_PE                                                   ((uint32_t)0x10000028)
#define USART_INT_TBE                                                  ((uint32_t)0x10000727)
#define USART_INT_TC                                                   ((uint32_t)0x10000626)
#define USART_INT_RBNE                                                 ((uint32_t)0x10000525)
#define USART_INT_IDLEF                                                ((uint32_t)0x10000424)
#define USART_INT_LBDF                                                 ((uint32_t)0x10000846)
#define USART_INT_CTSF                                                 ((uint32_t)0x1000096A)
#define USART_INT_ERIE                                                 ((uint32_t)0x10000060) 
#define USART_INT_ORE                                                  ((uint32_t)0x10000360)
#define USART_INT_NE                                                   ((uint32_t)0x10000260)
#define USART_INT_FE                                                   ((uint32_t)0x10000160)

#define USART_INT_EB                                                   ((uint32_t)0x00000C84)
#define USART_INT_RT                                                   ((uint32_t)0x00000B85)
/**
  * @}
  */
  
/**
  * @}
  */
  
/** @defgroup USART_Exported_Functions
  * @{
  */
void USART_DeInit(USART_TypeDef* USARTx);
void USART_Init(USART_TypeDef* USARTx, USART_InitPara* USART_InitParaStruct);
void USART_ParaInit(USART_InitPara* USART_InitParaStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitPara* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitPara* USART_ClockInitParaStruct);
void USART_Enable(USART_TypeDef* USARTx, TypeState NewValue);
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler);
void USART_MSBFirst_Enable(USART_TypeDef* USARTx, TypeState NewValue);
void USART_DataInvert_Enable(USART_TypeDef* USARTx, TypeState NewValue);
void USART_InvPin_Enable(USART_TypeDef* USARTx, uint32_t USART_InvertPin, TypeState NewValue);
void USART_ReceiverTimeOut_Enable(USART_TypeDef* USARTx, TypeState NewValue);
void USART_SetReceiveTimeOut_Threshold(USART_TypeDef* USARTx, uint32_t USART_ReceiverTimeOutThreshold);
void USART_DataSend(USART_TypeDef* USARTx,uint16_t Data);
uint16_t USART_DataReceive(USART_TypeDef* USARTx);
void USART_Address(USART_TypeDef* USARTx, uint8_t USART_Address);
void USART_MuteMode_Enable(USART_TypeDef* USARTx, TypeState NewState);
void USART_MuteModeWakeUp_Set(USART_TypeDef* USARTx, uint32_t USART_WakeUp);
void USART_SetLINBDLength(USART_TypeDef* USARTx,uint32_t USART_LINBreakDetectLength);
void USART_LIN_Enable(USART_TypeDef* USARTx,TypeState NewValue);
void USART_HalfDuplex_Enable(USART_TypeDef* USARTx, TypeState NewValue);
void USART_GuardTime_Set(USART_TypeDef* USARTx, uint8_t USART_GuardTime);
void USART_SmartCard_Enable(USART_TypeDef* USARTx,TypeState NewValue);
void USART_SmartCardNACK_Enable(USART_TypeDef* USARTx,TypeState NewValue);
void USART_AutoRetryNumber_Set(USART_TypeDef* USARTx,uint8_t USART_AutoNumber);
void USART_BlockLength_Set(USART_TypeDef* USARTx,uint8_t USART_BlockLength);
void USART_IrDA_Set(USART_TypeDef* USARTx,uint32_t USART_IrDAMode);
void USART_IrDA_Enable(USART_TypeDef* USARTx,TypeState NewValue);
void USART_DMA_Enable(USART_TypeDef* USARTx, uint32_t USART_DMAEnable, TypeState NewValue);
void USART_INT_Set(USART_TypeDef* USARTx, uint32_t USART_INT, TypeState NewValue);
TypeState USART_GetBitState(USART_TypeDef* USARTx, uint32_t USART_FLAG);
void USART_ClearBitState(USART_TypeDef* USARTx, uint32_t USART_FLAG);
TypeState USART_GetIntBitState(USART_TypeDef* USARTx, uint32_t USART_INT);
void USART_ClearIntBitState(USART_TypeDef* USARTx, uint32_t USART_INT);
void USART_SendBreak(USART_TypeDef* USARTx);
#ifdef __cplusplus
}
#endif

#endif /*__GD32F1X0_USART_H */
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
