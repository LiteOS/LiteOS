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
 * @file     ck_adc.c
 * @brief    CSI Source File for ADC Driver
 * @version  V1.0
 * @date     16. October 2017
 ******************************************************************************/
#include <csi_config.h>
#include <soc.h>
#include <csi_core.h>
#include <drv_adc.h>
#include <ck_adc.h>
#include <drv_gpio.h>
#include <io.h>
#include <string.h>

#define ADC_START_TRY_TIMES    10
#define ADC_WAIT_DATA_VALID_TIMES  10

#define ERR_ADC(errno) (CSI_DRV_ERRNO_ADC_BASE | errno)
#define ADC_NULL_PARAM_CHK(para)                  \
    do {                                        \
        if (para == NULL) {                     \
            return ERR_ADC(DRV_ERROR_PARAMETER);   \
        }                                       \
    } while (0)

typedef struct {
    adc_event_cb_t cb_event;
    uint32_t data_num;
} ck_adc_priv_t;

extern int32_t target_adc_init(uint32_t channel);
extern int32_t target_get_adc_count(void);

static ck_adc_priv_t adc_instance[CONFIG_ADC_NUM];

static const adc_capabilities_t adc_capabilities = {
    .single = 1,
    .continuous = 1,
    .scan = 1,
    .calibration = 0,
    .comparator = 1
};

void ck_adc_irqhandler(int idx)
{
    ck_adc_priv_t *adc_priv = &adc_instance[idx];

    if (HAL_IS_BIT_SET(getreg32((volatile uint32_t *)CK_ADC_IEFG), ADC_CONVERSION_COMLETED_INTERRUPT_BIT)) {
        clear_bit(ADC_CONVERSION_COMLETED_INTERRUPT_BIT, (volatile uint32_t *)CK_ADC_IEFG);
        adc_priv->cb_event(0, ADC_EVENT_CONVERSION_COMPLETE);
    }

    if (HAL_IS_BIT_SET(getreg32((volatile uint32_t *)CK_ADC_IEFG), ADC_DATA_OVERWRITE_INTERRUPT_BIT)) {
        clear_bit(ADC_DATA_OVERWRITE_INTERRUPT_BIT, (volatile uint32_t *)CK_ADC_IEFG);
        adc_priv->cb_event(0, ADC_EVENT_DATA_OVERWRITE);
    }

    if (HAL_IS_BIT_SET(getreg32((volatile uint32_t *)CK_ADC_IEFG), ADC_DATA_COMPARE_RIGHT_INTERRUPT_BIT)) {
        clear_bit(ADC_DATA_COMPARE_RIGHT_INTERRUPT_BIT, (volatile uint32_t *)CK_ADC_IEFG);
        adc_priv->cb_event(0, ADC_EVENT_DATA_COMPARE_VALID);
    }

}

/**
  \brief       get adc instance count.
  \return      adc instance count
*/
int32_t drv_adc_get_instance_count(void)
{
    return target_get_adc_count();
}

/**
  \brief       Initialize adc Interface. 1. Initializes the resources needed for the adc interface 2.registers event callback function.
  \param[in]   idx       adc index.
  \param[in]   cb_event  event call back function \ref adc_event_cb_t
  \return      return adc handle if success
*/
adc_handle_t drv_adc_initialize(int32_t idx, adc_event_cb_t cb_event)
{
    if ((idx < 0) || (idx > (CONFIG_ADC_NUM - 1))) {
        return NULL;
    }

    ck_adc_priv_t *adc_priv = &adc_instance[idx];
    adc_priv->cb_event  = cb_event;

    if (cb_event != NULL) {
        csi_vic_enable_irq(ADC_IRQn);
        set_bit(ADC_CONVERSION_COMLETED_INTERRUPT_BIT, (volatile uint32_t *)CK_ADC_IE);
    }

    return (adc_handle_t)adc_priv;
}

/**
  \brief       De-initialize adc Interface. stops operation and releases the software resources used by the interface
  \param[in]   handle   adc handle to operate.
  \return      error code
*/
int32_t drv_adc_uninitialize(adc_handle_t handle)
{
    ADC_NULL_PARAM_CHK(handle);

    ck_adc_priv_t *adc_priv = (ck_adc_priv_t *)handle;
    adc_priv->cb_event = NULL;

    csi_vic_disable_irq(ADC_IRQn);
    return 0;
}

