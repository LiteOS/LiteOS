/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2015, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

/** \file */

/** \page sdmmc_drv SD/MMC Driver Interface
 *
 *  Implementation for SD/MMC card SD/MMC mode physical layer driver.
 *  Supply a set of SD/MMC card SD/MMC mode's interface.
 *
 *  \section sdmmcd_usage Usage
 *  - Instant SD/MMC struct and uses SD/MMC definitions in application
 *    (see \subpage sdmmc_lib_cmd) for SD/MMC management:
 *   -# \ref sSdCard : Struct that defined by application for SD/MMC management.
 *   -# \ref sdmmc_cardtype : SD/SDIO/MMC card type definitions.
 *   -# \ref sdmmc_rc : Return codes for SD/MMC operation (functions).
 *  - General Card Support: Initialize card with SD_Init() and then you can
 *    read/write on card.
 *    -# SD_Init(): Run the SDcard initialization sequence
 *    -# SD_GetCardType() : Return SD/MMC reported card type.
 *  - SD/MMC Memory Card Operations
 *    -# SD_ReadBlocks() : Read blocks of data
 *    -# SD_WriteBlocks() : Write blocks of data
 *    -# SD_Read() : Read blocks of data with multi-access command
 *                   (Optimized read, see \ref sdmmc_read_op).
 *    -# SD_Write() : Read blocks of data with multi-access command
 *                    (Optimized write, see \ref sdmmc_write_op).
 *    -# SD_GetNumberBlocks() : Return SD/MMC card reported number of blocks.
 *    -# SD_GetBlockSize() : Return SD/MMC card reported block size.
 *    -# SD_GetTotalSizeKB() : Return size of SD/MMC card in KBytes.
 *  - SDIO Card Operations: SD_Init() also detects SDIO card and then SDIO
 *    read/write functions can be used.
 *    -# SDIO_ReadDirect() : Read bytes from registers.
 *    -# SDIO_WriteDirect() : Write one byte to register.
 *    -# SDIO_ReadBytes() : Read 1 ~ 512 bytes from card.
 *    -# SDIO_WriteBytes() : Write 1 ~ 512 bytes to card.
 */
/** \addtogroup libsdmmc_module
 *
 *  \copydoc sdmmc_drv
 *
 *  For more details, see \ref sdmmc_drv.
 *
 *  Related Files:\n
 *  sdmmc.h,\n
 *  sdio.h,\n
 *  sdmmc_cmd.h,\n
 *  sdmmc_hal.h,\n
 *  sdmmc.c.\n
 *
 * \sa \ref sdmmc_lib
 *
 *@{
 *  \defgroup sdmmc_api SD/MMC APIs
 *  @{
 *  Definitions and functions for SD/MMC card access.
 *    \defgroup sdmmc_spec_defines SD/MMC Spec. definitions
 *    \defgroup sdio_api SDIO APIs
 *    \defgroup sdmmc_cmd SD/MMC Lib Commands Definitions
 *  @}
 */

#ifndef _SDMMC_H
#define _SDMMC_H

/*------------------------------------------------------------------------------
 *      Includes
 *----------------------------------------------------------------------------*/
/** \addtogroup sdmmc_spec_defines
 *  @{
 */

#include <stdint.h>
#include "sdmmc_hal.h"
#include "sdio.h"

/*------------------------------------------------------------------------------
 *      Definitions
 *----------------------------------------------------------------------------*/

/** Default block size for SD/MMC card access */
#define SD_BLOCK_SIZE       512

/*----------------------------------------------------------------------------
 *         Macros
 *----------------------------------------------------------------------------*/

/** \addtogroup sdmmc_stream_acc SD/MMC bits/bytes fields access
 *      @{
 */
/** Get bits from a 32-b pointed data (total n bits) */
#define SD_BITS32(pDw, nbits, ibit, bits)    \
	((((uint32_t*)(pDw))[(nbits)/32-(ibit)/32-1] >> ((ibit)%32)) & (bits == 32 ? 0xFFFFFFFF : ((uint32_t)(1ul << (bits)) - 1)))
/** Get bits from a 8-b pointed data (total n bits) */
#define SD_BITS8(pB, nbits, ibit, bits)      \
	((((uint8_t*)(pB))[(nbits)/8-(ibit)/8-1] >> ((ibit)%8)) & ((1 << (bits)) - 1))
/** Get u8 from byte pointed data area */
#define SD_U8(pD, nBytes, iByte)    (((uint8_t*)(pD))[(iByte)])
/** Get u16 from data area */
#define SD_U16(pD, nBytes, iByte)  \
	((((uint8_t*)(pD))[(iByte)] << 0) +\
	  (((uint8_t*)(pD))[(iByte) + 1] << 8))
/**Get u32 from data area */
#define SD_U32(pD, nBytes, iByte)  \
	((((uint8_t*)(pD))[(iByte) ] <<  0) +\
	  (((uint8_t*)(pD))[(iByte) + 1] <<  8) +\
	  (((uint8_t*)(pD))[(iByte) + 2] << 16) +\
	  (((uint8_t*)(pD))[(iByte) + 3] << 24))
/**     @} */

/** \addtogroup sdmmc_ocr_acc SD/MMC OCR register fields (SD 2.0 & MMC 4.3)
 *      @{
 */
