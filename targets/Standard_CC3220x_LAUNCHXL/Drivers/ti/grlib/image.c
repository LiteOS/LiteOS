/* --COPYRIGHT--,BSD
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
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
//! \addtogroup image_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// The buffer that holds the converted palette of the image to be drawn. This
// buffer contains the actual data to be written to the LCD after translation.
//
//*****************************************************************************
static uint32_t g_pulConvertedPalette[256];

//*****************************************************************************
//
//! Converts the bitmap image palette.
//!
//! \param context is a pointer to the drawing context to use.
//! \param image is a pointer to the image.
//!
//! This function converts the palette of a bitmap image. The image palette is
//! in 24 bit RGB form, and this function converts that to a format to be sent
//! to the LCD using DpyColorTranslate function. The converted palette is
//! contained in a global buffer while the original image palette remains
//! unchanged.
//!
//! \return is the address of the global buffer containing the converted palette.
//
//*****************************************************************************
static uint32_t * Graphics_convertPalette(const Graphics_Context *context,
                                          const Graphics_Image *image)
{
    if(image->numColors > 256)
    {
        return(0);
    }

    int16_t i;
    int16_t numColors = image->numColors;
    const uint32_t  * pulPalette = &image->pPalette[0];
    for(i = 0; i < numColors; i++)
    {
        g_pulConvertedPalette[i] = Graphics_translateColorOnDisplay(
            context->display, *(pulPalette + i));
    }
    return(&g_pulConvertedPalette[0]);
}

//*****************************************************************************
//
//! Draws a bitmap image.
//!
//! \param context is a pointer to the drawing context to use.
//! \param bitmap is a pointer to the image to draw.
//! \param x is the X coordinate of the upper left corner of the image.
//! \param y is the Y coordinate of the upper left corner of the image.
//!
//! This function draws a bitmap image.  The image may be 1 bit per pixel, 4
//! bits per pixel or 8 bits per pixel (using a palette supplied in the image
//! data).  It can be uncompressed data, or it can be compressed using
//! several different compression types. Compression options are 4-bit run
//! length encoding, 8-bit run length encoding, and a custom run length encoding
//! variation written for complex 8-bit per pixel images.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawImage(const Graphics_Context *context,
                        const Graphics_Image *bitmap,
                        int16_t x,
                        int16_t y)
{
    int16_t bPP, width, height, x0, x1, x2;
    const uint32_t *palette;
    const uint8_t *image;

    //
    // Check the arguments.
    //
    assert(context);
    assert(bitmap);

    //
    // Get the image format from the image data.
    //
    bPP = bitmap->bPP;

    //
    // Get the image width from the image data.
    //
    width = bitmap->xSize;

    //
    // Get the image height from the image data.
    //
    height = bitmap->ySize;

    //
    // Return without doing anything if the entire image lies outside the
    // current clipping region.
    //
    if((x > context->clipRegion.xMax) ||
       ((x + width - 1) < context->clipRegion.xMin) ||
       (y > context->clipRegion.yMax) ||
       ((y + height - 1) < context->clipRegion.yMin))
    {
        return;
    }

    //
    // Get the starting X offset within the image based on the current clipping
    // region.
    //
    if(x < context->clipRegion.xMin)
    {
        x0 = context->clipRegion.xMin - x;
    }
    else
    {
        x0 = 0;
    }

    //
    // Get the ending X offset within the image based on the current clipping
    // region.
    //
    if((x + width - 1) > context->clipRegion.xMax)
    {
        x2 = context->clipRegion.xMax - x;
    }
    else
    {
        x2 = width - 1;
    }

    //
    // Reduce the height of the image, if required, based on the current
    // clipping region.
    //
    if((y + height - 1) > context->clipRegion.yMax)
    {
        height = context->clipRegion.yMax - y + 1;
    }

    //
    // The image palette is in 24 bit R-G-B format. The palette needs
    // to be translated into the color format accepted by the LCD
    // and stored in g_pulConvertedPalette[] buffer
    //
    palette = Graphics_convertPalette(context, bitmap);
    image = bitmap->pPixel;

    //
    // Check if the image is not compressed.
    //
    if(!(bPP & 0xF0))
    {
        //
        // The image is not compressed.  See if the top portion of the image
        // lies above the clipping region.
        //
        if(y < context->clipRegion.yMin)
        {
            //
            // Determine the number of rows that lie above the clipping region.
            //
            x1 = context->clipRegion.yMin - y;

            //
            // Skip past the data for the rows that lie above the clipping
            // region.
            //
            image += (((width * bPP) + 7) / 8) * x1;

            //
            // Decrement the image height by the number of skipped rows.
            //
            height -= x1;

            //
            // Increment the starting Y coordinate by the number of skipped
            // rows.
            //
            y += x1;
        }

        while(height--)
        {
            //
            // Draw this row of image pixels.
            //
            Graphics_drawMultiplePixelsOnDisplay(context->display, x + x0, y,
                                                 x0 & 7, x2 - x0 + 1, bPP,
                                                 image + ((x0 * bPP) / 8),
                                                 palette);

            //
            // Skip past the data for this row.
            //
            image += ((width * bPP) + 7) / 8;

            //
            // Increment the Y coordinate.
            //
            y++;
        }
    }
    else
    {
        //
        // The image is compressed with RLE4, RLE7 or RLE8 Algorithm
        //

        const uint8_t *pucData = image;
        uint8_t ucRunLength, ucTempRun, rleType;
        uint8_t ucNoOverrun = 1;
        uint16_t uiColor;
        uint16_t uiLineCnt = 0;
        uint16_t xS = x;

        rleType = (bPP >> 4) & 0x0F;
        bPP &= 0x0F;

        while(height--)
        {
            //
            // Draw this row of image pixels.
            //
            while(uiLineCnt < width)
            {
                if(ucNoOverrun)     // Read next byte
                {
                    if(rleType == 8)   // RLE 8 bit encoding
                    {
                        // Read Run Length
                        ucRunLength = *pucData++;
                        // Read Color Pointer
                        uiColor = *pucData++;
                    }
                    else if(rleType == 7)     // RLE 7 bit encoding
                    {
                        // Read Run Length
                        ucRunLength = (*pucData) >> 1;
                        // Read Color Pointer
                        uiColor = (*pucData++) & 0x01;
                    }
                    else  // rleType = 4; RLE 4 bit encoding
                    {
                        // Read Run Length
                        ucRunLength = (*pucData) >> 4;
                        // Read Color Pointer
                        uiColor = (*pucData++) & 0x0F;
                    }
                    uiColor = (*(uint16_t *)(palette + uiColor));
                }
                else
                {
                    //Last line run overran the edge
                    //Skip color and run length update and clear overrun flag
                    ucNoOverrun = 1;
                    ucRunLength = ucTempRun;
                }

                //After original value, but it still needs to be written once
                if((ucRunLength + uiLineCnt) > width)
                {
                    //Run length runs past current line

                    //Set left over run length for next line to ucTempRun
                    ucTempRun = ucRunLength - (width - uiLineCnt);
                    //Set ucRunLength to finish this line
                    ucRunLength = (width - uiLineCnt - 1);
                    //Set line overrun flag
                    ucNoOverrun = 0;
                }

                //Draw single pixel
                Graphics_drawPixelOnDisplay(context->display, x, y, uiColor);
                x++;

                uiLineCnt++;

                while(ucRunLength--)
                {
                    //Draw run of pixels
                    Graphics_drawPixelOnDisplay(context->display, x, y, uiColor);

                    x++;

                    uiLineCnt++;
                }
            }
            //End of line reached
            uiLineCnt = 0;    //Reset Pixel Count
            y++;             //Increment Y value
            x = xS;
        }
    }
}

//*****************************************************************************
//
//! Gets the number of colors in an image.
//!
//! \param image is a tImage struct
//!
//! This function determines the number of colors in the palette of an image.
//! This is only valid for 4bpp and 8bpp images; 1bpp images do not contain a
//! palette.
//!
//! \return Returns the number of colors in the image.
//
//*****************************************************************************
uint16_t Graphics_getImageColors(const Graphics_Image *image)
{
    return(image->numColors);
}

//*****************************************************************************
//
//! Gets the height of an image.
//!
//! \param image is a tImage struct
//!
//! This function determines the height of an image in pixels.
//!
//! \return Returns the height of the image in pixels.
//
//*****************************************************************************
uint16_t Graphics_getImageHeight(const Graphics_Image *image)
{
    return(image->ySize);
}

//*****************************************************************************
//
//! Gets the width of an image.
//!
//! \param image is a tImage struct
//!
//! This function determines the width of an image in pixels.
//!
//! \return Returns the width of the image in pixels.
//
//*****************************************************************************
uint16_t Graphics_getImageWidth(const Graphics_Image *image)
{
    return(image->xSize);
}

//*****************************************************************************
//
//! Determines the size of the buffer for a 1 BPP off-screen image.
//!
//! \param width is the width of the image in pixels.
//! \param height is the height of the image in pixels.
//!
//! This function determines the size of the memory buffer required to hold a
//! 1 BPP off-screen image of the specified geometry.
//!
//! \return Returns the number of bytes required by the image.
//
//*****************************************************************************
uint32_t Graphics_getOffscreen1BppImageSize(uint16_t width,
                                            uint16_t height)
{
    return (5 + (((width + 7) / 8) * height));
}

//*****************************************************************************
//
//! Determines the size of the buffer for a 4 BPP off-screen image.
//!
//! \param width is the width of the image in pixels.
//! \param height is the height of the image in pixels.
//!
//! This function determines the size of the memory buffer required to hold a
//! 4 BPP off-screen image of the specified geometry.
//!
//! \return Returns the number of bytes required by the image.
//
//*****************************************************************************
uint32_t Graphics_getOffscreen4BppImageSize(uint16_t width,
                                            uint16_t height)
{
    return (6 + (16 * 3) + (((width + 1) / 2) * height));
}

//*****************************************************************************
//
//! Determines the size of the buffer for an 8 BPP off-screen image.
//!
//! \param width is the width of the image in pixels.
//! \param height is the height of the image in pixels.
//!
//! This function determines the size of the memory buffer required to hold an
//! 8 BPP off-screen image of the specified geometry.
//!
//! \return Returns the number of bytes required by the image.
//
//*****************************************************************************
uint32_t Graphics_getOffScreen8BPPSize(uint16_t width,
                                       uint16_t height)
{
    return (6 + (256 * 3) + (width * height));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
