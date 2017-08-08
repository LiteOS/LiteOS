/**
  ******************************************************************************
  * @file    FMC/Write_Protection/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the FLASH Write_Protection example.
  ******************************************************************************
  */

  @brief
    This example is based on the GD32107C-EVAL board, it provides a description 
  of how to enable and disable the write protection for the embedded FLASH.
    
/**Enable Write protection**/ 
    To enable the Write Protection, uncomment the line "#define WRITE_PROTECTION_ENABLE"  
  in main.c file. Use the function FMC_OB_EnableWRP to protect a set of pages. 
  To load the new option byte values, it is necessary to use function FMC_OB_Reset() to 
  reset the system.
 
/**Disable Write protection**/
    To disable the Write Protection, uncomment the line "#define WRITE_PROTECTION_DISABLE"
  in main.c file. Use the function FMC_OB_Erase to disable the write protection a set of pages. 
  If other pages write protected, use the function FMC_OB_EnableWRP to protectpages. 
  To load the new option byte values, it is necessary to use function FMC_OB_Reset() to 
  reset the system.

    If the specified pages are not write protected, an erase and a write operation are
  performed.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/