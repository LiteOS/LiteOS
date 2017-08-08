/**
  ******************************************************************************
  * @file    ETH/Project/inc/netconf.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Headfile of the net configuration.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H
     
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Exported functions ------------------------------------------------------- */
void LwIP_Init(void);
void LwIP_Pkt_Handle(void);
void LwIP_Periodic_Handle(__IO uint32_t localtime);
void Display_Periodic_Handle(__IO uint32_t localtime);

#endif /* __NETCONF_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