#define SD_OCR_VDD_LOW          (1ul <<  7)     /**< SD: Reserved for Low Voltage Range */
#define MMC_OCR_VDD_170_195     (1ul <<  7)     /**< MMC: 1.7 ~ 1.95V, Dual vol and eMMC is 1 */
#define MMC_OCR_VDD_200_260     (0x3Ful << 8)   /**< MMC: 2.0 ~ 2.6 V */
#define SD_OCR_VDD_20_21        (1ul <<  8)
#define SD_OCR_VDD_21_22        (1ul <<  9)
#define SD_OCR_VDD_22_23        (1ul << 10)
#define SD_OCR_VDD_23_24        (1ul << 11)
#define SD_OCR_VDD_24_25        (1ul << 12)
#define SD_OCR_VDD_25_26        (1ul << 13)
#define SD_OCR_VDD_26_27        (1ul << 14)
#define SD_OCR_VDD_27_28        (1ul << 15)
#define SD_OCR_VDD_28_29        (1ul << 16)
#define SD_OCR_VDD_29_30        (1ul << 17)
#define SD_OCR_VDD_30_31        (1ul << 18)
#define SD_OCR_VDD_31_32        (1ul << 19)
#define SD_OCR_VDD_32_33        (1ul << 20)
#define SD_OCR_VDD_33_34        (1ul << 21)
#define SD_OCR_VDD_34_35        (1ul << 22)
#define SD_OCR_VDD_35_36        (1ul << 23)

#define SDIO_OCR_MP             (0x1ul << 27)   /**< SDIO: Memory present */
#define SDIO_OCR_NF             (0x3ul << 28)   /**< SDIO: Number of functions */
#define MMC_OCR_ACCESS_MODE     (0x3ul << 29)   /**< MMC: Access mode, 0x2 is sector mode */
#define MMC_OCR_ACCESS_BYTE     (0x0 << 29)     /**< MMC: Byte access mode */
#define MMC_OCR_ACCESS_SECTOR   (0x2 << 29)     /**< MMC: Sector access mode */
#define SD_OCR_CCS              (1ul << 30)     /**< SD Card Capacity Status (CCS) */
#define SD_OCR_BUSY             (1ul << 31)     /**< SD/MMC Card power up status bit (busy) */

/** We support 2.7 ~ 3.3V cards */
#define SD_HOST_VOLTAGE_RANGE     (SD_OCR_VDD_27_28 +\
								   SD_OCR_VDD_28_29 +\
								   SD_OCR_VDD_29_30 +\
								   SD_OCR_VDD_30_31 +\
								   SD_OCR_VDD_31_32 +\
								   SD_OCR_VDD_32_33 +\
								   SD_OCR_VDD_33_34 +\
								   SD_OCR_VDD_34_35 +\
								   SD_OCR_VDD_35_36)
/**     @}*/

/** \addtogroup sdmmc_cid_acc SD/MMC CID register fields
 *      @{
 */
/** CID register access (128 bits, 16 * 8 bits, 4 * 32 bits) */
#define SD_CID(pCid, field, bits)        SD_BITS32(pCid, 128, field, bits)
#define SD_CID_MID(pCid)    SD_CID(pCid, 120, 8)   /**< Manufacture ID */
#define eMMC_CID_CBX(pCid)  SD_CID(pCid, 112, 2)   /**< eMMC BGA(01)/CARD(00) */
#define SD_CID_OID1(pCid)   SD_CID(pCid, 112, 8)   /**< OEM/App ID Byte 1 */
#define SD_CID_OID0(pCid)   SD_CID(pCid, 104, 8)   /**< OEM/App ID Byte 0 */
#define eMMC_CID_OID(pCid)  SD_CID(pCid, 104, 8)   /**< eMMC OEM/App ID */
#define SD_CID_PNM4(pCid)   SD_CID(pCid,  96, 8)   /**< Product name byte 4 */
#define SD_CID_PNM3(pCid)   SD_CID(pCid,  88, 8)   /**< Product name byte 3 */
#define SD_CID_PNM2(pCid)   SD_CID(pCid,  80, 8)   /**< Product name byte 2 */
#define SD_CID_PNM1(pCid)   SD_CID(pCid,  72, 8)   /**< Product name byte 1 */
#define SD_CID_PNM0(pCid)   SD_CID(pCid,  64, 8)   /**< Product name byte 0 */
#define MMC_CID_PNM5(pCid)  SD_CID(pCid,  96, 8)   /**< Product name byte 5 */
#define MMC_CID_PNM4(pCid)  SD_CID(pCid,  88, 8)   /**< Product name byte 4 */
#define MMC_CID_PNM3(pCid)  SD_CID(pCid,  80, 8)   /**< Product name byte 3 */
#define MMC_CID_PNM2(pCid)  SD_CID(pCid,  72, 8)   /**< Product name byte 2 */
#define MMC_CID_PNM1(pCid)  SD_CID(pCid,  64, 8)   /**< Product name byte 1 */
#define MMC_CID_PNM0(pCid)  SD_CID(pCid,  56, 8)   /**< Product name byte 0 */

#define SD_CID_PRV(pCid)    SD_CID(pCid,  56, 8)   /**< Product Revision */
#define MMC_CID_PRV(pCid)   SD_CID(pCid,  48, 8)   /**< Product Revision */

#define SD_CID_PSN3(pCid)   SD_CID(pCid,  48,  8)  /**< Product serial 3 */
#define SD_CID_PSN2(pCid)   SD_CID(pCid,  40,  8)  /**< Product serial 2 */
#define SD_CID_PSN1(pCid)   SD_CID(pCid,  32,  8)  /**< Product serial 1 */
#define SD_CID_PSN0(pCid)   SD_CID(pCid,  24,  8)  /**< Product serial 0 */
#define MMC_CID_PSN3(pCid)  SD_CID(pCid,  40,  8)  /**< Product serial 3 */
#define MMC_CID_PSN2(pCid)  SD_CID(pCid,  32,  8)  /**< Product serial 2 */
#define MMC_CID_PSN1(pCid)  SD_CID(pCid,  24,  8)  /**< Product serial 1 */
#define MMC_CID_PSN0(pCid)  SD_CID(pCid,  16,  8)  /**< Product serial 0 */

