/**************************************************************************//**
 * @file
 * @brief Retarget stdout to SWO.
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#ifndef RETARGETSWO_H
#define RETARGETSWO_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RetargetIo
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int RETARGET_WriteChar(char c);
int RETARGET_ReadChar(void);
void RETARGET_SwoInit(void);

/**
 * @deprecated Use @ref RETARGET_SwoInit instead
 */
#define setupSWOForPrint RETARGET_SwoInit

#ifdef __cplusplus
}
#endif

/** @} (end group RetargetSWO) */
/** @} (end group Drivers) */

#endif
