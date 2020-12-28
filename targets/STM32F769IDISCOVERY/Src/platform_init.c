/**
******************************************************************************
* @file    platform_init.c
* @author  MCD Application Team
* @version V1.0.0
* @date    04-Aout-2017
* @brief   Board HW init
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: platfrom init
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "platform_init.h"
#include "cmsis_os.h"
#include "timer.h"


#define LTCD_LINE                   0


UART_HandleTypeDef huart1;
static osSemaphoreId vSyncEvent;
static uint32_t cptFlip;
static uint32_t gScreen[2];



#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

/**
* @brief  System Clock Configuration
*        The system Clock is configured as follow :
*           System Clock source           = PLL (HSE)
*           SYSCLK(Hz)                     = 216000000
*           HCLK(Hz)                       = 216000000
*           AHB Prescaler                 = 1
*           APB1 Prescaler                 = 4
*           APB2 Prescaler                 = 2
*           HSE Frequency(Hz)             = 25000000
*           PLL_M                         = 25
*           PLL_N                         = 432
*           PLL_P                         = 2
*           PLL_Q                         = 9
*           VDD(V)                         = 3.3
*           Main regulator output voltage = Scale1 mode
*           Flash Latency(WS)             = 7
* @param  None
* @retval None
*/

void SystemClock_Config(void)
{

  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  /*   __HAL_RCC_PWR_CLK_ENABLE(); */

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  /* Enable overdrive mode */
  HAL_PWREx_EnableOverDrive();

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 */
  /* Clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);

}

/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

static void UART_Config(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* Enable USART1 clock */
  __HAL_RCC_USART1_CLK_ENABLE();

  /* Configure USART1_TX (PA9) and USART1_RX (PA10) */
  GPIO_InitStructure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = baudrate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  HAL_UART_Init(&huart1);
}

/**
* @brief MPU configuration
**/

/*
We use SDRAM and stm32f769xx_flash_SDRAM.icf
 we use a double buffering
 so on top of the normal configuration ( nocache for eth and sram, we add LCD_FRAME_BUFFER_LAYER0 and LCD_FRAME_BUFFER_LAYER1

*/

__weak void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable MPU */
  HAL_MPU_Disable();

  /* SRAM main ram */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x20020000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* SRAM2 nocache */
  /* Configure the MPU attributes as Normal non-cacheable for Eth buffers the SRAM2 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x2007C000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
  MPU_InitStruct.SubRegionDisable = 0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1; /* Normal memory*/
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as Device for Ethernet Descriptors in the SRAM2 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x2007C000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512B;
  MPU_InitStruct.SubRegionDisable = 0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0; /* Device memory*/
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);


  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0xC0400000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
  MPU_InitStruct.SubRegionDisable = 0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as strongly ordred for QSPI (unused zone) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for QSPI (used 16Mbytes) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER5;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);


  /* Enable MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);


}


/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
  if(osKernelRunning())
  {
    osSemaphoreRelease(vSyncEvent);
  }
  HAL_LTDC_ProgramLineEvent(&hltdc_discovery, LTCD_LINE);

}


__weak void LCD_Vsync_Init()
{
  osSemaphoreDef(SEM);
  /* VSync will be sychronized by an event from the LCD line ISR */
  vSyncEvent = osSemaphoreCreate(osSemaphore(SEM),1);
  osSemaphoreWait(vSyncEvent, 0);
#ifdef _USE_FreeRTOS
  /* By default, LCD line as an higher priority than the kernel, we need to reduce it, in order to allow rtos syscall in the ISR */
  NVIC_SetPriority(LTDC_IRQn, (configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4));
  HAL_LTDC_ProgramLineEvent(&hltdc_discovery, LTCD_LINE);
#endif
}



/*

  Init the default LCD support
  This standard init could be overloaded to fix a tearing effect due to the use of a DSI screen

*/
__weak void GUI_LCD_Init(void)
{
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_Clear(0);

  BSP_LCD_SetLayerVisible(0, ENABLE);
  BSP_LCD_SelectLayer(0);
  BSP_LCD_DisplayOn();


  /* Compute screen placement */
  gScreen[0] = LCD_FB_START_ADDRESS;
  gScreen[1] = LCD_FB_START_ADDRESS + BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4;
  LCD_Vsync_Init();
}
/*

  Set the screen buffer

*/

__weak uint32_t LCD_SwapBuffer(void)
{
  uint32_t iScreen  = gScreen[cptFlip & 1];

  LTDC_LAYER(&hltdc_discovery, 0)->CFBAR = iScreen;
  __HAL_LTDC_RELOAD_CONFIG(&hltdc_discovery);
  cptFlip++;
  return gScreen[cptFlip & 1];
}

/*

  Wait the next vertical blancking

*/
__weak void LCD_WaitVSync(void)
{
  osSemaphoreWait(vSyncEvent, 1000);

}

void KeyWord_Button_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
   /* Configure PJ0 pin as input */
    gpio_init_structure.Pin = GPIO_PIN_0;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_0, GPIO_PIN_RESET);

  /* Configure PJ1pin as input */
    gpio_init_structure.Pin = GPIO_PIN_1;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_RESET);

  /* Configure PJ3 pin as input */
    gpio_init_structure.Pin = GPIO_PIN_3;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_3, GPIO_PIN_RESET);

  /* Configure PJ4 pin as input */
    gpio_init_structure.Pin = GPIO_PIN_4;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_4, GPIO_PIN_RESET);

    /* Configure PC6 pin as input */
    gpio_init_structure.Pin = GPIO_PIN_6;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOC, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

    /* Configure PC7 pin as input */
    gpio_init_structure.Pin = GPIO_PIN_7;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOC, &gpio_init_structure);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
}


void platform_Init(void)
{
#ifdef  USE_TRACEALYZER
#if ( configUSE_TRACE_FACILITY == 1 )
  vTraceEnable(TRC_START);
#endif
#endif

  /* LED configuration */
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);
  /* Clear LEDs */
  BSP_LED_Off(LED_RED);

  /* Mandatory for spirit mp3 codec ??????? */
#ifdef STM32F10X_CL
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_CRC, ENABLE);
#else
  RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
#endif /* STM32F10X_CL */

  KeyWord_Button_init();

  /* Initialize user button */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO /*BUTTON_MODE_GPIO*/);

  /* Mandatory for assets mng */
  BSP_QSPI_Init();
  BSP_QSPI_EnableMemoryMappedMode();

#if defined(LiteVS_USE_DEBUG)
  //GPIO_Config();
#endif

}

void HardwareInit(void)
{
    MPU_Config();

    /* HAL library initialization */

    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    UART_Config(115200);
    TimerInit();

    /* Initialize the SDRAM */
    BSP_SDRAM_Init();
}



/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void Stm32UartPuts(const char* str, uint32_t len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 0xffff);
    return;
}

