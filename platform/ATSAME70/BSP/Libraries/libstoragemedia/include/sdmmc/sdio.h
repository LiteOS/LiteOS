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

#ifndef _SDIO_H
#define _SDIO_H

/** \addtogroup sdio_api
 *  @{
 */

/*----------------------------------------------------------------------------
 *         Constants
 *----------------------------------------------------------------------------*/

/** \addtogroup sdio_status SDIO Status bits
 *      @{
 */
#define SDIO_R1_IDLE            (1ul <<  0) /**< in idle state */
#define SDIO_R1_ILLEGAL_COMMAND (1ul <<  2) /**< illegal command */
#define SDIO_R1_COM_CRC_ERROR   (1ul <<  3) /**< COM CRC error */
#define SDIO_R1_FUNCN_ERROR     (1ul <<  4) /**< Function number error */
#define SDIO_R1_PARAM_ERROR     (1ul <<  6) /**< Parameter error */

#define SDIO_R6_COM_CRC_ERROR   (1ul << 15) /**< CRC check of command fails */
#define SDIO_R6_ILLEGAL_COMMAND (1ul << 14) /**< Command not legal for the state */
#define SDIO_R6_ERROR           (1ul << 13) /**< General or unknown error */

#define SDIO_R5_Pos             (8)         /**< R5 starting position */
#define SDIO_R5_COM_CRC_ERROR   (1ul << 15)
#define SDIO_R5_ILLEGAL_COMMAND (1ul << 14)
#define SDIO_R5_IO_STATE        (3ul << 12) /**< DIS/CMD/TRN/RFU */
#define     SDIO_R5_STATE_DIS   (0ul << 12)
#define     SDIO_R5_STATE_CMD   (1ul << 12)
#define     SDIO_R5_STATE_TRN   (2ul << 12)
#define     SDIO_R5_STATE_RFU   (3ul << 12)
#define SDIO_R5_ERROR           (1ul << 11)
#define SDIO_R5_FUNCN_ERROR     (1ul <<  9)
#define SDIO_R5_OUT_OF_RANGE    (1ul <<  8)

#define SDIO_R4_OCR             (0xF <<  0) /**< OCR */
#define SDIO_R4_MP              (1ul << 27) /**< Memory Present */
#define SDIO_R4_NF              (3ul << 28) /**< Number of Functions */
/**     @}*/


/** \addtogroup sdio_fun_def SDIO Functions
 *  Here lists SDIO functions definitions
 *  - \ref SDIO_CIA or \ref SDIO_FN0
 *  - \ref SDIO_FN1
 *  - \ref SDIO_FN2
 *  - \ref SDIO_FN3
 *  - \ref SDIO_FN4
 *  - \ref SDIO_FN5
 *  - \ref SDIO_FN6
 *  - \ref SDIO_FN7
 *      @{*/
#define SDIO_CIA            0   /**< SDIO Function 0 (CIA) */
#define SDIO_FN0            0   /**< SDIO Function 0 */
#define SDIO_FN1            1   /**< SDIO Function 1 */
#define SDIO_FN2            2   /**< SDIO Function 2 */
#define SDIO_FN3            3   /**< SDIO Function 3 */
#define SDIO_FN4            4   /**< SDIO Function 4 */
#define SDIO_FN5            5   /**< SDIO Function 5 */
#define SDIO_FN6            6   /**< SDIO Function 6 */
#define SDIO_FN7            7   /**< SDIO Function 7 */
/**     @}*/

/** \addtogroup sdio_cccr_def SDIO Card Common Control Registers (CCCR)
 *  Here lists SDIO CCCR definitions
 *  -# \ref SDIO_CCCR_REG
 *  -# \ref SDIO_SD_REV_REG
 *  -# \ref SDIO_IOE_REG
 *  -# \ref SDIO_IOR_REG
 *  -# \ref SDIO_IEN_REG
 *  -# \ref SDIO_INT_REG
 *  -# \ref SDIO_IOA_REG
 *  -# \ref SDIO_BUS_CTRL_REG
 *  -# \ref SDIO_CAP_REG
 *  -# \ref SDIO_CIS_PTR_REG
 *  -# .
 *  -# .
 *  -# \ref SDIO_BUS_SUSP_REG
 *  -# \ref SDIO_FUN_SEL_REG
 *  -# \ref SDIO_EXEC_REG
 *  -# \ref SDIO_READY_REG
 *  -# \ref SDIO_FN0_BLKSIZ_REG
 *  -# .
 *  -# \ref SDIO_POWER_REG
 *  -# \ref SDIO_HS_REG
 *      @{*/
