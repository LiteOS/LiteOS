/**
  ******************************************************************************
  * @file    ETH/Project/inc/platform_cfg.h
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Hardware specific configuration file.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PLATFORM_CFG_H
#define _PLATFORM_CFG_H

/* Exported macro ------------------------------------------------------------*/
#define IP_ADDR0        192
#define IP_ADDR1        168
#define IP_ADDR2        51
#define IP_ADDR3        100
            
#define MASK_0          255
#define MASK_1          255
#define MASK_2          255
#define MASK_3          0
     
#define GW_ADDR0        192
#define GW_ADDR1        168
#define GW_ADDR2        51
#define GW_ADDR3        1    

#define DP83848_PHY       
#define PHY_ADDRESS      1 /* Relative to GD32F107C-EVAL Board */

//#define MII_MODE         
#define RMII_MODE         

#endif/* _PLATFORM_CFG_H */

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
