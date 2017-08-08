/**
  ******************************************************************************
  * @file    EXMC/EXMC_NAND/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the EXMC_NAND demo.
  ******************************************************************************
  */

  @brief
      This demo is based on the GD32107C-EVAL board, it shows how  to use EXMC 
  peripheral to control NAND memory. This board uses EXMC_BANK2_NAND to support 
  NAND memory. 
      This demo shows the write and read data operation process of NAND memory by 
  EXMC module. Firstly read NAND_ID. If read ID failure, print out the failure 
  prompt information. Otherwise write and read NAND memory.If the access correctly, 
  all leds will be turned on and print out the information to  access to NAND memory 
  successfully. Otherwise, only LED2 and LED3 are turned on print out the information 
  to access to NAND memory failure.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/