/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2015, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

/** \file
 *  \section Purpose
 *
 *    Definitions and methods for USB descriptor structures described by the
 *    USB specification.
 *
 *  \section Usage
 *
 */

#ifndef _USBDESCRIPTORS_H_
#define _USBDESCRIPTORS_H_
/** \addtogroup usb_general
 * @{
 * \addtogroup usb_descriptor USB Descriptors
 *  @{
 */

/*---------------------------------------------------------------------------
 *      Headers
 *---------------------------------------------------------------------------*/

/* These headers were introduced in C99 by working group
 * ISO/IEC JTC1/SC22/WG14.
 */
#include <stdint.h>

/*---------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/

/*--------- Generic Descriptors --------*/

/** \addtogroup usb_desc_type USB Descriptor types
 *      @{
 *  This section lists the codes of the usb descriptor types
 *  - \ref USBGenericDescriptor_DEVICE
 *  - \ref USBGenericDescriptor_CONFIGURATION
 *  - \ref USBGenericDescriptor_STRING
 *  - \ref USBGenericDescriptor_INTERFACE
 *  - \ref USBGenericDescriptor_ENDPOINT
 *  - \ref USBGenericDescriptor_DEVICEQUALIFIER
 *  - \ref USBGenericDescriptor_OTHERSPEEDCONFIGURATION
 *  - \ref USBGenericDescriptor_INTERFACEPOWER
 *  - \ref USBGenericDescriptor_OTG
 *  - \ref USBGenericDescriptor_DEBUG
 *  - \ref USBGenericDescriptor_INTERFACEASSOCIATION
 */
/**  Device descriptor type. */
#define USBGenericDescriptor_DEVICE                     1
/**  Configuration descriptor type. */
#define USBGenericDescriptor_CONFIGURATION              2
/**  String descriptor type. */
#define USBGenericDescriptor_STRING                     3
/**  Interface descriptor type. */
#define USBGenericDescriptor_INTERFACE                  4
/**  Endpoint descriptor type. */
#define USBGenericDescriptor_ENDPOINT                   5
/**  Device qualifier descriptor type. */
#define USBGenericDescriptor_DEVICEQUALIFIER            6
/**  Other speed configuration descriptor type. */
#define USBGenericDescriptor_OTHERSPEEDCONFIGURATION    7
/**  Interface power descriptor type. */
#define USBGenericDescriptor_INTERFACEPOWER             8
/**  On-The-Go descriptor type. */
#define USBGenericDescriptor_OTG                        9
/**  Debug descriptor type. */
#define USBGenericDescriptor_DEBUG                      10
/**  Interface association descriptor type. */
#define USBGenericDescriptor_INTERFACEASSOCIATION       11

#define USB_DT_BOS                                      0x0F

#define USBGenericDescriptor_DEVICE_CAPABILITY          0x10
/**     @}*/

/** USB protocol port */

#define  USB_EP_TYPE_MASK       0x03
#define  USB_EP_ADDR_MASK     0x0f
/**
 * \brief Endpoint transfer direction is IN
 */
#define  USB_EP_DIR_IN        0x80

/**
 * \brief Endpoint transfer direction is OUT
 */
#define  USB_EP_DIR_OUT       0x00

/**
 * \brief USB request data transfer direction (bmRequestType)
 */
#define  USB_REQ_DIR_OUT         (0<<7) //!< Host to device
#define  USB_REQ_DIR_IN          (1<<7) //!< Device to host
#define  USB_REQ_DIR_MASK        (1<<7) //!< Mask

/**
 * \brief USB request types (bmRequestType)
 */
#define  USB_REQ_TYPE_STANDARD   (0<<5) //!< Standard request
#define  USB_REQ_TYPE_CLASS      (1<<5) //!< Class-specific request
#define  USB_REQ_TYPE_VENDOR     (2<<5) //!< Vendor-specific request
#define  USB_REQ_TYPE_MASK       (3<<5) //!< Mask

