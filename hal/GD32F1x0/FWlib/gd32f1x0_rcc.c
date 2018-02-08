/**
  ******************************************************************************
  * @brief   RCC functions of the firmware library.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f1x0_rcc.h"

/** @addtogroup GD32F1x0_Firmware
  * @{
  */

/** @defgroup RCC 
  * @brief RCC driver modules
  * @{
  */ 

/** @defgroup RCC_Private_Defines
  * @{
  */
/* ---------------------- RCC registers mask -------------------------------- */
#define  GCCR_HSIEN_SET                        ((uint32_t)0x00000001)    /*!< RCC GCCR_HSIEN mask                              */
#ifdef GD32F170_190
#define  GCFGR_RESET                           ((uint32_t)0x08FF000C)    /*!< RCC GCFGR_Reset mask                             */
#else
#define  GCFGR_RESET                           ((uint32_t)0x083F000C)    /*!< RCC GCFGR_Reset mask                             */
#endif /* GD32F170_190 */
#define  GCCR_HSEEN_CKMEN_PLLEN_RESET          ((uint32_t)0xFEF6FFFF)    /*!< RCC GCCR_HSEEN_CKMEN_PLLEN mask                  */
#define  GCCR_HSEBPS_RESET                     ((uint32_t)0xFFFBFFFF)    /*!< RCC GCCR_HSEBPS mask                             */
#define  GCFGR_PLLSEL_PLLPREDV_PLLMF_RESET     ((uint32_t)0xF7C0FFFF)    /*!< RCC GCFGR_PLLSEL_PLLPREDV_PLLMF mask             */
#define  GCFGR2_HSEPREDIV_RESET                ((uint32_t)0xFFFFFFF0)    /*!< RCC GCFGR2_HSEPREDIV mask                        */
#define  GCFGR3_USART1SEL_CECSEL_ADCSEL_RESET   ((uint32_t)0xFFFFFEBC)   /*!< RCC GCFGR3_USART1SEL_CECSEL_ADCSEL mask          */
#ifdef GD32F170_190/* GD32F170_190 */
#define  GCCR2_HSI28_RESET                     ((uint32_t)0xFFFFFFFE)    /*!< RCC GCCR2_HSI28 mask                             */
#define  GCCR2_HSI28ADJ_OFFSET                 ((uint32_t)0x00000003)    /*!< RCC GCCR2_HSI28ADJ offset                        */
#define  GCFGR3_HSI28ADC_OFFSET                ((uint32_t)0x00000010)    /*!< RCC GCFGR3_HSI28ADC mask                         */
#else
#define  GCCR2_HSI14_RESET                     ((uint32_t)0xFFFFFFFE)    /*!< RCC GCCR2_HSI14 mask                             */
#define  GCCR2_HSI14ADJ_OFFSET                 ((uint32_t)0x00000003)    /*!< RCC GCCR2_HSI14ADJ mask                          */
#define  GCFGR3_HSI14ADC_OFFSET                ((uint32_t)0x00000010)    /*!< RCC GCFGR3_HSI14ADC mask                         */
#endif /* GD32F170_190 */
#define  GCIR_INT_RESET                        ((uint32_t)0x00000000)    /*!< RCC GCIR_INT mask                                */
#define  GCCR_HSIADJ_OFFSET                    ((uint32_t)0x00000003)    /*!< RCC GCCR_HSIADJ mask                             */
#define  GCCR_BYTE2_ADDRESS                    ((uint32_t)0x40021002)    /*!< GCCR register byte 2 (Bits[23:16]) base address  */
#define  GCFGR_BYTE3_ADDRESS                   ((uint32_t)0x40021007)    /*!< GCFGR register byte 3 (Bits[31:23]) base address */
#define  GCIR_BYTE1_ADDRESS                    ((uint32_t)0x40021009)    /*!< GCIR register byte 1 (Bits[15:8]) base address   */
#define  GCIR_BYTE2_ADDRESS                    ((uint32_t)0x4002100A)    /*!< GCIR register byte 2 (Bits[23:16]) base address  */
#define  FLAG_MASK                             ((uint8_t )0x1F)          /*!< RCC flag mask                                    */
#define  HSI_CLOCK_DIVIDED_2                   ((uint32_t)0x00000001)    /*!< RCC HSI clock divided by 2 mask                  */
#define  RCC_VC_KEY_ULOCKPOWER                 ((uint32_t)0x1A2B3C4D)    /*!< RCC RCC_VC_KEY_ULOCKPOWER mask                   */
#define  RCC_VC_KEY_CLEAR                      ((uint32_t)0x00000000)    /*!< RCC RCC_VC_KEY_CLEAR mask                        */
#define  RCC_GCFGR_PLLMF_3_0                   ((uint32_t)0x003C0000)    /*!< PLLMF[3:0] bits                                  */
#ifdef GD32F170_190
#define  GCFGR4_CKOUT2DIV_CKOUT2SRC_RESET      ((uint32_t)0xFFFFC0F8)    /*!< RCC GCFGR4_CKOUT2DIV_CKOUT2SRC mask              */
#define  RCC_GCFGR4_CKOUT2DIV_OFFSET           ((uint32_t)0x00000008)    /*!< RCC_GCFGR4_CKOUT2DIV offset                      */
#endif /* GD32F170_190 */

static __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

/**
  * @}
  */

/** @defgroup RCC_Private_Functions
  * @{
  */

/**
  * @brief  Reset the RCC clock configuration.
  * @param  None
  * @retval None
  */
void RCC_DeInit(void)
{
    /* Set RCC GCCR_HSIEN mask */
    RCC->GCCR |= GCCR_HSIEN_SET;
    
    /* Reset SCS[1:0], AHBPS[3:0], APB1PS[2:0],APB2PS[2:0], ADCPS[1:0], USBPS[1:0], CKOTUSRC[2:0], CKOUTDIV, and PLLDV bits*/
    RCC->GCFGR &= GCFGR_RESET;
    
    /* Reset HSEEN, CKMEN and PLLEN bits */
    RCC->GCCR &= GCCR_HSEEN_CKMEN_PLLEN_RESET;
    
    /* Reset HSEBPS bit */
    RCC->GCCR &= GCCR_HSEBPS_RESET;
    
    /* Reset PLLSEL, PLLPREDV and PLLMF[4:0] bits */
    RCC->GCFGR &= GCFGR_PLLSEL_PLLPREDV_PLLMF_RESET;
    
    /* Reset HSEPREDIV[3:0] bits */
    RCC->GCFGR2 &= GCFGR2_HSEPREDIV_RESET;
    
    /* Reset USARTSEL[1:0], CECSEL and ADCSEL bits */
    RCC->GCFGR3 &= GCFGR3_USART1SEL_CECSEL_ADCSEL_RESET;

#ifdef GD32F170_190
    /* Reset HSI28 bit */
    RCC->GCCR2 &= GCCR2_HSI28_RESET;
#else
    /* Reset HSI14 bit */
    RCC->GCCR2 &= GCCR2_HSI14_RESET;
#endif /* GD32F170_190 */

    /* Disable all interrupts */
    RCC->GCIR = GCIR_INT_RESET;
    
#ifdef GD32F170_190
    /* Reset CKOUT2DIV[5:0], CKOUT2SRC[2:0] */
    RCC->GCFGR4 = GCFGR4_CKOUT2DIV_CKOUT2SRC_RESET;
#endif /* GD32F170_190 */
}

/**
  * @brief  Configure the External High Speed oscillator (HSE).
  * @param  RCC_HSE: specify the new state of HSE.
  *   This parameter can be one of the following values:
  *     @arg RCC_HSE_OFF: turn off the HSE
  *     @arg RCC_HSE_ON: turn on the HSE
  *     @arg RCC_HSE_BYPASS: HSE bypassed with external clock
  * @retval None
  */
void RCC_HSEConfig(uint8_t RCC_HSE)
{
    /* Reset HSEEN and HSESTB bits */
    *(__IO uint8_t *) GCCR_BYTE2_ADDRESS = RCC_HSE_OFF;
    
    /* Set the new state of HSE */
    *(__IO uint8_t *) GCCR_BYTE2_ADDRESS = RCC_HSE;
}