#define SD_CID_MDT_Y(pCid)  SD_CID(pCid,  12, 8)   /**< Manufacturing Year (0=2000) */
#define MMC_CID_MDT_Y(pCid) SD_CID(pCid,  12, 4)   /**< Manufacturing Year (0=1997) */
#define SD_CID_MDT_M(pCid)  SD_CID(pCid,   8, 4)   /**< Manufacturing month */
#define SD_CID_CRC(pCid)    SD_CID(pCid,   1, 7)   /**< CRC7 checksum */
/**     @}*/

/** \addtogroup sdmmc_csd_acc SD/MMC CSD register fields
 *      @{
 */
/** CSD register access macroes (128 bits, 16 * 8 bits, 4 * 32  bits */
#define SD_CSD(pCsd, field, bits)    SD_BITS32(pCsd, 128, field, bits)
#define SD_CSD_STRUCTURE(pCsd)          SD_CSD(pCsd, 126, 2) /**< CSD structure */
#define SD_CSD_STRUCTURE_1_0            0 /**< SD v1.01~1.10, v2.0/Std Capacity */
#define SD_CSD_STRUCTURE_2_0            1 /**< SD v2.0/HC */
#define MMC_CSD_STRUCTURE_1_0           0 /**< MMC v1.0~1.2 */
#define MMC_CSD_STRUCTURE_1_1           1 /**< MMC v1.4~2.2 */
#define MMC_CSD_STRUCTURE_1_2           2 /**< MMC v3.1~3.31(v4.0), v4.1~(>v4.1) */
#define MMC_CSD_SPEC_VERS(pCsd)         SD_CSD(pCsd, 122, 4) /**< System spec version */
#define MMC_CSD_SPEC_VERS_1_0           0 /**< MMC v1.0~1.2 */
#define MMC_CSD_SPEC_VERS_1_4           1 /**< MMC v1.4 */
#define MMC_CSD_SPEC_VERS_2_0           2 /**< MMC v2.0~2.2 */
#define MMC_CSD_SPEC_VERS_3_1           3 /**< MMC v3.1~3.31 */
#define MMC_CSD_SPEC_VERS_4_0           4 /**< MMC v4.0(v4.0), v4.1~(>v4.1) */
#define SD_CSD_TAAC(pCsd)               SD_CSD(pCsd, 112, 8) /**< Data read-access-time-1 */
#define SD_CSD_NSAC(pCsd)               SD_CSD(pCsd, 104, 8) /**< Data read access-time-2 in CLK cycles */
#define SD_CSD_TRAN_SPEED(pCsd)         SD_CSD(pCsd, 96,  8) /**< Max. data transfer rate */
#define SD_CSD_CCC(pCsd)                SD_CSD(pCsd, 84, 12) /**< Card command class */
#define SD_CSD_READ_BL_LEN(pCsd)        SD_CSD(pCsd, 80,  4) /**< Max. read data block length */
#define SD_CSD_READ_BL_PARTIAL(pCsd)    SD_CSD(pCsd, 79,  1) /**< Bartial blocks for read allowed */
#define SD_CSD_WRITE_BLK_MISALIGN(pCsd) SD_CSD(pCsd, 78,  1) /**< Write block misalignment */
#define SD_CSD_READ_BLK_MISALIGN(pCsd)  SD_CSD(pCsd, 77,  1) /**< Read block misalignment */
#define SD_CSD_DSR_IMP(pCsd)            SD_CSD(pCsd, 76,  1) /**< DSP implemented */
#define SD_CSD_C_SIZE(pCsd)             ((SD_CSD(pCsd, 72,  2) << 10) + \
		(SD_CSD(pCsd, 64,  8) << 2)  + \
		SD_CSD(pCsd, 62,  2)) /**< Device size */
#define SD2_CSD_C_SIZE(pCsd)            ((SD_CSD(pCsd, 64,  6) << 16) + \
		(SD_CSD(pCsd, 56,  8) << 8)  + \
		SD_CSD(pCsd, 48,  8)) /**< Device size v2.0 */
#define SD_CSD_VDD_R_CURR_MIN(pCsd)     SD_CSD(pCsd, 59,  3) /**< Max. read current VDD min */
#define SD_CSD_VDD_R_CURR_MAX(pCsd)     SD_CSD(pCsd, 56,  3) /**< Max. read current VDD max */
#define SD_CSD_VDD_W_CURR_MIN(pCsd)     SD_CSD(pCsd, 53,  3) /**< Max. write current VDD min */
#define SD_CSD_VDD_W_CURR_MAX(pCsd)     SD_CSD(pCsd, 50,  3) /**< Max. write current VDD max */
#define SD_CSD_C_SIZE_MULT(pCsd)        SD_CSD(pCsd, 47,  3) /**< Device size multiplier */
#define SD_CSD_ERASE_BLK_EN(pCsd)       SD_CSD(pCsd, 46,  1) /**< Erase single block enable */
#define MMC_CSD_ERASE_BLK_EN(pCsd)      SD_CSD(pCsd, 46,  1) /**< Erase single block enable */
#define MMC_CSD_ERASE_GRP_SIZE(pCsd)    SD_CSD(pCsd, 42,  4) /**< Erase group size */
#define SD_CSD_ERASE_GRP_MULT(pCsd)     SD_CSD(pCsd, 37,  4) /**< Erase group size multiplier */
#define SD_CSD_SECTOR_SIZE(pCsd)       ((SD_CSD(pCsd, 40,  6) << 1) \
										+ SD_CSD(pCsd, 39,  1)) /**< Erase sector size*/
