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
#include <ti/grlib/radioButton.h>

//*****************************************************************************
//
//! \addtogroup radioButton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton is a pointer to the structure containing the extents of
//! the RadioButton.
//!
//! This function draws a RadioButton  . The RadioButton   will contain the
//! image passed in the RadioButton struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawRadioButton(const Graphics_Context *context,
                              const Graphics_RadioButton *radioButton)
{
    uint8_t outerRadius, innerRadius;
    uint16_t textXPos, textYPos;
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    Graphics_setForegroundColor(&g_sContext, radioButton->textColor);

    // Define outer circle position and radius
    outerRadius = (((radioButton->font)->height) >> 1);

    Graphics_drawCircle(&g_sContext,
                        radioButton->xPosition + outerRadius,
                        radioButton->yPosition + outerRadius,
                        (uint32_t) outerRadius);

    if(radioButton->selected)
    {
        Graphics_setForegroundColor(&g_sContext, radioButton->selectedColor);
        innerRadius = outerRadius - 2;
        Graphics_fillCircle(&g_sContext,
                            radioButton->xPosition + outerRadius,
                            radioButton->yPosition + outerRadius,
                            (uint32_t) innerRadius);
    }

    textXPos = (radioButton->xPosition) + (2 * outerRadius) + (radioButton->gap);
    textYPos = (radioButton->yPosition);

    Graphics_setForegroundColor(&g_sContext, radioButton->textColor);
    Graphics_setFont(&g_sContext,radioButton->font);

    Graphics_drawString(&g_sContext,
                        radioButton->text,
                        AUTO_STRING_LENGTH,
                        textXPos,
                        textYPos,
                        TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Determines if x and y coordinates are contained in RadioButton.
//!
//! \param radioButton   is a pointer to the structure containing the extents of
//! the RadioButton.
//! \param x x-coordinate to be determined if is inside RadioButton
//! \param y y-coordinate to be determined if is inside RadioButton
//!
//! This function determines if x and y coordinates are contains inside
//! RadioButton.
//!
//! \return true if x and y coordinates are inside RadioButton, false if not
//
//*****************************************************************************

bool Graphics_isRadioButtonSelected(const Graphics_RadioButton *radioButton,
                                    uint16_t x,
                                    uint16_t y)
{
    uint16_t stringSize = radioButton->numbOfChar *
                          (radioButton->font)->maxWidth;
    uint8_t radio = (((radioButton->font)->height) >> 1);

    return ((((x) >= radioButton->xPosition) &&
             ((x) <= radioButton->xPosition + stringSize +
              (2 * radio) + radioButton->gap) &&
             ((y)) >= radioButton->yPosition) &&
            ((y) <= radioButton->yPosition + (2 * radio)) ? true : false);
}

//*****************************************************************************
//
//! Draws a selected RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton   is a pointer to the structure containing the extents
//! of the RadioButton.
//!
//! This function draws a RadioButton using the selected parameters.
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawSelectedRadioButton(const Graphics_Context *context,
                                      const Graphics_RadioButton *radioButton)
{
    uint8_t outerRadius,innerRadius;
    uint32_t origFrgnd = g_sContext.foreground;
    Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

    // Define outer circle position and radius
    outerRadius = (((radioButton->font)->height) >> 1);
    innerRadius = outerRadius - 2;

    Graphics_setForegroundColor(&g_sContext, radioButton->selectedColor);
    Graphics_fillCircle(&g_sContext,
                        radioButton->xPosition + outerRadius,
                        radioButton->yPosition + outerRadius,
                        (uint32_t) innerRadius);

    Graphics_setFont(&g_sContext,origFont);
    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton   is a pointer to the structure containing the extents of
//! the RadioButton.
//!
//! This function draws a RadioButton using the released parameters.
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawReleasedRadioButton(const Graphics_Context *context,
                                      const Graphics_RadioButton *radioButton)
{
    uint8_t outerRadius,innerRadius;
    uint32_t origFrgnd = g_sContext.foreground;

    // Define outer circle position and radius
    outerRadius = (((radioButton->font)->height) >> 1);
    innerRadius = outerRadius - 2;

    Graphics_setForegroundColor(&g_sContext,radioButton->notSelectedColor);

    Graphics_fillCircle(&g_sContext,
                        radioButton->xPosition + outerRadius,
                        radioButton->yPosition + outerRadius,
                        (uint32_t) innerRadius);

    Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
