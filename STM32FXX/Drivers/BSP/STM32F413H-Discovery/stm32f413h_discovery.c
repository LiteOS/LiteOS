/**
  ******************************************************************************
  * @file    stm32f413h_discovery.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-January-2017
  * @brief   This file provides a set of firmware functions to manage LEDs, 
  *          push-buttons and COM ports available on STM32F413H-DISCOVERY board 
  *          (MB1209) from STMicroelectronics.
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
#include "stm32f413h_discovery.h"


/** @addtogroup BSP
  * @{
  */ 

/** @defgroup STM32F413H_DISCOVERY STM32F413H_DISCOVERY
  * @{
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL STM32F413H-DISCOVERY LOW LEVEL
  * @{
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions STM32F413H Discovery Low Level Private Typedef
  * @{
  */
typedef struct
{
  __IO uint16_t REG;
  __IO uint16_t RAM;
}LCD_CONTROLLER_TypeDef;
/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Private_Defines STM32F413H Discovery Low Level Private Def
  * @{
  */
/**
 * @brief STM32F413H DISCOVERY BSP Driver version number V1.0.0
   */
#define __STM32F413H_DISCOVERY_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32F413H_DISCOVERY_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32F413H_DISCOVERY_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32F413H_DISCOVERY_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F413H_DISCOVERY_BSP_VERSION        ((__STM32F413H_DISCOVERY_BSP_VERSION_MAIN << 24)\
                                                 |(__STM32F413H_DISCOVERY_BSP_VERSION_SUB1 << 16)\
                                                 |(__STM32F413H_DISCOVERY_BSP_VERSION_SUB2 << 8 )\
                                                 |(__STM32F413H_DISCOVERY_BSP_VERSION_RC))
											                                    
/* We use BANK3 as we use FMC_NE3 signal */
#define FMC_BANK3_BASE  ((uint32_t)(0x60000000 | 0x08000000))  
#define FMC_BANK3       ((LCD_CONTROLLER_TypeDef *) FMC_BANK3_BASE)

/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Private_Variables STM32F413H Discovery Low Level Variables 
  * @{
  */

const uint32_t GPIO_PIN[LEDn] = {LED3_PIN,
                                 LED4_PIN};


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED3_GPIO_PORT,
                                 LED4_GPIO_PORT};


GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT};

const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN};

const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {DISCOVERY_COM1};

