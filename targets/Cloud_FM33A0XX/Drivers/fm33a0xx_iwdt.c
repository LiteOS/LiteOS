/**
  ******************************************************************************
  * @file    fm33a00xx_iwdt.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "FM33A0XX_iwdt.h"  

/** @addtogroup fm33a0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup IWDT 
  * @brief IWDT driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup IWDT_Private_Functions
  * @{
  */ 


//Code_Start
//2018-04-02-13-21-26
//FM33A0XX_Driver_Gen_V1.4

/* IWDT清除寄存器 相关函数 */
void IWDT_IWDTSERV_Write(uint32_t SetValue)
{
	IWDT->IWDTSERV = (SetValue);
}

/* IWDT休眠4096s配置 相关函数 */
void IWDT_IWDTCFG_IWDTSLP4096S_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		IWDT->IWDTCFG |= (IWDT_IWDTCFG_IWDTSLP4096S_Msk);
	}
	else
	{
		IWDT->IWDTCFG &= ~(IWDT_IWDTCFG_IWDTSLP4096S_Msk);
	}
}

FunState IWDT_IWDTCFG_IWDTSLP4096S_Getable(void)
{
	if (IWDT->IWDTCFG & (IWDT_IWDTCFG_IWDTSLP4096S_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* IWDT溢出周期设置 相关函数 */
void IWDT_IWDTCFG_IWDTOVP_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = IWDT->IWDTCFG;
	tmpreg &= ~(IWDT_IWDTCFG_IWDTOVP_Msk);
	tmpreg |= (SetValue & IWDT_IWDTCFG_IWDTOVP_Msk);
	IWDT->IWDTCFG = tmpreg;
}

uint32_t IWDT_IWDTCFG_IWDTOVP_Get(void)
{
	return (IWDT->IWDTCFG & IWDT_IWDTCFG_IWDTOVP_Msk);
}

/* IWDT当前计数值 相关函数 */
uint32_t IWDT_IWDTCNT_Read(void)
{
	return (IWDT->IWDTCNT & IWDT_IWDTCNT_IWDTCNT_Msk);
}


void IWDT_Deinit(void)
{
	//IWDT->IWDTSERV = ;
	IWDT->IWDTCFG = 0x00000001;
	//IWDT->IWDTCNT = ;
}
//Code_End




void IWDT_Clr(void)
{
    IWDT->IWDTSERV = WDTSERV_key;
//	IWDT_IWDTSERV_Write(WDTSERV_key);
}



/******END OF FILE****/



