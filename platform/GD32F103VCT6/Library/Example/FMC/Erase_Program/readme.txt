/**
  ******************************************************************************
  * @file    FMC/Erase_Program/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the FLASH Erase_Program example.
  ******************************************************************************
  */

  @brief
    This example is based on the GD32107E-EVAL board, it provides a description 
  of how to Erase/Program FLASH memory.
    
    Before an erase operation, FMC_Unlock function is used firstly to unlock 
  the Flash memory Program/Erase Controller. 

  After the erase operation, A comparison between FLASH memory and 0xFFFFFFFF(Reset value) 
  is done to check that the FLASH memory has been correctly erased.

    Once the erase operation is finished correctly, the programming operation will be
  performed by using the FMC_ProgramWord function. A comparison between the FLASH memory 
  and the source data is done to check that all data have been correctly programed.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/