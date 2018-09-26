/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_SDSPI_DISK_H_
#define _FSL_SDSPI_DISK_H_

#include <stdint.h>
#include "diskio.h"
#include "fsl_common.h"
#include "board.h"

/*!
 * @addtogroup SD Disk over SPI
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Driver version. */
#define FSL_SDSPI_DRIVER_VERSION (MAKE_VERSION(2U, 1U, 1U)) /*2.1.1*/

/*! @brief Default block size */
#define FSL_SDSPI_DEFAULT_BLOCK_SIZE (512U)

/*! @brief SD card product name length united as bytes. */
#define SD_PRODUCT_NAME_BYTES (5U)

/*! @brief SD/MMC card initialization clock frequency */
#define SDMMC_CLOCK_400KHZ (400000U)
/*! @brief SD card bus frequency 1 in high-speed mode */
#define SD_CLOCK_25MHZ (25000000U)
/*! @brief SD card bus frequency 2 in high-speed mode */
#define SD_CLOCK_50MHZ (50000000U)
/*! @brief MMC card bus frequency 1 in high-speed mode */
#define MMC_CLOCK_26MHZ (26000000U)
/*! @brief MMC card bus frequency 2 in high-speed mode */
#define MMC_CLOCK_52MHZ (52000000U)

/*! @brief SD card CID register */
typedef struct _sd_cid
{
    uint8_t manufacturerID;                     /*!< Manufacturer ID [127:120] */
    uint16_t applicationID;                     /*!< OEM/Application ID [119:104] */
    uint8_t productName[SD_PRODUCT_NAME_BYTES]; /*!< Product name [103:64] */
    uint8_t productVersion;                     /*!< Product revision [63:56] */
    uint32_t productSerialNumber;               /*!< Product serial number [55:24] */
    uint16_t manufacturerData;                  /*!< Manufacturing date [19:8] */
} sd_cid_t;

/*! @brief SD card CSD register */
typedef struct _sd_csd
{
    uint8_t csdStructure;        /*!< CSD structure [127:126] */
    uint8_t dataReadAccessTime1; /*!< Data read access-time-1 [119:112] */
    uint8_t dataReadAccessTime2; /*!< Data read access-time-2 in clock cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;       /*!< Maximum data transfer rate [103:96] */
    uint16_t cardCommandClass;   /*!< Card command classes [95:84] */
    uint8_t readBlockLength;     /*!< Maximum read data block length [83:80] */
    uint16_t flags;              /*!< Flags in _sd_csd_flag */
    uint32_t deviceSize;         /*!< Device size [73:62] */
    /* Following fields from 'readCurrentVddMin' to 'deviceSizeMultiplier' exist in CSD version 1 */
    uint8_t readCurrentVddMin;    /*!< Maximum read current at VDD min [61:59] */
    uint8_t readCurrentVddMax;    /*!< Maximum read current at VDD max [58:56] */
    uint8_t writeCurrentVddMin;   /*!< Maximum write current at VDD min [55:53] */
    uint8_t writeCurrentVddMax;   /*!< Maximum write current at VDD max [52:50] */
    uint8_t deviceSizeMultiplier; /*!< Device size multiplier [49:47] */

    uint8_t eraseSectorSize;       /*!< Erase sector size [45:39] */
    uint8_t writeProtectGroupSize; /*!< Write protect group size [38:32] */
    uint8_t writeSpeedFactor;      /*!< Write speed factor [28:26] */
    uint8_t writeBlockLength;      /*!< Maximum write data block length [25:22] */
    uint8_t fileFormat;            /*!< File format [11:10] */
} sd_csd_t;