GPIO_TypeDef* COM_TX_PORT[COMn] = {DISCOVERY_COM1_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {DISCOVERY_COM1_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn] = {DISCOVERY_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {DISCOVERY_COM1_RX_PIN};

const uint16_t COM_TX_AF[COMn] = {DISCOVERY_COM1_TX_AF};

const uint16_t COM_RX_AF[COMn] = {DISCOVERY_COM1_RX_AF};

static FMPI2C_HandleTypeDef hI2cAudioHandler;

/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes STM32F413H Discovery Low Level Private Prototypes
  * @{
  */
static void FMPI2Cx_Init(FMPI2C_HandleTypeDef *i2c_handler);
static void FMPI2Cx_DeInit(FMPI2C_HandleTypeDef *i2c_handler);

static HAL_StatusTypeDef FMPI2Cx_ReadMultiple(FMPI2C_HandleTypeDef *fmpi2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef FMPI2Cx_WriteMultiple(FMPI2C_HandleTypeDef *fmpi2c_handler, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length);
static void              FMPI2Cx_Error(FMPI2C_HandleTypeDef *fmpi2c_handler, uint8_t Addr);

static void     FMC_BANK3_WriteData(uint16_t Data);
static void     FMC_BANK3_WriteReg(uint8_t Reg);
static uint16_t FMC_BANK3_ReadData(void);
static void     FMC_BANK3_Init(void);
static void     FMC_BANK3_MspInit(void);

/* LCD IO functions */
void            LCD_IO_Init(void);
void            LCD_IO_WriteData(uint16_t RegValue);
void            LCD_IO_WriteReg(uint8_t Reg);
void            LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size);
uint16_t        LCD_IO_ReadData(void);
void            LCD_IO_Delay(uint32_t Delay);

/* AUDIO IO functions */
void            AUDIO_IO_Init(void);
void            AUDIO_IO_DeInit(void);
void            AUDIO_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value);
uint16_t        AUDIO_IO_Read(uint8_t Addr, uint16_t Reg);
void            AUDIO_IO_Delay(uint32_t Delay);

/* TouchScreen (TS) IO functions */
void            TS_IO_Init(void);
void            TS_IO_DeInit(void);
void            TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t         TS_IO_Read(uint8_t Addr, uint8_t Reg);
uint16_t        TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void            TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length);
void            TS_IO_Delay(uint32_t Delay);

/**
  * @}
  */

/** @defgroup STM32F413H_DISCOVERY_LOW_LEVEL_Private_Functions STM32F413H Discovery Low Level Private Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32F413H DISCOVERY BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32F413H_DISCOVERY_BSP_VERSION;
}

/**
  * @brief  Configures LEDs.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED3
  *            @arg  LED4
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  LEDx_GPIO_CLK_ENABLE(Led);
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin   = GPIO_PIN[Led];
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_HIGH;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &gpio_init_structure);
}

/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED3
  *            @arg  LED4
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = GPIO_PIN[Led];
  
  /* Turn off LED */
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
  HAL_GPIO_DeInit(GPIO_PORT[Led], gpio_init_structure.Pin);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: LED to be set on 
  *          This parameter can be one of the following values:
  *            @arg  LED3
  *            @arg  LED4
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off. 
  * @param  Led: LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED3
  *            @arg  LED4
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED3
  *            @arg  LED4
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button
  * @param  ButtonMode: Button mode
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *            @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line 
  *                                    with interrupt generation capability  
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable the BUTTON clock */
  WAKEUP_BUTTON_GPIO_CLK_ENABLE();
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpio_init_structure.Pin = BUTTON_PIN[Button];
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
  }
  
  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Pin = BUTTON_PIN[Button];
    gpio_init_structure.Pull = GPIO_PULLDOWN;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING;
    
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button 
  * @note PB DeInit does not disable the GPIO clock
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef gpio_init_structure;

  gpio_init_structure.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}


/**
  * @brief  Returns the selected button state.
  * @param  Button: Button to be checked
  *          This parameter can be one of the following values:
  *            @arg  BUTTON_WAKEUP: Wakeup Push Button 
  * @retval The Button GPIO pin value (GPIO_PIN_RESET = button pressed)
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1 
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable GPIO clock */
  DISCOVERY_COMx_TX_GPIO_CLK_ENABLE(COM);
  DISCOVERY_COMx_RX_GPIO_CLK_ENABLE(COM);

  /* Enable USART clock */
  DISCOVERY_COMx_CLK_ENABLE(COM);

  /* Configure USART Tx as alternate function */
  gpio_init_structure.Pin = COM_TX_PIN[COM];
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Alternate = COM_TX_AF[COM];
  HAL_GPIO_Init(COM_TX_PORT[COM], &gpio_init_structure);

  /* Configure USART Rx as alternate function */
  gpio_init_structure.Pin = COM_RX_PIN[COM];
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = COM_RX_AF[COM];
  HAL_GPIO_Init(COM_RX_PORT[COM], &gpio_init_structure);

  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_Init(huart);
}

/**
  * @brief  DeInit COM port.
  * @param  COM: COM port to be configured.
  *          This parameter can be one of the following values:
  *            @arg  COM1 
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
void BSP_COM_DeInit(COM_TypeDef COM, UART_HandleTypeDef *huart)
{
  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_DeInit(huart);

  /* Enable USART clock */
  DISCOVERY_COMx_CLK_DISABLE(COM);

  /* DeInit GPIO pins can be done in the application 
     (by surcharging this __weak function) */

  /* GPIO pins clock, FMC clock and DMA clock can be shut down in the application 
     by surcharging this __weak function */ 
}


/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* I2C Routines *********************************/
/**
  * @brief  Initializes FMPI2C MSP.
  * @param  fmpi2c_handler : FMPI2C handler
  */
