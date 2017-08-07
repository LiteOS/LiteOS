/**
  ******************************************************************************
  * @file    EXMC/EXMC_LCD/EXMC_LCD.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   EXMC drive LCD.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "EXMC_LCD.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configure the LCD GPIO ports.
  * @param  None
  * @retval None
  */
void LCD_GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;
    /* Set PD0(D2), PD1(D3), PD4(NOE), PD5(NWE), PD8(D13), PD9(D14), PD10(D15), PD14(D0), PD15(D1)
       as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | 
                                  GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* Set PE7(D4), PE8(D5), PE9(D6), PE10(D7), PE11(D8), PE12(D9), PE13(D10),
       PE14(D11), PE15(D12) as alternate function push pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | 
                                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                  GPIO_PIN_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* PD7-NE1 configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* PE2-A23-RS configuration,1:R/W data, 0:R/W Command*/
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2; 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

/**
  * @brief  Configure the EXMC.
  * @param  None
  * @retval None
  */
void EXMC_LCD_Init(void)
{
    EXMC_NORSRAMInitPara  EXMC_NORSRAMInitParaStructure;
    EXMC_NORSRAMTimingInitPara  p;

    /* EXMC Bank1 NORSRAM1 timing configuration */
    p.EXMC_AsynAddressSetupTime = 2;
    p.EXMC_AsynAddressHoldTime = 0;
    p.EXMC_AsynDataSetupTime = 5;
    p.EXMC_BusLatency = 0;
    p.EXMC_SynCLKDivision = 0;
    p.EXMC_SynDataLatency = 0;
    p.EXMC_AsynAccessMode = EXMC_ACCESS_MODE_A;
    
    EXMC_NORSRAMInitParaStructure.EXMC_NORSRAMBank = EXMC_BANK1_NORSRAM1;
    EXMC_NORSRAMInitParaStructure.EXMC_AddressDataMux = EXMC_ADDRESS_DATA_MUX_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_MemoryType = EXMC_MEMORY_TYPE_SRAM;
    EXMC_NORSRAMInitParaStructure.EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_16B;
    EXMC_NORSRAMInitParaStructure.EXMC_BurstMode = EXMC_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_AsynWait = EXMC_ASYN_WAIT_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_NWAITPolarity = EXMC_NWAIT_POLARITY_LOW;
    EXMC_NORSRAMInitParaStructure.EXMC_WrapBurstMode = EXMC_WRAP_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_NWAITConfig = EXMC_NWAIT_CONFIG_BEFORE;
    EXMC_NORSRAMInitParaStructure.EXMC_MemoryWrite = EXMC_MEMORY_WRITE_ENABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_NWAITSignal = EXMC_NWAIT_SIGNAL_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_ExtendedMode = EXMC_EXTENDED_MODE_DISABLE;
    EXMC_NORSRAMInitParaStructure.EXMC_WriteMode = EXMC_ASYN_WRITE;
    EXMC_NORSRAMInitParaStructure.EXMC_ReadWriteTimingParaStruct = &p;
    EXMC_NORSRAMInitParaStructure.EXMC_WriteTimingParaStruct = &p;
    
    EXMC_NORSRAM_Init(&EXMC_NORSRAMInitParaStructure);  
    
    /* BANK 1 enable */
    EXMC_NORSRAM_Enable(EXMC_BANK1_NORSRAM1, ENABLE);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
