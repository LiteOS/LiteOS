/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "board.h"
#include "fsl_device_registers.h"

#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_power.h"

#include <stdbool.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "app_usbdmsc.h"

#include "app_led.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcFro
#define USB_FS_CLK_FREQ CLOCK_GetFreq(kCLOCK_FroHf)


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
USB_DATA_ALIGNMENT usb_device_inquiry_data_fromat_struct_t g_InquiryInfo = {
    (USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER << USB_DEVICE_MSC_UFI_PERIPHERAL_QUALIFIER_SHIFT) |
        USB_DEVICE_MSC_UFI_PERIPHERAL_DEVICE_TYPE,
    (uint8_t)(USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT << USB_DEVICE_MSC_UFI_REMOVABLE_MEDIUM_BIT_SHIFT),
    USB_DEVICE_MSC_UFI_VERSIONS,
    0x02,
    USB_DEVICE_MSC_UFI_ADDITIONAL_LENGTH,
    {0x00, 0x00, 0x00},
    {'N', 'X', 'P', ' ', 'S', 'E', 'M', 'I'},
    {'N', 'X', 'P', ' ', 'M', 'A', 'S', 'S', ' ', 'S', 'T', 'O', 'R', 'A', 'G', 'E'},
    {'0', '0', '0', '1'}};
USB_DATA_ALIGNMENT usb_device_mode_parameters_header_struct_t g_ModeParametersHeader = {
    /*refer to ufi spec mode parameter header*/
    0x0000, /*!< Mode Data Length*/
    0x00,   /*!<Default medium type (current mounted medium type)*/
    0x00,   /*!MODE SENSE command, a Write Protected bit of zero indicates the medium is write enabled*/
    {0x00, 0x00, 0x00, 0x00} /*!<This bit should be set to zero*/
};
/* Data structure of msc device, store the information ,such as class handle */
USB_DATA_ALIGNMENT static uint8_t s_StorageDisk[DISK_SIZE_NORMAL];
usb_msc_struct_t g_msc;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief device msc callback function.
 *
 * This function handle the disk class specified event.
 * @param handle          The USB class  handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the class specific event.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceMscCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_lba_information_struct_t *lbaInformationStructure;
    usb_device_lba_app_struct_t *lbaData;
    usb_device_ufi_app_struct_t *ufi;
		led_toggle(1);
    switch (event)
    {
        case kUSB_DeviceMscEventReadResponse:
            lbaData = (usb_device_lba_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventWriteResponse:
            lbaData = (usb_device_lba_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventWriteRequest:
            lbaData = (usb_device_lba_app_struct_t *)param;
            /*offset is the write start address get from write command, refer to class driver*/
            lbaData->buffer = g_msc.storageDisk + lbaData->offset * LENGTH_OF_EACH_LBA;
            break;
        case kUSB_DeviceMscEventReadRequest:
            lbaData = (usb_device_lba_app_struct_t *)param;
            /*offset is the read start address get from read command, refer to class driver*/
            lbaData->buffer = g_msc.storageDisk + lbaData->offset * LENGTH_OF_EACH_LBA;
            break;
        case kUSB_DeviceMscEventGetLbaInformation:
            lbaInformationStructure = (usb_device_lba_information_struct_t *)param;
            lbaInformationStructure->lengthOfEachLba = LENGTH_OF_EACH_LBA;
            lbaInformationStructure->totalLbaNumberSupports = TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL;
            lbaInformationStructure->logicalUnitNumberSupported = LOGICAL_UNIT_SUPPORTED;
            lbaInformationStructure->bulkInBufferSize = DISK_SIZE_NORMAL;
            lbaInformationStructure->bulkOutBufferSize = DISK_SIZE_NORMAL;
            break;
        case kUSB_DeviceMscEventTestUnitReady:
            /*change the test unit ready command's sense data if need, be careful to modify*/
            ufi = (usb_device_ufi_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventInquiry:
            ufi = (usb_device_ufi_app_struct_t *)param;
            ufi->size = sizeof(usb_device_inquiry_data_fromat_struct_t);
            ufi->buffer = (uint8_t *)&g_InquiryInfo;
            break;
        case kUSB_DeviceMscEventModeSense:
            ufi = (usb_device_ufi_app_struct_t *)param;
            ufi->size = sizeof(usb_device_mode_parameters_header_struct_t);
            ufi->buffer = (uint8_t *)&g_ModeParametersHeader;
            break;
        case kUSB_DeviceMscEventModeSelect:
            break;
        case kUSB_DeviceMscEventModeSelectResponse:
            ufi = (usb_device_ufi_app_struct_t *)param;
            break;
        case kUSB_DeviceMscEventFormatComplete:
            break;
        case kUSB_DeviceMscEventRemovalRequest:
            break;
        default:
            break;
    }
    return error;
}
/*!
 * @brief device callback function.
 *
 * This function handle the usb standard event. more information, please refer to usb spec chapter 9.
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t *temp16 = (uint16_t *)param;
    uint8_t *temp8 = (uint8_t *)param;
    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            g_msc.attach = 0;
            error = kStatus_USB_Success;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_msc.speed))
            {
                USB_DeviceSetSpeed(handle, g_msc.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (param)
            {
                g_msc.attach = 1;
                g_msc.currentConfiguration = *temp8;
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (g_msc.attach)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface < USB_MSC_INTERFACE_COUNT)
                {
                    g_msc.currentInterfaceAlternateSetting[interface] = alternateSetting;
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = g_msc.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | g_msc.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }
    return error;
}
/* USB device class information */
usb_device_class_config_struct_t msc_config[1] = {{
    USB_DeviceMscCallback, 0, &g_UsbDeviceMscConfig,
}};
/* USB device class configuration information */
usb_device_class_config_list_struct_t msc_config_list = {
    msc_config, USB_DeviceCallback, 1,
};
/*!
 * @brief USB Interrupt service routine.
 *
 * This function serves as the USB interrupt service routine.
 *
 * @return None.
 */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_msc.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_msc.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_msc.deviceHandle);
}
#endif

