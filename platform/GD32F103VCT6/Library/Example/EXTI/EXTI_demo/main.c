/**
  ******************************************************************************
  * @file    EXTI/EXTI_demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Main program body..
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private variables ---------------------------------------------------------*/
GPIO_InitPara GPIO_InitStructure;
EXTI_InitPara EXTI_InitStructure;
NVIC_InitPara NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void EXTI0_Config(void);
void EXTI14_Config(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Enable GPIOC clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
    
    /* Configure the LED2 and LED3 GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Turn on LED2 and LED3 */
    GPIO_SetBits(GPIOC, GPIO_PIN_0);
    GPIO_SetBits(GPIOC, GPIO_PIN_2);
    
    /* Configure the EXTI line0 and EXTI line14 */
    EXTI_DeInit(&EXTI_InitStructure);
    EXTI0_Config();
    EXTI14_Config();
    
    EXTI_SWINT_Enable(EXTI_LINE0);
    
    while(1)
    {
    }                                    
}

/**
  * @brief  Configure the EXTI line0.
  * @param  None
  * @retval None
  */
void EXTI0_Config(void)
{
    /* Enable GPIOC and AFIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA |
                               RCC_APB2PERIPH_AF, ENABLE);

    /* Configure PA0 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Connect EXTI Line0 to PA0 pin */
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_GPIOA, GPIO_PINSOURCE0);

    /* Configure EXTI Line0 and its interrupt to the lowest priority*/
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQ = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure the EXTI line14.
  * @param  None
  * @retval None
  */
void EXTI14_Config(void)
{
    /* Enable GPIOB and AFIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB |
                               RCC_APB2PERIPH_AF, ENABLE);
   
    /* Configure PB14 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect EXTI Line14 to PB14 pin */
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_GPIOB, GPIO_PINSOURCE14);

    /* Configure EXTI Line14 and its interrupt to the lowest priority*/
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQ = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
