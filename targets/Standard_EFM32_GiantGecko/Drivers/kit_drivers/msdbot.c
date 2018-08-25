/***************************************************************************//**
 * @file  msdbot.c
 * @brief Implements the host side of the Bulk Only Transport protocol for
 *        USB Mass Storage class Devices.
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


#include "em_usb.h"
#include "msdbot.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define TIMEOUT_1SEC       1000
#define TIMEOUT_2SEC       2000
#define DEFAULT_TIMEOUT    TIMEOUT_2SEC

/* Bulk endpoint "handles". */
static USBH_Ep_TypeDef *epOut = NULL;
static USBH_Ep_TypeDef *epIn  = NULL;

/* Bulk transfer timeout scale factor. */
static int timeoutFactor;

/* CSW buffer. */
STATIC_UBUF(csw, CSW_LEN);
static MSDBOT_CSW_TypeDef *pCsw = (MSDBOT_CSW_TypeDef*) csw;

/* Function prototypes. */
static bool CswMeaningful(MSDBOT_CBW_TypeDef *pCbw);
static bool CswValid(MSDBOT_CBW_TypeDef *pCbw);
static void ResetRecovery(void);

/** @endcond */

/***************************************************************************//**
 * @brief
 *   MSDBOT module initialization.
 *
 * @param[in] out
 *   Pointer to an MSD bulk OUT endpoint structure.
 *
 * @param[in] in
 *   Pointer to an MSD bulk IN endpoint structure.
 *
 * @return
 *   @ref MSDBOT_STATUS_OK on success, else @ref MSDBOT_INIT_ERROR.
 ******************************************************************************/
int MSDBOT_Init(USBH_Ep_TypeDef *out, USBH_Ep_TypeDef *in)
{
  /* Check if typedef's are properly packed. */

  if ((sizeof(MSDBOT_CBW_TypeDef) != CBW_LEN) ||
      (sizeof(MSDBOT_CSW_TypeDef) != CSW_LEN))
  {
    DEBUG_USB_API_PUTS("\nMSDBOT_Init(), typedef size error");
    EFM_ASSERT(false);
    return MSDBOT_INIT_ERROR;
  }

  /* Keep a local copy of bulk IN/OUT endpoint handles. */
  epOut = out;
  epIn  = in;

  /* Transfer timeouts are scaled according to device bus speed. */
  if (epIn->parentDevice->speed == PORT_FULL_SPEED)
    timeoutFactor = 512;
  else
    timeoutFactor = 64;

  return MSDBOT_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Perform an MSD Bulk Only Transfer (BOT).
 *
 * @param[in] cbw
 *   Pointer to a Command Block Wrapper (CBW) data structure.
 *
 * @param[in] data
 *   Data buffer for data to be transferred.
 *
 * @return
 *   A positive (or zero) value indicating the number of bytes transferred.
 *   @n A negative value indicates a transfer error code enumerated in
 *   @ref MSDBOT_Status_TypeDef.
 ******************************************************************************/
int MSDBOT_Xfer(void* cbw, void* data)
{
  uint32_t           len;
  int                timeout, result, direction, retVal;
  MSDBOT_CBW_TypeDef *pCbw = (MSDBOT_CBW_TypeDef*) cbw;

  /* Send CBW. */
  result = USBH_WriteB(epOut, cbw, CBW_LEN, DEFAULT_TIMEOUT);

  if (result != CBW_LEN)
  {
    ResetRecovery();
    return MSDBOT_XFER_ERROR;
  }

  retVal    = 0;
  direction = pCbw->Direction;
  len       = pCbw->dCBWDataTransferLength;

  /* Send/receive data (optional phase). */
  if (len)
  {
    timeout = DEFAULT_TIMEOUT + (len / timeoutFactor);

    if (direction)
      result = USBH_ReadB(epIn, data, len, timeout);
    else
      result = USBH_WriteB(epOut, data, len, timeout);

    retVal = result;

    if (result == USB_STATUS_EP_STALLED)
    {
      if (direction)
        USBH_UnStallEpB(epIn);
      else
        USBH_UnStallEpB(epOut);
    }

    else if (result <= 0)
    {
      ResetRecovery();
      return MSDBOT_XFER_ERROR;
    }
  }

  /* Retrieve CSW. */
  result = USBH_ReadB(epIn, csw, CSW_LEN, DEFAULT_TIMEOUT);

  if (result != CSW_LEN)
  {
    if (result == USB_STATUS_EP_STALLED)
    {
      if (direction)
        USBH_UnStallEpB(epIn);
      else
        USBH_UnStallEpB(epOut);

      result = USBH_ReadB(epIn, csw, CSW_LEN, DEFAULT_TIMEOUT);

      if (result != CSW_LEN)
      {
        ResetRecovery();
        return MSDBOT_XFER_ERROR;
      }
    }
    else
    {
      ResetRecovery();
      return MSDBOT_XFER_ERROR;
    }
  }

  if (CswValid(pCbw) && CswMeaningful(pCbw))
  {
    if (pCsw->bCSWStatus == USB_CLASS_MSD_CSW_CMDPASSED)
    {
      return retVal;
    }
    return MSDBOT_CMD_FAILED;
  }

  ResetRecovery();
  return MSDBOT_XFER_ERROR;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Check if a Command Status Wrapper (CSW) is meaningful.
 *   The term "meaningful" is defined in the MSD BOT standard.
 *
 * @param[in] cbw
 *   Pointer to a Command Block Wrapper (CBW) data structure.
 *
 * @return
 *   True if the CSW is meaningful, false otherwise.
 ******************************************************************************/
static bool CswMeaningful(MSDBOT_CBW_TypeDef *cbw)
{
  if (((pCsw->bCSWStatus == USB_CLASS_MSD_CSW_CMDPASSED) ||
       (pCsw->bCSWStatus == USB_CLASS_MSD_CSW_CMDFAILED)) &&
      (pCsw->dCSWDataResidue <= cbw->dCBWDataTransferLength))
    return true;

  return false;
}

/***************************************************************************//**
 * @brief
 *   Check if a Command Status Wrapper (CSW) is valid.
 *   The term "valid" is defined in the MSD BOT standard.
 *
 * @param[in] cbw
 *   Pointer to a Command Block Wrapper (CBW) data structure.
 *
 * @return
 *   True if the CSW is valid, false otherwise.
 ******************************************************************************/
static bool CswValid(MSDBOT_CBW_TypeDef *cbw)
{
  if ((pCsw->dCSWSignature == CSW_SIGNATURE) &&
      (pCsw->dCSWTag == cbw->dCBWTag))
    return true;

  return false;
}

/***************************************************************************//**
 * @brief
 *   Perform an MSD BOT reset recovery operation.
 ******************************************************************************/
static void ResetRecovery(void)
{
  USBH_ControlMsgB(&epIn->parentDevice->ep0,
                   USB_SETUP_DIR_H2D | USB_SETUP_RECIPIENT_INTERFACE |
                   USB_SETUP_TYPE_CLASS_MASK,             /* bmRequestType */
                   USB_MSD_BOTRESET,                      /* bRequest      */
                   0,                                     /* wValue        */
                   0,                                     /* wIndex        */
                   0,                                     /* wLength       */
                   NULL,                                  /* void* data    */
                   TIMEOUT_1SEC);                         /* int timeout   */

  USBH_UnStallEpB(epIn);
  USBH_UnStallEpB(epOut);
}

/** @endcond */
