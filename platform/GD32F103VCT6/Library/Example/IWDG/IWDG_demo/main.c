/**
  ******************************************************************************
  * @file    IWDG/IWDG-Demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private variables ---------------------------------------------------------*/
volatile uint32_t g_timedelay=0;
TypeState HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void Delay(uint32_t nCount);
void Delay_ms(uint32_t time);

/* Private function ----------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Enable GPIOC clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC,ENABLE);

    /* GPIO configuration */
    GPIO_Configuration();

    if (SysTick_Config(SystemCoreClock/1000))
    {
        /* Capture error */
        while (1)
        {}
    }
    Delay_ms(250);

    /* EXTI configuration */
    EXTI_Configuration();

    /* NVIC configuration */
    NVIC_Configuration();

    /* Enable write access to IWDG_PSR and IWDG_RLDR registers */
    IWDG_Write_Enable(IWDG_WRITEACCESS_ENABLE);

    /* IWDG counter clock: 40KHz(LSI) / 64 = 0.625 KHz */
    IWDG_SetPrescaler(IWDG_PRESCALER_64);

    /* Set counter reload value to 625 */
    IWDG_SetReloadValue(625);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

    /* Check if the system has resumed from IWDG reset */
    if (RCC_GetBitState(RCC_FLAG_IWDGRST) != RESET)
    {
        /* Set GPIOC pin 0 */
        GPIO_SetBits(GPIOC, GPIO_PIN_0);

        /* Clear reset flags */
        RCC_ClearBitState();

        while(1);
    }
    else
    {
        /* Reset GPIOC pin 0*/
        GPIO_ResetBits(GPIOC, GPIO_PIN_0);
    }

    while (1)
    {}
}

/**
  * @brief  Configure the different GPIO ports
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Configure GPIOC pin 0 and pin 2 as Output push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure GPIOC pin 13 as input floating (Key Button EXTI Line) */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configure EXTI Line9
  * @param  None
  * @retval None
  */
void EXTI_Configuration(void)
{
    EXTI_InitPara EXTI_InitStructure;

    RCC_APB2PeriphClock_Enable(RCC_APB2CCR_PCEN,ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2CCR_AFEN,ENABLE);

    /* Connect Key Button EXTI Line to  GPIOC Pin 13 */
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_GPIOC,GPIO_PINSOURCE13);

    /* Configure Key Button EXTI Line to generate an interrupt on falling edge */
    EXTI_ClearIntBitState(EXTI_LINE13);
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  Configure the nested vectored interrupt controller
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;
 
    /* Configure one bit for preemption priority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* Enable the EXTI4_15 Interrupt */
    NVIC_InitStructure.NVIC_IRQ = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

/**
  * @brief  Insert a delay time
  * @param  None
  * @retval None
  */
void Delay_ms(uint32_t time)
{
    g_timedelay = time;
    while(g_timedelay);
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
