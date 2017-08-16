 /**
  ******************************************************************************
  * @file    RTC/Calendar_demo/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the Calendar demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it explains how to configure the
  RTC module. In this demo, RTC peripheral is configured to display calendar.
    The RTC clock source can be chose to LSE or LSI by uncomment the corresponding
  define in rtc.c file. And the HyperTerminal should be connected to the evaluation 
  board via USART1.
    After start-up,if the BKP data register is written to a key value. If the value 
  is incorrect, the program will ask to set the time on the HyperTerminal. If the
  key value is correct, by the HyperTerminal set time,the calendar will be displayed
  on the HyperTerminal. 
    The RTC module is in the Backup Domain, and is not reset by the system reset or
  power reset. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/