#define SD_CSD_WP_GRP_SIZE(pCsd)        SD_CSD(pCsd, 32,  7) /**< Write protect group size*/
#define SD_CSD_WP_GRP_ENABLE(pCsd)      SD_CSD(pCsd, 31,  1) /**< write protect group enable*/
#define SD_CSD_R2W_FACTOR(pCsd)         SD_CSD(pCsd, 26,  3) /**< Write speed factor*/
#define SD_CSD_WRITE_BL_LEN(pCsd)      ((SD_CSD(pCsd, 24,  2) << 2) \
										+ SD_CSD(pCsd, 22,  2)) /**< Max write block length*/
#define SD_CSD_WRITE_BL_PARTIAL(pCsd)   SD_CSD(pCsd, 21,  1) /**< Partial blocks for write allowed*/
#define SD_CSD_CONTENT_PROT_APP(pCsd)   SD_CSD(pCsd, 16,  1) /**< File format group*/
#define SD_CSD_FILE_FORMAT_GRP(pCsd)    SD_CSD(pCsd, 15,  1) /**< File format group*/
#define SD_CSD_COPY(pCsd)               SD_CSD(pCsd, 14,  1) /**< Copy flag (OTP)*/
#define SD_CSD_PERM_WRITE_PROTECT(pCsd) SD_CSD(pCsd, 13,  1) /**< Permanent write protect*/
#define SD_CSD_TMP_WRITE_PROTECT(pCsd)  SD_CSD(pCsd, 12,  1) /**< Temporary write protection*/
#define SD_CSD_FILE_FORMAT(pCsd)        SD_CSD(pCsd, 10,  2) /**< File format*/
#define MMC_CSD_ECC(pCsd)               SD_CSD(pCsd,  8,  2) /**< ECC */
#define MMC_CSD_ECC_NONE                0 /**< none */
#define MMC_CSD_ECC_BCH                 1 /**< BCH, 3 correctable bits per block */
#define SD_CSD_CRC(pCsd)                SD_CSD(pCsd,  1,  7) /**< CRC*/

#define SD_CSD_MULT(pCsd)               (1 << (SD_CSD_C_SIZE_MULT(pCsd) + 2))
#define SD_CSD_BLOCKNR(pCsd)            ((SD_CSD_C_SIZE(pCsd) + 1) * SD_CSD_MULT(pCsd))
#define SD_CSD_BLOCKNR_HC(pCsd)         ((SD2_CSD_C_SIZE(pCsd) + 1) * 1024)
#define SD_CSD_BLOCK_LEN(pCsd)          (1 << SD_CSD_READ_BL_LEN(pCsd))
#define SD_CSD_TOTAL_SIZE(pCsd)         (SD_CSD_BLOCKNR(pCsd) * SD_CSD_BLOCK_LEN(pCsd))
#define SD_CSD_TOTAL_SIZE_HC(pCsd)      ((SD2_CSD_C_SIZE(pCsd) + 1) * 512* 1024)
/**     @}*/

/** \addtogroup sd_scr_acc SD SCR register fields
 *      @{
 */
/** SCR (Configuration register) access macros (64 bits, 2 * 32 bits, 8 * 8 bits). */
#define SD_SCR(pScr, field, bits)    SD_BITS32(pScr, 64, field, bits)
#define SD_SCR_STRUCTURE(pScr)              SD_SCR(pScr, 60, 4)
#define     SD_SCR_STRUCTURE_1_0            0 /**< SD v1.01~2.00 */
#define SD_SCR_SD_SPEC(pScr)                SD_SCR(pScr, 56, 4)
#define     SD_SCR_SD_SPEC_1_0              0 /**< SD v1.0~1.01 */
#define     SD_SCR_SD_SPEC_1_10             1 /**< SD v1.10 */
#define     SD_SCR_SD_SPEC_2_00             2 /**< SD v2.00 */
#define SD_SCR_DATA_STAT_AFTER_ERASE(pScr)  SD_SCR(pScr, 55, 1)
#define SD_SCR_SD_SECURITY(pScr)            SD_SCR(pScr, 52, 3)
#define     SD_SCR_SD_SECURITY_NO           0 /**< No security */
#define     SD_SCR_SD_SECURITY_NOTUSED      1 /**< Not used */
#define     SD_SCR_SD_SECURITY_1_01         2 /**< Version 1.01 */
#define     SD_SCR_SD_SECURITY_2_00         3 /**< Version 2.00 */
#define SD_SCR_SD_BUS_WIDTHS(pScr)          SD_SCR(pScr, 48, 4)
#define     SD_SCR_SD_BUS_WIDTH_1BITS       (1 << 0) /**< 1 bit (DAT0) */
#define     SD_SCR_SD_BUS_WIDTH_4BITS       (1 << 2) /**< 4 bit (DAT0~3) */
/**     @}*/

/** \addtogroup sd_card_status_acc SD Card Status fields
 *      @{
 */
