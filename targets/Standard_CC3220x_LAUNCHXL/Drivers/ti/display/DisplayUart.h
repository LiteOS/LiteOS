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
 *  @file       DisplayUart.h
 *
 *  @brief      Display.h implementation for UART output
 *
 *  # DisplayUart specifics #
 *
 *  DisplayUart has two sets of function pointer tables. One which adds some
 *  ANSI/VT100 codes to the output, for cursor placement etc, and a minimal
 *  implementation which basically is a wrapper for UART_write.
 *    * DisplayUartAnsi_fxnTable
 *    * DisplayUartMin_fxnTable
 *
 *
 *  ## DisplayUartMin #
 *
 *  DisplayUartMin simply formats and outputs the text over UART and adds a
 *  newline at the end of each statement.
 *
 *  Calls to Display_clear, Display_clearLine(s), and the line and column
 *  specifiers in Display_printf(handle, line, col, fmt, ...) are ignored.
 *
 *  ## DisplayUartAnsi #
 *
 *  DisplayUartAnsi will send the following escape-strings to the UART when
 *  it is opened:
 *    * Reset terminal
 *    * Clear screen
 *    * Set scrolling region from line 10 (not inclusive)
 *    * Set cursor to line 11
 *
 *  When Display_print(handle, line, col, fmt, ...) is called with a line number
 *  the following is sent:
 *    * Save cursor position
 *    * Set scroll region from line 10
 *    * Move cursor to line, column
 *    * String to be printed
 *    * Restore cursor position
 *
 *  If Display_printf is called with the line "number" `DisplayUart_SCROLLING`,
 *  the string to be printed is simply output at the current cursor location,
 *  without saving or restoring the position. If the terminal supports the
 *  scrolling region feature, as most do, then the terminal will ensure that
 *  the content output here will scroll up but not overwrite the text written
 *  outside the scroll region.
 *
 *  In this manner it is possible to have two separate outputs, one with a
 *  log of events, and one with fixed positions as on an LCD. Unless the
 *  `DisplayUart_SCROLLING` line specifier is used, any line number can be used,
 *  also those nominally inside the scrolling region.
 *
 *  There is also a helper file <ti/display/AnsiColor.h> with a macro to set the
 *  color and style of the text.
 *
 *  # Usage Example #
 *
 *  @code
 *  #include <ti/display/Display.h>
 *  #include <ti/display/DisplayUart.h>
 *  #include <ti/display/AnsiColor.h>
 *
 *  #define MAXPRINTLEN 128
 *
 *  DisplayUart_Object displayUartObject;
 *  static char uartStringBuf[MAXPRINTLEN];
 *
 *  const DisplayUart_HWAttrs displayUartHWAttrs = {
 *      .uartIdx      = Board_UART,
 *      .baudRate     = 115200,
 *      .mutexTimeout = BIOS_WAIT_FOREVER,
 *      .strBuf       = uartStringBuf,
 *      .strBufLen    = MAXPRINTLEN,
 *  };
 *
 *  const Display_Config Display_config[] = {
 *      {
 *          .fxnTablePtr = &DisplayUartAnsi_fxnTable,
 *        //.fxnTablePtr = &DisplayUartMin_fxnTable,
 *          .object = &displayUartObject,
 *          .hwAttrs = &displayUartHWAttrs
 *      }
 *  };
 *
 *  const uint8_t Display_count = sizeof(Display_config) / sizeof(Display_Config);
 *
 *  void myTask(uintptr_t a0, uintptr_t a1)
 *  {
 *    Display_Handle handle = Display_open(Display_Type_UART);
 *    Display_printf(handle, 1, 0, "Hello");
 *    Display_printf(handle, 2, 6, ANSI_COLOR(FG_GREEN) "World!" ANSI_COLOR(ATTR_RESET));
 *  }
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_display_DisplayUart__include
#define ti_display_DisplayUart__include

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/UART.h>
#include <ti/display/Display.h>
#include <stdint.h>

/* Ensure backwards compatibility. Old examples will use Minimal. */
#ifndef DisplayUart_fxnTable
#  define DisplayUart_fxnTable     DisplayUartMin_fxnTable
#endif

/* Line number that means 'put in scrolling section', if exists */
#define DisplayUart_SCROLLING       0xFF

extern const Display_FxnTable DisplayUartMin_fxnTable;
extern const Display_FxnTable DisplayUartAnsi_fxnTable;

/*!
 *  @brief DisplayUart Attributes
 *
 *  The DisplayUart driver uses a buffer for formatting messages, which
 *  is then passed to UART_write().  The location and size of
 *  the buffer are specified in a DisplayUart_HWAttrs structure.
 *  Access to the buffer is synchronized by a semaphore.  The timeout
 *  for acquiring the semaphore is specified in the attributes.
 */
typedef struct DisplayUart_HWAttrs
{
    /*! Index of uart in UART_config[] */
    unsigned int uartIdx;
    /*! Baud rate for uart */
    unsigned int baudRate;
    /*! Timeout for acquiring semaphore */
    unsigned int mutexTimeout;
    /*! Buffer for formatting messages */
    char        *strBuf;
    /*! Size of buffer */
    uint16_t     strBufLen;
} DisplayUart_HWAttrs;


/*!
 *  @brief      DisplayUart Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct DisplayUart_Object
{
    UART_Handle       hUart;
    SemaphoreP_Handle mutex;
    char              *lineClearSeq;
} DisplayUart_Object, *DisplayUart_Handle;


void DisplayUartMin_init(Display_Handle handle);
void DisplayUartAnsi_init(Display_Handle handle);

Display_Handle DisplayUartMin_open(Display_Handle, Display_Params * params);
Display_Handle DisplayUartAnsi_open(Display_Handle, Display_Params * params);

void DisplayUartMin_clear(Display_Handle handle);
void DisplayUartAnsi_clear(Display_Handle handle);

void DisplayUartMin_clearLines(Display_Handle handle,
                               uint8_t fromLine,
                               uint8_t toLine);
void DisplayUartAnsi_clearLines(Display_Handle handle,
                                uint8_t fromLine,
                                uint8_t toLine);

void DisplayUartMin_vprintf(Display_Handle handle, uint8_t line,
                            uint8_t column, char *fmt, va_list va);
void DisplayUartAnsi_vprintf(Display_Handle handle, uint8_t line,
                             uint8_t column, char *fmt, va_list va);

void DisplayUartMin_close(Display_Handle);
void DisplayUartAnsi_close(Display_Handle);

int DisplayUartMin_control(Display_Handle handle, unsigned int cmd, void *arg);
int DisplayUartAnsi_control(Display_Handle handle, unsigned int cmd, void *arg);

unsigned int   DisplayUartMin_getType(void);
unsigned int   DisplayUartAnsi_getType(void);

#endif /* ti_display_DisplayUart__include */
