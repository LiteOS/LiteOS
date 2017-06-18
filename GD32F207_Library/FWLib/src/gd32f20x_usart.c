/**
  ******************************************************************************
  * @file    gd32f20x_usart.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   USART functions of the firmware library.
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_usart.h"
#include "gd32f20x_rcc.h"
  
/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup USART 
  * @brief USART driver modules
  * @{
  */ 

/** @defgroup USART_Private_Defines
  * @{
  */
/* USART Interrupts mask */
#define INT_MASK                                        ((uint16_t)(0x0000001F))

/* USART CTLR1 initializes bit mask */
#define CTLR1_CLEAR_MASK                                ((uint16_t)(0x160C))

/* USART CTLR2 CLOCK initializes bit mask */
#define CTLR2_CLOCK_CLEAR_MASK                          ((uint16_t)(0x0F00))

/* USART CTLR3 initializes bit mask */
#define CTLR3_CLEAR_MASK                                ((uint16_t)(0x0300))

/* USART STR_FLAG mask */
#define STR_FLAG_MASK                                   ((uint32_t)0x00FFFFFF)
/**
  * @}
  */

/** @defgroup USART_Private_Functions
  * @{
  */
  
/**
  * @brief  Reset the USART peripheral.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  NewValue: new value of the USARTx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_DeInit(USART_TypeDef* USARTx)
{
    if (USARTx == USART1)
    {
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_USART1RST, ENABLE);
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_USART1RST, DISABLE);
    }
    else if (USARTx == USART2)
    {
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_USART2RST, ENABLE);
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_USART2RST, DISABLE);
    }
    else if (USARTx == USART3)
    {
        RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_USART3RST, ENABLE);
        RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_USART3RST, DISABLE);
    }    
    else if (USARTx == UART4)
    {
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_UART4RST, ENABLE);
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_UART4RST, DISABLE);
    }
    else if(USARTx == UART5)
    {
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_UART5RST, ENABLE);
         RCC_APB1PeriphReset_Enable(RCC_APB1PERIPH_UART5RST, DISABLE);
    }
    else if (USARTx == USART6)
    {
         RCC_APB2Periph2Reset_Enable(RCC_APB2PERIPH2_USART6RST, ENABLE);
         RCC_APB2Periph2Reset_Enable(RCC_APB2PERIPH2_USART6RST, DISABLE);
    }
    else if (USARTx == UART7)
    {
         RCC_APB1Periph2Reset_Enable(RCC_APB1PERIPH2_UART7RST, ENABLE);
         RCC_APB1Periph2Reset_Enable(RCC_APB1PERIPH2_UART7RST, DISABLE);
    }     
    else
    {
         if (USARTx == UART8)
         { 
          RCC_APB1Periph2Reset_Enable(RCC_APB1PERIPH2_UART8RST, ENABLE);
          RCC_APB1Periph2Reset_Enable(RCC_APB1PERIPH2_UART8RST, DISABLE);
         }
     }
 }

/**
  * @brief  Initialize the USART peripheral interface parameters.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_InitParaStruct: the struct USART_InitPara pointer.
  * @retval None
  */
