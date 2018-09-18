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
#include <stdint.h>
#include <stdbool.h>
#include <ti/grlib/grlib.h>
#include <ti/grlib/imageButton.h>

//*****************************************************************************
//
//! \addtogroup imageButton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton . The ImageButton  will contain the image
//! passed in the ImageButton  struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawImageButton(const Graphics_Context *context,
                              const Graphics_ImageButton *imageButton)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle borderRect = {
        imageButton->xPosition,
        imageButton->yPosition,
        imageButton->xPosition + imageButton->imageWidth +
        (2 * imageButton->borderWidth),
        imageButton->yPosition + imageButton->imageHeight +
        (2 * imageButton->borderWidth),
    };

    if(imageButton->selected)
    {
        Graphics_setForegroundColor(&g_sContext,imageButton->selectedColor);
    }
    else
    {
        Graphics_setForegroundColor(&g_sContext,imageButton->borderColor);
    }
    Graphics_fillRectangle(&g_sContext,&borderRect);

    Graphics_drawImage(&g_sContext,imageButton->image,
                       imageButton->xPosition + imageButton->borderWidth,
                       imageButton->yPosition + imageButton->borderWidth);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Determines if x and y coordinates are contained in ImageButton  .
//!
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//! \param x x-coordinate to be determined if is inside ImageButton
//! \param y y-coordinate to be determined if is inside ImageButton
//!
//! This function determines if x and y coordinates are contains inside ImageButton
//!
//! \return true if x and y coordinates are inside ImageButton, false if not
//
//*****************************************************************************

bool Graphics_isImageButtonSelected(const Graphics_ImageButton *imageButton,
                                    uint16_t x,
                                    uint16_t y)
{
    return ((((x) >= imageButton->xPosition) &&
             ((x) <= imageButton->xPosition + imageButton->imageWidth +
              imageButton->borderWidth) &&
             ((y)) >= imageButton->yPosition) &&
            ((y) <= imageButton->yPosition + imageButton->imageHeight +
             imageButton->borderWidth) ?
            true : false);
}

//*****************************************************************************
//
//! Draws a selected ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton  using the selected parameters.
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawSelectedImageButton(const Graphics_Context *context,
                                      const Graphics_ImageButton *imageButton)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle borderRect = {
        imageButton->xPosition,
        imageButton->yPosition,
        imageButton->xPosition + imageButton->imageWidth +
        (2 * imageButton->borderWidth),
        imageButton->yPosition + imageButton->imageHeight +
        (2 * imageButton->borderWidth),
    };

    Graphics_setForegroundColor(&g_sContext,imageButton->selectedColor);

    Graphics_fillRectangle(&g_sContext,&borderRect);

    Graphics_drawImage(&g_sContext,imageButton->image,
                       imageButton->xPosition + imageButton->borderWidth,
                       imageButton->yPosition + imageButton->borderWidth);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton  using the released parameters.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawReleasedImageButton(const Graphics_Context *context,
                                      const Graphics_ImageButton *imageButton)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle borderRect = {
        imageButton->xPosition,
        imageButton->yPosition,
        imageButton->xPosition + imageButton->imageWidth +
        (2 * imageButton->borderWidth),
        imageButton->yPosition + imageButton->imageHeight +
        (2 * imageButton->borderWidth),
    };

    Graphics_setForegroundColor(&g_sContext,imageButton->borderColor);

    Graphics_fillRectangle(&g_sContext,&borderRect);

    Graphics_drawImage(&g_sContext,imageButton->image,
                       imageButton->xPosition + imageButton->borderWidth,
                       imageButton->yPosition + imageButton->borderWidth);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
