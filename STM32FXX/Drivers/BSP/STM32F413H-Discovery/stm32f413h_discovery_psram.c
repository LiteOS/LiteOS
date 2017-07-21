/**
  ******************************************************************************
  * @file    stm32f413h_discovery_psram.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file includes the PSRAM driver for the IS61WV51216BLL-10MLI memory 
  *          device mounted on STM32F413H-DISCOVERY boards.
  @verbatim
  How To use this driver:
  -----------------------
   - This driver is used to drive the IS61WV51216BLL-10M PSRAM external memory mounted
     on STM32F413H-DisCovERYevaluation board.
   - This driver does not need a specific component driver for the PSRAM device
     to be included with.

  Driver description:
  ------------------
  + Initialization steps:
     o Initialize the PSRAM external memory using the BSP_PSRAM_Init() function. This 
       function includes the MSP layer hardware resources initialization and the
       FSMC controller configuration to interface with the external PSRAM memory.
  
  + PSRAM read/write operations
     o PSRAM external memory can be accessed with read/write operations once it is
       initialized.
       Read/write operation can be performed with AHB access using the functions
       BSP_PSRAM_ReadData()/BSP_PSRAM_WriteData(), or by DMA transfer using the functions
       BSP_PSRAM_ReadData_DMA()/BSP_PSRAM_WriteData_DMA().
     o The AHB access is performed with 16-bit width transaction, the DMA transfer
       configuration is fixed at single (no burst) halfword transfer 
       (see the PSRAM_MspInit() static function).
     o User can implement his own functions for read/write access with his desired 
       configurations.
     o If interrupt mode is used for DMA transfer, the function BSP_PSRAM_DMA_IRQHandler()
       is called in IRQ handler file, to serve the generated interrupt once the DMA 
       transfer is complete.
  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f413h_discovery_psram.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */ 
  
/** @defgroup STM32F413H_DISCOVERY_PSRAM STM32F413H_DISCOVERY PSRAM
  * @{
  */ 

/** @defgroup STM32F413H_DISCOVERY_PSRAM_Private_Variables STM32F413H DISCOVERY PSRAM Private Variables
  * @{
  */       
SRAM_HandleTypeDef psramHandle;
static FSMC_NORSRAM_TimingTypeDef Timing;

/**
  * @}
  */ 
    
/** @defgroup STM32F413H_DISCOVERY_PSRAM_Private_Functions STM32F413H DISCOVERY PSRAM Private Functions
  * @{
  */ 

/**
  * @brief  Initializes the PSRAM device.
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_Init(void)
{ 
  static uint8_t psram_status = PSRAM_ERROR;
  /* SRAM device configuration */
  psramHandle.Instance = FSMC_NORSRAM_DEVICE;
  psramHandle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  
  /* PSRAM device configuration */
  /* Timing configuration derived from system clock (up to 100Mhz)*/
  Timing.AddressSetupTime      = 3;
  Timing.AddressHoldTime       = 1;
  Timing.DataSetupTime         = 4;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision           = 2;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FSMC_ACCESS_MODE_A;
  
  psramHandle.Init.NSBank             = FSMC_NORSRAM_BANK1;
  psramHandle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  psramHandle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
  psramHandle.Init.MemoryDataWidth    = PSRAM_MEMORY_WIDTH;
  psramHandle.Init.BurstAccessMode    = PSRAM_BURSTACCESS;
  psramHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  psramHandle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  psramHandle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  psramHandle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  psramHandle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
  psramHandle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  psramHandle.Init.WriteBurst         = PSRAM_WRITEBURST;
  psramHandle.Init.ContinuousClock    = CONTINUOUSCLOCK_FEATURE;
    
  /* PSRAM controller initialization */
  BSP_PSRAM_MspInit(&psramHandle, NULL); /* __weak function can be rewritten by the application */
  if(HAL_SRAM_Init(&psramHandle, &Timing, &Timing) != HAL_OK)
  {
    psram_status = PSRAM_ERROR;
  }
  else
  {
    psram_status = PSRAM_OK;
  }
  return psram_status;
}

/**
  * @brief  DeInitializes the PSRAM device.
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_DeInit(void)
{ 
  static uint8_t psram_status = PSRAM_ERROR;
  /* PSRAM device de-initialization */
  psramHandle.Instance = FSMC_NORSRAM_DEVICE;
  psramHandle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

  if(HAL_SRAM_DeInit(&psramHandle) != HAL_OK)
  {
    psram_status = PSRAM_ERROR;
  }
  else
  {
    psram_status = PSRAM_OK;
  }
  
  /* PSRAM controller de-initialization */
  BSP_PSRAM_MspDeInit(&psramHandle, NULL);
  
  return psram_status;
}

/**
  * @brief  Reads an amount of data from the PSRAM device in polling mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory   
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{ 
  if(HAL_SRAM_Read_16b(&psramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Reads an amount of data from the PSRAM device in DMA mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory   
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_ReadData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Read_DMA(&psramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the PSRAM device in polling mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory   
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize) 
{ 
  if(HAL_SRAM_Write_16b(&psramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the PSRAM device in DMA mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory   
  * @retval PSRAM status
  */
uint8_t BSP_PSRAM_WriteData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize) 
{
  if(HAL_SRAM_Write_DMA(&psramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return PSRAM_ERROR;
  }
  else
  {
    return PSRAM_OK;
  } 
}

/**
  * @brief  Initializes PSRAM MSP.
  * @param  hsram: PSRAM handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void BSP_PSRAM_MspInit(SRAM_HandleTypeDef  *hsram, void *Params)
{  
  static DMA_HandleTypeDef dma_handle;
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);

  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable chosen DMAx clock */
  PSRAM_DMAx_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FSMC;
  
  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7      |\
                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12   |\
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7  | GPIO_PIN_8 | GPIO_PIN_9     |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4       |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4       |\
                              GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
  
  /* Configure common DMA parameters */
  dma_handle.Init.Channel             = PSRAM_DMAx_CHANNEL;
  dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
  dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
  dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
  dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  dma_handle.Init.Mode                = DMA_NORMAL;
  dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
  dma_handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  dma_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dma_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
  dma_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;
  
  dma_handle.Instance = PSRAM_DMAx_STREAM;
  
   /* Associate the DMA handle */
  __HAL_LINKDMA(hsram, hdma, dma_handle);
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&dma_handle);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&dma_handle);
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(PSRAM_DMAx_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(PSRAM_DMAx_IRQn);   
}

/**
  * @brief  DeInitializes SRAM MSP.
  * @param  hsram: SRAM handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  * @retval None
  */
__weak void BSP_PSRAM_MspDeInit(SRAM_HandleTypeDef *hsram, void *Params)
{  
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(PSRAM_DMAx_IRQn);
  
  if(hsram->Instance != NULL)
  {
    /* Deinitialize the stream for new transfer */
    HAL_DMA_DeInit(hsram->hdma);
  }
  
  /* GPIO pins clock, FSMC clock and DMA clock can be shut down in the applications
  by surcharging this __weak function */ 
}
/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
