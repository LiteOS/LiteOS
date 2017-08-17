/**
  ******************************************************************************
  * @file    TIMER/TIMER2_Singlepulse/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the
  TIMER2 peripheral in Single Pulse mode.
    The external signal is connected to TIMER2 CH1 pin (PA0) and the Falling 
  edge is used as active edge.
    The Single Pulse signal is output on TIMER2 CH2 pin (PA1).
    The TIMER2CLK frequency is set to SystemCoreClock (Hz),the Prescaler is 
  4,so the TIMER2 counter clock is 27MHz.
    Single Pulse value = (TIMER2_Period - TIMER2_Pulse) / TIMER2 counter clock
                       = (65535 - 11535) / 27MHz = 2.0 ms.

  @note
    When K2(PA0) is pressed,a single pulse is generated.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
