/**
 * Copyright (c) 2017 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include "sdk_config.h"
#if POWER_ENABLED

#include "nrf_drv_power.h"
#include "nrf_assert.h"
#include "nordic_common.h"

/* Validate configuration */
INTERRUPT_PRIORITY_VALIDATION(POWER_CONFIG_IRQ_PRIORITY);

/**
 * @internal
 * @defgroup nrf_drv_power_internals POWER driver internals
 * @ingroup nrf_drv_power
 *
 * Internal variables, auxiliary macros and functions of POWER driver.
 * @{
 */

/**
 * @brief Default configuration
 *
 * The structure with default configuration data.
 * This structure would be used if configuration pointer given
 * to the @ref nrf_drv_power_init is set to NULL.
 */
static const nrf_drv_power_config_t m_drv_power_config_default =
{
    .dcdcen = POWER_CONFIG_DEFAULT_DCDCEN,
#if NRF_POWER_HAS_VDDH
    .dcdcenhv = POWER_CONFIG_DEFAULT_DCDCENHV,
#endif
};

/**
 * @brief The initialization flag
 */
static bool m_initialized;

/**
 * @brief The handler of power fail comparator warning event
 */
static nrf_drv_power_pofwarn_event_handler_t m_pofwarn_handler;

#if NRF_POWER_HAS_SLEEPEVT
/**
 * @brief The handler of sleep event handler
 */
static nrf_drv_power_sleep_event_handler_t m_sleepevt_handler;
#endif

#if NRF_POWER_HAS_USBREG
/**
 * @brief The handler of USB power events
 */
static nrf_drv_power_usb_event_handler_t m_usbevt_handler;
#endif

/** @} */

bool nrf_drv_power_init_check(void)
{
    return m_initialized;
}

ret_code_t nrf_drv_power_init(nrf_drv_power_config_t const * p_config)
{
    nrf_drv_power_config_t const * p_used_config;
    if(m_initialized)
    {
        return NRF_ERROR_MODULE_ALREADY_INITIALIZED;
    }

    p_used_config = (p_config != NULL) ?
        p_config : (&m_drv_power_config_default);
    nrf_power_dcdcen_set(p_used_config->dcdcen);
#if NRF_POWER_HAS_VDDH
    nrf_power_dcdcen_vddh_set(p_used_config->dcdcenhv);
#endif

    nrf_drv_common_power_clock_irq_init();

    m_initialized = true;
    return NRF_SUCCESS;
}

void nrf_drv_power_uninit(void)
{
    ASSERT(m_initialized);
    nrf_drv_power_pof_uninit();
#if NRF_POWER_HAS_SLEEPEVT
    nrf_drv_power_sleepevt_uninit();
#endif
#if NRF_POWER_HAS_USBREG
    nrf_drv_power_usbevt_uninit();
#endif
    m_initialized = false;
}

void nrf_drv_power_pof_init(nrf_drv_power_pofwarn_config_t const * p_config)
{
    ASSERT(p_config != NULL);

    nrf_drv_power_pof_uninit();
    nrf_power_pofcon_set(true, p_config->thr);
#if NRF_POWER_HAS_VDDH
    nrf_power_pofcon_vddh_set(p_config->thrvddh);
#endif
    if(p_config->handler != NULL)
    {
        m_pofwarn_handler = p_config->handler;
        nrf_power_int_enable(NRF_POWER_INT_POFWARN_MASK);
    }
}

void nrf_drv_power_pof_uninit(void)
{
    nrf_power_int_disable(NRF_POWER_INT_POFWARN_MASK);
    m_pofwarn_handler = NULL;
}

#if NRF_POWER_HAS_SLEEPEVT
void nrf_drv_power_sleepevt_init(nrf_drv_power_sleepevt_config_t const * p_config)
{
    ASSERT(p_config != NULL);

    nrf_drv_power_sleepevt_uninit();
    if(p_config->handler != NULL)
    {
        uint32_t enmask = 0;
        m_sleepevt_handler = p_config->handler;
        if(p_config->en_enter)
        {
            enmask |= NRF_POWER_INT_SLEEPENTER_MASK;
        }
        if(p_config->en_exit)
        {
            enmask |= NRF_POWER_INT_SLEEPEXIT_MASK;
        }
        nrf_power_int_enable(enmask);
    }
}

