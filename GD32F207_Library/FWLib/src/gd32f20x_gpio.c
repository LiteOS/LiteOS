/**
  ******************************************************************************
  * @file    gd32f20x_gpio.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    15-Jul-2015
  * @brief   GPIO functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f20x_gpio.h"
#include "gd32f20x_rcc.h"

/** @addtogroup GD32F20x_Firmware
  * @{
  */

/** @defgroup GPIO 
  * @brief GPIO driver modules
  * @{
  */

/** @defgroup GPIO_Private_Defines
  * @{
  */
#define EVR_PORTPINCONFIG_MASK          ((uint16_t)0xFF80)
#define LSB_MASK                        ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK            ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK              ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK            ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK             ((uint32_t)0x00100000)
#define AFIO_PCFR1_MII_RMII             ((uint32_t)0x00800000)
#define AFIO_ESSR1_BITS_FIELDS          ((uint8_t)0x04)
#define AFIO_ESSR2_BITS_FIELDS          ((uint8_t)0x08)
#define AFIO_ESSR3_BITS_FIELDS          ((uint8_t)0x0c)
#define AFIO_ESSR_PIN_FIELDS            ((uint8_t)0x04)
#define AFIO_ESSR_4BIT_FIELDS           ((uint8_t)0x0f)
#define CTLR1_8BIT_FIELDS               ((uint32_t)0x00ff)
#define CTLR2_8BIT_FIELDS               ((uint32_t)0x00ff)
#define AFIO_PCFR2_FIELDS               ((uint32_t)0x80000000)

/**
  * @}
  */

/** @defgroup GPIO_Private_Functions 
  * @{
  */

/**
  * @brief  Reset the GPIOx peripheral.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval None
  */     
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
    if (GPIOx == GPIOA)
    {
        /* Enable GPIOA reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOA, ENABLE);
        /* Release GPIOA from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOA, DISABLE);
    }
    else if (GPIOx == GPIOB)
    {
        /* Enable GPIOB reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOB, ENABLE);
        /* Release GPIOB from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOB, DISABLE);
    }
    else if (GPIOx == GPIOC)
    {
        /* Enable GPIOC reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOC, ENABLE);
        /* Release GPIOC from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOC, DISABLE);
    }
    else if (GPIOx == GPIOD)
    {
        /* Enable GPIOD reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOD, ENABLE);
        /* Release GPIOD from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOD, DISABLE);
    }
    else if (GPIOx == GPIOE)
    {
        /* Enable GPIOE reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOE, ENABLE);
        /* Release GPIOE from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOE, DISABLE);
    }
    else if (GPIOx == GPIOF)
    {
        /* Enable GPIOF reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOF, ENABLE);
        /* Release GPIOF from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOF, DISABLE);
    }
    else if(GPIOx == GPIOG)
    {
        /* Enable GPIOG reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOG, ENABLE);
        /* Release GPIOG from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_GPIOG, DISABLE);
    }
    else if(GPIOx == GPIOH)
    {
        /* Enable GPIOH reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH2_GPIOH, ENABLE);
        /* Release GPIOH from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH2_GPIOH, DISABLE);
    }
    else if(GPIOx == GPIOI)
    {
        /* Enable GPIOI reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH2_GPIOI, ENABLE);
        /* Release GPIOI from reset state */
        RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH2_GPIOI, DISABLE);
    }
}

/**
  * @brief  Deinitialize the Alternate Functions (remap, event control
  *         and EXTI configuration) registers to their default reset values.
  * @param  None
  * @retval None
  */
void GPIO_AFDeInit(void)
{
    /* Enable AFIO reset state */
    RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_AF, ENABLE);
    /* Release AFIO from reset state */
    RCC_APB2PeriphReset_Enable(RCC_APB2PERIPH_AF, DISABLE);
}

