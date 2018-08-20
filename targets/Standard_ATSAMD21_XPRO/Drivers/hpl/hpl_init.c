/**
 * \file
 *
 * \brief HPL initialization related functionality implementation.
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

#include <hpl_gpio.h>
#include <hpl_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_config.h>
#include <hpl_pm_base.h>

#include <hpl_dma.h>
#include <hpl_dmac_config.h>

/* Referenced GCLKs (out of 0~7), should be initialized firstly
 * - GCLK 3 for DFLL48M
 * - GCLK 3 for FDPLL96M
 */
#define _GCLK_INIT_1ST 0x00000008
/* Not referenced GCLKs, initialized last */
#define _GCLK_INIT_LAST 0x000000F7

/**
 * \brief Initialize the hardware abstraction layer
 */
void _init_chip(void)
{
	hri_nvmctrl_set_CTRLB_RWS_bf(NVMCTRL, CONF_NVM_WAIT_STATE);

	_pm_init();
	_sysctrl_init_sources();
#if _GCLK_INIT_1ST
	_gclk_init_generators_by_fref(_GCLK_INIT_1ST);
#endif
	_sysctrl_init_referenced_generators();
	_gclk_init_generators_by_fref(_GCLK_INIT_LAST);

#if CONF_DMAC_ENABLE
	_pm_enable_bus_clock(PM_BUS_AHB, DMAC);
	_pm_enable_bus_clock(PM_BUS_APBB, DMAC);
	_dma_init();
#endif
}
