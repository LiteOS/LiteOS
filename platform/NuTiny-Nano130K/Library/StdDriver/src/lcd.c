/**************************************************************************//**
 * @file     lcd.c
 * @version  V1.00
 * $Revision: 9 $
 * $Date: 15/07/06 2:08p $
 * @brief    Nano100 series LCD driver header file
 *           The LCD driver can directly drives a LCD glass by creating the ac
 *           segment and common voltage signals automatically. It can support
 *           static, 1/2 duty, 1/3 duty, 1/4 duty, 1/5 duty, 1/6 duty LCD glass with up to 34
 *           segments with 6 COM or 36 segments with 4 COM.
 *
 * @note
 * Copyright (C) 2013~2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Nano100Series.h"



/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_LCD_Driver LCD Driver
  @{
*/


/// @cond HIDDEN_SYMBOLS

/** @addtogroup NANO100_LCD_EXPORTED_VARIABLES LCD Exported Variables
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

double g_LCDFreq;
static uint32_t g_LCDFrameRate; /* src:32768Hz, COM:4, FREQ Div:64, frame-rate 64Hz  */
/* src:10240Hz, COM:4, FREQ Div:32, frame-rate 40Hz  */


/*@}*/ /* end of group NANO100_LCD_EXPORTED_VARIABLES */

/// @endcond /* HIDDEN_SYMBOLS */

/** @addtogroup NANO100_LCD_EXPORTED_FUNCTIONS LCD Exported Functions
  @{
*/

/**
 *  @brief Enables a segment on the LCD display
 *
 *  @param[in] u32Com      COM number
 *  @param[in] u32Seg       Segment number
 *  @param[in] u32OnFlag   1: segment display
 *                     0: segment not display
 *
 *  @return None
 *
 */
void LCD_SetPixel(uint32_t u32Com, uint32_t u32Seg, uint32_t u32OnFlag)
{
    int32_t memnum = u32Seg / 4;
    int32_t seg_shift = 8*(u32Seg-(4*memnum));

    if(u32OnFlag) {
        if(memnum==0) {
            LCD->MEM_0 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==1) {
            LCD->MEM_1 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==2) {
            LCD->MEM_2 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==3) {
            LCD->MEM_3 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==4) {
            LCD->MEM_4 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==5) {
            LCD->MEM_5 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==6) {
            LCD->MEM_6 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==7) {
            LCD->MEM_7 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==8) {
            LCD->MEM_8 |= (1<<u32Com)<<seg_shift;
        } else if(memnum==9) {
            LCD->MEM_9 |= (1<<u32Com)<<seg_shift;
        }
    } else {
        if(memnum==0) {
            LCD->MEM_0 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==1) {
            LCD->MEM_1 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==2) {
            LCD->MEM_2 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==3) {
            LCD->MEM_3 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==4) {
            LCD->MEM_4 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==5) {
            LCD->MEM_5 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==6) {
            LCD->MEM_6 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==7) {
            LCD->MEM_7 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==8) {
            LCD->MEM_8 &= ~((1<<u32Com)<<seg_shift);
        } else if(memnum==9) {
            LCD->MEM_9 &= ~((1<<u32Com)<<seg_shift);
        }

    }

    if(CyclesPerUs > 0)
        SysTick->LOAD = 300 * CyclesPerUs;
    else
        SysTick->LOAD = 15;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}

/**
 *  @brief LCD Enable/Disable all segments
 *
 *  @param[in] u32OnOff  1: Enable all segments
 *                   0: Disable all segment
 *
 *  @return None
 *
 */
void LCD_SetAllPixels(uint32_t u32OnOff)
{
    uint32_t u32SetValue;

    if(u32OnOff) {
        u32SetValue = 0xFFFFFFFF;
    } else {
        u32SetValue = 0x00000000;
    }

    LCD->MEM_0 = u32SetValue;
    LCD->MEM_1 = u32SetValue;
    LCD->MEM_2 = u32SetValue;
    LCD->MEM_3 = u32SetValue;
    LCD->MEM_4 = u32SetValue;
    LCD->MEM_5 = u32SetValue;
    LCD->MEM_6 = u32SetValue;
    LCD->MEM_7 = u32SetValue;
    LCD->MEM_8 = u32SetValue;

    if(CyclesPerUs > 0)
        SysTick->LOAD = 300 * CyclesPerUs;
    else
        SysTick->LOAD = 15;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}


/**
 *  @brief Set Frame Count and Enable frame count
 *
 *  @param[in] u32Count   Frame count value
 *
 *
 *  @return real frame count value.
 *
 */
