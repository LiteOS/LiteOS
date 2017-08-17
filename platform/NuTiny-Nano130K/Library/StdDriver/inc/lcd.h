/****************************************************************************//**
 * @file     lcd.h
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 15/06/26 1:30p $
 * @brief    Nano100 series I2C driver header file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
//#include <stdbool.h>


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_LCD_Driver LCD Driver
  @{
*/


/** @addtogroup NANO100_LCD_EXPORTED_CONSTANTS LCD Exported Constants
  @{
*/

/// @cond
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/// @endcond

#define LCD_FREQ_DIV32      ((uint32_t) 0x00000000) /*!< Clock source (32 or 10KHz) divide by 32 */
#define LCD_FREQ_DIV64      ((uint32_t) 0x00000010) /*!< Clock source (32 or 10KHz) divide by 64 */
#define LCD_FREQ_DIV96      ((uint32_t) 0x00000020) /*!< Clock source (32 or 10KHz) divide by 96 */
#define LCD_FREQ_DIV128     ((uint32_t) 0x00000030) /*!< Clock source (32 or 10KHz) divide by 128 */
#define LCD_FREQ_DIV192     ((uint32_t) 0x00000040) /*!< Clock source (32 or 10KHz) divide by 192 */
#define LCD_FREQ_DIV256     ((uint32_t) 0x00000050) /*!< Clock source (32 or 10KHz) divide by 256 */
#define LCD_FREQ_DIV384     ((uint32_t) 0x00000060) /*!< Clock source (32 or 10KHz) divide by 384 */
#define LCD_FREQ_DIV512     ((uint32_t) 0x00000070) /*!< Clock source (32 or 10KHz) divide by 512 */

#define LCD_MUX_STATIC      ((uint32_t) 0x00000000) /*!< Static multiplexing */
#define LCD_MUX_ONE_SECOND  ((uint32_t) 0x00000002) /*!< Duplex multiplexing */
#define LCD_MUX_ONE_THIRD   ((uint32_t) 0x00000004) /*!< Triplex multiplexing */
#define LCD_MUX_ONE_FOURTH  ((uint32_t) 0x00000006) /*!< Quadruplex multiplexing */
#define LCD_MUX_ONE_FIFTH   ((uint32_t) 0x00000008) /*!< 1/5 duty */
#define LCD_MUX_ONE_SIXTH   ((uint32_t) 0x0000000A) /*!< 1/6 duty */

#define LCD_BIAS_STATIC     ((uint32_t) 0x00000000) /*!< Static bias */
#define LCD_BIAS_HALF       ((uint32_t) 0x00000002) /*!< 1/2 bias */
#define LCD_BIAS_THIRD      ((uint32_t) 0x00000004) /*!< 1/3 bias */

#define LCD_CPUMP_DIV1      ((uint32_t) 0x00000000) /*!< Clock source (32 or 10KHz) divide by 1 and is used by analog component */
#define LCD_CPUMP_DIV2      ((uint32_t) 0x00000800) /*!< Clock source (32 or 10KHz) divide by 2 */
#define LCD_CPUMP_DIV4      ((uint32_t) 0x00001000) /*!< Clock source (32 or 10KHz) divide by 4 */
#define LCD_CPUMP_DIV8      ((uint32_t) 0x00001800) /*!< Clock source (32 or 10KHz) divide by 8 */
#define LCD_CPUMP_DIV16     ((uint32_t) 0x00002000) /*!< Clock source (32 or 10KHz) divide by 16 */
#define LCD_CPUMP_DIV32     ((uint32_t) 0x00002800) /*!< Clock source (32 or 10KHz) divide by 32 */
#define LCD_CPUMP_DIV64     ((uint32_t) 0x00003000) /*!< Clock source (32 or 10KHz) divide by 64 */
#define LCD_CPUMP_DIV128    ((uint32_t) 0x00003800) /*!< Clock source (32 or 10KHz) divide by 128 */

#define LCD_CPVOl_2_6V  ((uint32_t) 0x00000000) /*!< Set charge pump voltage to 2.6 V */
#define LCD_CPVOl_2_7V  ((uint32_t) 0x00000100) /*!< Set charge pump voltage to 2.7 V */
#define LCD_CPVOl_2_8V  ((uint32_t) 0x00000200) /*!< Set charge pump voltage to 2.8 V */
#define LCD_CPVOl_2_9V  ((uint32_t) 0x00000300) /*!< Set charge pump voltage to 2.9 V */
#define LCD_CPVOl_3V    ((uint32_t) 0x00000400) /*!< Set charge pump voltage to 3 V */
#define LCD_CPVOl_3_1V  ((uint32_t) 0x00000500) /*!< Set charge pump voltage to 3.1 V */
#define LCD_CPVOl_3_2V  ((uint32_t) 0x00000600) /*!< Set charge pump voltage to 3.2 V */
#define LCD_CPVOl_3_3V  ((uint32_t) 0x00000700) /*!< Set charge pump voltage to 3.3 V */

