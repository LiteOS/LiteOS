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
 * @file     devices.c
 * @brief    source file for the devices
 * @version  V1.0
 * @date     24. August 2017
 ******************************************************************************/
#include "soc.h"
#include <csi_config.h>
#include <drv_usart.h>
#include <drv_timer.h>
#include <drv_rtc.h>
#include <drv_trng.h>
#include <drv_crc.h>

#include <drv_rsa.h>
#include <drv_eflash.h>
#include <drv_spi.h>
#include <drv_adc.h>
#include <drv_gpio.h>
#include <drv_aes.h>
#include <stdio.h>

#include "pin_name.h"
#include "pinmux.h"

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

#define writel(b,addr) (void)((*(volatile unsigned int *) (addr)) = (b))


struct {
    uint32_t base;
    uint32_t irq;
    uint32_t pin_num;
    port_name_e port;
}
const sg_gpio_config[CONFIG_GPIO_NUM] = {
    {CSKY_GPIO0_BASE, GPIOA_IRQn, 28, PORTA},
    {CSKY_GPIO1_BASE, GPIOB_IRQn, 4, PORTB},
};

typedef struct {
    int32_t    gpio_pin;
    uint32_t cfg_idx;    //idx of sg_gpio_config[]
} gpio_pin_map_t;
const static gpio_pin_map_t s_gpio_pin_map[] = {
    {PA0, 0},
    {PA1, 0},
    {PA2, 0},
    {PA3, 0},
    {PA4, 0},
    {PA5, 0},

    {PB0, 1},
    {PB1, 1},
    {PB2, 1},
    {PB3, 1},

    {PA6, 0},
    {PA7, 0},
    {PA8, 0},
    {PA9, 0},
    {PA10, 0},
    {PA11, 0},
    {PA12, 0},
    {PA13, 0},
    {PA14, 0},
    {PA15, 0},
    {PA16, 0},
    {PA17, 0},
    {PA18, 0},
    {PA19, 0},
    {PA20, 0},
    {PA21, 0},
    {PA22, 0},
    {PA23, 0},
    {PA24, 0},
    {PA25, 0},
    {PA26, 0},
    {PA27, 0}
};

int32_t target_gpio_port_init(port_name_e port, uint32_t *base, uint32_t *irq, uint32_t *pin_num)
{
    int i;

    for (i = 0; i < CONFIG_GPIO_NUM; i++) {
        if (sg_gpio_config[i].port == port) {
            *base = sg_gpio_config[i].base;
            *irq = sg_gpio_config[i].irq;
            *pin_num = sg_gpio_config[i].pin_num;
            return i;
        }
    }

    return -1;
}
/**
  \param[in]   instance idx, must not exceed return value of target_get_gpio_count()
  \brief       get gpio instance.
  \return      pointer to gpio instance
*/
int32_t target_gpio_pin_init(int32_t gpio_pin, uint32_t *port_idx)
{
    uint32_t idx;

    for (idx = 0; idx < sizeof(s_gpio_pin_map) / sizeof(gpio_pin_map_t); idx++) {
        if (s_gpio_pin_map[idx].gpio_pin == gpio_pin) {
            *port_idx = s_gpio_pin_map[idx].cfg_idx;

            /*pinmux*/
            if (idx >= 10) {
                return idx - 4;
            } else if (idx >= 6) {
                return idx - 6;
            } else {
                return idx;
            }
        }
    }

    return -1;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_timer_config[CONFIG_TIMER_NUM] = {
    {CSKY_TIM0_BASE, TIMA0_IRQn},
    {CSKY_TIM0_BASE + 0x14, TIMA1_IRQn},
    {CSKY_TIM1_BASE, TIMB0_IRQn},
    {CSKY_TIM1_BASE + 0x14, TIMB1_IRQn}

};

int32_t target_get_timer_count(void)
{
    return CONFIG_TIMER_NUM;
}

int32_t target_get_timer(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_timer_count()) {
        return NULL;
    }

    *base = sg_timer_config[idx].base;
    *irq = sg_timer_config[idx].irq;
    return idx;
}


#undef  CSKY_PMU_BASE
#define CSKY_PMU_BASE  0x40002000
#define BIT1 (0x1)
struct {
    uint32_t base;
    uint32_t irq;
}
const sg_rtc_config[CONFIG_RTC_NUM] = {
    {CSKY_RTC0_BASE, RTC_IRQn},
    {CSKY_RTC1_BASE, RTC1_IRQn}

};

int32_t target_get_rtc_count(void)
{
    return CONFIG_RTC_NUM;
}

