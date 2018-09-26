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

#ifndef __GRLIB_H__
#define __GRLIB_H__

#define NDEBUG
#include "assert.h"

//*****************************************************************************
//
//! \addtogroup primitives_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Make sure min and max are defined.
//
//*****************************************************************************
#ifndef min
#define min(a, b)               (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b)               (((a) < (b)) ? (b) : (a))
#endif

//*****************************************************************************
//
//! This structure defines the characteristics of a Bitmap Image
//
//*****************************************************************************
typedef struct Graphics_Image
{
    uint8_t bPP;                 //!< Bits per pixel and Compressed/Uncompressed
    uint16_t xSize;              //!< xSize
    uint16_t ySize;              //!< ySize
    uint16_t numColors;          //!< Number of Colors in Palette
    const uint32_t  * pPalette;  //!< Pointer to Palette
    const uint8_t * pPixel;      //!< Pointer to pixel data;
} Graphics_Image;

//*****************************************************************************
//
//! This structure defines the extents of a rectangle.  All points greater than
//! or equal to the minimum and less than or equal to the maximum are part of
//! the rectangle.
//
//*****************************************************************************
typedef struct Graphics_Rectangle
{
    int16_t xMin;              //!< The minimum X coordinate of the rectangle.
    int16_t yMin;            //!< The minimum Y coordinate of the rectangle.
    int16_t xMax;            //!< The maximum X coordinate of the rectangle.
    int16_t yMax;            //!< The maximum Y coordinate of the rectangle.
} Graphics_Rectangle;

typedef struct Graphics_Display Graphics_Display;

//*****************************************************************************
//
//! This structure defines the implementation of a display driver.
//
//*****************************************************************************
typedef struct Graphics_Display_Functions
{
    //
    //! A pointer to the function to draw a pixel on this display.
    //
    void (*const pfnPixelDraw) (const Graphics_Display * pDisplay, int16_t lX,
                                int16_t lY,
                                uint16_t ulValue);

    //
    //! A pointer to the function to draw multiple pixels on this display.
    //
    void (*const pfnPixelDrawMultiple) (const Graphics_Display * pDisplay, int16_t lX,
                                        int16_t lY,
                                        int16_t lX0, int16_t lCount,
                                        int16_t lBPP,
                                        const uint8_t * pucData,
                                        const uint32_t * pucPalette);

    //
    //! A pointer to the function to draw a horizontal line on this display.
    //
    void (*const pfnLineDrawH) (const Graphics_Display * pDisplay, int16_t lX1,
                                int16_t lX2, int16_t lY,
                                uint16_t ulValue);

    //
    //! A pointer to the function to draw a vertical line on this display.
    //
    void (*const pfnLineDrawV) (const Graphics_Display * pDisplay, int16_t lX,
                                int16_t lY1, int16_t lY2,
                                uint16_t ulValue);

    //
    //! A pointer to the function to draw a filled rectangle on this display.
    //
    void (*const pfnRectFill) (const Graphics_Display * pDisplay,
                               const Graphics_Rectangle * pRect,
                               uint16_t ulValue);

    //
    //! A pointer to the function to translate 24-bit RGB colors to
    //! display-specific colors.
    //
    uint32_t (*const pfnColorTranslate) (const Graphics_Display * pDisplay,
                                         uint32_t ulValue);

    //
    //! A pointer to the function to flush any cached drawing operations on
    //! this display.
    //
    void (*const pfnFlush) (const Graphics_Display * pDisplay);

    //
    //! A pointer to the function to clears Display. Contents of display buffer
    //! unmodified
    //
    void (*const pfnClearDisplay) (const Graphics_Display * pDisplay,
                                   uint16_t ulValue);
}
Graphics_Display_Functions;

//*****************************************************************************
//
//! This structure defines the characteristics of a display driver.
//
//*****************************************************************************
struct Graphics_Display
{
    int32_t size;                 //!< The size of this structure.
    void *displayData;            //!< A pointer to display driver-specific data.
    uint16_t width;                //!< The width of this display.
    uint16_t heigth;            //!< The height of this display.
    const Graphics_Display_Functions *pFxns;  //!< Implementation of this display
};

//*****************************************************************************
//
//! This structure describes a font used for drawing text onto the screen.
//
//*****************************************************************************
typedef struct Graphics_Font
{
    uint8_t format;        //!< The format of the font.  Can be one of FONT_FMT_UNCOMPRESSED or FONT_FMT_PIXEL_RLE.
    uint8_t maxWidth;    //!< The maximum width of a character; this is the width of the widest character in the font, though any individual character may be narrower than this width.
    uint8_t height;        //!< The height of the character cell; this may be taller than the font data for the characters (to provide inter-line spacing).
    uint8_t baseline;    //!< The offset between the top of the character cell and the baseline of  the glyph.  The baseline is the bottom row of a capital letter, below which only the descenders of the lower case letters occur.
    uint16_t offset[96]; //!< The offset within data to the data for each character in the font.
    const uint8_t *data; //!< A pointer to the data for the font.
} Graphics_Font;

//*****************************************************************************
//
//! This is a newer version of the structure which describes a font used
//! for drawing text onto the screen.  This variant allows a font to contain an
//! arbitrary, contiguous block of codepoints from the 256 basic characters in
//! an ISO8859-n font and allows support for accented characters in Western
//! European languages and any left-to-right typeface supported by an ISO8859
//! variant. Fonts encoded in this format may be used interchangeably with the
//! original fonts merely by casting the structure pointer when calling any
//! function or macro which expects a font pointer as a parameter.
//
//*****************************************************************************
typedef struct Graphics_FontEx
{
    uint8_t format;            //!< The format of the font.  Can be one of FONT_FMT_EX_UNCOMPRESSED or FONT_FMT_EX_PIXEL_RLE.
    uint8_t maxWidth;        //!< The maximum width of a character; this is the width of the widest character in the font, though any individual character may be narrower than this width.
    uint8_t height;            //!< The height of the character cell; this may be taller than the font data for the characters (to provide inter-line spacing).
    uint8_t baseline;        //!< The offset between the top of the character cell and the baseline of the glyph.  The baseline is the bottom row of a capital letter, below which only the descenders of the lower case letters occur.
    uint8_t first;        //!< The codepoint number representing the first character encoded in the font.
    uint8_t last;            //!< The codepoint number representing the last character encoded in the font.
    const uint16_t *offset;    //!< A pointer to a table containing the offset within data to the data for each character in the font.
    const uint8_t *data;    //!< A pointer to the data for the font.
} Graphics_FontEx;

//*****************************************************************************
//
//! This structure defines a drawing context to be used to draw onto the
//! screen.  Multiple drawing contexts may exist at any time.
//
//*****************************************************************************
typedef struct Graphics_Context
{
    int32_t size;                         //!< The size of this structure.
    const Graphics_Display *display;    //!< The screen onto which drawing operations are performed.
    Graphics_Rectangle clipRegion;        //!< The clipping region to be used when drawing onto the screen.
    uint32_t foreground;                 //!< The color used to draw primitives onto the screen.
    uint32_t background;                 //!< The background color used to draw primitives onto the screen.
    const Graphics_Font *font;            //!< The font used to render text onto the screen.
} Graphics_Context;