/*! @brief The bit shift for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_SHIFT (0U)
/*! @brief The bit mask for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_MASK (0x07U)
/*! @brief The bit shift for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_SHIFT (2U)
/*! @brief The bit mask for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_MASK (0x78U)
/*! @brief Read the value of FREQUENCY UNIT in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_RATE_UNIT(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_RATE_UNIT_MASK) >> SD_TRANSFER_SPEED_RATE_UNIT_SHIFT)
/*! @brief Read the value of TIME VALUE in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_TIME_VALUE(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_TIME_VALUE_MASK) >> SD_TRANSFER_SPEED_TIME_VALUE_SHIFT)

/*! @brief SD card SCR register */
typedef struct _sd_scr
{
    uint8_t scrStructure;             /*!< SCR Structure [63:60] */
    uint8_t sdSpecification;          /*!< SD memory card specification version [59:56] */
    uint16_t flags;                   /*!< SCR flags in _sd_scr_flag */
    uint8_t sdSecurity;               /*!< Security specification supported [54:52] */
    uint8_t sdBusWidths;              /*!< Data bus widths supported [51:48] */
    uint8_t extendedSecurity;         /*!< Extended security support [46:43] */
    uint8_t commandSupport;           /*!< Command support bits [33:32] */
    uint32_t reservedForManufacturer; /*!< reserved for manufacturer usage [31:0] */
} sd_scr_t;

/*! @brief SD card CSD register flags */
enum _sd_csd_flag
{
    kSD_CsdReadBlockPartialFlag = (1U << 0U),         /*!< Partial blocks for read allowed [79:79] */
    kSD_CsdWriteBlockMisalignFlag = (1U << 1U),       /*!< Write block misalignment [78:78] */
    kSD_CsdReadBlockMisalignFlag = (1U << 2U),        /*!< Read block misalignment [77:77] */
    kSD_CsdDsrImplementedFlag = (1U << 3U),           /*!< DSR implemented [76:76] */
    kSD_CsdEraseBlockEnabledFlag = (1U << 4U),        /*!< Erase single block enabled [46:46] */
    kSD_CsdWriteProtectGroupEnabledFlag = (1U << 5U), /*!< Write protect group enabled [31:31] */
    kSD_CsdWriteBlockPartialFlag = (1U << 6U),        /*!< Partial blocks for write allowed [21:21] */
    kSD_CsdFileFormatGroupFlag = (1U << 7U),          /*!< File format group [15:15] */
    kSD_CsdCopyFlag = (1U << 8U),                     /*!< Copy flag [14:14] */
    kSD_CsdPermanentWriteProtectFlag = (1U << 9U),    /*!< Permanent write protection [13:13] */
    kSD_CsdTemporaryWriteProtectFlag = (1U << 10U),   /*!< Temporary write protection [12:12] */
};

/*! @brief OCR register in SD card */
enum _sd_ocr_flag
{
    kSD_OcrPowerUpBusyFlag = (1U << 31U),                            /*!< Power up busy status */
    kSD_OcrHostCapacitySupportFlag = (1U << 30U),                    /*!< Card capacity status */
    kSD_OcrCardCapacitySupportFlag = kSD_OcrHostCapacitySupportFlag, /*!< Card capacity status */
    kSD_OcrSwitch18RequestFlag = (1U << 24U),                        /*!< Switch to 1.8V request */
    kSD_OcrSwitch18AcceptFlag = kSD_OcrSwitch18RequestFlag,          /*!< Switch to 1.8V accepted */
    kSD_OcrVdd27_28Flag = (1U << 15U),                               /*!< VDD 2.7-2.8 */
    kSD_OcrVdd28_29Flag = (1U << 16U),                               /*!< VDD 2.8-2.9 */
    kSD_OcrVdd29_30Flag = (1U << 17U),                               /*!< VDD 2.9-3.0 */
    kSD_OcrVdd30_31Flag = (1U << 18U),                               /*!< VDD 2.9-3.0 */
    kSD_OcrVdd31_32Flag = (1U << 19U),                               /*!< VDD 3.0-3.1 */
    kSD_OcrVdd32_33Flag = (1U << 20U),                               /*!< VDD 3.1-3.2 */
    kSD_OcrVdd33_34Flag = (1U << 21U),                               /*!< VDD 3.2-3.3 */
    kSD_OcrVdd34_35Flag = (1U << 22U),                               /*!< VDD 3.3-3.4 */
    kSD_OcrVdd35_36Flag = (1U << 23U),                               /*!< VDD 3.4-3.5 */
};

