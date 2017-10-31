/*!
 *****************************************************************************
   @file:    adi_flash_config.h
   @brief:   Configuration options for flash driver.
             This is specific to the flash driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 33205 $
   @date:    $Date: 2016-01-11 05:46:07 -0500 (Mon, 11 Jan 2016) $
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

#ifndef ADI_FLASH_CONFIG_H
#define ADI_FLASH_CONFIG_H
#include <adi_global_config.h>

/** @addtogroup Flash_Driver_Config Static Configuration
 *  @ingroup Flash_Driver
 *  @{
 */


/****SETTINGS THAT LIVE IN FEE INTERRUPT ENABLE (IEN) REGISTER****/


/*!
 * Configure a response to the 2-bit ECC ERROR events (in IEN).
 * - 0 Do not generate a response to ECC Error Events.
 * - 1 Generate Bus Errors in response to ECC Error Events.
 * - 2 Generate IRQs in response to ECC Error Events.
 */
#define ADI_FEE_CFG_ECC_ERROR_RESPONSE          (1u)
/*!
 * Configure a response to the 1-bit ECC CORRECTION events (in IEN).
 * - 0 Do not generate a response to ECC correction Events.
 * - 1 Generate Bus Errors in response to ECC correction Events.
 * - 2 Generate IRQs in response to ECC correction Events.
 */
#define ADI_FEE_CFG_ECC_CORRECTION_RESPONSE     (2u)



/****SETTINGS THAT LIVE IN FEE TIME PARAMETER 0 (TIME_PARAM0) REGISTER****/


/* It is recommended to NOT MODIFY flash timing parameters without keen insight and caution */
/*!
 * Configure flash non-volatile mass erase hold time.\n
 * Upper 4-bits of 11-bit value.\n
 * (Lower bits are hard-coded to 0x14.)\n
 * Hardware default value is 0xb.
 */
#define ADI_FEE_CFG_PARAM0_TNVH1          (0xbu)

/*!
 * Configure flash erase time.\n
 * Upper 4-bits of 19-bit value.\n
 * (Lower bits are hard-coded to 0x7370.)\n
 * Hardware default value is 0x8.
 */
#define ADI_FEE_CFG_PARAM0_TERASE          (0x8u)

/*!
 * Configure flash recovery time.\n
 * Upper 4-bits of 8-bit value.\n
 * (Lower bits are hard-coded to 0x2.)\n
 * Hardware default value is 0x9.
 */
#define ADI_FEE_CFG_PARAM0_TRCV          (0x9u)

/*!
 * Configure flash non-volatile hold time.\n
 * Upper 4-bits of 8-bit value.\n
 * (Lower bits are hard-coded to 0x1.)\n
 * Hardware default value is 0x5.
 */
#define ADI_FEE_CFG_PARAM0_TNVH          (0x5u)

/*!
 * Configure flash program time.\n
 * Upper 4-bits of 10-bit value.\n
 * (Lower bits are hard-coded to 0x7.)\n
 * Hardware default value is 0x0.
 */
#define ADI_FEE_CFG_PARAM0_TPROG          (0x0u)

/*!
 * Configure flash NVSTR-to-program setup time.\n
 * Upper 4-bits of 8-bit value.\n
 * (Lower bits are hard-coded to 0x2.)\n
 * Hardware default value is 0x9.
 */
#define ADI_FEE_CFG_PARAM0_TPGS          (0x9u)

/*!
 * Configure flash program/erase-to-NVSTR setup time.\n
 * Upper 4-bits of 8-bit value.\n
 * (Lower bits are hard-coded to 0x1.)\n
 * Hardware default value is 0x5.
 */
#define ADI_FEE_CFG_PARAM0_TNVS          (0x5u)

/*!
 * Configure flash reference clock divide-by-2 setting.\n
 * All timing parameters are referenced to this parameter.
 * - 0 Reference clock is not divided.
 * - 1 Reference clock is divided by 2.\n
 * Hardware default value is 0x0.
 */
#define ADI_FEE_CFG_PARAM0_CLKDIV          (0x0u)



/****SETTINGS THAT LIVE IN FEE TIME PARAMETER 1 (TIME_PARAM1) REGISTER****/


/* It is recommended to NOT MODIFY flash timing parameters without keen insight and caution */
/*!
 * Configure flash read access wait states.\n
 * Number of 3-bit read access wait states to use.\n
 * Maximum allowed value is 0x4.\n
 * Hardware default value is 0x0.
 */
#define ADI_FEE_CFG_PARAM1_WAITESTATES          (0x0u)

/*!
 * Configure flash sleep mode wake-up time.\n
 * Upper 4-bits of 8-bit value.\n
 * (Lower bits are hard-coded to 0xb.)\n
 * Hardware default value is 0x4.
 */
#define ADI_FEE_CFG_PARAM1_TWK          (0x4u)



/****SETTINGS THAT LIVE IN FEE SYSTEM ABOUT ENABLE (ABOUT_EN_XX) REGISTERS****/


/*!
 * Configure lower (0-31) flash system interrupt abort enables.\n
 * Allows system interrupts to abort an ongoing flash command.\n
 * Only 64 system interrupts are supported.\n
 * Lower interrupts (0-31) are encoded in ADI_FEE_CFG_ABORT_EN_LO,
 * - 0 Corresponding interrupt is prevented from aborting flash command.
 * - 1 Corresponding interrupt is allowed to abort flash command.\n
 * Hardware default value is 0x0.
 */
