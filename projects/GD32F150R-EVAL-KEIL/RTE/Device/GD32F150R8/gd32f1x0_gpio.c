/*!
    \file  gd32f1x0_gpio.c
    \brief gpio driver
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_gpio.h"

/*!
    \brief      reset GPIO port
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[out] none
    \retval     none
*/
void gpio_deinit(uint32_t port)
{
    if(GPIOA == port){
        rcu_periph_reset_enable(RCU_GPIOARST);
        rcu_periph_reset_disable(RCU_GPIOARST);
    }else if(GPIOB == port){
        rcu_periph_reset_enable(RCU_GPIOBRST);
        rcu_periph_reset_disable(RCU_GPIOBRST);
    }else if(GPIOC == port){
        rcu_periph_reset_enable(RCU_GPIOCRST);
        rcu_periph_reset_disable(RCU_GPIOCRST);
    }else if(GPIOD == port){
        rcu_periph_reset_enable(RCU_GPIODRST);
        rcu_periph_reset_disable(RCU_GPIOCRST);
    }else if(GPIOF == port){
        rcu_periph_reset_enable(RCU_GPIOFRST);
        rcu_periph_reset_disable(RCU_GPIOFRST);
    }
}

/*!
    \brief      set GPIO output mode
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  mode
      \arg        GPIO_MODE_INPUT: input mode
      \arg        GPIO_MODE_OUTPUT: output mode
      \arg        GPIO_MODE_AF: alternate function mode
      \arg        GPIO_MODE_ANALOG: analog mode
    \param[in]  pull_up_down
      \arg        GPIO_PUPD_NONE: without weak pull-up and pull-down resistors
      \arg        GPIO_PUPD_PULLUP: with weak pull-up resistors
      \arg        GPIO_PUPD_PULLDOWN:with weak pull-down resistors
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_mode_set(uint32_t port, uint8_t mode, uint8_t pull_up_down,uint16_t pin)
{
    uint16_t i;
    uint32_t ctl, pupd;

    ctl = GPIO_CTL(port);
    pupd = GPIO_PUD(port);

    for (i = 0; i < 16; i++){
        if (!((1 << i) & pin)){
            continue;
        }
        /* clear the specified pin mode bits */
        ctl &= ~GPIO_MODE_MASK(i);
        ctl |= GPIO_MODE_SET(i, mode);
        /* clear the specified pin pupd bits */
        pupd &= ~GPIO_PUPD_MASK(i);
        pupd |= GPIO_PUPD_SET(i, pull_up_down);
    }

    GPIO_CTL(port) = ctl;
    GPIO_PUD(port) = pupd;
}

/*!
    \brief      set GPIO output type and speed
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  mode
      \arg        GPIO_OTYPE_PP: push pull mode
      \arg        GPIO_OTYPE_OD: open drain mode
    \param[in]  speed
      \arg        GPIO_OSPEED_2MHZ: output max speed 2M 
      \arg        GPIO_OSPEED_10MHZ: output max speed 10M 
      \arg        GPIO_OSPEED_50MHZ: output max speed 50M 
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_output_options_set(uint32_t port, uint8_t otype, uint8_t speed,uint16_t pin)
{
    uint16_t i;
    uint32_t ospeedr;

    if(0x1 == otype){
        GPIO_OMODE(port) |= pin;
    }else{
        GPIO_OMODE(port) &= ~pin;
    }

    /* get the specified pin output speed bits value */
    ospeedr = GPIO_OSPD(port);

    for (i = 0; i < 16; i++){
        if (!((1 << i) & pin)){
            continue;
        }
        /* clear the specified pin output speed bits */
        ospeedr &= ~GPIO_OSPEED_MASK(i);
        ospeedr |= GPIO_OSPEED_SET(i, speed);
    }
    GPIO_OSPD(port) = ospeedr;
}

/*!
    \brief      set GPIO pin
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_set(uint32_t port, uint16_t pin)
{
    GPIO_BOP(port) = pin;
}

/*!
    \brief      reset GPIO pin
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_reset(uint32_t port, uint16_t pin)
{
    GPIO_BC(port) = pin;
}

/*!
    \brief      write data to the specified GPIO pin
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
                  GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[in]  bitvalue
      \arg        RESET: clear the port pin
      \arg        SET: set the port pin
    \param[out] none
    \retval     none
*/
void gpio_bit_write(uint32_t port, uint16_t pin, bit_status bit_value)
{
    if (RESET != bit_value){
        GPIO_BOP(port) = pin;
    }else{
        GPIO_BC(port) = pin;
    }
}

