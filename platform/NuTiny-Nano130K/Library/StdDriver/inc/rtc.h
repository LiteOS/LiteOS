/**************************************************************************//**
 * @file     rtc.h
 * @version  V1.00
 * $Revision: 7 $
 * $Date: 15/06/26 1:34p $
 * @brief    Nano100 series RTC driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_RTC_Driver RTC Driver
  @{
*/


/** @addtogroup NANO100_RTC_EXPORTED_CONSTANTS RTC Exported Constants
  @{
*/


#define RTC_INIT_KEY         0xA5EB1357  /*!< RTC Access Key     */
#define RTC_WRITE_KEY        0xA965         /*!< RTC Access Key  */

#define RTC_WAIT_COUNT       0xFFFFFFFF     /*!< Initial Time Out Value  */

#define RTC_YEAR2000         2000            /*!< RTC Reference */
#define RTC_FCR_REFERENCE    32761           /*!< RTC Reference */

#define RTC_CLOCK_12         0                /*!< RTC 12 Hour */
#define RTC_CLOCK_24         1                /*!< RTC 24 Hour */

#define RTC_AM               1                /*!< RTC AM */
#define RTC_PM               2                /*!< RTC PM */

#define RTC_TICK_1_SEC       ((uint32_t) 0x00000000)   /*!< Time tick is 1 second */
#define RTC_TICK_1_2_SEC     ((uint32_t) 0x00000001)   /*!< Time tick is 1/2 second */
#define RTC_TICK_1_4_SEC     ((uint32_t) 0x00000002)   /*!< Time tick is 1/4 second */
#define RTC_TICK_1_8_SEC     ((uint32_t) 0x00000003)   /*!< Time tick is 1/8 second */
#define RTC_TICK_1_16_SEC    ((uint32_t) 0x00000004)   /*!< Time tick is 1/16 second */
#define RTC_TICK_1_32_SEC    ((uint32_t) 0x00000005)   /*!< Time tick is 1/32 second */
#define RTC_TICK_1_64_SEC    ((uint32_t) 0x00000006)   /*!< Time tick is 1/64 second */
#define RTC_TICK_1_128_SEC   ((uint32_t) 0x00000007)   /*!< Time tick is 1/128 second */

#define RTC_SUNDAY       ((uint32_t) 0x00000000) /*!< Day of week is sunday */
#define RTC_MONDAY       ((uint32_t) 0x00000001) /*!< Day of week is monday */
#define RTC_TUESDAY      ((uint32_t) 0x00000002) /*!< Day of week is tuesday */
#define RTC_WEDNESDAY    ((uint32_t) 0x00000003) /*!< Day of week is wednesday */
#define RTC_THURSDAY     ((uint32_t) 0x00000004) /*!< Day of week is thursday */
#define RTC_FRIDAY       ((uint32_t) 0x00000005) /*!< Day of week is friday */
#define RTC_SATURDAY     ((uint32_t) 0x00000006) /*!< Day of week is saturday */


#define RTC_SNOOPER_RISING   0      /*!< Snooper Active Rising Edge */
#define RTC_SNOOPER_FALLING  1      /*!< Snooper Active Falling Edge */


/*@}*/ /* end of group NANO100_RTC_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_RTC_EXPORTED_STRUCTS RTC Exported Structs
  @{
*/

/**
  * @brief  RTC define Time Data Struct
  */
typedef struct {
    uint32_t u32Year;          /*!<  Year value */
    uint32_t u32Month;         /*!<  Month value */
    uint32_t u32Day;           /*!<  Day value */
    uint32_t u32DayOfWeek;     /*!<  Day of week value */
    uint32_t u32Hour;          /*!<  Hour value */
    uint32_t u32Minute;        /*!<  Minute value */
    uint32_t u32Second;        /*!<  Second value */
    uint32_t u32TimeScale;     /*!<  12-Hour, 24-Hour */
    uint32_t u32AmPm;          /*!<  Only Time Scale select 12-hr used */
} S_RTC_TIME_DATA_T;

/*@}*/ /* end of group NANO100_RTC_EXPORTED_STRUCTS */


/** @addtogroup NANO100_RTC_EXPORTED_FUNCTIONS RTC Exported Functions
  @{
*/


/**
 *  @brief    Read spare register
 *
 *  @param[in]    u32RegNum    The spare register number(0~23)
 *
 *  @return   Spare register content.
 *
 */
#define RTC_READ_SPARE_REGISTER(u32RegNum)    (RTC->SPR[u32RegNum])

/**
 *  @brief    Write spare register
 *
 *  @param[in]    u32RegNum    The spare register number(0~23)
 *  @param[in]    u32RegValue  The spare register value
 *
 *  @return   None
 *
 */