/** SD/MMC Card STatus bits */
#define SD_CST_OUT_OF_RANGE     (1UL << 31) /**< Arg out of allowed range */
#define SD_CST_ADDRESS_ERROR    (1UL << 30) /**< Misaligned address not match the block length */
#define SD_CST_BLOCK_LEN_ERROR  (1UL << 29) /**< Block length invalid or not match */
#define SD_CST_ERASE_SEQ_ERROR  (1UL << 28) /**< An error in the sequence of erase command */
#define SD_CST_ERASE_PARAM      (1UL << 27) /**< An invalid selection of write-blocks for erase */
#define SD_CST_WP_VIOLATION     (1UL << 26) /**< Write to a protected block or card */
#define SD_CST_CARD_IS_LOCKED   (1UL << 25) /**< Card is locked by the host */
#define SD_CST_LOCK_UNLOCK_FAILED (1UL << 24) /**< sequence or password error in lock/unlock */
#define SD_CST_COM_CRC_ERROR    (1UL << 23) /**< CRC of the command failed */
#define SD_CST_ILLEGAL_COMMAND  (1UL << 22) /**< Command not legal for the card state */
#define SD_CST_CARD_ECC_FAILED  (1UL << 21) /**< Card internal ECC applied but failed to correct */
#define SD_CST_CC_ERROR         (1UL << 20) /**< Internal card controller error */
#define SD_CST_ERROR            (1UL << 19) /**< General or unknow error */
#define MMC_CST_UNDERRUN        (1UL << 18) /**< Card could not sustain data transfer in stream read mode */
#define MMC_CST_OVERRUN         (1UL << 17) /**< Card could not sustain data programming in stream write mode */
#define SD_CST_CSD_OVERWRITE    (1UL << 16) /**< RO section of CSD not match or attempt to reverse the copy or permanent WP bits */
#define SD_CST_WP_ERASE_SKIP    (1UL << 15) /**< Only partial address space was erased */
#define SD_CST_ECC_DISABLED     (1UL << 14) /**< Command executed without using the internal ECC */
#define SD_CST_ERASE_RESET      (1UL << 13) /**< Erase sequence was cleared before executing because an out of erase sequence command was received */
#define SD_CST_CURRENT_STATE    (0xFUL << 9) /**< Card state mask */
#define SD_STATE_IDLE           0
#define SD_STATE_READY          1
#define SD_STATE_IDENT          2
#define SD_STATE_STBY           3
#define SD_STATE_TRAN           4
#define SD_STATE_DATA           5
#define SD_STATE_RCV            6
#define SD_STATE_PRG            7
#define SD_STATE_DIS            8
#define MMC_STATE_BTST          9
#define MMC_STATE_SLP           10
#define SD_CST_STATE_IDLE       (0x0UL << 9)    /**< Card IDLE */
#define SD_CST_STATE_READY      (0x1UL << 9)    /**< Card READY */
#define SD_CST_STATE_IDENT      (0x2UL << 9)    /**< Card Identify state */
#define SD_CST_STATE_STBY       (0x3UL << 9)    /**< Card STBY */
#define SD_CST_STATE_TRAN       (0x4UL << 9)    /**< Card TRAN */
#define SD_CST_STATE_DATA       (0x5UL << 9)    /**< Card DATA */
#define SD_CST_STATE_RCV        (0x6UL << 9)    /**< Card RCV */
#define SD_CST_STATE_PRG        (0x7UL << 9)    /**< Card PRG */
#define SD_CST_STATE_DIS        (0x8UL << 9)    /**< Card DIS */
#define SD_CST_STATE_BTST       (0x9UL << 9)    /**< MMC BTSR */
#define SD_CST_STATE_SLP        (0xAUL << 9)    /**< MMC SLP */
#define SD_CST_READY_FOR_DATA   (1UL << 8)  /**< buffer empty */
#define MMC_CST_SWITCH_ERROR    (1UL << 7)  /**< SWITCH command error */
#define SD_CST_APP_CMD          (1UL << 5)  /**< The card will expect ACMD */
#define SD_CST_AKE_SEQ_ERROR    (1UL << 3)  /**< Error in authentication */
/**     @}*/

/** \addtogroup sdmmc_sd_status SD/MMC status fields
 *      @{
 */
/** SD Status access macros (512 bits, 16 * 32 bits, 64 * 8 bits). */
#define SD_ST(pSt, field, bits)  SD_BITS32(pSt, 512, field, bits)
#define SD_ST_DAT_BUS_WIDTH(pSt)           SD_ST(pSt, 510, 2) /**< Bus width, 00: default, 10:4-bit */
#define     SD_ST_DATA_BUS_WIDTH_1BIT      0x0 /**< 1-bit bus width */
#define     SD_ST_DATA_BUS_WIDTH_4BIT      0x2 /**< 4-bit bus width */
#define SD_ST_SECURED_MODE(pSt)            SD_ST(pSt, 509, 1)  /**< Secured Mode */
#define SD_ST_CARD_TYPE(pSt)            SD_ST(pSt, 480, 16)
#define     SD_ST_CARD_TYPE_RW             0x0000 /**< Regular SD R/W Card */
#define     SD_ST_CARD_TYPE_ROM            0x0001 /**< SD ROM Card */
#define SD_ST_SIZE_OF_PROTECTED_AREA(pSt)  SD_ST(pSt, 448, 32) /**< STD: ThisSize*Multi*BlockLen, HC: Size in bytes */
#define SD_ST_SPEED_CLASS(pSt)             SD_ST(pSt, 440, 8) /** Speed Class, value can be calculated by Pw/2 */
#define     SD_ST_SPEED_CLASS_0            0
#define     SD_ST_SPEED_CLASS_2            1  // >= 2MB/s
#define     SD_ST_SPEED_CLASS_4            2  // >= 4MB/s
#define     SD_ST_SPEED_CLASS_6            3  // >= 6MB/s
#define SD_ST_PERFORMANCE_MOVE(pSt)        SD_ST(pSt, 432, 8) /**< 8-bit, by 1MB/s step. */
#define SD_ST_AU_SIZE(pSt)                 SD_ST(pSt, 428, 4) /**< AU Size, in power of 2 from 16KB */
#define     SD_ST_AU_SIZE_16K              1
#define     SD_ST_AU_SIZE_32K              2
#define     SD_ST_AU_SIZE_64K              3
#define     SD_ST_AU_SIZE_128K             4
#define     SD_ST_AU_SIZE_256K             5
#define     SD_ST_AU_SIZE_512K             6
#define     SD_ST_AU_SIZE_1M               7
#define     SD_ST_AU_SIZE_2M               8
#define     SD_ST_AU_SIZE_4M               9
#define SD_ST_ERASE_SIZE(pSt)              SD_ST(pSt, 408, 16) /**< 16-bit, number of AUs erased. */
#define SD_ST_ERASE_TIMEOUT(pSt)           SD_ST(pSt, 402, 6) /**< Timeout value for erasing areas */
#define SD_ST_ERASE_OFFSET(pSt)            SD_ST(pSt, 400, 2) /**< Fixed offset value added to erase time */
/**     @}*/

