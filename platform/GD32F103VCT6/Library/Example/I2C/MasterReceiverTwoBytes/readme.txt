/**
  ******************************************************************************
  * @file    I2C/MasterReceiverTwoBytes/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the MasterReceiverTwoBytes demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows the I2C programming 
  mode in master receiving mode which the master only received Two bytes of data. 
  In this demo,I2C1 is operated as master receiver,and the SCL line and SDA line 
  of I2C1 interface are controled by the I/O pin PB6 and PB7 respectively.
    This demo shows the receiving two bytes process of the master. And it will 
  store the received data in the I2C1_Buf_Read array.

  @note
    The demo doesn't perform the data transfer actually, which is due to no specific 
  slave.In the specific application,we must send the correct slave address, and the 
  master and the slave may need to be connected by the jumper if necessary.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/