//*****************************************************************************
//
// Deprecated struct names.  These definitions ensure backwards compatibility
// but new code should avoid using deprecated struct names since these will
// be removed at some point in the future.
//
//*****************************************************************************
#define ClrAliceBlue                 GRAPHICS_COLOR_ALICE_BLUE
#define ClrAntiqueWhite              GRAPHICS_COLOR_ANTIQUE_WHITE
#define ClrAqua                      GRAPHICS_COLOR_AQUA
#define ClrAquamarine                GRAPHICS_COLOR_AQUAMARINE
#define ClrAzure                     GRAPHICS_COLOR_AZURE
#define ClrBeige                     GRAPHICS_COLOR_BEIGE
#define ClrBisque                    GRAPHICS_COLOR_BISQUE
#define ClrBlack                     GRAPHICS_COLOR_BLACK
#define ClrBlanchedAlmond            GRAPHICS_COLOR_BLANCHED_ALMOND
#define ClrBlue                      GRAPHICS_COLOR_BLUE
#define ClrBlueViolet                GRAPHICS_COLOR_BLUE_VIOLET
#define ClrBrown                     GRAPHICS_COLOR_BROWN
#define ClrBurlyWood                 GRAPHICS_COLOR_BURLY_WOOD
#define ClrCadetBlue                 GRAPHICS_COLOR_CADET_BLUE
#define ClrChartreuse                GRAPHICS_COLOR_CHARTREUSE
#define ClrChocolate                 GRAPHICS_COLOR_CHOCOLATE
#define ClrCoral                     GRAPHICS_COLOR_CORAL
#define ClrCornflowerBlue            GRAPHICS_COLOR_CORNFLOWER_BLUE
#define ClrCornsilk                  GRAPHICS_COLOR_CORNSILK
#define ClrCrimson                   GRAPHICS_COLOR_CRIMSON
#define ClrCyan                      GRAPHICS_COLOR_CYAN
#define ClrDarkBlue                  GRAPHICS_COLOR_DARK_BLUE
#define ClrDarkCyan                  GRAPHICS_COLOR_DARK_CYAN
#define ClrDarkGoldenrod             GRAPHICS_COLOR_DARK_GOLDENROD
#define ClrDarkGray                  GRAPHICS_COLOR_DARK_GRAY
#define ClrDarkGreen                 GRAPHICS_COLOR_DARK_GREEN
#define ClrDarkKhaki                 GRAPHICS_COLOR_DARK_KHAKI
#define ClrDarkMagenta               GRAPHICS_COLOR_DARK_MAGENTA
#define ClrDarkOliveGreen            GRAPHICS_COLOR_DARK_OLIVE_GREEN
#define ClrDarkOrange                GRAPHICS_COLOR_DARK_ORANGE
#define ClrDarkOrchid                GRAPHICS_COLOR_DARK_ORCHID
#define ClrDarkRed                   GRAPHICS_COLOR_DARK_RED
#define ClrDarkSalmon                GRAPHICS_COLOR_DARK_SALMON
#define ClrDarkSeaGreen              GRAPHICS_COLOR_DARK_SEA_GREEN
#define ClrDarkSlateBlue             GRAPHICS_COLOR_DARK_SLATE_BLUE
#define ClrDarkSlateGray             GRAPHICS_COLOR_DARK_SLATE_GRAY
#define ClrDarkTurquoise             GRAPHICS_COLOR_DARK_TURQUOISE
#define ClrDarkViolet                GRAPHICS_COLOR_DARK_VIOLET
#define ClrDeepPink                  GRAPHICS_COLOR_DEEP_PINK
#define ClrDeepSkyBlue               GRAPHICS_COLOR_DEEP_SKY_BLUE
#define ClrDimGray                   GRAPHICS_COLOR_DIM_GRAY
#define ClrDodgerBlue                GRAPHICS_COLOR_DODGER_BLUE
#define ClrFireBrick                 GRAPHICS_COLOR_FIRE_BRICK
#define ClrFloralWhite               GRAPHICS_COLOR_FLORAL_WHITE
#define ClrForestGreen               GRAPHICS_COLOR_FOREST_GREEN
#define ClrFuchsia                   GRAPHICS_COLOR_FUCHSIA
#define ClrGainsboro                 GRAPHICS_COLOR_GAINSBORO
#define ClrGhostWhite                GRAPHICS_COLOR_GHOST_WHITE
#define ClrGold                      GRAPHICS_COLOR_GOLD
#define ClrGoldenrod                 GRAPHICS_COLOR_GOLDENRON
#define ClrGray                      GRAPHICS_COLOR_GRAY
#define ClrGreen                     GRAPHICS_COLOR_GREEN
#define ClrGreenYellow               GRAPHICS_COLOR_GREEN_YELLOW
#define ClrHoneydew                  GRAPHICS_COLOR_HONEYDEW
#define ClrHotPink                   GRAPHICS_COLOR_HOT_PINK
#define ClrIndianRed                 GRAPHICS_COLOR_INDIAN_RED
#define ClrIndigo                    GRAPHICS_COLOR_INDIGO
#define ClrIvory                     GRAPHICS_COLOR_IVORY
#define ClrKhaki                     GRAPHICS_COLOR_KHAKI
#define ClrLavender                  GRAPHICS_COLOR_LAVENDER
#define ClrLavenderBlush             GRAPHICS_COLOR_LAVENDER_BLUSH
#define ClrLawnGreen                 GRAPHICS_COLOR_LAWN_GREEN
#define ClrLemonChiffon              GRAPHICS_COLOR_LEMON_CHIFFON
#define ClrLightBlue                 GRAPHICS_COLOR_LIGHT_BLUE
#define ClrLightCoral                GRAPHICS_COLOR_LIGHT_CORAL
#define ClrLightCyan                 GRAPHICS_COLOR_LIGHT_CYAN
#define ClrLightGoldenrodYellow      GRAPHICS_COLOR_LIGHT_GOLDENRON_YELLOW
#define ClrLightGreen                GRAPHICS_COLOR_LIGHT_GREEN
#define ClrLightGrey                 GRAPHICS_COLOR_LIGHT_GRAY
#define ClrLightPink                 GRAPHICS_COLOR_LIGHT_PINK
#define ClrLightSalmon               GRAPHICS_COLOR_LIGHT_SALMON
#define ClrLightSeaGreen             GRAPHICS_COLOR_LIGHT_SEA_GREEN
#define ClrLightSkyBlue              GRAPHICS_COLOR_LIGHT_SKY_BLUE
#define ClrLightSlateGray            GRAPHICS_COLOR_LIGHT_SLATE_GRAY
#define ClrLightSteelBlue            GRAPHICS_COLOR_LIGHT_STEEL_BLUE
#define ClrLightYellow               GRAPHICS_COLOR_LIGHT_YELLOW
#define ClrLime                      GRAPHICS_COLOR_LIME
#define ClrLimeGreen                 GRAPHICS_COLOR_LIME_GREEN
#define ClrLinen                     GRAPHICS_COLOR_LINEN
#define ClrMagenta                   GRAPHICS_COLOR_MAGENTA
#define ClrMaroon                    GRAPHICS_COLOR_MAROON
#define ClrMediumAquamarine          GRAPHICS_COLOR_MEDIUM_AQUAMARINE
#define ClrMediumBlue                GRAPHICS_COLOR_MEDIUM_BLUE
#define ClrMediumOrchid              GRAPHICS_COLOR_MEDIUM_ORCHID
#define ClrMediumPurple              GRAPHICS_COLOR_MEDIUM_PURPLE
#define ClrMediumSeaGreen            GRAPHICS_COLOR_MEDIUM_SEA_GREEN
#define ClrMediumSlateBlue           GRAPHICS_COLOR_MEDIUM_SLATE_BLUE
#define ClrMediumSpringGreen         GRAPHICS_COLOR_MEDIUM_SPRING_GREEN
#define ClrMediumTurquoise           GRAPHICS_COLOR_MEDIUM_TURQUOISE
#define ClrMediumVioletRed           GRAPHICS_COLOR_MEDIUM_VIOLET_RED
#define ClrMidnightBlue              GRAPHICS_COLOR_MIGNIGHT_BLUE
#define ClrMintCream                 GRAPHICS_COLOR_MINT_CREAM
#define ClrMistyRose                 GRAPHICS_COLOR_MISTY_ROSE
#define ClrMoccasin                  GRAPHICS_COLOR_MOCCASIN
#define ClrNavajoWhite               GRAPHICS_COLOR_NAVAJO_WHITE
#define ClrNavy                      GRAPHICS_COLOR_NAVY
#define ClrOldLace                   GRAPHICS_COLOR_OLD_LACE
#define ClrOlive                     GRAPHICS_COLOR_OLIVE
#define ClrOliveDrab                 GRAPHICS_COLOR_OLIVE_DRAB
#define ClrOrange                    GRAPHICS_COLOR_ORANGE
#define ClrOrangeRed                 GRAPHICS_COLOR_ORANGE_RED
#define ClrOrchid                    GRAPHICS_COLOR_ORCHID
#define ClrPaleGoldenrod             GRAPHICS_COLOR_PALE_GOLDENRON
#define ClrPaleGreen                 GRAPHICS_COLOR_PALE_GREEN
#define ClrPaleTurquoise             GRAPHICS_COLOR_PALE_TURQUOISE
#define ClrPaleVioletRed             GRAPHICS_COLOR_PALE_VIOLET_RED
#define ClrPapayaWhip                GRAPHICS_COLOR_PAPAYA_WHIP
#define ClrPeachPuff                 GRAPHICS_COLOR_PEACH_PUFF
#define ClrPeru                      GRAPHICS_COLOR_PERU
#define ClrPink                      GRAPHICS_COLOR_PINK
#define ClrPlum                      GRAPHICS_COLOR_PLUM
#define ClrPowderBlue                GRAPHICS_COLOR_POWDER_BLUE
#define ClrPurple                    GRAPHICS_COLOR_PURPLE
#define ClrRed                       GRAPHICS_COLOR_RED
#define ClrRosyBrown                 GRAPHICS_COLOR_ROSY_BROWN
#define ClrRoyalBlue                 GRAPHICS_COLOR_ROYAL_BLUE
#define ClrSaddleBrown               GRAPHICS_COLOR_SADDLE_BROWN
#define ClrSalmon                    GRAPHICS_COLOR_SALMON
#define ClrSandyBrown                GRAPHICS_COLOR_SANDY_BROWN
#define ClrSeaGreen                  GRAPHICS_COLOR_SEA_GREEN
#define ClrSeashell                  GRAPHICS_COLOR_SEASHELL
#define ClrSienna                    GRAPHICS_COLOR_SIENNA
#define ClrSilver                    GRAPHICS_COLOR_SILVER
#define ClrSkyBlue                   GRAPHICS_COLOR_SKY_BLUE
#define ClrSlateBlue                 GRAPHICS_COLOR_SLATE_BLUE
#define ClrSlateGray                 GRAPHICS_COLOR_SLATE_GRAY
#define ClrSnow                      GRAPHICS_COLOR_SNOW
#define ClrSpringGreen               GRAPHICS_COLOR_SPRING_GREEN
#define ClrSteelBlue                 GRAPHICS_COLOR_STEEL_BLUE
#define ClrTan                       GRAPHICS_COLOR_TAN
#define ClrTeal                      GRAPHICS_COLOR_TEAL
#define ClrThistle                   GRAPHICS_COLOR_THISTLE
#define ClrTomato                    GRAPHICS_COLOR_TOMATO
#define ClrTurquoise                 GRAPHICS_COLOR_TURQUOISE
#define ClrViolet                    GRAPHICS_COLOR_VIOLET
#define ClrWheat                     GRAPHICS_COLOR_WHEAT
#define ClrWhite                     GRAPHICS_COLOR_WHITE
#define ClrWhiteSmoke                GRAPHICS_COLOR_WHITE_SMOKE
#define ClrYellow                    GRAPHICS_COLOR_YELLOW
#define ClrYellowGreen               GRAPHICS_COLOR_YELLOW_GREEN
#define ClrBlack                     GRAPHICS_COLOR_BLACK
#define FONT_FMT_UNCOMPRESSED            GRAPHICS_FONT_FMT_UNCOMPRESSED
#define FONT_FMT_PIXEL_RLE                GRAPHICS_FONT_FMT_PIXEL_RLE
#define FONT_EX_MARKER                    GRAPHICS_FONT_EX_MARKER
#define FONT_FMT_EX_UNCOMPRESSED        GRAPHICS_FONT_FMT_EX_UNCOMPRESSED
#define FONT_FMT_EX_PIXEL_RLE            GRAPHICS_FONT_FMT_EX_PIXEL_RLE
#define AUTO_STRING_LENGTH                GRAPHICS_AUTO_STRING_LENGTH
#define OPAQUE_TEXT                        GRAPHICS_OPAQUE_TEXT
#define TRANSPARENT_TEXT                GRAPHICS_TRANSPARENT_TEXT
#define IMAGE_FMT_1BPP_UNCOMP            GRAPHICS_IMAGE_FMT_1BPP_UNCOMP
#define IMAGE_FMT_2BPP_UNCOMP            GRAPHICS_IMAGE_FMT_2BPP_UNCOMP
#define IMAGE_FMT_4BPP_UNCOMP            GRAPHICS_IMAGE_FMT_4BPP_UNCOMP
#define IMAGE_FMT_8BPP_UNCOMP            GRAPHICS_IMAGE_FMT_8BPP_UNCOMP
#define IMAGE_FMT_1BPP_COMP_RLE4        GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE4
#define IMAGE_FMT_2BPP_UNCOMP            GRAPHICS_IMAGE_FMT_2BPP_UNCOMP
#define IMAGE_FMT_4BPP_COMP_RLE4        GRAPHICS_IMAGE_FMT_4BPP_COMP_RLE4
#define IMAGE_FMT_1BPP_COMP_RLE7         GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE7
#define IMAGE_FMT_1BPP_COMP_RLE8        GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE8
#define IMAGE_FMT_2BPP_COMP_RLE8         GRAPHICS_IMAGE_FMT_2BPP_COMP_RLE8
#define IMAGE_FMT_4BPP_COMP_RLE8         GRAPHICS_IMAGE_FMT_4BPP_COMP_RLE8
#define IMAGE_FMT_8BPP_COMP_RLE8        GRAPHICS_IMAGE_FMT_8BPP_COMP_RLE8
#define IMAGE_FMT_8BPP_COMP_RLEBLEND    GRAPHICS_IMAGE_FMT_8BPP_COMP_RLEBLEND
#define tFontEx                                 Graphics_FontEx
#define tFont                                     Graphics_Font
#define tDisplay                                 Graphics_Display
#define tRectangle                                 Graphics_Rectangle
#define tImage                                     Graphics_Image
#define tContext                                  Graphics_Context
#define sXMax                                    xMax
#define sXMin                                    xMin
#define sYMax                                    yMax
#define sYMin                                    yMin

