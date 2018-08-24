/**
  ******************************************************************************
  * @file    fm33a0xx_anac.c
  * @author  FM33a0 Application Team
  * @version V2.0.1
  * @date    23-1-2018
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_anac.h" 


/** @addtogroup fm33a0_StdPeriph_Driver
  * @{
  */

/** @defgroup ANAC 
  * @brief ANAC driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//Code_Start
//2018-04-02-13-20-01
//FM33A0XX_Driver_Gen_V1.4

/* PDR下电复位电压配置 相关函数 */
void ANAC_PDRCON_PDRCFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->PDRCON;
	tmpreg &= ~(ANAC_PDRCON_PDRCFG_Msk);
	tmpreg |= (SetValue & ANAC_PDRCON_PDRCFG_Msk);
	ANAC->PDRCON = tmpreg;
}

uint32_t ANAC_PDRCON_PDRCFG_Get(void)
{
	return (ANAC->PDRCON & ANAC_PDRCON_PDRCFG_Msk);
}

/* PDR下电复位使能 相关函数 */
void ANAC_PDRCON_PDREN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->PDRCON |= (ANAC_PDRCON_PDREN_Msk);
	}
	else
	{
		ANAC->PDRCON &= ~(ANAC_PDRCON_PDREN_Msk);
	}
}

FunState ANAC_PDRCON_PDREN_Getable(void)
{
	if (ANAC->PDRCON & (ANAC_PDRCON_PDREN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* BOR下电复位电压配置 相关函数 */
void ANAC_BORCON_BOR_PDRCFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->BORCON;
	tmpreg &= ~(ANAC_BORCON_BOR_PDRCFG_Msk);
	tmpreg |= (SetValue & ANAC_BORCON_BOR_PDRCFG_Msk);
	ANAC->BORCON = tmpreg;
}

uint32_t ANAC_BORCON_BOR_PDRCFG_Get(void)
{
	return (ANAC->BORCON & ANAC_BORCON_BOR_PDRCFG_Msk);
}

/* BOR关闭控制寄存器 相关函数 */
void ANAC_BORCON_OFF_BOR_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->BORCON |= (ANAC_BORCON_OFF_BOR_Msk);
	}
	else
	{
		ANAC->BORCON &= ~(ANAC_BORCON_OFF_BOR_Msk);
	}
}

