/**
  ******************************************************************************
  * @file    fm33a0xx_iwdt.h
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file contains all the functions prototypes for the IWDT firmware library.  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FM33A0XX_IWDT_H
#define __FM33A0XX_IWDT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX.h"

/** @addtogroup FM385_StdPeriph_Driver
  * @{
  */

/** @addtogroup IWDT
  * @{
  */ 

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define WDTSERV_key	((uint32_t)0x12345A5A)
	 
//Macro_START
//2018-04-02-13-21-26
//FM33A0XX_Driver_Gen_V1.4

#define	IWDT_IWDTSERV_IWDTSERV_Pos	0	/* IWDT清除寄存器 */
#define	IWDT_IWDTSERV_IWDTSERV_Msk	(0xffffffffU << IWDT_IWDTSERV_IWDTSERV_Pos)

#define	IWDT_IWDTCFG_IWDTSLP4096S_Pos	2	/* IWDT休眠4096s配置 */
#define	IWDT_IWDTCFG_IWDTSLP4096S_Msk	(0x1U << IWDT_IWDTCFG_IWDTSLP4096S_Pos)
	/* 0:休眠后仍使用IWDTOVP中配置的短周期 */
	/* 1:休眠后IWDT周期自动使用4096S */

#define	IWDT_IWDTCFG_IWDTOVP_Pos	0	/* IWDT溢出周期设置 */
#define	IWDT_IWDTCFG_IWDTOVP_Msk	(0x3U << IWDT_IWDTCFG_IWDTOVP_Pos)
#define	IWDT_IWDTCFG_IWDTOVP_125ms	(0x0U << IWDT_IWDTCFG_IWDTOVP_Pos)	/* x00：125ms */
#define	IWDT_IWDTCFG_IWDTOVP_500ms	(0x1U << IWDT_IWDTCFG_IWDTOVP_Pos)	/* x01：500ms */
#define	IWDT_IWDTCFG_IWDTOVP_2s	(0x2U << IWDT_IWDTCFG_IWDTOVP_Pos)	/* x10：2s */
#define	IWDT_IWDTCFG_IWDTOVP_8s	(0x3U << IWDT_IWDTCFG_IWDTOVP_Pos)	/* x11：8s */

#define	IWDT_IWDTCNT_IWDTCNT_Pos	0	/* IWDT当前计数值 */
#define	IWDT_IWDTCNT_IWDTCNT_Msk	(0x3ffffU << IWDT_IWDTCNT_IWDTCNT_Pos)
//Macro_End

/* Exported functions --------------------------------------------------------*/ 
extern void IWDT_Deinit(void);

/* IWDT清除寄存器 相关函数 */
extern void IWDT_IWDTSERV_Write(uint32_t SetValue);

/* IWDT休眠4096s配置 相关函数 */
extern void IWDT_IWDTCFG_IWDTSLP4096S_Setable(FunState NewState);
extern FunState IWDT_IWDTCFG_IWDTSLP4096S_Getable(void);

/* IWDT溢出周期设置 相关函数 */
extern void IWDT_IWDTCFG_IWDTOVP_Set(uint32_t SetValue);
extern uint32_t IWDT_IWDTCFG_IWDTOVP_Get(void);

/* IWDT当前计数值 相关函数 */
extern uint32_t IWDT_IWDTCNT_Read(void);
//Announce_End

	 
extern void IWDT_Clr(void);

#ifdef __cplusplus
}
#endif

#endif /* __FM33a0xx_IWDT_H */



/************************ (C) COPYRIGHT FMSHelectronics *****END OF FILE****/