//*****************************************************************************
//
// Deprecated function names.  These definitions ensure backwards compatibility
// but new code should avoid using deprecated function names since these will
// be removed at some point in the future.
//
//*****************************************************************************
#define GrCircleDraw                        Graphics_drawCircle
#define GrCircleFill                        Graphics_fillCircle
#define GrContextClipRegionSet                Graphics_setClipRegion
#define GrContextInit                        Graphics_initContext
#define GrImageDraw                            Graphics_drawImage
#define GrLineDraw                            Graphics_drawLine
#define GrLineDrawH                            Graphics_drawLineH
#define GrLineDrawV                            Graphics_drawLineV
#define GrRectDraw                            Graphics_drawRectangle
#define GrRectFill                            Graphics_fillRectangle
#define GrStringDraw                        Graphics_drawString
#define GrStringWidthGet                    Graphics_getStringWidth
#define GrRectOverlapCheck                    Graphics_isOverlappingRectangle
#define GrRectIntersectGet                    Graphics_getRectangleIntersection
#define GrContextBackgroundSet                   Graphics_setBackgroundColor
#define GrContextBackgroundSetTranslated \
    Graphics_setBackgroundColorTranslated
#define GrContextDpyWidthGet                   Graphics_getDisplayWidth
#define GrContextDpyHeightGet                Graphics_getDisplayHeight
#define GrContextFontSet                    Graphics_setFont
#define GrContextForegroundSet                Graphics_setForegroundColor
#define GrContextForegroundSetTranslated \
    Graphics_setForegroundColorTranslated
