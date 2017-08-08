/**
  ******************************************************************************
  * @file    PWR/Deep-sleep_demo/eval.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   None
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "eval.h"

/* Private variables ---------------------------------------------------------*/
extern  uint32_t Time_Delay = 0;
GPIO_InitPara GPIO_InitStructure;
EXTI_InitPara EXTI_InitStructure;
NVIC_InitPara NVIC_InitStructure;
      
/* Private functions ---------------------------------------------------------*/   
/**
  * @brief  Configure the LED GPIO.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
    /* Enable GPIOC clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
    
    /* Configure the LED2 and LED3 GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configure the KEY GPIO.
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{   
    /* Enable the GPIOB Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB, ENABLE);
    
    /* Configure the K4 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
}

/**
  * @brief  Configure RTC clock source and alarm.
  * @param  None
  * @retval None
  */
void RTC_AlarmConfig(void)
{ 
    /* Allow access to RTC */
    PWR_BackupAccess_Enable(ENABLE);
    
    /* Reset the Backup Domain */
    BKP_DeInit();
    
    /* Enable the LSE */
    RCC_LSEConfig(RCC_LSE_EN);
    
    /* Wait till LSE is ready */
    while(RCC_GetBitState(RCC_FLAG_LSESTB) == RESET)
    {
    }
    
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSOURCE_LSE);
    
    /* Enable the RTC Clock */
    RCC_RTCCLK_Enable(ENABLE);
    
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitRSF(); 
    
    /* Set the RTC time base to 1s */
    RTC_SetPrescaler(32767);    
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
     
    /* Enable the RTC Alarm interrupt */
    RTC_INT_Enable(RTC_INT_AF, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
    
    /* Wait till RTC Second event occurs */
    RTC_ClearBitState(RTC_FLAG_SF);
    while(RTC_GetBitState(RTC_FLAG_SF) == RESET);
    
    /* Set alarm in 3 second */
    RTC_SetAlarm(RTC_GetCounter()+ 3);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
    
    /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
    EXTI_ClearIntBitState(EXTI_LINE17);
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* NVIC configuration */   
    NVIC_InitStructure.NVIC_IRQ = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{   
    /* 1 bits for pre-emption priority and 3 bits for subpriority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);
    
    NVIC_InitStructure.NVIC_IRQ = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Insert a delay time.
  * @param  nms: the delay time length, in milliseconds.
  * @retval None
  */
void delay_ms(uint32_t nms)
{
    /* Setup SysTick Timer for 1 ms interrupts */
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
    Time_Delay = nms;
    while(Time_Delay);
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0x00;
}

/**
  * @brief  LED twinkles twice.
  * @param  GPIOx: Select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  * @note   This parameter can be GPIO_Pin_x where x can be:(0..15) for GPIOA, 
  *         GPIOB or GPIOC,(0..2) for GPIOD and(0..3) for GPIOF.
  * @retval None
  */
void LED_Spark(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{    
    GPIO_SetBits(GPIOx, GPIO_Pin);
    delay_ms(200);
    GPIO_ResetBits(GPIOx, GPIO_Pin);
    delay_ms(200);
    GPIO_SetBits(GPIOx, GPIO_Pin);
    delay_ms(200);
}

/**
  * @brief  Configure Button GPIO and EXTI Line.
  * @param  None 
  * @retval None
  */
void GPIOTOEXTI_Config(void)
{   
    /* Enable the GPIOC and AFIO Clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_AF, ENABLE);
    
    /* Configure the K3 pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Connect EXTI Line13 to PC13 pin */
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_GPIOC, GPIO_PINSOURCE13);

    /* Configure EXTI Line13 */
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
} 
    
/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
