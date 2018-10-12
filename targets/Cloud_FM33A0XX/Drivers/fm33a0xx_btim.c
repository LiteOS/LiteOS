/**
  ******************************************************************************
  * @file    fm33a0xx_btim.c
  * @author  FM33a0 Application Team
  * @version V2.0.1
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_btim.h"  


/** @addtogroup fm33a0_StdPeriph_Driver
  * @{
  */

/** @defgroup BTIM 
  * @brief BTIM driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup BTIM_Private_Functions
  * @{
  */ 

//Code_Start
//2018-04-02-13-20-21
//FM33A0XX_Driver_Gen_V1.4

/* 高位计数器启动控制 相关函数 */
void BTIMx_BTCR1_CHEN_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTCR1 |= (BTIMx_BTCR1_CHEN_Msk);
	}
	else
	{
		BTIMx->BTCR1 &= ~(BTIMx_BTCR1_CHEN_Msk);
	}
}

FunState BTIMx_BTCR1_CHEN_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTCR1 & (BTIMx_BTCR1_CHEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 低位计数器启动控制 相关函数 */
void BTIMx_BTCR1_CLEN_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTCR1 |= (BTIMx_BTCR1_CLEN_Msk);
	}
	else
	{
		BTIMx->BTCR1 &= ~(BTIMx_BTCR1_CLEN_Msk);
	}
}

FunState BTIMx_BTCR1_CLEN_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTCR1 & (BTIMx_BTCR1_CLEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 工作模式选择 相关函数 */
void BTIMx_BTCR1_MODE_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR1;
	tmpreg &= ~(BTIMx_BTCR1_MODE_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR1_MODE_Msk);
	BTIMx->BTCR1 = tmpreg;
}

uint32_t BTIMx_BTCR1_MODE_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR1 & BTIMx_BTCR1_MODE_Msk);
}

/* 计数沿或捕捉沿选择 相关函数 */
void BTIMx_BTCR1_EDGESEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR1;
	tmpreg &= ~(BTIMx_BTCR1_EDGESEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR1_EDGESEL_Msk);
	BTIMx->BTCR1 = tmpreg;
}

uint32_t BTIMx_BTCR1_EDGESEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR1 & BTIMx_BTCR1_EDGESEL_Msk);
}

/* 捕捉模式控制（仅捕捉模式下有效） 相关函数 */
void BTIMx_BTCR1_CAPMOD_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR1;
	tmpreg &= ~(BTIMx_BTCR1_CAPMOD_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR1_CAPMOD_Msk);
	BTIMx->BTCR1 = tmpreg;
}

uint32_t BTIMx_BTCR1_CAPMOD_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR1 & BTIMx_BTCR1_CAPMOD_Msk);
}

/* 带清零捕捉模式控制 相关函数 */
void BTIMx_BTCR1_CAPCLR_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR1;
	tmpreg &= ~(BTIMx_BTCR1_CAPCLR_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR1_CAPCLR_Msk);
	BTIMx->BTCR1 = tmpreg;
}

uint32_t BTIMx_BTCR1_CAPCLR_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR1 & BTIMx_BTCR1_CAPCLR_Msk);
}

/* 单次捕捉控制 相关函数 */
void BTIMx_BTCR1_CAPONCE_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR1;
	tmpreg &= ~(BTIMx_BTCR1_CAPONCE_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR1_CAPONCE_Msk);
	BTIMx->BTCR1 = tmpreg;
}

uint32_t BTIMx_BTCR1_CAPONCE_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR1 & BTIMx_BTCR1_CAPONCE_Msk);
}

/* PWM模式输出 相关函数 */
void BTIMx_BTCR1_PWM_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTCR1 |= (BTIMx_BTCR1_PWM_Msk);
	}
	else
	{
		BTIMx->BTCR1 &= ~(BTIMx_BTCR1_PWM_Msk);
	}
}

FunState BTIMx_BTCR1_PWM_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTCR1 & (BTIMx_BTCR1_PWM_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 计数器内部计数源信号选择 相关函数 */
void BTIMx_BTCR2_SIG2SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR2;
	tmpreg &= ~(BTIMx_BTCR2_SIG2SEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR2_SIG2SEL_Msk);
	BTIMx->BTCR2 = tmpreg;
}

