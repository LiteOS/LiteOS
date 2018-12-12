#ifndef __dal_gpio_H
#define __dal_gpio_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

typedef enum
{
  DAL_GPIO_A =0,
	DAL_GPIO_B , 
	DAL_GPIO_C , 
	DAL_GPIO_D ,
  DAL_GPIO_E ,
	DAL_GPIO_F ,
	DAL_GPIO_G ,
	DAL_GPIO_H ,
	
}dal_gpio_port_type;

typedef enum
{
  DAL_PIN_0 =0,
  DAL_PIN_1 ,
	DAL_PIN_2 ,
	DAL_PIN_3 ,
	DAL_PIN_4 ,
	DAL_PIN_5 ,
	DAL_PIN_6 ,
	DAL_PIN_7 ,
	DAL_PIN_8 ,
	DAL_PIN_9 ,
	DAL_PIN_10 ,
	DAL_PIN_11 ,
	DAL_PIN_12 ,
	DAL_PIN_13 ,
	DAL_PIN_14 ,
	DAL_PIN_15 ,
	
	
}dal_gpio_pin_type;




typedef enum
{
  DAL_GPIO_INPUT = 0,                      
  DAL_GPIO_OUTPUT_PP,  
  DAL_GPIO_OUTPUT_OD ,  
  DAL_GPIO_IT_RISING ,                  
  DAL_GPIO_IT_FALLING ,                 
  DAL_GPIO_IT_RISING_FALLING ,         
     

}dal_gpio_mode_type;


typedef enum
{
	DAL_GPIO_NOPULL = 0,
  DAL_GPIO_PULLUP,
	DAL_GPIO_PULLDOWN,
	

}dal_gpio_pull_type;

typedef struct gpio_init_s
{
  dal_gpio_port_type gpio_port_num;
	dal_gpio_pin_type gpio_pin_num;
	dal_gpio_mode_type gpio_mode;
	dal_gpio_pull_type gpio_pull;
  uint8_t dal_SubPriority;//if chooses the interrupt mode, set this value.  
  uint8_t dal_PreemptionPriority;//if chooses the interrupt mode, set this value.  
	
}dal_gpio_init_t;



extern uint8_t dal_gpio_init(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_deinit(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_set_output_pin(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_clear_output_pin(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_toggle_output_pin(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_read_input_pin(dal_gpio_init_t *gpio);
extern uint8_t dal_gpio_read_output_pin(dal_gpio_init_t *gpio);



#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

