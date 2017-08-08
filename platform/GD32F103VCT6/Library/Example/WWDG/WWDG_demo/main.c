/**
  ******************************************************************************
  * @file    WWDG/WWDG_demo/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;

/* Private function prototypes -----------------------------------------------*/
extern void Delay(uint32_t nTime);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Toggle the selected LED.
  * @param  Led: Specify the Led to be toggled. 
  *         This parameter can be one of following parameters:
  * @retval None
  */
void LEDToggle( void )
{
    GPIOC->DOR ^= GPIO_PIN_2 ;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    {
        GPIO_InitPara  GPIO_InitStructure;

        /* Enable the GPIOC Clock */
        RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC,ENABLE);

        /* Configure the GPIOC pin 0 and pin 2 */
        GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
        GPIO_Init(GPIOC,&GPIO_InitStructure);

        /* Turn on LED */
        GPIOC->BOR = GPIO_PIN_0 | GPIO_PIN_2;
    }

    /* Check if the system has resumed from WWDG reset */
    if (RCC_GetBitState(RCC_FLAG_WWDGRST) != RESET)
    {
        /* WWDGRST flag set */
        /* Turn on LED */
        GPIOC->BOR = GPIO_PIN_0;

        /* Clear reset flags */
        RCC_ClearBitState();

        while(1);
    }
    else
    {
        /* WWDGRST flag is not set */
        /* Turn off LED */
        GPIOC->BCR = GPIO_PIN_0;
    }

    /* Setup SysTick Time for 1 msec interrupts */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1)
        {}
    }

    /* WWDG configuration */
    /* Enable WWDG clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_WWDG , ENABLE);

    /* WWDG clock counter = (PCLK1 (108MHz)/2/4096)/8 = 1650Hz (~606 us) */
    WWDG_SetPrescalerValue(WWDG_PRESCALER_8);

    /* Set Window value to 80; WWDG counter should be refreshed only when the counter
    is below 80 (and greater than 64) otherwise a reset will be generated */
    WWDG_SetWindowValue(80);

    /* Enable WWDG and set counter value to 127, WWDG timeout = ~606 us * 64 = 38.9 ms
     In this case the refresh window is: ~606 * (127-80)= 28.5ms < refresh window < ~606 * 64 =38.9ms */
    WWDG_Enable(127);

    while (1)
    {
        /* Toggle LED */
        LEDToggle();

        /* Insert 25 ms delay */
        Delay(30);

        /* Update WWDG counter */
        WWDG_SetCounterValue(127);
    }
}

/**
  * @brief  Insert a delay time.
  * @param  nTime: specify the delay time length, in 10 ms.
  * @retval None
  */
void Delay(uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
