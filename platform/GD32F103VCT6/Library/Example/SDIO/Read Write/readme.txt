/**
  ******************************************************************************
  * @file    SDIO/Read Write/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the SDIO test demo.
  ******************************************************************************
  */
  
  @brief
    This demo is based on the GD32103C-EVAL board, it shows how to configure the 
  SDIO peripheral to communication with SD memory card.
    This demo realize the following functions:
    1.SD memory initialize.
    2.Print the information about the memory card use USART1.
    3.Single block read and write.
    4.Lock and unlock the card.
    5.Multiople block read and write.
    
  @note
    The USART1 is configured as follow:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
  
  /****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/