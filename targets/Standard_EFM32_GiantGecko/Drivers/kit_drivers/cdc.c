/**************************************************************************//**
 * @file cdc.c
 * @brief USB Communication Device Class (CDC) driver.
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
#include "em_device.h"
#include "em_common.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_dma.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_usb.h"
#include "bsp.h"
#include "dmactrl.h"
#include "cdc.h"

/**************************************************************************//**
 * @addtogroup Cdc
 * @{ Implements USB Communication Device Class (CDC).

@section cdc_intro CDC implementation.

   The source code of the CDC implementation resides in
   kits/common/drivers/cdc.c and cdc.h. This driver implements a basic
   USB to RS232 bridge.

@section cdc_config CDC device configuration options.

  This section contains a description of the configuration options for
  the driver. The options are @htmlonly #define's @endhtmlonly which are
  expected to be found in the application "usbconfig.h" header file.
  The values shown below are from the Giant Gecko DK3750 CDC example.

  @verbatim
// USB interface numbers. Interfaces are numbered from zero to one less than
// the number of concurrent interfaces supported by the configuration. A CDC
// device is by itself a composite device and has two interfaces.
// The interface numbers must be 0 and 1 for a standalone CDC device, for a
// composite device which includes a CDC interface it must not be in conflict
// with other device interfaces.
#define CDC_CTRL_INTERFACE_NO ( 0 )
#define CDC_DATA_INTERFACE_NO ( 1 )

// Endpoint address for CDC data reception.
#define CDC_EP_DATA_OUT ( 0x01 )

// Endpoint address for CDC data transmission.
#define CDC_EP_DATA_IN ( 0x81 )

// Endpoint address for the notification endpoint (not used).
#define CDC_EP_NOTIFY ( 0x82 )

// Timer id, see USBTIMER in the USB device stack documentation.
// The CDC driver has a Rx timeout functionality which require a timer.
#define CDC_TIMER_ID ( 0 )

// DMA related macros, select DMA channels and DMA request signals.
#define CDC_UART_TX_DMA_CHANNEL   ( 0 )
#define CDC_UART_RX_DMA_CHANNEL   ( 1 )
#define CDC_TX_DMA_SIGNAL         DMAREQ_UART1_TXBL
#define CDC_RX_DMA_SIGNAL         DMAREQ_UART1_RXDATAV

// UART/USART selection macros.
#define CDC_UART                  UART1
#define CDC_UART_CLOCK            cmuClock_UART1
#define CDC_UART_ROUTE            ( UART_ROUTE_RXPEN | UART_ROUTE_TXPEN | \
                                    UART_ROUTE_LOCATION_LOC2 )
#define CDC_UART_TX_PORT          gpioPortB
#define CDC_UART_TX_PIN           9
#define CDC_UART_RX_PORT          gpioPortB
#define CDC_UART_RX_PIN           10

// Activate the RS232 switch on DK's.
#define CDC_ENABLE_DK_UART_SWITCH() BSP_PeripheralAccess(BSP_RS232_UART, true)

// No RS232 switch on STK's. Leave the definition "empty".
#define CDC_ENABLE_DK_UART_SWITCH()

  @endverbatim
 ** @} ***********************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*** Typedef's and defines. ***/

#define CDC_BULK_EP_SIZE  (USB_FS_BULK_EP_MAXSIZE) /* This is the max. ep size.    */
#define CDC_USB_RX_BUF_SIZ  CDC_BULK_EP_SIZE /* Packet size when receiving on USB. */
#define CDC_USB_TX_BUF_SIZ  127    /* Packet size when transmitting on USB.  */

/* Calculate a timeout in ms corresponding to 5 char times on current     */
/* baudrate. Minimum timeout is set to 10 ms.                             */
#define CDC_RX_TIMEOUT    SL_MAX(10U, 50000 / (cdcLineCoding.dwDTERate))

/* The serial port LINE CODING data structure, used to carry information  */
/* about serial port baudrate, parity etc. between host and device.       */
SL_PACK_START(1)
typedef struct
{
  uint32_t dwDTERate;               /** Baudrate                            */
  uint8_t  bCharFormat;             /** Stop bits, 0=1 1=1.5 2=2            */
  uint8_t  bParityType;             /** 0=None 1=Odd 2=Even 3=Mark 4=Space  */
  uint8_t  bDataBits;               /** 5, 6, 7, 8 or 16                    */
  uint8_t  dummy;                   /** To ensure size is a multiple of 4 bytes */
} SL_ATTRIBUTE_PACKED cdcLineCoding_TypeDef;
SL_PACK_END()


