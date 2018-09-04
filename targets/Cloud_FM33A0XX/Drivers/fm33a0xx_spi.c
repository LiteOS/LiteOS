/**
  ******************************************************************************
  * @file    fm33a0xx_spi.c
  * @author  
  * @version V2.0.1
  * @date    
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of SPI:           
  *           
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/ 

#include "fm33A0xx_spi.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



//Code_Start
//2018-04-02-13-22-56
//FM33A0XX_Driver_Gen_V1.4

/* Master对MISO信号的采样位置调整，用于高速通信时补偿PCB走线延迟 相关函数 */
void SPIx_SPICR1_MSPA_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_MSPA_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_MSPA_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_MSPA_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_MSPA_Msk);
}

/* Slave MISO发送位置调整 相关函数 */
void SPIx_SPICR1_SSPA_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_SSPA_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_SSPA_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_SSPA_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_SSPA_Msk);
}

/* MASTER/SLAVE模式选择 相关函数 */
void SPIx_SPICR1_MM_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_MM_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_MM_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_MM_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_MM_Msk);
}

void SPIx_SPICR1_WAIT_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_WAIT_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_WAIT_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_WAIT_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_WAIT_Msk);
}

/* MASTER模式波特率配置 相关函数 */
void SPIx_SPICR1_BAUD_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_BAUD_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_BAUD_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_BAUD_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_BAUD_Msk);
}

/* 帧格式 相关函数 */
void SPIx_SPICR1_LSBF_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_LSBF_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_LSBF_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_LSBF_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_LSBF_Msk);
}

/* 时钟极性选择 相关函数 */
void SPIx_SPICR1_CPHOL_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_CPHOL_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_CPHOL_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_CPHOL_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_CPHOL_Msk);
}

/* 时钟相位选择 相关函数 */
void SPIx_SPICR1_CPHA_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR1;
	tmpreg &= ~(SPIx_SPICR1_CPHA_Msk);
	tmpreg |= (SetValue & SPIx_SPICR1_CPHA_Msk);
	SPIx->SPICR1 = tmpreg;
}

uint32_t SPIx_SPICR1_CPHA_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR1 & SPIx_SPICR1_CPHA_Msk);
}

/* Slave输入管脚滤波使能（SSN/SCK/MOSI） 相关函数 */
void SPIx_SPICR2_FLTEN_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPICR2 |= (SPIx_SPICR2_FLTEN_Msk);
	}
	else
	{
		SPIx->SPICR2 &= ~(SPIx_SPICR2_FLTEN_Msk);
	}
}

FunState SPIx_SPICR2_FLTEN_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPICR2 & (SPIx_SPICR2_FLTEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Master模式下SSN控制模式选择 相关函数 */
void SPIx_SPICR2_SSNM_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR2;
	tmpreg &= ~(SPIx_SPICR2_SSNM_Msk);
	tmpreg |= (SetValue & SPIx_SPICR2_SSNM_Msk);
	SPIx->SPICR2 = tmpreg;
}

uint32_t SPIx_SPICR2_SSNM_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR2 & SPIx_SPICR2_SSNM_Msk);
}

/* TXONLY硬件自动清空的使能 相关函数 */
void SPIx_SPICR2_TXO_AC_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR2;
	tmpreg &= ~(SPIx_SPICR2_TXO_AC_Msk);
	tmpreg |= (SetValue & SPIx_SPICR2_TXO_AC_Msk);
	SPIx->SPICR2 = tmpreg;
}

uint32_t SPIx_SPICR2_TXO_AC_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR2 & SPIx_SPICR2_TXO_AC_Msk);
}

/* TXONLY控制位 相关函数 */
void SPIx_SPICR2_TXO_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPICR2 |= (SPIx_SPICR2_TXO_Msk);
	}
	else
	{
		SPIx->SPICR2 &= ~(SPIx_SPICR2_TXO_Msk);
	}
}

