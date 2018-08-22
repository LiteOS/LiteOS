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

/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/dpl/SystemP.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>

#include <ti/grlib/grlib.h>
#include <ti/display/Display.h>

#include <ti/display/DisplaySharp.h>
#include <ti/display/lcd/SharpGrLib.h>

/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */
// Timeout of semaphore that controls exclusive to the LCD (infinite)
#define ACCESS_TIMEOUT    SemaphoreP_WAIT_FOREVER

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */


/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Display function table for sharp implementation */
const Display_FxnTable DisplaySharp_fxnTable = {
    DisplaySharp_init,
    DisplaySharp_open,
    DisplaySharp_clear,
    DisplaySharp_clearLines,
    DisplaySharp_vprintf,
    DisplaySharp_close,
    DisplaySharp_control,
    DisplaySharp_getType,
};

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */
/*!
 * @fn          DisplaySharp_init
 *
 * @brief       Does nothing.
 *
 * @return      void
 */
void DisplaySharp_init(Display_Handle Display_config)
{
}

/*!
 * @fn          DisplaySharp_open
 *
 * @brief       Initialize the LCD
 *
 * @descr       Initializes the pins used by the LCD, creates resource access
 *              protection semaphore, turns on the LCD device, initializes the
 *              frame buffer, initializes to white background/dark foreground,
 *              and finally clears the object->displayColor.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       params - display parameters
 *
 * @return      Pointer to Display_Config struct
 */
