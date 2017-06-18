/**
  ******************************************************************************
  * @file    gd32f20x_rng.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   RNG header file of the firmware library.
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20x_RNG_H
#define __GD32F20x_RNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup RNG
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/ 

/** @defgroup RNG_Exported_Constants
  * @{
  */
  
/** @defgroup RNG_flags_definition  
  * @{
  */ 
#define RNG_FLAG_DRDY                   ((uint8_t)0x0001) /*!< Data ready */
#define RNG_FLAG_CECS                   ((uint8_t)0x0002) /*!< Clock error current status */
#define RNG_FLAG_SECS                   ((uint8_t)0x0004) /*!< Seed error current status */

#define IS_RNG_GET_FLAG(RNG_FLAG)       (((RNG_FLAG) == RNG_FLAG_DRDY) || \
                                        ((RNG_FLAG) == RNG_FLAG_CECS) || \
                                        ((RNG_FLAG) == RNG_FLAG_SECS))
#define IS_RNG_CLEAR_FLAG(RNG_FLAG)     (((RNG_FLAG) == RNG_FLAG_CECS) || \
                                        ((RNG_FLAG) == RNG_FLAG_SECS))
/**
  * @}
  */ 

/** @defgroup RNG_interrupts_definition   
  * @{
  */  
#define RNG_INT_CEIS                    ((uint8_t)0x20) /*!< Clock error interrupt */
#define RNG_INT_SEIS                    ((uint8_t)0x40) /*!< Seed error interrupt */

#define IS_RNG_IT(IT)                   ((((IT) & (uint8_t)0x9F) == 0x00) && ((IT) != 0x00))
#define IS_RNG_GET_IT(RNG_IT)           (((RNG_IT) == RNG_IT_CEI) || ((RNG_IT) == RNG_IT_SEI))
/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup RNG_Exported_Functions
  * @{
  */
/*  Function used to set the RNG configuration to the default reset state *****/ 
void RNG_DeInit(void);

/* Configuration function *****************************************************/
void RNG_Enable(TypeState NewValue);

/* Get 32 bit Random data function ******************************************/
uint32_t RNG_GetRandomData(void);

/* Interrupts and flags management functions **********************************/
void RNG_INTConfig(TypeState NewValue);
TypeState RNG_GetBitState(uint8_t RNG_FLAG);
void RNG_ClearBitState(uint8_t RNG_FLAG);
TypeState RNG_GetIntBitState(uint8_t RNG_INT);
void RNG_ClearIntBitState(uint8_t RNG_INT);

#ifdef __cplusplus
}
#endif

#endif /*__GD32F20x_RNG_H */

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
