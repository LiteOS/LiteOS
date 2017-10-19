/*!
 *****************************************************************************
   @file:    adi_global_config.h
   @brief:   Configuration options for all the drivers.
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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ADI_GLOBAL_CONFIG_H
#define ADI_GLOBAL_CONFIG_H

/** @addtogroup GLOBAL_Driver_Config Global Static Configuration
 *  @brief   Configuration options for all the drivers.
 *  @{
 */

/*! @name RTOS used
 * In order to be used in a multi-threaded application, the device drivers
 * may require the use of some RTOS-specific signals like semaphores or actions
 * may be required when entering/exiting an interrupt. By specifying the RTOS
 * that the application uses, the drivers can map their requirements to the
 * specific RTOS, without requiring an OS abstraction layer.
 * @note This macros do not add the RTOS sources to the application, users need
 *       to set up the source and include paths in their application themselves
 * @note If the RTOS specified is not in the list of supported RTOS the build
 *       mechanism fails
 */
/**@{*/

/*! @hideinitializer Indicates that no RTOS is used (bare-metal applications) */
#define ADI_CFG_RTOS_NO_OS              (1)
/*! @hideinitializer Indicates that Micrium uCOS-III is used  */
#define ADI_CFG_RTOS_MICRIUM_III        (2)
/*! @hideinitializer Indicates that Micrium FreeRTOS is used */
#define ADI_CFG_RTOS_FREERTOS           (3)

/*! Configure the RTOS required across the project.
    It can be configured to one of the following macros:
    - #ADI_CFG_RTOS_NO_OS
    - #ADI_CFG_RTOS_MICRIUM_III
    - #ADI_CFG_RTOS_FREERTOS
 */
#define ADI_CFG_RTOS    ADI_CFG_RTOS_NO_OS

/**@}*/

/*! @name Low power mode support
    All applications may have to block when a buffer is being processed. In the
    case of an RTOS application, when a task is blocked waiting for a buffer, a
    different task can run. If no tasks are available then the idle task runs.
    In many RTOS the idle task can be configured so it perform actions like
    entering low power modes.

    In the case of a bare-metal (no RTOS) application, since there are no other
    tasks to be run, the driver can enter low power modes itself when it blocks.
   */

/*! Configures the drivers to enter low power mode (Flexi mode)
    when waiting for a buffer to be processed. This macro is applicable
    only when the drivers are operating in the bare metal mode (No RTOS).

    The possible values it can be configured to are:

    - 1 : Low power mode support required.
    - 0 : Low power mode support not required.
*/
#define ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT      (1)
/**@}*/



/*
** Verify the macro configuration
*/
#if ((ADI_CFG_RTOS != ADI_CFG_RTOS_NO_OS)       && \
     (ADI_CFG_RTOS != ADI_CFG_RTOS_MICRIUM_III)  && \
     (ADI_CFG_RTOS != ADI_CFG_RTOS_FREERTOS))
#error "ADI_CFG_RTOS macro wrongly configured"
#endif /* ADI_CFG_RTOS verification */

#if ((ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT != 0) && \
     (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT != 1))
#error "ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT macro is wrongly configured"
#endif

#if ((ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT == 1) && \
     (ADI_CFG_RTOS != ADI_CFG_RTOS_NO_OS))
#error "ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT cannot be set to 1 in multi-threaded applications"
#endif
/**
 *  @}
 */

#endif /* ADI_GLOBAL_CONFIG_H */
