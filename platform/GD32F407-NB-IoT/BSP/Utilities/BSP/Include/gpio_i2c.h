#ifndef _GPIO_I2C_H_
#define _GPIO_I2C_H_

#include "platform_config.h"

#define I2C_WR       0          /* Ð´¿ØÖÆbit */
#define I2C_RD       1          /* ¶Á¿ØÖÆbit */

void gpio_i2c_init(void);
void gpio_i2c_start(void);
void gpio_i2c_stop(void);
void gpio_i2c_sendbyte(uint8_t _byte);
uint8_t gpio_i2c_readbyte(void);
uint8_t gpio_i2c_waitack(void);
void gpio_i2c_ack(void);
void gpio_i2c_nack(void);
uint8_t gpio_i2c_checkdevice(uint8_t _address);

#endif /* _GPIO_I2C_H_ */
