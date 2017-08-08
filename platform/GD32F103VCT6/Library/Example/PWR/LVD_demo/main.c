/**
  ******************************************************************************
  * @file    PWR/LVD_demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Main program body..
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void EXTI_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Enable GPIOC clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
    
    /* Configure the LED2 GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Turn on LED2 */
    GPIO_SetBits(GPIOC, GPIO_PIN_0);
    
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);
    
    /* Configure the EXTI line16 */
    EXTI_Configuration();
    /* NVIC configuration */
    NVIC_Configuration();
    
    /* Configure the LVD threshold to 2.9V, and enable the LVD */
    PWR_LVDConfig(PWR_LVDT_7, ENABLE);
    
    while(1)
    {
    }
}

/**
  * @brief  Configure NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{    
    NVIC_InitPara NVIC_InitStructure;
    /* 1 bits for pre-emption priority and 3 bits for subpriority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);
    
    NVIC_InitStructure.NVIC_IRQ = LVD_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure the selected EXTI line.
  * @param  None
  * @retval None
  */
void EXTI_Configuration(void)
{
    EXTI_InitPara EXTI_InitStructure;
    
    /* Configure EXTI Line16(LVD Output) to generate an interrupt on rising and falling edges */
    EXTI_ClearIntBitState(EXTI_LINE16);
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
