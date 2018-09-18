/**
  ******************************************************************************
  * @file    fm33a0xx_aes.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    23-3-2018
  * @brief   This file contains all the functions prototypes for the AES firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_AES_H
#define __FM33A0XX_AES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"
	 
/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */
typedef struct
{
	uint32_t	KEYLEN;			//AES加密密钥长度，AESEN=1时不可修改
	uint32_t	CHMOD;			//AES数据流处理模式，AESEN=1时不可修改
	uint32_t	MODE;			//AES工作模式，AESEN=1时不可修改
	uint32_t	DATATYP;		//选择数据类型，AESEN=1时不可修改。具体交换规则可参考AES数据类型章节	
	FunState	DMAOEN;			//DMA数据自动读出使能
	FunState	DMAIEN;			//DMA数据自动写入使能
	FunState	ERRIE;			//错误标志（RDERR和WRERR）中断使能
	FunState	CCFIE;			//CCF标志中断使能
	FunState	AESEN;			//AES使能 
	
}AES_InitTypeDef;
	 
	 

//Macro_START
//2018-04-02-13-19-52
//FM33A0XX_Driver_Gen_V1.4

#define	AES_AESCR_KEYLEN_Pos	13	/* AES加密密钥长度，AESEN=1时不可修改 */
#define	AES_AESCR_KEYLEN_Msk	(0x3U << AES_AESCR_KEYLEN_Pos)
#define	AES_AESCR_KEYLEN_128BIT	(0x0U << AES_AESCR_KEYLEN_Pos)	/* 00：128bit */
#define	AES_AESCR_KEYLEN_192BIT	(0x1U << AES_AESCR_KEYLEN_Pos)	/* 01：192bit */
#define	AES_AESCR_KEYLEN_256BIT	(0x2U << AES_AESCR_KEYLEN_Pos)	/* 10：256bit */

#define	AES_AESCR_DMAOEN_Pos	12	/* DMA数据自动读出使能 */
#define	AES_AESCR_DMAOEN_Msk	(0x1U << AES_AESCR_DMAOEN_Pos)
	/* 0：不开启 */
	/* 1：开启 */

#define	AES_AESCR_DMAIEN_Pos	11	/* DMA数据自动写入使能 */
#define	AES_AESCR_DMAIEN_Msk	(0x1U << AES_AESCR_DMAIEN_Pos)
	/* 0：不开启 */
	/* 1：开启 */

#define	AES_AESCR_ERRIE_Pos	10	/* 错误标志（RDERR和WRERR）中断使能 */
#define	AES_AESCR_ERRIE_Msk	(0x1U << AES_AESCR_ERRIE_Pos)
	/* 0：不使能 */
	/* 1：使能 */

#define	AES_AESCR_CCFIE_Pos	9	/* CCF标志中断使能 */
#define	AES_AESCR_CCFIE_Msk	(0x1U << AES_AESCR_CCFIE_Pos)
	/* 0：不使能 */
	/* 1：使能 */

#define	AES_AESCR_ERRC_Pos	8	/* 清除错误标志写1清除WRERR和RDERR错误标志 */
#define	AES_AESCR_ERRC_Msk	(0x1U << AES_AESCR_ERRC_Pos)

#define	AES_AESCR_CCFC_Pos	7	/* 清除CCF标志写1清CCF标志
 */
#define	AES_AESCR_CCFC_Msk	(0x1U << AES_AESCR_CCFC_Pos)

#define	AES_AESCR_CHMOD_Pos	5	/* AES数据流处理模式，AESEN=1时不可修改 */
#define	AES_AESCR_CHMOD_Msk	(0x3U << AES_AESCR_CHMOD_Pos)
#define	AES_AESCR_CHMOD_ECB	(0x0U << AES_AESCR_CHMOD_Pos)	/* 00：ECB */
#define	AES_AESCR_CHMOD_CBC	(0x1U << AES_AESCR_CHMOD_Pos)	/* 01：CBC */
#define	AES_AESCR_CHMOD_CTR	(0x2U << AES_AESCR_CHMOD_Pos)	/* 10：CTR */
#define	AES_AESCR_CHMOD_MULTH	(0x3U << AES_AESCR_CHMOD_Pos)	/* 11：使用MultH模块 */

