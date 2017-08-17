/****************************************************************************//**
 * @file     spi.c
 * @version  V0.10
 * $Revision: 7 $
 * $Date: 15/05/28 1:33p $
 * @brief    NANO100 series SPI driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "nano100series.h"

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_SPI_Driver SPI Driver
  @{
*/


/** @addtogroup NANO100_SPI_EXPORTED_FUNCTIONS SPI Exported Functions
  @{
*/

/**
  * @brief  This function make SPI module be ready to transfer.
  *         By default, the SPI transfer sequence is MSB first and
  *         the automatic slave select function is disabled. In
  *         Slave mode, the u32BusClock must be NULL and the SPI clock
  *         divider setting will be 0.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32MasterSlave decides the SPI module is operating in master mode or in slave mode. Valid values are:
  *              - \ref SPI_MASTER
  *              - \ref SPI_SLAVE
  * @param[in]  u32SPIMode decides the transfer timing. Valid values are:
  *              - \ref SPI_MODE_0
  *              - \ref SPI_MODE_1
  *              - \ref SPI_MODE_2
  *              - \ref SPI_MODE_3
  * @param[in]  u32DataWidth decides the data width of a SPI transaction.
  * @param[in]  u32BusClock is the expected frequency of SPI bus clock in Hz.
  * @return Actual frequency of SPI peripheral clock.
  */
uint32_t SPI_Open(SPI_T *spi,
                  uint32_t u32MasterSlave,
                  uint32_t u32SPIMode,
                  uint32_t u32DataWidth,
                  uint32_t u32BusClock)
{
    if(u32DataWidth == 32)
        u32DataWidth = 0;

    spi->CTL = u32MasterSlave | (u32DataWidth << SPI_CTL_TX_BIT_LEN_Pos) | (u32SPIMode);

    return ( SPI_SetBusClock(spi, u32BusClock) );
}

/**
  * @brief Reset SPI module and disable SPI peripheral clock.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_Close(SPI_T *spi)
{
    /* Reset SPI */
    if(spi == SPI0) {
        SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_SPI0_RST_Msk;
        SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_SPI0_RST_Msk;
    } else if(spi == SPI1) {
        SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_SPI1_RST_Msk;
        SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_SPI1_RST_Msk;
    } else {
        SYS->IPRST_CTL2 |= SYS_IPRST_CTL2_SPI2_RST_Msk;
        SYS->IPRST_CTL2 &= ~SYS_IPRST_CTL2_SPI2_RST_Msk;
    }
}

/**
  * @brief Clear Rx FIFO buffer.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_ClearRxFIFO(SPI_T *spi)
{
    spi->FFCTL |= SPI_FFCTL_RX_CLR_Msk;
}

/**
  * @brief Clear Tx FIFO buffer.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_ClearTxFIFO(SPI_T *spi)
{
    spi->FFCTL |= SPI_FFCTL_TX_CLR_Msk;
}

/**
  * @brief Disable the automatic slave select function.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_DisableAutoSS(SPI_T *spi)
{
    spi->SSR &= ~SPI_SSR_AUTOSS_Msk;
}

/**
  * @brief Enable the automatic slave select function. Only available in Master mode.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32SSPinMask specifies slave select pins. (SPI_SS)
  * @param[in]  u32ActiveLevel specifies the active level of slave select signal. Valid values are:
  *              - \ref SPI_SS0_ACTIVE_HIGH
  *              - \ref SPI_SS0_ACTIVE_LOW
  *              - \ref SPI_SS1_ACTIVE_HIGH
  *              - \ref SPI_SS1_ACTIVE_LOW
  * @return none
  */
void SPI_EnableAutoSS(SPI_T *spi, uint32_t u32SSPinMask, uint32_t u32ActiveLevel)
{
    spi->SSR = (spi->SSR & ~(SPI_SSR_SS_LVL_Msk | SPI_SSR_SSR_Msk)) | (u32SSPinMask | u32ActiveLevel) | SPI_SSR_AUTOSS_Msk;
}

