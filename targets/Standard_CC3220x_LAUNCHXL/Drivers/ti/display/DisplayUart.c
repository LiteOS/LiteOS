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

/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */
#include <ti/drivers/UART.h>

#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

#include <string.h>


/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SystemP.h>

/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */
#ifndef MIN
#  define MIN(n, m)    (((n) > (m)) ? (m) : (n))
#endif

#define DISPLAY_UART_ESC_INITIAL            "\x1b\x63"    /* Reset terminal */                \
                                            "\x1b[2J"     /* Clear screen   */                \
                                            "\x1b[10r"    /* Scrolling region from line 10 */ \
                                            "\x1b[11;1H"  /* Set initial cursor to line 11 */

#define DISPLAY_UART_ESC_MOVEPOS_FMT        "\x1b\x37"    /* Save cursor position */ \
                                            "\x1b[10r"    /* Retransmit scroll    */ \
                                            "\x1b[%d;%dH" /* Move cursor fmt str  */

#define DISPLAY_UART_ESC_RESTOREPOS         "\x1b\x38"    /* Restore saved cursor pos */

#define DISPLAY_UART_ESC_CLEAR_SCREEN       "\x1b[2J"     /* Clear screen       */
#define DISPLAY_UART_ESC_CLEAR_CUR_LEFT     "\x1b[1K"     /* Clear cursor left  */
#define DISPLAY_UART_ESC_CLEAR_CUR_RIGHT    "\x1b[0K"     /* Clear cursor right */
#define DISPLAY_UART_ESC_CLEAR_BOTH         "\x1b[2K"     /* Clear line         */
#define DISPLAY_UART_ESC_CLEARSEQ_LEN       4

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */


/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Display function table for minimal UART implementation */
const Display_FxnTable DisplayUartMin_fxnTable = {
    DisplayUartMin_init,
    DisplayUartMin_open,
    DisplayUartMin_clear,
    DisplayUartMin_clearLines,
    DisplayUartMin_vprintf,
    DisplayUartMin_close,
    DisplayUartMin_control,
    DisplayUartMin_getType,
};

/* Display function table for UART ANSI implementation */
const Display_FxnTable DisplayUartAnsi_fxnTable = {
    DisplayUartAnsi_init,
    DisplayUartAnsi_open,
    DisplayUartAnsi_clear,
    DisplayUartAnsi_clearLines,
    DisplayUartAnsi_vprintf,
    DisplayUartAnsi_close,
    DisplayUartAnsi_control,
    DisplayUartAnsi_getType,
};

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */

 /*!
 * @fn          DisplayUartMin_init
 *
 * @brief       Does nothing.
 *
 * @return      void
 */
void DisplayUartMin_init(Display_Handle handle)
{
}

 /*!
 * @fn          DisplayUartAnsi_init
 *
 * @brief       Does nothing.
 *
 * @return      void
 */
void DisplayUartAnsi_init(Display_Handle handle)
{
}

 /*!
 * @fn          DisplayUartMin_open
 *
 * @brief       Initialize the UART transport
 *
 * @descr       Opens the UART index specified in the HWAttrs, and creates a
 *              mutex semaphore
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       params - display parameters
 *
 * @return      Pointer to Display_Config struct
 */
Display_Handle DisplayUartMin_open(Display_Handle hDisplay,
                                Display_Params *params)
{
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;

    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.baudRate  = hwAttrs->baudRate;
    uartParams.writeMode = UART_MODE_BLOCKING;

    object->mutex = SemaphoreP_createBinary(1);
    if (object->mutex == NULL)
    {
        DebugP_log0("DisplayUart.c: Couldn't create semaphore");
        return NULL;
    }

    UART_init();
    object->hUart = UART_open(hwAttrs->uartIdx, &uartParams);
    if (NULL == object->hUart)
    {
        DebugP_log0("DisplayUart.c: Couldn't open UART");
        return NULL;
    }

    /* Disable RX in case that is needed to save power */
    UART_control(object->hUart, UART_CMD_RXDISABLE, NULL);

    return hDisplay;
}

 /*!
 * @fn          DisplayUartAnsi_open
 *
 * @brief       Initialize the UART transport
 *
 * @descr       Opens the UART index specified in the HWAttrs, and creates a
 *              mutex semaphore
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       params - display parameters
 *
 * @return      Pointer to Display_Config struct
 */