/**
  * @brief  Wait for HSE start-up.
  * @param  None
  * @retval The HSE start-up result(SUCCESS or ERROR)
  */
TypeState RCC_WaitForHSEStartUp(void)
{
    __IO uint32_t HSE_StartOk_Counter = 0;
    TypeState HSEState = RESET;
    
    /* Wait until HSE is ready and if timeout to exit */
    while((HSE_StartOk_Counter != HSE_STARTUP_TIMEOUT) && (HSEState == RESET))
    {
        HSEState = RCC_GetBitState(RCC_FLAG_HSESTB);
        HSE_StartOk_Counter++;  
    }
    
    if(RCC_GetBitState(RCC_FLAG_HSESTB) != RESET)
    {
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

/**
  * @brief  Adjust the Internal High Speed oscillator (HSI) calibration value.
  * @param  HSICalibrationValue: the HSI calibration value.
  *   This parameter must be between 0 and 0x1F.
  * @retval None
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
    uint32_t temp_adjust = 0;
    
    temp_adjust = RCC->GCCR;
    
    /* Clear HSIADJ[4:0] bits */
    temp_adjust &= ~RCC_GCCR_HSIADJ;
    
    /* Set HSIADJ[4:0] bits according to HSICalibrationValue value */
    temp_adjust |= (uint32_t)HSICalibrationValue << GCCR_HSIADJ_OFFSET ;
    
    /* Store the calibration value */
    RCC->GCCR = temp_adjust;
}

/**
  * @brief  Enable or disable the Internal High Speed oscillator (HSI).
  * @param  NewValue: new value of the HSI.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSI_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->GCCR |= RCC_GCCR_HSIEN;
    }
    else
    {
        RCC->GCCR &= ~RCC_GCCR_HSIEN;
    }
}

#ifdef GD32F170_190
/**
  * @brief  Adjust the Internal High Speed oscillator (HSI28) calibration value.
  * @param  HSI28CalibrationValue: the HSI28 calibration value.
  *   This parameter must be  between 0 and 0x1F.
  * @retval None
  */
void RCC_AdjustHSI28CalibrationValue(uint8_t HSI28CalibrationValue)
{
    uint32_t temp_adjust = 0;
    
    temp_adjust = RCC->GCCR2;
    
    /* Reset HSI28ADJ[4:0] bits */
    temp_adjust &= ~RCC_GCCR2_HSI28ADJ;
    
    /* Set the HSI28ADJ[4:0] bits according to HSI28CalibrationValue value */
    temp_adjust |= (uint32_t)HSI28CalibrationValue << GCCR2_HSI28ADJ_OFFSET ;
    
    /* Store the calibration value */
    RCC->GCCR2 = temp_adjust;
}

/**
  * @brief  Enable or disable the Internal High Speed oscillator(HSI28).
  * @param  NewValue: new value of the HSI.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSI28_Enable(TypeState NewValue)
{
    if(NewValue != DISABLE)
    {
        RCC->GCCR2 |= RCC_GCCR2_HSI28EN;
    }
    else
    {
        RCC->GCCR2 &= ~RCC_GCCR2_HSI28EN;
    }
}
#else
/**
  * @brief  Adjust the Internal High Speed oscillator (HSI14) calibration value.
  * @param  HSI14CalibrationValue: the HSI14 calibration value.
  *   This parameter must be  between 0 and 0x1F.
  * @retval None
  */
void RCC_AdjustHSI14CalibrationValue(uint8_t HSI14CalibrationValue)
{
    uint32_t temp_adjust = 0;
    
    temp_adjust = RCC->GCCR2;
    
    /* Reset HSI14ADJ[4:0] bits */
    temp_adjust &= ~RCC_GCCR2_HSI14ADJ;
    
    /* Set the HSI14ADJ[4:0] bits according to HSI14CalibrationValue value */
    temp_adjust |= (uint32_t)HSI14CalibrationValue << GCCR2_HSI14ADJ_OFFSET ;
    
    /* Store the calibration value */
    RCC->GCCR2 = temp_adjust;
}

/**
  * @brief  Enable or disable the Internal High Speed oscillator(HSI14).
  * @param  NewValue: new value of the HSI.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSI14_Enable(TypeState NewValue)
{
    if(NewValue != DISABLE)
    {
        RCC->GCCR2 |= RCC_GCCR2_HSI14EN;
    }
    else
    {
        RCC->GCCR2 &= ~RCC_GCCR2_HSI14EN;
    }
}
#endif /* GD32F170_190 */

/**
  * @brief  Configure the External Low Speed oscillator (LSE).
  * @param  RCC_LSE: specify the new state of the LSE.
  *   This parameter can be one of the following values:
  *     @arg RCC_LSE_OFF: turn off the LSE 
  *     @arg RCC_LSE_EN: turn on the LSE 
  *     @arg RCC_LSE_Bypass: LSE bypassed with external clock
  * @retval None
  */
void RCC_LSEConfig(uint32_t RCC_LSE)
{
    /* Reset LSEEN and LSEBPS bits before configuring the LSE */
    RCC->BDCR &= ~(RCC_BDCR_LSEEN);
    RCC->BDCR &= ~(RCC_BDCR_LSEBPS);
    
    /* Configure LSE */
    RCC->BDCR |= RCC_LSE;
}

/**
  * @brief  Configure the External Low Speed oscillator (LSE) drive capability.
  * @param  RCC_LSEDrive: specify the new state of the LSE drive capability.
  *   This parameter can be one of the following values:
  *     @arg RCC_LSEDRIVE_LOW: LSE low drive capability.
  *     @arg RCC_LSEDRIVE_MEDIUMLOW: LSE medium low drive capability.
  *     @arg RCC_LSEDRIVE_MEDIUMHIGH: LSE medium high drive capability.
  *     @arg RCC_LSEDRIVE_HIGH: LSE high drive capability.
  * @retval None
  */
void RCC_LSEDriveConfig(uint32_t RCC_LSEDrive)
{
    /* Clear LSEDRI[1:0] bits */
    RCC->BDCR &= ~(RCC_BDCR_LSEDRI);
    
    /* Set the LSE drive capability */
    RCC->BDCR |= RCC_LSEDrive;
}

/**
  * @brief  Enable or disable the Internal Low Speed oscillator (LSI).
  * @param  NewValue: new value of the LSI.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_LSI_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->GCSR |= RCC_GCSR_LSIEN;
    }
    else
    {
        RCC->GCSR &= ~RCC_GCSR_LSIEN;
    }
}

/**
  * @brief  Configure the PLL clock source and multiplication factor.
  * @param  RCC_PLLSelect: specify the PLL clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_PLLSOURCE_HSI_DIV2: HSI divided by 2 selected as PLL clock source
  *     @arg RCC_PLLSOURCE_HSEPREDIV: HSE selected as PLL clock source
  * @note   if using HSE as PLL source, the minimum clock frequency for PLL is 2 MHz.
  * @param  RCC_PLLMF: specify the PLL multiplication factor,  RCC_PLLMUL_x where x:[2,32] 
  * @retval None
  */
void RCC_PLLConfig(uint32_t RCC_PLLSelect, uint32_t RCC_PLLMF)
{
    /* Clear PLLSEL [16] and PLLMF [27] [21:18] bits */
    RCC->GCFGR &= ~(RCC_GCFGR_PLLMF | RCC_GCFGR_PLLSEL);
    
    /* Set the PLLSEL and PLLMF */
    RCC->GCFGR |= (uint32_t)(RCC_PLLSelect | RCC_PLLMF);
}

/**
  * @brief  Enable or disable the PLL.
  * @param  NewValue: new value of the PLL.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_PLL_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->GCCR |= RCC_GCCR_PLLEN;
    }
    else
    {
        RCC->GCCR &= ~RCC_GCCR_PLLEN;
    }
}

/**
  * @brief  Configure the HSEPREDV division factor.
  * @param  RCC_HSEPREDV1_Div: specify the HSE division factor.
  *   This parameter can be RCC_HSEPREDV1_DIVx where x:[1,16]
  * @retval None
  */
void RCC_HSEPREDVConfig(uint32_t RCC_HSEPREDV1_Div)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR2;
    /* Clear HSEPREDV[3:0] bits */
    temp &= ~(RCC_GCFGR2_HSEPREDV1);
    /* Set the HSEPREDV division factor */
    temp |= RCC_HSEPREDV1_Div;
    /* Store the new value */
    RCC->GCFGR2 = temp;
}

