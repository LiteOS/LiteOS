#ifndef _LOS_BSP_DAC_H
#define _LOS_BSP_DAC_H

#ifdef LOS_STM32F429ZI

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor DAC Channel used and associated 
   resources */
/* Definition for DAC clock resources */
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()    
#define DACx_CHANNEL1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
     
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* Definition for DACx Channel1 Pin */
#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA 

/* Definition for DACx's Channel1 */
#define DACx_CHANNEL1                    DAC_CHANNEL_1

 /* Definition for ADCx's NVIC */
#define DACx_IRQn                        TIM6_DAC_IRQn  


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



#endif



#endif
