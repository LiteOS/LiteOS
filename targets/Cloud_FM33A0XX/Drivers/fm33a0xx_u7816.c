/**
  ******************************************************************************
  * @file    fm33a0xx_u7816.c
  * @author  FM33a0 Application Team
  * @version V2.0.1
  * @date    4-MAY-2017
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/
/* Includes ------------------------------------------------------------------*/

#include "FM33A0XX_u7816.h" 

//Code_Start
//2018-04-12-09-21-42
//FM33A0XX_Driver_Gen_V1.4

/* U7816通道发送使能控制 相关函数 */
void U7816x_U7816CTRL_TXEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816CTRL |= (U7816x_U7816CTRL_TXEN_Msk);
	}
	else
	{
		U7816x->U7816CTRL &= ~(U7816x_U7816CTRL_TXEN_Msk);
	}
}

FunState U7816x_U7816CTRL_TXEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816CTRL & (U7816x_U7816CTRL_TXEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* U7816通道接收使能控制 相关函数 */
void U7816x_U7816CTRL_RXEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816CTRL |= (U7816x_U7816CTRL_RXEN_Msk);
	}
	else
	{
		U7816x->U7816CTRL &= ~(U7816x_U7816CTRL_RXEN_Msk);
	}
}

FunState U7816x_U7816CTRL_RXEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816CTRL & (U7816x_U7816CTRL_RXEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* U7816时钟CLK输出使能控制 相关函数 */
void U7816x_U7816CTRL_CKOEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816CTRL |= (U7816x_U7816CTRL_CKOEN_Msk);
	}
	else
	{
		U7816x->U7816CTRL &= ~(U7816x_U7816CTRL_CKOEN_Msk);
	}
}

FunState U7816x_U7816CTRL_CKOEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816CTRL & (U7816x_U7816CTRL_CKOEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* U7816通道数据发送强上拉电阻自动有效控制 相关函数 */
void U7816x_U7816CTRL_HPUAT_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816CTRL |= (U7816x_U7816CTRL_HPUAT_Msk);
	}
	else
	{
		U7816x->U7816CTRL &= ~(U7816x_U7816CTRL_HPUAT_Msk);
	}
}

FunState U7816x_U7816CTRL_HPUAT_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816CTRL & (U7816x_U7816CTRL_HPUAT_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* U7816通道强上拉使能控制 相关函数 */
void U7816x_U7816CTRL_HPUEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816CTRL |= (U7816x_U7816CTRL_HPUEN_Msk);
	}
	else
	{
		U7816x->U7816CTRL &= ~(U7816x_U7816CTRL_HPUEN_Msk);
	}
}

FunState U7816x_U7816CTRL_HPUEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816CTRL & (U7816x_U7816CTRL_HPUEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* ERROR SIGNAL宽度选择 相关函数 */
void U7816x_U7816FRC_ERSW_Set(U7816x_Type* U7816x, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = U7816x->U7816FRC;
	tmpreg &= ~(U7816x_U7816FRC_ERSW_Msk);
	tmpreg |= (SetValue & U7816x_U7816FRC_ERSW_Msk);
	U7816x->U7816FRC = tmpreg;
}

uint32_t U7816x_U7816FRC_ERSW_Get(U7816x_Type* U7816x)
{
	return (U7816x->U7816FRC & U7816x_U7816FRC_ERSW_Msk);
}

/* ERROR SIGNAL后GUARDTIME宽度选择（仅在发送时有效） 相关函数 */
void U7816x_U7816FRC_ERSGD_Set(U7816x_Type* U7816x, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = U7816x->U7816FRC;
	tmpreg &= ~(U7816x_U7816FRC_ERSGD_Msk);
	tmpreg |= (SetValue & U7816x_U7816FRC_ERSGD_Msk);
	U7816x->U7816FRC = tmpreg;
}

uint32_t U7816x_U7816FRC_ERSGD_Get(U7816x_Type* U7816x)
{
	return (U7816x->U7816FRC & U7816x_U7816FRC_ERSGD_Msk);
}

/* BGT（block guard time）控制 相关函数 */
void U7816x_U7816FRC_BGTEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816FRC |= (U7816x_U7816FRC_BGTEN_Msk);
	}
	else
	{
		U7816x->U7816FRC &= ~(U7816x_U7816FRC_BGTEN_Msk);
	}
}

