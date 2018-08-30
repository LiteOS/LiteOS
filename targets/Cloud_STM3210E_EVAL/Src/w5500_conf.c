/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/*
**************************************************************************************************
* @file         w5500_conf.c
* @author       WIZnet Software Team 
* @version      V1.0
* @date         2015-02-14
* @brief        
**************************************************************************************************
*/

/* Includes -----------------------------------------------------------------*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"

#include "dwt.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

#include "los_swtmr.h"
#include "los_sys.h"

/* Defines ------------------------------------------------------------------*/
/* SPI I/O defines */
#define WIZ_SPIx                SPI2
#define WIZ_SPIx_GPIO_PORT      GPIOB
#define WIZ_SPIx_SCLK_PIN       GPIO_PIN_13
#define WIZ_SPIx_MISO_PIN       GPIO_PIN_14
#define WIZ_SPIx_MOSI_PIN       GPIO_PIN_15

#define WIZ_SPIx_SCS_PIN        GPIO_PIN_11
#define WIZ_SPIx_SCS_PORT       GPIOF

#define WIZ_INT_PIN             GPIO_PIN_9
#define WIZ_INT_PORT            GPIOF
#define WIZ_IRQn                EXTI9_5_IRQn
#define WIZ_INT_PRIORITY        0

#define WIZ_RESET_PIN           GPIO_PIN_8
#define WIZ_RESET_PORT          GPIOF

#define WIZ_ENABLE(__HANDLE__)      __HAL_SPI_ENABLE(__HANDLE__)
#define WIZ_DISABLE(__HANDLE__)     __HAL_SPI_DISABLE(__HANDLE__)
#define WIZ_RCC_CLK_ENABLE()        __HAL_RCC_SPI2_CLK_ENABLE()
#define WIZ_RCC_CLK_DISABLE()       __HAL_RCC_SPI2_CLK_DISABLE()

#define WIZ_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define WIZ_CS_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()
#define WIZ_INT_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()
#define WIZ_INT_CLK_DISABLE()       __HAL_RCC_GPIOF_CLK_DISABLE();
#define WIZ_RESET_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()

#define WIZ_CS_ENABLE()             HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_RESET)
#define WIZ_CS_DISABLE()            HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_SET)

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
CONFIG_MSG      ConfigMsg;
EEPROM_MSG_STR  EEPROM_MSG;

/* mac address */
uint8 mac[6] = {0x00,0x08,0xdc,0x11,0x11,0x11};

/* default local ip infor */
uint8 local_ip[4]   = {192,168,1,231};
uint8 subnet[4]     = {255,255,255,0};
uint8 gateway[4]    = {192,168,1,1};
uint8 dns_server[4] = {114,114,114,114};

uint16 local_port   = 5000;

/* remote/server ip infor */
uint8  remote_ip[4] = {192,168,0,106};
uint16 remote_port  = 8080;

/* ip source */
uint8	ip_from = IP_FROM_DEFINE;				

uint8   dhcp_ok   = 0;		/* dhcp got IP flag */
uint32	dhcp_time = 0;		/* DHCP counter */
vuint8	ntptimer  = 0;		/* NTP second counter */

SPI_HandleTypeDef g_w5500_hspi;

/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
extern void wiznet_irq_handler(void);
/* Private functions --------------------------------------------------------*/
static void SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (hspi->Instance == WIZ_SPIx)
    {
        WIZ_RCC_CLK_ENABLE();
        WIZ_GPIO_CLK_ENABLE();
        WIZ_CS_CLK_ENABLE();
        WIZ_RESET_CLK_ENABLE();

        /* CS */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Pin = WIZ_SPIx_SCS_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStruct);

        /* SCLK, MISO, MOSI */
        GPIO_InitStruct.Pin = WIZ_SPIx_SCLK_PIN | WIZ_SPIx_MISO_PIN | WIZ_SPIx_MOSI_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStruct);

        /* RESET */
        GPIO_InitStruct.Pin = WIZ_RESET_PIN; 
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(WIZ_RESET_PORT, &GPIO_InitStruct);	
    }
}

static void SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == WIZ_SPIx)
    {
        WIZ_RCC_CLK_DISABLE();
        HAL_GPIO_DeInit(WIZ_SPIx_GPIO_PORT, WIZ_SPIx_SCLK_PIN | WIZ_SPIx_MISO_PIN | WIZ_SPIx_MOSI_PIN);
        HAL_GPIO_DeInit(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN);
        HAL_GPIO_DeInit(WIZ_INT_PORT, WIZ_INT_PIN);
        HAL_GPIO_DeInit(WIZ_RESET_PORT, WIZ_RESET_PIN);
        WIZ_DISABLE(hspi);
    }
}