/** \addtogroup sd_switch_status SD Switch Status fields
 *      @{
 */
/** SD Switch Status access macros (512 bits, 16 * 32 bits, 64 * 8 bits). */
#define SD_SWITCH_ST(p, field, bits)       SD_BITS32(p, 512, field, bits)
#define SD_SWITCH_ST_MAX_CURRENT_CONSUMPTION(p)   SD_SWITCH_ST(p, 496, 16)
#define SD_SWITCH_ST_FUN_GRP6_INFO(p)             SD_SWITCH_ST(p, 480, 16)
#define SD_SWITCH_ST_FUN_GRP5_INFO(p)             SD_SWITCH_ST(p, 464, 16)
#define SD_SWITCH_ST_FUN_GRP4_INFO(p)             SD_SWITCH_ST(p, 448, 16)
#define SD_SWITCH_ST_FUN_GRP3_INFO(p)             SD_SWITCH_ST(p, 432, 16)
#define SD_SWITCH_ST_FUN_GRP2_INFO(p)             SD_SWITCH_ST(p, 416, 16)
#define SD_SWITCH_ST_FUN_GRP1_INFO(p)             SD_SWITCH_ST(p, 400, 16)
#define SD_SWITCH_ST_FUN_GRP6_RC(p)               SD_SWITCH_ST(p, 396, 4)
#define SD_SWITCH_ST_FUN_GRP5_RC(p)               SD_SWITCH_ST(p, 392, 4)
#define SD_SWITCH_ST_FUN_GRP4_RC(p)               SD_SWITCH_ST(p, 388, 4)
#define SD_SWITCH_ST_FUN_GRP3_RC(p)               SD_SWITCH_ST(p, 384, 4)
#define SD_SWITCH_ST_FUN_GRP2_RC(p)               SD_SWITCH_ST(p, 380, 4)
#define SD_SWITCH_ST_FUN_GRP1_RC(p)               SD_SWITCH_ST(p, 376, 4)
#define     SD_SWITCH_ST_FUN_GRP_RC_ERROR         0xF
#define SD_SWITCH_ST_DATA_STRUCT_VER(p)           SD_SWITCH_ST(p, 368, 8)
#define SD_SWITCH_ST_FUN_GRP6_BUSY(p)             SD_SWITCH_ST(p, 352, 16)
#define SD_SWITCH_ST_FUN_GRP5_BUSY(p)             SD_SWITCH_ST(p, 336, 16)
#define SD_SWITCH_ST_FUN_GRP4_BUSY(p)             SD_SWITCH_ST(p, 320, 16)
#define SD_SWITCH_ST_FUN_GRP3_BUSY(p)             SD_SWITCH_ST(p, 304, 16)
#define SD_SWITCH_ST_FUN_GRP2_BUSY(p)             SD_SWITCH_ST(p, 288, 16)
#define SD_SWITCH_ST_FUN_GRP1_BUSY(p)             SD_SWITCH_ST(p, 272, 16)
#define SD_SWITCH_ST_FUN_GRP_FUN_BUSY(funNdx)     (1 << (funNdx))
/**     @}*/

/** \addtogroup mmc_ext_csd MMC Extended CSD byte fields
 *      @{
 */
