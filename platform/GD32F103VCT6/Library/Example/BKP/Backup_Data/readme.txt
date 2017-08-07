/**
  ******************************************************************************
  * @file    BKP/Backup_Data/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the Backup_Data demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to store user data 
  in the Backup data registers.
    As the Backup domain still powered by VBAT when VDD is switched off, its contents
  are not lost if a battery is connected to VBAT pin. When JP0 is connected to 3.3V, 
  the board is powered up, LED3, LED4 and LED5 are on. After an external reset, LED3
  and LED4 are off, LED5 is on. Change JP0 connected to external battery, the board is
  executed a power-down and power-up operation, LED2, LED4 and LED5 are on.
  
  The program behaves as follows:

  1. After startup the program checks if the board has been powered up. If yes, the
  values in the BKP data registers are checked:
   - if a battery is connected to the VBAT pin, the values in the BKP data registers
     are retained
   - if no battery is connected to the VBAT pin, the values in the BKP data registers
     are lost
     
  2. After an external reset, the BKP data registers contents are not checked.

  Four LEDs are used to show the system state as follows:
  1. LED4 on / LED2 on: a POR/PDR reset occurred and the values in the BKP data
   registers are correct
   
  2. LED4 on / LED3 on: a POR/PDR reset occurred and the values in the BKP data
   registers are not correct or they have not yet been programmed (if it is the
   first time the program is executed)
   
  3. LED4 off / LED2 off / LED3 off: no POR/PDR reset occurred

  4. LED5 on: program running
  
  @note
    BT1 should have a 3.3V battery, JP0 can change the VBAT source.  

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/