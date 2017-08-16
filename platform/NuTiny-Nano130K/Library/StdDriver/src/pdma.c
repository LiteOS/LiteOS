/**************************************************************************//**
 * @file     pdma.c
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 14/09/29 3:50p $
 * @brief    Nano100 series PDMA driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"



/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_PDMA_Driver PDMA Driver
  @{
*/


/** @addtogroup NANO100_PDMA_EXPORTED_FUNCTIONS PDMA Exported Functions
  @{
*/

/**
 * @brief       PDMA Open
 *
 * @param[in]   u32Mask     Channel enable bits
 *
 * @return      None
 *
 * @details     This function enable the PDMA channels.
 */
void PDMA_Open(uint32_t u32Mask)
{
    PDMAGCR->GCRCSR |= (u32Mask << 8);
}

/**
 * @brief       PDMA Close
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function disable all PDMA channels.
 */
void PDMA_Close(void)
{
    PDMAGCR->GCRCSR = 0;
}

/**
 * @brief       Set PDMA Transfer Count
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Width        Data width. \ref PDMA_WIDTH_8, \ref PDMA_WIDTH_16, or \ref PDMA_WIDTH_32
 * @param[in]   u32TransCount   Transfer count
 *
 * @return      None
 *
 * @details     This function set the selected channel data width and transfer count.
 */
void PDMA_SetTransferCnt(uint32_t u32Ch, uint32_t u32Width, uint32_t u32TransCount)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));
    pdma->CSR = (pdma->CSR & ~PDMA_CSR_APB_TWS_Msk) | u32Width;
    switch (u32Width) {
    case PDMA_WIDTH_32:
        pdma->BCR = (u32TransCount << 2);
        break;

    case PDMA_WIDTH_8:
        pdma->BCR = u32TransCount;
        break;

    case PDMA_WIDTH_16:
        pdma->BCR = (u32TransCount << 1);
        break;

    default:
        ;
    }
}

/**
 * @brief       Set PDMA Transfer Address
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32SrcAddr      Source address
 * @param[in]   u32SrcCtrl      Source control attribute. \ref PDMA_SAR_INC, \ref PDMA_SAR_FIX, or \ref PDMA_SAR_WRA
 * @param[in]   u32DstAddr      destination address
 * @param[in]   u32DstCtrl      destination control attribute. \ref PDMA_DAR_INC, \ref PDMA_DAR_FIX, or \ref PDMA_DAR_WRA
 *
 * @return      None
 *
 * @details     This function set the selected channel source/destination address and attribute.
 */
void PDMA_SetTransferAddr(uint32_t u32Ch, uint32_t u32SrcAddr, uint32_t u32SrcCtrl, uint32_t u32DstAddr, uint32_t u32DstCtrl)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->SAR = u32SrcAddr;
    pdma->DAR = u32DstAddr;
    pdma->CSR = (pdma->CSR & ~(PDMA_CSR_SAD_SEL_Msk|PDMA_CSR_DAD_SEL_Msk)) | (u32SrcCtrl | u32DstCtrl);
}

/**
 * @brief       Set PDMA Transfer Mode
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Peripheral   The selected peripheral.
 *              \ref PDMA_SPI0_TX,  \ref PDMA_SPI1_TX,  \ref PDMA_UART0_TX, \ref PDMA_UART1_TX, \ref PDMA_USB_TX,
 *              \ref PDMA_I2S_TX,   \ref PDMA_DAC0_TX,  \ref PDMA_DAC1_TX,  \ref PDMA_SPI2_TX,  \ref PDMA_TMR0,
 *              \ref PDMA_TMR1,     \ref PDMA_TMR2,     \ref PDMA_TMR3,     \ref PDMA_SPI0_RX,  \ref PDMA_SPI1_RX,
 *              \ref PDMA_UART0_RX, \ref PDMA_UART1_RX, \ref PDMA_USB_RX,   \ref PDMA_I2S_RX,   \ref PDMA_ADC,
 *              \ref PDMA_SPI2_RX,  \ref PDMA_PWM0_CH0, \ref PDMA_PWM0_CH2, \ref PDMA_PWM1_CH0, \ref PDMA_PWM1_CH2,
 *              \ref PDMA_MEM
 * @param[in]   u32ScatterEn    Scatter-gather mode enable
 * @param[in]   u32DescAddr     Scatter-gather descriptor address
 *
 * @return      None
 *
 * @details     This function set the selected channel transfer mode. Include peripheral setting.
 */