/**
 * \brief USB recipient codes (bmRequestType)
 */
#define  USB_REQ_RECIP_DEVICE    (0<<0) //!< Recipient device
#define  USB_REQ_RECIP_INTERFACE (1<<0) //!< Recipient interface
#define  USB_REQ_RECIP_ENDPOINT  (2<<0) //!< Recipient endpoint
#define  USB_REQ_RECIP_OTHER     (3<<0) //!< Recipient other
#define  USB_REQ_RECIP_MASK      (0x1F) //!< Mask

/**
 * \brief Standard USB requests (bRequest)
 */
enum usb_reqid {
	USB_REQ_GET_STATUS = 0,
	USB_REQ_CLEAR_FEATURE = 1,
	USB_REQ_SET_FEATURE = 3,
	USB_REQ_SET_ADDRESS = 5,
	USB_REQ_GET_DESCRIPTOR = 6,
	USB_REQ_SET_DESCRIPTOR = 7,
	USB_REQ_GET_CONFIGURATION = 8,
	USB_REQ_SET_CONFIGURATION = 9,
	USB_REQ_GET_INTERFACE = 10,
	USB_REQ_SET_INTERFACE = 11,
	USB_REQ_SYNCH_FRAME = 12,
};

/**
 * \brief USB Device Capability types
 */
enum usb_capability_type {
	USB_DC_USB20_EXTENSION = 0x02,
};

/**
 * \brief USB Device Capability - USB 2.0 Extension
 * To fill bmAttributes field of usb_capa_ext_desc_t structure.
 */
enum usb_capability_extension_attr {
	USB_DC_EXT_LPM  = 0x00000002,
	USB_DC_EXT_BESL = 0x00000004,
	USB_DC_EXT_BESL_BASELINE_VALID = 0x00000008,
	USB_DC_EXT_BESL_DEEP_VALID = 0x00000010,
};
#define USB_DC_EXT_BESL_DEEP_OFFSET       8
#define USB_DC_EXT_BESL_DEEP(besl)        ((besl & 0xF) << USB_DC_EXT_BESL_DEEP_OFFSET)
#define USB_DC_EXT_BESL_BASELINE_OFFSET   12
#define USB_DC_EXT_BESL_BASELINE(besl)    ((besl & 0xF) << USB_DC_EXT_BESL_BASELINE_OFFSET)

#define BESL_125_US   0
#define BESL_150_US   1
#define BESL_200_US   2
#define BESL_300_US   3
#define BESL_400_US   4
#define BESL_500_US   5
#define BESL_1000_US  6
#define BESL_2000_US  7
#define BESL_3000_US  8
#define BESL_4000_US  9
#define BESL_5000_US  10
#define BESL_6000_US  11
#define BESL_7000_US  12
#define BESL_8000_US  13
#define BESL_9000_US  14
#define BESL_10000_US 15

/** Fields definition from a LPM TOKEN  */
#define  USB_LPM_ATTRIBUT_BLINKSTATE_MASK      (0xF << 0)
#define  USB_LPM_ATTRIBUT_BESL_MASK            (0xF << 4)
#define  USB_LPM_ATTRIBUT_REMOTEWAKE_MASK      (1 << 8)
#define  USB_LPM_ATTRIBUT_BLINKSTATE(value)    ((value & 0xF) << 0)
#define  USB_LPM_ATTRIBUT_BESL(value)          ((value & 0xF) << 4)
#define  USB_LPM_ATTRIBUT_REMOTEWAKE(value)    ((value & 1) << 8)
#define  USB_LPM_ATTRIBUT_BLINKSTATE_L1        USB_LPM_ATTRIBUT_BLINKSTATE(1)


/******** END usb protocol *//////////
/*--------- Device Descriptors --------*/

/** \addtogroup usb_release_number USB release numbers
 *      @{
 *  This section lists the codes of USB release numbers.
 *  - \ref USBDeviceDescriptor_USB2_00
 */

/**  The device supports USB 2.00. */
#define USBDeviceDescriptor_USB2_00         0x0200

