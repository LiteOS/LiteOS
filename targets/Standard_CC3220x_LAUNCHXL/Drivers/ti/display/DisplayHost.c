/*
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
 */

/* ----------------------------------------------------------------------------
 *  Includes
 * ----------------------------------------------------------------------------
 */

#include <ti/display/Display.h>
#include <ti/display/DisplayHost.h>

#include <ti/drivers/dpl/SystemP.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

/* ----------------------------------------------------------------------------
 *  Constants and macros
 * ----------------------------------------------------------------------------
 */
#if !defined(__IAR_SYSTEMS_ICC__)
#define FWRITE(BUF, SIZE, HANDLE, STREAM) fwrite(BUF, SIZE, HANDLE, STREAM)
#else
#include <yfuns.h>
#define FWRITE(BUF, SIZE, HANDLE, STREAM) __write(HANDLE, (unsigned char*) BUF,SIZE)
#endif

/* ----------------------------------------------------------------------------
 *   Type definitions
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 *                           Local variables
 * ----------------------------------------------------------------------------
 */
/* Display function table for DisplayHost implementation */
const Display_FxnTable DisplayHost_fxnTable = {
    DisplayHost_init,
    DisplayHost_open,
    DisplayHost_clear,
    DisplayHost_clearLines,
    DisplayHost_vprintf,
    DisplayHost_close,
    DisplayHost_control,
    DisplayHost_getType,
};

/* ----------------------------------------------------------------------------
 *                                          Functions
 * ----------------------------------------------------------------------------
 */

/*!
 * @fn          DisplayHost_init
 *
 * @brief       Does nothing.
 *
 * @return      void
 */
void DisplayHost_init(Display_Handle handle)
{
}

 /*!
 * @fn          DisplayHost_open
 *
 * @brief       Do nothing.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       params - display parameters
 *
 * @return      Pointer to Display_Config struct
 */
Display_Handle DisplayHost_open(Display_Handle handle,
                               Display_Params *params)
{
    return handle;
}


/*!
 * @fn          DisplayHost_clear
 *
 * @brief       Does nothing.
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayHost_clear(Display_Handle handle)
{
}


/*!
 * @fn          DisplayHost_clearLines
 *
 * @brief       Does nothing.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       lineFrom - line index (0 .. )
 * @param       lineTo - line index (0 .. )
 *
 * @return      void
 */
void DisplayHost_clearLines(Display_Handle handle, uint8_t fromLine,
                           uint8_t toLine)
{
}


/*!
 * @fn          DisplayHost_vprintf
 *
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       aN - optional format arguments
 *
 * @return      void
 */
void DisplayHost_vprintf(Display_Handle handle, uint8_t line,
                         uint8_t column, char *fmt, va_list va)
{
    DisplayHost_HWAttrs *hwAttrs = (DisplayHost_HWAttrs *)handle->hwAttrs;
    int     strSize;
    char   *buf = hwAttrs->strBuf;

    SystemP_vsnprintf(buf, hwAttrs->strBufLen - 1,
            fmt, va);

    strSize = strlen(hwAttrs->strBuf);
    hwAttrs->strBuf[strSize++] = '\n';

    FWRITE(buf, strSize, 1, stdout);
}


/*!
 * @fn          DisplayHost_close
 *
 * @brief       Does nothing
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayHost_close(Display_Handle handle)
{
}

/*!
 * @fn          DisplayHost_control
 *
 * @brief       Function for setting control parameters of the Display driver
 *              after it has been opened.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       cmd - command to execute
 * @param       arg - argument to the command
 *
 * @return      ::DISPLAY_STATUS_UNDEFINEDCMD because no commands are supported
 */
int DisplayHost_control(Display_Handle handle, unsigned int cmd, void *arg)
{
    return DISPLAY_STATUS_UNDEFINEDCMD;
}

/*!
 * @fn          DisplayHost_getType
 *
 * @brief       Returns type of transport
 *
 * @return      Display type
 */
unsigned int DisplayHost_getType(void)
{
    return Display_Type_HOST;
}