#define GrFlush                                Graphics_flushBuffer
#define GrClearDisplay                        Graphics_clearDisplay
#define GrFontBaselineGet                    Graphics_getFontBaseline
#define GrFontHeightGet                        Graphics_getFontHeight
#define GrFontMaxWidthGet                    Graphics_getFontMaxWidth
#define GrImageColorsGet                    Graphics_getImageColors
#define GrImageHeightGet                    Graphics_getImageHeight
#define GrImageWidthGet                        Graphics_getImageWidth
#define GrOffScreen1BPPSize \
    Graphics_getOffscreen1BppImageSize
#define GrOffScreen4BPPSize \
    Graphics_getOffscreen4BppImageSize
#define GrOffScreen8BPPSize \
    Graphics_getOffscreen8BppImageSize
#define GrPixelDraw                            Graphics_drawPixel
#define GrStringBaselineGet                    Graphics_getStringBaseline
#define GrStringDrawCentered                Graphics_drawStringCentered
#define GrStringHeightGet                    Graphics_getStringHeight
#define GrStringMaxWidthGet                    Graphics_getStringMaxWidth
#define DpyColorTranslate                    Graphics_translateColorOnDisplay
#define DpyFlush                            Graphics_flushOnDisplay
#define DpyClearDisplay                        Graphics_clearDisplayOnDisplay
#define DpyHeightGet                        Graphics_getHeightOfDisplay
#define DpyLineDrawH \
    Graphics_drawHorizontalLineOnDisplay
#define DpyLineDrawV                        Graphics_drawVerticalLineOnDisplay
#define DpyPixelDraw                        Graphics_drawPixelOnDisplay
#define DpyPixelDrawMultiple \
    Graphics_drawMultiplePixelsOnDisplay
#define DpyRectFill                            Graphics_fillRectangleOnDisplay
#define DpyWidthGet                            Graphics_getWidthOfDisplay
#define GrRectContainsPoint                    Graphics_isPointWithinRectangle

//*****************************************************************************
//
//! Indicates that the font data is stored in an uncompressed format.
//
//*****************************************************************************
#define GRAPHICS_FONT_FMT_UNCOMPRESSED   0x00

//*****************************************************************************
//
//! Indicates that the font data is stored using a pixel-based RLE format.
//
//*****************************************************************************
#define GRAPHICS_FONT_FMT_PIXEL_RLE      0x01

//*****************************************************************************
//
//! A marker used in the format field of a font to indicates that the font
//! data is stored using the new tFontEx structure.
//
//*****************************************************************************
#define GRAPHICS_FONT_EX_MARKER          0x80

//*****************************************************************************
//
//! Indicates that the font data is stored in an uncompressed format and uses
//! the tFontEx structure format.
//
//*****************************************************************************
#define GRAPHICS_FONT_FMT_EX_UNCOMPRESSED   (FONT_FMT_UNCOMPRESSED | \
                                             FONT_EX_MARKER)

//*****************************************************************************
//
//! Indicates that the font data is stored using a pixel-based RLE format and
//! uses the tFontEx structure format.
//
//*****************************************************************************
#define GRAPHICS_FONT_FMT_EX_PIXEL_RLE      (FONT_FMT_PIXEL_RLE | \
                                             FONT_EX_MARKER)

//*****************************************************************************
//
//! Value to automatically draw the entire length of the string
//! (subject to clipping)
//
//*****************************************************************************
#define GRAPHICS_AUTO_STRING_LENGTH     -1

//*****************************************************************************
//
//! Value to draw text opaque
//! The text foreground and background are drawn together
//
//*****************************************************************************
#define GRAPHICS_OPAQUE_TEXT     1

//*****************************************************************************
//
//! Value to draw text transparently
//! The text only (no background) is drawn
//
//*****************************************************************************
#define GRAPHICS_TRANSPARENT_TEXT     0

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with a single bit.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_1BPP_UNCOMP   0x01

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with two bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_2BPP_UNCOMP   0x02

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with four bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_4BPP_UNCOMP   0x04

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with eight bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_8BPP_UNCOMP   0x08

//*****************************************************************************
//
//! Indicates that the image data is compressed with 4 bit Run Length Encoding
//! and represents each pixel with a single bit.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE4     0x41

//*****************************************************************************
//
//! Indicates that the image data is compressed with 4 bit Run Length Encoding
//! and represents each pixel with two bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_2BPP_COMP_RLE4     0x42

//*****************************************************************************
//
//! Indicates that the image data is compressed with 4 bit Run Length Encoding
//! and represents each pixel with four bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_4BPP_COMP_RLE4     0x44

//*****************************************************************************
//
//! Indicates that the image data is compressed with 7 bit Run Length Encoding
//! and represents each pixel with a single bit.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE7     0x71

//*****************************************************************************
//
//! Indicates that the image data is compressed with 8 bit Run Length Encoding
//! and represents each pixel with a single bit.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_1BPP_COMP_RLE8     0x81

//*****************************************************************************
//
//! Indicates that the image data is compressed with 8 bit Run Length Encoding
//! and represents each pixel with two bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_2BPP_COMP_RLE8     0x82

//*****************************************************************************
//
//! Indicates that the image data is compressed with 8 bit Run Length Encoding
//! and represents each pixel with four bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_4BPP_COMP_RLE8     0x84

//*****************************************************************************
//
//! Indicates that the image data is compressed with 8 bit Run Length Encoding
//! and represents each pixel with eight bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_8BPP_COMP_RLE8     0x88

