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
 * @file     dw_wdt.c
 * @brief    CSI Source File for WDT Driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#include <csi_config.h>
#include <stdio.h>
#include "drv_wdt.h"
#include "dw_wdt.h"
#include "soc.h"
#include "csi_core.h"

#define ERR_WDT(errno) (CSI_DRV_ERRNO_WDT_BASE | errno)
#define SYSTEM_CLOCK_MS (SYSTEM_CLOCK / 1000)
uint32_t timeout_ms[16] = {
    (0x10000 << 0) / SYSTEM_CLOCK_MS,
    (0x10000 << 1) / SYSTEM_CLOCK_MS,
    (0x10000 << 2) / SYSTEM_CLOCK_MS,
    (0x10000 << 3) / SYSTEM_CLOCK_MS,
    (0x10000 << 4) / SYSTEM_CLOCK_MS,
    (0x10000 << 5) / SYSTEM_CLOCK_MS,
    (0x10000 << 6) / SYSTEM_CLOCK_MS,
    (0x10000 << 7) / SYSTEM_CLOCK_MS,
    (0x10000 << 8) / SYSTEM_CLOCK_MS,
    (0x10000 << 9) / SYSTEM_CLOCK_MS,
    (0x10000 << 10) / SYSTEM_CLOCK_MS,
    (0x10000 << 11) / SYSTEM_CLOCK_MS,
    (0x10000 << 12) / SYSTEM_CLOCK_MS,
    (0x10000 << 13) / SYSTEM_CLOCK_MS,
    (0x10000 << 14) / SYSTEM_CLOCK_MS,
    ((uint32_t)0x10000 << 15) / SYSTEM_CLOCK_MS
};
#define WDT_NULL_PARAM_CHK(para)                  \
    do {                                    \
        if (para == NULL) {                 \
            return ERR_WDT(DRV_ERROR_PARAMETER);   \
        }                                   \
    } while (0)

typedef struct {
    uint32_t base;
    uint32_t irq;
    wdt_event_cb_t cb_event;
} dw_wdt_priv_t;

extern int32_t target_get_wdt_count(void);
extern int32_t target_get_wdt(int32_t idx, uint32_t *base, uint32_t *irq);

static dw_wdt_priv_t wdt_instance[CONFIG_WDT_NUM];

static inline void dw_wdt_enable(dw_wdt_reg_t *addr)
{
    uint32_t value = addr->WDT_CR;
    value |= 1 << 0;
    addr->WDT_CR = value;
}

static inline void dw_wdt_disable(dw_wdt_reg_t *addr)
{
    uint32_t value = addr->WDT_CR;
    value &= ~(1 << 0);
    addr->WDT_CR = value;
}


void dw_wdt_irqhandler(int32_t idx)
{
    dw_wdt_priv_t *wdt_priv = &wdt_instance[idx];
    dw_wdt_reg_t *addr = (dw_wdt_reg_t *)(wdt_priv->base);
	printf("进入看门狗底层中断函数\n");
    addr->WDT_EOI;

    if (wdt_priv->cb_event) {
        wdt_priv->cb_event(idx, WDT_EVENT_TIMEOUT);
    }
}

/**
  \brief       Initialize WDT Interface. 1. Initializes the resources needed for the WDT interface 2.registers event callback function
  \param[in]   idx   wdt index
  \param[in]   cb_event  Pointer to \ref wdt_event_cb_t
  \return      pointer to wdt instance
*/
wdt_handle_t csi_wdt_initialize(int32_t idx, wdt_event_cb_t cb_event)
{
    if (idx < 0 || idx >= CONFIG_WDT_NUM) {
        return NULL;
    }

    uint32_t base = 0u;
    uint32_t irq = 0u;

    int32_t real_idx = target_get_wdt(idx, &base, &irq);

    if (real_idx != idx) {
        return NULL;
    }

    dw_wdt_priv_t *wdt_priv = &wdt_instance[idx];
    wdt_priv->base = base;
    wdt_priv->irq  = irq;

    wdt_priv->cb_event = cb_event;
    csi_vic_enable_irq(wdt_priv->irq);

    return (wdt_handle_t)wdt_priv;
}

