/**************************************************************************//**
 * @file emstatus.h
 * @brief EMSTATUS definitions.
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



#ifndef _EMSTATUS_H_
#define _EMSTATUS_H_

#include <stdint.h>
#include "em_types.h"          /* typedef uint32_t EMSTATUS; */

/** Common EMSTATUS codes: */
#define EMSTATUS_OK                   (0)            /**< Operation successful. */

/** EMSTATUS base codes for display modules. */
#define DISPLAY_EMSTATUS_BASE         (0x8011UL<<16) /**< EMSTATUS base code of DISPLAY driver interface. */
#define TEXTDISPLAY_EMSTATUS_BASE     (0x8012UL<<16) /**< EMSTATUS base code of TEXTDISPLAY module. */
#define PAL_EMSTATUS_BASE             (0x8013UL<<16) /**< EMSTATUS base code of PAL interface. */

#endif  /*  _EMSTATUS_H_  */