/**  The device supports USB 2.01. */
#define USBDeviceDescriptor_USB2_01         0x0201
/**     @}*/


/*--------- Configuration Descriptors --------*/

/** \addtogroup usb_attributes USB Device Attributes
 *      @{
 * This section lists the codes of the usb attributes.
 *  - \ref USBConfigurationDescriptor_BUSPOWERED_NORWAKEUP
 *  - \ref USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP
 *  - \ref USBConfigurationDescriptor_BUSPOWERED_RWAKEUP
 *  - \ref USBConfigurationDescriptor_SELFPOWERED_RWAKEUP
 *  - \ref USBConfigurationDescriptor_POWER
 */
/** Device is bus-powered and not support remote wake-up. */
#define USBConfigurationDescriptor_BUSPOWERED_NORWAKEUP  0x80
/** Device is self-powered and not support remote wake-up. */
#define USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP 0xC0
/** Device is bus-powered  and supports remote wake-up. */
#define USBConfigurationDescriptor_BUSPOWERED_RWAKEUP    0xA0
/** Device is self-powered and supports remote wake-up. */
#define USBConfigurationDescriptor_SELFPOWERED_RWAKEUP   0xE0
/** Device is self-powered and supports remote wake-up. */
#define USBConfigurationDescriptor_REMOTE_WAKEUP        0x20
/** Device is bus-powered  and supports remote wake-up. */
/** Calculates the value of the power consumption field given the value in mA.
 * \param power The power consumption value in mA
 * \return The value that should be set to the field in descriptor
 */
#define USBConfigurationDescriptor_POWER(power)     (power / 2)

#define USB_HOST_POWER_MAX  500
/**     @}*/

/**
 * \brief Standard USB Interface status flags
 *
 */
enum usb_interface_status {
	USB_IFACE_STATUS_RESERVED = 0
};

/**
 * \brief Standard USB endpoint status flags
 *
 */
enum usb_endpoint_status {
	USB_EP_STATUS_HALTED = 1,
};

/**
 * \brief Standard USB endpoint feature/status flags
 */
enum usb_endpoint_feature {
	USB_EP_FEATURE_HALT = 0,
};

/**
 * \brief Standard USB device feature flags
 *
 * \note valid for SetFeature request.
 */
enum usb_device_feature {
	USB_DEV_FEATURE_REMOTE_WAKEUP = 1, //!< Remote wakeup enabled
	USB_DEV_FEATURE_TEST_MODE = 2,     //!< USB test mode
	USB_DEV_FEATURE_OTG_B_HNP_ENABLE = 3,
	USB_DEV_FEATURE_OTG_A_HNP_SUPPORT = 4,
	USB_DEV_FEATURE_OTG_A_ALT_HNP_SUPPORT = 5
};

/*--------- Endpoint Descriptors --------*/

/** \addtogroup usb_ep_define USB Endpoint definitions
 *      @{
 * This section lists definitions and macro for endpoint descriptors.
 * - \ref usb_ep_dir USB Endpoint directions
 *    - \ref USBEndpointDescriptor_OUT
 *    - \ref USBEndpointDescriptor_IN
 *
 * - \ref usb_ep_type USB Endpoint types
 *    - \ref USBEndpointDescriptor_CONTROL
 *    - \ref USBEndpointDescriptor_ISOCHRONOUS
 *    - \ref USBEndpointDescriptor_BULK
 *    - \ref USBEndpointDescriptor_INTERRUPT
 *
 *  - \ref usb_ep_size USB Endpoint maximum sizes
 *     - \ref USBEndpointDescriptor_MAXCTRLSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXCTRLSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXBULKSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXBULKSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS
 *
 *  - \ref usb_ep_addr USB Endpoint address define
 *     - \ref USBEndpointDescriptor_ADDRESS
 */

/** \addtogroup usb_ep_dir USB Endpoint directions
 *          @{
 *  This section lists definitions of USB endpoint directions.
 *  - USBEndpointDescriptor_OUT
 *  - USBEndpointDescriptor_IN
 */
