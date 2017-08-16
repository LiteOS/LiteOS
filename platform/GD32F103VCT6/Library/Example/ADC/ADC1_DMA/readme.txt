/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the ADC1_DMA demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32F107C-EVAL board, it shows how to use the ADC1
  to convert analog signal to digital data through DMA.The ADC1 is configured 
  in continuous mode, PC3(channel13) is chosen as analog input pin. The ADC1 clock
  is configured to 12MHz(below 14MHz).
    As the AD convertion begins by software,the converted data from ADC1 DR register
  to SRAM begins continuously.Users can change the VR1 on the GD32F107C-EVAL board,
  measure TP1 pad on the  GD32F107C-EVAL board,and check if its value matchs the 
  converted data through the watch window.     
 
  @note
    The analog input pin should configured to AIN mode and the ADC1 clock should 
  below 14MHz. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/