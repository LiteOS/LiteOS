/**
  ******************************************************************************
  * @file    fm33a0xx_rcc.c
  * @author  FM33a0xx Application Team
  * @version V2.0.1
  * @date    24-12-2017
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the clock control (RCC) peripheral:
  *           + Internal/external clocks, PLL, CSS and MCO configuration
  *           + System, AHB and APB busses clocks configuration
  *           + Peripheral clocks configuration
  *           + Interrupts and flags management
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_rcc.h" 

/** @addtogroup fm33a0xx_StdPeriph_Driver
  * @{
  */

/** @defgroup RCC 
  * @brief RCC driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCC_Private_Functions
  * @{
  */
 
//Code_Start
//2018-04-02-13-22-07
//FM33A0XX_Driver_Gen_V1.4

/* Sleep/DeepSleep模式下禁止RCLP，仅在无晶体配置下有效 相关函数 */
void RCC_SYSCLKSEL_LPM_RCLP_OFF_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		RCC->SYSCLKSEL |= (RCC_SYSCLKSEL_LPM_RCLP_OFF_Msk);
	}
	else
	{
		RCC->SYSCLKSEL &= ~(RCC_SYSCLKSEL_LPM_RCLP_OFF_Msk);
	}
}

FunState RCC_SYSCLKSEL_LPM_RCLP_OFF_Getable(void)
{
	if (RCC->SYSCLKSEL & (RCC_SYSCLKSEL_LPM_RCLP_OFF_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Sleep/DeepSleep模式下EXTI采样设置 相关函数 */
void RCC_SYSCLKSEL_SLP_ENEXTI_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		RCC->SYSCLKSEL |= (RCC_SYSCLKSEL_SLP_ENEXTI_Msk);
	}
	else
	{
		RCC->SYSCLKSEL &= ~(RCC_SYSCLKSEL_SLP_ENEXTI_Msk);
	}
}

FunState RCC_SYSCLKSEL_SLP_ENEXTI_Getable(void)
{
	if (RCC->SYSCLKSEL & (RCC_SYSCLKSEL_SLP_ENEXTI_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* EXTI中断与数字滤波采样时钟选择 相关函数 */
void RCC_SYSCLKSEL_EXTICKSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->SYSCLKSEL;
	tmpreg &= ~(RCC_SYSCLKSEL_EXTICKSEL_Msk);
	tmpreg |= (SetValue & RCC_SYSCLKSEL_EXTICKSEL_Msk);
	RCC->SYSCLKSEL = tmpreg;
}

uint32_t RCC_SYSCLKSEL_EXTICKSEL_Get(void)
{
	return (RCC->SYSCLKSEL & RCC_SYSCLKSEL_EXTICKSEL_Msk);
}

/* APB时钟分频选择 相关函数 */
void RCC_SYSCLKSEL_APBPRES_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->SYSCLKSEL;
	tmpreg &= ~(RCC_SYSCLKSEL_APBPRES_Msk);
	tmpreg |= (SetValue & RCC_SYSCLKSEL_APBPRES_Msk);
	RCC->SYSCLKSEL = tmpreg;
}

uint32_t RCC_SYSCLKSEL_APBPRES_Get(void)
{
	return (RCC->SYSCLKSEL & RCC_SYSCLKSEL_APBPRES_Msk);
}

/* AHB时钟分频选择 相关函数 */
void RCC_SYSCLKSEL_AHBPRES_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->SYSCLKSEL;
	tmpreg &= ~(RCC_SYSCLKSEL_AHBPRES_Msk);
	tmpreg |= (SetValue & RCC_SYSCLKSEL_AHBPRES_Msk);
	RCC->SYSCLKSEL = tmpreg;
}

uint32_t RCC_SYSCLKSEL_AHBPRES_Get(void)
{
	return (RCC->SYSCLKSEL & RCC_SYSCLKSEL_AHBPRES_Msk);
}

/* 系统时钟源选择 相关函数 */
void RCC_SYSCLKSEL_SYSCLKSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->SYSCLKSEL;
	tmpreg &= ~(RCC_SYSCLKSEL_SYSCLKSEL_Msk);
	tmpreg |= (SetValue & RCC_SYSCLKSEL_SYSCLKSEL_Msk);
	RCC->SYSCLKSEL = tmpreg;
}