void USART_Init(USART_TypeDef* USARTx, USART_InitPara* USART_InitParaStruct)
{
    uint32_t divider = 0, apbclock = 0, temp = 0;
    RCC_ClocksPara RCC_ClocksState;
    
    USARTx->CTLR1 &= ~((uint32_t)USART_CTLR1_UEN);
    /* Initialize CTLR2 */ 
    temp = USARTx->CTLR2;
    /* Reset stop bits then set it use USART_STBits */
    temp &= ~((uint32_t)USART_CTLR2_STB);
    temp |= (uint32_t)USART_InitParaStruct->USART_STBits;
    
    USARTx->CTLR2 = temp;
    
    /*Initialize CTLR1*/
    temp = USARTx->CTLR1;
    /* Reset WL, PCEN, PM, TEN and REN bits */
    temp &= ~((uint32_t)CTLR1_CLEAR_MASK);
    
    /* According to USART_WL,USART_Parity,USART_RxorTx to configure the CTLR1 */
    temp |= (uint32_t)USART_InitParaStruct->USART_WL | USART_InitParaStruct->USART_Parity |
    USART_InitParaStruct->USART_RxorTx;
    USARTx->CTLR1 = temp;
    
    /*Initialize CTLR3*/
    temp = USARTx->CTLR3;
    /* Reset CTSEN and RTSEN bits */
    temp &= ~((uint32_t)CTLR3_CLEAR_MASK);
    
    /* According to USART_HardwareFlowControl to configure the CTLR3 */
    temp |= USART_InitParaStruct->USART_HardwareFlowControl;
    USARTx->CTLR3 = temp;
    
    /*Initialize USART BRR*/
    RCC_GetClocksFreq(&RCC_ClocksState);
    
    if ((USARTx == USART1) || (USARTx == USART6))
    {
        apbclock = RCC_ClocksState.APB2_Frequency;
    }
    else
    {
        apbclock = RCC_ClocksState.APB1_Frequency;
    }
    
    /* Get integer of baud-rate divide and raction of baud-rate divider */
    divider = (uint32_t)((apbclock) / ((USART_InitParaStruct->USART_BRR)));
    temp    = (uint32_t)((apbclock) % ((USART_InitParaStruct->USART_BRR)));
   
    /* Round the divider : if fractional part i greater than 0.5 increment divider */
    if (temp >=  (USART_InitParaStruct->USART_BRR) / 2)
    {
        divider++;
    } 
     
    USARTx->BRR = (uint16_t)divider;
}

/**
  * @brief  Initial the struct USART_InitPara
  * @param  USART_InitParaStruct: the struct USART_InitPara pointer
  * @retval None
  */
void USART_ParaInit(USART_InitPara* USART_InitParaStruct)
{
    /* USART_InitStruct members default value */
    USART_InitParaStruct->USART_BRR = 9600;
    USART_InitParaStruct->USART_WL   =USART_WL_8B;
    USART_InitParaStruct->USART_STBits =USART_STBITS_1;
    USART_InitParaStruct->USART_Parity =USART_PARITY_RESET;
    USART_InitParaStruct->USART_RxorTx =USART_RXORTX_RX | USART_RXORTX_TX;
    USART_InitParaStruct->USART_HardwareFlowControl =USART_HARDWAREFLOWCONTROL_NONE;
}   

/**
  * @brief  Initialize the USARTx peripheral Clock according to the 
  *         specified parameters in the USART_ClockInitStruct.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  USART_ClockInitStruct: pointer to a USART_ClockInitTypeDef structure that contains 
  *                                the configuration information for the specified USART peripheral. 
  * @retval None
  */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitPara* USART_ClockInitStruct)
{
    uint32_t temp = 0;
    
    temp = USARTx->CTLR2;
    /* Clear CKEN, CPL, CPH, LBCP and SSM bits */
    temp &= ~(CTLR2_CLOCK_CLEAR_MASK);
    
    /* Reset hen set it usethe USART Clock, CPL, CPH, LBCP */
    temp |= (uint32_t)(USART_ClockInitStruct->USART_CKEN | USART_ClockInitStruct->USART_CPL | 
                       USART_ClockInitStruct->USART_CPH | USART_ClockInitStruct->USART_LBCP);
    /* Write to USART CTLR2 */
    USARTx->CTLR2 = (uint16_t)temp;
}

/**
  * @brief  Initial the struct USART_ClockInitPara.
  * @param  USART_ClockInitParaStruct: the struct USART_ClockInitPara pointer
  * @retval None
  */
