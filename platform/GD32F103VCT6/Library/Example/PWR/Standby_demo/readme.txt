/**
  ******************************************************************************
  * @file    PWR/Standby_demo/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the Standby demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to enter the system 
  to Standby mode and wake-up using RTC Alarm and WKUP Pin. The RTC is configured
  to generate an 5s Alarm event, The K4 pin(EXTI Line14) is configured the falling 
  interrupt to set the alarm in 5s£¬and enter the Standby mode.
    The system enters and exits Standby mode as following: 
    After system start-up, the LED2 twinkles twice and then off. When the K4 is 
  pressed, the LED2 twinkles once and then off the RTC alarm is in 5s£¬then the 
  system enters the Standby mode. A rising edge on WKUP pin will wake-up the system 
  from Standby mode. If within 5s, no rising edge on WKUP pin generated, the RTC Alarm 
  will wake-up the system. When the system is in Standby mode, If WKUP pin is pressed,
  LED2 twinkles twice and off, then LED3 is on. Likewise, other wakeup source wake-up 
  the system, LED2 also twinkles twice and off, then LED3 is on.
  
  @note
    The external RESET also can wake-up the system from the Standby mode.  

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/