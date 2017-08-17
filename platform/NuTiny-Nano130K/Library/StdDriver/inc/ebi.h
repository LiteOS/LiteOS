/**************************************************************************//**
 * @file     ebi.h
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 14/09/30 4:21p $
 * @brief    Nano100 Series Flash Memory Controller Driver Header File
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef __EBI_H__
#define __EBI_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_EBI_Driver EBI Driver
  @{
*/


/** @addtogroup NANO100_EBI_EXPORTED_CONSTANTS EBI Exported Constants
  @{
*/


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Base Address                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_BASE_ADDR           0x60000000      /*!< EBI base address            */
#define EBI_MAX_SIZE            0x20000         /*!< Maximum size of EBI bank    */
#define EBI_TIMEOUT_COUNT       0x10000         /*!< Time-out value              */

/* Constants for EBI data bus width */
#define EBI_BUSWIDTH_8BIT       8               /*!< EBI bus width is 8-bit      */
#define EBI_BUSWIDTH_16BIT      16              /*!< EBI bus width is 16-bit     */

/*---------------------------------------------------------------------------------------------------------*/
/*  EBI MCLK divider                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_MCLKDIV_1         0               /*!< MCLK divided by 1           */
#define EBI_MCLKDIV_2         1               /*!< MCLK divided by 2           */
#define EBI_MCLKDIV_4         2               /*!< MCLK divided by 4           */
#define EBI_MCLKDIV_8         3               /*!< MCLK divided by 8           */
#define EBI_MCLKDIV_16        4               /*!< MCLK divided by 16          */
#define EBI_MCLKDIV_32        5               /*!< MCLK divided by 32          */

/*---------------------------------------------------------------------------------------------------------*/
/*  EBI timing setting                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define EBI_TIMING_FASTEST      0x0             /*!< EBI timing is the fastest   */
#define EBI_TIMING_VERYFAST     0x1             /*!< EBI timing is the very fast */
#define EBI_TIMING_FAST         0x2             /*!< EBI timing is the fast      */
#define EBI_TIMING_NORMAL       0x3             /*!< EBI timing is the normal    */
#define EBI_TIMING_SLOW         0x4             /*!< EBI timing is the slow      */
#define EBI_TIMING_VERYSLOW     0x5             /*!< EBI timing is the very slow */
#define EBI_TIMING_SLOWEST      0x6             /*!< EBI timing is the slowest   */


/*@}*/ /* end of group NANO100_EBI_EXPORTED_CONSTANTS */


/** @addtogroup NANO100_EBI_EXPORTED_FUNCTIONS EBI Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  EBI access macros                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/

/**
  * @brief Read one byte data from EBI.
  * @param[in] Addr EBI offset address.
  * @return Byte data read from EBI.
  * \hideinitializer
  */
#define EBI_READ_DATA8(Addr)            *((volatile unsigned char *)(EBI_BASE_ADDR+Addr))

/**
  * @brief Write one byte data to EBI.
  * @param[in] Addr EBI offset address.
  * @param[in] Data Byte data to be written.
  * @return None
  * \hideinitializer
  */
#define EBI_WRITE_DATA8(Addr, Data)     *((volatile unsigned char *)(EBI_BASE_ADDR+Addr))=Data

/**
  * @brief Read a half-word data from EBI.
  * @param[in] Addr EBI offset address.
  * @return Half-word data read from EBI.
  * \hideinitializer
  */
#define EBI_READ_DATA16(Addr)           *((volatile unsigned short *)(EBI_BASE_ADDR+Addr))

/**
  * @brief Write a half-word data to EBI.
  * @param[in] Addr EBI offset address.
  * @param[in] Data Half-word data to be written.
  * @return None
  * \hideinitializer
  */
#define EBI_WRITE_DATA16(Addr, Data)    *((volatile unsigned short *)(EBI_BASE_ADDR+Addr))=Data

/**
  * @brief Read a word data from EBI.
  * @param[in] Addr EBI offset address.
  * @return Word data read from EBI.
  * \hideinitializer
  */
#define EBI_READ_DATA32(Addr)           *((volatile unsigned int *)(EBI_BASE_ADDR+Addr))

/**
  * @brief Write a word data to EBI.
  * @param[in] Addr EBI offset address.
  * @param[in] Data Word data to be written.
  * @return None
  * \hideinitializer
  */
#define EBI_WRITE_DATA32(Addr, Data)    *((volatile unsigned int *)(EBI_BASE_ADDR+Addr))=Data

/*---------------------------------------------------------------------------------------------------------*/
/*  Functions                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

void EBI_Open(uint32_t u32Bank, uint32_t u32DataWidth, uint32_t u32TimingClass, uint32_t u32BusMode, uint32_t u32CSActiveLevel);
void EBI_Close(uint8_t u32Bank);
void EBI_SetBusTiming(uint32_t u32Bank, uint32_t u32TimingConfig, uint32_t u32MclkDiv);


/*@}*/ /* end of group NANO100_EBI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_EBI_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif   // __EBI_H__

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
