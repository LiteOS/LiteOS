/***************************************************************************//**
 * @file
 * @brief Provide SWO/ETM TRACE configuration parameters.
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

#ifndef __TRACECONFIG_H
#define __TRACECONFIG_H

#define BSP_TRACE_SWO_LOCATION GPIO_ROUTE_SWLOCATION_LOC1

/* Enable output on pin - GPIO Port C, Pin 15. */
#define TRACE_ENABLE_PINS()                         \
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK); \
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL

#endif
