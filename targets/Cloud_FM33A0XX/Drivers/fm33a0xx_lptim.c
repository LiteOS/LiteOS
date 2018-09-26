/**
  ******************************************************************************
  * @file    fm33a0xx_lptim.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_lptim.h" 


/** @addtogroup fm33a0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup LPTIM 
  * @brief LPTIM driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


//Code_Start
//2018-04-02-13-22-48
//FM33A0XX_Driver_Gen_V1.4

/* 外部Trigger滤波使能 相关函数 */
void LPTIM_LPTCFG_FLTEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LPTIM->LPTCFG &= ~(LPTIM_LPTCFG_FLTEN_Msk);
	}
	else
	{
		LPTIM->LPTCFG |= (LPTIM_LPTCFG_FLTEN_Msk);
	}
}

FunState LPTIM_LPTCFG_FLTEN_Getable(void)
{
	if (LPTIM->LPTCFG & (LPTIM_LPTCFG_FLTEN_Msk))
	{
		return DISABLE;
	}
	else
	{
		return ENABLE;
	}
}

/* 计数时钟分频选择 相关函数 */
void LPTIM_LPTCFG_DIVSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_DIVSEL_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_DIVSEL_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_DIVSEL_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_DIVSEL_Msk);
}

/* 时钟源选择 相关函数 */
void LPTIM_LPTCFG_CLKSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_CLKSEL_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_CLKSEL_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_CLKSEL_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_CLKSEL_Msk);
}

/* LPTIN输入边沿选择 相关函数 */
void LPTIM_LPTCFG_EDGESEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_EDGESEL_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_EDGESEL_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_EDGESEL_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_EDGESEL_Msk);
}

/* 外部触发边沿选择 相关函数 */
void LPTIM_LPTCFG_TRIGCFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_TRIGCFG_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_TRIGCFG_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_TRIGCFG_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_TRIGCFG_Msk);
}

/* 输出波形极性选择 相关函数 */
void LPTIM_LPTCFG_POLARITY_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_POLARITY_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_POLARITY_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_POLARITY_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_POLARITY_Msk);
}

/* 脉宽调制模式 相关函数 */
void LPTIM_LPTCFG_PWM_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_PWM_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_PWM_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_PWM_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_PWM_Msk);
}

/* 计数模式选择 相关函数 */
void LPTIM_LPTCFG_MODE_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_MODE_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_MODE_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_MODE_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_MODE_Msk);
}

/* 工作模式选择 相关函数 */
void LPTIM_LPTCFG_TMODE_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LPTIM->LPTCFG;
	tmpreg &= ~(LPTIM_LPTCFG_TMODE_Msk);
	tmpreg |= (SetValue & LPTIM_LPTCFG_TMODE_Msk);
	LPTIM->LPTCFG = tmpreg;
}

uint32_t LPTIM_LPTCFG_TMODE_Get(void)
{
	return (LPTIM->LPTCFG & LPTIM_LPTCFG_TMODE_Msk);
}

void LPTIM_LPTCNT_Write(uint32_t SetValue)
{
	LPTIM->LPTCNT = (SetValue & LPTIM_LPTCNT_CNT16_Msk);
}

uint32_t LPTIM_LPTCNT_Read(void)
{
	return (LPTIM->LPTCNT & LPTIM_LPTCNT_CNT16_Msk);
}

void LPTIM_LPTCMP_Write(uint32_t SetValue)
{
	LPTIM->LPTCMP = (SetValue & LPTIM_LPTCMP_COMPARE_REG_Msk);
}

uint32_t LPTIM_LPTCMP_Read(void)
{
	return (LPTIM->LPTCMP & LPTIM_LPTCMP_COMPARE_REG_Msk);
}

void LPTIM_LPTTARGET_Write(uint32_t SetValue)
{
	LPTIM->LPTTARGET = (SetValue & LPTIM_LPTTARGET_TARGET_REG_Msk);
}

uint32_t LPTIM_LPTTARGET_Read(void)
{
	return (LPTIM->LPTTARGET & LPTIM_LPTTARGET_TARGET_REG_Msk);
}

/* 外部触发中断 相关函数 */
void LPTIM_LPTIE_TRIGIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LPTIM->LPTIE |= (LPTIM_LPTIE_TRIGIE_Msk);
	}
	else
	{
		LPTIM->LPTIE &= ~(LPTIM_LPTIE_TRIGIE_Msk);
	}
}

