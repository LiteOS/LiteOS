
#ifndef _APP_USBDMSC_H_
#define _APP_USBDMSC_H_

#include "stdint.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/* Length of Each Logical Address Block */
#define LENGTH_OF_EACH_LBA (512U)
/* total number of logical blocks present */
#define TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL (48U)
/* Net Disk Size , default disk is 48*512, that is 24kByte, however , the disk reconnised by that PC only has 4k Byte,
 * This is caused by that the file system also need memory*/
#define DISK_SIZE_NORMAL (TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL * LENGTH_OF_EACH_LBA)

#define LOGICAL_UNIT_SUPPORTED (1U)

typedef struct _usb_msc_struct
{
    usb_device_handle deviceHandle;
    class_handle_t mscHandle;
    uint8_t *storageDisk;
    uint8_t diskLock;
    uint8_t read_write_error;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_MSC_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
} usb_msc_struct_t;

extern uint8_t usbdmsc_init(void);
extern void    usbdmsc_proc(void);

#endif
