/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     dw_timer.c
 * @brief    CSI Source File for timer Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include "drv_timer.h"
#include "dw_timer.h"
#if defined CONFIG_CHIP_SH810 || defined CONFIG_CHIP_SH807 || defined CONFIG_CHIP_SH610 || defined CONFIG_CHIP_SH610M
#include <drv_intc.h>
#endif
#include "soc.h"
#include "csi_core.h"

#define ERR_TIMER(errno) (CSI_DRV_ERRNO_TIMER_BASE | errno)

#define TIMER_NULL_PARAM_CHK(para)                  \
    do {                                        \
        if (para == NULL) {                     \
            return ERR_TIMER(DRV_ERROR_PARAMETER);   \
        }                                       \
    } while (0)

typedef struct {
    uint32_t base;
    uint32_t irq;
    timer_event_cb_t cb_event;
    uint32_t timeout;                  ///< the set time (us)
    uint32_t timeout_flag;
} dw_timer_priv_t;

extern int32_t target_get_timer_count(void);
extern int32_t target_get_timer(int32_t idx, uint32_t *base, uint32_t *irq);

static dw_timer_priv_t timer_instance[CONFIG_TIMER_NUM];
/**
  \brief      Make all the timers in the idle state.
  \param[in]  pointer to timer register base
*/
static void timer_deactive_control(dw_timer_reg_t *addr)
{
    /* stop the corresponding timer */
    addr->TxControl &= ~DW_TIMER_TXCONTROL_ENABLE;
    /* Disable interrupt. */
    addr->TxControl |= DW_TIMER_TXCONTROL_INTMASK;
}

void dw_timer_irqhandler(int idx)
{
	printf("\n\n[info]进入定时器底层中断服务函数\n");
    dw_timer_priv_t *timer_priv = &timer_instance[idx];
    timer_priv->timeout_flag = 1;

    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    addr->TxEOI;
	
    if (timer_priv->cb_event) {
        return timer_priv->cb_event(idx, TIMER_EVENT_TIMEOUT);
    }

}

/**
  \brief       Initialize TIMER Interface. 1. Initializes the resources needed for the TIMER interface 2.registers event callback function
  \param[in]   idx  instance timer index
  \param[in]   cb_event  Pointer to \ref timer_event_cb_t
  \return      pointer to timer instance
*/
timer_handle_t csi_timer_initialize(int32_t idx, timer_event_cb_t cb_event)
{
    if (idx < 0 || idx >= CONFIG_TIMER_NUM) {
        return NULL;
    }

    uint32_t base = 0u;
    uint32_t irq = 0u;

    int32_t real_idx = target_get_timer(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    dw_timer_priv_t *timer_priv = &timer_instance[idx];
    timer_priv->base = base;
    timer_priv->irq  = irq;

    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);
    timer_priv->timeout = DW_TIMER_INIT_DEFAULT_VALUE;

    timer_deactive_control(addr);
    timer_priv->cb_event = cb_event;

    if (cb_event != NULL) {
#if defined CONFIG_CHIP_SH810 || defined CONFIG_CHIP_SH807 || defined CONFIG_CHIP_SH610 || defined CONFIG_CHIP_SH610M
        csi_intc_enable_irq(timer_priv->irq);
#else
        csi_vic_enable_irq(timer_priv->irq);
#endif
    }

    return (timer_handle_t)timer_priv;
}

/**
  \brief       De-initialize TIMER Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle timer handle to operate.
  \return      error code
*/
int32_t csi_timer_uninitialize(timer_handle_t handle)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = (dw_timer_priv_t *)handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    timer_deactive_control(addr);
    timer_priv->cb_event = NULL;

#if defined CONFIG_CHIP_SH810 || defined CONFIG_CHIP_SH807 || defined CONFIG_CHIP_SH610 || defined CONFIG_CHIP_SH610M
    csi_intc_disable_irq(timer_priv->irq);
#else
    csi_vic_disable_irq(timer_priv->irq);
#endif
    return 0;
}

/**
  \brief       config timer mode.
  \param[in]   handle timer handle to operate.
  \param[in]   mode      \ref timer_mode_e
  \return      error code
*/
int32_t csi_timer_config(timer_handle_t handle, timer_mode_e mode)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    switch (mode) {
        case TIMER_MODE_FREE_RUNNING:
            addr->TxControl &= ~DW_TIMER_TXCONTROL_MODE;
            break;

        case TIMER_MODE_RELOAD:
            addr->TxControl |= DW_TIMER_TXCONTROL_MODE;
            break;

        default:
            return ERR_TIMER(DRV_ERROR_PARAMETER);
    }

    return 0;
}