FunState ANAC_BORCON_OFF_BOR_Getable(void)
{
	if (ANAC->BORCON & (ANAC_BORCON_OFF_BOR_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD电源跌落中断使能 相关函数 */
void ANAC_SVDCFG_PFIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->SVDCFG |= (ANAC_SVDCFG_PFIE_Msk);
	}
	else
	{
		ANAC->SVDCFG &= ~(ANAC_SVDCFG_PFIE_Msk);
	}
}

FunState ANAC_SVDCFG_PFIE_Getable(void)
{
	if (ANAC->SVDCFG & (ANAC_SVDCFG_PFIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD电源恢复中断使能 相关函数 */
void ANAC_SVDCFG_PRIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->SVDCFG |= (ANAC_SVDCFG_PRIE_Msk);
	}
	else
	{
		ANAC->SVDCFG &= ~(ANAC_SVDCFG_PRIE_Msk);
	}
}

FunState ANAC_SVDCFG_PRIE_Getable(void)
{
	if (ANAC->SVDCFG & (ANAC_SVDCFG_PRIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD报警阈值设置 相关函数 */
void ANAC_SVDCFG_SVDLVL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->SVDCFG;
	tmpreg &= ~(ANAC_SVDCFG_SVDLVL_Msk);
	tmpreg |= (SetValue & ANAC_SVDCFG_SVDLVL_Msk);
	ANAC->SVDCFG = tmpreg;
}

uint32_t ANAC_SVDCFG_SVDLVL_Get(void)
{
	return (ANAC->SVDCFG & ANAC_SVDCFG_SVDLVL_Msk);
}

/* 数字滤波使能（SVDMODE=1时必须置1） 相关函数 */
void ANAC_SVDCFG_DFEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->SVDCFG |= (ANAC_SVDCFG_DFEN_Msk);
	}
	else
	{
		ANAC->SVDCFG &= ~(ANAC_SVDCFG_DFEN_Msk);
	}
}

FunState ANAC_SVDCFG_DFEN_Getable(void)
{
	if (ANAC->SVDCFG & (ANAC_SVDCFG_DFEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD工作模式选择 相关函数 */
void ANAC_SVDCFG_SVDMOD_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->SVDCFG;
	tmpreg &= ~(ANAC_SVDCFG_SVDMOD_Msk);
	tmpreg |= (SetValue & ANAC_SVDCFG_SVDMOD_Msk);
	ANAC->SVDCFG = tmpreg;
}

uint32_t ANAC_SVDCFG_SVDMOD_Get(void)
{
	return (ANAC->SVDCFG & ANAC_SVDCFG_SVDMOD_Msk);
}

/* SVD间歇使能间隔 相关函数 */
void ANAC_SVDCFG_SVDITVL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->SVDCFG;
	tmpreg &= ~(ANAC_SVDCFG_SVDITVL_Msk);
	tmpreg |= (SetValue & ANAC_SVDCFG_SVDITVL_Msk);
	ANAC->SVDCFG = tmpreg;
}

uint32_t ANAC_SVDCFG_SVDITVL_Get(void)
{
	return (ANAC->SVDCFG & ANAC_SVDCFG_SVDITVL_Msk);
}

/* SVD测试使能，保持为0 相关函数 */
void ANAC_SVDCON_SVDTE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->SVDCON |= (ANAC_SVDCON_SVDTE_Msk);
	}
	else
	{
		ANAC->SVDCON &= ~(ANAC_SVDCON_SVDTE_Msk);
	}
}

FunState ANAC_SVDCON_SVDTE_Getable(void)
{
	if (ANAC->SVDCON & (ANAC_SVDCON_SVDTE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD使能 相关函数 */
void ANAC_SVDCON_SVDEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->SVDCON |= (ANAC_SVDCON_SVDEN_Msk);
	}
	else
	{
		ANAC->SVDCON &= ~(ANAC_SVDCON_SVDEN_Msk);
	}
}

FunState ANAC_SVDCON_SVDEN_Getable(void)
{
	if (ANAC->SVDCON & (ANAC_SVDCON_SVDEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* SVD电源检测输出(模拟模块直接输出的信号,仅在SVD工作的时候可查询) 相关函数 */
FlagStatus ANAC_SVDSIF_SVDO_Chk(void)
{
	if (ANAC->SVDSIF & ANAC_SVDSIF_SVDO_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 电源跌落中断标志寄存器，电源电压跌落到SVD阈值之下时置位，软件写1清零 相关函数 */
void ANAC_SVDSIF_PFF_Clr(void)
{
	ANAC->SVDSIF = ANAC_SVDSIF_PFF_Msk;
}

FlagStatus ANAC_SVDSIF_PFF_Chk(void)
{
	if (ANAC->SVDSIF & ANAC_SVDSIF_PFF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 电源恢复中断标志寄存器，电源电压上升到SVD阈值之上时置位，软件写1清零 相关函数 */
void ANAC_SVDSIF_PRF_Clr(void)
{
	ANAC->SVDSIF = ANAC_SVDSIF_PRF_Msk;
}

FlagStatus ANAC_SVDSIF_PRF_Chk(void)
{
	if (ANAC->SVDSIF & ANAC_SVDSIF_PRF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* SVD内部滤波后的电压检测标志，仅在使能数字滤波时有意义；软件避免写此寄存器。 相关函数 */
FlagStatus ANAC_SVDALM_SVDALM_B_Chk(void)
{
	if (ANAC->SVDALM & ANAC_SVDALM_SVDALM_B_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* XTLF停振检测报警中断使能,上电默认关闭，避免刚上电时晶振未起振触发中断
 相关函数 */
void ANAC_FDETIE_FDET_IE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->FDETIE |= (ANAC_FDETIE_FDET_IE_Msk);
	}
	else
	{
		ANAC->FDETIE &= ~(ANAC_FDETIE_FDET_IE_Msk);
	}
}

FunState ANAC_FDETIE_FDET_IE_Getable(void)
{
	if (ANAC->FDETIE & (ANAC_FDETIE_FDET_IE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 停振检测模块输出 相关函数 */
FlagStatus ANAC_FDETIF_FDETO_Chk(void)
{
	if (ANAC->FDETIF & ANAC_FDETIF_FDETO_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 停振检测中断标志寄存器，XTLF停振时硬件异步置位，软件写1清零；只有在FDETO不为0的情况下才能够清除此寄存器 相关函数 */
void ANAC_FDETIF_FDETIF_Clr(void)
{
	ANAC->FDETIF = ANAC_FDETIF_FDETIF_Msk;
}

FlagStatus ANAC_FDETIF_FDETIF_Chk(void)
{
	if (ANAC->FDETIF & ANAC_FDETIF_FDETIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* ADC中断使能 相关函数 */
void ANAC_ADCCON_ADC_IE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->ADCCON |= (ANAC_ADCCON_ADC_IE_Msk);
	}
	else
	{
		ANAC->ADCCON &= ~(ANAC_ADCCON_ADC_IE_Msk);
	}
}

FunState ANAC_ADCCON_ADC_IE_Getable(void)
{
	if (ANAC->ADCCON & (ANAC_ADCCON_ADC_IE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* ADC通道选择 相关函数 */
void ANAC_ADCCON_ADC_VANA_EN_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->ADCCON;
	tmpreg &= ~(ANAC_ADCCON_ADC_VANA_EN_Msk);
	tmpreg |= (SetValue & ANAC_ADCCON_ADC_VANA_EN_Msk);
	ANAC->ADCCON = tmpreg;
}

uint32_t ANAC_ADCCON_ADC_VANA_EN_Get(void)
{
	return (ANAC->ADCCON & ANAC_ADCCON_ADC_VANA_EN_Msk);
}

/* ADC使能信号 相关函数 */
void ANAC_ADCCON_ADC_EN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->ADCCON |= (ANAC_ADCCON_ADC_EN_Msk);
	}
	else
	{
		ANAC->ADCCON &= ~(ANAC_ADCCON_ADC_EN_Msk);
	}
}

FunState ANAC_ADCCON_ADC_EN_Getable(void)
{
	if (ANAC->ADCCON & (ANAC_ADCCON_ADC_EN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* ADC TRIM值,从出厂参数区获取 相关函数 */
void ANAC_ADCTRIM_Write(uint32_t SetValue)
{
	ANAC->ADCTRIM = (SetValue & ANAC_ADCTRIM_ADC_TRIM_Msk);
}

uint32_t ANAC_ADCTRIM_Read(void)
{
	return (ANAC->ADCTRIM & ANAC_ADCTRIM_ADC_TRIM_Msk);
}

/* ADC输出数据寄存器 相关函数 */
uint32_t ANAC_ADCDATA_Read(void)
{
	return (ANAC->ADCDATA & ANAC_ADCDATA_ADC_DATA_Msk);
}

/* ADC转换完成中断标志，硬件置位，软件写1清零，写0无效 相关函数 */
void ANAC_ADCIF_ADC_IF_Clr(void)
{
	ANAC->ADCIF = ANAC_ADCIF_ADC_IF_Msk;
}

FlagStatus ANAC_ADCIF_ADC_IF_Chk(void)
{
	if (ANAC->ADCIF & ANAC_ADCIF_ADC_IF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* ADC输入通道Buffer使能 相关函数 */
void ANAC_ADCINSEL_BUFEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->ADCINSEL |= (ANAC_ADCINSEL_BUFEN_Msk);
	}
	else
	{
		ANAC->ADCINSEL &= ~(ANAC_ADCINSEL_BUFEN_Msk);
	}
}

FunState ANAC_ADCINSEL_BUFEN_Getable(void)
{
	if (ANAC->ADCINSEL & (ANAC_ADCINSEL_BUFEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* ADC输入Buffer Bypass 相关函数 */
void ANAC_ADCINSEL_BUFBYP_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->ADCINSEL |= (ANAC_ADCINSEL_BUFBYP_Msk);
	}
	else
	{
		ANAC->ADCINSEL &= ~(ANAC_ADCINSEL_BUFBYP_Msk);
	}
}

FunState ANAC_ADCINSEL_BUFBYP_Getable(void)
{
	if (ANAC->ADCINSEL & (ANAC_ADCINSEL_BUFBYP_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* ADC输入通道选择 相关函数 */
void ANAC_ADCINSEL_BUFSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->ADCINSEL;
	tmpreg &= ~(ANAC_ADCINSEL_BUFSEL_Msk);
	tmpreg |= (SetValue & ANAC_ADCINSEL_BUFSEL_Msk);
	ANAC->ADCINSEL = tmpreg;
}

uint32_t ANAC_ADCINSEL_BUFSEL_Get(void)
{
	return (ANAC->ADCINSEL & ANAC_ADCINSEL_BUFSEL_Msk);
}

/* TRNG控制 相关函数 */
void ANAC_TRNGCON_TRNGEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->TRNGCON |= (ANAC_TRNGCON_TRNGEN_Msk);
	}
	else
	{
		ANAC->TRNGCON &= ~(ANAC_TRNGCON_TRNGEN_Msk);
	}
}

FunState ANAC_TRNGCON_TRNGEN_Getable(void)
{
	if (ANAC->TRNGCON & (ANAC_TRNGCON_TRNGEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器1输出，软件只读 相关函数 */
FlagStatus ANAC_COMP1CR_CMP1O_Chk(void)
{
	if (ANAC->COMP1CR & ANAC_COMP1CR_CMP1O_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 比较器1正极输入选择 相关函数 */
void ANAC_COMP1CR_V1PSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMP1CR;
	tmpreg &= ~(ANAC_COMP1CR_V1PSEL_Msk);
	tmpreg |= (SetValue & ANAC_COMP1CR_V1PSEL_Msk);
	ANAC->COMP1CR = tmpreg;
}

uint32_t ANAC_COMP1CR_V1PSEL_Get(void)
{
	return (ANAC->COMP1CR & ANAC_COMP1CR_V1PSEL_Msk);
}

/* 比较器1负极输入选择 相关函数 */
void ANAC_COMP1CR_V1NSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMP1CR;
	tmpreg &= ~(ANAC_COMP1CR_V1NSEL_Msk);
	tmpreg |= (SetValue & ANAC_COMP1CR_V1NSEL_Msk);
	ANAC->COMP1CR = tmpreg;
}

uint32_t ANAC_COMP1CR_V1NSEL_Get(void)
{
	return (ANAC->COMP1CR & ANAC_COMP1CR_V1NSEL_Msk);
}

/* 比较器1使能位 相关函数 */
void ANAC_COMP1CR_CMP1EN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMP1CR |= (ANAC_COMP1CR_CMP1EN_Msk);
	}
	else
	{
		ANAC->COMP1CR &= ~(ANAC_COMP1CR_CMP1EN_Msk);
	}
}

FunState ANAC_COMP1CR_CMP1EN_Getable(void)
{
	if (ANAC->COMP1CR & (ANAC_COMP1CR_CMP1EN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器2输出，软件只读 相关函数 */
FlagStatus ANAC_COMP2CR_CMP2O_Chk(void)
{
	if (ANAC->COMP2CR & ANAC_COMP2CR_CMP2O_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 比较器2正极输入选择 相关函数 */
void ANAC_COMP2CR_V2PSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMP2CR;
	tmpreg &= ~(ANAC_COMP2CR_V2PSEL_Msk);
	tmpreg |= (SetValue & ANAC_COMP2CR_V2PSEL_Msk);
	ANAC->COMP2CR = tmpreg;
}

uint32_t ANAC_COMP2CR_V2PSEL_Get(void)
{
	return (ANAC->COMP2CR & ANAC_COMP2CR_V2PSEL_Msk);
}

/* 比较器2负极输入选择 相关函数 */
void ANAC_COMP2CR_V2NSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMP2CR;
	tmpreg &= ~(ANAC_COMP2CR_V2NSEL_Msk);
	tmpreg |= (SetValue & ANAC_COMP2CR_V2NSEL_Msk);
	ANAC->COMP2CR = tmpreg;
}

uint32_t ANAC_COMP2CR_V2NSEL_Get(void)
{
	return (ANAC->COMP2CR & ANAC_COMP2CR_V2NSEL_Msk);
}

/* 比较器2使能位 相关函数 */
void ANAC_COMP2CR_CMP2EN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMP2CR |= (ANAC_COMP2CR_CMP2EN_Msk);
	}
	else
	{
		ANAC->COMP2CR &= ~(ANAC_COMP2CR_CMP2EN_Msk);
	}
}

FunState ANAC_COMP2CR_CMP2EN_Getable(void)
{
	if (ANAC->COMP2CR & (ANAC_COMP2CR_CMP2EN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器2数字滤波使能 相关函数 */
void ANAC_COMPICR_CMP2DF_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR |= (ANAC_COMPICR_CMP2DF_Msk);
	}
	else
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_CMP2DF_Msk);
	}
}

FunState ANAC_COMPICR_CMP2DF_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_CMP2DF_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器1数字滤波使能 相关函数 */
void ANAC_COMPICR_CMP1DF_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR |= (ANAC_COMPICR_CMP1DF_Msk);
	}
	else
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_CMP1DF_Msk);
	}
}

FunState ANAC_COMPICR_CMP1DF_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_CMP1DF_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器Buffer Bypass 相关函数 */
void ANAC_COMPICR_BUFBYP_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR |= (ANAC_COMPICR_BUFBYP_Msk);
	}
	else
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_BUFBYP_Msk);
	}
}

FunState ANAC_COMPICR_BUFBYP_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_BUFBYP_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器Buffer使能 相关函数 */
void ANAC_COMPICR_BUFENB_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_BUFENB_Msk);
	}
	else
	{
		ANAC->COMPICR |= (ANAC_COMPICR_BUFENB_Msk);
	}
}

FunState ANAC_COMPICR_BUFENB_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_BUFENB_Msk))
	{
		return DISABLE;
	}
	else
	{
		return ENABLE;
	}
}

/* 比较器2中断源选择 相关函数 */
void ANAC_COMPICR_CMP2SEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMPICR;
	tmpreg &= ~(ANAC_COMPICR_CMP2SEL_Msk);
	tmpreg |= (SetValue & ANAC_COMPICR_CMP2SEL_Msk);
	ANAC->COMPICR = tmpreg;
}

uint32_t ANAC_COMPICR_CMP2SEL_Get(void)
{
	return (ANAC->COMPICR & ANAC_COMPICR_CMP2SEL_Msk);
}

/* 比较器1中断源选择 相关函数 */
void ANAC_COMPICR_CMP1SEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ANAC->COMPICR;
	tmpreg &= ~(ANAC_COMPICR_CMP1SEL_Msk);
	tmpreg |= (SetValue & ANAC_COMPICR_CMP1SEL_Msk);
	ANAC->COMPICR = tmpreg;
}

uint32_t ANAC_COMPICR_CMP1SEL_Get(void)
{
	return (ANAC->COMPICR & ANAC_COMPICR_CMP1SEL_Msk);
}

/* 比较器2中断使能 相关函数 */
void ANAC_COMPICR_CMP2IE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR |= (ANAC_COMPICR_CMP2IE_Msk);
	}
	else
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_CMP2IE_Msk);
	}
}

FunState ANAC_COMPICR_CMP2IE_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_CMP2IE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器1中断使能 相关函数 */
void ANAC_COMPICR_CMP1IE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		ANAC->COMPICR |= (ANAC_COMPICR_CMP1IE_Msk);
	}
	else
	{
		ANAC->COMPICR &= ~(ANAC_COMPICR_CMP1IE_Msk);
	}
}

