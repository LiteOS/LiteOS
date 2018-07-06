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
 #include "M8266_WIFI_SPI_IF.h"
 
 
 /* global variables */
 uint32_t      						 fac_us = 0;
 link_params       				 link_occupied_flag_arrary[M8266_WIFI_MAX_LINK_NUM] = {0}; 
 __IO uint8_t  						 test_connect_func_flag = 0;
 __IO uint8_t  						 print_link_status_flag= 0;
 __IO uint8_t              m8266_init_complete_flag = 0;
 __IO uint8_t              task_process_data_received_gateway_flag = 1;

 uint8_t origin_data[MAX_DATA_LEN];

 uint32_t      						 sem_has_data_arrived;
 uint32_t      						 receive_data_tsk_handle;

 wifi_spi_interface_params wifi_spi_if_instance = 
 {
		.module_init       = m8266_wifi_init ,
    .connect     	     = m8266wifi_connect ,
	  .send        	     = m8266wifi_send_data ,
    .recv        	     = m8266wifi_spi_receive_data ,	 
    .recv_timeout	     = m8266wifi_spi_receive_data_timeout ,
	  .close             = m8266wifi_spi_close ,
	  .name              = ESP8266_MODULE_NAME ,
	  .p_link            = link_occupied_flag_arrary ,
	  .max_link_no       = M8266_WIFI_MAX_LINK_NUM , 
	  .check_link_params = m8266wifi_check_all_link_status
 };
 
 /* function prototypes */
 static void 			m8266wifi_hardware_config(void);
 static uint8_t 	m8266wifi_softstart_config(void);
 static uint8_t 	m8266wifi_spi_wait_sta_connecting_to_ap_and_get_ip(char* sta_ip, u8 max_wait_time_in_s);
 static void 			m8266hostif_delay_init(void);
 static void 			m8266wifi_module_delay_ms(u16 nms);
 static uint16_t  process_remote_port_from_string_to_integer(const char * port);
 
 static void      m8266wifi_gpio_exti_config(void);
 static void      m8266wifi_gpio_exti_irqhandler(void);
 static uint8_t   m8266_get_free_link_no( link_params * );

 uint32_t   	    create_task_process_data_received(void);
 void             task_process_data_received(void);
 
 
 /**
	 *@brief M8266 wifi module provides a external GPIO interrupt to facilitate data transmission
   *@params none
   *@ret none 
 */
 static void	m8266wifi_gpio_exti_config()
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
 
 /**
	 *@brief this function configures spi pin on MCU side
	 *@param none
	 *@ret none
 */
 static void m8266wifi_hardware_config()
 {
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_HandleTypeDef SPI_InitStructure;
	 
	 /*---initialize nRESET pin to output high---*/
	 /* clock enable */
	 M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN();
	
   /* gpio init */	 
	 GPIO_InitStructure.Pin 	  = M8266WIFI_nRESET_GPIO_PIN;												
	 GPIO_InitStructure.Mode 	  = GPIO_MODE_OUTPUT_PP;							
	 GPIO_InitStructure.Pull    = GPIO_PULLUP;
	 GPIO_InitStructure.Speed   = GPIO_SPEED_FREQ_HIGH;
	 HAL_GPIO_Init(M8266WIFI_nRESET_GPIO_PORT , &GPIO_InitStructure);
	
	 /* nRESET pin outputs high */
	 HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT , M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_SET);
	 
	 /*---initialize nCS pin to output high---*/
	 M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN();
	
	 GPIO_InitStructure.Pin 	  = M8266WIFI_SPI_nCS_GPIO_PIN;												
	 GPIO_InitStructure.Mode 	  = GPIO_MODE_OUTPUT_PP;							
	 GPIO_InitStructure.Pull    = GPIO_PULLUP;
	 GPIO_InitStructure.Speed   = GPIO_SPEED_FREQ_HIGH;
	 HAL_GPIO_Init(M8266WIFI_SPI_nCS_GPIO_PORT , &GPIO_InitStructure);
	 
	 HAL_GPIO_WritePin(M8266WIFI_SPI_nCS_GPIO_PORT , M8266WIFI_SPI_nCS_GPIO_PIN , GPIO_PIN_SET);
	 
	 /*---initialize the rest of spi pins---*/
   M8266_WIFI_SPI_Com_SCLK_CLOCK_EN();
	 M8266_WIFI_SPI_Com_MISO_CLOCK_EN();
	 M8266_WIFI_SPI_Com_MOSI_CLOCK_EN();
	 
	 /* init SCLK pin */
	 GPIO_InitStructure.Pin 	  = M8266_WIFI_SPI_Com_SCLK_PIN;												
	 GPIO_InitStructure.Mode 	  = GPIO_MODE_AF_PP ;	//	attention here						
	 GPIO_InitStructure.Pull    = GPIO_NOPULL ;     //attention here
	 GPIO_InitStructure.Speed   = GPIO_SPEED_FREQ_HIGH;
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
	 SPI_InitStructure.Init.BaudRatePrescaler 			= SPI_BAUDRATEPRESCALER_16;
	 SPI_InitStructure.Init.Direction 							= SPI_DIRECTION_2LINES;
	 SPI_InitStructure.Init.Mode 										= SPI_MODE_MASTER;
	 SPI_InitStructure.Init.DataSize 								= SPI_DATASIZE_8BIT;
	 SPI_InitStructure.Init.CLKPolarity 						= SPI_POLARITY_LOW;
	 SPI_InitStructure.Init.CLKPhase 								= SPI_PHASE_1EDGE;
	 SPI_InitStructure.Init.NSS 										= SPI_NSS_SOFT;
	 SPI_InitStructure.Init.FirstBit 								= SPI_FIRSTBIT_MSB;
	 SPI_InitStructure.Init.CRCPolynomial 					= 7;
	 SPI_InitStructure.Init.CRCCalculation 					= SPI_CRCCALCULATION_DISABLE;
	 
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
 static uint8_t m8266wifi_softstart_config()
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
		m8266wifi_module_delay_ms(1);
		
		HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT ,  M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_RESET);
		m8266wifi_module_delay_ms(5);
		
		HAL_GPIO_WritePin(M8266WIFI_nRESET_GPIO_PORT ,  M8266WIFI_nRESET_GPIO_PIN , GPIO_PIN_SET);
		m8266wifi_module_delay_ms(300);
		
		HAL_GPIO_WritePin(M8266WIFI_SPI_nCS_GPIO_PORT , M8266WIFI_SPI_nCS_GPIO_PIN , GPIO_PIN_SET);
		m8266wifi_module_delay_ms(500);
		
		/*---In softstart period , allow users to adjust baudrate---*/
		M8266WIFI_INTERFACE_SPI->CR1 &= (~SPI_CR1_BR);
		M8266WIFI_INTERFACE_SPI->CR1 |= M8266WIFI_SPI_BAUD_RATE_PRESCALER;
		M8266WIFI_INTERFACE_SPI->CR1 |=  SPI_CR1_SPE;
		
		if(M8266WIFI_INTERFACE_SPI == SPI1)
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
			while(1)
			{
				print_log("SPI select function error!!");
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
				 while(1)  
				 {
						print_log("Send one byte to wifi module and receive response failed");
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
				 while(1)               // flash LED1 and LED2 in frequency of 1s constantly when error
				 {
						print_log("stress test under this spi clock frequency failed(unstable)");
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

static void m8266wifi_module_delay_ms(u16 nms)
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

void M8266HostIf_delay_us(u8 nus)
{
	atiny_usleep(nus);
}

static void m8266wifi_module_delay_ms(u16 nms)
{
	osDelay(osMs2Tick(nms));
}

static void m8266hostif_delay_init(void)
{
	;
}


#endif
/*
---------------------------------------------------------------------------------------------------------
*/


 /**
	 *@breif wifi module init function , users use this function to complete total init operation
   *@params none
   *@ret 1--->OK 0--->fail
 */
 int32_t m8266_wifi_init(void)
 {
	 uint8_t ret = 0;
	 m8266_init_complete_flag = 0;
	 /* GPIO EXTI mode config */
	 m8266wifi_gpio_exti_config();
	 
	 /* initialize  wifi_spi_if_instance */
	 //M8266_WIFI_If_Init();
	 
	 /* hardware config:eg CLK , nCS , MISO... */
	 m8266wifi_hardware_config();
	 
	 /* start up M8266 wifi module */
	 ret = m8266wifi_softstart_config();
	  
	 if( ret == 1 )
	 {
			print_log("M8266 WIFI module start up OK");
	 }
	 
	 /* create a semaphore to synchronize task handling data received with interrupt indicating that data is coming */
	 ret = LOS_SemCreate(0 , &sem_has_data_arrived);
	 if(ret != LOS_OK)
	 {
		 print_log("create sem_has_data_arrived failed");
		 return 0;
	 }
	 else
	 {
			print_log("create sem_has_data_arrived OK and SemophoreID is %d" , sem_has_data_arrived);
	 }
	 
	 ret = create_task_process_data_received();
	 
	 if(ret == LOS_OK)
	 { 
			print_log("create_task_process_data_received OK");
	 }
	 else
	 {
			print_log("create_task_process_data_received failed");
		 return 0;
	 }
	  
	 m8266_init_complete_flag = 1;
	 print_log("M8266 wifi module init done");
	 ret = 1;
	 return ret;
	  
 }
 
 
 /**
	 *@brief translate remote port from string to integer to match requirements of the lib
   *@params port: a pointer points to a string like "65535"
   *@ret    return the translated port integer 
 */
 static uint16_t process_remote_port_from_string_to_integer(const char * port)
 {
		uint16_t translated_port_integer = 0;
	  const char * temp = port;
	 	 
		do
		{
			if((*temp) >= '0' && (*temp) <= '9' )
			{	
				translated_port_integer = translated_port_integer * 10 + (*temp) - '0';
				temp++;
			}
			else
			{
				print_log("remote port defined illegal");
				return 0;
			}
		}while(temp!= NULL && (*temp) != '\0');
		
		print_log("remote port translated OK and the result is %d" , translated_port_integer);
		
		return translated_port_integer;
 }
 
 
 /**
	 *@brief this function is used to check if there is a free link available
	 *@param pointer to an array
   *@ret	0~(M8266_WIFI_MAX_LINK_NUM-1)---> OK others--->fail
 */
 static uint8_t  m8266_get_free_link_no( link_params * p_link_param)
 {
	 int32_t ret , i;
		/* iterate through array to choose the free link first met */
	 for( i = 0 ; i < M8266_WIFI_MAX_LINK_NUM ; i++)
	 {
			if(p_link_param[i].busy_flag == 0)
			{
				ret = i;
				print_log("Checking link num:free link num found , link no is %d" , i);
				return ret;
			}
	 }
	 
	 /* if no free link is found , do something */
	 print_log("no free link num found!!!");
	 ret = M8266_WIFI_MAX_LINK_NUM;
	 return ret;
 }
 
 
 /**
	 *@breif establish a connection to server
	 *@params tcp_udp: the type of connection to set up                             
                      = 0, to set up connection of UDP                              
                      = 1, to set up connection as TCP Client                       
                      = 2, to set up connection as TCP Server
						local_port:  to specify the local port if any ,if local_port=0, then a random local port will be generated
						remote_addr: the string of ip or dns address of remote:e.g. "192.168.1.2" or "www.baidu.com"
						remote_port: the port value of remote server
						link_no:     the link_number to be used. max 8 links as per mutitiple link
   *@ret 0~3--->OK (-1)--->failed
 */
 int32_t m8266wifi_connect( const char * host, const char * port, int32_t proto)
{
	/* multi connection is temporaily not supported */
	uint8_t tcp_udp; 
	uint16_t local_port = 0;
	char* remote_addr = NULL;
  uint16_t remote_port;
	int32_t link_no = 0;
	uint16_t status=0;
	
	/* ret of this function */
	int8_t  ret = 0;
	
	tcp_udp = (proto == ATINY_PROTO_TCP)? 1 : 0;
	remote_addr = (char*)host;
	
	remote_port = process_remote_port_from_string_to_integer(port);
	
	if( remote_port == 0 )
	{
		print_log("remote port is 0 , please check");
		return -1;
	}
	
	/* get the free link no , if max link num is reached , connection should not be allowed  */
	link_no = m8266_get_free_link_no(link_occupied_flag_arrary);
	
	/* distinguish  test_connect_func_flag */
	if( (link_no >=0) && (link_no <= M8266_WIFI_MAX_LINK_NUM -1) )
	{
		if(test_connect_func_flag == 1)
		{
			print_log("execute M8266WIFI_Verify_Connect_Func");
			ret = M8266WIFI_SPI_Setup_Connection(tcp_udp, local_port, remote_addr, remote_port, link_no , 20, &status);
			if(ret == 0)
			{
				/* when ret == 0 , there is no need to go on */
				print_log("In test:connect to server failed");
				return -1;
			}
		}
		else //normal exection branch
		{
			ret = M8266WIFI_SPI_Setup_Connection(tcp_udp, local_port, remote_addr, remote_port, link_no , 20, &status);
			
			if(ret == 0 )
			{
				print_log("SPI setup connection %d failed and status is 0x%x" ,link_no , status);
				return -1;
			}
			else
			{
				if( test_connect_func_flag == 0)
				{
					/* maximum 4 links supported and set the corresponding flag */
					link_occupied_flag_arrary[link_no].link_fd = link_no;
					print_log("SPI setup connection OK and status is 0x%x , link no is %d" , status , link_no);
					
					uint32_t uwRet = LOS_QueueCreate((char*)"dataQueue" , \
						                                               16 , \
					                                                 &link_occupied_flag_arrary[link_no].queue_id ,\
                                            					     0 , sizeof(queue_element));
					if(uwRet != LOS_OK)
					{
						link_occupied_flag_arrary[link_no].busy_flag = 0;
						print_log("create a Queue attched to link %d failed" , link_no);
						return -1;
					}
					
					print_log("create a Queue %d attched to link %d OK" , link_occupied_flag_arrary[link_no].queue_id , link_no);
					
					/* set busy flag when connection on this link_no is successful totally */
					link_occupied_flag_arrary[link_no].busy_flag = 1;		
				}
			}
		}
	}
	else
	{
		print_log("link no invalid: link num is %d" , link_no);
		return -1;
	}
	
	return link_no;
}
 
 
/**
	*@breif this function verifies M8266WIFI_Connect , use MACROS defined in  M8266_WIFI_Config.h
  *       In this function link no is fixed to 0
  *@params none
	*@ret 1--->OK 0--->failed
*/
uint8_t m8266wifi_verify_connect_func(connection_params_struct * plink)
{
	int8_t   ret = 0;
	int32_t  link_no;
	test_connect_func_flag = 1;
	
  link_no = m8266wifi_connect( TEST_REMOTE_ADDRESS , TEST_REMOTE_PORT ,  TEST_TCP_UDP_TYPE );
	
	if(link_no == -1)
	{
		print_log(" connect to test server failed ");
		ret = 0;
	}
	else
	{
		link_occupied_flag_arrary[link_no].busy_flag = 1;
		
		print_log(" connect to test server OK:using link no %d" , link_no);
		
		ret = M8266WIFI_SPI_Query_Connection(link_no , \
																				 &plink->test_connection_type , \
																				 &plink->test_connection_state , \
		                                     &plink->test_local_port , \
		                                     plink->test_remote_ip , \
		                                     &plink->test_remote_port , \
		                                     &plink->test_status);
		
		/* query op OK , if not , just exit to save time */
		if(ret == 1)
		{
			print_log("test connection type is: %s",(plink->test_connection_type == 0)? "UDP" : "TCP" );
		
			switch( plink->test_connection_state )
			{
				case 0:
					print_log("test connection state:invalid");
					break;
				case 1:
					print_log("test connection state:waiting...");
					break;
				case 2:
					print_log("test connection state:tcp server spying");
					break;
				case 3:
					print_log("test connection state:linking...");
					break;
				case 4:
					print_log("test connection state:writing TX buffer");
					break;
				case 5:
					print_log("test connection state:writing RX buffer");
					break;
				case 6:
					print_log("test connection state:closed");
					break;
				default:
					print_log("invalid status");
			}
			
			print_log("test local port is:%d",plink->test_local_port);
			print_log("test remote ip address is:%s",plink->test_remote_ip);
			print_log("test remote port is:%d" , plink->test_remote_port);
			print_log("test status is 0x%x" , plink->test_status);	
			print_log("query params of test connecton is OK");
   }
	 else
	 {
			ret = 0;
			print_log("query params of test connecton failed");
	 }
	
		#if 0
		m8266wifi_module_delay_ms(5000);
		
		ret = M8266WIFI_SPI_Delete_Connection(TEST_REMOTE_LINK_NO , &test_status);
		
		if( ret == 0 )
		{
			print_log("delete test spi connection failed and status is 0x%x" , test_status);
			return 0;
		}
		else
		#endif
	
		{
			test_connect_func_flag = 0;
			link_occupied_flag_arrary[link_no].busy_flag = 0;
			print_log("delete test spi connection OK");	
		}
	}
	
	return ret;
}



/**
	*@brief check all link status:link number ranges from 0 ~ 3
  *@params none
  *@ret 1---> OK 0---> failed
*/
uint8_t m8266wifi_check_all_link_status(void)
{
	uint8_t index = 0 , ret;
	
	char tmp[128];
	connection_params_struct connection_params;
	
	print_link_status_flag = 1;
	
	printf("link_no		TCP/UDP		local_port		remote_ip		remote_port		status\r\n");
	
	for(index = 0 ; index < M8266_WIFI_MAX_LINK_NUM ; index++)
	{
		if(link_occupied_flag_arrary[index].busy_flag == 1)
		{
			ret = m8266wifi_query_certain_link_params(index , &connection_params);
			
			if(ret == 0)
			{
				print_log("Get connection params of link %d failed" , index);
				return ret;
			}
			else
			{
				sprintf(tmp , "%d		%s		%d			%s	%d			0x%x" , index , (connection_params.test_connection_type == 0)?"UDP":"TCP" \
				                                                    , connection_params.test_local_port , connection_params.test_remote_ip \
				                                                    , connection_params.test_remote_port , connection_params.test_status); 
				printf("%s\r\n" , tmp);
			}
		}
		else
		{
			sprintf(tmp , "%d		%s		%d			%s		%d			0x%x" , index , "none" \
				                                                    , 0 , "0.0.0.0" \
				                                                    , 0 , 0); 
				printf("%s\r\n" , tmp);
		}
	}
	print_link_status_flag = 0;
	
	return 1;
}


/**
	*@breif this function is used to send data the connected sever
  *@params id: transferred from agentiny  
           buf:start address of data to be sent 
           len: length of data to be sent
*@ret      -1:failed else return the number of data sent 
*/
int32_t m8266wifi_send_data(int32_t id , const uint8_t  *buf, size_t len)
{
	uint8_t link_no;
	uint16_t status =0;
	uint8_t ret = 0;
	
	if(len == 0)
	{
		print_log("the length of data to be sent is not allowed to be 0");
		return -1;
	}
	
	if(len > 65535)
	{
		print_log(" data len > 65535 , you may split your packet");
		return -1;
	}
	
	link_no = id;
	
	if( (link_no >= M8266_WIFI_MAX_LINK_NUM) || (link_no < 0) )
	{
		print_log("link num is greater than MAX");
		return -1;
	}
	
	ret = M8266WIFI_SPI_Send_Data((uint8_t *)buf, len , link_no, &status);
	
	/* if ret!= 0---> ret is the number of */
	if(ret != 0)
	{
		print_log("the length of data to be sent is %d and status is 0x%x" , ret , status);
	}
	
	return ret;
}


/**
	*@breif  This function is just used to query connection parameters of certain link
           distinguished with link_no(0~3)
	*@params link_no-> 0 ~ 3
  *@ret    1--->OK 0--->failed
*/
uint8_t m8266wifi_query_certain_link_params(uint8_t link_no , connection_params_struct * plink)
{
	uint8_t   ret = 0;
	
	/* entrance examination */
	if( link_no >= M8266_WIFI_MAX_LINK_NUM )
	{
		print_log(" link_no is illegal: your connection may have reached its climax ");
		return 0;
	}
	
	ret = M8266WIFI_SPI_Query_Connection(link_no , \
		                                   &plink->test_connection_type , \
		                                   &plink->test_connection_state , \
		                                   &plink->test_local_port , \
		                                   plink->test_remote_ip , \
		                                   &plink->test_remote_port , \
		                                   &plink->test_status);
	if(ret == 1)
	{	
		if(print_link_status_flag == 0)
		{
			print_log("test connection type is: %s",(plink->test_connection_type == 0)? "UDP" : "TCP" );
			
			switch( plink->test_connection_state )
			{
				case 0:
					print_log("test connection state:invalid");
					break;
				case 1:
					print_log("test connection state:waiting...");
					break;
				case 2:
					print_log("test connection state:tcp server spying");
					break;
				case 3:
					print_log("test connection state:linking...");
					break;
				case 4:
					print_log("test connection state:writing TX buffer");
					break;
				case 5:
					print_log("test connection state:writing RX buffer");
					break;
				case 6:
					print_log("test connection state:closed");
					break;
				default:
					print_log("invalid status");
			}
			
			print_log("test local port is:%d", plink->test_local_port);
			print_log("test remote ip address is:%s", plink->test_remote_ip);
			print_log("test remote port is:%d" , plink->test_remote_port);
			print_log("test status is 0x%x" , plink->test_status);
		}
 }
 else
 {
		ret = 0;
		print_log("query connection params failed and status is 0x%x" , plink->test_status);
 }
 
 return ret;
}


/**
	*@brief this task handles data received and determine whether the data needs to be uploaded
  *@params none
  *@ret none
*/
void	task_process_data_received()
{
	int ret = 0;
	uint16_t max_len, max_wait_in_ms;
	uint8_t   link_no;
	uint16_t  status;
	uint8_t * address_malloc;
	uint16_t  actual_len;
	queue_element queue_item; 
	
	for(;;)
	{ 
		/*  the task will be blocked for an "unlimited" time if no data arrives at wifi module  */
		LOS_SemPend(sem_has_data_arrived , LOS_WAIT_FOREVER);
		
		print_log("pass because semaphore is released");
		
		/* make sure data is no wifi module no matter which link it belongs to */
		
		//ret = M8266WIFI_SPI_Has_DataReceived();
		
		//if(ret == 1)
		do
		{
			/* clear data array */
			memset(origin_data , 0 , MAX_DATA_LEN);
			actual_len = M8266WIFI_SPI_RecvData(origin_data , MAX_DATA_LEN , 2000 , &link_no , &status);
			
			if(actual_len == 0)
			{
				print_log("0 byte data is read back!!!!");
			}
			else
			{
				print_log("%d bytes is read back from link %d and status is 0x%x" , actual_len , link_no , status);
				
				/* atiny_malloc:  allocate space on heap not stack of the task */
				address_malloc = (uint8_t*)atiny_malloc(actual_len * sizeof(uint8_t));
				
				if(address_malloc == NULL)
				{
					print_log("address allocates fail!!!!!!");
				}
				else
				{
					/* copy data from origin_data to address_malloc */
					memcpy(address_malloc , origin_data , actual_len);
					
					queue_item.addr = address_malloc;
					queue_item.len  = actual_len;
					
					if(LOS_QueueWriteCopy(link_occupied_flag_arrary[link_no].queue_id , &queue_item , sizeof(queue_element) , 0) != LOS_OK)
					{
						/* free memory address allocated if LOS_QueueWriteCopy failed */
						print_log("LOS_QueueWriteCopy executed fail!!!!!");
						atiny_free(address_malloc);
					}
				}
			}
			
			
		}while( M8266WIFI_SPI_Has_DataReceived() == 1 );
		
//		else
//		{
//			print_log(" no data , mis-judge happened!!!!!!!!!!!");
//		}
	}
}


/**
	*@brief this function uses LOS_TaskCreate to create  a task
  *@params none
  *@ret LOS_OK or not
*/
uint32_t  create_task_process_data_received(void)
{
	uint32_t uwRet = LOS_OK;
  TSK_INIT_PARAM_S task_init_param;

  task_init_param.usTaskPrio = 1;
  task_init_param.pcName = "task_process_data_received";
  task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_process_data_received;
  task_init_param.uwStackSize = 1024;   //minumun stack size is 304

  uwRet = LOS_TaskCreate(&receive_data_tsk_handle, &task_init_param);
  if(LOS_OK != uwRet)
  {
		return uwRet;
  }
	
  return uwRet;
}

/**
	*@brief receive data stored in wifi module without a specified timeout value--->LOS_WAIT_FOREVER
	*@params data--->pointer to space that stores data  max_len---> maximum received data length
  *@ret   0---> failed  !0---> OK
*/
 int32_t m8266wifi_spi_receive_data(int32_t id , uint8_t * buf , size_t len)
{
	uint32_t queue_item_len = sizeof(queue_element);
	queue_element queue_item;
	
	/* init queue item */
	queue_item.addr = NULL;
	queue_item.len  = 0;
	
	int ret = LOS_QueueReadCopy(link_occupied_flag_arrary[id].queue_id , &queue_item , &queue_item_len , LOS_WAIT_FOREVER);
	
	print_log("pass LOS_QueueReadCopy");
	
	if(ret == LOS_OK)
	{
		if( queue_item.len )
		{
			memcpy(buf , queue_item.addr , queue_item.len);
			
			/* free the space allocated immediately when data is copied completely */
			atiny_free(queue_item.addr);
			
			/* print out the data to visualize result */
			for(int i = 0 ; i < queue_item.len ; i++)
			{
				printf("%c" , buf[i]);
				printf("%c" , buf[i]);
				if( (i + 1)%16 == 0 )
					printf("\n");
			}
			printf("\n");
			
			print_log("In M8266WIFI_SPI_Receive_Data:upload data OK:link:%d , queue_id:%d ,  data_len:%d" ,id , link_occupied_flag_arrary[id].queue_id , queue_item.len);
		}
	}
	else
	{
		print_log("In M8266WIFI_SPI_Receive_Data: LOS_QueueReadCopy fail--->link:%d , queue id:%d" , id , link_occupied_flag_arrary[id].queue_id);
		return -1;
	}
	
	return queue_item.len;
}


/**
	*@breif receive data stored in the Wifi module but with the timeout limitation
  *@params data--->space that store the data , max_len--->maximum length of data that can be received at one time
           timeout---> if timeout is reached and data is not received, just return and report something
  *@ret 0---> failed !0--->OK
*/
int32_t  m8266wifi_spi_receive_data_timeout(int32_t id , uint8_t * buf , size_t len , uint32_t timeout)
{
	uint32_t queue_item_len = sizeof(queue_element);
	queue_element queue_item;
	
	/* init queue item */
	queue_item.addr = NULL;
	queue_item.len  = 0;
	
	/*  */
	int ret = LOS_QueueReadCopy(link_occupied_flag_arrary[id].queue_id , &queue_item , &queue_item_len , osMs2Tick(timeout) );
	
	print_log("pass LOS_QueueReadCopy");
	
	if(ret == LOS_OK)
	{
		if( queue_item.len )
		{
			memcpy(buf , queue_item.addr , queue_item.len);
			
			/* free the space allocated immediately when data is copied completely */
			atiny_free(queue_item.addr);
			
			/* print out the data to visualize result */
			for(int i = 0 ; i < queue_item.len ; i++)
			{
				printf("%c" , buf[i]);
				if( (i + 1)%16 == 0 )
					printf("\n");
			}
			printf("\n");
			
			print_log("In M8266WIFI_SPI_Receive_Data:upload data OK:link:%d , queue_id:%d ,  data_len:%d" ,id , link_occupied_flag_arrary[id].queue_id , queue_item.len);
		}
	}
	else
	{
		print_log("In M8266WIFI_SPI_Receive_Data: LOS_QueueReadCopy fail--->link:%d , queue id:%d" , id , link_occupied_flag_arrary[id].queue_id);
		return -1;
	}
	
	return queue_item.len;
}


/**
	*@brief this function shut down a connection accroding to fd(link num)
  *@params fd---> link num pass down from upper layer
  *@ret 1--->shutdown OK 0--->shutdown fail
*/
int32_t  m8266wifi_spi_close(int32_t fd)
{
	uint16_t status;
	
	print_log("enter M8266WIFI_SPI_Close ");
	
	/* entrance examination */
	if( fd < 0 || fd >= M8266_WIFI_MAX_LINK_NUM )
	{
		print_log("you cannot shutdown a connection which does not exist:the link no is beyond the max link no allowed");
		return 0;
	}
	
	/* clear Queue related params--->whether queue is deleted or not , reset relevant busy flag and free this link num  */
	{
		if( LOS_QueueDelete(link_occupied_flag_arrary[fd].queue_id) != LOS_OK )
		{
			print_log("delete link%d's queue%d fail" , fd , link_occupied_flag_arrary[fd].queue_id);
			link_occupied_flag_arrary[fd].busy_flag = 0;
		  return 0;
		}
		else
		{
			print_log("delete link%d's queue%d OK" , fd , link_occupied_flag_arrary[fd].queue_id);
			link_occupied_flag_arrary[fd].busy_flag = 0;
		}
	}
	
	
	m8266wifi_check_all_link_status();
	
//	/* close connection */
//	int ret = M8266WIFI_SPI_Delete_Connection(fd , &status);
//	if(ret != 1)
//	{
//		print_log("delete link:%d fail and status is 0x%x" , fd , status);
//		return 0;
//	}
	
	return 1;
}

