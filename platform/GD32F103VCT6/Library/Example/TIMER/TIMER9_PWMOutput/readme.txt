/**
  ******************************************************************************
  * @file    TIMER/TIMER9_PWMOutput/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GDM3201VB Start Kit board with GD32F103VKT6 device,
  it shows how to configure the TIMER peripheral in PWM (Pulse Width Modulation)
  mode.
    - The TIMER9CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 108
    so the TIMER9 counter clock is 1MHz.
    - TIMER9 Channel1,Channel2 PWM Period is 10000/1MHz = 10ms.
    - TIMER9 Channel1 duty cycle = (2500/ 10000)* 100 = 25%.
    - TIMER9 Channel2 duty cycle = (5000/ 10000)* 100 = 50%.

  @note
    Connect the TIMER9 pins to an oscilloscope to monitor the different waveforms:
    - TIMER9 CH1  pin (PA2).
    - TIMER9 CH2  pin (PA3).

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