static void FMPI2Cx_MspInit(FMPI2C_HandleTypeDef *fmpi2c_handler)
{
  GPIO_InitTypeDef  gpio_init_structure;  
  
  /* AUDIO FMPI2C MSP init */
  
  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  DISCOVERY_AUDIO_I2Cx_SCL_SDA_GPIO_CLK_ENABLE();
  
  /* Configure I2C Tx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_AUDIO_I2Cx_SCL_PIN;
  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = DISCOVERY_AUDIO_I2Cx_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_AUDIO_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);
  
  /* Configure I2C Rx as alternate function */
  gpio_init_structure.Pin = DISCOVERY_AUDIO_I2Cx_SDA_PIN;
  gpio_init_structure.Alternate = DISCOVERY_AUDIO_I2Cx_SCL_SDA_AF;
  HAL_GPIO_Init(DISCOVERY_AUDIO_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);
  
  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  DISCOVERY_AUDIO_I2Cx_CLK_ENABLE();
  
  /* Force the I2C peripheral clock reset */
  DISCOVERY_AUDIO_I2Cx_FORCE_RESET();
  
  /* Release the I2C peripheral clock reset */
  DISCOVERY_AUDIO_I2Cx_RELEASE_RESET();
  
  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_AUDIO_I2Cx_EV_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(DISCOVERY_AUDIO_I2Cx_EV_IRQn);
  
  /* Enable and set I2Cx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(DISCOVERY_AUDIO_I2Cx_ER_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(DISCOVERY_AUDIO_I2Cx_ER_IRQn);
}

/**
  * @brief  Initializes FMPI2C HAL.
  * @param  fmpi2c_handler : FMPI2C handler
  */
static void FMPI2Cx_Init(FMPI2C_HandleTypeDef *fmpi2c_handler)
{
  if(HAL_FMPI2C_GetState(fmpi2c_handler) == HAL_FMPI2C_STATE_RESET)
  {
    /* Audio FMPI2C configuration */
    fmpi2c_handler->Instance              = DISCOVERY_AUDIO_I2Cx;
    fmpi2c_handler->Init.Timing           = DISCOVERY_I2Cx_TIMING;
    fmpi2c_handler->Init.OwnAddress1      = 0;
    fmpi2c_handler->Init.AddressingMode   = FMPI2C_ADDRESSINGMODE_7BIT;
    fmpi2c_handler->Init.DualAddressMode  = FMPI2C_DUALADDRESS_DISABLE;
    fmpi2c_handler->Init.OwnAddress2      = 0;
    fmpi2c_handler->Init.OwnAddress2Masks = FMPI2C_OA2_NOMASK;
    fmpi2c_handler->Init.GeneralCallMode  = FMPI2C_GENERALCALL_DISABLE;
    fmpi2c_handler->Init.NoStretchMode    = FMPI2C_NOSTRETCH_DISABLE;
    /* Init the FMPI2C */
    FMPI2Cx_MspInit(fmpi2c_handler);
    HAL_FMPI2C_Init(fmpi2c_handler);
  }
}

/**
  * @brief  Reads multiple data.
  * @param  fmpi2c_handler : FMPI2C handler
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddress: Memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
static HAL_StatusTypeDef FMPI2Cx_ReadMultiple(FMPI2C_HandleTypeDef *fmpi2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddress,
                                           uint8_t *Buffer,
                                           uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_FMPI2C_Mem_Read(fmpi2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* FMPI2C error occurred */
    FMPI2Cx_Error(fmpi2c_handler, Addr);
  }
  return status;    
}

/**
  * @brief  Writes a value in a register of the device through BUS in using DMA mode.
  * @param  fmpi2c_handler : FMPI2C handler
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  MemAddress: Memory address
  * @param  Buffer: The target register value to be written
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2Cx_WriteMultiple(FMPI2C_HandleTypeDef *fmpi2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddress,
                                            uint8_t *Buffer,
                                            uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_FMPI2C_Mem_Write(fmpi2c_handler, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, 1000);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the FMPI2C Bus */
    FMPI2Cx_Error(fmpi2c_handler, Addr);
  }
  return status;
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  fmpi2c_handler : FMPI2C handler
  * @param  Addr: I2C Address
  * @retval None
  */