/*! @brief SD card specification version number */
enum _sd_specification_version
{
    kSD_SpecificationVersion1_0 = (1U << 0U), /*!< SD card version 1.0-1.01 */
    kSD_SpecificationVersion1_1 = (1U << 1U), /*!< SD card version 1.10 */
    kSD_SpecificationVersion2_0 = (1U << 2U), /*!< SD card version 2.00 */
    kSD_SpecificationVersion3_0 = (1U << 3U), /*!< SD card version 3.0 */
};

/*! @brief SD card bus width */
typedef enum _sd_data_bus_width
{
    kSD_DataBusWidth1Bit = 0U, /*!< SD data bus width 1-bit mode */
    kSD_DataBusWidth4Bit = 1U, /*!< SD data bus width 4-bit mode */
} sd_data_bus_width_t;

/*! @brief SD card switch mode */
typedef enum _sd_switch_mode
{
    kSD_SwitchCheck = 0U, /*!< SD switch mode 0: check function */
    kSD_SwitchSet = 1U,   /*!< SD switch mode 1: set function */
} sd_switch_mode_t;

/*!
 * @addtogroup SDSPI
 * @{
 */

/*! @brief SDSPI API status */
enum _sdspi_status
{
    kStatus_SDSPI_SetFrequencyFailed = MAKE_STATUS(kStatusGroup_SDSPI, 0U), /*!< Set frequency failed */
    kStatus_SDSPI_ExchangeFailed = MAKE_STATUS(kStatusGroup_SDSPI, 1U),     /*!< Exchange data on SPI bus failed */
    kStatus_SDSPI_WaitReadyFailed = MAKE_STATUS(kStatusGroup_SDSPI, 2U),    /*!< Wait card ready failed */
    kStatus_SDSPI_ResponseError = MAKE_STATUS(kStatusGroup_SDSPI, 3U),      /*!< Response is error */
    kStatus_SDSPI_WriteProtected = MAKE_STATUS(kStatusGroup_SDSPI, 4U),     /*!< Write protected */
    kStatus_SDSPI_GoIdleFailed = MAKE_STATUS(kStatusGroup_SDSPI, 5U),       /*!< Go idle failed */
    kStatus_SDSPI_SendCommandFailed = MAKE_STATUS(kStatusGroup_SDSPI, 6U),  /*!< Send command failed */
    kStatus_SDSPI_ReadFailed = MAKE_STATUS(kStatusGroup_SDSPI, 7U),         /*!< Read data failed */
    kStatus_SDSPI_WriteFailed = MAKE_STATUS(kStatusGroup_SDSPI, 8U),        /*!< Write data failed */
    kStatus_SDSPI_SendInterfaceConditionFailed =
        MAKE_STATUS(kStatusGroup_SDSPI, 9U), /*!< Send interface condition failed */
    kStatus_SDSPI_SendOperationConditionFailed =
        MAKE_STATUS(kStatusGroup_SDSPI, 10U),                                    /*!< Send operation condition failed */
    kStatus_SDSPI_ReadOcrFailed = MAKE_STATUS(kStatusGroup_SDSPI, 11U),          /*!< Read OCR failed */
    kStatus_SDSPI_SetBlockSizeFailed = MAKE_STATUS(kStatusGroup_SDSPI, 12U),     /*!< Set block size failed */
    kStatus_SDSPI_SendCsdFailed = MAKE_STATUS(kStatusGroup_SDSPI, 13U),          /*!< Send CSD failed */
    kStatus_SDSPI_SendCidFailed = MAKE_STATUS(kStatusGroup_SDSPI, 14U),          /*!< Send CID failed */
    kStatus_SDSPI_StopTransmissionFailed = MAKE_STATUS(kStatusGroup_SDSPI, 15U), /*!< Stop transmission failed */
    kStatus_SDSPI_SendApplicationCommandFailed =
        MAKE_STATUS(kStatusGroup_SDSPI, 16U), /*!< Send application command failed */
};

