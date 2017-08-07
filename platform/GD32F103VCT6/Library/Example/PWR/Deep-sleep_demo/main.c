/**
  ******************************************************************************
  * @file    PWR/Deep-sleep_demo/main.c
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
    /* Initialize LEDs and Key Buttons mounted on GD32107C-EVAL board */
    LED_Init();
    KEY_Init();
    
    /* Configure the GPIO to EXTI_line of Key Button */     
    GPIOTOEXTI_Config();
    
    /* NVIC configuration */
    NVIC_Configuration();
    /* LED2 twinkles twice */
    LED_Spark(GPIOC, GPIO_PIN_0);
 
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);    
    
    /* Configure the RTC alarm */
    RTC_AlarmConfig();

    /* Turn off LED2 */
    GPIO_ResetBits(GPIOC, GPIO_PIN_0);

    /*Enter Deep-sleep mode*/
    PWR_DEEPSLEEPMode_Entry(PWR_LDO_LOWPOWER, PWR_DEEPSLEEPENTRY_WFI);
    
    /* Reinitialize the system clock after wake up from deep-sleep mode */
    SystemInit();
   
    /* LED3 twinkles twice */
    LED_Spark(GPIOC, GPIO_PIN_2);
    
    while (1)
    {
        if(GPIO_ReadInputBit(GPIOB, GPIO_PIN_14) == 0)
        {
            /* Elimitate jitter */
            delay_ms(5);
            
            if(GPIO_ReadInputBit(GPIOB, GPIO_PIN_14) == 0)
            {
                /* Request to enter Deep-sleep mode with LDO in low power mode*/
                PWR_DEEPSLEEPMode_Entry(PWR_LDO_LOWPOWER, PWR_DEEPSLEEPENTRY_WFI);
                LED_Spark(GPIOC, GPIO_PIN_2);
            }
            else
            {
                LED_Spark(GPIOC, GPIO_PIN_0);
            }
        }
        else
        {
            LED_Spark(GPIOC, GPIO_PIN_0);
        }
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
