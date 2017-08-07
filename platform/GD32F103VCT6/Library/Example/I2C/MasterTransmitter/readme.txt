/**
  ******************************************************************************
  * @file    I2C/MasterTransmitter/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-20144 
  * @brief   Description of the MasterTransmitter demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows the I2C programming 
  mode in master transmitting mode.In this demo,I2C1 is operated as the master 
  transmitter, and the SCL line and SDA line of I2C1 interface are controled by 
  the I/O pin PB6 and PB7 respectively.
    This demo shows the sending process of the master.And it will send the data 
  in the I2C1_Buf_Write array through I2C1.

  @note
    The demo doesn't perform the data transfer actually, which is due to no specific 
  slave.In the specific application, we must send the correct slave address,and the 
  master and the slave may need to be connected by the jumper if necessary.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/