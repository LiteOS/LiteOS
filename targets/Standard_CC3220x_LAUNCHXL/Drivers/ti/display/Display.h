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
 *  @file       Display.h
 *
 *  @brief      Generic interface for text output
 *
 *  # Driver include #
 *  The display header file should be included in an application as follows:
 *  @code
 *  #include <ti/display/Display.h>
 *  @endcode
 *
 *  # Operation #
 *  The Display middleware-driver in TI-RTOS is designed to abstract operations
 *  and considerations specific to given output method.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation. The application initializes the Display driver by calling
 *  Display_init() and is then ready to open a Display by calling
 *  Display_open() and passing in a Display parameters data structure.
 *
 *  @note Display APIs can only be called from task context.
 *
 *  @note All Display statements will be removed and no display operations will
 *        take place if the symbol *Display_DISABLE_ALL* is defined globally,
 *        or before *Display.h* is included.
 *
 *  ## Opening the driver #
 *
 *  Because the Display driver serves as an abstraction, it is possible to
 *  specify the *type* of Display implementations that should be opened, as
 *  well as alternatively the config-array index value like with other TI-RTOS
 *  drivers.
 *
 *  Optionally, if no particular parameters are desired, the pointer to params
 *  can be replaced with NULL.
 *
 *  @code
 *  Display_Handle    handle;
 *  Display_Params    params;
 *
 *  Display_Params_init(&params);
 *  handle = Display_open(someConfigIndexValue, &params);
 *  //Display_open(Display_Type_HOST, &params);
 *  //Display_open(Display_Type_ANY, &params);
 *  //Display_open(Display_Type_UART, NULL);
 *  //Display_open(Display_Type_UART | Display_Type_LCD, &params);
 *  @endcode
 *
 *  ## Outputting data #
 *
 *  After the Display interface is opened, the user can call any of the APIs
 *  @code
 *  Display_clear(handle);
 *  Display_clearLines(handle, 0, 4);
 *  Display_printf(handle, 1, 0, "How are you %s?", yourName);
 *
 *  // Note that for floating point support, the .cfg file must have a line like
 *  // System.extendedFormats = "%$L%$S%$F%f"; // Add '%f' support
 *  Display_printf(handle, 3, 0, "Pi is %f", 3.1415);
 *  Display_close(handle);
 *  @endcode
 *
 *  It is also possible to prefer a graphical display if available. Perhaps
 *  most interesting for portable applications.
 *
 *  @code
 *  Display_Handle    handle;
 *  Display_Params    params;
 *
 *  Display_Params_init(&params);
 *  handle = Display_open(Display_Type_LCD, &params);
 *  if (NULL == handle)
 *     handle = Display_open(Display_Type_UART, &params);
 *  @endcode

 *
 *  @note Display_printX can only be called from Task context, unless
 *        the only opened interfaces specifically allow calls from any context.
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS applications. Its
 *  purpose is to redirect the module's APIs to specific implementations
 *  which are specified using a pointer to a Display_FxnTable.
 *
 *  The Display interface module is joined (at link time) to a NULL-terminated
 *  array of Display_Config data structures named *Display_config*.
 *  *Display_config* is implemented in the application with each entry being an
 *  instance of a Display implementation. Each entry in *Display_config*
 *  contains a:
 *  - (Display_FxnTable *) to a set of functions implementing an output channel
 *  - (void *) data object that is associated with the Display_FxnTable
 *  - (void *) hardware attributes that are associated to the Display_FxnTable
 *
 *  In practical terms that means the Display_Config struct for each specific
 *  implementation equates to an instance of an abstract Display 'class', where
 *  the functions are always provided the a Display_Config struct pointer in the
 *  parameter list, in lieu of what in some languages is called *this* or *self*.
 *
 *  For example, Display_open is called, and Display.c iterates over the
 *  available Display_Config's in the external Display_config array.
 *  For each *config*, config->fxns->open is called, and &config is provided for
 *  context.
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
 *  # Calling Context #
 *
 *  For most implementations of this interface it is only possible to call the
 *  APIs from Task or Main context because the implementations may rely on
 *  Semaphores or other RTOS services that do not work correctly or do not work
 *  at all in Hwi / Swi context.
 *
 *  Unless you know that the implementation is safe to use in Hwi/Swi, do not
 *  use this interface in other contexts than Main and Task.
 *
 *  ============================================================================
 */