FunState SPIx_SPICR2_TXO_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPICR2 & (SPIx_SPICR2_TXO_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Master模式下，软件可以通过此位控制SSN输出电平 相关函数 */
void SPIx_SPICR2_SSN_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR2;
	tmpreg &= ~(SPIx_SPICR2_SSN_Msk);
	tmpreg |= (SetValue & SPIx_SPICR2_SSN_Msk);
	SPIx->SPICR2 = tmpreg;
}

uint32_t SPIx_SPICR2_SSN_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR2 & SPIx_SPICR2_SSN_Msk);
}

/* Master模式下，SSN的控制方式 相关函数 */
void SPIx_SPICR2_SSNSEN_Set(SPIx_Type* SPIx, uint32_t SetValue)
{
	uint32_t tmpreg;
	tmpreg = SPIx->SPICR2;
	tmpreg &= ~(SPIx_SPICR2_SSNSEN_Msk);
	tmpreg |= (SetValue & SPIx_SPICR2_SSNSEN_Msk);
	SPIx->SPICR2 = tmpreg;
}

uint32_t SPIx_SPICR2_SSNSEN_Get(SPIx_Type* SPIx)
{
	return (SPIx->SPICR2 & SPIx_SPICR2_SSNSEN_Msk);
}

/* SPI使能 相关函数 */
void SPIx_SPICR2_SPIEN_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPICR2 |= (SPIx_SPICR2_SPIEN_Msk);
	}
	else
	{
		SPIx->SPICR2 &= ~(SPIx_SPICR2_SPIEN_Msk);
	}
}

FunState SPIx_SPICR2_SPIEN_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPICR2 & (SPIx_SPICR2_SPIEN_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

void SPIx_SPICR3_TXBFC_Clr(SPIx_Type* SPIx)
{
	SPIx->SPICR3 = SPIx_SPICR3_TXBFC_Msk;
}

void SPIx_SPICR3_RXBFC_Clr(SPIx_Type* SPIx)
{
	SPIx->SPICR3 = SPIx_SPICR3_RXBFC_Msk;
}

void SPIx_SPICR3_MERRC_Clr(SPIx_Type* SPIx)
{
	SPIx->SPICR3 = SPIx_SPICR3_MERRC_Msk;
}

void SPIx_SPICR3_SERRC_Clr(SPIx_Type* SPIx)
{
	SPIx->SPICR3 = SPIx_SPICR3_SERRC_Msk;
}

void SPIx_SPIIE_ERRIE_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPIIE |= (SPIx_SPIIE_ERRIE_Msk);
	}
	else
	{
		SPIx->SPIIE &= ~(SPIx_SPIIE_ERRIE_Msk);
	}
}

FunState SPIx_SPIIE_ERRIE_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPIIE & (SPIx_SPIIE_ERRIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

void SPIx_SPIIE_TXIE_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPIIE |= (SPIx_SPIIE_TXIE_Msk);
	}
	else
	{
		SPIx->SPIIE &= ~(SPIx_SPIIE_TXIE_Msk);
	}
}

