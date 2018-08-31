#ifndef __SPI_ADAPTER_LAYER_h__
#define __SPI_ADAPTER_LAYER_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "stm32f7xx.h"
#include "los_hwi.h"
#include "los_task.h"
#include "M8266WIFIDrv.h"
#include "cmsis_os2.h"
#include "los_typedef.h"
#include "memory.h"
#include "M8266_WIFI_SPI_IF.h"
#include "string.h"


void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
UINT32 LOS_HwiCreate( HWI_HANDLE_T uwHwiNum, HWI_PRIOR_T usHwiPrio, HWI_MODE_T usMode, HWI_PROC_FUNC pfnHandler, HWI_ARG_T uwArg );
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
UINT32 LOS_SemPost(UINT32 uwSemHandle);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
uint32_t HAL_RCC_GetPCLK2Freq(void);
UINT32 LOS_SemCreate(UINT16 usCount, UINT32 *puwSemHandle);
UINT32 LOS_QueueCreate(CHAR *pcQueueName, UINT16 usLen, UINT32 *puwQueueID, UINT32 uwFlags,UINT16 usMaxMsgSize);
UINT32 LOS_SemPend(UINT32 uwSemHandle, UINT32 uwTimeout);
UINT32 LOS_QueueWriteCopy(UINT32 uwQueueID, VOID *pBufferAddr, UINT32 uwBufferSize, UINT32 uwTimeOut);
UINT32 LOS_TaskCreate(UINT32 *puwTaskID, TSK_INIT_PARAM_S *pstInitParam);
UINT32 LOS_QueueReadCopy(UINT32 uwQueueID, VOID *pBufferAddr, UINT32 *puwBufferSize, UINT32 uwTimeOut);
UINT32 LOS_QueueDelete(UINT32 uwQueueID);
UINTPTR LOS_IntLock(VOID);
VOID LOS_IntRestore(UINTPTR uvIntSave);
void atiny_usleep(unsigned long usec);
void * atiny_malloc(size_t size);
void atiny_free(void* ptr);
void    m8266wifi_hardware_config(void);
uint8_t m8266wifi_softstart_config();
void    m8266wifi_gpio_exti_config(void);





#ifdef __cplusplus
}
#endif
#endif

