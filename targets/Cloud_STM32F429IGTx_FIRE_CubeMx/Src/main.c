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
#include "main.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"

#if defined WITH_AT_FRAMEWORK
#include "at/at_api_interface.h"
#if defined USE_NB_NEUL95
#include "nb_iot/los_nb_api.h"
#include "at/bc95.h"
#endif
#endif
#include "sys_init.h"
#include "agenttiny_lwm2m/agent_tiny_demo.h"
#ifdef SUPPORT_DTLS_SRV
#include "test_dtls_server.h"
#endif
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UINT32 g_TskHandle;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void demo_agenttiny_with_lwip(void)
{
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    hieth_hw_init();
    net_init();
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_LWIP\n");
#endif
}
void demo_nbiot_without_agenttiny(void)
{
#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
    #define AT_DTLS 0
    #if AT_DTLS
    sec_param_s sec;
    sec.setpsk = 1;
    sec.pskid = "868744031131026";
    sec.psk = "d1e1be0c05ac5b8c78ce196412f0cdb0";
    #endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP1: Init NB Module( NB Init )");
    printf("\r\n=====================================================\r\n");
#if AT_DTLS
    los_nb_init((const int8_t *)"180.101.147.115", (const int8_t *)"5684", &sec);
#else
    los_nb_init((const int8_t *)"180.101.147.115", (const int8_t *)"5683", NULL); //"139.159.140.34"
#endif
    printf("\r\n=====================================================");
    printf("\r\nSTEP2: Register Command( NB Notify )");
    printf("\r\n=====================================================\r\n");
    
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    los_nb_report("22", 2);
    los_nb_report("23", 1);
    
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_NB_NEUL95\n");
#endif

}
void demo_agenttiny_with_nbiot(void)
{
#if defined(WITH_AT_FRAMEWORK) && (defined(USE_NB_NEUL95))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry============================\n");
    los_nb_init((const int8_t *)"172.25.233.98",(const int8_t *)"5600",NULL);
    void agent_tiny_entry(void);
    los_nb_notify("\r\n+NSONMI:",strlen("\r\n+NSONMI:"),NULL,nb_cmd_match);
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_NB_NEUL95\n");
#endif
}
void demo_agenttiny_with_gprs(void)
{
#if defined(WITH_AT_FRAMEWORK) && (defined(USE_SIM900A))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry  USE_SIM900A============================\n");
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_SIM900A\n");
#endif
}

void demo_agenttiny_with_wifi(void)
{

#if defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266))
    extern at_adaptor_api at_interface;
    printf("\r\n=============agent_tiny_entry  USE_ESP8266============================\n");
    at_api_register(&at_interface);
    agent_tiny_entry();
#else
    printf("Please checkout if open WITH_AT_FRAMEWORK and USE_ESP8266\n");
#endif

}


VOID main_task(VOID)
{
    demo_agenttiny_with_nbiot();
}

UINT32 creat_main_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "main_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)main_task;

#ifdef CONFIG_FEATURE_FOTA
    task_init_param.uwStackSize = 0x2000; /* fota use mbedtls bignum to verify signature  consuming more stack  */
#else
    task_init_param.uwStackSize = 0x2000;
#endif

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t uwRet;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  //hal_rng_config();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /*call following func in at_usart_init*/
  //MX_USART3_UART_Init();
  //MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  uwRet = LOS_KernelInit();
  if (uwRet != LOS_OK)
  {
    return LOS_NOK;
  }
  printf("start main task!!\n");
  uwRet = creat_main_task();
  if (uwRet != LOS_OK)
  {
    return LOS_NOK;
  }
  (void)LOS_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //while (1)
  //{

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  //}
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