uint32_t RCC_SYSCLKSEL_SYSCLKSEL_Get(void)
{
	return (RCC->SYSCLKSEL & RCC_SYSCLKSEL_SYSCLKSEL_Msk);
}

/* RCHF频率选择寄存器 相关函数 */
void RCC_RCHFCON_FSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->RCHFCON;
	tmpreg &= ~(RCC_RCHFCON_FSEL_Msk);
	tmpreg |= (SetValue & RCC_RCHFCON_FSEL_Msk);
	RCC->RCHFCON = tmpreg;
}

uint32_t RCC_RCHFCON_FSEL_Get(void)
{
	return (RCC->RCHFCON & RCC_RCHFCON_FSEL_Msk);
}

/* RCHF使能寄存器 相关函数 */
void RCC_RCHFCON_RCHFEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		RCC->RCHFCON |= (RCC_RCHFCON_RCHFEN_Msk);
	}
	else
	{
		RCC->RCHFCON &= ~(RCC_RCHFCON_RCHFEN_Msk);
	}
}

FunState RCC_RCHFCON_RCHFEN_Getable(void)
{
	if (RCC->RCHFCON & (RCC_RCHFCON_RCHFEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* RCHF频率调校寄存器 相关函数 */
void RCC_RCHFTRIM_Write(uint32_t SetValue)
{
	RCC->RCHFTRIM = (SetValue & RCC_RCHFTRIM_RCHFTRIM_Msk);
}

uint32_t RCC_RCHFTRIM_Read(void)
{
	return (RCC->RCHFTRIM & RCC_RCHFTRIM_RCHFTRIM_Msk);
}

/* PLL倍频数 相关函数 */
void RCC_PLLCON_PLLDB_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->PLLCON;
	tmpreg &= ~(RCC_PLLCON_PLLDB_Msk);
	tmpreg |= (SetValue & RCC_PLLCON_PLLDB_Msk);
	RCC->PLLCON = tmpreg;
}

uint32_t RCC_PLLCON_PLLDB_Get(void)
{
	return (RCC->PLLCON & RCC_PLLCON_PLLDB_Msk);
}

/* PLL输出选择寄存器 相关函数 */
void RCC_PLLCON_PLLOSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->PLLCON;
	tmpreg &= ~(RCC_PLLCON_PLLOSEL_Msk);
	tmpreg |= (SetValue & RCC_PLLCON_PLLOSEL_Msk);
	RCC->PLLCON = tmpreg;
}

uint32_t RCC_PLLCON_PLLOSEL_Get(void)
{
	return (RCC->PLLCON & RCC_PLLCON_PLLOSEL_Msk);
}

/* PLL输入选择寄存器 相关函数 */
void RCC_PLLCON_PLLINSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->PLLCON;
	tmpreg &= ~(RCC_PLLCON_PLLINSEL_Msk);
	tmpreg |= (SetValue & RCC_PLLCON_PLLINSEL_Msk);
	RCC->PLLCON = tmpreg;
}

uint32_t RCC_PLLCON_PLLINSEL_Get(void)
{
	return (RCC->PLLCON & RCC_PLLCON_PLLINSEL_Msk);
}

/* PLL使能寄存器 相关函数 */
void RCC_PLLCON_PLLEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		RCC->PLLCON |= (RCC_PLLCON_PLLEN_Msk);
	}
	else
	{
		RCC->PLLCON &= ~(RCC_PLLCON_PLLEN_Msk);
	}
}