/** MMC Extended CSD access macro: get one byte (512 bytes). */
#define MMC_EXT8(p, i)                  SD_U8(p, 512, i)
/** MMC Extended CSD access macro: get one word (512 bytes). */
#define MMC_EXT32(p, i)                 SD_U32(p, 512, i)
#define MMC_EXT_S_CMD_SET_I             504 /**< Supported Command Sets slice */
#define MMC_EXT_S_CMD_SET(p)            MMC_EXT8(p, MMC_EXT_S_CMD_SET_I)
#define MMC_EXT_BOOT_INFO_I             228 /**< Boot information  slice */
#define MMC_EXT_BOOT_INFO(p)            MMC_EXT8(p, MMC_EXT_BOOT_INFO_I)
#define MMC_EXT_BOOT_SIZE_MULTI_I       226 /**< Boot partition size  slice */
#define MMC_EXT_BOOT_SIZE_MULTI(p)      MMC_EXT8(p, MMC_EXT_BOOT_SIZE_MULTI_I)
#define MMC_EXT_ACC_SIZE_I              225 /**< Access size slice */
#define MMC_EXT_ACC_SIZE(p)             MMC_EXT8(p, MMC_EXT_ACC_SIZE_I)
#define MMC_EXT_HC_ERASE_GRP_SIZE_I     224 /**< High-capacity erase time unit size slice */
#define MMC_EXT_HC_ERASE_GRP_SIZE(p)    MMC_EXT8(p, MMC_EXT_HC_ERASE_GRP_SIZE_I)
#define MMC_EXT_ERASE_TIMEOUT_MULT_I    223 /**< High-capacity erase timeout slice */
#define MMC_EXT_ERASE_TIMEOUT_MULT(p)   MMC_EXT8(p, MMC_EXT_ERASE_TIMEOUT_MULT_I)
#define MMC_EXT_REL_WR_SEC_C_I          222 /**< Reliable write sector count slice */
#define MMC_EXT_REL_WR_SEC_C(p)         MMC_EXT8(p, MMC_EXT_REL_WR_SEC_C_I)
#define MMC_EXT_HC_WP_GRP_SIZE_I        221 /**< High-capacity write protect group size slice */
#define MMC_EXT_HC_WP_GRP_SIZE(p)       MMC_EXT8(p, MMC_EXT_HC_WP_GRP_SIZE_I)
#define MMC_EXT_S_C_VCC_I               220 /**< Sleep current (VCC) */
#define MMC_EXT_S_C_VCC(p)              MMC_EXT8(p, MMC_EXT_S_C_VCC_I)
#define MMC_EXT_S_C_VCCQ_I              219 /**< Sleep current (VCC) */
#define MMC_EXT_S_C_VCCQ(p)             MMC_EXT8(p, MMC_EXT_S_C_VCCQ_I)
#define MMC_EXT_S_A_TIMEOUT_I           217 /**< Sleep current (VCCQ) */
#define MMC_EXT_S_A_TIMEOUT(p)          MMC_EXT8(p, MMC_EXT_S_A_TIMEOUT_I)
#define MMC_EXT_SEC_COUNT_I             212 /**< Sector Count slice */
#define MMC_EXT_SEC_COUNT(p)            MMC_EXT32(p, MMC_EXT_SEC_COUNT_I)
#define MMC_EXT_MIN_PERF_W_8_52_I       210 /**< Minimum Write Performance for 8bit @52MHz */
#define MMC_EXT_MIN_PERF_W_8_52(p)      MMC_EXT8(p, MMC_EXT_MIN_PERF_W_8_52_I)
#define MMC_EXT_MIN_PERF_R_8_52_I       209 /**< Minimum Read Performance for 8bit @52MHz */
#define MMC_EXT_MIN_PERF_R_8_52(p)      MMC_EXT8(p, MMC_EXT_MIN_PERF_R_8_52_I)
#define MMC_EXT_MIN_PERF_W_8_26_4_52_I  208 /**< Minimum Write Performance for 8bit @26MHz or 4bit @52MHz */
#define MMC_EXT_MIN_PERF_W_8_26_4_52(p) MMC_EXT8(p, MMC_EXT_MIN_PERF_W_8_26_4_52_I)
#define MMC_EXT_MIN_PERF_R_8_26_4_52_I  207 /**< Minimum Read Performance for 8bit @26MHz or 4bit @52MHz */
#define MMC_EXT_MIN_PERF_R_8_26_4_52(p) MMC_EXT8(p, MMC_EXT_MIN_PERF_R_8_26_4_52_I)
#define MMC_EXT_MIN_PERF_W_4_26_I       206 /**< Minimum Write Performance for 4bit @26MHz */
#define MMC_EXT_MIN_PERF_W_4_26(p)      MMC_EXT8(p, MMC_EXT_MIN_PERF_W_4_26_I)
#define MMC_EXT_MIN_PERF_R_4_26_I       205 /**< Minimum Read Performance for 4bit @26MHz */
#define MMC_EXT_MIN_PERF_R_4_26(p)      MMC_EXT8(p, MMC_EXT_MIN_PERF_R_4_26_I)
#define MMC_EXT_PWR_CL_26_360_I         203 /**< Power Class for 26MHz @ 3.6V */
#define MMC_EXT_PWR_CL_26_360(p)        MMC_EXT8(p, MMC_EXT_PWR_CL_26_360_I)
#define MMC_EXT_PWR_CL_52_360_I         202 /**< Power Class for 52MHz @ 3.6V */
#define MMC_EXT_PWR_CL_52_360(p)        MMC_EXT8(p, MMC_EXT_PWR_CL_52_360_I)
#define MMC_EXT_PWR_CL_26_195_I         201 /**< Power Class for 26MHz @ 1.95V */
#define MMC_EXT_PWR_CL_26_195(p)        MMC_EXT8(p, MMC_EXT_PWR_CL_26_195_I)
#define MMC_EXT_PWR_CL_52_195_I         200 /**< Power Class for 52MHz @ 1.95V */
#define MMC_EXT_PWR_CL_52_195(p)        MMC_EXT8(p, MMC_EXT_PWR_CL_52_195_I)
#define MMC_EXT_CARD_TYPE_I             196 /**< Card Type */
#define MMC_EXT_CARD_TYPE(p)            MMC_EXT8(p, MMC_EXT_CARD_TYPE_I)
#define MMC_EXT_CSD_STRUCTURE_I         194 /**< CSD Structure Version */
#define MMC_EXT_CSD_STRUCTURE(p)        MMC_EXT8(p, MMC_EXT_CSD_STRUCTURE_I)
#define MMC_EXT_EXT_CSD_REV_I           192 /**< Extended CSD Revision */
#define MMC_EXT_EXT_CSD_REV(p)          MMC_EXT8(p, MMC_EXT_EXT_CSD_REV_I)
#define MMC_EXT_CMD_SET_I               191 /**< Command Set */
#define MMC_EXT_CMD_SET(p)              MMC_EXT8(p, MMC_EXT_CMD_SET_I)
#define MMC_EXT_CMD_SET_REV_I           189 /**< Command Set Revision */
#define MMC_EXT_CMD_SET_REV(p)          MMC_EXT8(p, MMC_EXT_CMD_SET_REV_I)
#define MMC_EXT_POWER_CLASS_I           187 /**< Power Class */
#define MMC_EXT_POWER_CLASS(p)          MMC_EXT8(p, MMC_EXT_POWER_CLASS_I)
#define MMC_EXT_HS_TIMING_I             185 /**< High Speed Interface Timing */
#define MMC_EXT_HS_TIMING(p)            MMC_EXT8(p, MMC_EXT_HS_TIMING_I)
#define     MMC_EXT_HS_TIMING_EN        1
#define     MMC_EXT_HS_TIMING_DIS       0
#define MMC_EXT_BUS_WIDTH_I             183 /**< Bus Width Mode */
#define MMC_EXT_BUS_WIDTH(p)            MMC_EXT8(p, MMC_EXT_BUS_WIDTH_I)
#define     MMC_EXT_BUS_WIDTH_1BIT      0
#define     MMC_EXT_BUS_WIDTH_4BITS     1
#define     MMC_EXT_BUS_WIDTH_8BUTS     2
#define MMC_EXT_ERASED_MEM_CONT_I       181 /**< Erased Memory Content */
#define MMC_EXT_ERASED_MEM_CONT(p)      MMC_EXT8(p, MMC_EXT_ERASED_MEM_CONT_I)
#define MMC_EXT_BOOT_CONFIG_I           179 /**< Boot configuration slice */
#define MMC_EXT_BOOT_CONFIG(p)          MMC_EXT8(p, MMC_EXT_BOOT_CONFIG_I)
#define MMC_EXT_BOOT_BUS_WIDTH_I        177 /**< Boot bus width slice */
#define MMC_EXT_BOOT_BUS_WIDTH(p)       MMC_EXT8(p, MMC_EXT_BOOT_BUS_WIDTH_I)
#define MMC_EXT_ERASE_GROUP_DEF_I       175 /**< High-density erase group definition */
#define MMC_EXT_ERASE_GROUP_DEF(p)      MMC_EXT8(p, MMC_EXT_ERASE_GROUP_DEF_I)
/**     @}*/