#ifndef ti_display_Display__include
#define ti_display_Display__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

/* -----------------------------------------------------------------------------
 *                                          Constants
 * ------------------------------------------------------------------------------
 */
/**
 *  @defgroup DISPLAY_TYPES Meta-types of display implementations
 *  @{
 */
/*!
 * Display types that can be used when opening the display interface, to open
 * only specific types of interfaces. Can be combined with bitwise-OR.
 */
#define Display_Type_ANY      0xFFFFFFFF   /*!< All available displays       */
#define Display_Type_INVALID  0x00000000   /*!< Invalid type                 */
#define Display_Type_LCD      0x80000000   /*!< All LCD displays             */
#define Display_Type_UART     0x40000000   /*!< All UART virtual displays    */
#define Display_Type_LOG      0x20000000   /*!< All xdc.runtime.Log displays */
#define Display_Type_ITM      0x10000000   /*!< All ARM ITM displays         */
#define Display_Type_HOST     0x08000000   /*!< All displays using Host      */
#define Display_Type_ANSI     0x04000000   /*!< All displays accepting ANSI  */
#define Display_Type_GRLIB    0x00100000   /*!< All displays using GrLib     */
/** @}*/

/*! Max allowed displays. Any index value above this is a meta-type index. */
#define Display_MAXINDEX    15

/**
 *  @defgroup DISPLAY_CONTROL Display_control command and status codes
 *  These Displaymacros are reservations for Display.h
 *  @{
 */

/*!
 * Common Display_control command code reservation offset.
 * Display driver implementations should offset command codes with
 * DISPLAY_CMD_RESERVED growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define DISPLAYXYZ_CMD_COMMAND0     DISPLAY_CMD_RESERVED + 0
 * #define DISPLAYXYZ_CMD_COMMAND1     DISPLAY_CMD_RESERVED + 1
 * @endcode
 */
#define DISPLAY_CMD_RESERVED            32

/*!
 * Common Display_control status code reservation offset.
 * Display driver implementations should offset status codes with
 * DISPLAY_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define DISPLAYXYZ_STATUS_ERROR0    DISPLAY_STATUS_RESERVED - 0
 * #define DISPLAYXYZ_STATUS_ERROR1    DISPLAY_STATUS_RESERVED - 1
 * #define DISPLAYXYZ_STATUS_ERROR2    DISPLAY_STATUS_RESERVED - 2
 * @endcode
 */
#define DISPLAY_STATUS_RESERVED        -32

/**
 *  @defgroup DISPLAY_STATUS Status Codes
 *  DISPLAY_STATUS_* macros are general status codes returned by Display_control()
 *  @{
 *  @ingroup DISPLAY_CONTROL
 */

/*!
 * @brief   Successful status code returned by Display_control().
 *
 * Display_control() returns DISPLAY_STATUS_SUCCESS if the control code was
 * executed successfully.
 */
#define DISPLAY_STATUS_SUCCESS         0

/*!
 * @brief   Generic error status code returned by Display_control().
 *
 * Display_control() returns DISPLAY_STATUS_ERROR if the control code was not
 * executed successfully.
 */
#define DISPLAY_STATUS_ERROR          -1

/*!
 * @brief   An error status code returned by Display_control() for undefined
 * command codes.
 *
 * Display_control() returns DISPLAY_STATUS_UNDEFINEDCMD if the control code is
 * not recognized by the driver implementation.
 */
#define DISPLAY_STATUS_UNDEFINEDCMD   -2
/** @}*/

