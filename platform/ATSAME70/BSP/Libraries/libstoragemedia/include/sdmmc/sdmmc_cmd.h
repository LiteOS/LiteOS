/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/** \file */

/**
 * \page sdmmc_lib_cmd SD/MMC Lib Commands
 *
 * Implementation for SD/MMC low level driver. Supply a set of SD/MMC mode's
 * interface.
 *
 * \section Usage
 *
 * - Uses following Library definitions in HAL or application:
 *   -# \ref sdmmc_cardtype : SD/SDIO/MMC card type definitions.
 *   -# \ref sdmmc_rc : Return codes for SD/MMC operation (functions).
 *   -# \ref sSdCard : Struct that defined by application for SD/MMC management.
 * - Uses following Low Level Definitions in HAL:
 *   -# \ref sdmmc_ioctrls : IO control code for HAL implement.
 *   -# \ref sdmmc_cmd_op : Command operation code for HAL to implement.
 *   -# \ref sSdHalFunctions : HAL functions to implement.
 */

#ifndef _SDMMC_CMD_H
#define _SDMMC_CMD_H
/** \addtogroup sdmmc_cmd
 *
 *  \copydoc sdmmc_lib_cmd
 *
 *  See \ref sdmmc_lib_cmd for more details.
 *@{
 */

/*------------------------------------------------------------------------------
 *      Includes
 *----------------------------------------------------------------------------*/

#include <stdint.h>

/*------------------------------------------------------------------------------
 *      Definitions
 *----------------------------------------------------------------------------*/
/** \addtogroup sdmmc_cmd_defines SD/MMC General defines
    @{*/

/**
 *  \addtogroup sdmmc_cardtype SD/MMC Card Types
 *  Here lists the SD/MMC card types.
 *  - Card Type Category Bitmap
 *    - \ref CARD_TYPE_bmHC
 *    - \ref CARD_TYPE_bmSDMMC
 *      - \ref CARD_TYPE_bmUNKNOWN
 *      - \ref CARD_TYPE_bmSD
 *      - \ref CARD_TYPE_bmMMC
 *    - \ref CARD_TYPE_bmSDIO
 *  - Card Types
 *    - \ref CARD_UNKNOWN
 *    - \ref CARD_SD
 *    - \ref CARD_SDHC
 *    - \ref CARD_MMC
 *    - \ref CARD_MMCHD
 *    - \ref CARD_SDIO
 *    - \ref CARD_SDCOMBO
 *    - \ref CARD_SDHCCOMBO
 *      @{*/
#define CARD_TYPE_bmHC           (1 << 0)   /**< Bit for High-Capacity(Density) */
#define CARD_TYPE_bmSDMMC        (0x3 << 1) /**< Bits mask for SD/MMC */
#define CARD_TYPE_bmUNKNOWN      (0x0 << 1) /**< Bits for Unknown card */
#define CARD_TYPE_bmSD           (0x1 << 1) /**< Bits for SD */
#define CARD_TYPE_bmMMC          (0x2 << 1) /**< Bits for MMC */
#define CARD_TYPE_bmSDIO         (1 << 3)   /**< Bit for SDIO */
/** Card can not be identified */
#define CARD_UNKNOWN    (0)
/** SD Card (0x2) */
#define CARD_SD         (CARD_TYPE_bmSD)
/** SD High Capacity Card (0x3) */
#define CARD_SDHC       (CARD_TYPE_bmSD|CARD_TYPE_bmHC)
/** MMC Card (0x4) */
#define CARD_MMC        (CARD_TYPE_bmMMC)
/** MMC High-Density Card (0x5) */
#define CARD_MMCHD      (CARD_TYPE_bmMMC|CARD_TYPE_bmHC)
/** SDIO only card (0x8) */
#define CARD_SDIO       (CARD_TYPE_bmSDIO)
/** SDIO Combo, with SD embedded (0xA) */
#define CARD_SDCOMBO    (CARD_TYPE_bmSDIO|CARD_SD)
/** SDIO Combo, with SDHC embedded (0xB) */
#define CARD_SDHCCOMBO  (CARD_TYPE_bmSDIO|CARD_SDHC)
/**     @}*/