/**
  * @brief  Enable or disable the Clock Security System.
  * @param  NewValue: new value of the Clock Security System.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_HSEClockMonitor_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->GCCR |= RCC_GCCR_CKMEN;
    }
    else
    {
        RCC->GCCR &= ~RCC_GCCR_CKMEN;
    }
}

/**
  * @brief  Select the clock source to output on CKOUTSRC and the corresponding prescaler.
  * @param  RCC_CKOUTSRC: specify the clock source to output.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_CKOUTSRC_NOCLOCK: No clock selected.
  *     @arg RCC_CKOUTSRC_HSI14: selecte HSI14 to output.
  *     @arg RCC_CKOUTSRC_LSI: selecte LSI to output.
  *     @arg RCC_CKOUTSRC_LSE: selecte LSE to output.
  *     @arg RCC_CKOUTSRC_SYSCLK: selecte System clock to output.
  *     @arg RCC_CKOUTSRC_HSI: selecte HSI to output.
  *     @arg RCC_CKOUTSRC_HSE: selecte HSE to output.
  *     @arg RCC_CKOUTSRC_PLLCLK_DIV2: PLL clock divided by 2 selecte.
  *     @arg RCC_CKOUTSRC_PLLCLK_DIV1: PLL clock selected.
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_CKOUTSRC_NOCLOCK: No clock selected.
  *     @arg RCC_CKOUTSRC_HSI28: selecte HSI28 to output.
  *     @arg RCC_CKOUTSRC_LSI: selecte LSI to output.
  *     @arg RCC_CKOUTSRC_LSE: selecte LSE to output.
  *     @arg RCC_CKOUTSRC_SYSCLK: selecte System clock to output.
  *     @arg RCC_CKOUTSRC_HSI: selecte HSI to output.
  *     @arg RCC_CKOUTSRC_HSE: selecte HSE to output.
  *     @arg RCC_CKOUTSRC_PLLCLK_DIV2: PLL clock divided by 2 selecte.
  *     @arg RCC_CKOUTSRC_PLLCLK_DIV1: PLL clock selected.
  *
  * @param  RCC_CKOUTDIV: specify the corresponding prescaler on CKOUTSRC.
  *   This parameter can be one of the following values:
  *     @arg RCC_CKOUTDIV_1: CK_OUT clock is divided by 1.
  *     @arg RCC_CKOUTDIV_2: CK_OUT clock is divided by 2.
  *     @arg RCC_CKOUTDIV_4: CK_OUT clock is divided by 4.
  *     @arg RCC_CKOUTDIV_8: CK_OUT clock is divided by 8.
  *     @arg RCC_CKOUTDIV_16: CK_OUT clock is divided by 16.
  *     @arg RCC_CKOUTDIV_32: CK_OUT clock is divided by 32.
  *     @arg RCC_CKOUTDIV_64: CK_OUT clock is divided by 64.
  *     @arg RCC_CKOUTDIV_128: CK_OUT clock is divided by 128.    
  * @retval None
  */
void RCC_CKOUTSRCConfig(uint32_t RCC_CKOUTSRC, uint32_t RCC_CKOUTDIV)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR;
    /* Clear CKOUTSRC[2:0] bits */
    temp &= ~(RCC_GCFGR_CKOUTDIV | RCC_GCFGR_CKOUTSRC | RCC_GCFGR_PLLDV);
    /* Set the RCC_CKOUTSRC and RCC_CKOUTDIV */
    temp |= (RCC_CKOUTDIV | RCC_CKOUTSRC);
    /* Store the new value */
    RCC->GCFGR = temp;
}

#ifdef GD32F170_190
/**
  * @brief  Select the clock source to output on CKOUT2SRC and the corresponding prescaler.
  * @param  RCC_CKOUT2SRC: specify the clock source to output.
  *   This parameter can be one of the following values:
  *     @arg RCC_CKOUT2SRC_NOCLOCK: No clock selected.
  *     @arg RCC_CKOUT2SRC_HSI28: selecte HSI28 to output.
  *     @arg RCC_CKOUT2SRC_LSI: selecte LSI to output.
  *     @arg RCC_CKOUT2SRC_LSE: selecte LSE to output.
  *     @arg RCC_CKOUT2SRC_SYSCLK: selecte System clock to output.
  *     @arg RCC_CKOUT2SRC_HSI: selecte HSI to output.
  *     @arg RCC_CKOUT2SRC_HSE: selecte HSE to output.
  *     @arg RCC_CKOUT2SRC_PLLCLK_DIV2: PLL clock divided by 2 selecte.
  *     @arg RCC_CKOUT2SRC_PLLCLK_DIV1: PLL clock selected.
  * @param  RCC_CKOUT2DIV: specify the corresponding prescaler on CKOUT2SRC.
  *   This parameter can be one of 1 to 64:
  * @retval None
  */
void RCC_CKOUT2SRCConfig(uint32_t RCC_CKOUT2SRC, uint32_t RCC_CKOUT2DIV)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR4;
    /* Clear CKOUT2SRC[2:0], CKOUT2DIV[5:0] bits */
    temp &= ~(RCC_GCFGR4_CKOUT2SRC | RCC_GCFGR4_CKOUT2DIV);
    
    /* if the RCC_CKOUT2SRC is PLL, set the PLLDV of RCC->GCFGR*/
    if(RCC_CKOUT2SRC == RCC_CKOUT2SRC_PLLCLK_DIV2)
    {
        RCC->GCFGR &= ~RCC_GCFGR_PLLDV;
        RCC_CKOUT2SRC = RCC_GCFGR4_CKOUT2SRC_PLL;
    }
    else if(RCC_CKOUT2SRC == RCC_CKOUT2SRC_PLLCLK_DIV1)
    {
        RCC->GCFGR |= RCC_GCFGR_PLLDV;
        RCC_CKOUT2SRC = RCC_GCFGR4_CKOUT2SRC_PLL; 
    }
    
    /* Set the RCC_CKOUT2SRC and RCC_CKOUT2DIV */
    temp |= (((RCC_CKOUT2DIV - 1) << RCC_GCFGR4_CKOUT2DIV_OFFSET) | RCC_CKOUT2SRC);
    /* Store the new value */
    RCC->GCFGR4 = temp;
}
#endif /* GD32F170_190 */

/**
  * @brief  Configure the system clock (CK_SYS) 
  * @param  RCC_SYSCLKSource: specify the system clock source 
  *   This parameter can be one of the following values:
  *     @arg RCC_SYSCLKSOURCE_HSI:    selecte HSI as CK_SYS source
  *     @arg RCC_SYSCLKSOURCE_HSE:    selecte HSE as CK_SYS source
  *     @arg RCC_SYSCLKSOURCE_PLLCLK: selecte PLL as CK_SYS source
  * @retval None
  */
void RCC_CK_SYSConfig(uint32_t RCC_SYSCLKSource)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR;
    
    /* Clear SCS[1:0] bits */
    temp &= ~RCC_GCFGR_SCS;
    
    /* Set SCS[1:0] bits according to RCC_SYSCLKSource value */
    temp |= RCC_SYSCLKSource;
    
    /* Store the new value */
    RCC->GCFGR = temp;
}

/**
  * @brief  Get the system clock source.
  * @param  None
  * @retval Get the system clock source. The returned value can be one 
  *         of the following values:
  *           - 0x00: HSI used as CK_SYS source
  *           - 0x04: HSE used as CK_SYS source  
  *           - 0x08: PLL used as CK_SYS source
  */