void USART_ClockStructInit(USART_ClockInitPara* USART_ClockInitParaStruct)
{
    /*Reset USART_ClockInitStruct members default value */
    USART_ClockInitParaStruct->USART_CKEN = USART_CKEN_RESET;
    USART_ClockInitParaStruct->USART_CPL  = USART_CPL_LOW;
    USART_ClockInitParaStruct->USART_CPH  = USART_CPH_1EDGE;
    USART_ClockInitParaStruct->USART_LBCP = USART_LBCP_DISABLE;
}

/**
  * @brief  Enable or disable the specified USART peripheral.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  NewValue: new value of the USARTx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Enable(USART_TypeDef* USARTx, TypeState NewValue)
{
    /* Enable or disable the specified USART peripheral by setting the UEN bit in the CTLR1 register */ 
    if (NewValue!= DISABLE)
    {
        USARTx->CTLR1 |= USART_CTLR1_UEN;
    }
    else
    {
        USARTx->CTLR1 &= ~((uint32_t)USART_CTLR1_UEN);
    }
}
 
/**
  * @brief  Enable or disable the USART's most significant bit first.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  NewValue: new value of the USART most significant bit first
  *   This parameter can be: ENABLE or DISABLE.
  * @note   This function has to be called before calling USART_Enable() function.
  * @retval None
  */
void USART_MSBFirst_Enable(USART_TypeDef* USARTx, TypeState NewValue)
{
    /* By setting the MSBF bit in the CTLR2 register enable or disable the USART's most significant bit first*/  
    if (NewValue != DISABLE)
    {
        USARTx->CTLR4 |= USART_CTLR4_MSBF;
    }
    else
    {
        USARTx->CTLR4 &= ~((uint32_t)USART_CTLR4_MSBF);
    }
}

/**
  * @brief  Enable or disable data bit level inversion.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  NewValue: new levels of USART data.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_DataInvert_Enable(USART_TypeDef* USARTx, TypeState NewValue)
{
    /* By setting the DINV bit in the CTLR2 register enable or disable data bit level inversion */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR4 |= USART_CTLR4_DINV;
    }
    else
    {
        USARTx->CTLR4 &= (uint32_t)~((uint32_t)USART_CTLR4_DINV);
    }
}

/**
  * @brief  Enable or disable the Pin signal values inversion.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  USART_InvertPin:  The USART pin is inverted.
  *   This parameter can be :
  *     @arg USART_INVPIN_TX: USART Tx pin pin signal values inversion.
  *     @arg USART_INVPIN_RX: USART Rx pin signal values inversion.
  * @param  NewValue: new value 0f the USART pin.
  *   This parameter can be:
  *     @arg ENABLE: pin signal values are inverted (Vdd =0, Gnd =1).
  *     @arg DISABLE: pin signal values are not inverted (Vdd =1, Gnd =0).
  * @retval None
  */
void USART_InvPin_Enable(USART_TypeDef* USARTx, uint32_t USART_InvertPin, TypeState NewValue)
{
    /* By setting the TINV ,RINV in the USART CTLR2 register enable or disable the Pin signal values inversion */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR4 |=USART_InvertPin;
    }
    else
    {
        USARTx->CTLR4 &= ~USART_InvertPin;
    }
}
/**
  * @brief  Enable or disable the receiver TimeOut function of USART1.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  NewValue: new value of the USARTx receiver TimeOut function .
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_ReceiverTimeOut_Enable(USART_TypeDef* USARTx,TypeState NewValue)
{
    /* By setting the RTEN bit in the CTLR4 register enable or disable the receiver TimeOut function of USART1 */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR4 |= USART_CTLR4_RTEN;
    }
    else
    {
        USARTx->CTLR4 &= ~((uint32_t)USART_CTLR4_RTEN);
    }
}

/**
  * @brief  Set the receiver TimeOut threshold.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  USART_ReceiverTimeOutThreshold: the Receiver Time Out Threshold value.
  * @retval None
  */