/**  Endpoint receives data from the host. */
#define USBEndpointDescriptor_OUT           0
/**  Endpoint sends data to the host. */
#define USBEndpointDescriptor_IN            1

/**
 * \brief Endpoint transfer direction is IN
 */
#define  USB_EP_DIR_IN        0x80

/**
 * \brief Endpoint transfer direction is OUT
 */
#define  USB_EP_DIR_OUT       0x00
/**         @}*/

/** \addtogroup usb_ep_type USB Endpoint types
 *          @{
 *  This section lists definitions of USB endpoint types.
 *  - \ref USBEndpointDescriptor_CONTROL
 *  - \ref USBEndpointDescriptor_ISOCHRONOUS
 *  - \ref USBEndpointDescriptor_BULK
 *  - \ref USBEndpointDescriptor_INTERRUPT
 */
/**  Control endpoint type. */
#define USBEndpointDescriptor_CONTROL       0
/**  Isochronous endpoint type. */
#define USBEndpointDescriptor_ISOCHRONOUS   1
/**  Bulk endpoint type. */
#define USBEndpointDescriptor_BULK          2
/**  Interrupt endpoint type. */
#define USBEndpointDescriptor_INTERRUPT     3
   
  
/**  Synchronisation Type for Isochronous endpoint type. */
#define USBEndpointDescriptor_Asynchronous_ISOCHRONOUS          (1<<2)
#define USBEndpointDescriptor_Adaptive_ISOCHRONOUS              (2<<2)
#define USBEndpointDescriptor_Synchronous_ISOCHRONOUS           (3<<2)
   
/**  Usage Type for Isochronous endpoint type. */
#define USBEndpointDescriptor_Feedback_ISOCHRONOUS              (1<<2)
#define USBEndpointDescriptor_Explicit_Feedback_ISOCHRONOUS     (2<<2) 
/**         @}*/

/** \addtogroup usb_ep_size USB Endpoint maximum sizes
 *          @{
 *  This section lists definitions of USB endpoint maximum sizes.
 *  - \ref USBEndpointDescriptor_MAXCTRLSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXCTRLSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXBULKSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXBULKSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS
 */
/**  Maximum size for a full-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_FS                64
/**  Maximum size for a high-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_HS                64
/**  Maximum size for a full-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_FS                64
/**  Maximum size for a high-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_HS                512
/**  Maximum size for a full-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_FS           64
/**  Maximum size for a high-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_HS           1024
/**  Maximum size for a full-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS         1023
/**  Maximum size for a high-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS         1024
/**         @}*/

/** \addtogroup usb_ep_addr USB Endpoint address define
 *          @{
 *  This section lists macro for USB endpoint address definition.
 *  - \ref USBEndpointDescriptor_ADDRESS
 */
/**
 *  Calculates the address of an endpoint given its number and direction
 *  \param direction USB endpoint direction definition
 *  \param number USB endpoint number
 *  \return The value used to set the endpoint descriptor based on input number
 *          and direction
 */
#define USBEndpointDescriptor_ADDRESS(direction, number) \
    (((direction & 0x01) << 7) | (number & 0xF))
/**         @}*/
/**     @}*/


/*--------- Generic Descriptors --------*/

/** \addtogroup usb_string_descriptor USB String Descriptor Definitions
 *      @{
 *  This section lists the codes and macros for USB string descriptor definition.
 *
 *  \par Language IDs
 *  - USBStringDescriptor_ENGLISH_US
 *
 *  \par String Descriptor Length
 *  - USBStringDescriptor_LENGTH
 *
 *  \par ASCII to UNICODE conversion
 *  - USBStringDescriptor_UNICODE
 */
/** Language ID for US English. */
#define USBStringDescriptor_ENGLISH_US          0x09, 0x04
/**
 *  Calculates the length of a string descriptor given the number of ascii
 *  characters/language IDs in it.
 *  \param length The ascii format string length.
 *  \return The actual data length in bytes.
 */
