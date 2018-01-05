#include "los_bsp_dac.h"
#include <stdarg.h>

#ifdef LOS_STM32F429ZI

extern void Error_Handler(void);

DAC_HandleTypeDef    DacHandle;
DAC_ChannelConfTypeDef sConfigdac;


void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
	
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  DACx_CHANNEL1_GPIO_CLK_ENABLE();
	
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* DAC Channel1 GPIO pin configuration */
  GPIO_InitStruct.Pin = DACx_CHANNEL1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
	
	/* DAC Periph clock enable */
	DACx_CLK_ENABLE();

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* Enable the DMA1 Stream5 IRQ Channel */
	HAL_NVIC_SetPriority(DACx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DACx_IRQn);
	
}

/**
  * @brief  DeInitializes the DAC.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.  
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  DACx_FORCE_RESET();
  DACx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel1 GPIO pin */
  HAL_GPIO_DeInit(DACx_CHANNEL1_GPIO_PORT, DACx_CHANNEL1_PIN);
 
}

/**
  * @brief  Initializes ADC HAL.
  * @param  None
  * @retval None
  */
void DAC_DeInit(void)
{
    DacHandle.Instance   = DAC;
    
    HAL_DAC_DeInit(&DacHandle);
    HAL_DAC_MspDeInit(&DacHandle);
}

void DAC_Init(void)
{
  if(HAL_DAC_GetState(&DacHandle) == HAL_DAC_STATE_RESET)
  {
    /* ADC Config */
    DacHandle.Instance                     =DAC;
    
    HAL_DAC_MspInit(&DacHandle);
    HAL_DAC_Init(&DacHandle);
  }
}

void DAC_Ch1_EscalatorConfig(void)
{
	/*##-0- DeInit the DAC peripheral ##########################################*/

   if (HAL_DAC_DeInit(&DacHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }
	  /*##-1- Initialize the DAC peripheral ######################################*/
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-1- DAC channel1 Configuration #########################################*/
  sConfigdac.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfigdac.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfigdac, DACx_CHANNEL1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
}

/**
  * @brief  DAC Channel1 Triangle Configuration
  * @param  None
  * @retval None
  */
uint16_t DAC_Ch1_GetValue(void)
{	
	uint16_t  keyconvertedvalue = 0;
	
  /*##-1- Initialize the DAC peripheral ######################################*/
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- DAC channel2 Configuration #########################################*/
  sConfigdac.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfigdac.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfigdac, DACx_CHANNEL1) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
  /*##-3- DAC channel2 Triangle Wave generation configuration ################*/
  if(HAL_DACEx_TriangleWaveGenerate(&DacHandle, DACx_CHANNEL1, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
  {
    /* Triangle wave generation Error */
    Error_Handler();
  }
  
  /*##-4- Enable DAC Channel1 ################################################*/
  if(HAL_DAC_Start(&DacHandle, DACx_CHANNEL1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
	
  /*##-5- Set DAC channel1 DHR12RD register ##################################*/
  if(HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL1, DAC_ALIGN_12B_R, 0x100) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
	
	/*##-6-Check if the continuous conversion of regular channel is finished */
  if((HAL_DAC_GetState(&DacHandle) & HAL_DAC_STATE_READY) == HAL_DAC_STATE_READY)
  {
    /* Get the converted value of regular channel */
    keyconvertedvalue = HAL_DAC_GetValue(&DacHandle,DAC_CHANNEL_1);
  }
	  return keyconvertedvalue;
}


#endif
