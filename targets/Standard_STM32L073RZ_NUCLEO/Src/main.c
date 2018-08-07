
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "los_base.h"
#include "los_task.h"
#include "los_inspect_entry.h"
#include "los_api_event.h"
#include "los_api_mutex.h"
#include "los_api_sem.h"
#include "los_api_msgqueue.h"
#include "los_api_static_mem.h"
#include "los_api_systick.h"
#include "los_api_timer.h"
#include "los_api_list.h"
#include "los_api_dynamic_mem.h"

extern UART_HandleTypeDef huart2;


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
UINT32 g_TskHandle;		/** TY Record the Task's ID, when create Task...*/
uint16_t g_nPeriod;		/** LED blink period,  ms */		 


UINT32 Create_LED_Task(void);
UINT32 Create_Test_Task(void);
void LED_Task(void);
void Test_Task(void);


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  UINT32 uwRet = LOS_OK;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  
  /* USER CODE BEGIN 2 */  
  if (LOS_OK != LOS_KernelInit())	
  {
      return LOS_NOK;
  }

  /** Create LED task */
  uwRet = Create_LED_Task();
  if(uwRet != LOS_OK)
  {
      return uwRet;
  }

  uwRet = Create_Test_Task();
  if(uwRet != LOS_OK)
  {
      return uwRet;
  }


  /** OS start */
  LOS_Start();
  
  /* USER CODE END 2 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
/**************************************************************/

/**
  * Create LED task.
  */
UINT32 Create_LED_Task(void)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 3;
    task_init_param.pcName = "LED_Task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)LED_Task;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}
void LED_Task(void)
{    
	printf("LiteOS Create LED_Task SUCCESS.\r\n");
	g_nPeriod = 2000;
	
	while(1)
	{        
        printf("This is LED_Task. OS g_nPeriod is %d .\r\n", g_nPeriod);

		/** LED */
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		/** Insert delay 1000 ms,  there is  2 way .  or used  :  HAL_Delay(1000);*/
		LOS_TaskDelay(g_nPeriod);
	}
}

/** 
  * Create task 2.
  */
UINT32 Create_Test_Task(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;

	task_init_param.usTaskPrio = 4;
	task_init_param.pcName = "Test_Task";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Test_Task;
	task_init_param.uwStackSize = 0x200;

	uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
	if(LOS_OK != uwRet)
	{
		return uwRet;
	}
	return uwRet;
}
  
void Test_Task(void)
{
    uint32_t nCount=0;
	printf("LiteOS Create Test_Task SUCCESS.\r\n");

	/** Test.1 : Event */
	Example_SndRcvEvent();
	printf("Test finished: Example_SndRcvEvent \r\n");
	LOS_TaskDelay(1000);
	/** Test.2 : Mutex */
	Example_MutexLock();
	printf("Test finished: Example_MutexLock \r\n");
	LOS_TaskDelay(1000);
	/** Test.3 : Semphore */
	Example_Semphore();
	printf("Test finished: Example_Semphore \r\n");
	LOS_TaskDelay(1000);
	/** Test.4 : Semphore */
	Example_MsgQueue();
	printf("Test finished: Example_MsgQueue \r\n");
	LOS_TaskDelay(5000);
	/** Test.5 : StaticMem */	
	Example_StaticMem();
	printf("Test finished: Example_StaticMem \r\n");
	LOS_TaskDelay(1000);
	/** Test.6 : StaticMem */	
	Example_GetTick();
	printf("Test finished: Example_GetTick \r\n");
	LOS_TaskDelay(1000);	
	/** Test.7 : swTimer */
	Example_swTimer();
	printf("Test finished: Example_swTimer \r\n");
	LOS_TaskDelay(1000);
	/** Test.8 : list */
	Example_list();
	printf("Test finished: Example_list \r\n");
	LOS_TaskDelay(1000);
	/** Test.9 : Dyn_Mem */
	Example_Dyn_Mem();
	printf("Test finished: Example_Dyn_Mem \r\n");
	LOS_TaskDelay(1000);	
	
	while(1)
	{
        nCount = LOS_TickCountGet();
        printf("This is Test_Task. OS tick count is %d .\r\n", nCount);
		LOS_TaskDelay(5000);		
	}
}

/**
  * @overwritten 
  * uint32_t HAL_GetTick(void)
  */
uint32_t HAL_GetTick(void)
{
	return (uint32_t)LOS_TickCountGet();
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected to the EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(GPIO_Pin);

	/* NOTE: This function Should not be modified, when the callback is needed,
		the HAL_GPIO_EXTI_Callback could be implemented in the user file
	*/
	if ((1000 == g_nPeriod) || (500 == g_nPeriod)){
		g_nPeriod = 500;
	}else{
		g_nPeriod = 1000;
	}    
}

/**************************************************************/
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
