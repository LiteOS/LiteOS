/**
  ******************************************************************************
  * @file    I2S/I2S_Interrupt/main.c
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

/* Private variables ---------------------------------------------------------*/
I2S_InitPara I2S_InitStructure;
const uint16_t I2S3_Buffer_Tx[32] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                                     0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                                     0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                                     0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                     0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                                     0x3D3E, 0x3F40};

uint16_t I2S2_Buffer_Rx[32];
__IO uint32_t TxIdx = 0, RxIdx = 0;
TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
TypeState HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
TestStatus Buffercmp24bits(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

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

    SPI_I2S_DeInit(SPI3);
    SPI_I2S_DeInit(SPI2);
    /* I2S peripheral configuration */
    I2S_InitStructure.I2S_STD = I2S_STD_PHILLIPS;
    I2S_InitStructure.I2S_FrameFormat = I2S_FRAMEFORMAT_DL16b_CL32b;
    I2S_InitStructure.I2S_MCKOE = I2S_MCK_DISABLE;
    I2S_InitStructure.I2S_AudioFreq = I2S_AUDIOFREQ_48K;
    I2S_InitStructure.I2S_CKPL = I2S_CKPL_LOW;

    /* I2S3 Master Transmitter to I2S2 Slave Receiver communication */
    /* I2S3 configuration */
    I2S_InitStructure.I2S_Mode = I2S_MODE_MASTERTX;
    I2S_Init(SPI3, &I2S_InitStructure);

    /* I2S2 configuration */
    I2S_InitStructure.I2S_Mode = I2S_MODE_SLAVERX;
    I2S_Init(SPI2, &I2S_InitStructure);

    /* Enable the I2S3 TBE interrupt */
    SPI_I2S_INTConfig(SPI3, SPI_I2S_INT_TBE, ENABLE);

    /* Enable the I2S2 RBNE interrupt */
    SPI_I2S_INTConfig(SPI2, SPI_I2S_INT_RBNE, ENABLE);

    /* Enable the I2S2 */
    I2S_Enable(SPI2, ENABLE);

    /* Enable the I2S3 */
    I2S_Enable(SPI3, ENABLE);

    /* Wait the end of communication */
    while (RxIdx < 32)
    {}

    TransferStatus1 = Buffercmp(I2S2_Buffer_Rx, (uint16_t*)I2S3_Buffer_Tx, 32);
    /* TransferStatus1 = PASSED, if the data transmitted from I2S3 and received by
                               I2S2 are the same
     TransferStatus1 = FAILED, if the data transmitted from I2S3 and received by
                               I2S2 are different */
  
    /* Reinitialize the buffers */
    for (RxIdx = 0; RxIdx < 32; RxIdx++)
    {
        I2S2_Buffer_Rx[RxIdx] = 0;
    }
    TxIdx = 0;
    RxIdx = 0;

    SPI_I2S_DeInit(SPI3);
    SPI_I2S_DeInit(SPI2);  
  
    /* I2S peripheral configuration */
    I2S_InitStructure.I2S_STD = I2S_STD_PHILLIPS;
    I2S_InitStructure.I2S_FrameFormat = I2S_FRAMEFORMAT_DL24b_CL32b;
    I2S_InitStructure.I2S_MCKOE = I2S_MCK_DISABLE;
    I2S_InitStructure.I2S_AudioFreq = I2S_AUDIOFREQ_16K;
    I2S_InitStructure.I2S_CKPL = I2S_CKPL_LOW;

    /* I2S3 Master Transmitter to I2S2 Slave Receiver communication -----------*/
    /* I2S3 configuration */
    I2S_InitStructure.I2S_Mode = I2S_MODE_MASTERTX;
    I2S_Init(SPI3, &I2S_InitStructure);

    /* I2S2 configuration */
    I2S_InitStructure.I2S_Mode = I2S_MODE_SLAVERX;
    I2S_Init(SPI2, &I2S_InitStructure);

    /* Enable the I2S3 TxE interrupt */
    SPI_I2S_INTConfig(SPI3, SPI_I2S_INT_TBE, ENABLE);

    /* Enable the I2S2 RxNE interrupt */
    SPI_I2S_INTConfig(SPI2, SPI_I2S_INT_RBNE, ENABLE);

    /* Enable the I2S2 */
    I2S_Enable(SPI2, ENABLE);

    /* Enable the I2S3 */
    I2S_Enable(SPI3, ENABLE);

    /* Wait the end of communication */
    while (RxIdx < 32)
    {
    }

    TransferStatus2 = Buffercmp24bits(I2S2_Buffer_Rx, (uint16_t*)I2S3_Buffer_Tx, 32);
    /* TransferStatus2 = PASSED, if the data transmitted from I2S3 and received by
                         I2S2 are the same
       TransferStatus2 = FAILED, if the data transmitted from I2S3 and received by
                         I2S2 are different */     

    while (1)
    {
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(HSEStartUpStatus == SUCCESS)
    {
        RCC_AHBConfig(RCC_SYSCLK_DIV1); 
  
        RCC_APB2Config(RCC_APB2AHB_DIV1); 

        RCC_APB1Config(RCC_APB1AHB_DIV2);

        RCC_ADCCLKConfig(RCC_ADCCLK_APB2_DIV4); 
  
#ifndef GD32F10X_CL  
        RCC_PLLConfig(RCC_PLLSOURCE_HSE_DIV1, RCC_PLLMUL_9);

#else
        /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
        RCC_PREDV2Config(RCC_PREDIV2_DIV5);
        RCC_PLL2Config(RCC_PLL2MUL_8);
      
        /* Enable PLL2 */
        RCC_PLL2_Enable(ENABLE);

        /* Wait till PLL2 is ready */
        while (RCC_GetBitState(RCC_FLAG_PLL2STB) == RESET)
        { }

        /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
        RCC_PREDV1Config(RCC_PREDIV1_SOURCE_PLL2, RCC_PREDIV1_DIV5);
        RCC_PLLConfig(RCC_PLLSOURCE_PREDIV1, RCC_PLLMUL_9);

        RCC_PLL3Config(RCC_PLL3MUL_11);
        /* Enable PLL3 */
        RCC_PLL3_Enable(ENABLE);    
        /* Wait till PLL3 is ready */
        while (RCC_GetBitState(RCC_FLAG_PLL3STB) == RESET)
        { }
        
        RCC_I2S3CLKConfig( RCC_I2S3CLK_PLL3 );
        RCC_I2S2CLKConfig( RCC_I2S2CLK_PLL3 );  
#endif

        /* Enable PLL */ 
        RCC_PLL_Enable(ENABLE);

        /* Wait till PLL is ready */
        while(RCC_GetBitState(RCC_FLAG_PLLSTB) == RESET)
        { }

        /* Select PLL as system clock source */
        RCC_CK_SYSConfig(RCC_SYSCLKSOURCE_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while(RCC_GetCK_SYSSource() != 0x08)
        { }

    }
  
    /* Enable peripheral clocks */
    /* GPIOA, GPIOB, GPIOC and AF clocks enable */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB |RCC_APB2PERIPH_GPIOC|
                               RCC_APB2PERIPH_AF, ENABLE);

    /* SPI3 and SPI2 clocks enable */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_SPI3 | RCC_APB1PERIPH_SPI2, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Configure SPI2 pins: CK, WS and SD */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /* Remap SPI3 on PC10-PC11-PC12-PA4 GPIO pins */
    GPIO_PinRemapConfig(GPIO_REMAP_SPI3, ENABLE);
    /* Configure SPI3 pins: CK and SD */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10 | GPIO_PIN_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
    /* Configure SPI3 pins: WS */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_0);

    /* SPI3 IRQ Channel configuration */
    NVIC_InitStructure.NVIC_IRQ = SPI3_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 1;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* SPI2 IRQ channel configuration */
    NVIC_InitStructure.NVIC_IRQ = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
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

/**
  * @brief  Compares two buffers in 24 bits data format.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp24bits(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            if (*pBuffer1 != (*pBuffer2 & 0xFF00))
            {
                return FAILED;
            }
        }
        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}
/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/