/**
 *  \addtogroup sdmmc_rc SD/MMC Return Codes
 *  Here lists the SD/MMC driver return codes.
 *  - \ref SDMMC_ERROR
 *  - \ref SDMMC_ERROR_NORESPONSE
 *  - \ref SDMMC_ERROR_NOT_INITIALIZED
 *  - \ref SDMMC_ERROR_BUSY
 *  - \ref SDMMC_ERROR_PARAM
 *  - \ref SDMMC_ERROR_NOT_SUPPORT
 *  - \ref SDMMC_OK
 *  - \ref SDMMC_ERROR_LOCKED
 *      @{*/

/** SD/MMC Return codes */
typedef enum {
	SDMMC_OK = 0,           /**< Operation OK */
	SDMMC_LOCKED = 1,       /**< Failed because driver locked */
	SDMMC_BUSY = 2,         /**< Failed because driver busy */
	SDMMC_NO_RESPONSE = 3,  /**< Failed because card not respond */
	SDMMC_CHANGED,          /**< Setting param changed due to limitation */
	SDMMC_ERR,              /**< Failed with general error */
	SDMMC_ERR_IO,           /**< Failed because of IO error */
	SDMMC_ERR_RESP,         /**< Error reported in response code */
	SDMMC_NOT_INITIALIZED,  /**< Fail to initialize */
	SDMMC_PARAM,            /**< Parameter error */
	SDMMC_STATE,            /**< State error */
	SDMMC_USER_CANCEL,      /**< Canceled by user */
	SDMMC_NOT_SUPPORTED     /**< Command(Operation) not supported */
} eSDMMC_RC;

/** No error */
#define SDMMC_SUCCESS               SDMMC_OK
/** The driver is locked. */
#define SDMMC_ERROR_LOCKED          SDMMC_LOCKED
/** There was an error with the SD driver. */
#define SDMMC_ERROR                 SDMMC_ERR
/** The SD card did not answer the command. */
#define SDMMC_ERROR_NORESPONSE      SDMMC_NO_RESPONSE
/** The SD card is not initialized. */
#define SDMMC_ERROR_NOT_INITIALIZED SDMMC_NOT_INITIALIZED
/** The SD card is busy. */
#define SDMMC_ERROR_BUSY            SDMMC_BUSY
/** The input parameter error */
#define SDMMC_ERROR_PARAM           SDMMC_PARAM
/** Invalid state */
#define SDMMC_ERROR_STATE           SDMMC_STATE
/** User canceled */
#define SDMMC_ERROR_USER_CANCEL     SDMMC_USER_CANCEL
/** The operation is not supported. */
#define SDMMC_ERROR_NOT_SUPPORT     SDMMC_NOT_SUPPORTED
/**     @}*/

/**
 *  \addtogroup sdmmc_states SD/MMC Card States
 *      @{
 */
#define SDMMC_STATE_OFF             0x0 /**< Power OFF */
#define SDMMC_STATE_ON              0x1 /**< Power ON */
#define SDMMC_STATE_IDENT           0x2 /**< Card Identification (IDLE,READY,IDENT) */
#define SDMMC_STATE_INACTIVE        0x3 /**< Inactive */
#define SDMMC_STATE_IRQ             0x4 /**< Wait-IRQ, interrupt mode */
#define SDMMC_STATE_STBY            0x5 /**< Addressed */
#define SDMMC_STATE_TRAN            0x6 /**< Ready for transfer */
#define SDMMC_STATE_DATA_WR         0x7 /**< Data TX in progress */
#define SDMMC_STATE_DATA_RD         0x8 /**< Data RX in progress */
/**     @}*/

/** \ingroup sdmmc_hal_def
 *  \addtogroup sdmmc_ioctrls SD/MMC IO Control Codes
 *  Here lists the control codes for SD/MMC low level driver to support lib.
 *  - \ref SDMMC_IOCTL_POWER: Power ON/OFF control.
 *  - \ref SDMMC_IOCTL_SET_CLOCK: Set clock and return actual clock.
 *  - \ref SDMMC_IOCTL_GET_BUSMODE: Return max supported bus mode.
 *  - \ref SDMMC_IOCTL_GET_HSMODE: Return if HS mode is supported.
 *  - \ref SDMMC_IOCTL_GET_BOOTMODE: Return if Boot mode is supported.
 *      @{*/
