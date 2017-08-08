/**
  ******************************************************************************
  * @file    TIMER/TIMER10_OCToggle/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GDM3201VB Start Kit board with GD32F103VKT6 device,
  it shows how to configure the TIMER peripheral to generate the toggle signal.
    The TIMER10CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 
  54000,so the TIMER10 counter clock is 2KHz.
    TIMER10 Configuration: Output Compare Toggle Mode:
    CH1 update rate = TIMER10 counter clock / CHCC1 = 2000/2000 = 1 Hz 

  @note
    Connect the TIMER10 CH1 pin to LED3 to watch the signal:
    - TIMER10 CH1 pin (PB8)
    - LED3  pin (PC6)

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