#define SDIO_CCCR_REG       0x00    /**< CCCR/SDIO revision (RO) */
#define  SDIO_CCCR          (0xFUL << 0)/**< CCCR Format Version number */
#define  SDIO_CCCR_1_00     (0x0UL << 0)/**< CCCR/FBR Version 1.00 */
#define  SDIO_CCCR_1_10     (0x1UL << 0)/**< CCCR/FBR Version 1.10 */
#define  SDIO_CCCR_1_20     (0x2UL << 0)/**< CCCR/FBR Version 1.20 */
#define  SDIO_SDIO          (0xFUL << 4)/**< SDIO Specification */
#define  SDIO_SDIO_1_00     (0x0UL << 4)/**< SDIO Specification 1.00 */
#define  SDIO_SDIO_1_10     (0x1UL << 4)/**< SDIO Specification 1.10 */
#define  SDIO_SDIO_1_20     (0x2UL << 4)/**< SDIO Specification 1.20(unreleased) */
#define  SDIO_SDIO_2_00     (0x3UL << 4)/**< SDIO Specification Version 2.00 */
#define SDIO_SD_REV_REG     0x01    /**< SD Specification Revision (RO) */
#define  SDIO_SD            (0xFUL << 0)/**< SD Physical Specification */
#define  SDIO_SD_1_01       (0x0UL << 0)/**< SD 1.01 (Mar 2000) */
#define  SDIO_SD_1_10       (0x1UL << 0)/**< SD 1.10 (Oct 2004) */
#define  SDIO_SD_2_00       (0x2UL << 0)/**< SD 2.00 (May 2006) */
#define SDIO_IOE_REG        0x02    /**< I/O Enable (R/W) */
#define  SDIO_IOE           0xFEUL      /**< Enable/Disable Function */
#define  SDIO_IOE_FN1       (0x1UL << 1)/**< Function 1 Enable/Disable */
#define  SDIO_IOE_FN2       (0x1UL << 2)/**< Function 2 Enable/Disable */
#define  SDIO_IOE_FN3       (0x1UL << 3)/**< Function 3 Enable/Disable */
#define  SDIO_IOE_FN4       (0x1UL << 4)/**< Function 4 Enable/Disable */
#define  SDIO_IOE_FN5       (0x1UL << 5)/**< Function 5 Enable/Disable */
#define  SDIO_IOE_FN6       (0x1UL << 6)/**< Function 6 Enable/Disable */
#define  SDIO_IOE_FN7       (0x1UL << 7)/**< Function 7 Enable/Disable */
#define SDIO_IOR_REG        0x03    /**< I/O Ready (RO) */
#define  SDIO_IOR           0xFEUL      /**< I/O Function Ready */
#define  SDIO_IOR_FN1       (0x1UL << 1)/**< Function 1 ready */
#define  SDIO_IOR_FN2       (0x1UL << 2)/**< Function 2 ready */
#define  SDIO_IOR_FN3       (0x1UL << 3)/**< Function 3 ready */
#define  SDIO_IOR_FN4       (0x1UL << 4)/**< Function 4 ready */
#define  SDIO_IOR_FN5       (0x1UL << 5)/**< Function 5 ready */
#define  SDIO_IOR_FN6       (0x1UL << 6)/**< Function 6 ready */
#define  SDIO_IOR_FN7       (0x1UL << 7)/**< Function 7 ready */
#define SDIO_IEN_REG        0x04    /**< Int Enable */
#define  SDIO_IENM          0x01UL      /**< Int Enable Master (R/W) */
#define  SDIO_IEN           0xFEUL      /**< Int Enable for function (R/W) */
#define  SDIO_IEN_FN1       (0x1UL << 1)/**< Function 1 Int Enable */
#define  SDIO_IEN_FN2       (0x1UL << 2)/**< Function 2 Int Enable */
#define  SDIO_IEN_FN3       (0x1UL << 3)/**< Function 3 Int Enable */
#define  SDIO_IEN_FN4       (0x1UL << 4)/**< Function 4 Int Enable */
#define  SDIO_IEN_FN5       (0x1UL << 5)/**< Function 5 Int Enable */
#define  SDIO_IEN_FN6       (0x1UL << 6)/**< Function 6 Int Enable */
#define  SDIO_IEN_FN7       (0x1UL << 7)/**< Function 7 Int Enable */
#define SDIO_INT_REG        0x05    /**< Int Pending */
#define  SDIO_INT           0xFE      /**< Int Pending for functions (RO) */
#define  SDIO_INT_FN1       (0x1UL << 1)/**< Function 1 Int pending */
#define  SDIO_INT_FN2       (0x1UL << 2)/**< Function 2 Int pending */
#define  SDIO_INT_FN3       (0x1UL << 3)/**< Function 3 Int pending */
#define  SDIO_INT_FN4       (0x1UL << 4)/**< Function 4 Int pending */
#define  SDIO_INT_FN5       (0x1UL << 5)/**< Function 5 Int pending */
#define  SDIO_INT_FN6       (0x1UL << 6)/**< Function 6 Int pending */
#define  SDIO_INT_FN7       (0x1UL << 7)/**< Function 7 Int pending */
#define SDIO_IOA_REG        0x06    /**< I/O Abort */
#define  SDIO_AS            (0x7UL << 0)/**< Abort Select In Order (WO) */
#define  SDIO_AS_FN1        (0x1UL << 0)/**< Abort function 1 IO */
#define  SDIO_AS_FN2        (0x2UL << 0)/**< Abort function 2 IO */
#define  SDIO_AS_FN3        (0x3UL << 0)/**< Abort function 3 IO */
#define  SDIO_AS_FN4        (0x4UL << 0)/**< Abort function 4 IO */
#define  SDIO_AS_FN5        (0x5UL << 0)/**< Abort function 5 IO */
#define  SDIO_AS_FN6        (0x6UL << 0)/**< Abort function 6 IO */
#define  SDIO_AS_FN7        (0x7UL << 0)/**< Abort function 7 IO */
#define  SDIO_RES           (0x1UL << 3)/**< IO CARD RESET (WO) */
#define SDIO_BUS_CTRL_REG   0x07    /**< Bus Interface Control */
#define  SDIO_BUSWIDTH      (0x3UL << 0)/**< Data bus width (R/W) */
#define  SDIO_BUSWIDTH_1B   (0x0UL << 0)/**< 1-bit data bus */
#define  SDIO_BUSWIDTH_4B   (0x2UL << 0)/**< 4-bit data bus */
#define  SDIO_ECSI          (0x1UL << 5)/**< Enable Continuous SPI interrupt (R/W) */
#define  SDIO_SCSI          (0x1UL << 6)/**< Support Continuous SPI interrupt (RO) */
#define  SDIO_CD            (0x1UL << 7)/**< Connect(0)/Disconnect(1) pull-up on CD/DAT[3] (R/W) */
#define SDIO_CAP_REG        0x08    /**< Card Capability */
#define  SDIO_SDC           (0x1UL << 0)/**< Support Direct Commands during data transfer (RO) */
#define  SDIO_SMB           (0x1UL << 1)/**< Support Multi-Block (RO) */
#define  SDIO_SRW           (0x1UL << 2)/**< Support Read Wait (RO) */
#define  SDIO_SBS           (0x1UL << 3)/**< Support Suspend/Resume (RO) */
#define  SDIO_S4MI          (0x1UL << 4)/**< Support interrupt between blocks of data in 4-bit SD mode (RO) */
#define  SDIO_E4MI          (0x1UL << 5)/**< Enable interrupt between blocks of data in 4-bit SD mode (R/W) */
#define  SDIO_LSC           (0x1UL << 6)/**< Low-Speed Card (RO) */
#define  SDIO_4BLS          (0x1UL << 7)/**< 4-bit support for Low-Speed Card (RO) */
#define SDIO_CIS_PTR_REG    0x09    /**< Pointer to CIS (3B, LSB first) */
#define SDIO_BUS_SUSP_REG   0x0C    /**< Bus Suspend */
#define  SDIO_BS            (0x1UL << 0)/**< Bus Status (transfer on DAT[x] lines) (RO) */
#define  SDIO_BR            (0x1UL << 1)/**< Bus Release Request/Status (R/W) */
#define SDIO_FUN_SEL_REG    0x0D    /**< Function select */
#define  SDIO_DF            (0x1UL << 7)/**< Resume Data Flag (RO) */
#define  SDIO_FS            (0xFUL << 0)/**< Select Function (R/W) */
#define  SDIO_FS_CIA        (0x0UL << 0)/**< Select CIA (function 0) */
#define  SDIO_FS_FN1        (0x1UL << 0)/**< Select Function 1 */
#define  SDIO_FS_FN2        (0x2UL << 0)/**< Select Function 2 */
#define  SDIO_FS_FN3        (0x3UL << 0)/**< Select Function 3 */
#define  SDIO_FS_FN4        (0x4UL << 0)/**< Select Function 4 */
#define  SDIO_FS_FN5        (0x5UL << 0)/**< Select Function 5 */
#define  SDIO_FS_FN6        (0x6UL << 0)/**< Select Function 6 */
#define  SDIO_FS_FN7        (0x7UL << 0)/**< Select Function 7 */
#define  SDIO_FS_MEM        (0x8UL << 0)/**< Select memory in combo card */
#define SDIO_EXEC_REG       0x0E    /**< Exec Flags (RO) */
#define  SDIO_EXM           (0x1UL << 0)/**< Executing status of memory */
#define  SDIO_EX            (0xFEUL)    /**< Executing status of functions */
#define  SDIO_EX_FN1        (0x1UL << 1)/**< Executing status of function 1 */
#define  SDIO_EX_FN2        (0x1UL << 2)/**< Executing status of function 2 */
#define  SDIO_EX_FN3        (0x1UL << 3)/**< Executing status of function 3 */
#define  SDIO_EX_FN4        (0x1UL << 4)/**< Executing status of function 4 */
#define  SDIO_EX_FN5        (0x1UL << 5)/**< Executing status of function 5 */
#define  SDIO_EX_FN6        (0x1UL << 6)/**< Executing status of function 6 */
#define  SDIO_EX_FN7        (0x1UL << 7)/**< Executing status of function 7 */
#define SDIO_READY_REG      0x0F    /**< Ready Flags (RO) */
#define  SDIO_RFM           (0x1UL << 0)/**< Ready Flag for memory */
#define  SDIO_RF            (0xFEUL)    /**< Ready Flag for functions */
#define  SDIO_RF_FN1        (0x1UL << 1)/**< Ready Flag for function 1 */
#define  SDIO_RF_FN2        (0x1UL << 2)/**< Ready Flag for function 2 */
#define  SDIO_RF_FN3        (0x1UL << 3)/**< Ready Flag for function 3 */
#define  SDIO_RF_FN4        (0x1UL << 4)/**< Ready Flag for function 4 */
#define  SDIO_RF_FN5        (0x1UL << 5)/**< Ready Flag for function 5 */
#define  SDIO_RF_FN6        (0x1UL << 6)/**< Ready Flag for function 6 */
#define  SDIO_RF_FN7        (0x1UL << 7)/**< Ready Flag for function 7 */
#define SDIO_FN0_BLKSIZ_REG 0x10    /**< FN0 Block Size (2B, LSB first) (R/W) */
#define SDIO_POWER_REG      0x12    /**< Power Control */
#define  SDIO_SMPC          (0x1UL << 0)/**< Support Master Power Control (RO) */
#define  SDIO_EMPC          (0x1UL << 1)/**< Enable Master Power Control (R/W) */
#define SDIO_HS_REG         0x13    /**< High-Speed */
#define  SDIO_SHS           (0x1UL << 0)/**< Support High-Speed (RO) */
#define  SDIO_EHS           (0x1UL << 1)/**< Enable High-Speed (R/W) */
/**     @}*/

