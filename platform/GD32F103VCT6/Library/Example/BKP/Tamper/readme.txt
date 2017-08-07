/**
  ******************************************************************************
  * @file    BKP/Tamper/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-DEC-2014 
  * @brief   Description of the Tamper demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to write/read data 
  to/from Backup data registers and demonstrates the Tamper detection function. In 
  this demo, the TAMPER pin is configured active on low level. 
    After system start-up, it writes the data to all Backup data registers, then check
  whether the data were correctly written. If yes, LED2 is on, otherwise LED3 is on.
  When K3(TAMPER pin) is pressed, the Backup data registers are reset and the Tamper 
  interrupt is generated. In the corresponding ISR, it checks whether the Backup data 
  registers are cleared or not. If yes, LED4 is on, otherwise LED5 is on.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/