void USART_SetReceiveTimeOut_Threshold(USART_TypeDef* USARTx,uint32_t USART_ReceiverTimeOutThreshold)
{    
    /* Clear and set the receiver TimeOut threshold */
    USARTx->RTR &= ~((uint32_t)USART_RTR_RT);
    
    USARTx->RTR |= USART_ReceiverTimeOutThreshold;
}
/**
  * @brief  Set the system clock prescaler.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_Prescaler: specifies the prescaler clock.
  * @note   This function has to be called before calling USART_Enable() function.
  * @retval None
  */
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler)
{ 
    /* Clear and set the USART prescaler */
    USARTx->GTPR &= ~((uint16_t)USART_GTPR_PSC);
    
    USARTx->GTPR |= USART_Prescaler;
}

/**
  * @brief  Send data by the USART peripheral.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5 
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8

  * @param  Data: the data will be sent.
  * @retval None
  */
void USART_DataSend(USART_TypeDef* USARTx,uint16_t Data)
{
    USARTx->DR = (Data & (uint16_t)0x01FF);
}
   
/**
  * @brief  Receive data by the USART peripheral.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @retval The received data.
  */
uint16_t USART_DataReceive(USART_TypeDef* USARTx)
{
    return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}
 

/**
  * @brief  Set the address of the USART terminal.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_Address: Indicates the address of the USART node.
  * @retval None
  */
void USART_Address(USART_TypeDef* USARTx, uint8_t USART_Address)
{
    /* Clear the USART address and Set the USART terminal*/
    USARTx->CTLR2 &= ~USART_CTLR2_ADD;
    
    USARTx->CTLR2 |=USART_Address;
}

/**
  * @brief  Enable or disable the USART's mute mode.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  NewValue: the USART mute mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_MuteMode_Enable(USART_TypeDef* USARTx, TypeState NewValue)
{
    /* By setting the MEN bit in the CTLR1 register enable or disable the USART's mute mode*/
    if (NewValue != DISABLE)
    {
        USARTx->CTLR1 |= USART_CTLR1_RWU;
    }
    else
    {
        USARTx->CTLR1 &=~USART_CTLR1_RWU;
    }
}

/**
  * @brief  Set the USART WakeUp method from mute mode.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_WakeUp
  *   This parameter can be one of the following values 
  *     @arg USART_WAKEUP_IDLELINE
  *     @arg USART_WAKEUP_ADDRESSMARK 
  * @retval None
  */
void USART_MuteModeWakeUp_Set(USART_TypeDef* USARTx, uint32_t USART_WakeUp)
{
    USARTx->CTLR1 &= ~((uint16_t)USART_CTLR1_WM);
    USARTx->CTLR1 |= USART_WakeUp;
}


/**
  * @brief  Set the USART LIN Break detection length.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_LINBreakDetectLength
  *   This parameter can be one of the following values 
  *     @arg USART_LINBREAKDETECTLENGTH_10B
  *     @arg USART_LINBREAKDETECTLENGTH_11B 
  * @retval None
  */
void USART_SetLINBDLength(USART_TypeDef* USARTx,uint32_t USART_LINBreakDetectLength)
{
    USARTx->CTLR2 &= ~((uint16_t)USART_CTLR2_LBDL);
    USARTx->CTLR2 |= USART_LINBreakDetectLength;
}

/**
  * @brief  Enable or disable the USART's LIN mode.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  NewValue: the USART LIN mode value.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_LIN_Enable(USART_TypeDef* USARTx,TypeState NewValue)
{
    /* By setting the LINEN bit in the CTLR2 register enable or disable the USART's LIN mode */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR2 |= USART_CTLR2_LMEN;
    }
    else
    {
        USARTx->CTLR2 &=~((uint16_t)USART_CTLR2_LMEN);
    }
}