/**
  \brief       Set timer.
  \param[in]   instance  timer instance to operate.
  \param[in]   timeout the timeout value in microseconds(us).
  \return      error code
*/
int32_t csi_timer_set_timeout(timer_handle_t handle, uint32_t timeout)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = handle;
    timer_priv->timeout = timeout;
    return 0;
}

/**
  \brief       Start timer.
  \param[in]   handle timer handle to operate.
  \return      error code
*/
int32_t csi_timer_start(timer_handle_t handle)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = handle;

    timer_priv->timeout_flag = 0;

    uint32_t min_us = LSP_DEFAULT_FREQ / 1000000;
    uint32_t load;

    if (timer_priv->timeout > 0xffffffff / min_us) {
        return ERR_TIMER(DRV_ERROR_PARAMETER);
    }

    if (min_us) {
        load = (uint32_t)(timer_priv->timeout * min_us);
    } else {
        load = (uint32_t)(((timer_priv->timeout) * LSP_DEFAULT_FREQ) / 1000000);
    }

    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    if (timer_priv->timeout == 0) {
        addr->TxLoadCount = 0xffffffff;                           /* load time(us) */
    } else {
        if ((addr->TxControl | 0x2) == 0x2) {
            addr->TxLoadCount = 0xffffffff;                           /* load time(us) */
        } else {
            addr->TxLoadCount = load;                           /* load time(us) */
        }
    }

    addr->TxControl &= ~DW_TIMER_TXCONTROL_ENABLE;      /* disable the timer */
    addr->TxControl |= DW_TIMER_TXCONTROL_ENABLE;       /* enable the corresponding timer */
    addr->TxControl &= ~DW_TIMER_TXCONTROL_INTMASK;     /* enable interrupt */
    return 0;
}

/**
  \brief       Stop timer.
  \param[in]   handle timer handle to operate.
  \return      error code
*/
int32_t csi_timer_stop(timer_handle_t handle)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    addr->TxControl |= DW_TIMER_TXCONTROL_INTMASK;      /* enable interrupt */
    addr->TxControl &= ~DW_TIMER_TXCONTROL_ENABLE;      /* disable the timer */

    return 0;
}

/**
  \brief       suspend timer.
  \param[in]   instance  timer instance to operate.
  \return      error code
*/
int32_t csi_timer_suspend(timer_handle_t handle)
{
    TIMER_NULL_PARAM_CHK(handle);

    return ERR_TIMER(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       resume timer.
  \param[in]   handle timer handle to operate.
  \return      error code
*/
int32_t csi_timer_resume(timer_handle_t handle)
{
    TIMER_NULL_PARAM_CHK(handle);

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    addr->TxControl &= ~DW_TIMER_TXCONTROL_ENABLE;      /* stop the corresponding timer */
    addr->TxControl &= DW_TIMER_TXCONTROL_ENABLE;       /* restart the corresponding timer */

    return 0;
}

/**
  \brief       get timer current value
  \param[in]   handle timer handle to operate.
  \param[out]   value     timer current value
  \return      error code
*/
int32_t csi_timer_get_current_value(timer_handle_t handle, uint32_t *value)
{
    TIMER_NULL_PARAM_CHK(handle);
    TIMER_NULL_PARAM_CHK(value);

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    *value = addr->TxCurrentValue;
    return 0;
}

/**
  \brief       Get TIMER status.
  \param[in]   handle timer handle to operate.
  \return      TIMER status \ref timer_status_t
*/
timer_status_t csi_timer_get_status(timer_handle_t handle)
{
    timer_status_t timer_status = {0};

    if (handle == NULL) {
        return timer_status;
    }

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    if (addr->TxControl & DW_TIMER_TXCONTROL_ENABLE) {
        timer_status.active = 1;
    }

    if (timer_priv->timeout_flag == 1) {
        timer_status.timeout = 1;
    }

    return timer_status;
}

/**
  \brief       get timer reload value
  \param[in]   handle timer handle to operate.
  \param[out]   value    timer reload value
  \return      error code
*/
int32_t csi_timer_get_load_value(timer_handle_t handle, uint32_t *value)
{
    TIMER_NULL_PARAM_CHK(handle);
    TIMER_NULL_PARAM_CHK(value);

    dw_timer_priv_t *timer_priv = handle;
    dw_timer_reg_t *addr = (dw_timer_reg_t *)(timer_priv->base);

    *value = addr->TxLoadCount;
    return 0;
}

