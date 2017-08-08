/**
  ******************************************************************************
  * @file    CAN/LoopBack/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the CAN loopback demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to set a communication 
  with the CAN in loopback mode.
    The example first performs a transmission and a reception of a standard data
  frame by polling at 125 Kbps. The received frame is checked and print informations 
  indicate whether the communication was successful. Then, an extended data frame is 
  transmitted at 500 Kbps. Reception is done in the interrupt handler when the message 
  becomes pending in the FIFO and print informations indicate whether the communication 
  was successful.
    User can select CAN1 or CAN2 cell using the private defines in main.c
  in loopback mode.
 
  @note
    When using CAN2, please check that your device is Connectivity line. 
    P2,P3 and P4 must be fitted.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
