#include "dal_gpio.h"
#include "stdio.h"
GPIO_InitTypeDef GPIO_InitStruct;


/**
  * @brief  initialize the GPIO peripheral according to the specified parameters in the gpio.
  * @param  gpio_init_t *gpio : initialize paremeters list.
	* @retval 0:input parameters error 
	* @retval	1:success
					
*/
uint8_t dal_gpio_init(dal_gpio_init_t *gpio)
{
	uint32_t mode;
	
	switch(gpio->gpio_mode)
	{
		case DAL_GPIO_INPUT:
			mode = GPIO_MODE_INPUT;
			break;
		case DAL_GPIO_OUTPUT_PP:
			mode = GPIO_MODE_OUTPUT_PP;
			break;
		case DAL_GPIO_OUTPUT_OD:
			mode = GPIO_MODE_OUTPUT_OD;
			break;
		case DAL_GPIO_IT_FALLING:
			mode = GPIO_MODE_IT_FALLING;
			break;
		case DAL_GPIO_IT_RISING:
			mode = GPIO_MODE_IT_RISING;
			break;
		case DAL_GPIO_IT_RISING_FALLING:
			mode = GPIO_MODE_IT_RISING_FALLING;
			break;
		default:
			return 0;
	
	}
	switch (gpio->gpio_port_num)
	{
		
		case DAL_GPIO_A:
			__HAL_RCC_GPIOA_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		    break;
		case DAL_GPIO_B:
			__HAL_RCC_GPIOB_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			break;
	  case DAL_GPIO_C:
			__HAL_RCC_GPIOC_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			break;
		case DAL_GPIO_D:
			__HAL_RCC_GPIOD_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			break;
		case DAL_GPIO_E:
			__HAL_RCC_GPIOE_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
			break;
		case DAL_GPIO_F:
			__HAL_RCC_GPIOF_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
			break;
		case DAL_GPIO_G:
			__HAL_RCC_GPIOG_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
			break;
		case DAL_GPIO_H:
			__HAL_RCC_GPIOH_CLK_ENABLE();
	
			GPIO_InitStruct.Pin = 1<<(gpio->gpio_pin_num);
			GPIO_InitStruct.Mode = mode;
			GPIO_InitStruct.Pull = gpio->gpio_pull;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
			HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
			break;
		default:
			return 0;
			  
	}
	if(gpio->gpio_mode >= 3)
		{
			switch (gpio->gpio_pin_num)
			{
				case 0:
					HAL_NVIC_SetPriority(EXTI0_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI0_IRQn);
					break;
			  case 1:
					HAL_NVIC_SetPriority(EXTI1_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI1_IRQn);
					break;
				case 2:
					HAL_NVIC_SetPriority(EXTI2_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI2_IRQn);
					break;
				case 3:
					HAL_NVIC_SetPriority(EXTI3_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI3_IRQn);
					break;
				case 4:
					HAL_NVIC_SetPriority(EXTI4_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI4_IRQn);
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					HAL_NVIC_SetPriority(EXTI9_5_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
					break;
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
					HAL_NVIC_SetPriority(EXTI15_10_IRQn, gpio->dal_PreemptionPriority, gpio->dal_SubPriority);
					HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
					break;
				default:
					return 0;
			}
		}
		return 1;
  
}

/**
  * @brief  Toggle the specified GPIO pin.
  * @param  GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4 family
  * @param  GPIO_Pin specifies the pin to be toggled.
  * @retval 0:input parameters error 
	* @retval	1:success
 */
uint8_t dal_gpio_deinit(dal_gpio_init_t *gpio)
{
	
	switch (gpio->gpio_port_num)
	{
		case DAL_GPIO_A:
				HAL_GPIO_DeInit(GPIOA, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_B:
				HAL_GPIO_DeInit(GPIOB, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_C:
				HAL_GPIO_DeInit(GPIOC, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_D:
				HAL_GPIO_DeInit(GPIOD, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_E:
				HAL_GPIO_DeInit(GPIOE, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_F:
				HAL_GPIO_DeInit(GPIOF, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_G:
				HAL_GPIO_DeInit(GPIOG, (1<<(gpio->gpio_pin_num)));
				break;
		case DAL_GPIO_H:
				HAL_GPIO_DeInit(GPIOH, (1<<(gpio->gpio_pin_num)));
				break;
		default:
				return 0;
	}
	return 1;
	 
}

/**
  * @brief  set an output pin to be high level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:input parameters error 
	* @retval	1:success
  */
uint8_t dal_gpio_set_output_pin(dal_gpio_init_t *gpio)
{
	switch (gpio->gpio_port_num)
	{
		case DAL_GPIO_A:
				HAL_GPIO_WritePin(GPIOA, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_B:
				HAL_GPIO_WritePin(GPIOB, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_C:
				HAL_GPIO_WritePin(GPIOC, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_D:
				HAL_GPIO_WritePin(GPIOD, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_E:
				HAL_GPIO_WritePin(GPIOE, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_F:
				HAL_GPIO_WritePin(GPIOF, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_G:
				HAL_GPIO_WritePin(GPIOG, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		case DAL_GPIO_H:
				HAL_GPIO_WritePin(GPIOH, (1<<(gpio->gpio_pin_num)), GPIO_PIN_SET);
				break;
		default:
				return 0;
	 }
	return 1;
 
}




/**
  * @brief  set an output pin to be low level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:input parameters error 
	* @retval	1:success
  */
uint8_t dal_gpio_clear_output_pin(dal_gpio_init_t *gpio)
{
	switch (gpio->gpio_port_num)
	{
		case DAL_GPIO_A:
				HAL_GPIO_WritePin(GPIOA, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_B:
				HAL_GPIO_WritePin(GPIOB, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_C:
				HAL_GPIO_WritePin(GPIOC, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_D:
				HAL_GPIO_WritePin(GPIOD, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_E:
				HAL_GPIO_WritePin(GPIOE, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_F:
				HAL_GPIO_WritePin(GPIOF, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_G:
				HAL_GPIO_WritePin(GPIOG, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		case DAL_GPIO_H:
				HAL_GPIO_WritePin(GPIOH, (1<<(gpio->gpio_pin_num)), GPIO_PIN_RESET);
				break;
		default:
				return 0;
	}  
	return 1;
}

/**
  * @brief  toggle pin level.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:input parameters error 
	* @retval	1:success
  */
uint8_t dal_gpio_toggle_output_pin(dal_gpio_init_t *gpio)
{
	switch (gpio->gpio_port_num)
		{
			case DAL_GPIO_A:
					HAL_GPIO_TogglePin(GPIOA, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_B:
					HAL_GPIO_TogglePin(GPIOB, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_C:
					HAL_GPIO_TogglePin(GPIOC, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_D:
				  HAL_GPIO_TogglePin(GPIOD, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_E:
					HAL_GPIO_TogglePin(GPIOE, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_F:
					HAL_GPIO_TogglePin(GPIOF, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_G:
					HAL_GPIO_TogglePin(GPIOG, (1<<(gpio->gpio_pin_num)));
					break;
			case DAL_GPIO_H:
					HAL_GPIO_TogglePin(GPIOH, (1<<(gpio->gpio_pin_num)));
					break;
			default:
					return 0;
	  }  
		return 1;
}

/**
  * @brief  read the level of the input pin.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:low level
  * @retval 1:high level
	* @retval 2:input parameters error
  */
uint8_t dal_gpio_read_input_pin(dal_gpio_init_t *gpio)
{
	uint8_t ret;
	switch (gpio->gpio_port_num)
			{
				case 0:
					  ret =	HAL_GPIO_ReadPin(GPIOA, (1<<(gpio->gpio_pin_num)));
					  return ret;	
				case 1:
						ret =	HAL_GPIO_ReadPin(GPIOB, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 2:
						ret =	HAL_GPIO_ReadPin(GPIOC, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 3:
						ret =	HAL_GPIO_ReadPin(GPIOD, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 4:
						ret =	HAL_GPIO_ReadPin(GPIOE, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 5:
						ret =	HAL_GPIO_ReadPin(GPIOF, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 6:
						ret =	HAL_GPIO_ReadPin(GPIOG, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 7:
						ret =	HAL_GPIO_ReadPin(GPIOH, (1<<(gpio->gpio_pin_num)));
						return ret;	
				default:
						return 2;
			}  
 
  
}

/**
  * @brief  Read the specified input port pin.
  * @param  GPIOx: where x can be (A..H) to select the GPIO peripheral for STM32L4 family
  * @param  GPIO_Pin: specifies the port bit to read.
  *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadOutputPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIO_PinState bitstatus;

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->ODR & GPIO_Pin) != 0x00u)
  {
    bitstatus = GPIO_PIN_SET;
  }
  else
  {
    bitstatus = GPIO_PIN_RESET;
  }
  return bitstatus;
}
/**
  * @brief  read the level of the output pin.
  * @param  gpio_init_t *gpio : initialize paremeters list.
  * @retval 0:low level
  * @retval 1:high level
	* @retval 2:input parameters error
  */
uint8_t dal_gpio_read_output_pin(dal_gpio_init_t *gpio)
{
	uint8_t ret;
	switch (gpio->gpio_port_num)
			{
				case 0:
					  ret =	HAL_GPIO_ReadOutputPin(GPIOA, (1<<(gpio->gpio_pin_num)));
					  return ret;	
				case 1:
						ret =	HAL_GPIO_ReadOutputPin(GPIOB, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 2:
						ret =	HAL_GPIO_ReadOutputPin(GPIOC, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 3:
						ret =	HAL_GPIO_ReadOutputPin(GPIOD, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 4:
						ret =	HAL_GPIO_ReadOutputPin(GPIOE, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 5:
						ret =	HAL_GPIO_ReadOutputPin(GPIOF, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 6:
						ret =	HAL_GPIO_ReadOutputPin(GPIOG, (1<<(gpio->gpio_pin_num)));
						return ret;	
				case 7:
						ret =	HAL_GPIO_ReadOutputPin(GPIOH, (1<<(gpio->gpio_pin_num)));
						return ret;	
				default:
						return 2;
			}  
 
  
}





extern dal_gpio_init_t wakeup;


void dal_exti0_callback(void)
{
	uint8_t ret = 1;
	ret = dal_gpio_read_input_pin(&wakeup);
  printf("wakeup level is %d \r\n",ret);
  printf("enter PIN0 IT.\r\n");
 
}


void dal_exti1_callback(void)
{
   
 
}
void dal_exti2_callback(void)
{
   
 
}
void dal_exti3_callback(void)
{
   
 
}
void dal_exti4_callback(void)
{
   
 
}
void dal_exti5_callback(void)
{
   
 
}


void dal_exti6_callback(void)
{
   
 
}
void dal_exti7_callback(void)
{
   
 
}
void dal_exti8_callback(void)
{
   
 
}
void dal_exti9_callback(void)
{
   
 
}
void dal_exti10_callback(void)
{
   
 
}


void dal_exti11_callback(void)
{
   
 
}
void dal_exti12_callback(void)
{
   
 
}
void dal_exti13_callback(void)
{
   
 
}
void dal_exti14_callback(void)
{
   
 
}
void dal_exti15_callback(void)
{
   
 
}


void EXTI0_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    dal_exti0_callback();
  }
  
}


void EXTI1_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
			dal_exti1_callback();
		}
}


void EXTI2_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
			dal_exti2_callback();
		}
}
void EXTI3_IRQHandler(void)
{

	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
			dal_exti3_callback();
		}
		
}
void EXTI4_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
			dal_exti4_callback();
		}
}
void EXTI9_5_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
			dal_exti5_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
			dal_exti6_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
			dal_exti7_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
			dal_exti8_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
			dal_exti9_callback();
		}
}
void EXTI15_10_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
			dal_exti10_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
			dal_exti11_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
			dal_exti12_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
			dal_exti13_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
			dal_exti14_callback();
		}
		else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
			dal_exti15_callback();
		}
}