/**
  ******************************************************************************
  * @file    fm33a0xx_i2c.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    18-1-2018
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/
/* Includes ------------------------------------------------------------------*/

#include "fm33A0xx_i2c.h" 
#include <string.h>
/** @addtogroup fm33a0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup I2C 
  * @brief I2C driver modules
  * @{
  */ 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 //Code_Start
//2018-04-02-13-21-22
//FM33A0XX_Driver_Gen_V1.4

/* 当I2C模块禁止时，SCL固定为高低电平控制位 相关函数 */
void I2C_I2CCTRL_SCLHL_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_SCLHL_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_SCLHL_Msk);
	}
}

FunState I2C_I2CCTRL_SCLHL_Getable(void)
{
	if (I2C->I2CCTRL & (I2C_I2CCTRL_SCLHL_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 当I2C模块禁止时，SDA固定为高低电平控制位 相关函数 */
void I2C_I2CCTRL_SDAHL_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_SDAHL_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_SDAHL_Msk);
	}
}

FunState I2C_I2CCTRL_SDAHL_Getable(void)
{
	if (I2C->I2CCTRL & (I2C_I2CCTRL_SDAHL_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 主控接收模式下，主机给从机回应使能位 相关函数 */
void I2C_I2CCTRL_ACKEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_ACKEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_ACKEN_Msk);
	}
}

FunState I2C_I2CCTRL_ACKEN_Getable(void)
{
	if (I2C->I2CCTRL & (I2C_I2CCTRL_ACKEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 主控接收模式下，接收使能位 相关函数 */
void I2C_I2CCTRL_RCEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_RCEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_RCEN_Msk);
	}
}

FunState I2C_I2CCTRL_RCEN_Getable(void)
{
	if (I2C->I2CCTRL & (I2C_I2CCTRL_RCEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* STOP时序产生使能控制位 相关函数 */
void I2C_I2CCTRL_PEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_PEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_PEN_Msk);
	}
}

/* Repeated START时序产生使能控制位 相关函数 */
void I2C_I2CCTRL_RSEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_RSEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_RSEN_Msk);
	}
}

/* START时序产生使能控制位 相关函数 */
void I2C_I2CCTRL_SEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_SEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_SEN_Msk);
	}
}

/* I2C模块使能控制位 相关函数 */
void I2C_I2CCTRL_I2CEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CCTRL |= (I2C_I2CCTRL_I2CEN_Msk);
	}
	else
	{
		I2C->I2CCTRL &= ~(I2C_I2CCTRL_I2CEN_Msk);
	}
}