#define USBStringDescriptor_LENGTH(length)      ((length) * 2 + 2)
/**
 *  Converts an ascii character to its unicode representation.
 *  \param ascii The ASCII character to convert
 *  \return A 2-byte-array for the UNICODE based on given ASCII
 */
#define USBStringDescriptor_UNICODE(ascii)      (ascii), 0
/**     @}*/


/*---------------------------------------------------------------------------
 *      Types
 *---------------------------------------------------------------------------*/

/*
 *          Function types
 */

typedef uint32_t (*USBDescriptorParseFunction)(void *descriptor, void *parseArg);


/*
 *          Descriptor structs types
 */
#pragma pack(1)
#if defined   ( __CC_ARM   ) /* Keil ¦ÌVision 4 */
#elif defined ( __ICCARM__ ) /* IAR Ewarm */
#define __attribute__(...)
#define __packed__  packed
#elif defined (  __GNUC__  ) /* GCC CS3 */
#define __packed__  aligned(1)
#endif
/**
 \typedef USBGenericDescriptor
 \brief Holds the few fields shared by all USB descriptors.
 */
typedef struct _USBGenericDescriptor {

    /**  Length of the descriptor in bytes. */
    uint8_t bLength;
    /**  Descriptor type. */
    uint8_t bDescriptorType;

} __attribute__ ((__packed__)) USBGenericDescriptor; /* GCC */

/**
 * \typedef USBDeviceDescriptor
 * \brief USB standard device descriptor structure.
 */
typedef struct _USBDeviceDescriptor {

   /**  Size of this descriptor in bytes. */
   uint8_t bLength;
   /**  Descriptor type (USBGenericDescriptor_DEVICE). */
   uint8_t bDescriptorType;
   /**  USB specification release number in BCD format. */
   uint16_t bcdUSB;
   /**  Device class code. */
   uint8_t bDeviceClass;
   /** Device subclass code. */
   uint8_t bDeviceSubClass;
   /** Device protocol code. */
   uint8_t bDeviceProtocol;
   /**  Maximum packet size of endpoint 0 (in bytes). */
   uint8_t bMaxPacketSize0;
   /** Vendor ID. */
   uint16_t idVendor;
   /** Product ID. */
   uint16_t idProduct;
   /** Device release number in BCD format. */
   uint16_t bcdDevice;
   /** Index of the manufacturer string descriptor. */
   uint8_t iManufacturer;
   /** Index of the product string descriptor. */
   uint8_t iProduct;
   /** Index of the serial number string descriptor. */
   uint8_t iSerialNumber;
   /** Number of possible configurations for the device. */
   uint8_t bNumConfigurations;

} __attribute__ ((__packed__)) USBDeviceDescriptor; /* GCC */

/**
 * \typedef USBOtgDescriptor
 * \brief USB On-The-Go descriptor struct.
 */
typedef struct _USBOtgDescriptor {

   /** Size of this descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type (USBGenericDescriptor_OTG). */
   uint8_t bDescriptorType;
   /** Attribute Fields D7?: Reserved D1: HNP support D0: SRP support */
   uint8_t bmAttributes;

} __attribute__ ((__packed__)) USBOtgDescriptor; /* GCC */

/**
 * \typedef USBDeviceQualifierDescriptor
 * \brief Alternate device descriptor indicating the capabilities of the device
 *        in full-speed, if currently in high-speed; or in high-speed, if it is
 *        currently in full-speed. Only relevant for devices supporting the
 *        high-speed mode.
 */
