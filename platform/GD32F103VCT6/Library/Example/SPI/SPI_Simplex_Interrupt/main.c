/**
  ******************************************************************************
  * @file    SPI/SPI_Simplex_Interrupt/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "gd32f10x_it.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define BufferSize 32

/* Private variables ---------------------------------------------------------*/
SPI_InitPara SPI_InitStructure;
uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
                                            0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                            0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                            0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                            0x1F, 0x20};
uint8_t SPI_SLAVE_Buffer_Rx[BufferSize];
__IO uint8_t TxIdx = 0, RxIdx = 0;
volatile TestStatus TransferStatus = FAILED;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /* System clocks configuration */
    RCC_Configuration();
    /* NVIC configuration */
    NVIC_Configuration();
    /* GPIO configuration */
    GPIO_Configuration();

    /* SPI_MASTER configuration */
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_BDMTX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
    SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_LOW;
    SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_2EDGE;
    SPI_InitStructure.SPI_SWNSSEN = SPI_SWNSS_SOFT;
    SPI_InitStructure.SPI_PSC = SPI_PSC_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CRCPOL = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI_SLAVE configuration */
    SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_BDMRX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_SLAVE;
    SPI_Init(SPI3, &SPI_InitStructure);
    
    /* Enable SPI_MASTER TBE interrupt */
    SPI_I2S_INTConfig(SPI1, SPI_I2S_INT_TBE, ENABLE);
    /* Enable SPI_SLAVE RBNE interrupt */
    SPI_I2S_INTConfig(SPI3, SPI_I2S_INT_RBNE, ENABLE);
    /* Enable SPI1 AND SPI3 */
    SPI_Enable(SPI3, ENABLE);
    SPI_Enable(SPI1, ENABLE);

    /* Transfer procedure */
    while (RxIdx < BufferSize)
    { }

    /* Compare the receive data with send data */
    /* TransferStatus = PASSED, if the sended and received data are equal
       TransferStatus = FAILED, if the sended and received data are different */
    TransferStatus = Buffercmp(SPI_SLAVE_Buffer_Rx, SPI_MASTER_Buffer_Tx, BufferSize);

    while (1)
    {}
}

/**
  * @brief  Configure the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA|  RCC_APB2PERIPH_GPIOB |  RCC_APB2PERIPH_GPIOC| RCC_APB2PERIPH_AF , ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_SPI1,ENABLE);
    RCC_APB1PeriphClock_Enable( RCC_APB1PERIPH_SPI3 ,ENABLE );    
}
/**
  * @brief  Configure the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    GPIO_PinRemapConfig(GPIO_REMAP_SPI3, ENABLE);
    /* Configure SPI_MASTER pins: SCK and MOSI */
    GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_5| GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure SPI_SLAVE pins: SCK and MISO */
    /* Configure SCK pin as Alternate Function */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* Configure MISO pin as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;
    /* 1 bit for pre-emption priority, 3 bits for subpriority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_1);

    /* Configure and enable SPI_MASTER interrupt */
    NVIC_InitStructure.NVIC_IRQ = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 1;
    NVIC_InitStructure.NVIC_IRQSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Configure and enable SPI_SLAVE interrupt */
    NVIC_InitStructure.NVIC_IRQ = SPI3_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Compare two buffers.
  * @param  pBuffer1: buffers to be compared.
  * @param  pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