/** SD/MMC Low Level IO Control: Check busy.
    Must implement for low level driver busy check.
    IOCtrl(pSd, SDMMC_IOCTL_BUSY_CHECK, (uint32_t)pBusyFlag) */
#define SDMMC_IOCTL_BUSY_CHECK    0x0
/** SD/MMC Low Level IO Control: Power control.
    Recomended for SD/MMC/SDIO power control.
    IOCtrl(pSd, SDMMC_IOCTL_POWER, (uint32_t)ON/OFF) */
#define SDMMC_IOCTL_POWER         0x1
/** SD/MMC Low Level IO Control: Cancel command.
    IOCtrl(pSd, SDMMC_IOCTL_CANCEL_CMD, NULL) */
#define SDMMC_IOCTL_CANCEL_CMD    0x2
/** SD/MMC Low Level IO Control: Reset & disable HW.
    IOCtrl(pSd, SDMMC_IOCTL_RESET, NULL) */
#define SDMMC_IOCTL_RESET         0x3
/** SD/MMC Low Level IO Control: Set clock frequency, return applied frequency
    Recomended for clock selection
    IOCtrl(pSd, SDMMC_IOCTL_SET_CLOCK, (uint32_t*)pIoFreq) */
#define SDMMC_IOCTL_SET_CLOCK     0x11
/** SD/MMC Low Level IO Control: Set bus mode, return applied mode
    Recomended for bus mode selection
    IOCtrl(pSd, SDMMC_IOCTL_SET_BUSMODE, (uint32_t*)pIoBusMode) */
#define SDMMC_IOCTL_SET_BUSMODE   0x12
/** SD/MMC Low Level IO Control: Set HS mode, return applied mode
    Recommended for HS mode enable
    IOCtrl(pSd, SDMMC_IOCTL_SET_BUSMODE, (uint32_t*)pIoHsMode) */
#define SDMMC_IOCTL_SET_HSMODE    0x13
/** SD/MMC Low Level IO Control: Set Boot mode */
#define SDMMC_IOCTL_SET_BOOTMODE  0x14
/** SD/MMC Low Level IO Control: Get clock frequency */
#define SDMMC_IOCTL_GET_CLOCK     0x21
/** SD/MMC Low Level IO Control: Bus mode */
#define SDMMC_IOCTL_GET_BUSMODE   0x22
/** SD/MMC Low Level IO Control: High-speed mode */
#define SDMMC_IOCTL_GET_HSMODE    0x23
/** SD/MMC Low Level IO Control: Boot mode */
#define SDMMC_IOCTL_GET_BOOTMODE  0x24
/**     @}*/

/** \ingroup sdmmc_hal_def
 *  \addtogroup sdmmc_cmd_op SD/MMC Command Operations
 *      @{
 */
#define SDMMC_CMD_bmPOWERON     (0x1) /**< Do Power ON sequence */
#define SDMMC_CMD_bmCOMMAND     (0x1 <<  1) /**< Send command */
#define SDMMC_CMD_bmDATAMASK    (0x3 <<  2) /**< Data operation mask */
#define SDMMC_CMD_bmNODATA      (0x0 <<  2) /**< No data transfer */
#define SDMMC_CMD_RX             0x1        /**< data RX */
#define SDMMC_CMD_bmDATARX      (0x1 <<  2) /**< Bits for data RX */
#define SDMMC_CMD_TX             0x2        /**< data TX */
#define SDMMC_CMD_bmDATATX      (0x2 <<  2) /**< Bits for data TX */
#define SDMMC_CMD_STOPXFR        0x3        /**< data stop */
#define SDMMC_CMD_bmSTOPXFR     (0x3 <<  2) /**< Bits for transfer stop */
#define SDMMC_CMD_bmRESPMASK    (0x7 <<  4) /**< Bits masks response option */
#define SDMMC_CMD_bmRESP(R)     (((R)&0x7) << 4)    /**< Bits setup response */