void PDMA_SetTransferMode(uint32_t u32Ch, uint32_t u32Peripheral, uint32_t u32ScatterEn, uint32_t u32DescAddr)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    switch (u32Ch) {
    case 1:
        PDMAGCR->DSSR0 = (PDMAGCR->DSSR0 & ~DMA_GCR_DSSR0_CH1_SEL_Msk) | (u32Peripheral << DMA_GCR_DSSR0_CH1_SEL_Pos);
        break;
    case 2:
        PDMAGCR->DSSR0 = (PDMAGCR->DSSR0 & ~DMA_GCR_DSSR0_CH2_SEL_Msk) | (u32Peripheral << DMA_GCR_DSSR0_CH2_SEL_Pos);
        break;
    case 3:
        PDMAGCR->DSSR0 = (PDMAGCR->DSSR0 & ~DMA_GCR_DSSR0_CH3_SEL_Msk) | (u32Peripheral << DMA_GCR_DSSR0_CH3_SEL_Pos);
        break;
    case 4:
        PDMAGCR->DSSR1 = (PDMAGCR->DSSR1 & ~DMA_GCR_DSSR1_CH4_SEL_Msk) | u32Peripheral;
        break;
    default:
        ;
    }
    if (u32Peripheral == PDMA_MEM)
        pdma->CSR &= ~PDMA_CSR_MODE_SEL_Msk;
    else if (u32Peripheral & 0x10)
        pdma->CSR = (pdma->CSR & ~PDMA_CSR_MODE_SEL_Msk) | 0x4; /* IP to memory */
    else
        pdma->CSR = (pdma->CSR & ~PDMA_CSR_MODE_SEL_Msk) | 0x8; /* memory to IP */
}

/**
 * @brief       Set PDMA Timeout
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32OnOff        Enable/disable time out function
 * @param[in]   u32TimeOutCnt   Timeout count
 *
 * @return      None
 *
 * @details     This function set the timeout count.
 */
void PDMA_SetTimeOut(uint32_t u32Ch, uint32_t u32OnOff, uint32_t u32TimeOutCnt)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->TCR = (pdma->TCR & ~PDMA_TCR_PDMA_TCR_Msk) | u32TimeOutCnt;
    pdma->CSR = (pdma->CSR & ~PDMA_CSR_TO_EN_Msk) | (u32OnOff << PDMA_CSR_TO_EN_Pos);

}

/**
 * @brief       Trigger PDMA
 *
 * @param[in]   u32Ch           The selected channel
 *
 * @return      None
 *
 * @details     This function trigger the selected channel.
 */
void PDMA_Trigger(uint32_t u32Ch)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
}

/**
 * @brief       Enable Interrupt
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Mask         The Interrupt Type
 *
 * @return      None
 *
 * @details     This function enable the selected channel interrupt.
 */
void PDMA_EnableInt(uint32_t u32Ch, uint32_t u32Mask)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->IER |= u32Mask;
}

/**
 * @brief       Disable Interrupt
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Mask         The Interrupt Type
 *
 * @return      None
 *
 * @details     This function disable the selected channel interrupt.
 */
void PDMA_DisableInt(uint32_t u32Ch, uint32_t u32Mask)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->IER &= ~u32Mask;
}


/*@}*/ /* end of group NANO100_PDMA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_PDMA_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
