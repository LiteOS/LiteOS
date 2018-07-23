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
 
 #include "M8266WIFIDrv.h"
 #include "M8266_WIFI_Config.h"
 
 void m8266wifi_hardware_config(void);
 uint8_t m8266wifi_softstart_config();
 void m8266wifi_module_delay_ms(u16 nms);
 void	m8266wifi_gpio_exti_config();
 
 static void m8266hostif_delay_init(void);
 static uint8_t m8266wifi_spi_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s);
 static void	m8266wifi_gpio_exti_irqhandler(void);
 
 extern __IO uint8_t	m8266_init_complete_flag;
 extern  uint32_t     sem_has_data_arrived;
 
 /**
	 *@brief this function configures spi pin on MCU side
	 *@param none
	 *@ret none
 */
 void m8266wifi_hardware_config(void)
 {
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_HandleTypeDef SPI_InitStructure;
	 
	 /*---initialize nRESET pin to output high---*/
	 /* clock enable */
	 M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN();
	
   /* gpio init */	 
	 GPIO_InitStructure.Pin 	  = M8266WIFI_nRESET_GPIO_PIN;												
	 GPIO_InitStructure.Mode 	  = GPIO_MODE_OUTPUT_PP;							
	 GPIO_InitStructure.Pull    = GPIO_NOPULL;
	 GPIO_InitStructure.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(M8266WIFI_nRESET_GPIO_PORT , &GPIO_InitStructure);
	
	 /* nRESET pin outputs high */
	 HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT , M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_SET);
	 
	 /*---initialize nCS pin to output high---*/
	 M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN();
	
	 GPIO_InitStructure.Pin 	  = M8266WIFI_SPI_nCS_GPIO_PIN;												
	 GPIO_InitStructure.Mode 	  = GPIO_MODE_OUTPUT_PP;							
	 GPIO_InitStructure.Pull    = GPIO_NOPULL;
	 GPIO_InitStructure.Speed   =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(M8266WIFI_SPI_nCS_GPIO_PORT , &GPIO_InitStructure);
	 
	 HAL_GPIO_WritePin(M8266WIFI_SPI_nCS_GPIO_PORT , M8266WIFI_SPI_nCS_GPIO_PIN , GPIO_PIN_SET);
	 
	 /*---initialize the rest of spi pins---*/
   M8266_WIFI_SPI_Com_SCLK_CLOCK_EN();
	 M8266_WIFI_SPI_Com_MISO_CLOCK_EN();
	 M8266_WIFI_SPI_Com_MOSI_CLOCK_EN();
	 
	 /* init SCLK pin */
	 GPIO_InitStructure.Pin 	    = M8266_WIFI_SPI_Com_SCLK_PIN ;											
	 GPIO_InitStructure.Mode 	    = GPIO_MODE_AF_PP ;	//	attention here						
	 GPIO_InitStructure.Pull      = GPIO_PULLUP ;     //attention here
	 GPIO_InitStructure.Speed     = GPIO_SPEED_HIGH; 
	 GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;
	 HAL_GPIO_Init( M8266_WIFI_SPI_Com_SCLK_PORT , &GPIO_InitStructure);
	 
	 /* init MISO pin */
	 GPIO_InitStructure.Pin 	    = M8266_WIFI_SPI_Com_MISO_PIN;
	 HAL_GPIO_Init( M8266_WIFI_SPI_Com_MISO_PORT , &GPIO_InitStructure);
	 
	 /* init MOSI pin */
	 GPIO_InitStructure.Pin 	    = M8266_WIFI_SPI_Com_MOSI_PIN;
	 HAL_GPIO_Init( M8266_WIFI_SPI_Com_MOSI_PORT , &GPIO_InitStructure);
	 
	 M8266_WIFI_SPI_Peripheral_EN();
	 M8266_WIFI_SPI_FORCE_RESET();
	 M8266_WIFI_SPI_RELEASE_RESET();
	 
	 SPI_InitStructure.Instance 										= M8266WIFI_INTERFACE_SPI;
	 SPI_InitStructure.Init.BaudRatePrescaler 			= SPI_BAUDRATEPRESCALER_8;
	 SPI_InitStructure.Init.Direction 							= SPI_DIRECTION_2LINES;
	 SPI_InitStructure.Init.Mode 										= SPI_MODE_MASTER;
	 SPI_InitStructure.Init.DataSize 								= SPI_DATASIZE_8BIT;
	 SPI_InitStructure.Init.CLKPolarity 						= SPI_POLARITY_LOW;
	 SPI_InitStructure.Init.CLKPhase 								= SPI_PHASE_1EDGE;
	 SPI_InitStructure.Init.NSS 										= SPI_NSS_SOFT;
	 SPI_InitStructure.Init.FirstBit 								= SPI_FIRSTBIT_MSB;
	 SPI_InitStructure.Init.CRCPolynomial 					= 7;
	 SPI_InitStructure.Init.CRCCalculation 					= SPI_CRCCALCULATION_DISABLE;
	 SPI_InitStructure.Init.CRCLength               = SPI_CRC_LENGTH_DATASIZE;
	 SPI_InitStructure.Init.NSSPMode                = SPI_NSS_PULSE_DISABLE;
	 SPI_InitStructure.Init.TIMode                  = SPI_TIMODE_DISABLE;
	 
	 if(HAL_SPI_Init(&SPI_InitStructure) != HAL_OK)
   {
     /* Initialization Error */
     print_log("spi initialization error");
   }
	 else
	 {
		 print_log("spi initialization OK");
	 }
	 
	 __HAL_SPI_ENABLE(&SPI_InitStructure);
 }
 
 
 
 
 /**
	 *@brief this function is used to start up esp8266 wifi module through SPI interface
   *@params none
   *@ret 1---> OK 0---> failed
 */
 uint8_t m8266wifi_softstart_config()
 {
		uint32_t  	spi_clk = 40000000;
		uint8_t   	sta_ap_mode = 0;
		uint8_t   	connection_status = 0xFF;
		char 	      sta_ip[15+1]={0};
		char 	      ssid[32];
		sint8_t   	rssi;
		uint16_t  	status = 0;
		
		
		m8266hostif_delay_init();
		
		/*---HAL_Delay is just temporaily used!!!!!!!!---*/
		HAL_GPIO_WritePin(M8266WIFI_SPI_nCS_GPIO_PORT , M8266WIFI_SPI_nCS_GPIO_PIN , GPIO_PIN_RESET );
		m8266wifi_module_delay_ms(10);
		
		HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT ,  M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_RESET);
		m8266wifi_module_delay_ms(50);
		
		HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT ,  M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_SET);
		m8266wifi_module_delay_ms(300);
		
		HAL_GPIO_WritePin(M8266WIFI_SPI_nCS_GPIO_PORT , M8266WIFI_SPI_nCS_GPIO_PIN , GPIO_PIN_SET);
		m8266wifi_module_delay_ms(500);
		
		/*---In softstart period , allow users to adjust baudrate---*/
		M8266WIFI_INTERFACE_SPI->CR1 &= (~SPI_CR1_SPE);
		M8266WIFI_INTERFACE_SPI->CR1 &= (~SPI_CR1_BR);
		M8266WIFI_INTERFACE_SPI->CR1 |= M8266WIFI_SPI_BAUD_RATE_PRESCALER;
		M8266WIFI_INTERFACE_SPI->CR1 |= SPI_CR1_SPE;
		
		/* For example: STM32F767--->SPI1 and SPI4 is attached to PCLK2 */
		if( (M8266WIFI_INTERFACE_SPI == SPI1) || (M8266WIFI_INTERFACE_SPI == SPI4) )
		{
			/* choose approporiate prescaler */
			double expotential = ((M8266WIFI_SPI_BAUD_RATE_PRESCALER >> 3) + 1);
			double dividend = pow( BASE_CONSTANT , expotential );
			spi_clk = HAL_RCC_GetPCLK2Freq() / ((uint32_t)dividend) ;
			print_log("mark1:spi_clk after being adjusted is %d Hz" , spi_clk);
		}
		else
		{
			/* choose approporiate prescaler */
			double expotential = ((M8266WIFI_SPI_BAUD_RATE_PRESCALER >> 3) + 1);
			double dividend = pow( BASE_CONSTANT , expotential );
			spi_clk = HAL_RCC_GetPCLK1Freq() / ((uint32_t)dividend) ;
			print_log("mark2:spi_clk after being adjusted is %d Hz" , spi_clk);
		}
		
		/*------------below are procedures about actually start esp8266 wifi module--------------*/
		/* HAL_Delay is temporaily used */
		m8266wifi_module_delay_ms(1);
		
		/* It is very important to call M8266HostIf_SPI_Select() to tell the driver which SPI you used
 	     and how faster the SPI clock you used. The function must be called before SPI access */
		if(M8266HostIf_SPI_Select((uint32_t)M8266WIFI_INTERFACE_SPI, spi_clk, &status)==0)
		{
			//while(1)
			{
				print_log("SPI select function error!! and status is 0x%x" , status);
				return 0;
			}
		}
		else
		{
			print_log("configure SPI frequency OK and status is 0x%x",  status);
		}
		
		#ifdef EXECUTE_M8266_TEST_FUNC
		{
			 volatile uint32_t  i, j;
			 uint8_t  byte;

			 if(M8266WIFI_SPI_Interface_Communication_OK(&byte)==0) 	//	if SPI logical Communication failed
			 {
				 //while(1)  
				 {
						print_log("Send one byte to wifi module and receive response failed");
						return 0;
				 }
			 }
			 else
			 {
					print_log("Send one byte to wifi module and receive response OK");
			 }
			 
			 i = 100000;
			 j = M8266WIFI_SPI_Interface_Communication_Stress_Test(i);
			 if( (j<i)&&(i-j>10)) 		//  if SPI Communication stress test failed
			 {
				 //while(1)               // flash LED1 and LED2 in frequency of 1s constantly when error
				 {
						print_log("stress test under this spi clock frequency failed(unstable)");
						return 0;
				 }
			 }
			 else
			 {
					print_log("stress test under this frequency passed , you can fix to this frequency or raise it and test");
			 }
		}
		#endif
		
		/* set to STA+AP mode, not saved 
		   1--->STA Only, 2--->AP Only, 3--->STA+AP
		*/
		if(M8266WIFI_SPI_Set_Opmode(1, 0, &status)==0)  //attention here!!!
		{		
			print_log("SPI Set operating mode error 0x%x !!" , status);
			return 0;
		}
		else
		{
			print_log("SPI set operating mode to STA mode OK");
		}
		
		/* verify set mode operation  */
		if(M8266WIFI_SPI_Get_Opmode(&sta_ap_mode, &status)==0)
		{			
			print_log("SPI get operating mode error 0x%x !!" , status);
			return 0;
		}
		else
		{
			print_log("current mode is %d" , sta_ap_mode);
		}
		
		/* mode meets expectations */
		if(sta_ap_mode == 1 || sta_ap_mode == 3)
		{
			/* connect to AP */
			if(M8266WIFI_SPI_STA_Connect_Ap((uint8_t*)ESP8266_ApSSID, (uint8_t *)ESP8266_ApPWD, 0, 20, &status) == 0) // not saved, timeout=20s
			{
				print_log("SPI connect to AP failed and status is 0x%x" , status);
				return 0;
			}
			else
			{
				print_log("SPI connect to AP OK");
			}
			
			/* Wait the module retrieve ip address if it works in STA mode */ 
		  if(m8266wifi_spi_wait_sta_connecting_to_ap_and_get_ip(sta_ip, 10)==0) // max wait 10s to get sta ip
		  {
				print_log("did not get ip address in timeout you set");
			  return 0; 
	    }
		  else
			{
				print_log("In STA mode:ip address assigned is:%s" , sta_ip);
				
				if(M8266WIFI_SPI_Get_STA_Connection_Status(&connection_status, &status)==0)  // connection_status will give the status of last connecting
				{
					print_log("get connection status failed:connction status is %d and status is 0x%x" , connection_status , status);
					return 0;
				}
				else
				{
					print_log("get connection status successful:connction status is %d and status is 0x%x" , connection_status , status);
				}
		  }
			
			#ifdef EXECUTE_M8266_TEST_FUNC
			/*  */
			if(M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(ssid, &rssi, &status)==0)
			{
				print_log("get current SSID of AP error and status is 0x%x" , status);	
				return 0;
			}
			else
			{
				print_log("get current SSID of AP successful: ssid:%s , status:0x%x" , ssid , status);
			}
			#endif
		}
		
		return 1;
				
 }
 
 
 /**
	*@brief functions in the range below are needed because M8266WIFIDrv needs them hence must be implemented
  *--------------------------------------------------------------------------------------------------------
*/
void M8266HostIf_Set_SPI_nCS_Pin(u8 level)
{
	  if(level!=0)
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) 
		  M8266WIFI_SPI_nCS_GPIO->BSRRL = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) 
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32FXX in M8266_WIFI_Config.h
#endif

		else
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BRR   = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRRH = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) 
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN<<16;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32FXX in M8266_WIFI_Config.h
#endif
}