FunState LPTIM_LPTIE_TRIGIE_Getable(void)
{
	if (LPTIM->LPTIE & (LPTIM_LPTIE_TRIGIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 计数器溢出中断 相关函数 */
void LPTIM_LPTIE_OVIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LPTIM->LPTIE |= (LPTIM_LPTIE_OVIE_Msk);
	}
	else
	{
		LPTIM->LPTIE &= ~(LPTIM_LPTIE_OVIE_Msk);
	}
}

FunState LPTIM_LPTIE_OVIE_Getable(void)
{
	if (LPTIM->LPTIE & (LPTIM_LPTIE_OVIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较值匹配中断 相关函数 */
void LPTIM_LPTIE_COMPIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LPTIM->LPTIE |= (LPTIM_LPTIE_COMPIE_Msk);
	}
	else
	{
		LPTIM->LPTIE &= ~(LPTIM_LPTIE_COMPIE_Msk);
	}
}

FunState LPTIM_LPTIE_COMPIE_Getable(void)
{
	if (LPTIM->LPTIE & (LPTIM_LPTIE_COMPIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 外部触发标志 相关函数 */
void LPTIM_LPTIF_TRIGIF_Clr(void)
{
	LPTIM->LPTIF = LPTIM_LPTIF_TRIGIF_Msk;
}

FlagStatus LPTIM_LPTIF_TRIGIF_Chk(void)
{
	if (LPTIM->LPTIF & LPTIM_LPTIF_TRIGIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 计数器溢出标志 相关函数 */
void LPTIM_LPTIF_OVIF_Clr(void)
{
	LPTIM->LPTIF = LPTIM_LPTIF_OVIF_Msk;
}

FlagStatus LPTIM_LPTIF_OVIF_Chk(void)
{
	if (LPTIM->LPTIF & LPTIM_LPTIF_OVIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 比较值匹配标志 相关函数 */
void LPTIM_LPTIF_COMPIF_Clr(void)
{
	LPTIM->LPTIF = LPTIM_LPTIF_COMPIF_Msk;
}

FlagStatus LPTIM_LPTIF_COMPIF_Chk(void)
{
	if (LPTIM->LPTIF & LPTIM_LPTIF_COMPIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* LPT使能 相关函数 */
void LPTIM_LPTCTRL_LPTEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LPTIM->LPTCTRL |= (LPTIM_LPTCTRL_LPTEN_Msk);
	}
	else
	{
		LPTIM->LPTCTRL &= ~(LPTIM_LPTCTRL_LPTEN_Msk);
	}
}

FunState LPTIM_LPTCTRL_LPTEN_Getable(void)
{
	if (LPTIM->LPTCTRL & (LPTIM_LPTCTRL_LPTEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}


void LPTIM_Deinit(void)
{
	LPTIM->LPTCFG = 0x00000000;
	//LPTIM->LPTCNT = ;
	LPTIM->LPTCMP = 0x00000000;
	LPTIM->LPTTARGET = 0x00000000;
	LPTIM->LPTIE = 0x00000000;
	//LPTIM->LPTIF = ;
	LPTIM->LPTCTRL = 0x00000000;
}
//Code_End


void LPTIM_Init(LPTIM_InitTypeDef* para)
{	
	LPTIM_LPTCFG_FLTEN_Setable(para->LPTIM_FLTEN);
	LPTIM_LPTCFG_DIVSEL_Set(para->LPTIM_CLK_DIV);
	LPTIM_LPTCFG_CLKSEL_Set(para->LPTIM_CLK_SEL);
	LPTIM_LPTCFG_EDGESEL_Set(para->LPTIM_LPTIN_EDGE);
	LPTIM_LPTCFG_TRIGCFG_Set(para->LPTIM_TRIG_CFG);
	LPTIM_LPTCFG_POLARITY_Set(para->LPTIM_POLAR);
	LPTIM_LPTCFG_PWM_Set(para->LPTIM_PWM);
	LPTIM_LPTCFG_MODE_Set(para->LPTIM_MODE);
	LPTIM_LPTCFG_TMODE_Set(para->LPTIM_TMODE);

	LPTIM_LPTCMP_Write(para->LPTIM_compare_value);
	LPTIM_LPTTARGET_Write(para->LPTIM_target_value);
}


/******END OF FILE****/