/**
  * @brief  Initialize the GPIOx peripheral.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_InitStruct: The structuer contains configuration information.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitPara* GPIO_InitStruct)
{
    uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
    uint32_t tempreg = 0x00, pinmask = 0x00;

    /* GPIO Mode Configuration */
    currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
    if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
    {
        /* Output mode */
        currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
    }
    /* GPIO CTLR1 Configuration */
    /* Configure the eight low port pins */
    if (((uint32_t)GPIO_InitStruct->GPIO_Pin & CTLR1_8BIT_FIELDS) != 0x00)
    {
        tempreg = GPIOx->CTLR1;
        for (pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = ((uint32_t)0x01) << pinpos;
            /* Get the port pins position */
            currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
            if (currentpin == pos)
            {
                pos = pinpos << 2;
                /* Clear the corresponding low control register bits */
                pinmask = ((uint32_t)0x0F) << pos;
                tempreg &= ~pinmask;
                /* Write the mode configuration in the corresponding bits */
                tempreg |= (currentmode << pos);
                /* Reset the corresponding ODR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_IPD)
                {
                    GPIOx->BCR = (((uint32_t)0x01) << pinpos);
                }
                else
                {
                    /* Set the corresponding ODR bit */
                    if (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_IPU)
                    {
                        GPIOx->BOR = (((uint32_t)0x01) << pinpos);
                    }
                }
            }
        }
        GPIOx->CTLR1 = tempreg;
    }
    /* GPIO CTLR2 Configuration */
    /* Configure the eight high port pins */
    if (GPIO_InitStruct->GPIO_Pin > CTLR2_8BIT_FIELDS)
    {
        tempreg = GPIOx->CTLR2;
        for (pinpos = 0x00; pinpos < 0x08; pinpos++)
        {
            pos = (((uint32_t)0x01) << (pinpos + 0x08));
            /* Get the port pins position */
            currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
            if (currentpin == pos)
            {
                pos = pinpos << 2;
                /* Clear the corresponding high control register bits */
                pinmask = ((uint32_t)0x0F) << pos;
                tempreg &= ~pinmask;
                /* Write the mode configuration in the corresponding bits */
                tempreg |= (currentmode << pos);
                /* Reset the corresponding DOR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_IPD)
                {
                    GPIOx->BCR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
                /* Set the corresponding DOR bit */
                if (GPIO_InitStruct->GPIO_Mode == GPIO_MODE_IPU)
                {
                    GPIOx->BOR = (((uint32_t)0x01) << (pinpos + 0x08));
                }
            }
        }
        GPIOx->CTLR2 = tempreg;
    }
}

/**
  * @brief  Initial GPIO_InitParameter members.
  * @param  GPIO_InitParameter : pointer to a GPIO_InitPara structure.
  * @retval None
  */
void GPIO_ParaInit(GPIO_InitPara* GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin  = GPIO_PIN_ALL;
    GPIO_InitStruct->GPIO_Speed = GPIO_SPEED_2MHZ;
    GPIO_InitStruct->GPIO_Mode = GPIO_MODE_IN_FLOATING;
}

/**
  * @brief  Read the select input port.
  * @param  GPIOx: Select the GPIO peripheral.
  * @param  GPIO_Pin: Select the port.
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    if ((GPIOx->DIR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Read the specified GPIO input data port.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
    return ((uint16_t)GPIOx->DIR);
}

/**
  * @brief  Read the specified output data port bit.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: where pin can be (GPIO_PIN_0..GPIO_PIN_15) to select the GPIO peripheral.
  * @retval The output port pin value.
  */
