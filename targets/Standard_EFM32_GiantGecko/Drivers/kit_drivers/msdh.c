/**************************************************************************//**
 * @file  msdh.c
 * @brief Host side implementation of Mass Storage class Device (MSD) interface.
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


#include <inttypes.h>
#include "em_device.h"
#include "em_usb.h"
#include "msdscsi.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define DEV_ADDR    1                 /* USB bus address    */
#define BULK_OUT    &ep[ epOutIndex ] /* Endpoint "handles" */
#define BULK_IN     &ep[ epInIndex  ]
#define SCSI_INQUIRY_RETRIES       10 // Retries during MSDH_Init()
#define SCSI_TESTUNITREADY_RETRIES 10 // Retries during MSDH_Init()

static void PrintDeviceStrings(uint8_t *buf);
static bool QualifyDevice(uint8_t *buf);

static USBH_Device_TypeDef device;
static USBH_Ep_TypeDef     ep[ 2 ];
static int                 epOutIndex;
static int                 epInIndex;

/** @endcond */

/***************************************************************************//**
 * @brief
 *   Initialize an USB connected Mass Storage Device.
 *   Checks if the device is a valid MSD device. Will perform all
 *   necessary MSD initialization.
 *
 * @note
 *   This function assumes that prior calls to USBH_Init() and
 *   USBH_WaitForDeviceConnectionB() have been performed.
 *   The contents of the usbDeviceInfo data buffer will be overwritten.
 *
 * @param[in] usbDeviceInfo
 *   Pointer to USB enumeration information. usbDeviceInfo must have been
 *   initialized by a prior call to USBH_WaitForDeviceConnectionB().
 *
 * @param[in] usbDeviceInfoSize
 *   The size of the usbDeviceInfo data buffer.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_Init(uint8_t *usbDeviceInfo, int usbDeviceInfoSize)
{
  SL_ALIGN(4)
  MSDSCSI_InquiryData_TypeDef inquiryData  SL_ATTRIBUTE_ALIGN(4);

  SL_ALIGN(4)
  MSDSCSI_ReadCapacityData_TypeDef capacityData SL_ATTRIBUTE_ALIGN(4);

  SL_ALIGN(4)
  MSDSCSI_RequestSenseData_TypeDef reqSenseData SL_ATTRIBUTE_ALIGN(4);

  bool ready;
  int  result, i;

  /* Read all device descriptors. */
  if (USBH_QueryDeviceB(usbDeviceInfo, usbDeviceInfoSize, USBH_GetPortSpeed())
      != USB_STATUS_OK)
  {
    USB_PRINTF("\nUSB descriptor retrieval failed.");
    return false;
  }

  /* Check if a valid MSD device (will activate device if OK). */
  if (!QualifyDevice(usbDeviceInfo))
  {
    return false;
  }

  /* Initialize MSD SCSI module. */
  if (!MSDSCSI_Init(BULK_OUT, BULK_IN))
  {
    USB_PRINTF("\nMSD BOT initialization failed.");
    return false;
  }

  // Do a SCSI Inquiry to get some info from the device.
  // Some devices are slow to get started, do a couple of retries before
  // giving up.
  i = 0;
  do
  {
    ready = MSDSCSI_Inquiry(&inquiryData);
    if (!ready)
    {
      USBTIMER_DelayMs(500);
    }
    i++;
  } while (!ready && i < SCSI_INQUIRY_RETRIES);

  if (!ready)
  {
    USB_PRINTF("\nMSD SCSI Inquiry failed.");
    return false;
  }

  memcpy(usbDeviceInfo, &inquiryData.T10VendorId, sizeof(inquiryData.T10VendorId));
  usbDeviceInfo[ sizeof(inquiryData.T10VendorId) ] = '\0';
  USB_PRINTF("\nSCSI Inquiry Vendor ID string        : \"%s\"", usbDeviceInfo);

  memcpy(usbDeviceInfo, &inquiryData.ProductId, sizeof(inquiryData.ProductId));
  usbDeviceInfo[ sizeof(inquiryData.ProductId) ] = '\0';
  USB_PRINTF("\nSCSI Inquiry Product ID string       : \"%s\"", usbDeviceInfo);

  memcpy(usbDeviceInfo, &inquiryData.ProductRevisionLevel, sizeof(inquiryData.ProductRevisionLevel));
  usbDeviceInfo[ sizeof(inquiryData.ProductRevisionLevel) ] = '\0';
  USB_PRINTF("\nSCSI Inquiry Product Revision string : \"%s\"", usbDeviceInfo);

  /* Is it a block device ? */
  if ((inquiryData.PeripheralQualifier != 0) ||
      (inquiryData.PeripheralDeviceType != 0))
  {
    return false;
  }

  /* Wait for upto 5 seconds for device to become ready. */
  i = 0;
  do
  {
    result = MSDSCSI_RequestSense(&reqSenseData);
    ready  = MSDSCSI_TestUnitReady();
    if (!ready)
    {
      USBTIMER_DelayMs(500);
    }
    i++;
  } while (!ready && i < SCSI_TESTUNITREADY_RETRIES && result);

  if (!result)
  {
    USB_PRINTF("\n\nSCSI Request Sense execution error");
    return false;
  }

  if (!ready)
  {
    USB_PRINTF("\n\nMSD device not ready");
    return false;
  }

  /* Get device capacity. */
  if (!MSDSCSI_ReadCapacity(&capacityData))
  {
    USB_PRINTF("\n\nSCSI Read Capacity execution error");
    return false;
  }

  USB_PRINTF("\n\nSCSI Read Capacity LBA count         : %ld = %ld MiB",
             capacityData.LogicalBlockAddress,
             (uint32_t)
             (((uint64_t)capacityData.LogicalBlockAddress
               * capacityData.LogicalBlockLength) / (1024 * 1024)));
  USB_PRINTF("\nSCSI Read Capacity LBA size          : %ld\n\n",
             capacityData.LogicalBlockLength);

  return true;
}