/** \addtogroup sdio_fbr_def SDIO Function Basic Registers (FBR)
 *  Here lists SDIO Function Basic Register definitions.
 *  - SDIO_FBR_ADDR()
 *  -# \ref SDIO_FBR_CSA_IF
 *  -# \ref SDIO_FBR_EXT_IF
 *  -# \ref SDIO_FBR_PWR
 *  -# \ref SDIO_FBR_CIS_PTR
 *  -# .
 *  -# .
 *  -# \ref SDIO_FBR_CSA_PTR
 *  -# .
 *  -# .
 *  -# \ref SDIO_FBR_CSA_DATA
 *  -# \ref SDIO_FBR_BLK_SIZ
 *  -# .
 *      @{*/
#define SDIO_FBR_ADDR(fn, x)    (0x100*(fn) + (x))
#define SDIO_FBR_CSA_IF         0x0     /**< CSA and function interface code (RO) */
#define  SDIO_IFC               (0xFUL << 0)/**< Standard SDIO Fun Interface Code */
#define  SDIO_IFC_NO_IF         (0x0UL << 0)/**< No SDIO standard interface */
#define  SDIO_IFC_UART          (0x1UL << 0)/**< UART */
#define  SDIO_IFC_TA_BT         (0x2UL << 0)/**< Type-A Bluetooth */
#define  SDIO_IFC_TB_BT         (0x3UL << 0)/**< Type-B Bluetooth */
#define  SDIO_IFC_GPS           (0x4UL << 0)/**< GPS */
#define  SDIO_IFC_CAMERA        (0x5UL << 0)/**< Camera */
#define  SDIO_IFC_PHS           (0x6UL << 0)/**< PHS */
#define  SDIO_IFC_WLAN          (0x7UL << 0)/**< WLAN */
#define  SDIO_IFC_ATA           (0x8UL << 0)/**< Embedded SDIO-ATA */
#define  SDIO_IFC_EXT           (0xFUL << 0)/**< Check EXT interface code */
#define  SDIO_SCSA              (0x1UL << 6)/**< Function supports Code Storage Area (CSA) */
#define  SDIO_FBR_CSA           (0x1UL << 7)/**< Function CSA enable */
#define SDIO_FBR_EXT_IF         0x1     /**< Extended function interface code (RO) */
#define SDIO_FBR_PWR            0x2     /**< function power control */
#define  SDIO_SPS               (0x1UL << 0)/**< function support power selection (RO) */
#define  SDIO_EPS               (0x1UL << 1)/**< Low Current Mode/High Current Mode (R/W) */
#define SDIO_FBR_CIS_PTR        0x9     /**< Address pointer to function CIS (3B, LSB first) (RO) */
#define SDIO_FBR_CSA_PTR        0xC     /**< Address pointer to CSA (3B, LSB first) (R/W) */
#define SDIO_FBR_CSA_DATA       0xF     /**< Read/Write fifo to CSA (R/W) */
#define SDIO_FBR_BLK_SIZ        0x10    /**< Block size (2B, LSB first) (R/W) */
/**     @}*/