uint32_t BTIMx_BTCR2_SIG2SEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR2 & BTIMx_BTCR2_SIG2SEL_Msk);
}

/* 计数器内部捕捉源信号选择 相关函数 */
void BTIMx_BTCR2_SIG1SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR2;
	tmpreg &= ~(BTIMx_BTCR2_SIG1SEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR2_SIG1SEL_Msk);
	BTIMx->BTCR2 = tmpreg;
}

uint32_t BTIMx_BTCR2_SIG1SEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR2 & BTIMx_BTCR2_SIG1SEL_Msk);
}

/* 高位计数器源选择 相关函数 */
void BTIMx_BTCR2_CNTHSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR2;
	tmpreg &= ~(BTIMx_BTCR2_CNTHSEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR2_CNTHSEL_Msk);
	BTIMx->BTCR2 = tmpreg;
}

uint32_t BTIMx_BTCR2_CNTHSEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR2 & BTIMx_BTCR2_CNTHSEL_Msk);
}

/* 外部输入DIR控制使能 相关函数 */
void BTIMx_BTCR2_DIREN_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTCR2 |= (BTIMx_BTCR2_DIREN_Msk);
	}
	else
	{
		BTIMx->BTCR2 &= ~(BTIMx_BTCR2_DIREN_Msk);
	}
}

FunState BTIMx_BTCR2_DIREN_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTCR2 & (BTIMx_BTCR2_DIREN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 内部DIR控制信号使能 相关函数 */
void BTIMx_BTCR2_STDIR_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTCR2 |= (BTIMx_BTCR2_STDIR_Msk);
	}
	else
	{
		BTIMx->BTCR2 &= ~(BTIMx_BTCR2_STDIR_Msk);
	}
}

FunState BTIMx_BTCR2_STDIR_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTCR2 & (BTIMx_BTCR2_STDIR_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 低位计数器使能控制选择信号 相关函数 */
void BTIMx_BTCR2_SRCSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR2;
	tmpreg &= ~(BTIMx_BTCR2_SRCSEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR2_SRCSEL_Msk);
	BTIMx->BTCR2 = tmpreg;
}

uint32_t BTIMx_BTCR2_SRCSEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR2 & BTIMx_BTCR2_SRCSEL_Msk);
}

/* 输入信号2极性选择 相关函数 */
void BTIMx_BTCR2_DIRPO_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCR2;
	tmpreg &= ~(BTIMx_BTCR2_DIRPO_Msk);
	tmpreg |= (SetValue & BTIMx_BTCR2_DIRPO_Msk);
	BTIMx->BTCR2 = tmpreg;
}

uint32_t BTIMx_BTCR2_DIRPO_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCR2 & BTIMx_BTCR2_DIRPO_Msk);
}

/* RTCOUT2信号控制2 相关函数 */
void BTIMx_BTCFG1_RTCSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG1;
	tmpreg &= ~(BTIMx_BTCFG1_RTCSEL2_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG1_RTCSEL2_Msk);
	BTIMx->BTCFG1 = tmpreg;
}

uint32_t BTIMx_BTCFG1_RTCSEL2_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG1 & BTIMx_BTCFG1_RTCSEL2_Msk);
}

/* RTCOUT1信号控制1 相关函数 */
void BTIMx_BTCFG1_RTCSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG1;
	tmpreg &= ~(BTIMx_BTCFG1_RTCSEL1_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG1_RTCSEL1_Msk);
	BTIMx->BTCFG1 = tmpreg;
}

uint32_t BTIMx_BTCFG1_RTCSEL1_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG1 & BTIMx_BTCFG1_RTCSEL1_Msk);
}

/* Group2信号选择控制 相关函数 */
void BTIMx_BTCFG1_GRP2SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG1;
	tmpreg &= ~(BTIMx_BTCFG1_GRP2SEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG1_GRP2SEL_Msk);
	BTIMx->BTCFG1 = tmpreg;
}

uint32_t BTIMx_BTCFG1_GRP2SEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG1 & BTIMx_BTCFG1_GRP2SEL_Msk);
}

void BTIMx_BTCFG1_GRP1SEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG1;
	tmpreg &= ~(BTIMx_BTCFG1_GRP1SEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG1_GRP1SEL_Msk);
	BTIMx->BTCFG1 = tmpreg;
}