//*****************************************************************************
//
//! Indicates that the image data is compressed and represents each pixel with
//! info bits.
//
//*****************************************************************************
#define GRAPHICS_IMAGE_FMT_8BPP_COMP_RLEBLEND     0x28

//*****************************************************************************
//
// A set of color definitions.  This set is the subset of the X11 colors (from
// rgb.txt) that are supported by typical web browsers.
//
//*****************************************************************************
#define GRAPHICS_COLOR_ALICE_BLUE                  0x00F0F8FF
#define GRAPHICS_COLOR_ANTIQUE_WHITE             0x00FAEBD7
#define GRAPHICS_COLOR_AQUA                         0x0000FFFF
#define GRAPHICS_COLOR_AQUAMARINE                 0x007FFFD4
#define GRAPHICS_COLOR_AZURE                     0x00F0FFFF
#define GRAPHICS_COLOR_BEIGE                     0x00F5F5DC
#define GRAPHICS_COLOR_BISQUE                     0x00FFE4C4
#define GRAPHICS_COLOR_BLACK                     0x00000000
#define GRAPHICS_COLOR_BLANCHED_ALMOND             0x00FFEBCD
#define GRAPHICS_COLOR_BLUE                         0x000000FF
#define GRAPHICS_COLOR_BLUE_VIOLET                 0x008A2BE2
#define GRAPHICS_COLOR_BROWN                     0x00A52A2A
#define GRAPHICS_COLOR_BURLY_WOOD                 0x00DEB887
#define GRAPHICS_COLOR_CADET_BLUE                 0x005F9EA0
#define GRAPHICS_COLOR_CHARTREUSE                 0x007FFF00
#define GRAPHICS_COLOR_CHOCOLATE                 0x00D2691E
#define GRAPHICS_COLOR_CORAL                     0x00FF7F50
#define GRAPHICS_COLOR_CORNFLOWER_BLUE             0x006495ED
#define GRAPHICS_COLOR_CORNSILK                     0x00FFF8DC
#define GRAPHICS_COLOR_CRIMSON                     0x00DC143C
#define GRAPHICS_COLOR_CYAN                         0x0000FFFF
#define GRAPHICS_COLOR_DARK_BLUE                 0x0000008B
#define GRAPHICS_COLOR_DARK_CYAN                 0x00008B8B
#define GRAPHICS_COLOR_DARK_GOLDENROD             0x00B8860B
#define GRAPHICS_COLOR_DARK_GRAY                 0x00A9A9A9
#define GRAPHICS_COLOR_DARK_GREEN                 0x00006400
#define GRAPHICS_COLOR_DARK_KHAKI                 0x00BDB76B
#define GRAPHICS_COLOR_DARK_MAGENTA                 0x008B008B
#define GRAPHICS_COLOR_DARK_OLIVE_GREEN             0x00556B2F
#define GRAPHICS_COLOR_DARK_ORANGE                 0x00FF8C00
#define GRAPHICS_COLOR_DARK_ORCHID                 0x009932CC
#define GRAPHICS_COLOR_DARK_RED                     0x008B0000
#define GRAPHICS_COLOR_DARK_SALMON                 0x00E9967A
#define GRAPHICS_COLOR_DARK_SEA_GREEN             0x008FBC8F
#define GRAPHICS_COLOR_DARK_SLATE_BLUE             0x00483D8B
#define GRAPHICS_COLOR_DARK_SLATE_GRAY             0x002F4F4F
#define GRAPHICS_COLOR_DARK_TURQUOISE             0x0000CED1
#define GRAPHICS_COLOR_DARK_VIOLET                 0x009400D3
#define GRAPHICS_COLOR_DEEP_PINK                 0x00FF1493
#define GRAPHICS_COLOR_DEEP_SKY_BLUE             0x0000BFFF
#define GRAPHICS_COLOR_DIM_GRAY                     0x00696969
#define GRAPHICS_COLOR_DODGER_BLUE                 0x001E90FF
#define GRAPHICS_COLOR_FIRE_BRICK                 0x00B22222
#define GRAPHICS_COLOR_FLORAL_WHITE                 0x00FFFAF0
#define GRAPHICS_COLOR_FOREST_GREEN                 0x00228B22
#define GRAPHICS_COLOR_FUCHSIA                     0x00FF00FF
#define GRAPHICS_COLOR_GAINSBORO                 0x00DCDCDC
#define GRAPHICS_COLOR_GHOST_WHITE                 0x00F8F8FF
#define GRAPHICS_COLOR_GOLD                         0x00FFD700
#define GRAPHICS_COLOR_GOLDENRON                 0x00DAA520
#define GRAPHICS_COLOR_GRAY                         0x00808080
#define GRAPHICS_COLOR_GREEN                     0x00008000
#define GRAPHICS_COLOR_GREEN_YELLOW                 0x00ADFF2F
#define GRAPHICS_COLOR_HONEYDEW                     0x00F0FFF0
#define GRAPHICS_COLOR_HOT_PINK                     0x00FF69B4
#define GRAPHICS_COLOR_INDIAN_RED                 0x00CD5C5C
#define GRAPHICS_COLOR_INDIGO                     0x004B0082
#define GRAPHICS_COLOR_IVORY                     0x00FFFFF0
#define GRAPHICS_COLOR_KHAKI                     0x00F0E68C
#define GRAPHICS_COLOR_LAVENDER                     0x00E6E6FA
#define GRAPHICS_COLOR_LAVENDER_BLUSH             0x00FFF0F5
#define GRAPHICS_COLOR_LAWN_GREEN                 0x007CFC00
#define GRAPHICS_COLOR_LEMON_CHIFFON             0x00FFFACD
#define GRAPHICS_COLOR_LIGHT_BLUE                 0x00ADD8E6
#define GRAPHICS_COLOR_LIGHT_CORAL                 0x00F08080
#define GRAPHICS_COLOR_LIGHT_CYAN                 0x00E0FFFF
#define GRAPHICS_COLOR_LIGHT_GOLDENRON_YELLOW     0x00FAFAD2
#define GRAPHICS_COLOR_LIGHT_GREEN                 0x0090EE90
#define GRAPHICS_COLOR_LIGHT_GRAY                 0x00D3D3D3
#define GRAPHICS_COLOR_LIGHT_PINK                 0x00FFB6C1
#define GRAPHICS_COLOR_LIGHT_SALMON                 0x00FFA07A
#define GRAPHICS_COLOR_LIGHT_SEA_GREEN             0x0020B2AA
#define GRAPHICS_COLOR_LIGHT_SKY_BLUE             0x0087CEFA
#define GRAPHICS_COLOR_LIGHT_SLATE_GRAY             0x00778899
#define GRAPHICS_COLOR_LIGHT_STEEL_BLUE             0x00B0C4DE
#define GRAPHICS_COLOR_LIGHT_YELLOW                 0x00FFFFE0
#define GRAPHICS_COLOR_LIME                         0x0000FF00
#define GRAPHICS_COLOR_LIME_GREEN                 0x0032CD32
#define GRAPHICS_COLOR_LINEN                     0x00FAF0E6
#define GRAPHICS_COLOR_MAGENTA                     0x00FF00FF
#define GRAPHICS_COLOR_MAROON                     0x00800000
#define GRAPHICS_COLOR_MEDIUM_AQUAMARINE         0x0066CDAA
#define GRAPHICS_COLOR_MEDIUM_BLUE                 0x000000CD
#define GRAPHICS_COLOR_MEDIUM_ORCHID             0x00BA55D3
#define GRAPHICS_COLOR_MEDIUM_PURPLE             0x009370DB
#define GRAPHICS_COLOR_MEDIUM_SEA_GREEN             0x003CB371
#define GRAPHICS_COLOR_MEDIUM_SLATE_BLUE         0x007B68EE
#define GRAPHICS_COLOR_MEDIUM_SPRING_GREEN         0x0000FA9A
#define GRAPHICS_COLOR_MEDIUM_TURQUOISE             0x0048D1CC
#define GRAPHICS_COLOR_MEDIUM_VIOLET_RED         0x00C71585
#define GRAPHICS_COLOR_MIGNIGHT_BLUE             0x00191970
#define GRAPHICS_COLOR_MINT_CREAM                 0x00F5FFFA
#define GRAPHICS_COLOR_MISTY_ROSE                 0x00FFE4E1
#define GRAPHICS_COLOR_MOCCASIN                     0x00FFE4B5
#define GRAPHICS_COLOR_NAVAJO_WHITE                 0x00FFDEAD
#define GRAPHICS_COLOR_NAVY                         0x00000080
#define GRAPHICS_COLOR_OLD_LACE                     0x00FDF5E6
#define GRAPHICS_COLOR_OLIVE                     0x00808000
#define GRAPHICS_COLOR_OLIVE_DRAB                 0x006B8E23
#define GRAPHICS_COLOR_ORANGE                     0x00FFA500
#define GRAPHICS_COLOR_ORANGE_RED                 0x00FF4500
#define GRAPHICS_COLOR_ORCHID                     0x00DA70D6
#define GRAPHICS_COLOR_PALE_GOLDENRON             0x00EEE8AA
#define GRAPHICS_COLOR_PALE_GREEN                 0x0098FB98
#define GRAPHICS_COLOR_PALE_TURQUOISE             0x00AFEEEE
#define GRAPHICS_COLOR_PALE_VIOLET_RED             0x00DB7093
#define GRAPHICS_COLOR_PAPAYA_WHIP                 0x00FFEFD5
#define GRAPHICS_COLOR_PEACH_PUFF                 0x00FFDAB9
#define GRAPHICS_COLOR_PERU                         0x00CD853F
#define GRAPHICS_COLOR_PINK                         0x00FFC0CB
#define GRAPHICS_COLOR_PLUM                         0x00DDA0DD
#define GRAPHICS_COLOR_POWDER_BLUE                 0x00B0E0E6
#define GRAPHICS_COLOR_PURPLE                     0x00800080
#define GRAPHICS_COLOR_RED                         0x00FF0000
#define GRAPHICS_COLOR_ROSY_BROWN                 0x00BC8F8F
#define GRAPHICS_COLOR_ROYAL_BLUE                 0x004169E1
#define GRAPHICS_COLOR_SADDLE_BROWN                 0x008B4513
#define GRAPHICS_COLOR_SALMON                     0x00FA8072
#define GRAPHICS_COLOR_SANDY_BROWN                 0x00F4A460
#define GRAPHICS_COLOR_SEA_GREEN                 0x002E8B57
#define GRAPHICS_COLOR_SEASHELL                     0x00FFF5EE
#define GRAPHICS_COLOR_SIENNA                     0x00A0522D
#define GRAPHICS_COLOR_SILVER                     0x00C0C0C0
#define GRAPHICS_COLOR_SKY_BLUE                     0x0087CEEB
#define GRAPHICS_COLOR_SLATE_BLUE                 0x006A5ACD
#define GRAPHICS_COLOR_SLATE_GRAY                 0x00708090
#define GRAPHICS_COLOR_SNOW                         0x00FFFAFA
#define GRAPHICS_COLOR_SPRING_GREEN                 0x0000FF7F
#define GRAPHICS_COLOR_STEEL_BLUE                 0x004682B4
#define GRAPHICS_COLOR_TAN                         0x00D2B48C
#define GRAPHICS_COLOR_TEAL                         0x00008080
#define GRAPHICS_COLOR_THISTLE                     0x00D8BFD8
#define GRAPHICS_COLOR_TOMATO                     0x00FF6347
#define GRAPHICS_COLOR_TURQUOISE                 0x0040E0D0
#define GRAPHICS_COLOR_VIOLET                     0x00EE82EE
#define GRAPHICS_COLOR_WHEAT                     0x00F5DEB3
#define GRAPHICS_COLOR_WHITE                     0x00FFFFFF
#define GRAPHICS_COLOR_WHITE_SMOKE                 0x00F5F5F5
#define GRAPHICS_COLOR_YELLOW                     0x00FFFF00
#define GRAPHICS_COLOR_YELLOW_GREEN                 0x009ACD32
#define GRAPHICS_COLOR_BLACK                     0x00000000

