/**
  ******************************************************************************
  * @file    fm33a0xx_flash.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    3-1-2018
  * @brief   This file contains all the functions prototypes for the flash firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_FLASH_H
#define __FM33A0XX_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup NVMIF
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup NVMIF_Exported_Constants
  * @{
  */
#define flash_sector_erase_key0 	((uint32_t)0x96969696)
#define flash_sector_erase_key1 	((uint32_t)0xEAEAEAEA)
	 
#define flash_block_erase_key0 		((uint32_t)0x96969696)
#define flash_block_erase_key1 		((uint32_t)0x3C3C3C3C)

#define flash_chip_erase_key0 		((uint32_t)0x96969696)
#define flash_chip_erase_key1 		((uint32_t)0x7D7D7D7D)
	 
#define flash_erase_data 			((uint32_t)0x1234ABCD)

#define flash_PROG_key0 			((uint32_t)0xA5A5A5A5)
#define flash_PROG_key1 			((uint32_t)0xF1F1F1F1)

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

//Macro_START
//2018-04-02-13-21-07
//FM33A0XX_Driver_Gen_V1.4

#define	FLASH_FLSRDCON_WAIT_Pos	0	/* Flash读等待周期配置(CPU时钟超过24M后需开wait1) */
#define	FLASH_FLSRDCON_WAIT_Msk	(0x3U << FLASH_FLSRDCON_WAIT_Pos)
#define	FLASH_FLSRDCON_WAIT_0CYCLE	(0x0U << FLASH_FLSRDCON_WAIT_Pos)	/* 00/11：0 wait cycle */
#define	FLASH_FLSRDCON_WAIT_1CYCLE	(0x1U << FLASH_FLSRDCON_WAIT_Pos)	/* 01：1 wait cycle */
#define	FLASH_FLSRDCON_WAIT_2CYCLE	(0x2U << FLASH_FLSRDCON_WAIT_Pos)	/* 10：2 wait cycles */

#define	FLASH_PRFTCON_PRFTEN_Pos	0	/* 指令预取指使能，在WAIT==00的情况下写1无效 */
#define	FLASH_PRFTCON_PRFTEN_Msk	(0x1U << FLASH_PRFTCON_PRFTEN_Pos)

#define	FLASH_OPTBR_DBGCFGEN_Pos	31	/* 用户配置字寄存器 */
#define	FLASH_OPTBR_DBGCFGEN_Msk	(0x1U << FLASH_OPTBR_DBGCFGEN_Pos)

#define	FLASH_OPTBR_RAMBOOT_Pos	7
#define	FLASH_OPTBR_RAMBOOT_Msk	(0x1U << FLASH_OPTBR_RAMBOOT_Pos)

#define	FLASH_OPTBR_ACLOCKEN_Pos	2
#define	FLASH_OPTBR_ACLOCKEN_Msk	(0x3U << FLASH_OPTBR_ACLOCKEN_Pos)

#define	FLASH_OPTBR_DBRDPEN_Pos	0
#define	FLASH_OPTBR_DBRDPEN_Msk	(0x3U << FLASH_OPTBR_DBRDPEN_Pos)

#define	FLASH_ACLOCK1_ACLOCK_Pos	0	/* ACLOCK配置寄存器低32bit，分别用于控制Block31~Block0的应用代码读写锁定。1：读取和擦写权限锁定0：读取和擦写权限放开,软件只能写1，不能清零。 */
#define	FLASH_ACLOCK1_ACLOCK_Msk	(0xffffffffU << FLASH_ACLOCK1_ACLOCK_Pos)

#define	FLASH_ACLOCK2_ACLOCK_Pos	0	/* ACLOCK配置寄存器高32bit，分别用于控制Block63~Block32的应用代码读写锁定。1：读取和擦写权限锁定0：读取和擦写权限放开,软件只能写1，不能清零。 */
#define	FLASH_ACLOCK2_ACLOCK_Msk	(0xffffffffU << FLASH_ACLOCK2_ACLOCK_Pos)

