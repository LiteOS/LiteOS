/**
  ******************************************************************************
  * @file    fm33a0xx_wwdt.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    8-1-2018
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_wwdt.h" 


/** @addtogroup fm33a0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup WWDT 
  * @brief WWDT driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//Code_Start
//2018-04-02-13-22-31
//FM33A0XX_Driver_Gen_V1.4

/* WWDT控制寄存器 相关函数 */
void WWDT_WWDTCON_Write(uint32_t SetValue)
{
	WWDT->WWDTCON = (SetValue & WWDT_WWDTCON_WWDTCON_Msk);
}

/* 配置看门狗溢出时间 相关函数 */
void WWDT_WWDTCFG_WWDTCFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = WWDT->WWDTCFG;
	tmpreg &= ~(WWDT_WWDTCFG_WWDTCFG_Msk);
	tmpreg |= (SetValue & WWDT_WWDTCFG_WWDTCFG_Msk);
	WWDT->WWDTCFG = tmpreg;
}

uint32_t WWDT_WWDTCFG_WWDTCFG_Get(void)
{
	return (WWDT->WWDTCFG & WWDT_WWDTCFG_WWDTCFG_Msk);
}

/* WWDT计数寄存器值 相关函数 */
uint32_t WWDT_WWDTCNT_Read(void)
{
	return (WWDT->WWDTCNT & WWDT_WWDTCNT_WWDTCNT_Msk);
}

/* WWDT中断使能 相关函数 */
void WWDT_WWDTIE_WWDTIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		WWDT->WWDTIE |= (WWDT_WWDTIE_WWDTIE_Msk);
	}
	else
	{
		WWDT->WWDTIE &= ~(WWDT_WWDTIE_WWDTIE_Msk);
	}
}

FunState WWDT_WWDTIE_WWDTIE_Getable(void)
{
	if (WWDT->WWDTIE & (WWDT_WWDTIE_WWDTIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* WWDT 75%计时中断标志，写1清零 相关函数 */
void WWDT_WWDTIF_WWDTIF_Clr(void)
{
	WWDT->WWDTIF = WWDT_WWDTIF_WWDTIF_Msk;
}

FlagStatus WWDT_WWDTIF_WWDTIF_Chk(void)
{
	if (WWDT->WWDTIF & WWDT_WWDTIF_WWDTIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* WWDT的4096预分频计数器 相关函数 */
uint32_t WWDT_WWDTDIV_Read(void)
{
	return (WWDT->WWDTDIV & WWDT_WWDTDIV_DIVCNT_Msk);
}

void WWDT_Deinit(void)
{
	//WWDT->WWDTCON = ;
	//WWDT->WWDTCFG = 0x00000000;
	//WWDT->WWDTCNT = ;
	//WWDT->WWDTIE = 0x00000000;
	//WWDT->WWDTIF = 0x00000000;
	//WWDT->WWDTDIV = ;
}
//Code_End



void WWDT_Start(void)
{
    WWDT->WWDTCON = WWDT_START_key;
}

void WWDT_Clr(void)
{
    WWDT->WWDTCON = WWDT_CLR_key;
}

/******END OF FILE****/



