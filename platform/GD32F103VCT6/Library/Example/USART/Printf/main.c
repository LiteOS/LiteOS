/**
  ******************************************************************************
  * @file    USART/Printf/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.  
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Enable GPIOA clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_GPIOA , ENABLE );
    
    /* Enable USART1 APB clock */
    RCC_APB2PeriphClock_Enable( RCC_APB2PERIPH_USART1 , ENABLE );
    
    /* USART1 Pins configuration **************************************************/
    GPIO_DeInit( GPIOA );
    {
        /* Configure the GPIO ports */
        GPIO_InitPara GPIO_InitStructure;
       
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_9 ;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_SPEED_50MHZ;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
        GPIO_InitStructure.GPIO_Pin     = GPIO_PIN_10;
        GPIO_InitStructure.GPIO_Mode    = GPIO_MODE_IN_FLOATING;;
        GPIO_Init( GPIOA , &GPIO_InitStructure); 
    }

    { 
        USART_InitPara USART_InitStructure;
        
        USART_DeInit( USART1 );
        USART_InitStructure.USART_BRR           = 115200;
        USART_InitStructure.USART_WL            = USART_WL_8B;
        USART_InitStructure.USART_STBits            = USART_STBITS_1;
        USART_InitStructure.USART_Parity                = USART_PARITY_RESET;
        USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
        USART_InitStructure.USART_RxorTx                = USART_RXORTX_RX | USART_RXORTX_TX;
        USART_Init(USART1, &USART_InitStructure);
    }

    /* USART enable */
    USART_Enable(USART1, ENABLE);
    
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
    
    /* Loop until the end of transmission */
    /* The software must wait until TC=1. The TC flag remains cleared during all data
     transfers and it is set by hardware at the last frame end of transmission*/
    while ( USART_GetBitState( USART1 , USART_FLAG_TC ) == RESET )
    {
    }
    while (1)
    {
    }
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Loop until transmit data register is empty */
    while (USART_GetBitState( USART1 , USART_FLAG_TBE) == RESET)
    {
    }
    /* Place your implementation of fputc here */
    USART_DataSend( USART1 , (uint8_t) ch );
    return ch;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