static void w5500_irq_handler(void)
{
    /* EXTI line interrupt detected */
    if(__HAL_GPIO_EXTI_GET_IT(WIZ_INT_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(WIZ_INT_PIN);
        wiznet_irq_handler();
    }
}

/**
 * @brief   Configure w5500 gpio
 *          SPI is divided into four modes by different states of CPOL and CPHA
 *              MODE    CPOL    CPHA    SCL     SAMPLE
 *          ----------------------------------------------    
 *               0       0       0      LOW     1EDGE
 *               1       0       1      LOW     2EDGE
 *               2       1       0      HIGH    1EDGE
 *               3       1       1      HIGH    2EDGE
 *
 *          W5500 supports SPI mode 0 and mode 3, Mode 3 is configured here.   
 * @param   none
 * @return  none
 */
void w5500_config(void)
{
    g_w5500_hspi.Instance = WIZ_SPIx;
    g_w5500_hspi.State = HAL_SPI_STATE_RESET; // note
    g_w5500_hspi.Init.Mode = SPI_MODE_MASTER;
    g_w5500_hspi.Init.Direction = SPI_DIRECTION_2LINES;
    g_w5500_hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    g_w5500_hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    g_w5500_hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
    g_w5500_hspi.Init.NSS = SPI_NSS_SOFT;
    g_w5500_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    g_w5500_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_w5500_hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    g_w5500_hspi.Init.CRCPolynomial = 7;

    SPI_MspInit(&g_w5500_hspi);
    
    HAL_SPI_Init(&g_w5500_hspi);
    WIZ_ENABLE(&g_w5500_hspi);
    WIZ_CS_DISABLE();
}

void w5500_interrupt_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    WIZ_INT_CLK_ENABLE();
    
    /* INT */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = WIZ_INT_PIN;	
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    LOS_HwiCreate(WIZ_IRQn, WIZ_INT_PRIORITY, 0, w5500_irq_handler, 0);
    HAL_GPIO_Init(WIZ_INT_PORT, &GPIO_InitStruct);
}

/**
 * @brief   Initialize w5500
 * @param   none
 * @return  none
 */
void w5500_init(void)
{
    w5500_config();
    w5500_interrupt_config();
    w5500_reset();

    IINCHIP_WRITE(SIMR, 0xff);
}

void w5500_deinit(void)
{
    WIZ_INT_CLK_DISABLE();
    HAL_SPI_DeInit(&g_w5500_hspi);
    SPI_MspDeInit(&g_w5500_hspi);
}

/**
 * @brief   Hard reset w5500
 * @param   none
 * @return  none
 */
void w5500_reset(void)
{
    HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_RESET);
    wizDelayMs(10);  
    HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_SET);
    wizDelayMs(10);
}

/**
 * @brief   Configure w5500 ip address
 * @param   none
 * @return  none
 */
