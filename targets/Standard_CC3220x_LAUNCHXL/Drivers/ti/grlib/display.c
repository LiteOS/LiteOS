/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/

#include <ti/grlib/grlib.h>

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a pixel on a display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param x is the X coordinate of the pixel.
//! \param y is the Y coordinate of the pixel.
//! \param value is the color to draw the pixel.
//!
//! This function draws a pixel on a display.  This assumes that clipping has
//! already been performed.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawPixelOnDisplay(const Graphics_Display *display,
                                 uint16_t x,
                                 uint16_t y,
                                 uint16_t value)
{
    display->pFxns->pfnPixelDraw(display, x, y, value);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on a display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param x is the X coordinate of the first pixel.
//! \param y is the Y coordinate of the first pixel.
//! \param x0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param count is the number of pixels to draw.
//! \param bPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param data is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param palette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on a display, using the
//! supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawMultiplePixelsOnDisplay(const Graphics_Display *display,
                                          uint16_t x,
                                          uint16_t y,
                                          uint16_t x0,
                                          uint16_t count,
                                          uint16_t bPP,
                                          const uint8_t *data,
                                          const uint32_t *palette)
{
    display->pFxns->pfnPixelDrawMultiple(display, x, y, x0, count,
                                         bPP,
                                         data,
                                         palette);
}

//*****************************************************************************
//
//! Draws a horizontal line on a display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param x1 is the starting X coordinate of the line.
//! \param x2 is the ending X coordinate of the line.
//! \param y is the Y coordinate of the line.
//! \param value is the color to draw the line.
//!
//! This function draws a horizontal line on a display.  This assumes that
//! clipping has already been performed, and that both end points of the line
//! are within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawHorizontalLineOnDisplay(const Graphics_Display *display,
                                          uint16_t x1,
                                          uint16_t x2,
                                          uint16_t y,
                                          uint32_t value)
{
    display->pFxns->pfnLineDrawH(display, x1, x2, y, value);
}

//*****************************************************************************
//
//! Draws a vertical line on a display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param x is the X coordinate of the line.
//! \param y1 is the starting Y coordinate of the line.
//! \param y2 is the ending Y coordinate of the line.
//! \param value is the color to draw the line.
//!
//! This function draws a vertical line on a display.  This assumes that
//! clipping has already been performed, and that both end points of the line
//! are within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawVerticalLineOnDisplay(const Graphics_Display *display,
                                        uint16_t x,
                                        uint16_t y1,
                                        uint16_t y2,
                                        uint16_t value)
{
    display->pFxns->pfnLineDrawV(display, x, y1, y2, value);
}

//*****************************************************************************
//
//! Fills a rectangle on a display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param rect is a pointer to the structure describing the rectangle to
//! fill.
//! \param value is the color to fill the rectangle.
//!
//! This function fills a rectangle on the display.  This assumes that clipping
//! has already been performed, and that all sides of the rectangle are within
//! the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_fillRectangleOnDisplay(const Graphics_Display *display,
                                     const Graphics_Rectangle *rect,
                                     uint16_t value)
{
    display->pFxns->pfnRectFill(display, rect, value);
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param value is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
uint32_t Graphics_translateColorOnDisplay(const Graphics_Display *display,
                                          uint32_t value)
{
    return(display->pFxns->pfnColorTranslate(display, value));
}

//*****************************************************************************
//
//! Flushes cached drawing operations.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//!
//! This function flushes any cached drawing operations on a display.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_flushOnDisplay(const Graphics_Display *display)
{
    display->pFxns->pfnFlush(display);
}

//*****************************************************************************
//
//! Forces a clear Display.
//!
//! \param display is the pointer to the display driver structure for the
//! display to operate upon.
//! \param value is the current background color to fill the screen with.
//!
//! This function clears the Display
//!
//! \return None.
//
//*****************************************************************************
void Graphics_clearDisplayOnDisplay(const Graphics_Display *display,
                                    uint16_t value)
{
    display->pFxns->pfnClearDisplay(display, value);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
