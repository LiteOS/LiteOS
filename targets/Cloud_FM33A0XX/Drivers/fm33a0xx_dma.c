/**
  ******************************************************************************
  * @file    fm33a0xx_dma.c
  * @author  FM385 Application Team
  * @version V2.0.1
  * @date    27-3-2018
  * @brief   This file provides firmware functions to manage the following 
  *
*/

/* Includes ------------------------------------------------------------------*/
#include "fm33A0xx_dma.h" 

/** @addtogroup fm33a0_StdPeriph_Driver
  * @{
  */

/** @defgroup DMA 
  * @brief DMA driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup DMA_Private_Functions
  * @{
  */ 

//Code_Start
//2018-03-02-15-19-03
//FM33A0XX_Driver_Gen_V1.3

/* DMA全局使能 相关函数 */
void DMA_GLOBALCTRL_DMAEN_Setable(FunState NewState)
{
	if (NewState == ENABLE)
	{
		DMA->GLOBALCTRL |= (DMA_GLOBALCTRL_DMAEN_Msk);
	}
	else
	{
		DMA->GLOBALCTRL &= ~(DMA_GLOBALCTRL_DMAEN_Msk);
	}
}

FunState DMA_GLOBALCTRL_DMAEN_Getable(void)
{
	if (DMA->GLOBALCTRL & (DMA_GLOBALCTRL_DMAEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}


/* Channelx传输长度 相关函数 */
void DMA_CHxCTRL_TSIZE_Set(DMA_CH_Type CHx, uint32_t SetValue)
{
	uint32_t *pREG;
	uint32_t tmpreg;    
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);//DMA->CH0CTRL;
	
    SetValue <<= DMA_CHxCTRL_TSIZE_Pos;//手工添加
	tmpreg = *pREG;
	tmpreg &= ~(DMA_CHxCTRL_TSIZE_Msk);
	tmpreg |= (SetValue&DMA_CHxCTRL_TSIZE_Msk);
	*pREG = tmpreg;
}

uint32_t DMA_CHxCTRL_TSIZE_Get(DMA_CH_Type CHx)
{
	uint32_t *pREG;
    uint32_t tmpsize;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	
    tmpsize = (*pREG & DMA_CHxCTRL_TSIZE_Msk);
    tmpsize >>= DMA_CHxCTRL_TSIZE_Pos;
	return tmpsize;
}

/* Channelx优先级 相关函数 */
void DMA_CHxCTRL_PRI_Set(DMA_CH_Type CHx, uint32_t SetValue)
{
	uint32_t *pREG;
	uint32_t tmpreg;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	
	tmpreg = *pREG;
	tmpreg &= ~(DMA_CHxCTRL_PRI_Msk);
	tmpreg |= (SetValue&DMA_CHxCTRL_PRI_Msk);
	*pREG = tmpreg;
}

uint32_t DMA_CHxCTRL_PRI_Get(DMA_CH_Type CHx)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	return (*pREG & DMA_CHxCTRL_PRI_Msk);
}

/* Channelx RAM地址增减设置 相关函数 */
void DMA_CHxCTRL_INC_Set(DMA_CH_Type CHx, uint32_t SetValue)
{
	uint32_t *pREG;
	uint32_t tmpreg;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	
	tmpreg = *pREG;
	tmpreg &= ~(DMA_CHxCTRL_INC_Msk);
	tmpreg |= (SetValue&DMA_CHxCTRL_INC_Msk);
	*pREG = tmpreg;
}

uint32_t DMA_CHxCTRL_INC_Get(DMA_CH_Type CHx)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	return (*pREG & DMA_CHxCTRL_INC_Msk);
}

/* Channelx外设通道选择 相关函数 */
void DMA_CHxCTRL_SSEL_Set(DMA_CH_Type CHx, uint32_t SetValue)
{
	uint32_t *pREG;
	uint32_t tmpreg;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	
	tmpreg = *pREG;
	tmpreg &= ~(DMA_CHxCTRL_SSEL_Msk);
	tmpreg |= (SetValue&DMA_CHxCTRL_SSEL_Msk);
	*pREG = tmpreg;
}

uint32_t DMA_CHxCTRL_SSEL_Get(DMA_CH_Type CHx)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	return (*pREG & DMA_CHxCTRL_SSEL_Msk);
}

/* Channelx传输完成中断使能 相关函数 */
void DMA_CHxCTRL_FTIE_Setable(DMA_CH_Type CHx, FunState NewState)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	if (NewState == ENABLE)
	{
		*pREG |= (DMA_CHxCTRL_FTIE_Msk);
	}
	else
	{
		*pREG &= ~(DMA_CHxCTRL_FTIE_Msk);
	}
}

