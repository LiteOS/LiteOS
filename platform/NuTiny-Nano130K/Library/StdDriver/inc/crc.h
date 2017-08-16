/**************************************************************************//**
 * @file     crc.h
 * @version  V1.00
 * $Revision: 2 $
 * $Date: 15/06/10 4:50p $
 * @brief    Nano100 series CRC driver header file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_CRC_Driver CRC Driver
  @{
*/

/** @addtogroup NANO100_CRC_EXPORTED_CONSTANTS CRC Exported Constants
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  CRC Polynomial Mode Constant Definitions                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define CRC_CCITT           0x00000000UL            /*!<CRC Polynomial Mode - CCITT */
#define CRC_8               0x40000000UL            /*!<CRC Polynomial Mode - CRC8 */
#define CRC_16              0x80000000UL            /*!<CRC Polynomial Mode - CRC16 */
#define CRC_32              0xC0000000UL            /*!<CRC Polynomial Mode - CRC32 */

/*---------------------------------------------------------------------------------------------------------*/
/*  Checksum, Write data Constant Definitions                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define CRC_CHECKSUM_COM    0x08000000UL            /*!<CRC Checksum Complement */
#define CRC_CHECKSUM_RVS    0x02000000UL            /*!<CRC Checksum Reverse */
#define CRC_WDATA_COM       0x04000000UL            /*!<CRC Write Data Complement */
#define CRC_WDATA_RVS       0x01000000UL            /*!<CRC Write Data Reverse */

/*---------------------------------------------------------------------------------------------------------*/
/*  CPU Write Data Length Constant Definitions                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define CRC_CPU_WDATA_8     0x00000000UL            /*!<CRC 8-bit CPU Write Data */
#define CRC_CPU_WDATA_16    0x10000000UL            /*!<CRC 16-bit CPU Write Data */
#define CRC_CPU_WDATA_32    0x20000000UL            /*!<CRC 32-bit CPU Write Data */


/*@}*/ /* end of group NANO100_CRC_EXPORTED_CONSTANTS */

/** @addtogroup NANO100_CRC_EXPORTED_FUNCTIONS CRC Exported Functions
  @{
*/

/**
 * @brief       Enable CRC Interrupt
 *
 * @param[in]   u32Mask     Interrupt mask
 *
 * @return      None
 *
 * @details     This macro enable the interrupts.
 */
#define CRC_ENABLE_INT(u32Mask)   (PDMACRC->DMAIER |= (u32Mask))

/**
 * @brief       Disable CRC Interrupt
 *
 * @param[in]   u32Mask     Interrupt mask
 *
 * @return      None
 *
 * @details     This macro disable the interrupts.
 */
#define CRC_DISABLE_INT(u32Mask)   (PDMACRC->DMAIER &= ~(u32Mask))

/**
 * @brief       Get CRC Interrupt Flag
 *
 * @param[in]   None
 *
 * @return      Interrupt Flag
 *
 * @details     This macro gets the interrupt flag.
 */
#define CRC_GET_INT_FLAG()          ((uint32_t)(PDMACRC->DMAISR))

/**
 * @brief       Clear CRC Interrupt Flag
 *
 * @param[in]   u32Mask     Interrupt mask
 *
 * @return      None
 *
 * @details     This macro clear the interrupt flag.
 */
#define CRC_CLR_INT_FLAG(u32Mask)   (PDMACRC->DMAISR |= (u32Mask))

/**
 * @brief       Set CRC seed value
 *
 * @param[in]   u32Seed     Seed value
 *
 * @return      None
 *
 * @details     This macro set seed value.
 */
#define CRC_SET_SEED(u32Seed)   { PDMACRC->SEED = (u32Seed); PDMACRC->CTL |= DMA_CRC_CTL_CRC_RST_Msk; }

/**
 * @brief       Get CRC Seed value
 *
 * @param[in]   None
 *
 * @return      Seed Value
 *
 * @details     This macro gets the seed value.
 */
#define CRC_GET_SEED()   ((uint32_t)(PDMACRC->SEED))

/**
 * @brief       CRC write data
 *
 * @param[in]   u32Data     write data
 *
 * @return      None
 *
 * @details     This macro write CRC data.
 */
#define CRC_WRITE_DATA(u32Data)   (PDMACRC->WDATA = (u32Data))


/*********************************************************************/
void CRC_Open(uint32_t u32Mode, uint32_t u32Attribute, uint32_t u32Seed, uint32_t u32DataLen);
void CRC_StartDMATransfer(uint32_t u32SrcAddr, uint32_t u32ByteCount);
uint32_t CRC_GetChecksum(void);


/*@}*/ /* end of group NANO100_CRC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_CRC_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__CRC_H__

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