FunState RCC_PLLCON_PLLEN_Getable(void)
{
	if (RCC->PLLCON & (RCC_PLLCON_PLLEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* RCLP使能状态，只读 相关函数 */
FlagStatus RCC_RCLPCON_RCLP_EN_B_Chk(void)
{
	if (RCC->RCLPCON & RCC_RCLPCON_RCLP_EN_B_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* RCLP频率调校寄存器，调校步长约4% 相关函数 */
void RCC_RCLPTRIM_Write(uint32_t SetValue)
{
	RCC->RCLPTRIM = (SetValue & RCC_RCLPTRIM_RCLPTRIM_Msk);
}

uint32_t RCC_RCLPTRIM_Read(void)
{
	return (RCC->RCLPTRIM & RCC_RCLPTRIM_RCLPTRIM_Msk);
}

/* XTLF工作电流选择 相关函数 */
void RCC_XTLFIPW_XTLFIPW_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->XTLFIPW;
	tmpreg &= ~(RCC_XTLFIPW_XTLFIPW_Msk);
	tmpreg |= (SetValue & RCC_XTLFIPW_XTLFIPW_Msk);
	RCC->XTLFIPW = tmpreg;
}

uint32_t RCC_XTLFIPW_XTLFIPW_Get(void)
{
	return (RCC->XTLFIPW & RCC_XTLFIPW_XTLFIPW_Msk);
}

/* ADC工作时钟选择 相关函数 */
void RCC_PERCLKCON2_ADCCKSEL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->PERCLKCON2;
	tmpreg &= ~(RCC_PERCLKCON2_ADCCKSEL_Msk);
	tmpreg |= (SetValue & RCC_PERCLKCON2_ADCCKSEL_Msk);
	RCC->PERCLKCON2 = tmpreg;
}

uint32_t RCC_PERCLKCON2_ADCCKSEL_Get(void)
{
	return (RCC->PERCLKCON2 & RCC_PERCLKCON2_ADCCKSEL_Msk);
}

/* AHB Master优先级配置 相关函数 */
void RCC_MPRIL_MPRIL_Set(uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = RCC->MPRIL;
	tmpreg &= ~(RCC_MPRIL_MPRIL_Msk);
	tmpreg |= (SetValue & RCC_MPRIL_MPRIL_Msk);
	RCC->MPRIL = tmpreg;
}

uint32_t RCC_MPRIL_MPRIL_Get(void)
{
	return (RCC->MPRIL & RCC_MPRIL_MPRIL_Msk);
}

void RCC_Deinit(void)
{
	RCC->SYSCLKSEL = 0x02008300;
	RCC->RCHFCON = 0x00000001;
	//RCC->RCHFTRIM = 0x00000040;
	RCC->PLLCON = 0x01F30000;
	//RCC->RCLPCON = ;
	RCC->RCLPTRIM = 0x00000008;
	//RCC->XTLFIPW = 0x00000000;
	RCC->PERCLKCON1 = 0x80000058;
	RCC->PERCLKCON2 = 0x00060000;
	RCC->PERCLKCON3 = 0x00000000;
	RCC->PERCLKCON4 = 0x00000000;
	RCC->MPRIL = 0x80000000;
}
//Code_End

/** 
 *  Returns the frequencies of different on chip clocks *
 */
void RCC_GetClocksFreq(RCC_ClocksType* para)
{
    uint32_t tmp32 = 0,tmp_value = 0;

	para->RCHF_Frequency = 0;
	para->PLL_Frequency = 0;
	para->SYSCLK_Frequency = 0;
	para->AHBCLK_Frequency = 0;
	para->APBCLK_Frequency = 0;
	
	/* Get RCHF FSEL */
	if(ENABLE == RCC_RCHFCON_RCHFEN_Getable())
	{
		tmp32 = RCC_RCHFCON_FSEL_Get();
		tmp32 = tmp32 >> RCC_RCHFCON_FSEL_Pos;  
		/* RCHF clock frequency */
		para->RCHF_Frequency = __RCHF_INITIAL_CLOCK * (tmp32 + 1);
	}

	if(ENABLE == RCC_PLLCON_PLLEN_Getable())
	{
		/* Get Pll frequency */
		tmp32 = RCC_PLLCON_PLLINSEL_Get();
		tmp32 = tmp32 >> RCC_PLLCON_PLLINSEL_Pos;
		switch(tmp32)
		{
			case 0x01: //01:RCLP
				tmp_value = __RCLP_CLOCK;
				break;
			case 0x02: //10:RCHF/256
				tmp_value = para->RCHF_Frequency/256;
				break;
			default: //00/11:XTLF
				tmp_value = __XTLF_CLOCK;
				break;
		}
		tmp32 = RCC_PLLCON_PLLDB_Get();
		tmp32 = tmp32 >> RCC_PLLCON_PLLDB_Pos;
		para->PLL_Frequency = (tmp32 + 1) * tmp_value;
	}
	
	/* Get SYSCLK source */
    tmp32 = RCC_SYSCLKSEL_SYSCLKSEL_Get();
    tmp32 = tmp32 >> RCC_SYSCLKSEL_SYSCLKSEL_Pos;
    switch (tmp32)
    {
        case 0x00:  /* RCHF used as system clock source */
            para->SYSCLK_Frequency = para->RCHF_Frequency;
            break;
        case 0x01:  /* RCLP used as system clock  source */
            para->SYSCLK_Frequency = __RCLP_CLOCK;
            break;
        case 0x02:  /* LSCLK used as system clock  source */
            para->SYSCLK_Frequency = __XTLF_CLOCK;
            break;
        case 0x03:  /* PLL used as system clock  source */
            para->SYSCLK_Frequency = para->PLL_Frequency;
            break;
        default:
            para->SYSCLK_Frequency = para->RCHF_Frequency;
            break;
    }
	
	/* Get AHB PRES */
    tmp32 = RCC_SYSCLKSEL_AHBPRES_Get();
    tmp32 = tmp32 >> RCC_SYSCLKSEL_AHBPRES_Pos;  
    switch(tmp32)
    {
        case 0x04: //100:DIV2
            para->AHBCLK_Frequency = para->SYSCLK_Frequency/2;
            break;
        case 0x05: //101:DIV4
            para->AHBCLK_Frequency = para->SYSCLK_Frequency/4;
            break;
        case 0x06: //110:DIV8
            para->AHBCLK_Frequency = para->SYSCLK_Frequency/8;
            break;
        case 0x07: //111:DIV16
            para->AHBCLK_Frequency = para->SYSCLK_Frequency/16;
            break;
        default: //0XX:DIV1
            para->AHBCLK_Frequency = para->SYSCLK_Frequency;
            break;
    }

	/* Get APB PRES */
    tmp32 = RCC_SYSCLKSEL_APBPRES_Get();
    tmp32 = tmp32 >> RCC_SYSCLKSEL_APBPRES_Pos; 
	/* APB clock frequency */
    switch(tmp32)
    {
        case 0x04: //100:DIV2
            para->APBCLK_Frequency = para->AHBCLK_Frequency/2;
            break;
        case 0x05: //101:DIV4
            para->APBCLK_Frequency = para->AHBCLK_Frequency/4;
            break;
        case 0x06: //110:DIV8
            para->APBCLK_Frequency = para->AHBCLK_Frequency/8;
            break;
        case 0x07: //111:DIV16
            para->APBCLK_Frequency = para->AHBCLK_Frequency/16;
            break;
        default: //0XX:DIV1
            para->APBCLK_Frequency = para->AHBCLK_Frequency;
            break;
    }
}

/* 外设时钟配置 */
void RCC_PERCLK_SetableEx(uint32_t periph_def, FunState NewState)
{
	__IO uint32_t *p_reg = 0;
	uint32_t TempREG;
	
    switch((periph_def & 0x0F000000))
    {
		case 0x01000000:
			p_reg = &(RCC->PERCLKCON1);
			break;
		
		case 0x02000000:
			p_reg = &(RCC->PERCLKCON2);
			break;
		
		case 0x03000000:
			p_reg = &(RCC->PERCLKCON3);
			break;
		
		case 0x04000000:
			p_reg = &(RCC->PERCLKCON4);
			break;
		
		default:
			break;
    }
	if(p_reg)
	{
		periph_def &= 0x000000FF;
		TempREG = (0x1U << periph_def);
		
		if (NewState != DISABLE)
		{
			*p_reg |= TempREG;
		}
		else
		{
			*p_reg &= ~TempREG;
		}
	}
}

/* RCHF初始化 */
void RCC_RCHF_Init(RCC_RCHF_InitTypeDef* para)
{
	RCC_RCHFCON_FSEL_Set(para->FSEL);
	RCC_RCHFCON_RCHFEN_Setable(para->RCHFEN);
}

/* PLL倍频数 相关函数 */
void RCC_PLLDB_WriteEx(uint32_t SetValue)
{	
	RCC_PLLCON_PLLDB_Set(SetValue<<RCC_PLLCON_PLLDB_Pos);
}

uint32_t RCC_PLLDB_ReadEx(void)
{
	uint32_t tmpreg;
	
	tmpreg = RCC_PLLCON_PLLDB_Get();
	return (tmpreg>>RCC_PLLCON_PLLDB_Pos);
}

/* PLL初始化 */
void RCC_PLL_Init(RCC_PLL_InitTypeDef* para)
{
	RCC_PLLDB_WriteEx(para->PLLDB);
	RCC_PLLCON_PLLOSEL_Set(para->PLLOSEL);
	RCC_PLLCON_PLLINSEL_Set(para->PLLINSEL);
	RCC_PLLCON_PLLEN_Setable(para->PLLEN);
}

/* 系统时钟初始化 */
void RCC_SysClk_Init(RCC_SYSCLK_InitTypeDef* para)
{
	RCC_SYSCLKSEL_SYSCLKSEL_Set(para->SYSCLKSEL);
	RCC_SYSCLKSEL_AHBPRES_Set(para->AHBPRES);
	RCC_SYSCLKSEL_APBPRES_Set(para->APBPRES);
	RCC_SYSCLKSEL_EXTICKSEL_Set(para->EXTICKSEL);
	RCC_SYSCLKSEL_SLP_ENEXTI_Setable(para->SLP_ENEXTI);
	RCC_SYSCLKSEL_LPM_RCLP_OFF_Setable(para->LPM_RCLP_OFF);
}

/* rchf常温校准值载入(芯片复位后自动载入8M的校准值) */
//ClkMode 1 = 8M
//ClkMode 2 = 16M
//ClkMode 3 = 24M
//ClkMode 4 = 32M
void RCC_Init_RCHF_Trim( uint8_t ClkMode )
{
	uint32_t Temp32;
	
	if( ClkMode == 1 )	//8M
	{
		Temp32 = const_rchf_Trim8;
	}
	else if( ClkMode == 2 )//16M
	{
		Temp32 = const_rchf_Trim16;
	}
	else if( ClkMode == 3 )//24M
	{
		Temp32 = const_rchf_Trim24;
	}
	else if( ClkMode == 4 )//32M
	{
		Temp32 = const_rchf_Trim32;
	}
	else//默认8M
	{
		Temp32 = const_rchf_Trim8;
	}
	
	if( ((Temp32>>16)&0x0000FFFF) == ((~Temp32)&0x0000FFFF) )	//正反码校验判断
	{
		RCC_RCHFTRIM_Write(Temp32&0x0000007F);
	}	
	else
	{
		RCC_RCHFTRIM_Write(0x00000040);
	}
}

/******END OF FILE****/
