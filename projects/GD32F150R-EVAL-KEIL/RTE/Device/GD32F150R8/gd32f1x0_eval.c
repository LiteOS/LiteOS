 /*!
    \file  gd32f1x0_eval.c
    \brief firmware functions to manage leds, keys, COM ports
*/

/*
    Copyright (C) 2016 GigaDevice

    2014-12-26, V1.0.0, firmware for GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, firmware for GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_eval.h"

/* private variables */
uint32_t GPIO_PORT[LEDn] = {LED1_GPIO_PORT,
                            LED2_GPIO_PORT,
                            LED3_GPIO_PORT,
                            LED4_GPIO_PORT};

uint16_t GPIO_PIN[LEDn] = {LED1_PIN,
                           LED2_PIN, 
                           LED3_PIN, 
                           LED4_PIN};

rcu_periph_enum COM_CLK[COMn] = {EVAL_COM1_CLK,
#ifdef GD32F170_190
                                 EVAL_COM2_CLK
#endif
                                 };

uint32_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN,
#ifdef GD32F170_190
                             EVAL_COM2_TX_PIN
#endif
                             };

uint32_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN,
#ifdef GD32F170_190
                             EVAL_COM2_RX_PIN
#endif
                             };

rcu_periph_enum GPIO_CLK[LEDn] = {LED1_GPIO_CLK,
                                  LED2_GPIO_CLK, 
                                  LED3_GPIO_CLK, 
                                  LED4_GPIO_CLK
                                  };

uint32_t KEY_PORT[KEYn] = {WAKEUP_KEY_GPIO_PORT, 
                           TAMPER_KEY_GPIO_PORT,
#ifdef GD32F130_150                            
                           USER_KEY_GPIO_PORT
#endif
                          };

uint16_t KEY_PIN[KEYn] = {WAKEUP_KEY_PIN, 
                          TAMPER_KEY_PIN,
#ifdef GD32F130_150
                          USER_KEY_PIN
#endif
                         };

rcu_periph_enum KEY_CLK[KEYn] = {WAKEUP_KEY_GPIO_CLK, 
                                 TAMPER_KEY_GPIO_CLK,
#ifdef GD32F130_150
                                 USER_KEY_GPIO_CLK
#endif
                                };

exti_line_enum KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                      TAMPER_KEY_EXTI_LINE,
#ifdef GD32F130_150
                                      USER_KEY_EXTI_LINE
#endif
                                     };

uint16_t KEY_PORT_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                  TAMPER_KEY_EXTI_PORT_SOURCE,
#ifdef GD32F130_150
                                  USER_KEY_EXTI_PORT_SOURCE
#endif
                                 };

uint16_t KEY_PIN_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                 TAMPER_KEY_EXTI_PIN_SOURCE,
#ifdef GD32F130_150
                                 USER_KEY_EXTI_PIN_SOURCE
#endif
                                };

uint16_t KEY_IRQn[KEYn] = {WAKEUP_KEY_EXTI_IRQn, 
                           TAMPER_KEY_EXTI_IRQn,
#ifdef GD32F130_150
                           USER_KEY_EXTI_IRQn
#endif
                          };




/* GD321X0 eval low layer private functions */
/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the Led to be configured
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void  gd_eval_ledinit (led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the Led to be turned on
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_ledon(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the Led to be turned off
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_ledoff(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the Led to be toggled
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_ledtoggle(led_typedef_enum lednum)
{
#ifdef GD32F130_150
    GPIO_OCTL(GPIO_PORT[lednum]) ^= GPIO_PIN[lednum];
#elif defined(GD32F170_190)
    GPIO_TG(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
#endif
}

/*!
    \brief      configure key
    \param[in]  keynum: specify the key to be configured
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_keyinit(key_typedef_enum keynum, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[keynum]);
    rcu_periph_clock_enable(RCU_CFGCMP);

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[keynum], GPIO_MODE_INPUT, GPIO_PUPD_NONE,KEY_PIN[keynum]);

    if (key_mode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[keynum], 2, 0);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_PORT_SOURCE[keynum], KEY_PIN_SOURCE[keynum]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[keynum], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[keynum]);
    }
}

/*!
    \brief      return the selected button state
    \param[in]  button: specify the button to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the button GPIO pin value
*/
uint8_t gd_eval_keygetstate(key_typedef_enum button)
{
    return gpio_input_bit_get(KEY_PORT[button], KEY_PIN[button]);
}

/*!
    \brief      configure COM port
    \param[in]  COM: COM on the board
      \arg        EVAL_COM1: COM1 on the board
      \arg        EVAL_COM2: COM2 on the board
    \param[out] none
    \retval     none
*/
void gd_eval_COMinit(uint32_t COM)
{
    /* enable GPIO clock */
    uint32_t COM_ID;
    if(EVAL_COM1==COM)
    COM_ID = 0;
#ifdef GD32F170_190
    if(EVAL_COM2==COM)
    COM_ID = 1;
#endif /* GD32F170_190 */

    rcu_periph_clock_enable( EVAL_COM_GPIO_CLK);

#if defined(GD32F130_150)
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
#elif defined(GD32F170_190)
    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);
#else
    #error "Please define GD32F130_150 or GD32F170_190"
#endif 

    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_TX_PIN[COM_ID]);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM_GPIO_PORT, EVAL_COM_AF, COM_RX_PIN[COM_ID]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,COM_TX_PIN[COM_ID]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
    gpio_output_options_set(EVAL_COM_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,COM_RX_PIN[COM_ID]);

    /* USART configure */
    usart_deinit(COM);
    usart_baudrate_set(COM,115200);
    usart_transfer_config(COM,USART_RTEN_ENABLE);
}