FunState I2C_I2CCTRL_I2CEN_Getable(void)
{
	if (I2C->I2CCTRL & (I2C_I2CCTRL_I2CEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 写冲突检测位 相关函数 */
void I2C_I2CSTA_WCOL_Clr(void)
{
	I2C->I2CSTA = I2C_I2CSTA_WCOL_Msk;
}

FlagStatus I2C_I2CSTA_WCOL_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_WCOL_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* I2C接口状态位 相关函数 */
FlagStatus I2C_I2CSTA_RW_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_RW_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* STOP标志位，MCU查询后硬件清0 相关函数 */
FlagStatus I2C_I2CSTA_P_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_P_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* START标志位，MCU查询后硬件清0 相关函数 */
FlagStatus I2C_I2CSTA_S_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_S_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 缓冲器满状态位 相关函数 */
FlagStatus I2C_I2CSTA_BF_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_BF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 主控发送模式下，来自从机的回应信号 相关函数 */
void I2C_I2CSTA_ACKSTA_Clr(void)
{
	I2C->I2CSTA = I2C_I2CSTA_ACKSTA_Msk;
}

FlagStatus I2C_I2CSTA_ACKSTA_Chk(void)
{
	if (I2C->I2CSTA & I2C_I2CSTA_ACKSTA_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 主控接收模式下，主机回应信号的状态 相关函数 */
void I2C_I2CSTA_ACKDT_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = I2C->I2CSTA;
	tmpreg &= ~(I2C_I2CSTA_ACKDT_Msk);
	tmpreg |= (SetValue & I2C_I2CSTA_ACKDT_Msk);
	I2C->I2CSTA = tmpreg;
}

uint32_t I2C_I2CSTA_ACKDT_Get(void)
{
	return (I2C->I2CSTA & I2C_I2CSTA_ACKDT_Msk);
}

/* 波特率设置寄存器 相关函数 */
void I2C_I2CBRG_Write(uint32_t SetValue)
{
	I2C->I2CBRG = (SetValue & I2C_I2CBRG_SSPBRG_Msk);
}

uint32_t I2C_I2CBRG_Read(void)
{
	return (I2C->I2CBRG & I2C_I2CBRG_SSPBRG_Msk);
}

/* 收发缓冲寄存器 相关函数 */
void I2C_I2CBUF_Write(uint32_t SetValue)
{
	I2C->I2CBUF = (SetValue & I2C_I2CBUF_SSPBUF_Msk);
}

uint32_t I2C_I2CBUF_Read(void)
{
	return (I2C->I2CBUF & I2C_I2CBUF_SSPBUF_Msk);
}

/* I2C中断使能控制位 相关函数 */
void I2C_I2CIR_I2CIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CIR |= (I2C_I2CIR_I2CIE_Msk);
	}
	else
	{
		I2C->I2CIR &= ~(I2C_I2CIR_I2CIE_Msk);
	}
}

FunState I2C_I2CIR_I2CIE_Getable(void)
{
	if (I2C->I2CIR & (I2C_I2CIR_I2CIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* I2C中断标志位 相关函数 */
void I2C_I2CIR_I2CIF_Clr(void)
{
	if (I2C->I2CIR & (I2C_I2CIR_I2CIE_Msk))
	{
		I2C->I2CIR = (I2C_I2CIR_I2CIE_Msk|I2C_I2CIR_I2CIF_Msk);
	}
	else
	{
		I2C->I2CIR = I2C_I2CIR_I2CIF_Msk;
	}
}

FlagStatus I2C_I2CIR_I2CIF_Chk(void)
{
	if (I2C->I2CIR & I2C_I2CIR_I2CIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* I2C主状态机编码 相关函数 */
uint32_t I2C_I2CFSM_I2CFSM_Get(void)
{
	return (I2C->I2CFSM & I2C_I2CFSM_I2CFSM_Msk);
}

/* 错误标志中断使能 相关函数 */
void I2C_I2CERR_ERRIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		I2C->I2CERR |= (I2C_I2CERR_ERRIE_Msk);
	}
	else
	{
		I2C->I2CERR &= ~(I2C_I2CERR_ERRIE_Msk);
	}
}

FunState I2C_I2CERR_ERRIE_Getable(void)
{
	if (I2C->I2CERR & (I2C_I2CERR_ERRIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* OP_IDLE状态下错误标志位 相关函数 */
void I2C_I2CERR_OIERR_Clr(void)
{
	if (I2C->I2CERR & (I2C_I2CERR_ERRIE_Msk))
	{
		I2C->I2CERR = (I2C_I2CERR_ERRIE_Msk|I2C_I2CERR_OIERR_Msk);
	}
	else
	{
		I2C->I2CERR = I2C_I2CERR_OIERR_Msk;
	}
}

FlagStatus I2C_I2CERR_OIERR_Chk(void)
{
	if (I2C->I2CERR & I2C_I2CERR_OIERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* START_DONE状态下错误标志位 相关函数 */
void I2C_I2CERR_SDERR_Clr(void)
{
	if (I2C->I2CERR & (I2C_I2CERR_ERRIE_Msk))
	{
		I2C->I2CERR = (I2C_I2CERR_ERRIE_Msk|I2C_I2CERR_SDERR_Msk);
	}
	else
	{
		I2C->I2CERR = I2C_I2CERR_SDERR_Msk;
	}
}

FlagStatus I2C_I2CERR_SDERR_Chk(void)
{
	if (I2C->I2CERR & I2C_I2CERR_SDERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* IDLE状态下错误标志位 相关函数 */
void I2C_I2CERR_IERR_Clr(void)
{
	if (I2C->I2CERR & (I2C_I2CERR_ERRIE_Msk))
	{
		I2C->I2CERR = (I2C_I2CERR_ERRIE_Msk|I2C_I2CERR_IERR_Msk);
	}
	else
	{
		I2C->I2CERR = I2C_I2CERR_IERR_Msk;
	}
}

FlagStatus I2C_I2CERR_IERR_Chk(void)
{
	if (I2C->I2CERR & I2C_I2CERR_IERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


void I2C_Deinit(void)
{
	I2C->I2CCTRL = 0x00006000;
	I2C->I2CSTA = 0x00000000;
	I2C->I2CBRG = 0x00000013;
	//I2C->I2CBUF = ;
	I2C->I2CIR = 0x00000000;
	//I2C->I2CFSM = ;
	I2C->I2CERR = 0x00000000;
}
//Code_End

/* I2C波特率寄存器值计算
	输入：期望波特率@Hz， APBClk @Hz
	输出：I2CBRG对应期望波特率的配置值
*/
uint32_t I2C_BaudREG_Calc(uint32_t I2CClk, uint32_t APBClk)
{
	uint32_t TempREG;
	
	TempREG = APBClk/(4*I2CClk);
	if( TempREG >= 1 ) TempREG = TempREG - 1;
	
	return TempREG;
}


/******END OF FILE****/
