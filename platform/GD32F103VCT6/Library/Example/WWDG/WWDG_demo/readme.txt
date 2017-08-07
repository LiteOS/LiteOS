/**
  ******************************************************************************
  * @file    WWDG_demo/readme.txt
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the WWDG demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows the WWDG with different
  counter value and window value make the different WWDG timeout. As a result, LED1 
  display a different phenomenon. 
    In this demo, the WWDG counter value is set to 127, the refresh window is set to 
  80. In this case the WWDG timeout is set to 38.9ms(the timeout may varies due to 
  LSI frequency dispersion), the value in the delay function is set to 30ms, the WWDG 
  counter can reloaded normal,the LED1 is turn off, LED2 is blinking fast.
    If the value in the delay function is not set properly, the WWDG counter is not 
  reloaded. As a result, the system reset and the LED1 and LED2 are remain illuminated.
    In this example the system clock is set to 108 MHz.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/