#define ADI_FEE_CFG_ABORT_EN_LO          (0x0u)

/*!
 * Configure upper (32-63) flash system interrupt abort enables.\n
 * Allows system interrupts to abort an ongoing flash command.\n
 * Only 64 system interrupts are supported.\n
 * Upper interrupts (32-63) are encoded in ADI_FEE_CFG_ABORT_EN_HI.
 * - 0 Corresponding interrupt is prevented from aborting flash command.
 * - 1 Corresponding interrupt is allowed to abort flash command.\n
 * Hardware default value is 0x0.
 */
#define ADI_FEE_CFG_ABORT_EN_HI          (0x0u)



/****SETTINGS THAT LIVE IN ECC CONFIG REGISTER (ECC_CFG) REGISTER****/


/*!
 * ECC Start Page Pointer (in ECC_CFG).
 */
#define ADI_FEE_CFG_ECC_START_PAGE              (0u)

/*!
 * Enable/Disable ECC for info space (in ECC_CFG).
 * - 1 Enable Info Space.
 * - 0 Disable Info Space.
 */
#define ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE   (0u)

/*!
 * Enable/Disable ECC (in ECC_CFG).
 * - 1 Enable ECC.
 * - 0 Disable ECC.
 */
#define ADI_FEE_CFG_ENABLE_ECC                  (0u)



/************* Flash Driver Configuration Settings Checkers ***************/



/* IEN CHECKS */
#if ((ADI_FEE_CFG_ECC_ERROR_RESPONSE < 0u) || (ADI_FEE_CFG_ECC_ERROR_RESPONSE > 2u))
#error "ADI_FEE_CFG_ECC_ERROR_RESPONSE should be in the range 0-2."
#endif
#if ((ADI_FEE_CFG_ECC_CORRECTION_RESPONSE < 0u) || (ADI_FEE_CFG_ECC_CORRECTION_RESPONSE > 2u))
#error "ADI_FEE_CFG_ECC_CORRECTION_RESPONSE should be in the range 0-2."
#endif



/* PARAM0 CHECKS */
#if ((ADI_FEE_CFG_PARAM0_TNVH1 < 0u) || (ADI_FEE_CFG_PARAM0_TNVH1 > 15u))
#error "ADI_FEE_CFG_PARAM0_TNVH1 should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TERASE < 0u) || (ADI_FEE_CFG_PARAM0_TERASE > 15u))
#error "ADI_FEE_CFG_PARAM0_TERASE should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TRCV < 0u) || (ADI_FEE_CFG_PARAM0_TRCV > 15u))
#error "ADI_FEE_CFG_PARAM0_TRCV should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TNVH1 < 0u) || (ADI_FEE_CFG_PARAM0_TNVH1 > 15u))
#error "ADI_FEE_CFG_PARAM0_TNVH1 should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TPROG < 0u) || (ADI_FEE_CFG_PARAM0_TPROG > 15u))
#error "ADI_FEE_CFG_PARAM0_TPROG should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TPGS < 0u) || (ADI_FEE_CFG_PARAM0_TPGS > 15u))
#error "ADI_FEE_CFG_PARAM0_TPGS should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_TNVS < 0u) || (ADI_FEE_CFG_PARAM0_TNVS > 15u))
#error "ADI_FEE_CFG_PARAM0_TNVS should be in the range 0-15."
#endif
#if ((ADI_FEE_CFG_PARAM0_CLKDIV < 0u) || (ADI_FEE_CFG_PARAM0_CLKDIV > 1u))
#error "ADI_FEE_CFG_PARAM0_CLKDIV should be in the range 0-1."
#endif



/* PARAM1 CHECKS */
#if ((ADI_FEE_CFG_PARAM1_WAITESTATES < 0u) || (ADI_FEE_CFG_PARAM1_WAITESTATES > 4u))
#error "ADI_FEE_CFG_PARAM1_WAITESTATES should be in the range 0-4."
#endif
#if ((ADI_FEE_CFG_PARAM1_TWK < 0u) || (ADI_FEE_CFG_PARAM1_TWK > 15u))
#error "ADI_FEE_CFG_PARAM1_TWK should be in the range 0-15."
#endif



/* ABORT_EN_XX CHECKS */
#if ((ADI_FEE_CFG_ABORT_EN_LO < 0u) || (ADI_FEE_CFG_ABORT_EN_LO > 0XFFFFu))
#error "ADI_FEE_CFG_ABORT_EN_LO should be in 32-bit range."
#endif
#if ((ADI_FEE_CFG_ABORT_EN_HI < 0u) || (ADI_FEE_CFG_ABORT_EN_HI > 0XFFFFu))
#error "ADI_FEE_CFG_ABORT_EN_HI should be in 32-bit range."
#endif



/* ECC_CFG CHECKS */
#if (((ADI_FEE_CFG_ECC_START_PAGE >> 8u) << 8) != ADI_FEE_CFG_ECC_START_PAGE)
#error "ADI_FEE_CFG_ECC_START_PAGE has invalid  bits set in lower 8-bits."
#endif
#if ((ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE != 0u) && (ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE != 1u))
#error "ADI_FEE_CFG_ENABLE_ECC_FOR_INFO_SPACE should be 1 or 0."
#endif
#if ((ADI_FEE_CFG_ENABLE_ECC != 0u) && (ADI_FEE_CFG_ENABLE_ECC != 1u))
#error "ADI_FEE_CFG_ENABLE_ECC should be 1 or 0."
#endif

/*! @} */

#endif /* ADI_FLASH_CONFIG_H */
