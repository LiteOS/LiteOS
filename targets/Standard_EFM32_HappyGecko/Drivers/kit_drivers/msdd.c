/**************************************************************************//**
 * @file  msdd.c
 * @brief Mass Storage class Device (MSD) driver.
 * @version 5.0.0
 ******************************************************************************
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
#include "em_cmu.h"
#include "em_gpio.h"
#include "msdbot.h"
#include "msdscsi.h"
#include "msdd.h"
#include "msddmedia.h"

/**************************************************************************//**
 * @addtogroup Msd
 * @{ Implements USB Mass Storage Class (MSC).

@section msdd_intro MSC implementation for device.

   The source code of the device implementation resides in
   kits/common/drivers/msdd.c and msdd.h. The driver includes "msddmedia.h"
   to get the API definitions needed for media access. The drivers use the
   Bulk-Only Transport (BOT) mode of the MSC specification.

@section msdd_config MSC device configuration options.

  This section contains a description of the configuration options for
  the driver. The options are @htmlonly #define's @endhtmlonly which are
  expected to be found in the application "usbconfig.h" header file.

  @verbatim
// USB interface number. Interfaces are numbered from zero to one less than
// the number of concurrent interfaces supported by the configuration.
// The interface number must be 0 for a standalone MSC device, for a
// composite device which includes a MSC interface it must not be in conflict
// with other device interfaces.
#define MSD_INTERFACE_NO ( 0 )

// Endpoint address for data reception.
#define MSD_BULK_OUT ( 0x01 )

// Endpoint address for data transmission.
#define MSD_BULK_IN ( 0x81 )
  @endverbatim
 ** @} ***********************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*** Typedef's and defines. ***/
#define MSD_DIR_DATA_OUT    0
#define MSD_DIR_DATA_IN     1
#define MSD_MAX_BURST       32768U          /* 32 * 1024 */

/**************************************************************************//**
 * @brief MSD device state machine states.
 *****************************************************************************/
typedef enum
{
  MSDD_IDLE                 = 0,
  MSDD_WAITFOR_CBW          = 1,
  MSDD_WAITFOR_RECOVERY     = 2,
  MSDD_SEND_CSW             = 3,
  MSDD_WAIT_FOR_INUNSTALLED = 4,
  MSDD_STALL_IN             = 5,
  MSDD_ACCESS_INDIRECT      = 6,
  MSDD_WRITE_INDIRECT       = 7,
  MSDD_DO_CMD_TASK          = 8,
} msdState_TypeDef;

/*** Function prototypes. ***/

static int            CbwCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
__STATIC_INLINE bool  CswMeaningful(void);
__STATIC_INLINE bool  CswValid(void);
__STATIC_INLINE void  EnableNextCbw(void);
static void           ProcessScsiCdb(void);
__STATIC_INLINE void  SendCsw(void);
static void           UsbXferBotData(uint8_t *data, uint32_t len, USB_XferCompleteCb_TypeDef cb);
static void           XferBotData(uint32_t length);
static int            XferBotDataCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int            XferBotDataIndirectCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

/*** Variables ***/

/* Storage for one CBW */
STATIC_UBUF(cbw, USB_FS_BULK_EP_MAXSIZE);
static MSDBOT_CBW_TypeDef *pCbw = (MSDBOT_CBW_TypeDef*) &cbw;

SL_ALIGN(4)
/* Storage for one CSW */
static MSDBOT_CSW_TypeDef csw SL_ATTRIBUTE_ALIGN(4);
static MSDBOT_CSW_TypeDef *pCsw = &csw;

STATIC_UBUF(mediaBuffer, MEDIA_BUFSIZ);  /* Intermediate media storage buffer */

static MSDD_CmdStatus_TypeDef CmdStatus;
static MSDD_CmdStatus_TypeDef *pCmdStatus = &CmdStatus;
static msdState_TypeDef       savedState; /* MSD state machine state. */
static int ledPort;
static unsigned int ledPin;
static bool turResponse = true;         // Response on TEST UNIT READY command.

/**************************************************************************//**
 * @brief Preformated SCSI INQUIRY response data structure.
 *****************************************************************************/