FunState U7816x_U7816FRC_BGTEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816FRC & (U7816x_U7816FRC_BGTEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 控制接收数据奇偶校验出错时自动重发次数 相关函数 */
void U7816x_U7816FRC_REP_T_Set(U7816x_Type* U7816x, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = U7816x->U7816FRC;
	tmpreg &= ~(U7816x_U7816FRC_REP_T_Msk);
	tmpreg |= (SetValue & U7816x_U7816FRC_REP_T_Msk);
	U7816x->U7816FRC = tmpreg;
}

uint32_t U7816x_U7816FRC_REP_T_Get(U7816x_Type* U7816x)
{
	return (U7816x->U7816FRC & U7816x_U7816FRC_REP_T_Msk);
}

/* 奇偶校验类型选择 相关函数 */
void U7816x_U7816FRC_PAR_Set(U7816x_Type* U7816x, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = U7816x->U7816FRC;
	tmpreg &= ~(U7816x_U7816FRC_PAR_Msk);
	tmpreg |= (SetValue & U7816x_U7816FRC_PAR_Msk);
	U7816x->U7816FRC = tmpreg;
}

uint32_t U7816x_U7816FRC_PAR_Get(U7816x_Type* U7816x)
{
	return (U7816x->U7816FRC & U7816x_U7816FRC_PAR_Msk);
}

/* Guard Time长度控制位（发送时严格按照协议2etu） 相关函数 */
void U7816x_U7816FRC_FREN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816FRC |= (U7816x_U7816FRC_FREN_Msk);
	}
	else
	{
		U7816x->U7816FRC &= ~(U7816x_U7816FRC_FREN_Msk);
	}
}

FunState U7816x_U7816FRC_FREN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816FRC & (U7816x_U7816FRC_FREN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 缓发送数据奇偶校验错的处理方式选择 相关函数 */
void U7816x_U7816FRC_TREPEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816FRC |= (U7816x_U7816FRC_TREPEN_Msk);
	}
	else
	{
		U7816x->U7816FRC &= ~(U7816x_U7816FRC_TREPEN_Msk);
	}
}

FunState U7816x_U7816FRC_TREPEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816FRC & (U7816x_U7816FRC_TREPEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 接收数据奇偶校验错的处理方式选择 相关函数 */
void U7816x_U7816FRC_RREPEN_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816FRC |= (U7816x_U7816FRC_RREPEN_Msk);
	}
	else
	{
		U7816x->U7816FRC &= ~(U7816x_U7816FRC_RREPEN_Msk);
	}
}

FunState U7816x_U7816FRC_RREPEN_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816FRC & (U7816x_U7816FRC_RREPEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 传输次序反向编码使能 相关函数 */
void U7816x_U7816FRC_DICONV_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816FRC |= (U7816x_U7816FRC_DICONV_Msk);
	}
	else
	{
		U7816x->U7816FRC &= ~(U7816x_U7816FRC_DICONV_Msk);
	}
}

FunState U7816x_U7816FRC_DICONV_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816FRC & (U7816x_U7816FRC_DICONV_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 发送时插入的EGT时间（以ETU为单位） 相关函数 */
void U7816x_U7816EGT_Write(U7816x_Type* U7816x, uint32_t SetValue)
{
	U7816x->U7816EGT = (SetValue & U7816x_U7816EGT_TXEGT_Msk);
}

uint32_t U7816x_U7816EGT_Read(U7816x_Type* U7816x)
{
	return (U7816x->U7816EGT & U7816x_U7816EGT_TXEGT_Msk);
}

/* U7816时钟输出分频控制寄存器 相关函数 */
void U7816x_U7816CLKDIV_Write(U7816x_Type* U7816x, uint32_t SetValue)
{
	U7816x->U7816CLKDIV = (SetValue & U7816x_U7816CLKDIV_CLKDIV_Msk);
}

uint32_t U7816x_U7816CLKDIV_Read(U7816x_Type* U7816x)
{
	return (U7816x->U7816CLKDIV & U7816x_U7816CLKDIV_CLKDIV_Msk);
}

/* U7816预分频控制寄存器，控制7816通信分频比（波特率） 相关函数 */
void U7816x_U7816PDIV_Write(U7816x_Type* U7816x, uint32_t SetValue)
{
	U7816x->U7816PDIV = (SetValue & U7816x_U7816PDIV_PDIV_Msk);
}

uint32_t U7816x_U7816PDIV_Read(U7816x_Type* U7816x)
{
	return (U7816x->U7816PDIV & U7816x_U7816PDIV_PDIV_Msk);
}

/* U7816数据接收缓存寄存器 相关函数 */
uint32_t U7816x_U7816RXBUF_Read(U7816x_Type* U7816x)
{
	return (U7816x->U7816RXBUF & U7816x_U7816RXBUF_RXBUF_Msk);
}

/* U7816数据发送缓存寄存器 相关函数 */
void U7816x_U7816TXBUF_Write(U7816x_Type* U7816x, uint32_t SetValue)
{
	U7816x->U7816TXBUF = (SetValue & U7816x_U7816TXBUF_TXBUF_Msk);
}

/* 数据接收中断使能位。对应RX_FLAG中断标志位 相关函数 */
void U7816x_U7816IE_RXIE_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816IE |= (U7816x_U7816IE_RXIE_Msk);
	}
	else
	{
		U7816x->U7816IE &= ~(U7816x_U7816IE_RXIE_Msk);
	}
}

