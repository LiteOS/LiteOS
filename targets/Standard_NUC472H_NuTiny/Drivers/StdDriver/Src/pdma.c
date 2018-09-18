/**************************************************************************//**
 * @file     pdma.c
 * @version  V1.00
 * $Revision: 8 $
 * $Date: 15/03/16 3:17p $
 * @brief    NUC472/NUC442 PDMA driver source file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "NUC472_442.h"


static uint8_t u32ChSelect[PDMA_CH_MAX];

/** @addtogroup NUC472_442_Device_Driver NUC472/NUC442 Device Driver
  @{
*/

/** @addtogroup NUC472_442_PDMA_Driver PDMA Driver
  @{
*/


/** @addtogroup NUC472_442_PDMA_EXPORTED_FUNCTIONS PDMA Exported Functions
  @{
*/

/**
 * @brief       PDMA Open
 *
 * @param[in]   u32Mask     Channel enable bits.
 *
 * @return      None
 *
 * @details     This function enable the PDMA channels.
 */
void PDMA_Open(uint32_t u32Mask)
{
    int volatile i;

    for (i=0; i<PDMA_CH_MAX; i++) {
        PDMA->DSCT[i].CTL = 0;
        u32ChSelect[i] = 0x1f;
    }

    PDMA->CHCTL |= u32Mask;
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
    PDMA->CHCTL = 0;
}

/**
 * @brief       Set PDMA Transfer Count
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Width        Data width. PDMA_WIDTH_8, PDMA_WIDTH_16, or PDMA_WIDTH_32
 * @param[in]   u32TransCount   Transfer count
 *
 * @return      None
 *
 * @details     This function set the selected channel data width and transfer count.
 */
void PDMA_SetTransferCnt(uint32_t u32Ch, uint32_t u32Width, uint32_t u32TransCount)
{
    PDMA->DSCT[u32Ch].CTL &= ~(PDMA_DSCT_CTL_TXCNT_Msk | PDMA_DSCT_CTL_TXWIDTH_Msk);
    PDMA->DSCT[u32Ch].CTL |= (u32Width | ((u32TransCount-1) << PDMA_DSCT_CTL_TXCNT_Pos));
}

/**
 * @brief       Set PDMA Transfer Address
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32SrcAddr      Source address
 * @param[in]   u32SrcCtrl      Source control attribute. PDMA_SAR_INC or PDMA_SAR_FIX
 * @param[in]   u32DstAddr      destination address
 * @param[in]   u32DstCtrl      destination control attribute. PDMA_DAR_INC or PDMA_DAR_FIX
 *
 * @return      None
 *
 * @details     This function set the selected channel source/destination address and attribute.
 */
void PDMA_SetTransferAddr(uint32_t u32Ch, uint32_t u32SrcAddr, uint32_t u32SrcCtrl, uint32_t u32DstAddr, uint32_t u32DstCtrl)
{
    PDMA->DSCT[u32Ch].ENDSA = u32SrcAddr;
    PDMA->DSCT[u32Ch].ENDDA = u32DstAddr;
    PDMA->DSCT[u32Ch].CTL &= ~(PDMA_DSCT_CTL_SAINC_Msk | PDMA_DSCT_CTL_DAINC_Msk);
    PDMA->DSCT[u32Ch].CTL |= (u32SrcCtrl | u32DstCtrl);
}

/**
 * @brief       Set PDMA Transfer Mode
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Peripheral   The selected peripheral. PDMA_SPI0_TX, PDMA_UART0_TX, PDMA_I2S_TX,...PDMA_MEM
 * @param[in]   u32ScatterEn    Scatter-gather mode enable
 * @param[in]   u32DescAddr     Scatter-gather descriptor address
 *
 * @return      None
 *
 * @details     This function set the selected channel transfer mode. Include peripheral setting.
 */