Display_Handle DisplayUartAnsi_open(Display_Handle hDisplay,
                                    Display_Params *params)
{
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;

    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.baudRate  = hwAttrs->baudRate;
    uartParams.writeMode = UART_MODE_BLOCKING;

    object->mutex = SemaphoreP_createBinary(1);
    if (object->mutex == NULL)
    {
        DebugP_log0("DisplayUart.c: Couldn't create semaphore");
        return NULL;
    }

    UART_init();
    object->hUart = UART_open(hwAttrs->uartIdx, &uartParams);
    if (NULL == object->hUart)
    {
        DebugP_log0("DisplayUart.c: Couldn't open UART");
        return NULL;
    }

    /* Disable RX in case that is needed to save power */
    UART_control(object->hUart, UART_CMD_RXDISABLE, NULL);

    switch (params->lineClearMode)
    {
        case DISPLAY_CLEAR_BOTH:  object->lineClearSeq = DISPLAY_UART_ESC_CLEAR_BOTH; break;
        case DISPLAY_CLEAR_LEFT:  object->lineClearSeq = DISPLAY_UART_ESC_CLEAR_CUR_LEFT; break;
        case DISPLAY_CLEAR_RIGHT: object->lineClearSeq = DISPLAY_UART_ESC_CLEAR_CUR_RIGHT; break;
        default: /* fall-through */
        case DISPLAY_CLEAR_NONE: object->lineClearSeq = NULL; break;
    }

    /* Send VT100 initial config to terminal */
    UART_write(object->hUart, DISPLAY_UART_ESC_INITIAL, sizeof DISPLAY_UART_ESC_INITIAL - 1);

    return hDisplay;
}

/*!
 * @fn          DisplayUartMin_clear
 *
 * @brief       Does nothing, as output is stateless.
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayUartMin_clear(Display_Handle hDisplay)
{
}

/*!
 * @fn          DisplayUartAnsi_clear
 *
 * @brief       Does nothing, as output is stateless.
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayUartAnsi_clear(Display_Handle hDisplay)
{
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;

    if (SemaphoreP_pend(object->mutex, hwAttrs->mutexTimeout) == SemaphoreP_OK)
    {
        UART_write(object->hUart, DISPLAY_UART_ESC_CLEAR_SCREEN, 4);
        SemaphoreP_post(object->mutex);
    }
}

/*!
 * @fn          DisplayUartMin_clearLines
 *
 * @brief       Does nothing, as output is stateless.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       lineFrom - line index (0 .. )
 * @param       lineTo - line index (0 .. )
 *
 * @return      void
 */
void DisplayUartMin_clearLines(Display_Handle hDisplay,
                               uint8_t lineFrom, uint8_t lineTo)
{
}

/*!
 * @fn          DisplayUartAnsi_clearLines
 *
 * @brief       Does nothing, as output is stateless.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       lineFrom - line index (0 .. )
 * @param       lineTo - line index (0 .. )
 *
 * @return      void
 */
void DisplayUartAnsi_clearLines(Display_Handle hDisplay,
                                uint8_t lineFrom, uint8_t lineTo)
{
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;

    uint32_t      strSize = 0;
    uint32_t      curLine = 0;
    const uint8_t uartClearLineMoveDown[] = "\x1b[2K\x1b\x45";

    if (lineTo <= lineFrom)
    {
        lineTo = lineFrom;
    }

    if (SemaphoreP_pend(object->mutex, hwAttrs->mutexTimeout) == SemaphoreP_OK)
    {
        strSize += SystemP_snprintf(hwAttrs->strBuf, hwAttrs->strBufLen,
                                   DISPLAY_UART_ESC_MOVEPOS_FMT, lineFrom + 1, 0);

        for (curLine = lineFrom + 1; curLine < lineTo + 2; curLine++)
        {
            memcpy(hwAttrs->strBuf + strSize, uartClearLineMoveDown, sizeof uartClearLineMoveDown - 1);
            strSize += sizeof uartClearLineMoveDown - 1;

            if (hwAttrs->strBufLen - strSize < sizeof DISPLAY_UART_ESC_RESTOREPOS - 1 + sizeof uartClearLineMoveDown - 1)
            {
                UART_write(object->hUart, hwAttrs->strBuf, strSize);
                strSize = 0;
            }
        }

        memcpy(hwAttrs->strBuf + strSize, DISPLAY_UART_ESC_RESTOREPOS, sizeof DISPLAY_UART_ESC_RESTOREPOS - 1);
        strSize += sizeof DISPLAY_UART_ESC_RESTOREPOS - 1;

        UART_write(object->hUart, hwAttrs->strBuf, strSize);
        SemaphoreP_post(object->mutex);
    }
}


/*!
 * @fn          DisplayUartMin_vprintf
 *
 * @brief       Write a text string to UART with return and newline.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       aN - optional format arguments
 *
 * @return      void
 */
