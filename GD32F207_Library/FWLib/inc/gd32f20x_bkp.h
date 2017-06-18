/**
  ******************************************************************************
  * @file    gd32f20x_bkp.h
  * @author  MCU SD 
  * @version V1.0.0
  * @date    15-Jul-2015 
  * @brief   BKP header file of the firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F20X_BKP_H
#define __GD32F20X_BKP_H

#ifdef __cplusplus
    extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "gd32f20x.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @addtogroup BKP
  * @{
  */

/** @defgroup BKP_Exported_Constants
  * @{
  */

/** @defgroup TAMPER_1_pin_active_level 
  * @{
  */
#define BKP_TPAL1_HIGH              ((uint16_t)0x0000)
#define BKP_TPAL1_LOW               ((uint16_t)0x0002)

/**
  * @}
  */

/** @defgroup TAMPER_2_pin_active_level 
  * @{
  */
#define BKP_TPAL2_HIGH              ((uint16_t)0x0000)
#define BKP_TPAL2_LOW               ((uint16_t)0x0200)

/**
  * @}
  */

/** @defgroup RTC_clock_calibration_direction
  * @{
  */
#define BKP_RTCCLKCALDIR_UP         ((uint16_t)0x8000)
#define BKP_RTCCLKCALDIR_DOWN       ((uint16_t)0x0000)

/**
  * @}
  */

/** @defgroup RTC_clock_output_selection
  * @{
  */
#define BKP_RTCCLKOUTPUTSEL_DIV64   ((uint16_t)0x0000)
#define BKP_RTCCLKOUTPUTSEL_DIV1    ((uint16_t)0x4000)

/**
  * @}
  */

/** @defgroup RTC_output_on_the_TAMPER_1_pin
  * @{
  */
#define BKP_RTCOUTPUT_NULL          ((uint16_t)0x0000)
#define BKP_RTCOUTPUT_CLKCAL        ((uint16_t)0x0080)
#define BKP_RTCOUTPUT_ALARM         ((uint16_t)0x0100)
#define BKP_RTCOUTPUT_SECOND        ((uint16_t)0x0300)

/**
  * @}
  */

/** @defgroup BKP_Waveform_Detection
  * @{
  */
#define BKP_WAVEFORMDETECT_1        BKP_TPCR2_TPM_1
#define BKP_WAVEFORMDETECT_2        BKP_TPCR2_TPM_2

/**
  * @}
  */

/** @defgroup BKP_Tamper_flag
  * @{
  */
#define BKP_FLAG_TAMPER1            BKP_TIER_TEF_1
#define BKP_FLAG_TAMPER2            BKP_TIER_TEF_2

/**
  * @}
  */

/** @defgroup BKP_Tamper_interrupt_flag
  * @{
  */
#define BKP_INT_TAMPER1             BKP_TIER_TIF_1
#define BKP_INT_TAMPER2             BKP_TIER_TIF_2

/**
  * @}
  */

/** @defgroup Backup_Data_Register 
  * @{
  */
#define BKP_DR1                     ((uint16_t)0x0004)
#define BKP_DR2                     ((uint16_t)0x0008)
#define BKP_DR3                     ((uint16_t)0x000C)
#define BKP_DR4                     ((uint16_t)0x0010)
#define BKP_DR5                     ((uint16_t)0x0014)
#define BKP_DR6                     ((uint16_t)0x0018)
#define BKP_DR7                     ((uint16_t)0x001C)
#define BKP_DR8                     ((uint16_t)0x0020)
#define BKP_DR9                     ((uint16_t)0x0024)
#define BKP_DR10                    ((uint16_t)0x0028)
#define BKP_DR11                    ((uint16_t)0x0040)
#define BKP_DR12                    ((uint16_t)0x0044)
#define BKP_DR13                    ((uint16_t)0x0048)
#define BKP_DR14                    ((uint16_t)0x004C)
#define BKP_DR15                    ((uint16_t)0x0050)
#define BKP_DR16                    ((uint16_t)0x0054)
#define BKP_DR17                    ((uint16_t)0x0058)
#define BKP_DR18                    ((uint16_t)0x005C)
#define BKP_DR19                    ((uint16_t)0x0060)
#define BKP_DR20                    ((uint16_t)0x0064)
#define BKP_DR21                    ((uint16_t)0x0068)
#define BKP_DR22                    ((uint16_t)0x006C)
#define BKP_DR23                    ((uint16_t)0x0070)
#define BKP_DR24                    ((uint16_t)0x0074)
#define BKP_DR25                    ((uint16_t)0x0078)
#define BKP_DR26                    ((uint16_t)0x007C)
#define BKP_DR27                    ((uint16_t)0x0080)
#define BKP_DR28                    ((uint16_t)0x0084)
#define BKP_DR29                    ((uint16_t)0x0088)
#define BKP_DR30                    ((uint16_t)0x008C)
#define BKP_DR31                    ((uint16_t)0x0090)
#define BKP_DR32                    ((uint16_t)0x0094)
#define BKP_DR33                    ((uint16_t)0x0098)
#define BKP_DR34                    ((uint16_t)0x009C)
#define BKP_DR35                    ((uint16_t)0x00A0)
#define BKP_DR36                    ((uint16_t)0x00A4)
#define BKP_DR37                    ((uint16_t)0x00A8)
#define BKP_DR38                    ((uint16_t)0x00AC)
#define BKP_DR39                    ((uint16_t)0x00B0)
#define BKP_DR40                    ((uint16_t)0x00B4)
#define BKP_DR41                    ((uint16_t)0x00B8)
#define BKP_DR42                    ((uint16_t)0x00BC)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup BKP_Exported_Functions
  * @{
  */
void BKP_DeInit(void);
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);
void BKP_RTCOutputConfig(uint16_t BKP_RTCOUTPUT);
void BKP_SetRTCCalibrationDir(uint16_t BKP_RTCCLKCALDIR);
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue);
void BKP_RTCCLKOutputConfig(uint16_t BKP_RTCCLKOUTPUTSEL);
void BKP_Tamper_1PinConfig(uint16_t BKP_TPAL1, TypeState NewValue);
void BKP_Tamper_2PinConfig(uint16_t BKP_TPAL2, TypeState NewValue);
void BKP_Tamper_1INT_Enable(TypeState NewValue);
void BKP_Tamper_2INT_Enable(TypeState NewValue);
void BKP_WaveformDetect_Enable(uint16_t BKP_WAVEFORMDETECT, TypeState NewValue);
TypeState BKP_GetBitState(uint16_t BKP_FLAG);
void BKP_ClearBitState(uint16_t BKP_FLAG);
TypeState BKP_GetIntBitState(uint16_t BKP_INT);
void BKP_ClearIntBitState(uint16_t BKP_INT);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F20X_BKP_H */

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
