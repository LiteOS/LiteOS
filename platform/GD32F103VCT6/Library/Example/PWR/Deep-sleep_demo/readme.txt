/**
  ******************************************************************************
  * @file    PWR/Deep-sleep_demo/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the PWR Deep-sleep demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to enter the system 
  to Deep-sleep mode and wake-up using RTC Alarm and EXTI Line interrupt. The RTC 
  Alarm(EXTI Line17) is configured to generate an 3s Alarm event, The EXTI Line 
  interrupt(EXTI Line13) is configured to generate interrupt on falling edge.
    The system enters and exits Deep-sleep mode as following: 
    After system start-up, the LED2 twinkles twice, and the RTC is configured to 
  generate an 3s Alarm event, then the LED2 is off£¬the system enters Deep-sleep 
  mode, the RTC Alarm will wake-up the system after 3 seconds, After exit from 
  Deep-sleep mode, the system clock is reconfigured to its previous state (as HSE 
  and PLL are disabled in Deep-sleep mode), and the LED3 keeps on after it twinkles 
  twice, the LED2 keeps twinkling. When the K4 is pressed, the system enters the 
  Deep-sleep mode once again, LED2 and LED3 keeps on. When the K3 is pressed, the 
  system exits Deep-sleep mode, and return to the previous state(LED3 keeps on, 
  LED2 keeps twinkling).  

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/