uint32_t LCD_EnableFrameCounter(uint32_t u32Count)
{
    uint32_t div = 1;   // default prediv == LCD_FCPRESC_DIV1

    LCD->FCR = 0x00;
    LCD->FCSTS |= LCD_FCSTS_FCSTS_Msk;  // clear fcsts flag

    if(u32Count == 0) return 0;

    if(u32Count > 0x3F) { // top value max. 63 = 0x3F
        div = u32Count/64;

        if(div > 3) {
            div = 8;
            LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL_Msk | LCD_FCPRESC_DIV8;
        } else if(div > 1) {
            div = 4;
            LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL_Msk | LCD_FCPRESC_DIV4;
        } else {
            div = 2;
            LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL_Msk | LCD_FCPRESC_DIV2;
        }

        u32Count = (u32Count+(div/2))/div;
    } else {
        div = 1;
        LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL_Msk | LCD_FCPRESC_DIV1;
    }

    LCD->FCR = LCD->FCR & ~LCD_FCR_FCV_Msk | (u32Count << LCD_FCR_FCV_Pos);

    u32Count = u32Count*div;

    LCD->FCR |= LCD_FCR_FCEN_Msk;       // enable LCD frame count

    return u32Count;
}

/**
 *  @brief Disable frame count function
 *
 *  @param None
 *
 *  @return None
 *
 */
void LCD_DisableFrameCounter(void)
{
    LCD->FCR = 0x00;        // disable LCD frame count

    if( LCD->FCSTS & LCD_FCSTS_FCSTS_Msk)   // clear status flag
        LCD->FCSTS = LCD_FCSTS_FCSTS_Msk;
}


/**
 *  @brief LCD Initialization routine.
 *
 *  @param[in]  u32DrivingType  LCD driving type: \ref LCD_C_TYPE / \ref LCD_EXTERNAL_R_TYPE / \ref LCD_INTERNAL_R_TYPE / \ref LCD_EXTERNAL_C_TYPE
 *  @param[in]  u32ComNum  LCD Com number: 1 ~6
 *  @param[in]  u32BiasLevel  LCD bias level: \ref LCD_BIAS_STATIC / \ref LCD_BIAS_HALF / \ref LCD_BIAS_THIRD
 *  @param[in]  u32FramerateDiv  LCD frequency divider: \ref LCD_FREQ_DIV32 / \ref LCD_FREQ_DIV64 / \ref LCD_FREQ_DIV96 / \ref LCD_FREQ_DIV128 /
 *                                                  \ref LCD_FREQ_DIV192/ \ref LCD_FREQ_DIV256 / \ref LCD_FREQ_DIV384 / \ref LCD_FREQ_DIV512
 *  @param[in]  u32DrivingVol  LCD charge pump driving voltage: \ref LCD_CPVOl_2_6V / \ref LCD_CPVOl_2_7V / \ref LCD_CPVOl_2_8V / \ref LCD_CPVOl_2_9V /
 *                                                          \ref LCD_CPVOl_3V / \ref LCD_CPVOl_3_1V / \ref LCD_CPVOl_3_2V / \ref LCD_CPVOl_3_3V
 *
 *  @return LCD frame rate.
 *
 */
uint32_t LCD_Open(uint32_t u32DrivingType, uint32_t u32ComNum, uint32_t u32BiasLevel, uint32_t u32FramerateDiv, uint32_t u32DrivingVol)
{
    uint32_t clkdiv, muldiv;
    uint32_t lcd_freq_div[] = {32, 64, 96, 128, 192, 256, 384, 512};
    uint32_t multiplex_freq_div[] = {2, 4, 6, 8, 10, 12};
    uint32_t u32clk_src;

    /* IP reset */
    SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_LCD_RST_Msk;
    SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_LCD_RST_Msk;

    LCD_DisableDisplay();

    /* Turn all segments off */
    LCD_SetAllPixels(0);


    switch(u32DrivingType) {
    case LCD_C_TYPE:
    case LCD_EXTERNAL_C_TYPE:

        LCD->DISPCTL &= ~LCD_DISPCTL_BV_SEL_Msk;        // internal source for charge pump
        LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_FREQ_Msk | (LCD_CPUMP_DIV1);
        LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_CPUMP_VOL_SET_Msk | (u32DrivingVol);
        LCD->DISPCTL &= ~LCD_DISPCTL_IBRL_EN_Msk;
        LCD->DISPCTL |= LCD_DISPCTL_CPUMP_EN_Msk;       // enable charge pump

        break;

    case LCD_EXTERNAL_R_TYPE:
    case LCD_INTERNAL_R_TYPE:

        LCD->DISPCTL &= ~LCD_DISPCTL_CPUMP_EN_Msk;
        LCD->DISPCTL |= LCD_DISPCTL_BV_SEL_Msk;
        LCD->DISPCTL &= ~LCD_DISPCTL_IBRL_EN_Msk;
        LCD->DISPCTL |= (u32DrivingType == LCD_INTERNAL_R_TYPE)?LCD_DISPCTL_IBRL_EN_Msk:0;
        break;

    };

    LCD->CTL &= ~LCD_CTL_FREQ_Msk;
    LCD->CTL |= u32FramerateDiv;

    LCD->CTL = (LCD->CTL & ~LCD_CTL_MUX_Msk) | ((u32ComNum - 1) << LCD_CTL_MUX_Pos);
    LCD->DISPCTL = LCD->DISPCTL & ~LCD_DISPCTL_BIAS_SEL_Msk | u32BiasLevel;

    if((CLK->CLKSEL1 & CLK_CLKSEL1_LCD_S_Msk) == 0)
        u32clk_src = 32 * 1024;
    else
        u32clk_src = 10 * 1024;

    clkdiv = (LCD->CTL & LCD_CTL_FREQ_Msk) >> LCD_CTL_FREQ_Pos;
    muldiv = (LCD->CTL & LCD_CTL_MUX_Msk) >> LCD_CTL_MUX_Pos;

    g_LCDFreq = (double)u32clk_src / lcd_freq_div[clkdiv];
    g_LCDFrameRate = (uint32_t)g_LCDFreq / multiplex_freq_div[muldiv];

    return g_LCDFrameRate;
}