SL_ALIGN(4)
static const MSDSCSI_InquiryData_TypeDef InquiryData SL_ATTRIBUTE_ALIGN(4) =
{
  { .PeripheralDeviceType = 0, .PeripheralQualifier = 0 }, /* Block device  */
  { .Reserved1            = 0, .Removable           = 1 },

  .Version = 5,                                       /* T10 SPC-3 compliant */

  { .ResponseDataFormat = 2,                          /* T10 SPC-3 compliant reponse data */
    .HiSup = 0, .NormACA = 0, .Obsolete1 = 0 },

  .AdditionalLength = 31,

  { .Protect = 0, .Reserved2 = 0, .ThirdPartyCode = 0,
    .Tpgs    = 0, .Acc       = 0, .Sccs           = 0 },

  { .Addr16 = 0, .Obsolete2 = 0, .MChngr = 0, .MultiP = 0,
    .Vs1    = 0, .EncServ   = 0, .BQue   = 0 },

  { .Vs2  = 0, .CmdQue = 0, .Obsolete3 = 0, .Linked = 0,
    .Sync = 0, .Wbus16 = 0, .Obsolete4 = 0 },

  .T10VendorId          = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
  .ProductId            = { 'E', 'F', 'M', '3', '2', ' ', 'M', 'S', 'D', ' ', 'D', 'e', 'v', 'i', 'c', 'e' },
  .ProductRevisionLevel ={ '1', '.', '0', '0' }
};

/**************************************************************************//**
 * @brief
 *   Preformated SCSI REQUEST SENSE response data structure.
 *   Used when no error condition exists in the SCSI device server.
 *****************************************************************************/
SL_ALIGN(4)
static const MSDSCSI_RequestSenseData_TypeDef NoSenseData SL_ATTRIBUTE_ALIGN(4) =
{
  { .ResponseCode = 0x70, .Valid = 0 },
  .Obsolete = 0,
  { .SenseKey = 0, .Reserved =0, .Ili = 0, .Eom = 0, .FileMark = 0 },
  .Information      = 0,
  .AdditionalLength = 10,
  .CmdSpecificInfo  = 0,
  .Asc              = 0,
  .Ascq             = 0,
  .Fruc             = 0,
  { .SenseKeySpecific1 = 0, .Sksv = 0 },
  .SenseKeySpecific2 = 0,
  .SenseKeySpecific3 = 0
};

/**************************************************************************//**
 * @brief
 *   Preformated SCSI REQUEST SENSE response data structure.
 *   Used when host has issued an unsupported SCSI command, or when an
 *   invalid field in a SCSI command descriptor block (CDB) is detected.
 *****************************************************************************/
SL_ALIGN(4)
static const MSDSCSI_RequestSenseData_TypeDef IllegalSenseData SL_ATTRIBUTE_ALIGN(4) =
{
  { .ResponseCode = 0x70, .Valid = 0 },
  .Obsolete = 0,
  { .SenseKey = 5,                          /* SensKey = 5 => ILLEGAL REQUEST */
    .Reserved = 0, .Ili = 0, .Eom = 0, .FileMark = 0 },
  .Information      = 0,
  .AdditionalLength = 10,
  .CmdSpecificInfo  = 0,
  .Asc              = 0x24,    /* Asc/Ascq = 0x24/0x00 => INVALID FIELD IN CDB*/
  .Ascq             = 0,
  .Fruc             = 0,
  { .SenseKeySpecific1 = 0, .Sksv  = 0 },
  .SenseKeySpecific2 = 0,
  .SenseKeySpecific3 = 0
};

static volatile msdState_TypeDef        msdState;     /**< BOT statemachine state enumaration. */
static MSDSCSI_RequestSenseData_TypeDef *pSenseData;  /**< Points to current sense data.       */

/** @endcond */

/**************************************************************************//**
 * @brief Initialize MSD device.
 *
 * @param[in] activityLedPort
 *   Specify a GPIO port for a LED activity indicator (i.e. enum gpioPortX)
 *   Pass -1 if no indicator LED is available.
 *
 * @param[in] activityLedPin
 *   Pin number on activityLedPort for the LED activity indicator.
 *****************************************************************************/