#define SDMMC_CMD_bmCRC         (0x1 <<  7) /**< CRC is enabled (SPI only) */
#define SDMMC_CMD_bmOD          (0x1 <<  8) /**< Open-Drain is enabled (MMC) */
#define SDMMC_CMD_bmIO          (0x1 <<  9) /**< IO function */
#define SDMMC_CMD_bmBUSY        (0x1 << 10) /**< Do busy check */
/** Cmd: Do power on initialize */
#define SDMMC_CMD_POWERONINIT   (SDMMC_CMD_bmPOWERON)
/** Cmd: Data only, read */
#define SDMMC_CMD_DATARX        (SDMMC_CMD_bmDATARX)
/** Cmd: Data only, write */
#define SDMMC_CMD_DATATX        (SDMMC_CMD_bmDATATX)
/** Cmd: Command without data */
#define SDMMC_CMD_CNODATA(R)    (SDMMC_CMD_bmCOMMAND \
								  | SDMMC_CMD_bmRESP(R))
/** Cmd: Command with data, read */
#define SDMMC_CMD_CDATARX(R)    (SDMMC_CMD_bmCOMMAND \
								  | SDMMC_CMD_bmDATARX \
								  | SDMMC_CMD_bmRESP(R))
/** Cmd: Command with data, write */
#define SDMMC_CMD_CDATATX(R)    (SDMMC_CMD_bmCOMMAND \
								  | SDMMC_CMD_bmDATATX \
								  | SDMMC_CMD_bmRESP(R))
/** Cmd: Send Stop command */
#define SDMMC_CMD_CSTOP         (SDMMC_CMD_bmCOMMAND \
								  | SDMMC_CMD_bmSTOPXFR \
								  | SDMMC_CMD_bmRESP(1))
/** Cmd: Send Stop token for SPI */
#define SDMMC_CMD_STOPTOKEN     (SDMMC_CMD_bmSTOPXFR)
/**     @}*/

/** Default block size for SD/MMC access */
#define SDMMC_BLOCK_SIZE        512

/** @}*/
/*------------------------------------------------------------------------------
 *      Types
 *----------------------------------------------------------------------------*/
/** \addtogroup sdmmc_cmd_types SD/MMC General Types
    @{*/

/** SD/MMC end-of-command callback function. */
typedef void (*fSdmmcCallback)(uint32_t status, void *pArg);

/**
 * Sdmmc command operation settings union.
 */
typedef union _SdmmcCmdOperation {
	uint16_t wVal;
	struct _SdmmcOpBm {
		uint16_t powerON: 1, /**< Do power on initialize */
				 sendCmd: 1, /**< Send SD/MMC command */
				 xfrData: 2, /**< Send/Stop data transfer */
				 respType: 3, /**< Response type (1~7) */
				 crcON: 1,  /**< CRC is used (SPI) */
				 odON: 1,   /**< Open-Drain is ON (MMC) */
				 ioCmd: 1,  /**< SDIO command */
				 checkBsy: 1; /**< Busy check is ON */
	} bmBits;
} uSdmmcCmdOp;

/**
 * Sdmmc command instance.
 */
typedef struct _SdmmcCommand {

	/** Optional user-provided callback function. */
	fSdmmcCallback  fCallback;
	/** Optional argument to the callback function. */
	void            *pArg;

	/** Data buffer, with MCI_DMA_ENABLE defined 1, the buffer can be
	 * 1, 2 or 4 bytes aligned. It has to be 4 byte aligned if no DMA.
	 */
	uint8_t     *pData;
	/** Size of data block in bytes. */
	uint16_t    wBlockSize;
	/** Number of blocks to be transfered */
	uint16_t    wNbBlocks;
	/** Response buffer. */
	uint32_t    *pResp;

	/** Command argument. */
	uint32_t    dwArg;
	/** Command operation settings */
	uSdmmcCmdOp cmdOp;
	/** Command index */
	uint8_t     bCmd;
	/** Command return status */
	uint8_t     bStatus;
} sSdmmcCommand;

/**
 * \ingroup sdmmc_hal_def
 * SD/MMC Lock device function type */
typedef uint32_t (*fSdmmcLock)(void *pDrv, uint8_t bSlot);
/**
 * \ingroup sdmmc_hal_def
 * SD/MMC Release device function type */
typedef uint32_t (*fSdmmcRelease)(void *pDrv);
/**
 * \ingroup sdmmc_hal_def
 * SD/MMC Send Command function type */
typedef uint32_t (*fSdmmcSendCommand)(void *pDrv, sSdmmcCommand *pCmd);
/**
 * \ingroup sdmmc_hal_def
 * SD/MMC IO Control function type */
