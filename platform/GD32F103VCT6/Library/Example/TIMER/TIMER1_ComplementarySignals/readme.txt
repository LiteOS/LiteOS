/**
  ******************************************************************************
  * @file    TIMER/TIMER1_ComplementarySignals/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the
  TIMER1 peripheral to generate three complementary TIMER1 signals.
    TIMER1CLK is fixed to SystemCoreClock, the TIMER1 Prescaler is equal to 5400 
  so the TIMER1 counter clock used is 20KHz.
    The Three Duty cycles are computed as the following description: 
    The channel 1 duty cycle is set to 25% so channel 1N is set to 75%.
    The channel 2 duty cycle is set to 50% so channel 2N is set to 50%.
    The channel 3 duty cycle is set to 75% so channel 3N is set to 25%.
 
  @note
    Connect the TIMER1 pins to an oscilloscope to monitor the different waveforms:
    - TIMER1_CH1  pin (PA8)
    - TIMER1_CH1N pin (PB13)
    - TIMER1_CH2  pin (PA9)
    - TIMER1_CH1N pin (PB14)
    - TIMER1_CH3  pin (PA10)
    - TIMER1_CH3N pin (PB15)

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
