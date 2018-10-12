/**************************************************************************//***
 * @file
 * @brief Retarget stdout to TFT
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
#ifndef __RETARGETTFT_H
#define __RETARGETTFT_H

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

void RETARGET_TftInit(void);
void RETARGET_TftCrLf(int on);

int  RETARGET_ReadChar(void);
int  RETARGET_WriteChar(char c);

#if defined(__CROSSWORKS_ARM)
int __putchar(int ch);
int __getchar(void);
#endif

#ifdef __cplusplus
}
#endif

/** @} (end group RetargetIo) */
/** @} (end group Drivers) */

#endif
