/***************************************************************************//**
 * @file  msdd.h
 * @brief Mass Storage class Device (MSD) driver.
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
#ifndef __MSDD_H
#define __MSDD_H

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

#define MEDIA_BUFSIZ    4096      /**< Intermediate media storage buffer size */

/**************************************************************************//**
 * @brief Status info for one BOT CBW -> Data I/O -> CSW cycle.
 *****************************************************************************/
typedef struct
{
  bool        valid;     /**< True if the CBW is valid.    */
  uint8_t     direction; /**< Set if BOT direction is IN.  */
  uint8_t     *pData;    /**< Media data pointer.          */
  uint32_t    lba;       /**< SCSI Read/Write lba address. */
  uint32_t    xferLen;   /**< BOT transfer length.         */
  uint32_t    maxBurst;  /**< Max length of one transfer.  */
  enum { XFER_MEMORYMAPPED = 0, XFER_INDIRECT } xferType;
  /**< MSD media access type.       */
} MSDD_CmdStatus_TypeDef;

/*** MSDD Device Driver Function prototypes ***/

bool MSDD_Handler(void);
void MSDD_Init(int activityLedPort, uint32_t activityLedPin);
int  MSDD_SetupCmd(const USB_Setup_TypeDef *setup);
void MSDD_StateChangeEvent(USBD_State_TypeDef oldState,
                           USBD_State_TypeDef newState);

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group Drivers) */

#endif /* __MSDD_H */
