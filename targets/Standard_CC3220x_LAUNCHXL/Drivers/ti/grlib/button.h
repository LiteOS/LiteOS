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
#ifndef BUTTON_H_
#define BUTTON_H_

//*****************************************************************************
// typedefs
//*****************************************************************************

//! \brief This structure defines the characteristics of a Button
//!

typedef struct Graphics_Button
{
    uint16_t xMin;           /*!< x coordinate for upper left corner of button */
    uint16_t xMax;         /*!< x coordinate for lower right corner of button */
    uint16_t yMin;         /*!< y coordinate for upper left corner of button */
    uint16_t yMax;         /*!< y coordinate for lower right corner of button */
    uint8_t borderWidth;   /*!< border width */
    bool selected;           /*!< draws button with selected colors enabled */
    uint32_t fillColor;    /*!< Background button color when not selected */
    uint32_t borderColor;  /*!< Border color, border is drawn if borderWidth >0 */
    uint32_t selectedColor; /*!< Background button color when selected */
    uint32_t textColor;       /*!< Text color */
    uint32_t selectedTextColor; /*!< Selected text color */
    uint16_t textXPos;     /*!< x coordinate for text upper left corner */
    uint16_t textYPos;     /*!< y coordinate for text upper left corner */
    int8_t *text;           /*!< Text to be displayed in button */
    const Graphics_Font *font; /*!< Font for text to be displayed in button */
} Graphics_Button;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawButton(const Graphics_Context *context,
                                const Graphics_Button *button);
extern bool Graphics_isButtonSelected(const Graphics_Button *button,
                                      uint16_t x,
                                      uint16_t y);
extern void Graphics_drawSelectedButton(const Graphics_Context *context,
                                        const Graphics_Button *button);
extern void Graphics_drawReleasedButton(const Graphics_Context *context,
                                        const Graphics_Button *button);

#endif /* BUTTON_H_ */