/**
  * @brief  Enable or disable the Half-duplex mode.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  NewValue: the USART Half-duplex mode value.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_HalfDuplex_Enable(USART_TypeDef* USARTx, TypeState NewValue)
{
    /* By setting the HDEN bit in the CTLR3 register enable or disable the Half-duplex mode */ 
    if (NewValue  != DISABLE)
    {
        USARTx->CTLR3 |= USART_CTLR3_HDEN;
    }
    else
    {
        USARTx->CTLR3 &= ~USART_CTLR3_HDEN;
    }
}

/**
  * @brief  Set the the USART guard time.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  USART_GuardTime: the USART guard time.
  * @retval None
  */
void USART_GuardTime_Set(USART_TypeDef* USARTx, uint8_t USART_GuardTime)
{    
    /* Set the USART guard time */
    USARTx->GTPR &= ~((uint16_t)USART_GTPR_GT);
    
    USARTx->GTPR |= (uint16_t)((uint16_t)USART_GuardTime << 0x08);
}

/**
  * @brief  Enable or disable the USART's Smart Card mode.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  NewValue: the Smart Card mode value.
  *   This parameter can be: ENABLE or DISABLE. 
  * @retval None
  */
void USART_SmartCard_Enable(USART_TypeDef* USARTx,TypeState NewValue)
{
    /* By setting the SCEN bit in the CTLR3 register enable or disable the USART's Smart Card mode */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR3 |= USART_CTLR3_SCEN;
    }
    else
    {
        USARTx->CTLR3 &= ~((uint16_t)USART_CTLR3_SCEN);
    }
}

/**
  * @brief  Enable or disable NACK transmission.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  NewValue: the NACK transmission state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_SmartCardNACK_Enable(USART_TypeDef* USARTx,TypeState NewValue)
{
    /* By setting the NACK bit in the CTLR3 register Enable or disable NACK transmission */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR3 |= USART_CTLR3_NACK;
    }
    else
    {
        USARTx->CTLR3 &= ~((uint32_t)USART_CTLR3_NACK);
    }
}

/**
  * @brief  Set Smartcard auto-retry number.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6
  * @param  USART_AutoNumber: the Smart Card auto retry number.
  * @retval None
  */
void USART_AutoRetryNumber_Set(USART_TypeDef* USARTx,uint8_t USART_AutoNumber)
{    
    /* Set the USART auto retry count */
    USARTx->CTLR4 &= ~((uint32_t)USART_CTLR4_SCRTNUM);
    USARTx->CTLR4 |= ((uint32_t)USART_AutoNumber << 0x11);
}

/**
  * @brief  Set the Smart Card Block length.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg USART6 
  * @param  USART_BlockLength: the Smart Card block length.
  * @retval None
  */
void USART_BlockLength_Set(USART_TypeDef* USARTx,uint8_t USART_BlockLength)
{    
    /* Set the Smart Card Block length */
    USARTx->RTR &= ~((uint32_t)USART_RTR_BL);
    USARTx->RTR |= ((uint32_t)USART_BlockLength << 0x18);
}
/**
  * @brief  DMA enable for transmission or reception.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_DMAEnable: the DMA request.
  *   This parameter can be one of the following values 
  *     @arg USART_DMAREQ_RX
  *     @arg USART_DMAREQ_TX
  * @param  NewValue: the DMA Request sources state.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_DMA_Enable(USART_TypeDef* USARTx, uint32_t USART_DMAEnable, TypeState NewValue)
{
    /* Enable or disable the DMA transfer for transmission or reception by setting the DENT and/or
    DENR bits in the USART CTLR3 register */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR3 |= USART_DMAEnable;
    }
    else
    {
        USARTx->CTLR3 &= ~USART_DMAEnable;
    }
    
}

/**
  * @brief  Set the  USARTX IrDA low-power.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_IrDAMode
  *   This parameter can be one of the following values 
  *     @arg USART_IRDAMODE_LOWPOWER
  *     @arg USART_IRDAMODE_NORMAL.
  * @retval None
  */
