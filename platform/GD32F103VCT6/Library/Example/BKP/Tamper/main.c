/**
  ******************************************************************************
  * @file    BKP/Tamper/main.c 
  * @author  MCU SD
  * @version V1.0
  * @date    26-DEC-2014
  * @brief   Main program body.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private define ------------------------------------------------------------*/
#define BKP_DR_NUM              42

/* Private variables ---------------------------------------------------------*/
uint16_t BKPDataReg[BKP_DR_NUM] =
  {
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10, BKP_DR11, BKP_DR12, BKP_DR13, BKP_DR14, BKP_DR15, BKP_DR16,
    BKP_DR17, BKP_DR18, BKP_DR19, BKP_DR20, BKP_DR21, BKP_DR22, BKP_DR23, BKP_DR24,
    BKP_DR25, BKP_DR26, BKP_DR27, BKP_DR28, BKP_DR29, BKP_DR30, BKP_DR31, BKP_DR32,
    BKP_DR33, BKP_DR34, BKP_DR35, BKP_DR36, BKP_DR37, BKP_DR38, BKP_DR39, BKP_DR40,
    BKP_DR41, BKP_DR42
  };
  
/* Private function prototypes -----------------------------------------------*/
void LED_Init(void);
void NVIC_Configuration(void);
void WriteToBackupReg(uint16_t UserBackupData);
uint8_t CheckBackupReg(uint16_t UserBackupData);
uint32_t IsBackupRegClear(void);
    
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{     
    /* Initialize four LEDs mounted on GD32107C-EVAL board */
    LED_Init();
    
    /* NVIC configuration */
    NVIC_Configuration();
    
    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);
    
    /* Enable write access to the registers in Backup domain */
    PWR_BackupAccess_Enable(ENABLE);
    
    /* Disable the TAMPER pin */
    BKP_TamperPinConfig(BKP_TPAL_LOW, DISABLE);
    
    /* Disable the Tamper interrupt */
    BKP_TamperINT_Enable(DISABLE);
    
    /* Clear the bit flag of Tamper Event */
    BKP_ClearBitState();
    
    /* Enable the Tamper interrupt */
    BKP_TamperINT_Enable(ENABLE);

    /* Configure the TAMPER pin active on low level, and enable the TAMPER pin */
    BKP_TamperPinConfig(BKP_TPAL_LOW, ENABLE);

    /* Write data to Backup DRx registers */
    WriteToBackupReg(0x1226);
  
    /* Check if the written data are correct */
    if(CheckBackupReg(0x1226) == 0x00)
    {
        /* Turn on LED2 */
        GPIO_SetBits(GPIOC, GPIO_PIN_0);
      }
    else
      {
        /* Turn on LED3 */
        GPIO_SetBits(GPIOC, GPIO_PIN_2);
    }
           
    while (1)
    {    
    }
}

/**
  * @brief  Configure the LED GPIO.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
    GPIO_InitPara GPIO_InitStructure;
    
    /* Enable GPIOC and GPIOE clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOE, ENABLE);
    
    /* Configure the four LEDs GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  Write userdata to Backup DRx registers.
  * @param  UserBackupData: data to be written to Backup data registers.
  * @retval None
  */
void WriteToBackupReg(uint16_t UserBackupData)
{
    uint32_t temp = 0;
    
    for (temp = 0; temp < BKP_DR_NUM; temp++)
    {
        BKP_WriteBackupRegister(BKPDataReg[temp], UserBackupData + (temp * 0x50));
    }  
}

/**
  * @brief  Check if the Backup DRx registers values are correct or not.
  * @param  UserBackupData: data to be compared with Backup data registers.
  * @retval 
  *   0: All Backup DRx registers values are correct
  *   Value different from 0: Some Backup DRx registers values are
  *                           not correct
  */
uint8_t CheckBackupReg(uint16_t UserBackupData)
{
    uint32_t temp = 0;
    
    for (temp = 0; temp < BKP_DR_NUM; temp++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[temp]) != (UserBackupData + (temp * 0x50)))
        {
              return (temp + 1);
        }
    }                
    return 0;
}

/**
  * @brief  Check if the Backup Data registers are clear or not.
  * @param  None
  * @retval 
  *   0: All Backup DRx registers are clear
  *   Value different from 0: Some Backup DRx registers values are
  *                           not cleared
  */
uint32_t IsBackupRegClear(void)
{
    uint32_t temp = 0;
    
    for (temp = 0; temp < BKP_DR_NUM; temp++)
    {
        if (BKP_ReadBackupRegister(BKPDataReg[temp]) != 0x0000)
        {
            return (temp + 1);
        }
    }
    
    return 0;
}

/**
  * @brief  Configure NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    NVIC_InitPara NVIC_InitStructure;

    /* Enable TAMPER IRQn */
    NVIC_InitStructure.NVIC_IRQ = TAMPER_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