uint8_t RCC_GetCK_SYSSource(void)
{
    return ((uint8_t)(RCC->GCFGR & RCC_GCFGR_SCSS));
}

/**
  * @brief  Configure the AHB clock.
  * @param  RCC_CK_SYSDiv: specify the AHB clock divider. This clock is derived from 
  *         the system clock (CK_SYS).
  *   This parameter can be one of the following values:
  *     @arg RCC_SYSCLK_DIV1:   AHB clock = CK_SYS
  *     @arg RCC_SYSCLK_DIV2:   AHB clock = CK_SYS/2
  *     @arg RCC_SYSCLK_DIV4:   AHB clock = CK_SYS/4
  *     @arg RCC_SYSCLK_DIV8:   AHB clock = CK_SYS/8
  *     @arg RCC_SYSCLK_DIV16:  AHB clock = CK_SYS/16
  *     @arg RCC_SYSCLK_DIV64:  AHB clock = CK_SYS/64
  *     @arg RCC_SYSCLK_DIV128: AHB clock = CK_SYS/128
  *     @arg RCC_SYSCLK_DIV256: AHB clock = CK_SYS/256
  *     @arg RCC_SYSCLK_DIV512: AHB clock = CK_SYS/512
  * @retval None
  */
void RCC_AHBConfig(uint32_t RCC_CK_SYSDiv)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR;
    
    /* Clear AHBPS[3:0] bits */
    temp &= ~RCC_GCFGR_AHBPS;
    
    /* Set AHBPS[3:0] bits according to RCC_CK_SYSDiv value */
    temp |= RCC_CK_SYSDiv;
    
    /* Store the new value */
    RCC->GCFGR = temp;
}

/**
  * @brief  Configure the APB1 clock.
  * @param  RCC_APB1: specify the APB1 clock divider. This clock is derived from 
  *         the AHB clock.
  *   This parameter can be one of the following values:
  *     @arg RCC_APB1AHB_DIV1: APB1 clock = AHB
  *     @arg RCC_APB1AHB_DIV2: APB1 clock = AHB/2
  *     @arg RCC_APB1AHB_DIV4: APB1 clock = AHB/4
  *     @arg RCC_APB1AHB_DIV8: APB1 clock = AHB/8
  *     @arg RCC_APB1AHB_DIV16: APB1 clock = AHB/16
  * @retval None
  */
void RCC_APB1Config(uint32_t RCC_APB1)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR;
    
    /* Clear APB1PS[2:0] bits */
    temp &= ~RCC_GCFGR_APB1PS;
    
    /* Set APB1PS[2:0] bits according to RCC_APB1 value */
    temp |= RCC_APB1;
    
    /* Store the new value */
    RCC->GCFGR = temp;
}

/**
  * @brief  Configure the APB2 clock.
  * @param  RCC_APB2: specify the APB2 clock divider. This clock is derived from 
  *         the AHB clock.
  *   This parameter can be one of the following values:
  *     @arg RCC_APB2AHB_DIV1: APB2 clock = AHB
  *     @arg RCC_APB2AHB_DIV2: APB2 clock = AHB/2
  *     @arg RCC_APB2AHB_DIV4: APB2 clock = AHB/4
  *     @arg RCC_APB2AHB_DIV8: APB2 clock = AHB/8
  *     @arg RCC_APB2AHB_DIV16: APB2 clock = AHB/16
  * @retval None
  */
void RCC_APB2Config(uint32_t RCC_APB2)
{
    uint32_t temp = 0;
    
    temp = RCC->GCFGR;
    
    /* Clear APB2PS[2:0] bits */
    temp &= ~RCC_GCFGR_APB2PS;
    
    /* Set APB2PS[2:0] bits according to RCC_APB2 value */
    temp |= RCC_APB2;
    
    /* Store the new value */
    RCC->GCFGR = temp;
}

/**
  * @brief  Configure the ADC clock (ADCCLK).
  * @param  RCC_ADCCLK: specify the ADC clock source.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_ADCCLK_HSI14: ADC clock = HSI14 (14MHz)
  *     @arg RCC_ADCCLK_APB2_DIV2: ADC clock = APB2/2
  *     @arg RCC_ADCCLK_APB2_DIV4: ADC clock = APB2/4
  *     @arg RCC_ADCCLK_APB2_DIV6: ADC clock = APB2/6
  *     @arg RCC_ADCCLK_APB2_DIV8: ADC clock = APB2/8 
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_ADCCLK_HSI28_DIV2: ADC clock = HSI28/2 (14MHz)
  *     @arg RCC_ADCCLK_HSI28: ADC clock = HSI28 (28MHz)
  *     @arg RCC_ADCCLK_APB2_DIV2: ADC clock = APB2/2
  *     @arg RCC_ADCCLK_APB2_DIV4: ADC clock = APB2/4
  *     @arg RCC_ADCCLK_APB2_DIV6: ADC clock = APB2/6
  *     @arg RCC_ADCCLK_APB2_DIV8: ADC clock = APB2/8
  *  
  * @retval None
  */
void RCC_ADCCLKConfig(uint32_t RCC_ADCCLK)
{ 
    /* Clear ADCPS bit */
    RCC->GCFGR &= ~RCC_GCFGR_ADCPS;
    /* Set ADCPS bits according to RCC_APB2 value */
    RCC->GCFGR |= RCC_ADCCLK & 0xFFFF;
    
#ifdef GD32F170_190
    /* Clear ADCSEL , HSI28DIV bit */
    RCC->GCFGR3 &= ~(RCC_GCFGR3_ADCSEL | RCC_GCFGR3_HSI28DIV); 
    
    /* if RCC_ADCCLK is RCC_ADCCLK_HSI28, set RCC_GCFGR3_HSI28DIV*/
    if(RCC_ADCCLK  == RCC_ADCCLK_HSI28)
    {
        RCC->GCFGR3 |= RCC_GCFGR3_HSI28DIV;
        RCC_ADCCLK = RCC_ADCCLK_HSI28_DIV2;
    }
    
    /* Set ADCSEL bits according to RCC_ADCCLK value */
    RCC->GCFGR3 |= RCC_ADCCLK >> GCFGR3_HSI28ADC_OFFSET;
#else
    /* Clear ADCSEL bit */
    RCC->GCFGR3 &= ~RCC_GCFGR3_ADCSEL; 
    /* Set ADCSEL bits according to RCC_ADCCLK value */
    RCC->GCFGR3 |= RCC_ADCCLK >> GCFGR3_HSI14ADC_OFFSET;
#endif /* GD32F170_190 */    
}

#ifdef GD32F130_150
/**
  * @brief  Configure the USB clock (USBCLK).
  * @param  RCC_USBCLK: specify the USB clock source. This clock is derived 
  *         from the PLL.
  *   This parameter can be one of the following values:
  *     @arg RCC_USBCLK_PLL_DIV1_5: USB clock = PLL/1.5
  *     @arg RCC_USBCLK_PLL_DIV1: USB clock = PLL
  *     @arg RCC_USBCLK_PLL_DIV2: USB clock = PLL/2
  *     @arg RCC_USBCLK_PLL_DIV2_5: USB clock = PLL/2.5
  * @retval None
  */
void RCC_USBCLKConfig(uint32_t RCC_USBCLK)
{ 
    /* Clear USBPS bit */
    RCC->GCFGR &= ~RCC_GCFGR_USBPS;
    /* Set USBPS bits according to RCC_USBCLK value */
    RCC->GCFGR |= RCC_USBCLK;
}
#endif /* GD32F130_150 */

/**
  * @brief  Configure the CEC clock (CECCLK).
  * @param  RCC_CECCLK: specify the CEC clock source. This clock is derived 
  *         from the HSI or LSE clock.
  *   This parameter can be one of the following values:
  *     @arg RCC_CECCLK_HSI_DIV244: CEC clock = HSI/244 (32768Hz)
  *     @arg RCC_CECCLK_LSE: CEC clock = LSE
  * @retval None
  */
