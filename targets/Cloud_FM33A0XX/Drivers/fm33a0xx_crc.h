/**
  ******************************************************************************
  * @file    fm33a0xx_crc.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    8-1-2018
  * @brief   This file contains all the functions prototypes for the  firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_CRC_H
#define __FM33A0XX_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{       	
	uint32_t			CRCSEL;		/*!<CRC校验多项式选择*/
	FunState			RFLTIN;		/*!<CRC输入反转控制*/
	FunState			RFLTO;		/*!<CRC输出反转控制*/
	FunState			XOR;		/*!<输出异或使能*/
	uint32_t			CRC_XOR;	/*!<运算结果异或寄存器*/

}CRC_InitTypeDef;

/* Exported macro ------------------------------------------------------------*/


//Macro_START
//2018-04-02-13-20-31
//FM33A0XX_Driver_Gen_V1.4

#define	CRC_CRCDR_CRCDR_Pos	0	/* CRC数据寄存器 */
#define	CRC_CRCDR_CRCDR_Msk	(0xffffU << CRC_CRCDR_CRCDR_Pos)

#define	CRC_CRCCR_RFLTIN_Pos	6	/* CRC输入反转控制 */
#define	CRC_CRCCR_RFLTIN_Msk	(0x1U << CRC_CRCCR_RFLTIN_Pos)
	/* 0：输入不反转 */
	/* 1：输入按字节反转 */

#define	CRC_CRCCR_RFLTO_Pos	5	/* CRC输出反转控制 */
#define	CRC_CRCCR_RFLTO_Msk	(0x1U << CRC_CRCCR_RFLTO_Pos)
	/* 0：输入不反转 */
	/* 1：输入按字节反转 */

#define	CRC_CRCCR_RES_Pos	4	/* CRC结果标志位，只读 */
#define	CRC_CRCCR_RES_Msk	(0x1U << CRC_CRCCR_RES_Pos)

#define	CRC_CRCCR_BUSY_Pos	3	/* CRC运算标志位，只读 */
#define	CRC_CRCCR_BUSY_Msk	(0x1U << CRC_CRCCR_BUSY_Pos)

#define	CRC_CRCCR_XOR_Pos	2	/* 输出异或使能 */
#define	CRC_CRCCR_XOR_Msk	(0x1U << CRC_CRCCR_XOR_Pos)
	/* 0：输出不异或CRC_XOR寄存器 */
	/* 1：输出异或CRC_XOR寄存器 */

#define	CRC_CRCCR_CRCSEL_Pos	0	/* CRC校验多项式选择 */
#define	CRC_CRCCR_CRCSEL_Msk	(0x3U << CRC_CRCCR_CRCSEL_Pos)
#define	CRC_CRCCR_CRCSEL_CRC16	(0x0U << CRC_CRCCR_CRCSEL_Pos)	/* 00：CRC16 */
#define	CRC_CRCCR_CRCSEL_CRC8	(0x1U << CRC_CRCCR_CRCSEL_Pos)	/* 01：CRC8 */
#define	CRC_CRCCR_CRCSEL_CCITT	(0x2U << CRC_CRCCR_CRCSEL_Pos)	/* 10/11：CCITT */

#define	CRC_CRCLFSR_LFSR_Pos	0	/* CRC运算寄存器 */
#define	CRC_CRCLFSR_LFSR_Msk	(0xffffU << CRC_CRCLFSR_LFSR_Pos)

#define	CRC_CRCXOR_CRC_XOR_Pos	0	/* CRC输出异或寄存器 */
#define	CRC_CRCXOR_CRC_XOR_Msk	(0xffffU << CRC_CRCXOR_CRC_XOR_Pos)

#define	CRC_CRCFLSEN_FLSCRCEN_Pos	0	/* Flash内容CRC校验使能 */
#define	CRC_CRCFLSEN_FLSCRCEN_Msk	(0x1U << CRC_CRCFLSEN_FLSCRCEN_Pos)
	/* 0：不使能Flash CRC校验 */
	/* 1：启动Flash CRC校验 */

#define	CRC_CRCFLSAD_FLSAD_Pos	0	/* Flash校验起始地址（Word地址） */
#define	CRC_CRCFLSAD_FLSAD_Msk	(0x1ffffU << CRC_CRCFLSAD_FLSAD_Pos)

#define	CRC_CRCFLSSIZE_FLSS_Pos	0	/* CRC Flash校验数据长度（Word长度） */
#define	CRC_CRCFLSSIZE_FLSS_Msk	(0x1ffffU << CRC_CRCFLSSIZE_FLSS_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void CRC_Deinit(void);

/* CRC数据寄存器 相关函数 */
extern void CRC_CRCDR_Write(uint32_t SetValue);
extern uint32_t CRC_CRCDR_Read(void);

/* CRC输入反转控制 相关函数 */
extern void CRC_CRCCR_RFLTIN_Setable(FunState NewState);
extern FunState CRC_CRCCR_RFLTIN_Getable(void);

/* CRC输出反转控制 相关函数 */
extern void CRC_CRCCR_RFLTO_Setable(FunState NewState);
extern FunState CRC_CRCCR_RFLTO_Getable(void);

/* CRC结果标志位，只读 相关函数 */
extern FlagStatus CRC_CRCCR_RES_Chk(void);

/* CRC运算标志位，只读 相关函数 */
extern FlagStatus CRC_CRCCR_BUSY_Chk(void);

/* 输出异或使能 相关函数 */
extern void CRC_CRCCR_XOR_Setable(FunState NewState);
extern FunState CRC_CRCCR_XOR_Getable(void);

/* CRC校验多项式选择 相关函数 */
extern void CRC_CRCCR_CRCSEL_Set(uint32_t SetValue);
extern uint32_t CRC_CRCCR_CRCSEL_Get(void);

/* CRC运算寄存器 相关函数 */
extern void CRC_CRCLFSR_Write(uint32_t SetValue);
extern uint32_t CRC_CRCLFSR_Read(void);

/* CRC输出异或寄存器 相关函数 */
extern void CRC_CRCXOR_Write(uint32_t SetValue);
extern uint32_t CRC_CRCXOR_Read(void);

/* Flash内容CRC校验使能 相关函数 */
extern void CRC_CRCFLSEN_FLSCRCEN_Setable(FunState NewState);
extern FunState CRC_CRCFLSEN_FLSCRCEN_Getable(void);

/* Flash校验起始地址（Word地址） 相关函数 */
extern void CRC_CRCFLSAD_Write(uint32_t SetValue);
extern uint32_t CRC_CRCFLSAD_Read(void);

/* CRC Flash校验数据长度（Word长度） 相关函数 */
extern void CRC_CRCFLSSIZE_Write(uint32_t SetValue);
extern uint32_t CRC_CRCFLSSIZE_Read(void);
//Announce_End


/* CRC初始化配置函数*/
void CRC_Init(CRC_InitTypeDef* para);
	
#ifdef __cplusplus
}
#endif

#endif



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



