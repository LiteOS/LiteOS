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
//! \addtogroup string_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// The character printed by GrStringDraw in place of any character in the
// string which does not appear in the font.  When using a font which does not
// include this character, a space is left instead.
//
//*****************************************************************************
#define GRAPHICS_ABSENT_CHAR_REPLACEMENT '.'

//*****************************************************************************
//
// Counts the number of zeros at the start of a word.
//
//*****************************************************************************
static int16_t Graphics_getNumberOfLeadingZeros(int32_t x)
{
    int32_t y = 0x80, count = 0;
    int32_t i;
    for(i = 0; i < 32; i++)
    {
        if(0x00 != (x & y))
        {
            break;
        }
        count++;
        y = y >> 1;
    }
    return(count);
}

//*****************************************************************************
//
//! Determines the width of a string.
//!
//! \param context is a pointer to the drawing context to use.
//! \param string is the string in question.
//! \param length is the length of the string.
//!
//! This function determines the width of a string (or portion of the string)
//! when drawn with a particular font.  The \e length parameter allows a
//! portion of the string to be examined without having to insert a NULL
//! character at the stopping point (would not be possible if the string was
//! located in flash); specifying a length of -1 will cause the width of the
//! entire string to be computed.
//!
//! \return Returns the width of the string in pixels.
//
//*****************************************************************************
int32_t Graphics_getStringWidth(const Graphics_Context *context,
                                const int8_t *string,
                                int32_t length)
{
    const uint16_t *offset;
    const uint8_t *data;
    uint8_t first, last, absent;
    int32_t width;

    //
    // Check the arguments.
    //
    assert(context);
    assert(string);

    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
    if(context->font->format & GRAPHICS_FONT_EX_MARKER)
    {
        Graphics_FontEx *font;

        font = (Graphics_FontEx *)(context->font);

        data = font->data;
        offset = font->offset;
        first = font->first;
        last = font->last;

        //
        // Does the default absent character replacement exist in the font?
        //
        if((GRAPHICS_ABSENT_CHAR_REPLACEMENT >= first) &&
           (GRAPHICS_ABSENT_CHAR_REPLACEMENT <= last))
        {
            //
            // Yes - use the standard character when an absent character is
            // found.
            //
            absent = GRAPHICS_ABSENT_CHAR_REPLACEMENT;
        }
        else
        {
            //
            // The default absent character is not present in the font so use
            // the first character (we only use its width here) instead.
            //
            absent = font->first;
        }
    }
    else
    {
        data = context->font->data;
        offset = context->font->offset;
        first = 32;
        last = 126;
        absent = GRAPHICS_ABSENT_CHAR_REPLACEMENT;
    }

    //
    // Loop through the characters in the string.
    //
    for(width = 0; *string && length; string++, length--)
    {
        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if((*string >= first) && (*string <= last))
        {
            //
            // Add the width of this character as drawn with the given font.
            //
            width += data[offset[*string - first] + 1];
        }
        else
        {
            //
            // This character does not exist in the font so replace it with
            // a '.' instead.  This matches the approach taken in GrStringDraw
            // and ensures that the width returned here represents the
            // rendered dimension of the string.
            //
            width += data[offset[absent - first] + 1];
        }
    }

    //
    // Return the width of the string.
    //
    return(width);
}

