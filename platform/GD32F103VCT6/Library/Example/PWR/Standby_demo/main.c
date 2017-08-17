/**
  ******************************************************************************
  * @file    PWR/Standby_demo/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Main program body.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "eval.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{     
    /* Initialize LEDs mounted on GD32107C-EVAL board */
    LED_Init();
    
    /* Configure the GPIO to EXTI_line of Key Button */     
    GPIOTOEXTI_Config();
    
    /* NVIC configuration */
    NVIC_Configuration();
    
    /* LED2 twinkles twice */
    LED_Spark(GPIOC, GPIO_PIN_0);
 
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);    
        
    /* Enable WKUP pin */
    PWR_WKUP_Pin_Enable(ENABLE);
    
    /* Configure the RTC alarm */
    RTC_Configuration();
    
    /* Turn off LED2 */
    GPIO_ResetBits(GPIOC, GPIO_PIN_0);
    
    while (1)
    {
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
