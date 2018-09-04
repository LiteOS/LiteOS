/***************************************************************************//**
 * @file  msdscsi.h
 * @brief SCSI interface for Mass Storage Devices (MSD).
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#ifndef __MSDSCSI_H
#define __MSDSCSI_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Msd
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define SCSI_TESTUNIT_READY          0x00 /**< SCSI Test Unit Ready command opcode. */
#define SCSI_REQUESTSENSE            0x03 /**< SCSI Request Sense command opcode.   */
#define SCSI_INQUIRY                 0x12 /**< SCSI Inquiry command opcode.         */
#define SCSI_STARTSTOP_UNIT          0x1B /**< SCSI Start Stop Unit command opcode. */
#define SCSI_READCAPACITY            0x25 /**< SCSI Read Capacity command opcode.   */
#define SCSI_READ10                  0x28 /**< SCSI Read (10) command opcode.       */
#define SCSI_WRITE10                 0x2A /**< SCSI Write (10) command opcode.      */
#define SCSI_VERIFY10                0x2F /**< SCSI Verify (10) command opcode.     */

#define SCSI_READ10_LEN              10U  /**< SCSI Read (10) CBD length.       */
#define SCSI_WRITE10_LEN             10U  /**< SCSI Write (10) CDB length.      */
#define SCSI_VERIFY10_LEN            10U  /**< SCSI Verify (10) CDB length.     */
#define SCSI_INQUIRYDATA_LEN         36U  /**< SCSI Inquiry response data length. */
#define SCSI_STARTSTOPUNIT_LEN       6U   /**< SCSI Start Stop Unit CDB length. */
#define SCSI_REQUESTSENSE_LEN        6U   /**< SCSI Request Sense CDB length.   */
#define SCSI_REQUESTSENSEDATA_LEN    18U  /**< SCSI Request Sense response data length. */
#define SCSI_READCAPACITY_LEN        10U  /**< SCSI Read Capacity CDB length.   */
#define SCSI_READCAPACITYDATA_LEN    8U   /**< SCSI Read Capacity response data length. */

