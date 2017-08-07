/**
  ******************************************************************************
  * @file    MCUDGB/MCUDBG_Timer1 stop/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows that, when the corresponding bit(MCUDBG_TIMER1_STOP) in     MCUDBG control register(MCUDBG_CTLR) is set,When the core halted, the timer1 counters stopped counting and the      PWM outputs of all channels are stopped as well.It's benefit for debuging.
    the TIMER1 counter clock used is 20KHz.
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
	
    For Keil,in debug mode,when stopped,update the register window for TIMER1,you will see that the count value         will not change.And at the same time,the PWM outputs of all the channels of TIMER1,including complementary          channels will be stopped. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
