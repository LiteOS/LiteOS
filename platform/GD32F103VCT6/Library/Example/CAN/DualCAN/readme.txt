/**
  ******************************************************************************
  * @file    CAN/DualCAN/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the Dual CAN demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the 
  CAN1 and CAN2 peripherals to send and receive CAN frames in normal mode. pressing 
  User KEY or TAMPER push buttons:
  - When User KEY button is pressed, CAN1 sends a message to CAN2 and print it.
    when CAN2 receives correctly this message,the receive data will be printed. 
  - When TAMPER button is pressed, CAN2 sends a message to CAN1 and print it.
    when CAN1 receives correctly this message,the receive data will be printed.    
 
  @note
    User can select one from the preconfigured CAN baud rates from the private 
  defines in main.c.These rates is correct only when the system clock frequency 
  is 72M. 
    P2,P3 and P4 must be fitted.
    Connect JP14 CAN_L to JP15 CAN_L
    Connect JP14 CAN_H to JP15 CAN_H

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