void USART_IrDA_Set(USART_TypeDef* USARTx,uint32_t USART_IrDAMode)
{
    USARTx->CTLR3 &= ~((uint16_t)USART_CTLR3_IRLP);
    USARTx->CTLR3 |= USART_IrDAMode;
}

/**
  * @brief  Enable or disable the USART's IrDA interface.
  * @param  USARTx: the USART peripheral. 
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  * @param  NewValue: the IrDA mode value.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_IrDA_Enable(USART_TypeDef* USARTx,TypeState NewValue)
{
    /* By setting the IREN bit in the CTLR3 register enable or disable the USART's IrDA interface */
    if (NewValue != DISABLE)
    {
        USARTx->CTLR3 |= USART_CTLR3_IREN;
    }
    else
    {
        USARTx->CTLR3 &= ~((uint16_t)USART_CTLR3_IREN);
    }
}

/**
  * @brief  Detect the bit flag of STR register.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_FLAG: the flag of STR register.
  *   This parameter can be one of the following values:
  *     @arg USART_FLAG_CTSF: CTS Change flag.(not available for UART4 and UART5) 
  *     @arg USART_FLAG_LBDF: LIN Break detection flag
  *     @arg USART_FLAG_TBE:  Transmit data register empty flag.
  *     @arg USART_FLAG_TC:   Transmission Complete flag.
  *     @arg USART_FLAG_RBNE: Receive data register not empty flag.
  *     @arg USART_FLAG_IDLEF:Idle Line detection flag.
  *     @arg USART_FLAG_ORE:  OverRun Error flag.
  *     @arg USART_FLAG_NE:   Noise Error flag.
  *     @arg USART_FLAG_FE:   Framing Error flag.
  *     @arg USART_FLAG_PE:   Parity Error flag.
  *     @arg USART_FLAG_RTF                      
  *     @arg USART_FLAG_EBF                        
  *     @arg USART_FLAG_BSY                       
  * @retval The new state of USART_FLAG (SET or RESET).
  */
TypeState USART_GetBitState(USART_TypeDef* USARTx, uint32_t USART_FLAG)
{
    __IO uint32_t usartreg = 0, usartxbase = 0;
    
   /* Get the USARTx peripheral base address */
    usartxbase = (uint32_t)USARTx;
    
    /* Read flag register index */
    usartreg = USART_FLAG >> 28;
    
    /* Get bit[23:0] of the flag */
    USART_FLAG &= STR_FLAG_MASK;
    
    if(usartreg != 0)
    {
        /* Flag in USART STR register */
        usartxbase += 0x00;
    }
    else
    {
        /* Get the USARTx STR2 register address */
        usartxbase += 0x88;
    }
    
    if(((*(__IO uint32_t *)usartxbase) & USART_FLAG) != (uint32_t)RESET)
    {
        /* USART_FLAG is set */
        return SET;
    }
    else
    {
        /* USART_FLAG is reset */
        return RESET;
    }
    
}

/**
  * @brief  Enable or disable USART interrupt.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_INT: the USART interrupt
  *   This parameter can be one of the following values:                                             
  *     @arg USART_INT_PE                                                   
  *     @arg USART_INT_TBE                                                  
  *     @arg USART_INT_TC                                                   
  *     @arg USART_INT_RBNE                                                 
  *     @arg USART_INT_IDLEF                                                
  *     @arg USART_INT_LBDF                                                 
  *     @arg USART_INT_CTSF                                                 
  *     @arg USART_INT_ERIE
  *     @arg USART_INT_EB                                                  
  *     @arg USART_INT_RT                                                                                                      
  * @param  NewValue: the USART interrupt State.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_INT_Set(USART_TypeDef* USARTx, uint32_t USART_INT, TypeState NewValue)
{
    uint32_t  intpos=0 , usartreg=0;
    uint32_t  usartxbase = 0;
    
    usartxbase = (uint32_t)USARTx;
    
    /* Get the USART register index and the interrupt position */
    usartreg = ((uint8_t)(USART_INT) >> 0x05); 
    intpos = USART_INT & INT_MASK;
    
    /* Get the interrupt from which register: CTLR1,CTLR2 OR CTLR3 */
    if (usartreg == 0x01) 
    {
        usartxbase += 0x0C;
    }
    else if (usartreg == 0x02) 
    {
        usartxbase += 0x10;
    }
    else if (usartreg == 0x03) 
    {
       usartxbase += 0x14;       
    }
    else
    {
       usartxbase += 0x80;
    }
    if (NewValue != DISABLE)
    {
        *(__IO uint32_t*)usartxbase  |= (((uint32_t)0x01) << intpos);
    }
    else
    {
        *(__IO uint32_t*)usartxbase &= ~(((uint32_t)0x01) << intpos);
    }
}