/**
  \brief       Get driver capabilities.
  \param[in]   idx    adc index.
  \return      \ref adc_capabilities_t
*/
adc_capabilities_t drv_adc_get_capabilities(int32_t idx)
{
    if (idx > (CONFIG_ADC_NUM - 1)) {
        adc_capabilities_t ret;
        memset(&ret, 0, sizeof(adc_capabilities_t));
        return ret;
    }

    return adc_capabilities;
}

/**
  \brief       config adc mode.
  \param[in]   handle     adc handle to operate.
  \param[in]   config   adc_conf_t\ref  . pointer to adc configuration structure.
  \return      error code
*/
int32_t drv_adc_config(adc_handle_t handle, adc_conf_t *config)
{
    ADC_NULL_PARAM_CHK(handle);
    ADC_NULL_PARAM_CHK(config);
    ADC_NULL_PARAM_CHK(config->channel_array);
    ADC_NULL_PARAM_CHK(config->channel_nbr);
    ADC_NULL_PARAM_CHK(config->conv_cnt);

    if ((config->trigger != 0) && (config->trigger != 1)) {
        return -ADC_PARAM_INVALID;
    }

    if ((config->intrp_mode != 0) && (config->intrp_mode != 1)) {
        return -ADC_PARAM_INVALID;
    }

    if (config->channel_nbr > (CK_ADC_CH15 + 1)) {
        return -ADC_PARAM_INVALID;
    }

    uint32_t *ch_arr = config->channel_array;
    int i = 0;

    ck_adc_priv_t *adc_priv = (ck_adc_priv_t *)handle;

    if (config->mode == ADC_SINGLE) {
        if (config->channel_nbr != 1) {
            return -ADC_CHANNEL_ERROR;
        }

        if (config->conv_cnt != 1) {
            return -ADC_PARAM_INVALID;
        }

        ch_arr = config->channel_array;

        if (*ch_arr > CK_ADC_CH15) {
            return -ADC_CHANNEL_ERROR;
        }

        if (target_adc_init(*ch_arr) < 0) {
            return -ADC_CHANNEL_ERROR;
        }

        adc_priv->data_num = 1;
        clear_bit(CK_ADC_CR_CMS_2_BIT, (volatile uint32_t *)CK_ADC_CR);
        clear_bit(CK_ADC_CR_CMS_1_BIT, (volatile uint32_t *)CK_ADC_CR);
        write_bits(4, CK_ADC_CR_AOIC_BASE_BIT, (volatile uint32_t *)CK_ADC_CR, *config->channel_array);
    } else if (config->mode == ADC_CONTINUOUS) {
        if (config->conv_cnt > CK_ADC_CONTINOUS_MODE_MAX_CONVERT_TIMES || config->conv_cnt == 0) {
            return -ADC_CONVERSION_INFO_ERROR;
        }

        if (config->channel_nbr != 1) {
            return -ADC_CONVERSION_INFO_ERROR;
        }

        ch_arr = config->channel_array;

        if ((*ch_arr > CK_ADC_CH15)) {
            return -ADC_CHANNEL_ERROR;
        }

        if (*ch_arr > CK_ADC_CH15) {
            return -ADC_CHANNEL_ERROR;
        }

        if (target_adc_init(*ch_arr) < 0) {
            return -ADC_CHANNEL_ERROR;
        }

        adc_priv->data_num = config->conv_cnt;
        set_bit(CK_ADC_CR_CMS_2_BIT, (volatile uint32_t *)CK_ADC_CR);
        clear_bit(CK_ADC_CR_CMS_1_BIT, (volatile uint32_t *)CK_ADC_CR);
        write_bits(4, CK_ADC_CR_SEQC_BASE_BIT, (volatile uint32_t *)CK_ADC_CR, config->conv_cnt - 1);
        write_bits(4, CK_ADC_CR_AOIC_BASE_BIT, (volatile uint32_t *)CK_ADC_CR, *ch_arr);
    } else if (config->mode == ADC_SCAN) {
        if (config->conv_cnt != CK_ADC_SCAN_MODE_MAX_CONVERT_TIMES) {
            return -ADC_CONVERSION_INFO_ERROR;
        }

        if (config->channel_nbr > (CK_ADC_CH15 + 1)) {
            return -ADC_CHANNEL_ERROR;
        }

        for (i = 0; i < config->channel_nbr; i++) {
            if ((*ch_arr > CK_ADC_CH15)) {
                return -ADC_CHANNEL_ERROR;
            }

            if (i > 0) {
                if ((*ch_arr) != ((*(ch_arr - 1)) + 1)) {
                    return -ADC_CHANNEL_ERROR;
                }
            }

            ch_arr ++;
        }

        ch_arr = config->channel_array;

        for (i = 0; i < config->channel_nbr; i++) {
            if (target_adc_init(*ch_arr) < 0) {
                return -ADC_CHANNEL_ERROR;
            }

            ch_arr ++;
        }

        adc_priv->data_num = config->channel_nbr;
        clear_bit(CK_ADC_CR_CMS_2_BIT, (volatile uint32_t *)CK_ADC_CR);
        set_bit(CK_ADC_CR_CMS_1_BIT, (volatile uint32_t *)CK_ADC_CR);
        write_bits(4, CK_ADC_CR_AOIC_BASE_BIT, (volatile uint32_t *)CK_ADC_CR, i);
    } else {
        return -ADC_MODE_ERROR;
    }

    return 0;
}

