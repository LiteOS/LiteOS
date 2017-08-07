/**
  ******************************************************************************
  * @file    USART/Printf/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the Printf demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107-EVAL board, it shows how to retarget the C 
  library printf function to the USART.
  The USARTx is configured as follow:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
  @note
    JP5 TO USART1
    JP6 TO USART1

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/