void RCC_CECCLKConfig(uint32_t RCC_CECCLK)
{ 
    /* Clear CECSEL bit */
    RCC->GCFGR3 &= ~RCC_GCFGR3_CECSEL;
    /* Set CECSEL bits according to RCC_CECCLK value */
    RCC->GCFGR3 |= RCC_CECCLK;
}

/**
  * @brief  Configure the USART1 clock (USART1CLK).
  * @param  RCC_USARTCLK: specify the USART1 clock source. 
  *   This parameter can be one of the following values:
  *     @arg RCC_USART1CLK_APB2: USART1 clock = APB2 Clock
  *     @arg RCC_USART1CLK_CK_SYS: USART1 clock = CK_SYS Clock
  *     @arg RCC_USART1CLK_LSE: USART1 clock = LSE Clock
  *     @arg RCC_USART1CLK_HSI: USART1 clock = HSI Clock
  * @retval None
  */
void RCC_USARTCLKConfig(uint32_t RCC_USARTCLK)
{ 
    /* Clear USART1SEL[1:0] bit */
    RCC->GCFGR3 &= ~RCC_GCFGR3_USART1SEL;
    /* Set USART1SEL bits according to RCC_USARTCLK value */
    RCC->GCFGR3 |= RCC_USARTCLK;
}

/**
  * @brief  Configure the RTC clock (RTCCLK).           
  * @param  RCC_RTCCLKSource: specify the RTC clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_RTCCLKSource_LSE: selecte LSE as RTC clock
  *     @arg RCC_RTCCLKSource_LSI: selecte LSI as RTC clock
  *     @arg RCC_RTCCLKSource_HSE_DIV32: selecte HSE divided by 32 as RTC clock         
  * @note   if using HSE as RTC source, the maximum clock frequency for RTC is 2 MHz.                     
  * @retval None
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
    /* Select the RTC clock source */
    RCC->BDCR |= RCC_RTCCLKSource;
}

/**
  * @brief  Get the frequencies of the CK_SYS, AHB, APB1, APB2, ADC, CEC and USART1 clocks.    
  * @retval None
  */
void RCC_GetClocksFreq(RCC_ClocksPara* RCC_Clocks)
{
    uint32_t temp = 0, pllmf = 0, pllmf4=0,pllselect = 0, prediv1factor = 0, presc = 0;
    
    /* Get CK_SYS source -------------------------------------------------------*/
    temp = RCC->GCFGR & RCC_GCFGR_SCSS;
    
    switch (temp)
    {
        case 0x00:  /* HSI used as CK_SYS */
        RCC_Clocks->CK_SYS_Frequency = HSI_VALUE;
        break;
        case 0x04:  /* HSE used as CK_SYS */
        RCC_Clocks->CK_SYS_Frequency = HSE_VALUE;
        break;
        case 0x08:  /* PLL used as CK_SYS */
        /* Get PLL clock source and multiplication factor ----------------------*/
        /* Get PLLMF[3:0] */
        pllmf = RCC->GCFGR & RCC_GCFGR_PLLMF_3_0;
        /* Get PLLMF[4] */
        pllmf4 = RCC->GCFGR & RCC_GCFGR_PLLMF_4;
        pllmf4 = (( pllmf4 >> 27)*16);
        
        pllmf = ( pllmf >> 18) + pllmf4;
        
        if(pllmf >= 0x0F)
        {
            pllmf += 1;
        }
        else
        {
            pllmf += 2;
        }
        pllselect = RCC->GCFGR & RCC_GCFGR_PLLSEL;
        if (pllselect == 0x00)
        {
            /* HSI clock divided by 2 selected as PLL clock source */
            RCC_Clocks->CK_SYS_Frequency = (HSI_VALUE >>HSI_CLOCK_DIVIDED_2) * pllmf;
        }
        else
        {
            /*Get the HSEPREDV division factor */
            prediv1factor = (RCC->GCFGR2 & RCC_GCFGR2_HSEPREDV1) + 1;
            /* HSE clock selected as PLL clock source */
            RCC_Clocks->CK_SYS_Frequency = (HSE_VALUE / prediv1factor) * pllmf; 
        }      
        break;
        default: /* HSI used as system clock */
        RCC_Clocks->CK_SYS_Frequency = HSI_VALUE;
        break;
    }
    
    /* Get AHB prescaler */
    temp = RCC->GCFGR & RCC_GCFGR_AHBPS;
    temp = temp >> 4;
    presc = AHBPrescTable[temp]; 
    /* Get AHB clock frequency */
    RCC_Clocks->AHB_Frequency = RCC_Clocks->CK_SYS_Frequency >> presc;
    
    /* Get APB1 prescaler */
    temp = RCC->GCFGR & RCC_GCFGR_APB1PS;
    temp = temp >> 8;
    presc = APBPrescTable[temp];
    /* Get APB1 clock frequency */
    RCC_Clocks->APB1_Frequency = RCC_Clocks->AHB_Frequency >> presc;
    
    /* Get APB2 prescaler */
    temp = RCC->GCFGR & RCC_GCFGR_APB2PS;
    temp = temp >> 11;
    presc = APBPrescTable[temp];
    /* Get APB2 clock frequency */
    RCC_Clocks->APB2_Frequency = RCC_Clocks->AHB_Frequency >> presc;
    
    /* Get ADCCLK clock frequency */
    if((RCC->GCFGR3 & RCC_GCFGR3_ADCSEL) != RCC_GCFGR3_ADCSEL)
    {        
#ifdef GD32F170_190
        if((RCC->GCFGR3 & RCC_GCFGR3_HSI28DIV) != RCC_GCFGR3_HSI28DIV)
        {
            /* ADC Clock clock is derived from the HSI28 / 2 */
            RCC_Clocks->ADCCLK_Frequency = HSI28_VALUE / 2;
        }
        else
        {
            /* ADC Clock clock is derived from the HSI28 */
            RCC_Clocks->ADCCLK_Frequency = HSI28_VALUE;
        }
#else        
        /* ADC Clock clock is derived from the HSI14 */
        RCC_Clocks->ADCCLK_Frequency = HSI14_VALUE;
#endif /* GD32F170_190 */
    }
    else
    {
        temp = ((RCC->GCFGR & RCC_GCFGR_ADCPS) >>14 );
        switch (temp)
        {
            case 0: /* ADC Clock is derived from APB2/2 */
            RCC_Clocks->ADCCLK_Frequency = (RCC_Clocks->APB2_Frequency /2);
            break;
            case 1: /* ADC Clock is derived from APB2/4 */
            RCC_Clocks->ADCCLK_Frequency = (RCC_Clocks->APB2_Frequency /4);
            break;
            case 2: /* ADC Clock is derived from APB2/6 */
            RCC_Clocks->ADCCLK_Frequency = (RCC_Clocks->APB2_Frequency /6);
            break;
            case 3: /* ADC Clock is derived from APB2/8 */
            RCC_Clocks->ADCCLK_Frequency = (RCC_Clocks->APB2_Frequency /8);
            break;
        }      
    }
    
    /* Get CECCLK clock frequency */
    if((RCC->GCFGR3 & RCC_GCFGR3_CECSEL) != RCC_GCFGR3_CECSEL)
    {        
        /* CEC Clock is derived from HSI/244 */
        RCC_Clocks->CECCLK_Frequency = HSI_VALUE / 244;
    }
    else
    {
        /* CEC Clock is derived from LSE. */
        RCC_Clocks->CECCLK_Frequency = LSE_VALUE;
    }
    
    /* Get USART1CLK clock frequency */
    if((RCC->GCFGR3 & RCC_GCFGR3_USART1SEL) == RCC_USART1CLK_APB2)
    {
        /* USART1 Clock is derived from APB2 */
        RCC_Clocks->USART1CLK_Frequency = RCC_Clocks->APB2_Frequency;
    }
    else if((RCC->GCFGR3 & RCC_GCFGR3_USART1SEL) == RCC_GCFGR3_USART1SEL_0)
    {
        /* USART1 Clock is derived from System Clock */
        RCC_Clocks->USART1CLK_Frequency = RCC_Clocks->CK_SYS_Frequency;
    }
    else if((RCC->GCFGR3 & RCC_GCFGR3_USART1SEL) == RCC_GCFGR3_USART1SEL_1)
    {
        /* USART1 Clock is derived from LSE */
        RCC_Clocks->USART1CLK_Frequency = LSE_VALUE;
    }
    else if((RCC->GCFGR3 & RCC_GCFGR3_USART1SEL) == RCC_GCFGR3_USART1SEL)
    {
        /* USART1 Clock is derived from HSI */
        RCC_Clocks->USART1CLK_Frequency = HSI_VALUE;
    }
}