/*** Function prototypes. ***/

static int  UsbDataReceived(USB_Status_TypeDef status, uint32_t xferred,
                            uint32_t remaining);
static void DmaSetup(void);
static int  LineCodingReceived(USB_Status_TypeDef status,
                               uint32_t xferred,
                               uint32_t remaining);
static void SerialPortInit(void);
static void UartRxTimeout(void);

/*** Variables ***/

/*
 * The LineCoding variable must be 4-byte aligned as it is used as USB
 * transmit and receive buffer.
 */
SL_ALIGN(4)
SL_PACK_START(1)
static cdcLineCoding_TypeDef SL_ATTRIBUTE_ALIGN(4) cdcLineCoding =
{
  115200, 0, 0, 8, 0
};
SL_PACK_END()

STATIC_UBUF(usbRxBuffer0,  CDC_USB_RX_BUF_SIZ);   /* USB receive buffers.   */
STATIC_UBUF(usbRxBuffer1,  CDC_USB_RX_BUF_SIZ);
STATIC_UBUF(uartRxBuffer0, CDC_USB_TX_BUF_SIZ);   /* UART receive buffers.  */
STATIC_UBUF(uartRxBuffer1, CDC_USB_TX_BUF_SIZ);

static const uint8_t  *usbRxBuffer[  2 ] = { usbRxBuffer0, usbRxBuffer1 };
static const uint8_t  *uartRxBuffer[ 2 ] = { uartRxBuffer0, uartRxBuffer1 };

static int            usbRxIndex, usbBytesReceived;
static int            uartRxIndex, uartRxCount;
static int            LastUsbTxCnt;

static bool           dmaRxCompleted;
static bool           usbRxActive, dmaTxActive;
static bool           usbTxActive, dmaRxActive;

static DMA_CB_TypeDef DmaTxCallBack;    /** DMA callback structures. */
static DMA_CB_TypeDef DmaRxCallBack;

/** @endcond */

/**************************************************************************//**
 * @brief CDC device initialization.
 *****************************************************************************/
void CDC_Init( void )
{
  SerialPortInit();
  DmaSetup();
}

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements CDC class specific commands.
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted.
 *         USB_STATUS_REQ_UNHANDLED when command is unknown, the USB device
 *         stack will handle the request.
 *****************************************************************************/
