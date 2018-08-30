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
#include "hal_sd.h"
/* Defines ------------------------------------------------------------------*/
#ifdef HAL_SD_MODULE_ENABLED

#define SD_DETECT_PIN                    GPIO_PIN_11
#define SD_DETECT_GPIO_PORT              GPIOF
#define __SD_DETECT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define SD_DETECT_IRQn                   EXTI15_10_IRQn

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;

/* Private function prototypes ----------------------------------------------*/

void BSP_SD_MspInit(void *Params)
{
    GPIO_InitTypeDef gpioinitstruct = {0};

    /* Enable SDIO clock */
    __HAL_RCC_SDIO_CLK_ENABLE();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __SD_DETECT_GPIO_CLK_ENABLE();

    /* Common GPIO configuration */
    gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
    gpioinitstruct.Pull      = GPIO_PULLUP;
    gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* GPIOC configuration */
    gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;

    HAL_GPIO_Init(GPIOC, &gpioinitstruct);

    /* GPIOD configuration */
    gpioinitstruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &gpioinitstruct);

    /* SD Card detect pin configuration */
    gpioinitstruct.Mode      = GPIO_MODE_INPUT;
    gpioinitstruct.Pull      = GPIO_PULLUP;
    gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpioinitstruct.Pin       = SD_DETECT_PIN;
    HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpioinitstruct);
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_SD_IsDetected(void)
{
    __IO uint8_t status = SD_PRESENT;

    /* Check SD card detect pin */
    if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET) 
    {
        status = SD_NOT_PRESENT;
    }

    return status;
}

static void hal_sd_show_info(void)
{
    HAL_SD_CardCIDTypeDef cid;
    HAL_SD_GetCardCID(&hsd, &cid);
    
    printf("\r\nCard Version:");
    switch(hsd.SdCard.CardVersion)
	{
		case CARD_V1_X: printf("        CARD_V1_X\r\n");break;
		case CARD_V2_X: printf("        CARD_V2_X\r\n");break;
	}
    
    printf("Card Type:");
    switch(hsd.SdCard.CardType)
	{
		case CARD_SDSC:         printf("           CARD_SDSC\r\n");break;
		case CARD_SDHC_SDXC:    printf("           CARD_SDHC_SDXC\r\n");break;
		case CARD_SECURED:      printf("           CARD_SECURED\r\n");break;
	}	
  	printf("Card ManufacturerID: %d\r\n",cid.ManufacturerID);	
 	printf("Card RCA:            0x%08lX\r\n",hsd.SdCard.RelCardAdd);
	printf("Card Capacity:       %ld MB\r\n",(uint32_t)(((uint64_t)hsd.SdCard.BlockNbr*hsd.SdCard.BlockSize)>>20));
 	printf("Card BlockSize:      %ld B\r\n",hsd.SdCard.BlockSize);
    printf("DeviceSize:          %ld B\r\n\r\n",hsd.SdCard.BlockSize);
}

/* Public functions ---------------------------------------------------------*/
uint8_t hal_sd_init(void)
{
    uint8_t state = MSD_OK;

    /* uSD device interface configuration */
    hsd.Instance = SDIO;
    
    hsd.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide             = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv            = 7;//SDIO_TRANSFER_CLK_DIV;

    /* Check if the SD card is plugged in the slot */
    if(BSP_SD_IsDetected() != SD_PRESENT)
    {
        return MSD_ERROR;
    }

    /* Msp SD initialization */
    BSP_SD_MspInit(NULL);

    if(HAL_SD_Init(&hsd) != HAL_OK)
    {
        state = MSD_ERROR;
    }
#if 0 // has problem using 4-bit mode
    /* Configure SD Bus width */
    if(state == MSD_OK)
    {
        /* Enable wide operation */
        if(HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
        {
            state = MSD_ERROR;
        }
        else
        {
            state = MSD_OK;
        }
    }
#endif    
    hal_sd_show_info();
    return  state;
}


uint8_t hal_sd_read_blocks(uint32_t *buff, uint32_t count, uint32_t addr, uint32_t timeout)
{
    if(HAL_SD_ReadBlocks(&hsd, (uint8_t *)buff, addr, count, timeout) != HAL_OK)
    {
        return MSD_ERROR;
    }
    return MSD_OK;
}
uint8_t hal_sd_write_blocks(uint32_t *buff, uint32_t count, uint32_t addr, uint32_t timeout)
{
    if(HAL_SD_WriteBlocks(&hsd, (uint8_t *)buff, addr, count, timeout) != HAL_OK)
    {
        return MSD_ERROR;
    }
    return MSD_OK;
}
uint8_t hal_sd_erase(uint32_t start_addr, uint32_t end_addr)
{
    if(HAL_SD_Erase(&hsd, start_addr, end_addr) != HAL_OK)
    {
        return MSD_ERROR;
    }
    return MSD_OK;
}
uint8_t hal_sd_get_card_state(void)
{
    return((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

void hal_sd_get_card_info(HAL_SD_CardInfoTypeDef *info)
{
    /* Get SD card Information */
    HAL_SD_GetCardInfo(&hsd, info);
}

/* Private functions --------------------------------------------------------*/

#endif /* HAL_SD_MODULE_ENABLED */