uint32_t BTIMx_BTCFG1_GRP1SEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG1 & BTIMx_BTCFG1_GRP1SEL_Msk);
}

/* 外部输入信号选择控制2 相关函数 */
void BTIMx_BTCFG2_EXSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG2;
	tmpreg &= ~(BTIMx_BTCFG2_EXSEL2_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG2_EXSEL2_Msk);
	BTIMx->BTCFG2 = tmpreg;
}

uint32_t BTIMx_BTCFG2_EXSEL2_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG2 & BTIMx_BTCFG2_EXSEL2_Msk);
}

/* 外部输入信号选择控制1 相关函数 */
void BTIMx_BTCFG2_EXSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG2;
	tmpreg &= ~(BTIMx_BTCFG2_EXSEL1_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG2_EXSEL1_Msk);
	BTIMx->BTCFG2 = tmpreg;
}

uint32_t BTIMx_BTCFG2_EXSEL1_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG2 & BTIMx_BTCFG2_EXSEL1_Msk);
}

/* 内部输入信号选择控制2 相关函数 */
void BTIMx_BTCFG2_INSEL2_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG2;
	tmpreg &= ~(BTIMx_BTCFG2_INSEL2_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG2_INSEL2_Msk);
	BTIMx->BTCFG2 = tmpreg;
}

uint32_t BTIMx_BTCFG2_INSEL2_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG2 & BTIMx_BTCFG2_INSEL2_Msk);
}

void BTIMx_BTCFG2_INSEL1_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTCFG2;
	tmpreg &= ~(BTIMx_BTCFG2_INSEL1_Msk);
	tmpreg |= (SetValue & BTIMx_BTCFG2_INSEL1_Msk);
	BTIMx->BTCFG2 = tmpreg;
}

uint32_t BTIMx_BTCFG2_INSEL1_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCFG2 & BTIMx_BTCFG2_INSEL1_Msk);
}

/* 输入Group1的预分频寄存器 相关函数 */
void BTIMx_BTPRES_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTPRES = (SetValue & BTIMx_BTPRES_PRESCALE_Msk);
}

uint32_t BTIMx_BTPRES_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTPRES & BTIMx_BTPRES_PRESCALE_Msk);
}

void BTIMx_BTLOADCR_LHEN_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTLOADCR |= (BTIMx_BTLOADCR_LHEN_Msk);
	}
	else
	{
		BTIMx->BTLOADCR &= ~(BTIMx_BTLOADCR_LHEN_Msk);
	}
}

void BTIMx_BTLOADCR_LLEN_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTLOADCR |= (BTIMx_BTLOADCR_LLEN_Msk);
	}
	else
	{
		BTIMx->BTLOADCR &= ~(BTIMx_BTLOADCR_LLEN_Msk);
	}
}

/* 计数器低位计数值寄存器 相关函数 */
uint32_t BTIMx_BTCNTL_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCNTL & BTIMx_BTCNTL_CNTL_Msk);
}

/* 计数器高位计数值寄存器 相关函数 */
uint32_t BTIMx_BTCNTH_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCNTH & BTIMx_BTCNTH_CNTH_Msk);
}

/* 计数器高位预置数寄存器 相关函数 */
void BTIMx_BTPRESET_PRESETH_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTPRESET;
	tmpreg &= ~(BTIMx_BTPRESET_PRESETH_Msk);
	tmpreg |= (SetValue & BTIMx_BTPRESET_PRESETH_Msk);
	BTIMx->BTPRESET = tmpreg;
}

uint32_t BTIMx_BTPRESET_PRESETH_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTPRESET & BTIMx_BTPRESET_PRESETH_Msk);
}

/* 计数器低位预置数寄存器 相关函数 */
void BTIMx_BTPRESET_PRESETL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTPRESET;
	tmpreg &= ~(BTIMx_BTPRESET_PRESETL_Msk);
	tmpreg |= (SetValue & BTIMx_BTPRESET_PRESETL_Msk);
	BTIMx->BTPRESET = tmpreg;
}

uint32_t BTIMx_BTPRESET_PRESETL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTPRESET & BTIMx_BTPRESET_PRESETL_Msk);
}

