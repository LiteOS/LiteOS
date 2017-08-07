/**
  ******************************************************************************
  * @file    ETH/Project/src/readme.txt
  * @author  MCU SD 
  * @version V1.0
  * @date    26-Dec-2014 
  * @brief   Description of the ETH demo.
  ******************************************************************************
  */

  @brief
    This demo is based on the GD32107C-EVAL board, it shows how to configure the 
  ETH peripherals to send and receive frames in normal mode and use lwip tcp/ip 
  stack to realize ping, telnet and tcp server functions. 
 
  @note
    JP4,JP13,JP18 and JP19 must be fitted.
    If LCD is used, P2, P3 and JP24 also be fitted.
    User should config ip address,mask and gw of GD32107C-EVAL board according to the actual 
  net situation from the private defines in platform_cfg.h.
    If user needs dhcp function, it can be config from the private defines in lwipopts.h.
  This function is closed in default.

/****************** (C) COPYRIGHT 2014 GIGADEVICE *********END OF FILE*********/
