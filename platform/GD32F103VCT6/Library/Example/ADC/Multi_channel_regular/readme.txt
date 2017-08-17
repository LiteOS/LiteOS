/**
  ******************************************************************************
  * @file    ADC/Multi_channel_regular/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the Multi_channel_regular demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107V-EVAL board, it shows how to use the ADC1
  to convert 4 channels analog signal to digital data through DMA.The ADC1 is configured 
  in continuous mode,PC1(channel11)¡¢PC2(channel12)¡¢PC3(channel13)¡¢PC4(channel14) is 
  chosen as analog input pins.The ADC1 clock is configured to 12MHz(below 14MHz).
    As the AD convertion begins by software,the converted data from ADC1 DR register
  to ADC_ConvertedValue[] begins continuously. And the converted value of each channel
  is saved in seperated arrays. Each time the channel11 converted value over the programmed 
  high threshold (value 0B66) or below the low threshold (value 08BA),LED2 is lighted on 
  or else LED3 is lighted on.      
 
  @note
    The analog input pin should configured to AN mode and the ADC1 clock should 
  below 14MHz. 

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/