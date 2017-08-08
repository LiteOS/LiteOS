/**
  ******************************************************************************
  * @file    TIMER/Cascade_Synchro/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014  
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to synchronize 
  TIMER peripherals in cascade mode.
    In this example three timers are used:
    1/TIMER2 is configured as Master Timer:
    - PWM Mode is used.
    - The TIMER2 Update event is used as Trigger Output.

    2/TIMER3 is slave for TIMER2 and Master for TIMER1,
    - PWM Mode is used.
    - The ITR1(TIMER2) is used as input trigger.
    - External clock mode is used,the counter counts on the rising edges of
      the selected trigger.
    - The TIMER3 Update event is used as Trigger Output. 

    3/TIMER1 is slave for TIMER3,
    - PWM Mode is used.
    - The ITR2(TIMER3) is used as input trigger.
    - External clock mode is used,the counter counts on the rising edges of
    the selected trigger.
 
  @note
    The TIMERxCLK is fixed to 108 MHz, the TIMER2 counter clock is :
  108MHz/54000= 2 KHz.

    The Master Timer TIMER2 is running at TIMER2 frequency :
    TIMER2 frequency = (TIMER2 counter clock)/ (TIMER2 period + 1) = 0.5 Hz 
  and the duty cycle = TIMER2_CCR1/(TIMER2_ARR + 1) = 50%

    The TIMER3 is running:
    - At (TIMER2 frequency)/ (TIMER3 period + 1) = 0.25 Hz and a duty cycle
    equal to TIMER3_CCR1/(TIMER3_ARR + 1) = 50%

    The TIMER1 is running:
    - At (TIMER3 frequency)/ (TIMER1 period + 1) = 0.125 Hz and a duty cycle
    equal to TIMER1_CCR1/(TIMER1_ARR + 1) = 50%

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