/*! @brief SD card SCR register flags */
enum _sd_scr_flag
{
    kSD_ScrDataStatusAfterErase = (1U << 0U), /*!< Data status after erases [55:55] */
    kSD_ScrSdSpecification3 = (1U << 1U),     /*!< Specification version 3.00 or higher [47:47]*/
};

/*! @brief SDSPI card flag */
enum _sdspi_card_flag
{
    kSDSPI_SupportHighCapacityFlag = (1U << 0U), /*!< Card is high capacity */
    kSDSPI_SupportSdhcFlag = (1U << 1U),         /*!< Card is SDHC */
    kSDSPI_SupportSdxcFlag = (1U << 2U),         /*!< Card is SDXC */
    kSDSPI_SupportSdscFlag = (1U << 3U),         /*!< Card is SDSC */
};

/*! @brief SDSPI response type */
typedef enum _sdspi_response_type
{
    kSDSPI_ResponseTypeR1 = 0U,  /*!< Response 1 */
    kSDSPI_ResponseTypeR1b = 1U, /*!< Response 1 with busy */
    kSDSPI_ResponseTypeR2 = 2U,  /*!< Response 2 */
    kSDSPI_ResponseTypeR3 = 3U,  /*!< Response 3 */
    kSDSPI_ResponseTypeR7 = 4U,  /*!< Response 7 */
} sdspi_response_type_t;

/*! @brief SDSPI command */
typedef struct _sdspi_command
{
    uint8_t index;        /*!< Command index */
    uint32_t argument;    /*!< Command argument */
    uint8_t responseType; /*!< Response type */
    uint8_t response[5U]; /*!< Response content */
} sdspi_command_t;

/*! @brief SDSPI host state. */
typedef struct _sdspi_host
{
    uint32_t busBaudRate; /*!< Bus baud rate */

    status_t (*setFrequency)(uint32_t frequency);                   /*!< Set frequency of SPI */
    status_t (*exchange)(uint8_t *in, uint8_t *out, uint32_t size); /*!< Exchange data over SPI */
    uint32_t (*getCurrentMilliseconds)(void);                       /*!< Get current time in milliseconds */
} sdspi_host_t;
 
/*!
 * @brief SD Card Structure
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _sdspi_card
{
    sdspi_host_t *host;       /*!< Host state information */
    uint32_t relativeAddress; /*!< Relative address of the card */
    uint32_t flags;           /*!< Flags defined in _sdspi_card_flag. */
    uint8_t rawCid[16U];      /*!< Raw CID content */
    uint8_t rawCsd[16U];      /*!< Raw CSD content */
    uint8_t rawScr[8U];       /*!< Raw SCR content */
    uint32_t ocr;             /*!< Raw OCR content */
    sd_cid_t cid;             /*!< CID */
    sd_csd_t csd;             /*!< CSD */
    sd_scr_t scr;             /*!< SCR */
    uint32_t blockCount;      /*!< Card total block number */
    uint32_t blockSize;       /*!< Card block size */
} sdspi_card_t;