/**
  * @brief  Enable or disable the AHB peripheral clock.    
  * @param  RCC_AHBPeriph: specify the AHB peripheral clock.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_AHBPERIPH_GPIOA: GPIOA clock
  *     @arg RCC_AHBPERIPH_GPIOB: GPIOB clock
  *     @arg RCC_AHBPERIPH_GPIOC: GPIOC clock
  *     @arg RCC_AHBPERIPH_GPIOD: GPIOD clock
  *     @arg RCC_AHBPERIPH_GPIOF: GPIOF clock
  *     @arg RCC_AHBPERIPH_TSI: TSI clock
  *     @arg RCC_AHBPERIPH_CRC: CRC clock
  *     @arg RCC_AHBPERIPH_FMC: FMC clock
  *     @arg RCC_AHBPERIPH_SRAM: SRAM clock
  *     @arg RCC_AHBPERIPH_DMA1: DMA1 clock
  * @param  NewValue: new state of the peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphClock_Enable(uint32_t RCC_AHBPeriph, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->AHBCCR |= RCC_AHBPeriph;
    }
    else
    {
        RCC->AHBCCR &= ~RCC_AHBPeriph;
    }
}

/**
  * @brief  Enable or disable the APB1 peripheral clock.
  * @param  RCC_APB1Periph: specify the APB1 peripheral clock.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_APB1PERIPH_TIMER2: TIMER2 clock
  *     @arg RCC_APB1PERIPH_TIMER3: TIMER3 clock
  *     @arg RCC_APB1PERIPH_TIMER6: TIMER6 clock
  *     @arg RCC_APB1PERIPH_TIMER14: TIMER14 clock
  *     @arg RCC_APB1PERIPH_WWDG: WWDG clock
  *     @arg RCC_APB1PERIPH_SPI2: SPI2 clock
  *     @arg RCC_APB1PERIPH_SPI3: SPI3 clock
  *     @arg RCC_APB1PERIPH_USART2: USART2 clock
  *     @arg RCC_APB1PERIPH_I2C1: I2C1 clock
  *     @arg RCC_APB1PERIPH_I2C2: I2C2 clock
  *     @arg RCC_APB1PERIPH_USB: USB clock
  *     @arg RCC_APB1PERIPH_PWR: PWR clock
  *     @arg RCC_APB1PERIPH_DAC: DAC clock
  *     @arg RCC_APB1PERIPH_CEC: CEC clock
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_APB1PERIPH_TIMER2: TIMER2 clock
  *     @arg RCC_APB1PERIPH_TIMER3: TIMER3 clock
  *     @arg RCC_APB1PERIPH_TIMER6: TIMER6 clock
  *     @arg RCC_APB1PERIPH_TIMER14: TIMER14 clock
  *     @arg RCC_APB1PERIPH_LCD: LCD clock
  *     @arg RCC_APB1PERIPH_WWDG: WWDG clock
  *     @arg RCC_APB1PERIPH_SPI2: SPI2 clock
  *     @arg RCC_APB1PERIPH_SPI3: SPI3 clock
  *     @arg RCC_APB1PERIPH_USART2: USART2 clock
  *     @arg RCC_APB1PERIPH_I2C1: I2C1 clock
  *     @arg RCC_APB1PERIPH_I2C2: I2C2 clock
  *     @arg RCC_APB1PERIPH_CAN1: CAN1 clock
  *     @arg RCC_APB1PERIPH_CAN2: CAN2 clock
  *     @arg RCC_APB1PERIPH_PWR: PWR clock
  *     @arg RCC_APB1PERIPH_DAC: DAC clock
  *     @arg RCC_APB1PERIPH_CEC: CEC clock
  *     @arg RCC_APB1PERIPH_OPAMP: OPAMP clock
  *     @arg RCC_APB1PERIPH_IVREF: IVREF clock 
  *  
  * @param  NewValue: new state of the peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB1PeriphClock_Enable(uint32_t RCC_APB1Periph, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->APB1CCR |= RCC_APB1Periph;
    }
    else
    {
        RCC->APB1CCR &= ~RCC_APB1Periph;
    }
}

/**
  * @brief  Enable or disable the APB2 peripheral clock.
  * @param  RCC_APB2Periph: specify the APB2 peripheral clock.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB2PERIPH_CFG: SYSCFG clock
  *     @arg RCC_APB2PERIPH_CMP: CMP clock
  *     @arg RCC_APB2PERIPH_ADC1: ADC1 clock
  *     @arg RCC_APB2PERIPH_TIMER1: TIMER1 clock
  *     @arg RCC_APB2PERIPH_SPI1: SPI1 clock
  *     @arg RCC_APB2PERIPH_USART1: USART1 clock
  *     @arg RCC_APB2PERIPH_TIMER15: TIMER15 clock
  *     @arg RCC_APB2PERIPH_TIMER16: TIMER16 clock
  *     @arg RCC_APB2PERIPH_TIMER17: TIMER17 clock
  * @param  NewValue: new state of the peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphClock_Enable(uint32_t RCC_APB2Periph, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->APB2CCR |= RCC_APB2Periph;
    }
    else
    {
        RCC->APB2CCR &= ~RCC_APB2Periph;
    }
}

/**
  * @brief  Enable or disable the ACCR_I2C3 peripheral clock.
  * @note   I2C3 Clock is derived from APB1
  * @param  RCC_ACCRPeriph: specify the ACCR_I2C3 peripheral clock.
  *     @arg RCC_ACCRPERIPH_I2C3: I2C3 clock                             
  * @param  NewValue: new state of the peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_ACCRPeriphClock_Enable(uint32_t RCC_ACCRPeriph, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->RCC_I2C3CCR |= RCC_ACCRPeriph;
    }
    else
    {
        RCC->RCC_I2C3CCR &= ~RCC_ACCRPeriph;
    }
}

/**
  * @brief  Enable or disable the RTC clock.
  * @param  NewValue: new state of the RTC clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_RTCCLK_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->BDCR |= RCC_BDCR_RTCEN;
    }
    else
    {
        RCC->BDCR &= ~RCC_BDCR_RTCEN;
    }
}

/**
  * @brief  Force or release AHB peripheral reset.
  * @param  RCC_AHBPeriphRST: specify the AHB peripheral to reset.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_AHBPERIPH_GPIOARST: reset GPIOA clock 
  *     @arg RCC_AHBPERIPH_GPIOBRST: reset GPIOB clock 
  *     @arg RCC_AHBPERIPH_GPIOCRST: reset GPIOC clock 
  *     @arg RCC_AHBPERIPH_GPIODRST: reset GPIOD clock 
  *     @arg RCC_AHBPERIPH_GPIOFRST: reset GPIOF clock 
  *     @arg RCC_AHBPERIPH_TSIRST:   reset TSI clock 
  * @param  NewValue: new state of the peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_AHBPeriphReset_Enable(uint32_t RCC_AHBPeriphRST, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->AHBRCR |= RCC_AHBPeriphRST;
    }
    else
    {
        RCC->AHBRCR &= ~RCC_AHBPeriphRST;
    }
}

/**
  * @brief  Force or release APB2 peripheral reset.
  * @param  RCC_APB2PeriphRST: specifies the APB2 peripheral to reset.
  *   This parameter can be any combination of the following values:
  *     @arg RCC_APB2PERIPH_CFGRST:      reset CFG clock
  *     @arg RCC_APB2PERIPH_CMPRST:      reset CMP clock
  *     @arg RCC_APB2PERIPH_ADC1RST:     reset ADC1 clock
  *     @arg RCC_APB2PERIPH_TIMER1RST:   reset TIM1 clock
  *     @arg RCC_APB2PERIPH_SPI1RST:     reset SPI1 clock
  *     @arg RCC_APB2PERIPH_USART1RST:   reset USART1 clock
  *     @arg RCC_APB2PERIPH_TIMER15RST:  reset TIMER15 clock
  *     @arg RCC_APB2PERIPH_TIMER16RST:  reset TIMER16 clock
  *     @arg RCC_APB2PERIPH_TIMER17RST:  reset TIMER17 clock
  * @param  NewValue: new state of the peripheral reset.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RCC_APB2PeriphReset_Enable(uint32_t RCC_APB2PeriphRST, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->APB2RCR |= RCC_APB2PeriphRST;
    }
    else
    {
        RCC->APB2RCR &= ~RCC_APB2PeriphRST;
    }
}

/**
  * @brief  Force or release APB1 peripheral reset.
  * @param  RCC_APB1PeriphRST: specifies the APB1 peripheral to reset.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_APB1PERIPH_TIMER2RST: reset TIMER2 clock
  *     @arg RCC_APB1PERIPH_TIMER3RST: reset TIMER3 clock
  *     @arg RCC_APB1PERIPH_TIMER6RST: reset TIMER6 clock
  *     @arg RCC_APB1PERIPH_TIMER14RST: reset TIMER14 clock
  *     @arg RCC_APB1PERIPH_WWDGRST: reset WWDG clock
  *     @arg RCC_APB1PERIPH_SPI2RST: reset SPI2 clock
  *     @arg RCC_APB1PERIPH_SPI3RST: reset SPI3 clock
  *     @arg RCC_APB1PERIPH_USART2RST: reset USART2 clock
  *     @arg RCC_APB1PERIPH_I2C1RST: reset I2C1 clock
  *     @arg RCC_APB1PERIPH_I2C2RST: reset I2C2 clock
  *     @arg RCC_APB1PERIPH_USBRST: reset PWR clock
  *     @arg RCC_APB1PERIPH_PWRRST: reset PWR clock
  *     @arg RCC_APB1PERIPH_DACRST: reset DAC clock
  *     @arg RCC_APB1PERIPH_CECRST: reset CEC clock
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_APB1PERIPH_TIMER2RST: reset TIMER2 clock
  *     @arg RCC_APB1PERIPH_TIMER3RST: reset TIMER3 clock
  *     @arg RCC_APB1PERIPH_TIMER6RST: reset TIMER6 clock
  *     @arg RCC_APB1PERIPH_TIMER14RST: reset TIMER14 clock
  *     @arg RCC_APB1PERIPH_LCDRST: reset LCD clock
  *     @arg RCC_APB1PERIPH_WWDGRST: reset WWDG clock
  *     @arg RCC_APB1PERIPH_SPI2RST: reset SPI2 clock
  *     @arg RCC_APB1PERIPH_SPI3RST: reset SPI3 clock
  *     @arg RCC_APB1PERIPH_USART2RST: reset USART2 clock
  *     @arg RCC_APB1PERIPH_I2C1RST: reset I2C1 clock
  *     @arg RCC_APB1PERIPH_I2C2RST: reset I2C2 clock
  *     @arg RCC_APB1PERIPH_CAN1RST: reset CAN1 clock
  *     @arg RCC_APB1PERIPH_CAN2RST: reset CAN2 clock
  *     @arg RCC_APB1PERIPH_PWRRST: reset PWR clock
  *     @arg RCC_APB1PERIPH_DACRST: reset DAC clock
  *     @arg RCC_APB1PERIPH_CECRST: reset CEC clock
  *     @arg RCC_APB1PERIPH_OPAMPRST: reset OPAMP clock
  *     @arg RCC_APB1PERIPH_IVREFRST: reset IVREF clock
  *
  * @param  NewValue: new state of the peripheral clock (ENABLE or DISABLE).
  * @retval None
  */
