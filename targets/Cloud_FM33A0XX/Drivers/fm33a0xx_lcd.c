/**
  ******************************************************************************
  * @file    fm33a00xx_lcd.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    11-1-2018
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of....:
  *
*/
#include "fm33A0xx_lcd.h" 

//Code_Start
//2018-04-02-13-21-29
//FM33A0XX_Driver_Gen_V1.4

/* 防极化 相关函数 */
void LCD_DISPCTRL_ANTIPOLAR_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPCTRL |= (LCD_DISPCTRL_ANTIPOLAR_Msk);
	}
	else
	{
		LCD->DISPCTRL &= ~(LCD_DISPCTRL_ANTIPOLAR_Msk);
	}
}

FunState LCD_DISPCTRL_ANTIPOLAR_Getable(void)
{
	if (LCD->DISPCTRL & (LCD_DISPCTRL_ANTIPOLAR_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* LCD显示使能 相关函数 */
void LCD_DISPCTRL_LCDEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPCTRL |= (LCD_DISPCTRL_LCDEN_Msk);
	}
	else
	{
		LCD->DISPCTRL &= ~(LCD_DISPCTRL_LCDEN_Msk);
	}
}

FunState LCD_DISPCTRL_LCDEN_Getable(void)
{
	if (LCD->DISPCTRL & (LCD_DISPCTRL_LCDEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 显示闪烁使能 相关函数 */
void LCD_DISPCTRL_FLICK_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPCTRL |= (LCD_DISPCTRL_FLICK_Msk);
	}
	else
	{
		LCD->DISPCTRL &= ~(LCD_DISPCTRL_FLICK_Msk);
	}
}

FunState LCD_DISPCTRL_FLICK_Getable(void)
{
	if (LCD->DISPCTRL & (LCD_DISPCTRL_FLICK_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 测试模式使能，仅在DISPMD=1的情况下有效 相关函数 */
void LCD_DISPCTRL_TEST_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPCTRL |= (LCD_DISPCTRL_TEST_Msk);
	}
	else
	{
		LCD->DISPCTRL &= ~(LCD_DISPCTRL_TEST_Msk);
	}
}

FunState LCD_DISPCTRL_TEST_Getable(void)
{
	if (LCD->DISPCTRL & (LCD_DISPCTRL_TEST_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 测试模式选择 相关函数 */
void LCD_DISPCTRL_DISPMD_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPCTRL |= (LCD_DISPCTRL_DISPMD_Msk);
	}
	else
	{
		LCD->DISPCTRL &= ~(LCD_DISPCTRL_DISPMD_Msk);
	}
}

FunState LCD_DISPCTRL_DISPMD_Getable(void)
{
	if (LCD->DISPCTRL & (LCD_DISPCTRL_DISPMD_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* LCD测试控制位，仅在测试模式下有效 相关函数 */
void LCD_LCDTEST_LCCTRL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDTEST;
	tmpreg &= ~(LCD_LCDTEST_LCCTRL_Msk);
	tmpreg |= (SetValue & LCD_LCDTEST_LCCTRL_Msk);
	LCD->LCDTEST = tmpreg;
}

uint32_t LCD_LCDTEST_LCCTRL_Get(void)
{
	return (LCD->LCDTEST & LCD_LCDTEST_LCCTRL_Msk);
}

/* 测试模式使能 相关函数 */
void LCD_LCDTEST_TESTEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDTEST |= (LCD_LCDTEST_TESTEN_Msk);
	}
	else
	{
		LCD->LCDTEST &= ~(LCD_LCDTEST_TESTEN_Msk);
	}
}

FunState LCD_LCDTEST_TESTEN_Getable(void)
{
	if (LCD->LCDTEST & (LCD_LCDTEST_TESTEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 显示频率控制寄存器 相关函数 */
void LCD_DF_Write(uint32_t SetValue)
{
	LCD->DF = (SetValue & LCD_DF_DF_Msk);
}

uint32_t LCD_DF_Read(void)
{
	return (LCD->DF & LCD_DF_DF_Msk);
}

/* 闪烁显示点亮时间寄存器 相关函数 */
void LCD_TON_Write(uint32_t SetValue)
{
	LCD->TON = (SetValue & LCD_TON_TON_Msk);
}

uint32_t LCD_TON_Read(void)
{
	return (LCD->TON & LCD_TON_TON_Msk);
}

/* 闪烁显示熄灭时间寄存器 相关函数 */
void LCD_TOFF_Write(uint32_t SetValue)
{
	LCD->TOFF = (SetValue & LCD_TOFF_TOFF_Msk);
}

uint32_t LCD_TOFF_Read(void)
{
	return (LCD->TOFF & LCD_TOFF_TOFF_Msk);
}

/* 显示点亮中断使能 相关函数 */
void LCD_DISPIE_DONIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPIE |= (LCD_DISPIE_DONIE_Msk);
	}
	else
	{
		LCD->DISPIE &= ~(LCD_DISPIE_DONIE_Msk);
	}
}

FunState LCD_DISPIE_DONIE_Getable(void)
{
	if (LCD->DISPIE & (LCD_DISPIE_DONIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 显示熄灭中断使能 相关函数 */
void LCD_DISPIE_DOFFIE_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->DISPIE |= (LCD_DISPIE_DOFFIE_Msk);
	}
	else
	{
		LCD->DISPIE &= ~(LCD_DISPIE_DOFFIE_Msk);
	}
}

FunState LCD_DISPIE_DOFFIE_Getable(void)
{
	if (LCD->DISPIE & (LCD_DISPIE_DOFFIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* 显示点亮中断标志 相关函数 */
void LCD_DISPIF_DONIF_Clr(void)
{
	LCD->DISPIF = LCD_DISPIF_DONIF_Msk;
}

FlagStatus LCD_DISPIF_DONIF_Chk(void)
{
	if (LCD->DISPIF & LCD_DISPIF_DONIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 显示熄灭中断标志 相关函数 */
void LCD_DISPIF_DOFFIF_Clr(void)
{
	LCD->DISPIF = LCD_DISPIF_DOFFIF_Msk;
}

FlagStatus LCD_DISPIF_DOFFIF_Chk(void)
{
	if (LCD->DISPIF & LCD_DISPIF_DOFFIF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 偏置类型控制 相关函数 */
void LCD_LCDSET_BIASMD_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDSET;
	tmpreg &= ~(LCD_LCDSET_BIASMD_Msk);
	tmpreg |= (SetValue & LCD_LCDSET_BIASMD_Msk);
	LCD->LCDSET = tmpreg;
}

uint32_t LCD_LCDSET_BIASMD_Get(void)
{
	return (LCD->LCDSET & LCD_LCDSET_BIASMD_Msk);
}

/* 驱动波形选择 相关函数 */
void LCD_LCDSET_WFT_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDSET;
	tmpreg &= ~(LCD_LCDSET_WFT_Msk);
	tmpreg |= (SetValue & LCD_LCDSET_WFT_Msk);
	LCD->LCDSET = tmpreg;
}

uint32_t LCD_LCDSET_WFT_Get(void)
{
	return (LCD->LCDSET & LCD_LCDSET_WFT_Msk);
}

/* COM数量选择 相关函数 */
void LCD_LCDSET_LMUX_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDSET;
	tmpreg &= ~(LCD_LCDSET_LMUX_Msk);
	tmpreg |= (SetValue & LCD_LCDSET_LMUX_Msk);
	LCD->LCDSET = tmpreg;
}

uint32_t LCD_LCDSET_LMUX_Get(void)
{
	return (LCD->LCDSET & LCD_LCDSET_LMUX_Msk);
}

/* SC频率选择 相关函数 */
void LCD_LCDDRV_SCFSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDDRV;
	tmpreg &= ~(LCD_LCDDRV_SCFSEL_Msk);
	tmpreg |= (SetValue & LCD_LCDDRV_SCFSEL_Msk);
	LCD->LCDDRV = tmpreg;
}

uint32_t LCD_LCDDRV_SCFSEL_Get(void)
{
	return (LCD->LCDDRV & LCD_LCDDRV_SCFSEL_Msk);
}

/* 片外电容驱动模式下，驱动方式控制 相关函数 */
void LCD_LCDDRV_SC_CTRL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDDRV;
	tmpreg &= ~(LCD_LCDDRV_SC_CTRL_Msk);
	tmpreg |= (SetValue & LCD_LCDDRV_SC_CTRL_Msk);
	LCD->LCDDRV = tmpreg;
}

uint32_t LCD_LCDDRV_SC_CTRL_Get(void)
{
	return (LCD->LCDDRV & LCD_LCDDRV_SC_CTRL_Msk);
}

/* 驱动电流大小 相关函数 */
void LCD_LCDDRV_IC_CTRL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDDRV;
	tmpreg &= ~(LCD_LCDDRV_IC_CTRL_Msk);
	tmpreg |= (SetValue & LCD_LCDDRV_IC_CTRL_Msk);
	LCD->LCDDRV = tmpreg;
}

uint32_t LCD_LCDDRV_IC_CTRL_Get(void)
{
	return (LCD->LCDDRV & LCD_LCDDRV_IC_CTRL_Msk);
}

/* 驱动模式选择 相关函数 */
void LCD_LCDDRV_ENMODE_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDDRV;
	tmpreg &= ~(LCD_LCDDRV_ENMODE_Msk);
	tmpreg |= (SetValue & LCD_LCDDRV_ENMODE_Msk);
	LCD->LCDDRV = tmpreg;
}

uint32_t LCD_LCDDRV_ENMODE_Get(void)
{
	return (LCD->LCDDRV & LCD_LCDDRV_ENMODE_Msk);
}

/* 显示灰度设置 相关函数 */
void LCD_LCDBIAS_Write(uint32_t SetValue)
{
	LCD->LCDBIAS = (SetValue & LCD_LCDBIAS_LCDBIAS_Msk);
}

uint32_t LCD_LCDBIAS_Read(void)
{
	return (LCD->LCDBIAS & LCD_LCDBIAS_LCDBIAS_Msk);
}

/* BOOST环振频率配置信号,值越大频率越高，驱动越强，功耗越高 相关函数 */
void LCD_LCDBSTCON_OSC_CFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDBSTCON;
	tmpreg &= ~(LCD_LCDBSTCON_OSC_CFG_Msk);
	tmpreg |= (SetValue & LCD_LCDBSTCON_OSC_CFG_Msk);
	LCD->LCDBSTCON = tmpreg;
}

uint32_t LCD_LCDBSTCON_OSC_CFG_Get(void)
{
	return (LCD->LCDBSTCON & LCD_LCDBSTCON_OSC_CFG_Msk);
}

/* 基准源Buffer使能信号 相关函数 */
void LCD_LCDBSTCON_BUFENB_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDBSTCON &= ~(LCD_LCDBSTCON_BUFENB_Msk);
	}
	else
	{
		LCD->LCDBSTCON |= (LCD_LCDBSTCON_BUFENB_Msk);
	}
}

FunState LCD_LCDBSTCON_BUFENB_Getable(void)
{
	if (LCD->LCDBSTCON & (LCD_LCDBSTCON_BUFENB_Msk))
	{
		return DISABLE;
	}
	else
	{
		return ENABLE;
	}
}

/* 基准源Buffer Bypass 相关函数 */
void LCD_LCDBSTCON_BUFBYP_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDBSTCON |= (LCD_LCDBSTCON_BUFBYP_Msk);
	}
	else
	{
		LCD->LCDBSTCON &= ~(LCD_LCDBSTCON_BUFBYP_Msk);
	}
}

FunState LCD_LCDBSTCON_BUFBYP_Getable(void)
{
	if (LCD->LCDBSTCON & (LCD_LCDBSTCON_BUFBYP_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* VLCD下拉使能 相关函数 */
void LCD_LCDBSTCON_VLCDPD_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDBSTCON |= (LCD_LCDBSTCON_VLCDPD_Msk);
	}
	else
	{
		LCD->LCDBSTCON &= ~(LCD_LCDBSTCON_VLCDPD_Msk);
	}
}

FunState LCD_LCDBSTCON_VLCDPD_Getable(void)
{
	if (LCD->LCDBSTCON & (LCD_LCDBSTCON_VLCDPD_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* VLCD输出电压配置 相关函数 */
void LCD_LCDBSTCON_VLCDCFG_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = LCD->LCDBSTCON;
	tmpreg &= ~(LCD_LCDBSTCON_VLCDCFG_Msk);
	tmpreg |= (SetValue & LCD_LCDBSTCON_VLCDCFG_Msk);
	LCD->LCDBSTCON = tmpreg;
}

uint32_t LCD_LCDBSTCON_VLCDCFG_Get(void)
{
	return (LCD->LCDBSTCON & LCD_LCDBSTCON_VLCDCFG_Msk);
}

/* Booster测试信号 相关函数 */
void LCD_LCDBSTCON_BOOST_TEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDBSTCON |= (LCD_LCDBSTCON_BOOST_TEN_Msk);
	}
	else
	{
		LCD->LCDBSTCON &= ~(LCD_LCDBSTCON_BOOST_TEN_Msk);
	}
}

FunState LCD_LCDBSTCON_BOOST_TEN_Getable(void)
{
	if (LCD->LCDBSTCON & (LCD_LCDBSTCON_BOOST_TEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Booster使能信号 相关函数 */
void LCD_LCDBSTCON_BOOST_EN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		LCD->LCDBSTCON |= (LCD_LCDBSTCON_BOOST_EN_Msk);
	}
	else
	{
		LCD->LCDBSTCON &= ~(LCD_LCDBSTCON_BOOST_EN_Msk);
	}
}

FunState LCD_LCDBSTCON_BOOST_EN_Getable(void)
{
	if (LCD->LCDBSTCON & (LCD_LCDBSTCON_BOOST_EN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* LCD COM0~3输出使能控制 相关函数 */
void LCD_COM_EN_Write(uint32_t SetValue)
{
	LCD->COM_EN = (SetValue & LCD_COM_EN_COMEN_Msk);
}

uint32_t LCD_COM_EN_Read(void)
{
	return (LCD->COM_EN & LCD_COM_EN_COMEN_Msk);
}

/* LCD SEG0~31输出使能控制 相关函数 */
void LCD_SEG_EN0_Write(uint32_t SetValue)
{
	LCD->SEG_EN0 = (SetValue);
}

uint32_t LCD_SEG_EN0_Read(void)
{
	return (LCD->SEG_EN0);
}

/* LCD SEG32~43,COM4~7输出使能控制 相关函数 */
void LCD_SEG_EN1_Write(uint32_t SetValue)
{
	LCD->SEG_EN1 = (SetValue & LCD_SEG_EN1_SEGENx_Msk);
}

uint32_t LCD_SEG_EN1_Read(void)
{
	return (LCD->SEG_EN1 & LCD_SEG_EN1_SEGENx_Msk);
}

void LCD_Deinit(void)
{
	LCD->DISPCTRL = 0x00000000;
	LCD->LCDTEST = 0x00000000;
	LCD->DF = 0x00000000;
	LCD->TON = 0x00000000;
	LCD->TOFF = 0x00000000;
	LCD->DISPIE = 0x00000000;
	LCD->DISPIF = 0x00000000;
	LCD->LCDSET = 0x00000000;
	LCD->LCDDRV = 0x00000002;
	LCD->LCDBIAS = 0x0000000E;
	LCD->LCDBSTCON = 0x00000000;
	LCD->COM_EN = 0x00000000;
	LCD->SEG_EN0 = 0x00000000;
	LCD->SEG_EN1 = 0x00000000;
}
//Code_End

/*DISPDATAx显示数据寄存器刷新*/
void LCD_DISPDATAx_Refresh(uint32_t* DispBuf)
{
    LCD->DISPDATA0 = DispBuf[0];
    LCD->DISPDATA1 = DispBuf[1];
    LCD->DISPDATA2 = DispBuf[2];
    LCD->DISPDATA3 = DispBuf[3];
    LCD->DISPDATA4 = DispBuf[4];
    LCD->DISPDATA5 = DispBuf[5];
    LCD->DISPDATA6 = DispBuf[6];
    LCD->DISPDATA7 = DispBuf[7];
    LCD->DISPDATA8 = DispBuf[8];
    LCD->DISPDATA9 = DispBuf[9];
}

/*lcd boost功能配置*/
void LCD_Boost_Init(LCD_Boost_InitTypeDef* para)
{
	LCD_LCDBSTCON_OSC_CFG_Set(para->OSC_CFG);
	LCD_LCDBSTCON_VLCDCFG_Set(para->VLCDCFG);
	LCD_LCDBSTCON_BUFENB_Setable(para->BUFENB);
	LCD_LCDBSTCON_BUFBYP_Setable(para->BUFBYP);
	LCD_LCDBSTCON_VLCDPD_Setable(para->VLCDPD);	
	LCD_LCDBSTCON_BOOST_TEN_Setable(para->BOOST_TEN);
	LCD_LCDBSTCON_BOOST_EN_Setable(para->BOOST_EN);
}

/*LCD显示功能配置*/
void LCD_Init(LCD_InitTypeDef* para)
{
	LCD_LCDSET_LMUX_Set(para->LMUX);
	LCD_LCDDRV_ENMODE_Set(para->ENMODE);
	LCD_LCDSET_WFT_Set(para->WFT);
	LCD_DF_Write(para->DF);
	LCD_LCDSET_BIASMD_Set(para->BIASMD);
	LCD_LCDDRV_SCFSEL_Set(para->SCFSEL);
	LCD_LCDDRV_SC_CTRL_Set(para->SC_CTRL);
	LCD_LCDDRV_IC_CTRL_Set(para->IC_CTRL);	
	LCD_LCDBIAS_Write(para->LCDBIAS);
	LCD_DISPCTRL_ANTIPOLAR_Setable(para->ANTIPOLAR);
	
	LCD_DISPCTRL_TEST_Setable(para->TEST);
	LCD_DISPCTRL_DISPMD_Setable(para->DISPMD);

	LCD_LCDTEST_LCCTRL_Set(para->LCCTRL);
	LCD_LCDTEST_TESTEN_Setable(para->TESTEN);
	
	LCD_DISPCTRL_FLICK_Setable(para->FLICK);	
	LCD_TON_Write(para->TON);
	LCD_TOFF_Write(para->TOFF);
	LCD_DISPIE_DONIE_Setable(para->DONIE);
	LCD_DISPIE_DOFFIE_Setable(para->DOFFIE);
	LCD_DISPIF_DONIF_Clr();
	LCD_DISPIF_DOFFIF_Clr();
	
	LCD_DISPCTRL_LCDEN_Setable(para->LCDEN);	
}