int32_t drv_adc_comparator_config(adc_handle_t handle, adc_cmp_conf_t *config)
{
    ADC_NULL_PARAM_CHK(handle);
    ADC_NULL_PARAM_CHK(config);

    if (config->cmp_channel > CK_ADC_CH15) {
        return -ADC_CHANNEL_ERROR;
    }

    set_bit(CK_ADC_CMPR_CMPEN_BIT, (volatile uint32_t *)CK_ADC_CMPR);
    write_bits(1, CK_ADC_CMPR_CMPCOND_BIT, (volatile uint32_t *)CK_ADC_CMPR, config->cmp_condition);
    write_bits(4, CK_ADC_CMPR_CMPCH_BASE_BIT, (volatile uint32_t *)CK_ADC_CMPR, config->cmp_channel);
    write_bits(4, CK_ADC_CMPR_CMPD_BASE_BIT, (volatile uint32_t *)CK_ADC_CMPR, config->cmp_data);
    write_bits(4, CK_ADC_CMPR_CMPMATCNT_BASE_BIT, (volatile uint32_t *)CK_ADC_CMPR, config->cmp_match_cnt);

    set_bit(ADC_DATA_COMPARE_RIGHT_INTERRUPT_BIT, (volatile uint32_t *)CK_ADC_IE);

    return 0;
}

/**
  \brief       start adc.
  \param[in]   handle adc handle to operate.
  \return      error code
*/
int32_t drv_adc_start(adc_handle_t handle)
{
    ADC_NULL_PARAM_CHK(handle);

    uint32_t i = 0;
    clear_bit(CK_ADC_CR_ADEN_BIT, (volatile uint32_t *)CK_ADC_CR);

    while (!read_bits(1, CK_ADC_CR_ADEN_BIT, (volatile uint32_t *)CK_ADC_CR) && (i < ADC_START_TRY_TIMES)) {
        set_bit(CK_ADC_STC_ADSTC_BIT, (volatile uint32_t *)CK_ADC_STC);
        i ++;
    }

    set_bit(CK_ADC_STC_ADSTC_BIT, (volatile uint32_t *)CK_ADC_STC);
    return 0;
}

/**
  \brief       stop adc.
  \param[in]   handle adc handle to operate.
  \return      error code
*/
int32_t drv_adc_stop(adc_handle_t handle)
{
    ADC_NULL_PARAM_CHK(handle);

    clear_bit(CK_ADC_CR_ADEN_BIT, (volatile uint32_t *)CK_ADC_CR);
    return 0;
}