typedef struct _USBDeviceQualifierDescriptor {

   /** Size of the descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type (USBDESC_DEVICE_QUALIFIER or "USB device types"). */
   uint8_t bDescriptorType;
   /** USB specification release number (in BCD format). */
   uint16_t bcdUSB;
   /** Device class code. */
   uint8_t bDeviceClass;
   /** Device subclass code. */
   uint8_t bDeviceSubClass;
   /** Device protocol code. */
   uint8_t bDeviceProtocol;
   /** Maximum packet size of endpoint 0. */
   uint8_t bMaxPacketSize0;
   /** Number of possible configurations for the device. */
   uint8_t bNumConfigurations;
   /** Reserved. */
   uint8_t bReserved;

} __attribute__ ((__packed__)) USBDeviceQualifierDescriptor; /* GCC */

/**
 * \typedef USBConfigurationDescriptor
 * \brief USB standard configuration descriptor structure.
 */
typedef struct _USBConfigurationDescriptor {

   /** Size of the descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type
       (USBDESC_CONFIGURATION of \ref usb_desc_type USB Descriptor types). */
   uint8_t bDescriptorType;
   /** Length of all descriptors returned along with this configuration
       descriptor. */
   uint16_t wTotalLength;
   /** Number of interfaces in this configuration. */
   uint8_t bNumInterfaces;
   /** Value for selecting this configuration. */
   uint8_t bConfigurationValue;
   /** Index of the configuration string descriptor. */
   uint8_t iConfiguration;
   /** Configuration characteristics. */
   uint8_t bmAttributes;
   /** Maximum power consumption of the device when in this configuration. */
   uint8_t bMaxPower;

} __attribute__ ((__packed__)) USBConfigurationDescriptor; /* GCC*/

/**
 * \typedef USBDeviceBOSDescriptor
 * \brief USB Device BOS descriptor structure
 *
 * The BOS descriptor (Binary device Object Store) defines a root
 * descriptor that is similar to the configuration descriptor, and is
 * the base descriptor for accessing a family of related descriptors.
 * A host can read a BOS descriptor and learn from the wTotalLength field
 * the entire size of the device-level descriptor set, or it can read in
 * the entire BOS descriptor set of device capabilities.
 * The host accesses this descriptor using the GetDescriptor() request.
 * The descriptor type in the GetDescriptor() request is set to BOS.
 */
typedef struct _USBDeviceBOSDescriptor{
	uint8_t     bLength;
	uint8_t     bDescriptorType;
	uint16_t    wTotalLength;
	uint8_t     bNumDeviceCaps;
} __attribute__ ((__packed__)) USBDeviceBOSDescriptor;


/**
 * \typedef USBDeviceBOSDescriptor
 * \brief USB Device Capabilities - USB 2.0 Extension Descriptor structure
 *
 * Defines the set of USB 1.1-specific device level capabilities.
 */
typedef struct _USBDeviceCapabilities{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bDevCapabilityType;
	uint32_t  bmAttributes;
} __attribute__ ((__packed__))  USBDeviceCapabilities;

/**
 * \brief USB Device LPM Descriptor structure
 *
 * The BOS descriptor and capabilities descriptors for LPM.
 */
typedef struct _USB_DeviceLPMDescriptor{
	USBDeviceBOSDescriptor bos;
	USBDeviceCapabilities capa_ext;
}__attribute__ ((__packed__))  USB_DeviceLPMDescriptor;

/**
 * \typedef USBInterfaceAssociationDescriptor
 * \brief
 */
typedef struct _USBInterfaceAssociationDescriptor {

	/** Size of the descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type (USBGenericDescriptor_INTERFACE). */   
   uint8_t bDescriptorType;
   /** First Interface . */
   uint8_t bFirstInterface;
   /** Interface Count. */
   uint8_t bInterfaceCount;
   /** Function function code. */
   uint8_t bFunctionClass;
   /** Function subclass code. */
   uint8_t bFunctionSubClass;
   /** Function protocol code. */
   uint8_t bFunctionProtocol;
   /** Index of the function string descriptor. */
   uint8_t iFunction;
} __attribute__ ((__packed__)) USBInterfaceAssociationDescriptor; /* GCC*/

/**
 * \typedef USBInterfaceDescriptor
 * \brief USB standard interface descriptor structure.
 */
