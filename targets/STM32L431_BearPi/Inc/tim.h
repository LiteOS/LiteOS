/* *
 * *****************************************************************************
 * File Name          : TIM.h
 * Description        : This file provides code for the configuration
 * of the TIM instances.
 * *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 * opensource.org/licenses/BSD-3-Clause
 *
 * *****************************************************************************
 */
/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN Includes */
#include "main.h"
#include "los_base.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef enum {
    TIMER1 = 1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
    TIMER7,
    TIMER8
} Timer_t;
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
UINT64 StmGetTimerCycles(Timer_t num);
VOID StmTimerHwiCreate(VOID);
VOID StmTimerInit(VOID);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /* __ tim_H */

/* *
 * @}
 */

/* *
 * @}
 */

/* *********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
