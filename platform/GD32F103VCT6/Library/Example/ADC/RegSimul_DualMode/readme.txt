/**
  ******************************************************************************
  * @file    ADC/RegSimul_DualMode/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the RegSimul_DualMode demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32F107C-EVAL board to describe that how to use ADC1
  and ADC2 in regular simultaneous dual mode.ADC1 is configured to convert channel14 
  and channel17 regular channels continuously.ADC2 is configured to convert channel11
  and channel12 regular channels continuously. The connection between internal Vref 
  and channel17 is enabled for ADC1.

    Once the regular channels conversion is started by software, channel14 is converted
  on ADC1 and channel11 is converted on ADC2 at the same time. Then,the 32bits conversion 
  result is stored in ADC1 DR register. The DMA will transfer this data which will be
  stored in ADC_DualConvertedValueTab table. Continuously to those conversion,channel17 
  is converted on ADC1 and channel12 on ADC2. The combined conversion result is 
  transferred by DMA to the same destination buffer. The same procedure is   repeated 
  except that the specified number of data to be transferred by DMA is reached.

  The ADCs clocks are set to 14 MHz.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/