/**
 *  @defgroup DISPLAY_CMD Command Codes
 *  DISPLAY_CMD* macros are general command codes for Display_control(). Not
 *  all Display driver implementations support these command codes.
 *  @{
 *  @ingroup DISPLAY_CONTROL
 */

/*!
 * @brief Command used by Display_control() to release its transport layer
 *
 * This command will cause Display implementations that are using
 * a potentially shared transport mechanism to close the open handle.
 *
 * Methods requiring this transport will meanwhile be discarded.
 *
 * With this command @b arg is @a don't @a care.
 */
#define DISPLAY_CMD_TRANSPORT_CLOSE  0

/*!
 * @brief Command used by Display_control() to resume control of its transport
 *
 * This command will cause Display implementations that are using
 * a potentially shared transport mechanism to (re-)open the closed handle.
 *
 * With this command @b arg is @a don't @a care.
 */
#define DISPLAY_CMD_TRANSPORT_OPEN   1
/** @}*/

/** @}*/

/* -----------------------------------------------------------------------------
 *                                          Macros
 * ------------------------------------------------------------------------------
 */
#define Display_clearLine(handle, n)    Display_clearLines(handle, n, 0)
#define Display_isIndex(i)              (i <= Display_MAXINDEX)
#define Display_isMetaType(i)           (i > Display_MAXINDEX)

#if !defined(Display_DISABLE_ALL) || (!Display_DISABLE_ALL)

/*! Initializes the Display driver. \see Display_doInit() */
#  define Display_init() \
    Display_doInit()

/*! Open selected display id or the first display of type. \see Display_doOpen() */
#  define Display_open(id, params) \
    Display_doOpen(id, params)

/*! Initialize default parameters. \see Display_doParamsInit() */
#  define Display_Params_init(params) \
    Display_doParamsInit(params)

/*! Clear display \see Display_doClear() */
#  define Display_clear(handle) \
    Display_doClear(handle)

/*! Clear lines in display \see Display_doClearLines() */
#  define Display_clearLines(handle, fromLine, toLine) \
    Display_doClearLines(handle, fromLine, toLine)

/*! Output string fmt with variable arguments to display \see Display_doPrintf() */
#  define Display_printf Display_doPrintf

/*! Output string fmt with 0 arguments to display \see Display_doPrintf() */
#  define Display_print0(handle, line, col, fmt) \
    Display_printf(handle, line, col, fmt)

/*! Output string fmt with 1 argument to display \see Display_doPrintf() */
#  define Display_print1(handle, line, col, fmt, a0) \
    Display_printf(handle, line, col, fmt, a0)

/*! Output string fmt with 2 arguments to display \see Display_doPrintf() */
#  define Display_print2(handle, line, col, fmt, a0, a1) \
    Display_printf(handle, line, col, fmt, a0, a1)

/*! Output string fmt with 3 arguments to display \see Display_doPrintf() */
#  define Display_print3(handle, line, col, fmt, a0, a1, a2) \
    Display_printf(handle, line, col, fmt, a0, a1, a2)

/*! Output string fmt with 4 arguments to display \see Display_doPrintf() */
#  define Display_print4(handle, line, col, fmt, a0, a1, a2, a3) \
    Display_printf(handle, line, col, fmt, a0, a1, a2, a3)

/*! Output string fmt with 5 arguments to display \see Display_doPrintf() */
#  define Display_print5(handle, line, col, fmt, a0, a1, a2, a3, a4) \
    Display_printf(handle, line, col, fmt, a0, a1, a2, a3, a4)

/*! Get type of display */
#  define Display_getType(handle) \
    Display_doGetType(handle)

/*! Control a specific Display driver \see Display_doControl() */
#  define Display_control(handle, cmd, arg) \
    Display_doControl(handle, cmd, arg)

/*! Close display \see Display_doClose() */
#  define Display_close(handle) \
    Display_doClose(handle)