uint8_t GPIO_ReadOutputBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    if ((GPIOx->DOR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Read the specified GPIO output data port.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
    return ((uint16_t)GPIOx->DOR);
}

/**
  * @brief  Set the selected data port bits.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: where pin can be (GPIO_PIN_0..GPIO_PIN_15) to select the GPIO peripheral.
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BOR = GPIO_Pin;
}

/**
  * @brief  Clear the selected data port bits.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: where pin can be (GPIO_PIN_0..GPIO_PIN_15) to select the GPIO peripheral.
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BCR = GPIO_Pin;
}

/**
  * @brief  Set or clear the selected data port bit.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: where pin can be (GPIO_PIN_0..GPIO_PIN_15) to select the GPIO peripheral.
  * @param  BitVal: specifies the state of the port.Select one of the follwing values :
  *   @arg Bit_RESET: clear the port pin
  *   @arg Bit_SET: set the port pin
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitState BitVal)
{
    if (BitVal != Bit_RESET)
    {
        GPIOx->BOR = GPIO_Pin;
    }
    else
    {
        GPIOx->BCR = GPIO_Pin;
    }
}

/**
  * @brief  Write data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port data output register.
  * @retval None
  */
void GPIO_WritePort(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
    GPIOx->DOR = PortVal;
}

/**
  * @brief  Lock GPIO Pins configuration.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: where pin can be (GPIO_PIN_0..GPIO_PIN_15) to select the GPIO peripheral.
  * @retval None
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint32_t temp = 0x00010000;

    temp |= GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LOCKR = temp;
    /* Reset LCKK bit */
    GPIOx->LOCKR =  GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LOCKR = temp;
    /* Read LCKK bit*/
    temp = GPIOx->LOCKR;
    /* Read LCKK bit*/
    temp = GPIOx->LOCKR;
}

/**
  * @brief  Select the GPIO pin used as Event output.
  * @param  GPIO_PortSource: This parameter can be GPIO_PORT_SOURCE_GPIOx where x can be (A..I).
  * @param  GPIO_PinSource: This parameter can be GPIO_PINSOURCEx where x can be (0..15).
  * @retval None
  */
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
    uint32_t temp = 0x00;

    temp = AFIO->AFIO_EVR;
    /* Clear the PORT[6:4] and PIN[3:0] bits */
    temp &= EVR_PORTPINCONFIG_MASK;
    temp |= (uint32_t)GPIO_PortSource << 0x04;
    temp |= GPIO_PinSource;
    AFIO->AFIO_EVR = temp;
}

/**
  * @brief  Enable or disable the Event Output.
  * @param  AFIO_Event: AFIO_Event Enable or Disable bit.
  * @param  NewState: new state of the Event output.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_EventOutput_Enable(uint32_t AFIO_Event, TypeState NewState)
{
    /*  Enable or disable the Event Output */   
    if (NewState != DISABLE)
    {
       AFIO->AFIO_EVR  |= AFIO_EVR_EVOE_SET;
    }
    else
    {
       AFIO->AFIO_EVR &= AFIO_EVR_EVOE_RESET;
    }
}

