/**
  ******************************************************************************
  * @file    CRC/CRC_demo/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private variables ---------------------------------------------------------*/
uint32_t vab1 = 0, success_flag = 0;
uint32_t read32_1;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 
    GPIO_InitPara GPIO_InitStructure;

    vab1 = (uint32_t)0xabcd1234;

    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_CRC, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);

    /* Configure PC0 (LED2) as out pp */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    GPIO_ResetBits(GPIOC,GPIO_PIN_0);

    CRC_ResetDTR();
    read32_1 = CRC_CalcSingleData(vab1);

    /* Check the caculation result */
    if(read32_1 == 0xf7018a40)
    {
        success_flag = 0x1;
        GPIO_SetBits(GPIOC,GPIO_PIN_0);  
    }

    while (1)
    {
    }
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