FunState SPIx_SPIIE_TXIE_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPIIE & (SPIx_SPIIE_TXIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

void SPIx_SPIIE_RXIE_Setable(SPIx_Type* SPIx, FunState NewState)
{
	if (NewState == ENABLE)
	{
		SPIx->SPIIE |= (SPIx_SPIIE_RXIE_Msk);
	}
	else
	{
		SPIx->SPIIE &= ~(SPIx_SPIIE_RXIE_Msk);
	}
}

FunState SPIx_SPIIE_RXIE_Getable(SPIx_Type* SPIx)
{
	if (SPIx->SPIIE & (SPIx_SPIIE_RXIE_Msk))
	{
		return ENABLE;
	}
	else
	{
		return DISABLE;
	}
}

/* Master Error标志 相关函数 */
FlagStatus SPIx_SPIIF_MERR_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_MERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* Slave Error标志 相关函数 */
FlagStatus SPIx_SPIIF_SERR_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_SERR_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 接收缓存溢出，软件写1清零 相关函数 */
void SPIx_SPIIF_RXCOL_Clr(SPIx_Type* SPIx)
{
	SPIx->SPIIF = SPIx_SPIIF_RXCOL_Msk;
}

FlagStatus SPIx_SPIIF_RXCOL_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_RXCOL_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* 发送缓存溢出，软件写1清零 相关函数 */
void SPIx_SPIIF_TXCOL_Clr(SPIx_Type* SPIx)
{
	SPIx->SPIIF = SPIx_SPIIF_TXCOL_Msk;
}

FlagStatus SPIx_SPIIF_TXCOL_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_TXCOL_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* SPI空闲标志，只读 相关函数 */
FlagStatus SPIx_SPIIF_BUSY_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_BUSY_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* TX Buffer Empty标志位 相关函数 */
FlagStatus SPIx_SPIIF_TXBE_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_TXBE_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* RX Buffer Full标志位 相关函数 */
FlagStatus SPIx_SPIIF_RXBF_Chk(SPIx_Type* SPIx)
{
	if (SPIx->SPIIF & SPIx_SPIIF_RXBF_Msk)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/* SPI发送缓存 相关函数 */
void SPIx_SPITXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue)
{
	SPIx->SPITXBUF = (SetValue & SPIx_SPITXBUF_TXBUF_Msk);
}

uint32_t SPIx_SPITXBUF_Read(SPIx_Type* SPIx)
{
	return (SPIx->SPITXBUF & SPIx_SPITXBUF_TXBUF_Msk);
}

/* SPI接收缓存 相关函数 */
void SPIx_SPIRXBUF_Write(SPIx_Type* SPIx, uint32_t SetValue)
{
	SPIx->SPIRXBUF = (SetValue & SPIx_SPIRXBUF_RXBUF_Msk);
}

uint32_t SPIx_SPIRXBUF_Read(SPIx_Type* SPIx)
{
	return (SPIx->SPIRXBUF & SPIx_SPIRXBUF_RXBUF_Msk);
}

void SPIx_Deinit(SPIx_Type* SPIx)
{
	SPIx->SPICR1 = 0x00000108;
	SPIx->SPICR2 = 0x00000054;
	SPIx->SPICR3 = 0x00000000;
	SPIx->SPIIE = 0x00000000;
	//SPIx->SPIIF = 0x00000002;
	//SPIx->SPITXBUF = 0x00000000;
	//SPIx->SPIRXBUF = 0x00000000;
}
//Code_End



//Code_Start_HandBy
//void SPI_Master_Init(SPIx_Type* SPIx, SPI_Master_InitTypeDef* para)
void SPI_Master_Init(SPIx_Type* SPIx, SPI_Master_SInitTypeDef* para)
{
	SPI_Master_InitTypeDef init_para;

	SPIx_Deinit(SPIx);

	init_para.MSPA = SPIx_SPICR1_MSPA_NORMAL;
	init_para.WAIT = SPIx_SPICR1_WAIT_WAIT_1;
	init_para.BAUD_RATE = para->BAUD_RATE;
	init_para.LSBF = para->LSBF;
	init_para.CPHOL = para->CPHOL;
	init_para.CPHA = para->CPHA;
	init_para.SSNM = para->SSNM;
	init_para.TXO = DISABLE;
	init_para.TXO_AC = SPIx_SPICR2_TXO_AC_CLR_AT_CLOSE;
	init_para.SSNSEN = para->SSNSEN;

	init_para.SPIEN = DISABLE;
	init_para.ERRIE = DISABLE;
	init_para.TXIE = DISABLE;
	init_para.RXIE = DISABLE;
	
	SPIx_SPICR1_MSPA_Set(SPIx, init_para.MSPA);
	SPIx_SPICR1_WAIT_Set(SPIx, init_para.WAIT);
	SPIx_SPICR1_BAUD_Set(SPIx, init_para.BAUD_RATE);
	SPIx_SPICR1_LSBF_Set(SPIx, init_para.LSBF);
	SPIx_SPICR1_CPHOL_Set(SPIx, init_para.CPHOL);
	SPIx_SPICR1_CPHA_Set(SPIx, init_para.CPHA);
	SPIx_SPICR2_SSNM_Set(SPIx, init_para.SSNM);
	SPIx_SPICR2_TXO_Setable(SPIx, init_para.TXO);
	SPIx_SPICR2_TXO_AC_Set(SPIx, init_para.TXO_AC);
	SPIx_SPICR2_SSNSEN_Set(SPIx, init_para.SSNSEN);

	SPIx_SPICR1_MM_Set(SPIx, SPIx_SPICR1_MM_MASTER);//配置成主模式
}


void SPI_Slave_Init(SPIx_Type* SPIx, SPI_Slave_SInitTypeDef* para)
{
	SPI_Slave_InitTypeDef init_para;

	SPIx_Deinit(SPIx);

	init_para.SSPA = SPIx_SPICR1_SSPA_NORMAL;
	init_para.FLTEN = ENABLE;
	init_para.LSBF = para->LSBF;
	init_para.CPHOL = para->CPHOL;
	init_para.CPHA = para->CPHA;
	init_para.TXO = DISABLE;
	init_para.TXO_AC = SPIx_SPICR2_TXO_AC_CLR_AT_CLOSE;

	init_para.SPIEN = DISABLE;
	init_para.ERRIE = DISABLE;
	init_para.TXIE = DISABLE;
	init_para.RXIE = DISABLE;
	
	SPIx_SPICR1_SSPA_Set(SPIx, init_para.SSPA);
	SPIx_SPICR2_FLTEN_Setable(SPIx, init_para.FLTEN);
	SPIx_SPICR1_LSBF_Set(SPIx, init_para.LSBF);
	SPIx_SPICR1_CPHOL_Set(SPIx, init_para.CPHOL);
	SPIx_SPICR1_CPHA_Set(SPIx, init_para.CPHA);
	SPIx_SPICR2_TXO_Setable(SPIx, init_para.TXO);
	SPIx_SPICR2_TXO_AC_Set(SPIx, init_para.TXO_AC);

	SPIx_SPICR1_MM_Set(SPIx, SPIx_SPICR1_MM_SLAVE);//配置成从模式
}


void SPI_SSN_Set_Low(SPIx_Type* SPIx)//驱动级
{
	SPIx_SPICR2_SSN_Set(SPIx, SPIx_SPICR2_SSN_LOW);//拉低SSN信号
}

void SPI_SSN_Set_High(SPIx_Type* SPIx)//驱动级
{
	SPIx_SPICR2_SSN_Set(SPIx, SPIx_SPICR2_SSN_HIGH);//拉高SSN信号
}

unsigned char SPI_Recv_Byte(SPIx_Type* SPIx)//SPI接收一字节
{
	return SPIx_SPIRXBUF_Read(SPIx);
}
void SPI_Send_Byte(SPIx_Type* SPIx, unsigned char data)//SPI发送一字节
{
	SPIx_SPITXBUF_Write(SPIx, data);
}
unsigned char SPI_RW_Byte(SPIx_Type* SPIx, unsigned char data)//SPI发送并接收一字节
{
    unsigned char rx_data;

    while(1)
    {
    	if (SPIx_SPIIF_TXBE_Chk(SPIx) == SET)
		{
			break;
		}		
    }
    SPI_Send_Byte(SPIx, data);

    while(1)
    {
    	if (SPIx_SPIIF_RXBF_Chk(SPIx) == SET)
        {
            break;
        }
    }
    rx_data = SPI_Recv_Byte(SPIx);
    return rx_data;
}

//Code_End

/******END OF FILE****/