/*!
    \brief      write data to the specified GPIO port
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  data
      \arg        specify the value to be written to the port output data register
    \param[out] none
    \retval     none
*/
void gpio_port_write(uint32_t port, uint16_t data)
{
    GPIO_OCTL(port) = data;
}

/*!
    \brief      get GPIO pin input status
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     input state of gpio pin
*/
FlagStatus gpio_input_bit_get(uint32_t port, uint16_t pin)
{
    if((uint32_t)RESET != (GPIO_ISTAT(port)&(pin))){
        return SET; 
    }else{
        return RESET;
    }
}

/*!
    \brief      get GPIO all pins input status
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[out] none
    \retval     input state of gpio all pins
*/
uint16_t gpio_input_port_get(uint32_t port)
{
    return (GPIO_ISTAT(port));
}

/*!
    \brief      get GPIO pin output status
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     output state of gpio pin
*/
FlagStatus gpio_output_bit_get(uint32_t port, uint16_t pin)
{
    if((uint32_t)RESET !=(GPIO_OCTL(port)&(pin))){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      get GPIO all pins output status
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[out] none
    \retval     output state of gpio all pins
*/
uint16_t gpio_output_port_get(uint32_t port)
{
    return ((uint16_t)GPIO_OCTL(port));
}

/*!
    \brief      set GPIO alternate function
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  alt_func_num
      \arg        GPIO_AF_0: EVENTOUT, TIMER14, SPI0, I2S0, TIMER16,MCO, SWDAT, SWCLK, TIMER14,USART0, CEC, IR_OUT, SPI1 
      \arg        GPIO_AF_1: USART0, USART1, CEC, TIMER2, IR_OUT, EVENTOUT, I2C0, I2C1, TIMER14
      \arg        GPIO_AF_2: TIMER1, TIMER0, EVENTOUT, TIMER15, TIMER16
      \arg        GPIO_AF_3: TSI, I2C0, TIMER14, EVENTOUT
      \arg        GPIO_AF_4: TIMER13, I2C1, I2C0, USART1
      \arg        GPIO_AF_5: TIMER15, TIMER16
      \arg        GPIO_AF_6: EVENTOUT, SPI1 
      \arg        GPIO_AF_7: CMP0_OUT, CMP1_OUT
      \arg        GPIO_AF_8: 
      \arg        GPIO_AF_9: CAN0,CAN1
      \arg        GPIO_AF_10:
      \arg        GPIO_AF_11: SLCD
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none    
    \retval     none
*/
void gpio_af_set(uint32_t port, uint8_t alt_func_num, uint16_t pin)
{
    uint16_t i;
    uint32_t afrl, afrh;

    afrl = GPIO_AFSEL0(port);
    afrh = GPIO_AFSEL1(port);

    for (i = 0; i < 8; i++){
        if (!((1 << i) & pin)){
            continue;
        }
        /* clear the specified pin alternate function bits */
        afrl &= ~GPIO_AFR_MASK(i);
        afrl |= GPIO_AFR_SET(i, alt_func_num);
    }

    for (i = 8; i < 16; i++){
        if (!((1 << i) & pin)){
            continue;
        }
        /* clear the specified pin alternate function bits */
        afrh &= ~GPIO_AFR_MASK(i - 8);
        afrh |= GPIO_AFR_SET(i - 8, alt_func_num);
    }

    GPIO_AFSEL0(port) = afrl;
    GPIO_AFSEL1(port) = afrh;
}

/*!
    \brief      lock GPIO pin
    \param[in]  gpioport
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  gpiopin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_pin_lock(uint32_t port, uint16_t pin)
{
    uint32_t lock = 0x00010000;
    lock |= pin;
    /* lock key writing sequence: write 1->write 0->write 1-> read 0-> read 1 */
    GPIO_LOCK(port) = lock;
    GPIO_LOCK(port) = pin;
    GPIO_LOCK(port) = lock;
    lock = GPIO_LOCK(port);
    lock = GPIO_LOCK(port);
}

#ifdef GD32F170_190
/*!
    \brief      toggle GPIO pin
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[in]  pin
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_bit_toggle(uint32_t port, uint16_t pin)
{
    GPIO_TG(port) = pin;
}

/*!
    \brief      toggle GPIO port
    \param[in]  port
      \arg        GPIOx(x = A,B,C,D,F)
    \param[out] none
    \retval     none
*/
void gpio_port_toggle(uint32_t port, uint16_t pin)
{
    GPIO_TG(port) = 0x0000ffff;
}

#endif /* GD32F170_190 */