void DisplayUartMin_vprintf(Display_Handle hDisplay, uint8_t line,
                            uint8_t column, char *fmt, va_list va)
{
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;

    uint32_t strSize = 0;

    if (SemaphoreP_pend(object->mutex, hwAttrs->mutexTimeout) == SemaphoreP_OK)
    {
        SystemP_vsnprintf(hwAttrs->strBuf, hwAttrs->strBufLen - 2, fmt, va);

        strSize = strlen(hwAttrs->strBuf);
        hwAttrs->strBuf[strSize++] = '\r';
        hwAttrs->strBuf[strSize++] = '\n';

        UART_write(object->hUart, hwAttrs->strBuf, strSize);
        SemaphoreP_post(object->mutex);
    }
}

/*!
 * @fn          DisplayUartAnsi_vprintf
 *
 * @brief       Write a text string to UART with return and newline.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       aN - optional format arguments
 *
 * @return      void
 */
void DisplayUartAnsi_vprintf(Display_Handle hDisplay, uint8_t line,
                             uint8_t column, char *fmt, va_list va)
{
    DisplayUart_Object  *object  = (DisplayUart_Object  *)hDisplay->object;
    DisplayUart_HWAttrs *hwAttrs = (DisplayUart_HWAttrs *)hDisplay->hwAttrs;

    uint32_t       strSize = 0;

    char          *strBuf = hwAttrs->strBuf;
    const uint16_t bufLen = hwAttrs->strBufLen;

    if (SemaphoreP_pend(object->mutex, hwAttrs->mutexTimeout) == SemaphoreP_OK)
    {
        if (line != DisplayUart_SCROLLING)
        {
            /* Add cursor movement escape sequence */
            strSize += SystemP_snprintf(strBuf + strSize, bufLen - strSize - 2,
                                        DISPLAY_UART_ESC_MOVEPOS_FMT, line + 1, column + 1);

            /* Add line clearing escape sequence */
            if (object->lineClearSeq)
            {
                memcpy(strBuf + strSize, object->lineClearSeq, DISPLAY_UART_ESC_CLEARSEQ_LEN);
                strSize += DISPLAY_UART_ESC_CLEARSEQ_LEN;
            }
        }

        strSize += SystemP_vsnprintf(strBuf + strSize, bufLen - strSize - 2, fmt, va);

        if (line != DisplayUart_SCROLLING)
        {
            memcpy(strBuf + strSize, DISPLAY_UART_ESC_RESTOREPOS, sizeof DISPLAY_UART_ESC_RESTOREPOS - 1);
            strSize += 2;
        }
        else
        {
            strBuf[strSize++] = '\r';
            strBuf[strSize++] = '\n';
        }

        UART_write(object->hUart, strBuf, strSize);
        SemaphoreP_post(object->mutex);
    }
}

/*!
 * @fn          DisplayUartMin_close
 *
 * @brief       Closes the UART handle
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayUartMin_close(Display_Handle hDisplay)
{
    DisplayUart_Object *object = (DisplayUart_Object  *)hDisplay->object;

    // Not sure what happens if someone is writing
    UART_close(object->hUart);
    object->hUart = NULL;

    // Not sure what happens if someone is pending
    SemaphoreP_delete(object->mutex);
}

/*!
 * @fn          DisplayUartAnsi_close
 *
 * @brief       Closes the UART handle
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplayUartAnsi_close(Display_Handle hDisplay)
{
    DisplayUart_Object *object = (DisplayUart_Object  *)hDisplay->object;

    // Not sure what happens if someone is writing
    UART_close(object->hUart);
    object->hUart = NULL;

    // Not sure what happens if someone is pending
    SemaphoreP_delete(object->mutex);
}

/*!
 * @fn          DisplayUartMin_control
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
int DisplayUartMin_control(Display_Handle hDisplay, unsigned int cmd, void *arg)
{
    return DISPLAY_STATUS_UNDEFINEDCMD;
}

/*!
 * @fn          DisplayUartAnsi_control
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
int DisplayUartAnsi_control(Display_Handle hDisplay, unsigned int cmd, void *arg)
{
    return DISPLAY_STATUS_UNDEFINEDCMD;
}

/*!
 * @fn          DisplayUartMin_getType
 *
 * @brief       Returns type of transport
 *
 * @return      Display type UART
 */
unsigned int DisplayUartMin_getType(void)
{
    return Display_Type_UART;
}

/*!
 * @fn          DisplayUartAnsi_getType
 *
 * @brief       Returns type of transport
 *
 * @return      Display type UART
 */
unsigned int DisplayUartAnsi_getType(void)
{
    return Display_Type_UART | Display_Type_ANSI;
}