/**
  * @brief Set the SPI bus clock. Only available in Master mode.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32BusClock is the expected frequency of SPI bus clock.
  * @return Actual frequency of SPI peripheral clock.
  */
uint32_t SPI_SetBusClock(SPI_T *spi, uint32_t u32BusClock)
{
    uint32_t u32ClkSrc, u32Div = 0;

    if(spi == SPI0) {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI0_S_Msk) == CLK_CLKSEL2_SPI0_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    } else if(spi == SPI1) {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI1_S_Msk) == CLK_CLKSEL2_SPI1_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    } else {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI2_S_Msk) == CLK_CLKSEL2_SPI2_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    }

    if(u32BusClock > u32ClkSrc)
        u32BusClock = u32ClkSrc;

    if(u32BusClock != 0 ) {
        u32Div = (u32ClkSrc / u32BusClock) - 1;
        if(u32Div > SPI_CLKDIV_DIVIDER1_Msk)
            u32Div = SPI_CLKDIV_DIVIDER1_Msk;
    } else
        u32Div = 0;

    spi->CLKDIV = (spi->CLKDIV & ~SPI_CLKDIV_DIVIDER1_Msk) | u32Div;

    return ( u32ClkSrc / (u32Div+1) );
}

/**
  * @brief Enable FIFO mode with user-specified Tx FIFO threshold and Rx FIFO threshold configurations.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32TxThreshold decides the Tx FIFO threshold.
  * @param[in]  u32RxThreshold decides the Rx FIFO threshold.
  * @return none
  */
void SPI_EnableFIFO(SPI_T *spi, uint32_t u32TxThreshold, uint32_t u32RxThreshold)
{
    spi->FFCTL = (spi->FFCTL & ~(SPI_FFCTL_TX_THRESHOLD_Msk | SPI_FFCTL_RX_THRESHOLD_Msk) |
                  (u32TxThreshold << SPI_FFCTL_TX_THRESHOLD_Pos) |
                  (u32RxThreshold << SPI_FFCTL_RX_THRESHOLD_Pos));

    spi->CTL |= SPI_CTL_FIFOM_Msk;
}

/**
  * @brief Disable FIFO mode.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_DisableFIFO(SPI_T *spi)
{
    spi->CTL &= ~SPI_CTL_FIFOM_Msk;
}

/**
  * @brief Get the actual frequency of SPI bus clock. Only available in Master mode.
  * @param[in]  spi is the base address of SPI module.
  * @return Actual SPI bus clock frequency.
  */
uint32_t SPI_GetBusClock(SPI_T *spi)
{
    uint32_t u32Div;
    uint32_t u32ClkSrc;

    if(spi == SPI0) {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI0_S_Msk) == CLK_CLKSEL2_SPI0_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    } else if(spi == SPI1) {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI1_S_Msk) == CLK_CLKSEL2_SPI1_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    } else {
        if((CLK->CLKSEL2 & CLK_CLKSEL2_SPI2_S_Msk) == CLK_CLKSEL2_SPI2_S_HCLK)
            u32ClkSrc = CLK_GetHCLKFreq();
        else
            u32ClkSrc = CLK_GetPLLClockFreq();
    }

    u32Div = spi->CLKDIV & SPI_CLKDIV_DIVIDER1_Msk;
    return (u32ClkSrc / (u32Div + 1));
}

/**
  * @brief Enable FIFO related interrupts specified by u32Mask parameter.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32Mask is the combination of all related interrupt enable bits.
  *         Each bit corresponds to a interrupt bit.
  *         This parameter decides which interrupts will be enabled. Valid values are:
  *           - \ref SPI_IE_MASK
  *           - \ref SPI_SSTA_INTEN_MASK
  *           - \ref SPI_FIFO_TX_INTEN_MASK
  *           - \ref SPI_FIFO_RX_INTEN_MASK
  *           - \ref SPI_FIFO_RXOVR_INTEN_MASK
  *           - \ref SPI_FIFO_TIMEOUT_INTEN_MASK
  * @return none
  */