#define	AES_AESCR_MODE_Pos	3	/* AES工作模式，AESEN=1时不可修改 */
#define	AES_AESCR_MODE_Msk	(0x3U << AES_AESCR_MODE_Pos)
#define	AES_AESCR_MODE_ENCRYPT	(0x0U << AES_AESCR_MODE_Pos)	/* 00：模式1：加密 */
#define	AES_AESCR_MODE_KEYEXP	(0x1U << AES_AESCR_MODE_Pos)	/* 01：模式2：密钥扩展 */
#define	AES_AESCR_MODE_DECRYPT	(0x2U << AES_AESCR_MODE_Pos)	/* 10：模式3：解密 */
#define	AES_AESCR_MODE_KEYEXPDECRYPT	(0x3U << AES_AESCR_MODE_Pos)	/* 11：模式4：密钥扩展+解密 */

#define	AES_AESCR_DATATYP_Pos	1	/* 选择数据类型，AESEN=1时不可修改 */
#define	AES_AESCR_DATATYP_Msk	(0x3U << AES_AESCR_DATATYP_Pos)
#define	AES_AESCR_DATATYP_32BITNOEX	(0x0U << AES_AESCR_DATATYP_Pos)	/* 00：32bit数据不交换 */
#define	AES_AESCR_DATATYP_16BITEX	(0x1U << AES_AESCR_DATATYP_Pos)	/* 01：16bit数据半字交换 */
#define	AES_AESCR_DATATYP_8BITEX	(0x2U << AES_AESCR_DATATYP_Pos)	/* 10：8bit数据字节交换 */
#define	AES_AESCR_DATATYP_1BITEX	(0x3U << AES_AESCR_DATATYP_Pos)	/* 11：1bit数据比特交换 */

#define	AES_AESCR_AESEN_Pos	0	/* AES使能,在任何时候清除AESEN位都能够复位AES模块 */
#define	AES_AESCR_AESEN_Msk	(0x1U << AES_AESCR_AESEN_Pos)
	/* 0：不使能 */
	/* 1：使能 */

#define	AES_AESIF_WRERR_Pos	2	/* 写错误标志：在计算或输出阶段发生写操作时置位，软件对ERRC寄存器写1清零 */
#define	AES_AESIF_WRERR_Msk	(0x1U << AES_AESIF_WRERR_Pos)

#define	AES_AESIF_RDERR_Pos	1	/* 读错误标志：在计算或输入阶段发生读操作时置位，软件对ERRC寄存器写1清零 */
#define	AES_AESIF_RDERR_Msk	(0x1U << AES_AESIF_RDERR_Pos)

#define	AES_AESIF_CCF_Pos	0	/* AES计算完成标志，可以通过CCFC置1清除这个中断标志 */
#define	AES_AESIF_CCF_Msk	(0x1U << AES_AESIF_CCF_Pos)

#define	AES_AESDIN_AESDIN_Pos	0	/* 数据输入寄存器，当AES需要输入加解密数据时，应该往该寄存器连续写4次 */
#define	AES_AESDIN_AESDIN_Msk	(0xffffffffU << AES_AESDIN_AESDIN_Pos)

#define	AES_AESDOUT_AESDOUT_Pos	0	/* 数据输出寄存器，当AES计算完成后，可以分四次读出加解密的结果 */
#define	AES_AESDOUT_AESDOUT_Msk	(0xffffffffU << AES_AESDOUT_AESDOUT_Pos)


//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void AES_Deinit(void);

/* AES加密密钥长度，AESEN=1时不可修改 相关函数 */
extern void AES_AESCR_KEYLEN_Set(uint32_t SetValue);
extern uint32_t AES_AESCR_KEYLEN_Get(void);

