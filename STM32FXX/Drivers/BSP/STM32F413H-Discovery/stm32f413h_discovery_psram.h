/**
  ******************************************************************************
  * @file    stm32f413H_discovery_psram.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32f413h_discovery_psram.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F413H_DISCOVERY_PSRAM_H
#define __STM32F413H_DISCOVERY_PSRAM_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F413H_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32F413H_DISCOVERY_PSRAM
  * @{
  */    

/** @defgroup STM32F413H_DISCOVERY_PSRAM_Exported_Constants STM32F413H DISCOVERY PSRAM Exported Constants
  * @{
  */

/** 
  * @brief  PSRAM status structure definition  
  */     
#define   PSRAM_OK         ((uint8_t)0x00)
#define   PSRAM_ERROR      ((uint8_t)0x01)

#define PSRAM_DEVICE_ADDR  ((uint32_t)0x60000000)
#define PSRAM_DEVICE_SIZE  ((uint32_t)0x80000)  /* SRAM device size in Bytes */  
  
#define PSRAM_MEMORY_WIDTH         FSMC_NORSRAM_MEM_BUS_WIDTH_16
#define PSRAM_BURSTACCESS          FSMC_BURST_ACCESS_MODE_DISABLE  
#define PSRAM_WRITEBURST           FSMC_WRITE_BURST_DISABLE
#define CONTINUOUSCLOCK_FEATURE    FSMC_CONTINUOUS_CLOCK_SYNC_ONLY 

/* DMA definitions for SRAM DMA transfer */
#define PSRAM_DMAx_CLK_ENABLE      __HAL_RCC_DMA2_CLK_ENABLE
#define PSRAM_DMAx_CLK_DISABLE     __HAL_RCC_DMA2_CLK_DISABLE
#define PSRAM_DMAx_CHANNEL         DMA_CHANNEL_0
#define PSRAM_DMAx_STREAM          DMA2_Stream5  
#define PSRAM_DMAx_IRQn            DMA2_Stream5_IRQn
#define BSP_PSRAM_DMA_IRQHandler   DMA2_Stream5_IRQHandler   
 
/**
  * @}
  */ 
   
/** @defgroup STM32F413H_DISCOVERY_PSRAM_Exported_Functions STM32F413H DISCOVERY PSRAM Exported Functions
  * @{
  */  
uint8_t BSP_PSRAM_Init(void);
uint8_t BSP_PSRAM_DeInit(void);
uint8_t BSP_PSRAM_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_PSRAM_ReadData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_PSRAM_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
uint8_t BSP_PSRAM_WriteData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize);
   
/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
void    BSP_PSRAM_MspInit(SRAM_HandleTypeDef  *hsram, void *Params);
void    BSP_PSRAM_MspDeInit(SRAM_HandleTypeDef  *hsram, void *Params);

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

#ifdef __cplusplus
}
#endif

#endif /* __STM32F413H_DISCOVERY_PSRAM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
