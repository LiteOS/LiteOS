/* *
 * *****************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
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

/* Includes ------------------------------------------------------------------ */
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "los_hwi.h"

#define TIMER7_RELOAD 50000
/* USER CODE END 0 */

TIM_HandleTypeDef htim7;

/* TIM7 init function */
void MX_TIM7_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };

    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 8000 - 1;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 50000 - 1;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM7) {
        /* USER CODE BEGIN TIM7_MspInit 0 */

        /* USER CODE END TIM7_MspInit 0 */
        /* TIM7 clock enable */
        __HAL_RCC_TIM7_CLK_ENABLE();

        /* TIM7 interrupt Init */
        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        /* USER CODE BEGIN TIM7_MspInit 1 */

        /* USER CODE END TIM7_MspInit 1 */
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM7) {
        /* USER CODE BEGIN TIM7_MspDeInit 0 */

        /* USER CODE END TIM7_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM7_CLK_DISABLE();

        /* TIM7 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
        /* USER CODE BEGIN TIM7_MspDeInit 1 */

        /* USER CODE END TIM7_MspDeInit 1 */
    }
}

/* *
 * @brief This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void)
{
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */
UINT64 Timer7Getcycle(VOID)
{
    static UINT64 bacCycle;
    static UINT64 cycleTimes;
    UINT64 swCycles = htim7.Instance->CNT;

    if (swCycles <= bacCycle) {
        cycleTimes++;
    }
    bacCycle = swCycles;
    return swCycles + cycleTimes * TIMER7_RELOAD;
}

VOID StmTimerInit(VOID)
{
    MX_TIM7_Init();
}

VOID StmTimerHwiCreate(VOID)
{
    UINT32 ret;

    ret = LOS_HwiCreate(TIM_IRQ, 0, 0, TIM7_IRQHandler, 0); // 16: cortex-m irq num shift
    if (ret != 0) {
        printf("ret of TIM7 LOS_HwiCreate = %#x\n", ret);
        return;
    }
    HAL_TIM_Base_Start_IT(&htim7);
}

UINT64 StmGetTimerCycles(Timer_t num)
{
    UINT64 cycles = 0;

    switch (num) {
        case 7:
            cycles = Timer7Getcycle();
            break;
        default:
            printf("Wrong number of TIMER.\n");
    }
    return cycles;
}
/* USER CODE END 1 */

/* *********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