void SPI_EnableInt(SPI_T *spi, uint32_t u32Mask)
{
    if((u32Mask & SPI_IE_MASK) == SPI_IE_MASK)
        spi->CTL |= SPI_CTL_INTEN_Msk;

    if((u32Mask & SPI_SSTA_INTEN_MASK) == SPI_SSTA_INTEN_MASK)
        spi->SSR |= SPI_SSR_SSTA_INTEN_Msk;

    if((u32Mask & SPI_FIFO_TX_INTEN_MASK) == SPI_FIFO_TX_INTEN_MASK)
        spi->FFCTL |= SPI_FFCTL_TX_INTEN_Msk;

    if((u32Mask & SPI_FIFO_RX_INTEN_MASK) == SPI_FIFO_RX_INTEN_MASK)
        spi->FFCTL |= SPI_FFCTL_RX_INTEN_Msk;

    if((u32Mask & SPI_FIFO_RXOVR_INTEN_MASK) == SPI_FIFO_RXOVR_INTEN_MASK)
        spi->FFCTL |= SPI_FFCTL_RXOVR_INTEN_Msk;

    if((u32Mask & SPI_FIFO_TIMEOUT_INTEN_MASK) == SPI_FIFO_TIMEOUT_INTEN_MASK)
        spi->FFCTL |= SPI_FFCTL_TIMEOUT_EN_Msk;
}

/**
  * @brief Disable FIFO related interrupts specified by u32Mask parameter.
  * @param[in]  spi is the base address of SPI module.
  * @param[in]  u32Mask is the combination of all related interrupt enable bits.
  *         Each bit corresponds to a interrupt bit.
  *         This parameter decides which interrupts will be enabled. Valid values are:
  *           - \ref SPI_IE_MASK
  *           - \ref SPI_SSTA_INTEN_MASK
  *           - \ref SPI_FIFO_TX_INTEN_MASK
  *           - \ref SPI_FIFO_RX_INTEN_MASK
  *           - \ref SPI_FIFO_RXOVR_INTEN_MASK
  *           - \ref SPI_FIFO_TIMEOUT_INTEN_MASK
  * @return none
  */
void SPI_DisableInt(SPI_T *spi, uint32_t u32Mask)
{
    if((u32Mask & SPI_IE_MASK) == SPI_IE_MASK)
        spi->CTL &= ~SPI_CTL_INTEN_Msk;

    if((u32Mask & SPI_SSTA_INTEN_MASK) == SPI_SSTA_INTEN_MASK)
        spi->SSR &= ~SPI_SSR_SSTA_INTEN_Msk;

    if((u32Mask & SPI_FIFO_TX_INTEN_MASK) == SPI_FIFO_TX_INTEN_MASK)
        spi->FFCTL &= ~SPI_FFCTL_TX_INTEN_Msk;

    if((u32Mask & SPI_FIFO_RX_INTEN_MASK) == SPI_FIFO_RX_INTEN_MASK)
        spi->FFCTL &= ~SPI_FFCTL_RX_INTEN_Msk;

    if((u32Mask & SPI_FIFO_RXOVR_INTEN_MASK) == SPI_FIFO_RXOVR_INTEN_MASK)
        spi->FFCTL &= ~SPI_FFCTL_RXOVR_INTEN_Msk;

    if((u32Mask & SPI_FIFO_TIMEOUT_INTEN_MASK) == SPI_FIFO_TIMEOUT_INTEN_MASK)
        spi->FFCTL &= ~SPI_FFCTL_TIMEOUT_EN_Msk;
}

/**
  * @brief Enable wake-up function.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_EnableWakeup(SPI_T *spi)
{
    spi->CTL |= SPI_CTL_WKEUP_EN_Msk;
}

/**
  * @brief Disable wake-up function.
  * @param[in]  spi is the base address of SPI module.
  * @return none
  */
void SPI_DisableWakeup(SPI_T *spi)
{
    spi->CTL &= ~SPI_CTL_WKEUP_EN_Msk;
}

/*@}*/ /* end of group NANO100_SPI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_SPI_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
