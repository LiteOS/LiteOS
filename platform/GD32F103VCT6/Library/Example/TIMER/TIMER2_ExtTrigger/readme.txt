/**
  ******************************************************************************
  * @file    TIMER/TIMER2_ExtTrigger/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the TIMER demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to start TIMER
  peripherals with an external trigger.
    TIMER2 is configured as Slave Timer for an external Trigger connected to 
  TIMER2 TI1 pin :
    - The TIMER2 TI1FP1 is used as Trigger Input
    - Rising edge is used to start the TIMER2: Trigger Mode.
    - TIMER2 is used PWM2 Mode 
    The starts of the TIMER2 counter are controlled by the external trigger. 

  @note
    Connect the TIMER2 CH1 pin to +3V3 on the board as a external trigger with a
  rising edge.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