/**************************************************************************//**
 * @brief SCSI Inquiry Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                        */
  struct
  {
    uint8_t Evpd     : 1;         /**< Enable Vital Product data.             */
    uint8_t Obsolete : 1;         /**< Obsolete, expect any value.            */
    uint8_t Reserved : 6;         /**< Reserved, expect 0.                    */
  };
  uint8_t  PageCode;              /**< Pagecode.                              */
  uint16_t AllocationLength;      /**< Number of inquiry data bytes requested.*/
  uint8_t  Control;               /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_Inquiry_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Inquiry response data typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  struct
  {
    uint8_t PeripheralDeviceType : 5; /**< Peripheral device type .           */
    uint8_t PeripheralQualifier  : 3; /**< Peripheral qualifier.              */
  };
  struct
  {
    uint8_t Reserved1 : 7;        /**< Reserved, expect 0.                    */
    uint8_t Removable : 1;        /**< Removable medium indicator.            */
  };
  uint8_t Version;                /**< SPC standard version implementation.   */
  struct
  {
    uint8_t ResponseDataFormat : 4; /**< Response data format value.          */
    uint8_t HiSup              : 1; /**< Hierarchical Support indicator.      */
    uint8_t NormACA            : 1; /**< Normal Auto Contingent Allegiance support.*/
    uint8_t Obsolete1          : 2; /**< Obsolete, expect any value.          */
  };
  uint8_t AdditionalLength;         /**< Number of additional inquiry bytes that follow.*/
  struct
  {
    uint8_t Protect        : 1;     /**< Protection information support.      */
    uint8_t Reserved2      : 2;     /**< Reserved, expect 0.                  */
    uint8_t ThirdPartyCode : 1;     /**< Third Party copy command support.    */
    uint8_t Tpgs           : 2;     /**< Target Port Group Support.           */
    uint8_t Acc            : 1;     /**< Access Controls Coordinator.         */
    uint8_t Sccs           : 1;     /**< SCSI Controller Commands Supported.  */
  };
  struct
  {
    uint8_t Addr16    : 1;          /**< Wide SCSI address support.           */
    uint8_t Obsolete2 : 2;          /**< Obsolete, expect any value.          */
    uint8_t MChngr    : 1;          /**< Medium Changer indicator.            */
    uint8_t MultiP    : 1;          /**< Multi Port indicator.                */
    uint8_t Vs1       : 1;          /**< Vendor specific.                     */
    uint8_t EncServ   : 1;          /**< Enclosure Services support.          */
    uint8_t BQue      : 1;          /**< Full/Basic task management model.    */
  };
  struct
  {
    uint8_t Vs2       : 1;          /**< Vendor specific.                     */
    uint8_t CmdQue    : 1;          /**< Command queueing support.            */
    uint8_t Obsolete3 : 1;          /**< Obsolete, expect any value.          */
    uint8_t Linked    : 1;          /**< Linked commands support.             */
    uint8_t Sync      : 1;          /**< Synchronous data transfer support.   */
    uint8_t Wbus16    : 1;          /**< Wide SCSI data transfer support.     */
    uint8_t Obsolete4 : 2;          /**< Obsolete, expect any value.          */
  };
  uint8_t T10VendorId[ 8 ];         /**< T10 Vendor ID, 8 ASCII codes.        */
  uint8_t ProductId[ 16 ];          /**< Product ID, 16 ASCII codes.          */
  uint8_t ProductRevisionLevel[ 4 ]; /**< Product revision level, 4 ASCII codes.*/
} SL_ATTRIBUTE_PACKED MSDSCSI_InquiryData_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Request Sense Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t OpCode;                 /**< Command opcode.                        */
  struct
  {
    uint8_t Desc      : 1;        /**< Descriptor format.                     */
    uint8_t Reserved1 : 7;        /**< Reserved, expect 0.                    */
  };
  uint8_t Reserved2;              /**< Reserved, expect 0.                    */
  uint8_t Reserved3;              /**< Reserved, expect 0.                    */
  uint8_t AllocationLength;       /**< Number of sense data bytes requested.  */
  uint8_t Control;                /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_RequestSense_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Request Sense response data typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  struct
  {
    uint8_t ResponseCode : 7;     /**< Response code, 0x70 = Current errors.  */
    uint8_t Valid        : 1;     /**< Valid, qualifier for Information field.*/
  };
  uint8_t  Obsolete;              /**< Obsolete, expect any value.            */
  struct
  {
    uint8_t SenseKey : 4;         /**< Generic error information.             */
    uint8_t Reserved : 1;         /**< Reserved, expect 0.                    */
    uint8_t Ili      : 1;         /**< Incorrect length indicator.            */
    uint8_t Eom      : 1;         /**< End of medium indicator.               */
    uint8_t FileMark : 1;         /**< Filemark indicator.                    */
  };
  uint32_t Information;           /**< Information field.                     */
  uint8_t  AdditionalLength;      /**< Number of additional sense bytes that follow.*/
  uint32_t CmdSpecificInfo;       /**< Command specific information field.    */
  uint8_t  Asc;                   /**< Additional sense code.                 */
  uint8_t  Ascq;                  /**< Additional sense code qualifier.       */
  uint8_t  Fruc;                  /**< Field replacable unit code.            */
  struct
  {
    uint8_t SenseKeySpecific1 : 7;  /**< Sense key specific field.            */
    uint8_t Sksv              : 1;  /**< Sense key specific field qualifier.  */
  };
  uint8_t  SenseKeySpecific2;       /**< Sense key specific field.            */
  uint8_t  SenseKeySpecific3;       /**< Sense key specific field.            */
} SL_ATTRIBUTE_PACKED MSDSCSI_RequestSenseData_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Read Capacity Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                  */
  struct
  {
    uint8_t Obsolete  : 1;        /**< Obsolete, expect any value.      */
    uint8_t Reserved1 : 7;        /**< Reserved, expect 0.              */
  };
  uint32_t Lba;                   /**< Logical Block (sector) Address.  */
  uint16_t Reserved2;             /**< Reserved, expect 0.              */
  struct
  {
    uint8_t Pmi       : 1;        /**< Partial medium indicator.        */
    uint8_t Reserved3 : 7;        /**< Reserved, expect 0.              */
  };
  uint8_t  Control;               /**< Control byte.                    */
} SL_ATTRIBUTE_PACKED MSDSCSI_ReadCapacity_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Read Capacity response data typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint32_t LogicalBlockAddress; /**< Last Logical Block (sector) Address on media. */
  uint32_t LogicalBlockLength;  /**< Logical Block (sector) length in bytes.       */
} SL_ATTRIBUTE_PACKED MSDSCSI_ReadCapacityData_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Read 10 Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                        */
  struct
  {
    uint8_t Obsolete  : 1;        /**< Obsolete, expect any value.            */
    uint8_t FuaNv     : 1;        /**< Force Unit Access non-volatile cache.  */
    uint8_t Reserved1 : 1;        /**< Reserved, expect 0.                    */
    uint8_t Fua       : 1;        /**< Force Unit Access.                     */
    uint8_t Dpo       : 1;        /**< Disable Page Out.                      */
    uint8_t RdProtect : 3;        /**< Read Protect field.                    */
  };
  uint32_t Lba;                   /**< Logical Block (sector) Address.        */
  struct
  {
    uint8_t GroupNumber : 5;      /**< Group Number field.                    */
    uint8_t Reserved2   : 3;      /**< Reserved, expect 0.                    */
  };
  uint16_t TransferLength;        /**< Number of blocks (sectors) to transfer.*/
  uint8_t  Control;               /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_Read10_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Write 10 Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                        */
  struct
  {
    uint8_t Obsolete  : 1;        /**< Obsolete, expect any value.            */
    uint8_t FuaNv     : 1;        /**< Force Unit Access non-volatile cache.  */
    uint8_t Reserved1 : 1;        /**< Reserved, expect 0.                    */
    uint8_t Fua       : 1;        /**< Force Unit Access.                     */
    uint8_t Dpo       : 1;        /**< Disable Page Out.                      */
    uint8_t WrProtect : 3;        /**< Write Protect field.                   */
  };
  uint32_t Lba;                   /**< Logical Block (sector) Address.        */
  struct
  {
    uint8_t GroupNumber : 5;      /**< Group Number field.                    */
    uint8_t Reserved2   : 3;      /**< Reserved, expect 0.                    */
  };
  uint16_t TransferLength;        /**< Number of blocks (sectors) to transfer.*/
  uint8_t  Control;               /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_Write10_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Verify 10 Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                        */
  struct
  {
    uint8_t Obsolete  : 1;        /**< Obsolete, expect any value.            */
    uint8_t BytChk    : 1;        /**< Byte check bit.                        */
    uint8_t Reserved1 : 2;        /**< Reserved, expect 0.                    */
    uint8_t Dpo       : 1;        /**< Disable Page Out.                      */
    uint8_t VrProtect : 3;        /**< Verify Protect field.                  */
  };
  uint32_t Lba;                   /**< Logical Block (sector) Address.        */
  struct
  {
    uint8_t GroupNumber : 5;      /**< Group Number field.                    */
    uint8_t Reserved2   : 2;      /**< Reserved, expect 0.                    */
    uint8_t Restricted  : 1;      /**< Treat as a reserved field, expect 0.   */
  };
  uint16_t Verification;          /**< Number of blocks (sectors) to verify  .*/
  uint8_t  Control;               /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_Verify10_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief SCSI Start Stop Unit Command Descriptor Block (CDB) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct
{
  uint8_t  OpCode;                /**< Command opcode.                        */
  struct
  {
    uint8_t Immed     : 1;        /**< Immediate bit.                         */
    uint8_t Reserved1 : 7;        /**< Reserved, expect 0.                    */
  };
  uint8_t Reserved2;              /**< Reserved, expect 0.                    */
  struct
  {
    uint8_t PowerConditionModifier : 4; /**< Additional power condition info. */
    uint8_t Reserved3      : 4;   /**< Reserved, expect 0.                    */
  };
  struct
  {
    uint8_t Start          : 1;   /**< Start bit.                             */
    uint8_t LoEj           : 1;   /**< Load Eject.                            */
    uint8_t No_Flush       : 1;   /**< Dont flush cached data to the medium.  */
    uint8_t Reserved4      : 1;   /**< Reserved, expect 0.                    */
    uint8_t PowerCondition : 4;   /**< Power Condition field.                 */
  };
  uint8_t Control;                /**< Control byte.                          */
} SL_ATTRIBUTE_PACKED MSDSCSI_StartStopUnit_TypeDef;
SL_PACK_END()

/*** MSDSCSI Function prototypes ***/

#if defined(USB_HOST)

bool MSDSCSI_Init(USBH_Ep_TypeDef *out, USBH_Ep_TypeDef *in);
bool MSDSCSI_Inquiry(MSDSCSI_InquiryData_TypeDef *data);
bool MSDSCSI_Read10(uint32_t lba, uint16_t sectors, void *data);
bool MSDSCSI_ReadCapacity(MSDSCSI_ReadCapacityData_TypeDef *data);
bool MSDSCSI_RequestSense(MSDSCSI_RequestSenseData_TypeDef *data);
bool MSDSCSI_TestUnitReady(void);
bool MSDSCSI_Write10(uint32_t lba, uint16_t sectors, const void *data);

#endif

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group Drivers) */

#endif /* __MSDSCSI_H */