typedef struct _USBInterfaceDescriptor {

   /** Size of the descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type (USBGenericDescriptor_INTERFACE). */
   uint8_t bDescriptorType;
   /** Number of the interface in its configuration. */
   uint8_t bInterfaceNumber;
   /** Value to select this alternate interface setting. */
   uint8_t bAlternateSetting;
   /** Number of endpoints used by the interface (excluding endpoint 0). */
   uint8_t bNumEndpoints;
   /** Interface class code. */
   uint8_t bInterfaceClass;
   /** Interface subclass code. */
   uint8_t bInterfaceSubClass;
   /** Interface protocol code. */
   uint8_t bInterfaceProtocol;
   /** Index of the interface string descriptor. */
   uint8_t iInterface;

} __attribute__ ((__packed__)) USBInterfaceDescriptor; /* GCC */

/**
 * \typedef USBEndpointDescriptor
 * \brief USB standard endpoint descriptor structure.
 */
typedef struct _USBEndpointDescriptor {

   /**  Size of the descriptor in bytes. */
   uint8_t bLength;
   /**  Descriptor type (\ref USBGenericDescriptor_ENDPOINT). */
   uint8_t bDescriptorType;
   /**  Address and direction of the endpoint. */
   uint8_t bEndpointAddress;
   /**  Endpoint type and additional characteristics
        (for isochronous endpoints). */
   uint8_t bmAttributes;
   /**  Maximum packet size (in bytes) of the endpoint. */
   uint16_t wMaxPacketSize;
   /**  Polling rate of the endpoint. */
   uint8_t bInterval;

} __attribute__ ((__packed__)) USBEndpointDescriptor; /* GCC*/


/**
 * \brief A standard USB string descriptor structure
 */
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
}__attribute__ ((__packed__)) USBStringDescriptor;

typedef struct {
	USBStringDescriptor desc;
	uint16_t string[1];
} __attribute__ ((__packed__))USBStringLangIdDescriptor;


#pragma pack()

/*---------------------------------------------------------------------------
 *      Exported Functions
 *---------------------------------------------------------------------------*/

extern uint32_t USBGenericDescriptor_GetLength(
    const USBGenericDescriptor *descriptor);

extern uint8_t USBGenericDescriptor_GetType(
    const USBGenericDescriptor *descriptor);

extern USBGenericDescriptor *USBGenericDescriptor_GetNextDescriptor(
    const USBGenericDescriptor *descriptor);

extern USBGenericDescriptor *USBGenericDescriptor_Parse(
    const USBGenericDescriptor * descriptor,
    uint32_t totalLength,
    USBDescriptorParseFunction parseFunction,
    void * parseArg);


extern uint32_t USBConfigurationDescriptor_GetTotalLength(
    const USBConfigurationDescriptor *configuration);

extern uint8_t USBConfigurationDescriptor_GetNumInterfaces(
    const USBConfigurationDescriptor *configuration);

extern uint8_t USBConfigurationDescriptor_IsSelfPowered(
    const USBConfigurationDescriptor *configuration);

extern void USBConfigurationDescriptor_Parse(
    const USBConfigurationDescriptor *configuration,
    USBInterfaceDescriptor **interfaces,
    USBEndpointDescriptor **endpoints,
    USBGenericDescriptor **others);

extern uint8_t USBEndpointDescriptor_GetNumber(
    const USBEndpointDescriptor *endpoint);

extern uint8_t USBEndpointDescriptor_GetDirection(
    const USBEndpointDescriptor *endpoint);

extern uint8_t USBEndpointDescriptor_GetType(
    const USBEndpointDescriptor *endpoint);

extern uint16_t USBEndpointDescriptor_GetMaxPacketSize(
    const USBEndpointDescriptor *endpoint);

extern uint8_t USBEndpointDescriptor_GetInterval(
    const USBEndpointDescriptor *endpoint);


/** @}*/
/**@}*/
#endif /* #ifndef _USBDESCRIPTORS_H_ */