/**
  * @brief  Change the mapping of the specified pin.
  * @param  GPIO_Remap: selects the pin to remap.
  *   This parameter can be one of the following values:
  *     @arg GPIO_REMAP_SPI1
  *     @arg GPIO_REMAP_I2C1
  *     @arg GPIO_REMAP_USART1
  *     @arg GPIO_REMAP_USART2
  *     @arg GPIO_PARTIAL_REMAP_USART3
  *     @arg GPIO_FULL_REMAP_USART3
  *     @arg GPIO_PARTIAL_REMAP_TIMER1
  *     @arg GPIO_FULL_REMAP_TIMER1
  *     @arg GPIO_PARTIAL_REMAP1_TIMER2
  *     @arg GPIO_PARTIAL_REMAP2_TIMER2
  *     @arg GPIO_FULL_REMAP_TIMER2
  *     @arg GPIO_PARTIAL_REMAP_TIMER3
  *     @arg GPIO_FULL_REMAP_TIMER3
  *     @arg GPIO_REMAP_TIMER4
  *     @arg GPIO_REMAP1_CAN1
  *     @arg GPIO_REMAP2_CAN1
  *     @arg GPIO_REMAP_PD01
  *     @arg GPIO_REMAP_TIMER5CH4_LSI
  *     @arg GPIO_REMAP_ADC1_ETRGINJ
  *     @arg GPIO_REMAP_ADC1_ETRGREG
  *     @arg GPIO_REMAP_ADC2_ETRGINJ
  *     @arg GPIO_REMAP_ADC2_ETRGREG
  *     @arg GPIO_REMAP_ETH
  *     @arg GPIO_REMAP_CAN2
  *     @arg GPIO_REMAP_SWJ_NOJTRST
  *     @arg GPIO_REMAP_SWJ_JTAGDISABLE
  *     @arg GPIO_REMAP_SWJ_DISABLE
  *     @arg GPIO_REMAP_SPI3
  *     @arg GPIO_REMAP_TIMER2ITR1_PTP_SOF
  *     @arg GPIO_REMAP_PTP_PPS
  *     @arg GPIO_REMAP_TIMER15
  *     @arg GPIO_REMAP_TIMER16
  *     @arg GPIO_REMAP_TIMER17
  *     @arg GPIO_REMAP_CEC
  *     @arg GPIO_REMAP_TIMER1_DMA
  *     @arg GPIO_REMAP_TIMER9
  *     @arg GPIO_REMAP_TIMER10
  *     @arg GPIO_REMAP_TIMER11
  *     @arg GPIO_REMAP_TIMER13
  *     @arg GPIO_REMAP_TIMER14
  *     @arg GPIO_REMAP_EXMC_NADV
  *     @arg GPIO_REMAP_TIMER67_DAC_DMA
  *     @arg GPIO_REMAP_TIMER12
  *     @arg GPIO_REMAP_MISC
  * @param  NewState: new state of the port pin remapping.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, TypeState NewState)
{
    uint32_t temp = 0x00, temp1 = 0x00, tempreg = 0x00, tempmask = 0x00;

    if((GPIO_Remap & 0x80000000) == 0x80000000)
    {
        tempreg = AFIO->AFIO_PCFR2;
    }
    else
    {
        tempreg = AFIO->AFIO_PCFR1;
    }

    tempmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
    temp = GPIO_Remap & LSB_MASK;

    if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
    {
        tempreg &= DBGAFR_SWJCFG_MASK;
        AFIO->AFIO_PCFR1 &= DBGAFR_SWJCFG_MASK;
    }
    else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
    {
        temp1 = ((uint32_t)0x03) << tempmask;
        tempreg &= ~temp1;
        tempreg |= ~DBGAFR_SWJCFG_MASK;
    }
    else
    {
        tempreg &= ~(temp << ((GPIO_Remap >> 0x15)*0x10));
        tempreg |= ~DBGAFR_SWJCFG_MASK;
    }
    if (NewState != DISABLE)
    {
        tempreg |= (temp << ((GPIO_Remap >> 0x15)*0x10));
    }
    if((GPIO_Remap & AFIO_PCFR2_FIELDS) == AFIO_PCFR2_FIELDS)
    {
        AFIO->AFIO_PCFR2 = tempreg;
    }
    else
    {
        AFIO->AFIO_PCFR1 = tempreg;
    }
}

/**
  * @brief  Change the mapping of the specified pin.
  * @param  Remap_Reg: selects the Register to remap.
    *   This parameter can be one of the following values:
  *     @arg PCFR3
  *     @arg PCFR4
  *     @arg PCFR5
  *     @arg PCFR6
  * @param  GPIO_Remap: selects the pin to remap.
  *   This parameter can be one of the following values:
  *     @arg PCFR3_REMAP_PH01
  *     @arg PCFR3_REMAP_DCI_HSYNC
  *     @arg PCFR3_REMAP_DCI_D13_PG15
  *     @arg PCFR3_REMAP_DCI_D13_PI0
  *     @arg PCFR3_REMAP_DCI_D12
  *     @arg PCFR3_REMAP_DCI_D11_PF10
  *     @arg PCFR3_REMAP_DCI_D11_PH15
  *     @arg PCFR3_REMAP_DCI_D10_PD6
  *     @arg PCFR3_REMAP_DCI_D10_PI3
  *     @arg PCFR3_REMAP_DCI_D9_PH7
  *     @arg PCFR3_REMAP_DCI_D9_PI2
  *     @arg PCFR3_REMAP_DCI_D8_PH6
  *     @arg PCFR3_REMAP_DCI_D8_PI1
  *     @arg PCFR3_REMAP_DCI_D7_PE6
  *     @arg PCFR3_REMAP_DCI_D7_PI7
  *     @arg PCFR3_REMAP_DCI_D6_PE5
  *     @arg PCFR3_REMAP_DCI_D6_PI6
  *     @arg PCFR3_REMAP_DCI_D5_PD3
  *     @arg PCFR3_REMAP_DCI_D5_PI4
  *     @arg PCFR3_REMAP_DCI_D4_PE4
  *     @arg PCFR3_REMAP_DCI_D4_PH14
  *     @arg PCFR3_REMAP_DCI_D3_PE1
  *     @arg PCFR3_REMAP_DCI_D3_PG11
  *     @arg PCFR3_REMAP_DCI_D3_PH12
  *     @arg PCFR3_REMAP_DCI_D2_PE0
  *     @arg PCFR3_REMAP_DCI_D2_PG10
  *     @arg PCFR3_REMAP_DCI_D2_PH11
  *     @arg PCFR3_REMAP_DCI_D1_PC7
  *     @arg PCFR3_REMAP_DCI_D1_PH10
  *     @arg PCFR3_REMAP_DCI_D0_PC6
  *     @arg PCFR3_REMAP_DCI_D0_PH9
  *     @arg PCFR3_REMAP_DCI_VSYNC_PG9
  *     @arg PCFR3_REMAP_DCI_VSYNC_PI5
  *     @arg PCFR4_REMAP_LCD_B3_PG11
  *     @arg PCFR4_REMAP_LCD_B2_PG10
  *     @arg PCFR4_REMAP_LCD_G3_PG10
  *     @arg PCFR4_REMAP_LCD_CLK_PG7
  *     @arg PCFR4_REMAP_LCD_R7_PG6
  *     @arg PCFR4_REMAP_LCD_DE_PF10
  *     @arg PCFR4_REMAP_LCD_R7_PE15
  *     @arg PCFR4_REMAP_LCD_CLK_PE14
  *     @arg PCFR4_REMAP_LCD_DE_PE13 
  *     @arg PCFR4_REMAP_LCD_CLK_PE14
  *     @arg PCFR4_REMAP_LCD_B4_PE12
  *     @arg PCFR4_REMAP_LCD_G3_PE11
  *     @arg PCFR4_REMAP_LCD_G1_PE6
  *     @arg PCFR4_REMAP_LCD_G0_PE5
  *     @arg PCFR4_REMAP_LCD_B0_PE4
  *     @arg PCFR4_REMAP_LCD_B3_PD10
  *     @arg PCFR4_REMAP_LCD_G7_PD3
  *     @arg PCFR4_REMAP_LCD_R2_PC10
  *     @arg PCFR4_REMAP_LCD_G6_PC7
  *     @arg PCFR4_REMAP_LCD_HSYNC_PC6
  *     @arg PCFR4_REMAP_LCD_G5_PB11
  *     @arg PCFR4_REMAP_LCD_G4_PB10
  *     @arg PCFR4_REMAP_LCD_B7_PB9
  *     @arg PCFR4_REMAP_LCD_B6_PB8
  *     @arg PCFR4_REMAP_LCD_R6_PB1
  *     @arg PCFR4_REMAP_LCD_R3_PB0
  *     @arg PCFR4_REMAP_LCD_R5_PA12
  *     @arg PCFR4_REMAP_LCD_R4_PA11
  *     @arg PCFR4_REMAP_LCD_R6_PA8
  *     @arg PCFR4_REMAP_LCD_G2_PA6
  *     @arg PCFR4_REMAP_LCD_VSYNC_PA4
  *     @arg PCFR4_REMAP_LCD_B5_PA3
  *     @arg PCFR5_REMAP_SPI3_MOSI
  *     @arg PCFR5_REMAP_SPI2_SCK
  *     @arg PCFR5_REMAP_LCD_R1_PI3
  *     @arg PCFR5_REMAP_LCD_R0_PH4
  *     @arg PCFR5_REMAP_LCD_HSYNC_PI10
  *     @arg PCFR5_REMAP_LCD_VSYNC_PI9
  *     @arg PCFR5_REMAP_LCD_B7_PI07
  *     @arg PCFR5_REMAP_LCD_B6_PI06
  *     @arg PCFR5_REMAP_LCD_B5_PI5
  *     @arg PCFR5_REMAP_LCD_B4_PI4
  *     @arg PCFR5_REMAP_LCD_G7_PI2
  *     @arg PCFR5_REMAP_LCD_G6_PI1
  *     @arg PCFR5_REMAP_LCD_G5_PI0
  *     @arg PCFR5_REMAP_LCD_G4_PH15
  *     @arg PCFR5_REMAP_LCD_G3_PH14
  *     @arg PCFR5_REMAP_LCD_G2_PH13
  *     @arg PCFR5_REMAP_LCD_R6_PH12
  *     @arg PCFR5_REMAP_LCD_R5_PH11
  *     @arg PCFR5_REMAP_LCD_R4_PH10
  *     @arg PCFR5_REMAP_LCD_R3_PH9
  *     @arg PCFR5_REMAP_LCD_R2_PH8
  *     @arg PCFR5_REMAP_LCD_R1_PH3
  *     @arg PCFR5_REMAP_LCD_R0_PH2
  *     @arg PCFR5_REMAP_LCD_B1_PG12
  *     @arg PCFR5_REMAP_LCD_B4_PG12
  *     @arg PCFR6_REMAP_EXMC_SDNE1_PB6
  *     @arg PCFR6_REMAP_EXMC_SDNE0_PC2
  *     @arg PCFR6_REMAP_EXMC_SDCKE1_PB5
  *     @arg PCFR6_REMAP_EXMC_SDCKE0_PC3
  *     @arg PCFR6_REMAP_EXMC_SDNWE_PC0
  *     @arg PCFR6_REMAP_USART6_RX_PG9
  *     @arg PCFR6_REMAP_USART6_TX_PG14
  *     @arg PCFR6_REMAP_USART6_CTS_PG13
  *     @arg PCFR6_REMAP_USART6_RTS_PG12
  *     @arg PCFR6_REMAP_USART6_CK_PG7
  *     @arg PCFR6_REMAP_USART7
  *     @arg PCFR6_REMAP_ETH_RX_HI
  *     @arg PCFR6_REMAP_ETH_CRSCOL
  *     @arg PCFR6_REMAP_ETH_TXD01
  *     @arg PCFR6_REMAP_ETH_PPS_HI
  *     @arg PCFR6_REMAP_ETH_TXD3
  *     @arg PCFR6_REMAP_CAN_HI
  *     @arg PCFR6_REMAP_TM12
  *     @arg PCFR6_REMAP_USART4
  *     @arg PCFR6_REMAP_SPI2_IO_1
  *     @arg PCFR6_REMAP_SPI2_IO_2
  *     @arg PCFR6_REMAP_SPI2_NSCK_1
  *     @arg PCFR6_REMAP_SPI2_NSCK_2
  *     @arg PCFR6_REMAP_I2C2_1
  *     @arg PCFR6_REMAP_I2C2_2
  *     @arg PCFR6_REMAP_TM8
  *     @arg PCFR6_REMAP_TM8_CHN_1
  *     @arg PCFR6_REMAP_TM8_CHN_2
  *     @arg PCFR6_REMAP_TM5
  *     @arg PCFR6_REMAP_TM2_CH1
  *     @arg PCFR6_REMAP_I2C3_REMAP2_EN
  *     @arg PCFR6_REMAP_I2C3_REMAP1_EN
  * @param  NewState: new state of the port pin remapping.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_PinRemapConfig2(uint8_t Remap_Reg,uint32_t GPIO_Remap, TypeState NewState)
{
    uint32_t temp = 0x00;

    if(NewState != DISABLE)
    {
        /* Select AFIO_PCFR3 */
        if(Remap_Reg == PCFR3)
        {
            temp = AFIO->AFIO_PCFR3;
            temp |= GPIO_Remap;
            AFIO->AFIO_PCFR3 = temp;
        }
        /* Select AFIO_PCFR4 */
        else if(Remap_Reg == PCFR4)
        {
            temp = AFIO->AFIO_PCFR4;
            temp |= GPIO_Remap;
            AFIO->AFIO_PCFR4 = temp;
        }
        /* Select AFIO_PCFR5 */
        else if(Remap_Reg == PCFR5)
        {
            temp = AFIO->AFIO_PCFR5;
            temp |= GPIO_Remap;
            AFIO->AFIO_PCFR5 = temp;
        }
        /* Select AFIO_PCFR6 */
        else if(Remap_Reg == PCFR6)
        {
            temp = AFIO->AFIO_PCFR6;
            temp |= GPIO_Remap;
            AFIO->AFIO_PCFR6 = temp;
        }
    }
}

