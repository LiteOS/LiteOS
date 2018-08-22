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

#ifndef IMAGEBUTTON_H_
#define IMAGEBUTTON_H_

//*****************************************************************************
//
//! This structure defines the characteristics of a ImageButton
//
//*****************************************************************************
typedef struct Graphics_ImageButton
{
    uint16_t xPosition;        /*!< x coordinate for upper left corner of button */
    uint16_t yPosition;        /*!< y coordinate for upper left corner of button */
    uint8_t borderWidth;    /*!< border width in pixels */
    bool selected;            /*!< draws button with selected colors enabled */
    uint16_t imageHeight;    /*!< image height in pixels */
    uint16_t imageWidth;    /*!< image width in pixels */
    uint32_t borderColor;    /*!< Border color, border is drawn if borderWidth >0 */
    uint32_t selectedColor;    /*!< Background button color when selected */
    Graphics_Image *image;    /*!< Pointer to the image */
} Graphics_ImageButton;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawImageButton(const Graphics_Context *context,
                                     const Graphics_ImageButton *imageButton);
extern bool Graphics_isImageButtonSelected(
    const Graphics_ImageButton *imageButton,
    uint16_t x,
    uint16_t y);
extern void Graphics_drawSelectedImageButton(const Graphics_Context *context,
                                             const Graphics_ImageButton *
                                             imageButton);
extern void Graphics_drawReleasedImageButton(const Graphics_Context *context,
                                             const Graphics_ImageButton *
                                             imageButton);

#endif /* IMAGEBUTTON_H_ */
