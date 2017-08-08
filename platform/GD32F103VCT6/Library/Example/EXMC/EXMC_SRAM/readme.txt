/**
  ******************************************************************************
  * @file    EXMC/EXMC_SRAM/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the EXMC_SRAM demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GDM3201VB Start Kit board, it shows how  to use 
  EXMC peripheral to control SRAM memory. The EXMC of GD32F103VBT6 is multiplex, 
  the SRAM has independent data lines and address lines. And this board uses a 
  multiplexer switch to extend EXMC_NE1 in order to support three kinds of memory 
  type, such as NOR Flash, SRAM and LCD, but only one extended NE1 (EXMC_NE1_NOR, 
  EXMC_NE1_SRAM, EXMC_NE1_LCD) can be used at any time.So you should turn the switch
  to SRAM
      This demo shows the write and read data operation process of SRAM memory by 
  EXMC module. If the access correctly, LED2 and LED3 will be turned on. Otherwise, 
  turn on all the LEDs.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/