void MSDD_Init(int activityLedPort, uint32_t activityLedPin)
{
  if ( ( sizeof(MSDSCSI_Read10_TypeDef)           != SCSI_READ10_LEN           ) ||
       ( sizeof(MSDSCSI_Write10_TypeDef)          != SCSI_WRITE10_LEN          ) ||
       ( sizeof(MSDSCSI_Verify10_TypeDef)         != SCSI_VERIFY10_LEN         ) ||
       ( sizeof(MSDSCSI_RequestSense_TypeDef)     != SCSI_REQUESTSENSE_LEN     ) ||
       ( sizeof(InquiryData)                      != SCSI_INQUIRYDATA_LEN      ) ||
       ( sizeof(NoSenseData)                      != SCSI_REQUESTSENSEDATA_LEN ) ||
       ( sizeof(IllegalSenseData)                 != SCSI_REQUESTSENSEDATA_LEN ) ||
       ( sizeof(MSDSCSI_ReadCapacity_TypeDef)     != SCSI_READCAPACITY_LEN     ) ||
       ( sizeof(MSDSCSI_ReadCapacityData_TypeDef) != SCSI_READCAPACITYDATA_LEN ) ||
       ( sizeof(MSDSCSI_StartStopUnit_TypeDef)    != SCSI_STARTSTOPUNIT_LEN    )    )
  {
    DEBUG_USB_API_PUTS("\nMSDD_Init(), typedef size error");
    EFM_ASSERT(false);
    return;
  }

  if ( ( activityLedPort >= gpioPortA ) && ( activityLedPort <= gpioPortF ) )
    ledPort = activityLedPort;
  else
    ledPort = -1;

  ledPin     = activityLedPin;
  msdState   = MSDD_IDLE;
  pSenseData = (MSDSCSI_RequestSenseData_TypeDef*) &NoSenseData;

  if ( ledPort != -1 )
  {
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet((GPIO_Port_TypeDef)ledPort, ledPin, gpioModePushPull, 0);
  }
}

/**************************************************************************//**
 * @brief
 *   Serve the MSD state machine.
 *   This function should be called on a regular basis from your main loop.
 *   It cannot be called from within an interrupt handler.
 * @return
 *   Returns true if there is no pending tasks to perform. This means that
 *   energymodes (sleep) functionality can be used.
 *****************************************************************************/
bool MSDD_Handler(void)
{
  static uint32_t len;        /* Note: len is static ! */

  switch (msdState)
  {
  case MSDD_ACCESS_INDIRECT:
    if (pCmdStatus->xferLen)
    {
      len = SL_MIN(pCmdStatus->xferLen, pCmdStatus->maxBurst);

      msdState = MSDD_IDLE;
      if (pCmdStatus->direction)
      {
        MSDDMEDIA_Read(pCmdStatus, mediaBuffer, len / 512);
      }
      UsbXferBotData(mediaBuffer, len, XferBotDataIndirectCallback);
    }
    else
    {
      /* We are done ! */
      msdState = savedState;

      if (msdState == MSDD_SEND_CSW)
      {
        SendCsw();
        EnableNextCbw();
        msdState = MSDD_WAITFOR_CBW;
      }

      else if (msdState == MSDD_STALL_IN)
      {
        USBD_StallEp(MSD_BULK_IN);
        msdState = MSDD_WAIT_FOR_INUNSTALLED;
      }
    }
    break;

  case MSDD_WRITE_INDIRECT:
    MSDDMEDIA_Write(pCmdStatus, mediaBuffer, len / 512);
    pCmdStatus->lba += len / 512;
    msdState         = MSDD_ACCESS_INDIRECT;
    break;

  case MSDD_DO_CMD_TASK:
    if (pCbw->CBWCB[ 0 ] == SCSI_STARTSTOP_UNIT)
    {
      MSDDMEDIA_Flush();
    }
    /* else if ( .... )  Add more when needed. */
    SendCsw();
    EnableNextCbw();
    msdState = MSDD_WAITFOR_CBW;
    break;

  default:
    break;
  }
  return (msdState == MSDD_WAITFOR_CBW) || (msdState == MSDD_IDLE);
}

/**************************************************************************//**
 * @brief
 *   Called whenever a USB setup command is received.
 *   This function overrides standard CLEAR_FEATURE commands, and implements
 *   MSD class commands "Bulk-Only Mass Storage Reset" and "Get Max LUN".
 *
 * @param[in] setup
 *  Pointer to an USB setup packet.
 *
 * @return
 *  An appropriate status/error code. See USB_Status_TypeDef.
 *****************************************************************************/
