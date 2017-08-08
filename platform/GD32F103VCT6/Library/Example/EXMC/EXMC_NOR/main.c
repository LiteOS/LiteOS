/**
  ******************************************************************************
  * @file    EXMC/EXMC_NOR/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include <stdio.h>
#include "gd32f10x.h"
#include "exmc_norflash.h"

/* Private define ------------------------------------------------------------*/
#define LED_GPIO                  GPIOC
#define LED2_PIN                  GPIO_PIN_6
#define LED3_PIN                  GPIO_PIN_7
#define LED4_PIN                  GPIO_PIN_8
#define LED5_PIN                  GPIO_PIN_9
#define BUFFER_SIZE               0x400
#define WRITE_READ_ADDR           0x8000

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
uint16_t WriteBuffer[BUFFER_SIZE];
uint16_t ReadBuffer[BUFFER_SIZE];
uint32_t WriteReadStatus = 0, Index = 0;
NOR_IDTypeDef NOR_ID;
NOR_Status Status;

/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Value);
void RCC_Configuration(void);
void LED_Configuration(void);
void USART1_Configuration(void);
  
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Periph clock enable */
    RCC_Configuration();

    /* Config the LED GPIO */
    LED_Configuration();

    /* Config the USART1 */
    USART1_Configuration();

    printf("NOR Flash Init!\n\r");

    /* EXMC nor flash init */
    EXMC_NorFlash_Init();

    /* Read Nor Flash ID and printf */
    NOR_ReadID(&NOR_ID);
    printf("\n\rNor Flash ID:0x%X 0x%X 0x%X 0x%X\n\r",NOR_ID.Manufacturer_Code,NOR_ID.Device_Code,
                                                      NOR_ID.Block_Protection_Indicator,NOR_ID.Block_Protection_Status);

    NOR_ReturnToReadMode();

    /* Erase the nor flash block to be written data */
    Status = NOR_EraseBlock(WRITE_READ_ADDR);
    if(NOR_SUCCESS == Status)
    {
        printf("\n\rErase nor flash block successfully!\n\r");
    }
    else
    {
        printf("\n\rErase nor flash block failure!\n\r");
    }

    /* Whether address cross-border */
    if((WRITE_READ_ADDR + BUFFER_SIZE ) > NOR_MAX_ADDRESS)
    {
        printf("\n\rAddress cross-border\n\r");
        GPIO_SetBits(LED_GPIO, LED4_PIN | LED5_PIN);
        while(1)
        {

        }
    }

    /* Fill WriteBuffer with the specified value */
    Fill_Buffer(WriteBuffer, BUFFER_SIZE, 0x1234);

    /* Write data to nor flash, WRITE_READ_ADDR: the starting address of the write data */
    Status = NOR_WriteBuffer(WriteBuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    if(NOR_SUCCESS == Status)
    {
        printf("\n\rWrite data to nor flash block successfully!\n\r");
    }
    else
    {
        printf("\n\rWrite data to nor flash block failure!\n\r");
    }

    /* Read data from nor flash, WRITE_READ_ADDR: the starting address of the read data*/
    NOR_ReadBuffer(ReadBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

    /* Read and write data comparison for equality */
    WriteReadStatus = 0;
    for (Index = 0x00; Index < BUFFER_SIZE; Index++)
    {
        if (ReadBuffer[Index] != WriteBuffer[Index])
        {
            WriteReadStatus++;
            break;
        }
    }

    printf("\n\rThe result to access the nor flash:\n\r");
    if (WriteReadStatus == 0)
    {
         printf("\n\rAccess nor flash successfully!\n\r");
         GPIO_SetBits(LED_GPIO, LED2_PIN | LED3_PIN | LED4_PIN | LED5_PIN);
    }
    else
    { 
         printf("\n\rAccess nor flash failure!\n\r"); 
         GPIO_SetBits(LED_GPIO, LED2_PIN | LED3_PIN);
    }

    printf("\n\rPrintf data to be read: \n\r");
    printf("\n\r");
    for(Index = 0; Index < BUFFER_SIZE; Index++)
    {
        printf("%X ",ReadBuffer[Index]);
    }

    while(1)
    {

    }
}

/* Private functions ---------------------------------------------------------*/
/** 
  * @brief  Configure RCC Clock.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOD , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF    , ENABLE);

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE);
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);
}

/**
  * @brief  Configure LED GPIO.
  * @param  None
  * @retval None
  */
void LED_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = LED2_PIN | LED3_PIN | LED4_PIN | LED5_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_ResetBits(LED_GPIO, LED2_PIN | LED3_PIN | LED4_PIN | LED5_PIN);
}

/**
  * @brief  Configure USART1.
  * @param  None
  * @retval None
  */
void USART1_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    USART_InitPara USART_InitStructure;

    /* USART1 GPIO Configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 


    /* Configure the USART1*/ 
    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;

    USART_Init(USART1, &USART_InitStructure);
    USART_Enable(USART1, ENABLE);
}

/**
  * @brief  Retarget the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_DataSend( USART1 , (uint8_t) ch );

    /* Loop until transmit data register is empty */
    while (USART_GetBitState(USART1 , USART_FLAG_TBE) == RESET)
    {
    }

    return ch;
}

/**
  * @brief  Fill the buffer with specified value.
  * @param  pBuffer: Pointer on the Buffer to fill
  * @param  BufferSize: Size of the buffer to fill
  * @param  value: Value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Value)
{
    uint16_t IndexTmp = 0;

    /* Put in global buffer same values */
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
    {
      pBuffer[IndexTmp] = Value;
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