//*****************************************************************************
//
// Masks and shifts to aid in color format translation by drivers.
//
//*****************************************************************************
#define ClrRedMask              0x00FF0000
#define ClrRedShift             16
#define ClrGreenMask            0x0000FF00
#define ClrGreenShift           8
#define ClrBlueMask             0x000000FF
#define ClrBlueShift            0

//*****************************************************************************
//
// Prototypes for the predefined fonts in the graphics library.  ..Cm.. is the
// computer modern font, which is a serif font.  ..Cmsc.. is the computer
// modern small-caps font, which is also a serif font.  ..Cmss.. is the
// computer modern sans-serif font.
//
//*****************************************************************************
extern const Graphics_Font g_sFontCm12;
extern const Graphics_Font g_sFontCm12b;
extern const Graphics_Font g_sFontCm12i;
extern const Graphics_Font g_sFontCm14;
extern const Graphics_Font g_sFontCm14b;
extern const Graphics_Font g_sFontCm14i;
extern const Graphics_Font g_sFontCm16;
extern const Graphics_Font g_sFontCm16b;
extern const Graphics_Font g_sFontCm16i;
extern const Graphics_Font g_sFontCm18;
extern const Graphics_Font g_sFontCm18b;
extern const Graphics_Font g_sFontCm18i;
extern const Graphics_Font g_sFontCm20;
extern const Graphics_Font g_sFontCm20b;
extern const Graphics_Font g_sFontCm20i;
extern const Graphics_Font g_sFontCm22;
extern const Graphics_Font g_sFontCm22b;
extern const Graphics_Font g_sFontCm22i;
extern const Graphics_Font g_sFontCm24;
extern const Graphics_Font g_sFontCm24b;
extern const Graphics_Font g_sFontCm24i;
extern const Graphics_Font g_sFontCm26;
extern const Graphics_Font g_sFontCm26b;
extern const Graphics_Font g_sFontCm26i;
extern const Graphics_Font g_sFontCm28;
extern const Graphics_Font g_sFontCm28b;
extern const Graphics_Font g_sFontCm28i;
extern const Graphics_Font g_sFontCm30;
extern const Graphics_Font g_sFontCm30b;
extern const Graphics_Font g_sFontCm30i;
extern const Graphics_Font g_sFontCm32;
extern const Graphics_Font g_sFontCm32b;
extern const Graphics_Font g_sFontCm32i;
extern const Graphics_Font g_sFontCm34;
extern const Graphics_Font g_sFontCm34b;
extern const Graphics_Font g_sFontCm34i;
extern const Graphics_Font g_sFontCm36;
extern const Graphics_Font g_sFontCm36b;
extern const Graphics_Font g_sFontCm36i;
extern const Graphics_Font g_sFontCm38;
extern const Graphics_Font g_sFontCm38b;
extern const Graphics_Font g_sFontCm38i;
extern const Graphics_Font g_sFontCm40;
extern const Graphics_Font g_sFontCm40b;
extern const Graphics_Font g_sFontCm40i;
extern const Graphics_Font g_sFontCm42;
extern const Graphics_Font g_sFontCm42b;
extern const Graphics_Font g_sFontCm42i;
extern const Graphics_Font g_sFontCm44;
extern const Graphics_Font g_sFontCm44b;
extern const Graphics_Font g_sFontCm44i;
extern const Graphics_Font g_sFontCm46;
extern const Graphics_Font g_sFontCm46b;
extern const Graphics_Font g_sFontCm46i;
extern const Graphics_Font g_sFontCm48;
extern const Graphics_Font g_sFontCm48b;
extern const Graphics_Font g_sFontCm48i;
extern const Graphics_Font g_sFontCmsc12;
extern const Graphics_Font g_sFontCmsc14;
extern const Graphics_Font g_sFontCmsc16;
extern const Graphics_Font g_sFontCmsc18;
extern const Graphics_Font g_sFontCmsc20;
extern const Graphics_Font g_sFontCmsc22;
extern const Graphics_Font g_sFontCmsc24;
extern const Graphics_Font g_sFontCmsc26;
extern const Graphics_Font g_sFontCmsc28;
extern const Graphics_Font g_sFontCmsc30;
extern const Graphics_Font g_sFontCmsc32;
extern const Graphics_Font g_sFontCmsc34;
extern const Graphics_Font g_sFontCmsc36;
extern const Graphics_Font g_sFontCmsc38;
extern const Graphics_Font g_sFontCmsc40;
extern const Graphics_Font g_sFontCmsc42;
extern const Graphics_Font g_sFontCmsc44;
extern const Graphics_Font g_sFontCmsc46;
extern const Graphics_Font g_sFontCmsc48;
extern const Graphics_Font g_sFontCmss12;
extern const Graphics_Font g_sFontCmss12b;
extern const Graphics_Font g_sFontCmss12i;
extern const Graphics_Font g_sFontCmss14;
extern const Graphics_Font g_sFontCmss14b;
extern const Graphics_Font g_sFontCmss14i;
extern const Graphics_Font g_sFontCmss16;
extern const Graphics_Font g_sFontCmss16b;
extern const Graphics_Font g_sFontCmss16i;
extern const Graphics_Font g_sFontCmss18;
extern const Graphics_Font g_sFontCmss18b;
extern const Graphics_Font g_sFontCmss18i;
extern const Graphics_Font g_sFontCmss20;
extern const Graphics_Font g_sFontCmss20b;
extern const Graphics_Font g_sFontCmss20i;
extern const Graphics_Font g_sFontCmss22;
extern const Graphics_Font g_sFontCmss22b;
extern const Graphics_Font g_sFontCmss22i;
extern const Graphics_Font g_sFontCmss24;
extern const Graphics_Font g_sFontCmss24b;
extern const Graphics_Font g_sFontCmss24i;
extern const Graphics_Font g_sFontCmss26;
extern const Graphics_Font g_sFontCmss26b;
extern const Graphics_Font g_sFontCmss26i;
extern const Graphics_Font g_sFontCmss28;
extern const Graphics_Font g_sFontCmss28b;
extern const Graphics_Font g_sFontCmss28i;
extern const Graphics_Font g_sFontCmss30;
extern const Graphics_Font g_sFontCmss30b;
extern const Graphics_Font g_sFontCmss30i;
extern const Graphics_Font g_sFontCmss32;
extern const Graphics_Font g_sFontCmss32b;
extern const Graphics_Font g_sFontCmss32i;
extern const Graphics_Font g_sFontCmss34;
extern const Graphics_Font g_sFontCmss34b;
extern const Graphics_Font g_sFontCmss34i;
extern const Graphics_Font g_sFontCmss36;
extern const Graphics_Font g_sFontCmss36b;
extern const Graphics_Font g_sFontCmss36i;
extern const Graphics_Font g_sFontCmss38;
extern const Graphics_Font g_sFontCmss38b;
extern const Graphics_Font g_sFontCmss38i;
extern const Graphics_Font g_sFontCmss40;
extern const Graphics_Font g_sFontCmss40b;
extern const Graphics_Font g_sFontCmss40i;
extern const Graphics_Font g_sFontCmss42;
extern const Graphics_Font g_sFontCmss42b;
extern const Graphics_Font g_sFontCmss42i;
extern const Graphics_Font g_sFontCmss44;
extern const Graphics_Font g_sFontCmss44b;
extern const Graphics_Font g_sFontCmss44i;
extern const Graphics_Font g_sFontCmss46;
extern const Graphics_Font g_sFontCmss46b;
extern const Graphics_Font g_sFontCmss46i;
extern const Graphics_Font g_sFontCmss48;
extern const Graphics_Font g_sFontCmss48b;
extern const Graphics_Font g_sFontCmss48i;
extern const Graphics_Font g_sFontCmtt12;
extern const Graphics_Font g_sFontCmtt14;
extern const Graphics_Font g_sFontCmtt16;
extern const Graphics_Font g_sFontCmtt18;
extern const Graphics_Font g_sFontCmtt20;
extern const Graphics_Font g_sFontCmtt22;
extern const Graphics_Font g_sFontCmtt24;
extern const Graphics_Font g_sFontCmtt26;
extern const Graphics_Font g_sFontCmtt28;
extern const Graphics_Font g_sFontCmtt30;
extern const Graphics_Font g_sFontCmtt32;
extern const Graphics_Font g_sFontCmtt34;
extern const Graphics_Font g_sFontCmtt36;
extern const Graphics_Font g_sFontCmtt38;
extern const Graphics_Font g_sFontCmtt40;
extern const Graphics_Font g_sFontCmtt42;
extern const Graphics_Font g_sFontCmtt44;
extern const Graphics_Font g_sFontCmtt46;
extern const Graphics_Font g_sFontCmtt48;
extern const Graphics_Font g_sFontFixed6x8;

