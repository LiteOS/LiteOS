/**************************************************************************//**
 * @file     wdt.h
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 14/08/29 7:56p $
 * @brief    Nano100 series WDT driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_WDT_Driver WDT Driver
  @{
*/

/** @addtogroup NANO100_WDT_EXPORTED_CONSTANTS WDT Exported Constants
  @{
*/
#define WDT_TIMEOUT_2POW4           (0UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^4 * WDT clocks */
#define WDT_TIMEOUT_2POW6           (1UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^6 * WDT clocks */
#define WDT_TIMEOUT_2POW8           (2UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^8 * WDT clocks */
#define WDT_TIMEOUT_2POW10          (3UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^10 * WDT clocks */
#define WDT_TIMEOUT_2POW12          (4UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^12 * WDT clocks */
#define WDT_TIMEOUT_2POW14          (5UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^14 * WDT clocks */
#define WDT_TIMEOUT_2POW16          (6UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^16 * WDT clocks */
#define WDT_TIMEOUT_2POW18          (7UL << WDT_CTL_WTIS_Pos) /*!< WDT setting for timeout interval = 2^18 * WDT clocks */

#define WDT_RESET_DELAY_3CLK        (3UL << WDT_CTL_WTRDSEL_Pos)    /*!< WDT setting reset delay to 3 WDT clocks */
#define WDT_RESET_DELAY_18CLK       (2UL << WDT_CTL_WTRDSEL_Pos)    /*!< WDT setting reset delay to 18 WDT clocks */
#define WDT_RESET_DELAY_130CLK      (1UL << WDT_CTL_WTRDSEL_Pos)    /*!< WDT setting reset delay to 130 WDT clocks */
#define WDT_RESET_DELAY_1026CLK     (0UL << WDT_CTL_WTRDSEL_Pos)    /*!< WDT setting reset delay to 1026 WDT clocks */

/*@}*/ /* end of group NANO100_WDT_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_WDT_EXPORTED_FUNCTIONS WDT Exported Functions
  @{
*/

/**
  * @brief This macro clear WDT time-out reset system flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_RESET_FLAG()  (WDT->ISR = WDT_ISR_RST_IS_Msk)

/**
  * @brief This macro clear WDT time-out interrupt flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_TIMEOUT_INT_FLAG() (WDT->ISR = WDT_ISR_IS_Msk)

/**
  * @brief This macro clear WDT time-out wake-up system flag.
  * @param None
  * @return None
  * \hideinitializer
  */
#define WDT_CLEAR_TIMEOUT_WAKEUP_FLAG() (WDT->ISR = WDT_ISR_WAKE_IS_Msk)

/**
  * @brief This macro indicate WDT time-out to reset system or not.
  * @return WDT reset system or not
  * @retval 0 WDT did not cause system reset
  * @retval 1 WDT caused system reset
  * \hideinitializer
  */
#define WDT_GET_RESET_FLAG() (WDT->ISR & WDT_ISR_RST_IS_Msk ? 1 : 0)

/**
  * @brief This macro indicate WDT time-out interrupt occurred or not.
  * @return WDT time-out interrupt occurred or not
  * @retval 0 WDT time-out interrupt did not occur
  * @retval 1 WDT time-out interrupt occurred
  * \hideinitializer
  */
#define WDT_GET_TIMEOUT_INT_FLAG() (WDT->ISR & WDT_ISR_IS_Msk ? 1 : 0)

/**
  * @brief This macro indicate WDT time-out waked system up or not
  * @return WDT time-out waked system up or not
  * @retval 0 WDT did not wake up system
  * @retval 1 WDT waked up system
  * \hideinitializer
  */
#define WDT_GET_TIMEOUT_WAKEUP_FLAG() (WDT->ISR & WDT_ISR_WAKE_IS_Msk ? 1 : 0)

/**
  * @brief This macro is used to reset 18-bit WDT counter.
  * @details If WDT is activated and enabled to reset system, software must reset WDT counter
  *  before WDT time-out plus reset delay reached. Or WDT generate a reset signal.
  * \hideinitializer
  */
#define WDT_RESET_COUNTER() (WDT->CTL  |= WDT_CTL_WTR_Msk)

/**
 * @brief This function stops WDT counting and disable WDT module
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_Close(void)
{
    WDT->CTL = 0;
    return;
}

/**
 * @brief This function enables the WDT time-out interrupt
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_EnableInt(void)
{
    WDT->IER = WDT_IER_IE_Msk;
    return;
}

/**
 * @brief This function disables the WDT time-out interrupt
 * @param None
 * @return None
 */
__STATIC_INLINE void WDT_DisableInt(void)
{
    WDT->IER = 0;
    return;
}

void  WDT_Open(uint32_t u32TimeoutInterval,
               uint32_t u32ResetDelay,
               uint32_t u32EnableReset,
               uint32_t u32EnableWakeup);

/*@}*/ /* end of group NANO100_WDT_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_WDT_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__WDT_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
