/**
  ******************************************************************************
  * @file    I2C/SlaveTransmitter/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the SlaveTransmitter demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows the I2C programming 
  mode in slave transmitting mode.In this demo,I2C1 is operated as slave 
  transmitter,and the SCL line and SDA line of I2C1 interface are controled by 
  the I/O pin PB6 and PB7 respectively.
    This demo shows the sending data process of the slave. And it will send the 
  data in the I2C1_Buf_Write array through the I2C1.

  @note
    The demo doesn't perform the data transfer actually, which is due to no specific 
  master.In the specific application, we need config the master to start a data 
  transmission.In addition,the master and the slave may need to be connected by 
  the jumper if necessary.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/