FunState DMA_CHxCTRL_FTIE_Getable(DMA_CH_Type CHx)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	if (*pREG & (DMA_CHxCTRL_FTIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Channelx半程传输完成中断使能 相关函数 */
void DMA_CHxCTRL_HTIE_Setable(DMA_CH_Type CHx, FunState NewState)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	if (NewState == ENABLE)
	{
		*pREG |= (DMA_CHxCTRL_HTIE_Msk);
	}
	else
	{
		*pREG &= ~(DMA_CHxCTRL_HTIE_Msk);
	}
}

FunState DMA_CHxCTRL_HTIE_Getable(DMA_CH_Type CHx)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	if (*pREG & (DMA_CHxCTRL_HTIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Channelx使能 相关函数 */
void DMA_CHxCTRL_EN_Setable(DMA_CH_Type CHx, FunState NewState)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + CHx*8);
	if (NewState == ENABLE)
	{
		*pREG |= (DMA_CHxCTRL_EN_Msk);
	}
	else
	{
		*pREG &= ~(DMA_CHxCTRL_EN_Msk);
	}
}

/* Channelx指针寄存器 相关函数 */
void DMA_CHxRAMAD_Write(DMA_CH_Type CHx, uint32_t SetValue)
{
	uint32_t *pREG;
	
	pREG = (uint32_t *)(0x40000404 + 4 + CHx*8);
	*pREG = SetValue;
}

//uint32_t DMA_CHxRAMAD_Read(DMA_CH_Type CHx)
//{
//	uint32_t *pREG;
//	
//	pREG = (uint32_t *)(0x40000404 + 4 + CHx*8);
//	return (*pREG);
//}


/* Channel7FLASH指针地址 相关函数 */
void DMA_CH7FLSAD_Write(uint32_t SetValue)
{
//	DMA->CH7FLSAD = SetValue;
	DMA->CH7FLSAD = SetValue>>2;//CH7的flash地址是字地址，此处做了右移2bit处理，以便使用时可以使用普通地址
}

//uint32_t DMA_CH7FLSAD_Read(void)
//{
//	return (DMA->CH7FLSAD);
//}

/* Channel7RAM字指针地址(word地址) 相关函数 */
void DMA_CH7RAMAD_Write(uint32_t SetValue)
{
//	DMA->CH7RAMAD = SetValue;
	DMA->CH7RAMAD = SetValue>>2;//CH7的RAM地址是字地址，此处做了右移2bit处理，以便使用时可以使用普通地址
}

//uint32_t DMA_CH7RAMAD_Read(void)
//{
//	return (DMA->CH7RAMAD);
//}


/* Channelx传输半程标志 相关函数 */
void DMA_CHSTATUS_DMACHxHT_Clr(DMA_CH_Type CHx)
{
	DMA->CHSTATUS = (0x1U<<CHx);
}

FlagStatus DMA_CHSTATUS_DMACHxHT_Chk(DMA_CH_Type CHx)
{
	if (DMA->CHSTATUS & (0x1U<<CHx))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* Channelx传输完成标志 相关函数 */
void DMA_CHSTATUS_DMACHxFT_Clr(DMA_CH_Type CHx)
{
	DMA->CHSTATUS = (0x100U<<CHx);
}

FlagStatus DMA_CHSTATUS_DMACHxFT_Chk(DMA_CH_Type CHx)
{
	if (DMA->CHSTATUS & (0x100U<<CHx))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


void DMA_Deinit(void)
{
	DMA->GLOBALCTRL = 0x00000000;
	DMA->CH0CTRL = 0x00000000;
	DMA->CH0RAMAD = 0x00000000;
	DMA->CH1CTRL = 0x00000000;
	DMA->CH1RAMAD = 0x00000000;
	DMA->CH2CTRL = 0x00000000;
	DMA->CH2RAMAD = 0x00000000;
	DMA->CH3CTRL = 0x00000000;
	DMA->CH3RAMAD = 0x00000000;
	DMA->CH4CTRL = 0x00000000;
	DMA->CH4RAMAD = 0x00000000;
	DMA->CH5CTRL = 0x00000000;
	DMA->CH5RAMAD = 0x00000000;
	DMA->CH6CTRL = 0x00000000;
	DMA->CH6RAMAD = 0x00000000;
	DMA->CH7CTRL = 0x00000000;
	DMA->CH7FLSAD = 0x00000000;
	DMA->CH7RAMAD = 0x00000000;
	DMA->CHSTATUS = 0x00000000;
}
//Code_End
          

void DMA_Init(DMA_InitTypeDef* para)
{
	if( DMA_CH7 == para->CHx )
	{
		DMA_CH7RAMAD_Write(para->CHxRAMAD);//CH0~CH7通道RAM指针地址
		DMA_CH7FLSAD_Write(para->CH7FLSAD);//通道FLASH指针地址，仅针对通道7有意义
	}
	else
	{
		DMA_CHxRAMAD_Write(para->CHx, para->CHxRAMAD);//CH0~CH7通道RAM指针地址
	}
	
	DMA_CHxCTRL_TSIZE_Set(para->CHx, para->CHxTSIZE);	//通道传输长度
	DMA_CHxCTRL_PRI_Set(para->CHx, para->CHxPRI);		//通道优先级
	DMA_CHxCTRL_INC_Set(para->CHx, para->CHxINC);		//通道地址增长方向
	DMA_CHxCTRL_SSEL_Set(para->CHx, para->CHxSSEL);		//CH0~CH6外设通道选择,CH7传输方式选择
	DMA_CHxCTRL_FTIE_Setable(para->CHx, para->CHxFTIE);	//通道传输完成中断使能
	DMA_CHxCTRL_HTIE_Setable(para->CHx, para->CHxHTIE);	//通道传输半程中断使能
	DMA_CHxCTRL_EN_Setable(para->CHx, para->CHxEN);		//通道使能
}

/******END OF FILE****/