//*****************************************************************************
//
// Language identifiers supported by the string table processing functions.
//
//*****************************************************************************
#define GrLangZhPRC             0x0804      // Chinese (PRC)
#define GrLangZhTW              0x0404      // Chinese (Taiwan)
#define GrLangEnUS              0x0409      // English (United States)
#define GrLangEnUK              0x0809      // English (United Kingdom)
#define GrLangEnAUS             0x0C09      // English (Australia)
#define GrLangEnCA              0x1009      // English (Canada)
#define GrLangEnNZ              0x1409      // English (New Zealand)
#define GrLangFr                0x040C      // French (Standard)
#define GrLangDe                0x0407      // German (Standard)
#define GrLangHi                0x0439      // Hindi
#define GrLangIt                0x0410      // Italian (Standard)
#define GrLangJp                0x0411      // Japanese
#define GrLangKo                0x0412      // Korean
#define GrLangEsMX              0x080A      // Spanish (Mexico)
#define GrLangEsSP              0x0C0A      // Spanish (Spain)
#define GrLangSwKE              0x0441      // Swahili (Kenya)
#define GrLangUrIN              0x0820      // Urdu (India)
#define GrLangUrPK              0x0420      // Urdu (Pakistan)

//*****************************************************************************
//
// Prototypes for the graphics library functions.
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a circle.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the center of the circle.
//! \param y is the Y coordinate of the center of the circle.
//! \param radius is the radius of the circle.
//!
//! This function draws a circle, utilizing the Bresenham circle drawing
//! algorithm.  The extent of the circle is from \e x - \e radius to \e x +
//! \e radius and \e y - \e radius to \e y + \e radius, inclusive.
//!
//! \return None.
//
//*****************************************************************************
extern void Graphics_drawCircle(const Graphics_Context *context,
                                int32_t x,
                                int32_t y,
                                int32_t lRadius);
