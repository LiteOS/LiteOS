/**
  ******************************************************************************
  * @file    PWR/LVD_demo/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the LVD demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the 
  low voltage detector using an external interrupt line. In this demo, EXTI line16 
  is configured to generate an interrupt on either rising or falling edge of the 
  LVD output signal (which indicates that the VDD voltage is below the LVD threshold).
    After system start-up, LED2 is on, however, LED2 is toggled every time(LED2 is
  switched off) when the voltage drops below the 2.9V threshold in the interrupt 
  routine.   
 
  @note
    Use a variable DC power supply connected to the 3V3 input to the evaluation 
  board and dropping the voltage below 2.9V. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/