/**
  * @brief  Select the GPIO pin used as EXTI Line.
  * @param  GPIO_PortSource: selects the GPIO port to be used as source for EXTI lines.
  *   This parameter can be GPIO_PORT_SOURCE_GPIOx where x can be (A..I).
  * @param  GPIO_PinSource: specifies the EXTI line to be configured.
  *   This parameter can be GPIO_PINSOURCEx where x can be (0..15).
  * @retval None
  */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
    uint32_t temp = 0x00;

    temp = ((uint32_t)0x0f) << (AFIO_ESSR_PIN_FIELDS * (GPIO_PinSource & (uint8_t)0x03));
    /* Select EXTI0->EXTI3 bit */
    if(GPIO_PinSource < AFIO_ESSR1_BITS_FIELDS)
    {
        AFIO->AFIO_ESSR1 &= ~temp;
        AFIO->AFIO_ESSR1 |= (((uint32_t)GPIO_PortSource) << (AFIO_ESSR_PIN_FIELDS * (GPIO_PinSource & (uint8_t)0x03)));  
    }
    /* Select EXTI4->EXTI7 bit */
    else if(GPIO_PinSource < AFIO_ESSR2_BITS_FIELDS)
    {
        AFIO->AFIO_ESSR2 &= ~temp;
        AFIO->AFIO_ESSR2 |= (((uint32_t)GPIO_PortSource) << (AFIO_ESSR_PIN_FIELDS * (GPIO_PinSource & (uint8_t)0x03)));  
    }
    /* Select EXTI8->EXTI11 bit */
    else if(GPIO_PinSource < AFIO_ESSR3_BITS_FIELDS)
    {
        AFIO->AFIO_ESSR3 &= ~temp;
        AFIO->AFIO_ESSR3 |= (((uint32_t)GPIO_PortSource) << (AFIO_ESSR_PIN_FIELDS * (GPIO_PinSource & (uint8_t)0x03))); 
    }
    /* Select EXTI12->EXTI15 bit */
    else
    {
        AFIO->AFIO_ESSR4 &= ~temp;
        AFIO->AFIO_ESSR4 |= (((uint32_t)GPIO_PortSource) << (AFIO_ESSR_PIN_FIELDS * (GPIO_PinSource & (uint8_t)0x03))); 
    }
}

/**
  * @brief  Select the Ethernet media interface.
  * @param  GPIO_ETH_MediaInterface: specifies the Media Interface mode.
  *   This parameter can be one of the following values:
  *     @arg GPIO_ETH_MEDIA_INTERFACE_MII: MII mode
  *     @arg GPIO_ETH_MEDIA_INTERFACE_RMII: RMII mode
  * @retval None
  */
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface)
{
    uint32_t temp = 0;
    temp = AFIO->AFIO_PCFR1;
    /* Clear MII_RMII_SEL bit */
    temp &= ~((uint32_t)AFIO_PCFR1_MII_RMII);
    /* Configure MII_RMII bit according to GPIO_ETH_MediaInterface value */
    temp |= GPIO_ETH_MediaInterface;
    /* Store the new value */
    AFIO->AFIO_PCFR1 = temp;
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
