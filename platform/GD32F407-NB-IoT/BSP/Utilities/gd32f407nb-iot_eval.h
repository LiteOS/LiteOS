/*!
    \file  gd32f407nb-iot_eval.h
    \brief definitions for GD32F450_EVAL's leds, keys and COM ports hardware resources
*/

/*
    Copyright (C) 2016 GigaDevice

    2016-10-19, V1.0.0, firmware for GD32F450Z
*/

#ifndef GD32F407NB_IOT_EVAL_H
#define GD32F407NB_IOT_EVAL_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
/* exported types */
typedef enum 
{
    LED1 = 0,
    LED2 = 1
} led_typedef_enum;

typedef enum 
{
    KEY_USER1 = 0,
    KEY_USER2 = 1,
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* eval board low layer led */
#define LEDn                             2U

#define LED1_PIN                         GPIO_PIN_4
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCU_GPIOD
  
#define LED2_PIN                         GPIO_PIN_5
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCU_GPIOD

#define COMn                             1U
#define EVAL_COM1                        USART2
#define EVAL_COM_CLK                     RCU_USART2

#define EVAL_COM_TX_PIN                  GPIO_PIN_10
#define EVAL_COM_RX_PIN                  GPIO_PIN_11

#define EVAL_COM_GPIO_PORT               GPIOC
#define EVAL_COM_GPIO_CLK                RCU_GPIOC
#define EVAL_COM_AF                      GPIO_AF_7

#define KEYn                             2U

/* user1 push-button */
#define USER1_KEY_PIN                    GPIO_PIN_0
#define USER1_KEY_GPIO_PORT              GPIOA
#define USER1_KEY_GPIO_CLK               RCU_GPIOA
#define USER1_KEY_EXTI_LINE              EXTI_0
#define USER1_KEY_EXTI_PORT_SOURCE       EXTI_SOURCE_GPIOA
#define USER1_KEY_EXTI_PIN_SOURCE        EXTI_SOURCE_PIN0
#define USER1_KEY_EXTI_IRQn              EXTI0_IRQn

/* user2 push-button */
#define USER2_KEY_PIN                    GPIO_PIN_0
#define USER2_KEY_GPIO_PORT              GPIOB
#define USER2_KEY_GPIO_CLK               RCU_GPIOB
#define USER2_KEY_EXTI_LINE              EXTI_0
#define USER2_KEY_EXTI_PORT_SOURCE       EXTI_SOURCE_GPIOB
#define USER2_KEY_EXTI_PIN_SOURCE        EXTI_SOURCE_PIN0
#define USER2_KEY_EXTI_IRQn              EXTI0_IRQn  

/* function declarations */
/* configures led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected button state */
uint8_t gd_eval_key_state_get(key_typedef_enum button);
/* configure COM port */
void gd_eval_com_init(uint32_t com);


#ifdef cplusplus
}
#endif

#endif /* GD32F407NB_IOT_EVAL_H */
