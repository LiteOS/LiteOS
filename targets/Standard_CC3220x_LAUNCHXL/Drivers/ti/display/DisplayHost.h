/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       DisplayHost.h
 *
 *  @brief      Display driver for printf-like output.
 *
 *  # Driver include #
 *  The header file should be included in an application as follows:
 *  @code
 *  #include <ti/display/Display.h>
 *  #include <ti/display/DisplayHost.h>
 *  @endcode
 *
 *
 *  Below is an example configuration to use the HOST display.
 *
 *  @code
 *  #include <ti/display/Display.h>
 *  #include <ti/display/DisplayHost.h>
 *
 *  #define MAXPRINTLEN 1024
 *
 *  DisplayHost_Object displayHostObject;
 *  static char displayBuf[MAXPRINTLEN];
 *
 *  const DisplayHost_HWAttrs displayHostHWAttrs = {
 *      .strBuf = displayBuf,
 *      .strBufLen = MAXPRINTLEN
 *  };
 *
 *  const Display_Config Display_config[] = {
 *      {
 *          .fxnTablePtr = &DisplayHost_fxnTable,
 *          .object = &displayHostObject,
 *          .hwAttrs = &displayHostHWAttrs
 *      }
 *  };
 *
 *  const uint8_t Display_count = sizeof(Display_config) / sizeof(Display_Config);
 *  @endcode
 *
 *  # Calling Context #
 *
 *  For TIRTOS based applications, the Display_HOST interface is only
 *  callable from Task context.  The Display_HOST interface is callable
 *  from any context for FreeRTOS based applications.
 *
 *  ============================================================================
 */

#ifndef ti_display_DisplayHost__include
#define ti_display_DisplayHost__include

#ifdef __cplusplus
extern "C"
{
#endif

#include <ti/display/Display.h>

/*!
 *  @brief DisplayHost Attributes
 *
 *  The DisplayHost driver uses a buffer for formatting messages, which
 *  is then passed to an fwrite() function.  The location and size of
 *  the buffer are specified in a DisplayHost_HWAttrs structure.
 */
typedef struct DisplayHost_HWAttrs
{
    /*! Buffer for formatting messages */
    char        *strBuf;
    /*! Size of buffer */
    uint16_t     strBufLen;
} DisplayHost_HWAttrs;

/*!
 *  @brief      DisplayHost Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct DisplayHost_Object
{
    int       dummy;
} DisplayHost_Object, *DisplayHost_Handle;


/* DisplayHost function table pointer */
extern const Display_FxnTable DisplayHost_fxnTable;


/* DisplayHost functions */
void DisplayHost_init(Display_Handle handle);
Display_Handle DisplayHost_open(Display_Handle,
                               Display_Params * params);
void           DisplayHost_clear(Display_Handle handle);
void           DisplayHost_clearLines(Display_Handle handle, uint8_t fromLine,
                                     uint8_t toLine);
void           DisplayHost_vprintf(Display_Handle handle, uint8_t line,
                                   uint8_t column, char *fmt, va_list va);
void DisplayHost_close(Display_Handle);
int DisplayHost_control(Display_Handle handle, unsigned int cmd, void *arg);
unsigned int   DisplayHost_getType(void);
#ifdef __cplusplus
}
#endif

#endif /* ti_display_DisplayHost__include */
