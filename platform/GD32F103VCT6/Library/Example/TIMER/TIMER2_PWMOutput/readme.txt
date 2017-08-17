/**
  ******************************************************************************
  * @file    TIMER/TIMER2_PWMOutput/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the
  TIMER peripheral in PWM (Pulse Width Modulation) mode.
    The TIMER2CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 108
  so the TIMER2 counter clock is 1MHz.
    TIMER2 Channel2 duty cycle = (2500/ 10000)* 100 = 25%.
    TIMER2 Channel3 duty cycle = (5000/ 10000)* 100 = 50%.
    TIMER2 Channel4 duty cycle = (7500/ 10000)* 100 = 75%.

  @note
    Connect the TIMER2 pins to an oscilloscope to monitor the different waveforms:
    - TIMER2 CH1 pin (PA1)
    - TIMER2 CH2 pin (PA2)
    - TIMER2 CH3 pin (PA3)

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
