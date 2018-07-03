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
 
 #ifndef __M8266_WIFI_CONFIG_H__
 #define __M8266_WIFI_CONFIG_H__
 
 #include "stdio.h"
 #include "string.h"
 #include "stm32f1xx.h"
 #include "stm32f1xx_hal_conf.h"
 #include "math.h"
 #include "atiny_socket.h"
 #include "atiny_adapter.h"
 #include "cmsis_os2.h"
 #include "los_task.ph"
 #include "los_sem.h"
 #include "los_sem.ph"
 #include "los_queue.h"
 #include "M8266_WIFI_SPI_PRINT.h"
 
 
  /*
	Please read this header file thoroughly , it contians hardware(SPI) configuration , function protypes
	access point settings and so on. This header file is hardware related
  */
 
 
/**
 *@brief The MCU uses SPI interface to communicate with esp8266 wifi module , this 
				 header file contains all hardware configurations and other relevant macros
				 and function prototypes.
				 
				 And in this example(stm32f103xe) , 5 signal pins are used , which is listed
				 as follows:
				 SCLK---> spi clock pin : base clock reference for communications
				 MISO---> master input and slave output pin: input for master(in this context:master) , output for slave(in this context:eps8266 wifi module)
				 MOSI---> master output and slave input pin: output for master(in this context:master) , input for slave(in this context:esp8266 wifi module)
				 nCS ---> chip select pin: used for chip selection and before any communications begin ,nCS must be pulled low
				 nRESET---> reset esp8266 wifi module
				 
				 nCS and nRESET pins will be manipulated in normal GPIO mode
 */
 
 
 /**
 *@brief below are elaborate hardware configurations:
         SPI interface:we use SPI1 here , you can modify according to yours needs
				 SPI1:  PA4---> nCS pin , but configured as normal GPIO mode
				        PA5---> SCLK pin , configured in SPI mode
								PA6---> MISO pin , configured in SPI mode
								PA7---> MOSI pin , configured in SPI mode
								PB12---> nRESET pin , configured in normal GPIO mode
								3.3V and GND pins: pick on your own development board
 */
 
 extern osStatus_t osDelay (uint32_t ticks);


 /* data structure */
 typedef struct
 {
		uint8_t   test_connection_type;			
	  uint8_t   test_connection_state;
	  uint16_t  test_local_port;
		uint16_t  test_status;
		uint16_t	test_remote_port;
	  uint8_t   test_remote_ip[16];
 }connection_params_struct;


/*---for compile consideration: the lib used here is from third party---*/
/*if you use other STM32 chips choose among: MCU_IS_STM32F2XX ; MCU_IS_STM32F3XX ; MCU_IS_STM32F4XX ; MCU_IS_STM32F7XX */
 #ifndef  MCU_IS_STM32F1XX
 #define  MCU_IS_STM32F1XX
 #endif
 
 #define M8266WIFI_SPI_nCS_GPIO													M8266WIFI_SPI_nCS_GPIO_PORT
 #define M8266WIFI_SPI_nCS_PIN                          M8266WIFI_SPI_nCS_GPIO_PIN
/*---for compile consideration: the lib used here is from third party---*/


