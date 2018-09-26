/**
 * \file
 *
 * \brief SAM Power manager
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <hpl_pm_base.h>
#include <hpl_pm_config.h>
#include <hpl_reset.h>
#include <hpl_sleep.h>
#include <utils_assert.h>

/**
 * \brief Retrieve the reset reason
 */
enum reset_reason _get_reset_reason(void)
{
	return (enum reset_reason)hri_pm_read_RCAUSE_reg(PM);
}

/**
 * \brief Set the sleep mode for the device
 */
int32_t _set_sleep_mode(const uint8_t mode)
{
	switch (mode) {
	case 0:
	case 1:
	case 2:
		SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
		PM->SLEEP.reg = mode;
		return ERR_NONE;
	case 3:
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
		return ERR_NONE;
	default:
		return ERR_INVALID_ARG;
	}
}

/**
 * \brief Power Manager Init
 */
void _pm_init(void)
{
	hri_pm_set_CPUSEL_CPUDIV_bf(PM, CONF_CPU_DIV);
	hri_pm_set_APBASEL_APBADIV_bf(PM, CONF_APBA_DIV);
	hri_pm_set_APBBSEL_APBBDIV_bf(PM, CONF_APBB_DIV);
	hri_pm_set_APBCSEL_APBCDIV_bf(PM, CONF_APBC_DIV);
}
