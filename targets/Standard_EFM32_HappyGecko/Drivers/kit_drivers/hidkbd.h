/***************************************************************************//**
 * @file hidkbd.h
 * @brief USB Human Interface Devices (HID) class keyboard driver.
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
#ifndef __SILICON_LABS_HIDKBD_H__
#define __SILICON_LABS_HIDKBD_H__

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup HidKeyboard
 * @{
 ******************************************************************************/

#include <stdint.h>

#include "em_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

/** HID keyboard input report definition. */
SL_PACK_START(1)
typedef struct
{
  uint8_t modifier; /**< Key modifier byte as defined in HID spec. section "8.3  Report Format for Array Items". */
  uint8_t reserved; /**< Reserved, should be set to zero. */
  uint8_t key[ 6 ]; /**< Array of 6 key array indices, key index codes are defined in the "Keyboard page" section
                         of "USB HID Usage Tables document", also refer to usbkbdscancodes.c. */
} SL_ATTRIBUTE_PACKED HIDKBD_KeyReport_t;
SL_PACK_END()

/**************************************************************************//**
 * @brief
 *   Callback function pointer for HID output reports.
 *   This function will be called by the driver each time an output report is
 *   received by the device.
 *
 * @param[in] report Output report byte.
 *                   @n Bit 0 : State of keyboard NumLock LED.
 *                   @n Bit 1 : State of keyboard CapsLock LED.
 *                   @n Bit 2 : State of keyboard ScrollLock LED.
 *****************************************************************************/
typedef void (*HIDKBD_SetReportFunc_t)( uint8_t report );

/** HidKeyboard driver initialization structure.
 *  This data structure contains configuration options that the driver
 *  needs. The structure must be passed to @ref HIDKBD_Init() when initializing
 *  the driver.
 */
typedef struct
{
  void                    *hidDescriptor; /**< Pointer to the HID class descriptor in the user application. */
  HIDKBD_SetReportFunc_t  setReportFunc;  /**< Callback function pointer for HID output reports, may be NULL when no callback is needed. */
} HIDKBD_Init_t;

extern const char HIDKBD_ReportDescriptor[ 69 ];

void HIDKBD_Init( HIDKBD_Init_t *init );
int  HIDKBD_SetupCmd( const USB_Setup_TypeDef *setup );
void HIDKBD_StateChangeEvent( USBD_State_TypeDef oldState,
                              USBD_State_TypeDef newState );
void HIDKBD_KeyboardEvent( HIDKBD_KeyReport_t *report );

#ifdef __cplusplus
}
#endif

/** @} (end group HidKeyboard) */
/** @} (end group Drivers) */

#endif /* __SILICON_LABS_HIDKBD_H__ */
