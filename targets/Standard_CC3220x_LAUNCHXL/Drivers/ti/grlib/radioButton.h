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

#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

//*****************************************************************************
//
//! This structure defines the characteristics of a RadioButton
//
//*****************************************************************************
typedef struct Graphics_RadioButton
{
    uint16_t xPosition;            /*!< x coordinate for upper left corner of button */
    uint16_t yPosition;         /*!< y coordinate for upper left corner of button */
    bool selected;                /*!< draws button with selected colors enabled */
    uint8_t gap;                /*!< draws checbox with checkbox selected in pixels*/
    uint32_t textColor;            /*!< Text color */
    uint8_t numbOfChar;            /*!< Number of characters in checkbox message */
    uint32_t selectedColor;        /*!< Selected radio color */
    uint32_t notSelectedColor;    /*!< Not Selected radio color */
    const Graphics_Font *font;    /*!< Font for text to be displayed in button */
    int8_t *text;               /*!< Text to be displayed in button */
} Graphics_RadioButton;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawRadioButton(const Graphics_Context *context,
                                     const Graphics_RadioButton *radioButton);
extern bool Graphics_isRadioButtonSelected(
    const Graphics_RadioButton *radioButton,
    uint16_t x,
    uint16_t y);
extern void Graphics_drawSelectedRadioButton(const Graphics_Context *context,
                                             const Graphics_RadioButton *
                                             radioButton);
extern void Graphics_drawReleasedRadioButton(const Graphics_Context *context,
                                             const Graphics_RadioButton *
                                             radioButton);

#endif /* RADIOBUTTON_H_ */
