/***************************************************************************//**
 * @file displayconfigall.h
 * @brief Main configuration file for the DISPLAY driver software stack.
 * @version 5.0.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/



#ifndef __DISPLAYCONFIGALL_H
#define __DISPLAYCONFIGALL_H

/*
 * First, we list the default INCLUDE_XXX #defines which may be excluded later
 * by the kit or application specific configuration files.
 */
#define INCLUDE_PAL_GPIO_PIN_AUTO_TOGGLE

/* Then include the kit specific display configuration files which also includes
   the application specific configuration file and further selects which modules
   modules to include below. */
#include "displaypalconfig.h"
#include "displayconfig.h"

#ifdef INCLUDE_DISPLAY_SHARP_LS013B7DH03
#include "displayls013b7dh03config.h"
#endif

#ifdef INCLUDE_TEXTDISPLAY_SUPPORT
#include "textdisplayconfig.h"
#include "retargettextdisplayconfig.h"
#endif

#endif /* __DISPLAYCONFIGALL_H */
