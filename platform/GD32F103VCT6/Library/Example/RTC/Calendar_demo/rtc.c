/**
  ******************************************************************************
  * @file    RTC/Calendar_demo/rtc.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   RTC_CheckAndConfig,Time_Show and Time_Adjust.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* Private variables ---------------------------------------------------------*/

/* Enter the second interruption,set the Second interrupt flag to 1 */
__IO uint32_t TimeDisplay;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  USART1 configuration.115200 8-N-1.
  * @param  None
  * @retval None
  */
void USART1_Config(void)
{
    GPIO_InitPara GPIO_InitStructure;
    USART_InitPara USART_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure); 
    USART_Enable(USART1, ENABLE);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQ = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 1;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccess_Enable(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_EN);
    /* Wait till LSE is ready */
    while (RCC_GetBitState(RCC_FLAG_LSESTB) == RESET)
    {}
    
    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSOURCE_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLK_Enable(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitRSF();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();

    /* Enable the RTC Second */
    RTC_INT_Enable(RTC_INT_SF, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
}

/**
  * @brief  Return the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
uint32_t Time_Regulate(void)
{
    uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

    printf("\r\n==============Time Settings=====================================");
    printf("\r\n  Please Set Hours");

    while (Tmp_HH == 0xFF)
    {
        Tmp_HH = USART_Scanf(23);
    }
    printf(":  %d", Tmp_HH);
    printf("\r\n  Please Set Minutes");
    while (Tmp_MM == 0xFF)
    {
        Tmp_MM = USART_Scanf(59);
    }
    printf(":  %d", Tmp_MM);
    printf("\r\n  Please Set Seconds");
    while (Tmp_SS == 0xFF)
    {
        Tmp_SS = USART_Scanf(59);
    }
    printf(":  %d", Tmp_SS);

    /* Return the value to store in RTC counter register */
    return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

/**
  * @brief  Adjust time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
    /* Change the current time */
    RTC_SetCounter(Time_Regulate());
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitLWOFF();
}

/**
  * @brief  Display the current time.
  * @param  TimeVar: RTC counter value
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
    uint32_t THH = 0, TMM = 0, TSS = 0;

    /* Compute  hours */
    THH = TimeVar / 3600;
    /* Compute minutes */
    TMM = (TimeVar % 3600) / 60;
    /* Compute seconds */
    TSS = (TimeVar % 3600) % 60;

    printf(" Time: %0.2d:%0.2d:%0.2d\r\n", THH, TMM, TSS);
}

/**
  * @brief  Show the current time (HH:MM:SS) on the Hyperterminal.
  * @param  None
  * @retval None
  */
void Time_Show(void)
{
    printf("\n\r");

    /* Infinite loop */
    while (1)
    {
        /* If 1s has paased */
        if (TimeDisplay == 1)
        {
            /* Display current time */
            Time_Display(RTC_GetCounter());
            TimeDisplay = 0;
        }
    }
}

/**
  * @brief  Get numeric values from the hyperterminal.
  * @param  value: Input value from the hyperterminal.
  * @retval None
  */
uint8_t USART_Scanf(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while (index < 2)
    {
        /* Loop until RXNE = 1 */
        while (USART_GetBitState(USART1, USART_FLAG_RBNE) == RESET)
        {}
        tmp[index++] = (USART_DataReceive(USART1));

        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
        {
            printf("\n\rPlease enter valid number between 0 and 9\n");
            index--;
        }
    }
    /* Calculate the Corresponding value */
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    /* Check */
    if (index > value)
    {
        printf("\n\rPlease enter valid number between 0 and %d\n", value);
        return 0xFF;
    }
    return index;
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
