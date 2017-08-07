/**
  ******************************************************************************
  * @file    ETH/Project/src/gd32f107.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Function moduel configuration
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "platform_cfg.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
const uint8_t GD32F10x_STR[] = {"\r\n ############ Welcome GigaDevice ############\r\n"};

/* Private function prototypes -----------------------------------------------*/
extern void Delay_Count(__IO uint32_t nCount);
extern void Ethernet_MDIO_Config(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Ethernet_Security(void);
void Ethernet_Configuration(void);
void USART_Configuration(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Setup GD32 system (clocks, Ethernet, GPIO, NVIC) and GD32F107C-EVAL resources.
  * @param  None
  * @retval None
  */
void System_Setup(void)
{
    RCC_ClocksPara RCC_Clocks;

    /* Enable USART2 clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_USART1, ENABLE);


    /* Enable ETHERNET clock  */
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_ETH_MAC | RCC_AHBPERIPH_ETH_MAC_TX |
                              RCC_AHBPERIPH_ETH_MAC_RX, ENABLE);	 

    /* Enable GPIOs and ADC1 clocks */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB | RCC_APB2PERIPH_GPIOC |
                               RCC_APB2PERIPH_GPIOD | RCC_APB2PERIPH_GPIOE | RCC_APB2PERIPH_AF, ENABLE);
  
    /* NVIC configuration */
    NVIC_Configuration();  

    /* Configure the GPIO ports */
    GPIO_Configuration();

    USART_Configuration();

    /* To assure Ethernet Phy work well */ 
    Ethernet_Security();    

    /* Configure the Ethernet peripheral */
    Ethernet_Configuration();
  
    /* SystTick configuration: an interrupt every 10ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.CK_SYS_Frequency / 100);

    /* Update the SysTick IRQ priority should be higher than the Ethernet IRQ */
    /* The Localtime should be updated during the Ethernet packets processing */
    NVIC_SetPriority (SysTick_IRQn, 1);  
}

/**
  * @brief  Check ETH function
  * @param  None
  * @retval None
  */
void Ethernet_Security(void)
{
    /* MII/RMII Media interface selection */
#ifdef MII_MODE /* Mode MII with GD32F107C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

    /* Get HSE clock = 25MHz on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_HSE);

#elif defined RMII_MODE  /* Mode RMII with GD32F107C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MEDIAINTERFACE_RMII);

    /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
    RCC_PLL3Config(RCC_PLL3MUL_10);
    /* Enable PLL3 */
    RCC_PLL3_Enable(ENABLE);
    /* Wait till PLL3 is ready */
    while (RCC_GetBitState(RCC_FLAG_PLL3STB) == RESET)
    {}

    /* Get PLL3 clock on PA8 pin (MCO) */
    RCC_CKOUTSRCConfig(RCC_CKOUTSRC_PLL3CLK);
#endif

    ETH_MDIOInit(); 
    /* PHY initialization and configuration */
    /* Put the PHY in reset mode */
    ETH_SetPHYRegisterValue(PHY_ADDRESS, PHY_BCR, PHY_RESET);
    Delay_Count(2*PHY_RESETDELAY); 
    if(ETH_GetPHYRegisterValue(PHY_ADDRESS, 2) != 0x2000)
    {
        NVIC_SystemReset();
    }

    if((ETH_GetPHYRegisterValue(PHY_ADDRESS, 23) & 0x0020) != 0x0020)
    {
        NVIC_SystemReset();
    }  
}
/**
  * @brief  Configure the Ethernet Interface
  * @param  None
  * @retval None
  */