#define RTC_WRITE_SPARE_REGISTER(u32RegNum, u32RegValue)    (RTC->SPR[u32RegNum] = u32RegValue)

/**
 *  @brief    According to current time, return this year is leap year or not
 *
 *  @param    None
 *
 *  @return   0 = This year is not a leap year. \n
 *            1 = This year is a leap year.
 *
 */
#define RTC_IS_LEAP_YEAR()    ((RTC->LIR & (RTC_LIR_LIR_Msk))?1:0)

/**
 *  @brief    Clear alarm interrupt status.
 *
 *  @param    None
 *
 *  @return   None
 *
 */
#define RTC_CLEAR_ALARM_INT_FLAG()    (RTC->RIIR = RTC_RIIR_AIF_Msk)

/**
 *  @brief    Clear tick interrupt status.
 *
 *  @param    None
 *
 *  @return    None
 *
 */
#define RTC_CLEAR_TICK_INT_FLAG()    (RTC->RIIR = RTC_RIIR_TIF_Msk)

/**
 *  @brief    Clear tamper detect pin status.
 *
 *  @param[in]    u32PinNum    tamper detect pin number.
 *
 *  @return   None
 *
 */
#define RTC_CLEAR_TAMPER_FLAG(u32PinNum)    (RTC->RIIR = RTC_RIIR_SNOOPIF_Msk)

/**
 *  @brief    Get alarm interrupt status.
 *
 *  @param    None
 *
 *  @return   Alarm interrupt status
 *
 */
#define RTC_GET_ALARM_INT_FLAG()    ((RTC->RIIR & RTC_RIIR_AIF_Msk) >> RTC_RIIR_AIF_Pos)

/**
 *  @brief    Get alarm interrupt status.
 *
 *  @param    None
 *
 *  @return   Alarm interrupt status
 *
 */
#define RTC_GET_TICK_INT_FLAG()    ((RTC->RIIR & RTC_RIIR_TIF_Msk) >> RTC_RIIR_TIF_Pos)

/**
 *  @brief    Get tamper detect pin status.
 *
 *  @param    None
 *
 *  @return   1: Snooper Pin Event Detected \n
 *            0: Snooper Pin Event Never Detected
 *
 */
#define RTC_GET_TAMPER_FLAG()    ( (RTC->RIIR & RTC_RIIR_SNOOPIF_Msk) >> RTC_RIIR_SNOOPIF_Pos)

/**
 *  @brief    Enable Timer tick wakeup function.
 *
 *  @param    None
 *
 *  @return   None
 *
 */
#define RTC_ENABLE_TICK_WAKEUP() (RTC->TTR |= RTC_TTR_TWKE_Msk);

/**
 *  @brief    Disable Timer tick wakeup function.
 *
 *  @param    None
 *
 *  @return   None
 *
 */
#define RTC_DISABLE_TICK_WAKEUP() (RTC->TTR &= ~RTC_TTR_TWKE_Msk);


void RTC_Open(S_RTC_TIME_DATA_T *sPt);
void RTC_Close(void);
void RTC_32KCalibration(int32_t i32FrequencyX100);
void RTC_SetTickPeriod(uint32_t u32TickSelection);
void RTC_EnableInt(uint32_t u32IntFlagMask);
void RTC_DisableInt(uint32_t u32IntFlagMask);
uint32_t RTC_GetDayOfWeek(void);
void RTC_DisableTamperDetection(void);
void RTC_EnableTamperDetection(uint32_t u32PinCondition);
void RTC_SetAlarmTime(uint32_t u32Hour, uint32_t u32Minute, uint32_t u32Second, uint32_t u32TimeMode, uint32_t u32AmPm);
void RTC_SetAlarmDate(uint32_t u32Year, uint32_t u32Month, uint32_t u32Day);
void RTC_SetTime(uint32_t u32Hour, uint32_t u32Minute, uint32_t u32Second, uint32_t u32TimeMode, uint32_t u32AmPm);
void RTC_SetDate(uint32_t u32Year, uint32_t u32Month, uint32_t u32Day, uint32_t u32DayOfWeek);
void RTC_SetAlarmDateAndTime(S_RTC_TIME_DATA_T *sPt);
void RTC_SetDateAndTime(S_RTC_TIME_DATA_T *sPt);
void RTC_GetAlarmDateAndTime(S_RTC_TIME_DATA_T *sPt);
void RTC_GetDateAndTime(S_RTC_TIME_DATA_T *sPt);



/*@}*/ /* end of group NANO100_RTC_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO100_RTC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */


#ifdef  __cplusplus
}
#endif

#endif /* __RTC_H__ */


/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/