#define LCD_FCPRESC_DIV1 ((uint32_t) 0x00000000)    /*!< Set pre-scale divider value to 1 */
#define LCD_FCPRESC_DIV2 ((uint32_t) 0x00000004)    /*!< Set pre-scale divider value to 2 */
#define LCD_FCPRESC_DIV4 ((uint32_t) 0x00000008)    /*!< Set pre-scale divider value to 4 */
#define LCD_FCPRESC_DIV8 ((uint32_t) 0x0000000C)    /*!< Set pre-scale divider value to 8 */

#define LCD_FRAMECOUNT_INT  ((uint32_t) 0x00000001) /*!< Indicate frame count interrupt */
#define LCD_POWERDOWN_INT   ((uint32_t) 0x00000002) /*!< Indicate power down interrupt */
#define LCD_ALL_INT         ((uint32_t) 0x00000003) /*!< Indicate frame count & power down interrupt */

#define ERR_LCD_CAL_BLINK_FAIL      0xFFFF0000              /*!< Specifies that overflow to calculate the blinking frequency */

/*@}*/ /* end of group NANO100_LCD_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_LCD_EXPORTED_STRUCTS LCD Exported Structs
  @{
*/
typedef enum {
    LCD_C_TYPE = 0,          /*!< Select LCD C-Type */
    LCD_EXTERNAL_R_TYPE = 1, /*!< Select LCD External R-Type */
    LCD_INTERNAL_R_TYPE = 2, /*!< Select LCD Internal R-Type */
    LCD_EXTERNAL_C_TYPE = 3  /*!< Select LCD External C-Type */
} LCD_PanelType;

/*@}*/ /* end of group NANO100_LCD_EXPORTED_STRUCTS */


/** @addtogroup NANO100_LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/

/**
 *    @brief    Get LCD Power Down interrupt flag.
 *
 *    @param    None
 *
 *    @return    LCD Power Down interrupt Flag.
 */
#define LCD_GET_PD_INT_FLAG()        ((LCD->FCSTS & LCD_FCSTS_PDSTS_Msk) >> LCD_FCSTS_PDSTS_Pos)

/**
 *    @brief    Clear LCD Power Down interrupt flag.
 *
 *    @param    None
 *
 *    @return   None.
 */
#define LCD_CLR_PD_INT_FLAG()        (LCD->FCSTS = LCD_FCSTS_PDSTS_Msk)

/**
 *    @brief    Get LCD Frame Count interrupt flag.
 *
 *    @param    None
 *
 *    @return    LCD Frame Count interrupt Flag.
 */
#define LCD_GET_FRAME_CNT_INT_FLAG()  ((LCD->FCSTS & LCD_FCSTS_FCSTS_Msk) >> LCD_FCSTS_FCSTS_Pos)

/**
 *    @brief    Clear LCD Frame Count interrupt flag.
 *
 *    @param    None
 *
 *    @return   None.
 */
#define LCD_CLR_FRAME_CNT_INT_FLAG()   (LCD->FCSTS = LCD_FCSTS_FCSTS_Msk)

/**
 *    @brief    Enable LCD Power Down Display function.
 *
 *    @param    None
 *
 *    @return   None.
 */
#define LCD_ENABLE_PD_DISPLAY()    (LCD->CTL |= LCD_CTL_PDDISP_EN_Msk)

/**
 *    @brief    Disable LCD Power Down Display function.
 *
 *    @param    None
 *
 *    @return   None.
 */
#define LCD_DISABLE_PD_DISPLAY()   (LCD->CTL &= ~LCD_CTL_PDDISP_EN_Msk)

uint32_t LCD_EnableFrameCounter(uint32_t u32Count);
void LCD_DisableFrameCounter(void);
uint32_t LCD_EnableBlink(uint32_t u32ms);
void LCD_DisableBlink(void);
void LCD_EnableInt(uint32_t IntSrc);
void LCD_DisableInt(uint32_t IntSrc);
uint32_t LCD_Open(uint32_t u32DrivingType, uint32_t u32ComNum, uint32_t u32BiasLevel, uint32_t u32FramerateDiv, uint32_t u32DrivingVol);
void LCD_SetPixel(uint32_t u32Com, uint32_t u32Seg, uint32_t u32OnFlag);
void LCD_SetAllPixels(uint32_t u32OnOff);
void LCD_Close(void);

/**
 *  @brief Enable LCD controller
 *
 *  @param None
 *
 *  @return None
 *
 */
static __INLINE void LCD_EnableDisplay(void)
{
    /* Enable LCD */
    LCD->CTL |= LCD_CTL_EN_Msk;
}

/**
 *  @brief Disable LCD controller
 *
 *  @param None
 *
 *  @return None
 *
 */
static __INLINE void LCD_DisableDisplay(void)
{
    /* Enable LCD */
    LCD->CTL &= ~LCD_CTL_EN_Msk;
}



/*@}*/ /* end of group NANO100_LCD_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO100_LCD_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */


#ifdef __cplusplus
}
#endif

#endif  /* __LCD_H__ */



/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/