FunState U7816x_U7816IE_RXIE_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816IE & (U7816x_U7816IE_RXIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 数据发送中断使能位。对应TX_FLAG中断标志位 相关函数 */
void U7816x_U7816IE_TXIE_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816IE |= (U7816x_U7816IE_TXIE_Msk);
	}
	else
	{
		U7816x->U7816IE &= ~(U7816x_U7816IE_TXIE_Msk);
	}
}

FunState U7816x_U7816IE_TXIE_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816IE & (U7816x_U7816IE_TXIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 线路状态中断使能位。对应ERROR_FLAG中断标志位 相关函数 */
void U7816x_U7816IE_LSIE_Setable(U7816x_Type* U7816x, FunState NewState)
{
	if (NewState == ENABLE)
	{
		U7816x->U7816IE |= (U7816x_U7816IE_LSIE_Msk);
	}
	else
	{
		U7816x->U7816IE &= ~(U7816x_U7816IE_LSIE_Msk);
	}
}

FunState U7816x_U7816IE_LSIE_Getable(U7816x_Type* U7816x)
{
	if (U7816x->U7816IE & (U7816x_U7816IE_LSIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 错误标志，寄存器配置出错或传输过程中出错。硬件置位，清U7816ERR中相应错误标志清零 相关函数 */
FlagStatus U7816x_U7816IF_ERRIF_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816IF & U7816x_U7816IF_ERRIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 发送缓冲区空标志 相关函数 */
void U7816x_U7816IF_TXIF_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816IF = U7816x_U7816IF_TXIF_Msk;
}

FlagStatus U7816x_U7816IF_TXIF_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816IF & U7816x_U7816IF_TXIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收完成标志 相关函数 */
void U7816x_U7816IF_RXIF_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816IF = U7816x_U7816IF_RXIF_Msk;
}

FlagStatus U7816x_U7816IF_RXIF_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816IF & U7816x_U7816IF_RXIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 发送数据奇偶校验错误标志位 相关函数 */
void U7816x_U7816ERR_TPARERR_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816ERR = U7816x_U7816ERR_TPARERR_Msk;
}

FlagStatus U7816x_U7816ERR_TPARERR_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816ERR & U7816x_U7816ERR_TPARERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收数据奇偶校验错误标志位 相关函数 */
void U7816x_U7816ERR_RPARERR_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816ERR = U7816x_U7816ERR_RPARERR_Msk;
}

FlagStatus U7816x_U7816ERR_RPARERR_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816ERR & U7816x_U7816ERR_RPARERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收帧格式错误标志位 相关函数 */
void U7816x_U7816ERR_FRERR_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816ERR = U7816x_U7816ERR_FRERR_Msk;
}

FlagStatus U7816x_U7816ERR_FRERR_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816ERR & U7816x_U7816ERR_FRERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收溢出错误标志位 相关函数 */
void U7816x_U7816ERR_OVERR_Clr(U7816x_Type* U7816x)
{
	U7816x->U7816ERR = U7816x_U7816ERR_OVERR_Msk;
}