static void FMPI2Cx_Error(FMPI2C_HandleTypeDef *fmpi2c_handler, uint8_t Addr)
{
  /* De-initialize the FMPI2C communication bus */
  HAL_FMPI2C_DeInit(fmpi2c_handler);
  
  /* Re-Initialize the FMPI2C communication bus */
  FMPI2Cx_Init(fmpi2c_handler);
}

/**
  * @brief  Deinitializes FMPI2C interface
  * @param  fmpi2c_handler : FMPI2C handler
  */
 static void FMPI2Cx_DeInit(FMPI2C_HandleTypeDef *fmpi2c_handler)
{
  /* Audio and LCD I2C configuration */
  fmpi2c_handler->Instance = DISCOVERY_AUDIO_I2Cx;
  /* Disable FMPI2C block */
  __HAL_FMPI2C_DISABLE(fmpi2c_handler);
  
  /* DeInit the FMPI2C */
  HAL_FMPI2C_DeInit(fmpi2c_handler);
}
/*************************** FMC Routines ************************************/
/**
  * @brief  Initializes FMC_BANK3 MSP.
  */
static void FMC_BANK3_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;
    
  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
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
  
  /* GPIOD configuration: GPIO_PIN_7 is  FMC_NE1 , GPIO_PIN_11 ans GPIO_PIN_12 are PSRAM_A16 and PSRAM_A17 */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |\
                              GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_7|\
                              GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |\
                              GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);

  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_10 ; 
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
}

/**
  * @brief  Initializes LCD IOs.
  */ 
static void FMC_BANK3_Init(void) 
{  
  SRAM_HandleTypeDef hsram;
  FSMC_NORSRAM_TimingTypeDef sram_timing;

  /* Initialize the SRAM controller */
  FMC_BANK3_MspInit();
  
  /*** Configure the SRAM Bank 1 ***/
  /* Configure IPs */
  hsram.Instance  = FSMC_NORSRAM_DEVICE;
  hsram.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;
  
  /* Timing config */
  sram_timing.AddressSetupTime      = 3;
  sram_timing.AddressHoldTime       = 1;
  sram_timing.DataSetupTime         = 4;
  sram_timing.BusTurnAroundDuration = 1;
  sram_timing.CLKDivision           = 2;
  sram_timing.DataLatency           = 2;
  sram_timing.AccessMode            = FSMC_ACCESS_MODE_A;
      
  hsram.Init.NSBank             = FSMC_NORSRAM_BANK3;
  hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
  hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
  hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_ENABLE;
  hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
  hsram.Init.WriteFifo          = FSMC_WRITE_FIFO_DISABLE;
  hsram.Init.PageSize           = FSMC_PAGE_SIZE_NONE;
  hsram.Init.ContinuousClock    = FSMC_CONTINUOUS_CLOCK_SYNC_ONLY;

  HAL_SRAM_Init(&hsram, &sram_timing, &sram_timing);
}

/**
  * @brief  Writes register value.
  * @param  Data: Data to be written 
  */
static void FMC_BANK3_WriteData(uint16_t Data) 
{
  /* Write 16-bit Reg */
  FMC_BANK3->RAM = Data;
  __DSB();
}

/**
  * @brief  Writes register address.
  * @param  Reg: Register to be written
  */
static void FMC_BANK3_WriteReg(uint8_t Reg) 
{
  /* Write 16-bit Index, then write register */
  FMC_BANK3->REG = Reg;
  __DSB();
}

/**
  * @brief  Reads register value.
  * @retval Read value
  */
static uint16_t FMC_BANK3_ReadData(void) 
{
  return FMC_BANK3->RAM;
}

/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initializes LCD low level.
  */
void LCD_IO_Init(void) 
{
  FMC_BANK3_Init();
}

/**
  * @brief  Writes data on LCD data register.
  * @param  RegValue: Data to be written
  */
void LCD_IO_WriteData(uint16_t RegValue) 
{
  /* Write 16-bit Reg */
  FMC_BANK3_WriteData(RegValue);
}

/**
  * @brief  Writes several data on LCD data register.
  * @param  pData: pointer on data to be written
  * @param  Size: data amount in 16bits short unit
  */
void LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size)
{
  uint32_t  i;

  for (i = 0; i < Size; i++)
  {
    FMC_BANK3_WriteData(pData[i]);
  }
}

/**
  * @brief  Writes register on LCD register.
  * @param  Reg: Register to be written
  */
void LCD_IO_WriteReg(uint8_t Reg) 
{
  /* Write 16-bit Index, then Write Reg */
  FMC_BANK3_WriteReg(Reg);
}

/**
  * @brief  Reads data from LCD data register.
  * @retval Read data.
  */
uint16_t LCD_IO_ReadData(void) 
{
  return FMC_BANK3_ReadData();
}

/**
  * @brief  LCD delay
  * @param  Delay: Delay in ms
  */
void LCD_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/********************************* LINK AUDIO *********************************/

/**
  * @brief  Initializes Audio low level.
  */
void AUDIO_IO_Init(void) 
{
  FMPI2Cx_Init(&hI2cAudioHandler);
}

/**
  * @brief  Deinitializes Audio low level.
  */
void AUDIO_IO_DeInit(void)
{
  FMPI2Cx_DeInit(&hI2cAudioHandler);
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  */
void AUDIO_IO_Write(uint8_t Addr, uint16_t Reg, uint16_t Value)
{
  uint16_t tmp = Value;
  
  Value = ((uint16_t)(tmp >> 8) & 0x00FF);
  
  Value |= ((uint16_t)(tmp << 8)& 0xFF00);
  
  FMPI2Cx_WriteMultiple(&hI2cAudioHandler, Addr, Reg, FMPI2C_MEMADD_SIZE_16BIT,(uint8_t*)&Value, 2);
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @retval Data to be read
  */
uint16_t AUDIO_IO_Read(uint8_t Addr, uint16_t Reg)
{
  uint16_t read_value = 0, tmp = 0;
  
  FMPI2Cx_ReadMultiple(&hI2cAudioHandler, Addr, Reg, FMPI2C_MEMADD_SIZE_16BIT, (uint8_t*)&read_value, 2);
  
  tmp = ((uint16_t)(read_value >> 8) & 0x00FF);
  
  tmp |= ((uint16_t)(read_value << 8)& 0xFF00);
  
  read_value = tmp;
  
  return read_value;
}

/**
  * @brief  Reads multiple data with I2C communication
  *         channel from TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
uint16_t TS_IO_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
 return FMPI2Cx_ReadMultiple(&hI2cAudioHandler, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
  * @brief  Writes multiple data with I2C communication
  *         channel from MCU to TouchScreen.
  * @param  Addr: I2C address
  * @param  Reg: Register address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  */
void TS_IO_WriteMultiple(uint8_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
{
  FMPI2Cx_WriteMultiple(&hI2cAudioHandler, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, Buffer, Length);
}

/**
  * @brief  AUDIO Codec delay 
  * @param  Delay: Delay in ms
  */
void AUDIO_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/******************************** LINK TS (TouchScreen) *****************************/

/**
  * @brief  Initializes TS low level.
  */
void TS_IO_Init(void) 
{
  GPIO_InitTypeDef  gpio_init_structure;
      
  TS_RESET_GPIO_CLK_ENABLE();

  /* Configure Button pin as input */
  gpio_init_structure.Pin = TS_RESET_PIN;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TS_RESET_GPIO_PORT, &gpio_init_structure);
  
  FMPI2Cx_Init(&hI2cAudioHandler);
}

/**
  * @brief  Deinitializes TS low level.
  */
void TS_IO_DeInit(void)
{
  FMPI2Cx_DeInit(&hI2cAudioHandler);
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @retval Data to be read
  */
uint8_t TS_IO_Read(uint8_t Addr, uint8_t Reg)
{
  uint8_t read_value = 0;

  FMPI2Cx_ReadMultiple(&hI2cAudioHandler, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1);

  return read_value;
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  */
void TS_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  FMPI2Cx_WriteMultiple(&hI2cAudioHandler, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT,(uint8_t*)&Value, 1);
}

/**
  * @brief  Delay function used in TouchScreen low level driver.
  * @param  Delay: Delay in ms
  */
void TS_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
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
