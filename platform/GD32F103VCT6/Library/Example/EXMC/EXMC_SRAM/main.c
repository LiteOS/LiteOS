/**
  ******************************************************************************
  * @file    EXMC/EXMC_SRAM/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "EXMC_SRAM.h"

/* Private typedef -----------------------------------------------------------*/
#define BUFFER_SIZE        0x4              /*!< Write or read buffer size */
#define WRITE_READ_ADDR    0x0000           /*!< SRAM write or read address */

/* Private variables ---------------------------------------------------------*/
uint16_t TxBuffer[BUFFER_SIZE];
uint16_t RxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
void LED_Config(void);
void Fill_Buffer_16(uint16_t *pBuffer, uint16_t BufferLenght, uint16_t Offset);

/* Main program --------------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    uint16_t i = 0;
    uint16_t WriteReadStatus=0;

    /* Config LED */
    LED_Config();

    /* Config EXMC_bank1_NORSRAM1 and GPIOs to interface with the SRAM */
    EXMC_SRAM_Init();

    /* Fill TxBuffer */
    Fill_Buffer_16(TxBuffer, BUFFER_SIZE, 0x1213);

    /* Write data to SRAM  */
    EXMC_SRAM_WriteBuffer_16(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    
    /* Read data from SRAM */
    EXMC_SRAM_ReadBuffer_16(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    
    /* Compare two buffers */
    for (i = 0x00; i < BUFFER_SIZE; i++)
    {
        if (RxBuffer[i] != TxBuffer[i])
        {
            WriteReadStatus ++;
            break;
        }
    }
    if (WriteReadStatus == 0)
    {
        /* RxBuffer and TxBuffer are equal */
        /* Turn on LED2 and LED3 */
        GPIO_SetBits(GPIOC, GPIO_PIN_6|GPIO_PIN_7);
    }
    else
    {
        /* ERROR,RxBuffer and TxBuffer are different */
        /* Turn on all LEDs */
        GPIO_SetBits(GPIOC,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    }
    while (1)
    {
    }
}

/**
  * @brief  Fill the 16-bit buffer with specified value.
  * @param  pBuffer: Pointer on the Buffer to fill
  * @param  BufferSize: Size of the buffer to fill
  * @param  Offset: First value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer_16(uint16_t *pBuffer, uint16_t BufferLenght, uint16_t Offset)
{
    uint16_t IndexTmp = 0;
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
    {
        pBuffer[IndexTmp] = IndexTmp + Offset;
    }
}

/**
  * @brief  LED configuration.
  * @param  None
  * @retval None
  */
void LED_Config(void)
{
    GPIO_InitPara GPIO_InitStructure;
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
    /* LED */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOC,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    GPIO_ResetBits(GPIOC,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