/* 计数器低位加载寄存器 相关函数 */
void BTIMx_BTLOADL_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTLOADL = (SetValue & BTIMx_BTLOADL_LOADL_Msk);
}

uint32_t BTIMx_BTLOADL_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTLOADL & BTIMx_BTLOADL_LOADL_Msk);
}

/* 计数器高位加载寄存器 相关函数 */
void BTIMx_BTLOADH_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTLOADH = (SetValue & BTIMx_BTLOADH_LOADH_Msk);
}

uint32_t BTIMx_BTLOADH_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTLOADH & BTIMx_BTLOADH_LOADH_Msk);
}

/* 计数器低位比较寄存器 相关函数 */
void BTIMx_BTCMPL_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTCMPL = (SetValue & BTIMx_BTCMPL_CMPL_Msk);
}

uint32_t BTIMx_BTCMPL_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCMPL & BTIMx_BTCMPL_CMPL_Msk);
}

/* 计数器高位比较寄存器 相关函数 */
void BTIMx_BTCMPH_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTCMPH = (SetValue & BTIMx_BTCMPH_CMPH_Msk);
}

uint32_t BTIMx_BTCMPH_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTCMPH & BTIMx_BTCMPH_CMPH_Msk);
}

/* 计数器输出脉冲宽度计数器 相关函数 */
void BTIMx_BTOUTCNT_Write(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	BTIMx->BTOUTCNT = (SetValue & BTIMx_BTOUTCNT_OUTCNT_Msk);
}

uint32_t BTIMx_BTOUTCNT_Read(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTOUTCNT & BTIMx_BTOUTCNT_OUTCNT_Msk);
}

/* 输出清零 相关函数 */
void BTIMx_BTOCR_OUTCLR_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTOCR;
	tmpreg &= ~(BTIMx_BTOCR_OUTCLR_Msk);
	tmpreg |= (SetValue & BTIMx_BTOCR_OUTCLR_Msk);
	BTIMx->BTOCR = tmpreg;
}

uint32_t BTIMx_BTOCR_OUTCLR_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTOCR & BTIMx_BTOCR_OUTCLR_Msk);
}

/* 输出电平反向选择 相关函数 */
void BTIMx_BTOCR_OUTINV_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTOCR;
	tmpreg &= ~(BTIMx_BTOCR_OUTINV_Msk);
	tmpreg |= (SetValue & BTIMx_BTOCR_OUTINV_Msk);
	BTIMx->BTOCR = tmpreg;
}

uint32_t BTIMx_BTOCR_OUTINV_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTOCR & BTIMx_BTOCR_OUTINV_Msk);
}

/* 输出模式选择 相关函数 */
void BTIMx_BTOCR_OUTMOD_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTOCR;
	tmpreg &= ~(BTIMx_BTOCR_OUTMOD_Msk);
	tmpreg |= (SetValue & BTIMx_BTOCR_OUTMOD_Msk);
	BTIMx->BTOCR = tmpreg;
}

uint32_t BTIMx_BTOCR_OUTMOD_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTOCR & BTIMx_BTOCR_OUTMOD_Msk);
}

/* 输出信号源选择 相关函数 */
void BTIMx_BTOCR_OUTSEL_Set(BTIMx_Type* BTIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = BTIMx->BTOCR;
	tmpreg &= ~(BTIMx_BTOCR_OUTSEL_Msk);
	tmpreg |= (SetValue & BTIMx_BTOCR_OUTSEL_Msk);
	BTIMx->BTOCR = tmpreg;
}

uint32_t BTIMx_BTOCR_OUTSEL_Get(BTIMx_Type* BTIMx)
{
	return (BTIMx->BTOCR & BTIMx_BTOCR_OUTSEL_Msk);
}

/* 定时器高位比较发生信号 相关函数 */
void BTIMx_BTIE_CMPHIE_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTIE |= (BTIMx_BTIE_CMPHIE_Msk);
	}
	else
	{
		BTIMx->BTIE &= ~(BTIMx_BTIE_CMPHIE_Msk);
	}
}

