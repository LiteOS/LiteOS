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
#include <ti/grlib/button.h>

//*****************************************************************************
//
//! \addtogroup button_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button. The button will contain a text string and will
//! be created based on the parameters passed in the button struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawButton(const Graphics_Context *context,
                         const Graphics_Button *button)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle borderRect = {
        button->xMin,
        button->yMin,
        button->xMax,
        button->yMax,
    };

    Graphics_Rectangle innerRect = {
        button->xMin + button->borderWidth,
        button->yMin + button->borderWidth,
        button->xMax - button->borderWidth,
        button->yMax - button->borderWidth,
    };

    Graphics_setForegroundColor(&g_sContext,button->borderColor);
    Graphics_fillRectangle(&g_sContext, &borderRect);

    if(button->selected)
    {
        Graphics_setForegroundColor(&g_sContext,button->selectedColor);
    }
    else
    {
        Graphics_setForegroundColor(&g_sContext,button->fillColor);
    }

    Graphics_fillRectangle(&g_sContext, &innerRect);

    Graphics_setFont(&g_sContext,button->font);

    if(button->selected)
    {
        Graphics_setForegroundColor(&g_sContext,button->selectedTextColor);
    }
    else
    {
        Graphics_setForegroundColor(&g_sContext,button->textColor);
    }
    Graphics_drawString(&g_sContext,
                        button->text,
                        AUTO_STRING_LENGTH,
                        button->textXPos,
                        button->textYPos,
                        TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Determines if x and y coordinates are contained in button .
//!
//! \param button is a pointer to the structure containing the extents of the
//! button.
//! \param x x-coordinate to be determined if is inside button
//! \param y y-coordinate to be determined if is inside button
//!
//! This function determines if x and y coordinates are contains inside button
//!
//! \return true if x and y coordinates are inside button, false if not
//
//*****************************************************************************
bool Graphics_isButtonSelected(const Graphics_Button *button,
                               uint16_t x,
                               uint16_t y)
{
    return ((((x) >= button->xMin) && ((x) <= button->xMax) &&
             ((y)) >= button->yMin) && ((y) <= button->yMax) ? true : false);
}

//*****************************************************************************
//
//! Draws a selected Button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button using the selected parameters.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawSelectedButton(const Graphics_Context *context,
                                 const Graphics_Button *button)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle innerRect = {
        button->xMin + button->borderWidth,
        button->yMin + button->borderWidth,
        button->xMax - button->borderWidth,
        button->yMax - button->borderWidth,
    };

    Graphics_setForegroundColor(&g_sContext,button->selectedColor);
    Graphics_fillRectangle(&g_sContext, &innerRect);

    Graphics_setFont(&g_sContext,button->font);

    Graphics_setForegroundColor(&g_sContext,button->selectedTextColor);
    Graphics_drawString(&g_sContext,
                        button->text,
                        AUTO_STRING_LENGTH,
                        button->textXPos,
                        button->textYPos,
                        TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released Button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button using the released parameters.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawReleasedButton(const Graphics_Context *context,
                                 const Graphics_Button *button)
{
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_Rectangle innerRect = {
        button->xMin + button->borderWidth,
        button->yMin + button->borderWidth,
        button->xMax - button->borderWidth,
        button->yMax - button->borderWidth,
    };

    Graphics_setForegroundColor(&g_sContext,button->fillColor);
    Graphics_fillRectangle(&g_sContext, &innerRect);

    Graphics_setFont(&g_sContext,button->font);

    Graphics_setForegroundColor(&g_sContext,button->textColor);
    Graphics_drawString(&g_sContext,
                        button->text,
                        AUTO_STRING_LENGTH,
                        button->textXPos,
                        button->textYPos,
                        TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