void nrf_drv_power_sleepevt_uninit(void)
{
    nrf_power_int_disable(
        NRF_POWER_INT_SLEEPENTER_MASK |
        NRF_POWER_INT_SLEEPEXIT_MASK);
    m_sleepevt_handler = NULL;
}
#endif /* NRF_POWER_HAS_SLEEPEVT */

#if NRF_POWER_HAS_USBREG
void nrf_drv_power_usbevt_init(nrf_drv_power_usbevt_config_t const * p_config)
{
    nrf_drv_power_usbevt_uninit();
    if(p_config->handler != NULL)
    {
        m_usbevt_handler = p_config->handler;
        nrf_power_int_enable(
            NRF_POWER_INT_USBDETECTED_MASK |
            NRF_POWER_INT_USBREMOVED_MASK  |
            NRF_POWER_INT_USBPWRRDY_MASK);
    }
}

void nrf_drv_power_usbevt_uninit(void)
{
    nrf_power_int_disable(
        NRF_POWER_INT_USBDETECTED_MASK |
        NRF_POWER_INT_USBREMOVED_MASK  |
        NRF_POWER_INT_USBPWRRDY_MASK);
    m_usbevt_handler = NULL;
}
#endif /* NRF_POWER_HAS_USBREG */


/**
 * @ingroup nrf_drv_power_internals
 * @brief Interrupt handler
 *
 * POWER peripheral interrupt handler
 */
#if NRF_DRV_COMMON_POWER_CLOCK_ISR
void nrf_drv_power_onIRQ(void)
#else
void POWER_POWER_IRQHandler(void)
#endif
{
    uint32_t enabled = nrf_power_int_enable_get();
    if((0 != (enabled & NRF_POWER_INT_POFWARN_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_POFWARN))
    {
        ASSERT(m_pofwarn_handler != NULL); /* Cannot be null if event is enabled */
        m_pofwarn_handler();
    }
#if NRF_POWER_HAS_SLEEPEVT
    if((0 != (enabled & NRF_POWER_INT_SLEEPENTER_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_SLEEPENTER))
    {
        ASSERT(m_sleepevt_handler != NULL); /* Cannot be null if event is enabled */
        m_sleepevt_handler(NRF_DRV_POWER_SLEEP_EVT_ENTER);
    }
    if((0 != (enabled & NRF_POWER_INT_SLEEPEXIT_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_SLEEPEXIT))
    {
        ASSERT(m_sleepevt_handler != NULL); /* Cannot be null if event is enabled */
        m_sleepevt_handler(NRF_DRV_POWER_SLEEP_EVT_EXIT);
    }
#endif
#if NRF_POWER_HAS_USBREG
    if((0 != (enabled & NRF_POWER_INT_USBDETECTED_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_USBDETECTED))
    {
        ASSERT(m_usbevt_handler != NULL); /* Cannot be null if event is enabled */
        m_usbevt_handler(NRF_DRV_POWER_USB_EVT_DETECTED);
    }
    if((0 != (enabled & NRF_POWER_INT_USBREMOVED_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_USBREMOVED))
    {
        ASSERT(m_usbevt_handler != NULL); /* Cannot be null if event is enabled */
        m_usbevt_handler(NRF_DRV_POWER_USB_EVT_REMOVED);
    }
    if((0 != (enabled & NRF_POWER_INT_USBPWRRDY_MASK)) &&
        nrf_power_event_get_and_clear(NRF_POWER_EVENT_USBPWRRDY))
    {
        ASSERT(m_usbevt_handler != NULL); /* Cannot be null if event is enabled */
        m_usbevt_handler(NRF_DRV_POWER_USB_EVT_READY);
    }
#endif
}

#endif /* POWER_ENABLED */
