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
//! \addtogroup context_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Initializes a drawing context.
//!
//! \param context is a pointer to the drawing context to initialize.
//! \param display is a pointer to the Graphics_Display Info structure that
//! describes the display driver to use.
//! \param pFxns isa pointer to driver functions.
//!
//! This function initializes a drawing context, preparing it for use.  The
//! provided display driver will be used for all subsequent graphics
//! operations, and the default clipping region will be set to the extent of
//! the screen.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_initContext(Graphics_Context *context,
                          Graphics_Display *display,
                          const Graphics_Display_Functions *pFxns)
{
    //
    // Check the arguments.
    //
    assert(context);
    assert(display);

    //
    // Set the size of the context.
    //
    context->size = sizeof(Graphics_Context);

    //
    // Save the pointer to the display.
    //
    context->display = display;

    //
    // Save the pointer to the display characteristics.
    //
    display->pFxns = pFxns;

    //
    // Initialize the extent of the clipping region to the extents of the
    // screen.
    //
    context->clipRegion.xMin = 0;
    context->clipRegion.yMin = 0;
    context->clipRegion.xMax = display->width - 1;
    context->clipRegion.yMax = display->heigth - 1;

    //
    // Provide a default color and font.
    //
    context->foreground = 0;
    context->background = 0;
    context->font = 0;
}

//*****************************************************************************
//
//! Sets the extents of the clipping region.
//!
//! \param context is a pointer to the drawing context to use.
//! \param rect is a pointer to the structure containing the extents of the
//! clipping region.
//!
//! This function sets the extents of the clipping region.  The clipping region
//! is not allowed to exceed the extents of the screen, but may be a portion of
//! the screen.
//!
//! The supplied coordinate are inclusive; \e xMin of 1 and \e xMax of 1 will
//! define a clipping region that will display only the pixels in the X = 1
//! column.  A consequence of this is that the clipping region must contain
//! at least one row and one column.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setClipRegion(Graphics_Context *context,
                            Graphics_Rectangle *rect)
{
    uint32_t w, h;

    //
    // Check the arguments.
    //
    assert(context);
    assert(rect);

    //
    // Get the width and height of the display.
    //
    w = Graphics_getWidthOfDisplay(context->display);
    h = Graphics_getHeightOfDisplay(context->display);

    //
    // Set the extents of the clipping region, forcing them to reside within
    // the extents of the screen.
    //
    context->clipRegion.xMin = ((rect->xMin < 0) ? 0 :
                                ((rect->xMin >= w) ? (w - 1) :
                                 rect->xMin));
    context->clipRegion.yMin = ((rect->yMin < 0) ? 0 :
                                ((rect->yMin >= h) ? (h - 1) :
                                 rect->yMin));
    context->clipRegion.xMax = ((rect->xMax < 0) ? 0 :
                                ((rect->xMax >= w) ? (w - 1) :
                                 rect->xMax));
    context->clipRegion.yMax = ((rect->yMax < 0) ? 0 :
                                ((rect->yMax >= h) ? (h - 1) :
                                 rect->yMax));
}

//*****************************************************************************
//
//! Sets the background color to be used.
//!
//! \param context is a pointer to the drawing context to modify.
//! \param value is the 24-bit RGB color to be used.
//!
//! This function sets the background color to be used for drawing operations
//! in the specified drawing context.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setBackgroundColor(Graphics_Context *context,
                                 int32_t value)
{
    Graphics_Context *pC = context;
    pC->background = Graphics_translateColorOnDisplay(pC->display, value);
}

//*****************************************************************************
//
//! Sets the background color to be used.
//!
//! \param context is a pointer to the drawing context to modify.
//! \param value is the display driver-specific color to be used.
//!
//! This function sets the background color to be used for drawing operations
//! in the specified drawing context, using a color that has been previously
//! translated to a driver-specific color (for example, via
//! Graphics_translateColorDisplay()).
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setBackgroundColorTranslated(Graphics_Context *context,
                                           int32_t value)
{
    Graphics_Context *pC = context;
    pC->background = value;
}

//*****************************************************************************
//
//! Sets the font to be used.
//!
//! \param context is a pointer to the drawing context to modify.
//! \param font is a pointer to the font to be used.
//!
//! This function sets the font to be used for string drawing operations in the
//! specified drawing context.  If a tFontEx type font is to be used, cast its
//! pointer to a font pointer before passing it as the font parameter.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setFont(Graphics_Context *context,
                      const Graphics_Font *font)
{
    Graphics_Context *pC = context;
    const Graphics_Font *pF = font;
    pC->font = pF;
}

