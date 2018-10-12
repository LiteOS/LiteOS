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

/* Includes -----------------------------------------------------------------*/
#include "hal_sram.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_sram.h"

/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
SRAM_HandleTypeDef sramHandle;
static FSMC_NORSRAM_TimingTypeDef Timing;

/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
void BSP_SRAM_MspInit(void)
{
  static DMA_HandleTypeDef hdma1;
  GPIO_InitTypeDef gpioinitstruct = {0};
  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable DMA1 and DMA2 clocks */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_NOPULL;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  
/*-- GPIO Configuration ------------------------------------------------------*/
  /*!< SRAM Data lines configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                                GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 
  
  gpioinitstruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                                GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpioinitstruct);
  
  /*!< SRAM Address lines configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | 
                                GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpioinitstruct);
  
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
                                GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
  
  gpioinitstruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; 
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
   
  /*!< NOE and NWE configuration */  
  gpioinitstruct.Pin = GPIO_PIN_4 |GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
  
  /*!< NE3 configuration */
  gpioinitstruct.Pin = GPIO_PIN_10 |GPIO_PIN_12; 
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
  
  /*!< NBL0, NBL1 configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1; 
  HAL_GPIO_Init(GPIOE, &gpioinitstruct); 

  /* Configure common DMA parameters */
  hdma1.Init.Direction           = DMA_MEMORY_TO_MEMORY;
  hdma1.Init.PeriphInc           = DMA_PINC_ENABLE;
  hdma1.Init.MemInc              = DMA_MINC_ENABLE;
  hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  hdma1.Init.Mode                = DMA_NORMAL;
  hdma1.Init.Priority            = DMA_PRIORITY_HIGH;

  hdma1.Instance = DMA1_Channel1;
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&hdma1);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdma1);
  
  /* Associate the DMA handle to the FSMC SRAM one */
  sramHandle.hdma = &hdma1;
  
  /* NVIC configuration for DMA transfer complete interrupt */
//  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0xC, 0);
//  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);   
  
} 

uint8_t hal_sram_init(void)
{
  sramHandle.Instance  = FSMC_NORSRAM_DEVICE;
  sramHandle.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;
  
  /* SRAM device configuration */  
  Timing.AddressSetupTime      = 2;
  Timing.AddressHoldTime       = 1;
  Timing.DataSetupTime         = 2;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision           = 2;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FSMC_ACCESS_MODE_A;
  
  sramHandle.Init.NSBank             = FSMC_NORSRAM_BANK3;
  sramHandle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  sramHandle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
  sramHandle.Init.MemoryDataWidth    = SRAM_MEMORY_WIDTH;
  sramHandle.Init.BurstAccessMode    = SRAM_BURSTACCESS;
  sramHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  sramHandle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
  sramHandle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  sramHandle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  sramHandle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  sramHandle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
  sramHandle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  sramHandle.Init.WriteBurst         = SRAM_WRITEBURST;
    
  /* SRAM controller initialization */
  BSP_SRAM_MspInit();
  
  if(HAL_SRAM_Init(&sramHandle, &Timing, &Timing) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  }
}
uint8_t hal_sram_read_data(uint32_t addr, uint16_t *buff, uint32_t len)
{
    if(HAL_SRAM_Read_16b(&sramHandle, (uint32_t *)addr, buff, len) != HAL_OK)
    {
      return SRAM_ERROR;
    }
    else
    {
      return SRAM_OK;
    }

}
uint8_t hal_sram_write_data(uint32_t addr, uint16_t *buff, uint32_t len)
{
    if(HAL_SRAM_Write_16b(&sramHandle, (uint32_t *)addr, buff, len) != HAL_OK)
    {
      return SRAM_ERROR;
    }
    else
    {
      return SRAM_OK;
    }

}

/* Private functions --------------------------------------------------------*/