int32_t target_get_rtc(int32_t idx, uint32_t *base, uint32_t *irq)
{
    unsigned int value;

    if (idx >= target_get_rtc_count()) {
        return NULL;
    }

    value  = readl(CSKY_PMU_BASE);
    value &= ~BIT1;
    writel(value, CSKY_PMU_BASE);

    *base = sg_rtc_config[idx].base;
    *irq = sg_rtc_config[idx].irq;
    return idx;
}

struct {
    uint32_t base;
}
const sg_trng_config[CONFIG_TRNG_NUM] = {
    {CSKY_TRNG_BASE}
};

/**
  \param[in]   instance idx
  \brief       get trng instance.
  \return      pointer to trng instance
*/
int32_t target_get_trng(int32_t idx, uint32_t *base)
{
    *base = sg_trng_config[idx].base;
    return idx;
}

struct {
    uint32_t base;
}
const sg_crc_config[CONFIG_CRC_NUM] = {
    {CSKY_CRC_BASE}
};

/**
  \brief       get crc instance count.
  \return      crc instance count
*/
int32_t target_get_crc_count(void)
{
    return CONFIG_CRC_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_crc_count()
  \brief       get crc instance.
  \return      pointer to crc instance
*/
int32_t target_get_crc(int32_t idx, uint32_t *base)
{
    if (idx >= target_get_crc_count()) {
        return NULL;
    }

    *base = sg_crc_config[idx].base;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_usart_config[CONFIG_USART_NUM] = {
    {CSKY_UART0_BASE, UART0_IRQn},
    {CSKY_UART1_BASE, UART1_IRQn},
    {CSKY_UART2_BASE, UART2_IRQn},
};
typedef struct {
    int32_t    tx;
    int32_t    rx;
    int32_t    cts;
    int32_t    rts;
    uint16_t cfg_idx;    //idx of sg_usart_config[]
    uint16_t function;
} usart_pin_map_t;
const static usart_pin_map_t s_usart_pin_map[] = {
    {
        PA10,
        PA11,
        -1,
        -1,
        0,
        2
    },
    {
        PA17,
        PA16,
        -1,
        -1,
        1,
        0
    },
    {
        PA24,
        PA25,
        PA26,
        PA27,
        2,
        0
    },
};

/**
  \param[in]   instance idx, must not exceed return value of target_get_usart_count()
  \brief       get usart instance.
  \return      pointer to usart instance
*/
int32_t target_usart_init(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= sizeof(s_usart_pin_map) / sizeof(usart_pin_map_t)) {
        return -1;
    }

    *base = sg_usart_config[s_usart_pin_map[idx].cfg_idx].base;
    *irq = sg_usart_config[s_usart_pin_map[idx].cfg_idx].irq;
    /*pinmux*/
    return s_usart_pin_map[idx].cfg_idx;


}
/**
  \brief       control usart flow.
  \param[in]   tx_flow  The TX flow pin name
  \param[in]   rx_flow  The RX flow pin name
  \param[in]   flag 0-disable, 1-enable.
  \return      0 if setting ready ,negative for error code
*/
int32_t target_usart_flowctrl_init(int32_t idx, uint32_t flag)
{
    if (idx >= sizeof(s_usart_pin_map) / sizeof(usart_pin_map_t)) {
        return -1;
    }

    if (flag) {
        drv_pinmux_config(s_usart_pin_map[idx].rts, s_usart_pin_map[idx].function);
        drv_pinmux_config(s_usart_pin_map[idx].cts, s_usart_pin_map[idx].function);
    } else if (flag == 0) {
        drv_pinmux_config(s_usart_pin_map[idx].cts, 0xff);
        drv_pinmux_config(s_usart_pin_map[idx].rts, 0xff);
    } else {
        return -1;
    }

    return 0;
}


struct {
    uint32_t base;
    uint32_t irq;
}

const sg_spi_config[CONFIG_SPI_NUM] = {
    {CSKY_SPI0_BASE, SPI0_IRQn},
    {CSKY_SPI1_BASE, SPI1_IRQn}
};
typedef struct {
    int32_t    mosi;
    int32_t    miso;
    int32_t    sclk;
    int32_t    ssel;	//SSEL是一个从机选择脚，低电平有效
    uint32_t cfg_idx;    //idx of sg_iic_config[]
    uint16_t function;
} spi_pin_map_t;
const static spi_pin_map_t s_spi_pin_map[] = {
    {
        PB3,
        PA6,
        PB2,
        PA7,
        0,
        0
    },
    {
        PA22,
        PA23,
        PA21,
        PA18,
        1,
        0
    }
};

/**
  \param[in]   instance idx, must not exceed return value of target_get_spi_count()
  \brief       get spi instance.
  \return      pointer to spi instance
*/
int32_t target_spi_init(int32_t idx, uint32_t *base, uint32_t *irq, uint32_t *ssel)
{
    if (idx >= sizeof(s_spi_pin_map) / sizeof(spi_pin_map_t)) {
        return -1;
    }

    *base = sg_spi_config[s_spi_pin_map[idx].cfg_idx].base;
    *irq = sg_spi_config[s_spi_pin_map[idx].cfg_idx].irq;
    *ssel = s_spi_pin_map[idx].ssel;
    return s_spi_pin_map[idx].cfg_idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_aes_config[CONFIG_AES_NUM] = {
    {CSKY_AES_BASE, AES_IRQn}
};

/**
  \brief       get aes instance count.
  \return      aes instance count
*/
int32_t target_get_aes_count(void)
{
    return CONFIG_AES_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_aes_count()
  \brief       get aes instance.
  \return      pointer to aes instance
*/
int32_t target_get_aes(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_aes_count()) {
        return NULL;
    }

    *base = sg_aes_config[idx].base;
    *irq = sg_aes_config[idx].irq;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_rsa_config[CONFIG_RSA_NUM] = {
    {CSKY_RSA_BASE, RSA_IRQn}
};

/**
  \brief       get rsa instance count.
  \return      rsa instance count
*/
int32_t target_get_rsa_count(void)
{
    return CONFIG_RSA_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_rsa_count()
  \brief       get rsa instance.
  \return      pointer to rsa instance
*/
int32_t target_get_rsa(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_rsa_count()) {
        return NULL;
    }

    *base = sg_rsa_config[idx].base;
    *irq = sg_rsa_config[idx].irq;
    return idx;
}


struct {
    uint32_t base;
    eflash_info_t info;
}
const sg_eflash_config[CONFIG_EFLASH_NUM] = {
    {CSKY_EFLASH_CONTROL_BASE, {0x10000000, 0x1003f800, 0x1fc}}
};

/**
  \brief       get eflash instance count.
  \return      eflash instance count
*/
int32_t target_get_eflash_count(void)
{
    return CONFIG_EFLASH_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_eflash_count()
  \brief       get eflash instance.
  \return      pointer to eflash instance
*/
int32_t target_get_eflash(int32_t idx, uint32_t *base, eflash_info_t *info)
{
    if (idx >= target_get_eflash_count()) {
        return NULL;
    }

    *base = sg_eflash_config[idx].base;
    info->start = sg_eflash_config[idx].info.start;
    info->end = sg_eflash_config[idx].info.end;
    info->sector_count = sg_eflash_config[idx].info.sector_count;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_wdt_config[CONFIG_WDT_NUM] = {
    {CSKY_WDT_BASE, WDT_IRQn}
};

int32_t target_get_wdt_count(void)
{
    return CONFIG_WDT_NUM;
}

int32_t target_get_wdt(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_wdt_count()) {
        return NULL;
    }

    *base = sg_wdt_config[idx].base;
    *irq = sg_wdt_config[idx].irq;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_dmac_config[CONFIG_DMAC_NUM] = {
    {CSKY_DMAC0_BASE, SEU_DMAC_IRQn},
    {CSKY_DMAC1_BASE, NONSEU_DMAC_IRQn}
};

int32_t target_get_dmac_count(void)
{
    return CONFIG_DMAC_NUM;
}

int32_t target_get_dmac(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_dmac_count()) {
        return NULL;
    }

    *base = sg_dmac_config[idx].base;
    *irq = sg_dmac_config[idx].irq;
    return idx;
}


struct {
    uint32_t base;
    uint32_t irq;
}
const sg_iic_config[CONFIG_IIC_NUM] = {
    {CSKY_I2C0_BASE, I2C0_IRQn},
    {CSKY_I2C1_BASE, I2C1_IRQn}
};


typedef struct {
    int32_t    scl;
    int32_t    sda;
    uint16_t cfg_idx;    //idx of sg_iic_config[]
    uint16_t function;
} iic_pin_map_t;
const static iic_pin_map_t s_iic_pin_map[] = {
    {
        PA6,
        PA7,
        0,
        2
    },
    {
        PC0,
        PC1,
        1,
        0
    }
};


/**
  \param[in]   instance idx, must not exceed return value of target_get_iic_count()
  \brief       get iic instance.
  \return      pointer to iic instance
*/
int32_t target_iic_init(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= sizeof(s_iic_pin_map) / sizeof(iic_pin_map_t)) {
        return -1;
    }

    *base = sg_iic_config[s_iic_pin_map[idx].cfg_idx].base;
    *irq = sg_iic_config[s_iic_pin_map[idx].cfg_idx].irq;
    return s_iic_pin_map[idx].cfg_idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_pwm_config[CONFIG_PWM_NUM] = {
    {CSKY_PWM_BASE, PWM_IRQn},
};

typedef struct {
    int32_t    pwm_pin;
    uint32_t cfg_idx;    //idx of sg_pwm_config[]
    uint32_t ch_num;
    uint16_t function;
} pwm_pin_map_t;
const static pwm_pin_map_t s_pwm_pin_map[] = {
    {PA4, 0, 0, 1},
    {PA5, 0, 1, 1},
    {PB0, 0, 2, 1},
    {PB1, 0, 3, 1},

    {PB2, 0, 4, 1},
    {PB3, 0, 5, 1},
    {PA6, 0, 6, 1},
    {PA7, 0, 7, 1},

    {PA12, 0, 8, 0},
    {PA13, 0, 9, 0},
    {PA14, 0, 10, 0},
    {PA15, 0, 11, 0},

    {PA22, 0, 0, 1},
    {PA23, 0, 1, 1},
    {PC0, 0, 10, 2},
    {PC1, 0, 11, 2}

};

/**
  \param[in]   instance idx, must not exceed return value of target_get_pwm_count()
  \brief       get pwm instance.
  \return      pointer to pwm instance
*/
int32_t target_pwm_init(int32_t pwm_pin, uint32_t *ch_num, uint32_t *base, uint32_t *irq)
{
    uint32_t idx;

    for (idx = 0; idx < sizeof(s_pwm_pin_map) / sizeof(pwm_pin_map_t); idx++) {
        if (s_pwm_pin_map[idx].pwm_pin == pwm_pin) {
            *base = sg_pwm_config[s_pwm_pin_map[idx].cfg_idx].base;
            *irq = sg_pwm_config[s_pwm_pin_map[idx].cfg_idx].irq;
            *ch_num = s_pwm_pin_map[idx].ch_num;
            return s_pwm_pin_map[idx].cfg_idx;
        }
    }

    return -1;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_sha_config[CONFIG_SHA_NUM] = {
    {CSKY_SHA_BASE, SHA_IRQn}
};

/**
  \brief       get sha instance count.
  \return      sha instance count
*/
int32_t target_get_sha_count(void)
{
    return CONFIG_SHA_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_sha_count()
  \brief       get sha instance.
  \return      pointer to sha instance
*/
int32_t target_get_sha(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_sha_count()) {
        return NULL;
    }

    *base = sg_sha_config[idx].base;
    *irq = sg_sha_config[idx].irq;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_adc_config[CONFIG_ADC_NUM] = {
    {CSKY_ADC_CTL_BASE, ADC_IRQn}
};

/**
  \param[in]   initialize adc channel;
  \brief       get  instance.
  \return      0 - success, (-1) - fail.
*/
int32_t target_adc_init(int32_t channel)
{
    return 0;
}

/**
  \brief       get adc instance count.
  \return      adc instance count
*/
int32_t target_get_adc_count(void)
{
    return CONFIG_ADC_NUM;
}

/**
  \param[in]   instance idx, must not exceed return value of target_get_adc_count()
  \brief       get sha instance.
  \return      pointer to adc instance
*/
int32_t target_get_adc(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= target_get_adc_count()) {
        return NULL;
    }

    *base = sg_adc_config[idx].base;
    *irq = sg_adc_config[idx].irq;
    return idx;
}

struct {
    uint32_t base;
    uint32_t irq;
}
const sg_i2s_config[CONFIG_I2S_NUM] = {
    {CSKY_I2S_BASE, I2S_IRQn},
};

typedef struct {
    int32_t    mclk;
    int32_t    sclk;
    int32_t    wsclk;
    int32_t    sda;
    uint16_t cfg_idx;    //idx of sg_i2s_config[]
    uint16_t function;
} i2s_pin_map_t;
const static i2s_pin_map_t s_i2s_pin_map[] = {
    {
        PA24,
        PA25,
        PA26,
        PA27,
        0,
        1
    }
};

/**
  \param[in]   instance idx, must not exceed return value of target_get_i2s_count()
  \brief       get i2s instance.
  \return      pointer to i2s instance
*/
int32_t target_i2s_init(int32_t idx, uint32_t *base, uint32_t *irq)
{
    if (idx >= sizeof(s_i2s_pin_map) / sizeof(i2s_pin_map_t)) {
        return -1;
    }

    *base = sg_i2s_config[s_i2s_pin_map[idx].cfg_idx].base;
    *irq = sg_i2s_config[s_i2s_pin_map[idx].cfg_idx].irq;
    /*pinmux*/
    return s_i2s_pin_map[idx].cfg_idx;
}