typedef uint32_t (*fSdmmcIOCtrl)(void *pDrv, uint32_t dwCtrl, uint32_t param);

/**
 * \ingroup sdmmc_hal_def
 * \brief SD/MMC card HAL functions.
 */
typedef struct _SdHalFunctions {

	fSdmmcLock fLock;           /**< Pointer to lock funtion */
	fSdmmcRelease fRelease;     /**< Pointer to release function */
	fSdmmcSendCommand fCommand; /**< Pointer to command function */
	fSdmmcIOCtrl fIOCtrl;       /**< Pointer to IO control function */
} sSdHalFunctions;

/**
 * \brief SD/MMC card driver structure.
 * It holds the current command being processed and the SD/MMC card address.
 */
typedef struct _SdCard {
	void *pDrv;             /**< Pointer to unnderlying driver */
	sSdHalFunctions *pHalf; /**< Pointer to underlying functions */
	void *pExt;             /**< Pointer to extension data for SD/MMC/SDIO */
	uint32_t dwTranSpeed;   /**< Max supported transfer speed */

	uint32_t CID[128 / 8 / 4]; /**< Card Identification (CID register) */
	uint32_t EXT[512 / 4];  /**< Extended information, 32 bytes aligned */
	uint32_t SSR[512 / 8 /
				 4]; /**< SD Status (SSR register) 512bits, 32 bytes aligned */
	uint32_t CSD[128 / 8 / 4]; /**< Card-specific data (CSD register) */

	sSdmmcCommand sdCmd;    /**< Command instance for underlying driver */

	uint32_t dwTotalSize;   /**< Card total size
                                (0xffffffff to see number of blocks */
	uint32_t dwNbBlocks;    /**< Card total number of blocks */
	uint16_t wBlockSize;    /**< Card block size reported */

	uint16_t wCurrBlockLen; /**< Block length used */
	uint32_t dwCurrSpeed;   /**< Transfer speed used */
	uint32_t dwPrevBlk;     /**< Previous accessed block for memory */
	uint16_t wAddress;      /**< Current card address */
	uint8_t  bCardType;     /**< SD/MMC/SDIO card type \sa sdmmc_cardtype */
	uint8_t  bBusMode;      /**< 1/4/8 bit bus mode */
	uint8_t  bSlot;         /**< Card access slot */
	uint8_t  bState;        /**< Card state */
	uint8_t  bStatus;       /**< Card status */
	uint8_t  reserve;
} sSdCard;


/** \addtogroup sdmmc_struct_cmdarg SD/MMC command arguments
 *  Here lists the command arguments for SD/MMC.
 *  - CMD6 Argument
 *    - \ref _MmcCmd6Arg "MMC CMD6"
 *    - \ref _SdCmd6Arg  "SD CMD6"
 *  - \ref _SdioCmd52Arg CMD52
 *  - \ref _SdioCmd53Arg CMD53
 *      @{*/
/**
 * \typedef MmcCmd6Arg
 * Argument for MMC CMD6
 */
typedef struct _MmcCmd6Arg {
	uint8_t access;
	uint8_t index;
	uint8_t value;
	uint8_t cmdSet;
} MmcCmd6Arg, MmcSwitchArg;

/**
 * \typedef SdCmd6Arg
 * Argument for SD CMD6
 */
typedef struct _SdCmd6Arg {
	uint32_t accessMode: 4, /**< [ 3: 0] function group 1, access mode */
			 command: 4,    /**< [ 7: 4] function group 2, command system */
			 reserveFG3: 4, /**< [11: 8] function group 3, 0xF or 0x0 */
			 reserveFG4: 4, /**< [15:12] function group 4, 0xF or 0x0 */
			 reserveFG5: 4, /**< [19:16] function group 5, 0xF or 0x0 */
			 reserveFG6: 4, /**< [23:20] function group 6, 0xF or 0x0 */
			 reserved: 7,   /**< [30:24] reserved 0 */
			 mode: 1;       /**< [31   ] Mode, 0: Check, 1: Switch */
} SdCmd6Arg, SdSwitchArg;

/**
 * \typedef SdioCmd52Arg
 * Argument for SDIO CMD52
 */