extern void Graphics_fillCircle(const Graphics_Context *context,
                                int32_t x,
                                int32_t y,
                                int32_t lRadius);
extern void Graphics_setClipRegion(Graphics_Context *context,
                                   Graphics_Rectangle *rect);
extern void Graphics_initContext(Graphics_Context *context,
                                 Graphics_Display *display,
                                 const Graphics_Display_Functions *pFxns);
extern void Graphics_drawImage(const Graphics_Context *context,
                               const Graphics_Image *pBitmap,
                               int16_t x,
                               int16_t y);
extern void Graphics_drawLine(const Graphics_Context *context,
                              int32_t x1,
                              int32_t y1,
                              int32_t x2,
                              int32_t y2);
extern void Graphics_drawLineH(const Graphics_Context *context,
                               int32_t x1,
                               int32_t x2,
                               int32_t y);
extern void Graphics_drawLineV(const Graphics_Context *context,
                               int32_t x,
                               int32_t y1,
                               int32_t y2);
extern void Graphics_drawRectangle(const Graphics_Context *context,
                                   const Graphics_Rectangle *rect);
extern void Graphics_fillRectangle(const Graphics_Context *context,
                                   const Graphics_Rectangle *rect);
extern void Graphics_drawString(const Graphics_Context *context,
                                int8_t *string,
                                int32_t lLength,
                                int32_t x,
                                int32_t y,
                                bool opaque);
extern int32_t Graphics_getStringWidth(const Graphics_Context *context,
                                       const int8_t *string,
                                       int32_t lLength);
extern int32_t Graphics_isOverlappingRectangle(Graphics_Rectangle *psRect1,
                                               Graphics_Rectangle *psRect2);
extern int32_t Graphics_getRectangleIntersection(Graphics_Rectangle *psRect1,
                                                 Graphics_Rectangle *psRect2,
                                                 Graphics_Rectangle *
                                                 psIntersect);
extern void Graphics_setBackgroundColor(Graphics_Context *context,
                                        int32_t value);
extern uint16_t Graphics_getDisplayWidth(Graphics_Context *context);
extern uint16_t Graphics_getDisplayHeight(Graphics_Context *context);
extern void Graphics_setFont(Graphics_Context *context,
                             const Graphics_Font *font);
extern uint8_t Graphics_getFontBaseline(const Graphics_Font *font);
extern void Graphics_setForegroundColor(Graphics_Context *context,
                                        int32_t value);
extern void Graphics_setForegroundColorTranslated(Graphics_Context *context,
                                                  int32_t value);
extern uint8_t Graphics_getFontHeight(const Graphics_Font *font);
extern uint8_t Graphics_getFontMaxWidth(const Graphics_Font *font);
extern uint16_t Graphics_getImageColors(const Graphics_Image *image);
extern uint16_t Graphics_getImageHeight(const Graphics_Image *image);
extern uint16_t Graphics_getImageWidth(const Graphics_Image *image);
extern uint32_t Graphics_getOffscreen1BppImageSize(uint16_t width,
                                                   uint16_t height);
extern uint32_t Graphics_getOffscreen4BppImageSize(uint16_t width,
                                                   uint16_t height);
extern uint32_t Graphics_getOffScreen8BPPSize(uint16_t width,
                                              uint16_t height);
extern void Graphics_drawStringCentered(const Graphics_Context *context,
                                        int8_t *string,
                                        int32_t length,
                                        int32_t x,
                                        int32_t y,
                                        bool opaque);
extern uint8_t Graphics_getStringHeight(const Graphics_Context *context);
extern uint8_t Graphics_getStringMaxWidth(const Graphics_Context *context);
extern uint8_t Graphics_getStringBaseline(const Graphics_Context *context);
extern uint32_t Graphics_translateColorOnDisplay(
    const Graphics_Display *display,
    uint32_t value);
extern void Graphics_drawHorizontalLineOnDisplay(
    const Graphics_Display *display,
    uint16_t x1,
    uint16_t x2,
    uint16_t y,
    uint32_t value);
extern void Graphics_drawVerticalLineOnDisplay(const Graphics_Display *display,
                                               uint16_t x,
                                               uint16_t y1,
                                               uint16_t y2,
                                               uint16_t value);
extern void Graphics_fillRectangleOnDisplay(const Graphics_Display *display,
                                            const Graphics_Rectangle *rect,
                                            uint16_t value);
extern void Graphics_flushOnDisplay(const Graphics_Display *display);
extern void Graphics_drawPixel(const Graphics_Context *context,
                               uint16_t x,
                               uint16_t y);
extern void Graphics_clearDisplay(const Graphics_Context *context);
extern uint16_t Graphics_getHeightOfDisplay(const Graphics_Display *display);
extern void Graphics_flushBuffer(const Graphics_Context *context);
extern uint16_t Graphics_getWidthOfDisplay(const Graphics_Display *display);
extern bool Graphics_isPointWithinRectangle(const Graphics_Rectangle *rect,
                                            uint16_t x,
                                            uint16_t y);
extern void Graphics_drawPixelOnDisplay(const Graphics_Display *display,
                                        uint16_t x,
                                        uint16_t y,
                                        uint16_t value);
extern void Graphics_clearDisplayOnDisplay(const Graphics_Display *display,
                                           uint16_t value);
extern void Graphics_drawMultiplePixelsOnDisplay(
    const Graphics_Display *display,
    uint16_t x,
    uint16_t y,
    uint16_t x0,
    uint16_t count,
    uint16_t bPP,
    const uint8_t *data,
    const uint32_t *pucPalette);
extern void Graphics_initOffscreen1BppImage(Graphics_Display *display,
                                            uint8_t *image,
                                            int32_t width,
                                            int32_t height);
extern void Graphics_initOffscreen4BppImage(Graphics_Display *display,
                                            uint8_t *image,
                                            int32_t width,
                                            int32_t height);
extern void Graphics_setOffscreen4BppPalette(Graphics_Display *display,
                                             uint32_t *ppalette,
                                             uint32_t offset,
                                             uint32_t count);
extern void Graphics_initOffscreen8BppImage(Graphics_Display *display,
                                            uint8_t *image,
                                            int32_t width,
                                            int32_t height);
extern void Graphics_setOffscreen8BppPalette(Graphics_Display *display,
                                             uint32_t *ppalette,
                                             uint32_t offset,
                                             uint32_t count);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif // __GRLIB_H__