FlagStatus U7816x_U7816ERR_OVERR_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816ERR & U7816x_U7816ERR_OVERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* U7816接口发送了错误信号，正在等待对方重发数据.状态机进入发送错误信号状态时置位，收到数据起始位或者进入发送状态时硬件清零;
 相关函数 */
FlagStatus U7816x_U7816STA_WAIT_RPT_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816STA & U7816x_U7816STA_WAIT_RPT_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 发送数据忙标志 相关函数 */
FlagStatus U7816x_U7816STA_TXBUSY_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816STA & U7816x_U7816STA_TXBUSY_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收数据忙标志 相关函数 */
FlagStatus U7816x_U7816STA_RXBUSY_Chk(U7816x_Type* U7816x)
{
	if (U7816x->U7816STA & U7816x_U7816STA_RXBUSY_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


void U7816x_Deinit(U7816x_Type* U7816x)
{
	U7816x->U7816CTRL = 0x00000000;
	U7816x->U7816FRC = 0x00000006;
	U7816x->U7816EGT = 0x00000000;
	U7816x->U7816CLKDIV = 0x00000003;
	U7816x->U7816PDIV = 0x00000173;
	//U7816x->U7816RXBUF = ;
	//U7816x->U7816TXBUF = ;
	U7816x->U7816IE = 0x00000000;
	//U7816x->U7816IF = 0x00000002;
	//U7816x->U7816ERR = 0x00000000;
	//U7816x->U7816STA = 0x00000000;
}
//Code_End


/* U7816完整参数初始化函数 */
void U7816_Init(U7816x_Type* U7816x, U7816_InitTypeDef* para)
{
	U7816x_U7816CTRL_TXEN_Setable(U7816x, para->TXEN);		//发送使能
	U7816x_U7816CTRL_RXEN_Setable(U7816x, para->RXEN);		//接受使能
	U7816x_U7816CTRL_CKOEN_Setable(U7816x, para->CKOEN);		//U7816时钟CLK输出使能控制
	U7816x_U7816CTRL_HPUAT_Setable(U7816x, para->HPUAT);		//U7816通道数据发送强上拉电阻自动有效控制
	U7816x_U7816CTRL_HPUEN_Setable(U7816x, para->HPUEN);		//U7816通道强上拉使能控制
	
	U7816x_U7816FRC_ERSW_Set(U7816x, para->ERSW);		//ERROR SIGNAL宽度选择
	U7816x_U7816FRC_ERSGD_Set(U7816x, para->ERSGD);		//ERROR SIGNAL后GUARDTIME宽度选择（仅在发送时有效）
	U7816x_U7816FRC_BGTEN_Setable(U7816x, para->BGTEN);		//BGT（block guard time）控制
	U7816x_U7816FRC_REP_T_Set(U7816x, para->REP_T);		//控制接收数据奇偶校验出错时自动重发次数
	U7816x_U7816FRC_PAR_Set(U7816x, para->PAR);		//奇偶校验类型选择
	U7816x_U7816FRC_FREN_Setable(U7816x, para->FREN);		//Guard Time长度控制位（发送时严格按照协议2etu）
	U7816x_U7816FRC_TREPEN_Setable(U7816x, para->TREPEN);		//缓发送数据奇偶校验错的处理方式选择
	U7816x_U7816FRC_RREPEN_Setable(U7816x, para->RREPEN);		//接收数据奇偶校验错的处理方式选择
	U7816x_U7816FRC_DICONV_Setable(U7816x, para->DICONV);		//传输次序反向编码使能
	
	U7816x_U7816EGT_Write(U7816x, para->TXEGT);		//发送时插入的EGT时间（以ETU为单位）
	U7816x_U7816CLKDIV_Write(U7816x, para->CLKDIV);		//U7816时钟输出分频控制寄存器
	U7816x_U7816PDIV_Write(U7816x, para->PDIV);		//U7816预分频控制寄存器，控制7816通信分频比（波特率）
	
	U7816x_U7816IE_RXIE_Setable(U7816x, para->RXIE);		//数据接收中断使能位。对应RX_FLAG中断标志位
	U7816x_U7816IE_TXIE_Setable(U7816x, para->TXIE);		//数据发送中断使能位。对应TX_FLAG中断标志位
	U7816x_U7816IE_LSIE_Setable(U7816x, para->LSIE);		//线路状态中断使能位。对应ERROR_FLAG中断标志位
}

/******END OF FILE****/
