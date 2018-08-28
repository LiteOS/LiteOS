/**************************************************************************//**
 * @file
 * @brief VDD Voltage Check, using Voltage Comparator API
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


#include <stdint.h>
#include <stdbool.h>
#include "em_cmu.h"
#include "em_vcmp.h"
#include "vddcheck.h"

/**************************************************************************//**
 * @brief VCMP initialization routine
 *****************************************************************************/
void VDDCHECK_Init(void)
{
  /* Enable LE peripherals */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Enable VCMP clock */
  CMU_ClockEnable(cmuClock_VCMP, true);
}

/**************************************************************************//**
 * @brief VCMP deinitialization routine
 *****************************************************************************/
void VDDCHECK_Disable(void)
{
  /* Disable VCMP */
  VCMP_Disable();

  /* Disable clock to VCMP */
  CMU_ClockEnable(cmuClock_VCMP, false);
}

/**************************************************************************//**
 * @brief Check if voltage is higher than indicated
 *
 * @param vdd
 *        The voltage level to compare against.
 * @return
 *        Returns true if voltage is lower, false otherwise
 *****************************************************************************/
bool VDDCHECK_LowVoltage(float vdd)
{
  VCMP_Init_TypeDef vcmp = VCMP_INIT_DEFAULT;

  /* Configure VCMP */
  vcmp.triggerLevel = VCMP_VoltageToLevel(vdd);
  vcmp.warmup       = vcmpWarmTime128Cycles;
  vcmp.lowPowerRef  = false;
  vcmp.enable       = true;

  VCMP_Init(&vcmp);

  /* Delay until warm up ready */
  while (!VCMP_Ready()) ;

  /* If zero result, voltage is lower */
  if (VCMP_VDDHigher()) return false;

  /* Otherwise return false */
  return true;
}
