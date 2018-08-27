/**
  ******************************************************************************
  * @file    fm33a0xx_etim.c
  * @author  FM33a0 Application Team
  * @version V2.0.1
  * @date    26-2-2018
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_etim.h"  


/** @addtogroup fm33a0_StdPeriph_Driver
  * @{
  */

/** @defgroup ETIM 
  * @brief ETIM driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//Code_Start
//2018-04-02-13-21-03
//FM33A0XX_Driver_Gen_V1.4

/* 引脚输入数字滤波使能 相关函数 */
void ETIMx_ETxCR_EXFLT_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_EXFLT_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_EXFLT_Msk);
	}
}

FunState ETIMx_ETxCR_EXFLT_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_EXFLT_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* PWM输出控制 相关函数 */
void ETIMx_ETxCR_PWM_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_PWM_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_PWM_Msk);
	}
}

FunState ETIMx_ETxCR_PWM_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_PWM_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 启动控制 相关函数 */
void ETIMx_ETxCR_CEN_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_CEN_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_CEN_Msk);
	}
}

FunState ETIMx_ETxCR_CEN_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_CEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 工作模式选择 相关函数 */
void ETIMx_ETxCR_MOD_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxCR;
	tmpreg &= ~(ETIMx_ETxCR_MOD_Msk);
	tmpreg |= (SetValue & ETIMx_ETxCR_MOD_Msk);
	ETIMx->ETxCR = tmpreg;
}

uint32_t ETIMx_ETxCR_MOD_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxCR & ETIMx_ETxCR_MOD_Msk);
}

/* 扩展定时器级联使能 相关函数 */
void ETIMx_ETxCR_CASEN_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_CASEN_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_CASEN_Msk);
	}
}

FunState ETIMx_ETxCR_CASEN_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_CASEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） 相关函数 */
void ETIMx_ETxCR_EDGESEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxCR;
	tmpreg &= ~(ETIMx_ETxCR_EDGESEL_Msk);
	tmpreg |= (SetValue & ETIMx_ETxCR_EDGESEL_Msk);
	ETIMx->ETxCR = tmpreg;
}

uint32_t ETIMx_ETxCR_EDGESEL_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxCR & ETIMx_ETxCR_EDGESEL_Msk);
}

/* 捕捉模式控制 相关函数 */
void ETIMx_ETxCR_CAPMOD_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxCR;
	tmpreg &= ~(ETIMx_ETxCR_CAPMOD_Msk);
	tmpreg |= (SetValue & ETIMx_ETxCR_CAPMOD_Msk);
	ETIMx->ETxCR = tmpreg;
}

uint32_t ETIMx_ETxCR_CAPMOD_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxCR & ETIMx_ETxCR_CAPMOD_Msk);
}

/* 带清零捕捉模式控制 相关函数 */
void ETIMx_ETxCR_CAPCLR_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_CAPCLR_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_CAPCLR_Msk);
	}
}

FunState ETIMx_ETxCR_CAPCLR_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_CAPCLR_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 单次捕捉控制 相关函数 */
void ETIMx_ETxCR_CAPONCE_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxCR |= (ETIMx_ETxCR_CAPONCE_Msk);
	}
	else
	{
		ETIMx->ETxCR &= ~(ETIMx_ETxCR_CAPONCE_Msk);
	}
}

FunState ETIMx_ETxCR_CAPONCE_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxCR & (ETIMx_ETxCR_CAPONCE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 捕捉沿选择 相关函数 */
void ETIMx_ETxCR_CAPEDGE_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxCR;
	tmpreg &= ~(ETIMx_ETxCR_CAPEDGE_Msk);
	tmpreg |= (SetValue & ETIMx_ETxCR_CAPEDGE_Msk);
	ETIMx->ETxCR = tmpreg;
}

uint32_t ETIMx_ETxCR_CAPEDGE_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxCR & ETIMx_ETxCR_CAPEDGE_Msk);
}

/* 内部信号2源选择(捕捉源) 相关函数 */
void ETIMx_ETxINSEL_SIG2SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxINSEL;
	tmpreg &= ~(ETIMx_ETxINSEL_SIG2SEL_Msk);
	tmpreg |= (SetValue & ETIMx_ETxINSEL_SIG2SEL_Msk);
	ETIMx->ETxINSEL = tmpreg;
}

uint32_t ETIMx_ETxINSEL_SIG2SEL_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxINSEL & ETIMx_ETxINSEL_SIG2SEL_Msk);
}

/* 内部信号1源选择（在计数模式下计数源仅由此选择，捕捉模式下计数源） 相关函数 */
void ETIMx_ETxINSEL_SIG1SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxINSEL;
	tmpreg &= ~(ETIMx_ETxINSEL_SIG1SEL_Msk);
	tmpreg |= (SetValue & ETIMx_ETxINSEL_SIG1SEL_Msk);
	ETIMx->ETxINSEL = tmpreg;
}