/*! @brief Error bit in SPI mode R1 */
enum _sdspi_r1_error_status_flag
{
    kSDSPI_R1InIdleStateFlag = (1U << 0U),        /*!< In idle state */
    kSDSPI_R1EraseResetFlag = (1U << 1U),         /*!< Erase reset */
    kSDSPI_R1IllegalCommandFlag = (1U << 2U),     /*!< Illegal command */
    kSDSPI_R1CommandCrcErrorFlag = (1U << 3U),    /*!< Com crc error */
    kSDSPI_R1EraseSequenceErrorFlag = (1U << 4U), /*!< Erase sequence error */
    kSDSPI_R1AddressErrorFlag = (1U << 5U),       /*!< Address error */
    kSDSPI_R1ParameterErrorFlag = (1U << 6U),     /*!< Parameter error */
};

/*! @brief Error bit in SPI mode R2 */
enum _sdspi_r2_error_status_flag
{
    kSDSPI_R2CardLockedFlag = (1U << 0U),            /*!< Card is locked */
    kSDSPI_R2WriteProtectEraseSkip = (1U << 1U),     /*!< Write protect erase skip */
    kSDSPI_R2LockUnlockFailed = (1U << 1U),          /*!< Lock/unlock command failed */
    kSDSPI_R2ErrorFlag = (1U << 2U),                 /*!< Unknown error */
    kSDSPI_R2CardControllerErrorFlag = (1U << 3U),   /*!< Card controller error */
    kSDSPI_R2CardEccFailedFlag = (1U << 4U),         /*!< Card ecc failed */
    kSDSPI_R2WriteProtectViolationFlag = (1U << 5U), /*!< Write protect violation */
    kSDSPI_R2EraseParameterErrorFlag = (1U << 6U),   /*!< Erase parameter error */
    kSDSPI_R2OutOfRangeFlag = (1U << 7U),            /*!< Out of range */
    kSDSPI_R2CsdOverwriteFlag = (1U << 7U),          /*!< CSD overwrite */
};

/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_SHIFT (28U)
/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_MASK (0xFU)
/*! @brief The bit shift for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_SHIFT (8U)
/*! @brief The bit mask for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_MASK (0xFU)
/*! @brief The bit mask for VOLTAGE 2.7V to 3.6V field in R7 */
#define SDSPI_R7_VOLTAGE_27_36_MASK (0x1U << SDSPI_R7_VOLTAGE_SHIFT)
/*! @brief The bit shift for ECHO field in R7 */
#define SDSPI_R7_ECHO_SHIFT (0U)
/*! @brief The bit mask for ECHO field in R7 */
#define SDSPI_R7_ECHO_MASK (0xFFU)

/*! @brief Data error token mask */
#define SDSPI_DATA_ERROR_TOKEN_MASK (0xFU)
/*! @brief Data Error Token mask bit */
enum _sdspi_data_error_token
{
    kSDSPI_DataErrorTokenError = (1U << 0U),               /*!< Data error */
    kSDSPI_DataErrorTokenCardControllerError = (1U << 1U), /*!< Card controller error */
    kSDSPI_DataErrorTokenCardEccFailed = (1U << 2U),       /*!< Card ecc error */
    kSDSPI_DataErrorTokenOutOfRange = (1U << 3U),          /*!< Out of range */
};

/*! @brief Data Token */
typedef enum _sdspi_data_token
{
    kSDSPI_DataTokenBlockRead = 0xFEU,          /*!< Single block read, multiple block read */
    kSDSPI_DataTokenSingleBlockWrite = 0xFEU,   /*!< Single block write */
    kSDSPI_DataTokenMultipleBlockWrite = 0xFCU, /*!< Multiple block write */
    kSDSPI_DataTokenStopTransfer = 0xFDU,       /*!< Stop transmission */
} sdspi_data_token_t;

/* Data Response Token mask */
#define SDSPI_DATA_RESPONSE_TOKEN_MASK (0x1FU) /*!< Mask for data response bits */
/*! @brief Data Response Token */
typedef enum _sdspi_data_response_token
{
    kSDSPI_DataResponseTokenAccepted = 0x05U,   /*!< Data accepted */
    kSDSPI_DataResponseTokenCrcError = 0x0BU,   /*!< Data rejected due to CRC error */
    kSDSPI_DataResponseTokenWriteError = 0x0DU, /*!< Data rejected due to write error */
} sdspi_data_response_token_t;

