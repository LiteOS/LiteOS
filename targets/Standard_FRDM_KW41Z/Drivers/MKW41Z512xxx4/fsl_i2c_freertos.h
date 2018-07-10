/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __FSL_I2C_FREERTOS_H__
#define __FSL_I2C_FREERTOS_H__

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "portable.h"
#include "semphr.h"

#include "fsl_i2c.h"

/*!
 * @addtogroup i2c_freertos_driver I2C FreeRTOS driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @cond RTOS_PRIVATE
 * @brief I2C FreeRTOS handle
 */
typedef struct _i2c_rtos_handle
{
    I2C_Type *base;                 /*!< I2C base address */
    i2c_master_handle_t drv_handle; /*!< A handle of the underlying driver, treated as opaque by the RTOS layer */
    status_t async_status;          /*!< Transactional state of the underlying driver */
    SemaphoreHandle_t mutex;        /*!< A mutex to lock the handle during a transfer */
    SemaphoreHandle_t semaphore;    /*!< A semaphore to notify and unblock task when the transfer ends */
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticSemaphore_t mutexBuffer;     /*!< Statically allocated memory for mutex */
    StaticSemaphore_t semaphoreBuffer; /*!< Statically allocated memory for semaphore */
#endif
} i2c_rtos_handle_t;
/*! \endcond */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name I2C RTOS Operation
 * @{
 */

/*!
 * @brief Initializes I2C.
 *
 * This function initializes the I2C module and the related RTOS context.
 *
 * @param handle The RTOS I2C handle, the pointer to an allocated space for RTOS context.
 * @param base The pointer base address of the I2C instance to initialize.
 * @param masterConfig The configuration structure to set-up I2C in master mode.
 * @param srcClock_Hz The frequency of an input clock of the I2C module.
 * @return status of the operation.
 */
status_t I2C_RTOS_Init(i2c_rtos_handle_t *handle,
                       I2C_Type *base,
                       const i2c_master_config_t *masterConfig,
                       uint32_t srcClock_Hz);

/*!
 * @brief Deinitializes the I2C.
 *
 * This function deinitializes the I2C module and the related RTOS context.
 *
 * @param handle The RTOS I2C handle.
 */
status_t I2C_RTOS_Deinit(i2c_rtos_handle_t *handle);

/*!
 * @brief Performs the I2C transfer.
 *
 * This function performs the I2C transfer according to the data given in the transfer structure.
 *
 * @param handle The RTOS I2C handle.
 * @param transfer A structure specifying the transfer parameters.
 * @return status of the operation.
 */
status_t I2C_RTOS_Transfer(i2c_rtos_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @}
 */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* __FSL_I2C_FREERTOS_H__ */