void Ethernet_Configuration(void)
{
    ETH_InitPara ETH_InitStructure;

    /* MII/RMII Media interface selection */
#ifdef MII_MODE /* Mode MII with GD32F107C-EVAL  */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

    /* Get HSE clock = 25MHz on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_HSE);

#elif defined RMII_MODE  /* Mode RMII with GD32F107C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MEDIAINTERFACE_RMII);

    /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
    RCC_PLL3Config(RCC_PLL3MUL_10);
    /* Enable PLL3 */
    RCC_PLL3_Enable(ENABLE);
    /* Wait till PLL3 is ready */
    while (RCC_GetBitState(RCC_FLAG_PLL3STB) == RESET)
    {}

    /* Get PLL3 clock on PA8 pin (MCO) */
    RCC_CKOUTSRCConfig(RCC_CKOUTSRC_PLL3CLK);
#endif

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftReset();

    /* Wait for software reset */
    while (ETH_GetSoftResetStatus() == SET);

    /* ETHERNET Configuration */
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_ParaInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /* MAC */
    ETH_InitStructure.ETH_MAC_AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE  ;
    ETH_InitStructure.ETH_MAC_LoopbackMode = ETH_LOOPBACKMODE_DISABLE;
    ETH_InitStructure.ETH_MAC_RetryTransmission = ETH_RETRYTRANSMISSION_DISABLE;
    ETH_InitStructure.ETH_MAC_AutomaticPadCRCDrop = ETH_AUTOMATICPADCRCDROP_DISABLE;
    ETH_InitStructure.ETH_MAC_FilterDisable = ETH_FILTERDISABLE_DISABLE;
    ETH_InitStructure.ETH_MAC_BroadcastFramesReception = ETH_BROADCASTFRAMESRECEPTION_ENABLE;
    ETH_InitStructure.ETH_MAC_PromiscuousMode = ETH_PROMISCUOUSMODE_DISABLE;
    ETH_InitStructure.ETH_MAC_MulticastFramesFilter = ETH_MULTICASTFRAMESFILTER_PERFECT;
    ETH_InitStructure.ETH_MAC_UnicastFramesFilter = ETH_UNICASTFRAMESFILTER_PERFECT;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
    /* DMA */  
    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
       the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
       if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DMA_DropTCPIPChecksumErrorFrame = ETH_DROPTCPIPCHECKSUMERRORFRAME_ENABLE; 
    ETH_InitStructure.ETH_DMA_ReceiveStoreForward = ETH_RECEIVESTOREFORWARD_ENABLE;         
    ETH_InitStructure.ETH_DMA_TransmitStoreForward = ETH_TRANSMITSTOREFORWARD_ENABLE;     
 
    ETH_InitStructure.ETH_DMA_ForwardErrorFrames = ETH_FORWARDERRORFRAMES_DISABLE;       
    ETH_InitStructure.ETH_DMA_ForwardUndersizedGoodFrames = ETH_FORWARDUNDERSIZEDGOODFRAMES_DISABLE;   
    ETH_InitStructure.ETH_DMA_SecondFrameOperate = ETH_SECONDFRAMEOPERATE_ENABLE;                                                          
    ETH_InitStructure.ETH_DMA_AddressAligned = ETH_ADDRESSALIGNED_ENABLE;      
    ETH_InitStructure.ETH_DMA_FixedBurst = ETH_FIXEDBURST_ENABLE;                
    ETH_InitStructure.ETH_DMA_RxDMABurstLength = ETH_RXDMABURSTLENGTH_32BEAT;          
    ETH_InitStructure.ETH_DMA_TxDMABurstLength = ETH_TXDMABURSTLENGTH_32BEAT;                                                                 
    ETH_InitStructure.ETH_DMA_Arbitration = ETH_DMAARBITRATION_ROUNDROBIN_RXTX_2_1;

    /* Configure Ethernet */
    if(ETH_Init(&ETH_InitStructure, PHY_ADDRESS))
    {
        printf("---> link success"); 
    }
    else
    {
        printf(" ---> TIP: Failed to connect to the network, Ethernet_Configuration() ETH_ERROR");
        printf("\n\r Please check the network cable connected properly! After connecting the cable, please reset the system\n\r ");
        Delay_Count(20000000);
        Delay_Count(20000000);
        if(!ETH_Init(&ETH_InitStructure, PHY_ADDRESS))
        {
            Delay_Count(800000000);
        }
    } 
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAINTConfig(ETH_DMA_INT_NIS | ETH_DMA_INT_R, ENABLE);
}

/**
  * @brief  Configure the different GPIO ports.
  * @param  None
  * @retval None
  */
#ifdef MII_MODE  
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
    - ETH_MII_MDIO / ETH_RMII_MDIO: PA2
    - ETH_MII_MDC / ETH_RMII_MDC: PC1
    - ETH_MII_TXD2: PC2
    - ETH_MII_TX_EN / ETH_RMII_TX_EN: PB11
    - ETH_MII_TXD0 / ETH_RMII_TXD0: PB12
    - ETH_MII_TXD1 / ETH_RMII_TXD1: PB13
    - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT: PB5
    - ETH_MII_TXD3: PB8 */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 |
                                  GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Input (Reset Value):
    - ETH_MII_CRS CRS: PA0
    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK: PA1
    - ETH_MII_COL: PA3
    - ETH_MII_RX_DV / ETH_RMII_CRS_DV: PD8
    - ETH_MII_TX_CLK: PC3
    - ETH_MII_RXD0 / ETH_RMII_RXD0: PD9
    - ETH_MII_RXD1 / ETH_RMII_RXD1: PD10
    - ETH_MII_RXD2: PD11
    - ETH_MII_RXD3: PD12
    - ETH_MII_RX_ER: PB10 */

    /* ETHERNET pins remapp in GD32F107C-EVAL board: RX_DV and RxD[3:0] */
    GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

    /* Configure PA0, PA1 and PA3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB10 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC3 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PD8, PD9, PD10, PD11 and PD12 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    /* MCO pin configuration */
    /* Configure MCO (PA8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#else /* RMII_MODE */  
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
    - ETH_MII_MDIO / ETH_RMII_MDIO: PA2
    - ETH_MII_MDC / ETH_RMII_MDC: PC1
    - ETH_MII_TX_EN / ETH_RMII_TX_EN: PB11
    - ETH_MII_TXD0 / ETH_RMII_TXD0: PB12
    - ETH_MII_TXD1 / ETH_RMII_TXD1: PB13
    */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Input (Reset Value):
    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK: PA1
    - ETH_MII_RX_DV / ETH_RMII_CRS_DV: PA7
    - ETH_MII_RXD0 / ETH_RMII_RXD0: PC4
    - ETH_MII_RXD1 / ETH_RMII_RXD1: PC5
    */

    /* Configure PA1,PA7 as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_1|GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    /* Configure PC4, PC5,  as input */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_4 | GPIO_PIN_5 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    /* MCO pin configuration */
    /* Configure MCO (PA8) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
#endif

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara   NVIC_InitStructure;

    /* Set the Vector Table base location at 0x08000000 */
    NVIC_VectTableSet(NVIC_VECTTAB_FLASH, 0x0);

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_2); 
  
    /* Enable the Ethernet global Interrupt */
    NVIC_InitStructure.NVIC_IRQ = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 2;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
  
}


/**
  * @brief  Configure USART1.
  * @param  None
  * @retval None
  */
void USART_Configuration()
{
    GPIO_InitPara     GPIO_InitStructure;
    USART_InitPara    USART_InitStructure;
    /* Enable GPIO clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA, ENABLE);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BRR = 115200;
    USART_InitStructure.USART_WL = USART_WL_8B;
    USART_InitStructure.USART_STBits = USART_STBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_RESET;
    USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
    USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;
    /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);
    
    /* Enable USART */
    USART_Enable(USART1, ENABLE);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
