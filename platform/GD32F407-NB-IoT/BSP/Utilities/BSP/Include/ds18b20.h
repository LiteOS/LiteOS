
#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "platform_config.h"

#define DQ_PIN_SET          gpio_bit_set(DQ_GPIO_PORT, DQ_GPIO_PIN)
#define DQ_PIN_RESET        gpio_bit_reset(DQ_GPIO_PORT, DQ_GPIO_PIN)
#define IS_DQ_LOW           !gpio_input_bit_get(DQ_GPIO_PORT, DQ_GPIO_PIN)

void ds18b20_init(void);
int16_t DS18B20_ReadTempReg(void);
int16_t DS18B20_ReadTempByID(uint8_t *_id);
uint8_t DS18B20_ReadID(uint8_t *_id);

#endif