/**
  * @brief  Clear the bit flag.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_FLAG: the bit flag usart .
  *   This parameter can be one of the following values:
  *     @arg USART_FLAG_CTSF: CTS Change flag.(not available for UART4 and UART5)
  *     @arg USART_FLAG_LBDF: LIN Break detection flag
  *     @arg USART_FLAG_TC:   Transmission Complete flag.
  *     @arg USART_FLAG_RBNE: Receive data register not empty flag.
  *     @arg USART_FLAG_RTF                        
  *     @arg USART_FLAG_EBF                        
  * @note
  *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *     error) and IDLE (Idle line detected) flag bits are cleared by 
  *     software sequence: a read operation to USART_STR register 
  *     (USART_GetBitState()) followed by a read operation to USART_DR register 
  *     (USART_DataReceive()).
  *   - RXBE flag bit can be also cleared by a read to the USART_DR register 
  *     (USART_DataReceive()).
  *   - TC flag bit can be also cleared by software sequence: a read 
  *     operation to USART_STR register (USART_GetBitState()) followed by a write 
  *     operation to USART_DR register (USART_DataSend()).
  *   - TBE flag bit is cleared only by a write to the USART_DR register 
  *     (USART_DataSend()).
  * @retval None
  */
void USART_ClearBitState(USART_TypeDef* USARTx, uint32_t USART_FLAG)
{
    __IO uint32_t usartreg = 0;
        
    /* Read flag register index */
    usartreg = USART_FLAG >> 28;
    
    /* Get bit[23:0] of the flag */
    USART_FLAG &= STR_FLAG_MASK;
    
    if(usartreg != 0)
    {
        /* Flag in USART STR register */
       USARTx->STR =(uint32_t)~ USART_FLAG;
    }
    else
    {
        /* Get the USARTx STR2 register address */
       USARTx->STR2 =(uint32_t)~ USART_FLAG;
    }
}

/**
  * @brief  Detect the interrupt bit flag.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_INT: the USART interrupt bit flag.
  *   This parameter can be one of the following values:                                                
  *     @arg USART_INT_PE                                                   
  *     @arg USART_INT_TBE                                                  
  *     @arg USART_INT_TC                                                   
  *     @arg USART_INT_RBNE                                                 
  *     @arg USART_INT_IDLEF                                                
  *     @arg USART_INT_LBDF                                                 
  *     @arg USART_INT_CTSF:                                               
  *     @arg USART_INT_ORE                                                  
  *     @arg USART_INT_NE                                                   
  *     @arg USART_INT_FE
  *     @arg USART_INT_EB                                                  
  *     @arg USART_INT_RT  
  * @retval The new state of USART_INT (SET or RESET).
  */
