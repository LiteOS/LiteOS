/**************************************************************************//**
 * @file     crc.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/09/29 3:50p $
 * @brief    Nano100 series CRC driver source file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"



/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_CRC_Driver CRC Driver
  @{
*/


/** @addtogroup NANO100_CRC_EXPORTED_FUNCTIONS CRC Exported Functions
  @{
*/

/**
 * @brief       CRC Open
 *
 * @param[in]   u32Mode      CRC Polynomial Mode \ref CRC_CCITT, \ref CRC_8, \ref CRC_16, \ref CRC_32
 * @param[in]   u32Attribute Parameter attribute \ref CRC_CHECKSUM_COM, \ref CRC_CHECKSUM_RVS, \ref CRC_WDATA_COM, \ref CRC_WDATA_RVS
 * @param[in]   u32Seed      Seed value
 * @param[in]   u32DataLen   CPU Write Data Length \ref CRC_CPU_WDATA_8, \ref CRC_CPU_WDATA_16, \ref CRC_CPU_WDATA_32
 *
 * @return      None
 *
 * @details     This function enable the CRC channel.
 */
void CRC_Open(uint32_t u32Mode, uint32_t u32Attribute, uint32_t u32Seed, uint32_t u32DataLen)
{
    PDMAGCR->GCRCSR |= DMA_GCR_GCRCSR_CRC_CLK_EN_Msk;
    PDMACRC->SEED = u32Seed;
    PDMACRC->CTL = u32Mode | u32Attribute | u32DataLen | DMA_CRC_CTL_CRCCEN_Msk;
    /* When operated in CPU PIO mode, setting RST bit will reload the initial seed value (CRC_SEED register) */
    PDMACRC->CTL |= DMA_CRC_CTL_CRC_RST_Msk;
}

/**
 * @brief       CRC Start DMA transfer
 *
 * @param[in]   u32SrcAddr      Source address
 * @param[in]   u32ByteCount    Calculate byte count
 *
 * @return      None
 *
 * @details     This function start DMA transfer.
 */
void CRC_StartDMATransfer(uint32_t u32SrcAddr, uint32_t u32ByteCount)
{
    PDMACRC->DMASAR = u32SrcAddr;
    PDMACRC->DMABCR = u32ByteCount;
    PDMACRC->CTL |= DMA_CRC_CTL_TRIG_EN_Msk;
}

/**
 * @brief       Get CRC Checksum
 *
 * @param[in]   None
 *
 * @return      Checksum
 *
 * @details     This macro get the CRC checksum
 */
uint32_t CRC_GetChecksum(void)
{
    switch (PDMACRC->CTL & DMA_CRC_CTL_CRC_MODE_Msk) {
    case CRC_CCITT:
    case CRC_16:
        return (PDMACRC->CHECKSUM & 0xffff);

    case CRC_32:
        return (PDMACRC->CHECKSUM);

    case CRC_8:
        return (PDMACRC->CHECKSUM & 0xff);

    default:
        return 0;
    }
}


/*@}*/ /* end of group NANO100_CRC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_CRC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
