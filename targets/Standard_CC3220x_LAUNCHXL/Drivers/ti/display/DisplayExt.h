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
/** ============================================================================
 *  @file       DisplayExt.h
 *
 *  @brief      Extention of Display to access GrLib functionality in capable
 *              displays.
 *
 * # Driver include #
 *  The display header file should be included in an application as follows:
 *  @code
 *  #include <ti/DisplayExt.h>
 *  @endcode
 *
 *  # Operation #
 *  This extension depends on an already opened Display interface, and
 *  accesses the global Display_config object.
 *
 *  @note All Display statements will be removed and no display operations will
 *        take place if the symbol *Display_DISABLE_ALL* is defined globally,
 *        or before *Display.h* is included.
 *
 *  ## Using the extension #
 *
 *  First, the regular Display interface must be opened, and after that, APIs
 *  in this extension become usable.
 *
 *  @code
 *  #include <ti/display/Display.h>
 *  #include <ti/display/DisplayExt.h>
 *  #include <ti/grlib/grlib.h>
 *
 *  //...
 *  Display_Handle    handle;
 *  Display_Params    params;
 *
 *  Display_Params_init(&params);
 *  Display_open(Display_Type_GRLIB, &params);
 *  //Display_open(Display_ALL, &params);
 *
 *  Graphics_Context *pContext = DisplayExt_getGraphicsContext();
 *
 *  Graphics_Rectangle rect = {
 *      .sXMin = 10,
 *      .sXMax = 30,
 *      .sYMin = 10,
 *      .sYMax = 20,
 *  };
 *
 *  // It's possible that no compatible Display is available or opened.
 *  if (pContext != NULL)
 *  {
 *      Graphics_fillRectangle(pContext, &rect);
 *      Graphics_flushBuffer(pContext);
 *  }
 *  @endcode
 *
 *  ## Implementation #
 *
 *  This interface expects that all Display implementations that report type
 *  Display_Type_GRLIB will also 'inherit' the DisplayGrLib_Object so that the
 *  implementation specific instance object can be cast to the type
 *  DisplayGrLib_Object.
 *
 *  In practice this means that the Graphics_Context object used to control GrLib by
 *  the display implementation must be the first member of the Object
 *  structure used by that implementation.
 *
 *  For example, Display_SharpObject can be cast to DisplayGrLib_Object
 *  to access the Graphics_Context object, without this interface having prior
 *  knowledge about the Sharp display implementation.
 *
 *
 *  # Instrumentation #
 *
 *  The Display interface produces log statements if instrumentation is
 *  enabled.
 *
 *  Diagnostics Mask | Log details |
 *  ---------------- | ----------- |
 *  Diags_USER1      | basic operations performed |
 *  Diags_USER2      | detailed operations performed |
 *
 *  ============================================================================
 */
#ifndef ti_display_DisplayExt__include
#define ti_display_DisplayExt__include

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/display/Display.h>
#include <ti/grlib/grlib.h>

/* -----------------------------------------------------------------------------
 *                                          Constants
 * ------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                                          Macros
 * ------------------------------------------------------------------------------
 */
#if !defined(Display_DISABLE_ALL) || (!Display_DISABLE_ALL)
/*! Get Graphics_Context of first opened Display that uses GrLib \see Display_doGetGrlibContext() */
#  define DisplayExt_getGraphicsContext(handle) \
    DisplayExt_doGetGraphicsContext(handle)
#else
#  define DisplayExt_getGraphicsContext(handle)    NULL
#endif

/* -----------------------------------------------------------------------------
 *                                          Typedefs
 * ------------------------------------------------------------------------------
 */
typedef struct DisplayGrLib_Object
{
    Graphics_Context g_sContext;
} DisplayGrLib_Object, *DisplayGrLib_Handle;

/* -----------------------------------------------------------------------------
 *                                          Functions
 * ------------------------------------------------------------------------------
 */
/*!
 * @brief       Get the Graphics_Context object of the first available and open
 *              Display that uses GrLib for output
 *
 *              Useful for drawing graphics if you know that the display
 *              is GrLib capable.
 *
 * @note        Calling GrLib functions directly is not thread-safe, and
 *              can conflict with Display calls or other GrLib calls.
 *
 * @return      Graphics_Context pointer or NULL if no open display is found.
 */
Graphics_Context  *DisplayExt_doGetGraphicsContext(Display_Handle handle);

#ifdef __cplusplus
}
#endif

#endif //ti_display_DisplayExt__include