/** \addtogroup sdio_meta_def SDIO Card Metaformat
 *  Here lists definitions for SDIO metaformats.
 *  - \ref CISTPL_NULL
 *  - \ref CISTPL_DEVICE
 *  - \ref CISTPL_CHECKSUM
 *  - \ref CISTPL_VERS_1
 *  - \ref CISTPL_ALTSTR
 *  - \ref CISTPL_MANFID
 *  - \ref CISTPL_FUNCID
 *  - \ref CISTPL_FUNCE
 *  - \ref CISTPL_SDIO_STD
 *  - \ref CISTPL_SDIO_EXT
 *  - \ref CISTPL_END
 *      @{*/
#define CISTPL_NULL     0x00    /**< Null tuple (PCMCIA 3.1.9) */
#define CISTPL_DEVICE   0x01    /**< Device tuple (PCMCIA 3.2.2) */
#define CISTPL_CHECKSUM 0x10    /**< Checksum control (PCMCIA 3.1.1) */
#define CISTPL_VERS_1   0x15    /**< Level 1 version (PCMCIA 3.2.10) */
#define CISTPL_ALTSTR   0x16    /**< Alternate Language String (PCMCIA 3.2.1) */
#define CISTPL_MANFID   0x20    /**< Manufacturer Identification String (PCMCIA 3.2.9) */
#define CISTPL_FUNCID   0x21    /**< Function Identification (PCMCIA 3.2.7) */
#define CISTPL_FUNCE    0x22    /**< Function Extensions (PCMCIA 3.2.6) */
#define CISTPL_SDIO_STD 0x91    /**< Additional information for SDIO (PCMCIA 6.1.2) */
#define CISTPL_SDIO_EXT 0x92    /**< Reserved for future SDIO (PCMCIA 6.1.3) */
#define CISTPL_END      0xFF    /**< The End-of-chain Tuple (PCMCIA 3.1.2) */
/**     @}*/

/**@}*/
#endif /* #define _SDIO_H */

