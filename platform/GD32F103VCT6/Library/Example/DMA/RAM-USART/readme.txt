/**
  ******************************************************************************
  * @file    DMA/RAM-USART/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the DMA RAM-USART example.
  ******************************************************************************
  */

  @brief
    This example is based on the GD32107C-EVAL board, it provides a description 
  of how to use  DMA1 channel4 to transfer data buffer from RAM memory 
  to the Hyperterminal.
     
    DMA1 Channel1 is configured to transfer the contents of data buffer
  stored in "SRC_Const_Buffer" to the reception buffer(DST_Buffer) declared in RAM.    

    DMA1 Channel4 is configured to transfer the contents of data buffer
  stored in "DST_Buffer" to the Hyperterminal.    

    The start of transfer is triggered by software. At the end of the transfer, 
  a Transfer Complete interrupt is generated since it is enabled. A comparison between the
  source and destination buffers is done to check that all data have been correctly transferred.

  @note
    In order to prevent the conflict between DMA-USART transfer and printf function
  that they both use USART1 to transfer data, use Delay function to ensure the transfer of DMA-USART to complete.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/