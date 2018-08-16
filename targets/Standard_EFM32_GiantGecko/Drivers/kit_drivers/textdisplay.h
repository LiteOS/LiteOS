/**************************************************************************//**
 * @file textdisplay.h
 * @brief Text display interface
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/



#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_

#include <stdbool.h>
#include "emstatus.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Textdisplay
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/** EMSTATUS codes of the textdisplay module. */
#define TEXTDISPLAY_EMSTATUS_OK                                              (0) /**< Operation successful. */
#define TEXTDISPLAY_EMSTATUS_INVALID_PARAM       (TEXTDISPLAY_EMSTATUS_BASE | 1) /**< Invalid parameter. */
#define TEXTDISPLAY_EMSTATUS_OUT_OF_RANGE        (TEXTDISPLAY_EMSTATUS_BASE | 2) /**< Parameters out of range. */
#define TEXTDISPLAY_EMSTATUS_ALREADY_INITIALIZED (TEXTDISPLAY_EMSTATUS_BASE | 3) /**< Device is already initialized. */
#define TEXTDISPLAY_EMSTATUS_NOT_SUPPORTED       (TEXTDISPLAY_EMSTATUS_BASE | 4) /**< Feature/option not supported. */
#define TEXTDISPLAY_EMSTATUS_NOT_ENOUGH_MEMORY   (TEXTDISPLAY_EMSTATUS_BASE | 5) /**< Not enough memory. */
#define TEXTDISPLAY_EMSTATUS_NOT_INITIALIZED     (TEXTDISPLAY_EMSTATUS_BASE | 6) /**< Device is not initialized. */


/** VT52/VT100 Escape Sequence codes supported by TEXTDISPLAY: */
#ifdef INCLUDE_VIDEO_TERMINAL_ESCAPE_SEQUENCE_SUPPORT
#define TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100       "\033[H" /**< Move cursor to upper left corner. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT52         "\033H" /**< Move cursor to upper left corner. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE       "\033A" /**< Move cursor up one line. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE     "\033B" /**< Move cursor down one line. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_RIGHT_ONE_CHAR    "\033C" /**< Move cursor right one column. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_LEFT_ONE_CHAR     "\033D" /**< Move cursor left one column. */
#endif


/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** Handle for text display device. */
typedef void*  TEXTDISPLAY_Handle_t;

/** Initialization data structure for text display devices. */
typedef struct TEXTDISPLAY_Config_t
{
  int   displayDeviceNo;    /**< Display device number to initialize test
                                 display on. */
  bool  scrollEnable;       /**< Enable/disable scrolling mode on the text
                                 display. Scrolling mode will consume more
                                 memory because the lines will have to be
                                 stored in memory. */

  bool  lfToCrLf;           /**< Enable or disable LF to CR+LF conversion. */
} TEXTDISPLAY_Config_t;


/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

EMSTATUS TEXTDISPLAY_New         (TEXTDISPLAY_Config_t  *config,
                                  TEXTDISPLAY_Handle_t  *handle);
EMSTATUS TEXTDISPLAY_Delete      (TEXTDISPLAY_Handle_t  handle);
EMSTATUS TEXTDISPLAY_WriteChar   (TEXTDISPLAY_Handle_t  handle,
                                  char                  c);
EMSTATUS TEXTDISPLAY_WriteString (TEXTDISPLAY_Handle_t  handle,
                                  const char*           str);
EMSTATUS TEXTDISPLAY_LfToCrLf    (TEXTDISPLAY_Handle_t  handle,
                                  bool                  on);

#ifdef __cplusplus
}
#endif

/** @} (end group TEXTDISPLAY) */
/** @} (end group Drivers) */

#endif /* _TEXTDISPLAY_H_ */
