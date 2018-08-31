/**************************************************************************//**
 * @file displayls013b7dh03.h
 * @brief Configuration for the display driver for the Sharp Memory LCD
 *        LS013B7DH03
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




#ifndef _DISPLAY_LS013B7DH03_H_
#define _DISPLAY_LS013B7DH03_H_

#include "emstatus.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/* Display geometry */
#define LS013B7DH03_WIDTH           (128)
#define LS013B7DH03_HEIGHT          (128)


/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

/* Initialization function for the LS013B7DH03 device driver. */
EMSTATUS DISPLAY_Ls013b7dh03Init(void);

#ifdef __cplusplus
}
#endif

/** @endcond */

#endif /* _DISPLAY_LS013B7DH03_H_ */