#define	FLASH_EPCON_ERTYPE_Pos	8	/* 扇区擦 */
#define	FLASH_EPCON_ERTYPE_Msk	(0x3U << FLASH_EPCON_ERTYPE_Pos)
#define	FLASH_EPCON_ERTYPE_SECTOR	(0x0U << FLASH_EPCON_ERTYPE_Pos)	/* 00/11：Sector Erase */

#define	FLASH_EPCON_PREQ_Pos	1
#define	FLASH_EPCON_PREQ_Msk	(0x1U << FLASH_EPCON_PREQ_Pos)
#define	FLASH_EPCON_PREQ_NONE	(0x0U << FLASH_EPCON_PREQ_Pos)
#define	FLASH_EPCON_PREQ_PROG	(0x1U << FLASH_EPCON_PREQ_Pos)	/* Program Request软件置位，硬件完成编程后自动清零 */

#define	FLASH_EPCON_EREQ_Pos	0
#define	FLASH_EPCON_EREQ_Msk	(0x1U << FLASH_EPCON_EREQ_Pos)
#define	FLASH_EPCON_EREQ_NONE	(0x0U << FLASH_EPCON_EREQ_Pos)
#define	FLASH_EPCON_EREQ_ERASE	(0x1U << FLASH_EPCON_EREQ_Pos)	/* Erase Request软件置位，硬件完成擦除后自动清零 */

#define	FLASH_FLSKEY_FLSKEY_Pos	0	/* Flash擦写Key输入寄存器，软件或者SWD在启动擦写前必须正确地向此地址写入合法KEY序列。空地址，物理上无寄存器实现。 */
#define	FLASH_FLSKEY_FLSKEY_Msk	(0xffffffffU << FLASH_FLSKEY_FLSKEY_Pos)

#define	FLASH_FLSIE_AUTHIE_Pos	10	/* Flash中断使能 */
#define	FLASH_FLSIE_AUTHIE_Msk	(0x1U << FLASH_FLSIE_AUTHIE_Pos)
	/* Flash读写权限错误中断使能 */

#define	FLASH_FLSIE_KEYIE_Pos	9
#define	FLASH_FLSIE_KEYIE_Msk	(0x1U << FLASH_FLSIE_KEYIE_Pos)
	/* Flash KEY错误中断使能 */

#define	FLASH_FLSIE_CKIE_Pos	8
#define	FLASH_FLSIE_CKIE_Msk	(0x1U << FLASH_FLSIE_CKIE_Pos)
	/* 擦写定时时钟错误中断使能 */

#define	FLASH_FLSIE_PRDIE_Pos	1
#define	FLASH_FLSIE_PRDIE_Msk	(0x1U << FLASH_FLSIE_PRDIE_Pos)
	/* 编程完成标志中断使能 */

#define	FLASH_FLSIE_ERDIE_Pos	0
#define	FLASH_FLSIE_ERDIE_Msk	(0x1U << FLASH_FLSIE_ERDIE_Pos)
	/* 擦写完成标志中断使能 */

#define	FLASH_FLSIF_AUTHIF_Pos	10	/* Flash中断标志 */
#define	FLASH_FLSIF_AUTHIF_Msk	(0x1U << FLASH_FLSIF_AUTHIF_Pos)

#define	FLASH_FLSIF_KEYIF_Pos	9
#define	FLASH_FLSIF_KEYIF_Msk	(0x1U << FLASH_FLSIF_KEYIF_Pos)

#define	FLASH_FLSIF_CKIF_Pos	8
#define	FLASH_FLSIF_CKIF_Msk	(0x1U << FLASH_FLSIF_CKIF_Pos)

#define	FLASH_FLSIF_PRDIF_Pos	1
#define	FLASH_FLSIF_PRDIF_Msk	(0x1U << FLASH_FLSIF_PRDIF_Pos)

#define	FLASH_FLSIF_ERDIF_Pos	0
#define	FLASH_FLSIF_ERDIF_Msk	(0x1U << FLASH_FLSIF_ERDIF_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void FLASH_Deinit(void);

