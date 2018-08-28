/**************************************************************************//**
 * @file
 * @brief Si114x UV Index measurement
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

#ifndef __SI1146_UV_INDEX_H
#define __SI1146_UV_INDEX_H

int Si114x_MeasureUVIndex(u16 *uvIndex);

int Si114x_ConfigureUV(int fullInit, HANDLE *handle);

int Si114x_EnableVisRange(int vis_range_bit);


#endif