/**
 *  @brief The function is used to disable LCD controller.
 *
 *  @param None
 *
 *  @return None
 *
 */
void LCD_Close(void)
{
    LCD_DisableDisplay();
}


/**
 *  @brief Enable Blink function in LCD controller
 *
 *  @param[in] u32ms Blinking display time(unit: ms).
 *
 *  @return Real blinking delay time(ms).
 *
 */
uint32_t LCD_EnableBlink(uint32_t u32ms)
{
    uint32_t prescale=LCD_FCPRESC_DIV1, div=1;
    uint32_t framecount;

    if((1000/u32ms) > g_LCDFrameRate) u32ms = (1000/g_LCDFrameRate);

    framecount = (uint32_t) (u32ms / (1000/g_LCDFrameRate)) ;

    if(framecount > 0x3F) {
        for(div=2; div<=8; div*=2) {
            framecount = (uint32_t) (u32ms / (1000/(g_LCDFrameRate/div)) );

            if( framecount <= 0x40 )
                break;
        }
        if(div==2) prescale = LCD_FCPRESC_DIV2;
        else if(div==4) prescale = LCD_FCPRESC_DIV4;
        else if(div==8) prescale = LCD_FCPRESC_DIV8;
        else return 0;
    } else if(framecount == 0) {
        framecount = 1;
    }

    LCD->FCR = LCD->FCR & ~LCD_FCR_PRESCL_Msk | prescale;
    LCD->FCR = LCD->FCR & ~LCD_FCR_FCV_Msk | ((framecount - 1) << LCD_FCR_FCV_Pos);
    LCD->FCR |= LCD_FCR_FCEN_Msk;

    /* Enable Blink LCD */
    LCD->CTL |= LCD_CTL_BLINK_Msk;

    return ( (framecount*1000)/(g_LCDFrameRate/div) );
}


/**
 *  @brief Disable Blink function in LCD controller
 *
 *  @param None
 *
 *  @return None
 *
 */
void LCD_DisableBlink(void)
{
    /* Disable Blink LCD */
    LCD->CTL &= ~LCD_CTL_BLINK_Msk;

    /* Disable frame count */
    LCD->FCR = 0x00;        // disable LCD frame count

    if( LCD->FCSTS & LCD_FCSTS_FCSTS_Msk)   // clear status flag
        LCD->FCSTS = LCD_FCSTS_FCSTS_Msk;

}

/**
 *  @brief This function is used to enable LCD interrupt
 *
 *  @param[in] IntSrc         Interrupt Source: \ref LCD_FRAMECOUNT_INT / \ref LCD_POWERDOWN_INT / \ref LCD_ALL_INT
 *
 *  @return None
 *
 */
void LCD_EnableInt(uint32_t IntSrc)
{
    if((IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT ) {
        LCD->FCR |= LCD_FCR_FCEN_Msk;
    }

    if((IntSrc & LCD_POWERDOWN_INT) == LCD_POWERDOWN_INT ) {
        LCD->CTL |= LCD_CTL_PDINT_EN_Msk;
    }

}

/**
 *  @brief This function is used to disable LCD specified interrupt
 *
 *  @param[in] IntSrc   Interrupt Source: \ref LCD_FRAMECOUNT_INT / \ref LCD_POWERDOWN_INT / \ref LCD_ALL_INT
 *
 *  @return None
 *
 */
void LCD_DisableInt(uint32_t IntSrc)
{
    if((IntSrc & LCD_FRAMECOUNT_INT) == LCD_FRAMECOUNT_INT ) {
        LCD->FCR &= ~LCD_FCR_FCEN_Msk;
        LCD->FCSTS = LCD_FCSTS_FCSTS_Msk;
    }

    if((IntSrc & LCD_POWERDOWN_INT) == LCD_POWERDOWN_INT ) {
        LCD->CTL &= ~LCD_CTL_PDINT_EN_Msk;
        LCD->FCSTS = LCD_FCSTS_PDSTS_Msk;
    }
}

/*@}*/ /* end of group NANO100_LCD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_LCD_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013~2014 Nuvoton Technology Corp. ***/