/***************************************************************************//**
 * @brief
 *   Get sectorcount from the device.
 *
 * @param[out] sectorCount
 *   Number of sectors on the device.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_GetSectorCount(uint32_t *sectorCount)
{
  SL_ALIGN(4)
  MSDSCSI_ReadCapacityData_TypeDef capacityData SL_ATTRIBUTE_ALIGN(4);

  if (!MSDSCSI_ReadCapacity(&capacityData))
    return false;

  *sectorCount = capacityData.LogicalBlockAddress;

  return true;
}

/***************************************************************************//**
 * @brief
 *   Get sectorsize from the device.
 *
 * @param[out] sectorSize
 *   The size of device sectors.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_GetSectorSize(uint16_t *sectorSize)
{
  SL_ALIGN(4)
  MSDSCSI_ReadCapacityData_TypeDef capacityData SL_ATTRIBUTE_ALIGN(4);

  if (!MSDSCSI_ReadCapacity(&capacityData))
    return false;

  *sectorSize = (uint16_t) capacityData.LogicalBlockLength;

  return true;
}

/***************************************************************************//**
 * @brief
 *   Get blocksize from the device.
 *
 * @note
 *   This function assumes that block size equals sector size.
 *
 * @param[out] blockSize
 *   The block size of device storage memory.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_GetBlockSize(uint32_t *blockSize)
{
  SL_ALIGN(4)
  MSDSCSI_ReadCapacityData_TypeDef capacityData SL_ATTRIBUTE_ALIGN(4);

  if (!MSDSCSI_ReadCapacity(&capacityData))
    return false;

  *blockSize = capacityData.LogicalBlockLength;

  return true;
}

/***************************************************************************//**
 * @brief
 *   Read sectors from device.
 *
 * @param[in] lba
 *   Sector address (LBA) of first sector to read.
 *
 * @param[in] sectors
 *   Number of sectors to read.
 *
 * @param[out] data
 *   Data buffer through which data is returned to caller.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_ReadSectors(uint32_t lba, uint16_t sectors, void *data)
{
  return MSDSCSI_Read10(lba, sectors, data);
}

/***************************************************************************//**
 * @brief
 *   Write sectors to device.
 *
 * @param[in] lba
 *   Sector address (LBA) of first sector to write.
 *
 * @param[in] sectors
 *   Number of sectors to write.
 *
 * @param[out] data
 *   Data buffer containing data to be written.
 *
 * @return
 *   Returns true on success, false otherwise.
 ******************************************************************************/