#else
#  define Display_init()
#  define Display_open(id, params)    NULL
#  define Display_Params_init(params)
#  define Display_clear(handle)
#  define Display_clearLines(handle, fromLine, toLine)
#  define Display_printf(handle, line, col, fmt, ...)
#  define Display_print0(handle, line, col, fmt)
#  define Display_print1(handle, line, col, fmt, a0)
#  define Display_print2(handle, line, col, fmt, a0, a1)
#  define Display_print3(handle, line, col, fmt, a0, a1, a2)
#  define Display_print4(handle, line, col, fmt, a0, a1, a2, a3)
#  define Display_print5(handle, line, col, fmt, a0, a1, a2, a3, a4)
#  define Display_getType(handle) Display_Type_INVALID
#  define Display_control(handle, cmd, arg)  NULL
#  define Display_close(handle)
#endif

/* -----------------------------------------------------------------------------
 *                                          Typedefs
 * ------------------------------------------------------------------------------
 */
/*!
 *  @brief      A handle for specific Display implementations.
 */
typedef struct Display_Config * Display_Handle;

/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_initFxn().
 */
typedef void (*Display_initFxn)(Display_Handle handle);

/*!
 *  @brief      How to treat existing elements on a line when writing text.
 */
typedef enum Display_LineClearMode
{
    DISPLAY_CLEAR_NONE = 0,   /*!< Do not clear anything before writing      */
    DISPLAY_CLEAR_LEFT,       /*!< Clear pixels to left of text on the line  */
    DISPLAY_CLEAR_RIGHT,      /*!< Clear pixels to right of text on the line */
    DISPLAY_CLEAR_BOTH        /*!< Clear pixels on both sides of text        */
} Display_LineClearMode;

/*!
 *  @brief Display Parameters
 *
 *  Display parameters are used with the Display_open() call. Default values for
 *  these parameters are set using Display_Params_init().
 *
 *  @sa         Display_Params_init()
 */
typedef struct Display_Params
{
    Display_LineClearMode lineClearMode;  /* Default clear entire line */
} Display_Params;

/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_open().
 */
typedef Display_Handle (*Display_openFxn)(Display_Handle handle,
                                          Display_Params *params);
/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_clear().
 */
typedef void (*Display_clearFxn)(Display_Handle handle);

/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_clearLines().
 */
typedef void (*Display_clearLinesFxn)(Display_Handle handle,
                                      uint8_t fromLine,
                                      uint8_t toLine);

/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_vprintf().
 */
typedef void (*Display_vprintfFxn)(Display_Handle handle,
                                   uint8_t line,
                                   uint8_t column,
                                   char *fmt,
                                   va_list va);

/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_close().
 */
