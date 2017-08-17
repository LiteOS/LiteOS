/**
  ******************************************************************************
  * @file    TIMER/TIMER1_DMA/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to use DMA with
  TIMER1 Update request to transfer Data from memory to TIMER1 capture compare 
  register 1.
    TIMER1CLK is fixed to SystemCoreClock, the TIMER1 Prescaler is equal to 108
  so the TIMER1 counter clock used is 1MHz.
    The objective is to configure TIMER1 channel 1(PA8) to generate PWM signal with
  a frequency equal to 1KHz and a variable duty cycle(25%,50%,75%) that is changed
  by the DMA after a specific number of Update DMA request.

    The number of this repetitive requests is defined by the TIMER1 Repetition 
  counter,each 2 Update Requests, the TIMER1 Channel 1 Duty Cycle changes to the 
  next new value defined by the buffer.
  
  @note


/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