uint32_t ETIMx_ETxINSEL_SIG1SEL_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxINSEL & ETIMx_ETxINSEL_SIG1SEL_Msk);
}

/* GROUP2 信号选择控制 相关函数 */
void ETIMx_ETxINSEL_GRP2SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxINSEL;
	tmpreg &= ~(ETIMx_ETxINSEL_GRP2SEL_Msk);
	tmpreg |= (SetValue & ETIMx_ETxINSEL_GRP2SEL_Msk);
	ETIMx->ETxINSEL = tmpreg;
}

uint32_t ETIMx_ETxINSEL_GRP2SEL_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxINSEL & ETIMx_ETxINSEL_GRP2SEL_Msk);
}

/* GROUP1 信号选择控制 相关函数 */
void ETIMx_ETxINSEL_GRP1SEL_Set(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = ETIMx->ETxINSEL;
	tmpreg &= ~(ETIMx_ETxINSEL_GRP1SEL_Msk);
	tmpreg |= (SetValue & ETIMx_ETxINSEL_GRP1SEL_Msk);
	ETIMx->ETxINSEL = tmpreg;
}

uint32_t ETIMx_ETxINSEL_GRP1SEL_Get(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxINSEL & ETIMx_ETxINSEL_GRP1SEL_Msk);
}

/* ETx预分频寄存器1 相关函数 */
void ETIMx_ETxPESCALE1_Write(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	ETIMx->ETxPESCALE1 = (SetValue & ETIMx_ETxPESCALE1_PRESCALE1_Msk);
}

uint32_t ETIMx_ETxPESCALE1_Read(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxPESCALE1 & ETIMx_ETxPESCALE1_PRESCALE1_Msk);
}

/* ETx预分频寄存器2 相关函数 */
void ETIMx_ETxPESCALE2_Write(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	ETIMx->ETxPESCALE2 = (SetValue & ETIMx_ETxPESCALE2_PRESCALE2_Msk);
}

uint32_t ETIMx_ETxPESCALE2_Read(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxPESCALE2 & ETIMx_ETxPESCALE2_PRESCALE2_Msk);
}

/* ETx初值寄存器 相关函数 */
void ETIMx_ETxIVR_Write(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	ETIMx->ETxIVR = (SetValue & ETIMx_ETxIVR_INITVALUE_Msk);
}

uint32_t ETIMx_ETxIVR_Read(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxIVR & ETIMx_ETxIVR_INITVALUE_Msk);
}

/* ETx比较寄存器 相关函数 */
void ETIMx_ETxCMP_Write(ETIMx_Type* ETIMx, uint32_t SetValue)
{
	ETIMx->ETxCMP = (SetValue & ETIMx_ETxCMP_CMP_Msk);
}

uint32_t ETIMx_ETxCMP_Read(ETIMx_Type* ETIMx)
{
	return (ETIMx->ETxCMP & ETIMx_ETxCMP_CMP_Msk);
}

/* 扩展定时器比较中断使能 相关函数 */
void ETIMx_ETxIE_CMPIE_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxIE |= (ETIMx_ETxIE_CMPIE_Msk);
	}
	else
	{
		ETIMx->ETxIE &= ~(ETIMx_ETxIE_CMPIE_Msk);
	}
}

FunState ETIMx_ETxIE_CMPIE_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIE & (ETIMx_ETxIE_CMPIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 扩展定时器捕捉中断使能 相关函数 */
void ETIMx_ETxIE_CAPIE_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxIE |= (ETIMx_ETxIE_CAPIE_Msk);
	}
	else
	{
		ETIMx->ETxIE &= ~(ETIMx_ETxIE_CAPIE_Msk);
	}
}

FunState ETIMx_ETxIE_CAPIE_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIE & (ETIMx_ETxIE_CAPIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 扩展定时器溢出中断使能 相关函数 */
void ETIMx_ETxIE_OVIE_Setable(ETIMx_Type* ETIMx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		ETIMx->ETxIE |= (ETIMx_ETxIE_OVIE_Msk);
	}
	else
	{
		ETIMx->ETxIE &= ~(ETIMx_ETxIE_OVIE_Msk);
	}
}