void PDMA_SetTransferMode(uint32_t u32Ch, uint32_t u32Peripheral, uint32_t u32ScatterEn, uint32_t u32DescAddr)
{
    u32ChSelect[u32Ch] = u32Peripheral;
    switch (u32Ch) {
    case 0:
        PDMA->REQSEL0_3 = (PDMA->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC0_Msk) | u32Peripheral;
        break;
    case 1:
        PDMA->REQSEL0_3 = (PDMA->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC1_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC1_Pos);
        break;
    case 2:
        PDMA->REQSEL0_3 = (PDMA->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC2_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC2_Pos);
        break;
    case 3:
        PDMA->REQSEL0_3 = (PDMA->REQSEL0_3 & ~PDMA_REQSEL0_3_REQSRC3_Msk) | (u32Peripheral << PDMA_REQSEL0_3_REQSRC3_Pos);
        break;
    case 4:
        PDMA->REQSEL4_7 = (PDMA->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC4_Msk) | u32Peripheral;
        break;
    case 5:
        PDMA->REQSEL4_7 = (PDMA->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC5_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC5_Pos);
        break;
    case 6:
        PDMA->REQSEL4_7 = (PDMA->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC6_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC6_Pos);
        break;
    case 7:
        PDMA->REQSEL4_7 = (PDMA->REQSEL4_7 & ~PDMA_REQSEL4_7_REQSRC7_Msk) | (u32Peripheral << PDMA_REQSEL4_7_REQSRC7_Pos);
        break;
    case 8:
        PDMA->REQSEL8_11 = (PDMA->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC8_Msk) | u32Peripheral;
        break;
    case 9:
        PDMA->REQSEL8_11 = (PDMA->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC9_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC9_Pos);
        break;
    case 10:
        PDMA->REQSEL8_11 = (PDMA->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC10_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC10_Pos);
        break;
    case 11:
        PDMA->REQSEL8_11 = (PDMA->REQSEL8_11 & ~PDMA_REQSEL8_11_REQSRC11_Msk) | (u32Peripheral << PDMA_REQSEL8_11_REQSRC11_Pos);
        break;
    case 12:
        PDMA->REQSEL12_15 = (PDMA->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC12_Msk) | u32Peripheral;
        break;
    case 13:
        PDMA->REQSEL12_15 = (PDMA->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC13_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC13_Pos);
        break;
    case 14:
        PDMA->REQSEL12_15 = (PDMA->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC14_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC14_Pos);
        break;
    case 15:
        PDMA->REQSEL12_15 = (PDMA->REQSEL12_15 & ~PDMA_REQSEL12_15_REQSRC15_Msk) | (u32Peripheral << PDMA_REQSEL12_15_REQSRC15_Pos);
        break;
    default:
        ;
    }

    if (u32ScatterEn) {
        PDMA->DSCT[u32Ch].CTL = (PDMA->DSCT[u32Ch].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_SCATTER;
        PDMA->DSCT[u32Ch].NEXT = u32DescAddr - (PDMA->SCATBA);
    } else
        PDMA->DSCT[u32Ch].CTL = (PDMA->DSCT[u32Ch].CTL & ~PDMA_DSCT_CTL_OPMODE_Msk) | PDMA_OP_BASIC;
}

/**
 * @brief       Set PDMA Burst Type
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32BurstType    Burst mode or single mode
 * @param[in]   u32BurstSize    Set the size of burst mode
 *
 * @return      None
 *
 * @details     This function set the selected channel burst type and size.
 */
void PDMA_SetBurstType(uint32_t u32Ch, uint32_t u32BurstType, uint32_t u32BurstSize)
{
    PDMA->DSCT[u32Ch].CTL &= ~(PDMA_DSCT_CTL_TXTYPE_Msk | PDMA_DSCT_CTL_BURSIZE_Msk);
    PDMA->DSCT[u32Ch].CTL |= (u32BurstType | u32BurstSize);
}

/**
 * @brief       Set PDMA TimeOut Count
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
    switch(u32Ch) {
    case 0:
        PDMA->TOC0_1 = (PDMA->TOC0_1 & ~PDMA_TOC0_1_TOC0_Msk) | u32TimeOutCnt;
        break;
    case 1:
        PDMA->TOC0_1 = (PDMA->TOC0_1 & ~PDMA_TOC0_1_TOC1_Msk) | (u32TimeOutCnt << PDMA_TOC0_1_TOC1_Pos);
        break;
    case 2:
        PDMA->TOC2_3 = (PDMA->TOC2_3 & ~PDMA_TOC2_3_TOC2_Msk) | u32TimeOutCnt;
        break;
    case 3:
        PDMA->TOC2_3 = (PDMA->TOC2_3 & ~PDMA_TOC2_3_TOC3_Msk) | (u32TimeOutCnt << PDMA_TOC2_3_TOC3_Pos);
        break;
    case 4:
        PDMA->TOC4_5 = (PDMA->TOC4_5 & ~PDMA_TOC4_5_TOC4_Msk) | u32TimeOutCnt;
        break;
    case 5:
        PDMA->TOC4_5 = (PDMA->TOC4_5 & ~PDMA_TOC4_5_TOC5_Msk) | (u32TimeOutCnt << PDMA_TOC4_5_TOC5_Pos);
        break;
    case 6:
        PDMA->TOC6_7 = (PDMA->TOC6_7 & ~PDMA_TOC6_7_TOC6_Msk) | u32TimeOutCnt;
        break;
    case 7:
        PDMA->TOC6_7 = (PDMA->TOC6_7 & ~PDMA_TOC6_7_TOC7_Msk) | (u32TimeOutCnt << PDMA_TOC6_7_TOC7_Pos);
        break;
    case 8:
        PDMA->TOC8_9 = (PDMA->TOC8_9 & ~PDMA_TOC8_9_TOC8_Msk) | u32TimeOutCnt;
        break;
    case 9:
        PDMA->TOC8_9 = (PDMA->TOC8_9 & ~PDMA_TOC8_9_TOC9_Msk) | (u32TimeOutCnt << PDMA_TOC8_9_TOC9_Pos);
        break;
    case 10:
        PDMA->TOC10_11 = (PDMA->TOC10_11 & ~PDMA_TOC10_11_TOC10_Msk) | u32TimeOutCnt;
        break;
    case 11:
        PDMA->TOC10_11 = (PDMA->TOC10_11 & ~PDMA_TOC10_11_TOC11_Msk) | (u32TimeOutCnt << PDMA_TOC10_11_TOC11_Pos);
        break;
    case 12:
        PDMA->TOC12_13 = (PDMA->TOC12_13 & ~PDMA_TOC12_13_TOC12_Msk) | u32TimeOutCnt;
        break;
    case 13:
        PDMA->TOC12_13 = (PDMA->TOC12_13 & ~PDMA_TOC12_13_TOC13_Msk) | (u32TimeOutCnt << PDMA_TOC12_13_TOC13_Pos);
        break;
    case 14:
        PDMA->TOC14_15 = (PDMA->TOC14_15 & ~PDMA_TOC14_15_TOC14_Msk) | u32TimeOutCnt;
        break;
    case 15:
        PDMA->TOC14_15 = (PDMA->TOC14_15 & ~PDMA_TOC14_15_TOC15_Msk) | (u32TimeOutCnt << PDMA_TOC14_15_TOC15_Pos);
        break;
    default:
        ;
    }
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
    if (u32ChSelect[u32Ch] == PDMA_MEM)
        PDMA->SWREQ = (1 << u32Ch);
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
    PDMA->INTEN |= (1 << u32Ch);
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
    PDMA->INTEN &= ~(1 << u32Ch);
}

/*@}*/ /* end of group NUC472_442_PDMA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NUC472_442_PDMA_Driver */

/*@}*/ /* end of group NUC472_442_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
