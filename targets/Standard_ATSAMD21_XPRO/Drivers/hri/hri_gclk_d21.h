/**
 * \file
 *
 * \brief SAM GCLK
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

#ifdef _SAMD21_GCLK_COMPONENT_
#ifndef _HRI_GCLK_D21_H_INCLUDED_
#define _HRI_GCLK_D21_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <hal_atomic.h>

#if defined(ENABLE_GCLK_CRITICAL_SECTIONS)
#define GCLK_CRITICAL_SECTION_ENTER() CRITICAL_SECTION_ENTER()
#define GCLK_CRITICAL_SECTION_LEAVE() CRITICAL_SECTION_LEAVE()
#else
#define GCLK_CRITICAL_SECTION_ENTER()
#define GCLK_CRITICAL_SECTION_LEAVE()
#endif

typedef uint16_t hri_gclk_clkctrl_reg_t;
typedef uint32_t hri_gclk_genctrl_reg_t;
typedef uint32_t hri_gclk_gendiv_reg_t;
typedef uint8_t  hri_gclk_ctrl_reg_t;
typedef uint8_t  hri_gclk_status_reg_t;

static inline void hri_gclk_wait_for_sync(const void *const hw)
{
	while (((const Gclk *)hw)->STATUS.bit.SYNCBUSY)
		;
}

static inline bool hri_gclk_is_syncing(const void *const hw)
{
	return ((const Gclk *)hw)->STATUS.bit.SYNCBUSY;
}

static inline bool hri_gclk_get_STATUS_SYNCBUSY_bit(const void *const hw)
{
	return (((Gclk *)hw)->STATUS.reg & GCLK_STATUS_SYNCBUSY) >> GCLK_STATUS_SYNCBUSY_Pos;
}

static inline hri_gclk_status_reg_t hri_gclk_get_STATUS_reg(const void *const hw, hri_gclk_status_reg_t mask)
{
	uint8_t tmp;
	tmp = ((Gclk *)hw)->STATUS.reg;
	tmp &= mask;
	return tmp;
}

static inline hri_gclk_status_reg_t hri_gclk_read_STATUS_reg(const void *const hw)
{
	return ((Gclk *)hw)->STATUS.reg;
}

static inline void hri_gclk_set_CTRL_SWRST_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->CTRL.reg |= GCLK_CTRL_SWRST;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_CTRL_SWRST_bit(const void *const hw)
{
	uint8_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->CTRL.reg;
	tmp = (tmp & GCLK_CTRL_SWRST) >> GCLK_CTRL_SWRST_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_set_CTRL_reg(const void *const hw, hri_gclk_ctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->CTRL.reg |= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_ctrl_reg_t hri_gclk_get_CTRL_reg(const void *const hw, hri_gclk_ctrl_reg_t mask)
{
	uint8_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->CTRL.reg;
	tmp &= mask;
	return tmp;
}

static inline void hri_gclk_write_CTRL_reg(const void *const hw, hri_gclk_ctrl_reg_t data)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->CTRL.reg = data;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CTRL_reg(const void *const hw, hri_gclk_ctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->CTRL.reg &= ~mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CTRL_reg(const void *const hw, hri_gclk_ctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->CTRL.reg ^= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_ctrl_reg_t hri_gclk_read_CTRL_reg(const void *const hw)
{
	hri_gclk_wait_for_sync(hw);
	return ((Gclk *)hw)->CTRL.reg;
}

static inline void hri_gclk_set_CLKCTRL_CLKEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg |= GCLK_CLKCTRL_CLKEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_CLKCTRL_CLKEN_bit(const void *const hw)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_CLKEN) >> GCLK_CLKCTRL_CLKEN_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_CLKCTRL_CLKEN_bit(const void *const hw, bool value)
{
	uint16_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp &= ~GCLK_CLKCTRL_CLKEN;
	tmp |= value << GCLK_CLKCTRL_CLKEN_Pos;
	((Gclk *)hw)->CLKCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CLKCTRL_CLKEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg &= ~GCLK_CLKCTRL_CLKEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CLKCTRL_CLKEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg ^= GCLK_CLKCTRL_CLKEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_CLKCTRL_WRTLOCK_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg |= GCLK_CLKCTRL_WRTLOCK;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_CLKCTRL_WRTLOCK_bit(const void *const hw)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_WRTLOCK) >> GCLK_CLKCTRL_WRTLOCK_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_CLKCTRL_WRTLOCK_bit(const void *const hw, bool value)
{
	uint16_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp &= ~GCLK_CLKCTRL_WRTLOCK;
	tmp |= value << GCLK_CLKCTRL_WRTLOCK_Pos;
	((Gclk *)hw)->CLKCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CLKCTRL_WRTLOCK_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg &= ~GCLK_CLKCTRL_WRTLOCK;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CLKCTRL_WRTLOCK_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg ^= GCLK_CLKCTRL_WRTLOCK;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_CLKCTRL_ID_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg |= GCLK_CLKCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_get_CLKCTRL_ID_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_ID(mask)) >> GCLK_CLKCTRL_ID_Pos;
	return tmp;
}

static inline void hri_gclk_write_CLKCTRL_ID_bf(const void *const hw, hri_gclk_clkctrl_reg_t data)
{
	uint16_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp &= ~GCLK_CLKCTRL_ID_Msk;
	tmp |= GCLK_CLKCTRL_ID(data);
	((Gclk *)hw)->CLKCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CLKCTRL_ID_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg &= ~GCLK_CLKCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CLKCTRL_ID_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg ^= GCLK_CLKCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_read_CLKCTRL_ID_bf(const void *const hw)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_ID_Msk) >> GCLK_CLKCTRL_ID_Pos;
	return tmp;
}

static inline void hri_gclk_set_CLKCTRL_GEN_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg |= GCLK_CLKCTRL_GEN(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_get_CLKCTRL_GEN_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_GEN(mask)) >> GCLK_CLKCTRL_GEN_Pos;
	return tmp;
}

static inline void hri_gclk_write_CLKCTRL_GEN_bf(const void *const hw, hri_gclk_clkctrl_reg_t data)
{
	uint16_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp &= ~GCLK_CLKCTRL_GEN_Msk;
	tmp |= GCLK_CLKCTRL_GEN(data);
	((Gclk *)hw)->CLKCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CLKCTRL_GEN_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg &= ~GCLK_CLKCTRL_GEN(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CLKCTRL_GEN_bf(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg ^= GCLK_CLKCTRL_GEN(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_read_CLKCTRL_GEN_bf(const void *const hw)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp = (tmp & GCLK_CLKCTRL_GEN_Msk) >> GCLK_CLKCTRL_GEN_Pos;
	return tmp;
}

static inline void hri_gclk_set_CLKCTRL_reg(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg |= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_get_CLKCTRL_reg(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	uint16_t tmp;
	tmp = ((Gclk *)hw)->CLKCTRL.reg;
	tmp &= mask;
	return tmp;
}

static inline void hri_gclk_write_CLKCTRL_reg(const void *const hw, hri_gclk_clkctrl_reg_t data)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg = data;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_CLKCTRL_reg(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg &= ~mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_CLKCTRL_reg(const void *const hw, hri_gclk_clkctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->CLKCTRL.reg ^= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_clkctrl_reg_t hri_gclk_read_CLKCTRL_reg(const void *const hw)
{
	return ((Gclk *)hw)->CLKCTRL.reg;
}

static inline void hri_gclk_set_GENCTRL_GENEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_GENEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_GENEN_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_GENEN) >> GCLK_GENCTRL_GENEN_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_GENEN_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_GENEN;
	tmp |= value << GCLK_GENCTRL_GENEN_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_GENEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_GENEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_GENEN_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_GENEN;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_IDC_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_IDC;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_IDC_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_IDC) >> GCLK_GENCTRL_IDC_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_IDC_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_IDC;
	tmp |= value << GCLK_GENCTRL_IDC_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_IDC_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_IDC;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_IDC_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_IDC;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_OOV_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_OOV;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_OOV_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_OOV) >> GCLK_GENCTRL_OOV_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_OOV_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_OOV;
	tmp |= value << GCLK_GENCTRL_OOV_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_OOV_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_OOV;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_OOV_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_OOV;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_OE_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_OE;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_OE_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_OE) >> GCLK_GENCTRL_OE_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_OE_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_OE;
	tmp |= value << GCLK_GENCTRL_OE_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_OE_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_OE;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_OE_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_OE;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_DIVSEL_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_DIVSEL;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_DIVSEL_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_DIVSEL) >> GCLK_GENCTRL_DIVSEL_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_DIVSEL_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_DIVSEL;
	tmp |= value << GCLK_GENCTRL_DIVSEL_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_DIVSEL_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_DIVSEL;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_DIVSEL_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_DIVSEL;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_RUNSTDBY_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_RUNSTDBY;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline bool hri_gclk_get_GENCTRL_RUNSTDBY_bit(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_RUNSTDBY) >> GCLK_GENCTRL_RUNSTDBY_Pos;
	return (bool)tmp;
}

static inline void hri_gclk_write_GENCTRL_RUNSTDBY_bit(const void *const hw, bool value)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_RUNSTDBY;
	tmp |= value << GCLK_GENCTRL_RUNSTDBY_Pos;
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_RUNSTDBY_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_RUNSTDBY;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_RUNSTDBY_bit(const void *const hw)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_RUNSTDBY;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_set_GENCTRL_ID_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_get_GENCTRL_ID_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_ID(mask)) >> GCLK_GENCTRL_ID_Pos;
	return tmp;
}

static inline void hri_gclk_write_GENCTRL_ID_bf(const void *const hw, hri_gclk_genctrl_reg_t data)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_ID_Msk;
	tmp |= GCLK_GENCTRL_ID(data);
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_ID_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_ID_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_read_GENCTRL_ID_bf(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_ID_Msk) >> GCLK_GENCTRL_ID_Pos;
	return tmp;
}

static inline void hri_gclk_set_GENCTRL_SRC_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= GCLK_GENCTRL_SRC(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_get_GENCTRL_SRC_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_SRC(mask)) >> GCLK_GENCTRL_SRC_Pos;
	return tmp;
}

static inline void hri_gclk_write_GENCTRL_SRC_bf(const void *const hw, hri_gclk_genctrl_reg_t data)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= ~GCLK_GENCTRL_SRC_Msk;
	tmp |= GCLK_GENCTRL_SRC(data);
	((Gclk *)hw)->GENCTRL.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_SRC_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~GCLK_GENCTRL_SRC(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_SRC_bf(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= GCLK_GENCTRL_SRC(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_read_GENCTRL_SRC_bf(const void *const hw)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp = (tmp & GCLK_GENCTRL_SRC_Msk) >> GCLK_GENCTRL_SRC_Pos;
	return tmp;
}

static inline void hri_gclk_set_GENCTRL_reg(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg |= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_get_GENCTRL_reg(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	uint32_t tmp;
	hri_gclk_wait_for_sync(hw);
	tmp = ((Gclk *)hw)->GENCTRL.reg;
	tmp &= mask;
	return tmp;
}

static inline void hri_gclk_write_GENCTRL_reg(const void *const hw, hri_gclk_genctrl_reg_t data)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg = data;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENCTRL_reg(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg &= ~mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENCTRL_reg(const void *const hw, hri_gclk_genctrl_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	hri_gclk_wait_for_sync(hw);
	((Gclk *)hw)->GENCTRL.reg ^= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_genctrl_reg_t hri_gclk_read_GENCTRL_reg(const void *const hw)
{
	hri_gclk_wait_for_sync(hw);
	return ((Gclk *)hw)->GENCTRL.reg;
}

static inline void hri_gclk_set_GENDIV_ID_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg |= GCLK_GENDIV_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_get_GENDIV_ID_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	uint32_t tmp;
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp = (tmp & GCLK_GENDIV_ID(mask)) >> GCLK_GENDIV_ID_Pos;
	return tmp;
}

static inline void hri_gclk_write_GENDIV_ID_bf(const void *const hw, hri_gclk_gendiv_reg_t data)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp &= ~GCLK_GENDIV_ID_Msk;
	tmp |= GCLK_GENDIV_ID(data);
	((Gclk *)hw)->GENDIV.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENDIV_ID_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg &= ~GCLK_GENDIV_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENDIV_ID_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg ^= GCLK_GENDIV_ID(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_read_GENDIV_ID_bf(const void *const hw)
{
	uint32_t tmp;
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp = (tmp & GCLK_GENDIV_ID_Msk) >> GCLK_GENDIV_ID_Pos;
	return tmp;
}

static inline void hri_gclk_set_GENDIV_DIV_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg |= GCLK_GENDIV_DIV(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_get_GENDIV_DIV_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	uint32_t tmp;
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp = (tmp & GCLK_GENDIV_DIV(mask)) >> GCLK_GENDIV_DIV_Pos;
	return tmp;
}

static inline void hri_gclk_write_GENDIV_DIV_bf(const void *const hw, hri_gclk_gendiv_reg_t data)
{
	uint32_t tmp;
	GCLK_CRITICAL_SECTION_ENTER();
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp &= ~GCLK_GENDIV_DIV_Msk;
	tmp |= GCLK_GENDIV_DIV(data);
	((Gclk *)hw)->GENDIV.reg = tmp;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENDIV_DIV_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg &= ~GCLK_GENDIV_DIV(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENDIV_DIV_bf(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg ^= GCLK_GENDIV_DIV(mask);
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_read_GENDIV_DIV_bf(const void *const hw)
{
	uint32_t tmp;
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp = (tmp & GCLK_GENDIV_DIV_Msk) >> GCLK_GENDIV_DIV_Pos;
	return tmp;
}

static inline void hri_gclk_set_GENDIV_reg(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg |= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_get_GENDIV_reg(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	uint32_t tmp;
	tmp = ((Gclk *)hw)->GENDIV.reg;
	tmp &= mask;
	return tmp;
}

static inline void hri_gclk_write_GENDIV_reg(const void *const hw, hri_gclk_gendiv_reg_t data)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg = data;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_clear_GENDIV_reg(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg &= ~mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline void hri_gclk_toggle_GENDIV_reg(const void *const hw, hri_gclk_gendiv_reg_t mask)
{
	GCLK_CRITICAL_SECTION_ENTER();
	((Gclk *)hw)->GENDIV.reg ^= mask;
	GCLK_CRITICAL_SECTION_LEAVE();
}

static inline hri_gclk_gendiv_reg_t hri_gclk_read_GENDIV_reg(const void *const hw)
{
	return ((Gclk *)hw)->GENDIV.reg;
}

#ifdef __cplusplus
}
#endif

#endif /* _HRI_GCLK_D21_H_INCLUDED */
#endif /* _SAMD21_GCLK_COMPONENT_ */