int MSDD_SetupCmd(const USB_Setup_TypeDef *setup)
{
  int retVal;
  static uint32_t tmp;

  retVal = USB_STATUS_REQ_UNHANDLED;

  /* Check if it is MSD class command: "Bulk-Only Mass Storage Reset" */

  if ( ( setup->Type      == USB_SETUP_TYPE_CLASS          ) &&
       ( setup->Direction == USB_SETUP_DIR_OUT             ) &&
       ( setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE ) &&
       ( setup->bRequest  == USB_MSD_BOTRESET              ) &&
       ( setup->wValue    == 0                             ) &&
       ( setup->wIndex    == MSD_INTERFACE_NO              ) &&
       ( setup->wLength   == 0                             )    )
  {
    if (msdState == MSDD_WAITFOR_RECOVERY)
    {
      msdState = MSDD_IDLE;
    }
    retVal = USB_STATUS_OK;
  }


  /* Check if it is MSD class command: "Get Max LUN" */

  else if ( ( setup->Type      == USB_SETUP_TYPE_CLASS          ) &&
            ( setup->Direction == USB_SETUP_DIR_IN              ) &&
            ( setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE ) &&
            ( setup->bRequest  == USB_MSD_GETMAXLUN             ) &&
            ( setup->wValue    == 0                             ) &&
            ( setup->wIndex    == MSD_INTERFACE_NO              ) &&
            ( setup->wLength   == 1                             )    )
  {
    /* Only one LUN (i.e. no support for multiple LUN's). Reply "0". */
    tmp    = 0;
    retVal = USBD_Write(0, (void*) &tmp, 1, NULL);
  }


  /* Check if it is a standard CLEAR_FEATURE endpoint command */

  else if ( ( setup->Type      == USB_SETUP_TYPE_STANDARD      ) &&
            ( setup->Direction == USB_SETUP_DIR_OUT            ) &&
            ( setup->Recipient == USB_SETUP_RECIPIENT_ENDPOINT ) &&
            ( setup->bRequest  == CLEAR_FEATURE                ) &&
            ( setup->wValue    == USB_FEATURE_ENDPOINT_HALT    ) &&
            ( setup->wLength   == 0                            )    )
  {
    if ( ( ( setup->wIndex & 0xFF) == MSD_BULK_OUT ) ||
         ( ( setup->wIndex & 0xFF) == MSD_BULK_IN  )    )
    {
      retVal = USB_STATUS_OK;

      /* Dont unstall ep's when waiting for reset recovery */
      if (msdState != MSDD_WAITFOR_RECOVERY)
      {
        retVal = USBD_UnStallEp(setup->wIndex & 0xFF);

        if ((setup->wIndex & 0xFF) == MSD_BULK_IN)
        {
          if (msdState == MSDD_WAIT_FOR_INUNSTALLED)
          {
            SendCsw();
            EnableNextCbw();
            msdState = MSDD_WAITFOR_CBW;
          }
        }
        else
        {
          EnableNextCbw();
          msdState = MSDD_WAITFOR_CBW;
        }
      }
    }
  }

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Called whenever the USB device has changed its device state.
 *
 * @param[in] oldState
 *   The device USB state just leaved. See USBD_State_TypeDef.
 *
 * @param[in] newState
 *   New (the current) USB device state. See USBD_State_TypeDef.
 *****************************************************************************/
void MSDD_StateChangeEvent( USBD_State_TypeDef oldState,
                            USBD_State_TypeDef newState )
{
  if (newState == USBD_STATE_CONFIGURED)
  {
    /* We have been configured, start MSD functionality ! */
    EnableNextCbw();
    msdState = MSDD_WAITFOR_CBW;
    turResponse = true;           // Set TEST UNIT READY response value.
  }

  else if ((oldState == USBD_STATE_CONFIGURED) &&
           (newState != USBD_STATE_SUSPENDED))
  {
    /* We have been de-configured */
    msdState = MSDD_IDLE;
  }

  else if (newState == USBD_STATE_SUSPENDED)
  {
    /* We have been suspended.                     */
    msdState = MSDD_IDLE;

    /* Reduce current consumption to below 2.5 mA. */
  }
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/**************************************************************************//**
 * @brief
 *   Called on USB transfer completion callback. Will qualify and parse a
 *   Command Block Wrapper (CBW).
 *
 * @param[in] status
 *   The transfer status.
 *
 * @param[in] xferred
 *   Number of bytes actually transferred.
 *
 * @param[in] remaining
 *   Number of bytes not transferred.
 *
 * @return
 *   USB_STATUS_OK.
 *****************************************************************************/
static int CbwCallback(USB_Status_TypeDef status,
                       uint32_t xferred, uint32_t remaining)
{
  (void) remaining;

  if ( ( msdState == MSDD_WAITFOR_CBW ) &&
       ( status   == USB_STATUS_OK    ) &&
       ( xferred  == CBW_LEN          ) &&
       ( CswValid()                   ) &&
       ( CswMeaningful()              )    )
  {
    if ( ledPort != -1 )
      GPIO_PinOutToggle((GPIO_Port_TypeDef)ledPort, ledPin);

    /* Check the SCSI command descriptor block (CDB) */
    ProcessScsiCdb();

    if (pCmdStatus->valid)
      pCsw->bCSWStatus = USB_CLASS_MSD_CSW_CMDPASSED;
    else
      pCsw->bCSWStatus = USB_CLASS_MSD_CSW_CMDFAILED;

    pCsw->dCSWSignature   = CSW_SIGNATURE;
    pCsw->dCSWTag         = pCbw->dCBWTag;
    pCsw->dCSWDataResidue = pCbw->dCBWDataTransferLength;

    /* Check the "thirteen cases" */

    if ((pCbw->dCBWDataTransferLength != 0) &&
        (pCbw->Direction != pCmdStatus->direction))
    {
      /* Handle cases 8 and 10 */
      pCsw->bCSWStatus = USB_CLASS_MSD_CSW_PHASEERROR;

      if (pCbw->Direction)
      {
        /* Host expects to receive data, case 8 */
        USBD_StallEp(MSD_BULK_IN);
        msdState = MSDD_WAIT_FOR_INUNSTALLED;
      }
      else
      {
        /* Host expects to send data, case 10 */
        USBD_StallEp(MSD_BULK_OUT);
        SendCsw();
        msdState = MSDD_IDLE;
      }
    }

    else if (pCbw->Direction || (pCbw->dCBWDataTransferLength == 0))
    {
      /* SCSI IN commands or commands without data phase */
      /* Handle cases 1-7 */

      if (pCbw->dCBWDataTransferLength == 0)
      {
        /* Host expects no data, case 1, 2 or 3 */
        if (pCmdStatus->xferLen)
        {
          /* Device has data to transmit, case 2 & 3 */
          pCsw->bCSWStatus = USB_CLASS_MSD_CSW_PHASEERROR;
        }

        if ((pCmdStatus->xferLen == 0) &&
            (pCmdStatus->xferType == XFER_INDIRECT))
        {
          /* Commands with no data phase which require timeconsuming  */
          /* processing are executed in MSDD_Handler()                */
          msdState = MSDD_DO_CMD_TASK;
        }
        else
        {
          SendCsw();
          EnableNextCbw();
          msdState = MSDD_WAITFOR_CBW;
        }
      }
      else if (pCbw->dCBWDataTransferLength == pCmdStatus->xferLen)
      {
        /* Host and device agree on transferlength, case 6 */
        /* Send data to host */
        msdState = MSDD_SEND_CSW;
        XferBotData(pCmdStatus->xferLen);
      }
      else if (pCbw->dCBWDataTransferLength > pCmdStatus->xferLen)
      {
        /* Host expects more data than device can provide, case 4 and 5 */

        if (pCmdStatus->xferLen > 0)
        {
          /* Device has data, case 5 */
          /* Send data to host */
          msdState = MSDD_STALL_IN;
          XferBotData(pCmdStatus->xferLen);
        }
        else
        {
          /* Device has no data, case 4 */
          USBD_StallEp(MSD_BULK_IN);
          msdState = MSDD_WAIT_FOR_INUNSTALLED;
        }
      }
      else
      {
        /* Host expects less data than device will provide, case 7 */
        pCsw->bCSWStatus = USB_CLASS_MSD_CSW_PHASEERROR;
        /* Send data to host */
        msdState = MSDD_SEND_CSW;
        XferBotData(pCbw->dCBWDataTransferLength);
      }
    }

    else /* Host Direction is OUT and Host transferlength > 0 */
    {
      /* SCSI OUT commands */
      /* Handle cases 9, 11, 12 and 13 */

      if (pCbw->dCBWDataTransferLength == pCmdStatus->xferLen)
      {
        /* Host and device agree on transferlength, case 12 */

        /* Read data from host */
        msdState = MSDD_SEND_CSW;
        XferBotData(pCmdStatus->xferLen);
      }
      else if (pCbw->dCBWDataTransferLength > pCmdStatus->xferLen)
      {
        /* Host intend to send more data than device expects, case 9 & 11 */
        pCsw->bCSWStatus = USB_CLASS_MSD_CSW_CMDFAILED;
        USBD_StallEp(MSD_BULK_OUT);
        SendCsw();
        msdState = MSDD_IDLE;
      }
      else
      {
        /* Host has less data than device expects to receive, case 13 */
        pCsw->bCSWStatus = USB_CLASS_MSD_CSW_PHASEERROR;
        USBD_StallEp(MSD_BULK_OUT);
        SendCsw();
        msdState = MSDD_IDLE;
      }
    }
    return USB_STATUS_OK;
  }

  if ((status == USB_STATUS_OK) &&
      (USBD_GetUsbState() == USBD_STATE_CONFIGURED))
  {
    /* Stall both Ep's and wait for reset recovery */
    USBD_StallEp(MSD_BULK_OUT);
    USBD_StallEp(MSD_BULK_IN);
    msdState = MSDD_WAITFOR_RECOVERY;
  }

  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   Check if a Command Block Wrapper (CBW) is meaningful.
 *   The term "meaningful" is defined in the USB Mass Storage Class BOT spec.
 *****************************************************************************/
__STATIC_INLINE bool CswMeaningful(void)
{
  if ( ( pCbw->Reserved1 == 0 ) &&
       ( pCbw->Obsolete  == 0 ) &&
       ( pCbw->Reserved2 == 0 ) &&
       ( pCbw->Lun       == 0 ) &&
       ( pCbw->Reserved3 == 0 )    )
  {
    return true;
  }

  return false;
}

/**************************************************************************//**
 * @brief
 *   Check if a Command Block Wrapper (CBW) is valid.
 *   The term "valid" is defined in the USB Mass Storage Class BOT spec.
 *****************************************************************************/
__STATIC_INLINE bool CswValid(void)
{
  return pCbw->dCBWSignature == CBW_SIGNATURE ? true : false; /* Ascii USBC */
}

/**************************************************************************//**
 * @brief
 *   Start a USB bulk-out transfer to get next CBW.
 *****************************************************************************/
__STATIC_INLINE void EnableNextCbw(void)
{
  USBD_Read(MSD_BULK_OUT, (void*) &cbw, USB_FS_BULK_EP_MAXSIZE, CbwCallback);
}

/**************************************************************************//**
 * @brief
 *   Parse a SCSI command.
 *   A minimal, yet sufficient SCSI command subset is supported.
 *****************************************************************************/
static void ProcessScsiCdb(void)
{
  MSDSCSI_Inquiry_TypeDef       *cbI;
  MSDSCSI_RequestSense_TypeDef  *cbRS;
  MSDSCSI_ReadCapacity_TypeDef  *cbRC;
  MSDSCSI_Read10_TypeDef        *cbR10;
  MSDSCSI_Write10_TypeDef       *cbW10;
  MSDSCSI_Verify10_TypeDef      *cbV10;
  MSDSCSI_StartStopUnit_TypeDef *cbSSU;

  SL_ALIGN(4)
  static MSDSCSI_ReadCapacityData_TypeDef ReadCapData SL_ATTRIBUTE_ALIGN(4);

  pCmdStatus->valid    = false;
  pCmdStatus->xferType = XFER_MEMORYMAPPED;
  pCmdStatus->maxBurst = MSD_MAX_BURST;

  switch (pCbw->CBWCB[ 0 ])
  {
  case SCSI_INQUIRY:
    cbI = (MSDSCSI_Inquiry_TypeDef*) &pCbw->CBWCB;

    if ((cbI->Evpd == 0) && (cbI->PageCode == 0))
    {
      /* Standard Inquiry data request */
      pCmdStatus->valid     = true;
      pCmdStatus->direction = MSD_DIR_DATA_IN;
      pCmdStatus->pData     = (uint8_t*) &InquiryData;
      pCmdStatus->xferLen   = SL_MIN(SCSI_INQUIRYDATA_LEN,
                                     __REV16(cbI->AllocationLength));
    }
    break;

  case SCSI_REQUESTSENSE:
    cbRS = (MSDSCSI_RequestSense_TypeDef*) &pCbw->CBWCB;

    if ((cbRS->Desc == 0) && (cbRS->Reserved1 == 0) &&
        (cbRS->Reserved2 == 0) && (cbRS->Reserved3 == 0))
    {
      pCmdStatus->valid     = true;
      pCmdStatus->direction = MSD_DIR_DATA_IN;
      pCmdStatus->pData     = (uint8_t*) pSenseData;
      pCmdStatus->xferLen   = SL_MIN(SCSI_REQUESTSENSEDATA_LEN,
                                     cbRS->AllocationLength);
      pSenseData = (MSDSCSI_RequestSenseData_TypeDef*) &NoSenseData;
    }
    break;

  case SCSI_READCAPACITY:
    cbRC = (MSDSCSI_ReadCapacity_TypeDef*) &pCbw->CBWCB;

    if ((cbRC->Pmi == 0) && (cbRC->Lba == 0))
    {
      ReadCapData.LogicalBlockAddress = __REV(MSDDMEDIA_GetSectorCount() - 1);
      ReadCapData.LogicalBlockLength  = __REV(512);

      pCmdStatus->valid     = true;
      pCmdStatus->direction = MSD_DIR_DATA_IN;
      pCmdStatus->pData     = (uint8_t*) &ReadCapData;
      pCmdStatus->xferLen   = SCSI_READCAPACITYDATA_LEN;
    }
    break;

  case SCSI_READ10:
    cbR10 = (MSDSCSI_Read10_TypeDef*) &pCbw->CBWCB;

    pCmdStatus->direction = MSD_DIR_DATA_IN;
    pCmdStatus->valid     = MSDDMEDIA_CheckAccess(pCmdStatus,
                                                  __REV(cbR10->Lba),
                                                  __REV16(cbR10->TransferLength));
    break;

  case SCSI_WRITE10:
    cbW10 = (MSDSCSI_Write10_TypeDef*) &pCbw->CBWCB;

    pCmdStatus->direction = MSD_DIR_DATA_OUT;
    pCmdStatus->valid     = MSDDMEDIA_CheckAccess(pCmdStatus,
                                                  __REV(cbW10->Lba),
                                                  __REV16(cbW10->TransferLength));
    break;

  case SCSI_VERIFY10:
    cbV10 = (MSDSCSI_Verify10_TypeDef*) &pCbw->CBWCB;

    if ((cbV10->BytChk      == 0) && (cbV10->Reserved1 == 0) &&
        (cbV10->Dpo         == 0) && (cbV10->VrProtect == 0) &&
        (cbV10->GroupNumber == 0) && (cbV10->Reserved2 == 0) &&
        (cbV10->Restricted  == 0))
    {
      pCmdStatus->valid     = true;
      pCmdStatus->direction = pCbw->Direction;
      pCmdStatus->xferLen   = 0;
    }
    break;

  case SCSI_TESTUNIT_READY:
    pCmdStatus->valid     = turResponse;
    pCmdStatus->direction = pCbw->Direction;
    pCmdStatus->xferLen   = 0;
    return;

  case SCSI_STARTSTOP_UNIT:
    cbSSU = (MSDSCSI_StartStopUnit_TypeDef*) &pCbw->CBWCB;
    if ((cbSSU->Reserved1      == 0) && (cbSSU->Reserved2 == 0) &&
        (cbSSU->Reserved3      == 0) && (cbSSU->Reserved4 == 0) &&
        (cbSSU->PowerCondition == 0) && (cbSSU->LoEj      == 1) &&
        (cbSSU->Start          == 0))
    {
      // Eject media.
      turResponse = false;              // Set TEST UNIT READY response value.
    }
    pCmdStatus->valid     = true;
    pCmdStatus->direction = pCbw->Direction;
    pCmdStatus->xferLen   = 0;
    pCmdStatus->xferType  = XFER_INDIRECT;
    break;
  }

  if (!pCmdStatus->valid)
  {
    pCmdStatus->xferLen   = 0;
    pCmdStatus->direction = pCbw->Direction;
    pSenseData            = (MSDSCSI_RequestSenseData_TypeDef*) &IllegalSenseData;
  }
}

/**************************************************************************//**
 * @brief
 *   Start a USB bulk-in transfer to send a CSW back to host.
 *****************************************************************************/
__STATIC_INLINE void SendCsw(void)
{
  if ( ledPort != -1 )
    GPIO_PinOutToggle((GPIO_Port_TypeDef)ledPort, ledPin);

  USBD_Write(MSD_BULK_IN, (void*) &csw, CSW_LEN, NULL);
}

/**************************************************************************//**
 * @brief
 *   Start a USB bulk-in or bulk-out transfer to transfer a data payload
 *   to/from host.
 *
 * @param[in] data
 *   Pointer to data payload.
 *
 * @param[in] len
 *   Number of bytes to transfer.
 *
 * @param[in] cb
 *   Transfer completion callback.
 *****************************************************************************/
static void UsbXferBotData(uint8_t *data, uint32_t len,
                           USB_XferCompleteCb_TypeDef cb)
{
  if (pCmdStatus->direction)
  {
    USBD_Write(MSD_BULK_IN, data, len, cb);
  }
  else
  {
    USBD_Read(MSD_BULK_OUT, data, len, cb);
  }
}

/**************************************************************************//**
 * @brief
 *   Start a USB bulk-in or bulk-out transfer to transfer a data payload
 *   to/from host according to the transfer mode of the transfer.
 *
 * @param[in] len
 *   Number of bytes to transfer.
 *****************************************************************************/
static void XferBotData(uint32_t length)
{
  pCmdStatus->xferLen   = length;
  pCsw->dCSWDataResidue = pCbw->dCBWDataTransferLength;

  if (pCmdStatus->xferType == XFER_INDIRECT)
  {
    /* Access media in "background" polling loop, i.e. in MSDD_Handler() */
    savedState = msdState;
    msdState   = MSDD_ACCESS_INDIRECT;
  }
  else
  {
    UsbXferBotData(pCmdStatus->pData,
                   SL_MIN(length, pCmdStatus->maxBurst),
                   XferBotDataCallback);
  }
}

/**************************************************************************//**
 * @brief
 *   Called on USB transfer completion callback for memory-mapped transfers.
 *   Will initiate a new transfer if there is more data available.
 *   If all data has been sent it will either send a CSW or stall the bulk-in
 *   endpoint if needed.
 *
 * @param[in] status
 *   The transfer status.
 *
 * @param[in] xferred
 *   Number of bytes actually transferred.
 *
 * @param[in] remaining
 *   Number of bytes not transferred.
 *
 * @return
 *   USB_STATUS_OK.
 *****************************************************************************/
static int XferBotDataCallback(USB_Status_TypeDef status,
                               uint32_t xferred, uint32_t remaining)
{
  (void) status;
  (void) remaining;

  pCmdStatus->xferLen   -= xferred;
  pCsw->dCSWDataResidue -= xferred;

  if (pCmdStatus->xferLen)
  {
    pCmdStatus->pData += xferred;
    UsbXferBotData(pCmdStatus->pData,
                   SL_MIN(pCmdStatus->xferLen, pCmdStatus->maxBurst),
                   XferBotDataCallback);
  }
  else
  {
    if (msdState == MSDD_SEND_CSW)
    {
      SendCsw();
      EnableNextCbw();
      msdState = MSDD_WAITFOR_CBW;
    }

    else if (msdState == MSDD_STALL_IN)
    {
      USBD_StallEp(MSD_BULK_IN);
      msdState = MSDD_WAIT_FOR_INUNSTALLED;
    }
  }

  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   Called on USB transfer completion callback for indirect access media.
 *   Signals MSD state change back to MSDD_Handler().
 *
 * @param[in] status
 *   The transfer status.
 *
 * @param[in] xferred
 *   Number of bytes actually transferred.
 *
 * @param[in] remaining
 *   Number of bytes not transferred.
 *
 * @return
 *   USB_STATUS_OK.
 *****************************************************************************/
static int XferBotDataIndirectCallback(USB_Status_TypeDef status,
                                       uint32_t xferred, uint32_t remaining)
{
  (void) status;
  (void) remaining;

  pCmdStatus->xferLen   -= xferred;
  pCsw->dCSWDataResidue -= xferred;

  if (pCmdStatus->direction)
  {
    pCmdStatus->lba += xferred / 512;
    msdState         = MSDD_ACCESS_INDIRECT;
  }
  else
  {
    msdState = MSDD_WRITE_INDIRECT;
  }

  return USB_STATUS_OK;
}

/** @endcond */
