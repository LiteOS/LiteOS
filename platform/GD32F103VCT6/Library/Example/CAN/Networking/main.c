/**
  ******************************************************************************
  * @file    CAN/Networking/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.  
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "stdio.h"

/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define USE_CAN_NUM                    1          /*!< 1 or 2 */

#define KEY_GPIO_PORT                  GPIOB
#define KEY_GPIO_PIN                   GPIO_PIN_14
#define KEY_PRESSED                    0
#define KEY_NOT_PRESSED                1

#if (1 == USE_CAN_NUM) 
    #define CANX                       CAN1
    #define GPIO_CAN                   GPIOD
    #define GPIO_REMAPPING_CAN         GPIO_REMAP2_CAN1
    #define GPIO_PIN_CAN_RX            GPIO_PIN_0
    #define GPIO_PIN_CAN_TX            GPIO_PIN_1
#else /*__CAN2_USED__*/
#ifdef GD32F10X_CL    
    #define CANX                       CAN2
    #define GPIO_CAN                   GPIOB
    #define GPIO_REMAPPING_CAN         GPIO_REMAP_CAN2
    #define GPIO_PIN_CAN_RX            GPIO_PIN_5
    #define GPIO_PIN_CAN_TX            GPIO_PIN_6
#else 
    /* CAN2 is not implemented in the device */
    #error "CAN2 is implemented only in Connectivity line devices"
#endif /* GD32F10X_CL */     
#endif /* USE_CAN_NUM */

/* Private variables ---------------------------------------------------------*/
CAN_InitPara                CAN_InitStructure;
CAN_FilterInitPara    CAN_FilterInitStructure;
CanTxMessage  TxMessage;
uint8_t KeyNumber = 0x0;
extern CanRxMessage  RxMessage;
extern TypeState g_recvflag; 

/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void Init_RxMes(CanRxMessage *RxMessage);
void Delay(void);
void USART1_Config(void);
void GPIO_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{         
    /* NVIC configuration */
    NVIC_Config();
    GPIO_Config();
    USART1_Config();
    /* CAN configuration */
    CAN_Config();
    
    CAN_INTConfig(CANX, CAN_INT_RFNE0, ENABLE);
    printf("GD32F10x CAN networking test\r\n");
    printf("Press User Key button to transmit data\r\n");
    /* Transmit */
    TxMessage.StdId = 0x321;
    TxMessage.ExtId = 0x01;
    TxMessage.FT = CAN_FT_DATA;
    TxMessage.FF = CAN_FF_STANDARD;
    TxMessage.DLC = 1;
    /* Infinite loop */
    while(1)
    {
        while(GPIO_ReadInputBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == KEY_PRESSED)
        {
            if(KeyNumber == 0x10) 
            {
                KeyNumber = 0x00;
            }
            else
            {
                TxMessage.Data[0] = KeyNumber++;
                printf("Transmit data: %x\r\n", TxMessage.Data[0]);
                CAN_Transmit(CANX, &TxMessage);
                Delay();
                while(GPIO_ReadInputBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == KEY_PRESSED)
                {
                }
            }
        }
        if(SET == g_recvflag)
        {
            g_recvflag = RESET;
            printf("Recive data: %x\r\n", RxMessage.Data[0]);
        }
    }  
}

/**
  * @brief  Configure the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{  
    /* CAN register init */
    CAN_DeInit(CANX);
    CAN_StructInit(&CAN_InitStructure);

#if (1 == USE_CAN_NUM) 
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_CAN1, ENABLE);
#else /*__CAN2_USED__*/
#ifdef GD32F10X_CL 
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_CAN1, ENABLE);
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_CAN2, ENABLE);
#else
    /* CAN2 is not implemented in the device */
    #error "CAN2 is implemented only in Connectivity line devices"
#endif /* GD32F10X_CL */  
#endif /* __CAN1_USED__ */
    
    /* CAN cell init */
    CAN_InitStructure.CAN_TTC = DISABLE;
    CAN_InitStructure.CAN_ABOR = DISABLE;
    CAN_InitStructure.CAN_AWK = DISABLE;
    CAN_InitStructure.CAN_ARD = DISABLE;
    CAN_InitStructure.CAN_RFOD = DISABLE;
    CAN_InitStructure.CAN_TFO = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_MODE_NORMAL;
  
    /* CAN Baudrate = 1MBps*/
    CAN_InitStructure.CAN_SJW = CAN_SJW_1TQ;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3TQ;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5TQ;
    CAN_InitStructure.CAN_Prescaler = 4;
    CAN_Init(CANX, &CAN_InitStructure);

    /* CAN filter init */
#if (1 == USE_CAN_NUM) 
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
#ifdef GD32F10X_CL  
    CAN_FilterInitStructure.CAN_FilterNumber = 14;
#else 
    /* CAN2 is not implemented in the device */
    #error "CAN2 is implemented only in Connectivity line devices"
#endif /* GD32F10X_CL */    
#endif /* USE_CAN_NUM */
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FILTERMODE_MASK;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterInitStructure.CAN_FilterListHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterListLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskListHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskListLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssociation = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterWork = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);  
}

/**
  * @brief  Configure the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
    NVIC_InitPara NVIC_InitStructure;

    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_0);
  
#ifndef GD32F10X_CL
#if (1 == USE_CAN_NUM) 
    NVIC_InitStructure.NVIC_IRQ = USB_LP_CAN1_RX0_IRQn;
#else  /*__CAN2_USED__*/
    /* CAN2 is not implemented in the device */
    #error "CAN2 is implemented only in Connectivity line devices"
#endif /*__CAN1_USED__*/
#else
#if (1 == USE_CAN_NUM)  
    NVIC_InitStructure.NVIC_IRQ = CAN1_RX0_IRQn;
#else  /*__CAN2_USED__*/
    NVIC_InitStructure.NVIC_IRQ = CAN2_RX0_IRQn;
#endif /*__CAN1_USED__*/

#endif /* GD32F10X_CL*/
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initialize a Rx Message.
  * @param  CanRxMsg *RxMessage
  * @retval None
  */
void Init_RxMes(CanRxMessage *RxMessage)
{
    uint8_t i = 0;

    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->FF = CAN_FF_STANDARD;
    RxMessage->DLC = 0;
    RxMessage->FI = 0;
    for (i = 0;i < 8;i++)
    {
        RxMessage->Data[i] = 0x00;
    }
}

/**
  * @brief  Configure GPIO.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    GPIO_InitPara  GPIO_InitStructure;

    /* GPIO clock enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF | 
                               RCC_APB2PERIPH_GPIOA| 
                               RCC_APB2PERIPH_GPIOB| 
                               RCC_APB2PERIPH_GPIOD, ENABLE);
    GPIO_PinRemapConfig(GPIO_REMAPPING_CAN , ENABLE);
    /* Configure USART1 TX pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 RX pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure User Key Button pin */
    GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPU;
    GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CAN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(void)
{
    uint16_t nTime = 0x0000;

    for(nTime = 0; nTime <0xFFF; nTime++)
    {
    }
}

/**
  * @brief  Configure the USART1.
  * @param  None
  * @retval None
  */
void USART1_Config(void)
{
    USART_InitPara USART_InitStructure;
    /* Configure the USART1*/ 
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE);                      
    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;

    USART_Init(USART1, &USART_InitStructure);
    /* Enable the USART1 */
    USART_Enable(USART1, ENABLE);                           
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    USART_DataSend(USART1, (uint8_t) ch);
    /* Loop until the end of transmission */
    while (USART_GetBitState(USART1, USART_FLAG_TBE) == RESET)
    {
    }
    return ch;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