/* Flash读等待周期配置(CPU时钟超过24M后需开wait1) 相关函数 */
extern void FLASH_FLSRDCON_WAIT_Set(uint32_t SetValue);
extern uint32_t FLASH_FLSRDCON_WAIT_Get(void);

/* 用户配置字寄存器 相关函数 */
extern FlagStatus FLASH_OPTBR_DBGCFGEN_Chk(void);
extern FlagStatus FLASH_OPTBR_RAMBOOT_Chk(void);
extern FlagStatus FLASH_OPTBR_ACLOCKEN_Chk(void);
extern FlagStatus FLASH_OPTBR_DBRDPEN_Chk(void);

/* ACLOCK配置寄存器低32bit，分别用于控制Block31~Block0的应用代码读写锁定。1：读取和擦写权限锁定0：读取和擦写权限放开,软件只能写1，不能清零。 相关函数 */
extern void FLASH_ACLOCK1_Write(uint32_t SetValue);
extern uint32_t FLASH_ACLOCK1_Read(void);

/* ACLOCK配置寄存器高32bit，分别用于控制Block63~Block32的应用代码读写锁定。1：读取和擦写权限锁定0：读取和擦写权限放开,软件只能写1，不能清零。 相关函数 */
extern void FLASH_ACLOCK2_Write(uint32_t SetValue);
extern uint32_t FLASH_ACLOCK2_Read(void);

/* 扇区擦 相关函数 */
extern void FLASH_EPCON_ERTYPE_Set(uint32_t SetValue);
extern uint32_t FLASH_EPCON_ERTYPE_Get(void);
extern void FLASH_EPCON_PREQ_Set(uint32_t SetValue);
extern uint32_t FLASH_EPCON_PREQ_Get(void);
extern void FLASH_EPCON_EREQ_Set(uint32_t SetValue);
extern uint32_t FLASH_EPCON_EREQ_Get(void);

/* Flash擦写Key输入寄存器，软件或者SWD在启动擦写前必须正确地向此地址写入合法KEY序列。空地址，物理上无寄存器实现。 相关函数 */
extern void FLASH_FLSKEY_Write(uint32_t SetValue);

/* Flash中断使能 相关函数 */
extern void FLASH_FLSIE_AUTHIE_Setable(FunState NewState);
extern FunState FLASH_FLSIE_AUTHIE_Getable(void);
extern void FLASH_FLSIE_KEYIE_Setable(FunState NewState);
extern FunState FLASH_FLSIE_KEYIE_Getable(void);
extern void FLASH_FLSIE_CKIE_Setable(FunState NewState);
extern FunState FLASH_FLSIE_CKIE_Getable(void);
extern void FLASH_FLSIE_PRDIE_Setable(FunState NewState);
extern FunState FLASH_FLSIE_PRDIE_Getable(void);
extern void FLASH_FLSIE_ERDIE_Setable(FunState NewState);
extern FunState FLASH_FLSIE_ERDIE_Getable(void);

/* Flash中断标志 相关函数 */
extern void FLASH_FLSIF_AUTHIF_Clr(void);
extern FlagStatus FLASH_FLSIF_AUTHIF_Chk(void);
extern void FLASH_FLSIF_KEYIF_Clr(void);
extern FlagStatus FLASH_FLSIF_KEYIF_Chk(void);
extern void FLASH_FLSIF_CKIF_Clr(void);
extern FlagStatus FLASH_FLSIF_CKIF_Chk(void);
extern void FLASH_FLSIF_PRDIF_Clr(void);
extern FlagStatus FLASH_FLSIF_PRDIF_Chk(void);
extern void FLASH_FLSIF_ERDIF_Clr(void);
extern FlagStatus FLASH_FLSIF_ERDIF_Chk(void);
//Announce_End


extern void FLASH_Erase_Sector(uint32_t erase_addr);
extern void FLASH_Prog_SingleByte(uint32_t prog_addr,uint8_t prog_data);
extern void FLASH_Prog_ByteString(uint32_t prog_addr,uint8_t* prog_data, uint16_t Len);

#ifdef __cplusplus
}
#endif

#endif /* __FM33A0XX_RCC_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