FunState BTIMx_BTIE_CMPHIE_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIE & (BTIMx_BTIE_CMPHIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 定时器低位比较发生信号 相关函数 */
void BTIMx_BTIE_CMPLIE_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTIE |= (BTIMx_BTIE_CMPLIE_Msk);
	}
	else
	{
		BTIMx->BTIE &= ~(BTIMx_BTIE_CMPLIE_Msk);
	}
}

FunState BTIMx_BTIE_CMPLIE_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIE & (BTIMx_BTIE_CMPLIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 定时器高位溢出信号 相关函数 */
void BTIMx_BTIE_OVHIE_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTIE |= (BTIMx_BTIE_OVHIE_Msk);
	}
	else
	{
		BTIMx->BTIE &= ~(BTIMx_BTIE_OVHIE_Msk);
	}
}

FunState BTIMx_BTIE_OVHIE_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIE & (BTIMx_BTIE_OVHIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 定时器低位溢出信号 相关函数 */
void BTIMx_BTIE_OVLIE_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTIE |= (BTIMx_BTIE_OVLIE_Msk);
	}
	else
	{
		BTIMx->BTIE &= ~(BTIMx_BTIE_OVLIE_Msk);
	}
}

FunState BTIMx_BTIE_OVLIE_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIE & (BTIMx_BTIE_OVLIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 定时器捕捉产生信号 相关函数 */
void BTIMx_BTIE_CAPIE_Setable(BTIMx_Type* BTIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		BTIMx->BTIE |= (BTIMx_BTIE_CAPIE_Msk);
	}
	else
	{
		BTIMx->BTIE &= ~(BTIMx_BTIE_CAPIE_Msk);
	}
}

FunState BTIMx_BTIE_CAPIE_Getable(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIE & (BTIMx_BTIE_CAPIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 捕捉沿状态 相关函数 */
void BTIMx_BTIF_EDGESTA_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_EDGESTA_Msk;
}

FlagStatus BTIMx_BTIF_EDGESTA_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_EDGESTA_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 高位比较发生信号 相关函数 */
void BTIMx_BTIF_CMPHIF_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_CMPHIF_Msk;
}

FlagStatus BTIMx_BTIF_CMPHIF_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_CMPHIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 低位比较发生信号 相关函数 */
void BTIMx_BTIF_CMPLIF_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_CMPLIF_Msk;
}

FlagStatus BTIMx_BTIF_CMPLIF_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_CMPLIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 定时器高位溢出信号 相关函数 */
void BTIMx_BTIF_OVHIF_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_OVHIF_Msk;
}

FlagStatus BTIMx_BTIF_OVHIF_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_OVHIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 定时器低位溢出信号 相关函数 */
void BTIMx_BTIF_OVLIF_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_OVLIF_Msk;
}

FlagStatus BTIMx_BTIF_OVLIF_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_OVLIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 定时器捕捉产生信号 相关函数 */
void BTIMx_BTIF_CAPIF_Clr(BTIMx_Type* BTIMx)
{
	BTIMx->BTIF = BTIMx_BTIF_CAPIF_Msk;
}

