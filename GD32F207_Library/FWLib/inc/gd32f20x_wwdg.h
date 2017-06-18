/**
  ******************************************************************************
  * @file    gd32f20x_wwdg.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   WWDG header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_WWDG_H
#define __GD32F20X_WWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup WWDG
  * @{
  */ 

/** @defgroup WWDG_Exported_Constants
  * @{
  */
  
/** @defgroup WWDG_PRESCALER
  * @{
  */
#define WWDG_PRESCALER_1    ((uint32_t)0x00000000)
#define WWDG_PRESCALER_2    ((uint32_t)0x00000080)
#define WWDG_PRESCALER_4    ((uint32_t)0x00000100)
#define WWDG_PRESCALER_8    ((uint32_t)0x00000180)

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */ 
/** @defgroup WWDG_Exported_Functions
  * @{
  */
  
void WWDG_DeInit(void);
void WWDG_SetPrescalerValue(uint32_t PrescalerValue);
void WWDG_SetWindowValue(uint8_t WindowValue);
void WWDG_EnableInt(void);
void WWDG_SetCounterValue(uint8_t CounterValue);
void WWDG_Enable(uint8_t CounterValue);
TypeState WWDG_GetBitState(void);
void WWDG_ClearBitState(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20X_WWDG_H */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