FunState ANAC_COMPICR_CMP1IE_Getable(void)
{
	if (ANAC->COMPICR & (ANAC_COMPICR_CMP1IE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较器2中断标志，硬件置位，软件写1清零 相关函数 */
void ANAC_COMPIF_CMP2IF_Clr(void)
{
	ANAC->COMPIF = ANAC_COMPIF_CMP2IF_Msk;
}

FlagStatus ANAC_COMPIF_CMP2IF_Chk(void)
{
	if (ANAC->COMPIF & ANAC_COMPIF_CMP2IF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 比较器1中断标志，硬件置位，软件写1清零 相关函数 */
void ANAC_COMPIF_CMP1IF_Clr(void)
{
	ANAC->COMPIF = ANAC_COMPIF_CMP1IF_Msk;
}

FlagStatus ANAC_COMPIF_CMP1IF_Chk(void)
{
	if (ANAC->COMPIF & ANAC_COMPIF_CMP1IF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

void ANAC_Deinit(void)
{
	ANAC->PDRCON = 0x00000003;
	ANAC->BORCON = 0x00000002;
	ANAC->SVDCFG = 0x00000008;
	ANAC->SVDCON = 0x00000000;
	//ANAC->SVDSIF = ;
	//ANAC->SVDALM = ;
	ANAC->FDETIE = 0x00000000;
	//ANAC->FDETIF = ;
	ANAC->ADCCON = 0x00000000;
	ANAC->ADCTRIM = 0x00000000;
	//ANAC->ADCDATA = ;
	//ANAC->ADCIF = ;
	ANAC->ADCINSEL = 0x00000000;
	ANAC->TRNGCON = 0x00000000;
	ANAC->COMP1CR = 0x00000000;
	ANAC->COMP2CR = 0x00000000;
	ANAC->COMPICR = 0x00000040;
	//ANAC->COMPIF = ;
}
//Code_End



/*SVD 初始化配置函数*/
void ANAC_SVD_Init(ANAC_SVD_InitTypeDef* para)
{
	ANAC_SVDCFG_SVDMOD_Set(para->SVDMOD);	//SVD工作模式选择
	ANAC_SVDCFG_SVDITVL_Set(para->SVDITVL);	//SVD间歇使能间隔设置
	ANAC_SVDCFG_SVDLVL_Set(para->SVDLVL);	//SVD报警阈值设置
	ANAC_SVDCFG_DFEN_Setable(para->DFEN);	//SVD数字滤波设置（SVDMODE=1时必须置1）
	ANAC_SVDCFG_PFIE_Setable(para->PFIE);	//SVD电源跌落中断设置
	ANAC_SVDCFG_PRIE_Setable(para->PRIE);	//SVD电源恢复中断设置
	ANAC_SVDCON_SVDTE_Setable(DISABLE);	//关闭测试功能
	ANAC_SVDCON_SVDEN_Setable(para->SVDEN);	//SVD使能设置
}

/*ADC 初始化配置函数*/
void ANAC_ADC_Init(ANAC_ADC_InitTypeDef* para)
{
	ANAC_ADCTRIM_Write(para->ADC_TRIM);				//<等效参考电压调校寄存器
	ANAC_ADCCON_ADC_VANA_EN_Set(para->ADC_VANA_EN);	//<内部通道、外部通道选择
	ANAC_ADCINSEL_BUFSEL_Set(para->BUFSEL);			//<ADC输入通道选择
	ANAC_ADCINSEL_BUFBYP_Setable(para->BUFBYP);		//<ADC输入Buffer Bypass，ADC输入Buffer Bypass，使用ADC测量外部信号输入时，关闭bypass， 使用ADC测量内部信号时，必须将此位置1（bypass）
	ANAC_ADCINSEL_BUFEN_Setable(para->BUFEN);		//<ADC输入通道Buffer使能，常开即可
	ANAC_ADCCON_ADC_IE_Setable(para->ADC_IE);		//<ADC中断使能
	ANAC_ADCCON_ADC_EN_Setable(para->ADC_EN);		//<ADC使能
}

/*ADC 测量通道配置函数，简化型*/
void ANAC_ADC_Channel_SetEx(uint8_t ChSel_def)
{
	switch( ChSel_def )
	{
		case CH_PTAT://温度传感器通道
			ANAC_ADCCON_ADC_VANA_EN_Set(ANAC_ADCCON_ADC_VANA_EN_PTAT);	//测温	
			ANAC_ADCTRIM_Write(const_adc_TrimT);						//配置参考电压
			ANAC_ADCINSEL_BUFBYP_Setable(ENABLE);	//内部信号测量，bypassbuffer
			ANAC_ADCINSEL_BUFEN_Setable(ENABLE);	//
			break;

		case CH_VDD://电源VDD通道
			ANAC_ADCCON_ADC_VANA_EN_Set(ANAC_ADCCON_ADC_VANA_EN_VOLTAGE);	//测VDD电压，注意ADC量程为4.5V（2.2mV*0x7FF）,高于4.5V的电压返回0x7FF
			ANAC_ADCTRIM_Write(const_adc_TrimV);							//配置参考电压
			ANAC_ADCINSEL_BUFBYP_Setable(ENABLE);		//内部输入
			ANAC_ADCINSEL_BUFEN_Setable(ENABLE);		//电压
			ANAC_ADCINSEL_BUFSEL_Set(ANAC_ADCINSEL_BUFSEL_VDD);//vdd
			break;
		
		case CH_IN1://外部输入通道
		case CH_IN2:
		case CH_IN3:
		case CH_IN4:
		case CH_IN5:
		case CH_IN6:
		case CH_IN7:
		case CH_IN8:
			ANAC_ADCCON_ADC_VANA_EN_Set(ANAC_ADCCON_ADC_VANA_EN_VOLTAGE);	//测电压，输入电压不可高于芯片VDD，注意ADC量程为4.5V（2.2mV*0x7FF）,高于4.5V的电压返回0x7FF
			ANAC_ADCTRIM_Write(const_adc_TrimV);							//配置参考电压
			ANAC_ADCINSEL_BUFBYP_Setable(DISABLE);	//外部信号测量，关闭bypassbuffer
			ANAC_ADCINSEL_BUFEN_Setable(ENABLE);	//打开bufferen
			ANAC_ADCINSEL_BUFSEL_Set((ANAC_ADCINSEL_BUFSEL_ADC_IN1 + (ChSel_def - CH_IN1)));//输入通道选择
			break;
		
		default:
			break;
	}
}


/*ADC 电压计算函数
	输入：AD值	 float型（方便处理平均值）
	输出：电压@mV float型
*/
//被测电压低于100mV后测试结果不太准确
//被测电压超过4.4V后测试结果不太准确
//被测电压不可超过电源电压
//被测电压不可超过4.5V
float ANAC_ADC_VoltageCalc(float fADCData)
{
	float fVolt = 0;
		
	if( (const_adc_Slope > 2000) && (const_adc_Slope < 2400) )
	{
		//使用成测数据	
		//ADC值换算为电压值
//		fVolt = fADCData*((const_adc_Slope/1000.0))+(float)const_adc_Offset/100.0;	
		if(fADCData <= 30)
			fVolt = 0;
		else if(fADCData <= 50)
			fVolt = (fADCData-30)*5;
		else if(fADCData <= 150)
			fVolt = (fADCData*(const_adc_Slope/1000.0));
		else if(fADCData <= 500)
			fVolt = (fADCData*0.98*(const_adc_Slope/1000.0));
		else
			fVolt = (fADCData*(const_adc_Slope/1000.0))+const_adc_Offset/100.0;	
	}
	else
	{
		//调试临时使用
		fVolt = (fADCData - 10)*2.174;
	}
	
	return fVolt;
}

/*ADC 温度计算函数
	输入：AD值	 float型（方便处理平均值）
	输入：电源电压 uint08型 仅支持3/5V，用于做电压补偿
	输出：温度@℃ float型
*/
float ANAC_ADC_TemperatureCalc(float fADCData ,uint8_t Vdd)
{
	float fTestADC, fTestT, fFa;
	float fTemperature;
		
	if( const_temp_TestA == 0x1E )
	{
		//使用温度定标数据
		fTestT = const_temp_TestA+(float)(const_temp_TestB/0x80)*0.5;
		if( Vdd == 3 )
		{
			fTestADC = const_adc_Test  - const_T_Offset3V*const_TmpsH;
		}
		else
		{			
			fTestADC = const_adc_Test  - const_T_Offset5V*const_TmpsH;
		}
	}
	else
	{
		//调试临时使用
		fTestT = 30;
		fTestADC = 1500;
	}
	
	if( fADCData > (fTestADC-25) )	//高温
		fFa = const_TmpsH;
	else
		fFa = const_TmpsL;

	fTemperature = (fADCData - fTestADC)/fFa + fTestT;
	
	return fTemperature;
}


/*COMP 初始化配置函数*/
void ANAC_COMPx_Init(ANAC_COMPx_InitTypeDef* para)
{
	if(para->COMPx == 1)//比较器1
	{
		ANAC_COMP1CR_V1PSEL_Set(para->VxPSEL);		//比较器x正极输入选择
		ANAC_COMP1CR_V1NSEL_Set(para->VxNSEL);		//比较器x负极输入选择
		ANAC_COMPICR_CMP1SEL_Set(para->CMPxSEL);	//比较器x中断源选择
		ANAC_COMPICR_CMP1IE_Setable(para->CMPxIE);	//较器x中断使能
		ANAC_COMPICR_CMP1DF_Setable(para->CMPxDF);	//比较器x数字滤波使能
		ANAC_COMP1CR_CMP1EN_Setable(para->CMPxEN);	//比较器x使能
	}
	else		//比较器2
	{
		ANAC_COMP2CR_V2PSEL_Set(para->VxPSEL);		//比较器x正极输入选择
		ANAC_COMP2CR_V2NSEL_Set(para->VxNSEL);		//比较器x负极输入选择
		ANAC_COMPICR_CMP2SEL_Set(para->CMPxSEL);	//比较器x中断源选择
		ANAC_COMPICR_CMP2IE_Setable(para->CMPxIE);	//较器x中断使能
		ANAC_COMPICR_CMP2DF_Setable(para->CMPxDF);	//比较器x数字滤波使能
		ANAC_COMP2CR_CMP2EN_Setable(para->CMPxEN);	//比较器x使能	
	}
}


/******END OF FILE****/