void RCC_APB1PeriphReset_Enable(uint32_t RCC_APB1PeriphRST, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->APB1RCR |= RCC_APB1PeriphRST;
    }
    else
    {
        RCC->APB1RCR &= ~RCC_APB1PeriphRST;
    }
}

/**
  * @brief  Force or release ARCR_I2C3 peripheral reset.
  * @param  RCC_ARCRPeriphRST
  *     @arg RCC_ARCRPERIPH_I2C3RST:   reset I2C3 clock
  * @param  NewValue: new state of the peripheral clock (ENABLE or DISABLE).
  * @retval None
  */
void RCC_ARCRPeriphReset_Enable(uint32_t RCC_ARCRPeriphRST, TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->RCC_I2C3RCR |= RCC_ARCRPeriphRST;
    }
    else
    {
        RCC->RCC_I2C3RCR &= ~RCC_ARCRPeriphRST;
    }
}

/**
  * @brief  Force or release the Backup domain reset.
  * @param  NewValue: new state of the Backup domain reset (ENABLE or DISABLE).
  * @retval None
  */
void RCC_BackupReset_Enable(TypeState NewValue)
{
    if (NewValue != DISABLE)
    {
        RCC->BDCR |= RCC_BDCR_BKPRST;
    }
    else
    {
        RCC->BDCR &= ~RCC_BDCR_BKPRST;
    }
}

/**
  * @brief  Enable or disable RCC interrupts.
  * @param  RCC_INT: specify the RCC interrupt sources.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: LSI ready interrupt
  *     @arg RCC_INT_LSESTB: LSE ready interrupt
  *     @arg RCC_INT_HSISTB: HSI ready interrupt
  *     @arg RCC_INT_HSESTB: HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: PLL ready interrupt
  *     @arg RCC_INT_HSI14STB: HSI14 ready interrupt  
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: LSI ready interrupt
  *     @arg RCC_INT_LSESTB: LSE ready interrupt
  *     @arg RCC_INT_HSISTB: HSI ready interrupt
  *     @arg RCC_INT_HSESTB: HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: PLL ready interrupt
  *     @arg RCC_INT_HSI28STB: HSI28 ready interrupt
  *
  * @param  NewValue: new state of the RCC interrupts (ENABLE or DISABLE).
  * @retval None
  */
void RCC_INTConfig(uint8_t RCC_INT, TypeState NewValue)
{  
    if (NewValue != DISABLE)
    {
        /* Perform Byte access to RCC_GCIR[13:8] bits to enable the selected interrupts */
        *(__IO uint8_t *) GCIR_BYTE1_ADDRESS |= RCC_INT;
    }
    else
    {
        /* Perform Byte access to GRCC_GCIR[13:8] bits to disable the selected interrupts */
        *(__IO uint8_t *) GCIR_BYTE1_ADDRESS &= (uint8_t)~RCC_INT;
    }
}

/**
  * @brief  Check whether the specified RCC flag is set or not.
  * @param  RCC_FLAG: specify the flag to check.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_FLAG_HSISTB: HSI clock ready  
  *     @arg RCC_FLAG_HSESTB: HSE clock ready
  *     @arg RCC_FLAG_PLLSTB: PLL clock ready
  *     @arg RCC_FLAG_LSESTB: LSE clock ready
  *     @arg RCC_FLAG_LSISTB: LSI clock ready
  *     @arg RCC_FLAG_OBLSTB: Option Byte Loader (OBL) reset 
  *     @arg RCC_FLAG_EPRST: Pin reset
  *     @arg RCC_FLAG_V12RST: V12 power domain reset  
  *     @arg RCC_FLAG_POPDRST: POR/PDR reset
  *     @arg RCC_FLAG_SWRST: Software reset
  *     @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *     @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *     @arg RCC_FLAG_LPRST: Low Power reset
  *     @arg RCC_FLAG_HSI14STB: HSI14 clock ready
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_FLAG_HSISTB: HSI clock ready  
  *     @arg RCC_FLAG_HSESTB: HSE clock ready
  *     @arg RCC_FLAG_PLLSTB: PLL clock ready
  *     @arg RCC_FLAG_LSESTB: LSE clock ready
  *     @arg RCC_FLAG_LSISTB: LSI clock ready
  *     @arg RCC_FLAG_OBLSTB: Option Byte Loader (OBL) reset 
  *     @arg RCC_FLAG_EPRST: Pin reset
  *     @arg RCC_FLAG_V12RST: V12 power domain reset  
  *     @arg RCC_FLAG_POPDRST: POR/PDR reset
  *     @arg RCC_FLAG_SWRST: Software reset
  *     @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *     @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *     @arg RCC_FLAG_LPRST: Low Power reset
  *     @arg RCC_FLAG_HSI28STB: HSI28 clock ready  
  *  
  * @retval The new state of RCC_FLAG (SET or RESET).
  */
