/**
  ******************************************************************************
  * @file    IWDG/IWDG_demo/readme.txt
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the IWDG demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to reload at regulate
  period the IWDG counter using the EXTI interrupt. The IWDG timeout is set to 1s 
  (the timeout may varies due to LSI frequency dispersion).
    An EXTI is connected to a specific GPIO pin and configured to generate an interrupt
  on its falling edge: when the EXTI Line interrupt is triggered (by pressing the Key 
  push-button on the board), the corresponding interrupt is served. In the ISR, IWDG 
  counter is reloaded). As a result, when the IWDG counter is reloaded, which prevents 
  any IWDG reset, LED1 remain illuminated.   
    If the EXTI Line interrupt does not occur, the IWDG counter is not reloaded before
  the IWDG counter reaches 00h, and the IWDG reset. If the IWDG reset is generated, LED1 
  is turned off with the system reset.
    In this example the system clock is set to 108 MHz.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/