int CDC_SetupCmd(const USB_Setup_TypeDef *setup)
{
  int retVal = USB_STATUS_REQ_UNHANDLED;

  if ( ( setup->Type      == USB_SETUP_TYPE_CLASS          ) &&
       ( setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE )    )
  {
    switch (setup->bRequest)
    {
    case USB_CDC_GETLINECODING:
      /********************/
      if ( ( setup->wValue    == 0                     ) &&
           ( setup->wIndex    == CDC_CTRL_INTERFACE_NO ) && /* Interface no. */
           ( setup->wLength   == 7                     ) && /* Length of cdcLineCoding. */
           ( setup->Direction == USB_SETUP_DIR_IN      )    )
      {
        /* Send current settings to USB host. */
        USBD_Write(0, (void*) &cdcLineCoding, 7, NULL);
        retVal = USB_STATUS_OK;
      }
      break;

    case USB_CDC_SETLINECODING:
      /********************/
      if ( ( setup->wValue    == 0                     ) &&
           ( setup->wIndex    == CDC_CTRL_INTERFACE_NO ) && /* Interface no. */
           ( setup->wLength   == 7                     ) && /* Length of cdcLineCoding. */
           ( setup->Direction != USB_SETUP_DIR_IN      )    )
      {
        /* Get new settings from USB host. */
        USBD_Read(0, (void*) &cdcLineCoding, 7, LineCodingReceived);
        retVal = USB_STATUS_OK;
      }
      break;

    case USB_CDC_SETCTRLLINESTATE:
      /********************/
      if ( ( setup->wIndex  == CDC_CTRL_INTERFACE_NO ) &&   /* Interface no.  */
           ( setup->wLength == 0                     )    ) /* No data.       */
      {
        /* Do nothing ( Non compliant behaviour !! ) */
        retVal = USB_STATUS_OK;
      }
      break;
    }
  }

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Callback function called each time the USB device state is changed.
 *   Starts CDC operation when device has been configured by USB host.
 *
 * @param[in] oldState The device state the device has just left.
 * @param[in] newState The new device state.
 *****************************************************************************/
void CDC_StateChangeEvent( USBD_State_TypeDef oldState,
                           USBD_State_TypeDef newState)
{
  if (newState == USBD_STATE_CONFIGURED)
  {
    /* We have been configured, start CDC functionality ! */

    if (oldState == USBD_STATE_SUSPENDED)   /* Resume ?   */
    {
    }

    /* Start receiving data from USB host. */
    usbRxIndex  = 0;
    usbRxActive = true;
    dmaTxActive = false;
    USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
              CDC_USB_RX_BUF_SIZ, UsbDataReceived);

    /* Start receiving data on UART. */
    uartRxIndex    = 0;
    LastUsbTxCnt   = 0;
    uartRxCount    = 0;
    dmaRxActive    = true;
    usbTxActive    = false;
    dmaRxCompleted = true;
    DMA_ActivateBasic(CDC_UART_RX_DMA_CHANNEL, true, false,
                      (void *) uartRxBuffer[ uartRxIndex ],
                      (void *) &(CDC_UART->RXDATA),
                      CDC_USB_TX_BUF_SIZ - 1);
    USBTIMER_Start(CDC_TIMER_ID, CDC_RX_TIMEOUT, UartRxTimeout);
  }

  else if ((oldState == USBD_STATE_CONFIGURED) &&
           (newState != USBD_STATE_SUSPENDED))
  {
    /* We have been de-configured, stop CDC functionality. */
    USBTIMER_Stop(CDC_TIMER_ID);
    /* Stop DMA channels. */
    DMA->CHENC = ( 1 << CDC_UART_TX_DMA_CHANNEL ) |
                 ( 1 << CDC_UART_RX_DMA_CHANNEL );
  }

  else if (newState == USBD_STATE_SUSPENDED)
  {
    /* We have been suspended, stop CDC functionality. */
    /* Reduce current consumption to below 2.5 mA.     */
    USBTIMER_Stop(CDC_TIMER_ID);
    /* Stop DMA channels. */
    DMA->CHENC = ( 1 << CDC_UART_TX_DMA_CHANNEL ) |
                 ( 1 << CDC_UART_RX_DMA_CHANNEL );
  }
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/**************************************************************************//**
 * @brief Callback function called whenever a new packet with data is received
 *        on USB.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int UsbDataReceived(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining)
{
  (void) remaining;            /* Unused parameter. */

  if ((status == USB_STATUS_OK) && (xferred > 0))
  {
    usbRxIndex ^= 1;

    if (!dmaTxActive)
    {
      /* dmaTxActive = false means that a new UART Tx DMA can be started. */
      dmaTxActive = true;
      DMA_ActivateBasic(CDC_UART_TX_DMA_CHANNEL, true, false,
                        (void *) &(CDC_UART->TXDATA),
                        (void *) usbRxBuffer[ usbRxIndex ^ 1 ],
                        xferred - 1);

      /* Start a new USB receive transfer. */
      USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
                CDC_USB_RX_BUF_SIZ, UsbDataReceived);
    }
    else
    {
      /* The UART transmit DMA callback function will start a new DMA. */
      usbRxActive      = false;
      usbBytesReceived = xferred;
    }
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief Callback function called whenever a UART transmit DMA has completed.
 *
 * @param[in] channel DMA channel number.
 * @param[in] primary True if this is the primary DMA channel.
 * @param[in] user    Optional user supplied parameter.
 *****************************************************************************/
static void DmaTxComplete(unsigned int channel, bool primary, void *user)
{
  (void) channel;              /* Unused parameter. */
  (void) primary;              /* Unused parameter. */
  (void) user;                 /* Unused parameter. */
  CORE_DECLARE_IRQ_STATE;

  /*
   * As nested interrupts may occur and we rely on variables usbRxActive
   * and dmaTxActive etc, we must handle this function as a critical region.
   */
  CORE_ENTER_ATOMIC();

  if (!usbRxActive)
  {
    /* usbRxActive = false means that an USB receive packet has been received.*/
    DMA_ActivateBasic(CDC_UART_TX_DMA_CHANNEL, true, false,
                      (void *) &(CDC_UART->TXDATA),
                      (void *) usbRxBuffer[ usbRxIndex ^ 1 ],
                      usbBytesReceived - 1);

    /* Start a new USB receive transfer. */
    usbRxActive = true;
    USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuffer[ usbRxIndex ],
              CDC_USB_RX_BUF_SIZ, UsbDataReceived);
  }
  else
  {
    /* The USB receive complete callback function will start a new DMA. */
    dmaTxActive = false;
  }

  CORE_EXIT_ATOMIC();
}