//*****************************************************************************
//
//! Gets the baseline of a font.
//!
//! \param font is a pointer to the font to query.
//!
//! This function determines the baseline position of a font.  The baseline is
//! the offset between the top of the font and the bottom of the capital
//! letters.  The only font data that exists below the baseline are the
//! descenders on some lower-case letters (such as ``y'').
//!
//! \return Returns the baseline of the font, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getFontBaseline(const Graphics_Font *font)
{
    return(font->baseline);
}

//*****************************************************************************
//
//! Sets the foreground color to be used.
//!
//! \param context is a pointer to the drawing context to modify.
//! \param value is the 24-bit RGB color to be used.
//!
//! This function sets the color to be used for drawing operations in the
//! specified drawing context.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setForegroundColor(Graphics_Context *context,
                                 int32_t value)
{
    context->foreground = Graphics_translateColorOnDisplay(context->display,
                                                           value);
}

//*****************************************************************************
//
//! Sets the foreground color to be used.
//!
//! \param context is a pointer to the drawing context to modify.
//! \param value is the display driver-specific color to be used.
//!
//! This function sets the foreground color to be used for drawing operations
//! in the specified drawing context, using a color that has been previously
//! translated to a driver-specific color (for example, via
//! Graphics_translateColorDisplay()).
//!
//! \return None.
//
//*****************************************************************************
void Graphics_setForegroundColorTranslated(Graphics_Context *context,
                                           int32_t value)
{
    context->foreground = value;
}

//*****************************************************************************
//
//! Gets the height of a font.
//!
//! \param font is a pointer to the font to query.
//!
//! This function determines the height of a font.  The height is the offset
//! between the top of the font and the bottom of the font, including any
//! ascenders and descenders.
//!
//! \return Returns the height of the font, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getFontHeight(const Graphics_Font *font)
{
    return(font->height);
}

//*****************************************************************************
//
//! Gets the maximum width of a font.
//!
//! \param font is a pointer to the font to query.
//!
//! This function determines the maximum width of a font.  The maximum width is
//! the width of the widest individual character in the font.
//!
//! \return Returns the maximum width of the font, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getFontMaxWidth(const Graphics_Font *font)
{
    return(font->maxWidth);
}

//*****************************************************************************
//
//! Gets the width of the display being used by this drawing context.
//!
//! \param context is a pointer to the drawing context to query.
//!
//! This function returns the width of the display that is being used by this
//! drawing context.
//!
//! \return Returns the width of the display in pixels.
//
//*****************************************************************************
uint16_t Graphics_getDisplayWidth(Graphics_Context *context)
{
    return(context->display->width);
}

//*****************************************************************************
//
//! Gets the height of the display being used by this drawing context.
//!
//! \param context is a pointer to the drawing context to query.
//!
//! This function returns the height of the display that is being used by this
//! drawing context.
//!
//! \return Returns the height of the display in pixels.
//
//*****************************************************************************
uint16_t Graphics_getDisplayHeight(Graphics_Context *context)
{
    return(context->display->heigth);
}

//*****************************************************************************
//
//! Gets the height of the display.
//!
//! \param display is a pointer to the display driver structure for the
//! display to query.
//!
//! This function determines the height of the display.
//!
//! \return Returns the height of the display in pixels.
//
//*****************************************************************************
uint16_t Graphics_getHeightOfDisplay(const Graphics_Display *display)
{
    return(display->heigth);
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param context is a pointer to the drawing context to use.
//!
//! This function flushes any cached drawing operations.  For display drivers
//! that draw into a local frame buffer before writing to the actual display,
//! calling this function will cause the display to be updated to match the
//! contents of the local frame buffer.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_flushBuffer(const Graphics_Context *context)
{
    Graphics_flushOnDisplay(context->display);
}

//*****************************************************************************
//
//! Forces a clear screen. Contents of Display buffer unmodified
//!
//! \param context is a pointer to the drawing context to use.
//!
//! This function forces a clear screen.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_clearDisplay(const Graphics_Context *context)
{
    Graphics_clearDisplayOnDisplay(context->display,context->background);
}

//*****************************************************************************
//
//! Draws a pixel.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the pixel.
//! \param y is the Y coordinate of the pixel.
//!
//! This function draws a pixel if it resides within the clipping region.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawPixel(const Graphics_Context *context,
                        uint16_t x,
                        uint16_t y)
{
    if((x >= context->clipRegion.xMin) &&
       (x <= context->clipRegion.xMax) &&
       (y >= context->clipRegion.yMin) &&
       (y <= context->clipRegion.yMax))
    {
        Graphics_drawPixelOnDisplay(context->display, x, y,
                                    context->foreground);
    }
}

//*****************************************************************************
//
//! Gets the width of the display.
//!
//! \param display is a pointer to the display driver structure for the
//! display to query.
//!
//! This function determines the width of the display.
//!
//! \return Returns the width of the display in pixels.
//
//*****************************************************************************
uint16_t Graphics_getWidthOfDisplay(const Graphics_Display *display)
{
    return(display->width);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
