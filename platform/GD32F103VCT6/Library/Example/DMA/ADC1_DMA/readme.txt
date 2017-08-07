/**
  ******************************************************************************
  * @file    DMA/ADC1_DMA/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the ADC1_DMA example.
  ******************************************************************************
  */

  @brief
    This example is based on the GD32107C-EVAL board, it provides a description 
  of how to use  DMA1 channel1 to transfer continuously converted data from ADC1 
  to RAM memory.
     
    The ADC1 is configured to convert continuously channel13. The ADC Channel13
  mapped on pin PC.03.   

    After the ADC1 conversion occures, the DMA1 channel1 transfers the data from
  ADC1 RDTR register to the ADC_ConvertedValue variable. After voltage conversion
  calculated, the voltage value is stored in AD_value variable. MCU uses printf 
  function to send the voltage value to the Hyperterminal using USART1.

  
  @note
    The ADC1 input clock shall not exceed 14 MHZ.
/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/