/* DMA数据自动读出使能 相关函数 */
extern void AES_AESCR_DMAOEN_Setable(FunState NewState);
extern FunState AES_AESCR_DMAOEN_Getable(void);

/* DMA数据自动写入使能 相关函数 */
extern void AES_AESCR_DMAIEN_Setable(FunState NewState);
extern FunState AES_AESCR_DMAIEN_Getable(void);

/* 错误标志（RDERR和WRERR）中断使能 相关函数 */
extern void AES_AESCR_ERRIE_Setable(FunState NewState);
extern FunState AES_AESCR_ERRIE_Getable(void);

/* CCF标志中断使能 相关函数 */
extern void AES_AESCR_CCFIE_Setable(FunState NewState);
extern FunState AES_AESCR_CCFIE_Getable(void);

/* 清除错误标志写1清除WRERR和RDERR错误标志 相关函数 */
extern void AES_AESCR_ERRC_Clr(void);

/* 清除CCF标志写1清CCF标志
 相关函数 */
extern void AES_AESCR_CCFC_Clr(void);

/* AES数据流处理模式，AESEN=1时不可修改 相关函数 */
extern void AES_AESCR_CHMOD_Set(uint32_t SetValue);
extern uint32_t AES_AESCR_CHMOD_Get(void);

/* AES工作模式，AESEN=1时不可修改 相关函数 */
extern void AES_AESCR_MODE_Set(uint32_t SetValue);
extern uint32_t AES_AESCR_MODE_Get(void);

/* 选择数据类型，AESEN=1时不可修改 相关函数 */
extern void AES_AESCR_DATATYP_Set(uint32_t SetValue);
extern uint32_t AES_AESCR_DATATYP_Get(void);

/* AES使能,在任何时候清除AESEN位都能够复位AES模块 相关函数 */
extern void AES_AESCR_AESEN_Setable(FunState NewState);
extern FunState AES_AESCR_AESEN_Getable(void);

/* 写错误标志：在计算或输出阶段发生写操作时置位，软件对ERRC寄存器写1清零 相关函数 */
extern FlagStatus AES_AESIF_WRERR_Chk(void);

/* 读错误标志：在计算或输入阶段发生读操作时置位，软件对ERRC寄存器写1清零 相关函数 */
extern FlagStatus AES_AESIF_RDERR_Chk(void);

/* AES计算完成标志，可以通过CCFC置1清除这个中断标志 相关函数 */
extern FlagStatus AES_AESIF_CCF_Chk(void);

/* 数据输入寄存器，当AES需要输入加解密数据时，应该往该寄存器连续写4次 相关函数 */
extern void AES_AESDIN_Write(uint32_t SetValue);
extern uint32_t AES_AESDIN_Read(void);

/* 数据输出寄存器，当AES计算完成后，可以分四次读出加解密的结果 相关函数 */
extern void AES_AESDOUT_Write(uint32_t SetValue);
extern uint32_t AES_AESDOUT_Read(void);
//Announce_End

/*加解密密钥输入，key0存放密钥最低32bit */
extern void AES_AESKEY_WriteEx(uint8_t *KeyIn, uint8_t Len);

/*加解密密钥输出，第一个字存放密钥最低32bit  */
extern void AES_AESKEY_ReadEx(uint8_t *KeyOut, uint8_t Len);

/*初始向量输入 */
extern void AES_AESIVR_WriteEx(uint8_t *IVRIn);

/*初始向量读取 */
extern void AES_AESIVR_ReadEx(uint8_t *IVROut);

/*加解密数据输入，16字节（128bit）分组数据输入  */
extern void AES_AESDIN_GroupWrite_128BIT(uint8_t *DataIn);
   						
/*加解密数据输出，16字节（128bit）分组数据输出  */
extern void AES_AESDOUT_GroupRead_128BIT(uint8_t *DataOut);

extern uint8_t AES_GroupWriteAndRead_128BIT(uint8_t *DataIn, uint8_t *DataOut);

/* AES模块初始化函数 */
extern void AES_Init(AES_InitTypeDef* para);

#ifdef __cplusplus
}
#endif

#endif



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



