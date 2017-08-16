/**
  ******************************************************************************
  * @file    ADC/AnalogWatchdog/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the AnalogWatchdog demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32F107C-EVAL board, it shows how to use the ADC1
  analog watchdog to guard continuously an ADC channel.The ADC1 is configured 
  in continuous mode, PC3(channel13) is chosen as analog input pin.The ADC1 clock
  is configured to 12MHz(below 14MHz).
    Each time the channel11 converted value over the programmed analog watchdog high 
  threshold (value 0x0A00) or below the analog watchdog low threshold (value 0x0400)
  an AWD interrupt is generated .    
 
  @note
    The analog input pin should configured to AIN mode and the ADC1 clock should 
  below 14MHz. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/