TypeState RCC_GetBitState(uint8_t RCC_FLAG)
{
    uint32_t temp = 0;
    uint32_t statusreg = 0;
    
    /* Get the RCC register index */
    temp = RCC_FLAG >> 5;
    
    
    if (temp == 0)               
    {
        /* The flag to check is in GCCR register */
        statusreg = RCC->GCCR;
    }
    else if (temp == 1)          
    {
        /* The flag to check is in BDCR register */   
        statusreg = RCC->BDCR;
    }
    else if (temp == 2)         
    {
         /* The flag to check is in GCSR register */
        statusreg = RCC->GCSR;
    }
    else                        
    {
        /* The flag to check is in GCR2 register */
        statusreg = RCC->GCCR2;
    }   
    
    /* Get the flag position */
    temp = RCC_FLAG & FLAG_MASK;
    
    if ((statusreg & ((uint32_t)1 << temp)) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
  * @brief  Clear the RCC all reset flags.
  * @param  None
  * @retval None
  */
void RCC_ClearBitState(void)
{
    /* Set RSTFC bit to clear all reset flags */
    RCC->GCSR |= RCC_GCSR_RSTFC;
}

/**
  * @brief  Check whether the RCC interrupt has occurred or not.
  * @param  RCC_INT: specify the RCC interrupt source to check.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: the flag of LSI ready interrupt
  *     @arg RCC_INT_LSESTB: the flag of LSE ready interrupt
  *     @arg RCC_INT_HSISTB: the flag of HSI ready interrupt
  *     @arg RCC_INT_HSESTB: the flag of HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: the flag of PLL ready interrupt
  *     @arg RCC_INT_HSI14STB: the flag of HSI14 ready interrupt 
  *     @arg RCC_INT_CKM: the flag of Clock Security System interrupt
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: the flag of LSI ready interrupt
  *     @arg RCC_INT_LSESTB: the flag of LSE ready interrupt
  *     @arg RCC_INT_HSISTB: the flag of HSI ready interrupt
  *     @arg RCC_INT_HSESTB: the flag of HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: the flag of PLL ready interrupt
  *     @arg RCC_INT_HSI28STB: the flag of HSI28 ready interrupt 
  *     @arg RCC_INT_CKM: the flag of Clock Security System interrupt
  *
  * @retval The new state of RCC_INT (SET or RESET).
  */
TypeState RCC_GetIntBitState(uint8_t RCC_INT)
{  
  /* Check the status of the RCC interrupt */
    if ((RCC->GCIR & RCC_INT) != (uint32_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}

/**
  * @brief  Clear the RCC interrupt bits.
  * @param  RCC_INT: specify the interrupt bit to clear.
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: LSI ready interrupt
  *     @arg RCC_INT_LSESTB: LSE ready interrupt
  *     @arg RCC_INT_HSISTB: HSI ready interrupt
  *     @arg RCC_INT_HSESTB: HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: PLL ready interrupt
  *     @arg RCC_INT_HSI14STB: HSI14 ready interrupt  
  *     @arg RCC_INT_CKM: Clock Security System interrupt
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_INT_LSISTB: LSI ready interrupt
  *     @arg RCC_INT_LSESTB: LSE ready interrupt
  *     @arg RCC_INT_HSISTB: HSI ready interrupt
  *     @arg RCC_INT_HSESTB: HSE ready interrupt
  *     @arg RCC_INT_PLLSTB: PLL ready interrupt
  *     @arg RCC_INT_HSI28STB: HSI28 ready interrupt  
  *     @arg RCC_INT_CKM: Clock Security System interrupt 
  *
  * @retval None
  */
void RCC_ClearIntBitState(uint8_t RCC_INT)
{
    /* Perform Byte access to RCC_GCIR[23:16] bits to clear the selected interrupt bits */
    *(__IO uint8_t *) GCIR_BYTE2_ADDRESS = RCC_INT;
}

/**
  * @brief  Unlock the power.
  * @note   Write 0x1A2B3C4D to RCC_VC_KEY to Unlock the power.
  * @param  None
  * @retval None
  */
void RCC_UnlockPower(void)
{ 
    /* Clear RCC_VC_KEY bit */
    RCC->RCC_VC_KEY &= RCC_VC_KEY_CLEAR;
    /* Unlock the power according to RCC_VC_KEY_ULOCKPOWER */
    RCC->RCC_VC_KEY |= RCC_VC_KEY_ULOCKPOWER;
}

/**
  * @brief  Configure the kernel voltage in Deep-sleep mode.
  * @note   Only unlock the power,this configuration is effective.
  * @param  RCC_KERNEL_VOL: specify the kernel voltage. 
  *
  *   For @b GD32F130 or GD32F150 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_KERNEL_VOL1_2: The kernel voltage in Deep-sleep mode is 1.2V 
  *     @arg RCC_KERNEL_VOL1_1: The kernel voltage in Deep-sleep mode is 1.1V 
  *     @arg RCC_KERNEL_VOL1_0: The kernel voltage in Deep-sleep mode is 1.0V
  *     @arg RCC_KERNEL_VOL0_9: The kernel voltage in Deep-sleep mode is 0.9V
  *
  *   For @b GD32F170 or GD32F190 devices, this parameter can be any combination of 
  *   the following values:
  *     @arg RCC_KERNEL_VOL1_8: The kernel voltage in Deep-sleep mode is 1.8V 
  *     @arg RCC_KERNEL_VOL1_6: The kernel voltage in Deep-sleep mode is 1.6V 
  *     @arg RCC_KERNEL_VOL1_4: The kernel voltage in Deep-sleep mode is 1.4V
  *     @arg RCC_KERNEL_VOL1_2: The kernel voltage in Deep-sleep mode is 1.2V
  *
  * @retval None
  */
void RCC_KERNELVOLConfig(uint32_t RCC_KERNEL_VOL) 
{ 
    /* Clear DEEPSLEEP_VC bit */
    RCC->RCC_DEEPSLEEP_VC &= ~RCC_DEEPSLEEP_VC_CLEAR;
    /* Set DEEPSLEEP_VC bits according to RCC_KERNEL_VOL value */
    RCC->RCC_DEEPSLEEP_VC |= RCC_KERNEL_VOL;
}

#ifdef GD32F130_150
/**
  * @brief  Configure the voltage of power down.
  * @param  RCC_POWERDOWNVOL: specify the voltage of power down.
  *   This parameter can be one of the following values:
  *     @arg RCC_POWERDOWNVOL2_6: The voltage of power down is 2.6V.
  *     @arg RCC_POWERDOWNVOL1_8: The voltage of power down is 1.8V
  * @retval None
  */
void RCC_POWERDOWNVOLConfig(uint32_t RCC_POWERDOWNVOL)
{ 
    /* Clear RCC_PDR_S bit */
    RCC->RCC_PDR_S &= ~RCC_PDR_S_CLEAR;
    /* Set RCC_PDR_S bit according to RCC_POWERDOWNVOL value */
    RCC->RCC_PDR_S |= RCC_POWERDOWNVOL;
}
#endif /* GD32F130_150 */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
