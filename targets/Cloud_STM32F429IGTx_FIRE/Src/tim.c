/* *
 * *****************************************************************************
 * File Name          : TIM.c
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

/* Includes ------------------------------------------------------------------ */
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "los_hwi.h"

#define TIMER3_RELOAD 50000
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
VOID MX_TIM3_Init(VOID)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig = { 0 };

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 9000 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 50000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
}

VOID HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM3) {
        /* USER CODE BEGIN TIM3_MspInit 0 */

        /* USER CODE END TIM3_MspInit 0 */
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();

        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspInit 1 */

        /* USER CODE END TIM3_MspInit 1 */
    }
}

VOID HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM3) {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */

        /* USER CODE END TIM3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /* TIM3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
        /* USER CODE BEGIN TIM3_MspDeInit 1 */

        /* USER CODE END TIM3_MspDeInit 1 */
    }
}

/* *
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

UINT64 Timer3Getcycle(VOID)
{
    static UINT64 bacCycle;
    static UINT64 cycleTimes;
    UINT64 swCycles = htim3.Instance->CNT;

    if (swCycles <= bacCycle) {
        cycleTimes++;
    }
    bacCycle = swCycles;
    return swCycles + cycleTimes * TIMER3_RELOAD;
}

VOID StmTimerInit(VOID)
{
    MX_TIM3_Init();
}

VOID StmTimerHwiCreate(VOID)
{
    UINT32 ret;

    ret = LOS_HwiCreate(TIM_IRQ, 0, 0, TIM3_IRQHandler, 0); // 16: cortex-m irq num shift
    if (ret != 0) {
        printf("ret of TIM3 LOS_HwiCreate = %#x\n", ret);
        return;
    }
    HAL_TIM_Base_Start_IT(&htim3);
}

UINT64 StmGetTimerCycles(Timer_t num)
{
    UINT64 cycles = 0;

    switch (num) {
        case 3:
            cycles = Timer3Getcycle();
            break;
        default:
            printf("Wrong number of TIMER.\n");
    }
    return cycles;
}
/* USER CODE END 1 */

/* *********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
