/**
  ******************************************************************************
  * @file    system_gd32f20x.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Header File.
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup GD32F20x_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_GD32F20X_H
#define __SYSTEM_GD32F20X_H
#include "stdint.h"
#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup GD32F20x_System_Includes
  * @{
  */

/**
  * @}
  */

/** @addtogroup GD32F20x_System_Exported_types
  * @{
  */
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

/**
  * @}
  */

/** @addtogroup GD32F20x_System_Exported_Functions
  * @{
  */
extern void SystemInit(void);
extern void SystemCoreClock_Update(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_GD32F20X_H */

/**
  * @}
  */
  
/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