//*****************************************************************************
//
//! Draws a string.
//!
//! \param context is a pointer to the drawing context to use.
//! \param string is a pointer to the string to be drawn.
//! \param length is the number of characters from the string that should be
//! drawn on the screen.
//! \param x is the X coordinate of the upper left corner of the string
//! position on the screen.
//! \param y is the Y coordinate of the upper left corner of the string
//! position on the screen.
//! \param opaque is true if the background of each character should be drawn
//! and false if it should not (leaving the background as is).
//!
//! This function draws a string of test on the screen.  The \e length
//! parameter allows a portion of the string to be examined without having to
//! insert a NULL character at the stopping point (which would not be possible
//! if the string was located in flash); specifying a length of -1 will cause
//! the entire string to be rendered (subject to clipping).
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawString(const Graphics_Context *context,
                         int8_t *string,
                         int32_t length,
                         int32_t x,
                         int32_t y,
                         bool opaque)
{
    int32_t idx, x0, y0, count, off, on, bit;
    const uint8_t *data;
    const uint8_t *glyphs;
    const uint16_t *offset;
    uint8_t first, last, absent;
    Graphics_Context sContext;

    int32_t ySave = y;
    y = 0;

    //
    // Check the arguments.
    //
    assert(context);
    assert(string);

    //
    // Copy the drawing context into a local structure that can be modified.
    //
    sContext = *context;

    //
    // Extract various parameters from the font depending upon whether it's
    // in the tFont or Graphics_FontEx format.
    //
    if(context->font->format & GRAPHICS_FONT_EX_MARKER)
    {
        Graphics_FontEx *font;

        font = (Graphics_FontEx *)(context->font);

        glyphs = font->data;
        offset = font->offset;
        first = font->first;
        last = font->last;

        //
        // Does the default absent character replacement exist in the font?
        //
        if((GRAPHICS_ABSENT_CHAR_REPLACEMENT >= first) &&
           (GRAPHICS_ABSENT_CHAR_REPLACEMENT <= last))
        {
            //
            // Yes - use the standard character when an absent character is
            // found.
            //
            absent = GRAPHICS_ABSENT_CHAR_REPLACEMENT;
        }
        else
        {
            //
            // The default absent character is not present in the font so use
            // the first character instead.
            //
            absent = font->first;
        }
    }
    else
    {
        glyphs = context->font->data;
        offset = context->font->offset;
        first = 32;
        last = 126;
        absent = GRAPHICS_ABSENT_CHAR_REPLACEMENT;
    }

    //
    // Loop through the characters in the string.
    //
    while(*string && length--)
    {
        //
        // Stop drawing the string if the right edge of the clipping region has
        // been exceeded.
        //
        if(x > sContext.clipRegion.xMax)
        {
            break;
        }

        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with the "absent" character (usually '.').
        //
        if((*string >= first) && (*string <= last))
        {
            data = (glyphs + offset[*string - first]);
        }
        else
        {
            data = (glyphs + offset[absent - first]);
        }
        string++;

        //
        // See if the entire character is to the left of the clipping region.
        //
        if((x + data[1]) < sContext.clipRegion.xMin)
        {
            //
            // Increment the X coordinate by the width of the character.
            //
            x += data[1];

            //
            // Go to the next character in the string.
            //
            continue;
        }

        //
        // Loop through the bytes in the encoded data for this glyph.
        //
        for(idx = 2, x0 = 0, bit = 0, y0 = 0; idx < data[0]; )
        {
            //
            // See if the bottom of the clipping region has been exceeded.
            //
            if((y + y0) > sContext.clipRegion.yMax)
            {
                //
                // Stop drawing this character.
                //
                break;
            }

            //
            // See if the font is uncompressed.
            //
            if((sContext.font->format & ~GRAPHICS_FONT_EX_MARKER) ==
               GRAPHICS_FONT_FMT_UNCOMPRESSED)
            {
                //
                // Count the number of off pixels from this position in the
                // glyph image.
                //
                for(off = 0; idx < data[0]; )
                {
                    //
                    // Get the number of zero pixels at this position.
                    //
                    count = Graphics_getNumberOfLeadingZeros(data[idx] << bit);

                    //
                    // If there were more than 8, then it is a "false" result
                    // since it counted beyond the end of the current byte.
                    // Therefore, simply limit it to the number of pixels
                    // remaining in this byte.
                    //
                    if(count > 8)
                    {
                        count = 8 - bit;
                    }

                    //
                    // Increment the number of off pixels.
                    //
                    off += count;

                    //
                    // Increment the bit position within the byte.
                    //
                    bit += count;

                    //
                    // See if the end of the byte has been reached.
                    //
                    if(bit == 8)
                    {
                        //
                        // Advance to the next byte and continue counting off
                        // pixels.
                        //
                        bit = 0;
                        idx++;
                    }
                    else
                    {
                        //
                        // Since the end of the byte was not reached, there
                        // must be an on pixel.  Therefore, stop counting off
                        // pixels.
                        //
                        break;
                    }
                }

                //
                // Count the number of on pixels from this position in the
                // glyph image.
                //
                for(on = 0; idx < data[0]; )
                {
                    //
                    // Get the number of one pixels at this location (by
                    // inverting the data and counting the number of zeros).
                    //
                    count = Graphics_getNumberOfLeadingZeros(~(data[idx] << bit));

                    //
                    // If there were more than 8, then it is a "false" result
                    // since it counted beyond the end of the current byte.
                    // Therefore, simply limit it to the number of pixels
                    // remaining in this byte.
                    //
                    if(count > 8)
                    {
                        count = 8 - bit;
                    }

                    //
                    // Increment the number of on pixels.
                    //
                    on += count;

                    //
                    // Increment the bit position within the byte.
                    //
                    bit += count;

                    //
                    // See if the end of the byte has been reached.
                    //
                    if(bit == 8)
                    {
                        //
                        // Advance to the next byte and continue counting on
                        // pixels.
                        //
                        bit = 0;
                        idx++;
                    }
                    else
                    {
                        //
                        // Since the end of the byte was not reached, there
                        // must be an off pixel.  Therefore, stop counting on
                        // pixels.
                        //
                        break;
                    }
                }
            }
            //
            // Otherwise, the font is compressed with a pixel RLE scheme.
            //
            else
            {
                //
                // See if this is a byte that encodes some on and off pixels.
                //
                if(data[idx])
                {
                    //
                    // Extract the number of off pixels.
                    //
                    off = (data[idx] >> 4) & 15;

                    //
                    // Extract the number of on pixels.
                    //
                    on = data[idx] & 15;

                    //
                    // Skip past this encoded byte.
                    //
                    idx++;
                }
                //
                // Otherwise, see if this is a repeated on pixel byte.
                //
                else if(data[idx + 1] & 0x80)
                {
                    //
                    // There are no off pixels in this encoding.
                    //
                    off = 0;

                    //
                    // Extract the number of on pixels.
                    //
                    on = (data[idx + 1] & 0x7f) * 8;

                    //
                    // Skip past these two encoded bytes.
                    //
                    idx += 2;
                }
                //
                // Otherwise, this is a repeated off pixel byte.
                //
                else
                {
                    //
                    // Extract the number of off pixels.
                    //
                    off = data[idx + 1] * 8;

                    //
                    // There are no on pixels in this encoding.
                    //
                    on = 0;

                    //
                    // Skip past these two encoded bytes.
                    //
                    idx += 2;
                }
            }

            //
            // Loop while there are any off pixels.
            //
            while(off)
            {
                //
                // See if the bottom of the clipping region has been exceeded.
                //
                if((y + y0) > sContext.clipRegion.yMax)
                {
                    //
                    // Ignore the remainder of the on pixels.
                    //
                    break;
                }

                //
                // See if there is more than one on pixel that will fit onto
                // the current row.
                //
                if((off > 1) && ((x0 + 1) < data[1]))
                {
                    //
                    // Determine the number of on pixels that will fit on this
                    // row.
                    //
                    count = (((x0 + off) > data[1]) ? data[1] - x0 :
                             off);

                    //
                    // If this row is within the clipping region, draw a
                    // horizontal line that corresponds to the sequence of on
                    // pixels.
                    //
                    if(((y + y0) >= sContext.clipRegion.yMin) && opaque)
                    {
                        sContext.foreground = context->background;
                        Graphics_drawLineH(&sContext, x + x0, x + x0 + count -
                                           1,
                                           ySave + y + y0);
                    }

                    //
                    // Decrement the count of on pixels by the number on this
                    // row.
                    //
                    off -= count;

                    //
                    // Increment the X offset by the number of on pixels.
                    //
                    x0 += count;
                }
                //
                // Otherwise, there is only a single on pixel that can be
                // drawn.
                //
                else
                {
                    //
                    // If this pixel is within the clipping region, then draw
                    // it.
                    //
                    if(((x + x0) >= sContext.clipRegion.xMin) &&
                       ((x + x0) <= sContext.clipRegion.xMax) &&
                       ((y + y0) >= sContext.clipRegion.yMin) && opaque)
                    {
                        Graphics_drawPixelOnDisplay(context->display, x + x0,
                                                    ySave + y + y0,
                                                    context->background);
                    }

                    //
                    // Decrement the count of on pixels.
                    //
                    off--;

                    //
                    // Increment the X offset.
                    //
                    x0++;
                }

                //
                // See if the X offset has reached the right side of the
                // character glyph.
                //
                if(x0 == data[1])
                {
                    //
                    // Increment the Y offset.
                    //
                    y0++;

                    //
                    // Reset the X offset to the left side of the character
                    // glyph.
                    //
                    x0 = 0;
                }
            }

            //
            // Loop while there are any on pixels.
            //
            while(on)
            {
                //
                // See if the bottom of the clipping region has been exceeded.
                //
                if((y + y0) > sContext.clipRegion.yMax)
                {
                    //
                    // Ignore the remainder of the on pixels.
                    //
                    break;
                }

                //
                // See if there is more than one on pixel that will fit onto
                // the current row.
                //
                if((on > 1) && ((x0 + 1) < data[1]))
                {
                    //
                    // Determine the number of on pixels that will fit on this
                    // row.
                    //
                    count = (((x0 + on) > data[1]) ? data[1] - x0 :
                             on);

                    //
                    // If this row is within the clipping region, draw a
                    // horizontal line that corresponds to the sequence of on
                    // pixels.
                    //
                    if((y + y0) >= sContext.clipRegion.yMin)
                    {
                        sContext.foreground = context->foreground;
                        Graphics_drawLineH(&sContext, x + x0, x + x0 + count -
                                           1,
                                           ySave + y + y0);
                    }

                    //
                    // Decrement the count of on pixels by the number on this
                    // row.
                    //
                    on -= count;

                    //
                    // Increment the X offset by the number of on pixels.
                    //
                    x0 += count;
                }
                //
                // Otherwise, there is only a single on pixel that can be
                // drawn.
                //
                else
                {
                    //
                    // If this pixel is within the clipping region, then draw
                    // it.
                    //
                    if(((x + x0) >= sContext.clipRegion.xMin) &&
                       ((x + x0) <= sContext.clipRegion.xMax) &&
                       ((y + y0) >= sContext.clipRegion.yMin))
                    {
                        Graphics_drawPixelOnDisplay(context->display, x + x0,
                                                    ySave + y + y0,
                                                    context->foreground);
                    }

                    //
                    // Decrement the count of on pixels.
                    //
                    on--;

                    //
                    // Increment the X offset.
                    //
                    x0++;
                }

                //
                // See if the X offset has reached the right side of the
                // character glyph.
                //
                if(x0 == data[1])
                {
                    //
                    // Increment the Y offset.
                    //
                    y0++;

                    //
                    // Reset the X offset to the left side of the character
                    // glyph.
                    //
                    x0 = 0;
                }
            }
        }

        //
        // Increment the X coordinate by the width of the character.
        //
        x += data[1];
    }
}

