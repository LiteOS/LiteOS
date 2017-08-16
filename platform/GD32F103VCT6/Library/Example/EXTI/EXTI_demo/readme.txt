/**
  ******************************************************************************
  * @file    EXTI/EXTI_demo/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the EXTI demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to generate the 
  interrupt request, and toggle the LED2 and LED3. After EXTI configuration, a 
  software interrupt is generated on the EXTI0, when rising edge is detected on
  EXTI0, LED2 toggles. When falling edge is detected on EXTI14, LED3 toggles. 
  After the system start-up, LED2 is off, LED3 is on. When K2 or K4 is pressed, 
  LED2 or LED3 is toggled.   

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/