/**
  \brief       receiving data from ADC receiver.
  \param[in]   handle ADC handle to operate.
  \param[out]  data  Pointer to buffer for data to receive from ADC receiver.
  \param[in]   num   Number of data items to receive.
  \return      error code
*/
int32_t drv_adc_read(adc_handle_t handle, uint32_t *data, uint32_t num)
{
    ADC_NULL_PARAM_CHK(handle);
    ADC_NULL_PARAM_CHK(data);

    ck_adc_priv_t *adc_priv = (ck_adc_priv_t *)handle;

    uint32_t read_num = 0;
    uint32_t *pbuf = data;
    uint32_t get_mode = 0;
    uint32_t real_mode = 0;

    get_mode =  read_bits(2, CK_ADC_CR_CMS_1_BIT, (volatile uint32_t *)CK_ADC_CR);
    real_mode = read_bits(2, CK_ADC_CR_CMS_1_BIT, (volatile uint32_t *)CK_ADC_CR);

    if (real_mode == CK_ADC_SINGLE_MODE) {
        get_mode = ADC_SINGLE;
    } else if (real_mode == CK_ADC_SCAN_MODE) {
        get_mode = ADC_SCAN;
    } else if (real_mode == CK_ADC_CONTINUOUS) {
        get_mode = ADC_CONTINUOUS;
    } else {
        return -ADC_MODE_ERROR;
    }

    set_bit(CK_ADC_STC_ADSTC_BIT, (volatile uint32_t *)CK_ADC_STC);

    if (num != adc_priv->data_num) {
        return -ADC_PARAM_INVALID;
    }

    if (get_mode == ADC_SINGLE) {
        uint32_t i = 0;

        while (i < ADC_WAIT_DATA_VALID_TIMES) {
            if ((read_bits(1, CK_ADC_SR_VALID_BIT, (volatile uint32_t *)CK_ADC_SR) == 1)) {
                break;
            }
        }

        *pbuf = read_bits(CK_ADC_DATA_WIDTH, CK_ADC_DR_DATA_BASE_BIT, (volatile uint32_t *)CK_ADC_DR);
        return SUCCESS;
    } else if (get_mode == ADC_CONTINUOUS) {
        while (read_num < adc_priv->data_num && (read_bits(1, CK_ADC_SR_VALID_BIT, (volatile uint32_t *)CK_ADC_SR) == 1)) {
            if (read_num < adc_priv->data_num) {
                *pbuf ++ = read_bits(CK_ADC_DATA_WIDTH, CK_ADC_DR_DATA_BASE_BIT, (volatile uint32_t *)CK_ADC_DR);
                read_num ++;
            }
        }

        if (read_num == adc_priv->data_num) {
            return SUCCESS;
        } else if (read_num < adc_priv->data_num) {
            return -ADC_DATA_LOST;
        } else {
            return -ADC_DATA_OVERFLOW;
        }
    } else if (get_mode == ADC_SCAN) {
        while (read_num < adc_priv->data_num && (read_bits(1, CK_ADC_SR_VALID_BIT, (volatile uint32_t *)CK_ADC_SR) == 1)) {
            if (read_num < adc_priv->data_num) {
                *pbuf ++ = read_bits(CK_ADC_DATA_WIDTH, CK_ADC_DR_DATA_BASE_BIT, (volatile uint32_t *)CK_ADC_DR);
                read_num ++;
            }
        }

        if (read_num  == adc_priv->data_num) {
            return SUCCESS;
        } else if (read_num < adc_priv->data_num) {
            return -ADC_DATA_LOST;
        } else {
            return -ADC_DATA_OVERFLOW;
        }
    } else {
        return -ADC_MODE_ERROR;
    }

    return -ADC_MODE_ERROR;
}

/**
  \brief       Get ADC status.
  \param[in]   handle   adc handle to operate.
  \return      ADC status \ref adc_status_t
*/
adc_status_t drv_adc_get_status(adc_handle_t handle)
{
    adc_status_t adc_status = {0};

    if (handle == NULL) {
        return adc_status;
    }

    uint32_t get_stat = read_bits(1, CK_ADC_SR_BUSY_BIT, (volatile uint32_t *)CK_ADC_SR);

    if (get_stat == 0) {
        adc_status.complete = 1;
    } else if (get_stat == 1) {
        adc_status.busy = 1;
    }

    return adc_status;
}
