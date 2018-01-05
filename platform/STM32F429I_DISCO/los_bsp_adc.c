#include "los_bsp_adc.h"
#include <stdarg.h>

#ifdef LOS_STM32F429ZI

ADC_HandleTypeDef hnucleo_Adc;
/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef sConfig;

extern void Error_Handler(void);

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  ADCx_CHANNEL_GPIO_CLK_ENABLE();
 
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* ADC3 Channel8 GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
	
	/* ADC3 Periph clock enable */
  ADCx_CLK_ENABLE();
  
  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for ADC interrupt */
  HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADCx_IRQn);
}

/**
  * @brief  DeInitializes ADC.
  * @param  None
  * @note ADC DeInit does not disable the GPIO clock
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{ 
  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC3 Channel8 GPIO pin */
  HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}

/**
  * @brief  Initializes ADC HAL.
  * @param  None
  * @retval None
  */
void ADC_DeInit(void)
{
    hnucleo_Adc.Instance   = ADCx;
    
    HAL_ADC_DeInit(&hnucleo_Adc);
    HAL_ADC_MspDeInit(&hnucleo_Adc);
}

/**
  * @brief  Initializes ADC HAL.
  */
void ADC_Init(void)
{
  if(HAL_ADC_GetState(&hnucleo_Adc) == HAL_ADC_STATE_RESET)
  {
    /* ADC Config */
    hnucleo_Adc.Instance                   = ADCx;
    hnucleo_Adc.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4; /* (must not exceed 36MHz) */
    hnucleo_Adc.Init.Resolution            = ADC_RESOLUTION12b;
    hnucleo_Adc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hnucleo_Adc.Init.ContinuousConvMode    = DISABLE;
    hnucleo_Adc.Init.DiscontinuousConvMode = DISABLE;
    hnucleo_Adc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hnucleo_Adc.Init.EOCSelection          = EOC_SINGLE_CONV;
    hnucleo_Adc.Init.NbrOfConversion       = 1;
    hnucleo_Adc.Init.DMAContinuousRequests = DISABLE;    
    
    HAL_ADC_MspInit(&hnucleo_Adc);
    HAL_ADC_Init(&hnucleo_Adc);
  }
}

/**
  * @brief  Configures joystick available on adafruit 1.8" TFT shield 
  *         managed through ADC to detect motion.
  * @param  None
  * @retval Joystickstatus (0=> success, 1=> fail) 
  */
void ADC_Ch_EscalatorConfig(void)
{  
   //ADC_Init();
	
	/*##-0- DeInit the ADC peripheral ##########################################*/

   if (HAL_ADC_DeInit(&hnucleo_Adc) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }
	  /*##-1- Initialize the ADC peripheral ######################################*/
  if(HAL_ADC_Init(&hnucleo_Adc) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
   
  /* Select the ADC Channel to be converted */
  sConfig.Channel      = ADCx_CHANNEL;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  sConfig.Rank         = 1;
	
	if(HAL_ADC_ConfigChannel(&hnucleo_Adc, &sConfig) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
}

/**
  * @brief  Returns the ADC Channel conversion.
  * @note   To know which Joystick key is pressed we need to detect the voltage
  *         level on each key output
  *           - None  : 3.3 V / 4095
  *           - SEL   : 1.055 V / 1308
  *           - DOWN  : 0.71 V / 88
  *           - LEFT  : 3.0 V / 3720 
  *           - RIGHT : 0.595 V / 737
  *           - UP    : 1.65 V / 2046
  * @retval JOYState_TypeDef: Code of the Joystick key pressed.
  */

uint16_t ADC_Ch_GetValue(void)
{	
	JOYState_TypeDef state;
  uint16_t  keyconvertedvalue = 0;
	/*##-0- DeInit the ADC peripheral ##########################################*/

   if (HAL_ADC_DeInit(&hnucleo_Adc) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }
  /*##-1- Initialize the ADC peripheral ######################################*/
  if(HAL_ADC_Init(&hnucleo_Adc) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
  /* Select the ADC Channel to be converted */
  sConfig.Channel      = ADCx_CHANNEL;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  sConfig.Rank         = 1;

	if(HAL_ADC_ConfigChannel(&hnucleo_Adc, &sConfig) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
  /*##-4- Enable DAC Channel1 ################################################*/
  if(HAL_ADC_Start(&hnucleo_Adc) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
	
	/* Wait for the end of conversion */
  HAL_ADC_PollForConversion(&hnucleo_Adc, 10);
	
	  /* Check if the continuous conversion of regular channel is finished */
  if((HAL_ADC_GetState(&hnucleo_Adc) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)

  {
    /* Get the converted value of regular channel */
    keyconvertedvalue = HAL_ADC_GetValue(&hnucleo_Adc);
  }
  
  if((keyconvertedvalue > 2010) && (keyconvertedvalue < 2090))
  {
    state = JOY_UP;
  }
  else if((keyconvertedvalue > 680) && (keyconvertedvalue < 780))
  {
    state = JOY_RIGHT;
  }
  else if((keyconvertedvalue > 1270) && (keyconvertedvalue < 1350))
  {
    state = JOY_SEL;
  }
  else if((keyconvertedvalue > 50) && (keyconvertedvalue < 130))
  {
    state = JOY_DOWN;
  }
  else if((keyconvertedvalue > 3680) && (keyconvertedvalue < 3760))
  {
    state = JOY_LEFT;
  }
  else
  {
    state = JOY_NONE;
  }
  
  /* Loop while a key is pressed */
  if(state != JOY_NONE)
  { 
    keyconvertedvalue = HAL_ADC_GetValue(&hnucleo_Adc);  
  }
  /* Return the code of the Joystick key pressed */
  return keyconvertedvalue;

}

#endif
