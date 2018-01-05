#ifndef _LOS_BSP_ADC_H
#define _LOS_BSP_ADC_H

#ifdef LOS_STM32F429ZI

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

typedef enum 
{ 
  JOY_NONE  = 0,
  JOY_SEL   = 1,
  JOY_DOWN  = 2,
  JOY_LEFT  = 3,
  JOY_RIGHT = 4,
  JOY_UP    = 5
}JOYState_TypeDef;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */
#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()    
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
     
#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_10
#define ADCx_CHANNEL_GPIO_PORT          GPIOF 

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_8

 /* Definition for ADCx's NVIC */
#define ADCx_IRQn                       ADC_IRQn  

/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */



#endif



#endif