typedef void (*Display_closeFxn)(Display_Handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              Display_control().
 */
typedef int (*Display_controlFxn)(Display_Handle handle,
                                  unsigned int cmd,
                                  void *arg);
/*!
 *  @brief      A function pointer to a specific implementation of
 *              Display_getType().
 */
typedef unsigned int (*Display_getTypeFxn)(void);

/*!
 *  @brief      The definition of a Display function table that contains the
 *              required set of functions to control a specific Display driver
 *              implementation.
 */
typedef struct Display_FxnTable
{
    Display_initFxn       initFxn;
    Display_openFxn       openFxn;
    Display_clearFxn      clearFxn;
    Display_clearLinesFxn clearLinesFxn;
    Display_vprintfFxn    vprintfFxn;
    Display_closeFxn      closeFxn;
    Display_controlFxn    controlFxn;
    Display_getTypeFxn    getTypeFxn;
} Display_FxnTable;

/*!
 *  @brief Display Global configuration
 *
 *  The Display_Config structure contains a set of pointers used to characterize
 *  the Display interface implementation.
 *
 *  This structure needs to be defined before calling Display_open() and it must
 *  not be changed thereafter.
 *
 *  @sa     Display_init()
 */
typedef struct Display_Config
{
    /*! Pointer to a table of driver-specific implementations of Display APIs */
    Display_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void                   *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void const             *hwAttrs;
} Display_Config;


/* -----------------------------------------------------------------------------
 *                                          Functions
 * ------------------------------------------------------------------------------
 */
/*!
 * @brief       Initialize all the selected Display implementations.
 *
 *              The index parameter can be an index in the global Display_config
 *              array, or a meta-type signifying which types of display to open.
 *
 *              The different allowed values in addition to actual index are:
 *                - Display_Type_ANY      open and use the first available display
 *                - Display_Type_HOST     open and use HOST type displays
 *                - Display_Type_LCD      open and use LCD type displays
 *                - Display_Type_UART     open and use UART type displays
 *                - Display_Type_LOG      open and use xdc.runtime.Log type displays
 *                - Display_ITM      open and use ARM ITM output type displays
 *                - Display_Type_GRLIB    open and use GrLib capable displays
 *              It is possible to combine types: Display_Type_UART | Display_Type_LCD
 *
 * @note        Only one display is opened. If the type matches more than one,
 *              only the first one found in the Display_config array is opened.
 *
 * @pre        Display_init() has been called
 *
 * @param       id - Index of Display in Display_config array, or
 *                   alternatively the type(s) of displays to open
 * @param       params - display parameters
 *
 * @return      Display_Handle of opened Display or NULL.
 *
 * @sa          Display_doInit()
 * @sa          Display_doClose()
 */
Display_Handle  Display_doOpen(uint32_t id, Display_Params *params);

/*!
 * @brief       Initializes parameter structure with default parameters.
 *
 * @param       params
 *
 * @return      void
 */
void Display_doParamsInit(Display_Params *params);

/*!
 * @brief       Calls the clear fxn of all opened Display implementations
 *
 * @param       handle - handle of display
 *
 * @return      void
 */
void  Display_doClear(Display_Handle handle);

/*!
 * @brief       Calls the clearLines fxn of all opened Display implementations
 *
 * @param       handle - handle of display
 * @param       fromLine - line index (0 .. )
 * @param       toLine - line index (0 .. )
 *
 * @return      void
 */
void  Display_doClearLines(Display_Handle handle, uint8_t fromLine, uint8_t toLine);

/*!
 * @brief       Calls the output function of all opened Display implementations
 *
 * @param       handle - handle of display
 * @param       line - line index (0..)
 * @param       column - column index (0..)
 * @param       fmt - format string
 * @param       ... - optional arguments
 *
 * @return      void
 */
void  Display_doPrintf(Display_Handle handle, uint8_t line, uint8_t column,
                       char *fmt, ...);

/*!
 * @brief       Closes selected Display implementations
 *
 * @return      void
 */
void  Display_doClose(Display_Handle handle);

/*!
 * @brief       Gets the type of display for the handle.
 * @see         DISPLAY_TYPES
 *
 * @return      uint32_t representing Display_Type_Xxxx
 */
uint32_t  Display_doGetType(Display_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a given
 *          Display_Handle.
 *
 *  Commands for Display_control can originate from Display.h or from implementation
 *  specific Display*.h (_DisplaySharp.h_, _DisplayUart.h_, etc.. ) files.
 *  While commands from Display.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific Display*.h files add
 *  unique driver capabilities but are not API portable across all Display driver
 *  implementations.
 *
 *  Commands supported by Display.h follow a DISPLAY_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by Display*.h follow a DISPLAY*_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref DISPLAY_CMD "Display_control command codes" for command codes.
 *
 *  See @ref DISPLAY_STATUS "Display_control return status codes" for status codes.
 *
 *  @pre    Display_open() has to be called first.
 *
 *  @param  handle      A Display handle returned from Display()
 *
 *  @param  cmd         Display.h or Display*.h commands.
 *
 *  @param  arg         An optional R/W (read/write) command argument
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 */
void  Display_doControl(Display_Handle handle, unsigned int cmd, void *arg);

/*!
 *  @brief  Function to initializes the Display driver
 *
 *  @pre    The Display_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other Display driver APIs.
 */
void Display_doInit(void);

#ifdef __cplusplus
}
#endif

#endif //ti_display_Display__include
