/**
 * \file
 *
 * \brief SAM PAC
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifdef _SAMD21_PAC_COMPONENT_
#ifndef _HRI_PAC_D21_H_INCLUDED_
#define _HRI_PAC_D21_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <hal_atomic.h>

#if defined(ENABLE_PAC_CRITICAL_SECTIONS)
#define PAC_CRITICAL_SECTION_ENTER() CRITICAL_SECTION_ENTER()
#define PAC_CRITICAL_SECTION_LEAVE() CRITICAL_SECTION_LEAVE()
#else
#define PAC_CRITICAL_SECTION_ENTER()
#define PAC_CRITICAL_SECTION_LEAVE()
#endif

typedef uint32_t hri_pac_wpset_reg_t;

static inline void hri_pac_set_WP_WP_bf(const void *const hw, hri_pac_wpset_reg_t mask)
{
	((Pac *)hw)->WPSET.reg = PAC_WPSET_WP(mask);
}

static inline hri_pac_wpset_reg_t hri_pac_get_WP_WP_bf(const void *const hw, hri_pac_wpset_reg_t mask)
{
	uint32_t tmp;
	tmp = ((Pac *)hw)->WPSET.reg;
	tmp = (tmp & PAC_WPSET_WP(mask)) >> PAC_WPSET_WP_Pos;
	return tmp;
}

static inline hri_pac_wpset_reg_t hri_pac_read_WP_WP_bf(const void *const hw)
{
	uint32_t tmp;
	tmp = ((Pac *)hw)->WPSET.reg;
	tmp = (tmp & PAC_WPSET_WP_Msk) >> PAC_WPSET_WP_Pos;
	return tmp;
}

static inline void hri_pac_write_WP_WP_bf(const void *const hw, hri_pac_wpset_reg_t data)
{
	((Pac *)hw)->WPSET.reg = PAC_WPSET_WP(data);
	((Pac *)hw)->WPCLR.reg = ~PAC_WPSET_WP(data);
}

static inline void hri_pac_clear_WP_WP_bf(const void *const hw, hri_pac_wpset_reg_t mask)
{
	((Pac *)hw)->WPCLR.reg = PAC_WPSET_WP(mask);
}

static inline void hri_pac_set_WP_reg(const void *const hw, hri_pac_wpset_reg_t mask)
{
	((Pac *)hw)->WPSET.reg = mask;
}

static inline hri_pac_wpset_reg_t hri_pac_get_WP_reg(const void *const hw, hri_pac_wpset_reg_t mask)
{
	uint32_t tmp;
	tmp = ((Pac *)hw)->WPSET.reg;
	tmp &= mask;
	return tmp;
}

static inline hri_pac_wpset_reg_t hri_pac_read_WP_reg(const void *const hw)
{
	return ((Pac *)hw)->WPSET.reg;
}

static inline void hri_pac_write_WP_reg(const void *const hw, hri_pac_wpset_reg_t data)
{
	((Pac *)hw)->WPSET.reg = data;
	((Pac *)hw)->WPCLR.reg = ~data;
}

static inline void hri_pac_clear_WP_reg(const void *const hw, hri_pac_wpset_reg_t mask)
{
	((Pac *)hw)->WPCLR.reg = mask;
}

#ifdef __cplusplus
}
#endif

#endif /* _HRI_PAC_D21_H_INCLUDED */
#endif /* _SAMD21_PAC_COMPONENT_ */