typedef struct _SdioCmd52Arg {
	uint32_t data: 8,       /**< [ 7: 0] data for writing */
			 stuff0: 1,     /**< [    8] reserved */
			 regAddress: 17, /**< [25: 9] register address */
			 stuff1: 1,     /**< [   26] reserved */
			 rawFlag: 1,    /**< [   27] Read after Write flag */
			 functionNum: 3, /**< [30:28] Number of the function */
			 rwFlag: 1;     /**< [   31] Direction, 1:write, 0:read. */
} SdioCmd52Arg, SdioRwDirectArg;
/**
 * \typedef SdioCmd53Arg
 * Argument for SDIO CMD53
 */
typedef struct _SdioCmd53Arg {
	uint32_t count: 9,       /**< [ 8: 0] Byte mode: number of bytes to transfer,
                                                   0 cause 512 bytes transfer.
                                         Block mode: number of blocks to transfer,
                                                    0 set count to infinite. */
			 regAddress: 17, /**< [25: 9] Start Address I/O register */
			 opCode: 1,     /**< [   26] 1:Incrementing address, 0: fixed */
			 blockMode: 1,  /**< [   27] (Optional) 1:block mode */
			 functionNum: 3, /**< [30:28] Number of the function */
			 rwFlag: 1;     /**< [   31] Direction, 1:WR, 0:RD */
} SdioCmd53Arg, SdioRwExtArg;
/**     @}*/


/** \addtogroup sdmmc_resp_struct SD/MMC Responses Structs
 *  Here lists the command responses for SD/MMC.
 *  - \ref _SdmmcR1 "R1"
 *  - \ref _SdmmcR3 "R3"
 *  - \ref _MmcR4 "MMC R4", \ref _SdioR4 "SDIO R4"
 *  - \ref _SdmmcR5 "R5"
 *  - \ref _SdR6 "R6"
 *  - \ref _SdR7 "R7"
 *      @{*/
/**
 * Response R1 (normal response command)
 */
typedef struct _SdmmcR1 {
	uint32_t cardStatus;    /**< [32: 0] Response card status flags */
} SdmmcR1, SdmmcR1b;

/**
 * Response R3 (OCR)
 */
typedef struct _SdmmcR3 {
	uint32_t OCR;           /**< [32: 0] OCR register */
} SdmmcR3;

/**
 * Response R4 (MMC Fast I/O CMD39)
 */
typedef struct _MmcR4 {
	uint32_t regData: 8,    /**< [ 7: 0] Read register contents */
			 regAddr: 7,    /**< [14: 8] Register address */
			 status: 1,     /**< [   15] Status */
			 RCA: 16;       /**< [31:16] RCA */
} MmcR4;

/**
 * Response R4 (SDIO), no CRC.
 */
typedef struct _SdioR4 {
	uint32_t OCR: 24,           /**< [23: 0]       Operation Conditions Register */
			 reserved: 3,       /**< [26:24]       Reserved */
			 memoryPresent: 1,   /**< [   27] MP    Set to 1 if contains
                                                   SD memory */
			 nbIoFunction: 3,    /**< [30:28] NF    Total number of I/O functions
                                                   supported */
			 C: 1;              /**< [   31] IORDY Set to 1 if card is ready */
} SdioR4;

/**
 * Response R5 (MMC Interrupt request CMD40 / SDIO CMD52)
 */
typedef struct _SdmmcR5 {
	uint32_t data: 8,       /**< [ 7: 0] Response data */
			 response: 8,   /**< [15: 8] Response status flags */
			 RCA: 16;       /**< [31:16] (MMC) Winning card RCA */
} SdmmcR5;

/**
 * Response R6 (SD RCA)
 */
typedef struct _SdR6 {
	uint32_t status: 16,    /**< [15: 0] Response status */
			 RCA: 16;       /**< [31:16] New published RCA */
} SdR6;
/**
 * Response R7 (Card interface condition)
 */
typedef struct _SdR7 {
	uint32_t checkPatten: 8, /**< [ 7: 0] Echo-back of check pattern */
			 voltage: 4,    /**< [11: 8] Voltage accepted */
			 reserved: 20;  /**< [31:12] reserved bits */
} SdR7;

/**     @}*/

/** @}*/
/*----------------------------------------------------------------------------
 *         Exported functions
 *----------------------------------------------------------------------------*/

/**@}*/
#endif /* #define _SDMMC_CMD_H */