/**************************************************************************//**
 * @brief Callback function called whenever a packet with data has been
 *        transmitted on USB
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int UsbDataTransmitted(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  (void) xferred;              /* Unused parameter. */
  (void) remaining;            /* Unused parameter. */

  if (status == USB_STATUS_OK)
  {
    if (!dmaRxActive)
    {
      /* dmaRxActive = false means that a new UART Rx DMA can be started. */

      USBD_Write(CDC_EP_DATA_IN, (void*) uartRxBuffer[ uartRxIndex ^ 1],
                 uartRxCount, UsbDataTransmitted);
      LastUsbTxCnt = uartRxCount;

      dmaRxActive    = true;
      dmaRxCompleted = true;
      DMA_ActivateBasic(CDC_UART_RX_DMA_CHANNEL, true, false,
                        (void *) uartRxBuffer[ uartRxIndex ],
                        (void *) &(CDC_UART->RXDATA),
                        CDC_USB_TX_BUF_SIZ - 1);
      uartRxCount = 0;
      USBTIMER_Start(CDC_TIMER_ID, CDC_RX_TIMEOUT, UartRxTimeout);
    }
    else
    {
      /* The UART receive DMA callback function will start a new DMA. */
      usbTxActive = false;
    }
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief Callback function called whenever a UART receive DMA has completed.
 *
 * @param[in] channel DMA channel number.
 * @param[in] primary True if this is the primary DMA channel.
 * @param[in] user    Optional user supplied parameter.
 *****************************************************************************/
static void DmaRxComplete(unsigned int channel, bool primary, void *user)
{
  (void) channel;              /* Unused parameter. */
  (void) primary;              /* Unused parameter. */
  (void) user;                 /* Unused parameter. */
  CORE_DECLARE_IRQ_STATE;

  /*
   * As nested interrupts may occur and we rely on variables usbTxActive
   * and dmaRxActive etc, we must handle this function as a critical region.
   */
  CORE_ENTER_ATOMIC();

  uartRxIndex ^= 1;

  if (dmaRxCompleted)
  {
    uartRxCount = CDC_USB_TX_BUF_SIZ;
  }
  else
  {
    uartRxCount = CDC_USB_TX_BUF_SIZ - 1 -
                  ((dmaControlBlock[ 1 ].CTRL & _DMA_CTRL_N_MINUS_1_MASK)
                   >> _DMA_CTRL_N_MINUS_1_SHIFT);
  }

  if (!usbTxActive)
  {
    /* usbTxActive = false means that a new USB packet can be transferred. */
    usbTxActive = true;
    USBD_Write(CDC_EP_DATA_IN, (void*) uartRxBuffer[ uartRxIndex ^ 1],
               uartRxCount, UsbDataTransmitted);
    LastUsbTxCnt = uartRxCount;

    /* Start a new UART receive DMA. */
    dmaRxCompleted = true;
    DMA_ActivateBasic(CDC_UART_RX_DMA_CHANNEL, true, false,
                      (void *) uartRxBuffer[ uartRxIndex ],
                      (void *) &(CDC_UART->RXDATA),
                      CDC_USB_TX_BUF_SIZ - 1);
    uartRxCount = 0;
    USBTIMER_Start(CDC_TIMER_ID, CDC_RX_TIMEOUT, UartRxTimeout);
  }
  else
  {
    /* The USB transmit complete callback function will start a new DMA. */
    dmaRxActive = false;
    USBTIMER_Stop(CDC_TIMER_ID);
  }

  CORE_EXIT_ATOMIC();
}

/**************************************************************************//**
 * @brief
 *   Called each time UART Rx timeout period elapses.
 *   Implements UART Rx rate monitoring, i.e. we must behave differently when
 *   UART Rx rate is slow e.g. when a person is typing characters, and when UART
 *   Rx rate is maximum.
 *****************************************************************************/
static void UartRxTimeout(void)
{
  int      cnt;
  uint32_t dmaCtrl;

  dmaCtrl = dmaControlBlock[ 1 ].CTRL;

  /* Has the DMA just completed ? */
  if ((dmaCtrl & _DMA_CTRL_CYCLE_CTRL_MASK) == DMA_CTRL_CYCLE_CTRL_INVALID)
  {
    return;
  }

  cnt = CDC_USB_TX_BUF_SIZ - 1 -
        ((dmaCtrl & _DMA_CTRL_N_MINUS_1_MASK) >> _DMA_CTRL_N_MINUS_1_SHIFT);

  if ((cnt == 0) && (LastUsbTxCnt == CDC_BULK_EP_SIZE))
  {
    /*
     * No activity on UART Rx, send a ZERO length USB package if last USB
     * USB package sent was CDC_BULK_EP_SIZE (max. EP size) long.
     */
    /* Stop Rx DMA channel. */
    DMA->CHENC     = 1 << CDC_UART_RX_DMA_CHANNEL;
    dmaRxCompleted = false;
    /* Call DMA completion callback. */
    DmaRxComplete(CDC_UART_RX_DMA_CHANNEL, true, NULL);
    return;
  }

  if ((cnt > 0) && (cnt == uartRxCount))
  {
    /*
     * There is curently no activity on UART Rx but some chars have been
     * received. Stop DMA and transmit the chars we have got so far on USB.
     */
    /* Stop Rx DMA channel. */
    DMA->CHENC     = 1 << CDC_UART_RX_DMA_CHANNEL;
    dmaRxCompleted = false;
    /* Call DMA completion callback. */
    DmaRxComplete(CDC_UART_RX_DMA_CHANNEL, true, NULL);
    return;
  }

  /* Restart timer to continue monitoring. */
  uartRxCount = cnt;
  USBTIMER_Start(CDC_TIMER_ID, CDC_RX_TIMEOUT, UartRxTimeout);
}

/**************************************************************************//**
 * @brief
 *   Callback function called when the data stage of a CDC_SET_LINECODING
 *   setup command has completed.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK if data accepted.
 *         USB_STATUS_REQ_ERR if data calls for modes we can not support.
 *****************************************************************************/
static int LineCodingReceived(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  uint32_t frame = 0;
  (void) remaining;

  /* We have received new serial port communication settings from USB host. */
  if ((status == USB_STATUS_OK) && (xferred == 7))
  {
    /* Check bDataBits, valid values are: 5, 6, 7, 8 or 16 bits. */
    if (cdcLineCoding.bDataBits == 5)
      frame |= USART_FRAME_DATABITS_FIVE;

    else if (cdcLineCoding.bDataBits == 6)
      frame |= USART_FRAME_DATABITS_SIX;

    else if (cdcLineCoding.bDataBits == 7)
      frame |= USART_FRAME_DATABITS_SEVEN;

    else if (cdcLineCoding.bDataBits == 8)
      frame |= USART_FRAME_DATABITS_EIGHT;

    else if (cdcLineCoding.bDataBits == 16)
      frame |= USART_FRAME_DATABITS_SIXTEEN;

    else
      return USB_STATUS_REQ_ERR;

    /* Check bParityType, valid values are: 0=None 1=Odd 2=Even 3=Mark 4=Space  */
    if (cdcLineCoding.bParityType == 0)
      frame |= USART_FRAME_PARITY_NONE;

    else if (cdcLineCoding.bParityType == 1)
      frame |= USART_FRAME_PARITY_ODD;

    else if (cdcLineCoding.bParityType == 2)
      frame |= USART_FRAME_PARITY_EVEN;

    else if (cdcLineCoding.bParityType == 3)
      return USB_STATUS_REQ_ERR;

    else if (cdcLineCoding.bParityType == 4)
      return USB_STATUS_REQ_ERR;

    else
      return USB_STATUS_REQ_ERR;

    /* Check bCharFormat, valid values are: 0=1 1=1.5 2=2 stop bits */
    if (cdcLineCoding.bCharFormat == 0)
      frame |= USART_FRAME_STOPBITS_ONE;

    else if (cdcLineCoding.bCharFormat == 1)
      frame |= USART_FRAME_STOPBITS_ONEANDAHALF;

    else if (cdcLineCoding.bCharFormat == 2)
      frame |= USART_FRAME_STOPBITS_TWO;

    else
      return USB_STATUS_REQ_ERR;

    /* Program new UART baudrate etc. */
    CDC_UART->FRAME = frame;
    USART_BaudrateAsyncSet(CDC_UART, 0, cdcLineCoding.dwDTERate, usartOVS16);

    return USB_STATUS_OK;
  }
  return USB_STATUS_REQ_ERR;
}

/**************************************************************************//**
 * @brief Initialize the DMA peripheral.
 *****************************************************************************/
static void DmaSetup(void)
{
  /* DMA configuration structs. */
  DMA_Init_TypeDef       dmaInit;
  DMA_CfgChannel_TypeDef chnlCfgTx, chnlCfgRx;
  DMA_CfgDescr_TypeDef   descrCfgTx, descrCfgRx;

  /* Initialize the DMA. */
  dmaInit.hprot        = 0;
  dmaInit.controlBlock = dmaControlBlock;
  DMA_Init(&dmaInit);

  /*---------- Configure DMA channel for UART Tx. ----------*/

  /* Setup the interrupt callback routine */
  DmaTxCallBack.cbFunc  = DmaTxComplete;
  DmaTxCallBack.userPtr = NULL;

  /* Setup the channel */
  chnlCfgTx.highPri   = false;    /* Can't use with peripherals. */
  chnlCfgTx.enableInt = true;     /* Interrupt needed when buffers are used. */
  chnlCfgTx.select    = CDC_TX_DMA_SIGNAL;
  chnlCfgTx.cb        = &DmaTxCallBack;
  DMA_CfgChannel(CDC_UART_TX_DMA_CHANNEL, &chnlCfgTx);

  /* Setup channel descriptor. */
  /* Destination is UART Tx data register and doesn't move. */
  descrCfgTx.dstInc = dmaDataIncNone;
  descrCfgTx.srcInc = dmaDataInc1;
  descrCfgTx.size   = dmaDataSize1;

  /* We have time to arbitrate again for each sample. */
  descrCfgTx.arbRate = dmaArbitrate1;
  descrCfgTx.hprot   = 0;

  /* Configure primary descriptor. */
  DMA_CfgDescr(CDC_UART_TX_DMA_CHANNEL, true, &descrCfgTx);

  /*---------- Configure DMA channel for UART Rx. ----------*/

  /* Setup the interrupt callback routine. */
  DmaRxCallBack.cbFunc  = DmaRxComplete;
  DmaRxCallBack.userPtr = NULL;

  /* Setup the channel */
  chnlCfgRx.highPri   = false;    /* Can't use with peripherals. */
  chnlCfgRx.enableInt = true;     /* Interrupt needed when buffers are used. */
  chnlCfgRx.select    = CDC_RX_DMA_SIGNAL;
  chnlCfgRx.cb        = &DmaRxCallBack;
  DMA_CfgChannel(CDC_UART_RX_DMA_CHANNEL, &chnlCfgRx);

  /* Setup channel descriptor. */
  /* Source is UART Rx data register and doesn't move. */
  descrCfgRx.dstInc = dmaDataInc1;
  descrCfgRx.srcInc = dmaDataIncNone;
  descrCfgRx.size   = dmaDataSize1;

  /* We have time to arbitrate again for each sample. */
  descrCfgRx.arbRate = dmaArbitrate1;
  descrCfgRx.hprot   = 0;

  /* Configure primary descriptor. */
  DMA_CfgDescr(CDC_UART_RX_DMA_CHANNEL, true, &descrCfgRx);
}

/**************************************************************************//**
 * @brief Initialize the UART peripheral.
 *****************************************************************************/
static void SerialPortInit(void)
{
  USART_InitAsync_TypeDef init  = USART_INITASYNC_DEFAULT;

  /* Configure GPIO pins. */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high. */
  GPIO_PinModeSet(CDC_UART_TX_PORT, CDC_UART_TX_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet(CDC_UART_RX_PORT, CDC_UART_RX_PIN, gpioModeInput, 0);

  /* Enable DK mainboard RS232/UART switch. */
  CDC_ENABLE_DK_UART_SWITCH();

  /* Enable peripheral clocks. */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(CDC_UART_CLOCK, true);

  /* Configure UART for basic async operation. */
  init.enable = usartDisable;
  USART_InitAsync(CDC_UART, &init);

  /* Enable Tx/Rx pins and set correct UART location. */
  CDC_UART->ROUTE = CDC_UART_ROUTE;

  /* Finally enable it */
  USART_Enable(CDC_UART, usartEnable);
}

/** @endcond */