/**
  \brief       De-initialize WDT Interface. stops operation and releases the software resources used by the interface
  \param[in]   instance  wdt instance to operate.
  \return      \ref execution_status
*/
int32_t csi_wdt_uninitialize(wdt_handle_t handle)
{
    WDT_NULL_PARAM_CHK(handle);

    dw_wdt_priv_t *wdt_priv = handle;

    wdt_priv->cb_event = NULL;
    csi_vic_disable_irq(wdt_priv->irq);
    return 0;
}

/**
  \brief       Set the WDT value. value = (2^t*0xffff * 10^6 /freq)/10^3(t: 0 ~ 15).
  \param[in]   handle wdt handle to operate.
  \param[in]   value     the timeout value(ms) \ref:timeout_ms[]
  \return      \ref execution_status
*/
int32_t csi_wdt_set_timeout(wdt_handle_t handle, uint32_t value)
{
    WDT_NULL_PARAM_CHK(handle);
    uint32_t i = 0u;

    for (i = 0; i <= 15 ; i++) {
        if (timeout_ms[i] >= value) {
            break;
        }

        if (i == 15) {
            return ERR_WDT(DRV_ERROR_PARAMETER);
        }
    }

    dw_wdt_priv_t *wdt_priv = handle;
    dw_wdt_reg_t *addr = (dw_wdt_reg_t *)(wdt_priv->base);

    uint32_t config = addr->WDT_CR;
    uint32_t en_stat = 0;   /*origin wdt enable status*/

    if ((config & 0x1) != 0) {
        en_stat = 1;
    }

    config = 0;
    addr->WDT_CR = config;

    /*before configuration, must disable wdt first*/
    dw_wdt_disable(addr);
    i += i << 4;
    addr->WDT_TORR = i;

    if (en_stat == 1) {
        dw_wdt_enable(addr);
        csi_wdt_restart(handle);
    }

    return 0;
}

/**
  \brief       Start the WDT.
  \param[in]   handle wdt handle to operate.
  \return      \ref execution_status
*/
int32_t csi_wdt_start(wdt_handle_t handle)
{
    WDT_NULL_PARAM_CHK(handle);

    dw_wdt_priv_t *wdt_priv = handle;
    dw_wdt_reg_t *addr = (dw_wdt_reg_t *)(wdt_priv->base);

    dw_wdt_enable(addr);
    csi_wdt_restart(handle);
    return 0;
}

/**
  \brief       Stop the WDT.
  \param[in]   handle wdt handle to operate.
  \return      \ref execution_status
*/
int32_t csi_wdt_stop(wdt_handle_t handle)
{
    WDT_NULL_PARAM_CHK(handle);

    return ERR_WDT(DRV_ERROR_UNSUPPORTED);
}

/**
  \brief       Restart the WDT.
  \param[in]   handle wdt handle to operate.
  \return      \ref execution_status
*/
int32_t csi_wdt_restart(wdt_handle_t handle)
{
    WDT_NULL_PARAM_CHK(handle);

    dw_wdt_priv_t *wdt_priv = handle;
    dw_wdt_reg_t *addr = (dw_wdt_reg_t *)(wdt_priv->base);

    addr->WDT_CRR = DW_WDT_CRR_RESET;

    return 0;
}

/**
  \brief       Read the WDT Current value.
  \param[in]   handle wdt handle to operate.
  \param[in]   value     Pointer to the Value.
  \return      \ref execution_status
*/
int32_t csi_wdt_read_current_value(wdt_handle_t handle, uint32_t *value)
{
    WDT_NULL_PARAM_CHK(handle);
    WDT_NULL_PARAM_CHK(value);

    dw_wdt_priv_t *wdt_priv = handle;
    dw_wdt_reg_t *addr = (dw_wdt_reg_t *)(wdt_priv->base);

    *value = addr->WDT_CCVR;
    return 0;
}

