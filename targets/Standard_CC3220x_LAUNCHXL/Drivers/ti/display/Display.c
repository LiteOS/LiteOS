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

/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */
#include <ti/display/Display.h>
#include <stdarg.h>
#include <stdint.h>

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

/* -----------------------------------------------------------------------------
 *  Externs
 * -----------------------------------------------------------------------------
 */
extern const Display_Config Display_config[];

extern const uint8_t Display_count;

/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Default Display parameters structure */
const Display_Params Display_defaultParams = {
    DISPLAY_CLEAR_BOTH,   /* Clear entire line before writing */
};

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */

/*
 *  ======== Display_doOpen ========
 */
Display_Handle Display_doOpen(uint32_t id, Display_Params *params)
{
    Display_Handle handle;

    if (params == NULL)
    {
        params = (Display_Params *)&Display_defaultParams;
    }

    /* Call each driver's open function */
    uint32_t i;
    for (i = 0; i < Display_count; i++)
    {
        handle = (Display_Handle) & Display_config[i];

        /* Open if id matches, or if meta-type matches */
        if (id == i || (handle->fxnTablePtr->getTypeFxn() & id))
        {
            if (NULL == handle->fxnTablePtr->openFxn(handle, params))
            {
                // Couldn't open. Continue trying to open in case a type was
                // provided and there's more than one of the type.
                continue;
            }
            else
            {
                // Return first matching
                return handle;
            }
        }
    }
    // Couldn't open.
    DebugP_log0("Couldn't open selected Displays");
    return NULL;
}

/*
 *  ======== Display_doParamsInit ========
 */
void Display_doParamsInit(Display_Params *params)
{
    *params = Display_defaultParams;
}


/*
 *  ======== Display_doClear ========
 */
void Display_doClear(Display_Handle handle)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return;
    }

    handle->fxnTablePtr->clearFxn(handle);
}


/*
 *  ======== Display_doClearLines ========
 */
void Display_doClearLines(Display_Handle handle, uint8_t fromLine, uint8_t toLine)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return;
    }

    handle->fxnTablePtr->clearLinesFxn(handle, fromLine, toLine);
}


/*
 *  ======== Display_doPrintf ========
 */
void Display_doPrintf(Display_Handle handle, uint8_t line, uint8_t column,
                      char *fmt, ...)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return;
    }

    va_list va;
    va_start(va, fmt);

    handle->fxnTablePtr->vprintfFxn(handle, line, column, fmt, va);
}

/*
 *  ======== Display_doControl ========
 */
void  Display_doControl(Display_Handle handle, unsigned int cmd, void *arg)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return;
    }

    handle->fxnTablePtr->controlFxn(handle, cmd, arg);
}

/*
 *  ======== Display_doClose ========
 */
void Display_doClose(Display_Handle handle)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return;
    }

    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== Display_doGetType ========
 */
uint32_t Display_doGetType(Display_Handle handle)
{
    if (NULL == handle)
    {
        DebugP_log0("Trying to use NULL-handle.");
        return Display_Type_INVALID;
    }

    return handle->fxnTablePtr->getTypeFxn();
}

/*
 *  ======== Display_init ========
 */
void Display_doInit(void)
{
    int i;

    /* Call each driver's init function */
    for (i = 0; i < Display_count; i++) {
        Display_config[i].fxnTablePtr->initFxn((Display_Handle)&(Display_config[i]));
    }
}
