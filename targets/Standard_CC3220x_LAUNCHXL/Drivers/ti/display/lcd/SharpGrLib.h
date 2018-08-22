/*
 * Copyright (c) 2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//*****************************************************************************
//
// SharpGrLib.h - Prototypes for the Sharp LCD graphicslib back-end
//
//*****************************************************************************


#ifndef __SHARPLCD_H__
#define __SHARPLCD_H__

#include <ti/drivers/SPI.h>

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// Define LCD Screen Orientation Here
#define LANDSCAPE

//Maximum Colors in an image color palette
#define MAX_PALETTE_COLORS                2

#define SHARP_BLACK                       0x00
#define SHARP_WHITE                       0xFF

#define SHARP_SEND_TOGGLE_VCOM_COMMAND    0x01
#define SHARP_SKIP_TOGGLE_VCOM_COMMAND    0x00

#define SHARP_LCD_TRAILER_BYTE            0x00

#define SHARP_VCOM_TOGGLE_BIT             0x40
#define SHARP_LCD_CMD_CHANGE_VCOM         0x00
#define SHARP_LCD_CMD_CLEAR_SCREEN        0x20
#define SHARP_LCD_CMD_WRITE_LINE          0x80


//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************

//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color. This particular driver
// requires the 8-8-8 24 bit RGB color to convert into mono color
// 1 = White, 0 = Black
//
// \return Returns the display-driver specific color
//
//*****************************************************************************

#define DPYCOLORTRANSLATE(c)    ((c != 0) ? c = 1 : c) // Why not (!!c)?


//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern const Graphics_Display_Functions g_sharpFxns;

extern void SharpGrLib_sendToggleVCOMCommand();
extern void SharpGrLib_init(SPI_Handle hSpi, uint32_t csPin);

#endif // __SHARPLCD_H__
