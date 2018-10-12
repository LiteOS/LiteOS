/**************************************************************************//**
 * @file
 * @brief helper functions for configuring SWO
 * @version 5.0.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_cmu.h"
#include "bsp_trace.h"

/**
 * @brief
 *   Output a single character on SWO.
 *
 * @detail
 *   This function will output a character on SWO using ITM channel 0.
 */
int RETARGET_WriteChar(char c)
{
  return ITM_SendChar(c);
}

/**
 * @brief
 *   This function returns 0 always
 *
 * @details
 *   SWO is a one-way link, it is not possible to read data from SWO.
 */
int RETARGET_ReadChar(void)
{
  return 0;
}

/**
 * @brief
 *   Setup SWO for output
 */
void RETARGET_SwoInit(void)
{
  BSP_TraceSwoSetup();
}