TypeState USART_GetIntBitState(USART_TypeDef* USARTx, uint32_t USART_INT)
{
    uint16_t bitpos = 0, itmask = 0, usartreg = 0;
   __IO uint32_t usartSTRreg = 0;
    
    /* Get the USART register index and the interrupt position */
    usartreg = (((uint8_t)USART_INT) >> 0x05);
    
    itmask = (USART_INT)&(INT_MASK);
    
    itmask = (uint16_t)0x01 << itmask;
    
    if (usartreg == 0x01)
    {
        itmask &= USARTx->CTLR1;
    }
    else
    {
        if (usartreg == 0x02) 
        {
            itmask &= USARTx->CTLR2;
        }
        else if (usartreg == 0x03) 
        {
            itmask &= USARTx->CTLR3;
        }
        else
        {
            itmask &= USARTx->CTLR4;
        }
    }
    
    bitpos = USART_INT >> 0x08;
    bitpos = (uint32_t)0x01 << bitpos;

        
    /* Read flag register index */
    usartSTRreg = USART_INT >> 28;
    
    if(usartSTRreg != 0)
    {
        /* Flag in USART STR register */
        bitpos &= USARTx->STR;
    }
    else
    {
        /* Get the USARTx STR2 register address */
       bitpos &= USARTx->STR2;
    }

    if ((itmask != (uint16_t)RESET)&&(bitpos != (uint16_t)RESET))
    {
        return  SET;
    }
    else
    {
        return  RESET;
    }
}

/**
  * @brief  Clear the interrupt bit flag.
  * @param  USARTx: the USART peripheral.
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @param  USART_INT: the interrupt bit flag.
  *   This parameter can be one of the following values:                                                                                  *     @arg USART_INT_TC         
  *     @arg USART_INT_LBDF                                                 
  *     @arg USART_INT_CTSF: CTS Flag interrupt (not available for UART4, UART5,UART7,UART8)                                                  
  *     @arg USART_INT_RBNE
  *     @arg USART_INT_TC                                                 
  *     @arg USART_INT_EB: CTS Flag interrupt (not available for UART4, UART5,UART7,UART8)                                                  
  *     @arg USART_INT_RT        
  * @note
  *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *     error) and IDLE (Idle line detected) interrupt bits are cleared by 
  *     software sequence: a read operation to USART_STR register 
  *     (USART_GetINTState()) followed by a read operation to USART_DR register 
  *     (USART_DataReceive()).
  *   - RXBE interrupt bit can be also cleared by a read to the USART_DR register 
  *     (USART_DataReceive()).
  *   - TC interrupt bit can be also cleared by software sequence: a read 
  *     operation to USART_STR register (USART_GetINTState()) followed by a write 
  *     operation to USART_DR register (USART_DataSend()).
  *   - TBE interrupt bit is cleared only by a write to the USART_DR register 
  *     (USART_DataSend()).
  * @retval None
  */
void USART_ClearIntBitState(USART_TypeDef* USARTx, uint32_t USART_INT)
{
    uint32_t bitpos = 0, itmask = 0;
    __IO uint32_t usartSTRreg = 0;
    
    bitpos = USART_INT >> 0x08;
    itmask = ((uint32_t)0x01 << (uint32_t)bitpos);
    
    /* Read flag register index */
    usartSTRreg = USART_INT >> 28;
    
    if(usartSTRreg != 0)
    {
        /* Flag in USART STR register */
         USARTx->STR = (uint32_t)~itmask;
    }
    else
    {
        /* Get the USARTx STR2 register address */
        USARTx->STR2 = (uint32_t)~itmask;
    }
   
}

/**
  * @brief  Send break command.
  * @param  USARTx: the USART peripheral
  *   This parameter can be one of the following values 
  *     @arg USART1
  *     @arg USART2
  *     @arg USART3
  *     @arg UART4
  *     @arg UART5
  *     @arg USART6
  *     @arg UART7
  *     @arg UART8
  * @retval None
  */
void USART_SendBreak(USART_TypeDef* USARTx)
{
  
    /* Send break command */
    USARTx->CTLR1 |= USART_CTLR1_SBKCMD;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2015 GIGADEVICE *****END OF FILE****/
