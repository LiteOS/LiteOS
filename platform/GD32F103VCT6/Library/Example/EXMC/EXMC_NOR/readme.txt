/**
  ******************************************************************************
  * @file    EXMC/EXMC_NOR/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the EXMC_NOR demo.
  ******************************************************************************
  */

  @brief
      This demo is based on the GDM3201VB Start Kit board, it shows how  to use 
  EXMC peripheral to control NOR memory. The EXMC of GD32F103VBT6 is multiplex, 
  the NOR has independent data lines and address lines. And this board uses a 
  multiplexer switch to extend EXMC_NE1 in order to support three kinds of memory 
  type, such as NOR memeory, SRAM and LCD, but only one extended NE1 (EXMC_NE1_NOR, 
  EXMC_NE1_SRAM, EXMC_NE1_LCD) can be used at any time.So you should turn the switch
  to NOR memeory.
      This demo shows the write and read data operation process of NOR memory by 
  EXMC module. If the access correctly, all leds will be turned on and print out 
  the information to access to NOR memory successfully. Otherwise, only LED2 and 
  LED3 are turned on print out the information to access to NOR memory failure. 
  Whether access to the NOR memory is successful or not, the data to be read will 
  be printed out.      

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