#ifndef WIFI8266_USE_OS
void M8266HostIf_delay_us(u8 nus)
{
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				
	SysTick->VAL=0x00;        				
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00; 
}

void m8266wifi_module_delay_ms(u16 nms)
{
	 u16 i, j;
	 for(i=0; i<nms; i++)
	   for(j=0; j<4; j++)									// delay 1ms. Call 4 times of delay_us(250), since M8266HostIf_delay_us(u8 nus), nus max 256
	      M8266HostIf_delay_us(250);
}


static void m8266hostif_delay_init(void)
{
	/* clear SysTick->CTRL CLKSOURCE bit to use AHB/8 clock to count */
	SysTick->CTRL &= (~SysTick_CTRL_CLKSOURCE_Pos);
	fac_us = HAL_RCC_GetHCLKFreq() / UNIT_MHZ / 8;
	print_log("fac_us is %d" , fac_us);
}

#else


/**
  *@brief this function is a must for this wifi module and  dwt_delay_init() has to be
  *executed in main function or other places where you think is appropraite in order to
  *init data watchpoint trace(dwt)
 */
void M8266HostIf_delay_us(u8 nus)
{
	atiny_usleep(nus);
}

void m8266wifi_module_delay_ms(u16 nms)
{
	osDelay(osMs2Tick(nms));
}