bool MSDH_WriteSectors(uint32_t lba, uint16_t sectors, const void *data)
{
  return MSDSCSI_Write10(lba, sectors, data);
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Check if a device is a valid Mass Storage Device.
 *   A valid device will be fully enumerated and activated.
 *
 * @param[in] buf
 *   Enumeration data buffer.
 *
 * @return
 *   Returns true if a valid MSD device, false otherwise.
 ******************************************************************************/
static bool QualifyDevice(uint8_t *buf)
{
  int  i;
  bool retVal = false;
  bool epIn   = false, epOut = false;

  if ((USBH_QGetDeviceDescriptor(buf)->bDeviceClass == 0) &&
      (USBH_QGetDeviceDescriptor(buf)->bDeviceSubClass == 0) &&
      (USBH_QGetDeviceDescriptor(buf)->bDeviceProtocol == 0) &&
      (USBH_QGetInterfaceDescriptor(buf, 0, 0)->bInterfaceClass == USB_CLASS_MSD) &&
      (USBH_QGetInterfaceDescriptor(buf, 0, 0)->bInterfaceSubClass == USB_CLASS_MSD_SCSI_CMDSET) &&
      (USBH_QGetInterfaceDescriptor(buf, 0, 0)->bInterfaceProtocol == USB_CLASS_MSD_BOT_TRANSPORT) &&
      (USBH_QGetInterfaceDescriptor(buf, 0, 0)->bNumEndpoints >= 2))
  {
    /*
     * OK so far, scan through endpoints and look for one BULK IN and
     * one BULK OUT endpoint.
     */
    for (i = 0; i < USBH_QGetInterfaceDescriptor(buf, 0, 0)->bNumEndpoints; i++)
    {
      if (USBH_QGetEndpointDescriptor(buf, 0, 0, i)->bmAttributes == USB_EPTYPE_BULK)
      {
        if (USBH_QGetEndpointDescriptor(buf, 0, 0, i)->bEndpointAddress & USB_EP_DIR_IN)
        {
          if (!epIn)
          {
            epIn      = true;
            epInIndex = i;
          }
        }
        else
        {
          if (!epOut)
          {
            epOut      = true;
            epOutIndex = i;
          }
        }
      }

      /* Success ? */
      if (epIn && epOut)
        break;
    }

    if ((epIn && epOut) && (epInIndex < 2) && (epOutIndex < 2))
    {
      /*
       * Some MSD devices has more than the two required bulk endpoints.
       * We will only accept devices where the two bulk endpoints are
       * the two first endpoints within the interface.
       */
      USB_PRINTF("\nThis is a valid MSD device.");
      retVal = true;
    }
  }

  if (retVal == false)
  {
    USBH_PrintDeviceDescriptor(USBH_QGetDeviceDescriptor(buf));
    USBH_PrintConfigurationDescriptor(USBH_QGetConfigurationDescriptor(buf, 0), USB_CONFIG_DESCSIZE);
    USBH_PrintInterfaceDescriptor(USBH_QGetInterfaceDescriptor(buf, 0, 0));

    for (i = 0; i < USBH_QGetInterfaceDescriptor(buf, 0, 0)->bNumEndpoints; i++)
    {
      USBH_PrintEndpointDescriptor(USBH_QGetEndpointDescriptor(buf, 0, 0, i));
    }
    USB_PRINTF("\nThis is not a valid MSD device, review device descriptors.");
  }
  else
  {
    /* All set, activate the device. */

    USBH_InitDeviceData(&device, buf, ep, 2, USBH_GetPortSpeed());
    PrintDeviceStrings(buf);
    USBH_SetAddressB(&device, DEV_ADDR);
    USBH_SetConfigurationB(&device, device.confDesc.bConfigurationValue);

    /* Assign Host Channels to the endpoints */
    USBH_AssignHostChannel(BULK_OUT, 2);
    USBH_AssignHostChannel(BULK_IN, 3);

    USB_PRINTF("\n\nDevice VID/PID is 0x%04X/0x%04X, device bus speed is %s",
               device.devDesc.idVendor, device.devDesc.idProduct,
               USBH_GetPortSpeed() == PORT_FULL_SPEED ? "FULL" : "LOW");
  }

  return retVal;
}

/***************************************************************************//**
 * @brief
 *   Print USB device strings.
 *
 * @param[in] buf
 *   Place holder for device strings, no check on buffer size.
 ******************************************************************************/
static void PrintDeviceStrings(uint8_t *buf)
{
  /* Get and print device string descriptors. */

  if (device.devDesc.iManufacturer)
  {
    USBH_GetStringB(&device, buf, 255, device.devDesc.iManufacturer,
                    USB_LANGID_ENUS);
    USBH_PrintString("\n\niManufacturer = \"",
                     (USB_StringDescriptor_TypeDef*) buf, "\"");
  }
  else
  {
    USB_PRINTF("\n\niManufacturer = <NONE>");
  }

  if (device.devDesc.iProduct)
  {
    USBH_GetStringB(&device, buf, 255, device.devDesc.iProduct,
                    USB_LANGID_ENUS);
    USBH_PrintString("\niProduct      = \"",
                     (USB_StringDescriptor_TypeDef*) buf, "\"");
  }
  else
  {
    USB_PRINTF("\niProduct      = <NONE>");
  }

  if (device.devDesc.iSerialNumber)
  {
    USBH_GetStringB(&device, buf, 255, device.devDesc.iSerialNumber,
                    USB_LANGID_ENUS);
    USBH_PrintString("\niSerialNumber = \"",
                     (USB_StringDescriptor_TypeDef*) buf, "\"\n");
  }
  else
  {
    USB_PRINTF("\niSerialNumber = <NONE>\n");
  }
}

/** @endcond */
