/**
  ******************************************************************************
  * @file    TIMER/TIMER3_InputCapture/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <gd32f10x.h>
#include "systick.h"
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);
int fputc(int ch, FILE *f);
void USART1_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    USART_DataSend( USART1, (uint8_t) ch );

    /* Loop until transmit data register is empty */
    while (USART_GetBitState ( USART1 , USART_FLAG_TC ) == RESET);

    return ch;
}

/**
  * @brief  Configure the USART peripheral.
  * @param  None
  * @retval None
  */
void USART1_Configuration(void)
{
    GPIO_InitPara  GPIO_InitStructure;
    USART_InitPara USART_InitStructure;

    /* Enable the USART1 clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_USART1 , ENABLE );

    /* Configure PA9 (USART1_Tx) as push-pull */
    GPIO_DeInit( GPIOA );
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init( GPIOA , &GPIO_InitStructure); 

    /* Configure PA10 (USART1_Rx) as input floating */ 
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init( GPIOA , &GPIO_InitStructure);

    /* USART1  configuration */      
    USART_DeInit( USART1 );
    USART_InitStructure.USART_BRR                 = 115200;
    USART_InitStructure.USART_WL                  = USART_WL_8B;
    USART_InitStructure.USART_STBits              = USART_STBITS_1;
    USART_InitStructure.USART_Parity              = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx              = USART_RXORTX_RX | USART_RXORTX_TX;
    USART_Init(USART1, &USART_InitStructure);

    /* USART1 enable */
    USART_Enable(USART1, ENABLE);
}

/**
  * @brief  Configure system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
   /* GPIOA, GPIOC and AF clocks enable */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOC |
                                RCC_APB2PERIPH_AF, ENABLE);
}

/**
  * @brief  Configure the GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Configure PC0 (LED2) as PP output */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    /* Configure PA6(TIMER3 CH1) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* Enable the TIMER3 Interrupt */
    NVIC_InitStructure.NVIC_IRQ                = TIMER3_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority     = 1;
    NVIC_InitStructure.NVIC_IRQEnable          = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  Configure the TIMER peripheral.
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    /* TIMER3 configuration: Input Capture mode -------------------
    The external signal is connected to TIMER3 CH1 pin(PA6)
    The Rising edge is used as active edge
    The TIMER3 CHCC1 is used to compute the frequency value
    ------------------------------------------------------------ */

    TIMER_BaseInitPara TIM_TimeBaseStructure;
    TIMER_ICInitPara   TIM_ICInitStructure;;

    /* TIMER3 clock enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER3,ENABLE);

    /* TIMER3  configuration */
    TIMER_DeInit(TIMER3);
    TIM_TimeBaseStructure.TIMER_Prescaler     = 53999;
    TIM_TimeBaseStructure.TIMER_CounterMode   = TIMER_COUNTER_UP;
    TIM_TimeBaseStructure.TIMER_Period        = 65535;
    TIM_TimeBaseStructure.TIMER_ClockDivision = TIMER_CDIV_DIV1;
    TIMER_BaseInit(TIMER3,&TIM_TimeBaseStructure);

    /* Input capture configuration */
    TIM_ICInitStructure.TIMER_CH          = TIMER_CH_1;
    TIM_ICInitStructure.TIMER_ICPolarity  = TIMER_IC_POLARITY_RISING;
    TIM_ICInitStructure.TIMER_ICSelection = TIMER_IC_SELECTION_DIRECTTI;
    TIM_ICInitStructure.TIMER_ICPrescaler = TIMER_IC_PSC_DIV1;
    TIM_ICInitStructure.TIMER_ICFilter    = 0x0;
    TIMER_ICInit(TIMER3, &TIM_ICInitStructure);

    /* Auto-reload preload enable */
    TIMER_CARLPreloadConfig(TIMER3,ENABLE);

    /* Clear the CH1 Interrupt flag an Enable the CH1 Interrupt Request */
    TIMER_ClearIntBitState(TIMER3,TIMER_INT_CH1);
    TIMER_INTConfig(TIMER3,TIMER_INT_CH1,ENABLE);

    /* TIMER enable counter*/
    TIMER_Enable( TIMER3, ENABLE );
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main()
{
    RCC_Configuration();
    GPIO_Configuration();
    SysTick_Configuration();
    NVIC_Configuration();
    USART1_Configuration();
    TIM_Configuration();

    while(1)
    {
        GPIO_SetBits(GPIOC,GPIO_PIN_0);
        Delay_1ms(1000);
        GPIO_ResetBits(GPIOC, GPIO_PIN_0);
        Delay_1ms(1000);
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
