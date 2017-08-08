/**
  ******************************************************************************
  * @file    EXMC/EXMC_NAND/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   The main function file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include <stdio.h>
#include "gd32f10x.h"
#include "exmc_nandflash.h"

/* Private define ------------------------------------------------------------*/
#define LED_GPIO1                  GPIOC
#define LED_GPIO2                  GPIOE
#define LED2_PIN                   GPIO_PIN_0
#define LED3_PIN                   GPIO_PIN_2
#define LED4_PIN                   GPIO_PIN_0
#define LED5_PIN                   GPIO_PIN_1
#define BUFFER_SIZE                0x400
#define NAND_HY_MakerID            0xAD
#define NAND_HY_DeviceID           0xF1

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
NAND_IDTypeDef NAND_ID;
uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];
__IO uint32_t WriteReadStatus = 0, status= 0;
uint32_t j = 0;
uint32_t k = 0;
uint32_t  WriteReadAddr ;
uint16_t zone, block, page, pageoffset;

/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Value);
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

    /* Config the EXMC access mode */
    NAND_Init();

    /* Read NAND ID */
    NAND_ReadID(&NAND_ID);
    printf("Nand Flash ID:0x%X 0x%X 0x%X 0x%X\n\r",NAND_ID.Maker_ID,NAND_ID.Device_ID,
                                                   NAND_ID.Third_ID,NAND_ID.Fourth_ID);

        /* Write and read nand flash */
    if((NAND_ID.Maker_ID == NAND_HY_MakerID) && (NAND_ID.Device_ID == NAND_HY_DeviceID))  
    {
        /* Set the read and write the address */
        zone = 0;
        block = 10;
        page = 0;
        pageoffset = 1100;
        WriteReadAddr = ((zone * NAND_ZONE_SIZE + block) * NAND_BLOCK_SIZE + page) 
                        * NAND_PAGE_SIZE + pageoffset;

        /* Whether address cross-border */
        if((WriteReadAddr + BUFFER_SIZE ) > NAND_MAX_ADDRESS)
        {
            printf("\n\rAddress cross-border\n\r");
            GPIO_SetBits(LED_GPIO2, LED4_PIN | LED5_PIN);
            while(1)
            {

            }
        }

        /* Fill WriteBuffer with 0x01 */
        Fill_Buffer(TxBuffer, BUFFER_SIZE , 0x01);

        /* Write data to nand flash */
        status = NAND_Write(WriteReadAddr, TxBuffer, BUFFER_SIZE);
        if(status == NAND_OK)
        {
            printf("\n\rWrite data successfully!\n\r");
        }
        else
        {
            printf("\n\rWrite data failure!n\r");
        }

        /* Read data from nand flash */
        status = NAND_Read(WriteReadAddr, RxBuffer, BUFFER_SIZE);
        if(status == NAND_OK)
        {
            printf("\n\rRead data successfully!\n\r");
        }
        else
        {
            printf("\n\rRead data failure!\n\r");
        }

        /* Read and write data comparison for equality */
        WriteReadStatus = 0;
        for(j = 0; j < BUFFER_SIZE; j++)
        {
            if(TxBuffer[j] != RxBuffer[j])
            {
                WriteReadStatus++;
                break;
            }
        }

        printf("\n\rThe result to access the nand flash:\n\r");
        if (WriteReadStatus == 0)
        { 
            printf("\n\rAccess nand flash successfully!\n\r");

            GPIO_SetBits(LED_GPIO1, LED2_PIN | LED3_PIN);
            GPIO_SetBits(LED_GPIO2, LED4_PIN | LED5_PIN);
        }
        else
        { 
            printf("\n\rAccess nand flash failure!\n\r");
            GPIO_SetBits(LED_GPIO1, LED2_PIN | LED3_PIN);
        }
        printf("\n\rPrintf data to be read:\n\r");
        printf("\n\r");
        for(k = 0; k < BUFFER_SIZE; k ++)
        {
            printf("0x%02X ",RxBuffer[k]);
        }
    }
    else
    {
        printf("\n\rRead NAND ID failure!\n\r");
    }

    while(1)
    {

    }
}

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

    GPIO_InitStructure.GPIO_Pin = LED2_PIN | LED3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(LED_GPIO1,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED4_PIN | LED5_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(LED_GPIO2,&GPIO_InitStructure);

    GPIO_ResetBits(LED_GPIO1, LED2_PIN | LED3_PIN);
    GPIO_ResetBits(LED_GPIO2, LED4_PIN | LED5_PIN);
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
    while (USART_GetBitState( USART1 , USART_FLAG_TBE) == RESET)
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
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Value)
{
    uint16_t IndexTmp = 0;

    /* Put in global buffer same values */
    for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
    {
        pBuffer[IndexTmp] = Value;
    }
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