//*****************************************************************************
//
//! Gets the baseline of a string.
//!
//! \param context is a pointer to the drawing context to query.
//!
//! This function determines the baseline position of a string.  The baseline
//! is the offset between the top of the string and the bottom of the capital
//! letters.  The only string data that exists below the baseline are the
//! descenders on some lower-case letters (such as ``y'').
//!
//! \return Returns the baseline of the string, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getStringBaseline(const Graphics_Context *context)
{
    return(context->font->baseline);
}

//*****************************************************************************
//
//! Draws a centered string.
//!
//! \param context is a pointer to the drawing context to use.
//! \param string is a pointer to the string to be drawn.
//! \param length is the number of characters from the string that should be
//! drawn on the screen.
//! \param x is the X coordinate of the center of the string position on the
//! screen.
//! \param y is the Y coordinate of the center of the string position on the
//! screen.
//! \param opaque is \b true if the background of each character should be
//! drawn and \b false if it should not (leaving the background as is).
//!
//! This function draws a string of test on the screen centered upon the
//! provided position.  The \e lLength parameter allows a portion of the
//! string to be examined without having to insert a NULL character at the
//! stopping point (which would not be possible if the string was located in
//! flash); specifying a length of -1 will cause the entire string to be
//! rendered (subject to clipping).
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawStringCentered(const Graphics_Context *context,
                                 int8_t *string,
                                 int32_t length,
                                 int32_t x,
                                 int32_t y,
                                 bool opaque)
{
    Graphics_drawString(context, string, length,
                        (x) -
                        (Graphics_getStringWidth(context, string, length) / 2),
                        (y) - (context->font->baseline / 2), opaque);
}

//*****************************************************************************
//
//! Gets the height of a string.
//!
//! \param context is a pointer to the drawing context to query.
//!
//! This function determines the height of a string.  The height is the offset
//! between the top of the string and the bottom of the string, including any
//! ascenders and descenders.  Note that this will not account for the case
//! where the string in question does not have any characters that use
//! descenders but the font in the drawing context does contain characters with
//! descenders.
//!
//! \return Returns the height of the string, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getStringHeight(const Graphics_Context *context)
{
    return(context->font->height);
}

//*****************************************************************************
//
//! Gets the maximum width of a character in a string.
//!
//! \param context is a pointer to the drawing context to query.
//!
//! This function determines the maximum width of a character in a string.  The
//! maximum width is the width of the widest individual character in the font
//! used to render the string, which may be wider than the widest character
//! that is used to render a particular string.
//!
//! \return Returns the maximum width of a character in a string, in pixels.
//
//*****************************************************************************
uint8_t Graphics_getStringMaxWidth(const Graphics_Context *context)
{
    return(context->font->maxWidth);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