/*!
 * @brief device application init function.
 *
 * This function init the usb stack and sdhc driver.
 *
 * @return None.
 */
void USB_DeviceApplicationInit(void)
{
    uint8_t irq_no;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
    uint8_t ehci_irq[] = USBHS_IRQS;
    irq_no = ehci_irq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
    uint8_t khci_irq[] = USB_IRQS;
    irq_no = khci_irq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irq_no = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

    g_msc.speed = USB_SPEED_FULL;
    g_msc.attach = 0;
    g_msc.mscHandle = (class_handle_t)NULL;
    g_msc.deviceHandle = NULL;
    g_msc.storageDisk = &s_StorageDisk[0];

    if (kStatus_USB_Success != USB_DeviceClassInit(CONTROLLER_ID, &msc_config_list, &g_msc.deviceHandle))
    {
        usb_echo("USB device init failed\r\n");
    }
    else
    {
        usb_echo("USB device mass storage demo\r\n");
        g_msc.mscHandle = msc_config_list.config->classHandle;
    }

    NVIC_SetPriority((IRQn_Type)irq_no, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irq_no);

    USB_DeviceRun(g_msc.deviceHandle);
}

uint8_t usbdmsc_init(void)
{
	/* USB vbus pin */
	IOCON_PinMuxSet(IOCON, 1, 6, IOCON_MODE_INACT | IOCON_FUNC7 | IOCON_DIGITAL_EN);
	
	POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*Turn on USB Phy */
	USB_DeviceApplicationInit();

	return 1;
}

void usbdmsc_proc(void)
{
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
        USB_DeviceEhciTaskFunction(g_msc.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
        USB_DeviceKhciTaskFunction(g_msc.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_msc.deviceHandle);
#endif
#endif
}

// end file