Display_Handle DisplaySharp_open(Display_Handle hDisplay,
                                 Display_Params *params)
{
    DisplaySharp_HWAttrsV1 *hwAttrs = (DisplaySharp_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplaySharp_Object  *object  = (DisplaySharp_Object  *)hDisplay->object;

    GPIO_init();    /* It is OK to call GPIO_init() and SPI_init() multiple times. */
    SPI_init();

    object->lineClearMode = params->lineClearMode;

    GPIO_setConfig(hwAttrs->csPin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    if (hwAttrs->powerPin != (uint32_t)-1) {
        GPIO_setConfig(hwAttrs->powerPin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    }
    GPIO_setConfig(hwAttrs->enablePin, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.bitRate = 4000000;

    object->hSpi = SPI_open(hwAttrs->spiIndex, &spiParams);

    if (object->hSpi == NULL)
    {
        return NULL;
    }

    // Init colors
    object->displayColor.bg = ClrBlack;
    object->displayColor.fg = ClrWhite;

    object->semLCD = SemaphoreP_createBinary(1);
    if (object->semLCD == NULL)
    {
        SPI_close(object->hSpi);
        return NULL;
    }

    // Grab LCD
    SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT);

    // Initialize the GrLib back-end transport
    SharpGrLib_init(object->hSpi, hwAttrs->csPin);

    object->displayBuffer = hwAttrs->displayBuf;
    object->g_sDisplay.size          = sizeof(struct Graphics_Display);
    object->g_sDisplay.pFxns         = &g_sharpFxns;
    object->g_sDisplay.displayData   = object->displayBuffer;
    object->g_sDisplay.heigth        = hwAttrs->pixelHeight;
    object->g_sDisplay.width         = hwAttrs->pixelWidth;

    // Graphics library init
    Graphics_initContext(&object->g_sContext, &object->g_sDisplay, &g_sharpFxns);

    // Graphics properties
    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
    Graphics_setBackgroundColor(&object->g_sContext, object->displayColor.bg);
    Graphics_setFont(&object->g_sContext, &g_sFontFixed6x8);

    // Clear display
    Graphics_clearDisplay(&object->g_sContext);
    Graphics_flushBuffer(&object->g_sContext);

    // Release LCD
    SemaphoreP_post(object->semLCD);

    return hDisplay;
}


/*!
 * @fn          DisplaySharp_clear
 *
 * @brief       Clears the display
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplaySharp_clear(Display_Handle hDisplay)
{
    DisplaySharp_Object *object = (DisplaySharp_Object  *)hDisplay->object;

    // Grab LCD
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        Graphics_clearDisplay(&object->g_sContext);
        Graphics_flushBuffer(&object->g_sContext);

        // Release LCD
        SemaphoreP_post(object->semLCD);
    }
}


/*!
 * @fn          DisplaySharp_clearLines
 *
 * @brief       Clears lines lineFrom-lineTo of the display, inclusive
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       lineFrom - line index (0 .. )
 * @param       lineTo - line index (0 .. )
 *
 * @return      void
 */
void DisplaySharp_clearLines(Display_Handle hDisplay,
                             uint8_t lineFrom, uint8_t lineTo)
{
    DisplaySharp_Object *object = (DisplaySharp_Object  *)hDisplay->object;

    if (lineTo <= lineFrom)
    {
        lineTo = lineFrom;
    }

    Graphics_Rectangle rect = {
        .xMin = 0,
        .xMax = object->g_sContext.clipRegion.xMax,
        .yMin = lineFrom * object->g_sContext.font->height,
        .yMax = (lineTo + 1) * object->g_sContext.font->height - 1,
    };

    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.bg);
    Graphics_fillRectangle(&object->g_sContext, &rect);
    Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
    Graphics_flushBuffer(&object->g_sContext);
}


/*!
 * @fn          DisplaySharp_put5
 *
 * @brief       Write a text string to a specific line/column of the display
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       aN - optional format arguments
 *
 * @return      void
 */
void DisplaySharp_vprintf(Display_Handle hDisplay, uint8_t line,
                          uint8_t column, char *fmt, va_list va)
{
    DisplaySharp_Object *object = (DisplaySharp_Object  *)hDisplay->object;

    uint8_t xp, yp, clearStartX, clearEndX;

    char    dispStr[23];

    // Grab LCD
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        xp          = column * object->g_sContext.font->maxWidth + 1;
        yp          = line * object->g_sContext.font->height + 0;
        clearStartX = clearEndX = xp;

        switch (object->lineClearMode)
        {
        case DISPLAY_CLEAR_LEFT:
            clearStartX = 0;
            break;
        case DISPLAY_CLEAR_RIGHT:
            clearEndX = object->g_sContext.clipRegion.xMax;
            break;
        case DISPLAY_CLEAR_BOTH:
            clearStartX = 0;
            clearEndX   = object->g_sContext.clipRegion.xMax;
            break;
        case DISPLAY_CLEAR_NONE:
        default:
            break;
        }

        if (clearStartX != clearEndX)
        {
            Graphics_Rectangle rect = {
                .xMin = clearStartX,
                .xMax = clearEndX,
                .yMin = yp,
                .yMax = yp + object->g_sContext.font->height - 1,
            };

            Graphics_setForegroundColor(&object->g_sContext, object->displayColor.bg);
            Graphics_fillRectangle(&object->g_sContext, &rect);
            Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
        }

        SystemP_vsnprintf(dispStr, sizeof(dispStr), fmt, va);

        // Draw a text on the display
        Graphics_drawString(&object->g_sContext,
                           (int8_t *)dispStr,
                           AUTO_STRING_LENGTH,
                           xp,
                           yp,
                           OPAQUE_TEXT);

        Graphics_flushBuffer(&object->g_sContext);

        // Release LCD
        SemaphoreP_post(object->semLCD);
    }
}


/*!
 * @fn          DisplaySharp_close
 *
 * @brief       Turns of the display and releases the LCD control pins
 *
 * @param       hDisplay - pointer to Display_Config struct
 *
 * @return      void
 */
void DisplaySharp_close(Display_Handle hDisplay)
{
    DisplaySharp_HWAttrsV1 *hwAttrs = (DisplaySharp_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplaySharp_Object  *object  = (DisplaySharp_Object  *)hDisplay->object;

    // Grab LCD
    if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
    {
        // Turn off the display
        GPIO_write(hwAttrs->enablePin, 0x00);

        SPI_close(object->hSpi);
        object->hSpi = NULL;

        // Deconfigure GrLib back-end
        SharpGrLib_init(NULL, 0);

        // Release LCD
        SemaphoreP_post(object->semLCD);
    }
}

/*!
 * @fn          DisplaySharp_control
 *
 * @brief       Function for setting control parameters of the Display driver
 *              after it has been opened.
 *
 * @param       hDisplay - pointer to Display_Config struct
 * @param       cmd - command to execute, supported commands are:
 *              | Command                        | Description             |
 *              |------------------------------- |-------------------------|
 *              | ::DISPLAY_CMD_TRANSPORT_CLOSE  | Close SPI but leave control pins |
 *              | ::DISPLAY_CMD_TRANSPORT_OPEN   | Re-open SPI driver      |
 * @param       arg - argument to the command
 *
 * @return      ::DISPLAY_STATUS_SUCCESS if success, or error code if error.
 */
int DisplaySharp_control(Display_Handle hDisplay, unsigned int cmd, void *arg)
{
    DisplaySharp_HWAttrsV1 *hwAttrs = (DisplaySharp_HWAttrsV1 *)hDisplay->hwAttrs;
    DisplaySharp_Object  *object  = (DisplaySharp_Object  *)hDisplay->object;

    /* Initialize return value */
    int ret = DISPLAY_STATUS_ERROR;

    /* Perform command */
    switch(cmd)
    {
        case DISPLAY_CMD_TRANSPORT_CLOSE:
            // Grab LCD
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                if (object->hSpi)
                {
                    // Close SPI and tell back-end there is no SPI
                    SPI_close(object->hSpi);
                    SharpGrLib_init(NULL, 0);
                    object->hSpi = NULL;
                    ret = DISPLAY_STATUS_SUCCESS;
                }
                // Release LCD
                SemaphoreP_post(object->semLCD);
            }
            break;

        case DISPLAY_CMD_TRANSPORT_OPEN:
            // Grab LCD
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                if (NULL == object->hSpi)
                {
                    // Re-open SPI and re-init back-end
                    SPI_Params spiParams;
                    SPI_Params_init(&spiParams);
                    spiParams.bitRate = 4000000;
                    object->hSpi = SPI_open(hwAttrs->spiIndex, &spiParams);
                    SharpGrLib_init(object->hSpi, hwAttrs->csPin);
                    ret = DISPLAY_STATUS_SUCCESS;
                }
                // Release LCD
                SemaphoreP_post(object->semLCD);
            }
            break;

        case DISPLAYSHARP_CMD_SET_COLORS:
            // Grab LCD
            if (SemaphoreP_pend(object->semLCD, ACCESS_TIMEOUT) == SemaphoreP_OK)
            {
                object->displayColor = *(DisplaySharpColor_t *)arg;

                Graphics_setForegroundColor(&object->g_sContext, object->displayColor.fg);
                Graphics_setBackgroundColor(&object->g_sContext, object->displayColor.bg);

                // Release LCD
                SemaphoreP_post(object->semLCD);

                // Return success
                ret = DISPLAY_STATUS_SUCCESS;
            }
            break;

        default:
            /* The command is not defined */
            ret = SPI_STATUS_UNDEFINEDCMD;
            break;
    }

    return ret;
}

/*!
 * @fn          DisplaySharp_getType
 *
 * @brief       Returns type of transport
 *
 * @return      Display type define LCD
 */
unsigned int DisplaySharp_getType(void)
{
    return Display_Type_LCD | Display_Type_GRLIB;
}
