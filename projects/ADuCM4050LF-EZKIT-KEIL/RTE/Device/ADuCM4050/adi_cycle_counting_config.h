/*! *****************************************************************************
 * @file    adi_cycle_counting_config.h
 * @brief   Cycle Counting Framework configuration
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/


#ifndef ADI_CYCLE_COUNTING_CONFIG_H
#define ADI_CYCLE_COUNTING_CONFIG_H

/** @addtogroup CYCLE_COUNTING_Config Static Configuration
 *  @ingroup cyclecount_logging
 *  @{
 */


/************* Cycle Counting Configuration ***************/

/*! Global enable. This must be enabled for any other functionality to work\n
    0u disabled
	1u enabled
*/
#define ADI_CYCLECOUNT_ENABLED                          (0u)

/*! SPI Interrupt Mode ISR Cycle Counting Enabled\n
   0 -  Disables the recording of SPI ISR cycle counting.
   1 -  Enables the recording of SPI ISR cycle counting.
*/
#define ADI_CYCLECOUNT_SPI_ISR_ENABLED                  (0u)


/*! CRC Interrupt Mode ISR Cycle Counting Enabled\n
   0 -  Disables the recording of CRC ISR cycle counting.
   1 -  Enables the recording of CRC ISR cycle counting.
*/
#define ADI_CYCLECOUNT_CRC_ISR_ENABLED                  (0u)


/*! SPORT Interrupt Mode ISR Cycle Counting Enabled\n
   0 -  Disables the recording of SPORT ISR cycle counting.
   1 -  Enables the recording of SPORT ISR cycle counting.
*/
#define ADI_CYCLECOUNT_SPORT_ISR_ENABLED                (0u)

/*! UART Interrupt Mode ISR Cycle Counting Enabled\n
   0 -  Disables the recording of UART ISR cycle counting.
   1 -  Enables the recording of UART ISR cycle counting.
*/
#define ADI_CYCLECOUNT_UART_ISR_ENABLED                 (0u)


/*! A user application may desire/require cycle counting in an application defined API
    or ISR. Set this macro to the number of required.
*/
#define ADI_CYCLECOUNT_NUMBER_USER_DEFINED_APIS         (0u)

/*!
 * Cycle count 'stack' nesting depth. Adjust as needed.
 * This should map to the maximum number of nested interrupts an application might experience.
 */
#define ADI_CYCLECOUNT_STACK_SIZE 10

/**
 *  @}
 */

#endif /* ADI_CYCLE_COUNTING_CONFIG_H */