/** \addtogroup sd_cmd8 SD CMD8 arguments
 *      @{
 */
#define SD_CMD8_VHS             (0xFul << 8)   /**< Voltage supplied */
#define SD_CMD8_VHS_27_36       (0x1ul << 8)   /**< 2.7 ~ 3.6V */
#define SD_CMD8_VHS_LOW_VOL     (0x2ul << 8)   /**< Reserved for Low Voltage Range */
#define SD_CMD8_CHECK_PATTERN   (0xAA  << 0)   /**< Recommended check pattern */
/**     @}*/
/**   @}*/

/*----------------------------------------------------------------------------
 *      Types
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Functions
 *----------------------------------------------------------------------------*/
/** \addtogroup sdmmc_api
        @{*/

extern uint8_t SD_Init(sSdCard *pSd);
extern void SD_DeInit(sSdCard *pSd);

extern uint8_t SD_SetupBusMode(sSdCard *pSd, uint8_t bMode);
extern uint8_t SD_SetupHSMode(sSdCard *pSd, uint8_t bMode);

extern uint8_t  SD_GetCardType(sSdCard *pSd);
extern uint32_t SD_GetNumberBlocks(sSdCard *pSd);
extern uint32_t SD_GetBlockSize(sSdCard *pSd);
extern uint32_t SD_GetTotalSizeKB(sSdCard *pSd);

extern uint8_t SD_ReadBlocks(sSdCard  *pSd,
							 uint32_t dwAddr,
							 void    *pData,
							 uint32_t dwNbBlocks);
extern uint8_t SD_WriteBlocks(sSdCard  *pSd,
							  uint32_t dwAddr,
							  const void *pData,
							  uint32_t dwNbBlocks);

extern uint8_t SD_Read(sSdCard *pSd,
					   uint32_t dwAddr,
					   void    *pData,
					   uint32_t dwNbBlocks,
					   fSdmmcCallback fCallback,
					   void *pArg);
extern uint8_t SD_Write(sSdCard *pSd,
						uint32_t dwAddr,
						const void *pData,
						uint32_t dwNbBlocks,
						fSdmmcCallback fCallback,
						void *pArg);

extern uint8_t SDIO_ReadDirect(
	sSdCard *pSd,
	uint8_t bFunctionNum,
	uint32_t dwAddress,
	uint8_t *pData,
	uint32_t dwSize);

extern uint8_t SDIO_WriteDirect(
	sSdCard *pSd,
	uint8_t bFunctionNum,
	uint32_t dwAddress,
	uint8_t bDataByte);

extern uint8_t SDIO_ReadBytes(
	sSdCard *pSd,
	uint8_t bFunctionNum,
	uint32_t dwAddress,
	uint8_t bIsFixedAddress,
	uint8_t *pData,
	uint16_t wSize,
	fSdmmcCallback fCallback,
	void *pArg);

extern uint8_t SDIO_WriteBytes(
	sSdCard *pSd,
	uint8_t bFunctionNum,
	uint32_t dwAddress,
	uint8_t bIsFixedAddress,
	uint8_t *pData,
	uint16_t wSize,
	fSdmmcCallback fCallback,
	void *pArg);


extern void _DumpREG(void *pREG, uint32_t dwSize);

extern void SDIO_DumpCardInformation(sSdCard *pSd);

extern void SD_DumpCID(void *pCID);

extern void SD_DumpCSD(void *pCSD);

extern void SD_DumpExtCSD(void *pExtCSD);

extern void SD_DumpSCR(void *pSCR);

extern void SD_DumpSdStatus(void *pSdST);

/**     @}*/
/** @}*/
/**@}*/
#endif /* #define _SDMMC_H */

