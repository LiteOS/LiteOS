#ifndef __DHT12_H
#define __DHT12_H

#include "los_config.h"
//#include "los_bsp_adapter.h"
#include "fm33A0xx_gpio.h"
#include "fm33A0xx_rcc.h"
#include "define_all.h"

#define TAIBI_BOARD_TYPE_B  


/* define bsp dht12 parameters, pay attention to TYPE_A and TYPE_B */

#ifdef TAIBI_BOARD_TYPE_A
/* dht12: DQ <-> PD8 */

#define BSP_DHT12_CLK_ENABLE() 	do {    \
                                        __HAL_RCC_GPIOD_CLK_ENABLE();  \
                                    } while(0)
                                    
#define GPIO_PORT_DHT12_DQ      GPIOD
#define GPIO_PIN_DHT12_DQ	    GPIO_PIN_8

#define DHT12_DQ_MODE_INPUT()  {GPIOD->CRH&=0XFFFFFFF0;GPIOD->CRH|=8;}
#define DHT12_DQ_MODE_OUTPUT() {GPIOD->CRH&=0XFFFFFFF0;GPIOD->CRH|=3;}

#endif  /* TAIBI_BOARD_TYPE_A */

#ifdef TAIBI_BOARD_TYPE_B
/* dht12: DQ <-> PA4 */

#define BSP_DHT12_CLK_ENABLE() 	do {    \
                                        __HAL_RCC_GPIOA_CLK_ENABLE();  \
                                    } while(0)
                                    
#define GPIO_PORT_DHT12_DQ      GPIOA
#define GPIO_PIN_DHT12_DQ	    GPIO_Pin_10

#define DHT12_DQ_MODE_INPUT()  {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=8<<16;}
#define DHT12_DQ_MODE_OUTPUT() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=3<<16;}

#endif /* TAIBI_BOARD_TYPE_B */


#define DHT12_DQ_WRITE(n)   ( n ? GPIO_SetBits(GPIO_PORT_DHT12_DQ, GPIO_PIN_DHT12_DQ) \
                                : GPIO_ResetBits(GPIO_PORT_DHT12_DQ, GPIO_PIN_DHT12_DQ) )
#define DHT12_DQ_READ()  GPIO_ReadInputDataBit(GPIO_PORT_DHT12_DQ, GPIO_PIN_DHT12_DQ)


UINT8 DHT12_Init(void);    /*init DHT12 sensor*/
UINT8 DHT12_Read_Data(INT8 *temp, UINT8 *humi);  /*read temp and humidity */
UINT8 DHT12_Read_Byte(void);  /*read one byte */
UINT8 DHT12_Read_Bit(void); /*read one bit */
UINT8 DHT12_Check(void); /*check DHT12*/
void DHT12_Rst(void); /*reset DHT12 sensor*/  
#endif