FunState ETIMx_ETxIE_OVIE_Getable(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIE & (ETIMx_ETxIE_OVIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 比较状态中断标志 相关函数 */
void ETIMx_ETxIF_CMPIF_Clr(ETIMx_Type* ETIMx)
{
	ETIMx->ETxIF = ETIMx_ETxIF_CMPIF_Msk;
}

FlagStatus ETIMx_ETxIF_CMPIF_Chk(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIF & ETIMx_ETxIF_CMPIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 捕捉沿状态标志 相关函数 */
FlagStatus ETIMx_ETxIF_EDGESTA_Chk(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIF & ETIMx_ETxIF_EDGESTA_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 扩展定时器捕捉产生中断标志 相关函数 */
void ETIMx_ETxIF_CAPIF_Clr(ETIMx_Type* ETIMx)
{
	ETIMx->ETxIF = ETIMx_ETxIF_CAPIF_Msk;
}

FlagStatus ETIMx_ETxIF_CAPIF_Chk(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIF & ETIMx_ETxIF_CAPIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 扩展定时器溢出中断标志 相关函数 */
void ETIMx_ETxIF_OVIF_Clr(ETIMx_Type* ETIMx)
{
	ETIMx->ETxIF = ETIMx_ETxIF_OVIF_Msk;
}

FlagStatus ETIMx_ETxIF_OVIF_Chk(ETIMx_Type* ETIMx)
{
	if (ETIMx->ETxIF & ETIMx_ETxIF_OVIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


void ETIMx_Deinit(ETIMx_Type* ETIMx)
{
	ETIMx->ETxCR = 0x00000000;
	ETIMx->ETxINSEL = 0x00000000;
	ETIMx->ETxPESCALE1 = 0x00000000;
	ETIMx->ETxPESCALE2 = 0x00000000;
	ETIMx->ETxIVR = 0x00000000;
	ETIMx->ETxCMP = 0x00000000;
	ETIMx->ETxIE = 0x00000000;
	//ETIMx->ETxIF = 0x00000000;
}
//Code_End


/*ETIMx 初始化配置函数*/
void ETIMx_Init(ETIMx_Type* ETIMx, ETIM_InitTypeDef* para)
{
	//信号源参数
	ETIMx_ETxINSEL_SIG1SEL_Set(ETIMx, para->sig_src_para.SIG1SEL);		/* 内部信号1源选择（在计数模式下计数源仅由此选择，捕捉模式下计数源） */
	ETIMx_ETxINSEL_SIG2SEL_Set(ETIMx, para->sig_src_para.SIG2SEL);		/* 内部信号2源选择(捕捉源) */
	ETIMx_ETxINSEL_GRP1SEL_Set(ETIMx, para->sig_src_para.GRP1SEL);		/* GROUP1 信号选择控制 */
	ETIMx_ETxINSEL_GRP2SEL_Set(ETIMx, para->sig_src_para.GRP2SEL);		/* GROUP2 信号选择控制 */
	ETIMx_ETxPESCALE1_Write(ETIMx, para->sig_src_para.PRESCALE1);		/* ETx预分频寄存器1 */
	ETIMx_ETxPESCALE2_Write(ETIMx, para->sig_src_para.PRESCALE2);		/* ETx预分频寄存器2 */
	
	//控制类参数
	ETIMx_ETxCR_EXFLT_Setable(ETIMx, para->ctrl_para.EXFLT);		/* 引脚输入数字滤波使能 */
	ETIMx_ETxCR_MOD_Set(ETIMx, para->ctrl_para.MOD);				/* 工作模式选择 */
	ETIMx_ETxCR_CASEN_Setable(ETIMx, para->ctrl_para.CASEN);		/* 扩展定时器级联使能 */
	ETIMx_ETxCR_EDGESEL_Set(ETIMx, para->ctrl_para.EDGESEL);		/* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） */

	ETIMx_ETxCR_PWM_Setable(ETIMx, para->ctrl_para.PWM);		/* PWM输出控制 */

	ETIMx_ETxCR_CAPMOD_Set(ETIMx, para->ctrl_para.CAPMOD);			/* 捕捉模式控制 */
	ETIMx_ETxCR_CAPCLR_Setable(ETIMx, para->ctrl_para.CAPCLR);		/* 带清零捕捉模式控制 */
	ETIMx_ETxCR_CAPONCE_Setable(ETIMx, para->ctrl_para.CAPONCE);	/* 单次捕捉控制 */
	ETIMx_ETxCR_CAPEDGE_Set(ETIMx, para->ctrl_para.CAPEDGE);		/* 捕捉沿选择 */

	ETIMx_ETxIVR_Write(ETIMx, para->ctrl_para.INITVALUE);	/* ETx初值寄存器 */
	ETIMx_ETxCMP_Write(ETIMx, para->ctrl_para.CMP);			/* ETx比较寄存器 */

	ETIMx_ETxIE_CMPIE_Setable(ETIMx, para->ctrl_para.CMPIE);		/* 扩展定时器比较中断使能 */
	ETIMx_ETxIE_CAPIE_Setable(ETIMx, para->ctrl_para.CAPIE);		/* 扩展定时器捕捉中断使能 */
	ETIMx_ETxIE_OVIE_Setable(ETIMx, para->ctrl_para.OVIE);			/* 扩展定时器溢出中断使能 */

	ETIMx_ETxCR_CEN_Setable(ETIMx, para->ctrl_para.CEN);		/* 启动控制 */
}


/******END OF FILE****/