void w5500_set_ip(void)
{
	memcpy(ConfigMsg.mac, mac,          6);
	memcpy(ConfigMsg.lip, local_ip,     4);
	memcpy(ConfigMsg.sub, subnet,       4);
	memcpy(ConfigMsg.gw,  gateway,      4);
	memcpy(ConfigMsg.dns, dns_server,   4);
    
	if(ip_from == IP_FROM_DEFINE)
    {
		printf(" IP from DEFINE\r\n");
	}
    /* Use ip infor from EEPROM */
	else if(ip_from == IP_FROM_EEPROM)
	{
		read_config_from_eeprom();		
		
		/* match mac */		
		if( EEPROM_MSG.mac[0] == mac[0] && EEPROM_MSG.mac[1] == mac[1] && EEPROM_MSG.mac[2] == mac[2])		
		{
			printf(" IP from EEPROM\r\n");
			memcpy(ConfigMsg.lip, EEPROM_MSG.lip,   4);				
			memcpy(ConfigMsg.sub, EEPROM_MSG.sub,   4);
			memcpy(ConfigMsg.gw,  EEPROM_MSG.gw,    4);
		}
		else
		{
            /* The EEPROM is not configured, and W5500 is configured using defined IP information 
             and written to the EEPROM */
			printf(" Use default IP infor and write to EEPROM\r\n");
			write_config_to_eeprom();	
		}			
	}
	/* Use ip infor from DHCP, need invoke DHCP subprograme */		
	else if(ip_from == IP_FROM_DHCP)								
	{
		if(dhcp_ok == 1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
	}
	/* config version infor */
	ConfigMsg.sw_ver[0] = FW_VER_HIGH;
	ConfigMsg.sw_ver[1] = FW_VER_LOW;	

	/* write ip infors to w5500 */
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);
	getSUBR(subnet);
	getGAR(gateway);
	printf(" W5500 IP           : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	printf(" W5500 subnet mask  : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	printf(" W5500 gateway      : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
 * @brief   Configure w5500 mac address
 * @param   none
 * @return  none
 */
void w5500_set_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
 * @brief   Set the chip port SCSn of W5500 to be low
 * @param   none
 * @return  none
 */
void iinchip_csoff(void)
{
	WIZ_CS_ENABLE();
}

/**
 * @brief   Set the chip port SCSn of W5500 to be high
 * @param   none
 * @return  none
 */
void iinchip_cson(void)
{	
   WIZ_CS_DISABLE();
}

/**
 * @brief   Send one byte data
 * @param   dat: the send data
 * @return  received data
 */
uint8  IINCHIP_SpiSendData(uint8 dat)
{
    uint8 rxdata = 0;
    uint8 txdata = dat;
#if 0    
    /* Loop while DR register in not emplty */
    while (__HAL_SPI_GET_FLAG(&g_w5500_hspi, SPI_FLAG_TXE) == RESET);

    /* Send byte through the SPI1 peripheral */
    HAL_SPI_Transmit(&g_w5500_hspi, &txdata, 1, HAL_MAX_DELAY);

    /* Wait to receive a byte */
    while (__HAL_SPI_GET_FLAG(&g_w5500_hspi, SPI_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    HAL_SPI_Receive(&g_w5500_hspi, &rxdata, 1, HAL_MAX_DELAY);
#else
    HAL_SPI_TransmitReceive(&g_w5500_hspi, &txdata, &rxdata, 1, HAL_MAX_DELAY);
#endif
    
    return rxdata;
}

/**
 * @brief   write data to w5500
 * @param   addrbsb : The address of write data
 * @param   data    : Written data
 * @return  none
 */
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
 * @brief   read one byte data from w5500
 * @param   addrbsb : The address of read data
 * @return  read data
 */
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}
/**
 * @brief   Write len bytes data to W5500
 * @param   addrbsb : The address of write data
 * @param   buf : buffer of data
 * @param   len : length of data
 * @return  length of written data
 */
uint16 wiz_write_buf(uint32 addrbsb,  uint8* buf, uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}
/**
 * @brief   Read len byte data from W5500
 * @param   addrbsb : The address of read data
 * @param   buf : buffer of data
 * @param   len : length of data
 * @return  length of read data
 */
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}

/**
 * @brief   Write config infor to eeprom
 * @param   none
 * @return  none
 */
void write_config_to_eeprom(void)
{
//	uint16 dAddr = 0;
//	eeprom_WriteBytes(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN);				
	wizDelayMs(10);																							
}
/**
 * @brief   Read config infor from eeprom
 * @param   none
 * @return  none
 */
void read_config_from_eeprom(void)
{
//	eeprom_ReadBytes(EEPROM_MSG.mac,0,EEPROM_MSG_LEN);
	wizDelayUs(10);
}

static void Timer1_Callback(UINT32 arg)
{
    dhcp_time++;
#ifndef	__NTP_H__
	ntptimer++;
#endif
}

void swTimer1_init(void)
{
    UINT16 id1;
    UINT32 uwRet = LOS_OK;
    
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_PERIOD, Timer1_Callback, &id1, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_PERIOD, Timer1_Callback, &id1, 1);
#endif
    if (LOS_OK != uwRet)
    {
        printf("create Timer1 failed\n");
    }
    else
    {
        printf("create Timer1 success\n");
    }

    uwRet = LOS_SwtmrStart(id1);
    if (LOS_OK != uwRet)
    {
        printf("start Timer1 failed\n");
    }
    else
    {
        printf("start Timer1 sucess\n");
    }
}

void dhcp_timer_init(void)
{
    swTimer1_init();																	
}

void ntp_timer_init(void)
{
    swTimer1_init();																	
}

/* mcu software reset */
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf("system rebooting...\r\n");
  JumpAddress = *(volatile uint32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

void wizDelayUs(uint32 usec)
{
    delayus(usec);
}

void wizDelayMs(uint32 msec)
{
    delayms(msec);
}

void wizDelayS(uint32 sec)
{
    for(; sec > 0; sec--)
      wizDelayMs(1000);
}
