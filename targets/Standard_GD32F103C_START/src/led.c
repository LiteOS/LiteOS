/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
 #include "led.h"
 #include "sys_init.h"
 
 static uint32_t GPIO_PORT[LEDn] = {LED1_GPIO_PORT};
 static uint32_t GPIO_PIN[LEDn] = {LED1_PIN};
 static rcu_periph_enum GPIO_CLK[LEDn] = {LED1_GPIO_CLK};
 /*!
     \brief      configure led GPIO
     \param[in]  lednum: specify the led to be configured
       \arg        LED1
     \param[out] none
     \retval     none
 */
 void  gd_eval_led_init (led_typedef_enum lednum)
 {
     /* enable the led clock */
     rcu_periph_clock_enable(GPIO_CLK[lednum]);
     /* configure led GPIO port */ 
     gpio_init(GPIO_PORT[lednum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN[lednum]);
 
     GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
 }

 /*!
     \brief      turn on selected led
     \param[in]  lednum: specify the led to be turned on
       \arg        LED1
     \param[out] none
     \retval     none
 */
 void gd_eval_led_on(led_typedef_enum lednum)
 {
     GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
 }
 
 /*!
     \brief      turn off selected led
     \param[in]  lednum: specify the led to be turned off
       \arg        LED1
     \param[out] none
     \retval     none
 */
 void gd_eval_led_off(led_typedef_enum lednum)
 {
     GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
 }