#define UNIT_MHZ (uint32_t)1000000 


 /* nRESET configuration */ 
 #define  M8266WIFI_nRESET_GPIO_RCC_CLOCK_EN()				  __HAL_RCC_GPIOB_CLK_ENABLE()
 #define  M8266WIFI_nRESET_GPIO_PORT									  GPIOB
 #define  M8266WIFI_nRESET_GPIO_PIN									    GPIO_PIN_12

 /* nCS configurations */
 #define  M8266WIFI_SPI_nCS_GPIO_RCC_CLOCK_EN()				  __HAL_RCC_GPIOA_CLK_ENABLE()
 #define  M8266WIFI_SPI_nCS_GPIO_PORT								    GPIOA
 #define  M8266WIFI_SPI_nCS_GPIO_PIN									  GPIO_PIN_4

 /* SCLK | MOSI | MISO configurations */
 #define M8266_WIFI_SPI_Com_SCLK_CLOCK_EN()             __HAL_RCC_GPIOA_CLK_ENABLE()
 #define M8266_WIFI_SPI_Com_MISO_CLOCK_EN()             __HAL_RCC_GPIOA_CLK_ENABLE()
 #define M8266_WIFI_SPI_Com_MOSI_CLOCK_EN()             __HAL_RCC_GPIOA_CLK_ENABLE() 
 
 #define M8266_WIFI_SPI_Com_SCLK_PORT                  GPIOA
 #define M8266_WIFI_SPI_Com_MISO_PORT                  GPIOA
 #define M8266_WIFI_SPI_Com_MOSI_PORT                  GPIOA
 
 #define M8266_WIFI_SPI_Com_SCLK_PIN                   GPIO_PIN_5
 #define M8266_WIFI_SPI_Com_MISO_PIN                   GPIO_PIN_6
 #define M8266_WIFI_SPI_Com_MOSI_PIN                   GPIO_PIN_7
 
 #define M8266_WIFI_SPI_Peripheral_EN()                __HAL_RCC_SPI1_CLK_ENABLE()
 #define M8266_WIFI_SPI_FORCE_RESET()                  __HAL_RCC_SPI1_FORCE_RESET()
 #define M8266_WIFI_SPI_RELEASE_RESET()                __HAL_RCC_SPI1_RELEASE_RESET()

 /* SPI base address */
 #ifndef SPI_BaudRatePrescaler_2
 #define SPI_BaudRatePrescaler_2 				 ((uint32_t)0x00000000U)
 #define SPI_BaudRatePrescaler_4         ((uint32_t)0x00000008U)
 #define SPI_BaudRatePrescaler_8         ((uint32_t)0x00000010U)
 #define SPI_BaudRatePrescaler_16        ((uint32_t)0x00000018U)
 #define SPI_BaudRatePrescaler_32        ((uint32_t)0x00000020U)
 #define SPI_BaudRatePrescaler_64        ((uint32_t)0x00000028U)
 #define SPI_BaudRatePrescaler_128       ((uint32_t)0x00000030U)
 #define SPI_BaudRatePrescaler_256       ((uint32_t)0x00000038U)
 #endif

 /* choose a baud rate prescaler according to SPI you choose */
 #define  M8266WIFI_INTERFACE_SPI								      SPI1
 #define  M8266WIFI_SPI_BAUD_RATE_PRESCALER           SPI_BaudRatePrescaler_4    
 
 #define  BASE_CONSTANT	(double)2
 #define  M8266_WIFI_MAX_LINK_NUM	4
 #define  MAX_DATA_LEN	1024
 
 /* functions prototypes */
 uint8_t 	m8266wifi_verify_connect_func(connection_params_struct * plink); //this function is used individually
 uint8_t 	m8266wifi_check_all_link_status(void);
 uint8_t 	m8266wifi_query_certain_link_params(uint8_t link_no , connection_params_struct * plink);
 
 int32_t  m8266_wifi_init(void);
 int32_t 	m8266wifi_connect(const char * host, const char * port, int32_t proto);
 int32_t 	m8266wifi_send_data(int32_t id , const uint8_t  *buf, size_t len);
 int32_t  m8266wifi_spi_receive_data(int32_t id , uint8_t * buf , size_t len);
 int32_t  m8266wifi_spi_receive_data_timeout(int32_t id , uint8_t * buf , size_t len , uint32_t timeout);
 int32_t  m8266wifi_spi_close(int32_t fd);
 				
 /* test macros */
 #define TEST_TCP_UDP_TYPE     ATINY_PROTO_TCP  //refer to atiny_socket.h
 #define TEST_REMOTE_ADDRESS	(const char *)"www.baidu.com"
 #define TEST_REMOTE_PORT     (const char *)"80"
 #define TEST_REMOTE_LINK_NO  (uint8_t)0  //maximum--->4 //fixed to 0 is suggested
 #define TEST_LOCAL_PORT      (uint16_t)0 
 
 /* use OS macro */
 #define WIFI8266_USE_OS
																			 																		 
 /* communication macros */
#define      ESP8266_ApSSID                       "TP-LINK_80D8BE"     
#define      ESP8266_ApPWD                        "87654321"  
#define      ESP8266_MODULE_NAME                  (char*)"M8266_SPI"
															 
 /* GPIO(interrupt mode) configuration used to trigger receiving data  */
#define      M8266_GPIO_EXTI_PORT            GPIOB
#define      M8266_GPIO_EXTI_PIN             GPIO_PIN_0
#define      M8266_GPIO_EXTI_IRQ_NUM         EXTI0_IRQn
#define      M8266_GPIO_EXTI_IRQ_PRIORITY    0
#define      M8266_GPIO_EXTI_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
 
 #endif
 
 