FlagStatus BTIMx_BTIF_CAPIF_Chk(BTIMx_Type* BTIMx)
{
	if (BTIMx->BTIF & BTIMx_BTIF_CAPIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


void BTIMx_Deinit(BTIMx_Type* BTIMx)
{
	BTIMx->BTCR1 = 0x00000000;
	BTIMx->BTCR2 = 0x00000000;
	BTIMx->BTCFG1 = 0x00000000;
	BTIMx->BTCFG2 = 0x00000000;
	BTIMx->BTPRES = 0x00000000;
	BTIMx->BTLOADCR = 0x00000000;
	//BTIMx->BTCNTL = ;
	//BTIMx->BTCNTH = ;
	BTIMx->BTPRESET = 0x00000000;
	BTIMx->BTLOADL = 0x00000000;
	BTIMx->BTLOADH = 0x00000000;
	//BTIMx->BTCMPL = ;
	//BTIMx->BTCMPH = ;
	BTIMx->BTOUTCNT = 0x00000000;
	BTIMx->BTOCR = 0x00000000;
	BTIMx->BTIE = 0x00000000;
	//BTIMx->BTIF = ;
}
//Code_End



void BTIMx_Init(BTIMx_Type* BTIMx, BTIM_InitTypeDef* para)
{
	BTIMx_Deinit(BTIMx);

	//捕捉参数选择
	BTIMx_BTCR1_CAPMOD_Set(BTIMx, para->cap_para.CAPMOD);//捕捉模式控制，脉冲宽度或脉冲周期
	BTIMx_BTCR1_CAPCLR_Set(BTIMx, para->cap_para.CAPCLR);//捕捉是否需要清零计数器
	BTIMx_BTCR1_CAPONCE_Set(BTIMx, para->cap_para.CAPONCE);//捕捉是否单次捕捉

	//信号选择类
	BTIMx_BTCFG1_RTCSEL1_Set(BTIMx, para->sig_src_para.RTCSEL1);//选择RTCOUT1信号选择
	BTIMx_BTCFG1_RTCSEL2_Set(BTIMx, para->sig_src_para.RTCSEL2);//选择RTCOUT2信号选择
	BTIMx_BTCFG2_INSEL1_Set(BTIMx, para->sig_src_para.INSEL1);//选择内部输入信号1
	BTIMx_BTCFG2_INSEL2_Set(BTIMx, para->sig_src_para.INSEL2);//选择内部输入信号2
    BTIMx_BTCFG2_EXSEL1_Set(BTIMx, para->sig_src_para.EXSEL1);//选择外部输入信号1
    BTIMx_BTCFG2_EXSEL2_Set(BTIMx, para->sig_src_para.EXSEL2);//选择外部输入信号2
	
	
	BTIMx_BTCFG1_GRP1SEL_Set(BTIMx, para->sig_src_para.GRP1SEL);//选择GROUP1信号选择
	BTIMx_BTCFG1_GRP2SEL_Set(BTIMx, para->sig_src_para.GRP2SEL);//选择GROUP2信号选择
	
	BTIMx_BTCR2_SIG2SEL_Set(BTIMx, para->sig_src_para.CNTSRC);//选择内部计数源信号
	BTIMx_BTCR2_SIG1SEL_Set(BTIMx, para->sig_src_para.CAPSRC);//选择内部捕捉源信号

	BTIMx_BTCR2_CNTHSEL_Set(BTIMx, para->sig_src_para.CNTHSEL);//高位计数器计数源选择
	BTIMx_BTCR2_SRCSEL_Set(BTIMx, para->sig_src_para.SRCSEL);//选择低位计数器是否长使能

	//DIR控制类
	BTIMx_BTCR2_DIREN_Setable(BTIMx, para->dir_para.DIREN);//外部dir控制或不使能
	//BTIMx_BTCR2_STDIR_Setable(BTIMx, para->dir_para.STDIR);//内部dir控制信号，这个应该不需要
	BTIMx_BTCR2_DIRPO_Set(BTIMx, para->dir_para.DIRPO);//外部输入dir信号极性选择


	//控制类参数
	BTIMx_BTCR1_MODE_Set(BTIMx, para->ctrl_para.MODE);//工作模式，16位捕捉还是8位定时计数
	BTIMx_BTCR1_EDGESEL_Set(BTIMx, para->ctrl_para.EDGESEL);//沿选择，上升沿还是下降沿
	BTIMx_BTPRES_Write(BTIMx, para->ctrl_para.PRESCALE);//选择分频系数

	BTIMx_BTPRESET_PRESETH_Set(BTIMx, para->ctrl_para.PRESETH);//高位预加载值
	BTIMx_BTPRESET_PRESETL_Set(BTIMx, para->ctrl_para.PRESETL);//低位预加载值

	BTIMx_BTLOADH_Write(BTIMx, para->ctrl_para.LOADH);//高位加载值
	BTIMx_BTLOADL_Write(BTIMx, para->ctrl_para.LOADL);//低位加载值
	
	//输出类参数
	BTIMx_BTOCR_OUTMOD_Set(BTIMx, para->out_para.OUTMOD);//输出模式选择
	BTIMx_BTOCR_OUTSEL_Set(BTIMx, para->out_para.OUTSEL);//输出信号选择
	BTIMx_BTOUTCNT_Write(BTIMx, para->out_para.OUTCNT);//输出脉冲宽度设置
	BTIMx_BTOCR_OUTINV_Set(BTIMx, para->out_para.OUTINV);//输出电平反向选择
	
}


