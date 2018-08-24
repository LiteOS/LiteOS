/**
  ******************************************************************************
  * @file    fm33a0xx_trng.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    30-11-2017
  * @brief   This file contains all the functions prototypes for the RCC firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_TRNG_H
#define __FM33A0XX_TRNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/* TRNG控制 相关函数 */
extern void ANAC_TRNGCON_TRNGEN_Setable(FunState NewState);
extern FunState ANAC_TRNGCON_TRNGEN_Getable(void);
	 
#define TRNG_TRNGCON_RNGEN_Setable ANAC_TRNGCON_TRNGEN_Setable
#define TRNG_TRNGCON_RNGEN_Getable ANAC_TRNGCON_TRNGEN_Getable
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

//Macro_START
//2018-04-02-13-22-21
//FM33A0XX_Driver_Gen_V1.4

#define	TRNG_TRNGCON_RNGEN_Pos	0	/* RNG使能寄存器，软件写1使能 */
#define	TRNG_TRNGCON_RNGEN_Msk	(0x1U << TRNG_TRNGCON_RNGEN_Pos)
	/* 0：关闭RNG */
	/* 1：使能RNG */

#define	TRNG_RNGOUT_RNGOUT_Pos	0	/* 随机数生成结果或CRC运算结果寄存器 */
#define	TRNG_RNGOUT_RNGOUT_Msk	(0xffffffffU << TRNG_RNGOUT_RNGOUT_Pos)

#define	TRNG_RNGIE_RNGIE_Pos	0	/* 随机数生成失败中断使能寄存器
 */
#define	TRNG_RNGIE_RNGIE_Msk	(0x1U << TRNG_RNGIE_RNGIE_Pos)
	/* 0：禁止随机数生成失败中断 */
	/* 1：使能随机数生成失败中断 */

#define	TRNG_RNGIF_LFSREN_Pos	1	/* LFSR使能标志,本寄存器不会引起模块中断，仅供查询 */
#define	TRNG_RNGIF_LFSREN_Msk	(0x1U << TRNG_RNGIF_LFSREN_Pos)

#define	TRNG_RNGIF_RNF_Pos	0	/* 随机数生成失败标志，软件写1清0 */
#define	TRNG_RNGIF_RNF_Msk	(0x1U << TRNG_RNGIF_RNF_Pos)

#define	TRNG_CRCCON_CRCEN_Pos	0	/* CRC控制寄存器 */
#define	TRNG_CRCCON_CRCEN_Msk	(0x1U << TRNG_CRCCON_CRCEN_Pos)
	/* 0：CRC关闭 */
	/* 1：CRC使能 */

#define	TRNG_CRCIN_CRCIN_Pos	0	/* CRC运算数据输入寄存器 */
#define	TRNG_CRCIN_CRCIN_Msk	(0xffffffffU << TRNG_CRCIN_CRCIN_Pos)

#define	TRNG_CRCFLAG_CRCD_Pos	0	/* CRC计算完成标志，软件写0清零 */
#define	TRNG_CRCFLAG_CRCD_Msk	(0x1U << TRNG_CRCFLAG_CRCD_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void TRNG_Deinit(void);

///* RNG使能寄存器，软件写1使能 相关函数 */
//extern void TRNG_TRNGCON_RNGEN_Setable(FunState NewState);
//extern FunState TRNG_TRNGCON_RNGEN_Getable(void);

/* 随机数生成结果或CRC运算结果寄存器 相关函数 */
extern uint32_t TRNG_RNGOUT_Read(void);

/* 随机数生成失败中断使能寄存器
 相关函数 */
extern void TRNG_RNGIE_RNGIE_Setable(FunState NewState);
extern FunState TRNG_RNGIE_RNGIE_Getable(void);

/* LFSR使能标志,本寄存器不会引起模块中断，仅供查询 相关函数 */
extern FlagStatus TRNG_RNGIF_LFSREN_Chk(void);

/* 随机数生成失败标志，软件写1清0 相关函数 */
extern void TRNG_RNGIF_RNF_Clr(void);
extern FlagStatus TRNG_RNGIF_RNF_Chk(void);

/* CRC控制寄存器 相关函数 */
extern void TRNG_CRCCON_CRCEN_Setable(FunState NewState);
extern FunState TRNG_CRCCON_CRCEN_Getable(void);

/* CRC运算数据输入寄存器 相关函数 */
extern void TRNG_CRCIN_Write(uint32_t SetValue);
extern uint32_t TRNG_CRCIN_Read(void);

/* CRC计算完成标志，软件写0清零 相关函数 */
extern void TRNG_CRCFLAG_CRCD_Clr(void);
extern FlagStatus TRNG_CRCFLAG_CRCD_Chk(void);
//Announce_End


#ifdef __cplusplus
}
#endif

#endif 



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