/*! @brief SD card individual commands */
typedef enum _sd_command
{
    kSD_SendRelativeAddress = 3U,    /*!< Send Relative Address */
    kSD_Switch = 6U,                 /*!< Switch Function */
    kSD_SendInterfaceCondition = 8U, /*!< Send Interface Condition */
    kSD_VoltageSwitch = 11U,         /*!< Voltage Switch */
    kSD_SpeedClassControl = 20U,     /*!< Speed Class control */
    kSD_EraseWriteBlockStart = 32U,  /*!< Write Block Start */
    kSD_EraseWriteBlockEnd = 33U,    /*!< Write Block End */
} sd_command_t;

/*! @brief SD card individual application commands */
typedef enum _sd_application_command
{
    kSD_ApplicationSetBusWdith = 6U,              /*!< Set Bus Width */
    kSD_ApplicationStatus = 13U,                  /*!< Send SD status */
    kSD_ApplicationSendNumberWriteBlocks = 22U,   /*!< Send Number Of Written Blocks */
    kSD_ApplicationSetWriteBlockEraseCount = 23U, /*!< Set Write Block Erase Count */
    kSD_ApplicationSendOperationCondition = 41U,  /*!< Send Operation Condition */
    kSD_ApplicationSetClearCardDetect = 42U,      /*!< Set Connnect/Disconnect pull up on detect pin */
    kSD_ApplicationSendScr = 51U,                 /*!< Send Scr */
} sd_application_command_t;

/*! @brief SD/MMC card common commands */
typedef enum _sdmmc_command
{
    kSDMMC_GoIdleState = 0U,         /*!< Go Idle State */
    kSDMMC_AllSendCid = 2U,          /*!< All Send CID */
    kSDMMC_SetDsr = 4U,              /*!< Set DSR */
    kSDMMC_SelectCard = 7U,          /*!< Select Card */
    kSDMMC_SendCsd = 9U,             /*!< Send CSD */
    kSDMMC_SendCid = 10U,            /*!< Send CID */
    kSDMMC_StopTransmission = 12U,   /*!< Stop Transmission */
    kSDMMC_SendStatus = 13U,         /*!< Send Status */
    kSDMMC_GoInactiveState = 15U,    /*!< Go Inactive State */
    kSDMMC_SetBlockLength = 16U,     /*!< Set Block Length */
    kSDMMC_ReadSingleBlock = 17U,    /*!< Read Single Block */
    kSDMMC_ReadMultipleBlock = 18U,  /*!< Read Multiple Block */
    kSDMMC_SendTuningBlock = 19U,    /*!< Send Tuning Block */
    kSDMMC_SetBlockCount = 23U,      /*!< Set Block Count */
    kSDMMC_WriteSingleBlock = 24U,   /*!< Write Single Block */
    kSDMMC_WriteMultipleBlock = 25U, /*!< Write Multiple Block */
    kSDMMC_ProgramCsd = 27U,         /*!< Program CSD */
    kSDMMC_SetWriteProtect = 28U,    /*!< Set Write Protect */
    kSDMMC_ClearWriteProtect = 29U,  /*!< Clear Write Protect */
    kSDMMC_SendWriteProtect = 30U,   /*!< Send Write Protect */
    kSDMMC_Erase = 38U,              /*!< Erase */
    kSDMMC_LockUnlock = 42U,         /*!< Lock Unlock */
    kSDMMC_ApplicationCommand = 55U, /*!< Send Application Command */
    kSDMMC_GeneralCommand = 56U,     /*!< General Purpose Command */
    kSDMMC_ReadOcr = 58U,            /*!< Read OCR */
} sdmmc_command_t;

