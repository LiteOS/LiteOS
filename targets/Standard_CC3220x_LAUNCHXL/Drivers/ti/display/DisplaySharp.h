/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
 */

#ifndef ti_display_DisplaySharp__include
#define ti_display_DisplaySharp__include

#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/SPI.h>

#include <ti/display/Display.h>
#include <ti/grlib/grlib.h>

#include <stdint.h>

/**
 *  @addtogroup DISPLAY_CMD
 *  DISPLAYSHARP_CMD_* macros are command codes only defined in the
 *  DisplaySharp.h driver implementation and need to:
 *  @code
 *  #include <ti/display/DisplaySharp.h>
 *  @endcode
 *  @{
 */

/*!
 * @brief Command used by Display_control to invert the display
 *
 * This command allows changing the background and foreground colors from
 * the default, depending on the value (1 - invert, 0 - don't invert) of @b arg
 *
 * With this command @b arg is of type @c DisplaySharpColor_t *.
 */
#define DISPLAYSHARP_CMD_SET_COLORS  DISPLAY_CMD_RESERVED + 0
/** @}*/

/*!
 *  @internal
 *  @brief
 *  Display function table pointer
 */
extern const Display_FxnTable DisplaySharp_fxnTable;

/*!
 *  @brief Foreground and background color struct, compatible with GrLib.
 *
 *  Default values on DisplaySharp_open() are - from grlib.h:
 *    bg - ClrBlack
 *    fg - ClrWhite
 */
typedef struct
{
    uint32_t fg;
    uint32_t bg;
} DisplaySharpColor_t;

typedef struct DisplaySharp_HWAttrsV1
{
    uint32_t spiIndex;
    uint32_t csPin;
    uint32_t powerPin;
    uint32_t enablePin;
    uint16_t pixelHeight;
    uint16_t pixelWidth;
    void    *displayBuf;
} DisplaySharp_HWAttrsV1;

typedef uint8_t   DisplaySharp_Buf_96x96[96 * 96 / 8];
typedef uint8_t   DisplaySharp_Buf_128x128[128 * 128 / 8];

typedef struct DisplaySharp_Object
{
    Graphics_Context      g_sContext;
    SPI_Handle            hSpi;
    Display_LineClearMode lineClearMode;
    Graphics_Display      g_sDisplay;
    DisplaySharpColor_t   displayColor;
    SemaphoreP_Handle     semLCD;
    uint8_t              *displayBuffer;
} DisplaySharp_Object, *DisplaySharp_Handle;

void DisplaySharp_init(Display_Handle handle);
Display_Handle DisplaySharp_open(Display_Handle,
                                 Display_Params * params);
void           DisplaySharp_clear(Display_Handle handle);
void           DisplaySharp_clearLines(Display_Handle handle,
                                       uint8_t fromLine,
                                       uint8_t toLine);
void           DisplaySharp_vprintf(Display_Handle handle, uint8_t line,
                                    uint8_t column, char *fmt, va_list va);
void DisplaySharp_close(Display_Handle);
int DisplaySharp_control(Display_Handle handle, unsigned int cmd, void *arg);
unsigned int   DisplaySharp_getType(void);
#endif // ti_display_DisplaySharp__include
