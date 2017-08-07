/**
  ******************************************************************************
  * @file    TIMER/TIMER2_OCActive/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the
  TIMER peripheral to generate three different signals with three different delays.
    The TIMER2CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 54000
  so the TIMER2 counter clock is 2KHz.
    And generate 3 signals with 3 different delays:
    TIMER2_CH1 delay = 4000/2000  = 2s
    TIMER2_CH2 delay = 8000/2000  = 4s
    TIMER2_CH3 delay = 12000/2000 = 6s

  @note 
    Connect the TIMER2 pins to leds to watch the signal:
    - TIMER2_CH1  pin (PA0)
    - TIMER2_CH2  pin (PA1)
    - TIMER2_CH3  pin (PA2)
    - LED3  pin (PC2)
    - LED4  pin (PE0)
    - LED5  pin (PE1)

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