/*! @brief SD card command class */
enum _sdmmc_command_class
{
    kSDMMC_CommandClassBasic = (1U << 0U),               /*!< Card command class 0 */
    kSDMMC_CommandClassBlockRead = (1U << 2U),           /*!< Card command class 2 */
    kSDMMC_CommandClassBlockWrite = (1U << 4U),          /*!< Card command class 4 */
    kSDMMC_CommandClassErase = (1U << 5U),               /*!< Card command class 5 */
    kSDMMC_CommandClassWriteProtect = (1U << 6U),        /*!< Card command class 6 */
    kSDMMC_CommandClassLockCard = (1U << 7U),            /*!< Card command class 7 */
    kSDMMC_CommandClassApplicationSpecific = (1U << 8U), /*!< Card command class 8 */
    kSDMMC_CommandClassInputOutputMode = (1U << 9U),     /*!< Card command class 9 */
    kSDMMC_CommandClassSwitch = (1U << 10U),             /*!< Card command class 10 */
};



/*************************************************************************************************
 * API - SD disk interface
 ************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SD over SPI Disk Function
 * @{
 */

/*!
 * @brief Initializes SD disk over SPI.
 *
 * @param physicalDrive Physical drive number.
 * @retval STA_NOINIT Failed.
 * @retval RES_OK Success.
 */
DSTATUS sdspi_disk_initialize(uint8_t physicalDrive);

/*!
 * Gets SD over SPI disk status
 *
 * @param physicalDrive Physical drive number.
 * @retval STA_NOINIT Failed.
 * @retval RES_OK Success.
 */
DSTATUS sdspi_disk_status(uint8_t physicalDrive);

/*!
 * @brief Reads SD disk over SPI.
 *
 * @param physicalDrive Physical drive number.
 * @param buffer The data buffer pointer to store read content.
 * @param sector The start sector number to be read.
 * @param count The sector count to be read.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT sdspi_disk_read(uint8_t physicalDrive, uint8_t *buffer, uint32_t sector, uint8_t count);

/*!
 * @brief Writes to SD disk over SPI.
 *
 * @param physicalDrive Physical drive number.
 * @param buffer The data buffer pointer to store write content.
 * @param sector The start sector number to be written.
 * @param count The sector count to be written.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT sdspi_disk_write(uint8_t physicalDrive, const uint8_t *buffer, uint32_t sector, uint8_t count);

/*!
 * @brief SD over SPI disk IO operation.
 *
 * @param physicalDrive Physical drive number.
 * @param command The command to be set.
 * @param buffer The buffer to store command result.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT sdspi_disk_ioctl(uint8_t physicalDrive, uint8_t command, void *buffer);

/*************************************************************************************************
 * API - SPI interface
 ************************************************************************************************/
/*!
 * @brief Initializes the SPI.
 */
void spi_init(void);

/*!
 * @brief Sets the SPI bus frequency.
 *
 * @param frequency The frequency to set.
 * @retval kStatus_Success Success.
 * @retval kStatus_Fail Failed.
 */
status_t spi_set_frequency(uint32_t frequency);

/*!
 * @brief Transfers data over SPI bus in full-duplex way.
 *
 * @param in The buffer to save the data to be sent.
 * @param out The buffer to save the data to be read.
 * @param size The transfer data size.
 * @return The status of the function DSPI_MasterTransferPolling().
 */
status_t spi_exchange(uint8_t *in, uint8_t *out, uint32_t size);

/*!
 * @brief Initializes the timer to generator 1ms interrupt used to get current time in milliseconds.
 */
void timer_init(void);

/*!
 * @brief Gets current time in milliseconds.
 *
 * @return Current time in milliseconds.
 */
uint32_t timer_get_current_milliseconds(void);

/*!
 * @brief Initializes the host descriptor.
 */
void sdspi_host_init(void);
extern uint8_t file_ls(void);
/* @} */
#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SDSPI_DISK_H_ */
