/**************************************************************************//**
 * @file     i2s.c
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 15/06/08 4:58p $
 * @brief    Nano100 series I2S driver header file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include <stdio.h>
#include "Nano100Series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_I2S_Driver I2S Driver
  @{
*/

/** @addtogroup NANO100_I2S_EXPORTED_FUNCTIONS I2S Exported Functions
  @{
*/

/// @cond HIDDEN_SYMBOLS
/**
  * @brief  This function is used to get I2S source clock frequency.
  * @param[in]  i2s is the base address of I2S module.
  * @return I2S source clock frequency (Hz).
  */
static uint32_t I2S_GetSourceClockFreq(I2S_T *i2s)
{
    uint32_t u32Freq, u32ClkSrcSel;

    // get I2S selection clock source
    u32ClkSrcSel = CLK->CLKSEL2 & CLK_CLKSEL2_I2S_S_Msk;

    switch (u32ClkSrcSel) {
    case CLK_CLKSEL2_I2S_S_HXT:
        u32Freq = __HXT;
        break;

    case CLK_CLKSEL2_I2S_S_PLL:
        u32Freq = CLK_GetPLLClockFreq();
        break;

    case CLK_CLKSEL2_I2S_S_HIRC:
        u32Freq = __HIRC;
        break;

    default:
        u32Freq = __HIRC;
        break;
    }

    return u32Freq;
}
/// @endcond /* HIDDEN_SYMBOLS */

/**
  * @brief  This function configures some parameters of I2S interface for general purpose use.
  *         The sample rate may not be used from the parameter, it depends on system's clock settings,
  *         but real sample rate used by system will be returned for reference.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32MasterSlave I2S operation mode. Valid values are:
  *                                     - \ref I2S_MODE_MASTER
  *                                     - \ref I2S_MODE_SLAVE
  * @param[in] u32SampleRate Sample rate
  * @param[in] u32WordWidth Data length. Valid values are:
  *                                     - \ref I2S_DATABIT_8
  *                                     - \ref I2S_DATABIT_16
  *                                     - \ref I2S_DATABIT_24
  *                                     - \ref I2S_DATABIT_32
  * @param[in] u32Channels: Audio format. Valid values are:
  *                                     - \ref I2S_MONO
  *                                     - \ref I2S_STEREO
  * @param[in] u32DataFormat: Data format. Valid values are:
  *                                     - \ref I2S_FORMAT_I2S
  *                                     - \ref I2S_FORMAT_MSB
  * @param[in] u32AudioInterface: Audio interface. Valid values are:
  *                                     - \ref I2S_I2S
  * @return Real sample rate.
  */
uint32_t I2S_Open(I2S_T *i2s, uint32_t u32MasterSlave, uint32_t u32SampleRate, uint32_t u32WordWidth, uint32_t u32Channels, uint32_t u32DataFormat, uint32_t u32AudioInterface)
{
    uint8_t u8Divider;
    uint32_t u32BitRate, u32SrcClk;

    SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_I2S_RST_Msk;
    SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_I2S_RST_Msk;

    i2s->CTRL = u32MasterSlave | u32WordWidth | u32Channels | u32DataFormat | u32AudioInterface | I2S_FIFO_TX_LEVEL_WORD_4 | I2S_FIFO_RX_LEVEL_WORD_4;

    u32SrcClk = I2S_GetSourceClockFreq(i2s);

    u32BitRate = u32SampleRate * (((u32WordWidth>>4) & 0x3) + 1) * 16;
    u8Divider = ((u32SrcClk/u32BitRate) >> 1) - 1;
    i2s->CLKDIV = (i2s->CLKDIV & ~I2S_CLKDIV_BCLK_DIV_Msk) | (u8Divider << 8);

    //calculate real sample rate
    u32BitRate = u32SrcClk / (2*(u8Divider+1));
    u32SampleRate = u32BitRate / ((((u32WordWidth>>4) & 0x3) + 1) * 16);

    i2s->CTRL |= I2S_CTRL_I2SEN_Msk;

    return u32SampleRate;
}

/**
  * @brief  Disable I2S function and I2S clock.
  * @param[in]  i2s is the base address of I2S module.
  * @return none
  */
void I2S_Close(I2S_T *i2s)
{
    i2s->CTRL &= ~I2S_CTRL_I2SEN_Msk;
}

/**
  * @brief This function enables the interrupt according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the combination of all related interrupt enable bits.
  *            Each bit corresponds to a interrupt bit.
  * @return none
  */
void I2S_EnableInt(I2S_T *i2s, uint32_t u32Mask)
{
    i2s->INTEN |= u32Mask;
}

/**
  * @brief This function disables the interrupt according to the mask parameter.
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32Mask is the combination of all related interrupt enable bits.
  *            Each bit corresponds to a interrupt bit.
  * @return none
  */
void I2S_DisableInt(I2S_T *i2s, uint32_t u32Mask)
{
    i2s->INTEN &= ~u32Mask;
}

/**
  * @brief  Enable MCLK .
  * @param[in] i2s is the base address of I2S module.
  * @param[in] u32BusClock is the target MCLK clock
  * @return Actual MCLK clock
  */
uint32_t I2S_EnableMCLK(I2S_T *i2s, uint32_t u32BusClock)
{
    uint8_t u8Divider;
    uint32_t u32SrcClk, u32Reg;

    u32SrcClk = I2S_GetSourceClockFreq(i2s);
    if (u32BusClock == u32SrcClk)
        u8Divider = 0;
    else
        u8Divider = (u32SrcClk/u32BusClock) >> 1;

    i2s->CLKDIV = (i2s->CLKDIV & ~I2S_CLKDIV_MCLK_DIV_Msk) | u8Divider;

    i2s->CTRL |= I2S_CTRL_MCLKEN_Msk;

    u32Reg = i2s->CLKDIV & I2S_CLKDIV_MCLK_DIV_Msk;

    if (u32Reg == 0)
        return u32SrcClk;
    else
        return ((u32SrcClk >> 1) / u32Reg);
}

/**
  * @brief  Disable MCLK .
  * @param[in] i2s is the base address of I2S module.
  * @return none
  */
void I2S_DisableMCLK(I2S_T *i2s)
{
    i2s->CTRL &= ~I2S_CTRL_MCLKEN_Msk;
}

/**
  * @brief  Configure FIFO threshold setting.
  * @param[in]  i2s The pointer of the specified I2S module.
  * @param[in]  u32TxThreshold Decides the TX FIFO threshold. It could be 0 ~ 7.
  * @param[in]  u32RxThreshold Decides the RX FIFO threshold. It could be 0 ~ 7.
  * @return None
  * @details Set TX FIFO threshold and RX FIFO threshold configurations.
  */
void I2S_SetFIFO(I2S_T *i2s, uint32_t u32TxThreshold, uint32_t u32RxThreshold)
{
    i2s->CTRL = (i2s->CTRL & ~(I2S_CTRL_TXTH_Msk | I2S_CTRL_RXTH_Msk) |
                 (u32TxThreshold << I2S_CTRL_TXTH_Pos) |
                 (u32RxThreshold << I2S_CTRL_RXTH_Pos));
}
/*@}*/ /* end of group NANO100_I2S_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_I2S_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