static void m8266hostif_delay_init(void)
{
	;
}

#endif


/**
	*@breif Wait the module got ip address if it works in STA mode
	*@params sta_ip,  max_wait_time_in_s
  *@ret 1--->OK 0---> failed
 */ 
static uint8_t m8266wifi_spi_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s)
{
	 u16  i;
	 u16  status=0;
	 
	 for(i=0; i < 1*max_wait_time_in_s; i++)  // max wait
	 {
		 if( (M8266WIFI_SPI_Get_STA_IP_Addr(sta_ip , &status) ==1) && (strcmp(sta_ip, "0.0.0.0")!=0) )
		 {
				break;
		 }
     
		 /* HAL_Delay is temporaily used !!*/		 
		 m8266wifi_module_delay_ms(1000);
		 continue;
   }
	 
   if( i>=1*max_wait_time_in_s )
	 {
		 return 0; // false
	 }
	 
	 return 1;  // true
}


 /**
	 *@brief M8266 wifi module provides a external GPIO interrupt to facilitate data transmission
   *@params none
   *@ret none 
 */
 void	m8266wifi_gpio_exti_config()
 {
		GPIO_InitTypeDef   GPIO_InitStructure;
		
	  /* enable GPIOx clk */
		M8266_GPIO_EXTI_CLK_ENABLE();
	 
	  /* initialize EXTI GPIO hardware */
		GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Pin  = M8266_GPIO_EXTI_PIN;
		HAL_GPIO_Init(M8266_GPIO_EXTI_PORT, &GPIO_InitStructure);
	 
		/* use Huawei LiteOS func to insert your irq handler and config other params */
		LOS_HwiCreate(M8266_GPIO_EXTI_IRQ_NUM , M8266_GPIO_EXTI_IRQ_PRIORITY , 0 , m8266wifi_gpio_exti_irqhandler , NULL);
	 
		print_log("GPIO EXTI mode init OK");
 }
 
 
 
 /**
	 *@brief GPIO EXTI irq handler
   *@params none
   *@ret none
 */
 static void	m8266wifi_gpio_exti_irqhandler(void)
 {
		print_log("enter GPIO EXTI interrupt");
	 
	  /* make sure gpio interrupt actually happens */
		if(__HAL_GPIO_EXTI_GET_IT(M8266_GPIO_EXTI_PIN) != RESET)
		{
			/* clear pending flag */
			__HAL_GPIO_EXTI_CLEAR_IT(M8266_GPIO_EXTI_PIN);
			HAL_GPIO_EXTI_Callback(M8266_GPIO_EXTI_PIN);
			
			/* process EXTI interrupt */
			{
				/* release semaphore when data has arrived : this interrupt is an reminder but
					 do not care which link these data belongs to.It is another task job to process them*/
				if( m8266_init_complete_flag == 1 )
				{	
					/* release semphore to activate the task */
					if( LOS_SemPost(sem_has_data_arrived) != LOS_OK)
						print_log("release semaphore failed");
					else
						//print_log("semaphore count is %d" , osSemaphoreGetCount(&sem_has_data_arrived));
						;
				}
			}
		}
		
 }
