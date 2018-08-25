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
// Details of this font:
//     Style: fixed
//     Size: 8 point
//     Bold: no
//     Italic: no
//     Memory usage: 960 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The uncompressed data for the fixed-point 6x8 font.
//
//*****************************************************************************
static const uint8_t g_pucFontFixed6x8Data[760] =
{
    8,   6,   0,   0,   0,   0,   0,   0,   8,   6,  32, 130,
    8,   0,   2,   0,   8,   6,  81,  69,   0,   0,   0,   0,
    8,   6,  81,  79, 148, 249,  69,   0,   8,   6,  33, 234,
    28,  43, 194,   0,   8,   6, 195,  33,   8,  66,  97, 128,
    8,   6,  98,  74,  16, 170,  70, 128,   8,   6,  96, 132,
    0,   0,   0,   0,   8,   6,  16, 132,  16,  64, 129,   0,
    8,   6,  64, 129,   4,  16, 132,   0,   8,   6,   0, 138,
    156, 168, 128,   0,   8,   6,   0, 130,  62,  32, 128,   0,
    8,   6,   0,   0,   0,  96, 132,   0,   8,   6,   0,   0,
    62,   0,   0,   0,   8,   6,   0,   0,   0,   1, 134,   0,
    8,   6,   0,  33,   8,  66,   0,   0,   8,   6, 114,  41,
    170, 202,  39,   0,   8,   6,  33, 130,   8,  32, 135,   0,
    8,   6, 114,  32, 132,  33,  15, 128,   8,   6, 248,  66,
    4,  10,  39,   0,   8,   6,  16, 197,  36, 248,  65,   0,
    8,   6, 250,  15,   2,  10,  39,   0,   8,   6,  49,   8,
    60, 138,  39,   0,   8,   6, 248,  33,   8,  65,   4,   0,
    8,   6, 114,  40, 156, 138,  39,   0,   8,   6, 114,  40,
    158,   8,  70,   0,   8,   6,   1, 134,   0,  97, 128,   0,
    8,   6,   1, 134,   0,  96, 132,   0,   8,   6,  16, 132,
    32,  64, 129,   0,   8,   6,   0,  15, 128, 248,   0,   0,
    8,   6,  64, 129,   2,  16, 132,   0,   8,   6, 114,  32,
    132,  32,   2,   0,   8,   6, 114,  32, 154, 170, 167,   0,
    8,   6, 114,  40, 162, 250,  40, 128,   8,   6, 242,  40,
    188, 138,  47,   0,   8,   6, 114,  40,  32, 130,  39,   0,
    8,   6, 226,  72, 162, 138,  78,   0,   8,   6, 250,   8,
    60, 130,  15, 128,   8,   6, 250,   8,  60, 130,   8,   0,
    8,   6, 114,  40,  46, 138,  39, 128,   8,   6, 138,  40,
    190, 138,  40, 128,   8,   6, 112, 130,   8,  32, 135,   0,
    8,   6,  56,  65,   4,  18,  70,   0,   8,   6, 138,  74,
    48, 162,  72, 128,   8,   6, 130,   8,  32, 130,  15, 128,
    8,   6, 139, 106, 170, 138,  40, 128,   8,   6, 138,  44,
    170, 154,  40, 128,   8,   6, 114,  40, 162, 138,  39,   0,
    8,   6, 242,  40, 188, 130,   8,   0,   8,   6, 114,  40,
    162, 170,  70, 128,   8,   6, 242,  40, 188, 162,  72, 128,
    8,   6, 122,   8,  28,   8,  47,   0,   8,   6, 248, 130,
    8,  32, 130,   0,   8,   6, 138,  40, 162, 138,  39,   0,
    8,   6, 138,  40, 162, 137,  66,   0,   8,   6, 138,  40,
    170, 170, 165,   0,   8,   6, 138,  37,   8,  82,  40, 128,
    8,   6, 138,  40, 148,  32, 130,   0,   8,   6, 248,  33,
    8,  66,  15, 128,   8,   6, 113,   4,  16,  65,   7,   0,
    8,   6,   2,   4,   8,  16,  32,   0,   8,   6, 112,  65,
    4,  16,  71,   0,   8,   6,  33,  72, 128,   0,   0,   0,
    8,   6,   0,   0,   0,   0,  15, 128,   8,   6,  64, 129,
    0,   0,   0,   0,   8,   6,   0,   7,   2, 122,  39, 128,
    8,   6, 130,  11,  50, 138,  47,   0,   8,   6,   0,   7,
    32, 130,  39,   0,   8,   6,   8,  38, 166, 138,  39, 128,
    8,   6,   0,   7,  34, 250,   7,   0,   8,   6,  49,  36,
    56,  65,   4,   0,   8,   6,   1, 232, 162, 120,  39,   0,
    8,   6, 130,  11,  50, 138,  40, 128,   8,   6,  32,   6,
    8,  32, 135,   0,   8,   6,  16,   3,   4,  18,  70,   0,
    8,   6, 130,   9,  40, 194, 137,   0,   8,   6,  96, 130,
    8,  32, 135,   0,   8,   6,   0,  13,  42, 170,  40, 128,
    8,   6,   0,  11,  50, 138,  40, 128,   8,   6,   0,   7,
    34, 138,  39,   0,   8,   6,   0,  15,  34, 242,   8,   0,
    8,   6,   0,   6, 166, 120,  32, 128,   8,   6,   0,  11,
    50, 130,   8,   0,   8,   6,   0,   7,  32, 112,  47,   0,
    8,   6,  65,  14,  16,  65,  35,   0,   8,   6,   0,   8,
    162, 138, 102, 128,   8,   6,   0,   8, 162, 137,  66,   0,
    8,   6,   0,   8, 162, 170, 165,   0,   8,   6,   0,   8,
    148,  33,  72, 128,   8,   6,   0,   8, 162, 120,  39,   0,
    8,   6,   0,  15, 132,  33,  15, 128,   8,   6,  16, 130,
    16,  32, 129,   0,   8,   6,  32, 130,   8,  32, 130,   0,
    8,   6,  64, 130,   4,  32, 132,   0,   8,   6,  66, 161,
    0,   0,   0,   0,
};

//*****************************************************************************
//
// The font definition for the fixed-point 6x8 font.
//
//*****************************************************************************
const Graphics_Font g_sFontFixed6x8 =
{
    //
    // The format of the font.
    //
    GRAPHICS_FONT_FMT_UNCOMPRESSED,

    //
    // The maximum width of the font.
    //
    6,

    //
    // The height of the font.
    //
    8,

    //
    // The baseline of the font.
    //
    7,

    //
    // The offset to each character in the font.
    //
    {
        0,    8,   16,   24,   32,   40,   48,   56,
        64,   72,   80,   88,   96,  104,  112,  120,
        128,  136,  144,  152,  160,  168,  176,  184,
        192,  200,  208,  216,  224,  232,  240,  248,
        256,  264,  272,  280,  288,  296,  304,  312,
        320,  328,  336,  344,  352,  360,  368,  376,
        384,  392,  400,  408,  416,  424,  432,  440,
        448,  456,  464,  472,  480,  488,  496,  504,
        512,  520,  528,  536,  544,  552,  560,  568,
        576,  584,  592,  600,  608,  616,  624,  632,
        640,  648,  656,  664,  672,  680,  688,  696,
        704,  712,  720,  728,  736,  744,  752,
    },

    //
    // A pointer to the actual font data
    //
    g_pucFontFixed6x8Data
};
